/**
 ********************************************************************
 * @file    test_payload_cam_emu_common.c
 * @brief
 *
 * @copyright (c) 2021 ZIYAN. All rights reserved.
 *
 * All information contained herein is, and remains, the property of ZIYAN.
 * The intellectual and technical concepts contained herein are proprietary
 * to ZIYAN and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of ZIYAN.
 *
 * If you receive this source code without ZIYAN’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify ZIYAN of its removal. ZIYAN reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "math.h"
#include "test_payload_cam_emu_base.h"
#include "utils/util_misc.h"
#include "ziyan_logger.h"
#include "ziyan_platform.h"
#include "ziyan_payload_camera.h"
#include "ziyan_aircraft_info.h"
#include "ziyan_gimbal.h"
// #include "ziyan_xport.h"
#include "gimbal_emu/test_payload_gimbal_emu.h"

/* Private constants ---------------------------------------------------------*/
#define PAYLOAD_CAMERA_EMU_TASK_FREQ            (100)
#define PAYLOAD_CAMERA_EMU_TASK_STACK_SIZE      (2048)
#define SDCARD_TOTAL_SPACE_IN_MB                (32 * 1024)
#define SDCARD_PER_PHOTO_SPACE_IN_MB            (4)
#define SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB   (2)
#define ZOOM_OPTICAL_FOCAL_MAX_LENGTH           (300)
#define ZOOM_OPTICAL_FOCAL_MIN_LENGTH           (10)
#define ZOOM_OPTICAL_FOCAL_LENGTH_STEP          (10)
#define ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP     (5)
#define ZOOM_DIGITAL_BASE_FACTOR                (1.0)
#define ZOOM_DIGITAL_STEP_FACTOR                (0.1)
#define ZOOM_DIGITAL_MAX_FACTOR                 (6.0)
#define FOCUS_MAX_RINGVALUE                     (1000)
#define FOCUS_MID_RINGVALUE                     (500)
#define IMAGE_SENSOR_X_SIZE                     (88.0f) // unit: 0.1mm
#define IMAGE_SENSOR_Y_SIZE                     (66.0f) // unit: 0.1mm
#define CENTER_POINT_IN_SCREEN_X_VALUE          (0.5f)
#define CENTER_POINT_IN_SCREEN_Y_VALUE          (0.5f)
#define TAP_ZOOM_DURATION                       (2000) // unit: ms
#define INTERVAL_PHOTOGRAPH_ALWAYS_COUNT        (255)
#define INTERVAL_PHOTOGRAPH_INTERVAL_INIT_VALUE (1)    // unit: s
#define TAKING_PHOTO_SPENT_TIME_MS_EMU          (500)
#define PHOTO_FILE_PATH                         "../../../../../api_sample/camera_media_emu/media_file/ZIYAN_0001_ORG.jpg"

// Attention: you can select which camera function you want to run, default: all on.
#define USER_CAMERA_EMU_METERING_ON                1
#define USER_CAMERA_EMU_FOCUS_ON                   1
#define USER_CAMERA_EMU_HYBRID_ZOOM_ON             1
#define USER_CAMERA_EMU_TAP_ZOOM_ON                1

/* Private types -------------------------------------------------------------*/
typedef struct {
    E_ZiyanGimbalRotationMode rotationMode;
    T_ZiyanGimbalRotationProperty rotationProperty;
    T_ZiyanAttitude3d rotationValue;
} T_TestCameraGimbalRotationArgument;

/* Private variables ---------------------------------------------------------*/
static bool s_isCamInited = false;

static T_ZiyanCameraCommonHandler s_commonHandler;
static T_ZiyanCameraExposureMeteringHandler s_exposureMeteringHandler;
static T_ZiyanCameraFocusHandler s_focusHandler;
static T_ZiyanCameraDigitalZoomHandler s_digitalZoomHandler;
static T_ZiyanCameraOpticalZoomHandler s_opticalZoomHandler;
static T_ZiyanCameraTapZoomHandler s_tapZoomHandler;
static T_ZiyanCameraExtendHandler s_extendHander;

static T_ZiyanTaskHandle s_userCameraThread;

static T_ZiyanCameraSystemState s_cameraState = {0};
static E_ZiyanCameraShootPhotoMode s_cameraShootPhotoMode = ZIYAN_CAMERA_SHOOT_PHOTO_MODE_SINGLE;
static E_ZiyanCameraBurstCount s_cameraBurstCount = ZIYAN_CAMERA_BURST_COUNT_2;
static T_ZiyanCameraPhotoTimeIntervalSettings s_cameraPhotoTimeIntervalSettings = {INTERVAL_PHOTOGRAPH_ALWAYS_COUNT,
                                                                                 INTERVAL_PHOTOGRAPH_INTERVAL_INIT_VALUE};
static T_ZiyanCameraSDCardState s_cameraSDCardState = {0};
static T_ZiyanMutexHandle s_commonMutex = {0};

static E_ZiyanCameraMeteringMode s_cameraMeteringMode = ZIYAN_CAMERA_METERING_MODE_CENTER;
static T_ZiyanCameraSpotMeteringTarget s_cameraSpotMeteringTarget = {0};

static E_ZiyanCameraFocusMode s_cameraFocusMode = ZIYAN_CAMERA_FOCUS_MODE_AUTO;
static T_ZiyanCameraPointInScreen s_cameraFocusTarget = {0};
static uint32_t s_cameraFocusRingValue = FOCUS_MID_RINGVALUE;
static T_ZiyanCameraFocusAssistantSettings s_cameraFocusAssistantSettings = {0};

static E_ZiyanCameraZoomDirection s_cameraZoomDirection = ZIYAN_CAMERA_ZOOM_DIRECTION_OUT;
static E_ZiyanCameraZoomSpeed s_cameraZoomSpeed = ZIYAN_CAMERA_ZOOM_SPEED_NORMAL;
static uint32_t s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
static ziyan_f32_t s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
static bool s_isStartContinuousOpticalZoom = false;
static bool s_isOpticalZoomReachLimit = false;
static T_ZiyanMutexHandle s_zoomMutex = {0};

static bool s_isTapZoomEnabled = false;
static T_ZiyanCameraTapZoomState s_cameraTapZoomState = {0};
static uint8_t s_tapZoomMultiplier = 1;
static uint32_t s_tapZoomStartTime = 0;
static bool s_isStartTapZoom = false;
static bool s_isTapZooming = false;
static T_TestCameraGimbalRotationArgument s_tapZoomNewestGimbalRotationArgument = {0};
static uint32_t s_tapZoomNewestTargetHybridFocalLength = 0; // unit: 0.1mm
static T_ZiyanMutexHandle s_tapZoomMutex = NULL;
static E_ZiyanCameraVideoStreamType s_cameraVideoStreamType;

static bool range_finder_switch = true;
static ziyan_f32_t range_finder_distance = 0;
static T_ZiyanLocation range_finder_target_location = {0};

/* Private functions declaration ---------------------------------------------*/
static T_ZiyanReturnCode GetSystemState(T_ZiyanCameraSystemState *systemState);
static T_ZiyanReturnCode SetMode(E_ZiyanCameraMode mode);
static T_ZiyanReturnCode StartRecordVideo(void);
static T_ZiyanReturnCode StopRecordVideo(void);
static T_ZiyanReturnCode StartShootPhoto(void);
static T_ZiyanReturnCode StopShootPhoto(void);
static T_ZiyanReturnCode SetShootPhotoMode(E_ZiyanCameraShootPhotoMode mode);
static T_ZiyanReturnCode GetShootPhotoMode(E_ZiyanCameraShootPhotoMode *mode);
static T_ZiyanReturnCode SetPhotoBurstCount(E_ZiyanCameraBurstCount burstCount);
static T_ZiyanReturnCode GetPhotoBurstCount(E_ZiyanCameraBurstCount *burstCount);
static T_ZiyanReturnCode SetPhotoTimeIntervalSettings(T_ZiyanCameraPhotoTimeIntervalSettings settings);
static T_ZiyanReturnCode GetPhotoTimeIntervalSettings(T_ZiyanCameraPhotoTimeIntervalSettings *settings);
static T_ZiyanReturnCode GetSDCardState(T_ZiyanCameraSDCardState *sdCardState);
static T_ZiyanReturnCode FormatSDCard(void);

static T_ZiyanReturnCode SetMeteringMode(E_ZiyanCameraMeteringMode mode);
static T_ZiyanReturnCode GetMeteringMode(E_ZiyanCameraMeteringMode *mode);
static T_ZiyanReturnCode SetSpotMeteringTarget(T_ZiyanCameraSpotMeteringTarget target);
static T_ZiyanReturnCode GetSpotMeteringTarget(T_ZiyanCameraSpotMeteringTarget *target);

static T_ZiyanReturnCode SetFocusMode(E_ZiyanCameraFocusMode mode);
static T_ZiyanReturnCode GetFocusMode(E_ZiyanCameraFocusMode *mode);
static T_ZiyanReturnCode SetFocusTarget(T_ZiyanCameraPointInScreen target);
static T_ZiyanReturnCode GetFocusTarget(T_ZiyanCameraPointInScreen *target);
static T_ZiyanReturnCode SetFocusAssistantSettings(T_ZiyanCameraFocusAssistantSettings settings);
static T_ZiyanReturnCode GetFocusAssistantSettings(T_ZiyanCameraFocusAssistantSettings *settings);
static T_ZiyanReturnCode SetFocusRingValue(uint32_t value);
static T_ZiyanReturnCode GetFocusRingValue(uint32_t *value);
static T_ZiyanReturnCode GetFocusRingValueUpperBound(uint32_t *value);

static T_ZiyanReturnCode SetDigitalZoomFactor(ziyan_f32_t factor);
static T_ZiyanReturnCode SetOpticalZoomFocalLength(uint32_t focalLength);
static T_ZiyanReturnCode GetOpticalZoomFocalLength(uint32_t *focalLength);
static T_ZiyanReturnCode GetOpticalZoomSpec(T_ZiyanCameraOpticalZoomSpec *spec);
static T_ZiyanReturnCode StartContinuousOpticalZoom(E_ZiyanCameraZoomDirection direction, E_ZiyanCameraZoomSpeed speed);
static T_ZiyanReturnCode StopContinuousOpticalZoom(void);
static T_ZiyanReturnCode GetTapZoomState(T_ZiyanCameraTapZoomState *state);
static T_ZiyanReturnCode SetTapZoomEnabled(bool enabledFlag);
static T_ZiyanReturnCode GetTapZoomEnabled(bool *enabledFlag);
static T_ZiyanReturnCode SetTapZoomMultiplier(uint8_t multiplier);
static T_ZiyanReturnCode GetTapZoomMultiplier(uint8_t *multiplier);
static T_ZiyanReturnCode TapZoomAtTarget(T_ZiyanCameraPointInScreen target);
static T_ZiyanReturnCode ZiyanTest_CameraHybridZoom(uint32_t focalLength);
static T_ZiyanReturnCode ZiyanTest_CameraRotationGimbal(T_TestCameraGimbalRotationArgument gimbalRotationArgument);

static T_ZiyanReturnCode SetRangeFinderSwitch(bool enable);
static T_ZiyanReturnCode GetRangeFinderSwitch(bool* enable);
static T_ZiyanReturnCode GetRangeFinderData(ziyan_f32_t* distance);
static T_ZiyanReturnCode GetRangeFinderLocation(T_ZiyanLocation* location);

static void *UserCamera_Task(void *arg);

/* Exported functions definition ---------------------------------------------*/

/* Private functions definition-----------------------------------------------*/


#ifdef FFMPEG_INSTALLED
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define STREAM_PORT 9600
#define STREAM_ADDR "127.0.0.1"
#define INPUT_FILE "/home/levi/Videos/test.h264"  // Replace with your actual video file path
#define INPUT_FILE2 "test.h264"

#endif

void* push_video_stream8(void* arg)
{
#if defined(FFMPEG_INSTALLED) && (LIBAVFORMAT_VERSION_MAJOR >= 57)
    while(1){

        // Initialize FFmpeg libraries
        av_register_all();
        avformat_network_init();

        AVFormatContext *input_format_context = NULL;
        AVCodecContext *codec_context = NULL;
        AVCodec *codec = NULL;
        AVPacket packet;

        char cwd[256] = {0};
        char file_path[256] = {0};
        getcwd(cwd, sizeof(cwd));
        sprintf(file_path, "%s/%s", cwd, INPUT_FILE2);
        

        char input_file[256] = {0};

        if (access(file_path, F_OK) == 0) {
            memcpy(input_file, file_path, strlen(file_path));
            printf("select: %s\n", file_path);
        } else {
            memcpy(input_file, INPUT_FILE, strlen(INPUT_FILE));
            printf("select: %s\n", INPUT_FILE);
        }
        
        printf("input file: %s\n", input_file);

        // Open the input MP4 file
        if (avformat_open_input(&input_format_context, input_file, NULL, NULL) < 0) {
            fprintf(stderr, "Error opening input file\n");
            return NULL;
        }

        // Retrieve stream information
        if (avformat_find_stream_info(input_format_context, NULL) < 0) {
            fprintf(stderr, "Error finding stream info\n");
            return NULL;
        }

        // Find the video stream
        int video_stream_index = -1;
        for (int i = 0; i < input_format_context->nb_streams; i++) {
            if (input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                video_stream_index = i;
                break;
            }
        }

        if (video_stream_index == -1) {
            fprintf(stderr, "Video stream not found\n");
            return NULL;
        }

        // Find the decoder for the video stream
        AVStream *video_stream = input_format_context->streams[video_stream_index];
        codec = avcodec_find_decoder(video_stream->codecpar->codec_id);
        if (!codec) {
            fprintf(stderr, "Codec not found\n");
            return NULL;
        }

        codec_context = avcodec_alloc_context3(codec);
        if (!codec_context) {
            fprintf(stderr, "Could not allocate codec context\n");
            return NULL;
        }

        if (avcodec_parameters_to_context(codec_context, video_stream->codecpar) < 0) {
            fprintf(stderr, "Failed to copy codec parameters to context\n");
            return NULL;
        }

        if (avcodec_open2(codec_context, codec, NULL) < 0) {
            fprintf(stderr, "Error opening codec\n");
            return NULL;
        }

        // Initialize packet structure
        av_init_packet(&packet);
        packet.data = NULL;
        packet.size = 0;

        // Start reading frames and send packets
        while (av_read_frame(input_format_context, &packet) >= 0) {
            if (packet.stream_index == video_stream_index) {
                // Send the H.264 encoded packet over UDP
                ZiyanPayloadCamera_SendVideoStream(packet.data, packet.size);
                // printf("Sent packet of size %d\n", packet.size);
                usleep(1000 * 33);
            }

            // Free the packet after sending
            av_packet_unref(&packet);
        }

        // Clean up and close
        avcodec_free_context(&codec_context);
        avformat_close_input(&input_format_context);
    }

#endif
    return 0;
}




static T_ZiyanReturnCode GetSystemState(T_ZiyanCameraSystemState *systemState)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *systemState = s_cameraState;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetMode(E_ZiyanCameraMode mode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraState.cameraMode = mode;
    USER_LOG_INFO("set camera mode:%d", mode);

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StartRecordVideo(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    ziyanStat = osalHandler->MutexLock(s_commonMutex);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", ziyanStat);
        return ziyanStat;
    }

    if (s_cameraState.isRecording != false) {
        USER_LOG_ERROR("camera is already in recording state");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
        goto out;
    }

    s_cameraState.isRecording = true;
    USER_LOG_INFO("start record video");

out:
    ziyanStat = osalHandler->MutexUnlock(s_commonMutex);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", ziyanStat);
        return ziyanStat;
    }

    return returnCode;
}

static T_ZiyanReturnCode StopRecordVideo(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    ziyanStat = osalHandler->MutexLock(s_commonMutex);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", ziyanStat);
        return ziyanStat;
    }

    if (s_cameraState.isRecording != true) {
        USER_LOG_ERROR("camera is not in recording state");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
        goto out;
    }

    s_cameraState.isRecording = false;
    s_cameraState.currentVideoRecordingTimeInSeconds = 0;
    USER_LOG_INFO("stop record video");

out:
    ziyanStat = osalHandler->MutexUnlock(s_commonMutex);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", ziyanStat);
        return ziyanStat;
    }

    return returnCode;
}

static T_ZiyanReturnCode StartShootPhoto(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("start shoot photo");
    s_cameraState.isStoring = true;

    if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
        s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_SINGLE_PHOTO;
    } else if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_BURST) {
        s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_BURST_PHOTO;
    } else if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_INTERVAL) {
        s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_INTERVAL_PHOTO;
        s_cameraState.isShootingIntervalStart = true;
        s_cameraState.currentPhotoShootingIntervalTimeInSeconds = s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds;
    }

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StopShootPhoto(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("stop shoot photo");
    s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE;
    s_cameraState.isStoring = false;
    s_cameraState.isShootingIntervalStart = false;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetShootPhotoMode(E_ZiyanCameraShootPhotoMode mode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraShootPhotoMode = mode;
    USER_LOG_INFO("set shoot photo mode:%d", mode);

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetShootPhotoMode(E_ZiyanCameraShootPhotoMode *mode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *mode = s_cameraShootPhotoMode;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);\
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetPhotoBurstCount(E_ZiyanCameraBurstCount burstCount)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraBurstCount = burstCount;
    USER_LOG_INFO("set photo burst count:%d", burstCount);

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetPhotoBurstCount(E_ZiyanCameraBurstCount *burstCount)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *burstCount = s_cameraBurstCount;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetPhotoTimeIntervalSettings(T_ZiyanCameraPhotoTimeIntervalSettings settings)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_cameraPhotoTimeIntervalSettings.captureCount = settings.captureCount;
    s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds = settings.timeIntervalSeconds;
    USER_LOG_INFO("set photo interval settings count:%d seconds:%d", settings.captureCount,
                  settings.timeIntervalSeconds);
    s_cameraState.currentPhotoShootingIntervalCount = settings.captureCount;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetPhotoTimeIntervalSettings(T_ZiyanCameraPhotoTimeIntervalSettings *settings)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(settings, &s_cameraPhotoTimeIntervalSettings, sizeof(T_ZiyanCameraPhotoTimeIntervalSettings));

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetSDCardState(T_ZiyanCameraSDCardState *sdCardState)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(sdCardState, &s_cameraSDCardState, sizeof(T_ZiyanCameraSDCardState));

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode FormatSDCard(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("format sdcard");

    memset(&s_cameraSDCardState, 0, sizeof(T_ZiyanCameraSDCardState));
    s_cameraSDCardState.isInserted = true;
    s_cameraSDCardState.isVerified = true;
    s_cameraSDCardState.totalSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.remainSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.availableCaptureCount = SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_PHOTO_SPACE_IN_MB;
    s_cameraSDCardState.availableRecordingTimeInSeconds =
        SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetMeteringMode(E_ZiyanCameraMeteringMode mode)
{
    USER_LOG_INFO("set metering mode:%d", mode);
    s_cameraMeteringMode = mode;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMeteringMode(E_ZiyanCameraMeteringMode *mode)
{
    *mode = s_cameraMeteringMode;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetSpotMeteringTarget(T_ZiyanCameraSpotMeteringTarget target)
{
    USER_LOG_INFO("set spot metering area col:%d row:%d", target.col, target.row);
    memcpy(&s_cameraSpotMeteringTarget, &target, sizeof(T_ZiyanCameraSpotMeteringTarget));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetSpotMeteringTarget(T_ZiyanCameraSpotMeteringTarget *target)
{
    memcpy(target, &s_cameraSpotMeteringTarget, sizeof(T_ZiyanCameraSpotMeteringTarget));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetFocusMode(E_ZiyanCameraFocusMode mode)
{
    USER_LOG_INFO("set focus mode:%d", mode);
    s_cameraFocusMode = mode;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetFocusMode(E_ZiyanCameraFocusMode *mode)
{
    *mode = s_cameraFocusMode;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetFocusTarget(T_ZiyanCameraPointInScreen target)
{
    USER_LOG_INFO("set focus target x:%.2f y:%.2f", target.focusX, target.focusY);
    memcpy(&s_cameraFocusTarget, &target, sizeof(T_ZiyanCameraPointInScreen));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetFocusTarget(T_ZiyanCameraPointInScreen *target)
{
    memcpy(target, &s_cameraFocusTarget, sizeof(T_ZiyanCameraPointInScreen));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetFocusAssistantSettings(T_ZiyanCameraFocusAssistantSettings settings)
{
    USER_LOG_INFO("set focus assistant setting MF:%d AF:%d", settings.isEnabledMF, settings.isEnabledAF);
    memcpy(&s_cameraFocusAssistantSettings, &settings, sizeof(T_ZiyanCameraFocusAssistantSettings));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetFocusAssistantSettings(T_ZiyanCameraFocusAssistantSettings *settings)
{
    memcpy(settings, &s_cameraFocusAssistantSettings, sizeof(T_ZiyanCameraFocusAssistantSettings));

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetFocusRingValue(uint32_t value)
{
    USER_LOG_INFO("set focus ring value:%d", value);
    s_cameraFocusRingValue = value;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetFocusRingValue(uint32_t *value)
{
    *value = s_cameraFocusRingValue;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetFocusRingValueUpperBound(uint32_t *value)
{
    *value = FOCUS_MAX_RINGVALUE;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetDigitalZoomFactor(ziyan_f32_t factor)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("set digital zoom factor:%.2f", factor);
    s_cameraDigitalZoomFactor = factor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetOpticalZoomFocalLength(uint32_t focalLength)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("set optical zoom focal length:%d", focalLength);
    s_isOpticalZoomReachLimit = false;
    s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
    s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetOpticalZoomFocalLength(uint32_t *focalLength)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *focalLength = s_cameraOpticalZoomFocalLength;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetOpticalZoomSpec(T_ZiyanCameraOpticalZoomSpec *spec)
{
    spec->maxFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
    spec->minFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
    spec->focalLengthStep = ZOOM_OPTICAL_FOCAL_LENGTH_STEP;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StartContinuousOpticalZoom(E_ZiyanCameraZoomDirection direction, E_ZiyanCameraZoomSpeed speed)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("start continuous optical zoom direction:%d speed:%d", direction, speed);
    s_isStartContinuousOpticalZoom = true;
    s_cameraZoomDirection = direction;
    s_cameraZoomSpeed = speed;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StopContinuousOpticalZoom(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("stop continuous optical zoom");
    s_isStartContinuousOpticalZoom = false;
    s_cameraZoomDirection = ZIYAN_CAMERA_ZOOM_DIRECTION_OUT;
    s_cameraZoomSpeed = ZIYAN_CAMERA_ZOOM_SPEED_NORMAL;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetTapZoomState(T_ZiyanCameraTapZoomState *state)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_tapZoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    memcpy(state, &s_cameraTapZoomState, sizeof(T_ZiyanCameraTapZoomState));

    returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetTapZoomEnabled(bool enabledFlag)
{
    USER_LOG_INFO("set tap zoom enabled flag: %d.", enabledFlag);
    s_isTapZoomEnabled = enabledFlag;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetTapZoomEnabled(bool *enabledFlag)
{
    *enabledFlag = s_isTapZoomEnabled;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetTapZoomMultiplier(uint8_t multiplier)
{
    USER_LOG_INFO("set tap zoom multiplier: %d.", multiplier);
    s_tapZoomMultiplier = multiplier;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetTapZoomMultiplier(uint8_t *multiplier)
{
    *multiplier = s_tapZoomMultiplier;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode TapZoomAtTarget(T_ZiyanCameraPointInScreen target)
{
    T_ZiyanReturnCode returnCode;
    E_ZiyanGimbalRotationMode rotationMode;
    T_ZiyanGimbalRotationProperty rotationProperty = {0};
    T_ZiyanAttitude3d rotationValue = {0};
    float hybridFocalLength = 0; // unit: 0.1mm
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    USER_LOG_INFO("tap zoom at target: x %f, y %f.", target.focusX, target.focusY);

    if (s_isTapZoomEnabled != true) {
        USER_LOG_WARN("tap zoom is not enabled.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (s_isTapZooming || s_isStartTapZoom) {
        USER_LOG_WARN("The last tap zoom process is not over.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
    }

    rotationMode = ZIYAN_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE;
    rotationProperty.relativeAngleRotation.actionTime = TAP_ZOOM_DURATION / 10;

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    /* Calculation formula: rotation angle = arctan((coordinate of target in sensor - coordinate of center point in
     * sensor) / hybrid focal length). Here, suppose that images of all pixels of sensor are displayed in screen,
     * and that center of the image sensor coincides with center of rotation of the gimbal, and that optical axis of
     * camera coincides with x-axis of gimbal. */
    hybridFocalLength = (ziyan_f32_t) s_cameraOpticalZoomFocalLength * s_cameraDigitalZoomFactor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    rotationValue.pitch = (int32_t) (
        atan2f((target.focusY - CENTER_POINT_IN_SCREEN_Y_VALUE) * IMAGE_SENSOR_Y_SIZE, hybridFocalLength) * 1800 /
        ZIYAN_PI);
    rotationValue.yaw = (int32_t) (
        atan2f((target.focusX - CENTER_POINT_IN_SCREEN_X_VALUE) * IMAGE_SENSOR_X_SIZE, hybridFocalLength) * 1800 /
        ZIYAN_PI);

    returnCode = osalHandler->MutexLock(s_tapZoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_tapZoomNewestGimbalRotationArgument.rotationMode = rotationMode;
    s_tapZoomNewestGimbalRotationArgument.rotationProperty = rotationProperty;
    s_tapZoomNewestGimbalRotationArgument.rotationValue = rotationValue;
    s_tapZoomNewestTargetHybridFocalLength = (uint32_t) (hybridFocalLength * (float) s_tapZoomMultiplier);

    returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_isStartTapZoom = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanTest_CameraHybridZoom(uint32_t focalLength)
{
    if (focalLength >= ZOOM_OPTICAL_FOCAL_MIN_LENGTH && focalLength <= ZOOM_OPTICAL_FOCAL_MAX_LENGTH) {
        s_cameraOpticalZoomFocalLength = focalLength;
        s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;
    } else if (focalLength < ZOOM_OPTICAL_FOCAL_MIN_LENGTH) {
        s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
        s_cameraDigitalZoomFactor = ZOOM_DIGITAL_BASE_FACTOR;

        USER_LOG_ERROR("focal length %d is smaller than min optical focal length %d.", focalLength,
                       ZOOM_OPTICAL_FOCAL_MIN_LENGTH);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    } else {
        s_cameraOpticalZoomFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
        s_cameraDigitalZoomFactor = (float) focalLength / ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanTest_CameraRotationGimbal(T_TestCameraGimbalRotationArgument gimbalRotationArgument)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanAircraftInfoBaseInfo aircraftBaseInfo = {0};

    returnCode = ZiyanAircraftInfo_GetBaseInfo(&aircraftBaseInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft information error: 0x%08llX.", returnCode);
        return returnCode;
    }

    if (aircraftBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_NONE) {
        returnCode = ZiyanTest_GimbalRotate(gimbalRotationArgument.rotationMode, gimbalRotationArgument.rotationProperty,
                                          gimbalRotationArgument.rotationValue);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("rotate gimbal error: 0x%08llX.", returnCode);
            return returnCode;
        }
    }else {
        USER_LOG_ERROR("PSDK adapter type is invalid: %d.", aircraftBaseInfo.ziyanAdapterType);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserCamera_Task(void *arg)
{
    static uint32_t photoCnt = 0;
    static uint32_t step = 0;
    T_ZiyanReturnCode returnCode;
    int32_t tempFocalLength = 0;
    ziyan_f32_t tempDigitalFactor = 0.0f;
    uint32_t currentTime = 0;
    bool isStartIntervalPhotoAction = false;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(1000 / PAYLOAD_CAMERA_EMU_TASK_FREQ);
        step++;

        returnCode = osalHandler->MutexLock(s_commonMutex);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
            continue;
        }

        s_cameraSDCardState.availableRecordingTimeInSeconds =
            s_cameraSDCardState.remainSpaceInMB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;
        s_cameraSDCardState.availableCaptureCount = s_cameraSDCardState.remainSpaceInMB / SDCARD_PER_PHOTO_SPACE_IN_MB;

        //realize your photo shoot and storage logic here
        if (s_cameraState.shootingState != ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE &&
            photoCnt++ > TAKING_PHOTO_SPENT_TIME_MS_EMU / (1000 / PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            photoCnt = 0;

            //store the photo after shooting finished
            if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB;
                s_cameraState.isStoring = false;
                s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE;
            } else if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_BURST) {
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB * s_cameraBurstCount;
                s_cameraState.isStoring = false;
                s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE;
            } else if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_INTERVAL) {
                if (isStartIntervalPhotoAction == true) {
                    s_cameraState.isStoring = false;
                    s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE;
                    s_cameraSDCardState.remainSpaceInMB =
                        s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_PHOTO_SPACE_IN_MB;
                }
            }

            //check the remain space of sdcard
            if (s_cameraSDCardState.remainSpaceInMB > SDCARD_TOTAL_SPACE_IN_MB) {
                s_cameraSDCardState.remainSpaceInMB = 0;
                s_cameraSDCardState.isFull = true;
            }

#if ziyan_ARCH_SYS_LINUX
            T_ZiyanCameraMediaFileInfo mediaFileInfo = {0};

            // push added media file information
            if (s_cameraShootPhotoMode == ziyan_CAMERA_SHOOT_PHOTO_MODE_SINGLE) {
                if (UtilFile_IsFileExist(PHOTO_FILE_PATH) == true) {
                    psdkStat = PsdkTest_CameraMediaGetFileInfo(PHOTO_FILE_PATH, &mediaFileInfo);
                    if (psdkStat != ziyan_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        PsdkLogger_UserLogError("Get media file info error 0x%08llX", psdkStat);
                    }

                    psdkStat = PsdkPayloadCamera_PushAddedMediaFileInfo(PHOTO_FILE_PATH, mediaFileInfo);
                    if (psdkStat != ziyan_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        PsdkLogger_UserLogError("Push added media file info error 0x%08llX", psdkStat);
                    }
                } else {
                    PsdkLogger_UserLogWarn("Can't found the media file by path. "
                                           "Probably because media file has not existed. "
                                           "Please check media file if exists. ");
                }
            }
#endif
        }

        returnCode = osalHandler->MutexUnlock(s_commonMutex);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
            continue;
        }

        // 10Hz
        if (USER_UTIL_IS_WORK_TURN(step, 10, PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            returnCode = osalHandler->MutexLock(s_zoomMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                continue;
            }

#ifdef USER_CAMERA_EMU_HYBRID_ZOOM_ON
            //Add logic here for zoom camera
            if (s_isStartContinuousOpticalZoom == true) {
                tempDigitalFactor = s_cameraDigitalZoomFactor;
                tempFocalLength = (int32_t) s_cameraOpticalZoomFocalLength;
                if (s_isOpticalZoomReachLimit == false) {
                    if (s_cameraZoomDirection == ZIYAN_CAMERA_ZOOM_DIRECTION_IN) {
                        tempFocalLength += ((int) s_cameraZoomSpeed - ZIYAN_CAMERA_ZOOM_SPEED_SLOWEST + 1) *
                                           ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP;
                    } else if (s_cameraZoomDirection == ZIYAN_CAMERA_ZOOM_DIRECTION_OUT) {
                        tempFocalLength -= ((int) s_cameraZoomSpeed - ZIYAN_CAMERA_ZOOM_SPEED_SLOWEST + 1) *
                                           ZOOM_OPTICAL_FOCAL_LENGTH_CTRL_STEP;
                    }

                    if (tempFocalLength > ZOOM_OPTICAL_FOCAL_MAX_LENGTH) {
                        s_isOpticalZoomReachLimit = true;
                        tempFocalLength = ZOOM_OPTICAL_FOCAL_MAX_LENGTH;
                    }

                    if (tempFocalLength < ZOOM_OPTICAL_FOCAL_MIN_LENGTH) {
                        tempFocalLength = ZOOM_OPTICAL_FOCAL_MIN_LENGTH;
                    }
                } else {
                    if (s_cameraZoomDirection == ZIYAN_CAMERA_ZOOM_DIRECTION_IN) {
                        tempDigitalFactor += (ziyan_f32_t) ZOOM_DIGITAL_STEP_FACTOR;
                    } else if (s_cameraZoomDirection == ZIYAN_CAMERA_ZOOM_DIRECTION_OUT) {
                        tempDigitalFactor -= (ziyan_f32_t) ZOOM_DIGITAL_STEP_FACTOR;
                    }

                    if (tempDigitalFactor > (ziyan_f32_t) ZOOM_DIGITAL_MAX_FACTOR) {
                        tempDigitalFactor = (ziyan_f32_t) ZOOM_DIGITAL_MAX_FACTOR;
                    }

                    if (tempDigitalFactor < (ziyan_f32_t) ZOOM_DIGITAL_BASE_FACTOR) {
                        s_isOpticalZoomReachLimit = false;
                        tempDigitalFactor = ZOOM_DIGITAL_BASE_FACTOR;
                    }
                }
                s_cameraOpticalZoomFocalLength = (uint16_t) tempFocalLength;
                s_cameraDigitalZoomFactor = tempDigitalFactor;
            }
#endif

#if USER_CAMERA_EMU_TAP_ZOOM_ON
            returnCode = osalHandler->MutexLock(s_tapZoomMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                goto out;
            }

            if (s_isStartTapZoom) {
                s_isStartTapZoom = false;
                s_isTapZooming = true;

                returnCode = osalHandler->GetTimeMs(&s_tapZoomStartTime);
                if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("get start time error: 0x%08llX.", returnCode);
                }

                returnCode = ZiyanTest_CameraRotationGimbal(s_tapZoomNewestGimbalRotationArgument);
                if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
                    USER_LOG_ERROR("rotate gimbal error: 0x%08llX.", returnCode);
                else
                    s_cameraTapZoomState.isGimbalMoving = true;

                returnCode = ZiyanTest_CameraHybridZoom(s_tapZoomNewestTargetHybridFocalLength);
                if (returnCode == ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    s_cameraTapZoomState.zoomState = (ziyan_f32_t) s_tapZoomNewestTargetHybridFocalLength >
                                                     ((ziyan_f32_t) s_cameraOpticalZoomFocalLength *
                                                      s_cameraDigitalZoomFactor)
                                                     ? ZIYAN_CAMERA_TAP_ZOOM_STATE_ZOOM_IN
                                                     : ZIYAN_CAMERA_TAP_ZOOM_STATE_ZOOM_OUT;
                } else if (returnCode == ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE) {
                    USER_LOG_ERROR("hybrid zoom focal length beyond limit.");
                    s_cameraTapZoomState.zoomState = ZIYAN_CAMERA_TAP_ZOOM_STATE_ZOOM_LIMITED;
                } else {
                    USER_LOG_ERROR("hybrid zoom error: 0x%08llX.", returnCode);
                }
            } else if (s_isTapZooming) {
                returnCode = osalHandler->GetTimeMs(&currentTime);
                if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("get start time error: 0x%08llX.", returnCode);
                }

                if ((currentTime - s_tapZoomStartTime) >= TAP_ZOOM_DURATION) {
                    s_cameraTapZoomState.zoomState = ZIYAN_CAMERA_TAP_ZOOM_STATE_IDLE;
                    s_cameraTapZoomState.isGimbalMoving = false;
                    s_isTapZooming = false;
                }
            }

            returnCode = osalHandler->MutexUnlock(s_tapZoomMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                goto out;
            }
#endif

out:
            returnCode = osalHandler->MutexUnlock(s_zoomMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                continue;
            }
        }

        // 1Hz
        if (USER_UTIL_IS_WORK_TURN(step, 1, PAYLOAD_CAMERA_EMU_TASK_FREQ)) {
            returnCode = osalHandler->MutexLock(s_commonMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
                continue;
            }

            if (s_cameraState.isRecording) {
                s_cameraState.currentVideoRecordingTimeInSeconds++;
                s_cameraSDCardState.remainSpaceInMB =
                    s_cameraSDCardState.remainSpaceInMB - SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;
                if (s_cameraSDCardState.remainSpaceInMB > SDCARD_TOTAL_SPACE_IN_MB) {
                    s_cameraSDCardState.remainSpaceInMB = 0;
                    s_cameraSDCardState.isFull = true;
                }
            }

            if (s_cameraState.isShootingIntervalStart == false) {
                isStartIntervalPhotoAction = false;
            }

            if (s_cameraShootPhotoMode == ZIYAN_CAMERA_SHOOT_PHOTO_MODE_INTERVAL
                && s_cameraState.isShootingIntervalStart == true && s_cameraPhotoTimeIntervalSettings.captureCount > 0
                && s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds > 0) {
                s_cameraState.currentPhotoShootingIntervalTimeInSeconds--;

                if ((s_cameraState.currentPhotoShootingIntervalTimeInSeconds == 0 &&
                     s_cameraState.currentPhotoShootingIntervalCount > 0) ||
                    (s_cameraState.isShootingIntervalStart == true && isStartIntervalPhotoAction == false)) {

                    isStartIntervalPhotoAction = true;

                    s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_INTERVAL_PHOTO;
                    s_cameraState.isStoring = true;
                    s_cameraState.currentPhotoShootingIntervalTimeInSeconds
                        = s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds;
                    if (s_cameraState.currentPhotoShootingIntervalCount < INTERVAL_PHOTOGRAPH_ALWAYS_COUNT) {
                        USER_LOG_INFO("interval taking photograph count:%d interval_time:%ds",
                                      (s_cameraPhotoTimeIntervalSettings.captureCount -
                                       s_cameraState.currentPhotoShootingIntervalCount + 1),
                                      s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds);
                        s_cameraState.currentPhotoShootingIntervalCount--;
                        if (s_cameraState.currentPhotoShootingIntervalCount == 0) {
                            s_cameraState.shootingState = ZIYAN_CAMERA_SHOOTING_PHOTO_IDLE;
                            s_cameraState.isStoring = false;
                            s_cameraState.isShootingIntervalStart = false;
                        }
                    } else {
                        USER_LOG_INFO("interval taking photograph always, interval_time:%ds",
                                      s_cameraPhotoTimeIntervalSettings.timeIntervalSeconds);
                        s_cameraState.currentPhotoShootingIntervalCount--;
                    }
                }
            }

            returnCode = osalHandler->MutexUnlock(s_commonMutex);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
                continue;
            }
        }
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/* Private functions definition-----------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_CameraEmuBaseStartService(void)
{
    T_ZiyanReturnCode returnCode;
    char ipAddr[ZIYAN_IP_ADDR_STR_SIZE_MAX] = {0};
    uint16_t port = 0;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    T_ZiyanAircraftInfoBaseInfo aircraftInfoBaseInfo = {0};

    returnCode = osalHandler->MutexCreate(&s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexCreate(&s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexCreate(&s_tapZoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("create mutex used to lock tap zoom arguments error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanPayloadCamera_Init();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init payload camera error:0x%08llX", returnCode);
        return returnCode;
    }

    /* Init the SDcard parameters */
    s_cameraSDCardState.isInserted = true;
    s_cameraSDCardState.isVerified = true;
    s_cameraSDCardState.totalSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.remainSpaceInMB = SDCARD_TOTAL_SPACE_IN_MB;
    s_cameraSDCardState.availableCaptureCount = SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_PHOTO_SPACE_IN_MB;
    s_cameraSDCardState.availableRecordingTimeInSeconds =
        SDCARD_TOTAL_SPACE_IN_MB / SDCARD_PER_SECONDS_RECORD_SPACE_IN_MB;

    /* Register the camera common handler */
    s_commonHandler.GetSystemState = GetSystemState;
    s_commonHandler.SetMode = SetMode;
    s_commonHandler.GetMode = ZiyanTest_CameraGetMode;
    s_commonHandler.StartRecordVideo = StartRecordVideo;
    s_commonHandler.StopRecordVideo = StopRecordVideo;
    s_commonHandler.StartShootPhoto = StartShootPhoto;
    s_commonHandler.StopShootPhoto = StopShootPhoto;
    s_commonHandler.SetShootPhotoMode = SetShootPhotoMode;
    s_commonHandler.GetShootPhotoMode = GetShootPhotoMode;
    s_commonHandler.SetPhotoBurstCount = SetPhotoBurstCount;
    s_commonHandler.GetPhotoBurstCount = GetPhotoBurstCount;
    s_commonHandler.SetPhotoTimeIntervalSettings = SetPhotoTimeIntervalSettings;
    s_commonHandler.GetPhotoTimeIntervalSettings = GetPhotoTimeIntervalSettings;
    s_commonHandler.GetSDCardState = GetSDCardState;
    s_commonHandler.FormatSDCard = FormatSDCard;

    returnCode = ZiyanPayloadCamera_RegCommonHandler(&s_commonHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register common handler error:0x%08llX", returnCode);
    }

    /* Register the camera metering handler */
#if USER_CAMERA_EMU_METERING_ON
    s_exposureMeteringHandler.SetMeteringMode = SetMeteringMode;
    s_exposureMeteringHandler.GetMeteringMode = GetMeteringMode;
    s_exposureMeteringHandler.SetSpotMeteringTarget = SetSpotMeteringTarget;
    s_exposureMeteringHandler.GetSpotMeteringTarget = GetSpotMeteringTarget;

    returnCode = ZiyanPayloadCamera_RegExposureMeteringHandler(&s_exposureMeteringHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register exposure metering handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera focus handler */
#if USER_CAMERA_EMU_FOCUS_ON
    s_focusHandler.SetFocusMode = SetFocusMode;
    s_focusHandler.GetFocusMode = GetFocusMode;
    s_focusHandler.SetFocusTarget = SetFocusTarget;
    s_focusHandler.GetFocusTarget = GetFocusTarget;
    s_focusHandler.SetFocusAssistantSettings = SetFocusAssistantSettings;
    s_focusHandler.GetFocusAssistantSettings = GetFocusAssistantSettings;
    s_focusHandler.SetFocusRingValue = SetFocusRingValue;
    s_focusHandler.GetFocusRingValue = GetFocusRingValue;
    s_focusHandler.GetFocusRingValueUpperBound = GetFocusRingValueUpperBound;

    returnCode = ZiyanPayloadCamera_RegFocusHandler(&s_focusHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register adjustable focal point handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera digital zoom handler */
#if USER_CAMERA_EMU_HYBRID_ZOOM_ON
    s_digitalZoomHandler.SetDigitalZoomFactor = SetDigitalZoomFactor;
    s_digitalZoomHandler.GetDigitalZoomFactor = ZiyanTest_CameraGetDigitalZoomFactor;

    returnCode = ZiyanPayloadCamera_RegDigitalZoomHandler(&s_digitalZoomHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register digital zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }

    /* Register the camera optical zoom handler */
    s_opticalZoomHandler.SetOpticalZoomFocalLength = SetOpticalZoomFocalLength;
    s_opticalZoomHandler.GetOpticalZoomFocalLength = GetOpticalZoomFocalLength;
    s_opticalZoomHandler.GetOpticalZoomFactor = ZiyanTest_CameraGetOpticalZoomFactor;
    s_opticalZoomHandler.GetOpticalZoomSpec = GetOpticalZoomSpec;
    s_opticalZoomHandler.StartContinuousOpticalZoom = StartContinuousOpticalZoom;
    s_opticalZoomHandler.StopContinuousOpticalZoom = StopContinuousOpticalZoom;

    returnCode = ZiyanPayloadCamera_RegOpticalZoomHandler(&s_opticalZoomHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register optical zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    /* Register the camera tap zoom handler */
#if USER_CAMERA_EMU_TAP_ZOOM_ON
    s_tapZoomHandler.GetTapZoomState = GetTapZoomState;
    s_tapZoomHandler.SetTapZoomEnabled = SetTapZoomEnabled;
    s_tapZoomHandler.GetTapZoomEnabled = GetTapZoomEnabled;
    s_tapZoomHandler.SetTapZoomMultiplier = SetTapZoomMultiplier;
    s_tapZoomHandler.GetTapZoomMultiplier = GetTapZoomMultiplier;
    s_tapZoomHandler.TapZoomAtTarget = TapZoomAtTarget;

    returnCode = ZiyanPayloadCamera_RegTapZoomHandler(&s_tapZoomHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register tap zoom handler error:0x%08llX", returnCode);
        return returnCode;
    }
#endif

    s_extendHander.GetRangeFinderSwitch = GetRangeFinderSwitch;
    s_extendHander.SetRangeFinderSwitch = SetRangeFinderSwitch;
    s_extendHander.GetRangeFinderData   = GetRangeFinderData;
    s_extendHander.GetRangeFinderLocation = GetRangeFinderLocation;

    returnCode = ZiyanPayloadCamera_RegExtendHandler(&s_extendHander);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("camera register extend handler error:0x%08llX", returnCode);
        return returnCode;
    }

    
    T_ZiyanTaskHandle handle_test;
    osalHandler->TaskCreate("camera_vedio_task", push_video_stream8,
                                PAYLOAD_CAMERA_EMU_TASK_STACK_SIZE, NULL, &handle_test);

    /* Create the camera emu taskHandle */
    if (osalHandler->TaskCreate("user_camera_task", UserCamera_Task,
                                PAYLOAD_CAMERA_EMU_TASK_STACK_SIZE, NULL, &s_userCameraThread)
        != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user camera taskHandle create error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (ZiyanAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft information error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = ZiyanPayloadCamera_SetVideoStreamType(ZIYAN_CAMERA_VIDEO_STREAM_TYPE_H264_ZIYAN_FORMAT);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("ZIYAN camera set video stream error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    s_cameraVideoStreamType = ZIYAN_CAMERA_VIDEO_STREAM_TYPE_H264_ZIYAN_FORMAT;

    returnCode = ZiyanPayloadCamera_GetVideoStreamRemoteAddress(ipAddr, &port);
    if (returnCode == ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_DEBUG("Get video stream remote address: %s, port: %d", ipAddr, port);
    } else {
        USER_LOG_ERROR("get video stream remote address error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    s_isCamInited = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraGetDigitalZoomFactor(ziyan_f32_t *factor)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *factor = s_cameraDigitalZoomFactor;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraGetOpticalZoomFactor(ziyan_f32_t *factor)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    //Formula:factor = currentFocalLength / minFocalLength
    *factor = (ziyan_f32_t) s_cameraOpticalZoomFocalLength / ZOOM_OPTICAL_FOCAL_MIN_LENGTH;

    returnCode = osalHandler->MutexUnlock(s_zoomMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraGetMode(E_ZiyanCameraMode *mode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *mode = s_cameraState.cameraMode;

    returnCode = osalHandler->MutexUnlock(s_commonMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraGetVideoStreamType(E_ZiyanCameraVideoStreamType *type)
{
    if (s_isCamInited == false) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE;
    }

    *type = s_cameraVideoStreamType;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

bool ZiyanTest_CameraIsInited(void)
{
    return s_isCamInited;
}


T_ZiyanReturnCode SetRangeFinderSwitch(bool enable)
{
    range_finder_switch = enable;
    USER_LOG_INFO("Set range finder switch:%d.", range_finder_switch);
    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode GetRangeFinderSwitch(bool* enable)
{
    if (enable == NULL){
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *enable = range_finder_switch;
    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode GetRangeFinderData(ziyan_f32_t* distance)
{
    if (distance == NULL){
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    range_finder_distance += 0.1f;
    *distance = range_finder_distance;
    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode GetRangeFinderLocation(T_ZiyanLocation* location)
{
    if (location == NULL){
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    range_finder_target_location.latitude += 0.0001f;
    range_finder_target_location.longitude += 0.0001f;
    range_finder_target_location.altitude += 0.1f;
    *location = range_finder_target_location;
    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

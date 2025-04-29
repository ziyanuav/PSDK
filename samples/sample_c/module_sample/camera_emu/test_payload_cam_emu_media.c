/**
 ********************************************************************
 * @file    test_payload_cam_emu_media.c
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
#include <fcntl.h>
#include <stdlib.h>
#include "ziyan_logger.h"
#include "utils/util_misc.h"
#include "utils/util_time.h"
#include "utils/util_file.h"
#include "utils/util_buffer.h"
#include "test_payload_cam_emu_media.h"
#include "test_payload_cam_emu_base.h"
#include "camera_emu/ziyan_media_file_manage/ziyan_media_file_core.h"
#include "ziyan_high_speed_data_channel.h"
#include "ziyan_aircraft_info.h"

/* Private constants ---------------------------------------------------------*/
#define FFMPEG_CMD_BUF_SIZE                 (256 + 256)
#define SEND_VIDEO_TASK_FREQ                 120
#define VIDEO_FRAME_MAX_COUNT                18000 // max video duration 10 minutes
#define VIDEO_FRAME_AUD_LEN                  6
#define DATA_SEND_FROM_VIDEO_STREAM_MAX_LEN  60000

/* Private types -------------------------------------------------------------*/
typedef enum {
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP = 0,
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE = 1,
    TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START = 2,
} E_TestPayloadCameraPlaybackCommand;

typedef struct {
    uint8_t isInPlayProcess;
    uint16_t videoIndex;
    char filePath[ZIYAN_FILE_PATH_SIZE_MAX];
    uint32_t videoLengthMs;
    uint64_t startPlayTimestampsUs;
    uint64_t playPosMs;
} T_ZiyanPlaybackInfo;

typedef struct {
    E_TestPayloadCameraPlaybackCommand command;
    uint32_t timeMs;
    char path[ZIYAN_FILE_PATH_SIZE_MAX];
} T_TestPayloadCameraPlaybackCommand;

typedef struct {
    float durationS;
    uint32_t positionInFile;
    uint32_t size;
} T_TestPayloadCameraVideoFrameInfo;

/* Private functions declaration ---------------------------------------------*/
static T_ZiyanReturnCode ZiyanPlayback_StopPlay(T_ZiyanPlaybackInfo *playbackInfo);
static T_ZiyanReturnCode ZiyanPlayback_PausePlay(T_ZiyanPlaybackInfo *playbackInfo);
static T_ZiyanReturnCode ZiyanPlayback_SetPlayFile(T_ZiyanPlaybackInfo *playbackInfo, const char *filePath,
                                               uint16_t index);
static T_ZiyanReturnCode ZiyanPlayback_SeekPlay(T_ZiyanPlaybackInfo *playbackInfo, uint32_t seekPos);
static T_ZiyanReturnCode ZiyanPlayback_StartPlay(T_ZiyanPlaybackInfo *playbackInfo);
static T_ZiyanReturnCode ZiyanPlayback_GetPlaybackStatus(T_ZiyanPlaybackInfo *playbackInfo,
                                                     T_ZiyanCameraPlaybackStatus *playbackStatus);
static T_ZiyanReturnCode ZiyanPlayback_GetVideoLengthMs(const char *filePath, uint32_t *videoLengthMs);
static T_ZiyanReturnCode ZiyanPlayback_StartPlayProcess(const char *filePath, uint32_t playPosMs);
static T_ZiyanReturnCode ZiyanPlayback_StopPlayProcess(void);
static T_ZiyanReturnCode
ZiyanPlayback_VideoFileTranscode(const char *inPath, const char *outFormat, char *outPath, uint16_t outPathBufferSize);
static T_ZiyanReturnCode
ZiyanPlayback_GetFrameInfoOfVideoFile(const char *path, T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                    uint32_t frameInfoBufferCount, uint32_t *frameCount);
static T_ZiyanReturnCode ZiyanPlayback_GetFrameRateOfVideoFile(const char *path, float *frameRate);
static T_ZiyanReturnCode
ZiyanPlayback_GetFrameNumberByTime(T_TestPayloadCameraVideoFrameInfo *frameInfo, uint32_t frameCount,
                                 uint32_t *frameNumber, uint32_t timeMs);
static T_ZiyanReturnCode GetMediaFileDir(char *dirPath);
static T_ZiyanReturnCode GetMediaFileOriginData(const char *filePath, uint32_t offset, uint32_t length,
                                              uint8_t *data);

static T_ZiyanReturnCode CreateMediaFileThumbNail(const char *filePath);
static T_ZiyanReturnCode GetMediaFileThumbNailInfo(const char *filePath, T_ZiyanCameraMediaFileInfo *fileInfo);
static T_ZiyanReturnCode GetMediaFileThumbNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                 uint8_t *data);
static T_ZiyanReturnCode DestroyMediaFileThumbNail(const char *filePath);

static T_ZiyanReturnCode CreateMediaFileScreenNail(const char *filePath);
static T_ZiyanReturnCode GetMediaFileScreenNailInfo(const char *filePath, T_ZiyanCameraMediaFileInfo *fileInfo);
static T_ZiyanReturnCode GetMediaFileScreenNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                  uint8_t *data);
static T_ZiyanReturnCode DestroyMediaFileScreenNail(const char *filePath);

static T_ZiyanReturnCode DeleteMediaFile(char *filePath);
static T_ZiyanReturnCode SetMediaPlaybackFile(const char *filePath);
static T_ZiyanReturnCode StartMediaPlayback(void);
static T_ZiyanReturnCode StopMediaPlayback(void);
static T_ZiyanReturnCode PauseMediaPlayback(void);
static T_ZiyanReturnCode SeekMediaPlayback(uint32_t playbackPosition);
static T_ZiyanReturnCode GetMediaPlaybackStatus(T_ZiyanCameraPlaybackStatus *status);

static T_ZiyanReturnCode StartDownloadNotification(void);
static T_ZiyanReturnCode StopDownloadNotification(void);

_Noreturn static void *UserCameraMedia_SendVideoTask(void *arg);

/* Private variables -------------------------------------------------------------*/
static T_ZiyanCameraMediaDownloadPlaybackHandler s_psdkCameraMedia = {0};
static T_ZiyanPlaybackInfo s_playbackInfo = {0};
static T_ZiyanTaskHandle s_userSendVideoThread;
static T_UtilBuffer s_mediaPlayCommandBufferHandler = {0};
static T_ZiyanMutexHandle s_mediaPlayCommandBufferMutex = {0};
static T_ZiyanSemaHandle s_mediaPlayWorkSem = NULL;
static uint8_t s_mediaPlayCommandBuffer[sizeof(T_TestPayloadCameraPlaybackCommand) * 32] = {0};
static const char *s_frameKeyChar = "[PACKET]";
static const char *s_frameDurationTimeKeyChar = "duration_time";
static const char *s_framePositionKeyChar = "pos";
static const char *s_frameSizeKeyChar = "size";
static T_ZiyanMediaFileHandle s_mediaFileThumbNailHandle;
static T_ZiyanMediaFileHandle s_mediaFileScreenNailHandle;
static const uint8_t s_frameAudInfo[VIDEO_FRAME_AUD_LEN] = {0x00, 0x00, 0x00, 0x01, 0x09, 0x10};
static char s_mediaFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX] = {0};
static bool s_isMediaFileDirPathConfigured = false;

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_CameraEmuMediaStartService(void)
{
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    T_ZiyanReturnCode returnCode;
    const T_ZiyanDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportionOfHighspeedChannel =
        {10, 60, 30};
    T_ZiyanAircraftInfoBaseInfo aircraftInfoBaseInfo = {0};

    if (ZiyanAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft information error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    s_psdkCameraMedia.GetMediaFileDir = GetMediaFileDir;
    s_psdkCameraMedia.GetMediaFileOriginInfo = ZiyanTest_CameraMediaGetFileInfo;
    s_psdkCameraMedia.GetMediaFileOriginData = GetMediaFileOriginData;

    s_psdkCameraMedia.CreateMediaFileThumbNail = CreateMediaFileThumbNail;
    s_psdkCameraMedia.GetMediaFileThumbNailInfo = GetMediaFileThumbNailInfo;
    s_psdkCameraMedia.GetMediaFileThumbNailData = GetMediaFileThumbNailData;
    s_psdkCameraMedia.DestroyMediaFileThumbNail = DestroyMediaFileThumbNail;

    s_psdkCameraMedia.CreateMediaFileScreenNail = CreateMediaFileScreenNail;
    s_psdkCameraMedia.GetMediaFileScreenNailInfo = GetMediaFileScreenNailInfo;
    s_psdkCameraMedia.GetMediaFileScreenNailData = GetMediaFileScreenNailData;
    s_psdkCameraMedia.DestroyMediaFileScreenNail = DestroyMediaFileScreenNail;

    s_psdkCameraMedia.DeleteMediaFile = DeleteMediaFile;

    s_psdkCameraMedia.SetMediaPlaybackFile = SetMediaPlaybackFile;

    s_psdkCameraMedia.StartMediaPlayback = StartMediaPlayback;
    s_psdkCameraMedia.StopMediaPlayback = StopMediaPlayback;
    s_psdkCameraMedia.PauseMediaPlayback = PauseMediaPlayback;
    s_psdkCameraMedia.SeekMediaPlayback = SeekMediaPlayback;
    s_psdkCameraMedia.GetMediaPlaybackStatus = GetMediaPlaybackStatus;

    s_psdkCameraMedia.StartDownloadNotification = StartDownloadNotification;
    s_psdkCameraMedia.StopDownloadNotification = StopDownloadNotification;

    if (ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS != osalHandler->SemaphoreCreate(0, &s_mediaPlayWorkSem)) {
        USER_LOG_ERROR("SemaphoreCreate(\"%s\") error.", "s_mediaPlayWorkSem");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (osalHandler->MutexCreate(&s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex create error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    UtilBuffer_Init(&s_mediaPlayCommandBufferHandler, s_mediaPlayCommandBuffer, sizeof(s_mediaPlayCommandBuffer));

    if (aircraftInfoBaseInfo.aircraftType == ZIYAN_AIRCRAFT_TYPE_SHADOW_PLUS ||
        aircraftInfoBaseInfo.aircraftType == ZIYAN_AIRCRAFT_TYPE_SHADOW_MAX) {
        returnCode = ZiyanPayloadCamera_RegMediaDownloadPlaybackHandler(&s_psdkCameraMedia);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("psdk camera media function init error.");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }
    }

    returnCode = ZiyanHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportionOfHighspeedChannel);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set data channel bandwidth width proportion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (ZiyanPlatform_GetHalNetworkHandler() != NULL || ZiyanPlatform_GetHalUsbBulkHandler() != NULL) {
        returnCode = osalHandler->TaskCreate("user_camera_media_task", UserCameraMedia_SendVideoTask, 2048,
                                             NULL, &s_userSendVideoThread);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("user send video task create error.");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraEmuSetMediaFilePath(const char *path)
{
    memset(s_mediaFileDirPath, 0, sizeof(s_mediaFileDirPath));
    memcpy(s_mediaFileDirPath, path, USER_UTIL_MIN(strlen(path), sizeof(s_mediaFileDirPath) - 1));
    s_isMediaFileDirPathConfigured = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_CameraMediaGetFileInfo(const char *filePath, T_ZiyanCameraMediaFileInfo *fileInfo)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanMediaFileHandle mediaFileHandle;

    returnCode = ZiyanMediaFile_CreateHandle(filePath, &mediaFileHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetMediaFileType(mediaFileHandle, &fileInfo->type);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        goto out;
    }

    returnCode = ZiyanMediaFile_GetMediaFileAttr(mediaFileHandle, &fileInfo->mediaFileAttr);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        goto out;
    }

    returnCode = ZiyanMediaFile_GetFileSizeOrg(mediaFileHandle, &fileInfo->fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        goto out;
    }

out:
    returnCode = ZiyanMediaFile_DestroyHandle(mediaFileHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static T_ZiyanReturnCode ZiyanPlayback_StopPlay(T_ZiyanPlaybackInfo *playbackInfo)
{
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanPlayback_StopPlayProcess();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop play error ");
    }

    playbackInfo->isInPlayProcess = 0;
    playbackInfo->playPosMs = 0;

    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_PausePlay(T_ZiyanPlaybackInfo *playbackInfo)
{
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    if (playbackInfo->isInPlayProcess) {
        playbackCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE;

        if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex lock error");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }

        if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
            UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &playbackCommand,
                           sizeof(T_TestPayloadCameraPlaybackCommand));
        } else {
            USER_LOG_ERROR("Media playback command buffer is full.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
        }

        if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex unlock error");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }
        osalHandler->SemaphorePost(s_mediaPlayWorkSem);
    }

    playbackInfo->isInPlayProcess = 0;

    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_SetPlayFile(T_ZiyanPlaybackInfo *playbackInfo, const char *filePath, uint16_t index)
{
    T_ZiyanReturnCode returnCode;

    if (strlen(filePath) > ZIYAN_FILE_PATH_SIZE_MAX) {
        USER_LOG_ERROR("Ziyan playback file path out of length range error\n");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    strcpy(playbackInfo->filePath, filePath);
    playbackInfo->videoIndex = index;

    returnCode = ZiyanPlayback_GetVideoLengthMs(filePath, &playbackInfo->videoLengthMs);

    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanPlayback_SeekPlay(T_ZiyanPlaybackInfo *playbackInfo, uint32_t seekPos)
{
    T_ZiyanRunTimeStamps ti;
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanPlayback_PausePlay(playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("pause play error \n");
        return returnCode;
    }

    playbackInfo->playPosMs = seekPos;
    returnCode = ZiyanPlayback_StartPlayProcess(playbackInfo->filePath, playbackInfo->playPosMs);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start playback process error \n");
        return returnCode;
    }

    playbackInfo->isInPlayProcess = 1;
    ti = ZiyanUtilTime_GetRunTimeStamps();
    playbackInfo->startPlayTimestampsUs = ti.realUsec - playbackInfo->playPosMs * 1000;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanPlayback_StartPlay(T_ZiyanPlaybackInfo *playbackInfo)
{
    T_ZiyanRunTimeStamps ti;
    T_ZiyanReturnCode returnCode;

    if (playbackInfo->isInPlayProcess == 1) {
        //already in playing, return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    returnCode = ZiyanPlayback_StartPlayProcess(playbackInfo->filePath, playbackInfo->playPosMs);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start play process error \n");
        return returnCode;
    }

    playbackInfo->isInPlayProcess = 1;

    ti = ZiyanUtilTime_GetRunTimeStamps();
    playbackInfo->startPlayTimestampsUs = ti.realUsec - playbackInfo->playPosMs * 1000;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanPlayback_GetPlaybackStatus(T_ZiyanPlaybackInfo *playbackInfo,
                                                     T_ZiyanCameraPlaybackStatus *playbackStatus)
{
    T_ZiyanRunTimeStamps timeStamps;

    memset(playbackStatus, 0, sizeof(T_ZiyanCameraPlaybackStatus));

    //update playback pos info
    if (playbackInfo->isInPlayProcess) {
        timeStamps = ZiyanUtilTime_GetRunTimeStamps();
        playbackInfo->playPosMs = (timeStamps.realUsec - playbackInfo->startPlayTimestampsUs) / 1000;

        if (playbackInfo->playPosMs >= playbackInfo->videoLengthMs) {
            if (ZiyanPlayback_PausePlay(playbackInfo) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            }
        }
    }

    //set playback status
    if (playbackInfo->isInPlayProcess == 0 && playbackInfo->playPosMs != 0) {
        playbackStatus->playbackMode = ZIYAN_CAMERA_PLAYBACK_MODE_PAUSE;
    } else if (playbackInfo->isInPlayProcess) {
        playbackStatus->playbackMode = ZIYAN_CAMERA_PLAYBACK_MODE_PLAY;
    } else {
        playbackStatus->playbackMode = ZIYAN_CAMERA_PLAYBACK_MODE_STOP;
    }

    playbackStatus->playPosMs = playbackInfo->playPosMs;
    playbackStatus->videoLengthMs = playbackInfo->videoLengthMs;

    if (playbackInfo->videoLengthMs != 0) {
        playbackStatus->videoPlayProcess =
            (playbackInfo->videoLengthMs - playbackInfo->playPosMs) / playbackInfo->videoLengthMs;
    } else {
        playbackStatus->videoPlayProcess = 0;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanPlayback_GetVideoLengthMs(const char *filePath, uint32_t *videoLengthMs)
{
    FILE *fp;
    T_ZiyanReturnCode returnCode;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    float hour, minute, second;
    char tempTailStr[128];
    int ret;

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffmpeg -i \"%s\" 2>&1 | grep \"Duration\"", filePath);
    fp = popen(ffmpegCmdStr, "r");

    if (fp == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    ret = fscanf(fp, "  Duration: %f:%f:%f,%127s", &hour, &minute, &second, tempTailStr);
    if (ret <= 0) {
        USER_LOG_ERROR("MP4 File Get Duration Error\n");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        goto out;
    }

    *videoLengthMs = (uint32_t) ((hour * 3600 + minute * 60 + second) * 1000);
    returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

out:
    pclose(fp);

    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_StartPlayProcess(const char *filePath, uint32_t playPosMs)
{
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_TestPayloadCameraPlaybackCommand mediaPlayCommand = {0};
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    mediaPlayCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START;
    mediaPlayCommand.timeMs = playPosMs;

    if (strlen(filePath) >= sizeof(mediaPlayCommand.path)) {
        USER_LOG_ERROR("File path is too long.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }
    memcpy(mediaPlayCommand.path, filePath, strlen(filePath));

    if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex lock error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
        UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &mediaPlayCommand,
                       sizeof(T_TestPayloadCameraPlaybackCommand));
    } else {
        USER_LOG_ERROR("Media playback command buffer is full.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }

    if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex unlock error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    osalHandler->SemaphorePost(s_mediaPlayWorkSem);
    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_StopPlayProcess(void)
{
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    playbackCommand.command = TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP;

    if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex lock error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (UtilBuffer_GetUnusedSize(&s_mediaPlayCommandBufferHandler) >= sizeof(T_TestPayloadCameraPlaybackCommand)) {
        UtilBuffer_Put(&s_mediaPlayCommandBufferHandler, (const uint8_t *) &playbackCommand,
                       sizeof(T_TestPayloadCameraPlaybackCommand));
    } else {
        USER_LOG_ERROR("Media playback command buffer is full.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
    }

    if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("mutex unlock error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    osalHandler->SemaphorePost(s_mediaPlayWorkSem);
    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_VideoFileTranscode(const char *inPath, const char *outFormat, char *outPath,
                                                      uint16_t outPathBufferSize)
{
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_ZiyanReturnCode ziyanStatus = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    char *directory = NULL;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    directory = osalHandler->Malloc(ZIYAN_FILE_PATH_SIZE_MAX);
    if (directory == NULL) {
        USER_LOG_ERROR("malloc memory for directory fail.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    ziyanStatus = ZiyanUserUtil_GetCurrentFileDirPath(inPath, ZIYAN_FILE_PATH_SIZE_MAX, directory);
    if (ziyanStatus != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get directory of file error: 0x%08llX.", ziyanStatus);
        returnCode = ziyanStatus;
        goto out;
    }

    snprintf(outPath, outPathBufferSize, "%sout.%s", directory, outFormat);
    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE,
             "echo \"y\" | ffmpeg -i \"%s\" -codec copy -f \"%s\" \"%s\" 1>/dev/null 2>&1", inPath,
             outFormat, outPath);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute transcode command fail.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        goto out;
    }

    pclose(fpCommand);

out:
    osalHandler->Free(directory);

    return returnCode;
}

static T_ZiyanReturnCode
ZiyanPlayback_GetFrameInfoOfVideoFile(const char *path, T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                    uint32_t frameInfoBufferCount, uint32_t *frameCount)
{
    long ret;
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE];
    char *frameInfoString = NULL;
    char *frameLocation = NULL;
    char frameParameterFormat[50] = {0};
    char *frameDurationTimeLocation = NULL;
    float frameDurationTimeS = 0;
    uint32_t frameNumber = 0;
    char *framePositionLocation = NULL;
    uint32_t framePosition = 0;
    char *frameSizeLocation = NULL;
    uint32_t frameSize = 0;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    frameInfoString = osalHandler->Malloc(VIDEO_FRAME_MAX_COUNT * 1024);
    if (frameInfoString == NULL) {
        USER_LOG_ERROR("malloc memory for frame info fail.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }
    memset(frameInfoString, 0, VIDEO_FRAME_MAX_COUNT * 1024);

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffprobe -show_packets \"%s\" 2>/dev/null", path);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute show frames commands fail.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        goto out2;
    }

    ret = (long) fread(frameInfoString, 1, VIDEO_FRAME_MAX_COUNT * 1024, fpCommand);
    if (ret < 0) {
        USER_LOG_ERROR("read show frames commands result error.");
        goto out1;
    }
    frameInfoString[ret] = '\0';

    frameLocation = frameInfoString;
    *frameCount = 0;
    while (1) {
        // find frame
        frameLocation = strstr(frameLocation, s_frameKeyChar);
        if (frameLocation == NULL) {
            USER_LOG_DEBUG("reach file tail.");
            break;
        }

        // find frame duration
        frameDurationTimeLocation = strstr(frameLocation, s_frameDurationTimeKeyChar);
        if (frameDurationTimeLocation == NULL) {
            USER_LOG_ERROR("can not find pkt_duration_time.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%f", s_frameDurationTimeKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(frameDurationTimeLocation, frameParameterFormat, &frameDurationTimeS);
        if (ret <= 0) {
            USER_LOG_ERROR("can not find pkt_duration_time.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].durationS = frameDurationTimeS;

        // find frame position
        framePositionLocation = strstr(frameLocation, s_framePositionKeyChar);
        if (framePositionLocation == NULL) {
            USER_LOG_ERROR("can not found pkt_pos.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%d", s_framePositionKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(framePositionLocation, frameParameterFormat, &framePosition);
        if (ret <= 0) {
            USER_LOG_ERROR("can not found pkt_pos.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].positionInFile = framePosition;

        // find frame size
        frameSizeLocation = strstr(frameLocation, s_frameSizeKeyChar);
        if (frameSizeLocation == NULL) {
            USER_LOG_ERROR("can not found pkt_size.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }

        ret = snprintf(frameParameterFormat, sizeof(frameParameterFormat), "%s=%%d", s_frameSizeKeyChar);
        if (ret < 0) {
            USER_LOG_ERROR("snprintf frameParameterFormat fail.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
            goto out1;
        }

        ret = sscanf(frameSizeLocation, frameParameterFormat, &frameSize);
        if (ret <= 0) {
            USER_LOG_ERROR("can not find pkt_size.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
            goto out1;
        }
        frameInfo[frameNumber].size = frameSize;

        frameLocation += strlen(s_frameKeyChar);
        frameNumber++;
        (*frameCount)++;

        if (frameNumber >= frameInfoBufferCount) {
            USER_LOG_ERROR("frame buffer is full.");
            returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE;
            goto out1;
        }
    }

out1:
    pclose(fpCommand);

out2:
    osalHandler->Free(frameInfoString);

    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_GetFrameRateOfVideoFile(const char *path, float *frameRate)
{
    int ret;
    T_ZiyanReturnCode returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    FILE *fpCommand = NULL;
    char ffmpegCmdStr[FFMPEG_CMD_BUF_SIZE] = {0};
    int frameRateMolecule = 0;
    int frameRateDenominator = 0;

    snprintf(ffmpegCmdStr, FFMPEG_CMD_BUF_SIZE, "ffprobe -show_streams \"%s\" 2>/dev/null | grep r_frame_rate", path);
    fpCommand = popen(ffmpegCmdStr, "r");
    if (fpCommand == NULL) {
        USER_LOG_ERROR("execute show frame rate command fail.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ret = fscanf(fpCommand, "r_frame_rate=%d/%d", &frameRateMolecule, &frameRateDenominator);
    if (ret <= 0) {
        USER_LOG_ERROR("can not find frame rate.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
        goto out;
    }
    *frameRate = (float) frameRateMolecule / (float) frameRateDenominator;

out:
    pclose(fpCommand);

    return returnCode;
}

static T_ZiyanReturnCode ZiyanPlayback_GetFrameNumberByTime(T_TestPayloadCameraVideoFrameInfo *frameInfo,
                                                        uint32_t frameCount, uint32_t *frameNumber, uint32_t timeMs)
{
    uint32_t i = 0;
    double camulativeTimeS = 0;
    double timeS = (double) timeMs / 1000.0;

    for (i = 0; i < frameCount; ++i) {
        camulativeTimeS += frameInfo[i].durationS;

        if (camulativeTimeS >= timeS) {
            *frameNumber = i;
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
}

static T_ZiyanReturnCode GetMediaFileDir(char *dirPath)
{
    T_ZiyanReturnCode returnCode;
    char curFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX];
    char tempPath[ZIYAN_FILE_PATH_SIZE_MAX];

    returnCode = ZiyanUserUtil_GetCurrentFileDirPath(__FILE__, sizeof(curFileDirPath), curFileDirPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    snprintf(dirPath, ZIYAN_FILE_PATH_SIZE_MAX + 32, "%smedia_file", curFileDirPath);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMediaFileOriginData(const char *filePath, uint32_t offset, uint32_t length, uint8_t *data)
{
    T_ZiyanReturnCode returnCode;
    uint32_t realLen = 0;
    T_ZiyanMediaFileHandle mediaFileHandle;

    returnCode = ZiyanMediaFile_CreateHandle(filePath, &mediaFileHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetDataOrg(mediaFileHandle, offset, length, data, &realLen);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get data error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_DestroyHandle(mediaFileHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode CreateMediaFileThumbNail(const char *filePath)
{
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanMediaFile_CreateHandle(filePath, &s_mediaFileThumbNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_CreateThm(s_mediaFileThumbNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create thumb nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMediaFileThumbNailInfo(const char *filePath, T_ZiyanCameraMediaFileInfo *fileInfo)
{
    T_ZiyanReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_GetMediaFileType(s_mediaFileThumbNailHandle, &fileInfo->type);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetMediaFileAttr(s_mediaFileThumbNailHandle, &fileInfo->mediaFileAttr);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetFileSizeThm(s_mediaFileThumbNailHandle, &fileInfo->fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMediaFileThumbNailData(const char *filePath, uint32_t offset, uint32_t length, uint8_t *data)
{
    T_ZiyanReturnCode returnCode;
    uint16_t realLen = 0;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_GetDataThm(s_mediaFileThumbNailHandle, offset, length, data, &realLen);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get data error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode DestroyMediaFileThumbNail(const char *filePath)
{
    T_ZiyanReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileThumbNailHandle == NULL) {
        USER_LOG_ERROR("Media file thumb nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_DestoryThm(s_mediaFileThumbNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy thumb nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_DestroyHandle(s_mediaFileThumbNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode CreateMediaFileScreenNail(const char *filePath)
{
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanMediaFile_CreateHandle(filePath, &s_mediaFileScreenNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_CreateScr(s_mediaFileScreenNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file create screen nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMediaFileScreenNailInfo(const char *filePath, T_ZiyanCameraMediaFileInfo *fileInfo)
{
    T_ZiyanReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_GetMediaFileType(s_mediaFileScreenNailHandle, &fileInfo->type);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get type error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetMediaFileAttr(s_mediaFileScreenNailHandle, &fileInfo->mediaFileAttr);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get attr error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_GetFileSizeScr(s_mediaFileScreenNailHandle, &fileInfo->fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode GetMediaFileScreenNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                  uint8_t *data)
{
    T_ZiyanReturnCode returnCode;
    uint16_t realLen = 0;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_GetDataScr(s_mediaFileScreenNailHandle, offset, length, data, &realLen);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file get size error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode DestroyMediaFileScreenNail(const char *filePath)
{
    T_ZiyanReturnCode returnCode;

    USER_UTIL_UNUSED(filePath);

    if (s_mediaFileScreenNailHandle == NULL) {
        USER_LOG_ERROR("Media file screen nail handle null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    returnCode = ZiyanMediaFile_DestroyScr(s_mediaFileScreenNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy screen nail error stat:0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanMediaFile_DestroyHandle(s_mediaFileScreenNailHandle);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file destroy handle error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode DeleteMediaFile(char *filePath)
{
    T_ZiyanReturnCode returnCode;

    USER_LOG_INFO("delete media file:%s", filePath);
    returnCode = UtilFile_Delete(filePath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Media file delete error stat:0x%08llX", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetMediaPlaybackFile(const char *filePath)
{
    USER_LOG_INFO("set media playback file:%s", filePath);
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanPlayback_StopPlay(&s_playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    returnCode = ZiyanPlayback_SetPlayFile(&s_playbackInfo, filePath, 0);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    returnCode = ZiyanPlayback_StartPlay(&s_playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StartMediaPlayback(void)
{
    T_ZiyanReturnCode returnCode;

    USER_LOG_INFO("start media playback");
    returnCode = ZiyanPlayback_StartPlay(&s_playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start media playback status error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_ZiyanReturnCode StopMediaPlayback(void)
{
    T_ZiyanReturnCode returnCode;

    USER_LOG_INFO("stop media playback");
    returnCode = ZiyanPlayback_StopPlay(&s_playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_ZiyanReturnCode PauseMediaPlayback(void)
{
    T_ZiyanReturnCode returnCode;

    USER_LOG_INFO("pause media playback");
    returnCode = ZiyanPlayback_PausePlay(&s_playbackInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("pause media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_ZiyanReturnCode SeekMediaPlayback(uint32_t playbackPosition)
{
    T_ZiyanReturnCode returnCode;

    USER_LOG_INFO("seek media playback:%d", playbackPosition);
    returnCode = ZiyanPlayback_SeekPlay(&s_playbackInfo, playbackPosition);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("seek media playback error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    return returnCode;
}

static T_ZiyanReturnCode GetMediaPlaybackStatus(T_ZiyanCameraPlaybackStatus *status)
{
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanPlayback_GetPlaybackStatus(&s_playbackInfo, status);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get playback status error, stat:0x%08llX", returnCode);
        return returnCode;
    }

    status->videoPlayProcess = (uint8_t) (((float) s_playbackInfo.playPosMs / (float) s_playbackInfo.videoLengthMs) *
                                          100);

    USER_LOG_DEBUG("get media playback status %d %d %d %d", status->videoPlayProcess, status->playPosMs,
                   status->videoLengthMs, status->playbackMode);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StartDownloadNotification(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportion = {0};

    USER_LOG_DEBUG("media download start notification.");

    bandwidthProportion.dataStream = 0;
    bandwidthProportion.videoStream = 0;
    bandwidthProportion.downloadStream = 100;

    returnCode = ZiyanHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportion);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set bandwidth proportion for high speed channel error, stat:0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StopDownloadNotification(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanDataChannelBandwidthProportionOfHighspeedChannel bandwidthProportion = {0};

    USER_LOG_DEBUG("media download stop notification.");

    bandwidthProportion.dataStream = 10;
    bandwidthProportion.videoStream = 60;
    bandwidthProportion.downloadStream = 30;

    returnCode = ZiyanHighSpeedDataChannel_SetBandwidthProportion(bandwidthProportion);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set bandwidth proportion for high speed channel error, stat:0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserCameraMedia_SendVideoTask(void *arg)
{
    int ret;
    T_ZiyanReturnCode returnCode;
    static uint32_t sendVideoStep = 0;
    FILE *fpFile = NULL;
    unsigned long dataLength = 0;
    uint16_t lengthOfDataToBeSent = 0;
    int lengthOfDataHaveBeenSent = 0;
    char *dataBuffer = NULL;
    T_TestPayloadCameraPlaybackCommand playbackCommand = {0};
    uint16_t bufferReadSize = 0;
    char *videoFilePath = NULL;
    char *transcodedFilePath = NULL;
    float frameRate = 1.0f;
    uint32_t waitDuration = 1000 / SEND_VIDEO_TASK_FREQ;
    uint32_t rightNow = 0;
    uint32_t sendExpect = 0;
    T_TestPayloadCameraVideoFrameInfo *frameInfo = NULL;
    uint32_t frameNumber = 0;
    uint32_t frameCount = 0;
    uint32_t startTimeMs = 0;
    bool sendVideoFlag = true;
    bool sendOneTimeFlag = false;
    T_ZiyanDataChannelState videoStreamState = {0};
    E_ZiyanCameraMode mode = ZIYAN_CAMERA_MODE_SHOOT_PHOTO;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    uint32_t frameBufSize = 0;
    E_ZiyanCameraVideoStreamType videoStreamType;
    char curFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX];
    char tempPath[ZIYAN_FILE_PATH_SIZE_MAX];

    USER_UTIL_UNUSED(arg);

    returnCode = ZiyanUserUtil_GetCurrentFileDirPath(__FILE__, ZIYAN_FILE_PATH_SIZE_MAX, curFileDirPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        exit(1);
    }
    if (s_isMediaFileDirPathConfigured == true) {
        snprintf(tempPath, ZIYAN_FILE_PATH_SIZE_MAX, "%sPSDK_0005.h264", s_mediaFileDirPath);
    } else {
        snprintf(tempPath, ZIYAN_FILE_PATH_SIZE_MAX, "%smedia_file/PSDK_0005.h264", curFileDirPath);
    }

    videoFilePath = osalHandler->Malloc(ZIYAN_FILE_PATH_SIZE_MAX);
    if (videoFilePath == NULL) {
        USER_LOG_ERROR("malloc memory for video file path fail.");
        exit(1);
    }

    transcodedFilePath = osalHandler->Malloc(ZIYAN_FILE_PATH_SIZE_MAX);
    if (transcodedFilePath == NULL) {
        USER_LOG_ERROR("malloc memory for transcoded file path fail.");
        exit(1);
    }

    frameInfo = osalHandler->Malloc(VIDEO_FRAME_MAX_COUNT * sizeof(T_TestPayloadCameraVideoFrameInfo));
    if (frameInfo == NULL) {
        USER_LOG_ERROR("malloc memory for frame info fail.");
        exit(1);
    }
    memset(frameInfo, 0, VIDEO_FRAME_MAX_COUNT * sizeof(T_TestPayloadCameraVideoFrameInfo));

    returnCode = ZiyanPlayback_StopPlayProcess();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("stop playback and start liveview error: 0x%08llX.", returnCode);
        exit(1);
    }

    (void)osalHandler->GetTimeMs(&rightNow);
    sendExpect = rightNow + waitDuration;
    while (1) {
        (void)osalHandler->GetTimeMs(&rightNow);
        if (sendExpect > rightNow) {
            waitDuration = sendExpect - rightNow;
        } else {
            waitDuration = 1000 / SEND_VIDEO_TASK_FREQ;
        }
        (void)osalHandler->SemaphoreTimedWait(s_mediaPlayWorkSem, waitDuration);

        // response playback command
        if (osalHandler->MutexLock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex lock error");
            continue;
        }

        bufferReadSize = UtilBuffer_Get(&s_mediaPlayCommandBufferHandler, (uint8_t *) &playbackCommand,
                                        sizeof(T_TestPayloadCameraPlaybackCommand));

        if (osalHandler->MutexUnlock(s_mediaPlayCommandBufferMutex) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("mutex unlock error");
            continue;
        }

        if (bufferReadSize != sizeof(T_TestPayloadCameraPlaybackCommand))
            goto send;

        switch (playbackCommand.command) {
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_STOP:
                snprintf(videoFilePath, ZIYAN_FILE_PATH_SIZE_MAX, "%s", tempPath);
                startTimeMs = 0;
                sendVideoFlag = true;
                sendOneTimeFlag = false;
                break;
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_PAUSE:
                sendVideoFlag = false;
                goto send;
            case TEST_PAYLOAD_CAMERA_MEDIA_PLAY_COMMAND_START:
                snprintf(videoFilePath, ZIYAN_FILE_PATH_SIZE_MAX, "%s", playbackCommand.path);
                startTimeMs = playbackCommand.timeMs;
                sendVideoFlag = true;
                sendOneTimeFlag = true;
                break;
            default:
                USER_LOG_ERROR("playback command invalid: %d.", playbackCommand.command);
                sendVideoFlag = false;
                goto send;
        }

        // video send preprocess
        returnCode = ZiyanPlayback_VideoFileTranscode(videoFilePath, "h264", transcodedFilePath,
                                                    ZIYAN_FILE_PATH_SIZE_MAX);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("transcode video file error: 0x%08llX.", returnCode);
            continue;
        }

        returnCode = ZiyanPlayback_GetFrameRateOfVideoFile(transcodedFilePath, &frameRate);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get frame rate of video error: 0x%08llX.", returnCode);
            continue;
        }

        returnCode = ZiyanPlayback_GetFrameInfoOfVideoFile(transcodedFilePath, frameInfo, VIDEO_FRAME_MAX_COUNT,
                                                         &frameCount);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get frame info of video error: 0x%08llX.", returnCode);
            continue;
        }

        returnCode = ZiyanPlayback_GetFrameNumberByTime(frameInfo, frameCount, &frameNumber,
                                                      startTimeMs);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get start frame number error: 0x%08llX.", returnCode);
            continue;
        }

        if (fpFile != NULL)
            fclose(fpFile);

        fpFile = fopen(transcodedFilePath, "rb+");
        if (fpFile == NULL) {
            USER_LOG_ERROR("open video file fail.");
            continue;
        }

        send:
            if (fpFile == NULL) {
                USER_LOG_ERROR("open video file fail.");
                continue;
            }

            if (sendVideoFlag != true)
                continue;

            returnCode = ZiyanTest_CameraGetMode(&mode);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                continue;
            }

            returnCode = ZiyanTest_CameraGetVideoStreamType(&videoStreamType);
            if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                continue;
            }

            if (mode == ZIYAN_CAMERA_MODE_PLAYBACK && s_playbackInfo.isInPlayProcess == false) {
                continue;
            }

            frameBufSize = frameInfo[frameNumber].size;
            if (videoStreamType == ZIYAN_CAMERA_VIDEO_STREAM_TYPE_H264_ZIYAN_FORMAT) {
                frameBufSize = frameBufSize + VIDEO_FRAME_AUD_LEN;
            }

            dataBuffer = calloc(frameBufSize, 1);
            if (dataBuffer == NULL) {
                USER_LOG_ERROR("malloc fail.");
                goto free;
            }

            ret = fseek(fpFile, frameInfo[frameNumber].positionInFile, SEEK_SET);
            if (ret != 0) {
                USER_LOG_ERROR("fseek fail.");
                goto free;
            }

            dataLength = fread(dataBuffer, 1, frameInfo[frameNumber].size, fpFile);
            if (dataLength != frameInfo[frameNumber].size) {
                USER_LOG_ERROR("read data from video file error.");
            } else {
                USER_LOG_DEBUG("read data from video file success, len = %d B\r\n", dataLength);
            }

            if (videoStreamType == ZIYAN_CAMERA_VIDEO_STREAM_TYPE_H264_ZIYAN_FORMAT) {
                memcpy(&dataBuffer[frameInfo[frameNumber].size], s_frameAudInfo, VIDEO_FRAME_AUD_LEN);
                dataLength = dataLength + VIDEO_FRAME_AUD_LEN;
            }

            lengthOfDataHaveBeenSent = 0;
            while (dataLength - lengthOfDataHaveBeenSent) {
                lengthOfDataToBeSent = USER_UTIL_MIN(DATA_SEND_FROM_VIDEO_STREAM_MAX_LEN,
                                                    dataLength - lengthOfDataHaveBeenSent);
                printf("send video stream, len = %d B\r\n", lengthOfDataToBeSent);
                returnCode = ZiyanPayloadCamera_SendVideoStream((const uint8_t *) dataBuffer + lengthOfDataHaveBeenSent,
                                                            lengthOfDataToBeSent);
                if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("send video stream error: 0x%08llX.", returnCode);
                }
                lengthOfDataHaveBeenSent += lengthOfDataToBeSent;
            }

            (void)osalHandler->GetTimeMs(&sendExpect);
            sendExpect += (1000 / frameRate);

            if ((frameNumber++) >= frameCount) {
                USER_LOG_DEBUG("reach file tail.");
                frameNumber = 0;

                if (sendOneTimeFlag == true)
                    sendVideoFlag = false;
            }

            returnCode = ZiyanPayloadCamera_GetVideoStreamState(&videoStreamState);
            if (returnCode == ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_DEBUG(
                    "video stream state: realtimeBandwidthLimit: %d, realtimeBandwidthBeforeFlowController: %d, realtimeBandwidthAfterFlowController:%d busyState: %d.",
                    videoStreamState.realtimeBandwidthLimit, videoStreamState.realtimeBandwidthBeforeFlowController,
                    videoStreamState.realtimeBandwidthAfterFlowController,
                    videoStreamState.busyState);
            } else {
                USER_LOG_ERROR("get video stream state error.");
            }

        free:
            free(dataBuffer);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

//#endif

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

/**
 ********************************************************************
 * @file    test_widget_speaker.c
 * @brief
 *
 * @copyright (c) 2018 ZIYAN. All rights reserved.
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
#include "test_widget_speaker.h"
#include "ziyan_logger.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "utils/util_misc.h"
#include "utils/util_md5.h"
#include <ziyan_aircraft_info.h>

#ifdef OPUS_INSTALLED

#include <opus/opus.h>

#endif

#ifdef ALSA_INSTALLED

#include <alsa/asoundlib.h>

#endif

/* Private constants ---------------------------------------------------------*/
#define WIDGET_SPEAKER_TASK_STACK_SIZE          (2048)

/*! Attention: replace your audio device name here. */
// #define WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME    "alsa_output.usb-C-Media_Electronics_Inc._USB_Audio_Device-00.analog-stereo"
#define WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME    "alsa_card.usb-GeneralPlus_USB_Audio_Device-00"

#define WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME     "test_audio.opus"
#define WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME      "test_audio.pcm"

#define WIDGET_SPEAKER_TTS_FILE_NAME            "test_tts.txt"
#define WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME     "tts_audio.wav"
#define WIDGET_SPEAKER_TTS_FILE_MAX_SIZE        (3000)

/* The frame size is hardcoded for this sample code but it doesn't have to be */
#define WIDGET_SPEAKER_AUDIO_OPUS_MAX_PACKET_SIZE          (3 * 1276)
#define WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE           (6 * 960)
#define WIDGET_SPEAKER_AUDIO_OPUS_SAMPLE_RATE              (16000)
#define WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS                 (1)

#define WIDGET_SPEAKER_AUDIO_OPUS_DECODE_FRAME_SIZE_8KBPS  (40)
#define WIDGET_SPEAKER_AUDIO_OPUS_DECODE_BITRATE_8KBPS     (8000)

/* The speaker initialization parameters */
#define WIDGET_SPEAKER_DEFAULT_VOLUME                (60)
#define EKHO_INSTALLED                               (1)

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_ZiyanWidgetSpeakerHandler s_speakerHandler = {0};
static T_ZiyanMutexHandle s_speakerMutex = {0};
static T_ZiyanWidgetSpeakerState s_speakerState = {0};
static T_ZiyanTaskHandle s_widgetSpeakerTestThread;

static FILE *s_audioFile = NULL;
static FILE *s_ttsFile = NULL;
static bool s_isDecodeFinished = true;
static uint16_t s_decodeBitrate = 0;

#ifdef ALSA_INSTALLED
    snd_pcm_t           *pcm_handle = NULL;
    snd_pcm_hw_params_t *hw_params  = NULL;
    snd_pcm_sw_params_t *sw_params  = NULL;
#endif

/* Private functions declaration ---------------------------------------------*/
static void SetSpeakerState(E_ZiyanWidgetSpeakerState speakerState);
static T_ZiyanReturnCode GetSpeakerState(T_ZiyanWidgetSpeakerState *speakerState);
static T_ZiyanReturnCode SetWorkMode(E_ZiyanWidgetSpeakerWorkMode workMode);
static T_ZiyanReturnCode SetPlayMode(E_ZiyanWidgetSpeakerPlayMode playMode);
static T_ZiyanReturnCode StartPlay(void);
static T_ZiyanReturnCode StopPlay(void);
static T_ZiyanReturnCode SetVolume(uint8_t volume);
static T_ZiyanReturnCode ReceiveTtsData(E_ZiyanWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size);
static T_ZiyanReturnCode ReceiveAudioData(E_ZiyanWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size);
#ifdef SYSTEM_ARCH_LINUX
static void *ZiyanTest_WidgetSpeakerTask(void *arg);
static uint32_t ZiyanTest_GetVoicePlayProcessId(void);
static uint32_t ZiyanTest_KillVoicePlayProcess(uint32_t pid);
static T_ZiyanReturnCode ZiyanTest_DecodeAudioData(void);
static T_ZiyanReturnCode ZiyanTest_PlayAudioData(void);
static T_ZiyanReturnCode ZiyanTest_PlayTtsData(void);
static T_ZiyanReturnCode ZiyanTest_CheckFileMd5Sum(const char *path, uint8_t *buf, uint16_t size);
#endif

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_WidgetSpeakerStartService(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    s_speakerHandler.GetSpeakerState = GetSpeakerState;
    s_speakerHandler.SetWorkMode = SetWorkMode;
    s_speakerHandler.StartPlay = StartPlay;
    s_speakerHandler.StopPlay = StopPlay;
    s_speakerHandler.SetPlayMode = SetPlayMode;
    s_speakerHandler.SetVolume = SetVolume;
    s_speakerHandler.ReceiveTtsData = ReceiveTtsData;
    s_speakerHandler.ReceiveVoiceData = ReceiveAudioData;

    returnCode = osalHandler->MutexCreate(&s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Create speaker mutex error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = ZiyanWidget_RegSpeakerHandler(&s_speakerHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Register speaker handler error: 0x%08llX", returnCode);
        return returnCode;
    }

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    s_speakerState.state = ZIYAN_WIDGET_SPEAKER_STATE_IDEL;
    s_speakerState.workMode = ZIYAN_WIDGET_SPEAKER_WORK_MODE_VOICE;
    s_speakerState.playMode = ZIYAN_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    returnCode = SetVolume(WIDGET_SPEAKER_DEFAULT_VOLUME);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set speaker volume error: 0x%08llX", returnCode);
        return returnCode;
    }

#ifdef SYSTEM_ARCH_LINUX
    if (osalHandler->TaskCreate("user_widget_speaker_task", ZiyanTest_WidgetSpeakerTask, WIDGET_SPEAKER_TASK_STACK_SIZE,
                                NULL,
                                &s_widgetSpeakerTestThread) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Ziyan widget speaker test task create error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
#endif

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifdef ALSA_INSTALLED
T_ZiyanReturnCode ZiyanTest_AsoundInit()
{
    // 打开默认音频设备
    if (snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf(stderr, "无法打开音频设备\n");
        return 1;
    }

    // 分配硬件参数结构体
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(pcm_handle, hw_params);

    // 设置硬件参数
    snd_pcm_hw_params_set_access(pcm_handle,   hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle,   hw_params, SND_PCM_FORMAT_S16_LE);  // 16-bit little-endian
    snd_pcm_hw_params_set_rate(pcm_handle,     hw_params, WIDGET_SPEAKER_AUDIO_OPUS_SAMPLE_RATE, 0); // 设置采样率为 16kHz
    snd_pcm_hw_params_set_channels(pcm_handle, hw_params, WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS); // 设置单声道
    snd_pcm_hw_params_set_buffer_size(pcm_handle, hw_params, 16000 * 8);
    snd_pcm_hw_params_set_period_size(pcm_handle, hw_params, 1280, 0);

    // 应用硬件参数
    if (snd_pcm_hw_params(pcm_handle, hw_params) < 0) {
        fprintf(stderr, "无法设置硬件参数\n");
        return 1;
    }

    // 设置软件参数
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(pcm_handle, sw_params);
    snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, 1280);
    snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, 0);

    // 应用软件参数
    if (snd_pcm_sw_params(pcm_handle, sw_params) < 0) {
        fprintf(stderr, "无法设置软件参数\n");
        return 1;
    }

    // snd_pcm_prepare(pcm_handle);

    return 0;
}
#endif


T_ZiyanReturnCode ZiyanTest_PcmPlay(uint8_t* pcm_data, uint64_t size)
{
#ifdef ALSA_INSTALLED
    if((pcm_handle != NULL)){
        int err = snd_pcm_writei(pcm_handle, pcm_data, size);

        snd_pcm_sframes_t avail = snd_pcm_avail(pcm_handle);
        if((16000 * 8 - avail) <= 16000){

            uint8_t pcm_empty[16000];
            memset(pcm_empty, 0, sizeof(pcm_empty));
            snd_pcm_writei(pcm_handle, pcm_empty, sizeof(pcm_empty));
        }
        // snd_pcm_sframes_t avail = 0;
        printf("play pcm %d : %ld : %ld\n", err, size, avail);

        // 错误处理
        if (err == -EPIPE) {
            fprintf(stderr, "发生缓冲区溢出错误！\n");
            snd_pcm_prepare(pcm_handle);
        } else if (err < 0) {
            fprintf(stderr, "播放失败: %s\n", snd_strerror(err));
        } else if (err < 640) {
            fprintf(stderr, "警告: 写入的帧数少于预期\n");
        }
    }
#endif
}

/* Private functions definition-----------------------------------------------*/
#ifdef SYSTEM_ARCH_LINUX

static uint32_t ZiyanTest_GetVoicePlayProcessId(void)
{
    FILE *fp;
    char cmdStr[128];
    uint32_t pid;
    int ret;

    snprintf(cmdStr, 128, "pgrep ffplay");
    fp = popen(cmdStr, "r");
    if (fp == NULL) {
        USER_LOG_ERROR("fp is null.");
        return 0;
    }

    ret = fscanf(fp, "%u", &pid);
    if (ret <= 0) {
        pid = 0;
        goto out;
    }

out:
    pclose(fp);

    return pid;
}

static uint32_t ZiyanTest_KillVoicePlayProcess(uint32_t pid)
{
    FILE *fp;
    char cmdStr[128];

    snprintf(cmdStr, 128, "kill %d", pid);
    fp = popen(cmdStr, "r");
    if (fp == NULL) {
        USER_LOG_ERROR("fp is null.");
        return 0;
    }

    pclose(fp);

    return pid;
}

static T_ZiyanReturnCode ZiyanTest_DecodeAudioData(void)
{
#ifdef OPUS_INSTALLED
    FILE *fin;
    FILE *fout;
    OpusDecoder *decoder;
    opus_int16 out[WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS];
    uint8_t cbits[WIDGET_SPEAKER_AUDIO_OPUS_MAX_PACKET_SIZE];
    int32_t nbBytes;
    int32_t err;

    /*! Attention: you can use "ffmpeg -i xxx.mp3 -ar 16000 -ac 1 out.wav" and use opus-tools to generate opus file for test */
    fin = fopen(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, "r");
    if (fin == NULL) {
        fprintf(stderr, "failed to open input file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* Create a new decoder state. */
    decoder = opus_decoder_create(WIDGET_SPEAKER_AUDIO_OPUS_SAMPLE_RATE, WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS, &err);
    if (err < 0) {
        fprintf(stderr, "failed to create decoder: %s\n", opus_strerror(err));
        goto close_fin;
    }

    fout = fopen(WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME, "a+");
    if (fout == NULL) {
        fprintf(stderr, "failed to open output file: %s\n", strerror(errno));
        goto close_fin;
    }

    while (1) {
        int i;
        unsigned char pcm_bytes[WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS * 4] = {0};
        int frame_size;

        /* Read a 16 bits/sample audio frame. */
        nbBytes = fread(cbits, 1, s_decodeBitrate / WIDGET_SPEAKER_AUDIO_OPUS_DECODE_BITRATE_8KBPS *
                                  WIDGET_SPEAKER_AUDIO_OPUS_DECODE_FRAME_SIZE_8KBPS * 8, fin);

        /* Decode the data. In this example, frame_size will be constant because
           the encoder is using a constant frame size. However, that may not
           be the case for all encoders, so the decoder must always check
           the frame size returned. */
        frame_size = opus_decode(decoder, cbits, nbBytes, out, WIDGET_SPEAKER_AUDIO_OPUS_MAX_FRAME_SIZE, 0);
        if (frame_size < 0) {
            fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
            goto close_fout;
        }

        /* Convert to little-endian ordering. */
        for (i = 0; i < WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS * frame_size; i++) {
            pcm_bytes[2 * i] = out[i] & 0xFF;
            pcm_bytes[2 * i + 1] = (out[i] >> 8) & 0xFF;
        }
        /* Write the decoded audio to file. */
        fwrite(pcm_bytes, sizeof(short), frame_size * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS, fout);
#ifdef ALSA_INSTALLED
        ZiyanTest_PcmPlay(pcm_bytes, frame_size * WIDGET_SPEAKER_AUDIO_OPUS_CHANNELS);
#endif

        if (feof(fin))
            break;
    }

    USER_LOG_INFO("Decode Finished...");
    s_isDecodeFinished = true;

decode_data_failed:
    opus_decoder_destroy(decoder);
create_decoder_failed:
    fclose(fout);
open_pcm_audio_failed:
    fclose(fin);
#endif
    return EXIT_SUCCESS;

#ifdef OPUS_INSTALLED
close_fout:
    fclose(fout);

close_fin:
    fclose(fin);

    return EXIT_FAILURE;
#endif

}

static T_ZiyanReturnCode ZiyanTest_PlayAudioData(void)
{
    char cmdStr[128];

    memset(cmdStr, 0, sizeof(cmdStr));
    USER_LOG_INFO("Start Playing...");

    snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 -f s16le -i %s 2>/dev/null",
             WIDGET_SPEAKER_AUDIO_PCM_FILE_NAME);

    return ZiyanUserUtil_RunSystemCmd(cmdStr);
}

static T_ZiyanReturnCode ZiyanTest_PlayTtsData(void)
{
    FILE *txtFile;
    uint8_t data[WIDGET_SPEAKER_TTS_FILE_MAX_SIZE] = {0};
    int32_t readLen;
    char cmdStr[WIDGET_SPEAKER_TTS_FILE_MAX_SIZE + 128];
    T_ZiyanAircraftInfoBaseInfo aircraftInfoBaseInfo;
    T_ZiyanReturnCode returnCode;

    returnCode = ZiyanAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (aircraftInfoBaseInfo.aircraftType == ZIYAN_AIRCRAFT_TYPE_SHADOW_PLUS ||
        aircraftInfoBaseInfo.aircraftType == ZIYAN_AIRCRAFT_TYPE_SHADOW_MAX) {
        return ZiyanTest_PlayAudioData();
    } else {
        txtFile = fopen(WIDGET_SPEAKER_TTS_FILE_NAME, "r");
        if (txtFile == NULL) {
            USER_LOG_ERROR("failed to open input file: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        readLen = fread(data, 1, WIDGET_SPEAKER_TTS_FILE_MAX_SIZE - 1, txtFile);
        if (readLen <= 0) {
            USER_LOG_ERROR("Read tts file failed, error code: %d", readLen);
            fclose(txtFile);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND;
        }

        data[readLen] = '\0';

        fclose(txtFile);

        USER_LOG_INFO("Read tts file success, len: %d", readLen);
        USER_LOG_INFO("Content: %s", data);

        memset(cmdStr, 0, sizeof(cmdStr));

        SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_IN_TTS_CONVERSION);

#if EKHO_INSTALLED
        /*! Attention: you can use other tts opensource function to convert txt to speech, example used ekho v7.5 */
        snprintf(cmdStr, sizeof(cmdStr), " ekho %s -s 20 -p 20 -a 100 -o %s", data,
                 WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME);
#else
        USER_LOG_WARN(
        "Ekho is not installed, please visit https://www.eguidedog.net/ekho.php to install it or use other TTS tools to convert audio");
#endif
        ZiyanUserUtil_RunSystemCmd(cmdStr);

        SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_PLAYING);
        USER_LOG_INFO("Start TTS Playing...");
        memset(cmdStr, 0, sizeof(cmdStr));
        snprintf(cmdStr, sizeof(cmdStr), "ffplay -nodisp -autoexit -ar 16000 -ac 1 -f s16le -i %s 2>/dev/null",
                 WIDGET_SPEAKER_TTS_OUTPUT_FILE_NAME);

        return ZiyanUserUtil_RunSystemCmd(cmdStr);
    }
}

static T_ZiyanReturnCode ZiyanTest_CheckFileMd5Sum(const char *path, uint8_t *buf, uint16_t size)
{
    MD5_CTX md5Ctx;
    uint32_t readFileTotalSize = 0;
    uint16_t readLen;
    T_ZiyanReturnCode returnCode;
    uint8_t readBuf[1024] = {0};
    uint8_t md5Sum[16] = {0};
    FILE *file = NULL;;

    UtilMd5_Init(&md5Ctx);

    file = fopen(path, "rb");
    if (file == NULL) {
        USER_LOG_ERROR("Open tts file error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    while (1) {
        returnCode = fseek(file, readFileTotalSize, SEEK_SET);
        if (returnCode != 0) {
            USER_LOG_INFO("fseek file error");
        }

        readLen = fread(readBuf, 1, sizeof(readBuf), file);
        if (readLen > 0) {
            readFileTotalSize += readLen;
            UtilMd5_Update(&md5Ctx, readBuf, readLen);
        }

        if (feof(file))
            break;
    }

    UtilMd5_Final(&md5Ctx, md5Sum);
    fclose(file);

    if (size == sizeof(md5Sum)) {
        if (memcmp(md5Sum, buf, sizeof(md5Sum)) == 0) {
            USER_LOG_INFO("MD5 sum check success");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
        } else {
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    } else {
        USER_LOG_ERROR("MD5 sum length error");
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#endif

static void SetSpeakerState(E_ZiyanWidgetSpeakerState speakerState)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
    }

    s_speakerState.state = speakerState;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
    }
}

static T_ZiyanReturnCode GetSpeakerState(T_ZiyanWidgetSpeakerState *speakerState)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    *speakerState = s_speakerState;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetWorkMode(E_ZiyanWidgetSpeakerWorkMode workMode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Set widget speaker work mode: %d", workMode);
    s_speakerState.workMode = workMode;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetPlayMode(E_ZiyanWidgetSpeakerPlayMode playMode)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Set widget speaker play mode: %d", playMode);
    s_speakerState.playMode = playMode;

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StartPlay(void)
{
    uint32_t pid;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

#ifdef SYSTEM_ARCH_LINUX
    pid = ZiyanTest_GetVoicePlayProcessId();
    if (pid != 0) {
        ZiyanTest_KillVoicePlayProcess(pid);
    }
#endif

    osalHandler->TaskSleepMs(5);
    USER_LOG_INFO("Start widget speaker play");
    SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_PLAYING);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode StopPlay(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    uint32_t pid;

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    USER_LOG_INFO("Stop widget speaker play");
    s_speakerState.state = ZIYAN_WIDGET_SPEAKER_STATE_IDEL;

#ifdef SYSTEM_ARCH_LINUX
    pid = ZiyanTest_GetVoicePlayProcessId();
    if (pid != 0) {
        ZiyanTest_KillVoicePlayProcess(pid);
    }
#endif

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode SetVolume(uint8_t volume)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    char cmdStr[128];
    int32_t ret = 0;
    float realVolume;

    returnCode = osalHandler->MutexLock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("lock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    realVolume = 1.5f * (float) volume;
    s_speakerState.volume = volume;

    USER_LOG_INFO("Set widget speaker volume: %d", volume);

#ifdef PLATFORM_ARCH_x86_64
    snprintf(cmdStr, sizeof(cmdStr), "pactl list | grep %s -q", WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME);
    ret = system(cmdStr);
    if (ret == ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        memset(cmdStr, 0, sizeof(cmdStr));
        snprintf(cmdStr, sizeof(cmdStr), "pactl set-sink-volume %s %d%%", WIDGET_SPEAKER_USB_AUDIO_DEVICE_NAME,
                 (int32_t) realVolume);

        returnCode = ZiyanUserUtil_RunSystemCmd(cmdStr);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Set widget speaker volume error: %d", ret);
        }
    } else {
        USER_LOG_WARN("No audio device found, please add audio device and init speaker volume here.");
    }
#else
    USER_LOG_WARN("No audio device found, please add audio device and init speaker volume here!!!");
#endif

    returnCode = osalHandler->MutexUnlock(s_speakerMutex);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("unlock mutex error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ReceiveTtsData(E_ZiyanWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size)
{
    uint16_t writeLen;
    T_ZiyanReturnCode returnCode;

    if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_START) {
        USER_LOG_INFO("Create tts file.");
#ifdef SYSTEM_ARCH_LINUX
        s_ttsFile = fopen(WIDGET_SPEAKER_TTS_FILE_NAME, "wb");
        if (s_ttsFile == NULL) {
            USER_LOG_ERROR("Open tts file error.");
        }
#endif
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT) {
        USER_LOG_INFO("Transmit tts file, offset: %d, size: %d", offset, size);
#ifdef SYSTEM_ARCH_LINUX
        if (s_ttsFile != NULL) {
            fseek(s_ttsFile, offset, SEEK_SET);
            writeLen = fwrite(buf, 1, size, s_ttsFile);
            if (writeLen != size) {
                USER_LOG_ERROR("Write tts file error %d", writeLen);
            }
        }
#endif
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_FINISH) {
        USER_LOG_INFO("Close tts file.");
#ifdef SYSTEM_ARCH_LINUX
        if (s_ttsFile != NULL) {
            fclose(s_ttsFile);
            s_ttsFile = NULL;
        }

        returnCode = ZiyanTest_CheckFileMd5Sum(WIDGET_SPEAKER_TTS_FILE_NAME, buf, size);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("File md5 sum check failed");
        }
#endif
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_IDEL);
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ReceiveAudioData(E_ZiyanWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size)
{
    uint16_t writeLen;
    T_ZiyanReturnCode returnCode;
    T_ZiyanWidgetTransDataContent transDataContent = {0};

    if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_START) {
        s_isDecodeFinished = false;
#ifdef SYSTEM_ARCH_LINUX
        USER_LOG_INFO("Create voice file.");
        s_audioFile = fopen(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, "wb");
        if (s_audioFile == NULL) {
            USER_LOG_ERROR("Create tts file error.");
        }
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
#endif

        memcpy(&transDataContent, buf, size);
        s_decodeBitrate = transDataContent.transDataStartContent.fileDecodeBitrate;
        USER_LOG_INFO("Create voice file: %s, decoder bitrate: %d.", transDataContent.transDataStartContent.fileName,
                      transDataContent.transDataStartContent.fileDecodeBitrate);
    } else if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT) {
        USER_LOG_INFO("Transmit voice file, offset: %d, size: %d", offset, size);
#ifdef SYSTEM_ARCH_LINUX
        if (s_audioFile != NULL) {
            fseek(s_audioFile, offset, SEEK_SET);
            writeLen = fwrite(buf, 1, size, s_audioFile);
            if (writeLen != size) {
                USER_LOG_ERROR("Write tts file error %d", writeLen);
            }
        }
#endif
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_TRANSMITTING);
        }
    } else if (event == ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_FINISH) {
        USER_LOG_INFO("Close voice file.");
        if (s_audioFile != NULL) {
            fclose(s_audioFile);
            s_audioFile = NULL;
        }

#ifdef SYSTEM_ARCH_LINUX
        returnCode = ZiyanTest_CheckFileMd5Sum(WIDGET_SPEAKER_AUDIO_OPUS_FILE_NAME, buf, size);
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("File md5 sum check failed");
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
#endif
        if (s_speakerState.state != ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            SetSpeakerState(ZIYAN_WIDGET_SPEAKER_STATE_IDEL);
        }
#ifdef SYSTEM_ARCH_LINUX
        ZiyanTest_DecodeAudioData();
#endif
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifdef SYSTEM_ARCH_LINUX
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *ZiyanTest_WidgetSpeakerTask(void *arg)
{
    T_ZiyanReturnCode ziyanReturnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

#ifdef ALSA_INSTALLED
    ZiyanTest_AsoundInit();
#endif

    USER_UTIL_UNUSED(arg);

    while (1) {
        osalHandler->TaskSleepMs(10);

        if (s_speakerState.state == ZIYAN_WIDGET_SPEAKER_STATE_PLAYING) {
            if (s_speakerState.playMode == ZIYAN_WIDGET_SPEAKER_PLAY_MODE_LOOP_PLAYBACK) {
                if (s_speakerState.workMode == ZIYAN_WIDGET_SPEAKER_WORK_MODE_VOICE) {
                    USER_LOG_DEBUG("Waiting opus decoder finished...");
                    while (s_isDecodeFinished == false) {
                        osalHandler->TaskSleepMs(1);
                    }
                    ziyanReturnCode = ZiyanTest_PlayAudioData();
                    if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        USER_LOG_ERROR("Play audio data failed, error: 0x%08llX.", ziyanReturnCode);
                    }
                } else {
                    ziyanReturnCode = ZiyanTest_PlayTtsData();
                    if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        USER_LOG_ERROR("Play tts data failed, error: 0x%08llX.", ziyanReturnCode);
                    }
                }
                osalHandler->TaskSleepMs(1000);
            } else {
                if (s_speakerState.workMode == ZIYAN_WIDGET_SPEAKER_WORK_MODE_VOICE) {
                    USER_LOG_DEBUG("Waiting opus decoder finished...");
                    while (s_isDecodeFinished == false) {
                        osalHandler->TaskSleepMs(1);
                    }
                    ziyanReturnCode = ZiyanTest_PlayAudioData();
                    if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        USER_LOG_ERROR("Play audio data failed, error: 0x%08llX.", ziyanReturnCode);
                    }
                } else {
                    ziyanReturnCode = ZiyanTest_PlayTtsData();
                    if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                        USER_LOG_ERROR("Play tts data failed, error: 0x%08llX.", ziyanReturnCode);
                    }
                }

                ziyanReturnCode = osalHandler->MutexLock(s_speakerMutex);
                if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("lock mutex error: 0x%08llX.", ziyanReturnCode);
                }

                if (s_speakerState.playMode == ZIYAN_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY) {
                    s_speakerState.state = ZIYAN_WIDGET_SPEAKER_STATE_IDEL;
                }

                ziyanReturnCode = osalHandler->MutexUnlock(s_speakerMutex);
                if (ziyanReturnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                    USER_LOG_ERROR("unlock mutex error: 0x%08llX.", ziyanReturnCode);
                }
            }
        }
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif
#endif

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

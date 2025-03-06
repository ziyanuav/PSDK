/**
 ********************************************************************
 * @file    ziyan_media_file_core.c
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
#include <string.h>
#include <ziyan_logger.h>

#include "ziyan_media_file_core.h"
#include "ziyan_media_file_jpg.h"
#include "ziyan_media_file_mp4.h"
#include "ziyan_platform.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Private values ------------------------------------------------------------*/
//@formatter:off
static const T_ZiyanMediaFileOptItem s_mediaFileOpt[] =
{
    //JPEG File Operation Item
    {
        ZIYAN_CAMERA_FILE_TYPE_JPEG ,
        ZiyanMediaFile_IsSupported_JPG,
        ZiyanMediaFile_GetAttrFunc_JPG,
        ZiyanMediaFile_GetDataOrigin_JPG,
        ZiyanMediaFile_GetFileSizeOrigin_JPG,
        ZiyanMediaFile_CreateThumbNail_JPG,
        ZiyanMediaFile_GetFileSizeThumbNail_JPG,
        ZiyanMediaFile_GetDataThumbNail_JPG,
        ZiyanMediaFile_DestroyThumbNail_JPG,
        ZiyanMediaFile_CreateScreenNail_JPG,
        ZiyanMediaFile_GetFileSizeScreenNail_JPG,
        ZiyanMediaFile_GetDataScreenNail_JPG,
        ZiyanMediaFile_DestroyScreenNail_JPG,
    },
    //MP4 File Operation Item
    {
        ZIYAN_CAMERA_FILE_TYPE_MP4 ,
        ZiyanMediaFile_IsSupported_MP4,
        ZiyanMediaFile_GetAttrFunc_MP4,
        ZiyanMediaFile_GetDataOrigin_MP4,
        ZiyanMediaFile_GetFileSizeOrigin_MP4,
        ZiyanMediaFile_CreateThumbNail_MP4,
        ZiyanMediaFile_GetFileSizeThumbNail_MP4,
        ZiyanMediaFile_GetDataThumbNail_MP4,
        ZiyanMediaFile_DestroyThumbNail_MP4,
        ZiyanMediaFile_CreateScreenNail_MP4,
        ZiyanMediaFile_GetFileSizeScreenNail_MP4,
        ZiyanMediaFile_GetDataScreenNail_MP4,
        ZiyanMediaFile_DestroyScreenNail_MP4,
    },
};
static const uint32_t s_mediaFileOptCount = sizeof (s_mediaFileOpt) / sizeof(T_ZiyanMediaFileOptItem);
//@formatter:on

/* Exported functions definition ---------------------------------------------*/
bool ZiyanMediaFile_IsSupported(const char *filePath)
{
    int i;

    for (i = 0; i < s_mediaFileOptCount; i++) {
        if (s_mediaFileOpt[i].isSupportedFunc(filePath) == true) {
            return true;
        }
    }

    return false;
}

T_ZiyanReturnCode ZiyanMediaFile_CreateHandle(const char *filePath, T_ZiyanMediaFileHandle *pMediaFileHandle)
{
    int optIndex;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    for (optIndex = 0; optIndex < s_mediaFileOptCount; optIndex++) {
        if (s_mediaFileOpt[optIndex].isSupportedFunc(filePath) == true) {
            break;
        }
    }

    if (optIndex == s_mediaFileOptCount) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *pMediaFileHandle = osalHandler->Malloc(sizeof(T_ZiyanMediaFile));
    if (*pMediaFileHandle == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    (*pMediaFileHandle)->filePath = osalHandler->Malloc(strlen(filePath) + 1);
    if ((*pMediaFileHandle)->filePath == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    (*pMediaFileHandle)->mediaFileOptItem = s_mediaFileOpt[optIndex];
    (*pMediaFileHandle)->mediaFileThm.privThm = NULL;
    (*pMediaFileHandle)->mediaFileScr.privScr = NULL;

    strcpy((*pMediaFileHandle)->filePath, filePath);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanMediaFile_DestroyHandle(T_ZiyanMediaFileHandle mediaFileHandle)
{
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    osalHandler->Free(mediaFileHandle->filePath);
    osalHandler->Free(mediaFileHandle);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanMediaFile_GetMediaFileType(T_ZiyanMediaFileHandle mediaFileHandle,
                                              E_ZiyanCameraMediaFileType *mediaFileType)
{
    *mediaFileType = mediaFileHandle->mediaFileOptItem.mediaFileType;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanMediaFile_GetMediaFileAttr(T_ZiyanMediaFileHandle mediaFileHandle,
                                              T_ZiyanCameraMediaFileAttr *mediaFileAttr)
{
    if (mediaFileHandle->mediaFileOptItem.getAttrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getAttrFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getAttrFunc(mediaFileHandle, mediaFileAttr);
}

T_ZiyanReturnCode ZiyanMediaFile_GetDataOrg(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint32_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataOrgFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataOrgFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataOrgFunc(mediaFileHandle, offset, len, data, realLen);
}

T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeOrg(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeOrgFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeOrgFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeOrgFunc(mediaFileHandle, fileSize);
}

T_ZiyanReturnCode ZiyanMediaFile_CreateThm(T_ZiyanMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.createThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle createThmFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.createThmFunc(mediaFileHandle);
}

T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeThm(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeThmFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeThmFunc(mediaFileHandle, fileSize);
}

T_ZiyanReturnCode ZiyanMediaFile_GetDataThm(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataThmFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataThmFunc(mediaFileHandle, offset, len, data, realLen);
}

T_ZiyanReturnCode ZiyanMediaFile_DestoryThm(T_ZiyanMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.destroyThmFunc == NULL) {
        USER_LOG_ERROR("Media file handle destroyThmFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.destroyThmFunc(mediaFileHandle);
}

T_ZiyanReturnCode ZiyanMediaFile_CreateScr(T_ZiyanMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.creatScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle creatScrFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.creatScrFunc(mediaFileHandle);
}

T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeScr(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t *fileSize)
{
    if (mediaFileHandle->mediaFileOptItem.getFileSizeScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getFileSizeScrFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getFileSizeScrFunc(mediaFileHandle, fileSize);
}

T_ZiyanReturnCode ZiyanMediaFile_GetDataScr(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen)
{
    if (mediaFileHandle->mediaFileOptItem.getDataScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle getDataScrFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.getDataScrFunc(mediaFileHandle, offset, len, data, realLen);
}

T_ZiyanReturnCode ZiyanMediaFile_DestroyScr(T_ZiyanMediaFileHandle mediaFileHandle)
{
    if (mediaFileHandle->mediaFileOptItem.destroyScrFunc == NULL) {
        USER_LOG_ERROR("Media file handle destroyScrFunc null error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return mediaFileHandle->mediaFileOptItem.destroyScrFunc(mediaFileHandle);
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

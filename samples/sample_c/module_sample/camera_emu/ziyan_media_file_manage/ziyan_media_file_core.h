/**
 ********************************************************************
 * @file    ziyan_media_file_core.h
 * @brief   This is the header file for "ziyan_media_file_core.c", defining the structure and
 * (exported) function prototypes.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PSDK_MEDIA_FILE_CORE_H
#define PSDK_MEDIA_FILE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <ziyan_typedef.h>
#include <ziyan_payload_camera.h>

/* Exported constants --------------------------------------------------------*/
#define PSDK_MEDIA_FILE_PATH_LEN_MAX           512             /*max file path len */
#define PSDK_MEDIA_DIR_PATH_LEN_MAX            256             /*max dir path len */

/* Exported types ------------------------------------------------------------*/
typedef struct {
    void *privThm;
} T_ZiyanMediaFileThm;

typedef struct {
    void *privScr;
} T_ZiyanMediaFileScr;

struct _ZiyanMediaFile;

typedef struct {
    E_ZiyanCameraMediaFileType mediaFileType;
    bool (*isSupportedFunc)(const char *filePath);

    T_ZiyanReturnCode (*getAttrFunc)(struct _ZiyanMediaFile *mediaFileHandle, T_ZiyanCameraMediaFileAttr *mediaFileAttr);

    T_ZiyanReturnCode (*getDataOrgFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint32_t *realLen);
    T_ZiyanReturnCode (*getFileSizeOrgFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);

    T_ZiyanReturnCode (*createThmFunc)(struct _ZiyanMediaFile *mediaFileHandle);
    T_ZiyanReturnCode (*getFileSizeThmFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);
    T_ZiyanReturnCode (*getDataThmFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint16_t *realLen);
    T_ZiyanReturnCode (*destroyThmFunc)(struct _ZiyanMediaFile *mediaFileHandle);

    T_ZiyanReturnCode (*creatScrFunc)(struct _ZiyanMediaFile *mediaFileHandle);
    T_ZiyanReturnCode (*getFileSizeScrFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);
    T_ZiyanReturnCode (*getDataScrFunc)(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                      uint8_t *data, uint16_t *realLen);
    T_ZiyanReturnCode (*destroyScrFunc)(struct _ZiyanMediaFile *mediaFileHandle);
} T_ZiyanMediaFileOptItem;

typedef struct _ZiyanMediaFile {
    char *filePath;
    T_ZiyanMediaFileOptItem mediaFileOptItem;
    T_ZiyanMediaFileThm mediaFileThm;
    T_ZiyanMediaFileScr mediaFileScr;
} T_ZiyanMediaFile, *T_ZiyanMediaFileHandle;

/* Exported functions --------------------------------------------------------*/
bool ZiyanMediaFile_IsSupported(const char *filePath);
T_ZiyanReturnCode ZiyanMediaFile_CreateHandle(const char *filePath, T_ZiyanMediaFileHandle *pMediaFileHandle);
T_ZiyanReturnCode ZiyanMediaFile_DestroyHandle(T_ZiyanMediaFileHandle mediaFileHandle);

T_ZiyanReturnCode ZiyanMediaFile_GetMediaFileType(T_ZiyanMediaFileHandle mediaFileHandle,
                                              E_ZiyanCameraMediaFileType *mediaFileType);
T_ZiyanReturnCode ZiyanMediaFile_GetMediaFileAttr(T_ZiyanMediaFileHandle mediaFileHandle,
                                              T_ZiyanCameraMediaFileAttr *mediaFileAttr);

T_ZiyanReturnCode ZiyanMediaFile_GetDataOrg(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint32_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeOrg(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);

T_ZiyanReturnCode ZiyanMediaFile_CreateThm(T_ZiyanMediaFileHandle mediaFileHandle);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeThm(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t *fileSize);
T_ZiyanReturnCode ZiyanMediaFile_GetDataThm(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_DestoryThm(T_ZiyanMediaFileHandle mediaFileHandle);

T_ZiyanReturnCode ZiyanMediaFile_CreateScr(T_ZiyanMediaFileHandle mediaFileHandle);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeScr(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t *fileSize);
T_ZiyanReturnCode ZiyanMediaFile_GetDataScr(T_ZiyanMediaFileHandle mediaFileHandle, uint32_t offset, uint16_t len,
                                        uint8_t *data, uint16_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_DestroyScr(T_ZiyanMediaFileHandle mediaFileHandle);

#ifdef __cplusplus
}
#endif

#endif // PSDK_MEDIA_FILE_CORE_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

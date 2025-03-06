/**
 ********************************************************************
 * @file    ziyan_media_file_jpg.h
 * @brief   This is the header file for "ziyan_media_file_jpg.c", defining the structure and
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
#ifndef PSDK_MEDIA_FILE_JPG_H
#define PSDK_MEDIA_FILE_JPG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <ziyan_payload_camera.h>
#include <ziyan_typedef.h>
#include "ziyan_media_file_core.h"

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
bool ZiyanMediaFile_IsSupported_JPG(const char *filePath);

T_ZiyanReturnCode ZiyanMediaFile_GetAttrFunc_JPG(struct _ZiyanMediaFile *mediaFileHandle,
                                             T_ZiyanCameraMediaFileAttr *mediaFileAttr);

T_ZiyanReturnCode ZiyanMediaFile_GetDataOrigin_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                               uint8_t *data, uint32_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeOrigin_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);

T_ZiyanReturnCode ZiyanMediaFile_CreateThumbNail_JPG(struct _ZiyanMediaFile *mediaFileHandle);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeThumbNail_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);
T_ZiyanReturnCode
ZiyanMediaFile_GetDataThumbNail_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                  uint8_t *data, uint16_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_DestroyThumbNail_JPG(struct _ZiyanMediaFile *mediaFileHandle);

T_ZiyanReturnCode ZiyanMediaFile_CreateScreenNail_JPG(struct _ZiyanMediaFile *mediaFileHandle);
T_ZiyanReturnCode ZiyanMediaFile_GetFileSizeScreenNail_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t *fileSize);
T_ZiyanReturnCode
ZiyanMediaFile_GetDataScreenNail_JPG(struct _ZiyanMediaFile *mediaFileHandle, uint32_t offset, uint16_t len,
                                   uint8_t *data, uint16_t *realLen);
T_ZiyanReturnCode ZiyanMediaFile_DestroyScreenNail_JPG(struct _ZiyanMediaFile *mediaFileHandle);

#ifdef __cplusplus
}
#endif

#endif // PSDK_MEIDA_FILE_JPG_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    osal_fs.h
 * @brief   This is the header file for "osal_fs.c", defining the structure and
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
#ifndef OSAL_FS_H
#define OSAL_FS_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode Osal_FileOpen(const char *fileName, const char *fileMode, T_ZiyanFileHandle *fileObj);

T_ZiyanReturnCode Osal_FileClose(T_ZiyanFileHandle fileObj);

T_ZiyanReturnCode Osal_FileWrite(T_ZiyanFileHandle fileObj, const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_ZiyanReturnCode Osal_FileRead(T_ZiyanFileHandle fileObj, uint8_t *buf, uint32_t len, uint32_t *realLen);

T_ZiyanReturnCode Osal_FileSeek(T_ZiyanFileHandle fileObj, uint32_t offset);

T_ZiyanReturnCode Osal_FileSync(T_ZiyanFileHandle fileObj);

T_ZiyanReturnCode Osal_DirOpen(const char *filePath, T_ZiyanDirHandle *dirObj);

T_ZiyanReturnCode Osal_DirClose(T_ZiyanDirHandle dirObj);

T_ZiyanReturnCode Osal_DirRead(T_ZiyanDirHandle dirObj, T_ZiyanFileInfo *fileInfo);

T_ZiyanReturnCode Osal_Mkdir(const char *filePath);

T_ZiyanReturnCode Osal_Unlink(const char *filePath);

T_ZiyanReturnCode Osal_Rename(const char *oldFilePath, const char *newFilePath);

T_ZiyanReturnCode Osal_Stat(const char *filePath, T_ZiyanFileInfo *fileInfo);

#ifdef __cplusplus
}
#endif

#endif // OSAL_FS_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

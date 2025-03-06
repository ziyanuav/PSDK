/**
 ********************************************************************
 * @file    util_file.h
 * @brief   This is the header file for "util_file.c", defining the structure and
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
#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SYSTEM_ARCH_LINUX

/* Includes ------------------------------------------------------------------*/
#include <ziyan_typedef.h>
#include <stdio.h>

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint32_t second: 5;
    uint32_t minute: 6;
    uint32_t hour: 5;
    uint32_t day: 5;
    uint32_t month: 4;
    uint32_t year: 7;
} T_UtilFileCreateTime;

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode UtilFile_GetCreateTime(const char *filePath, T_UtilFileCreateTime *createTime);
T_ZiyanReturnCode UtilFile_GetFileSizeByPath(const char *filePath, uint32_t *fileSize);
T_ZiyanReturnCode UtilFile_GetFileDataByPath(const char *filePath, uint32_t offset, uint32_t len,
                                           uint8_t *data, uint32_t *realLen);
T_ZiyanReturnCode UtilFile_Delete(const char *filePath);

T_ZiyanReturnCode UtilFile_GetFileSize(FILE *file, uint32_t *fileSize);
T_ZiyanReturnCode UtilFile_GetFileData(FILE *file, uint32_t offset, uint16_t len, uint8_t *data, uint16_t *realLen);

#ifdef __cplusplus
}
#endif

#endif

#endif // UTIL_FILE_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

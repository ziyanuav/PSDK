/**
 ********************************************************************
 * @file    util_misc.h
 * @brief   This is the header file for "util_misc.c", defining the structure and
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
#ifndef UTIL_MISC_H
#define UTIL_MISC_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define USER_UTIL_UNUSED(x)                                 ((x) = (x))
#define USER_UTIL_MIN(a, b)                                 (((a) < (b)) ? (a) : (b))
#define USER_UTIL_MAX(a, b)                                 (((a) > (b)) ? (a) : (b))
#define USER_UTIL_IS_WORK_TURN(step, workfreq, taskfreq)    (!((step) % (uint32_t) ((taskfreq) / (workfreq))))
#define UTIL_OFFSETOF(type, member)                         ((size_t) & ((type *)0 )-> member)
#define UTIL_ARRAY_SIZE(array)                              ((unsigned int) (sizeof(array) / sizeof((array)[0])))

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode ZiyanUserUtil_GetCurrentFileDirPath(const char *filePath, uint32_t pathBufferSize, char *dirPath);
void ZiyanUserUtil_PrintProgressBar(uint16_t currentProgress, uint16_t totalProgress, char *userData);
T_ZiyanReturnCode ZiyanUserUtil_RunSystemCmd(const char *systemCmdStr);

#ifdef __cplusplus
}
#endif

#endif // UTIL_MISC_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

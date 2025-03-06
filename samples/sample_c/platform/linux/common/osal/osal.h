/**
 ********************************************************************
 * @file    osal.h
 * @brief   This is the header file for "osal.c", defining the structure and
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
#ifndef OSAL_H
#define OSAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode Osal_TaskCreate(const char *name, void *(*taskFunc)(void *),
                                uint32_t stackSize, void *arg, T_ZiyanTaskHandle *task);
T_ZiyanReturnCode Osal_TaskDestroy(T_ZiyanTaskHandle task);
T_ZiyanReturnCode Osal_TaskSleepMs(uint32_t timeMs);

T_ZiyanReturnCode Osal_MutexCreate(T_ZiyanMutexHandle *mutex);
T_ZiyanReturnCode Osal_MutexDestroy(T_ZiyanMutexHandle mutex);
T_ZiyanReturnCode Osal_MutexLock(T_ZiyanMutexHandle mutex);
T_ZiyanReturnCode Osal_MutexUnlock(T_ZiyanMutexHandle mutex);

T_ZiyanReturnCode Osal_SemaphoreCreate(uint32_t initValue, T_ZiyanSemaHandle *semaphore);
T_ZiyanReturnCode Osal_SemaphoreDestroy(T_ZiyanSemaHandle semaphore);
T_ZiyanReturnCode Osal_SemaphoreWait(T_ZiyanSemaHandle semaphore);
T_ZiyanReturnCode Osal_SemaphoreTimedWait(T_ZiyanSemaHandle semaphore, uint32_t waitTime);
T_ZiyanReturnCode Osal_SemaphorePost(T_ZiyanSemaHandle semaphore);

T_ZiyanReturnCode Osal_GetTimeMs(uint32_t *ms);
T_ZiyanReturnCode Osal_GetTimeUs(uint64_t *us);
T_ZiyanReturnCode Osal_GetRandomNum(uint16_t *randomNum);

void *Osal_Malloc(uint32_t size);
void Osal_Free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // OSAL_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

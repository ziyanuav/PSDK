/**
 ********************************************************************
 * @file   osal.c
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
#include "osal.h"
#include "ziyan_typedef.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static uint32_t s_localTimeMsOffset = 0;
static uint64_t s_localTimeUsOffset = 0;

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/

/* Private functions definition-----------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/

T_ZiyanReturnCode Osal_TaskCreate(const char *name, void *(*taskFunc)(void *), uint32_t stackSize, void *arg,
                                T_ZiyanTaskHandle *task)
{
    int result;
    char nameDealed[16] = {0};

    *task = malloc(sizeof(pthread_t));
    if (*task == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    result = pthread_create(*task, NULL, taskFunc, arg);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (name != NULL)
        strncpy(nameDealed, name, sizeof(nameDealed) - 1);
    result = pthread_setname_np(*(pthread_t *) *task, nameDealed);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode Osal_TaskDestroy(T_ZiyanTaskHandle task)
{
    if (task == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }
    pthread_cancel(*(pthread_t *) task);
    free(task);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode Osal_TaskSleepMs(uint32_t timeMs)
{
    usleep(1000 * timeMs);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Declare the mutex container, initialize the mutex, and
 * create mutex ID.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_MutexCreate(T_ZiyanMutexHandle *mutex)
{
    int result;

    if (!mutex) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    *mutex = malloc(sizeof(pthread_mutex_t));
    if (*mutex == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    result = pthread_mutex_init(*mutex, NULL);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Delete the created mutex.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_MutexDestroy(T_ZiyanMutexHandle mutex)
{
    int result = 0;

    if (!mutex) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    result = pthread_mutex_destroy(mutex);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    free(mutex);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Acquire and lock the mutex when peripheral access is required
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_MutexLock(T_ZiyanMutexHandle mutex)
{
    int result = 0;

    if (!mutex) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    result = pthread_mutex_lock(mutex);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Unlock and release the mutex, when done with the peripheral access.
 * @param mutex:  pointer to the created mutex ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_MutexUnlock(T_ZiyanMutexHandle mutex)
{
    int result = 0;

    if (!mutex) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    result = pthread_mutex_unlock(mutex);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Declare the semaphore container, initialize the semaphore, and
 * create semaphore ID.
 * @param semaphore: pointer to the created semaphore ID.
 * @param initValue: initial value of semaphore.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_SemaphoreCreate(uint32_t initValue, T_ZiyanSemaHandle *semaphore)
{
    int result;

    *semaphore = malloc(sizeof(sem_t));
    if (*semaphore == NULL) {
        return
            ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    result = sem_init(*semaphore, 0, (unsigned int) initValue);
    if (result != 0) {
        return
            ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return
        ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Delete the created semaphore.
 * @param semaphore: pointer to the created semaphore ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_SemaphoreDestroy(T_ZiyanSemaHandle semaphore)
{
    int result;

    result = sem_destroy((sem_t *) semaphore);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    free(semaphore);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Wait the semaphore until token becomes available.
 * @param semaphore: pointer to the created semaphore ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_SemaphoreWait(T_ZiyanSemaHandle semaphore)
{
    int result;

    result = sem_wait(semaphore);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Wait the semaphore until token becomes available.
 * @param semaphore: pointer to the created semaphore ID.
 * @param waitTime: timeout value of waiting semaphore, unit: millisecond.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_SemaphoreTimedWait(T_ZiyanSemaHandle semaphore, uint32_t waitTime)
{
    int result;
    struct timespec semaphoreWaitTime;
    struct timeval systemTime;

    gettimeofday(&systemTime, NULL);

    systemTime.tv_usec += waitTime * 1000;
    if (systemTime.tv_usec >= 1000000) {
        systemTime.tv_sec += systemTime.tv_usec / 1000000;
        systemTime.tv_usec %= 1000000;
    }

    semaphoreWaitTime.tv_sec = systemTime.tv_sec;
    semaphoreWaitTime.tv_nsec = systemTime.tv_usec * 1000;

    result = sem_timedwait(semaphore, &semaphoreWaitTime);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Release the semaphore token.
 * @param semaphore: pointer to the created semaphore ID.
 * @return an enum that represents a status of PSDK
 */
T_ZiyanReturnCode Osal_SemaphorePost(T_ZiyanSemaHandle semaphore)
{
    int result;

    result = sem_post(semaphore);
    if (result != 0) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Get the system time for ms.
 * @return an uint32 that the time of system, uint:ms
 */
T_ZiyanReturnCode Osal_GetTimeMs(uint32_t *ms)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    *ms = (time.tv_sec * 1000 + time.tv_usec / 1000);

    if (s_localTimeMsOffset == 0) {
        s_localTimeMsOffset = *ms;
        *ms = *ms - s_localTimeMsOffset;
    } else {
        *ms = *ms - s_localTimeMsOffset;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode Osal_GetTimeUs(uint64_t *us)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    *us = (time.tv_sec * 1000000 + time.tv_usec);

    if (s_localTimeUsOffset == 0) {
        s_localTimeUsOffset = *us;
        *us = *us - s_localTimeMsOffset;
    } else {
        *us = *us - s_localTimeMsOffset;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode Osal_GetRandomNum(uint16_t *randomNum)
{
    srand(time(NULL));
    *randomNum = random() % 65535;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

void *Osal_Malloc(uint32_t size)
{
    return malloc(size);
}

void Osal_Free(void *ptr)
{
    free(ptr);
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

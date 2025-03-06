/**
 ********************************************************************
 * @file    ziyan_hms_manager.h
 * @brief   This is the header file for "ziyan_hms_manager.c", defining the structure and
 * (exported) function prototypes.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ZIYAN_HMS_MANAGER_H
#define ZIYAN_HMS_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint32_t errorCode;
    uint8_t componentIndex;
    uint8_t errorLevel;
} T_ZiyanHmsInfo;

typedef struct {
    T_ZiyanHmsInfo *hmsInfo;
    uint32_t hmsInfoNum;
} T_ZiyanHmsInfoTable;

typedef T_ZiyanReturnCode (*ZiyanHmsInfoCallback)(T_ZiyanHmsInfoTable hmsInfoTable);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise hms manager module, and user should call this function
 * before using hms manager features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsManager_Init(void);

/**
 * @brief DeInitialize hms manager module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsManager_DeInit(void);

/**
 * @brief Register callback to get hms info.
 * @note: Data is pushed with a frequency of 1Hz.
 * @param callback: see reference of ZiyanHmsInfoCallback.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsManager_RegHmsInfoCallback(ZiyanHmsInfoCallback callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_HMS_MANAGER_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    ziyan_power_management.h
 * @brief   This is the header file for "ziyan_power_management.c", defining the structure and
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
#ifndef ZIYAN_POWER_MANAGEMENT_H
#define ZIYAN_POWER_MANAGEMENT_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Pin state.
 */
typedef enum {
    ZIYAN_POWER_MANAGEMENT_PIN_STATE_RESET = 0, /*!< Specifies pin is in low level state. */
    ZIYAN_POWER_MANAGEMENT_PIN_STATE_SET = 1, /*!< Specifies pin is in high level state. */
} E_ZiyanPowerManagementPinState;

/**
 * @brief Prototype of callback function used to set level of high power application pin.
 * @param pinState: level state of pin to be set.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*ZiyanWriteHighPowerApplyPinCallback)(E_ZiyanPowerManagementPinState pinState);

/**
 * @brief Prototype of callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @warning User can not execute blocking style operations or functions in a callback function, because that will block ZIYAN
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param powerOffPreparationFlag: pointer to memory space used to store power off preparation flag. True represents
 * that payload has completed all work and is ready to power off. False represents that payload is not ready to power
 * off.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*ZiyanPowerOffNotificationCallback)(bool *powerOffPreparationFlag);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise power management module, and user should call this function before using power management features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPowerManagement_Init(void);

/**
 * @brief DeInitialise power management module, and user should call this function before using power management features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPowerManagement_DeInit(void);

/**
 * @brief Apply high power from aircraft in blocking mode.
 * @details Before applying, user should register callback function used to set level state of high power application
 * pin using ZiyanPowerManagement_RegWriteHighPowerApplyPinCallback() function. After applying high power, power pin of
 * ZIYAN adapter will output high power based predetermined specification.
 * @note Max execution time of this function is slightly larger than 600ms.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPowerManagement_ApplyHighPowerSync(void);

/**
 * @brief Register callback function used to set level state of high power application pin. Must be called before
 * applying high power.
 * @param callback: pointer to the callback function.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPowerManagement_RegWriteHighPowerApplyPinCallback(ZiyanWriteHighPowerApplyPinCallback callback);

/**
 * @brief Register callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @details After registering, the callback function will be called at a fixed frequency when aircraft will power off.
 * User fill in the power off preparation flag, and once the payload is ready to power off, the callback function will not
 * be called. After a specified time, if the payload is not ready to power off, the aircraft will power off immediately. The
 * specified time is 10s.
 * @param callback: pointer to callback function used to notify aircraft power off message and get payload power off
 * preparation flag.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPowerManagement_RegPowerOffNotificationCallback(ZiyanPowerOffNotificationCallback callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_POWER_MANAGEMENT_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

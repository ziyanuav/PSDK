/**
 ********************************************************************
 * @file    dij_core.h
 * @brief   This is the header file for "dij_core.c", defining the structure and
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
#ifndef ZIYAN_CORE_H
#define ZIYAN_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    char appName[32]; /*!< Specifies ZIYAN SDK app name. This info can be obtained by logging in to the
                           developer website https://developer.ziyan.com/user/apps/#all. End with '\0'. */
    char appId[16]; /*!< Specifies ZIYAN SDK app ID. This info can be obtained by logging in to the
                         developer website https://developer.ziyan.com/user/apps/#all. */
    char appKey[32]; /*!< Specifies ZIYAN SDK app key. This info can be obtained by logging in to the
                          developer website https://developer.ziyan.com/user/apps/#all. */
    char appLicense[512]; /*!< Specifies ZIYAN SDK app license. This info can be obtained by logging in to the
                               developer website https://developer.ziyan.com/user/apps/#all. */
    char developerAccount[64]; /*!< Specifies ZIYAN SDK developer account email. This info can be obtained by
                                    logging in to the developer website https://developer.ziyan.com/user/apps/#all.
                                    Developer's account and other related information need to be able to correspond.
                                    End with '\0'. */
    char baudRate[16]; /*!< Specifies ZIYAN SDK communication serial port baud rate. */
} T_ZiyanUserInfo;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the Payload SDK core in blocking mode.
 * @note The order of calling this call is crucial. It must be done after registering console/OSAL/HAL handler functions
 * It must be done after registering console/OSAL/HAL handler functions and before using other functional module
 * interfaces. Correctly fill in the developer information to ensure successful initialization. See the
 * See the "PSDK Initialization" tutorial for more.
 * This function does not return until the correct aircraft type and PSDK adapter type is obtained. The logic ensures
 * that aircraft and PSDK adapter have been started up normally before PSDK functional module and user's program run.
 * General execution time of this function is 2-4 seconds.
 * @param userInfo: pointer to the PSDK application information.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_Init(const T_ZiyanUserInfo *userInfo);

/**
 * @brief Sets an alias for a ZIYAN application or product that meets the condition for ZIYAN application or product.
 * If an alias exists, it will be displayed in ZIYAN Pilot.
 * @note Still need to pass in correct ZIYAN APP name that is obtained from ZIYAN SDK developer website to ZiyanCore_Init()
 * interface. The ZIYAN APP name will be used to bind and verification.
 * The alias will take effect after a short delay, up to a maximum of 1 second.
 * @param productAlias: A pointer to the product alias string, which must end with '\0'. The maximum length of the string is 31 characters.
 * If the alias string exceeds 31 characters, it will be truncated before being passed in.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_SetAlias(const char *productAlias);

/**
 * @brief Set custom firmware version for ZIYAN application or product.
 * @details Payload firmware version will always display in ZIYAN Pilot payload settings interface.
 * @param version: the custom firmware version to be set.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_SetFirmwareVersion(T_ZiyanFirmwareVersion version);

/**
 * @brief Set custom serial number for ZIYAN application or product.
 * @details Payload custom serial number will always display in ZIYAN Pilot payload settings interface.
 * @param productSerialNumber: the custom serial number to be set, needs to be no larger than 32 bytes.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_SetSerialNumber(const char *productSerialNumber);

/**
 * @brief Notify that the Payload SDK core application starts.
 * @note The order of calling this interface requires special attention, The call needs to be completed after all the
 * module initialize and register interfaces.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_ApplicationStart(void);

/**
 * @brief DeInitialize the Payload SDK core in blocking mode.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanCore_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif // DIJ_CORE_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

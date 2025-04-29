/**
 ********************************************************************
 * @file    ziyan_aircraft_info.h
 * @brief   This is the header file for "ziyan_aircraft_info.c", defining the structure and
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
#ifndef ZIYAN_AIRCRAFT_INFO_H
#define ZIYAN_AIRCRAFT_INFO_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Information related to mobile APP.
 */
typedef struct {
    E_ZiyanMobileAppLanguage appLanguage; /*!< Mobile APP system language */
    E_ZiyanMobileAppScreenType appScreenType; /*!< Mobile APP screen size type. */
} T_ZiyanMobileAppInfo;

/**
 * @brief Basic information about the aircraft system, mainly including some constant parameters information.
 */
typedef struct {
    E_ZiyanAircraftSeries aircraftSeries; /*!< Aircraft series. */
    E_ZiyanMountPositionType mountPositionType; /*!< Mount position type. */
    E_ZiyanAircraftType aircraftType; /*!< Aircraft type. */
    E_ZiyanSdkAdapterType ziyanAdapterType; /*!< ZIYAN adapter type. */
    E_ZiyanMountPosition mountPosition; /*!< Payload mount position. */
} T_ZiyanAircraftInfoBaseInfo;

/**
 * @brief Aircraft version information.
 */
typedef struct {
    uint8_t debugVersion;
    uint8_t modifyVersion;
    uint8_t minorVersion;
    uint8_t majorVersion;
} T_ZiyanAircraftVersion;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Basic information about the aircraft system, including aircraft type and ZIYAN adapter type.
 * @param baseInfo: Pointer to a memory space where the aircraft's basic information will be stored.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanAircraftInfo_GetBaseInfo(T_ZiyanAircraftInfoBaseInfo *baseInfo);

/**
 * @brief Get information related to mobile app.
 * @note Returns unknown for app language and screen type if the RC or app is not connected to the aircraft system.
 * @param mobileAppInfo: Pointer to a memory space where the mobile app information will be stored.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanAircraftInfo_GetMobileAppInfo(T_ZiyanMobileAppInfo *mobileAppInfo);

/**
 * @brief Get connection status between the payload and the aircraft.
 * @note Update period: 1Hz
 * @param isConnected: Pointer to connection status.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanAircraftInfo_GetConnectionStatus(bool *isConnected);

/**
 * @brief Get version of the aircraft.
 * @param aircraftVersion: Pointer to aircraft version.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanAircraftInfo_GetAircraftVersion(T_ZiyanAircraftVersion *aircraftVersion);

#ifdef __cplusplus
}
#endif

#endif //ZIYAN_AIRCRAFT_INFO_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

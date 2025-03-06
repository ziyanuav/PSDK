/**
 ********************************************************************
 * @file    ziyan_waypoint_v2.h
 * @brief   This is the header file for "ziyan_waypoint_v2.c", defining the structure and
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
#ifndef ZIYAN_WAYPOINT_V2_H
#define ZIYAN_WAYPOINT_V2_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"
#include "ziyan_waypoint_v2_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef T_ZiyanReturnCode (*WaypointV2EventCbFunc)(T_ZiyanWaypointV2MissionEventPush eventData);
typedef T_ZiyanReturnCode (*WaypointV2StateCbFunc)(T_ZiyanWaypointV2MissionStatePush stateData);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Initialise waypoint v2 module, and user should call this function
 * before using waypoint v2 features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Init(void);

/**
 * @brief Deinitialise waypoint v2 module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Deinit(void);

/**
 * @brief Init waypoint v2 mission settings
 * @param info: see reference of T_ZiyanWayPointV2MissionSettings.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_UploadMission(const T_ZiyanWayPointV2MissionSettings *info);

/**
 * @brief Start execute waypoint v2 mission
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Start(void);

/**
 * @brief Stop execute waypoint v2 mission
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Stop(void);

/**
 * @brief Pause execute waypoint v2 mission
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Pause(void);

/**
 * @brief Resume execute waypoint v2 mission
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_Resume(void);

/**
 * @brief Get the global cruise speed setting from flight controller
 * @param cruiseSpeed: see references of T_ZiyanWaypointV2GlobalCruiseSpeed.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_GetGlobalCruiseSpeed(T_ZiyanWaypointV2GlobalCruiseSpeed *cruiseSpeed);

/**
 * @brief Set the global cruise speed setting to flight controller
 * @param cruiseSpeed: see references of T_ZiyanWaypointV2GlobalCruiseSpeed.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_SetGlobalCruiseSpeed(T_ZiyanWaypointV2GlobalCruiseSpeed cruiseSpeed);

/**
 * @brief Subscribe to waypointV2 event with a callback function.
 * @note There will be data when the event is triggered.
 * @param callback: see references of WaypointV2EventCbFunc.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_RegisterMissionEventCallback(WaypointV2EventCbFunc callback);

/**
 * @brief Subscribe to waypointV2 mission state with a callback function.
 * @note Data is pushed with a frequency of 10Hz.
 * @param callback: see references of WaypointV2StateCbFunc.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV2_RegisterMissionStateCallback(WaypointV2StateCbFunc callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_WAYPOINT_V2_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    ziyan_waypoint_v3.h
 * @brief   This is the header file for "ziyan_waypoint_v3.c", defining the structure and
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
#ifndef ZIYAN_WAYPOINT_V3_H
#define ZIYAN_WAYPOINT_V3_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
* @brief Waypoint v3 mission action.
*/
typedef enum {
    ZIYAN_WAYPOINT_V3_ACTION_START = 0, /*!< Waypoint v3 mission start action. */
    ZIYAN_WAYPOINT_V3_ACTION_STOP = 1, /*!< Waypoint v3 mission stop action. */
    ZIYAN_WAYPOINT_V3_ACTION_PAUSE = 2, /*!< Waypoint v3 mission pause action. */
    ZIYAN_WAYPOINT_V3_ACTION_RESUME = 3, /*!< Waypoint v3 mission resume action. */
} E_ZiyanWaypointV3Action;

/**
*  Waypoint v3 current mission state.
*/
typedef enum {
    ZIYAN_WAYPOINT_V3_MISSION_STATE_IDLE = 0, /*!< Waypoint v3 mission in idle state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_PREPARE = 16, /*!< Waypoint v3 mission in prepare state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_TRANS_MISSION = 32, /*!< Waypoint v3 mission in trans mission state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_MISSION = 48, /*!< Waypoint v3 mission in mission state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_BREAK = 64, /*!< Waypoint v3 mission in break state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_RESUME = 80, /*!< Waypoint v3 mission in resume state. */
    ZIYAN_WAYPOINT_V3_MISSION_STATE_RETURN_FIRSTPOINT = 98, /*!< Waypoint v3 mission in return first point state. */
} E_ZiyanWaypointV3MissionState;

/**
*  Waypoint v3 current action state.
*/
typedef enum {
    ZIYAN_WAYPOINT_V3_ACTION_STATE_IDLE = 0, /*!< Waypoint v3 action in idle state. */
    ZIYAN_WAYPOINT_V3_ACTION_STATE_RUNNING = 1, /*!< Waypoint v3 action in idle state. */
    ZIYAN_WAYPOINT_V3_ACTION_STATE_FINISHED = 5, /*!< Waypoint v3 action in idle state. */
} E_ZiyanWaypointV3ActionState;

/**
*  Waypoint v3 mission state.
*/
typedef struct {
    E_ZiyanWaypointV3MissionState state; /*!< Waypoint v3 current mission state, #E_ZiyanWaypointV3MissionState. */
    uint32_t wayLineId; /*!< Waypoint v3 current way line id. */
    uint16_t currentWaypointIndex; /*!< Waypoint v3 current waypoint index. */
} T_ZiyanWaypointV3MissionState;

/**
*  Waypoint v3 action state.
*/
typedef struct {
    E_ZiyanWaypointV3ActionState state; /*!< Waypoint v3 current action state, #E_ZiyanWaypointV3ActionState. */
    uint32_t wayLineId; /*!< Waypoint v3 current way line id. */
    uint16_t currentWaypointIndex; /*!< Waypoint v3 current waypoint index. */
    uint16_t actionGroupId; /*!< Waypoint v3 current action group index. */
    uint16_t actionId; /*!< Waypoint v3 current action index. */
} T_ZiyanWaypointV3ActionState;

/**
 * @brief Prototype of callback function used to receive the waypoint v3 mission state.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param missionState: current waypoint v3 mission state.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*WaypointV3MissionStateCallback)(T_ZiyanWaypointV3MissionState missionState);

/**
 * @brief Prototype of callback function used to receive the waypoint v3 mission state.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param missionState: current waypoint v3 mission state.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*WaypointV3ActionStateCallback)(T_ZiyanWaypointV3ActionState missionState);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise waypoint v3 module, and user should call this function.
 * before using waypoint v3 features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_Init(void);

/**
 * @brief Deinitialise waypoint v3 module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_DeInit(void);

/**
 * @brief Upload kmz file by raw data.
 * @note The size of kmz file is very small, you can use this interface to upload quickly.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_UploadKmzFile(const uint8_t *data, uint32_t dataLen);

/**
 * @brief Execute the mission action.
 * @note This action should be called after uploading the kmz file.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_Action(E_ZiyanWaypointV3Action action);

/**
 * @brief Register the mission state callback for waypoint mission.
 * @note If you want to monitor the state of waypoint mission, this interface should be called before uploading kmz
 * file or executing this mission action.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_RegMissionStateCallback(WaypointV3MissionStateCallback callback);

/**
 * @brief Register the action state callback for waypoint mission.
 * @note If you want to monitor the state of waypoint action, this interface should be called before uploading kmz
 * file or executing this mission action.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWaypointV3_RegActionStateCallback(WaypointV3ActionStateCallback callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_WAYPOINT_V3_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

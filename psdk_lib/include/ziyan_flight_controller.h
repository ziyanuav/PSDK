/**
 ********************************************************************
 * @file    ziyan_flight_controller.h
 * @brief   This is the header file for "ziyan_flight_controller.c", defining the structure and
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
#ifndef ZIYAN_FLIGHT_CONTROLLER_H
#define ZIYAN_FLIGHT_CONTROLLER_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define EMERGENCY_STOP_MOTOR_MSG_MAX_LENGTH 10 /*!<  Max length of emergency stop motor message. */

/* Exported types ------------------------------------------------------------*/
typedef uint16_t E_ZiyanFlightControllerGoHomeAltitude; /*!< Unit:meter, range 20~500 */

/**
 * @brief The aircraft's actions when RC is lost.
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_RC_LOST_ACTION_HOVER = 0,  /*!< Aircraft will execute hover action when RC is lost. */
    ZIYAN_FLIGHT_CONTROLLER_RC_LOST_ACTION_LANDING = 1,  /*!< Aircraft will execute land action when RC is lost. */
    ZIYAN_FLIGHT_CONTROLLER_RC_LOST_ACTION_GOHOME = 2,  /*!< Aircraft will execute go-home action when RC is lost. */
} E_ZiyanFlightControllerRCLostAction;

/**
 * @brief Enable/Disable RTK position enum
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_DISABLE_RTK_POSITION = 0, /*!< 0: The aircraft will use GPS data instead of RTK data to execute
                                                     * actions which requires location information(waypoint, go home...)
                                                     */
    ZIYAN_FLIGHT_CONTROLLER_ENABLE_RTK_POSITION = 1, /*!< 1:The aircraft will use RTK data instead of GPS data to execute
                                                    * actions which requires location information(waypoint, go home...)*/
} E_ZiyanFlightControllerRtkPositionEnableStatus;

/**
 * @brief Enable/Disable obstacle sensing enum
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_DISABLE_OBSTACLE_AVOIDANCE = 0, /*!< 0: The aircraft will not perform obstacle sensing in
                                                           * the specified direction */
    ZIYAN_FLIGHT_CONTROLLER_ENABLE_OBSTACLE_AVOIDANCE = 1, /*!< 0: The aircraft will perform obstacle sensing in the
                                                          * specified direction */
} E_ZiyanFlightControllerObstacleAvoidanceEnableStatus;

/**
 * @brief Enable/Disable emergency stop motor function enum
 * @note Enable emergency-stop-motor function is very dangerous in the air. It will make the aircraft crash!!!
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_ENABLE_EMERGENCY_STOP_MOTOR = 0x01, /*!< Execute emergency-stop-motor action */
} E_ZiyanFlightControllerEmergencyStopMotor;

/**
 * @brief Obtain/Release joystick control permission command enum
 * @note You have obtained joystick control permission successfully before using joystick.
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_RELEASE_JOYSTICK_CTRL_AUTHORITY = 0, /*!< Obtain joystick permission */
    ZIYAN_FLIGHT_CONTROLLER_OBTAIN_JOYSTICK_CTRL_AUTHORITY = 1, /*!< Release joystick permission */
} E_ZiyanFlightControllerJoystickCtrlAuthorityAction;

/**
 * @brief The aircraft's joystick control permission owner enum
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_RC = 0,  /*!< RC could control aircraft with joystick. */
    ZIYAN_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_MSDK = 1, /*!< MSDK could control aircraft with joystick. */
    ZIYAN_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_INTERNAL = 2, /*!< Special Internal modules could control aircraft
                                                                 * with joystick. */
    ZIYAN_FLIGHT_CONTROLLER_JOYSTICK_CTRL_AUTHORITY_OSDK = 4, /*!< PSDK could control aircraft with joystick. */
} E_ZiyanFlightControllerJoystickCtrlAuthority;

/**
 * @brief The aircraft's joystick control permission switch reason enum
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_MSDK_GET_JOYSTICK_CTRL_AUTH_EVENT = 1, /*!< MSDK gets the joystick control permission. */
    ZIYAN_FLIGHT_CONTROLLER_INTERNAL_GET_JOYSTICK_CTRL_AUTH_EVENT = 2,  /*!< A specific internal modules gets the joystick control permission. */
    ZIYAN_FLIGHT_CONTROLLER_OSDK_GET_JOYSTICK_CTRL_AUTH_EVENT = 3, /*!< PSDK gets the joystick control permission. */
    ZIYAN_FLIGHT_CONTROLLER_RC_LOST_GET_JOYSTICK_CTRL_AUTH_EVENT = 4, /*!< Reset the joystick control permission to RC when executing RC lost action */
    ZIYAN_FLIGHT_CONTROLLER_RC_NOT_P_MODE_RESET_JOYSTICK_CTRL_AUTH_EVENT = 5,  /*!< Reset the joystick control permission to RC when RC is not in P mode */
    ZIYAN_FLIGHT_CONTROLLER_RC_SWITCH_MODE_GET_JOYSTICK_CTRL_AUTH_EVENT = 6,  /*!< Set the joystick control permission to RC when RC switches control mode(T/APS) */
    ZIYAN_FLIGHT_CONTROLLER_RC_PAUSE_GET_JOYSTICK_CTRL_AUTH_EVENT = 7, /*!< Reset the joystick control permission to RC when RC pauses */
    ZIYAN_FLIGHT_CONTROLLER_RC_REQUEST_GO_HOME_GET_JOYSTICK_CTRL_AUTH_EVENT = 8, /*!< Reset the joystick control permission to RC when RC requests to go home*/
    ZIYAN_FLIGHT_CONTROLLER_LOW_BATTERY_GO_HOME_RESET_JOYSTICK_CTRL_AUTH_EVENT = 9, /*!< Reset the joystick control permission to RC when aircraft is executing low-battery-go-home*/
    ZIYAN_FLIGHT_CONTROLLER_LOW_BATTERY_LANDING_RESET_JOYSTICK_CTRL_AUTH_EVENT = 10, /*!< Reset the joystick control permission to RC when aircraft is executing low-battery-landing*/
    ZIYAN_FLIGHT_CONTROLLER_OSDK_LOST_GET_JOYSTICK_CTRL_AUTH_EVENT = 11, /*!< Reset the joystick control permission to RC when PSDK is lost*/
    ZIYAN_FLIGHT_CONTROLLER_NERA_FLIGHT_BOUNDARY_RESET_JOYSTICK_CTRL_AUTH_EVENT = 12, /*!< Reset the joystick control permission to RC when aircraft is near boundary.*/
} E_ZiyanFlightControllerJoystickCtrlAuthoritySwitchEvent;

/**
 * @brief The aircraft's joystick control permission switch event info enum
 */
typedef struct {
    E_ZiyanFlightControllerJoystickCtrlAuthority curJoystickCtrlAuthority; /*!< The aircraft's joystick control permission owner */
    E_ZiyanFlightControllerJoystickCtrlAuthoritySwitchEvent joystickCtrlAuthoritySwitchEvent; /*!< The aircraft's joystick control permission switch reason */
} T_ZiyanFlightControllerJoystickCtrlAuthorityEventInfo;

/**
 * @brief Prototype of callback function used to get joystick control permission switch event info.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*JoystickCtrlAuthorityEventCbFunc)(
    T_ZiyanFlightControllerJoystickCtrlAuthorityEventInfo eventData);

/**
 * @brief Prototype of callback function used to get the trigger FTS event.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*TriggerFtsEventCallback)(void);

/**
 * @brief Horizon control mode in joystick mode enum
 * @note Only when the GPS signal is good (health_flag >=3)，horizontal position control (ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_POSITION_CONTROL_MODE)
 * related control modes can be used. Only when GPS signal is good (health_flag >=3)，or advanced sensing system is working properly with Autopilot，
 * horizontal velocity control（ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_VELOCITY_CONTROL_MODE) related control modes can be used.
 */
typedef enum {
    /**
     * @brief Control pitch & roll & angle of the aircraft.
     * @note Need to be referenced to either the ground or body frame by E_ZiyanFlightControllerHorizontalCoordinate setting.
     * Limit: -35 degree to 35 degree
     */
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_ANGLE_CONTROL_MODE = 0,
    /**
     * @brief Set the control mode to control horizontal vehicle velocities.
     * @note Need to be referenced to either the ground or body frame by E_ZiyanFlightControllerHorizontalCoordinate setting
     * Limit: -30m/s to 30 m/s
     */
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_VELOCITY_CONTROL_MODE = 1,
    /**
     * @brief Set the control mode to control position offsets of pitch & roll directions.
     * @note Need to be referenced to either the ground or body frame by E_ZiyanFlightControllerHorizontalCoordinate setting
     * Limit: N/A
     */
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_POSITION_CONTROL_MODE = 2,
    /**
     * @brief Set the control mode to control rate of change of the vehicle's attitude.
     * @note Need to be referenced to either the ground or body frame by E_ZiyanFlightControllerHorizontalCoordinate setting
     * Limit: -150deg/s to 150.0 deg/s
     */
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_ANGULAR_RATE_CONTROL_MODE = 3
} E_ZiyanFlightControllerHorizontalControlMode;

/**
 * @brief Vertical control mode enum in joystick mode
 * @note We don not recommend using VERTICAL_POSITION control mode indoors when the aircraft's flying height exceeds 3 meters.
 * This is because barometer can be inaccurate indoors, and the vertical controller may fail to keep the height of the aircraft.
 */
typedef enum {
    /**
     * @brief Set the control mode to control the vertical speed of aircraft, setting the upward as positive/
     * @note Limit: -5 m/s to 5 m/s
     */
    ZIYAN_FLIGHT_CONTROLLER_VERTICAL_VELOCITY_CONTROL_MODE = 0,

    /**
     * @brief Set the control mode to control the height of aircraft
     * @note Limit: 0 m to 120 m
     */
    ZIYAN_FLIGHT_CONTROLLER_VERTICAL_POSITION_CONTROL_MODE = 1,

    /**
     * @brief Set the control mode to directly control the thrust
     * @note Range: 0 % to 100 %
     */
    ZIYAN_FLIGHT_CONTROLLER_VERTICAL_THRUST_CONTROL_MODE = 2,
} E_ZiyanFlightControllerVerticalControlMode;

/**
 * @brief Yaw control mode enum in joystick mode
 */
typedef enum {
    /**
     * @brief Set the control mode to control yaw angle.
     * @note Yaw angle is referenced to the ground frame. In this control mode, Ground frame is enforced in Autopilot.
     */
    ZIYAN_FLIGHT_CONTROLLER_YAW_ANGLE_CONTROL_MODE = 0x00,

    /**
     * @brief Set the control-mode to control yaw angular velocity
     * @note Same reference frame as YAW_ANGLE.
     * Limit: -150 deg/s to 150 deg/s
     */
    ZIYAN_FLIGHT_CONTROLLER_YAW_ANGLE_RATE_CONTROL_MODE = 1
} E_ZiyanFlightControllerYawControlMode;

/**
 * @brief Horizontal coordinate enum in joystick mode
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_GROUND_COORDINATE = 0, /*!< Set the x-y of ground frame as the horizontal frame (NEU) */
    ZIYAN_FLIGHT_CONTROLLER_HORIZONTAL_BODY_COORDINATE = 1 /*!< Set the x-y of body frame as the horizontal frame (FRU) */
} E_ZiyanFlightControllerHorizontalCoordinate;

/*!
 * @brief Stable mode enum in joystick mode
 * @note Only works in horizontal velocity control mode. In velocity stable mode, aircraft will brake and hover at one position once
 * the input command is zero. In velocity non-stable mode, aircraft will follow the velocity command and not hover when the command is zero.
 * That means aircraft will drift with the wind.
 */
typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_STABLE_CONTROL_MODE_DISABLE = 0, /*!< Disable the stable mode */
    ZIYAN_FLIGHT_CONTROLLER_STABLE_CONTROL_MODE_ENABLE = 1   /*!< Enable the stable mode */
} E_ZiyanFlightControllerStableControlMode;

typedef enum {
    ZIYAN_FLIGHT_CONTROLLER_ENABLE_RC_LOST_ACTION = 0,
    ZIYAN_FLIGHT_CONTROLLER_DISABLE_RC_LOST_ACTION = 1,
} E_ZiyanFlightControllerRCLostActionEnableStatus;

/**
 * @brief Joystick mode.
 * @note You need to set joystick mode first before start to send joystick command to aircraft.
 */
typedef struct {
    E_ZiyanFlightControllerHorizontalControlMode horizontalControlMode; /*!< See reference of E_ZiyanFlightControllerHorizontalControlMode*/
    E_ZiyanFlightControllerVerticalControlMode verticalControlMode; /*!< See reference of E_ZiyanFlightControllerVerticalControlMode*/
    E_ZiyanFlightControllerYawControlMode yawControlMode; /*!< See reference of E_ZiyanFlightControllerYawControlMode*/
    E_ZiyanFlightControllerHorizontalCoordinate horizontalCoordinate; /*!< See reference of E_ZiyanFlightControllerHorizontalCoordinate*/
    E_ZiyanFlightControllerStableControlMode stableControlMode; /*!< See reference of E_ZiyanFlightControllerStableControlMode*/
} T_ZiyanFlightControllerJoystickMode;

#pragma pack(1)
typedef struct {
    ziyan_f32_t x;   /*!< Controls the x axis.*/
    ziyan_f32_t y;   /*!< Controls the y axis.*/
    ziyan_f32_t z;   /*!< Controls the z axis, setting upward as positive. */
    ziyan_f32_t yaw; /*!< Yaw position/velocity control w.r.t. the ground frame.*/
} T_ZiyanFlightControllerJoystickCommand;// pack(1)

typedef struct {
    ziyan_f64_t latitude;  /*!< unit: rad */
    ziyan_f64_t longitude; /*!< unit: rad */
} T_ZiyanFlightControllerHomeLocation; // pack(1)

typedef struct {
    char serialNum[32];
} T_ZiyanFlightControllerGeneralInfo;

typedef struct {
    ziyan_f64_t latitude;  /*!< unit: rad */
    ziyan_f64_t longitude; /*!< unit: rad */
    uint16_t altitude;
} T_ZiyanFlightControllerRidInfo;

#pragma pack()

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise flight controller module
 * @param ridInfo: Must report the correct RID information before using PSDK to control the aircraft.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_Init(T_ZiyanFlightControllerRidInfo ridInfo);

/**
 * @brief DeInitialise flight controller module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_DeInit(void);

/**
 * @brief Enable/Disable RTK position function.
 * @details Enabling RTK means that RTK data will be used instead of GPS during flight.
 * @param rtkEnableStatus: refer to "E_ZiyanFlightControllerRtkPositionEnableStatus", inheriting from Pilot.
 * @return Execution result.
 */
T_ZiyanReturnCode
ZiyanFlightController_SetRtkPositionEnableStatus(E_ZiyanFlightControllerRtkPositionEnableStatus rtkEnableStatus);

/**
 * @brief Get RTK enable status.
 * @note Enabling RTK means that RTK data will be used during intelligent flight.
 * @param rtkEnableStatus: refer to "E_ZiyanFlightControllerRtkPositionEnableStatus", inheriting from Pilot.
 * @return Execution result.
 */
T_ZiyanReturnCode
ZiyanFlightController_GetRtkPositionEnableStatus(E_ZiyanFlightControllerRtkPositionEnableStatus *rtkEnableStatus);

/**
 * @brief Set RC lost action.
 * @note Valid when RC and PSDK are both lost. It only supports M30.
 * @param rcLostAction: actions when RC is lost.(hover/landing/go home).It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetRCLostAction(E_ZiyanFlightControllerRCLostAction rcLostAction);

/**
 * @brief Get RC lost action(hover/landing/gohome).
 * @note Valid when RC and PSDK are both lost. It only supports M30.
 * @param rcLostAction: see reference of E_ZiyanFlightControllerRCLostAction.It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetRCLostAction(E_ZiyanFlightControllerRCLostAction *rcLostAction);

/**
 * @brief Enable/Disable horizontal visual(forwards,backwards,left,right) obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param horizontalObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetHorizontalVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus horizontalObstacleAvoidanceEnableStatus);

/**
 * @brief Get the switch status of horizontal visual(forwards,backwards,left,right) obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param horizontalObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetHorizontalVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus *horizontalObstacleAvoidanceEnableStatus);

/**
 * @brief Enable/Disable horizontal radar obstacle sensing.
 * @note It will be valid only if you install CSM radar successfully.For detailed parameters of obstacle sensing,
 * it is recommended to read the official user manual in https://www.ziyan.com.
 * @param horizontalObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetHorizontalRadarObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus horizontalObstacleAvoidanceEnableStatus);

/**
 * @brief Get the switch status of horizontal radar obstacle sensing.
 * @note It will be valid only if you install CSM radar successfully.For detailed parameters of obstacle sensing,
 * it is recommended to read the official user manual in https://www.ziyan.com/uk/matrice-300/downloads.
 * @param horizontalObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetHorizontalRadarObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus *horizontalObstacleAvoidanceEnableStatus);

/**
 * @brief Enable/Disable upwards visual obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param upwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetUpwardsVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus upwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Get the switch status of upwards visual obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param upwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetUpwardsVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus *upwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Enable/Disable upwards radar obstacle sensing.
 * @note It will be valid only if you install CSM radar successfully.For detailed parameters of obstacle sensing,
 * it is recommended to read the official user manual in https://www.ziyan.com.
 * @param upwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetUpwardsRadarObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus upwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Get the switch status of upwards radar obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param upwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetUpwardsRadarObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus *upwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Enable/Disable downwards visual obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param downwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetDownwardsVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus downwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Get the switch status of downwards visual obstacle sensing.
 * @note For detailed parameters of obstacle sensing, it is recommended to read the official user manual in
 * https://www.ziyan.com.
 * @param downwardsObstacleAvoidanceEnableStatus: see reference of E_ZiyanFlightControllerObstacleAvoidanceEnableStatus.
 * It inherits from Pilot's param.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetDownwardsVisualObstacleAvoidanceEnableStatus(
    E_ZiyanFlightControllerObstacleAvoidanceEnableStatus *downwardsObstacleAvoidanceEnableStatus);

/**
 * @brief Arrest flying means emergency braking
 * @note When the aircraft is on the ground, it will stop motors and display "hms description" on App. when the aircraft is
 * in the air, it will continue flying and display "hms description" on App only.
 * If you use this interface, you need to use "ZiyanFlightController_CancelArrestFlying" to quit arrest-flying status, then
 * then the aircraft can fly again.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_ArrestFlying(void);

/**
 * @brief Quit status of arrest-flying.
 * @note The aircraft need to quit status of arrest-flying to continue flying after arresting flying.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_CancelArrestFlying(void);

/**
 * @brief Turn on motors when the aircraft is on the ground.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_TurnOnMotors(void);

/**
 * @brief Turn off motors when the aircraft is on the ground.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_TurnOffMotors(void);

/**
 * @brief Emergency stop motor in any case.
 * @note If you want to turn on motor after emergency stopping motor, you need to use the interface to send disable
 * command to quit lock-motor status.
 * @param cmd: see reference of E_ZiyanFlightControllerEmergencyStopMotor
 * @param debugMsg:inject debug message to flight control FW for logging, size limit: 10 bytes
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_EmergencyStopMotor(E_ZiyanFlightControllerEmergencyStopMotor cmd,
                                                       char debugMsg[EMERGENCY_STOP_MOTOR_MSG_MAX_LENGTH]);
/**
 * @brief Request taking off action when the aircraft is on the ground.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_StartTakeoff(void);

/**
 * @brief Request landing action when the aircraft is in the air.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_StartLanding(void);

/**
 * @brief Request cancelling landing action when the aircraft is landing
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_CancelLanding(void);

/**
 * @brief Confirm the landing when the aircraft is 0.7 m above the ground.
 * @note When the clearance between the aircraft and the ground is less than 0.7m, the aircraft will pause landing and
 * wait for user's confirmation. This API is for confirm landing. If the ground is not suitable for landing, user
 * must use RC to control it landing manually or force landing.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_StartConfirmLanding(void);

/**
 * @brief Force landing in any case.
 * @note This API will ignore the smart landing function. When using this API, it will landing directly (would not stop
 * at 0.7m and wait user's command). Attention: it may make the aircraft crash!!!
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_StartForceLanding(void);

/**
 * @brief Set customized GPS(not RTK) home location.
 * @note Set customized home location failed reason may as follows:
 * 1. The distance between new home location and last home location is larger than MAX_FLY_RADIUS(20 km).
 * 2. Record initial home location failed after start aircraft.
 * @param homeLocation: homeLocation include latitude and longitude
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetHomeLocationUsingGPSCoordinates(T_ZiyanFlightControllerHomeLocation homeLocation);

/**
 * @brief Set home location using current aircraft GPS (not RTK) location.
 * @note Set home location failed reasons may as follows:
 * 1. Aircraft's gps level can't reach the condition of recording home location.
 * 2. Record initial home location failed after start aircraft.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetHomeLocationUsingCurrentAircraftLocation(void);

/**
 * @brief Set go home altitude.
 * @note If aircraft's current altitude is higher than the setting value of go home altitude, aircraft will go home
 * using current altitude. Otherwise, it will climb to setting of go home altitude ,and then execute go home action.
 * Go home altitude setting is 20-1500 m.
 * @param altitude: go home altitude, unit: meter
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_SetGoHomeAltitude(E_ZiyanFlightControllerGoHomeAltitude altitude);

/**
 * @brief Get go home altitude.
 * @param altitude: go home altitude, unit: meter
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetGoHomeAltitude(E_ZiyanFlightControllerGoHomeAltitude *altitude);

/**
 * @brief Get country code.
 * @param countryCode: Pointer of buffer to return country code. The country code indicates the current country or
 * region where the aircraft is located. Please refer to the ISO 3166-1 code table for the specific meaning of the
 * country code.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetCountryCode(uint16_t *countryCode);

/**
 * @brief Request go home action when the aircraft is in the air
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_StartGoHome(void);

/**
 * @brief Request cancel go home action when the aircraft is going home
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_CancelGoHome(void);

/**
 * @brief Obtain aircraft's joystick control permission.
 * @note 1.You have to obtain joystick control permission successfully before you using joystick to control aircraft.
 * 2. RC must be in p-mode.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_ObtainJoystickCtrlAuthority(void);

/**
 * @brief Release aircraft's joystick control permission.
 * @note RC must be in p-mode.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_ReleaseJoystickCtrlAuthority(void);

/**
 * @brief Subscribe to joystick control permission switch event with a callback function.
 * @note it will be triggered once the joystick control permission switch event occurs.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_RegJoystickCtrlAuthorityEventCallback(JoystickCtrlAuthorityEventCbFunc callback);

/**
 * @brief Set expected joystick mode before requesting joystick.
 * @param joystickMode: include horizontal/vertical/yaw control mode, stable control mode.
 */
void ZiyanFlightController_SetJoystickMode(T_ZiyanFlightControllerJoystickMode joystickMode);

/**
 * @brief Request execute joystick action.
 * @param joystickCommand: include x/y/z/yaw.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_ExecuteJoystickAction(T_ZiyanFlightControllerJoystickCommand joystickCommand);

/**
 * @brief Request emergency brake action.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_ExecuteEmergencyBrakeAction(void);

/**
 * @brief Request cancel emergency brake action.
 * @note It is only support on M30.If you use ZiyanFlightController_ExecuteEmergencyBrakeAction(), you need to use
 * "ZiyanFlightController_CancelEmergencyBrakeAction()" to allow aircraft to execute aircraft action again.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_CancelEmergencyBrakeAction(void);

/**
 * @brief Get general info of the aircraft.
 * @param generalInfo: the struct stored the serial num which contains a array of chars var in case the user gives an
 * illegal length character pointer
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_GetGeneralInfo(T_ZiyanFlightControllerGeneralInfo *generalInfo);

/*! @brief The command decides whether execute RC lost action or not when PSDK is running
  * @note  This setting only affects the behavior of the aircraft when the RC lost and the PSDK is connected.
  *         if the command is enable, the aircraft will not execute RC lost action when RC is lost but PSDK is running;
  *         if the command is disable, the aircraft will execute RC lost action when RC is lost but PSDK is running
  *         the aircraft will execute RC lost action when RC is lost and PSDK is lost whatever the command is.
  *         default command is disable.
  * @param executeRCLostActionOrNotWhenOnboardOn  enable:1;disable:0
  * @return T_ZiyanReturnCode error code
   */
T_ZiyanReturnCode
ZiyanFlightController_SetRCLostActionEnableStatus(E_ZiyanFlightControllerRCLostActionEnableStatus command);

/*! @brief get RC lost action enable status(enable or disable)
 *  @param command executeRCLostActionOrNotWhenOnboardOn, enable:1;disable:0
 *  @return  T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode
ZiyanFlightController_GetEnableRCLostActionStatus(E_ZiyanFlightControllerRCLostActionEnableStatus *command);

/**
 * @brief Register callback function for the trigger FTS event.
 * @note The timing of the trigger of the callback function of the FTS is determined by the aircraft, and the trigger
 *       execution action of the FTS needs to be implemented in the callback function and the correct return value
*        must be returned, otherwise the aircraft will always be triggered.
 * @param callback: the callback for the trigger FTS event.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanFlightController_RegTriggerFtsEventCallback(TriggerFtsEventCallback callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_FLIGHT_CONTROLLER_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

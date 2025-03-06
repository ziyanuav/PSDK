/**
 ********************************************************************
 * @file    ziyan_waypoint_v2_type.h
 * @brief   This is the header file for "ziyan_waypoint_v2_type.c", defining the structure and
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
#ifndef ZIYAN_WAYPOINT_V2_TYPE_H
#define ZIYAN_WAYPOINT_V2_TYPE_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Type define ziyan_f32_t as T_ZiyanWaypointV2GlobalCruiseSpeed.
 * @details Type used for global cruise speed of waypoint mission, unit: m/s.
 */
typedef ziyan_f32_t T_ZiyanWaypointV2GlobalCruiseSpeed;

/**
*  Actions will be taken when the waypoint mission is finished.
*/
typedef enum {

    /**
     *  No further action will be taken.
     *  The aircraft can be controlled by the remote controller.
     */
    ZIYAN_WAYPOINT_V2_FINISHED_NO_ACTION,

    /**
     *  Goes home when the mission is finished. The aircraft will
     *  land directly if it is within 20 meters away from the home point.
     */
    ZIYAN_WAYPOINT_V2_FINISHED_GO_HOME,

    /**
     *  The aircraft will land automatically at the last waypoint.
     */
    ZIYAN_WAYPOINT_V2_FINISHED_AUTO_LANDING,

    /**
     *  The aircraft will go back to the first waypoint and hover.
     */
    ZIYAN_WAYPOINT_V2_FINISHED_GO_TO_FIRST_WAYPOINT,

    /**
     *  When the aircraft reaches its final waypoint, it will hover without ending the
     *  mission. The joystick  can still be used to pull the aircraft back along its
     *  previous waypoints. The only way this mission  can end is if stopMission is
     *  called.
     */
    ZIYAN_WAYPOINT_V2_FINISHED_CONTINUE_UNTIL_STOP
} E_ZIYANWaypointV2MissionFinishedAction;

typedef enum {
    /**
     * Stops current mission when the remote controller signal(RC) is lost.
     * And execute rc lost action which is choosed by users on APP.
     * note:the option can be invalid only if you didn't obtain joystick authority
     * (ZiyanFlightController_ObtainJoystickCtrlAuthority).Or it will continue executing
     * the mission
     */
    ZIYAN_WAYPOINT_V2_MISSION_STOP_WAYPOINT_V2_AND_EXECUTE_RC_LOST_ACTION,

    /**
     *  Continue executing the mission when the remote controller signal(RC) is lost..
     */
    ZIYAN_WAYPOINT_V2_MISSION_KEEP_EXECUTE_WAYPOINT_V2,
} E_ZIYANWaypointV2MissionActionWhenRcLost;

typedef enum {

    /**
     *  Go to the waypoint safely. The aircraft will rise to the same altitude of the
     *  waypoint if the current  altitude is lower then the waypoint altitude. It then
     *  goes to the waypoint coordinate from the current  altitude, and proceeds to the
     *  altitude of the waypoint.
     */
    ZIYAN_WAYPOINT_V2_MISSION_GO_TO_FIRST_WAYPOINT_MODE_SAFELY,

    /**
     *  Go to the waypoint from the current aircraft point to the waypoint directly.
     */
    ZIYAN_WAYPOINT_V2_MISSION_GO_TO_FIRST_WAYPOINT_MODE_POINT_TO_POINT,
} E_ZIYANWaypointV2MissionGotoFirstWaypointMode;

/**
*  Waypoint flight path mode.
*/
typedef enum {

    /**
     *  In the mission, the aircraft will go to the waypoint along a curve and fly past the waypoint.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_GO_TO_POINT_ALONG_CURVE,

    /**
     *  In the mission, the aircraft will go to the waypoint along a curve and stop at the waypoint.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_GO_TO_POINT_ALONG_CURVE_AND_STOP,

    /**
     *  In the mission, the aircraft will go to the waypoint along a straight line and stop at the waypoint.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_GO_TO_POINT_IN_STRAIGHT_AND_STOP,

    /**
     *  In the mission, the aircraft will fly from the previous waypoint to the next waypoint along a smooth curve without stopping at this waypoint.
     *  the next in a curved motion,  adhering to the ``ZIYANWaypointV2_dampingDistance``, which is
     *  set in ``ZIYANWaypointV2``.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_COORDINATE_TURN,

    /**
     *  In the mission, the aircraft will go to the first waypoint along a straight line.
     *  This is only valid for the first waypoint.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_GO_TO_FIRST_POINT_ALONG_STRAIGHT_LINE,

    /**
     *  Straight exit the Last waypoint, Only valid for last waypoint.
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_STRAIGHT_OUT,

    /**
     *  Unknown
     */
    ZIYAN_WAYPOINT_V2_FLIGHT_PATH_MODE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2FlightPathMode;

/**
*  Represents current aircraft's heading mode on current waypoint.
*/
typedef enum {

    /**
     *  Aircraft's heading will always be in the direction of flight.
     */
    ZIYAN_WAYPOINT_V2_HEADING_MODE_AUTO,

    /**
     * Aircraft's heading will be set to the heading when reaching the first waypoint.
     * Before reaching the first waypoint, the aircraft's heading can be controlled by
     * the remote controller. When the aircraft reaches the first waypoint, its
     * heading will be fixed.
     */
    ZIYAN_WAYPOINT_V2_HEADING_FIXED,

    /**
     *  The aircraft's heading in the mission can be controlled by the remote controller.
     */
    ZIYAN_WAYPOINT_V2_HEADING_MANUAL,

    /**
     * In the mission, the aircraft's heading will change dynamically and adapt to the heading set at the next waypoint.
     * See ``ZIYANWaypointV2_heading`` to preset the heading.
     */
    ZIYAN_WAYPOINT_V2_HEADING_WAYPOINT_CUSTOM,

    /**
     *  Aircraft's heading will always toward point of interest.
     *  using ``ZIYANWaypointV2_pointOfInterest`` setting point of interest coordiate and ``ZIYANWaypointV2_pointOfInterestAltitude``
     *  setting point of interset altitute.
     */
    ZIYAN_WAYPOINT_V2_HEADING_TOWARDS_POINT_OF_INTEREST,

    /**
     *  The aircraft's heading rotate simultaneously with its gimbal's yaw.
     */
    ZIYAN_WAYPOINT_V2_HEADING_GIMBAL_YAW_FOLLOW,

    /**
     *  Unknown.
     */
    ZIYAN_WAYPOINT_V2_HEADING_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2HeadingMode;

/**
*  The direction when the aircraft changes its heading to adapt to the heading at the waypoint.
*/
typedef enum {

    /**
     *  The aircraft's heading rotates clockwise.
     */
    ZIYAN_WAYPOINT_V2_TURN_MODE_CLOCK_WISE,

    /**
     *  The aircraft's heading rotates counterclockwise.
     */
    ZIYAN_WAYPOINT_V2_TURN_MODE_COUNTER_CLOCK_WISE,

    /**
     *  Changes the heading of the aircraft by rotating the aircraft anti-clockwise.
     */
    ZIYAN_WAYPOINT_V2_TURN_MODE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2TurnMode;

/**
 *  All the possible state of ``WaypointV2MissionOperator``.
 */
typedef enum {
    /**
      *  Waypoint mission state is ground station not start.
      */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_GROUND_STATION_NOT_START = 0,

    /**
     *  Waypoint mission state is mission prepared.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_MISSION_PREPARED = 1,

    /**
     *  Waypoint mission state is enter mission.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_ENTER_MISSION = 2,

    /**
     *  Waypoint mission state is execute flying route mission.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_EXECUTING = 3,

    /**
     *  Waypoint mission state is pause state.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_PAUSED = 4,

    /**
     *  Waypoint mission state is enter mission after ending pause.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_ENTER_MISSION_AFTER_ENDING_PAUSE = 5,

    /**
     *  Waypoint mission state is exit mission.
     */
    ZIYAN_WAYPOINT_V2_MISSION_STATE_EXIT_MISSION = 6,
} E_ZIYANWaypointV2MissionState;

/*! waypoint position relative to WayPointV2InitSettings's reference point
 * unit: m
 */
typedef struct {
    ziyan_f32_t positionX; /*! X distance to reference point, North is positive*/
    ziyan_f32_t positionY; /*! Y distance to reference point, East is positive*/
    ziyan_f32_t positionZ; /*! Z distance to reference point, UP is positive*/
} T_ZiyanWaypointV2RelativePosition;

/**
 *  Represents current waypoint's speed config.
 */
typedef struct {
    /*! 0: set local waypoint's cruise speed,
     *  1: unset local waypoint's cruise speed*/
    uint16_t useLocalCruiseVel;

    /*! 0: set local waypoint's max speed,
     *  1: unset local waypoint's max speed*/
    uint16_t useLocalMaxVel;
} T_ZiyanWaypointV2Config;

typedef struct {
    /*! waypoint position relative to WayPointV2InitSettings's reference point
     * unit: m
     */
    ziyan_f64_t longitude;
    ziyan_f64_t latitude;
    ziyan_f32_t relativeHeight; /*! relative to takeoff height*/

    /**
     *  Waypoint flight path mode.
     */
    E_ZIYANWaypointV2FlightPathMode waypointType;

    /**
     *  Represents current aircraft's heading mode on current waypoint.
     */
    E_ZIYANWaypointV2HeadingMode headingMode;

    /**
     *  Represents current waypoint's speed config.
     */
    T_ZiyanWaypointV2Config config;

    uint16_t dampingDistance;

    /**
    *  The heading to which the aircraft will rotate by the time it reaches the
    *  waypoint. The aircraft heading  will gradually change between two waypoints with
    *  different headings if the waypoint  mission's `headingMode` is set  to
    *  `ZIYANWaypointV2_ZIYANWaypointV2HeadingMode_WaypointCustom`. A heading has a range of
    *  [-180, 180] degrees, where 0 represents True North.
    */
    ziyan_f32_t heading;

    /**
    *  Determines whether the aircraft will turn clockwise or anticlockwise when
    *  changing its heading.
    */
    E_ZIYANWaypointV2TurnMode turnMode;

    /**
    *  Property is used when ``ZIYANWaypointV2_headingMode`` is
    *  ``ZIYANWaypointV2_ZIYANWaypointV2HeadingMode_TowardPointOfInterest``.
    *  Aircraft will always be heading to point while executing mission. Default is
    *  "kCLLocationCoordinate2DInvalid".
    */
    T_ZiyanWaypointV2RelativePosition pointOfInterest;

    /**
    *  While the aircraft is travelling between waypoints, you can offset its speed by
    *  using the throttle joystick on the remote controller. "maxFlightSpeed" is this
    *  offset when the joystick is pushed to maximum deflection. For example, If
    *  maxFlightSpeed is 10 m/s, then pushing the throttle joystick all the way up will
    *  add 10 m/s to the aircraft speed, while pushing down will subtract 10 m/s from
    *  the aircraft speed. If the remote controller stick is not at maximum deflection,
    *  then the offset speed will be interpolated between "[0, maxFlightSpeed]"" with a
    *  resolution of 1000 steps. If the offset speed is negative, then the aircraft
    *  will fly backwards to previous waypoints. When it reaches the first waypoint, it
    *  will then hover in place until a positive speed is applied. "maxFlightSpeed" has
    *  a range of [2,15] m/s.
    *
    *  unit: m/s
    */
    ziyan_f32_t maxFlightSpeed;

    /**
    *  The base automatic speed of the aircraft as it moves between waypoints with
    *  range [-15, 15] m/s. The aircraft's actual speed is a combination of the base
    *  automatic speed, and the speed control given by the throttle joystick on the
    *  remote controller. If "autoFlightSpeed >0": Actual speed is "autoFlightSpeed" +
    *  Joystick Speed (with combined max of "maxFlightSpeed") If "autoFlightSpeed =0":
    *  Actual speed is controlled only by the remote controller joystick. If
    *  "autoFlightSpeed <0" and the aircraft is at the first waypoint, the aircraft
    *  will hover in place until the speed is made positive by the remote controller
    *  joystick. In flight controller firmware 3.2.10.0 or above, different speeds
    *  between individual waypoints can also be set in waypoint objects which will
    *  overwrite "autoFlightSpeed".
    *
    *  unit: m/s.
    */
    ziyan_f32_t autoFlightSpeed;
} T_ZiyanWaypointV2;

typedef struct {
    T_ZiyanWaypointV2 *waypointV2;
    uint16_t waypointV2Num;
} T_ZiyanWaypointV2List;

/**
*  Possible types of action trigger.
*/
typedef enum {
    /**
     *  The action will be triggered when action associated executes.
     *  The parameters should be defined by ``ZIYANWaypointV2Action_E_ZIYANWaypointV2AssociateTriggerParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_TRIGGER_ACTION_ASSOCIATED = 2,

    /**
     *  The action will be triggered when the aircraft flies from one waypoint to the next.
     *  The parameters should be defined by ``ZIYANWaypointV2Action_T_ZIYANWaypointV2TrajectoryTriggerParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_TRIGGER_TYPE_TRAJECTORY,

    /**
     *  The action will be triggered when the aircraft flies between two waypoints
     *  The parameters should be defined by ``ZIYANWaypointV2Action_T_ZIYANWaypointV2IntervalTriggerParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_TRIGGER_TYPE_INTERVAL,

    /**
     *  The action will be trigger when the aircraft reach the waypoint point.
     *  The parameters should be setting by ``ZIYANWaypointV2SampleReachPointTriggerParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_TRIGGER_TYPE_SAMPLE_REACH_POINT,

    /**
     *  Unknown
     */
    ZIYAN_WAYPOINT_V2_ACTION_TRIGGER_TYPE_UNKNOWN = 0xFF
} E_ZIYANWaypointV2ActionTriggerType;

/**
*  The type of ``ZIYANWaypointV2Action_E_ZIYANWaypointV2AssociateTriggerParam``,
*  Determines the time to execute the trigger associated with another one.
*/
typedef enum {

    /**
     *  The trigger starts simultaneously with the trigger that is associated.
     */
    ZIYAN_WAYPOINT_V2_TRIGGER_ASSOCIATED_TIMING_TYPE_SIMULTANEOUSLY = 1,

    /**
     *  The trigger starts after the trigger associated has finished.
     */
    ZIYAN_WAYPOINT_V2_TRIGGER_ASSOCIATED_TIMING_TYPE_AFTER_FINISHED,

    /**
     *  Unkown timing type.
     */
    ZIYAN_WAYPOINT_V2_TRIGGER_ASSOCIATED_TIMING_TYPE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2TriggerAssociatedTimingType;

/**
*  The type of ``ZIYANWaypointV2Action_ZIYANWaypointV2IntervalTriggerParam``,
*  Determines the interval type of how action repeats.
*/
typedef enum {
    /**
     *  The action will be repeated after a particular period of time.
     */
    ZIYAN_WAYPOINT_V2_ACTION_INTERVAL_TYPE_TIME = 1,

    /**
     *  The action will be repeated after a particular distance.
     */
    ZIYAN_WAYPOINT_V2_ACTION_INTERVAL_TYPE_DISTANCE,

    /**
     *  Unknown action trigger type.
     */
    ZIYAN_WAYPOINT_V2_ACTION_INTERVAL_TYPE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2ActionIntervalType;

/**
*  Possible types of action actuator.
*/
typedef enum {

    /**
     *  The action will be executed by the camera.
     *  The parameters should be defined by ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_TYPE_CAMERA = 1,

    /**
     *  The action will be executed by the gimbal.
     *  The parameters should be defined by ``ZIYANWaypointV2Action_ZIYANWaypointV2GimbalActuatorParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_TYPE_GIMBAL = 2,

    /**
     *  The action will executes by control aircraft.
     *  The parameters should be setting by ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_TYPE_AIRCRAFT_CONTROL = 4,

    /**
     *  Unknown actuator type.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_TYPE_UNKNOWN = 0xFF
} E_ZIYANWaypointV2ActionActuatorType;

/**
*  Possible types of camera actuator operation.
*/
typedef enum {

    /**
     *  Starts to shoot a photo.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_TAKE_PHOTO = 1,

    /**
     *  Starts to record a video.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_START_RECORD_VIDEO,

    /**
     *  Stops to record a video.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_STOP_RECORD_VIDEO,

    /**
     *  Starts focus.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_FOCUS,

    /**
     *  Starts focal lenth. Only support those support flocal lenth cameras.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_FOCUL_LENGTH,

    /**
     *  Unknown.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_CAMERA_OPERATION_TYPE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2ActionActuatorCameraOperationType;

/**
*  The type of gimbal actuator operation.
*/
typedef enum {

    /**
     *  Rotates the gimbal. Only valid when the trigger type is
     *  ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2TriggerAssociatedTimingType_ReachPoint``.
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_GIMBAL_OPERATION_TYPE_ROTATE_GIMBAL = 1,

    /**
     *  Unknown
     */
    ZIYAN_WAYPOINT_V2_ACTION_ACTUATOR_GIMBAL_OPERATION_TYPE_UNKNOWN = 0xFF,
} E_ZIYANWaypointV2ActionActuatorGimbalOperationType;

/*! Get the remain memory  ack data
 */
typedef struct {
    uint16_t totalMemory;
    uint16_t remainMemory;
} T_ZiyanWaypointV2GetRemainMemory;

/*! Get the mission's start and stop index ack data
 */
typedef struct {
    uint32_t result;
    uint16_t startIndex;
    uint16_t endIndex;
} T_ZiyanWaypointV2GetWaypointStartEndIndex;

#pragma pack(1)
typedef struct {
    /**
     *  It determines the index of the waypoint at which the action will be
     * triggered.
     */
    uint16_t waypointIndex;

    uint16_t terminateNum;

} T_ZIYANWaypointV2SampleReachPointTriggerParam;

/**
*  This class defines
* ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionTriggerType_ActionAssociated``.
*/
typedef struct {
    /**
     *  The type of assciate trigger.see "E_ZIYANWaypointV2TriggerAssociatedTimingType"
     */
    uint8_t actionAssociatedType: 7;

    /**
     * Waiting time uint
     * 1:seconds 0:milliseconds
     */

    uint8_t waitTimeUint: 1;

    /**
     *  Waiting time in seconds after ActionTrigger starts.
     */
    uint8_t waitingTime;

    /**
     *  Associated action ID.
     */
    uint16_t actionIdAssociated;
} T_ZIYANWaypointV2AssociateTriggerParam;

/**
*  This class represents a trajectory trigger action when should be trigger.
*/
typedef struct {
    /**
     *  It determines the index of the waypoint at which the trigger starts.
     */
    uint16_t startIndex;

    /**
     *  It determines the waypoint when the trigger stops.
     */
    uint16_t endIndex;
} T_ZIYANWaypointV2TrajectoryTriggerParam;

/**
*  This class defines
* ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionTriggerType_Trajectory``.
*/
typedef struct {
    /**
     *  It determines the index of the waypoint at which the trigger starts.
     */
    uint16_t startIndex;

    /**
     *  If the
     * ``ZIYANWaypointV2Action_T_ZIYANWaypointV2IntervalTriggerParam_actionIntervalType``
     * is ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2TriggerAssociatedTimingType_Time``
     *  The time interval in 0.01 seconds when two action are executed as the aircraft
     * moves
     *  from the current waypoint to the next waypoint.
     *  If the
     * ``ZIYANWaypointV2Action_T_ZIYANWaypointV2IntervalTriggerParam_actionIntervalType``
     * is
     * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2TriggerAssociatedTimingType_Distance``
     *  The distance interval in 0.01 meters when two action are executed as the
     * aircraft
     * moves
     *  from the current waypoint to the next waypoint.
      */
    uint16_t interval;
    /**
     *  The type of interval trigger.
     *  See ``E_ZIYANWaypointV2ActionIntervalType``.
     */
    uint8_t actionIntervalType;

} T_ZIYANWaypointV2IntervalTriggerParam;

typedef struct {
    //see "E_ZIYANWaypointV2ActionTriggerType"
    uint8_t actionTriggerType;
    union {
        T_ZIYANWaypointV2SampleReachPointTriggerParam sampleReachPointTriggerParam;
        T_ZIYANWaypointV2AssociateTriggerParam associateTriggerParam;
        T_ZIYANWaypointV2TrajectoryTriggerParam trajectoryTriggerParam;
        T_ZIYANWaypointV2IntervalTriggerParam intervalTriggerParam;
    };
} T_ZIYANWaypointV2Trigger;

/*! Focus point parameter
 */
typedef struct {
    /*! x axis focus point value
     * range: [0,1]
     */
    ziyan_f32_t x;
    /*! y axis focus point value
     * range: [0,1]
     */
    ziyan_f32_t y;
} T_ZiyanWaypointV2CGPoint;

/**
*  This class defines a camera focus operation for
* ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam``.
*/
typedef struct {
    /**
     *  The lens focus target point. When the focus mode is auto, the target point
     *  is the focal point. When the focus mode is manual, the target point is the
     * zoom
     *  out area if the focus assistant is enabled for the manual mode.
     *  The range for x and y is from 0.0 to 1.0. The point [0.0, 0.0] represents
     * the top-left angle of the screen.
     */
    T_ZiyanWaypointV2CGPoint focusTarget;

    /**
     * focus type:
     * 0:point focus
     * 1:rectangle focus
     */
    uint8_t regionType;

    /**
     * Normalized focus area width(0,1)
     */
    ziyan_f32_t width;

    /**
     * Normalized focus area height(0,1)
     */

    ziyan_f32_t height;

    uint32_t reserve;

    /**
     * [0,255]
     */
    uint8_t retryTimes;
} T_ZIYANWaypointV2CameraFocusParam;

/**
*  This class defines a camera focal length operation for
* ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam``.
*/
typedef struct {
    /**
     *  Focal length of zoom lens. Valid range is
     * [``ZIYANCamera_ZIYANCameraOpticalZoomSpec_minFocalLength``,
     * ``ZIYANCamera_ZIYANCameraOpticalZoomSpec_minFocalLength``]
     *  and must be a multiple of
     * ``ZIYANCamera_ZIYANCameraOpticalZoomSpec_focalLengthStep``.
     *  Only support by those camera
     * ``ZIYANCamera_CameraSettings_isOpticalZoomSupported`` return ``TRUE``.
     */
    uint16_t focalLength;

    uint8_t retryTimes;

} T_ZIYANWaypointV2CameraFocalLengthParam;

/**
*  This class defines the parameters for
* ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator``.
*  This determines how the camera will be performed when a waypoint mission is
* executing.
*/
typedef struct {
    /**
     *  The operation type of camera actuator.
     *  See
     * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorCameraOperationType``.
     */
    uint16_t operationType;

    union {
        /**
         *  The parameters for camera focus operation. It is valid only when
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam_operationType``
         *  is
         * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorCameraOperationType_Focus``
         */
        T_ZIYANWaypointV2CameraFocusParam focusParam;

        /**
         *  The parameters for camera focus length operation. It is valid only when
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2CameraActuatorParam_operationType``
         *  is
         * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorCameraOperationType_FocalLength``
         */
        T_ZIYANWaypointV2CameraFocalLengthParam zoomParam;
    };

} T_ZIYANWaypointV2CameraActuatorParam;

/*! Gimbal rotation parameter
 */
typedef struct {

    int16_t x;  /*! gimbal roll angle,  unit: 0.1 deg,range:[-3600, 3600]*/
    int16_t y;  /*! gimbal pitch angle, unit: 0.1 deg,range:[-3600, 3600]*/
    int16_t z;  /*! gimbal yaw angle,   unit: 0.1 deg,range:[-3600, 3600]*/
    uint8_t ctrl_mode: 1;   /*! 0: absolute position control, 1:relative position control*/
    uint8_t rollCmdIgnore: 1;/*! 0: roll command normal,  1: roll command ignore*/
    uint8_t pitchCmdIgnore: 1;/*! 0: pitch command normal,  1: pitch command ignore*/
    uint8_t yawCmdIgnore: 1;  /*! 0: yaw command normal,  1: yaw command ignore*/
    uint8_t absYawModeRef: 1; /*! 0: absoluate rotate yaw relative to aircraft,
                            *  1: absoluate rotate yaw relative to North*/
    uint8_t reserved: 3;
    uint8_t durationTime;  /*! 0: rotate time,unit:0.1s, range[1,255]*/
} T_ZIYANGimbalRotation;

/**
*  This class defines the parameters for
* ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator``. It determines how the
*  gimbal actuator will be performed when a waypoint mission is executed.
*/
typedef struct {
    /**
     *  The operation type of gimbal actuator.
     *  See
     * ``E_ZIYANWaypointV2ActionActuatorGimbalOperationType``.
     */
    uint16_t operationType;

    /**
     *  The rotation of gimbal actuator.
     */
    T_ZIYANGimbalRotation rotation;
} T_ZIYANWaypointV2GimbalActuatorParam;

/**
* Possible types of aircraft control actuator operation.
*/
typedef enum {
    /**
     *  Rotates the aircraft's yaw.
     */
    ZIYANWaypointV2ActionActuatorAircraftControlOperationTypeRotateYaw = 1,

    /**
     *  Keeps the aircraft stop flying or start flying.
     */
    ZIYANWaypointV2ActionActuatorAircraftControlOperationTypeFlyingControl = 2,

    /**
     *  Unknown
     */
    ZIYANWaypointV2ActionActuatorAircraftControlOperationTypeUnknown = 0xFF,
} E_ZIYANWaypointV2ActionActuatorAircraftControlOperationType;

/**
*  This class defines how the aircraft rotates on the yaw axis.
*/
typedef struct {
    /**
     *  Determines the aircraft rotate heading relative.
     *  if ``TRUE``, when the aircraft is rotating, relative to the current angle.
     */
    uint8_t isRelative: 1;

    uint8_t reserved: 7;

    /**
     *  Determines the direction how aircraft changes its heading.
     */
    ziyan_f32_t yaw;

} T_ZIYANWaypointV2AircraftControlRotateHeadingParam;

/**
*  This class defines if the aircraft starts or stops the flight.
*/
typedef struct {
    /**
     *  Determines the aircraft start flying or stop flying.
     *  ``TRUE`` for the aircraft to start flying.
     */
    uint8_t isStartFlying: 1;
    uint8_t reserved: 7;
} T_ZIYANWaypointV2AircraftControlFlyingParam;

/**
*  This class defines the parameters for
* ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator``. It determines how the
*  aircraft control actuator will be performed when a waypoint mission is
* executed.
*/
typedef struct {
    /**
     *  The operation type of aircraft control actuator.
     *  See
     * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorAircraftControlOperationType``.
     */
    uint16_t operationType;

    union {
        /**
         *  The parameter for rotating the aircraft's heading. It's valid only when
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2AircraftControlParam_operationType``
         *  is
         * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorAircraftControlOperationType_RotateYaw``
         */

        T_ZIYANWaypointV2AircraftControlRotateHeadingParam yawRotatingParam;

        /**
         *  The parameters to control flying behavior.  It's valid only when
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2AircraftControlParam_operationType``
         *  is
         * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorAircraftControlOperationType_FlyingControl``
         */

        T_ZIYANWaypointV2AircraftControlFlyingParam flyControlParam;
    };
} T_ZIYANWaypointV2AircraftControlParam;

/**
*  This class defines an actuator for ``ZIYANWaypointV2Action``. It determines
* how the
*  action is performed when a waypoint mission is executed.
*/
typedef struct {
    /**
     *  The type of Actuator.see "E_ZIYANWaypointV2ActionActuatorType"
     */
    uint8_t actuatorType;

    /**
     *  The index of actuator. It is valid when the diagnostics is related
     *  to camera or gimbal and the connected product has multiple gimbals and
     * cameras.
     */
    uint8_t actuatorIndex;

    union {
        /**
         *  The camera actuator param, It is valid only when the
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator_type``
         *  is ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorType_Camera``.
         */
        T_ZIYANWaypointV2CameraActuatorParam cameraActuatorParam;

        /**
         *  Parameters for gimbal actuator. It is valid only when the
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator_type``
         *  is ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorType_Gimbal``.
         */
        T_ZIYANWaypointV2GimbalActuatorParam gimbalActuatorParam;

        /**
         *  Parameters for aircraft control actuator. It is valid only when the
         * ``ZIYANWaypointV2Action_ZIYANWaypointV2Actuator_type``
         *  is
         * ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2ActionActuatorType_AircraftControl``.
         */
        T_ZIYANWaypointV2AircraftControlParam aircraftControlActuatorParam;
    };
} T_ZIYANWaypointV2Actuator;

/**
*  This class represents an action for ``ZIYANWaypointV2Mission``. It
* determines how
*  action is performed when a waypoint mission is executed.
*/
typedef struct {
    /**
     *  The ID of Action.
     */
    uint16_t actionId;

    /**
     *  The trigger of action.
     */
    T_ZIYANWaypointV2Trigger trigger;

    /**
     *  The actuator of action.
     */
    T_ZIYANWaypointV2Actuator actuator;
} T_ZIYANWaypointV2Action;

/**
 * @brief Waypoint V2 Mission Initialization Settings Internal
 * User have no need to use it
 */
typedef struct {
    uint16_t version;
    uint8_t reserved;
    uint32_t missionID;
    uint16_t missTotalLen;
    uint16_t waypointCount;
    uint8_t repeatTimes;
    //see "E_ZIYANWaypointV2MissionFinishedAction"
    uint8_t finishedAction;
    uint16_t maxFlightSpeed;
    uint16_t autoFlightSpeed;
    uint16_t startIndex;
    uint8_t actionWhenRcLost;
    //see "E_ZIYANWaypointV2MissionGotoFirstWaypointMode"
    uint8_t gotoFirstWaypointMode;
    ziyan_f64_t refLati;
    ziyan_f64_t refLong;
    ziyan_f32_t refAlti;
} T_WayPointV2InitSettingsInternal;

/*! Mission's event data
 */
typedef union {
    /*event ID:0x01*/
    uint8_t interruptReason; //0x00:rc triggered interrupt

    /*!event ID:0x02
     * 0x00:Finish recover from pause;
     * 0x10:Start recover from break;
     * 0x11:Finish recover from break;
     * 0x12:Fail recover from break.
     */
    uint8_t recoverProcess;

    /*!event ID:0x03
     * 0x00:finished successfully.
     * 0x10:External user trigger ended successfully.
     * 0x20:Request the route when motors is starting on the ground.
     * 0x21:Auto takeoff time out.
     * 0x22:Auto takeoff did not reach the predetermined altitude.
     * 0x23:AUto takeoff abnormal.
     * 0x24:Preempted by high priority tasks.
     * 0x25:Unknown reason.
     * 0x26:Route planning failure
     */
    uint8_t exitReason;

    /*event ID:0x10*/
    uint16_t waypointIndex;

    /*event ID:0x11*/
    struct T_ZiyanWaypointV2MissionExecEvent {
        uint8_t currentMissionExecTimes;
        uint8_t finishedAllMissExecTimes: 1; //0:not finished; 1:finished all exec num
        uint8_t reserved: 7;
    } T_ZiyanWaypointV2MissionExecEvent;

    /* event ID:0x12*/
    uint8_t avoidState;

    /*ID:0x30*/
    struct T_ZiyanWaypointV2ActionExecEvent {
        uint16_t actionId;
        /*!
         * 0x00: device id not mounted or identified
         * 0x01: is free
         * 0x02: is running
         * 0x03: be paused
         */
        uint8_t preActuatorState;
        /*!
         * 0x00: device id not mounted or identified
         * 0x01: is free
         * 0x02: is running
         * 0x03: be paused
         */
        uint8_t curActuatorState;
        /*!
         * error code
         */
        uint32_t result;
    } T_ZiyanWaypointV2ActionExecEvent;
} U_ZiyanWaypointV2EventData;

/*! Mission's event push ack data
 */
typedef struct {
    /*!
     * 0x01: mission interrupt event
     * 0x02: mission resume  event
     * 0x03: mission stop event
     * 0x10: mission arrival event
     * 0x11: route finished event
     * 0x12: avoid obstacle event
     * 0x30: action switch event
     */
    uint8_t event;
    uint32_t FCTimestamp;
    U_ZiyanWaypointV2EventData data;
} T_ZiyanWaypointV2MissionEventPush;
#pragma pack()

/*! Mission's state push data
 */
typedef struct {
    uint16_t curWaypointIndex;
    uint8_t state; /*!< enum-type: E_ZIYANWaypointV2MissionState. */
    uint16_t velocity; /*!< Uint:0.01m/s. */
} T_ZiyanWaypointV2MissionStatePush;

typedef struct {
    T_ZIYANWaypointV2Action *actions;
    uint16_t actionNum;
} T_ZIYANWaypointV2ActionList;

/**
 * @brief Waypoint V2 Mission Initialization settings
 * @details This is one of the few structs in the OSDK codebase that
 * is used in both a sending and a receiving API.
 */
typedef struct {

    /**
     *  The Mission ID. Use to distinguish different mission
     */
    uint32_t missionID;

    /**
     *  Mission execution can be repeated more than once. A value of 0 means the mission
     *  only executes once, and does not repeat. A value of 1 means the mission will
     *  execute a total of two times.
     */
    uint8_t repeatTimes;

    /**
     *  Action the aircraft will take when the waypoint mission is complete.
     */
    E_ZIYANWaypointV2MissionFinishedAction finishedAction;

    /**
     *  While the aircraft is travelling between waypoints, you can offset its speed by
     *  using the throttle joystick  on the remote controller. `maxFlightSpeed` is this
     *  offset when the joystick is pushed  to maximum deflection. For example, If
     *  maxFlightSpeed is 10 m/s, then pushing the throttle joystick all the  way up
     *  will add 10 m/s to the aircraft speed, while pushing down will subtract 10 m/s
     *  from the aircraft  speed. If the remote controller stick is not at maximum
     *  deflection, then the offset speed will be  interpolated between [0,
     *  `maxFlightSpeed`] with a resolution of 1000 steps. If the  offset speed is
     *  negative, then the aircraft will fly backwards to previous waypoints. When it
     *  reaches the  first waypoint, it will then hover in place until a positive speed
     *  is  applied. `maxFlightSpeed` has a range of [2,15] m/s.
     *
     *  unit: m/s
     */
    ziyan_f32_t maxFlightSpeed;

    /**
     *  The base automatic speed of the aircraft as it moves between waypoints with
     *  range [-15, 15] m/s. The  aircraft's actual speed is a combination of the base
     *  automatic speed, and the speed control given by  the throttle joystick on the
     *  remote controller. If `autoFlightSpeed` >0: Actual  speed is `autoFlightSpeed` +
     *  Joystick Speed (with combined max  of `maxFlightSpeed`) If `autoFlightSpeed` =0:
     *  Actual speed is  controlled only by the remote controller joystick. If
     *  `autoFlightSpeed` <0 and the  aircraft is at the first waypoint, the aircraft
     *  will hover in place until the speed is made positive by  the remote controller
     *  joystick.
     *
     *  unit: m/s
     */
    ziyan_f32_t autoFlightSpeed;

    /**
     *  Determines action when connection between the  aircraft
     *  and remote controller is lost.
     *
     */
    E_ZIYANWaypointV2MissionActionWhenRcLost actionWhenRcLost;

    /**
     *  Defines how the aircraft will go to the first waypoint from its current
     *  position. Default  is ``ZIYANWaypointV2MissionV2_ZIYANWaypointV2MissionGotoWaypointMode_Safely``.
     */
    E_ZIYANWaypointV2MissionGotoFirstWaypointMode gotoFirstWaypointMode;

    T_ZiyanWaypointV2 *mission;
    /**
    *  The Mission waypoint total number could not exceed 65535
    */
    uint16_t missTotalLen;

    T_ZIYANWaypointV2ActionList actionList;
} T_ZiyanWayPointV2MissionSettings;

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // ZIYAN_WAYPOINT_V2_TYPE_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

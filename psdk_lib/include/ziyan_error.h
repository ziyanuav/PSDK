/**
 ********************************************************************
 * @file    ziyan_error.h
 * @brief   This is the header file for "ziyan_error.c", defining the structure and
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
#ifndef ZIYAN_ERROR_H
#define ZIYAN_ERROR_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define ZIYAN_ERROR_MODULE_INDEX_OFFSET  32u
#define ZIYAN_ERROR_MODULE_INDEX_MASK    0x000000FF00000000u
#define ZIYAN_ERROR_RAW_CODE_OFFSET      0u
#define ZIYAN_ERROR_RAW_CODE_MASK        0x00000000FFFFFFFFu

#define ZIYAN_ERROR_CODE(moduleIndex, rawErrCode) \
((uint64_t)0 | \
((((uint64_t)(moduleIndex)) << (ZIYAN_ERROR_MODULE_INDEX_OFFSET)) & (ZIYAN_ERROR_MODULE_INDEX_MASK)) | \
((((uint64_t)(rawErrCode)) << (ZIYAN_ERROR_RAW_CODE_OFFSET)) & (ZIYAN_ERROR_RAW_CODE_MASK)))

/**
 * @brief Error objects, specifying error code, error description, error reason and error recovery suggestion below.
 * @attention Require arrange by error code from small to large.
 */
#define ZIYAN_ERROR_OBJECTS \
/* system module error message */ \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS, "Execution successfully.", NULL}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_REQUEST_PARAMETER, "Request parameters are invalid.", "Please double-check requested parameters."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_EXECUTING_HIGHER_PRIORITY_TASK, "A higher priority task is being executed.", "Please stop the higher priority task or try again later."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT, "Operation is not supported.", "Please check input parameters or contact ZIYAN for help."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_TIMEOUT, "Execution timeout.", "Please contact ZIYAN for help."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED, "Memory allocation failed.", "Please check system configure."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER, "Input parameters are invalid.", "Please double-check requested parameters."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE, "Operation is not supported in current state.", "Please try again later."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR, "System error.", "Please contact ZIYAN for help."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_HARDWARE_ERR, "Hardware error.", "Please contact ZIYAN for help."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_INSUFFICIENT_ELECTRICITY, "Low battery.", "Please replacement battery for machine and try again."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN, "Unknown error.", NULL}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND, "Parameters are not found.", NULL}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE, "Out of range.", "Please check parameters."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_BUSY, "System is busy.", "Please try again later."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_DUPLICATE, "Have existed the same object.", "Please input valid parameters."}, \
{ZIYAN_ERROR_SYSTEM_MODULE_CODE_ADAPTER_NOT_MATCH, "PSDK adapter do not meet requirements.", "Please try again after replacing PSDK adapter."}, \
 \
/* gimbal module error message */ \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_POSITIVE_LIMIT, "Pitch axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_NEGATIVE_LIMIT, "Pitch axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_POSITIVE_LIMIT, "Roll axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_NEGATIVE_LIMIT, "Roll axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_POSITIVE_LIMIT, "Yaw axis gimbal reach positive limit.", "Please do not rotate towards positive direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_NEGATIVE_LIMIT, "Yaw axis gimbal reach negative limit.", "Please do not rotate towards negative direction."}, \
{ZIYAN_ERROR_GIMBAL_MODULE_CODE_NON_CONTROL_AUTHORITY, "Current device do not have control authority of the gimbal.", "Please do not control gimbal with other devices that have high control priority simultaneously."}, \
 \
/* payload collaboration module error message */ \
{ZIYAN_ERROR_PAYLOAD_COLLABORATION_MODULE_CODE_POSITION_NOT_MATCH, "Payload mount position do not meet requirements.", "Please read API and user documentation, ensuring input parameters satisfy requirements."}, \
\
/* activation error message of identity module */ \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_PARAMETER_ERROR, "Activation parameter error.", "Please check the validity of the activation parameters."}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ENCODE_ERROR, "Activation encode error", "Please check the validity of the activation parameters"}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NEW_DEVICE_ERROR, "Activate a new device error", \
"\r\n* Double-check your app_id and app_key. Does it match with your ZIYAN developer account?\r\n"\
"* If this is a new device or has been previously activated with another app_id and app_key, you need to activate it through the ZIYAN Assistant 2 with Internet.\r\n"\
"* Please make sure you download the correct version of ZIYAN Assistant 2 for your drone.\r\n"\
}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED, "Not connection to ZIYAN Assistant 2.", "Please connect your drone to ZIYAN Assistant 2 while activating the new device at first time."}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NETWORK_ERROR, "ZIYAN Assistant 2 not connected to the internet.", "Please activate through the ZIYAN Assistant 2 with Internet"}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SERVER_ACCESS_REFUSED, "ZIYAN server refuse this activation request.", "Please check the validity of the activation parameters."}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ACCESS_LEVEL_ERROR, "Activate level error.", "Please check your app_id level."}, \
{ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_OSDK_VERSION_ERROR, "Activate a wrong osdk version", "Please check your OSDK version whether match the drone or not."}, \
\
/* subscription module error message */ \
{ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_INVALID_TOPIC_FREQ, "Frequency of topic is invalid.", "Please read API and user documentation, ensuring input parameters satisfy requirements."}, \
{ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_DUPLICATE, "Topic is subscribed repeatedly.", "Please do not subscribe a topic repeatedly."}, \
{ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUBSCRIBED, "Requested topic have not been subscribed.", "Please try to get value after subscribing topic."}, \
{ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TIMESTAMP_NOT_ENABLE, "Requested topic do not have timestamp data. Did not enable timestamp when subscribe topic.", "Please subscribe topic enabled timestamp."},\
{ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUPPORTED, "Requested topic is not supported.", "Please check the topic is supported or not on current aircraft version."}, \
\
/* mop channel module error message */ \
{ZIYAN_ERROR_MOP_CHANNEL_MODULE_CODE_CONNECTION_CLOSE, "Connection of channel is closed. The peer channel do not work or abnormally be closed.", "Please confirm state of the peer channel and reaccept the connection request of MSDK/OSDK"}, \
\
/* flight controller module error message */ \
{ZIYAN_ERROR_FC_MODULE_CODE_RC_MODE_ERROR, "RC_MODE_ERROR", "Please check the RC mode"}, \
{ZIYAN_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_SUCCESS, "RELEASE_CONTROL_SUCCESS", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_SUCCESS, "OBTAIN_CONTROL_SUCCESS", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_IN_PROGRESS, "OBTAIN_CONTROL_IN_PROGRESS", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_IN_PROGRESS, "RELEASE_CONTROL_IN_PROGRESS", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_RC_NEED_MODE_P, "RC_NEED_MODE_P", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_RC_NEED_MODE_F, "RC_NEED_MODE_F", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_PARAM_READ_WRITE_INVALID_PARAM, "read or write invalid param", "Please check hash value and param value"} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_IOC_OBTAIN_CONTROL_ERROR, "IOC_OBTAIN_CONTROL_ERROR", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_KEY_ERROR, "Activate key error", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_NO_AUTHORIZATION_ERROR, "No authorization", "Please finish activation firstly"} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_NO_RIGHTS_ERROR, "No rights error", NULL} ,\
{ZIYAN_ERROR_FC_MODULE_CODE_SYSTEM_ERROR, "Unknown system error", NULL} ,\
\
/* flight controller action module error message */\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_OBTAIN_RELEASE_JOYSTICK_AUTH_SUCCESS, "Obtain/Release joystick authority success", NULL} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_DEVICE_NOT_ALLOW, "The requesting device is not allowed to obtain/release joystick control authority, only support OSDK/MSDK", "Please use right devices(OSDK/MSDK)"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_TAKING_OFF, "Not allowed to obtain/release joystick control authority when drone is taking off ", "Please do it before or after taking off"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_LANDING, "Not allowed to obtain/release joystick control authority when drone is landing", "Please do it before or after landing"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_CMD_INVALID, "Invalid input command", "Please check your input command which only support 0/1"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_RC_NOT_P_MODE, "Not allowed to obtain/release joystick control authority when rc is not in P_MODE", "Please switch RC to P_MODE"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_CMD_LENGTH_ERROR, "Invalid length of input command", "Please input valid length command"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_HAS_NO_JOYSTICK_AUTHORITY, "Not allowed to release joystick control authority when drone has no joystick authority", "Please obtain joystick authority first"} ,\
{ZIYAN_ERROR_FC_JOYSTICK_MODULE_IN_RC_LOST_ACTION, "Not allowed to obtain/release joystick control authority when drone is executing rc lost action", "Please change battery"} ,\
\
/* flight controller action module error message */\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_ON, "now motor is on", "Please check motor status"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_OFF, "now motor is off", "Please check motor status"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_AIR, "aircraft is in air", "Please check aircraft flight status"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_NOT_IN_AIR, "aircraft is not in air", "Please check aircraft flight status"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_HOME_POINT_NOT_SET, "home point not set", "Please set home point"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_BAD_GPS, "bad GPS", "Please fly where the GPS signal is good"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_SIMULATION, "in simulation", "Please exit simulation first"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_CANNOT_START_MOTOR, "can not start motor", "Please check motor status"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_LOW_VOLTAGE, "low voltage", "Please change battery"} ,\
{ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_SPEED_TOO_LARGE, "speed too large", "Please slow down"} ,\
                          \
/* flight controller arrest flying module error message */\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_SET_SUCCESS, "register/logout/execute arrest-flying success", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_NULL_POINTER, "null pointer when register/logout arrest-flying", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_HAS_REGISTED, "duplicate hms code", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_NOT_FIND, "invalid hms code", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_ADD_ITEM_NO_DECRIPTION, "no description when register arrest-flying", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_ID_INVALID, "invalid request id", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE, "hms code is not in white list for allowing stop motor in the air", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_INVALID_FORMAT_HMSCODE, "invalid hms code format", NULL} ,\
{ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMSCODE_NOT_IN_WHITE_TABLE, "hms code is not in arrest-flying white list",NULL} ,\
\
/* flight controller home location module error message */\
{ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_UNKNOWN_FAILED_REASON, "set home location fail, unknown reason", NULL} ,\
{ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_INVALID_GPS_COORDINATE, "invalid GPS coordinate when set APP or RC to be home location", NULL} ,\
{ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_NOT_BE_RECORD, "home location is not inited", "Please waiting for aircraft recording home location"} ,\
{ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_GPS_NOT_READY, "GPS level < 4", NULL} ,\
{ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_DIS_TOO_FAR, "new home location is more than 20km away from current home locaton(APP/RC)", NULL} ,\
\
/* flight controller emergency stop motor module error message */\
{ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_VERSION_NOT_MATCH, "emergency stop motor version not match", NULL} ,\
{ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_CMD_INVALID, "emergency stop motor cmd invalid", NULL} ,\
/* camera manager module error message */\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND, "Command not supported", "Check the firmware or command validity"} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_TIMEOUT, "Camera's execution of this action has timed out", "Try again or check the firmware or command"} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED, "Camera's execution of this action is out of memory", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER,  "Camera received invalid parameters", "Check the validity of the parameter"} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE, "Camera is busy or the command is not supported in the Camera's current state", "Check current camera state is if appropriate fot the CMD"} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED, "The time stamp of the camera is not sync", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED, "Camera failed to set the parameters it received", "Please check the parameter to set is if supported in your devices."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED, "Camera param get failed", "Please check the parameter to get is if supported in your devices."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_MISSING, "Camera has no SD Card", "Please install SD card."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_FULL, "The Camera's SD Card is full", "Please make sure the SD card has enough space."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_ERROR, "Error accessing the SD Card", "Please check the validity of the SD card."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SENSOR_ERROR, "Camera sensor error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SYSTEM_ERROR, "Camera system error", "Please recheck all the running conditions or contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG, "Camera param get failed", "Please check the validity of the parameter length"} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_MODULE_INACTIVATED, "Camera module is not activated", "Please activate the module first."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS, "The seq number of Firmware data is invalid", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR, "Firmware check error", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR, "Camera flash write error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH, "Firmware type is invalid", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED, "Remote Control is disconnected now", "Please check the connection with remote control is if OK."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_HARDWARE_ERROR, "Camera hardware error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED, "Disconnect with aircraft", "Please check the connection with aircraft is if OK."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE, "Camera cannot not upgrade in current status", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNDEFINE_ERROR, "Undefined error", "Please contact <dev@ziyan.com> for help."} ,\
\
/* gimbal manager module error message */\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND, "Command not supported", "Check the firmware or command validity"} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_TIMEOUT, "Camera's execution of this action has timed out", "Try again or check the firmware or command"} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED, "Camera's execution of this action is out of memory", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER,  "Camera received invalid parameters", "Check the validity of the parameter"} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE, "Camera is busy or the command is not supported in the Camera's current state", "Check current camera state is if appropriate fot the CMD"} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED, "The time stamp of the camera is not sync", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED, "Camera failed to set the parameters it received", "Please check the parameter to set is if supported in your devices."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED, "Camera param get failed", "Please check the parameter to get is if supported in your devices."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_MISSING, "Camera has no SD Card", "Please install SD card."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_FULL, "The Camera's SD Card is full", "Please make sure the SD card has enough space."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_ERROR, "Error accessing the SD Card", "Please check the validity of the SD card."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SENSOR_ERROR, "Camera sensor error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SYSTEM_ERROR, "Camera system error", "Please recheck all the running conditions or contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG, "Camera param get failed", "Please check the validity of the parameter length"} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_MODULE_INACTIVATED, "Camera module is not activated", "Please activate the module first."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS, "The seq number of Firmware data is invalid", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR, "Firmware check error", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR, "Camera flash write error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH, "Firmware type is invalid", "Please check the validity of the camera firmware."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED, "Remote Control is disconnected now", "Please check the connection with remote control is if OK."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_HARDWARE_ERROR, "Camera hardware error", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED, "Disconnect with aircraft", "Please check the connection with aircraft is if OK."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE, "Camera cannot not upgrade in current status", "Please contact <dev@ziyan.com> for help."} ,\
{ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNDEFINE_ERROR, "Undefined error", "Please contact <dev@ziyan.com> for help."} ,\
\
/* waypoint v2 module error message */   \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_SUCCESS, "Execute waypoint v2 cmd successfully", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALID_DATA_LENGTH, "The length of the data is illegal based on the protocol ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALD_FLOAT_NUM, "Invalid float number (NAN or INF) ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_VERSION_NO_MATCH, "Waypoint mission version can't match with firmware ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_UNKNOWN, "Fatal error	 Unexpected result	 ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_RESV, "Reserved", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_MANY, "Min_initial_waypoint_num is large than permitted_max_waypoint_num ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_FEW, "Min_initial_waypoint_num is less than permitted_min_waypoint_num ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_END_INDEX, "Waypoint_end_index is equal or large than total_waypoint_num ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID, "The start index is greater than end index of upload wps ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM, "The end index of uplod wps is greater than inited total numbers ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE, "The index of first and end waypoint expected to download is not in range of stored in FC ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP, "Current position is far away from the first waypoint. ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_CLOSE, "It is too close from two adjacent waypoints",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_FAR, "The distance betwween two adjacent waypoints is not in[0.5m, 5000m]",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL, "The max vel of uplod wp is greater than global max vel ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX, "The local cruise vel of upload wp is greater than local max vel ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX, "The local cruise vel of upload wp is greater than global max vel ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL, "Global_max_vel is greater than permitted_max_vel or less than permitted_min_vel ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL, "Global_cruise_vel is greater than global_max_vel ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG, "Goto_first_point_mode is out of range of waypoint_goto_first_flag_t_enum ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION, "Finished_action is out of range of wp_plan_finish_action_t_enum ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION, "Rc_lost_action is out of range of wp_plan_rc_lost_action_t_enum ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID, "The yaw mode of upload wp is invalid. reference to waypoint2_yaw_mode_t defined in math_waypoint_planner.h ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE, "The yaw command of upload wp is not in range. the range for MR:[-180 180]",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID, "The yaw turn direction of upload wp is invalid. it should be 0:clockwise or 1:anti-clockwise ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID, "The wp type of upload wp is invalid. reference to waypoint_type_t defined in math_waypoint_planner.h ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GO_STOP_CMD_INVALID, "Go/stop command is invalid. ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD, "The command of pause/recovery is not equal to any of the command enum ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD, "The command of break/restore is not equal to any of the command enum ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_REF_POINT, "Initial reference point position coordinate exceed set range ",NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS,  "The damping dis is greater than or equal the distance of adjacent point ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE, "Cann't set wp_line_exit type to wp ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INFO_NOT_UPLOADED, "The init info of Ground Station is not uploaded yet ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_HAS_NOT_UPLOADED, "The wp has not uploaded yet ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH, "Min_initial_waypoint_num is not uploaded. ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_HAS_STARTED, "Waypoint plan has started when received go command. ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING, "Waypoint plan not running when received stop command. ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY, "Ground station(GS) is not started(used by pause/recovery) ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE, "Ground station(GS) is not started(used by break/restore) ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NOT_IN_WP_MIS, "Not in the waypoint mission(MIS)(cannot pause/recovery or break/restore) ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_PAUSED, "The current status is paused",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_PAUSED, "Not in paused status",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_BROKEN, "The current status is broken",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_BROKEN, "Not in break status",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED, "The configuration forbid using pause/recovery API ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED, "The configuration forbid using break/restore API ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_BREAK_POINT, "No break point is recorded for restore ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_CUR_TRAJ_PROJECT, "No current trajectory project point is recorded for restore ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NXT_TRAJ_PROJECT, "No next trajectory project point is recorded for restore ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NNT_TRAJ_PROJECT, "No next the next trajectory project point is recorded for restore ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE, "The index of upload wp is not continue after the store wp ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP, "The WP_LINE_ENTER wp_type set to a wp which is not the init start waypoint ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED, "The waypoint plan has started when initializing waypoint ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE, "Waypoint damping distance exceed set range ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE, "Waypoint position coordinate exceed rational range ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO, "First waypoint type error",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT, "Waypoint position exceed radius limit ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT, "Waypoint position exceed height limit ",NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_WP_MIS_CHECK_FAIL, "Head_node is null or atti_not_healthy or gyro_not_healthy or horiz_vel_not healthy or horiz_abs_pos_not_healthy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_HOME_NOT_RECORDED, "The home point is no recorded yet	 which will be executed at the first time of GPS level > 3(MR	FW). ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_LOW_LOCATION_ACCURACY, "Current location accuracy is low for bad GPS signal. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RTK_CONDITION_IS_NOT_READY, "Use rtk_data	 but rtk is not connected or rtk_data is invalid ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_CROSS_NFZ, "The trajectory cross the NFZ ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_BAT_LOW, "Current capacity of smart battery or voltage of non-smart battery is lower than level 1 or level 2 threshold ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED, "The ID of Action is duplicated. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH, "There is no enough memory space for new Action Item ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE, "The size of buffer used to get the info of Action is less than the size of Action. Normally users can not get this. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND, "The ID of Action is not found. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR, "The download action start id is bigger than the action end id ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED, "Can not download or get min-max action ID for no action items stored in action kernel ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_TYPE_INVALID, "The type ID of Trigger is invalid. It might not defined or the information is empty. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX, "Wp_end_index is less than wp_start_index in reach_waypoint_trigger. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM, "Interval_wp_num is large than the difference of wp_start_index and wp_end_index in reach_waypoint_trigger. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM , "Auto_terminate_wp_num is large than interval_wp_num in reach_waypoint_trigger. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE, "The associate_type is greater than the maximum value. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE, "The interval type is greater than the maximum value. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED, "The execution of Actuator is not supported	 e.g.	 try to stop camera shooting. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID, "The type ID of Actuator is invalid. It might not defined or the information is empty. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID, "The Function ID of Actuator is invalid. It might not defined or the information is empty. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL, "Fail to send shot cmd to camera for no camera or camera is busy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL, "Fail to send video start cmd to camera for no camera or camera is busy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL, "Fail to send video stop cmd to camera for no camera or camera is not busy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID, "Camera focus param xy exceed valid range (0	 1). ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL, "Fail to send focus cmd to camera for no camera or camera is busy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL, "Fail to send focalize cmd to camera for no camera or camera is busy. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID, "Focal distance of camera focalize function exceed valid range. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_EXEC_FAIL, "This err code indicate camera fail to exec coressponding cmd	 and the low 8 bit", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD, "Gimbal roll	pitch yaw angle ctrl cmd param invalid	", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD, "Gimbal duration param invalid	 unable to exec. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE, "Gimbal fail to arrive target angle . ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL, "Fail to send cmd to gimbal for gimbal is busy or no gimbal. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL , "Fail to stop gimbal uniform ctrl because index error. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_RESV, "Reserved error code", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE, "Yaw angle is lager max yaw angle. ", NULL}, \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT, "Failed to target yaw angle because of timeout. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED, "Failed to target yaw angle because of timeout. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL, "Failed to current and target vel not equal. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_RESV, "Failed to payload reserved error code. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD, "Failed to sned cmd to payload. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED, "Failed to execute payload actuator. ", NULL},    \
/* waypoint v3 module error message */   \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_COMMON_SUCCESS, "Execute waypoint v3 cmd successfully. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_MISSION_ID_NOT_EXIST, "Execute waypoint v3 mission id not match. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WAYLINE_INFO_ERROR , "Execute waypoint v3 wayline info error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WPMZ_FILE_VERSION_NOT_MATCH, "Execute waypoint v3 wpmz file not match. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WPMZ_FILE_LOAD_ERROR, "Load waypoint v3 wpmz file error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_NO_BREAK_INFO, "Execute waypoint v3 no break info error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CMD_INVALID, "Execute waypoint v3 cmd invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_START_WAYLINE_WHEN_WAYLINE_RUNNING, "Cannot start wayline when wayline running. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_BREAK_WAYLINE_IN_CUR_STATE, "Cannot break wayline in current state. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_STOP_WAYLINE_WHEN_WAYLINE_NOT_RUNNING, "Cannot stop wayline when wayline not running. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_BREAK_WAYLINE_WHEN_WAYLINE_NOT_RUNNING, "Cannot break wayline when wayline not running. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_REQUEST_DRONE_CONTROL, "Flight mission conflict, unable to obtain control auth of the aircraft. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_RESUME_WAYLINE_IN_CUR_STATE, "Failed to resume wayline in current state. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HEIGHT_LIMIT, "Execute waypoint v3 failed due to height limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RADIUS_LIMIT, "Execute waypoint v3 failed due to radius limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CROSS_FLYLIMIT_AERA, "Execute waypoint v3 failed due to cross flylimit area. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_LOW_LIMIT, "Execute waypoint v3 failed due to low limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_OBSTACAL_STOP, "Execute waypoint v3 failed due to obstacal stop. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RTK_DISCONNECT, "Execute waypoint v3 failed due to rtk disconnect. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_BOUNDARY_LIMIT, "Execute waypoint v3 failed due to boundary limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RC_PITCH_ROLL_BREAK, "Execute waypoint v3 failed due to rc pitch roll break. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AIRPORT_HEIGHT_LIMIT, "Execute waypoint v3 failed due to airport height limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_TAKEOFF_FAIL, "Failed to request take off. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AUTOTAKEOFF_RUN_FAIL, "Failed to run auto take off. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_WAYLINE_FAIL, "Failed to request wayline. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AGRO_PLAN_FAIL, "Failed to agro plan. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_QUICK_TAKEOFF_ASSIST_FAIL, "Failed to request quick take off assist. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_QUICK_TAKEOFF_ASSIST_RUN_FAIL, "Failed to run quick takeoff assist. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_VFENCE_LIMIT, "Execute waypoint v3 failed due to vfence limit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_GPS_INVALID, "Execute waypoint v3 failed due to gps invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_START_AT_CURRENT_RC_MODE, "Failed to start at current rc mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HOME_POINT_NOT_RECORDED, "Execute waypoint v3 failed due to homepoint not recorded. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_LOWER_BATTERY, "Execute waypoint v3 failed due to lower battery. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RETURN_HOME, "Execute waypoint v3 failed due to return home. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ADSB_ERROR, "Execute waypoint v3 failed due to adsb error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RC_LOST, "Execute waypoint v3 failed due to rc lost. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RTK_NOT_READY, "Execute waypoint v3 failed due to rtk not ready. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_IS_MOVING, "Execute waypoint v3 failed due to drone is moving. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_ON_GROUND_MOTOR_ON, "Execute waypoint v3 failed due to drone on ground motor on. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_CAMERA_INVALID, "Execute waypoint v3 failed due to surface follow camera invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_HEIGHT_INVALID, "Execute waypoint v3 failed due to surface follow height invalid.", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_MAP_WRONG, "Execute waypoint v3 failed due to surface follow map wrong. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HOMEPOINT_NOT_MATCH_RTK, "Execute waypoint v3 failed due to homepoint not match rtk. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_STRONG_WIND_GOHOME, "Execute waypoint v3 failed due to strong wind gohome. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_CRITICAL_ERROR, "Execute waypoint v3 failed due to drone critical error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_FIND_PAYLOAD, "Execute waypoint v3 failed due to cannot find payload. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_EXECUTION_FAILED, "Execute waypoint v3 failed due to action execution failed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_FARM_NO_PESTICIDE, "Execute waypoint v3 failed due to farm no pesticide. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RADAR_DISCONNECT, "Execute waypoint v3 failed due to radar disconnect. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_EXIT, "Execute waypoint v3 failed due to user exit. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_BREAK, "Execute waypoint v3 failed due to user break. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_SET_GOHOME, "Execute waypoint v3 failed due to user set gohome. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_AGRO_PLANNER_STATE_CHANGE, "Execute waypoint v3 failed due to user agro panner state change. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_SWITCH_RC_MODE, "Execute waypoint v3 failed due to user switch rc mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INIT_FAIL, "Execute waypoint v3 failed due to traj init failed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_JOB_EXIT_BUT_MIS_RUNNING, "Execute waypoint v3 failed due to traj job exit but mis running. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_ON_GROUND_MOTOR_ON_CANNOT_GO, "Execute waypoint v3 failed due to traj on ground motor is turn on. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_START_INDEX_OR_PROG, "Execute waypoint v3 failed due to traj invalid start index. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_CSYS_MODE, "Execute waypoint v3 failed due to traj invalid csys mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_HEIGHT_MODE, "Execute waypoint v3 failed due to traj invalid height mode. ", NULL},\
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_FLY_WP_MODE, "Execute waypoint v3 failed due to traj invalid fly wp mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_YAW_MODE, "Execute waypoint v3 failed due to traj invalid yaw mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_TURN_DIR_MODE, "Execute waypoint v3 failed due to traj invalid turn dir mode. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_WP_TYPE, "Execute waypoint v3 failed due to traj invalid wp type. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_FIR_LAS_WP_TYPE_ERROR, "Execute waypoint v3 failed due to fir las wp type error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GLOB_VEL_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj wp global vel out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_NUM_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj wp num out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LAT_LONG_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj lat or lon out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_DAMP_DIS_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj damp dis out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_MAX_VEL_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj max vel out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_VEL_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj vel out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_YAW_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj wp yaw out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_YAW_MODE_IN_VERT_SEGM, "Execute waypoint v3 failed due to traj invalid yaw mode in vert segm. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_MISSION_ID_CHANGED, "Execute waypoint v3 failed due to traj wp break info mission id changed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_PROGRESS_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj wp break info progress out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_INVALID_MISSION_STATE, "Execute waypoint v3 failed due to traj wp break info invalid mission state. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_WP_INDEX_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj wp break info wp index out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_BREAK_LAT_LONG_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj break lat or lon out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_BREAK_INFO_WP_YAW_OUT_OF_RANGE, "Execute waypoint v3 failed due to traj break info wp yaw out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_BREAK_INFO_FLAG , "Execute waypoint v3 failed due to traj invalid break info flag. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GET_TRAJ_INFO_FAILED, "Execute waypoint v3 failed due to traj get info failed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GENERATE_FAIL, "Execute waypoint v3 failed due to traj generate failed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LIB_RUN_FAIL, "Execute waypoint v3 failed due to traj lib run failed. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LIB_EMERGENCY_BRAKE, "Execute waypoint v3 failed due to traj lib emergency brake. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NOT_FOUND, "Execute waypoint v3 failed due to action not found. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INDEX_REPEATED, "Execute waypoint v3 failed due to action index repeated. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INFO_SIZE_TOO_LONG_OR_TOO_SHORT, "Execute waypoint v3 failed due to size too long or too short. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_EMPTY, "Execute waypoint v3 failed due to action tree empty. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_EMPTY, "Execute waypoint v3 failed due to action tree layer empty. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_ID_REPEATED, "Execute waypoint v3 failed due to action repeated. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NODE_CHILDREN_NUM_LT_2, "Execute waypoint v3 failed due to action mode children num less than 2. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INDEX_OUT_OF_RANGE, "Execute waypoint v3 failed due to action index out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_ID_IS_65535, "Execute waypoint v3 failed due to action id is 65535. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NODE_CHILDNUM_SUM_NOT_EQ_NEXT_LAYER_SIZE, "Execute waypoint v3 failed due to action node child sum not equal next layer size. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_MORE, "Execute waypoint v3 failed due to action tree layer num too more. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_LESS, "Execute waypoint v3 failed due to action tree layer num too less. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_GROUP_NUM_OUT_OF_RANGE, "Execute waypoint v3 failed due to action group num out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_GROUP_VALID_RANGE_ERROR, "Execute waypoint v3 failed due to action group valid range error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_ROOT_STATUS_INVALID, "Execute waypoint v3 failed due to action tree root status invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_NODE_STATUS_INVALID, "Execute waypoint v3 failed due to action tree mode status invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_OUT_OF_RANGE, "Execute waypoint v3 failed due to break info action group id out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_STATUS_TREE_SIZE_ERROR, "Execute waypoint v3 failed due to action status tree size error. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_TRIGGER_RUN_RESULT_INVALID, "Execute waypoint v3 failed due to break info trigger run result invalid. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_REPEATED, "Execute waypoint v3 failed due to break info action group id repeated. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_REPEATED, "Execute waypoint v3 failed due to break info action location repeated. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_OUT_OF_RANGE, "Execute waypoint v3 failed due to break info action location out of range. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_RESUME_ID_NOT_IN_BREAK_INFO, "Execute waypoint v3 failed due to resume id not in break info. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_RESUME_INFO_MODIFY_ACTION_STATUS_FROM_NO_INTERRUPT_TO_INTERRUPT, "Execute waypoint v3 failed due to modify action status from no interrupt to interrupt. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_RESUME_FAIL_FOR_INVALID_RESUME_INFO, "Execute waypoint v3 failed due to action resume failed for invalid resume info. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_NOT_FOUND, "Execute waypoint v3 failed due to actuator not found. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRIGGER_NOT_FOUND, "Execute waypoint v3 failed due to trigger not found. ", NULL},    \
{ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRIGGER_SINGLE_TIME_CHECK_FAIL, "Execute waypoint v3 failed due to single time check failed. ", NULL},    \

#define ZIYAN_RETURN_CODE_OK                 ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS             /*!< Payload SDK return code represents as status is ok. */
#define ZIYAN_RETURN_CODE_ERR_ALLOC          ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED /*!< Payload SDK return code represents as status alloc error. */
#define ZIYAN_RETURN_CODE_ERR_TIMEOUT        ZIYAN_ERROR_SYSTEM_MODULE_CODE_TIMEOUT             /*!< Payload SDK return code represents as status timeout error. */
#define ZIYAN_RETURN_CODE_ERR_NOT_FOUND      ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND           /*!< Payload SDK return code represents as status not found error. */
#define ZIYAN_RETURN_CODE_ERR_OUT_OF_RANGE   ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE        /*!< Payload SDK return code represents as status out of range error. */
#define ZIYAN_RETURN_CODE_ERR_PARAM          ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER   /*!< Payload SDK return code represents as status parameter error. */
#define ZIYAN_RETURN_CODE_ERR_SYSTEM         ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR        /*!< Payload SDK return code represents as status system error. */
#define ZIYAN_RETURN_CODE_ERR_BUSY           ZIYAN_ERROR_SYSTEM_MODULE_CODE_BUSY                /*!< Payload SDK return code represents as status busy error. */
#define ZIYAN_RETURN_CODE_ERR_UNSUPPORT      ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT          /*!< Payload SDK return code represents as status nonsupport error. */
#define ZIYAN_RETURN_CODE_ERR_UNKNOWN        ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN             /*!< Payload SDK return code represents as status unknown error. */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief ZIYAN module enum for defining error code.
 */
typedef enum {
    ZIYAN_ERROR_MODULE_SYSTEM = 0,
    ZIYAN_ERROR_MODULE_PLATFORM,
    ZIYAN_ERROR_MODULE_LOGGER,
    ZIYAN_ERROR_MODULE_TIME_SYNC,
    ZIYAN_ERROR_MODULE_COMMAND,
    ZIYAN_ERROR_MODULE_CAMERA,
    ZIYAN_ERROR_MODULE_GIMBAL,
    ZIYAN_ERROR_MODULE_XPORT,
    ZIYAN_ERROR_MODULE_PAYLOAD_COLLABORATION,
    ZIYAN_ERROR_MODULE_WIDGET,
    ZIYAN_ERROR_MODULE_CORE,
    ZIYAN_ERROR_MODULE_IDENTITY,
    ZIYAN_ERROR_MODULE_TRANSMISSION,
    ZIYAN_ERROR_MODULE_DATA_CHANNEL,
    ZIYAN_ERROR_MODULE_SUBSCRIPTION,
    ZIYAN_ERROR_MODULE_MOP_CHANNEL,
    ZIYAN_ERROR_MODULE_POSITIONING,
    ZIYAN_ERROR_MODULE_POWER_MANAGEMENT,
    ZIYAN_ERROR_MODULE_AIRCRAFTINFO,
    ZIYAN_ERROR_MODULE_PRODUCTINFO,
    ZIYAN_ERROR_MODULE_FLOWCONTROLLER,
    ZIYAN_ERROR_MODULE_DOWNLOADER,
    ZIYAN_ERROR_MODULE_PARAMETER,
    ZIYAN_ERROR_MODULE_UTIL,
    ZIYAN_ERROR_MODULE_USER,
    ZIYAN_ERROR_MODULE_NEGOTIATE,
    ZIYAN_ERROR_MODULE_UPGRADE,
    ZIYAN_ERROR_MODULE_FC_BASIC,
    ZIYAN_ERROR_MODULE_FC_JOYSTICK,
    ZIYAN_ERROR_MODULE_FC_ACTION,
    ZIYAN_ERROR_MODULE_FC_ARREST_FLYING,
    ZIYAN_ERROR_MODULE_FC_HOME_LOCATION,
    ZIYAN_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR,
    ZIYAN_ERROR_MODULE_CAMERA_MANAGER,
    ZIYAN_ERROR_MODULE_GIMBAL_MANAGER,
    ZIYAN_ERROR_MODULE_WAYPOINT_V2,
    ZIYAN_ERROR_MODULE_WAYPOINT_V3,
    ZIYAN_ERROR_MODULE_ERROR,
} E_ZiyanErrorModule;

/**
 * @brief Raw error code of system module.
 */
typedef enum {
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_SUCCESS = 0x000,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_REQUEST_PARAMETER = 0x0D4,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_EXECUTING_HIGHER_PRIORITY_TASK = 0x0D7,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT = 0x0E0,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_TIMEOUT = 0x0E1,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_MEMORY_ALLOC_FAILED = 0x0E2,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_PARAMETER = 0x0E3,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT_IN_CURRENT_STATE = 0x0E4,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_SYSTEM_ERROR = 0x0EC,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_MODULE_INACTIVATED = 0xEE, /*!< Module is too not activated yet */
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_HARDWARE_ERR = 0x0FA,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INSUFFICIENT_ELECTRICITY = 0x0FB,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_UNKNOWN = 0x0FF,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NOT_FOUND = 0x100,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_OUT_OF_RANGE = 0x101,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_BUSY = 0x102,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_DUPLICATE = 0x103,
    ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_ADAPTER_NOT_MATCH = 0x104,
} E_ZiyanErrorSystemModuleRawCode;

/**
 * @brief Raw error code of activation of identity module.
 */
typedef enum {
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_PARAMETER_ERROR = 0x0001,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ENCODE_ERROR = 0x0002,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NEW_DEVICE_ERROR = 0x0003,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED = 0x0004,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NETWORK_ERROR = 0x0005,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SERVER_ACCESS_REFUSED = 0x0006,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ACCESS_LEVEL_ERROR = 0x0007,
    ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_OSDK_VERSION_ERROR = 0x0008,
} E_ZiyanErrorIdentityModuleActivationRawCode;

/**
 * @brief Raw error code of gimbal module.
 */
typedef enum {
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_POSITIVE_LIMIT = 0x000,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_NEGATIVE_LIMIT = 0x001,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_POSITIVE_LIMIT = 0x002,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_NEGATIVE_LIMIT = 0x003,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_POSITIVE_LIMIT = 0x004,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_NEGATIVE_LIMIT = 0x005,
    ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_NON_CONTROL_AUTHORITY = 0x006,
} E_ZiyanErrorGimbalModuleRawCode;

/**
 * @brief Raw error code of payload collaboration module.
 */
typedef enum {
    ZIYAN_ERROR_PAYLOAD_COLLABORATION_MODULE_RAW_CODE_POSITION_NOT_MATCH = 0x000,
} E_ZiyanErrorPayloadCollaborationModuleRawCode;

/**
 * @brief Raw error code of subscription module.
 */
typedef enum {
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_INVALID_TOPIC_FREQ = 0x000,
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_DUPLICATE = 0x001,
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUBSCRIBED = 0x002,
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TIMESTAMP_NOT_ENABLE = 0x003,
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUPPORTED = 0x004,
} E_ZiyanErrorSubscriptionModuleRawCode;

/**
 * @brief Raw error code of mop channel module.
 */
typedef enum {
    ZIYAN_ERROR_MOP_CHANNEL_MODULE_RAW_CODE_CONNECTION_CLOSE = 0x00B,
} E_ZiyanErrorMopChannelModuleRawCode;

/**
 * @brief Raw error code of flight controller module.
 */
typedef enum {
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_MODE_ERROR = 0x0000,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_SUCCESS = 0x0001,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_SUCCESS = 0x0002,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_IN_PROGRESS = 0x0003,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_IN_PROGRESS = 0x0004,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_P = 0x0005,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_F = 0x0006,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_PARAM_READ_WRITE_INVALID_PARAM = 0x0007,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_IOC_OBTAIN_CONTROL_ERROR = 0x00C9,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_KEY_ERROR = 0xFF00,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_NO_AUTHORIZATION_ERROR = 0xFF01,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_NO_RIGHTS_ERROR = 0xFF02,
    ZIYAN_ERROR_FC_MODULE_RAW_CODE_SYSTEM_ERROR = 0xFF03,
} E_ZiyanErrorFCModuleRawCode;

/**
 * @brief Raw error code of flight controller joystick module.
 */
typedef enum {
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_SUCCESS = 0x00,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_DEVICE_NOT_ALLOW = 0x01,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_ENGINE_STARTING = 0x02,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_TAKING_OFF = 0x03,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_LANDING = 0x04,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_INVALID = 0x05,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_RC_NOT_P_MODE = 0x06,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_LENGTH_ERROR = 0x07,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_HAS_NO_JOYSTICK_AUTHORITY = 0x08,
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_IN_RC_LOST_ACTION = 0x09,
} E_ZiyanErrorFCJoystickModuleRawCode;

/**
 * @brief Raw error code of flight controller action module.
 */
typedef enum {
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_ON = 0x01,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_OFF = 0x02,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_AIR = 0x03,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_NOT_IN_AIR = 0x04,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_HOME_POINT_NOT_SET = 0x05,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_BAD_GPS = 0x06,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_SIMULATION = 0x07,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_CANNOT_START_MOTOR = 0x11,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_LOW_VOLTAGE = 0x12,
    ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_SPEED_TOO_LARGE = 0x14,
} E_ZiyanErrorFCActionModuleRawCode;

/**
 * @brief Raw error code of flight controller arrest flying module.
 */
typedef enum {
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_SET_SUCCESS = 0x00,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_NULL_POINTER = 0x01,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_HAS_REGISTED = 0x02,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_NOT_FIND = 0x03,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_ADD_ITEM_NO_DECRIPTION = 0x04,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_ID_INVALID = 0x05,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE = 0x06,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_INVALID_FORMAT_HMSCODE = 0x07,
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMSCODE_NOT_IN_WHITE_TABLE = 0x08,
} E_ZiyanErrorFCArrestFlyingModuleRawCode;

/**
 * @brief Raw error code of flight controller home location module.
 */
typedef enum {
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_UNKNOWN_FAILED_REASON = 0x01,
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_INVALID_GPS_COORDINATE = 0x02,
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_NOT_BE_RECORD = 0x03,
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_GPS_NOT_READY = 0x04,
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_DIS_TOO_FAR = 0x05,
} E_ZiyanErrorFCHomeLocationModuleRawCode;

/**
 * @brief Raw error code of flight controller emergency stop motor module.
 */
typedef enum {
    ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_VERSION_NOT_MATCH = 0x01,
    ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_CMD_INVALID = 0x02,
} E_ZiyanErrorFCKillSwitchModuleRawCode;

/**
 * @brief Raw error code of camera manager
 */
typedef enum {
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND = 0xE0, /*!< Do not support this command */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_TIMEOUT = 0xE1, /*!< Execution timeout */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED = 0xE2, /*!< Memory alloc failed */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER = 0xE3, /*!< Invalid parameter for the command */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = 0xE4, /*!< Do not support this command in the current state */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = 0xE5, /*!< Timestamp of camera is not synchronized */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED = 0xE6, /*!< Setting parameter failed */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED = 0xE7, /*!< Getting parameter failed */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING = 0xE8, /*!< SD card is not installed */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL = 0xE9, /*!< SD card is full */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR = 0xEA, /*!< Error accessing the SD Card */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR = 0xEB, /*!< Sensor go wrong */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR = 0xEC, /*!< System error */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG = 0xED, /*!< Length of the parameter is too long */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = 0xF0, /*!< Fireware data number is a discontinuous number */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR = 0xF2, /*!< Error verifying fireware */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR = 0xF4, /*!< Error writing flash */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH = 0xF6, /*!< Firmware type don't match */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED = 0xF8, /*!< Not connect remote control yet */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR = 0xFA, /*!< Hardware fault */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED = 0xFC, /*!< Aircraft is not connected yet */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE = 0xFE, /*!< Cannot upgrade in current status (Please reboot or contact with ZIYAN support */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR = 0xFF, /*!< Undefined error */
} E_ZiyanErrorCameraManagerModuleRawCode;

/**
 * @brief Raw error code of gimbal manager
 */
typedef enum {
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND = 0xE0, /*!< Do not support this command */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_TIMEOUT = 0xE1, /*!< Execution timeout */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED = 0xE2, /*!< Memory alloc failed */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER = 0xE3, /*!< Invalid parameter for the command */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = 0xE4, /*!< Do not support this command in the current state */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = 0xE5, /*!< Timestamp of camera is not synchronized */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED = 0xE6, /*!< Setting parameter failed */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED = 0xE7, /*!< Getting parameter failed */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING = 0xE8, /*!< SD card is not installed */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL = 0xE9, /*!< SD card is full */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR = 0xEA, /*!< Error accessing the SD Card */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR = 0xEB, /*!< Sensor go wrong */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR = 0xEC, /*!< System error */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG = 0xED, /*!< Length of the parameter is too long */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = 0xF0, /*!< Fireware data number is a discontinuous number */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR = 0xF2, /*!< Error verifying fireware */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR = 0xF4, /*!< Error writing flash */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH = 0xF6, /*!< Firmware type don't match */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED = 0xF8, /*!< Not connect remote control yet */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR = 0xFA, /*!< Hardware fault */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED = 0xFC, /*!< Aircraft is not connected yet */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE = 0xFE, /*!< Cannot upgrade in current status (Please reboot or contact with ZIYAN support */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR = 0xFF, /*!< Undefined error */
} E_ZiyanErrorGimbalManagerModuleRawCode;

/**
 * @brief Waypoint v2 total error raw code.
 */
typedef enum {
    /* Waypoint v2 common error raw code */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_SUCCESS = 0x00000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALID_DATA_LENGTH = 0x00001,   /*!< the length of the data is illegal based on the protocol */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALD_FLOAT_NUM = 0x00002,   /*!< invalid float number (NAN or INF) */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_VERSION_NO_MATCH = 0x00003,   /*!< waypoint mission version can't match with firmware*/
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_UNKNOWN = 0x0ffff,   /*!< Fatal error! Unexpected result! */

    /* The enum that defines the detail error code of flight-trajectory module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_RESV = 0x10000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_MANY = 0x10001,   /*!< min_initial_waypoint_num is large than permitted_max_waypoint_num */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_FEW = 0x10002,   /*!< min_initial_waypoint_num is less than permitted_min_waypoint_num */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_END_INDEX = 0x10003,   /*!< waypoint_end_index is equal or large than total_waypoint_num */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID = 0x10004,   /*!< the start index is greater than end index of upload wps */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM = 0x10005,   /*!< the end index of uplod wps is greater than inited total numbers */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE = 0x10006,   /*!< the index of first and end waypoint expected to download is not in range of stored in FC */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP = 0x10008,   /*!< current position is far away from the first waypoint. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_CLOSE = 0x1000a,   /*!< it is too close from two adjacent waypoints, the value of which might be distinguish from products */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_FAR = 0x1000b,   /*!< the distance betwween two adjacent waypoints is not in[0.5m, 5000m] the value of which might be distinguish from products */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL = 0x1000c,   /*!< the max vel of uplod wp is greater than global max vel */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX = 0x1000d,   /*!< the local cruise vel of upload wp is greater than local max vel */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX = 0x1000e,   /*!< the local cruise vel of upload wp is greater than global max vel */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL = 0x1000f,   /*!< global_max_vel is greater than permitted_max_vel or less than permitted_min_vel */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL = 0x10010,   /*!< global_cruise_vel is greater than global_max_vel */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG = 0x10011,   /*!< goto_first_point_mode is out of range of waypoint_goto_first_flag_t_enum */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION = 0x10012,   /*!< finished_action is out of range of wp_plan_finish_action_t_enum */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION = 0x10013,   /*!< rc_lost_action is out of range of wp_plan_rc_lost_action_t_enum */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID = 0x10014,   /*!< the yaw mode of upload wp is invalid. reference to waypoint2_yaw_mode_t defined in math_waypoint_planner.h */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE = 0x10015,   /*!< the yaw command of upload wp is not in range. the range for MR:[-180 180],unit:degree */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID = 0x10016,   /*!< the yaw turn direction of upload wp is invalid. it should be 0:clockwise or 1:anti-clockwise */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID = 0x10017,   /*!< the wp type of upload wp is invalid. reference to waypoint_type_t defined in math_waypoint_planner.h */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GO_STOP_CMD_INVALID = 0x10018,   /*!< go/stop command is invalid. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD = 0x10019,   /*!< the command of pause/recovery is not equal to any of the command enum */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD = 0x1001a,   /*!< the command of break/restore is not equal to any of the command enum */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_REF_POINT = 0x1001b,   /*!< initial reference point position coordinate exceed set range */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS = 0x1001c,    /*!< the damping dis is greater than or equal the distance of adjacent point */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE = 0x1001d,   /*!< cann't set wp_line_exit type to wp */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INFO_NOT_UPLOADED = 0x1001e,   /*!< the init info of Ground Station is not uploaded yet */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_HAS_NOT_UPLOADED = 0x1001f,   /*!< the wp has not uploaded yet */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH = 0x10020,   /*!< min_initial_waypoint_num is not uploaded. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_HAS_STARTED = 0x10021,   /*!< waypoint plan has started when received go command. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING = 0x10022,   /*!< waypoint plan not running when received stop command. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY = 0x10023,   /*!< ground station(GS) is not started(used by pause/recovery) */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE = 0x10024,   /*!< ground station(GS) is not started(used by break/restore) */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NOT_IN_WP_MIS = 0x10025,   /*!< not in the waypoint mission(MIS)(cannot pause/recovery or break/restore) */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_PAUSED = 0x10026,   /*!< the current status is paused, cannot command pause again */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_PAUSED = 0x10027,   /*!< not in paused status, cannot command recovery */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_BROKEN = 0x10028,   /*!< the current status is broken, cannot command break again */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_BROKEN = 0x10029,   /*!< not in break status, cannot command restore */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED = 0x1002a,   /*!< the configuration forbid using pause/recovery API */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED = 0x1002b,   /*!< the configuration forbid using break/restore API */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_BREAK_POINT = 0x1002c,   /*!< no break point is recorded for restore */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_CUR_TRAJ_PROJECT = 0x1002d,   /*!< no current trajectory project point is recorded for restore */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NXT_TRAJ_PROJECT = 0x1002e,   /*!< no next trajectory project point is recorded for restore */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NNT_TRAJ_PROJECT = 0x1002f,   /*!< no next the next trajectory project point is recorded for restore */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE = 0x10030,   /*!< the index of upload wp is not continue after the store wp */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP = 0x10031,   /*!< the WP_LINE_ENTER wp_type set to a wp which is not the init start waypoint */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED = 0x10032,   /*!< the waypoint plan has started when initializing waypoint */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE = 0x10033,   /*!< waypoint damping distance exceed set range */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE = 0x10034,   /*!< waypoint position coordinate exceed rational range */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO = 0x10035,   /*!< first waypoint type error, it can not be WP_TURN_NO */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT = 0x10038,   /*!< waypoint position exceed radius limit */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT = 0x10039,   /*!< waypoint position exceed height limit */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_EXCEED_HEIGHT_LIMIT = 0x10040,   /*!< the uploaded waypoint exceed height limit */

    /* The enum that defines the detail error code of flight-status module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RESV = 0x20000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_WP_MIS_CHECK_FAIL = 0x20001,   /*!< head_node is null or atti_not_healthy or gyro_not_healthy or horiz_vel_not healthy or horiz_abs_pos_not_healthy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_HOME_NOT_RECORDED = 0x20002,   /*!< the home point is no recorded yet, which will be executed at the first time of GPS level > 3(MR/FW). */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_LOW_LOCATION_ACCURACY = 0x20003,   /*!< current location accuracy is low for bad GPS signal. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RTK_CONDITION_IS_NOT_READY = 0x20005,   /*!< use rtk_data, but rtk is not connected or rtk_data is invalid */

    /* The enum that defines the detail error code of flight-secure module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_RESV = 0x30000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_CROSS_NFZ = 0x30001,   /*!< the trajectory cross the NFZ */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_BAT_LOW = 0x30002,   /*!< current capacity of smart battery or voltage of non-smart battery is lower than level 1 or level 2 threshold */

    /* The enum that defines the detail error code of action-common module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_RESV = 0x400000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED = 0x400001,   /*!< the ID of Action is duplicated. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH = 0x400002,   /*!< there is no enough memory space for new Action Item */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE = 0x400003,   /*!< the size of buffer used to get the info of Action is less than the size of Action. Normally users can not get this. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND = 0x400004,   /*!< the ID of Action is not found. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR = 0x400005,   /*!< the download action start id is bigger than the action end id */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED = 0x400006,   /*!< can not download or get min-max action ID for no action items stored in action kernel */

    /* The enum that defines the detail error code of trigger module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_RESV = 0x410000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_TYPE_INVALID = 0x410001,   /*!< the type ID of Trigger is invalid. It might not defined or the information is empty. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX = 0x410021,   /*!< wp_end_index is less than wp_start_index in reach_waypoint_trigger. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM = 0x410022,   /*!< interval_wp_num is large than the difference of wp_start_index and wp_end_index in reach_waypoint_trigger. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM = 0x410023,   /*!< auto_terminate_wp_num is large than interval_wp_num in reach_waypoint_trigger. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE = 0x410041,   /*!< the associate_type is greater than the maximum value.  */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE = 0x410081,   /*!< the interval type is greater than the maximum value. */

    /* The enum that defines the detail error code of actuator-common module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_RESV = 0x420000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED = 0x420001,   /*!< the execution of Actuator is not supported, e.g., try to stop camera shooting. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID = 0x420002,   /*!< the type ID of Actuator is invalid. It might not defined or the information is empty. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID = 0x420003,   /*!< the Function ID of Actuator is invalid. It might not defined or the information is empty. */

    /* The enum that defines the detail error code of action-camera module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_RESV = 0x430000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL = 0x430001,   /*!< fail to send shot cmd to camera for no camera or camera is busy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL = 0x430002,    /*!< fail to send video start cmd to camera for no camera or camera is busy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL = 0x430003,    /*!< fail to send video stop cmd to camera for no camera or camera is not busy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID = 0x430004,   /*!< camera focus param xy exceed valid range (0, 1). */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL = 0x430005,   /*!< fail to send focus cmd to camera for no camera or camera is busy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL = 0x430006,   /*!< fail to send focalize cmd to camera for no camera or camera is busy. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID = 0x430007,   /*!< focal distance of camera focalize function exceed valid range. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_EXEC_FAIL = 0x430100,   /*!< this err code indicate camera fail to exec coressponding cmd, and the low 8 bit
                                                                                  will be replaced by the return code from camera, for example: 0x01E0 means current cmd
                                                                                  is not supported, 0x01E8 means SD card is not inserted and so on, the detailed camera
                                                                                  return code could be found in camera protocal. */

    /* The enum that defines the detail error code of action-gimbal module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_RESV = 0x440000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD = 0x440001,   /*!< gimbal roll/pitch/yaw angle ctrl cmd param invalid, unable to exec. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD = 0x440002,   /*!< gimbal duration param invalid, unable to exec. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE = 0x440003,   /*!< gimbal fail to arrive target angle . */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL = 0x440004,   /*!< fail to send cmd to gimbal for gimbal is busy or no gimbal. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL = 0x440005, /*!< fail to stop gimbal uniform ctrl because index error.*/

    /* The enum that defines the detail error code of action-flight module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_RESV = 0x460000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE = 0x460001,   /*!< yaw angle is lager max yaw angle. */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT = 0x460002,   /*!< faile to target yaw angle, because of timeout.*/
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED = 0x460003,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL = 0x460004,

    /* The enum that defines the detail error code of payload module */
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_RESV = 0x470000,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD = 0x470001,
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED = 0x470002,
} E_ZiyanErrorWaypointV2ModuleRawCode;

typedef enum {
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SUCCESS = 0x0000,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_MISSION_ID_NOT_EXIST = 1,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WAYLINE_INFO_ERROR = 2,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WPMZ_FILE_VERSION_NOT_MATCH = 3,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WPMZ_FILE_LOAD_ERROR = 4,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_NO_BREAK_INFO = 5,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CMD_INVALID = 6,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_START_WAYLINE_WHEN_WAYLINE_RUNNING = 257,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_BREAK_WAYLINE_IN_CUR_STATE = 258,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_STOP_WAYLINE_WHEN_WAYLINE_NOT_RUNNING = 259,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_BREAK_WAYLINE_WHEN_WAYLINE_NOT_RUNNING = 260,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_REQUEST_DRONE_CONTROL = 261,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_RESUME_WAYLINE_IN_CUR_STATE = 262,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HEIGHT_LIMIT = 513,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RADIUS_LIMIT = 514,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CROSS_FLYLIMIT_AERA = 515,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_LOW_LIMIT = 516,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_OBSTACAL_STOP = 517,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RTK_DISCONNECT = 518,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_BOUNDARY_LIMIT = 519,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RC_PITCH_ROLL_BREAK = 520,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AIRPORT_HEIGHT_LIMIT = 521,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_TAKEOFF_FAIL = 522,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AUTOTAKEOFF_RUN_FAIL = 523,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_WAYLINE_FAIL = 524,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AGRO_PLAN_FAIL = 525,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_QUICK_TAKEOFF_ASSIST_FAIL = 526,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_QUICK_TAKEOFF_ASSIST_RUN_FAIL = 527,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_VFENCE_LIMIT = 528,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_GPS_INVALID = 769,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_START_AT_CURRENT_RC_MODE = 770,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HOME_POINT_NOT_RECORDED = 771,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_LOWER_BATTERY = 772,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RETURN_HOME = 773,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ADSB_ERROR = 774,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RC_LOST = 775,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RTK_NOT_READY = 776,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_IS_MOVING = 777,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_ON_GROUND_MOTOR_ON = 778,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_CAMERA_INVALID = 779,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_HEIGHT_INVALID = 780,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_MAP_WRONG = 781,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HOMEPOINT_NOT_MATCH_RTK = 782,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_STRONG_WIND_GOHOME = 784,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_CRITICAL_ERROR = 1023,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_FIND_PAYLOAD = 1025,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_EXECUTION_FAILED = 1026,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_FARM_NO_PESTICIDE = 1027,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RADAR_DISCONNECT = 1028,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_EXIT = 1281,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_BREAK = 1282,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_SET_GOHOME = 1283,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_AGRO_PLANNER_STATE_CHANGE = 1284,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_SWITCH_RC_MODE = 1285,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INIT_FAIL = 1536,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_JOB_EXIT_BUT_MIS_RUNNING = 1537,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_ON_GROUND_MOTOR_ON_CANNOT_GO = 1538,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_START_INDEX_OR_PROG = 1539,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_CSYS_MODE = 1540,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_HEIGHT_MODE = 1541,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_FLY_WP_MODE = 1542,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_YAW_MODE = 1543,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_TURN_DIR_MODE = 1544,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_WP_TYPE = 1545,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_FIR_LAS_WP_TYPE_ERROR = 1546,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GLOB_VEL_OUT_OF_RANGE = 1547,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_NUM_OUT_OF_RANGE = 1548,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LAT_LONG_OUT_OF_RANGE = 1549,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_DAMP_DIS_OUT_OF_RANGE = 1550,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_MAX_VEL_OUT_OF_RANGE = 1551,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_VEL_OUT_OF_RANGE = 1552,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_YAW_OUT_OF_RANGE = 1553,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_YAW_MODE_IN_VERT_SEGM = 1554,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_MISSION_ID_CHANGED = 1555,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_PROGRESS_OUT_OF_RANGE = 1556,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_INVALID_MISSION_STATE = 1557,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_WP_INDEX_OUT_OF_RANGE = 1558,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_BREAK_LAT_LONG_OUT_OF_RANGE = 1559,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_BREAK_INFO_WP_YAW_OUT_OF_RANGE = 1560,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_INFO_FLAG = 1561,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GET_TRAJ_INFO_FAILED = 1562,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GENERATE_FAIL = 1563,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LIB_RUN_FAIL = 1564,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LIB_EMERGENCY_BRAKE = 1565,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NOT_FOUND = 1588,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INDEX_REPEATED = 1591,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INFO_SIZE_TOO_LONG_OR_TOO_SHORT = 1592,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_EMPTY = 1593,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_EMPTY = 1594,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_REPEATED = 1595,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NODE_CHILDREN_NUM_LT_2 = 1596,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INDEX_OUT_OF_RANGE = 1597,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_IS_65535 = 1598,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NODE_CHILDNUM_SUM_NOT_EQ_NEXT_LAYER_SIZE = 1599,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_MORE = 1600,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_LESS = 1601,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_GROUP_NUM_OUT_OF_RANGE = 1602,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_GROUP_VALID_RANGE_ERROR = 1603,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_ROOT_STATUS_INVALID = 1604,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_NODE_STATUS_INVALID = 1605,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_OUT_OF_RANGE = 1606,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_STATUS_TREE_SIZE_ERROR = 1607,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_TRIGGER_RUN_RESULT_INVALID = 1608,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_REPEATED = 1609,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_REPEATED = 1610,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_OUT_OF_RANGE = 1611,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_RESUME_ID_NOT_IN_BREAK_INFO = 1612,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_RESUME_INFO_MODIFY_ACTION_STATUS_FROM_NO_INTERRUPT_TO_INTERRUPT = 1613,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_RESUME_FAIL_FOR_INVALID_RESUME_INFO = 1614,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_NOT_FOUND = 1634,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRIGGER_NOT_FOUND = 1649,
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRIGGER_SINGLE_TIME_CHECK_FAIL = 1650,
} E_ZiyanErrorWaypoint3ModuleRawCode;

//@formatter:off
/**
 * @brief ZIYAN error code complete works. Users can search all error messages here.
 * @details Please get error description, error reason and error recovery suggestion of every error code from
 * ::ZIYAN_ERROR_OBJECTS macro.
 */
enum ZiyanErrorCode {
    /* system module error code, including some common error code */
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_SUCCESS),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_REQUEST_PARAMETER = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_REQUEST_PARAMETER),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_EXECUTING_HIGHER_PRIORITY_TASK = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_EXECUTING_HIGHER_PRIORITY_TASK),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_TIMEOUT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_TIMEOUT),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_MEMORY_ALLOC_FAILED),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INVALID_PARAMETER),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_NONSUPPORT_IN_CURRENT_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NONSUPPORT_IN_CURRENT_STATE),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_SYSTEM_ERROR),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_HARDWARE_ERR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_HARDWARE_ERR),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_INSUFFICIENT_ELECTRICITY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_INSUFFICIENT_ELECTRICITY),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_UNKNOWN),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_NOT_FOUND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_NOT_FOUND),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_OUT_OF_RANGE),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_BUSY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_BUSY),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_DUPLICATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_DUPLICATE),
    ZIYAN_ERROR_SYSTEM_MODULE_CODE_ADAPTER_NOT_MATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SYSTEM, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_ADAPTER_NOT_MATCH),

    /* gimbal module error code */
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_POSITIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_POSITIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_PITCH_REACH_NEGATIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_PITCH_REACH_NEGATIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_POSITIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_POSITIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_ROLL_REACH_NEGATIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_ROLL_REACH_NEGATIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_POSITIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_POSITIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_YAW_REACH_NEGATIVE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_YAW_REACH_NEGATIVE_LIMIT),
    ZIYAN_ERROR_GIMBAL_MODULE_CODE_NON_CONTROL_AUTHORITY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL, ZIYAN_ERROR_GIMBAL_MODULE_RAW_CODE_NON_CONTROL_AUTHORITY),

    /* payload collaboration module error code */
    ZIYAN_ERROR_PAYLOAD_COLLABORATION_MODULE_CODE_POSITION_NOT_MATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_PAYLOAD_COLLABORATION, ZIYAN_ERROR_PAYLOAD_COLLABORATION_MODULE_RAW_CODE_POSITION_NOT_MATCH),

    /* activation error code of identity module */
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_PARAMETER_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_PARAMETER_ERROR),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ENCODE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ENCODE_ERROR),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NEW_DEVICE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NEW_DEVICE_ERROR),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SOFTWARE_NOT_CONNECTED),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_NETWORK_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_NETWORK_ERROR),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_SERVER_ACCESS_REFUSED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_SERVER_ACCESS_REFUSED),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_ACCESS_LEVEL_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_ACCESS_LEVEL_ERROR),
    ZIYAN_ERROR_IDENTITY_MODULE_CODE_ACTIVATE_OSDK_VERSION_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_IDENTITY, ZIYAN_ERROR_IDENTITY_MODULE_RAW_CODE_ACTIVATE_OSDK_VERSION_ERROR),

    /* subscription module error code */
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_INVALID_TOPIC_FREQ = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SUBSCRIPTION, ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_INVALID_TOPIC_FREQ),
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_DUPLICATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SUBSCRIPTION, ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_DUPLICATE),
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUBSCRIBED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SUBSCRIPTION, ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUBSCRIBED),
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TIMESTAMP_NOT_ENABLE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SUBSCRIPTION, ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TIMESTAMP_NOT_ENABLE),
    ZIYAN_ERROR_SUBSCRIPTION_MODULE_CODE_TOPIC_NOT_SUPPORTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_SUBSCRIPTION, ZIYAN_ERROR_SUBSCRIPTION_MODULE_RAW_CODE_TOPIC_NOT_SUPPORTED),

    /* mop channel module error code */
    ZIYAN_ERROR_MOP_CHANNEL_MODULE_CODE_CONNECTION_CLOSE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_MOP_CHANNEL, ZIYAN_ERROR_MOP_CHANNEL_MODULE_RAW_CODE_CONNECTION_CLOSE),

    /* Flight controller basic errors */
    ZIYAN_ERROR_FC_MODULE_CODE_RC_MODE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_MODE_ERROR),
    ZIYAN_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_SUCCESS= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_SUCCESS),
    ZIYAN_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_SUCCESS= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_SUCCESS),
    ZIYAN_ERROR_FC_MODULE_CODE_OBTAIN_CONTROL_IN_PROGRESS= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_OBTAIN_CONTROL_IN_PROGRESS),
    ZIYAN_ERROR_FC_MODULE_CODE_RELEASE_CONTROL_IN_PROGRESS= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_RELEASE_CONTROL_IN_PROGRESS),
    ZIYAN_ERROR_FC_MODULE_CODE_RC_NEED_MODE_P= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_P),
    ZIYAN_ERROR_FC_MODULE_CODE_RC_NEED_MODE_F= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_RC_NEED_MODE_F),
    ZIYAN_ERROR_FC_MODULE_CODE_PARAM_READ_WRITE_INVALID_PARAM= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_PARAM_READ_WRITE_INVALID_PARAM),
    ZIYAN_ERROR_FC_MODULE_CODE_IOC_OBTAIN_CONTROL_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_IOC_OBTAIN_CONTROL_ERROR),
    ZIYAN_ERROR_FC_MODULE_CODE_KEY_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_KEY_ERROR),
    ZIYAN_ERROR_FC_MODULE_CODE_NO_AUTHORIZATION_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_NO_AUTHORIZATION_ERROR),
    ZIYAN_ERROR_FC_MODULE_CODE_NO_RIGHTS_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_NO_RIGHTS_ERROR),
    ZIYAN_ERROR_FC_MODULE_CODE_SYSTEM_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_BASIC, ZIYAN_ERROR_FC_MODULE_RAW_CODE_SYSTEM_ERROR),

    /* Flight controller joystick errors */
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_OBTAIN_RELEASE_JOYSTICK_AUTH_SUCCESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_SUCCESS),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_DEVICE_NOT_ALLOW= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_DEVICE_NOT_ALLOW),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_TAKING_OFF= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_TAKING_OFF),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_LANDING= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_LANDING),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_CMD_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_INVALID),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_RC_NOT_P_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_RC_NOT_P_MODE),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_CMD_LENGTH_ERROR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_CMD_LENGTH_ERROR),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_HAS_NO_JOYSTICK_AUTHORITY= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_HAS_NO_JOYSTICK_AUTHORITY),
    ZIYAN_ERROR_FC_JOYSTICK_MODULE_IN_RC_LOST_ACTION= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_JOYSTICK, ZIYAN_ERROR_FC_JOYSTICK_MODULE_RAW_CODE_IN_RC_LOST_ACTION),

    /* Flight controller action errors */
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_ON= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_ON),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_MOTOR_OFF= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_MOTOR_OFF),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_AIR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_AIR),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_NOT_IN_AIR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_NOT_IN_AIR),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_HOME_POINT_NOT_SET= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_HOME_POINT_NOT_SET),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_BAD_GPS= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_BAD_GPS),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_IN_SIMULATION= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_IN_SIMULATION),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_CANNOT_START_MOTOR= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_CANNOT_START_MOTOR),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_LOW_VOLTAGE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_LOW_VOLTAGE),
    ZIYAN_ERROR_FC_ACTION_MODULE_TRIGGER_ERROR_SPEED_TOO_LARGE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ACTION, ZIYAN_ERROR_FC_ACTION_MODULE_RAW_CODE_TRIGGER_ERROR_SPEED_TOO_LARGE),

    /* Flight controller arrest flying errors */
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_SET_SUCCESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_SET_SUCCESS),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_LOGOUT_NULL_POINTER= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_LOGOUT_NULL_POINTER),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_HAS_REGISTED= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_HAS_REGISTED),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMS_CODE_NOT_FIND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMS_CODE_NOT_FIND),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_ADD_ITEM_NO_DECRIPTION = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_ADD_ITEM_NO_DECRIPTION),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_REGISTER_ID_INVALID= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_REGISTER_ID_INVALID),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_STOP_IN_AIR_HMSCODE_NOT_IN_WHITE_TABLE),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_INVALID_FORMAT_HMSCODE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_INVALID_FORMAT_HMSCODE),
    ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_HMSCODE_NOT_IN_WHITE_TABLE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_ARREST_FLYING, ZIYAN_ERROR_FC_ARREST_FLYING_MODULE_RAW_CODE_HMSCODE_NOT_IN_WHITE_TABLE),

    /* Flight controller home location errors */
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_UNKNOWN_FAILED_REASON= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_HOME_LOCATION, ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_UNKNOWN_FAILED_REASON),
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_INVALID_GPS_COORDINATE= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_HOME_LOCATION, ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_INVALID_GPS_COORDINATE),
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_NOT_BE_RECORD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_HOME_LOCATION, ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_NOT_BE_RECORD),
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_GPS_NOT_READY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_HOME_LOCATION, ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_GPS_NOT_READY),
    ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_DIS_TOO_FAR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_HOME_LOCATION, ZIYAN_ERROR_FC_HOME_LOCATION_MODULE_RAW_CODE_DIS_TOO_FAR),

    /* Flight controller emergency stop motor errors */
    ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_VERSION_NOT_MATCH= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR, ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_VERSION_NOT_MATCH),
    ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_CMD_INVALID= ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_FC_EMERGENCY_STOP_MOTOR, ZIYAN_ERROR_FC_EMERGENCY_STOP_MOTOR_MODULE_RAW_CODE_CMD_INVALID),

    /* Camera manager errors */
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_TIMEOUT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_TIMEOUT ),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_MISSING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_FULL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SD_CARD_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SENSOR_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_SYSTEM_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_MODULE_INACTIVATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_MODULE_INACTIVATED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_HARDWARE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE),
    ZIYAN_ERROR_CAMERA_MANAGER_MODULE_CODE_UNDEFINE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_CAMERA_MANAGER, ZIYAN_ERROR_CAMERA_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR),

    /* Gimbal manager errors */
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_TIMEOUT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_TIMEOUT ),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_RAM_ALLOCATION_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_RAM_ALLOCATION_FAILED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_INVALID_COMMAND_PARAMETER = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_INVALID_COMMAND_PARAMETER),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNSUPPORTED_COMMAND_IN_CUR_STATE),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_CAMERA_TIME_NOT_SYNCHRONIZED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CAMERA_TIME_NOT_SYNCHRONIZED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_SET_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_SET_FAILED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_GET_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_GET_FAILED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_MISSING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_MISSING),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_FULL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_FULL),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SD_CARD_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SD_CARD_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SENSOR_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SENSOR_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_SYSTEM_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_SYSTEM_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_PARAMETER_TOTAL_TOO_LONG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_PARAMETER_TOTAL_TOO_LONG),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_MODULE_INACTIVATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_SYSTEM_MODULE_RAW_CODE_MODULE_INACTIVATED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_DATA_NUM_DISCONTINUOUS),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_VERIFICATION_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_VERIFICATION_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FLASH_WRITE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FLASH_WRITE_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_FIRMWARE_TYPE_MISMATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_FIRMWARE_TYPE_MISMATCH),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_REMOTE_CONTROL_UNCONNECTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_REMOTE_CONTROL_UNCONNECTED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_HARDWARE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_HARDWARE_ERROR),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_AIRCRAFT_UNCONNECTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_AIRCRAFT_UNCONNECTED),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_CANNOT_UPGRADE_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_CANNOT_UPGRADE_IN_CUR_STATE),
    ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_CODE_UNDEFINE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_GIMBAL_MANAGER, ZIYAN_ERROR_GIMBAL_MANAGER_MODULE_RAW_CODE_UNDEFINE_ERROR),

	/* Waypoint v2 total errors */
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_SUCCESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_SUCCESS),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALID_DATA_LENGTH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALID_DATA_LENGTH),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_INVALD_FLOAT_NUM = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_INVALD_FLOAT_NUM),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_VERSION_NO_MATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_VERSION_NO_MATCH),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_COMMON_UNKNOWN = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_COMMON_UNKNOWN),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_MANY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_MANY),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_NUM_TOO_FEW = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_NUM_TOO_FEW  ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_END_INDEX = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_END_INDEX ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_START_ID_GT_END_ID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_END_ID_GT_TOTAL_NUM),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DOWNLOAD_WPS_NOT_IN_STORED_RAGNE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_CUR_POS_IS_FAR_AWAY_FROM_FIRST_WP),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_CLOSE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_CLOSE ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_ADJ_WPS_TOO_FAR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_ADJ_WPS_TOO_FAR  ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_MAX_VEL_GT_GLOBAL ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_LOCAL_MAX),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_LOCAL_CRUISE_VEL_GT_GLOBAL_MAX),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GLOBAL_MAX_VEL  ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GLOBAL_CRUISE_VEL_GT_MAX_VEL ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_GOTO_FIRST_FLAG ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_FINISHED_ACTION ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_RC_LOST_ACTION),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_MODE_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_CMD_NOT_IN_RANGE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_YAW_TURN_DIRECTION_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_TYPE_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GO_STOP_CMD_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GO_STOP_CMD_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_PAUSE_RECOVERY_CMD),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_RESTORE_CMD),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INVALID_REF_POINT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INVALID_REF_POINT),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_GE_DIS_OF_ADJ_POINTS),
    ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_CANNT_SET_WP_LINE_EXIT_TYPE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_INFO_NOT_UPLOADED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_INFO_NOT_UPLOADED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_HAS_NOT_UPLOADED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_HAS_NOT_UPLOADED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOADED_WP_NOT_ENOUGH),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_HAS_STARTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_HAS_STARTED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_PAUSE_RECOVERY),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_GS_NOT_RUNNING_FOR_BREAK_RESTORE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NOT_IN_WP_MIS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NOT_IN_WP_MIS ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_PAUSED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_PAUSED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_PAUSED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_PAUSED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_HAS_BEEN_BROKEN = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_HAS_BEEN_BROKEN),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_MIS_NOT_BROKEN = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_MIS_NOT_BROKEN),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_PAUSE_RECOVERY_NOT_SUPPORTED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_BREAK_RESTORE_NOT_SUPPORTED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_BREAK_POINT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_BREAK_POINT ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_CUR_TRAJ_PROJECT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_CUR_TRAJ_PROJECT),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NXT_TRAJ_PROJECT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NXT_TRAJ_PROJECT),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_NO_NNT_TRAJ_PROJECT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_NO_NNT_TRAJ_PROJECT),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_UPLOAD_WP_ID_NOT_CONTINUE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_LINE_ENTER_NOT_SET_TO_START_WP ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_INIT_WP_WHEN_PLAN_HAS_STARTED ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_DAMPING_DIS_EXCEED_RANGE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WAYPOINT_COOR_EXCEED_RANGE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_FIRST_WP_TYPE_IS_WP_TURN_NO),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_RADIUS_LIMIT ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRAJ_WP_EXCEED_HEIGHT_LIMIT ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_WP_MIS_CHECK_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_WP_MIS_CHECK_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_HOME_NOT_RECORDED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_HOME_NOT_RECORDED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_LOW_LOCATION_ACCURACY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_LOW_LOCATION_ACCURACY),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_STATUS_RTK_CONDITION_IS_NOT_READY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_STATUS_RTK_CONDITION_IS_NOT_READY),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_CROSS_NFZ = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_CROSS_NFZ),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_SECURE_BAT_LOW = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_SECURE_BAT_LOW),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_DUPLICATED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ITEMS_SPACE_NOT_ENOUGH),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_SIZE_GT_BUF_SIZE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_NOT_FOUND),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_DOWNLOAD_ACTION_ID_RANGE_ERROR),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTION_COMMON_NO_ACTION_ITEMS_STORED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_TYPE_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_TYPE_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_END_INDEX_LT_START_INDEX),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_INTERVAL_WP_NUM),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_REACH_WP_INVALID_AUTO_TERMINATE_WP_NUM),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_ASSOCIATE_INVALID_TYPE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_TRIGGER_SIMPLE_INTERVAL_INVALID_TYPE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_EXEC_NON_SUPPORTED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_TYPE_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_FUNC_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_SINGLE_SHOT_CMD_TO_CAMERA_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_START_CMD_TO_CAMERA_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_VIDEO_STOP_CMD_TO_CAMERA_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCUS_PARAM_XY_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCUS_CMD_TO_CAMERA_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_SEND_FOCALIZE_CMD_TO_CAMERA_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_FOCAL_DISTANCE_INVALID),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_CAMERA_EXEC_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_CAMERA_EXEC_FAIL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_RPY_ANGLE_CTRL_CMD),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_INVALID_DURATION_CMD),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_ARRIVE_TGT_ANGLE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_FAIL_TO_SEND_CMD_TO_GIMBAL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_GIMBAL_THIS_INDEX_OF_GIMBAL_NOT_DOING_UNIFORM_CTRL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_INVALID_YAW_ANGLE),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_YAW_TO_TGT_ANGLE_TIMEOUT),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_ACTION_YAW_OCCUPIED),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_FLIGHT_CUR_AND_TGT_VEL_CLE_STATUE_EQUAL),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_RESV = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_RESV),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_FAIL_TO_SEND_CMD_TO_PAYLOAD),
	ZIYAN_ERROR_WAYPOINT_V2_MODULE_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V2, ZIYAN_ERROR_WAYPOINT_V2_MODULE_RAW_CODE_ACTUATOR_PAYLOAD_EXEC_FAILED),

    /* Waypoint v3 total errors */
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_COMMON_SUCCESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SUCCESS),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_MISSION_ID_NOT_EXIST = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_MISSION_ID_NOT_EXIST),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WAYLINE_INFO_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WAYLINE_INFO_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WPMZ_FILE_VERSION_NOT_MATCH = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WPMZ_FILE_VERSION_NOT_MATCH),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_WPMZ_FILE_LOAD_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_WPMZ_FILE_LOAD_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_NO_BREAK_INFO = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_NO_BREAK_INFO),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CMD_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CMD_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_START_WAYLINE_WHEN_WAYLINE_RUNNING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_START_WAYLINE_WHEN_WAYLINE_RUNNING),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_BREAK_WAYLINE_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_BREAK_WAYLINE_IN_CUR_STATE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_STOP_WAYLINE_WHEN_WAYLINE_NOT_RUNNING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_STOP_WAYLINE_WHEN_WAYLINE_NOT_RUNNING),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_BREAK_WAYLINE_WHEN_WAYLINE_NOT_RUNNING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_BREAK_WAYLINE_WHEN_WAYLINE_NOT_RUNNING),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_REQUEST_DRONE_CONTROL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_REQUEST_DRONE_CONTROL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_RESUME_WAYLINE_IN_CUR_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_RESUME_WAYLINE_IN_CUR_STATE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HEIGHT_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HEIGHT_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RADIUS_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RADIUS_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CROSS_FLYLIMIT_AERA = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CROSS_FLYLIMIT_AERA),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_LOW_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_LOW_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_OBSTACAL_STOP = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_OBSTACAL_STOP),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RTK_DISCONNECT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RTK_DISCONNECT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_BOUNDARY_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_BOUNDARY_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RC_PITCH_ROLL_BREAK = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RC_PITCH_ROLL_BREAK),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AIRPORT_HEIGHT_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AIRPORT_HEIGHT_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_TAKEOFF_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_TAKEOFF_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AUTOTAKEOFF_RUN_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AUTOTAKEOFF_RUN_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_WAYLINE_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_WAYLINE_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_AGRO_PLAN_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_AGRO_PLAN_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_REQUEST_QUICK_TAKEOFF_ASSIST_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_REQUEST_QUICK_TAKEOFF_ASSIST_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_QUICK_TAKEOFF_ASSIST_RUN_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_QUICK_TAKEOFF_ASSIST_RUN_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_VFENCE_LIMIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_VFENCE_LIMIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_GPS_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_GPS_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_START_AT_CURRENT_RC_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_START_AT_CURRENT_RC_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HOME_POINT_NOT_RECORDED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HOME_POINT_NOT_RECORDED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_LOWER_BATTERY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_LOWER_BATTERY),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RETURN_HOME = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RETURN_HOME),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ADSB_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ADSB_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RC_LOST = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RC_LOST),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RTK_NOT_READY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RTK_NOT_READY),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_IS_MOVING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_IS_MOVING),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_ON_GROUND_MOTOR_ON = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_ON_GROUND_MOTOR_ON),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_CAMERA_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_CAMERA_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_HEIGHT_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_HEIGHT_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_SURFACE_FOLLOW_MAP_WRONG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_SURFACE_FOLLOW_MAP_WRONG),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_HOMEPOINT_NOT_MATCH_RTK = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_HOMEPOINT_NOT_MATCH_RTK),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_STRONG_WIND_GOHOME = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_STRONG_WIND_GOHOME),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_DRONE_CRITICAL_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_DRONE_CRITICAL_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_CANNOT_FIND_PAYLOAD = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_CANNOT_FIND_PAYLOAD),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_EXECUTION_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_EXECUTION_FAILED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_FARM_NO_PESTICIDE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_FARM_NO_PESTICIDE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_RADAR_DISCONNECT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_RADAR_DISCONNECT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_EXIT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_EXIT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_BREAK = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_BREAK),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_SET_GOHOME = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_SET_GOHOME),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_AGRO_PLANNER_STATE_CHANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_AGRO_PLANNER_STATE_CHANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_USER_SWITCH_RC_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_USER_SWITCH_RC_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INIT_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INIT_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_JOB_EXIT_BUT_MIS_RUNNING = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_JOB_EXIT_BUT_MIS_RUNNING),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_ON_GROUND_MOTOR_ON_CANNOT_GO = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_ON_GROUND_MOTOR_ON_CANNOT_GO),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_START_INDEX_OR_PROG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_START_INDEX_OR_PROG),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_CSYS_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_CSYS_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_HEIGHT_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_HEIGHT_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_FLY_WP_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_FLY_WP_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_YAW_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_YAW_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_TURN_DIR_MODE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_TURN_DIR_MODE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_WP_TYPE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_WP_TYPE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_FIR_LAS_WP_TYPE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_FIR_LAS_WP_TYPE_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GLOB_VEL_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GLOB_VEL_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_NUM_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_NUM_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LAT_LONG_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LAT_LONG_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_DAMP_DIS_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_DAMP_DIS_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_MAX_VEL_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_MAX_VEL_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_VEL_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_VEL_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_YAW_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_YAW_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_YAW_MODE_IN_VERT_SEGM = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_YAW_MODE_IN_VERT_SEGM),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_MISSION_ID_CHANGED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_MISSION_ID_CHANGED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_PROGRESS_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_PROGRESS_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_INVALID_MISSION_STATE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_INVALID_MISSION_STATE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_WP_BREAK_INFO_WP_INDEX_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_WP_BREAK_INFO_WP_INDEX_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_BREAK_LAT_LONG_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_BREAK_LAT_LONG_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_BREAK_INFO_WP_YAW_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_BREAK_INFO_WP_YAW_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_INVALID_BREAK_INFO_FLAG = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_INVALID_BREAK_INFO_FLAG),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GET_TRAJ_INFO_FAILED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GET_TRAJ_INFO_FAILED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_GENERATE_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_GENERATE_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LIB_RUN_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LIB_RUN_FAIL),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRAJ_LIB_EMERGENCY_BRAKE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRAJ_LIB_EMERGENCY_BRAKE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NOT_FOUND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NOT_FOUND),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INDEX_REPEATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INDEX_REPEATED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INFO_SIZE_TOO_LONG_OR_TOO_SHORT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INFO_SIZE_TOO_LONG_OR_TOO_SHORT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_EMPTY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_EMPTY),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_EMPTY = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_EMPTY),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_ID_REPEATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_REPEATED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NODE_CHILDREN_NUM_LT_2 = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NODE_CHILDREN_NUM_LT_2),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_INDEX_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_INDEX_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_ID_IS_65535 = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_ID_IS_65535),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_NODE_CHILDNUM_SUM_NOT_EQ_NEXT_LAYER_SIZE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_NODE_CHILDNUM_SUM_NOT_EQ_NEXT_LAYER_SIZE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_MORE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_MORE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_LESS = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_LAYER_NUM_TOO_LESS),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_GROUP_NUM_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_GROUP_NUM_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_GROUP_VALID_RANGE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_GROUP_VALID_RANGE_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_ROOT_STATUS_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_ROOT_STATUS_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_TREE_NODE_STATUS_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_TREE_NODE_STATUS_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_STATUS_TREE_SIZE_ERROR = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_STATUS_TREE_SIZE_ERROR),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_TRIGGER_RUN_RESULT_INVALID = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_TRIGGER_RUN_RESULT_INVALID),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_REPEATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_GROUP_ID_REPEATED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_REPEATED = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_REPEATED),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_OUT_OF_RANGE = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_BREAK_INFO_ACTION_LOCATION_OUT_OF_RANGE),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_RESUME_ID_NOT_IN_BREAK_INFO = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_RESUME_ID_NOT_IN_BREAK_INFO),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_RESUME_INFO_MODIFY_ACTION_STATUS_FROM_NO_INTERRUPT_TO_INTERRUPT = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_RESUME_INFO_MODIFY_ACTION_STATUS_FROM_NO_INTERRUPT_TO_INTERRUPT),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTION_COMMON_ACTION_RESUME_FAIL_FOR_INVALID_RESUME_INFO = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTION_COMMON_ACTION_RESUME_FAIL_FOR_INVALID_RESUME_INFO),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_ACTUATOR_COMMON_ACTUATOR_NOT_FOUND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_ACTUATOR_COMMON_ACTUATOR_NOT_FOUND),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRIGGER_NOT_FOUND = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRIGGER_NOT_FOUND),
    ZIYAN_ERROR_WAYPOINT_V3_MODULE_CODE_TRIGGER_SINGLE_TIME_CHECK_FAIL = ZIYAN_ERROR_CODE(ZIYAN_ERROR_MODULE_WAYPOINT_V3, ZIYAN_ERROR_WAYPOINT_V3_MODULE_RAW_CODE_TRIGGER_SINGLE_TIME_CHECK_FAIL),
};
//@formatter:on

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_ERROR_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

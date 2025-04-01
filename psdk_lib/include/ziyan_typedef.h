/**
 ********************************************************************
 * @file    ziyan_typedef.h
 * @brief   This is the header file for "ziyan_typedef.c", defining the structure and
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
#ifndef ZIYAN_TYPEDEF_H
#define ZIYAN_TYPEDEF_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ziyan_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define ZIYAN_PI                                  (3.14159265358979323846f)
#define ZIYAN_FILE_NAME_SIZE_MAX                  256
#define ZIYAN_FILE_PATH_SIZE_MAX                  (ZIYAN_FILE_NAME_SIZE_MAX + 256)
#define ZIYAN_IP_ADDR_STR_SIZE_MAX                15
#define ZIYAN_MD5_BUFFER_LEN                      16

#define ZIYAN_SUBSCRIPTION_MODULE_INDEX_OFFSET    24u
#define ZIYAN_SUBSCRIPTION_MODULE_INDEX_MASK      0xFF000000u
#define ZIYAN_SUBSCRIPTION_TOPIC_CODE_OFFSET      0u
#define ZIYAN_SUBSCRIPTION_TOPIC_CODE_MASK        0x00FFFFFFu

#define ZIYAN_DATA_SUBSCRIPTION_TOPIC(subscriptionModule, topicCode) \
    (uint32_t)\
    (((((uint32_t)(subscriptionModule)) << (ZIYAN_SUBSCRIPTION_MODULE_INDEX_OFFSET)) & (ZIYAN_SUBSCRIPTION_MODULE_INDEX_MASK)) | \
    ((((uint32_t)(topicCode)) << (ZIYAN_SUBSCRIPTION_TOPIC_CODE_OFFSET)) & (ZIYAN_SUBSCRIPTION_TOPIC_CODE_MASK)))

#define ZIYAN_ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))

/**
 * @brief Type define double as ziyan_f64_t.
 */
typedef double ziyan_f64_t;
/**
 * @brief Type define float as ziyan_f32_t.
 */
typedef float ziyan_f32_t;
/**
 * @brief Type define uint64 as T_ZiyanReturnCode.
 * @details The type can be any value of ::ZiyanErrorCode.
 */
typedef uint64_t T_ZiyanReturnCode;

typedef enum {
    ZIYAN_MOUNT_POSITION_TYPE_UNKNOWN             = 0,
    ZIYAN_MOUNT_POSITION_TYPE_PAYLOAD_PORT        = 1,
} E_ZiyanMountPositionType;

typedef enum {
    ZIYAN_MOUNT_POSITION_UNKNOWN                  = 0,
    ZIYAN_MOUNT_POSITION_PAYLOAD_PORT_NO1         = 1,
    ZIYAN_MOUNT_POSITION_PAYLOAD_PORT_NO2         = 2,
    ZIYAN_MOUNT_POSITION_PAYLOAD_PORT_NO3         = 3,
} E_ZiyanMountPosition;

typedef enum {
    ZIYAN_AIRCRAFT_SERIES_UNKNOWN                 = 0,
    ZIYAN_AIRCRAFT_SERIES_SHADOW_PLUS             = 1,
    ZIYAN_AIRCRAFT_SERIES_SHADOW_MAX              = 2,
} E_ZiyanAircraftSeries;

typedef enum {
    ZIYAN_AIRCRAFT_TYPE_UNKNOWN                   = 0, /*!< Aircraft type is unknown. */
    ZIYAN_AIRCRAFT_TYPE_SHADOW_PLUS               = 1, /*!< Aircraft type is Shadow Plus. */
    ZIYAN_AIRCRAFT_TYPE_SHADOW_MAX                = 2, /*!< Aircraft type is Shadow Max. */
} E_ZiyanAircraftType;

/**
 * @brief Camera type.
 */
typedef enum {
    ZIYAN_CAMERA_TYPE_UNKNOWN                     = 0, /*!< Camera type is unknown. */
} E_ZiyanCameraType;

/**
 * @brief Camera supported media file subtype.
 */
typedef enum
{
    ZIYAN_DOWNLOAD_FILE_ORG                       = 0, /*!< Media sub file origin data type. */
    ZIYAN_DOWNLOAD_FILE_LDR                       = 21, /*!< Media sub file cloud point raw data type. */
    ZIYAN_DOWNLOAD_FILE_SIG                       = 22, /*!< Media sub file point cloud signature type. */
    ZIYAN_DOWNLOAD_FILE_RTK                       = 23, /*!< Media sub file point cloud real-time kinematic type. */
    ZIYAN_DOWNLOAD_FILE_CLC                       = 25, /*!< Media sub file radar-camera external reference type. */
    ZIYAN_DOWNLOAD_FILE_CLI                       = 26, /*!< Media sub file radar-IMU external reference type. */
    ZIYAN_DOWNLOAD_FILE_IMU                       = 27, /*!< Media sub file IMU data type. */
    ZIYAN_DOWNLOAD_FILE_RTL                       = 28, /*!< Media sub file RTK boom data type. */
    ZIYAN_DOWNLOAD_FILE_RTB                       = 29, /*!< Media sub file RTK base station data type. */
    ZIYAN_DOWNLOAD_FILE_RTS                       = 30, /*!< Media sub file RTK secondary antenna data type. */
    ZIYAN_DOWNLOAD_FILE_RPOS                      = 31, /*!< Media sub file real-time fusion of attitude and position data type. */
} E_ZiyanCameraMediaFileSubType;

/**
 * @brief Camera supported media file type.
 */
typedef enum {
    ZIYAN_CAMERA_FILE_TYPE_JPEG                   = 0, /*!< Media file JPEG type. */
    ZIYAN_CAMERA_FILE_TYPE_DNG                    = 1, /*!< Media file DNG type. */
    ZIYAN_CAMERA_FILE_TYPE_MOV                    = 2, /*!< Media file MOV type. */
    ZIYAN_CAMERA_FILE_TYPE_MP4                    = 3, /*!< Media file MP4 type. */
    ZIYAN_CAMERA_FILE_TYPE_TIFF                   = 5, /*!< Media file TIFF type. */
    ZIYAN_CAMERA_FILE_TYPE_LDRT                   = 24, /*!< Media file LDRT type. */
    ZIYAN_CAMERA_FILE_TYPE_RPT                    = 25, /*!< Media file RPT type. */
    ZIYAN_CAMERA_FILE_TYPE_UNKNOWN                = 255, /*!< Media file unknown type. */
} E_ZiyanCameraMediaFileType;

/**
 * @brief Camera optical zoom specifies.
 */
typedef struct {
    uint16_t maxFocalLength; /*!< The maximum focal length of the lens, unit: 0.1mm. */
    uint16_t minFocalLength; /*!< The minimum focal length of the lens, unit: 0.1mm. */
    uint16_t focalLengthStep; /*!< The minimum interval of focal length change, unit: 0.1mm. */
} T_ZiyanCameraOpticalZoomSpec;

/**
 * @brief Gimbal work mode, specifies how gimbal follow aircraft movement.
 */
typedef enum {
    ZIYAN_GIMBAL_MODE_FREE                        = 0, /*!< Free mode, fix gimbal attitude in the ground coordinate, ignoring movement of aircraft. */
    ZIYAN_GIMBAL_MODE_FPV                         = 1, /*!< FPV (First Person View) mode, only control roll and yaw angle of gimbal in the ground coordinate to follow aircraft. */
    ZIYAN_GIMBAL_MODE_YAW_FOLLOW                  = 2, /*!< Yaw follow mode, only control yaw angle of gimbal in the ground coordinate to follow aircraft. */
} E_ZiyanGimbalMode;

/**
 * @brief Gimbal rotation mode, specifies control style.
 */
typedef enum {
    ZIYAN_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE     = 0, /*!< Relative angle rotation mode, represents rotating gimbal specified angles based on current angles. */
    ZIYAN_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE     = 1, /*!< Absolute angle rotation mode, represents rotating gimbal to specified angles in the ground coordinate. */
    ZIYAN_GIMBAL_ROTATION_MODE_SPEED              = 2, /*!< Speed rotation mode, specifies rotation speed of gimbal in the ground coordinate. */
} E_ZiyanGimbalRotationMode;

/**
 * @brief Mobile APP system language.
 */
typedef enum {
    ZIYAN_MOBILE_APP_LANGUAGE_UNKNOWN             = 255, /*!< The system language of the mobile app is unknown */
    ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH             = 0, /*!< The system language of the mobile app is English */
    ZIYAN_MOBILE_APP_LANGUAGE_CHINESE             = 1, /*!< The system language of the mobile app is Chinese */
    ZIYAN_MOBILE_APP_LANGUAGE_JAPANESE            = 2, /*!< The system language of the mobile app is Japanese */
    ZIYAN_MOBILE_APP_LANGUAGE_FRENCH              = 3, /*!< The system language of the mobile app is French */
} E_ZiyanMobileAppLanguage;

/**
 * @brief Mobile APP screen size type.
 */
typedef enum {
    ZIYAN_MOBILE_APP_SCREEN_TYPE_UNKNOWN          = 255, /*!< Mobile APP screen type is unknown. */
    ZIYAN_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN       = 0, /*!< Screen size is 6 inches or larger. */
    ZIYAN_MOBILE_APP_SCREEN_TYPE_LITTLE_SCREEN    = 1, /*!< Screen size is less than 6 inches. */
} E_ZiyanMobileAppScreenType;

/**
 * @brief Subscription frequency type.
 */
typedef enum {
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ            = 1,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_5_HZ            = 5,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_10_HZ           = 10,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_50_HZ           = 50,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_100_HZ          = 100,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_200_HZ          = 200,
    ZIYAN_DATA_SUBSCRIPTION_TOPIC_400_HZ          = 400,
} E_ZiyanDataSubscriptionTopicFreq;

/**
 * @brief ZIYAN module enum for defining data subscription module.
 */
typedef enum {
    ZIYAN_DATA_SUBSCRIPTION_MODULE_FC             = 0,
    ZIYAN_DATA_SUBSCRIPTION_MODULE_CAMERA,
    ZIYAN_DATA_SUBSCRIPTION_MODULE_ERROR,
} E_ZiyanDataSubscriptionModule;

/**
 * @brief SDK adapter type.
 */
typedef enum {
    ZIYAN_SDK_ADAPTER_TYPE_UNKNOWN                = 0, /*!< SDK adapter type is unknown. */
    ZIYAN_SDK_ADAPTER_TYPE_NONE                   = 1, /*!< No external adapter is connected. */
} E_ZiyanSdkAdapterType;

typedef enum {
    ZIYAN_CHANNEL_ADDRESS_UNKNOWN                 = 0,
    ZIYAN_CHANNEL_ADDRESS_PAYLOAD_PORT_NO1        = 1,
    ZIYAN_CHANNEL_ADDRESS_PAYLOAD_PORT_NO2        = 2,
    ZIYAN_CHANNEL_ADDRESS_PAYLOAD_PORT_NO3        = 3,
} E_ZiyanChannelAddress;

/**
 * @brief Camera focus target point when in focus mode.
 */
typedef struct {
    ziyan_f32_t focusX; /*!< Specifies the horizontal coordinate within the zone. Range: 0 to 1.
                            The point [0.0, 0.0] represents the top-left corner of the screen.*/
    ziyan_f32_t focusY; /*!< Specifies vertical zone coordinate. Range: 0 to 1. */
} T_ZiyanCameraPointInScreen;

/**
 * @brief Camera time interval settings for interval shoot-photo mode.
 */
typedef struct {
    uint8_t captureCount; /*!< Specifies the total capture count of interval settings.
 *                             0: reserved, 1-254: specific number, 255: continuous capture until stopped. */
    uint16_t timeIntervalSeconds; /*!< Specifies the interval time between two captures, unit: s*/
} T_ZiyanCameraPhotoTimeIntervalSettings;

/**
 * @brief Camera zoom speeds.
 */
typedef enum {
    ZIYAN_CAMERA_ZOOM_SPEED_SLOWEST               = 72, /*!< Slowest zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_SLOW                  = 73, /*!< Slow zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_MODERATELY_SLOW       = 74, /*!< Slightly slower than normal zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_NORMAL                = 75, /*!< Normal zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_MODERATELY_FAST       = 76, /*!< Slightly faster than normal zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_FAST                  = 77, /*!< Fast zoom speed. */
    ZIYAN_CAMERA_ZOOM_SPEED_FASTEST               = 78, /*!< Fastest zoom speed. */
} E_ZiyanCameraZoomSpeed;

typedef enum {
    /*! The burst mode can capture 2 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_2                    = 2,
    /*! The burst mode can capture 3 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_3                    = 3,
    /*! The burst mode can capture 5 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_5                    = 5,
    /*! The burst mode can capture 7 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_7                    = 7,
    /*! Supports capturing 10 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_10                   = 10,
    /*! Supports capturing 14 pictures per trigger. */
    ZIYAN_CAMERA_BURST_COUNT_14                   = 14,
    /*! Unknown burst count. */
    ZIYAN_CAMERA_BURST_COUNT_KNOWN                = 0xFF,
} E_ZiyanCameraBurstCount;

/**
 * @brief Camera zoom direction.
 */
typedef enum {
    ZIYAN_CAMERA_ZOOM_DIRECTION_OUT               = 0, /*!< Zooms out, reducing the zoom factor. */
    ZIYAN_CAMERA_ZOOM_DIRECTION_IN                = 1, /*!< Zooms in, increasing the zoom factor. */
} E_ZiyanCameraZoomDirection;
#pragma pack (1)

/**
 * @brief Data channel state.
 */
typedef struct {
    /*! Realtime bandwidth limitation, varying with link status of aircraft system for some channels, such as data
     * stream, video stream and download stream. Must ensure actual bandwidth of data transmission is less than
     * realtime bandwidth limitation, unit: byte/s. */
    int32_t realtimeBandwidthLimit;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated before flow controller, unit: byte/s. */
    int32_t realtimeBandwidthBeforeFlowController;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated after flow controller, unit:
     * byte/s. If specified channel without flow controller. the value is equal to
     * ::realtimeBandwidthBeforeFlowController. */
    int32_t realtimeBandwidthAfterFlowController;

    /*! State specified whether the channel is busy or not. When data can not be sent to the endpoint directly, instead be
     * sent to buffer of flow controller or discarded, the busy state will be set. At this time, the user should stop
     * transmitting data or decrease the amount of data to be transmitted via this channel. When data bandwidth restore to
     * normal, this state will be clear again. */
    bool busyState;
} T_ZiyanDataChannelState;

/**
 * @brief Represents a vector using int32 coordinates.
 */
typedef struct Vector3d{
    int32_t x; /*!< X-coordinate of the vector. */
    int32_t y; /*!< Y-coordinate of the vector. */
    int32_t z; /*!< Z-coordinate of the vector. */
} T_ZiyanVector3d;

/**
 * @brief Represents a vector using floating-point coordinates.
 */
typedef struct Vector3f{
    ziyan_f32_t x; /*!< X-coordinate of the vector. */
    ziyan_f32_t y; /*!< Y-coordinate of the vector. */
    ziyan_f32_t z; /*!< Z-coordinate of the vector. */
} T_ZiyanVector3f;

/**
 * @brief Represents an attitude using int32 values for pitch, roll, and yaw.
 */
typedef struct {
    int32_t pitch; /*!< Pitch angle in degrees. */
    int32_t roll;  /*!< Roll angle in degrees. */
    int32_t yaw;   /*!< Yaw angle in degrees. */
} T_ZiyanAttitude3d;

/**
 * @brief Represents an attitude using floating-point values for pitch, roll, and yaw.
 */
typedef struct {
    ziyan_f32_t pitch; /*!< Pitch angle in degrees. */
    ziyan_f32_t roll;  /*!< Roll angle in degrees. */
    ziyan_f32_t yaw;   /*!< Yaw angle in degrees. */
} T_ZiyanAttitude3f;

typedef struct 
{
    ziyan_f64_t longitude; /*!< Longitude, unit: rad. */
    ziyan_f64_t latitude; /*!< Latitude, unit: rad. */
    ziyan_f32_t altitude; /*!< Altitude, WGS 84 reference ellipsoid, unit: m. */
}T_ZiyanLocation;


/**
 * @brief Represents a quaternion, when converted to a rotation matrix or Euler angles.
 */
typedef struct {
    ziyan_f32_t q0; /*!< Quaternion component w. */
    ziyan_f32_t q1; /*!< Quaternion component x. */
    ziyan_f32_t q2; /*!< Quaternion component y. */
    ziyan_f32_t q3; /*!< Quaternion component z. */
} T_ZiyanQuaternion4f;

/**
 * @brief Timestamp data structure.
 */
typedef struct {
    uint32_t millisecond;
    uint32_t microsecond;
} T_ZiyanDataTimestamp;

/**
 * @brief The firmware version of payload.
 * @note The firmware version is displayed as AA.BB.CC.DD where AA is majorVersion,
 * BB is minorVersion, CC is modifyVersion, and DD is debugVersion.
 */
typedef struct {
    uint8_t majorVersion;   /*!< Major version number, ranging from 0 to 99. */
    uint8_t minorVersion;   /*!< Minor version number, ranging from 0 to 99. */
    uint8_t modifyVersion;  /*!< Modification version number, ranging from 0 to 99. */
    uint8_t debugVersion;   /*!< Debug version number, ranging from 0 to 99. */
} T_ZiyanFirmwareVersion;

#pragma pack ()
/**
 * @brief Prototype of callback function used to receive data of topic.
 * @warning User can not execute blocking style operations or functions in the callback function, because that will block PSDK
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param data: pointer to data of the topic, user need transfer type of this pointer to the corresponding data structure
 * pointer for getting every item of the topic conveniently.
 * @param dataSize: the size of memory space pointed by data argument, equal to data structure size corresponding to the topic.
 * @param timestamp: pointer to timestamp corresponding this data. Use flight controller power-on timestamp on M300 RTK.
 * Use payload local timestamp on M30/M30T.
 * @return Execution result.
 */
typedef T_ZiyanReturnCode (*ZiyanReceiveDataOfTopicCallback)(const uint8_t *data, uint16_t dataSize,
                                                         const T_ZiyanDataTimestamp *timestamp);

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // ZIYAN_TYPEDEF_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

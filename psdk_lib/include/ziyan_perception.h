/**
 ********************************************************************
 * @file    ziyan_perception.h
 * @brief   This is the header file for "ziyan_perception.c", defining the structure and
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
#ifndef ZIYAN_PERCEPTION_H
#define ZIYAN_PERCEPTION_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define IMAGE_MAX_DIRECTION_NUM                     6
#define ZIYAN_PERCEPTION_INTRINSICS_PARAM_ARRAY_NUM   9
#define ZIYAN_PERCEPTION_ROTATION_PARAM_ARRAY_NUM     9
#define ZIYAN_PERCEPTION_TRANSLATION_PARAM_ARRAY_NUM  3

/* Exported types ------------------------------------------------------------*/
/**
 * @bref Perception camera direction
 */
typedef enum {
    ZIYAN_PERCEPTION_RECTIFY_DOWN = 0,
    ZIYAN_PERCEPTION_RECTIFY_FRONT = 1,
    ZIYAN_PERCEPTION_RECTIFY_REAR = 2,
    ZIYAN_PERCEPTION_RECTIFY_UP = 3,
    ZIYAN_PERCEPTION_RECTIFY_LEFT = 4,
    ZIYAN_PERCEPTION_RECTIFY_RIGHT = 5
} E_ZiyanPerceptionDirection;

/**
 * @bref Perception camera design location
 */
typedef enum {
    RECTIFY_DOWN_LEFT = 1,
    RECTIFY_DOWN_RIGHT = 2,
    RECTIFY_FRONT_LEFT = 3,
    RECTIFY_FRONT_RIGHT = 4,
    RECTIFY_REAR_LEFT = 5,
    RECTIFY_REAR_RIGHT = 6,
    RECTIFY_UP_LEFT = 21,
    RECTIFY_UP_RIGHT = 22,
    RECTIFY_LEFT_LEFT = 23,
    RECTIFY_LEFT_RIGHT = 24,
    RECTIFY_RIGHT_LEFT = 25,
    RECTIFY_RIGHT_RIGHT = 26
} E_ZiyanPerceptionCameraPosition;

#pragma pack(1)
/**
 * @bref Perception camera ram image info
 */
typedef struct {
    uint32_t index;
    uint8_t direction;
    uint8_t bpp;
    uint32_t width;
    uint32_t height;
} T_ZiyanPerceptionRawImageInfo;

/**
 * @bref Perception camera image info
 */
typedef struct {
    T_ZiyanPerceptionRawImageInfo rawInfo;
    uint16_t dataId;
    uint16_t sequence;
    //see enum E_ZiyanPerceptionCamPosition
    uint32_t dataType;
    uint64_t timeStamp;
} T_ZiyanPerceptionImageInfo;

/**
 * @bref Perception camera parameters
 */
typedef struct {
    uint8_t direction;
    float leftIntrinsics[ZIYAN_PERCEPTION_INTRINSICS_PARAM_ARRAY_NUM];
    float rightIntrinsics[ZIYAN_PERCEPTION_INTRINSICS_PARAM_ARRAY_NUM];
    float rotationLeftInRight[ZIYAN_PERCEPTION_ROTATION_PARAM_ARRAY_NUM];
    float translationLeftInRight[ZIYAN_PERCEPTION_TRANSLATION_PARAM_ARRAY_NUM];
} T_ZiyanPerceptionCameraParameters;

/**
 * @bref Perception camera parameters packet
 */
typedef struct {
    uint32_t timeStamp;
    uint32_t directionNum;
    T_ZiyanPerceptionCameraParameters cameraParameters[IMAGE_MAX_DIRECTION_NUM];
} T_ZiyanPerceptionCameraParametersPacket;
#pragma pack()

/**
 * @bref Callback type to receive stereo camera image
 */
typedef void(*ZiyanPerceptionImageCallback)(T_ZiyanPerceptionImageInfo imageInfo, uint8_t *imageRawBuffer,
                                          uint32_t bufferLen);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the perception module.
 * @note The interface initialization needs to be after ZiyanCore_Init.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPerception_Init(void);

/**
 * @brief Deinitialize the perception module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPerception_Deinit(void);

/**
 * @brief Subscribe the raw images of both stereo cameras in the same direction. Default frequency at 20 Hz.
 * @param direction: direction to specify the direction of the subscription. Ref to E_ZiyanPerceptionDirection
 * @param callback callback to observer the stereo camera image and info.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPerception_SubscribePerceptionImage(E_ZiyanPerceptionDirection direction,
                                                       ZiyanPerceptionImageCallback callback);

/**
 * @brief Unsubscribe the raw image of both stereo cameras in the same direction.
 * @param direction: direction to specify the direction of the subscription. Ref to E_ZiyanPerceptionDirection
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPerception_UnsubscribePerceptionImage(E_ZiyanPerceptionDirection direction);


/**
 * @brief Get the internal and external parameters of all stereo cameras.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPerception_GetStereoCameraParameters(T_ZiyanPerceptionCameraParametersPacket *packet);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_PERCEPTION_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    test_camera_manager.h
 * @brief   This is the header file for "test_camera_manager.c", defining the structure and
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
#ifndef TEST_CAMERA_MANAGER_H
#define TEST_CAMERA_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"
#include "ziyan_camera_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_SHUTTER_SPEED,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_APERTURE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_EV,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ISO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_FOCUS_POINT,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_TAP_ZOOM_POINT,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ZOOM_PARAM,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_SINGLE_PHOTO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_BURST_PHOTO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_INTERVAL_PHOTO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RECORD_VIDEO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_DOWNLOAD_AND_DELETE_MEDIA_FILE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_DOWNLOAD_FILE_LIST_BY_SLICES,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_THERMOMETRY,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_GET_LIDAR_RANGING_INFO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_IR_CAMERA_ZOOM_PARAM,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_NIGHT_SCENE_MODE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAPTURE_RECORDING_STREAMS,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOW_STORAGE_INFO,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_FORMAT_SD_CARD,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_LINK_ZOOM,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_USER_CUSTOM_DIR_FILE_NAME,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RESET_CAMERA_SETTINGS,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_AE_LOCK_MODE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_FOCUS_RING_VALUE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_CONNECT_STATUS_TEST,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_GET_PHOTO_VIDEO_PARAM,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_METERING_MODE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_METERING_POINT,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_FFC_MODE_AND_TRRIGER,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_GAIN_MODE,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_GET_CAMERA_STATUS,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SUBSCRIBE_POINT_CLOUD,
    E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_INDEX_MAX
} E_ZiyanTestCameraManagerSampleSelect;
/* Exported functions --------------------------------------------------------*/

/*! @brief Sample to set exposure compensation value for camera, using async
 * api
 *
 *  @note In this interface, exposure compensation value will be got then be
 * set.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure compensation value
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetEV(E_ZiyanMountPosition position,
                                           E_ZiyanCameraManagerExposureCompensation exposureCompensation);

/*! @brief Sample to set exposure mode for camera, using async api
 *
 *  @note In this interface, exposure will be got then be set.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param dataTarget the target exposure mode
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetExposureMode(E_ZiyanMountPosition position,
                                                     E_ZiyanCameraManagerExposureMode exposureMode);

/*! @brief Sample to set ISO value for camera, using async api
 *
 *  @note In this interface, ISO will be got then be set.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param dataTarget the target ISO value
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetISO(E_ZiyanMountPosition position,
                                            E_ZiyanCameraManagerISO isoData);
/*! @brief Sample to set shutter speed for camera, using async api
 *
 *  @note In this interface, shutter speed will be got then be set.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param dataTarget the target shutter speed
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetShutterSpeed(E_ZiyanMountPosition position,
                                                     E_ZiyanCameraManagerShutterSpeed shutterSpeed);

/*! @brief Sample to set shutter aperture value for camera, using async api
 *
 *  @note In this interface, aperture value will be got then be set.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param dataTarget the target aperture value
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetAperture(E_ZiyanMountPosition position,
                                                 E_ZiyanCameraManagerAperture aperture);

/*! @brief Sample to set focus point for camera, using async api
 *
 *  @note In this interface, focus mode will be set to be AUTO. Then the
 * focus point will be set to be (x, y)
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param x the x value of target focus point, 0~1
 *  @param y the y value of target focus point, 0~1
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetFocusPoint(E_ZiyanMountPosition position,
                                                   T_ZiyanCameraManagerFocusPosData tapFocusPos);

/*! @brief Sample to set tap-zoom point for camera, using async api
 *
 *  @note In this interface, tap-zoom function will be enable and the
 * multiplier will be set. Then the tap-zoom function will start with the
 * target tap-zoom point (x, y)
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param multiplier the zoom multiplier of each tap zoom
 *  @param x the x value of target tap-zoom point, 0~1
 *  @param y the y value of target tap-zoom point, 0~1
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerSetTapZoomPoint(E_ZiyanMountPosition position, uint8_t multiplier,
                                                     T_ZiyanCameraManagerTapZoomPosData tapZoomPosData);
/*! @brief Sample to execute continuous zoom on camera, using sync api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @note In this interface, the zoom will start with the designated direction
 * and speed, and will stop after zoomTimeInSecond second(s).
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param direction the choice of zoom out or zoom in
 *  @param speed zooming speed
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStartContinuousZoom(E_ZiyanMountPosition position,
                                                         E_ZiyanCameraZoomDirection zoomDirection,
                                                         E_ZiyanCameraZoomSpeed zoomSpeed);

/*! @brief Sample to execute position zoom on camera, using sync api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @note In this interface, the zoom will set the zoom factor as the your
 * target value.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param factor target zoom factor
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerOpticalZoom(E_ZiyanMountPosition position,
                                                 E_ZiyanCameraZoomDirection zoomDirection,
                                                 ziyan_f32_t factor);

/*! @brief Sample to stop continuous zoom on camera, using async api
 *
 *  @note It is only supported by X5, X5R and X5S camera on Osmo with lens
 * Olympus M.Zuiko ED 14-42mm f/3.5-5.6 EZ, Z3 camera, Z30 camera.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStopContinuousZoom(E_ZiyanMountPosition position);

/*! @brief Sample to shoot single photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a single photo.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStartShootSinglePhoto(E_ZiyanMountPosition position);

/*! @brief Sample to shoot burst photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a burst photo.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param count The number of pictures in each burst shooting
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStartShootBurstPhoto(E_ZiyanMountPosition position,
                                                          E_ZiyanCameraBurstCount burstCount);

/*! @brief Sample to start shooting interval photo, using async api
 *
 *  @note In this interface, camera will be set to be the SHOOT_PHOTO mode
 * then start to shoot a interval photo.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @param intervalData the parameter of interval shooting
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStartShootIntervalPhoto(E_ZiyanMountPosition position,
                                                             T_ZiyanCameraPhotoTimeIntervalSettings intervalData);

/*! @brief Sample to stop shooting, using async api
 *
 *  @note In this interface, camera will stop all the shooting action
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStopShootPhoto(E_ZiyanMountPosition position);

/*! @brief Sample to start record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then start to record video.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStartRecordVideo(E_ZiyanMountPosition position);

/*! @brief Sample to stop record video, using async api
 *
 *  @note In this interface, camera will be set to be the RECORD_VIDEO mode
 * then stop recording video.
 *  @param index payload node index, input limit see enum
 * ZIYAN::OSDK::PayloadIndexType
 *  @return T_ZiyanReturnCode error code
 */
T_ZiyanReturnCode ZiyanTest_CameraManagerStopRecordVideo(E_ZiyanMountPosition position);

T_ZiyanReturnCode ZiyanTest_CameraManagerRunSample(E_ZiyanMountPosition mountPosition,
                                               E_ZiyanTestCameraManagerSampleSelect cameraManagerSampleSelect);

#ifdef __cplusplus
}
#endif

#endif // TEST_CAMERA_MANAGER_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    ziyan_gimbal.h
 * @brief   ZIYAN gimbal module is used to achieve the third gimbal for developer. The module can be used only when the ZIYAN
 * adapter is Skyport, rather than XPort.
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
#ifndef ZIYAN_GIMBAL_H
#define ZIYAN_GIMBAL_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/**
 * @brief Gimbal calibration stage and result.
 */
typedef enum {
    ZIYAN_GIMBAL_CALIBRATION_STAGE_COMPLETE = 0, /*!< Calibration have been completed and the result is successful. */
    ZIYAN_GIMBAL_CALIBRATION_STAGE_PROCRESSING = 1, /*!< Gimbal is calibrating. */
    ZIYAN_GIMBAL_CALIBRATION_STAGE_FAILED = 3, /*!< Calibration have been completed and the result is failure. */
} E_ZiyanGimbalCalibrationStage;

/**
 * @brief Axis of gimbal.
 */
typedef enum {
    ZIYAN_GIMBAL_AXIS_PITCH = 0, /*!< Pitch axis. */
    ZIYAN_GIMBAL_AXIS_ROLL = 1, /*!< Roll axis. */
    ZIYAN_GIMBAL_AXIS_YAW = 2, /*!< Yaw axis. */
} E_ZiyanGimbalAxis;

/**
 * @brief Reset mode of gimbal.
 */
typedef enum {
    /*! Only reset yaw axis of gimbal. Reset angle of yaw axis to the sum of yaw axis angle of aircraft and fine tune angle
     * of yaw axis of gimbal. */
    ZIYAN_GIMBAL_RESET_MODE_YAW = 1,
    /*! Reset yaw axis and pitch axis of gimbal. Reset angle of yaw axis to sum of yaw axis angle of aircraft and fine tune 
     * angle of yaw axis of gimbal, and reset pitch axis angle to the fine tune angle. */
    ZIYAN_GIMBAL_RESET_MODE_PITCH_AND_YAW = 3,
    /*! Reset only pitch axis of gimbal. */
    ZIYAN_GIMBAL_RESET_MODE_PITCH_ONLY = 4,
    /*! Reset only roll axis of gimbal. */
    ZIYAN_GIMBAL_RESET_MODE_ROLL_ONLY = 5,
    /*! Reset only yaw axis of gimbal. */
    ZIYAN_GIMBAL_RESET_MODE_YAW_ONLY = 6,
    /*! Reset yaw axis and pitch axis of gimbal. Reset angle of yaw axis to sum of yaw axis angle of aircraft and fine tune
     * angle of yaw axis of gimbal, and reset pitch axis angle to sum of -90 degree and fine tune angle if gimbal
     * downward, sum of 90 degree and fine tune angle if upward. */
    ZIYAN_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD_AND_YAW = 11,
    /*! Reset pitch axis of gimbal. Reset pitch axis angle to sum of -90 degree and fine tune angle if gimbal downward,
     * sum of 90 degree and fine tune angle if upward. */
    ZIYAN_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD = 12,
} E_ZiyanGimbalResetMode;

/**
 * @brief Smooth factor of gimbal controller.
 */
typedef struct {
    uint8_t pitch; /*!< Smooth factor of pitch axis, range from 0 to 30. */
    uint8_t yaw; /*!< Smooth factor of yaw axis, range from 0 to 30. */
} T_ZiyanGimbalControllerSmoothFactor;

/**
 * @brief Max speed percentage of every axis of gimbal.
 */
typedef struct {
    uint8_t pitch; /*!< Max speed percentage of pitch axis, range from 1 to 100. */
    uint8_t yaw; /*!< Max speed percentage of yaw axis, range from 1 to 100. */
} T_ZiyanGimbalControllerMaxSpeedPercentage;

/**
 * @brief Data structure describes gimbal system state.
 */
typedef struct {
    bool resettingFlag; /*!< Specifies whether gimbal is resetting now or not. */
    bool mountedUpward; /*!< Specifies whether gimbal is upward or not. */
    bool blockingFlag; /*!< Specifies whether gimbal is stuck or not. */
    bool pitchRangeExtensionEnabledFlag; /*!< Specifies whether extended angle range of gimbal pitch axis is enabled or not. */
    E_ZiyanGimbalMode gimbalMode; /*!< Gimbal mode. */
    T_ZiyanAttitude3d fineTuneAngle; /*!< Fine tune angles, unit: 0.1 degree. */
    T_ZiyanGimbalControllerSmoothFactor smoothFactor; /*!< Smooth factor of gimbal controller. */
    T_ZiyanGimbalControllerMaxSpeedPercentage maxSpeedPercentage; /*!< Max speed percentage of gimbal controller. */
} T_ZiyanGimbalSystemState;

/**
 * @brief Reach limit flag.
 */
typedef struct {
    bool pitch; /*!< Specifies whether gimbal has reached angle limit of pitch axis, if true, reached the limit. */
    bool roll; /*!< Specifies whether gimbal has reached angle limit of roll axis, if true, reached the limit. */
    bool yaw; /*!< Specifies whether gimbal has reached angle limit of yaw axis, if true, reached the limit. */
} T_ZiyanGimbalReachLimitFlag;

/**
 * @brief Gimbal attitude related information.
 */
typedef struct {
    T_ZiyanAttitude3d attitude; /*!< Gimbal attitude in the ground coordinate, unit: 0.1 degree. */
    T_ZiyanGimbalReachLimitFlag reachLimitFlag; /*!< Reach limit flag, being set when the joint angles of gimbal reach limits. */
} T_ZiyanGimbalAttitudeInformation;

/**
 * @brief Gimbal calibration state.
 */
typedef struct {
    bool calibratingFlag; /*!< Specifies whether gimbal is calibrating now or not. */
    bool lastCalibrationResult; /*!< Result of the last calibration, and true specifies success. */
    uint8_t currentCalibrationProgress; /*!< Progress percentage of current calibration. If gimbal is calibrating now and fill in real progress percentage, if not, fill in zero. The item ranges from 0 to 100. */
    E_ZiyanGimbalCalibrationStage currentCalibrationStage; /*!< Current calibration stage and result. If gimbal is calibrating now and fill in ::ZIYAN_GIMBAL_CALIBRATION_STAGE_PROCRESSING, if not, fill in result of the last calibration. */
} T_ZiyanGimbalCalibrationState;

/**
 * @brief Gimbal rotation command property.
 */
typedef struct {
    struct {
        bool pitch; /*!< Specifies whether ignore rotation command of pitch axis, and true specifies ignoring. */
        bool roll; /*!< Specifies whether ignore rotation command of roll axis, and true specifies ignoring. */
        bool yaw; /*!< Specifies whether ignore rotation command of yaw axis, and true specifies ignoring. */
    } rotationValueInvalidFlag; /*!< Specifies whether ignore rotation command of some axis, including pitch, roll and yaw. */

    union {
        struct {
            uint16_t actionTime; /*!< Action time during which gimbal move to target angles, unit: 0.01s. */
        } relativeAngleRotation; /*!< Property of relative angle rotation command. */
        struct {
            uint16_t actionTime; /*!< Action time during which gimbal move to target angles, unit: 0.01s. */
            /*! Joint angle valid flag, specifying whether
             * T_ZiyanGimbalRotationProperty::absoluteAngleRotation::jointAngle is valid when absolute angle control. */
            bool jointAngleValid;
            /*! Joint angles of gimbal, unit: 0.1 degree. If
             * T_ZiyanGimbalRotationProperty::absoluteAngleRotation::jointAngleValid is false, specifying here joint
             * angles are invalid, and please ignore the joint angles. If joint angles are valid, users should ensure
             * that target joint angles of gimbal is approximately the same as the specified value. */
            T_ZiyanAttitude3d jointAngle;
        } absoluteAngleRotation; /*!< Property of absolute angle rotation command. */
    };
} T_ZiyanGimbalRotationProperty;

/**
 * @brief Common handler function prototypes of gimbal, that is, some common callback function prototypes of gimbal.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block ZIYAN
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 */
typedef struct {
    /*!
     * @brief Prototype of callback function used to report system state of gimbal.
     * @param systemState: pointer to memory space used to store gimbal system state.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetSystemState)(T_ZiyanGimbalSystemState *systemState);

    /*!
     * @brief Prototype of callback function used to report attitude information of gimbal.
     * @param attitudeInformation: pointer to memory space used to store gimbal attitude information.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetAttitudeInformation)(T_ZiyanGimbalAttitudeInformation *attitudeInformation);

    /*!
     * @brief Prototype of callback function used to report calibration state of gimbal.
     * @param calibrationState: pointer to memory space used to store gimbal calibration state.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetCalibrationState)(T_ZiyanGimbalCalibrationState *calibrationState);

    /*!
     * @brief Prototype of callback function used to rotate gimbal. Rotation callback function will be called when
     * other modules control gimbal.
     * @warning User should control execution time of the callback function to be short, because this callback function
     * may be called at very high frequency in some scenarios, just like 200Hz.
     * @param rotationMode: mode of rotation command.
     * @param rotationProperty: property of rotation command.
     * @param rotationValue: value of rotation command, unit: 0.1 degree (if rotation mode is
     * ::ZIYAN_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE or ::ZIYAN_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE), 0.1 degree/s
     * (if rotation mode is ::ZIYAN_GIMBAL_ROTATION_MODE_SPEED).
     * @return Execution result.
     */
    T_ZiyanReturnCode (*Rotate)(E_ZiyanGimbalRotationMode rotationMode, T_ZiyanGimbalRotationProperty rotationProperty,
                              T_ZiyanAttitude3d rotationValue);

    /*!
     * @brief Prototype of callback function used to start calibrating gimbal.
     * @note Maximum allowable time of gimbal calibration is 120s. If gimbal has not completed calibration during
     * 120s, ZIYAN Pilot believe calibration failed.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*StartCalibrate)(void);

    /**
     * @brief Set smooth factor for gimbal controller, using to smooth control.
     * @param smoothingFactor: smooth factor. The larger the value, the smaller the acceleration of gimbal. Recommended
     * calculation formula is "acc = 10000 * (0.8 ^ (1 + x)) deg/s^2", x is smooth factor. Range from 0 to 30.
     * @param axis: axis to be set.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*SetControllerSmoothFactor)(uint8_t smoothingFactor, E_ZiyanGimbalAxis axis);

    /*!
     * @brief Prototype of callback function used to enable or disable extended pitch axis angle range.
     * @details Switching the gimbal limit euler angle of pitch axis to the extended limit angle or the default limit
     * angle.
     * @param enabledFlag: flag specified whether enable or disable extended pitch axis angle range.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*SetPitchRangeExtensionEnabled)(bool enabledFlag);

    /**
     * @brief Set max speed percentage for gimbal controller.
     * @param maxSpeedPercentage: max speed value. Recommended calculation formula is "spd = default_max_spd * x / 100",
     * x is default max speed value. Range from 1 to 100.
     * @param axis: axis to be set.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*SetControllerMaxSpeedPercentage)(uint8_t maxSpeedPercentage, E_ZiyanGimbalAxis axis);

    /**
     * @brief Restore factory settings of gimbal, including fine tune angle, pitch angle extension enable flag and max
     * speed etc.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*RestoreFactorySettings)(void);

    /*!
     * @brief Prototype of callback function used to set gimbal work mode.
     * @param mode: gimbal work mode.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*SetMode)(E_ZiyanGimbalMode mode);

    /*!
     * @brief Prototype of callback function used to reset gimbal.
     * @note Reset command will interrupt rotation process.
     * @param mode: Reset mode.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*Reset)(E_ZiyanGimbalResetMode mode);

    /*!
     * @brief Prototype of callback function used to fine tune angle of gimbal.
     * @param fineTuneAngle: fine tune angle, unit: 0.1 degree. Angle value of greater than 0 specifies rotation
     * towards the positive direction of body coordinate.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*FineTuneAngle)(T_ZiyanAttitude3d fineTuneAngle);

    /**
     * @brief Prototype of callback function used to report rotation speed of gimbal.
     * @note The callback function will be called at 50 Hz. Developers should ensure that, the average rotation speed
     * of gimbal within 20 ms after the callback function is called is approximately equal to the reported rotation
     * speed.
     * @param rotationSpeed: pointer to memory space used to store rotation speed, in body coordinate, unit:
     * 0.1 degree/s.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetRotationSpeed)(T_ZiyanAttitude3d *rotationSpeed);

    /**
     * @brief Prototype of callback function used to report joint angle of gimbal.
     * @param jointAngle: pointer to memory space used to store joint angles, unit: 0.1 degree.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetJointAngle)(T_ZiyanAttitude3d *jointAngle);
} T_ZiyanGimbalCommonHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise gimbal module, and user should call this function before using gimbal features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanGimbal_Init(void);

/**
 * @brief Deinitialise gimbal module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanGimbal_DeInit(void);

/**
 * @brief Register common handler functions/callback functions of gimbal. After registering, callback functions will be
 * called automatically.
 * @param commonHandler: pointer to common handler functions of gimbal.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanGimbal_RegCommonHandler(const T_ZiyanGimbalCommonHandler *commonHandler);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_GIMBAL_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

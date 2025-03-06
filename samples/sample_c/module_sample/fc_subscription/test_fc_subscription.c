/**
 ********************************************************************
 * @file    test_fc_subscription.c
 * @brief
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

/* Includes ------------------------------------------------------------------*/
#include <utils/util_misc.h>
#include <math.h>
#include "test_fc_subscription.h"
#include "ziyan_logger.h"
#include "ziyan_platform.h"
// #include "widget_interaction_test/test_widget_interaction.h"

/* Private constants ---------------------------------------------------------*/
#define FC_SUBSCRIPTION_TASK_FREQ         (1)
#define FC_SUBSCRIPTION_TASK_STACK_SIZE   (1024)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *UserFcSubscription_Task(void *arg);
static T_ZiyanReturnCode ZiyanTest_FcSubscriptionReceiveQuaternionCallback(const uint8_t *data, uint16_t dataSize,
                                                                       const T_ZiyanDataTimestamp *timestamp);

static T_ZiyanReturnCode ZiyanTest_FcSubscriptionReceivePositionFusedCallback(const uint8_t *data, uint16_t dataSize,
                                                                          const T_ZiyanDataTimestamp *timestamp);

/* Private variables ---------------------------------------------------------*/
static T_ZiyanTaskHandle s_userFcSubscriptionThread;
static bool s_userFcSubscriptionDataShow = false;
static uint8_t s_totalSatelliteNumberUsed = 0;
static uint32_t s_userFcSubscriptionDataCnt = 0;

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_FcSubscriptionStartService(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanOsalHandler *osalHandler = NULL;

    osalHandler = ZiyanPlatform_GetOsalHandler();
    ziyanStat = ZiyanFcSubscription_Init();
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init data subscription module error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_QUATERNION, ZIYAN_DATA_SUBSCRIPTION_TOPIC_50_HZ,
                                               ZiyanTest_FcSubscriptionReceiveQuaternionCallback);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic quaternion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic quaternion success.");
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_VELOCITY, ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic velocity error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic velocity success.");
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_ACCELERATION_RAW, ZIYAN_DATA_SUBSCRIPTION_TOPIC_5_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic acceleration raw error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic acceleration raw success.");
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW, ZIYAN_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic angular rate raw error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic angular rate raw success.");
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_DETAILS, ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic gps details error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        USER_LOG_DEBUG("Subscribe topic gps details success.");
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED, ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               ZiyanTest_FcSubscriptionReceivePositionFusedCallback);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic position fused error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (osalHandler->TaskCreate("user_subscription_task", UserFcSubscription_Task,
                                FC_SUBSCRIPTION_TASK_STACK_SIZE, NULL, &s_userFcSubscriptionThread) !=
        ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("user data subscription task create error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_FcSubscriptionRunSample(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    T_ZiyanFcSubscriptionVelocity velocity = {0};
    T_ZiyanDataTimestamp timestamp = {0};
    T_ZiyanFcSubscriptionGpsPosition gpsPosition = {0};
    T_ZiyanFcSubscriptionSingleBatteryInfo singleBatteryInfo = {0};

    USER_LOG_INFO("Fc subscription sample start");
    s_userFcSubscriptionDataShow = true;

    USER_LOG_INFO("--> Step 1: Init fc subscription module");
    ziyanStat = ZiyanFcSubscription_Init();
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("init data subscription module error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("--> Step 2: Subscribe the topics of quaternion, velocity and gps position");
    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_QUATERNION, ZIYAN_DATA_SUBSCRIPTION_TOPIC_5_HZ,
                                               ZiyanTest_FcSubscriptionReceiveQuaternionCallback);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic quaternion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_VELOCITY, ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic velocity error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ziyanStat = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_POSITION, ZIYAN_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               NULL);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Subscribe topic gps position error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("--> Step 3: Get latest value of the subscribed topics in the next 10 seconds\r\n");

    for (int i = 0; i < 10; ++i) {
        osalHandler->TaskSleepMs(1000 / FC_SUBSCRIPTION_TASK_FREQ);
        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_VELOCITY,
                                                          (uint8_t *) &velocity,
                                                          sizeof(T_ZiyanFcSubscriptionVelocity),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic velocity error.");
        } else {
            USER_LOG_INFO("velocity: x = %f y = %f z = %f healthFlag = %d, timestamp ms = %d us = %d.", velocity.data.x,
                          velocity.data.y,
                          velocity.data.z, velocity.health, timestamp.millisecond, timestamp.microsecond);
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_POSITION,
                                                          (uint8_t *) &gpsPosition,
                                                          sizeof(T_ZiyanFcSubscriptionGpsPosition),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps position error.");
        } else {
            USER_LOG_INFO("gps position: x = %d y = %d z = %d.", gpsPosition.x, gpsPosition.y, gpsPosition.z);
        }

        //Attention: if you want to subscribe the single battery info on M300 RTK, you need connect USB cable to
        //OSDK device or use topic ZIYAN_FC_SUBSCRIPTION_TOPIC_BATTERY_INFO instead.
        // ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX1,
        //                                                   (uint8_t *) &singleBatteryInfo,
        //                                                   sizeof(T_ZiyanFcSubscriptionSingleBatteryInfo),
        //                                                   &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic battery single info index1 error.");
        } else {
            USER_LOG_INFO(
                "battery single info index1: capacity percent = %ld% voltage = %ldV temperature = %.2f degree.",
                singleBatteryInfo.batteryCapacityPercent,
                singleBatteryInfo.currentVoltage / 1000,
                (ziyan_f32_t) singleBatteryInfo.batteryTemperature / 10);
        }

        // ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INFO_INDEX2,
        //                                                   (uint8_t *) &singleBatteryInfo,
        //                                                   sizeof(T_ZiyanFcSubscriptionSingleBatteryInfo),
        //                                                   &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic battery single info index2 error.");
        } else {
            USER_LOG_INFO(
                "battery single info index2: capacity percent = %ld% voltage = %ldV temperature = %.2f degree.\r\n",
                singleBatteryInfo.batteryCapacityPercent,
                singleBatteryInfo.currentVoltage / 1000,
                (ziyan_f32_t) singleBatteryInfo.batteryTemperature / 10);
        }
    }

    USER_LOG_INFO("--> Step 4: Unsubscribe the topics of quaternion, velocity and gps position");
    ziyanStat = ZiyanFcSubscription_UnSubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_QUATERNION);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("UnSubscribe topic quaternion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ziyanStat = ZiyanFcSubscription_UnSubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_VELOCITY);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("UnSubscribe topic quaternion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    ziyanStat = ZiyanFcSubscription_UnSubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_POSITION);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("UnSubscribe topic quaternion error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    USER_LOG_INFO("--> Step 5: Deinit fc subscription module");

    ziyanStat = ZiyanFcSubscription_DeInit();
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit fc subscription error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    s_userFcSubscriptionDataShow = false;
    USER_LOG_INFO("Fc subscription sample end");

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_FcSubscriptionDataShowTrigger(void)
{
    s_userFcSubscriptionDataShow = !s_userFcSubscriptionDataShow;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_FcSubscriptionGetTotalSatelliteNumber(uint8_t *number)
{
    *number = s_totalSatelliteNumberUsed;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserFcSubscription_Task(void *arg)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanFcSubscriptionVelocity velocity = {0};
    T_ZiyanFcSubscriptionAccelerationRaw accele_raw = {0};
    T_ZiyanFcSubscriptionAngularRateRaw  angular_rate_raw = {0};
    T_ZiyanFcSubscriptionPositionFused   position_fused = {0};
    T_ZiyanDataTimestamp timestamp = {0};
    T_ZiyanFcSubscriptionGpsPosition gpsPosition = {0};
    T_ZiyanFcSubscriptionGpsDetails gpsDetails = {0};
    T_ZiyanFcSubscriptionGpsTime gpsTime = 0;
    T_ZiyanFcSubscriptionGpsDate gpsDate = 0;
    T_ZiyanOsalHandler *osalHandler = NULL;

    USER_UTIL_UNUSED(arg);
    osalHandler = ZiyanPlatform_GetOsalHandler();

    while (1) {
        osalHandler->TaskSleepMs(1000 / FC_SUBSCRIPTION_TASK_FREQ);

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_VELOCITY,
                                                          (uint8_t *) &velocity,
                                                          sizeof(T_ZiyanFcSubscriptionVelocity),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic velocity error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("velocity: x %f y %f z %f, healthFlag %d.", velocity.data.x, velocity.data.y,
                          velocity.data.z, velocity.health);
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_POSITION,
                                                          (uint8_t *) &gpsPosition,
                                                          sizeof(T_ZiyanFcSubscriptionGpsPosition),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps position error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("gps position: x %d y %d z %d.", gpsPosition.x, gpsPosition.y, gpsPosition.z);
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_DETAILS,
                                                          (uint8_t *) &gpsDetails,
                                                          sizeof(T_ZiyanFcSubscriptionGpsDetails),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps details error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("gps total satellite number used: %d %d %d.",
                          gpsDetails.gpsSatelliteNumberUsed,
                          gpsDetails.glonassSatelliteNumberUsed,
                          gpsDetails.totalSatelliteNumberUsed);
            s_totalSatelliteNumberUsed = gpsDetails.totalSatelliteNumberUsed;
        }


        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_TIME,
                                                          &gpsTime,
                                                          sizeof(T_ZiyanFcSubscriptionGpsTime),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_GPS_DATE,
                                                          &gpsDate,
                                                          sizeof(T_ZiyanFcSubscriptionGpsDate),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("get gps date time: %d %d", gpsDate, gpsTime);
        }


        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_ACCELERATION_RAW,
                                                          &accele_raw,
                                                          sizeof(T_ZiyanFcSubscriptionAccelerationRaw),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW,
                                                          &angular_rate_raw,
                                                          sizeof(T_ZiyanFcSubscriptionAngularRateRaw),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic gps time error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("accele: %f : %f : %f", accele_raw.x, accele_raw.y, accele_raw.z);
            USER_LOG_INFO("gyro: %f : %f : %f", angular_rate_raw.x, angular_rate_raw.y, angular_rate_raw.z);
        }

        ziyanStat = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_POSITION_FUSED,
                                                          &position_fused,
                                                          sizeof(T_ZiyanFcSubscriptionPositionFused),
                                                          &timestamp);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("get value of topic position fused error.");
        }

        if (s_userFcSubscriptionDataShow == true) {
            USER_LOG_INFO("position fused: %f : %f : %f", position_fused.latitude, position_fused.longitude, position_fused.altitude);
        }

    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_ZiyanReturnCode ZiyanTest_FcSubscriptionReceiveQuaternionCallback(const uint8_t *data, uint16_t dataSize,
                                                                       const T_ZiyanDataTimestamp *timestamp)
{
    T_ZiyanFcSubscriptionQuaternion *quaternion = (T_ZiyanFcSubscriptionQuaternion *) data;
    ziyan_f64_t pitch, yaw, roll;

    USER_UTIL_UNUSED(dataSize);

    pitch = (ziyan_f64_t) asinf(-2 * quaternion->q1 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q2) * 57.3;
    roll = (ziyan_f64_t) atan2f(2 * quaternion->q2 * quaternion->q3 + 2 * quaternion->q0 * quaternion->q1,
                             -2 * quaternion->q1 * quaternion->q1 - 2 * quaternion->q2 * quaternion->q2 + 1) * 57.3;
    yaw = (ziyan_f64_t) atan2f(2 * quaternion->q1 * quaternion->q2 + 2 * quaternion->q0 * quaternion->q3,
                             -2 * quaternion->q2 * quaternion->q2 - 2 * quaternion->q3 * quaternion->q3 + 1) * 57.3;

    if (s_userFcSubscriptionDataShow == true) {
        if (s_userFcSubscriptionDataCnt++ % ZIYAN_DATA_SUBSCRIPTION_TOPIC_50_HZ == 0) {
            USER_LOG_INFO("receive quaternion data.");
            USER_LOG_INFO("timestamp: millisecond %u microsecond %u.", timestamp->millisecond,
                          timestamp->microsecond);
            USER_LOG_INFO("quaternion: %f %f %f %f.", quaternion->q0, quaternion->q1, quaternion->q2,
                          quaternion->q3);

            USER_LOG_INFO("euler angles: pitch = %.2f roll = %.2f yaw = %.2f.\r\n", pitch, roll, yaw);
            ZiyanTest_WidgetLogAppend("pitch = %.2f roll = %.2f yaw = %.2f.", pitch, roll, yaw);
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_ZiyanReturnCode ZiyanTest_FcSubscriptionReceivePositionFusedCallback(const uint8_t *data, uint16_t dataSize,
                                                                          const T_ZiyanDataTimestamp *timestamp)
{
    USER_UTIL_UNUSED(dataSize);
    T_ZiyanFcSubscriptionPositionFused position_fused;
    position_fused.latitude = ((T_ZiyanFcSubscriptionPositionFused*)data)->latitude;
    position_fused.longitude = ((T_ZiyanFcSubscriptionPositionFused*)data)->longitude;
    position_fused.altitude = ((T_ZiyanFcSubscriptionPositionFused*)data)->altitude;
    position_fused.visibleSatelliteNumber = ((T_ZiyanFcSubscriptionPositionFused*)data)->visibleSatelliteNumber;
    if (s_userFcSubscriptionDataShow == true) {
        USER_LOG_INFO("position fused: %f %f %f : %d\n", position_fused.latitude, position_fused.longitude, position_fused.altitude, position_fused.visibleSatelliteNumber);
    }
    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

/**
 ********************************************************************
 * @file    ziyan_sensor_publication.h
 * @brief   This is the header file for "ziyan_sensor_publication.c", defining the structure and
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
#ifndef ZIYAN_SENSOR_PUBLICATION_H
#define ZIYAN_SENSOR_PUBLICATION_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Sensor type.
 */
typedef enum {
    ZIYAN_SENSOR_TYPE_UNKNOWN        = 0, /*!< */
    ZIYAN_SENSOR_TYPE_RANGE_FINDER   = 1, /*!< */
} E_ZiyanSensorType;


#pragma pack(1)

/**
 * @brief ranger finder data structure.
 */
typedef struct Thermometer {
    ziyan_f32_t temperature; /*!< w, rad (when converted to a rotation matrix or Euler angles). */
} T_ZiyanSensorThermometer;


#pragma pack()

/**
 * @brief ZIYAN_FC_SUBSCRIPTION_TOPIC_ACCELERATION_GROUND sensor data structure.
 */
typedef ziyan_f32_t T_ZiyanSensorRangeFinder;


/**
 * @brief Sensor publication handler.
 */
typedef struct {
    /*! The tpye of sensor. refer to ::E_ZiyanSensorType */
    E_ZiyanSensorType  sensorType;

    /*! The quantity of sensor data. */
    uint8_t sensorQuantity;

    /*! The update interval time of sensor data. unit: ms */
    uint16_t sensorUpdateIntervalTime;

    T_ZiyanReturnCode (*SetSensorSwitch)(uint8_t index, bool enable);

    /**
     * @brief 
     */
    T_ZiyanReturnCode (*GetSensorData)(uint8_t index, uintptr_t* data, uint16_t dataSize);

} T_ZiyanSensorPublicationHandler;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise sensor publication module in blocking mode. This function has to be called before register any
 * data, to initialize run environment of sensor publication module, if need to publish to aircraft.
 * @note Max execution time of this function is slightly larger than 500ms.
 * @note This function has to be called in user task, rather than main() function, and after scheduler being started.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanSensorPublication_Init(void);


/**
 * @brief Deinitialize sensor publication module. When sensor publication will no longer be used, can call this function
 * to deinitialize the sensor publication module. Deinitialization function will help to release all system resource data
 * subscription has occupied.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanSensorPublication_DeInit(void);


/**
 * @brief Register handler for sensor data subscription.
 * @param SensorHandler: sensor handler for sensor data subscription.
 * @return Execution result 
 */
T_ZiyanReturnCode ZiyanSensorPublication_RegSensorHandler(const T_ZiyanSensorPublicationHandler* SensorHandler);


#ifdef __cplusplus
}
#endif

#endif // ZIYAN_PAYLOAD_SENSOR_H

/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/
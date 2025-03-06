/**
 ********************************************************************
 * @file    ziyan_interest_point.h
 * @version V1.0.0
 * @date    2020/11/19
 * @brief   This is the header file for "ziyan_interest_point.c", defining the structure and
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
#ifndef ZIYAN_INTEREST_POINT_H
#define ZIYAN_INTEREST_POINT_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    ziyan_f32_t curSpeed;
    ziyan_f32_t radius;
    uint8_t state;
} T_ZiyanInterestPointMissionState;

typedef struct {
    ziyan_f64_t latitude;
    ziyan_f64_t longitude;
    ziyan_f32_t speed;
} T_ZiyanInterestPointSettings;

typedef T_ZiyanReturnCode (*InterestPointMissionStateCallback)(T_ZiyanInterestPointMissionState missionState);

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode ZiyanInterestPoint_Init(void);

T_ZiyanReturnCode ZiyanInterestPoint_DeInit(void);

T_ZiyanReturnCode ZiyanInterestPoint_Start(T_ZiyanInterestPointSettings settings);

T_ZiyanReturnCode ZiyanInterestPoint_Stop(void);

T_ZiyanReturnCode ZiyanInterestPoint_SetSpeed(ziyan_f32_t speed);

T_ZiyanReturnCode ZiyanInterestPoint_RegMissionStateCallback(InterestPointMissionStateCallback callback);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_INTEREST_POINT_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

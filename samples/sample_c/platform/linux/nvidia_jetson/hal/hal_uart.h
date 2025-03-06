/**
 ********************************************************************
 * @file    hal_uart.h
 * @brief   This is the header file for "hal_uart.c", defining the structure and
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
#ifndef HAL_UART_H
#define HAL_UART_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "stdlib.h"

#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
//User can config dev based on there environmental conditions
#define LINUX_UART_DEV1    "/dev/ttyUSB0"
#define LINUX_UART_DEV2    "/dev/ttyACM0"

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode HalUart_Init(E_ZiyanHalUartNum uartNum, uint32_t baudRate, T_ZiyanUartHandle *uartHandle);
T_ZiyanReturnCode HalUart_DeInit(T_ZiyanUartHandle uartHandle);
T_ZiyanReturnCode HalUart_WriteData(T_ZiyanUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen);
T_ZiyanReturnCode HalUart_ReadData(T_ZiyanUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);
T_ZiyanReturnCode HalUart_GetStatus(E_ZiyanHalUartNum uartNum, T_ZiyanUartStatus *status);

#ifdef __cplusplus
}
#endif

#endif // HAL_UART_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

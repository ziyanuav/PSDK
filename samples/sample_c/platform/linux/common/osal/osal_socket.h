/**
 ********************************************************************
 * @file    osal_socket.h
 * @brief   This is the header file for "osal_socket.c", defining the structure and
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
#ifndef OSAL_SOCKET_H
#define OSAL_SOCKET_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode Osal_Socket(E_ZiyanSocketMode mode, T_ZiyanSocketHandle *socketHandle);

T_ZiyanReturnCode Osal_Close(T_ZiyanSocketHandle socketHandle);

T_ZiyanReturnCode Osal_Bind(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port);

T_ZiyanReturnCode Osal_UdpSendData(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_ZiyanReturnCode Osal_UdpRecvData(T_ZiyanSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen);

T_ZiyanReturnCode Osal_TcpListen(T_ZiyanSocketHandle socketHandle);

T_ZiyanReturnCode Osal_TcpAccept(T_ZiyanSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                               T_ZiyanSocketHandle *outSocketHandle);

T_ZiyanReturnCode Osal_TcpConnect(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port);

T_ZiyanReturnCode Osal_TcpSendData(T_ZiyanSocketHandle socketHandle,
                                 const uint8_t *buf, uint32_t len, uint32_t *realLen);

T_ZiyanReturnCode Osal_TcpRecvData(T_ZiyanSocketHandle socketHandle,
                                 uint8_t *buf, uint32_t len, uint32_t *realLen);

#ifdef __cplusplus
}
#endif

#endif // OSAL_SOCKET_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

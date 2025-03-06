/**
 ********************************************************************
 * @file    hal_usb_bulk.h
 * @brief   This is the header file for "hal_usb_bulk.c", defining the structure and
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
#ifndef HAL_USB_BULK_H
#define HAL_USB_BULK_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef LIBUSB_INSTALLED

#include <libusb-1.0/libusb.h>

#endif

#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define LINUX_USB_BULK1_EP_OUT_FD               "/dev/usb-ffs/bulk1/ep1"
#define LINUX_USB_BULK1_EP_IN_FD                "/dev/usb-ffs/bulk1/ep2"

#define LINUX_USB_BULK1_INTERFACE_NUM           (7)
#define LINUX_USB_BULK1_END_POINT_IN            (0x88)
#define LINUX_USB_BULK1_END_POINT_OUT           (5)

#define LINUX_USB_BULK2_EP_OUT_FD               "/dev/usb-ffs/bulk2/ep1"
#define LINUX_USB_BULK2_EP_IN_FD                "/dev/usb-ffs/bulk2/ep2"

#define LINUX_USB_BULK2_INTERFACE_NUM           (8)
#define LINUX_USB_BULK2_END_POINT_IN            (0x89)
#define LINUX_USB_BULK2_END_POINT_OUT           (6)

#ifdef PLATFORM_ARCH_x86_64
#define LINUX_USB_VID                         (0x0B95)
#define LINUX_USB_PID                         (0x1790)
#else
#define LINUX_USB_VID                         (0x0955)
#define LINUX_USB_PID                         (0x7020)
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode HalUsbBulk_Init(T_ZiyanHalUsbBulkInfo usbBulkInfo, T_ZiyanUsbBulkHandle *usbBulkHandle);
T_ZiyanReturnCode HalUsbBulk_DeInit(T_ZiyanUsbBulkHandle usbBulkHandle);
T_ZiyanReturnCode HalUsbBulk_WriteData(T_ZiyanUsbBulkHandle usbBulkHandle, const uint8_t *buf, uint32_t len,
                                     uint32_t *realLen);
T_ZiyanReturnCode HalUsbBulk_ReadData(T_ZiyanUsbBulkHandle usbBulkHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);
T_ZiyanReturnCode HalUsbBulk_GetDeviceInfo(T_ZiyanHalUsbBulkDeviceInfo *deviceInfo);

#ifdef __cplusplus
}
#endif

#endif // HAL_USB_BULK_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/**
 ********************************************************************
 * @file    ziyan_config_manager.h
 * @brief   This is the header file for "ziyan_config_manager.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2018 ZIYAN. All rights reserved.
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
#ifndef ZIYAN_CONFIG_MANAGER_H
#define ZIYAN_CONFIG_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"
#include "ziyan_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define USER_DEVICE_NAME_STR_MAX_SIZE    (64)

/* Exported types ------------------------------------------------------------*/
typedef enum {
    ZIYAN_USER_LINK_CONFIG_USE_ONLY_UART,
    ZIYAN_USER_LINK_CONFIG_USE_UART_AND_NETWORK_DEVICE,
    ZIYAN_USER_LINK_CONFIG_USE_UART_AND_USB_BULK_DEVICE,
} E_ZiyanUserLinkConfigType;

typedef struct {
    E_ZiyanUserLinkConfigType type;
    struct {
        char uart1DeviceName[USER_DEVICE_NAME_STR_MAX_SIZE];
        bool uart2DeviceEnable;
        char uart2DeviceName[USER_DEVICE_NAME_STR_MAX_SIZE];
    } uartConfig;
    struct {
        char networkDeviceName[USER_DEVICE_NAME_STR_MAX_SIZE];
        // M300/M350 RTK payload port no need
        uint16_t networkUsbAdapterVid;
        uint16_t networkUsbAdapterPid;
    } networkConfig;
    struct {
        uint16_t usbDeviceVid;
        uint16_t usbDevicePid;

        char usbBulk1DeviceName[USER_DEVICE_NAME_STR_MAX_SIZE];
        uint8_t usbBulk1InterfaceNum;
        uint8_t usbBulk1EndpointIn;
        uint8_t usbBulk1EndpointOut;

        char usbBulk2DeviceName[USER_DEVICE_NAME_STR_MAX_SIZE];
        uint8_t usbBulk2InterfaceNum;
        uint8_t usbBulk2EndpointIn;
        uint8_t usbBulk2EndpointOut;
    } usbBulkConfig;
} T_ZiyanUserLinkConfig;

/* Exported functions --------------------------------------------------------*/
T_ZiyanReturnCode ZiyanUserConfigManager_LoadConfiguration(const char *path);
void ZiyanUserConfigManager_GetAppInfo(T_ZiyanUserInfo *userInfo);
void ZiyanUserConfigManager_GetLinkConfig(T_ZiyanUserLinkConfig *linkConfig);
bool ZiyanUserConfigManager_IsEnable(void);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_CONFIG_MANAGER_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

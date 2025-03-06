/**
 ********************************************************************
 * @file    ziyan_config_manager.c
 * @brief
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

/* Includes ------------------------------------------------------------------*/
#include <utils/util_misc.h>
#include <ziyan_logger.h>
#include <utils/util_file.h>
#include <ziyan_aircraft_info.h>
#include "ziyan_config_manager.h"
#include "utils/cJSON.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static T_ZiyanUserInfo s_configManagerUserInfo = {0};
static T_ZiyanUserLinkConfig s_configManagerLinkInfo = {0};
static bool s_configManagerIsEnable = false;

/* Private functions declaration ---------------------------------------------*/
static T_ZiyanReturnCode ZiyanUserConfigManager_GetAppInfoInner(const char *path, T_ZiyanUserInfo *userInfo);
static T_ZiyanReturnCode ZiyanUserConfigManager_GetLinkConfigInner(const char *path, T_ZiyanUserLinkConfig *linkConfig);

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanUserConfigManager_LoadConfiguration(const char *path)
{
    T_ZiyanReturnCode returnCode;

    if (path == NULL) {
        perror("Config file path is null.\n");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    printf("Load configuration start, config file path is: %s\r\n", path);

    returnCode = ZiyanUserConfigManager_GetAppInfoInner(path, &s_configManagerUserInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        perror("Get app info failed.\n");
    }

    returnCode = ZiyanUserConfigManager_GetLinkConfigInner(path, &s_configManagerLinkInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        perror("Get link info failed.\n");
    }

    printf("\r\nLoad configuration successfully.\r\n");

    s_configManagerIsEnable = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

void ZiyanUserConfigManager_GetAppInfo(T_ZiyanUserInfo *userInfo)
{
    memcpy(userInfo, &s_configManagerUserInfo, sizeof(T_ZiyanUserInfo));
}

void ZiyanUserConfigManager_GetLinkConfig(T_ZiyanUserLinkConfig *linkConfig)
{
    memcpy(linkConfig, &s_configManagerLinkInfo, sizeof(T_ZiyanUserLinkConfig));
}

bool ZiyanUserConfigManager_IsEnable(void)
{
    return s_configManagerIsEnable;
}

/* Private functions definition-----------------------------------------------*/
static T_ZiyanReturnCode ZiyanUserConfigManager_GetAppInfoInner(const char *path, T_ZiyanUserInfo *userInfo)
{
    T_ZiyanReturnCode returnCode;
    uint32_t fileSize = 0;
    uint32_t readRealSize = 0;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    uint8_t *jsonData = NULL;
    cJSON *jsonRoot = NULL;
    cJSON *jsonItem = NULL;
    cJSON *jsonValue = NULL;

#ifdef SYSTEM_ARCH_LINUX
    returnCode = UtilFile_GetFileSizeByPath(path, &fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file size by path failed, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    USER_LOG_DEBUG("Get config json file size is %d", fileSize);

    jsonData = osalHandler->Malloc(fileSize + 1);
    if (jsonData == NULL) {
        USER_LOG_ERROR("Malloc failed.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    memset(jsonData, 0, fileSize);

    UtilFile_GetFileDataByPath(path, 0, fileSize, jsonData, &readRealSize);

    jsonData[readRealSize] = '\0';

    jsonRoot = cJSON_Parse((char *) jsonData);
    if (jsonRoot == NULL) {
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        goto jsonDataFree;
    }

    jsonItem = cJSON_GetObjectItem(jsonRoot, "ziyan_sdk_app_info");
    if (jsonItem != NULL) {
        jsonValue = cJSON_GetObjectItem(jsonItem, "user_app_name");
        if (jsonValue != NULL) {
            strcpy(userInfo->appName, jsonValue->valuestring);
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "user_app_id");
        if (jsonValue != NULL) {
            strcpy(userInfo->appId, jsonValue->valuestring);
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "user_app_key");
        if (jsonValue != NULL) {
            strcpy(userInfo->appKey, jsonValue->valuestring);
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "user_app_license");
        if (jsonValue != NULL) {
            strcpy(userInfo->appLicense, jsonValue->valuestring);
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "user_develop_account");
        if (jsonValue != NULL) {
            strcpy(userInfo->developerAccount, jsonValue->valuestring);
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "user_baud_rate");
        if (jsonValue != NULL) {
            strcpy(userInfo->baudRate, jsonValue->valuestring);
        }
    }

    if (strlen(userInfo->appName) >= sizeof(userInfo->appName) ||
        strlen(userInfo->appId) > sizeof(userInfo->appId) ||
        strlen(userInfo->appKey) > sizeof(userInfo->appKey) ||
        strlen(userInfo->appLicense) > sizeof(userInfo->appLicense) ||
        strlen(userInfo->developerAccount) >= sizeof(userInfo->developerAccount) ||
        strlen(userInfo->baudRate) > sizeof(userInfo->baudRate)) {
        USER_LOG_ERROR("Length of user information string is beyond limit. Please check.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
        goto jsonDataFree;
    }

    if (!strcmp(userInfo->appName, "your_app_name") ||
        !strcmp(userInfo->appId, "your_app_id") ||
        !strcmp(userInfo->appKey, "your_app_key") ||
        !strcmp(userInfo->appLicense, "your_app_license") ||
        !strcmp(userInfo->developerAccount, "your_developer_account") ||
        !strcmp(userInfo->baudRate, "your_baud_rate")) {
        USER_LOG_ERROR(
            "Please fill in correct user information to 'samples/sample_c++/platform/linux/manifold2/application/ziyan_sdk_config.json' file.");
        returnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
        goto jsonDataFree;
    }

jsonDataFree:
    osalHandler->Free(jsonData);
#endif

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanUserConfigManager_GetLinkConfigInner(const char *path, T_ZiyanUserLinkConfig *linkConfig)
{
    T_ZiyanReturnCode returnCode;
    uint32_t fileSize = 0;
    uint32_t readRealSize = 0;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    uint8_t *jsonData = NULL;
    cJSON *jsonRoot = NULL;
    cJSON *jsonItem = NULL;
    cJSON *jsonValue = NULL;
    cJSON *jsonConfig = NULL;
    int32_t configValue;

#ifdef SYSTEM_ARCH_LINUX

    returnCode = UtilFile_GetFileSizeByPath(path, &fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file size by path failed, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    USER_LOG_DEBUG("Get config json file size is %d", fileSize);

    jsonData = osalHandler->Malloc(fileSize + 1);
    if (jsonData == NULL) {
        USER_LOG_ERROR("Malloc failed.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }

    memset(jsonData, 0, fileSize);

    UtilFile_GetFileDataByPath(path, 0, fileSize, jsonData, &readRealSize);

    jsonData[readRealSize] = '\0';

    jsonRoot = cJSON_Parse((char *) jsonData);
    if (jsonRoot == NULL) {
        goto jsonDataFree;
    }

    jsonItem = cJSON_GetObjectItem(jsonRoot, "ziyan_sdk_link_config");
    if (jsonItem != NULL) {
        jsonValue = cJSON_GetObjectItem(jsonItem, "link_select");
        if (jsonValue != NULL) {
            printf("\r\nSelect link type: %s\r\n", jsonValue->valuestring);
            if (strcmp(jsonValue->valuestring, "use_only_uart") == 0) {
                linkConfig->type = ZIYAN_USER_LINK_CONFIG_USE_ONLY_UART;
            } else if (strcmp(jsonValue->valuestring, "use_uart_and_network_device") == 0) {
                linkConfig->type = ZIYAN_USER_LINK_CONFIG_USE_UART_AND_NETWORK_DEVICE;
            } else if (strcmp(jsonValue->valuestring, "use_uart_and_usb_bulk_device") == 0) {
                linkConfig->type = ZIYAN_USER_LINK_CONFIG_USE_UART_AND_USB_BULK_DEVICE;
            }
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "uart_config");
        if (jsonValue != NULL) {
            jsonConfig = cJSON_GetObjectItem(jsonValue, "uart1_device_name");
            printf("\r\nConfig uart1 device name: %s\r\n", jsonConfig->valuestring);
            strcpy(linkConfig->uartConfig.uart1DeviceName, jsonConfig->valuestring);

            jsonConfig = cJSON_GetObjectItem(jsonValue, "uart2_device_name");
            printf("Config uart2 device name: %s\r\n", jsonConfig->valuestring);
            strcpy(linkConfig->uartConfig.uart2DeviceName, jsonConfig->valuestring);

            jsonConfig = cJSON_GetObjectItem(jsonValue, "uart2_device_enable");
            printf("Config uart2 device enable: %s\r\n", jsonConfig->valuestring);
            if (strcmp(jsonConfig->valuestring, "true") == 0) {
                linkConfig->uartConfig.uart2DeviceEnable = true;
            } else {
                linkConfig->uartConfig.uart2DeviceEnable = false;
            }
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "network_config");
        if (jsonValue != NULL) {
            jsonConfig = cJSON_GetObjectItem(jsonValue, "network_device_name");
            printf("\r\nConfig network device name: %s\r\n", jsonConfig->valuestring);
            strcpy(linkConfig->networkConfig.networkDeviceName, jsonConfig->valuestring);

            jsonConfig = cJSON_GetObjectItem(jsonValue, "network_usb_adapter_vid");
            printf("Config network usb adapter vid: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->networkConfig.networkUsbAdapterVid = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "network_usb_adapter_pid");
            printf("Config network usb adapter vid: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->networkConfig.networkUsbAdapterPid = configValue;
        }

        jsonValue = cJSON_GetObjectItem(jsonItem, "usb_bulk_config");
        if (jsonValue != NULL) {
            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_device_vid");
            printf("\r\nConfig usb device vid: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbDeviceVid = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_device_pid");
            printf("Config usb device pid: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbDevicePid = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk1_device_name");
            printf("Config usb bulk1 device name: %s\r\n", jsonConfig->valuestring);
            strcpy(linkConfig->usbBulkConfig.usbBulk1DeviceName, jsonConfig->valuestring);

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk1_interface_num");
            printf("Config usb bulk1 interface num: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbBulk1InterfaceNum = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk1_endpoint_in");
            printf("Config usb bulk1 endpoint in: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbBulk1EndpointIn = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk1_endpoint_out");
            printf("Config usb bulk1 endpoint out: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbBulk1EndpointOut = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk2_device_name");
            printf("Config usb bulk2 device name: %s\r\n", jsonConfig->valuestring);
            strcpy(linkConfig->usbBulkConfig.usbBulk2DeviceName, jsonConfig->valuestring);

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk2_interface_num");
            printf("Config usb bulk2 interface num: %s\r\n", jsonConfig->valuestring);
            sscanf(jsonConfig->valuestring, "%X", &configValue);
            linkConfig->usbBulkConfig.usbBulk2InterfaceNum = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk2_endpoint_in");
            printf("Config usb bulk2 endpoint in: %s\r\n", jsonConfig->valuestring);
            linkConfig->usbBulkConfig.usbBulk2EndpointIn = configValue;

            jsonConfig = cJSON_GetObjectItem(jsonValue, "usb_bulk2_endpoint_out");
            printf("Config usb bulk2 endpoint out: %s\r\n", jsonConfig->valuestring);
            linkConfig->usbBulkConfig.usbBulk2EndpointOut = configValue;
        }
    }

jsonDataFree:
    osalHandler->Free(jsonData);
#endif

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

/**
 ********************************************************************
 * @file    main.c
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
#include <ziyan_platform.h>
#include <ziyan_logger.h>
#include <ziyan_core.h>
#include <utils/util_misc.h>
#include <errno.h>
#include <signal.h>
#include "monitor/sys_monitor.h"
#include "osal/osal.h"
#include "osal/osal_fs.h"
#include "osal/osal_socket.h"
#include "../hal/hal_uart.h"
#include "../hal/hal_network.h"
#include "../hal/hal_usb_bulk.h"
#include "ziyan_sdk_app_info.h"
#include "ziyan_aircraft_info.h"
#include "widget/test_widget.h"
#include "ziyan_sdk_config.h"



/* Private constants ---------------------------------------------------------*/
#define ZIYAN_LOG_PATH                    "Logs/ZIYAN"
#define ZIYAN_LOG_INDEX_FILE_NAME         "Logs/latest"
#define ZIYAN_LOG_FOLDER_NAME             "Logs"
#define ZIYAN_LOG_PATH_MAX_SIZE           (128)
#define ZIYAN_LOG_FOLDER_NAME_MAX_SIZE    (32)
#define ZIYAN_LOG_MAX_COUNT               (10)
#define ZIYAN_SYSTEM_CMD_STR_MAX_SIZE     (64)
#define ZIYAN_SYSTEM_RESULT_STR_MAX_SIZE  (128)

#define ZIYAN_USE_WIDGET_INTERACTION       0

/* Private types -------------------------------------------------------------*/
typedef struct {
    pid_t tid;
    char name[16];
    float pcpu;
} T_ThreadAttribute;

/* Private values -------------------------------------------------------------*/
static FILE *s_ziyanLogFile;
static FILE *s_ziyanLogFileCnt;
static pthread_t s_monitorThread = 0;

/* Private functions declaration ---------------------------------------------*/
static T_ZiyanReturnCode ZiyanUser_PrepareSystemEnvironment(void);
static T_ZiyanReturnCode ZiyanUser_FillInUserInfo(T_ZiyanUserInfo *userInfo);
static T_ZiyanReturnCode ZiyanUser_PrintConsole(const uint8_t *data, uint16_t dataLen);
static T_ZiyanReturnCode ZiyanUser_LocalWrite(const uint8_t *data, uint16_t dataLen);
static T_ZiyanReturnCode ZiyanUser_LocalWriteFsInit(const char *path);
// static void *ZiyanUser_MonitorTask(void *argument);
// static T_ZiyanReturnCode ZiyanTest_HighPowerApplyPinInit();
// static T_ZiyanReturnCode ZiyanTest_WriteHighPowerApplyPin(E_ZiyanPowerManagementPinState pinState);
static void ZiyanUser_NormalExitHandler(int signalNum);

/* Exported functions definition ---------------------------------------------*/
int main(int argc, char **argv)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanUserInfo userInfo;
    T_ZiyanAircraftInfoBaseInfo aircraftInfoBaseInfo;
    T_ZiyanAircraftVersion aircraftInfoVersion;
    T_ZiyanFirmwareVersion firmwareVersion = {
        .majorVersion   = 1,
        .minorVersion   = 0,
        .modifyVersion  = 0,
        .debugVersion   = 0,
    };

    USER_UTIL_UNUSED(argc);
    USER_UTIL_UNUSED(argv);

    // attention: when the program is hand up ctrl-c will generate the coredump file
    signal(SIGTERM, ZiyanUser_NormalExitHandler);

    /*!< Step 1: Prepare system environment, such as osal, hal uart, console function and so on. */
    returnCode = ZiyanUser_PrepareSystemEnvironment();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Prepare system environment error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    USER_LOG_INFO("run main test: %d:%d", 111, __LINE__);

    /*!< Step 2: Fill your application information in ziyan_sdk_app_info.h and use this interface to fill it. */
    returnCode = ZiyanUser_FillInUserInfo(&userInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Fill user info error, please check user info config");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    // /*!< Step 3: Initialize the Payload SDK core by your application information. */
    returnCode = ZiyanCore_Init(&userInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Core init error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    returnCode = ZiyanAircraftInfo_GetBaseInfo(&aircraftInfoBaseInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    // returnCode = ZiyanAircraftInfo_GetAircraftVersion(&aircraftInfoVersion);
    // if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("get aircraft version info error");
    // } else {
    //     USER_LOG_INFO("Aircraft version is V%02d.%02d.%02d.%02d", aircraftInfoVersion.majorVersion,
    //                     aircraftInfoVersion.minorVersion, aircraftInfoVersion.modifyVersion,
    //                     aircraftInfoVersion.debugVersion);
    // }

    // returnCode = ZiyanCore_SetAlias("PSDK_APPALIAS");
    // if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("set alias error");
    //     return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    // }

    // returnCode = ZiyanCore_SetFirmwareVersion(firmwareVersion);
    // if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("set firmware version error");
    //     return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    // }

    // returnCode = ZiyanCore_SetSerialNumber("PSDK12345678XX");
    // if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    //     USER_LOG_ERROR("set serial number error");
    //     return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    // }

//     /*!< Step 4: Initialize the selected modules by macros in ziyan_sdk_config.h . */
// #ifdef CONFIG_MODULE_SAMPLE_POWER_MANAGEMENT_ON
//     T_ZiyanTestApplyHighPowerHandler applyHighPowerHandler = {
//         .pinInit = ZiyanTest_HighPowerApplyPinInit,
//         .pinWrite = ZiyanTest_WriteHighPowerApplyPin,
//     };

//     returnCode = ZiyanTest_RegApplyHighPowerHandler(&applyHighPowerHandler);
//     if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//         USER_LOG_ERROR("regsiter apply high power handler error");
//     }

//     returnCode = ZiyanTest_PowerManagementStartService();
//     if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//         USER_LOG_ERROR("power management init error");
//     }
// #endif

// #ifdef CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON
//     returnCode = ZiyanTest_DataTransmissionStartService();
//     if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//         USER_LOG_ERROR("widget sample init error");
//     }
// #endif

//     if (aircraftInfoBaseInfo.mountPosition == ZIYAN_MOUNT_POSITION_EXTENSION_PORT &&
//         aircraftInfoBaseInfo.aircraftType == ZIYAN_AIRCRAFT_TYPE_M300_RTK) {
//         returnCode = ZiyanTest_WidgetInteractionStartService();
//         if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//             USER_LOG_ERROR("widget interaction sample init error");
//         }

//         returnCode = ZiyanTest_WidgetSpeakerStartService();
//         if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//             USER_LOG_ERROR("widget speaker test init error");
//         }
//     } else {
#ifdef CONFIG_MODULE_SAMPLE_CAMERA_EMU_ON
        returnCode = ZiyanTest_CameraEmuBaseStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("camera emu common init error");
        }
#endif

#ifdef CONFIG_MODULE_SAMPLE_CAMERA_MEDIA_ON
        returnCode = ZiyanTest_CameraEmuMediaStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("camera emu media init error");
        }
#endif

#ifdef CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON
        returnCode = ZiyanTest_FcSubscriptionStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("data subscription sample init error\n");
        }
#endif

#ifdef CONFIG_MODULE_SAMPLE_GIMBAL_EMU_ON
        if (aircraftInfoBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_NONE ||
            aircraftInfoBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_UNKNOWN) {
            
            if (ZiyanTest_GimbalStartService() != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
                USER_LOG_ERROR("psdk gimbal init error");
            }
        }
#endif

// #ifdef CONFIG_MODULE_SAMPLE_XPORT_ON
//         if (aircraftInfoBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_XPORT) {
//             if (ZiyanTest_XPortStartService() != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//                 USER_LOG_ERROR("psdk xport init error");
//             }
//         }
// #endif

#ifdef CONFIG_MODULE_SAMPLE_WIDGET_ON
#if ZIYAN_USE_WIDGET_INTERACTION
        returnCode = ZiyanTest_WidgetInteractionStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("widget interaction test init error");
        }
#else
        returnCode = ZiyanTest_WidgetStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("widget sample init error");
        }
#endif
#endif

#ifdef CONFIG_MODULE_SAMPLE_WIDGET_SPEAKER_ON
        returnCode = ZiyanTest_WidgetSpeakerStartService();
        if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("widget speaker test init error");
        }
#endif

// #ifdef CONFIG_MODULE_SAMPLE_MOP_CHANNEL_ON
//         returnCode = ZiyanTest_MopChannelStartService();
//         if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//             USER_LOG_ERROR("mop channel sample init error");
//         }
// #endif

// #ifdef CONFIG_MODULE_SAMPLE_PAYLOAD_COLLABORATION_ON
//         if (aircraftInfoBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_SKYPORT_V2 ||
//             aircraftInfoBaseInfo.ziyanAdapterType == ZIYAN_SDK_ADAPTER_TYPE_XPORT) {
//             returnCode = ZiyanTest_PayloadCollaborationStartService();
//             if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//                 USER_LOG_ERROR("Payload collaboration sample init error\n");
//             }
//         }
// #endif

// #ifdef CONFIG_MODULE_SAMPLE_UPGRADE_ON
//         T_ZiyanTestUpgradePlatformOpt linuxUpgradePlatformOpt = {
//             .rebootSystem = ZiyanUpgradePlatformLinux_RebootSystem,
//             .cleanUpgradeProgramFileStoreArea = ZiyanUpgradePlatformLinux_CleanUpgradeProgramFileStoreArea,
//             .createUpgradeProgramFile = ZiyanUpgradePlatformLinux_CreateUpgradeProgramFile,
//             .writeUpgradeProgramFile = ZiyanUpgradePlatformLinux_WriteUpgradeProgramFile,
//             .readUpgradeProgramFile = ZiyanUpgradePlatformLinux_ReadUpgradeProgramFile,
//             .closeUpgradeProgramFile = ZiyanUpgradePlatformLinux_CloseUpgradeProgramFile,
//             .replaceOldProgram = ZiyanUpgradePlatformLinux_ReplaceOldProgram,
//             .setUpgradeRebootState = ZiyanUpgradePlatformLinux_SetUpgradeRebootState,
//             .getUpgradeRebootState = ZiyanUpgradePlatformLinux_GetUpgradeRebootState,
//             .cleanUpgradeRebootState = ZiyanUpgradePlatformLinux_CleanUpgradeRebootState,
//         };
//         T_ZiyanTestUpgradeConfig testUpgradeConfig = {
//             .firmwareVersion = firmwareVersion,
//             .transferType = ZIYAN_FIRMWARE_TRANSFER_TYPE_DCFTP,
//             .needReplaceProgramBeforeReboot = true
//         };
//         if (ZiyanTest_UpgradeStartService(&linuxUpgradePlatformOpt, testUpgradeConfig) !=
//             ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//             USER_LOG_ERROR("psdk upgrade init error");
//         }
// #endif

// #ifdef CONFIG_MODULE_SAMPLE_HMS_CUSTOMIZATION_ON
//         returnCode = ZiyanTest_HmsCustomizationStartService();
//         if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
//             USER_LOG_ERROR("hms test init error");
//         }
// #endif
//     }

    // /*!< Step 5: Tell the ZIYAN Pilot you are ready. */
    returnCode = ZiyanCore_ApplicationStart();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("start sdk application error");
    }

    // if (pthread_create(&s_monitorThread, NULL, ZiyanUser_MonitorTask, NULL) != 0) {
    //     USER_LOG_ERROR("create monitor task fail.");
    // }

    // if (pthread_setname_np(s_monitorThread, "monitor task") != 0) {
    //     USER_LOG_ERROR("set name for monitor task fail.");
    // }

    while (1) {
        sleep(1);
    }
}

/* Private functions definition-----------------------------------------------*/
static T_ZiyanReturnCode ZiyanUser_PrepareSystemEnvironment(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler osalHandler = {
        .TaskCreate = Osal_TaskCreate,
        .TaskDestroy = Osal_TaskDestroy,
        .TaskSleepMs = Osal_TaskSleepMs,
        .MutexCreate= Osal_MutexCreate,
        .MutexDestroy = Osal_MutexDestroy,
        .MutexLock = Osal_MutexLock,
        .MutexUnlock = Osal_MutexUnlock,
        .SemaphoreCreate = Osal_SemaphoreCreate,
        .SemaphoreDestroy = Osal_SemaphoreDestroy,
        .SemaphoreWait = Osal_SemaphoreWait,
        .SemaphoreTimedWait = Osal_SemaphoreTimedWait,
        .SemaphorePost = Osal_SemaphorePost,
        .Malloc = Osal_Malloc,
        .Free = Osal_Free,
        .GetRandomNum = Osal_GetRandomNum,
        .GetTimeMs = Osal_GetTimeMs,
        .GetTimeUs = Osal_GetTimeUs,
    };

    T_ZiyanLoggerConsole printConsole = {
        .func = ZiyanUser_PrintConsole,
        .consoleLevel = ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_INFO,
        .isSupportColor = true,
    };

    T_ZiyanLoggerConsole localRecordConsole = {
        .consoleLevel = ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_DEBUG,
        .func = ZiyanUser_LocalWrite,
        .isSupportColor = true,
    };

    T_ZiyanHalUartHandler uartHandler = {
        .UartInit = HalUart_Init,
        .UartDeInit = HalUart_DeInit,
        .UartWriteData = HalUart_WriteData,
        .UartReadData = HalUart_ReadData,
        .UartGetStatus = HalUart_GetStatus,
    };

    T_ZiyanHalNetworkHandler networkHandler = {
        .NetworkInit = HalNetWork_Init,
        .NetworkDeInit = HalNetWork_DeInit,
        .NetworkGetDeviceInfo = HalNetWork_GetDeviceInfo,
    };

    T_ZiyanHalUsbBulkHandler usbBulkHandler = {
        .UsbBulkInit = HalUsbBulk_Init,
        .UsbBulkDeInit = HalUsbBulk_DeInit,
        .UsbBulkWriteData = HalUsbBulk_WriteData,
        .UsbBulkReadData = HalUsbBulk_ReadData,
        .UsbBulkGetDeviceInfo = HalUsbBulk_GetDeviceInfo,
    };

    T_ZiyanFileSystemHandler fileSystemHandler = {
        .FileOpen = Osal_FileOpen,
        .FileClose = Osal_FileClose,
        .FileWrite = Osal_FileWrite,
        .FileRead = Osal_FileRead,
        .FileSync = Osal_FileSync,
        .FileSeek = Osal_FileSeek,
        .DirOpen = Osal_DirOpen,
        .DirClose = Osal_DirClose,
        .DirRead = Osal_DirRead,
        .Mkdir = Osal_Mkdir,
        .Unlink = Osal_Unlink,
        .Rename = Osal_Rename,
        .Stat = Osal_Stat,
    };

    T_ZiyanSocketHandler socketHandler = {
        .Socket = Osal_Socket,
        .Bind = Osal_Bind,
        .Close = Osal_Close,
        .UdpSendData = Osal_UdpSendData,
        .UdpRecvData = Osal_UdpRecvData,
        .TcpListen = Osal_TcpListen,
        .TcpAccept = Osal_TcpAccept,
        .TcpConnect = Osal_TcpConnect,
        .TcpSendData = Osal_TcpSendData,
        .TcpRecvData = Osal_TcpRecvData,
    };

    returnCode = ZiyanPlatform_RegOsalHandler(&osalHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    returnCode = ZiyanPlatform_RegHalUartHandler(&uartHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal uart handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (ZiyanUser_LocalWriteFsInit(ZIYAN_LOG_PATH) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("file system init error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = ZiyanLogger_AddConsole(&printConsole);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("add printf console error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    returnCode = ZiyanLogger_AddConsole(&localRecordConsole);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("add printf console error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }


#if (CONFIG_HARDWARE_CONNECTION == ZIYAN_USE_UART_AND_USB_BULK_DEVICE)
    returnCode = ZiyanPlatform_RegHalUsbBulkHandler(&usbBulkHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal usb bulk handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#elif (CONFIG_HARDWARE_CONNECTION == ZIYAN_USE_UART_AND_NETWORK_DEVICE)
    returnCode = ZiyanPlatform_RegHalNetworkHandler(&networkHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register hal network handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    //Attention: if you want to use camera stream view function, please uncomment it.
    returnCode = ZiyanPlatform_RegSocketHandler(&socketHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal socket handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
#elif (CONFIG_HARDWARE_CONNECTION == ZIYAN_USE_ONLY_UART)
    /*!< Attention: Only use uart hardware connection.
     */
#endif

    returnCode = ZiyanPlatform_RegFileSystemHandler(&fileSystemHandler);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        printf("register osal filesystem handler error");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanUser_FillInUserInfo(T_ZiyanUserInfo *userInfo)
{
    if (userInfo == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memset(userInfo->appName, 0, sizeof(userInfo->appName));
    memset(userInfo->appId, 0, sizeof(userInfo->appId));
    memset(userInfo->appKey, 0, sizeof(userInfo->appKey));
    memset(userInfo->appLicense, 0, sizeof(userInfo->appLicense));
    memset(userInfo->developerAccount, 0, sizeof(userInfo->developerAccount));
    memset(userInfo->baudRate, 0, sizeof(userInfo->baudRate));

    if (strlen(USER_APP_NAME) >= sizeof(userInfo->appName) ||
        strlen(USER_APP_ID) > sizeof(userInfo->appId) ||
        strlen(USER_APP_KEY) > sizeof(userInfo->appKey) ||
        strlen(USER_APP_LICENSE) > sizeof(userInfo->appLicense) ||
        strlen(USER_DEVELOPER_ACCOUNT) >= sizeof(userInfo->developerAccount) ||
        strlen(USER_BAUD_RATE) > sizeof(userInfo->baudRate)) {
        USER_LOG_ERROR("Length of user information string is beyond limit. Please check.");
        sleep(1);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (!strcmp(USER_APP_NAME, "your_app_name") ||
        !strcmp(USER_APP_ID, "your_app_id") ||
        !strcmp(USER_APP_KEY, "your_app_key") ||
        !strcmp(USER_BAUD_RATE, "your_app_license") ||
        !strcmp(USER_DEVELOPER_ACCOUNT, "your_developer_account") ||
        !strcmp(USER_BAUD_RATE, "your_baud_rate")) {
        USER_LOG_ERROR(
            "Please fill in correct user information to 'samples/sample_c/platform/linux/manifold2/application/ziyan_sdk_app_info.h' file.");
        sleep(1);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    strncpy(userInfo->appName, USER_APP_NAME, sizeof(userInfo->appName) - 1);
    memcpy(userInfo->appId, USER_APP_ID, USER_UTIL_MIN(sizeof(userInfo->appId), strlen(USER_APP_ID)));
    memcpy(userInfo->appKey, USER_APP_KEY, USER_UTIL_MIN(sizeof(userInfo->appKey), strlen(USER_APP_KEY)));
    memcpy(userInfo->appLicense, USER_APP_LICENSE,
           USER_UTIL_MIN(sizeof(userInfo->appLicense), strlen(USER_APP_LICENSE)));
    memcpy(userInfo->baudRate, USER_BAUD_RATE, USER_UTIL_MIN(sizeof(userInfo->baudRate), strlen(USER_BAUD_RATE)));
    strncpy(userInfo->developerAccount, USER_DEVELOPER_ACCOUNT, sizeof(userInfo->developerAccount) - 1);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanUser_PrintConsole(const uint8_t *data, uint16_t dataLen)
{
    USER_UTIL_UNUSED(dataLen);

    printf("%s", data);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanUser_LocalWrite(const uint8_t *data, uint16_t dataLen)
{
    uint32_t realLen;

    if (s_ziyanLogFile == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    realLen = fwrite(data, 1, dataLen, s_ziyanLogFile);
    fflush(s_ziyanLogFile);
    if (realLen == dataLen) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    } else {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
}

static T_ZiyanReturnCode ZiyanUser_LocalWriteFsInit(const char *path)
{
    T_ZiyanReturnCode ziyanReturnCode = ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    char filePath[ZIYAN_LOG_PATH_MAX_SIZE];
    char systemCmd[ZIYAN_SYSTEM_CMD_STR_MAX_SIZE];
    char folderName[ZIYAN_LOG_FOLDER_NAME_MAX_SIZE];
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    uint16_t logFileIndex = 0;
    uint16_t currentLogFileIndex;
    uint8_t ret;

    if (localTime == NULL) {
        printf("Get local time error.\r\n");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (access(ZIYAN_LOG_FOLDER_NAME, F_OK) != 0) {
        sprintf(folderName, "mkdir %s", ZIYAN_LOG_FOLDER_NAME);
        ret = system(folderName);
        if (ret != 0) {
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    }

    s_ziyanLogFileCnt = fopen(ZIYAN_LOG_INDEX_FILE_NAME, "rb+");
    if (s_ziyanLogFileCnt == NULL) {
        s_ziyanLogFileCnt = fopen(ZIYAN_LOG_INDEX_FILE_NAME, "wb+");
        if (s_ziyanLogFileCnt == NULL) {
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    } else {
        ret = fseek(s_ziyanLogFileCnt, 0, SEEK_SET);
        if (ret != 0) {
            printf("Seek log count file error, ret: %d, errno: %d.\r\n", ret, errno);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ret = fread((uint16_t *) &logFileIndex, 1, sizeof(uint16_t), s_ziyanLogFileCnt);
        if (ret != sizeof(uint16_t)) {
            printf("Read log file index error.\r\n");
        }
    }

    currentLogFileIndex = logFileIndex;
    logFileIndex++;

    ret = fseek(s_ziyanLogFileCnt, 0, SEEK_SET);
    if (ret != 0) {
        printf("Seek log file error, ret: %d, errno: %d.\r\n", ret, errno);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    ret = fwrite((uint16_t *) &logFileIndex, 1, sizeof(uint16_t), s_ziyanLogFileCnt);
    if (ret != sizeof(uint16_t)) {
        printf("Write log file index error.\r\n");
        fclose(s_ziyanLogFileCnt);
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    fclose(s_ziyanLogFileCnt);

    sprintf(filePath, "%s_%04d_%04d%02d%02d_%02d-%02d-%02d.log", path, currentLogFileIndex,
            localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    s_ziyanLogFile = fopen(filePath, "wb+");
    if (s_ziyanLogFile == NULL) {
        USER_LOG_ERROR("Open filepath time error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (logFileIndex >= ZIYAN_LOG_MAX_COUNT) {
        sprintf(systemCmd, "rm -rf %s_%04d*.log", path, currentLogFileIndex - ZIYAN_LOG_MAX_COUNT);
        ret = system(systemCmd);
        if (ret != 0) {
            printf("Remove file error, ret:%d.\r\n", ret);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }
    }

    return ziyanReturnCode;
}

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wmissing-noreturn"
// #pragma GCC diagnostic ignored "-Wreturn-type"

// static void *ZiyanUser_MonitorTask(void *argument)
// {
//     unsigned int i = 0;
//     unsigned int threadCount = 0;
//     pid_t *tidList = NULL;
//     T_ThreadAttribute *threadAttribute = NULL;
//     T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

//     USER_UTIL_UNUSED(argument);

//     while (1) {
//         threadCount = Monitor_GetThreadCountOfProcess(getpid());
//         tidList = osalHandler->Malloc(threadCount * sizeof(pid_t));
//         if (tidList == NULL) {
//             USER_LOG_ERROR("malloc fail.");
//             goto delay;
//         }
//         Monitor_GetTidListOfProcess(getpid(), tidList, threadCount);

//         threadAttribute = osalHandler->Malloc(threadCount * sizeof(T_ThreadAttribute));
//         if (threadAttribute == NULL) {
//             USER_LOG_ERROR("malloc fail.");
//             goto freeTidList;
//         }
//         for (i = 0; i < threadCount; ++i) {
//             threadAttribute[i].tid = tidList[i];
//         }

//         USER_LOG_DEBUG("thread pcpu:");
//         USER_LOG_DEBUG("tid\tname\tpcpu");
//         for (i = 0; i < threadCount; ++i) {
//             threadAttribute[i].pcpu = Monitor_GetPcpuOfThread(getpid(), tidList[i]);
//             Monitor_GetNameOfThread(getpid(), tidList[i], threadAttribute[i].name, sizeof(threadAttribute[i].name));
//             USER_LOG_DEBUG("%d\t%15s\t%f %%.", threadAttribute[i].tid, threadAttribute[i].name,
//                            threadAttribute[i].pcpu);
//         }

//         USER_LOG_DEBUG("heap used: %d B.", Monitor_GetHeapUsed(getpid()));
//         USER_LOG_DEBUG("stack used: %d B.", Monitor_GetStackUsed(getpid()));

//         osalHandler->Free(threadAttribute);
// freeTidList:
//         osalHandler->Free(tidList);

// delay:
//         sleep(10);
//     }
// }

// // static T_ZiyanReturnCode ZiyanTest_HighPowerApplyPinInit()
// // {
// //     return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
// // }

// // static T_ZiyanReturnCode ZiyanTest_WriteHighPowerApplyPin(E_ZiyanPowerManagementPinState pinState)
// // {
// //     //attention: please pull up the HWPR pin state by hardware.
// //     return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
// // }

static void ZiyanUser_NormalExitHandler(int signalNum)
{
    USER_UTIL_UNUSED(signalNum);
    exit(0);
}

#pragma GCC diagnostic pop

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

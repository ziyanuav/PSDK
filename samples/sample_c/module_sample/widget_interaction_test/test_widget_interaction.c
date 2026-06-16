/**
 ********************************************************************
 * @file    test_widget_interaction.c
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
#include "test_widget_interaction.h"
#include <ziyan_widget.h>
#include <ziyan_logger.h>
#include "../utils/util_misc.h"
#include <ziyan_platform.h>
#include <stdio.h>
// #include <waypoint_v2/test_waypoint_v2.h>
#include <hms/test_hms.h>
// #include <gimbal_manager/test_gimbal_manager.h>
#include <camera_manager/test_camera_manager.h>
// #include <flight_control/test_flight_control.h>
#include <fc_subscription/test_fc_subscription.h>
#include <ziyan_perception.h>
// #include "liveview/test_liveview.h"
// #include "perception/test_perception.h"
#include "file_binary_array_list_en.h"
#include <stdarg.h>
#include "ziyan_aircraft_info.h"
#include "ziyan_core.h"
// #include <payload_collaboration/test_payload_collaboration.h>
// #include <waypoint_v3/test_waypoint_v3.h>
#include "ziyan_sdk_config.h"
#include "hms/hms_text_c/en/hms_text_config_json.h"
#include "ziyan_hms.h"

/* Private constants ---------------------------------------------------------*/
#define WIDGET_DIR_PATH_LEN_MAX         (256)
#define WIDGET_TASK_STACK_SIZE          (2048)

#define WIDGET_LOG_STRING_MAX_SIZE      (40)
#define WIDGET_LOG_LINE_MAX_NUM         (5)

#define ZIYAN_HMS_ERROR_CODE_VALUE0    0x1E020000
#define ZIYAN_HMS_ERROR_CODE_VALUE1    0x1E020001
#define ZIYAN_HMS_ERROR_CODE_VALUE2    0x1E020002
#define ZIYAN_HMS_ERROR_CODE_VALUE3    0x1E020003
#define ZIYAN_HMS_ERROR_CODE_VALUE4    0x1E020004


/* Private types -------------------------------------------------------------*/
typedef enum {
    E_ZIYAN_SAMPLE_INDEX_FC_SUBSCRIPTION = 0,
    E_ZIYAN_SAMPLE_INDEX_WAYPOINT_V2 = 1,
    E_ZIYAN_SAMPLE_INDEX_WAYPOINT_V3 = 2,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_LANDING = 3,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_POSITION_CTRL_LANDING = 4,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_GO_HOME_FORCE_LANDING = 5,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_VELOCITY_CTRL_LANDING = 6,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_ARREST_FLYING = 7,
    E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_SET_GET_PARAM = 8,
    E_ZIYAN_SAMPLE_INDEX_HMS_MANAGER = 9,
    E_ZIYAN_SAMPLE_INDEX_GIMBAL_MANAGER_FREE_MODE = 10,
    E_ZIYAN_SAMPLE_INDEX_GIMBAL_MANAGER_YAW_FOLLOW_MODE = 11,
    E_ZIYAN_SAMPLE_INDEX_LIVEVIEW = 12,
    E_ZIYAN_SAMPLE_INDEX_PERCEPTION = 13,
    E_ZIYAN_SAMPLE_INDEX_SWITCH_ALIAS = 14,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_SHUTTER_SPEED = 15,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_APERTURE = 16,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_EV = 17,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_ISO = 18,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_FOCUS_POINT = 19,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_TAP_ZOOM = 20,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_OPTICAL_ZOOM = 21,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_SINGLE_PHOTO = 22,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_BURST_PHOTO = 23,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_INTERVAL_PHOTO = 24,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_RECORDER_VIDEO = 25,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_MEDIA_DOWNLOAD = 26,
    E_ZIYAN_SAMPLE_INDEX_CAMMGR_THERMOMETRY = 27,
    E_ZIYAN_SAMPLE_INDEX_UNKNOWN = 0xFF,
} E_ZiyanExtensionPortSampleIndex;

typedef enum {
    E_ZIYAN_HMS_ERROR_CODE_INDEX1 = 0,
    E_ZIYAN_HMS_ERROR_CODE_INDEX2,
    E_ZIYAN_HMS_ERROR_CODE_INDEX3,
    E_ZIYAN_HMS_ERROR_CODE_INDEX4,
    E_ZIYAN_HMS_ERROR_CODE_INDEX5,
} E_ZiyanExtensionPortHmsErrorCodeIndex;

typedef enum {
    E_ZIYAN_HMS_ERROR_LEVEL_INDEX1 = 0,
    E_ZIYAN_HMS_ERROR_LEVEL_INDEX2,
    E_ZIYAN_HMS_ERROR_LEVEL_INDEX3,
    E_ZIYAN_HMS_ERROR_LEVEL_INDEX4,
    E_ZIYAN_HMS_ERROR_LEVEL_INDEX5,
} E_ZiyanExtensionPortHmsErrorLevelIndex;

typedef struct {
    bool valid;
    char content[WIDGET_LOG_STRING_MAX_SIZE];
} T_ZiyanTestWidgetLog;

/* Private functions declaration ---------------------------------------------*/
static void *ZiyanTest_WidgetTask(void *arg);
static void *ZiyanTest_WidgetInteractionTask(void *arg);
static T_ZiyanReturnCode ZiyanTestWidget_SetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData);
static T_ZiyanReturnCode ZiyanTestWidget_GetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t *value,
                                                    void *userData);
static T_ZiyanReturnCode ZiyanTestWidget_TriggerChangeAlias(void);

/* Private values ------------------------------------------------------------*/
static T_ZiyanTaskHandle s_widgetTestThread;
static T_ZiyanTaskHandle s_widgetInteractionTestThread;
static E_ZiyanExtensionPortSampleIndex s_extensionPortSampleIndex = E_ZIYAN_SAMPLE_INDEX_FC_SUBSCRIPTION;
static E_ZiyanExtensionPortHmsErrorCodeIndex s_extensionPortErrcodeIndex = E_ZIYAN_HMS_ERROR_CODE_INDEX1;
static E_ZiyanExtensionPortHmsErrorLevelIndex s_extensionPortErrLevelIndex = E_ZIYAN_HMS_ERROR_LEVEL_INDEX1;
static bool s_isInjectErrcode = false;
static bool s_isEliminateErrcode = false;
static bool s_isallowRunFlightControlSample = false;
static bool s_isSampleStart = false;
static E_ZiyanMountPosition s_mountPosition = ZIYAN_MOUNT_POSITION_PAYLOAD_PORT_NO1;
static T_ZiyanTestWidgetLog s_ziyanTestWidgetLog[WIDGET_LOG_LINE_MAX_NUM] = {0};
static T_ZiyanAircraftInfoBaseInfo s_aircraftInfoBaseInfo = {0};
static bool s_isAliasChanged = false;

static const T_ZiyanWidgetHandlerListItem s_widgetHandlerList[] = {
    {0,  ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {1,  ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {2,  ZIYAN_WIDGET_TYPE_SWITCH,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {3,  ZIYAN_WIDGET_TYPE_SCALE,         ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {4,  ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {5,  ZIYAN_WIDGET_TYPE_SCALE,         ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {6,  ZIYAN_WIDGET_TYPE_INT_INPUT_BOX, ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {7,  ZIYAN_WIDGET_TYPE_SWITCH,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {8,  ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {9,  ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {10, ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {11, ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {12, ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {13, ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {14, ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
};

static const char *s_widgetTypeNameArray[] = {
    "Unknown",
    "Button",
    "Switch",
    "Scale",
    "List",
    "Int input box"
};

static const uint32_t s_widgetHandlerListCount = sizeof(s_widgetHandlerList) / sizeof(T_ZiyanWidgetHandlerListItem);
static int32_t s_widgetValueList[sizeof(s_widgetHandlerList) / sizeof(T_ZiyanWidgetHandlerListItem)] = {0};
static bool s_isWidgetFileDirPathConfigured = false;
static char s_widgetFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX] = {0};

/* Exported functions definition ---------------------------------------------*/
void ZiyanTest_WidgetLogAppend(const char *fmt, ...)
{
    va_list args;
    char string[WIDGET_LOG_STRING_MAX_SIZE];
    int32_t i;

    va_start(args, fmt);
    vsnprintf(string, WIDGET_LOG_STRING_MAX_SIZE, fmt, args);
    va_end(args);

    for (i = 0; i < WIDGET_LOG_LINE_MAX_NUM; ++i) {
        if (s_ziyanTestWidgetLog[i].valid == false) {
            s_ziyanTestWidgetLog[i].valid = true;
            strcpy(s_ziyanTestWidgetLog[i].content, string);
            break;
        }
    }

    if (i == WIDGET_LOG_LINE_MAX_NUM) {
        for (i = 0; i < WIDGET_LOG_LINE_MAX_NUM - 1; i++) {
            strcpy(s_ziyanTestWidgetLog[i].content, s_ziyanTestWidgetLog[i + 1].content);
        }
        strcpy(s_ziyanTestWidgetLog[WIDGET_LOG_LINE_MAX_NUM - 1].content, string);
    }
}

T_ZiyanReturnCode ZiyanTest_WidgetInteractionStartService(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    //Step 1 : Init ZIYAN Widget
    ziyanStat = ZiyanWidget_Init();
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Ziyan test widget init error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

#ifdef SYSTEM_ARCH_LINUX_DISABLE
    //Step 2 : Set UI Config (Linux environment)
    char curFileDirPath[WIDGET_DIR_PATH_LEN_MAX];
    char tempPath[WIDGET_DIR_PATH_LEN_MAX];

    ziyanStat = ZiyanUserUtil_GetCurrentFileDirPath(__FILE__, WIDGET_DIR_PATH_LEN_MAX, curFileDirPath);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

    if (s_isWidgetFileDirPathConfigured == true) {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", s_widgetFileDirPath);
    } else {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/en_big_screen", curFileDirPath);
    }

    //set default ui config path
    ziyanStat = ZiyanWidget_RegDefaultUiConfigByDirPath(tempPath);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default widget ui config error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

    //set ui config for English language
    ziyanStat = ZiyanWidget_RegUiConfigByDirPath(ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH,
                                             ZIYAN_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN,
                                             tempPath);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add widget ui config error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

    //set ui config for Chinese language
    if (s_isWidgetFileDirPathConfigured == true) {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/cn_big_screen", s_widgetFileDirPath);
    } else {
        snprintf(tempPath, WIDGET_DIR_PATH_LEN_MAX, "%swidget_file/cn_big_screen", curFileDirPath);
    }

    ziyanStat = ZiyanWidget_RegUiConfigByDirPath(ZIYAN_MOBILE_APP_LANGUAGE_CHINESE,
                                             ZIYAN_MOBILE_APP_SCREEN_TYPE_BIG_SCREEN,
                                             tempPath);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add widget ui config error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }
#else
    //Step 2 : Set UI Config (RTOS environment)
    T_ZiyanWidgetBinaryArrayConfig enWidgetBinaryArrayConfig = {
        .binaryArrayCount = g_testEnBinaryArrayCount,
        .fileBinaryArrayList = g_testEnFileBinaryArrayList
    };

    //set default ui config
    ziyanStat = ZiyanWidget_RegDefaultUiConfigByBinaryArray(&enWidgetBinaryArrayConfig);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default widget ui config error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }
#endif
    //Step 3 : Set widget handler list
    ziyanStat = ZiyanWidget_RegHandlerList(s_widgetHandlerList, s_widgetHandlerListCount);
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Set widget handler list error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

    //Step 4 : Run widget api sample task
    if (osalHandler->TaskCreate("user_widget_task", ZiyanTest_WidgetTask, WIDGET_TASK_STACK_SIZE, NULL,
                                &s_widgetTestThread) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Ziyan widget test task create error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (osalHandler->TaskCreate("user_widget_task", ZiyanTest_WidgetInteractionTask, WIDGET_TASK_STACK_SIZE, NULL,
                                &s_widgetInteractionTestThread) != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Ziyan widget test task create error.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_WidgetInteractionSetConfigFilePath(const char *path)
{
    memset(s_widgetFileDirPath, 0, sizeof(s_widgetFileDirPath));
    memcpy(s_widgetFileDirPath, path, USER_UTIL_MIN(strlen(path), sizeof(s_widgetFileDirPath) - 1));
    s_isWidgetFileDirPathConfigured = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wformat"
#endif

/* Private functions definition-----------------------------------------------*/
static void *ZiyanTest_WidgetTask(void *arg)
{
    char message[ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN];
    uint32_t sysTimeMs = 0;
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    USER_UTIL_UNUSED(arg);

    while (1) {
        ziyanStat = osalHandler->GetTimeMs(&sysTimeMs);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Get system time ms error, stat = 0x%08llX", ziyanStat);
        }
#ifndef USER_FIRMWARE_MAJOR_VERSION
        snprintf(message, ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN,
                 "System time : %u ms \r\n%s \r\n%s \r\n%s \r\n%s \r\n%s \r\n",
                 sysTimeMs,
                 s_ziyanTestWidgetLog[0].content,
                 s_ziyanTestWidgetLog[1].content,
                 s_ziyanTestWidgetLog[2].content,
                 s_ziyanTestWidgetLog[3].content,
                 s_ziyanTestWidgetLog[4].content);
#else
        snprintf(message, ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN,
                 "System time : %u ms\r\nVersion: v%02d.%02d.%02d.%02d\r\nBuild time: %s %s", sysTimeMs,
                 USER_FIRMWARE_MAJOR_VERSION, USER_FIRMWARE_MINOR_VERSION,
                 USER_FIRMWARE_MODIFY_VERSION, USER_FIRMWARE_DEBUG_VERSION,
                 __DATE__, __TIME__);
#endif

        ziyanStat = ZiyanWidgetFloatingWindow_ShowMessage(message);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            USER_LOG_ERROR("Floating window show message error, stat = 0x%08llX", ziyanStat);
        }

        osalHandler->TaskSleepMs(200);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wformat"
#endif

static void *ZiyanTest_WidgetInteractionTask(void *arg)
{
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();
    T_ZiyanReturnCode returnCode;
    uint32_t errorCode;
    E_ZiyanHmsErrorLevel errorLevel;

    returnCode = ZiyanAircraftInfo_GetBaseInfo(&s_aircraftInfoBaseInfo);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("get aircraft base info error");
        return NULL;
    }

    while (1) {
        osalHandler->TaskSleepMs(100);
        printf("s_isInjectErrcode = %d, s_isEliminateErrcode = %d, s_extensionPortErrcodeIndex = %d, s_extensionPortErrLevelIndex = %d\n", (int)s_isInjectErrcode, (int)s_isEliminateErrcode, (int)s_extensionPortErrcodeIndex, (int)s_extensionPortErrLevelIndex);
        if (s_isInjectErrcode == true && s_isEliminateErrcode == false) {
            switch (s_extensionPortErrcodeIndex) {
                case E_ZIYAN_HMS_ERROR_CODE_INDEX1:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE0;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX2:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE1;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX3:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE2;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX4:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE3;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX5:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE4;
                    break;
                default:
                    break;
            }
            switch (s_extensionPortErrLevelIndex) {
                case E_ZIYAN_HMS_ERROR_LEVEL_INDEX1:
                    errorLevel = ZIYAN_HMS_ERROR_LEVEL_NONE;
                    break;
                case E_ZIYAN_HMS_ERROR_LEVEL_INDEX2:
                    errorLevel = ZIYAN_HMS_ERROR_LEVEL_HINT;
                    break;
                case E_ZIYAN_HMS_ERROR_LEVEL_INDEX3:
                    errorLevel = ZIYAN_HMS_ERROR_LEVEL_WARN;
                    break;
                case E_ZIYAN_HMS_ERROR_LEVEL_INDEX4:
                    errorLevel = ZIYAN_HMS_ERROR_LEVEL_CRITICAL;
                    break;
                case E_ZIYAN_HMS_ERROR_LEVEL_INDEX5:
                    errorLevel = ZIYAN_HMS_ERROR_LEVEL_FATAL;
                    break;
                default:
                    break;
            }
            ZiyanHmsCustomization_InjectHmsErrorCode(errorCode, errorLevel);
            osalHandler->TaskSleepMs(500);
            s_isInjectErrcode = false;
            s_isEliminateErrcode = false;
            continue;
        }
        if (s_isEliminateErrcode == true && s_isInjectErrcode == false) {
            switch (s_extensionPortErrcodeIndex) {
                case E_ZIYAN_HMS_ERROR_CODE_INDEX1:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE0;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX2:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE1;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX3:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE2;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX4:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE3;
                    break;
                case E_ZIYAN_HMS_ERROR_CODE_INDEX5:
                    errorCode = ZIYAN_HMS_ERROR_CODE_VALUE4;
                    break;
                default:
                    break;
            }
            ZiyanHmsCustomization_EliminateHmsErrorCode(errorCode);
            osalHandler->TaskSleepMs(500);
            s_isInjectErrcode = false;
            s_isEliminateErrcode = false;
            continue;
        }

        if (s_isSampleStart != true) {
            continue;
        }

        osalHandler->TaskSleepMs(500);

        s_isSampleStart = false;
        printf("\r\n");
        USER_LOG_INFO("--------------------------------------------------------------------------------------------->");
        ZiyanTest_WidgetLogAppend("-> Sample Start");
        printf("s_extensionPortSampleIndex = %d\n", (int)s_extensionPortSampleIndex);

        // if (s_aircraftInfoBaseInfo.mountPositionType == ZIYAN_MOUNT_POSITION_TYPE_EXTENSION_PORT ||
        //     s_aircraftInfoBaseInfo.mountPositionType == ZIYAN_MOUNT_POSITION_TYPE_EXTENSION_LITE_PORT) {
            switch (s_extensionPortSampleIndex) {
                // case E_ZIYAN_SAMPLE_INDEX_WAYPOINT_V2:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_WaypointV2RunSample();
                //     } else {
                //         ZiyanTest_WidgetLogAppend("Please turn on the 'unlock flight control restrictions'");
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_WAYPOINT_V3:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_WaypointV3RunSample();
                //     } else {
                //         ZiyanTest_WidgetLogAppend("Please turn on the 'unlock flight control restrictions'");
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_LANDING:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_FlightControlRunSample(E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_LANDING);
                //     } else {
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_POSITION_CTRL_LANDING:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_FlightControlRunSample(
                //             E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_POSITION_CTRL_LANDING);
                //     } else {
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_GO_HOME_FORCE_LANDING:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_FlightControlRunSample(
                //             E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_GO_HOME_FORCE_LANDING);
                //     } else {
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_TAKE_OFF_VELOCITY_CTRL_LANDING:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_FlightControlRunSample(
                //             E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_TAKE_OFF_VELOCITY_CTRL_LANDING);
                //     } else {
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_ARREST_FLYING:
                //     if (s_isallowRunFlightControlSample == true) {
                //         ZiyanTest_FlightControlRunSample(E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_ARREST_FLYING);
                //     } else {
                //         USER_LOG_WARN("Please turn on the 'unlock flight control restrictions' switch.");
                //     }
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_FLIGHT_CONTROL_SET_GET_PARAM:
                //     ZiyanTest_FlightControlRunSample(E_ZIYAN_TEST_FLIGHT_CTRL_SAMPLE_SELECT_SET_GET_PARAM);
                //     break;
                case E_ZIYAN_SAMPLE_INDEX_FC_SUBSCRIPTION:
                    ZiyanTest_FcSubscriptionRunSample();
                    break;
                // case E_ZIYAN_SAMPLE_INDEX_GIMBAL_MANAGER_FREE_MODE:
                //     ZiyanTest_GimbalManagerRunSample(s_mountPosition, ZIYAN_GIMBAL_MODE_FREE);
                //     break;
                // case E_ZIYAN_SAMPLE_INDEX_GIMBAL_MANAGER_YAW_FOLLOW_MODE:
                //     ZiyanTest_GimbalManagerRunSample(s_mountPosition, ZIYAN_GIMBAL_MODE_YAW_FOLLOW);
                //     break;
                case E_ZIYAN_SAMPLE_INDEX_HMS_MANAGER:
                    ZiyanTest_HmsManagerRunSample(ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH);
                    break;
//                 case E_ZIYAN_SAMPLE_INDEX_LIVEVIEW:
// #ifdef SYSTEM_ARCH_LINUX
//                     ZiyanTest_LiveviewRunSample(s_mountPosition);
// #else
//                     USER_LOG_WARN("This feature does not support RTOS platform.");
// #endif
//                     break;
//                 case E_ZIYAN_SAMPLE_INDEX_PERCEPTION:
// #ifdef SYSTEM_ARCH_LINUX
//                     ZiyanTest_PerceptionRunSample(ZIYAN_PERCEPTION_RECTIFY_FRONT);
// #else
//                     USER_LOG_WARN("This feature does not support RTOS platform.");
// #endif
//                     break;
                case E_ZIYAN_SAMPLE_INDEX_SWITCH_ALIAS:
                    ZiyanTestWidget_TriggerChangeAlias();
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_SHUTTER_SPEED:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_SHUTTER_SPEED);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_APERTURE:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_APERTURE);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_EV:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_EV);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_ISO:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ISO);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_FOCUS_POINT:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_FOCUS_POINT);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_TAP_ZOOM:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_TAP_ZOOM_POINT);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_OPTICAL_ZOOM:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SET_CAMERA_ZOOM_PARAM);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_SINGLE_PHOTO:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_SINGLE_PHOTO);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_BURST_PHOTO:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_BURST_PHOTO);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_INTERVAL_PHOTO:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_SHOOT_INTERVAL_PHOTO);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_RECORDER_VIDEO:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_RECORD_VIDEO);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_MEDIA_DOWNLOAD:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_DOWNLOAD_AND_DELETE_MEDIA_FILE);
                    break;
                case E_ZIYAN_SAMPLE_INDEX_CAMMGR_THERMOMETRY:
                    ZiyanTest_CameraManagerRunSample(s_mountPosition,
                                                   E_ZIYAN_TEST_CAMERA_MANAGER_SAMPLE_SELECT_THERMOMETRY);
                    break;
                default:
                    break;
            }
        // } else {
        //     USER_LOG_WARN("Can't support on payload port.");
        // }

        USER_LOG_INFO("--------------------------------------------------------------------------------------------->");
        ZiyanTest_WidgetLogAppend("-> Sample End");
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_ZiyanReturnCode ZiyanTestWidget_SetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData)
{
    USER_UTIL_UNUSED(userData);

    ZiyanTest_WidgetLogAppend("SetWidget type:%s index:%d value:%d",
                            s_widgetTypeNameArray[widgetType], index, value);
    USER_LOG_INFO("Set widget value, widgetType = %s, widgetIndex = %d ,widgetValue = %d",
                  s_widgetTypeNameArray[widgetType], index, value);
    s_widgetValueList[index] = value;

    if (widgetType == ZIYAN_WIDGET_TYPE_SWITCH && index == 7) {
        s_isallowRunFlightControlSample = value;
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_LIST && index == 8) {
        s_mountPosition = value + 1;
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_LIST && index == 9) {
        s_extensionPortSampleIndex = value;
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_BUTTON && index == 10) {
        if (value == 1) {
            s_isSampleStart = true;
        }
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_LIST && index == 11) {
        s_extensionPortErrcodeIndex = value;
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_LIST && index == 12) {
        s_extensionPortErrLevelIndex = value;
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_BUTTON && index == 13) {
        if (value == 1) {
            s_isInjectErrcode = true;
            s_isEliminateErrcode = false;
        }
    }

    if (widgetType == ZIYAN_WIDGET_TYPE_BUTTON && index == 14) {
        if (value == 1) {
            s_isInjectErrcode = false;
            s_isEliminateErrcode = true;
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanTestWidget_GetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t *value,
                                                    void *userData)
{
    USER_UTIL_UNUSED(userData);
    USER_UTIL_UNUSED(widgetType);

    *value = s_widgetValueList[index];

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_ZiyanReturnCode ZiyanTestWidget_TriggerChangeAlias(void)
{
    USER_LOG_INFO("Payload alias sample start");

    if (s_isAliasChanged == false) {
        // ZiyanCore_SetAlias("測試test!@#$%^&*()");
        USER_LOG_INFO("Set payload alias to '測試test!@#$%^&*()' ");
        s_isAliasChanged = true;
    } else {
        // ZiyanCore_SetAlias("PSDK_APPALIAS");
        USER_LOG_INFO("Set payload alias to 'PSDK_APPALIAS' ");
        s_isAliasChanged = false;
    }

    USER_LOG_INFO("Payload alias sample end");

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

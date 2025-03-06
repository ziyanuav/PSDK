/**
 ********************************************************************
 * @file    test_widget.c
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
#include "test_widget.h"
#include <ziyan_widget.h>
#include <ziyan_logger.h>
#include "../utils/util_misc.h"
#include <ziyan_platform.h>
#include <stdio.h>
#include "file_binary_array_list_en.h"

/* Private constants ---------------------------------------------------------*/
#define WIDGET_DIR_PATH_LEN_MAX         (256)
#define WIDGET_TASK_STACK_SIZE          (2048)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *ZiyanTest_WidgetTask(void *arg);
static T_ZiyanReturnCode ZiyanTestWidget_SetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData);
static T_ZiyanReturnCode ZiyanTestWidget_GetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t *value,
                                                    void *userData);

/* Private values ------------------------------------------------------------*/
static T_ZiyanTaskHandle s_widgetTestThread;
static bool s_isWidgetFileDirPathConfigured = false;
static char s_widgetFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX] = {0};

static const T_ZiyanWidgetHandlerListItem s_widgetHandlerList[] = {
    {0, ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {1, ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {2, ZIYAN_WIDGET_TYPE_SWITCH,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {3, ZIYAN_WIDGET_TYPE_SCALE,         ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {4, ZIYAN_WIDGET_TYPE_BUTTON,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {5, ZIYAN_WIDGET_TYPE_SCALE,         ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {6, ZIYAN_WIDGET_TYPE_INT_INPUT_BOX, ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {7, ZIYAN_WIDGET_TYPE_SWITCH,        ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
    {8, ZIYAN_WIDGET_TYPE_LIST,          ZiyanTestWidget_SetWidgetValue, ZiyanTestWidget_GetWidgetValue, NULL},
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

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_WidgetStartService(void)
{
    T_ZiyanReturnCode ziyanStat;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    //Step 1 : Init ZIYAN Widget
    ziyanStat = ZiyanWidget_Init();
    if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Ziyan test widget init error, stat = 0x%08llX", ziyanStat);
        return ziyanStat;
    }

#ifdef SYSTEM_ARCH_LINUX
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
        .binaryArrayCount = g_EnBinaryArrayCount,
        .fileBinaryArrayList = g_EnFileBinaryArrayList
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

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode ZiyanTest_WidgetSetConfigFilePath(const char *path)
{
    memset(s_widgetFileDirPath, 0, sizeof(s_widgetFileDirPath));
    memcpy(s_widgetFileDirPath, path, USER_UTIL_MIN(strlen(path), sizeof(s_widgetFileDirPath) - 1));
    s_isWidgetFileDirPathConfigured = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

__attribute__((weak)) void ZiyanTest_WidgetLogAppend(const char *fmt, ...)
{

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
        snprintf(message, ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN, "System time : %u ms", sysTimeMs);
#else
        snprintf(message, ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN,
                 "System time : %u ms\r\nVersion: v%02d.%02d.%02d.%02d\r\nBuild time: %s %s", sysTimeMs,
                 USER_FIRMWARE_MAJOR_VERSION, USER_FIRMWARE_MINOR_VERSION,
                 USER_FIRMWARE_MODIFY_VERSION, USER_FIRMWARE_DEBUG_VERSION,
                 __DATE__, __TIME__);
#endif


        ziyanStat = ZiyanWidgetFloatingWindow_ShowMessage(message);
        if (ziyanStat != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            // USER_LOG_ERROR("Floating window show message error, stat = 0x%08llX", ziyanStat);
        }

        osalHandler->TaskSleepMs(1000);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

#include <stdio.h>
#include <stdlib.h>

static T_ZiyanReturnCode ZiyanTestWidget_SetWidgetValue(E_ZiyanWidgetType widgetType, uint32_t index, int32_t value,
                                                    void *userData)
{
    USER_UTIL_UNUSED(userData);

    USER_LOG_INFO("Set widget value, widgetType = %s, widgetIndex = %d ,widgetValue = %d",
                  s_widgetTypeNameArray[widgetType], index, value);
    s_widgetValueList[index] = value;

    if(index == 0){
        uint8_t brightness = value;
        brightness = (brightness == 0) ? 30  : brightness;
        brightness = (brightness == 1) ? 255 : brightness;

        FILE *fp;
        const char *brightness_file = "/sys/class/backlight/amdgpu_bl1/brightness";  // 根据你的系统路径调整

        fp = fopen(brightness_file, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return -1;
        }

        fprintf(fp, "%d", brightness);  // 写入新的亮度值
        fclose(fp);
    }

    if(index == 1){
        uint8_t brightness = value;
        brightness = (brightness == 0)  ? 30  : brightness;
        brightness = (brightness == 1)  ? 140 : brightness;
        brightness = (brightness == 2)  ? 255 : brightness;

        FILE *fp;
        const char *brightness_file = "/sys/class/backlight/amdgpu_bl1/brightness";  // 根据你的系统路径调整

        fp = fopen(brightness_file, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return -1;
        }

        fprintf(fp, "%d", brightness);  // 写入新的亮度值
        fclose(fp);
    }

    if(index == 2){
        uint8_t brightness = value;
        brightness = (brightness == 0) ? 30  : brightness;
        brightness = (brightness == 1) ? 255 : brightness;

        FILE *fp;
        const char *brightness_file = "/sys/class/backlight/amdgpu_bl1/brightness";  // 根据你的系统路径调整

        fp = fopen(brightness_file, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return -1;
        }

        fprintf(fp, "%d", brightness);  // 写入新的亮度值
        fclose(fp);
    }

    if(index == 3){
        uint8_t brightness = value;
        brightness = (brightness < 30)  ? 30  : brightness;
        brightness = (brightness > 255) ? 255 : brightness;

        FILE *fp;
        const char *brightness_file = "/sys/class/backlight/amdgpu_bl1/brightness";  // 根据你的系统路径调整

        fp = fopen(brightness_file, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return -1;
        }

        fprintf(fp, "%d", brightness);  // 写入新的亮度值
        fclose(fp);
    }

    if(index == 6){
        uint8_t brightness = value;
        brightness = (brightness < 30)  ? 30  : brightness;
        brightness = (brightness > 255) ? 255 : brightness;

        FILE *fp;
        const char *brightness_file = "/sys/class/backlight/amdgpu_bl1/brightness";  // 根据你的系统路径调整

        fp = fopen(brightness_file, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return -1;
        }

        fprintf(fp, "%d", brightness);  // 写入新的亮度值
        fclose(fp);
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

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

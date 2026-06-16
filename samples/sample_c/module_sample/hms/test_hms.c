/**
 ********************************************************************
 * @file    test_hms.c
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
#include <widget_interaction_test/test_widget_interaction.h>
#include <utils/util_misc.h>
#include <utils/cJSON.h>
#include <utils/util_file.h>
#include "test_hms.h"
#include "ziyan_hms.h"
#include "ziyan_hms_info_table.h"
#include "ziyan_logger.h"
#include "ziyan_platform.h"
#include "ziyan_fc_subscription.h"
#include "hms_text_c/en/hms_text_config_json.h"

/* Private constants ---------------------------------------------------------*/
#define MAX_HMS_PRINT_COUNT              (150)
#define MAX_BUFFER_LEN                   (256)
#define MIN_HMS_ERROR_LEVEL              (0)
#define MID_HMS_ERROR_LEVEL              (3)
#define MAX_HMS_ERROR_LEVEL              (6)
#define HMS_DIR_PATH_LEN_MAX             (256)

#define ZIYAN_CUSTOM_HMS_CODE_INJECT_ON    (0)

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static const char *oldReplaceAlarmIdStr = "%alarmid";
static const char *oldReplaceIndexStr = "%index";
static const char *oldReplaceComponentIndexStr = "%component_index";
static bool isHmsManagerInit = false;
static T_ZiyanHmsFileBinaryArray s_EnHmsTextConfigFileBinaryArrayList[] = {
    {hms_text_config_json_fileName, hms_text_config_json_fileSize, hms_text_config_json_fileBinaryArray},
};
static uint8_t *s_hmsJsonData = NULL;
static E_ZiyanMobileAppLanguage s_hmsLanguage = ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH;
static bool s_isHmsConfigFileDirPathConfigured = false;
static char s_hmsConfigFileDirPath[ZIYAN_FILE_PATH_SIZE_MAX - 32] = {0};

/* Private functions declaration ---------------------------------------------*/
static T_ZiyanReturnCode ZiyanTest_HmsManagerInit(void);
static T_ZiyanReturnCode ZiyanTest_HmsManagerDeInit(void);
static T_ZiyanFcSubscriptionFlightStatus ZiyanTest_GetValueOfFlightStatus(void);
static bool ZiyanTest_ReplaceStr(char *buffer, uint32_t bufferMaxLen, const char *target, const char *dest);
static bool ZiyanTest_MarchErrCodeInfoTable(T_ZiyanHmsInfoTable hmsInfoTable);
static bool ZiyanTest_MarchErrCodeInfoTableByJson(T_ZiyanHmsInfoTable hmsInfoTable);
static T_ZiyanReturnCode ZiyanTest_HmsInfoCallback(T_ZiyanHmsInfoTable hmsInfoTable);

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode ZiyanTest_HmsManagerRunSample(E_ZiyanMobileAppLanguage language)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler;

    s_hmsLanguage = language;

    USER_LOG_INFO("Hms Sample Start");
    ZiyanTest_WidgetLogAppend("Hms Sample Start");

    USER_LOG_INFO("--> Step 1: Init hms sample");
    ZiyanTest_WidgetLogAppend("--> Step 1: Init hms sample");
    returnCode = ZiyanTest_HmsManagerInit();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Hms sample init error, error code:0x%08llX", returnCode);
        goto out;
    }

    osalHandler = ZiyanPlatform_GetOsalHandler();
    USER_LOG_INFO("--> Step 2: Register callback function of push HMS information");
    ZiyanTest_WidgetLogAppend("--> Step 2: Register callback function of push HMS information");
    returnCode = ZiyanHmsManager_RegHmsInfoCallback(ZiyanTest_HmsInfoCallback);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Register callback function of push HMS information failed, error code:0x%08llX", returnCode);
        goto out;
    }

    osalHandler->TaskSleepMs(10000);

out:
    USER_LOG_INFO("--> Step 3: Deinit hms sample");
    ZiyanTest_WidgetLogAppend("--> Step 3: Deinit hms sample");
    returnCode = ZiyanTest_HmsManagerDeInit();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Hms sample deinit error, error code:0x%08llX", returnCode);
    }

    USER_LOG_INFO("Hms Sample End");
    ZiyanTest_WidgetLogAppend("Hms Sample End");

    return returnCode;
}

T_ZiyanReturnCode ZiyanTest_HmsCustomizationStartService(void)
{
    T_ZiyanReturnCode returnCode;
#ifdef SYSTEM_ARCH_LINUX
    char curFileDirPath[HMS_DIR_PATH_LEN_MAX - 32];
    char tempPath[HMS_DIR_PATH_LEN_MAX];
#endif
    returnCode = ZiyanHmsCustomization_Init();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Hms init error, error code:0x%08llX", returnCode);
        return returnCode;
    }

#ifdef SYSTEM_ARCH_LINUX
    //Step 2 : Set hms text Config (Linux environment)
    returnCode = ZiyanUserUtil_GetCurrentFileDirPath(__FILE__, HMS_DIR_PATH_LEN_MAX, curFileDirPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    if (s_isHmsConfigFileDirPathConfigured == true) {
        snprintf(tempPath, HMS_DIR_PATH_LEN_MAX, "%shms_text/en", s_hmsConfigFileDirPath);
    } else {
        snprintf(tempPath, HMS_DIR_PATH_LEN_MAX, "%shms_text/en", curFileDirPath);
    }
    USER_LOG_DEBUG("HmsTextConfig path = %s\n", tempPath);
    //set default hms text config path
    returnCode = ZiyanHmsCustomization_RegDefaultHmsTextConfigByDirPath(tempPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default hms text config error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    //set hms text config for English language
    returnCode = ZiyanHmsCustomization_RegHmsTextConfigByDirPath(ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH,
                                                               tempPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add hms text config error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    //set hms text config for Chinese language
    if (s_isHmsConfigFileDirPathConfigured == true) {
        snprintf(tempPath, HMS_DIR_PATH_LEN_MAX, "%shms_text/cn", s_hmsConfigFileDirPath);
    } else {
        snprintf(tempPath, HMS_DIR_PATH_LEN_MAX, "%shms_text/cn", curFileDirPath);
    }

    returnCode = ZiyanHmsCustomization_RegHmsTextConfigByDirPath(ZIYAN_MOBILE_APP_LANGUAGE_CHINESE,
                                                               tempPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add hms text config error, stat = 0x%08llX", returnCode);
        return returnCode;
    }
#else
    //Step 2 : Set hms text Config (RTOS environment)
    T_ZiyanHmsBinaryArrayConfig enHmsTextBinaryArrayConfig = {
        .binaryArrayCount = sizeof(s_EnHmsTextConfigFileBinaryArrayList) / sizeof(T_ZiyanHmsFileBinaryArray),
        .fileBinaryArrayList = s_EnHmsTextConfigFileBinaryArrayList
    };

    //set default hms text config
    returnCode = ZiyanHmsCustomization_RegDefaultHmsTextConfigByBinaryArray(&enHmsTextBinaryArrayConfig);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Add default hms text config error, stat = 0x%08llX", returnCode);
        return returnCode;
    }
#endif

#if ZIYAN_CUSTOM_HMS_CODE_INJECT_ON
    ZiyanHmsCustomization_InjectHmsErrorCode(0x1E020000, ZIYAN_HMS_ERROR_LEVEL_FATAL);
    ZiyanHmsCustomization_InjectHmsErrorCode(0x1E020001, ZIYAN_HMS_ERROR_LEVEL_CRITICAL);
    ZiyanHmsCustomization_InjectHmsErrorCode(0x1E020002, ZIYAN_HMS_ERROR_LEVEL_WARN);
    ZiyanHmsCustomization_InjectHmsErrorCode(0x1E020003, ZIYAN_HMS_ERROR_LEVEL_HINT);
#endif

    return returnCode;
}

T_ZiyanReturnCode ZiyanTest_HmsCustomizationSetConfigFilePath(const char *path)
{
    memset(s_hmsConfigFileDirPath, 0, sizeof(s_hmsConfigFileDirPath));
    memcpy(s_hmsConfigFileDirPath, path, USER_UTIL_MIN(strlen(path), sizeof(s_hmsConfigFileDirPath) - 1));
    s_isHmsConfigFileDirPathConfigured = true;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
/* Private functions definition-----------------------------------------------*/
static T_ZiyanReturnCode ZiyanTest_HmsManagerInit(void)
{
    T_ZiyanReturnCode returnCode;
    char curFileDirPath[HMS_DIR_PATH_LEN_MAX - 32];
    char tempFileDirPath[HMS_DIR_PATH_LEN_MAX];
    uint32_t fileSize = 0;
    uint32_t readRealSize = 0;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = ZiyanFcSubscription_Init();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Hms sample init data subscription module failed, error code:0x%08llX", returnCode);
        return returnCode;
    }

    /*! subscribe fc data */
    returnCode = ZiyanFcSubscription_SubscribeTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT,
                                                  ZIYAN_DATA_SUBSCRIPTION_TOPIC_10_HZ,
                                                  NULL);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("HMS sample subscribe topic flight status error, error code:0x%08llX", returnCode);
        return returnCode;
    }

#ifdef SYSTEM_ARCH_LINUX

    returnCode = ZiyanUserUtil_GetCurrentFileDirPath(__FILE__, HMS_DIR_PATH_LEN_MAX, curFileDirPath);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file current path error, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    snprintf(tempFileDirPath, HMS_DIR_PATH_LEN_MAX, "%s/data/hms.json", curFileDirPath);

    returnCode = UtilFile_GetFileSizeByPath(tempFileDirPath, &fileSize);
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get file size by path failed, stat = 0x%08llX", returnCode);
        return returnCode;
    }

    USER_LOG_DEBUG("Hms json file size is %d", fileSize);

    s_hmsJsonData = osalHandler->Malloc(fileSize);
    if (s_hmsJsonData == NULL) {
        USER_LOG_ERROR("Malloc failed.");
    }

    UtilFile_GetFileDataByPath(tempFileDirPath, 0, fileSize, s_hmsJsonData, &readRealSize);
#endif

    isHmsManagerInit = true;

    return ZiyanHmsManager_Init();
}

static T_ZiyanReturnCode ZiyanTest_HmsManagerDeInit(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    returnCode = ZiyanFcSubscription_DeInit();
    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Deinit data subscription module failed, error code:0x%08llX",
                       returnCode);
        return returnCode;
    }

#ifdef SYSTEM_ARCH_LINUX
    osalHandler->Free(s_hmsJsonData);
#endif

    isHmsManagerInit = false;

    return ZiyanHmsManager_DeInit();
}

static T_ZiyanFcSubscriptionFlightStatus ZiyanTest_GetValueOfFlightStatus(void)
{
    T_ZiyanReturnCode returnCode;
    T_ZiyanFcSubscriptionFlightStatus flightStatus;
    T_ZiyanDataTimestamp flightStatusTimestamp = {0};

    if (isHmsManagerInit == false) {
        return ZIYAN_FC_SUBSCRIPTION_FLIGHT_STATUS_ON_GROUND;
    }

    returnCode = ZiyanFcSubscription_GetLatestValueOfTopic(ZIYAN_FC_SUBSCRIPTION_TOPIC_STATUS_FLIGHT,
                                                         (uint8_t *) &flightStatus,
                                                         sizeof(T_ZiyanFcSubscriptionFlightStatus),
                                                         &flightStatusTimestamp);

    if (returnCode != ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        USER_LOG_ERROR("Get value of topic flight status failed, error code:0x%08llX", returnCode);
        flightStatus = 0;
    } else {
        USER_LOG_DEBUG("Timestamp: millisecond %u microsecond %u.", flightStatusTimestamp.millisecond,
                       flightStatusTimestamp.microsecond);
        USER_LOG_DEBUG("Flight status: %d.", flightStatus);
    }

    return flightStatus;
}

static bool ZiyanTest_ReplaceStr(char *buffer, uint32_t bufferMaxLen, const char *target, const char *dest)
{
    char printBuffHeader[MAX_BUFFER_LEN] = {0};
    uint32_t printBuffHeaderCpyCnt = 0;
    char printBuffTail[MAX_BUFFER_LEN] = {0};
    uint32_t printBuffTailCpyCnt = 0;
    char *targetOffset = NULL;

    targetOffset = strstr(buffer, target);
    if (!targetOffset) {
        return false;
    }

    printBuffHeaderCpyCnt = targetOffset - buffer;
    if (printBuffHeaderCpyCnt > sizeof(printBuffHeader)) {
        printBuffHeaderCpyCnt = sizeof(printBuffHeader);
    }
    memcpy(printBuffHeader, buffer, printBuffHeaderCpyCnt);

    printBuffTailCpyCnt = strlen(targetOffset + strlen(target));
    if (printBuffTailCpyCnt > sizeof(printBuffTail)) {
        printBuffTailCpyCnt = sizeof(printBuffTail);
    }
    memcpy(printBuffTail, targetOffset + strlen(target), printBuffTailCpyCnt);

    snprintf(buffer, bufferMaxLen, "%s%s%s", printBuffHeader, dest, printBuffTail);
    return true;
}

static bool ZiyanTest_MarchErrCodeInfoTable(T_ZiyanHmsInfoTable hmsInfoTable)
{
    char alarmIdStr[20] = {0};
    char sensorIdStr[20] = {0};
    char componentIdStr[20] = {0};
    char printBuff[256] = {0};
    const char *originalAlarmInfo = NULL;
    uint8_t hmsCodeMatchFlag = 0;

    if (!hmsInfoTable.hmsInfo) {
        USER_LOG_ERROR("Hms info table is null");
        return false;
    }

    for (int i = 0; i < hmsInfoTable.hmsInfoNum; i++) {
        hmsCodeMatchFlag = 0;
        for (int j = 0; j < sizeof(hmsErrCodeInfoTbl) / sizeof(T_ZiyanHmsErrCodeInfo); j++) {
            if (hmsInfoTable.hmsInfo[i].errorCode == hmsErrCodeInfoTbl[j].alarmId) {
                hmsCodeMatchFlag = 1;
                snprintf(alarmIdStr, sizeof(alarmIdStr), "%u", hmsInfoTable.hmsInfo[i].errorCode);
                //note:sensor_idx:[0,5].In order to be consistent with the display of pilot, add one.
                snprintf(sensorIdStr, sizeof(sensorIdStr), "%d", hmsInfoTable.hmsInfo[i].componentIndex + 1);
                snprintf(componentIdStr, sizeof(componentIdStr), "0x%02X", hmsInfoTable.hmsInfo[i].componentIndex + 1);
                if (ZiyanTest_GetValueOfFlightStatus() == ZIYAN_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR) {
                    originalAlarmInfo = hmsErrCodeInfoTbl[j].flyAlarmInfo;
                } else {
                    originalAlarmInfo = hmsErrCodeInfoTbl[j].groundAlarmInfo;
                }
                originalAlarmInfo = hmsErrCodeInfoTbl[j].groundAlarmInfo;
                if (strlen(originalAlarmInfo)) {
                    snprintf(printBuff, sizeof(printBuff), "%s", originalAlarmInfo);
                    ZiyanTest_ReplaceStr(printBuff, sizeof(printBuff), oldReplaceAlarmIdStr, alarmIdStr);
                    ZiyanTest_ReplaceStr(printBuff, sizeof(printBuff), oldReplaceIndexStr, sensorIdStr);
                    ZiyanTest_ReplaceStr(printBuff, sizeof(printBuff), oldReplaceComponentIndexStr, componentIdStr);
                    if (hmsInfoTable.hmsInfo[i].errorLevel > MIN_HMS_ERROR_LEVEL &&
                        hmsInfoTable.hmsInfo[i].errorLevel < MID_HMS_ERROR_LEVEL) {
                        USER_LOG_WARN("[ErrorCode:0x%2x] %s", hmsInfoTable.hmsInfo[i].errorCode, printBuff);
                    } else if (hmsInfoTable.hmsInfo[i].errorLevel >= MID_HMS_ERROR_LEVEL &&
                               hmsInfoTable.hmsInfo[i].errorLevel < MAX_HMS_ERROR_LEVEL) {
                        USER_LOG_ERROR("[ErrorCode:0x%2x] %s", hmsInfoTable.hmsInfo[i].errorCode, printBuff);
                    }
                }
            }
        }
        if (!hmsCodeMatchFlag) {
            USER_LOG_WARN("[ErrorCode:0x%2x] There are no matching documents in the current hmsErrCodeInfoTbl for now.",
                          hmsInfoTable.hmsInfo[i].errorCode);
        }
    }

    return true;
}

static bool ZiyanTest_MarchErrCodeInfoTableByJson(T_ZiyanHmsInfoTable hmsInfoTable)
{
    cJSON *hmsJsonRoot = NULL;
    cJSON *hmsErrorCode = NULL;
    cJSON *hmsLanguage = NULL;
    char hmsErrorCodeString[HMS_DIR_PATH_LEN_MAX] = {0};

    hmsJsonRoot = cJSON_Parse((char *) s_hmsJsonData);
    if (hmsJsonRoot == NULL) {
        return 0;
    }

    for (int i = 0; i < hmsInfoTable.hmsInfoNum; i++) {
        if (ZiyanTest_GetValueOfFlightStatus() == ZIYAN_FC_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR) {
            sprintf(hmsErrorCodeString, "fpv_tip_0x%08X_in_the_sky", hmsInfoTable.hmsInfo[i].errorCode);
        } else {
            sprintf(hmsErrorCodeString, "fpv_tip_0x%08X", hmsInfoTable.hmsInfo[i].errorCode);
        }

        hmsErrorCode = cJSON_GetObjectItem(hmsJsonRoot, hmsErrorCodeString);
        if (hmsErrorCode != NULL) {
            if (s_hmsLanguage == ZIYAN_MOBILE_APP_LANGUAGE_CHINESE) {
                hmsLanguage = cJSON_GetObjectItem(hmsErrorCode, "zh");
            } else if (s_hmsLanguage == ZIYAN_MOBILE_APP_LANGUAGE_ENGLISH) {
                hmsLanguage = cJSON_GetObjectItem(hmsErrorCode, "en");
            } else if (s_hmsLanguage == ZIYAN_MOBILE_APP_LANGUAGE_JAPANESE) {
                hmsLanguage = cJSON_GetObjectItem(hmsErrorCode, "ja");
            } else if (s_hmsLanguage == ZIYAN_MOBILE_APP_LANGUAGE_FRENCH) {
                hmsLanguage = cJSON_GetObjectItem(hmsErrorCode, "fr");
            }
            if (hmsLanguage != NULL) {
                if (hmsInfoTable.hmsInfo[i].errorLevel > MIN_HMS_ERROR_LEVEL &&
                    hmsInfoTable.hmsInfo[i].errorLevel < MID_HMS_ERROR_LEVEL) {
                    USER_LOG_WARN("[ErrorCode: 0x%2x]: %s", hmsInfoTable.hmsInfo[i].errorCode,
                                  hmsLanguage->valuestring);
                } else if (hmsInfoTable.hmsInfo[i].errorLevel >= MID_HMS_ERROR_LEVEL &&
                           hmsInfoTable.hmsInfo[i].errorLevel < MAX_HMS_ERROR_LEVEL) {
                    USER_LOG_ERROR("[ErrorCode: 0x%2x]: %s", hmsInfoTable.hmsInfo[i].errorCode,
                                   hmsLanguage->valuestring);
                }
            } else {
                USER_LOG_WARN("[ErrorCode: 0x%2x] There are no matching documents for this language for now.",
                              hmsInfoTable.hmsInfo[i].errorCode);
            }
        } else {
            USER_LOG_WARN("[ErrorCode: 0x%2x] There are no matching documents in the current json for now.",
                          hmsInfoTable.hmsInfo[i].errorCode);
        }
    }

    cJSON_Delete(hmsJsonRoot);
}

static T_ZiyanReturnCode ZiyanTest_HmsInfoCallback(T_ZiyanHmsInfoTable hmsInfoTable)
{
#ifdef SYSTEM_ARCH_LINUX
    ZiyanTest_MarchErrCodeInfoTableByJson(hmsInfoTable);
#else
    if (!ZiyanTest_MarchErrCodeInfoTable(hmsInfoTable)) {
        USER_LOG_ERROR("March HMS Information failed.");
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
#endif

    if (hmsInfoTable.hmsInfoNum == 0) {
        USER_LOG_INFO("All systems of drone are running well now.");
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

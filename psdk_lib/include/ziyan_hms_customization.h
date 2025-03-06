/**
 ********************************************************************
 * @file    ziyan_hms_customization.h
 * @brief   This is the header file for "ziyan_hms_customization.c", defining the structure and
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
#ifndef ZIYAN_HMS_CUSTOMIZATION_H
#define ZIYAN_HMS_CUSTOMIZATION_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
    ZIYAN_HMS_ERROR_LEVEL_NONE = 0,
    ZIYAN_HMS_ERROR_LEVEL_HINT,
    ZIYAN_HMS_ERROR_LEVEL_WARN,
    ZIYAN_HMS_ERROR_LEVEL_CRITICAL,
    ZIYAN_HMS_ERROR_LEVEL_FATAL,
} E_ZiyanHmsErrorLevel;

typedef struct {
    char *fileName; /*!< The file name of the hms text config file */
    uint32_t fileSize; /*!< The file size of the hms text config file, uint : byte */
    const uint8_t *fileBinaryArray; /*!< The binary C array of the hms text config file */
} T_ZiyanHmsFileBinaryArray;

typedef struct {
    uint16_t binaryArrayCount; /*!< Binary array count. */
    T_ZiyanHmsFileBinaryArray *fileBinaryArrayList; /*!< Pointer to binary array list */
} T_ZiyanHmsBinaryArrayConfig;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise hms customization module, and user should call this function
 * before using hms customization features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_Init(void);

/**
 * @brief DeInitialize hms manager module.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_DeInit(void);

/**
 * @brief Inject custom hms error code to APP.
 * @note: For the same hms error code, a single call is enough, no need to call repeatedly.
 * @param errorCode: hms error code, value range: [0x1E020000 ~ 0x1E02FFFF].
 * @param errorLevel: hms error level, see reference of E_ZiyanHmsErrorLevel.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_InjectHmsErrorCode(uint32_t errorCode, E_ZiyanHmsErrorLevel errorLevel);

/**
 * @brief Eliminate custom hms error code to APP.
 * @note: For the same hms error code, a single call is enough, no need to call repeatedly.
 * @param errorCode: hms error code, value range: [0x1E020000 ~ 0x1E02FFFF].
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_EliminateHmsErrorCode(uint32_t errorCode);

/**
 * @brief Register default hms text configuration file by directory path.
 * @param configDirPath: the hms text configuration by directory path.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_RegDefaultHmsTextConfigByDirPath(const char *configDirPath);

/**
 * @brief Register hms text configuration file by directory path.
 * @note Different hms text configurations for several language require the same hms config.
 * @param appLanguage: mobile app language type.
 * @param configDirPath: the hms text configuration by directory path.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_RegHmsTextConfigByDirPath(E_ZiyanMobileAppLanguage appLanguage,
                                                              const char *configDirPath);

/**
 * @brief Register default hms text configuration config by binary array.
 * @note In RTOS, most likely there is no file system. The hms text config file content can use C array express. Use this
 * function and ZiyanHms_RegDefaultHmsTextConfigByBinaryArray set hms text configuration. When the language is not cover in
 * your setting by ZiyanHms_RegHmsTextConfigByBinaryArray, the hms text configuration uses setting by this function.
 * @param binaryArrayConfig: the binary array config for hms text configuration.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_RegDefaultHmsTextConfigByBinaryArray(const T_ZiyanHmsBinaryArrayConfig
                                                                         *binaryArrayConfig);

/**
 * @brief Register hms text config by binary array configuration.
 * @note Different hms text configurations for several language require the same hms config.
 * @param appLanguage: mobile app language type.
 * @param binaryArrayConfig: the binary array config for hms text configuration.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanHmsCustomization_RegHmsTextConfigByBinaryArray(E_ZiyanMobileAppLanguage appLanguage,
                                                                  const T_ZiyanHmsBinaryArrayConfig *binaryArrayConfig);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_HMS_CUSTOMIZATION_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

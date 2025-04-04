/**
 ********************************************************************
 * @file    ziyan_logger.h
 * @brief   This is the header file for "ziyan_logger.c", defining the structure and
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
#ifndef ZIYAN_LOGGER_H
#define ZIYAN_LOGGER_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/**
* @brief The console method that needs to be registered.
* @note  Before registering the console method, you need to test the methods that need to be registered to ensure
*        that they can be used normally.
*/
typedef T_ZiyanReturnCode (*ConsoleFunc)(const uint8_t *data, uint16_t dataLen);

/**
 * @brief Logger console level.
 */
typedef enum {
    ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_ERROR = 0, /*!< Logger console error level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
    ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_WARN = 1, /*!< Logger console warning level.The method and level of the console are
                                                    associated with each other. If the level of the registered console
                                                    method is lower than this level, the level interface will not be
                                                    printed successfully. */
    ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_INFO = 2, /*!< Logger console info level. The method and level of the console are
                                                 associated with each other. If the level of the registered console
                                                 method is lower than this level, the level interface will not be
                                                 printed successfully. */
    ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_DEBUG = 3, /*!< Logger console debug level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
} E_ZiyanLoggerConsoleLogLevel;

/**
 * @brief Logger console content.
 */
typedef struct {
    ConsoleFunc func;
    uint8_t consoleLevel;
    bool isSupportColor;
} T_ZiyanLoggerConsole;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Add the console function and level for Payload SDK.
 * @note When registering the console, you need to provide the method of the console and the level corresponding to
 * the method. Log levels from high to low are Debug, Info, Warn, and Error, the log function module can print all
 * logs not higher than the specified level. Maximum support for registering eight different console methods at the
 * same time.Before registering the console method, you should test the registration method ensure that all the method
 * are normally. If you registered multiple methods at the same time, all the methods will be print.
 * @param console: pointer to the console function.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanLogger_AddConsole(T_ZiyanLoggerConsole *console);

/**
 * @brief Remove the console function and level for Payload SDK.
 * @param console: pointer to the console function.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanLogger_RemoveConsole(T_ZiyanLoggerConsole *console);
/**
 * @brief Print out the selected level log of the specified format by the registration method.
 * @note The registered method is printed according to the corresponding level. If the level of the console is lower
 * than the level at which the log needs to be printed, it will not be printed successfully.
 * @param fmt: pointer to the format string that needs print out.
 * @param ...: Variable parameters, consistent with the use of the system interface print out.
 */
void ZiyanLogger_UserLogOutput(E_ZiyanLoggerConsoleLogLevel level, const char *fmt, ...);

/* Exported constants --------------------------------------------------------*/
#define USER_LOG_DEBUG(fmt, ...)    \
        ZiyanLogger_UserLogOutput(ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_DEBUG, "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_INFO(fmt, ...)     \
        ZiyanLogger_UserLogOutput(ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_INFO, "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_WARN(fmt, ...)     \
        ZiyanLogger_UserLogOutput(ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_WARN, "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)
#define USER_LOG_ERROR(fmt, ...)    \
        ZiyanLogger_UserLogOutput(ZIYAN_LOGGER_CONSOLE_LOG_LEVEL_ERROR, "[%s:%d) " fmt, __FUNCTION__, __LINE__ , ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_LOGGER_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

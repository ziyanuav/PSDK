/**
 ********************************************************************
 * @file    ziyan_platform.h
 * @brief   This is the header file for "ziyan_platform.c", defining the structure and
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
#ifndef ZIYAN_PLATFORM_H
#define ZIYAN_PLATFORM_H

/* Includes ------------------------------------------------------------------*/
#include "ziyan_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
* @brief Platform handle of uart operation.
*/
typedef void *T_ZiyanUartHandle;

/**
* @brief Platform handle of usb bulk operation.
*/
typedef void *T_ZiyanUsbBulkHandle;

/**
* @brief Platform handle of network operation.
*/
typedef void *T_ZiyanNetworkHandle;

/**
* @brief Platform handle of thread task operation.
*/
typedef void *T_ZiyanTaskHandle;

/**
* @brief Platform handle of mutex operation.
*/
typedef void *T_ZiyanMutexHandle;

/**
* @brief Platform handle of semaphore operation.
*/
typedef void *T_ZiyanSemaHandle;

/**
* @brief Platform handle of file operation.
*/
typedef void *T_ZiyanFileHandle;

/**
* @brief Platform handle of dir operation.
*/
typedef void *T_ZiyanDirHandle;

/**
* @brief Platform handle of socket operation.
*/
typedef void *T_ZiyanSocketHandle;

typedef enum {
    /**
    * All aircraft type support，users can connect via chip serial port or USB to TTL serial port.
    * Baud rate support list on M300 RTK Payload Port: 115200, 230400, 460800, 921600.
    * Baud rate support list on M300 RTK Extension Port: 115200, 230400, 460800, 921600, 1000000.
    * Baud rate support list on M30/M30T: 115200, 230400, 460800, 921600, 1000000.
    * Baud rate support list on M3E/M3T: 921600.
    * Baud rate support list on M350 RTK Payload Port: 115200, 230400, 460800, 921600.
    * Baud rate support list on M350 RTK Extension Port: 115200, 230400, 460800, 921600, 1000000.
    * */
    ZIYAN_HAL_UART_NUM_0,
    /**
    * Only support on M300/M350 RTK Extension Port by USB virtual serial port, such as /dev/ttyACM0.
    * Baud rate support list on M300 RTK Extension Port: 921600.
    * Baud rate support list on M350 RTK Extension Port: 921600.
    * */
    ZIYAN_HAL_UART_NUM_1,
} E_ZiyanHalUartNum;

typedef enum {
    ZIYAN_HAL_USB_BULK_NUM_0 = 0,
    ZIYAN_HAL_USB_BULK_NUM_1,
    ZIYAN_HAL_USB_BULK_NUM_MAX,
} E_ZiyanHalUsbBulkNum;

typedef enum {
    ZIYAN_SOCKET_MODE_UDP,
    ZIYAN_SOCKET_MODE_TCP,
} E_ZiyanSocketMode;

typedef struct {
    bool isConnect;
} T_ZiyanUartStatus;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} T_ZiyanTime;

typedef struct {
    uint32_t size;
    T_ZiyanTime createTime;
    T_ZiyanTime modifyTime;
    char path[ZIYAN_FILE_PATH_SIZE_MAX];
    bool isDir;
} T_ZiyanFileInfo;

typedef struct {
    T_ZiyanReturnCode (*UartInit)(E_ZiyanHalUartNum uartNum, uint32_t baudRate, T_ZiyanUartHandle *uartHandle);

    T_ZiyanReturnCode (*UartDeInit)(T_ZiyanUartHandle uartHandle);

    T_ZiyanReturnCode (*UartWriteData)(T_ZiyanUartHandle uartHandle, const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*UartReadData)(T_ZiyanUartHandle uartHandle, uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*UartGetStatus)(E_ZiyanHalUartNum uartNum, T_ZiyanUartStatus *status);
} T_ZiyanHalUartHandler;

typedef struct {
    uint16_t interfaceNum;
    uint16_t endPointIn;
    uint16_t endPointOut;
} T_ZiyanHalUsbBulkChannelInfo;

typedef struct {
    bool isUsbHost;
    // attention: if 'isUsbHost' equals false, the following parameters is not valid.
    uint16_t pid;
    uint16_t vid;
    T_ZiyanHalUsbBulkChannelInfo channelInfo;
} T_ZiyanHalUsbBulkInfo;

typedef struct {
    uint16_t pid;
    uint16_t vid;
    T_ZiyanHalUsbBulkChannelInfo channelInfo[ZIYAN_HAL_USB_BULK_NUM_MAX];
} T_ZiyanHalUsbBulkDeviceInfo;

typedef struct {
    struct {
        uint16_t vid;
        uint16_t pid;
    } usbNetAdapter;
} T_ZiyanHalNetworkDeviceInfo;

typedef struct {
    T_ZiyanReturnCode (*UsbBulkInit)(T_ZiyanHalUsbBulkInfo usbBulkInfo, T_ZiyanUsbBulkHandle *usbBulkHandle);

    T_ZiyanReturnCode (*UsbBulkDeInit)(T_ZiyanUsbBulkHandle usbBulkHandle);

    T_ZiyanReturnCode (*UsbBulkWriteData)(T_ZiyanUsbBulkHandle usbBulkHandle, const uint8_t *buf,
                                        uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*UsbBulkReadData)(T_ZiyanUsbBulkHandle usbBulkHandle, uint8_t *buf,
                                       uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*UsbBulkGetDeviceInfo)(T_ZiyanHalUsbBulkDeviceInfo *deviceInfo);
} T_ZiyanHalUsbBulkHandler;

typedef struct {
    T_ZiyanReturnCode (*NetworkInit)(const char *ipAddr, const char *netMask, T_ZiyanNetworkHandle *networkHandle);

    T_ZiyanReturnCode (*NetworkDeInit)(T_ZiyanNetworkHandle networkHandle);

    T_ZiyanReturnCode (*NetworkGetDeviceInfo)(T_ZiyanHalNetworkDeviceInfo *deviceInfo);
} T_ZiyanHalNetworkHandler;

typedef struct {
    T_ZiyanReturnCode (*TaskCreate)(const char *name, void *(*taskFunc)(void *),
                                  uint32_t stackSize, void *arg, T_ZiyanTaskHandle *task);

    T_ZiyanReturnCode (*TaskDestroy)(T_ZiyanTaskHandle task);

    T_ZiyanReturnCode (*TaskSleepMs)(uint32_t timeMs);

    T_ZiyanReturnCode (*MutexCreate)(T_ZiyanMutexHandle *mutex);

    T_ZiyanReturnCode (*MutexDestroy)(T_ZiyanMutexHandle mutex);

    T_ZiyanReturnCode (*MutexLock)(T_ZiyanMutexHandle mutex);

    T_ZiyanReturnCode (*MutexUnlock)(T_ZiyanMutexHandle mutex);

    T_ZiyanReturnCode (*SemaphoreCreate)(uint32_t initValue, T_ZiyanSemaHandle *semaphore);

    T_ZiyanReturnCode (*SemaphoreDestroy)(T_ZiyanSemaHandle semaphore);

    T_ZiyanReturnCode (*SemaphoreWait)(T_ZiyanSemaHandle semaphore);

    T_ZiyanReturnCode (*SemaphoreTimedWait)(T_ZiyanSemaHandle semaphore, uint32_t waitTimeMs);

    T_ZiyanReturnCode (*SemaphorePost)(T_ZiyanSemaHandle semaphore);

    T_ZiyanReturnCode (*GetTimeMs)(uint32_t *ms);

    T_ZiyanReturnCode (*GetTimeUs)(uint64_t *us);

    T_ZiyanReturnCode (*GetRandomNum)(uint16_t *randomNum);

    void *(*Malloc)(uint32_t size);

    void (*Free)(void *ptr);
} T_ZiyanOsalHandler;

typedef struct {
    T_ZiyanReturnCode (*FileOpen)(const char *fileName, const char *fileMode, T_ZiyanFileHandle *fileObj);

    T_ZiyanReturnCode (*FileClose)(T_ZiyanFileHandle fileObj);

    T_ZiyanReturnCode (*FileWrite)(T_ZiyanFileHandle fileObj, const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*FileRead)(T_ZiyanFileHandle fileObj, uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*FileSeek)(T_ZiyanFileHandle fileObj, uint32_t offset);

    T_ZiyanReturnCode (*FileSync)(T_ZiyanFileHandle fileObj);

    T_ZiyanReturnCode (*DirOpen)(const char *filePath, T_ZiyanDirHandle *dirObj);

    T_ZiyanReturnCode (*DirClose)(T_ZiyanDirHandle dirObj);

    T_ZiyanReturnCode (*DirRead)(T_ZiyanDirHandle dirObj, T_ZiyanFileInfo *fileInfo);

    T_ZiyanReturnCode (*Mkdir)(const char *filePath);

    T_ZiyanReturnCode (*Unlink)(const char *filePath);

    T_ZiyanReturnCode (*Rename)(const char *oldFilePath, const char *newFilePath);

    T_ZiyanReturnCode (*Stat)(const char *filePath, T_ZiyanFileInfo *fileInfo);
} T_ZiyanFileSystemHandler;

typedef struct {
    T_ZiyanReturnCode (*Socket)(E_ZiyanSocketMode mode, T_ZiyanSocketHandle *socketHandle);

    T_ZiyanReturnCode (*Close)(T_ZiyanSocketHandle socketHandle);

    T_ZiyanReturnCode (*Bind)(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port);

    T_ZiyanReturnCode (*UdpSendData)(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port,
                                   const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*UdpRecvData)(T_ZiyanSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                   uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*TcpListen)(T_ZiyanSocketHandle socketHandle);

    T_ZiyanReturnCode (*TcpAccept)(T_ZiyanSocketHandle socketHandle, char *ipAddr, uint32_t *port,
                                 T_ZiyanSocketHandle *outSocketHandle);

    T_ZiyanReturnCode (*TcpConnect)(T_ZiyanSocketHandle socketHandle, const char *ipAddr, uint32_t port);

    T_ZiyanReturnCode (*TcpSendData)(T_ZiyanSocketHandle socketHandle,
                                   const uint8_t *buf, uint32_t len, uint32_t *realLen);

    T_ZiyanReturnCode (*TcpRecvData)(T_ZiyanSocketHandle socketHandle,
                                   uint8_t *buf, uint32_t len, uint32_t *realLen);
} T_ZiyanSocketHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Register the handler for hal uart interfaces by your platform.
 * @note It should be noted that the interface in hal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param halUartHandler: pointer to the handler for hal uart interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegHalUartHandler(const T_ZiyanHalUartHandler *halUartHandler);

/**
 * @brief Register the handler for usb bulk interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for usb bulk interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegHalUsbBulkHandler(const T_ZiyanHalUsbBulkHandler *halUsbBulkHandler);

/**
 * @brief Register the handler for hal network interfaces by your platform.
 * @note It should be noted that the interface in hal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration.
 * @attention The interface needs to be called at the beginning of the application for registration, otherwise, the
 * subsequent functions will not work properly.
 * @param osalHandler: pointer to the handler for network handler interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegHalNetworkHandler(const T_ZiyanHalNetworkHandler *halNetworkHandler);

/**
 * @brief Register the handler for osal interfaces by your platform.
 * @note It should be noted that the interface in osal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param osalHandler: pointer to the handler for osal interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegOsalHandler(const T_ZiyanOsalHandler *osalHandler);

/**
 * @brief Register the handler for file-system interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for file-system interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegFileSystemHandler(const T_ZiyanFileSystemHandler *fileSystemHandler);

/**
 * @brief Register the handler for socket interfaces by your platform.
 * @param fileSystemHandler: pointer to the handler for socket interfaces by your platform.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanPlatform_RegSocketHandler(const T_ZiyanSocketHandler *socketHandler);

/**
 * @brief Get the handler of osal interfaces.
 * @return Pointer to osal handler.
 */
T_ZiyanOsalHandler *ZiyanPlatform_GetOsalHandler(void);

/**
 * @brief Get the handler of usb bulk interfaces.
 * @return Pointer to usb bulk handler.
 */
T_ZiyanHalUsbBulkHandler *ZiyanPlatform_GetHalUsbBulkHandler(void);

/**
 * @brief Get the handler of network interfaces.
 * @return Pointer to network handler.
 */
T_ZiyanHalNetworkHandler *ZiyanPlatform_GetHalNetworkHandler(void);

/**
 * @brief Get the handler of file-system interfaces.
 * @return Pointer to file-system handler.
 */
T_ZiyanFileSystemHandler *ZiyanPlatform_GetFileSystemHandler(void);

/**
 * @brief Get the handler of socket interfaces.
 * @return Pointer to socket handler.
 */
T_ZiyanSocketHandler *ZiyanPlatform_GetSocketHandler(void);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_PLATFORM_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

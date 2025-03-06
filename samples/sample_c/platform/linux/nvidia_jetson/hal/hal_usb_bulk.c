/**
 ********************************************************************
 * @file    hal_usb_bulk.c
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
#include "hal_usb_bulk.h"
#include "ziyan_logger.h"

/* Private constants ---------------------------------------------------------*/
#define LINUX_USB_BULK_TRANSFER_TIMEOUT_MS    (50)
#define LINUX_USB_BULK_TRANSFER_WAIT_FOREVER  (-1)

/* Private types -------------------------------------------------------------*/
typedef struct {
#ifdef LIBUSB_INSTALLED
    libusb_device_handle *handle;
#else
    void *handle;
#endif
    int32_t ep1;
    int32_t ep2;
    uint32_t interfaceNum;
    T_ZiyanHalUsbBulkInfo usbBulkInfo;
} T_HalUsbBulkObj;

/* Private values -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_ZiyanReturnCode HalUsbBulk_Init(T_ZiyanHalUsbBulkInfo usbBulkInfo, T_ZiyanUsbBulkHandle *usbBulkHandle)
{
    int32_t ret;
    struct libusb_device_handle *handle = NULL;

    *usbBulkHandle = malloc(sizeof(T_HalUsbBulkObj));
    if (*usbBulkHandle == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    if (usbBulkInfo.isUsbHost == true) {
#ifdef LIBUSB_INSTALLED
        ret = libusb_init(NULL);
        if (ret < 0) {
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        handle = libusb_open_device_with_vid_pid(NULL, usbBulkInfo.vid, usbBulkInfo.pid);
        if (handle == NULL) {
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ret = libusb_claim_interface(handle, usbBulkInfo.channelInfo.interfaceNum);
        if (ret != LIBUSB_SUCCESS) {
            printf("libusb claim interface error");
            libusb_close(handle);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        ((T_HalUsbBulkObj *) *usbBulkHandle)->handle = handle;
        memcpy(&((T_HalUsbBulkObj *) *usbBulkHandle)->usbBulkInfo, &usbBulkInfo, sizeof(usbBulkInfo));
#endif
    } else {
        ((T_HalUsbBulkObj *) *usbBulkHandle)->handle = handle;
        memcpy(&((T_HalUsbBulkObj *) *usbBulkHandle)->usbBulkInfo, &usbBulkInfo, sizeof(usbBulkInfo));
        ((T_HalUsbBulkObj *) *usbBulkHandle)->interfaceNum = usbBulkInfo.channelInfo.interfaceNum;

        if (usbBulkInfo.channelInfo.interfaceNum == LINUX_USB_BULK1_INTERFACE_NUM) {
            ((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 = open(LINUX_USB_BULK1_EP_OUT_FD, O_RDWR);
            if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 < 0) {
                return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
            }

            ((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 = open(LINUX_USB_BULK1_EP_IN_FD, O_RDWR);
            if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 < 0) {
                return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
            }
        } else if (usbBulkInfo.channelInfo.interfaceNum == LINUX_USB_BULK2_INTERFACE_NUM) {
            ((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 = open(LINUX_USB_BULK2_EP_OUT_FD, O_RDWR);
            if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep1 < 0) {
                return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
            }

            ((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 = open(LINUX_USB_BULK2_EP_IN_FD, O_RDWR);
            if (((T_HalUsbBulkObj *) *usbBulkHandle)->ep2 < 0) {
                return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
            }
        }
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode HalUsbBulk_DeInit(T_ZiyanUsbBulkHandle usbBulkHandle)
{
    struct libusb_device_handle *handle = NULL;
    T_ZiyanOsalHandler *osalHandler = ZiyanPlatform_GetOsalHandler();

    if (usbBulkHandle == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
#ifdef LIBUSB_INSTALLED
        libusb_release_interface(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.interfaceNum);
        osalHandler->TaskSleepMs(100);
        libusb_exit(NULL);
#endif
    } else {
        close(((T_HalUsbBulkObj *) usbBulkHandle)->ep1);
        close(((T_HalUsbBulkObj *) usbBulkHandle)->ep2);
    }

    free(usbBulkHandle);

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode HalUsbBulk_WriteData(T_ZiyanUsbBulkHandle usbBulkHandle, const uint8_t *buf, uint32_t len,
                                     uint32_t *realLen)
{
    int32_t ret;
    int32_t actualLen;
    struct libusb_device_handle *handle = NULL;

    if (usbBulkHandle == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
#ifdef LIBUSB_INSTALLED
        ret = libusb_bulk_transfer(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.endPointOut,
                                   (uint8_t *) buf, len, &actualLen, LINUX_USB_BULK_TRANSFER_TIMEOUT_MS);
        if (ret < 0) {
            USER_LOG_ERROR("Write usb bulk data failed, errno = %d", ret);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        *realLen = actualLen;
#endif
    } else {
        *realLen = write(((T_HalUsbBulkObj *) usbBulkHandle)->ep1, buf, len);
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode HalUsbBulk_ReadData(T_ZiyanUsbBulkHandle usbBulkHandle, uint8_t *buf, uint32_t len,
                                    uint32_t *realLen)
{
    int32_t ret;
    struct libusb_device_handle *handle = NULL;
    int32_t actualLen;

    if (usbBulkHandle == NULL) {
        return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    handle = ((T_HalUsbBulkObj *) usbBulkHandle)->handle;

    if (((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.isUsbHost == true) {
#ifdef LIBUSB_INSTALLED
        ret = libusb_bulk_transfer(handle, ((T_HalUsbBulkObj *) usbBulkHandle)->usbBulkInfo.channelInfo.endPointIn,
                                   buf, len, &actualLen, LINUX_USB_BULK_TRANSFER_WAIT_FOREVER);
        if (ret < 0) {
            USER_LOG_ERROR("Read usb bulk data failed, errno = %d", ret);
            return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
        }

        *realLen = actualLen;
#endif
    } else {
        *realLen = read(((T_HalUsbBulkObj *) usbBulkHandle)->ep2, buf, len);
    }

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_ZiyanReturnCode HalUsbBulk_GetDeviceInfo(T_ZiyanHalUsbBulkDeviceInfo *deviceInfo)
{
    //attention: this interface only be called in usb device mode.
    deviceInfo->vid = LINUX_USB_VID;
    deviceInfo->pid = LINUX_USB_PID;

    // This bulk channel is used to obtain ZIYAN camera video stream and push 3rd-party camera video stream.
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_0].interfaceNum = LINUX_USB_BULK1_INTERFACE_NUM;
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_0].endPointIn = LINUX_USB_BULK1_END_POINT_IN;
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_0].endPointOut = LINUX_USB_BULK1_END_POINT_OUT;

    // This bulk channel is used to obtain ZIYAN perception image and download camera media file.
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_1].interfaceNum = LINUX_USB_BULK2_INTERFACE_NUM;
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_1].endPointIn = LINUX_USB_BULK2_END_POINT_IN;
    deviceInfo->channelInfo[ZIYAN_HAL_USB_BULK_NUM_1].endPointOut = LINUX_USB_BULK2_END_POINT_OUT;

    return ZIYAN_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT ZIYAN Innovations *****END OF FILE****/

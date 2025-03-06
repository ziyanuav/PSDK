/**
 ********************************************************************
 * @file    ziyan_widget.h
 * @brief   This is the header file for "ziyan_widget.c", defining the structure and
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
#ifndef ZIYAN_WIDGET_H
#define ZIYAN_WIDGET_H

/* Includes ------------------------------------------------------------------*/
#include <ziyan_typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/*! The maximum length of a message that can be displayed by the mobile app floating window */
#define ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN         255

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Widget types.
 */
typedef enum {
    ZIYAN_WIDGET_TYPE_UNKNOW = 0,
    ZIYAN_WIDGET_TYPE_BUTTON = 1, /*!< button widget type */
    ZIYAN_WIDGET_TYPE_SWITCH = 2, /*!< switch widget type */
    ZIYAN_WIDGET_TYPE_SCALE = 3, /*!< scale widget type */
    ZIYAN_WIDGET_TYPE_LIST = 4, /*!< list widget type */
    ZIYAN_WIDGET_TYPE_INT_INPUT_BOX = 5, /*!< integer input box widget type */
} E_ZiyanWidgetType;

/**
 * @brief Button widget state.
 */
typedef enum {
    ZIYAN_WIDGET_BUTTON_STATE_PRESS_DOWN = 1, /*!< Button is pressed down */
    ZIYAN_WIDGET_BUTTON_STATE_RELEASE_UP = 0, /*!< Button is released up */
} E_ZiyanWidgetButtonState;

/**
 * @brief Switch widget state.
 */
typedef enum {
    ZIYAN_WIDGET_SWITCH_STATE_OFF = 0, /*!< Switch is turned off */
    ZIYAN_WIDGET_SWITCH_STATE_ON = 1 /*!< Switch is turned on */
} E_ZiyanWidgetSwitchState;

/**
 * @brief Switch widget speaker work mode.
 */
typedef enum {
    ZIYAN_WIDGET_SPEAKER_WORK_MODE_TTS = 0,
    ZIYAN_WIDGET_SPEAKER_WORK_MODE_VOICE = 1,
} E_ZiyanWidgetSpeakerWorkMode;

/**
 * @brief Switch widget speaker play mode.
 */
typedef enum {
    ZIYAN_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY = 0,
    ZIYAN_WIDGET_SPEAKER_PLAY_MODE_LOOP_PLAYBACK = 1,
} E_ZiyanWidgetSpeakerPlayMode;

/**
 * @brief Switch widget speaker state.
 */
typedef enum {
    ZIYAN_WIDGET_SPEAKER_STATE_IDEL = 0,
    ZIYAN_WIDGET_SPEAKER_STATE_TRANSMITTING = 1,
    ZIYAN_WIDGET_SPEAKER_STATE_PLAYING = 2,
    ZIYAN_WIDGET_SPEAKER_STATE_ERROR = 3,
    ZIYAN_WIDGET_SPEAKER_STATE_IN_TTS_CONVERSION = 4,
} E_ZiyanWidgetSpeakerState;

/**
 * @brief Switch widget transmit data event.
 */
typedef enum {
    ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_START,
    ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT,
    ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_FINISH,
    ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_ABORT,
} E_ZiyanWidgetTransmitDataEvent;

/**
 * @brief Widget file binary array.
 */
typedef struct {
    char *fileName; /*!< The file name of the widget file */
    uint32_t fileSize; /*!< The file size of the widget file, uint : byte */
    const uint8_t *fileBinaryArray; /*!< The binary C array of the widget file */
} T_ZiyanWidgetFileBinaryArray;

/**
 * @brief Widget binary array config.
 */
typedef struct {
    uint16_t binaryArrayCount; /*!< Binary array count. */
    T_ZiyanWidgetFileBinaryArray *fileBinaryArrayList; /*!< Pointer to binary array list */
} T_ZiyanWidgetBinaryArrayConfig;

/**
 * @brief Widget handler item.
 */
typedef struct {
    /*! The index of widget, the index can be numbered starting from 0 and cannot be repeated */
    uint32_t widgetIndex;

    /*! The type of widget, refer to ::E_ZiyanWidgetType */
    E_ZiyanWidgetType widgetType;

    /**
     * @brief Prototype of callback function used to set widget value, the function will be call when the user triggers
     *        the widget.
     * @param widgetType: the type of widget, refer to ::E_ZiyanWidgetType.
     * @param index: the index of widget.
     * @param value: the value of widget, need be set.
     * if the widget type is ZIYAN_WIDGET_TYPE_BUTTON, the value is refer to ::E_ZiyanWidgetButtonState;
     * if the widget type is ZIYAN_WIDGET_TYPE_SWITCH, the value is refer to ::E_ZiyanWidgetSwitchState;
     * if the widget type is ZIYAN_WIDGET_TYPE_SCALE, the value is range from 0 to 100, which represents the percentage
     * of the scale slider;
     * if the Widget type is ZIYAN_WIDGET_TYPE_LIST, the value is range from 0 to N-1 (N is the value of list item
     * count), which represents which item is chosen;
     * if the widget type is ZIYAN_WIDGET_TYPE_INT_INPUT_BOX, the value is the input value of int input box widget.
     * @param userData: the user data need used in callback.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*SetWidgetValue)(E_ZiyanWidgetType widgetType, uint32_t index, int32_t value, void *userData);

    /**
     * @brief Prototype of callback function used to get widget value.
     * @param widgetType: the type of widget, refer to ::E_ZiyanWidgetType.
     * @param index
     * @param value: the value of widget, need be set.
     * if the widget type is ZIYAN_WIDGET_TYPE_BUTTON, the value is refer to ::E_ZiyanWidgetButtonState;
     * if the widget type is ZIYAN_WIDGET_TYPE_SWITCH, the value is refer to ::E_ZiyanWidgetSwitchState;
     * if the widget type is ZIYAN_WIDGET_TYPE_SCALE, the value is range from 0 to 100, which represents the percentage
     * of the scale slider;
     * if the Widget type is ZIYAN_WIDGET_TYPE_LIST, the value is range from 0 to N-1 (N is the value of list item
     * count), which represents which item is chosen;
     * if the widget type is ZIYAN_WIDGET_TYPE_INT_INPUT_BOX, the value is the input value of int input box widget.
     * @param userData: the user data need used in callback function.
     * @return Execution result.
     */
    T_ZiyanReturnCode (*GetWidgetValue)(E_ZiyanWidgetType widgetType, uint32_t index, int32_t *value, void *userData);

    /*! the user data need used in SetWidgetValue and GetWidgetValue callback function. */
    void *userData;
} T_ZiyanWidgetHandlerListItem;

typedef struct {
    union {
        /*! When event is 'ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_START', the buf contains file name, uuid and decoder bitrate. */
        struct {
            uint8_t fileName[32];
            uint8_t fileUuid[32];
            uint32_t fileDecodeBitrate;
        } transDataStartContent;

        /*! When event is 'ZIYAN_WIDGET_TRANSMIT_DATA_EVENT_START', the buf contains file md5 sum. */
        struct {
            uint8_t md5Sum[16];
        } transDataEndContent;
    };
} T_ZiyanWidgetTransDataContent;

typedef struct {
    E_ZiyanWidgetSpeakerState state;
    E_ZiyanWidgetSpeakerWorkMode workMode;
    E_ZiyanWidgetSpeakerPlayMode playMode;
    uint8_t volume;
} T_ZiyanWidgetSpeakerState;

typedef struct {
    T_ZiyanReturnCode (*GetSpeakerState)(T_ZiyanWidgetSpeakerState *speakerState);
    T_ZiyanReturnCode (*SetWorkMode)(E_ZiyanWidgetSpeakerWorkMode workMode);
    T_ZiyanReturnCode (*SetPlayMode)(E_ZiyanWidgetSpeakerPlayMode playMode);
    T_ZiyanReturnCode (*SetVolume)(uint8_t volume);

    T_ZiyanReturnCode (*StartPlay)(void);
    T_ZiyanReturnCode (*StopPlay)(void);

    T_ZiyanReturnCode (*ReceiveTtsData)(E_ZiyanWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size);
    T_ZiyanReturnCode (*ReceiveVoiceData)(E_ZiyanWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size);
} T_ZiyanWidgetSpeakerHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise widget module, and user should call this function before using widget features.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_Init(void);

/**
 * @brief  Register default widget UI configuration file directory path.
 * @note Under Linux system, there are two functions to set the custom widget configuration directory path, function
 *       ZiyanWidget_RegDefaultConfigByDirPath and ZiyanWidget_RegUiConfigByDirPath. When you don't need multi-language
 *       and multi-screen size support, you can just use ZiyanWidget_RegDefaultUiConfigByDirPath function set widget UI
 *       Config directory path. If you need support multi-language and multi-screen size support, you can use function
 *       ZiyanWidget_RegUiConfigByDirPath to specify widget configuration. When the language and screen size is not
 *       cover in your setting by ZiyanWidget_RegUiConfigByDirPath, the widget UI configuration uses setting by
 *       ZiyanWiget_RegDefaultUiConfigByDirPath function.
 * @param widgetConfigDirPath: the widget UI configuration directory path.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegDefaultUiConfigByDirPath(const char *widgetConfigDirPath);

/**
 * @brief Register widget UI configuration file directory path.
 * @note Different widget UI configurations for several language and screen size require the same widget type, index and
 * count.
 * @param appLanguage: mobile app language type.
 * @param appScreenType: mobile app screen type.
 * @param widgetConfigDirPath: the widget UI configuration directory path.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegUiConfigByDirPath(E_ZiyanMobileAppLanguage appLanguage,
                                               E_ZiyanMobileAppScreenType appScreenType,
                                               const char *widgetConfigDirPath);

/**
 * @brief Register default widget UI config by binary array configuration.
 * @note In RTOS, most likely there is no file system. The widget config file content can use C array express. Use this
 * function and ZiyanWidget_RegDefaultUiConfigBinaryArray set widget UI configuration. When the language and screen size
 * is not cover in your setting by ZiyanWidget_RegUiConfigByBinaryArray, the widget UI configuration uses setting by this
 * function.
 * @param binaryArrayConfig: the binary array config for widget UI configuration.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegDefaultUiConfigByBinaryArray(const T_ZiyanWidgetBinaryArrayConfig *binaryArrayConfig);

/**
 * @brief Register widget UI config by binary array configuration.
 * @note Different widget UI configurations for several language and screen size require the same widget type, index and
 * count.
 * @param appLanguage: mobile app language type.
 * @param screenType: mobile app screen type.
 * @param binaryArrayConfig: the binary array config for widget UI configuration.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegUiConfigByBinaryArray(E_ZiyanMobileAppLanguage appLanguage,
                                                   E_ZiyanMobileAppScreenType screenType,
                                                   const T_ZiyanWidgetBinaryArrayConfig *binaryArrayConfig);

/**
 * @brief Register handler list for widgets.
 * @param widgetHandlerList: widget handler list for widgets.
 * @param itemCount: the item count of widget handler list.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegHandlerList(const T_ZiyanWidgetHandlerListItem *widgetHandlerList, uint32_t itemCount);

/**
 * @brief Send message to mobile app floating window.
 * @note the message length can't more than ZIYAN_WIDGET_FLOATING_WINDOW_MSG_MAX_LEN. The max data bandwidth of floating
 * windows message is 2KB/s.
 * @param str: pointer to message string.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidgetFloatingWindow_ShowMessage(const char *str);

/**
 * @brief Get data transmission state of floating window channel. User can use the state as base for controlling
 * floating windows message send.
 * @param state: pointer to floating window channel state.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidgetFloatingWindow_GetChannelState(T_ZiyanDataChannelState *state);

/**
 * @brief Register the handler for widget speaker function interfaces.
 * @note This interface registers the widget speaker function interface, including speaker settings, play operation,
 * speaker status interface.
 * @param widgetSpeakerHandler: pointer to the handler for widget speaker functions.
 * @return Execution result.
 */
T_ZiyanReturnCode ZiyanWidget_RegSpeakerHandler(const T_ZiyanWidgetSpeakerHandler *widgetSpeakerHandler);

#ifdef __cplusplus
}
#endif

#endif // ZIYAN_WIDGET_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

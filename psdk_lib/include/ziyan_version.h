/**
 ********************************************************************
 * @file    ziyan_version.h
 * @brief   This is the header file for "ziyan_version.c", defining the structure and
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
#ifndef ZIYAN_VERSION_H
#define ZIYAN_VERSION_H

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define ZIYAN_SDK_VERSION_MAJOR     1 /*!< ZIYAN SDK major version num, when have incompatible API changes. Range from 0 to 99. */
#define ZIYAN_SDK_VERSION_MINOR     0 /*!< ZIYAN SDK minor version num, when add functionality in a backwards compatible manner changes. Range from 0 to 99. */
#define ZIYAN_SDK_VERSION_MODIFY    0 /*!< ZIYAN SDK modify version num, when have backwards compatible bug fixes changes. Range from 0 to 99. */
#define ZIYAN_SDK_VERSION_DEBUG     3 /*!< ZIYAN SDK version beta info, release version will be 0, when beta version release changes. Range from 0 to 255. */
#define ZIYAN_SDK_VERSION_BUILD     1 /*!< ZIYAN SDK version build info, when jenkins trigger build changes. Range from 0 to 65535. */

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/


/* SDK version info. ---------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // ZIYAN_VERSION_H
/************************ (C) COPYRIGHT ZIYAN Innovations *******END OF FILE******/

/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPI2C
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/
#ifndef FLEXIO_I2C_IP_CFGDEFINES_H
#define FLEXIO_I2C_IP_CFGDEFINES_H

/**
*   @file
*
*   @addtogroup FLEXIO_I2C_DRIVER_CONFIGURATION Flexio_I2c Driver Configurations
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "[!"ecu:get('I2c.FlexIO.I2cHeaderName')"!].h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_I2C_IP_CFGDEFINES_VENDOR_ID                     43
#define FLEXIO_I2C_IP_CFGDEFINES_MODULE_ID                     255
#define FLEXIO_I2C_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION      4
#define FLEXIO_I2C_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION      4
#define FLEXIO_I2C_IP_CFGDEFINES_AR_RELEASE_REVISION_VERSION   0
#define FLEXIO_I2C_IP_CFGDEFINES_SW_MAJOR_VERSION              2
#define FLEXIO_I2C_IP_CFGDEFINES_SW_MINOR_VERSION              0
#define FLEXIO_I2C_IP_CFGDEFINES_SW_PATCH_VERSION              1

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

#define I2C_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2c_MemMap.h"

[!NOCODE!][!//
[!VAR "masterConfig" = "0"!]
[!LOOP "I2cGlobalConfig/I2cChannel/*"!] /* Loop through all configured channels */
    [!IF "contains(I2cHwChannel,'FLEXIO')"!] /* Select only FLEXIO channels */
    [!VAR "sclChannel" = "substring-after(node:value(node:ref(I2cFlexIOConfiguration/SclFlexioRef)/FlexioMclChannelId), '_')"!]
    [!VAR "sdaChannel" = "substring-after(node:value(node:ref(I2cFlexIOConfiguration/SdaFlexioRef)/FlexioMclChannelId), '_')"!]
    [!VAR "pinScl" = "substring-after(node:value(node:ref(I2cFlexIOConfiguration/SclFlexioRef)/FlexioMclPinId), '_')"!]
    [!VAR "pinSda" = "substring-after(node:value(node:ref(I2cFlexIOConfiguration/SdaFlexioRef)/FlexioMclPinId), '_')"!]
[!CODE!]

/* Macros that indicate FLEXIO channels used by I2C */

#ifndef FLEXIO_0_CH_[!"$sclChannel"!]_USED
    #define FLEXIO_0_CH_[!"$sclChannel"!]_USED
#else
    #error "CHANNEL_[!"$sclChannel"!] cannot be used by I2C driver. Channel locked by another driver"
#endif

#ifdef FLEXIO_0_CH_[!"$sclChannel"!]_USED
    #define I2C_CHANNEL_[!"$sclChannel"!]_USED
#else
    #error "CHANNEL_[!"$sclChannel"!] cannot be used by I2C driver. Channel locked by another driver"
#endif

#ifndef FLEXIO_0_CH_[!"$sdaChannel"!]_USED
    #define FLEXIO_0_CH_[!"$sdaChannel"!]_USED
#else
    #error "CHANNEL_[!"$sdaChannel"!] cannot be used by I2C driver. Channel locked by another driver"
#endif

#ifdef FLEXIO_0_CH_[!"$sdaChannel"!]_USED
    #define I2C_CHANNEL_[!"$sdaChannel"!]_USED
#else
    #error "CHANNEL_[!"$sdaChannel"!] cannot be used by I2C driver. Channel locked by another driver"
#endif

/*Macros that indicate FLEXIO pins used by I2C */

#ifndef FLEXIO_0_PIN_[!"$pinScl"!]_USED
    #define FLEXIO_0_PIN_[!"$pinScl"!]_USED
#else
    #error "PIN_[!"$pinScl"!] cannot be used by I2C driver. Pin used by another driver!"
#endif

#ifndef FLEXIO_0_PIN_[!"$pinSda"!]_USED
    #define FLEXIO_0_PIN_[!"$pinSda"!]_USED
#else
    #error "PIN_[!"$pinSda"!] cannot be used by I2C driver. Pin used by another driver!"
#endif

[!ENDCODE!]
    [!ENDIF!]
[!ENDLOOP!] /* End of the loop */
[]

[!ENDNOCODE!][!//

#define I2C_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2c_MemMap.h"

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_I2C_IP_CFGDEFINES_H */


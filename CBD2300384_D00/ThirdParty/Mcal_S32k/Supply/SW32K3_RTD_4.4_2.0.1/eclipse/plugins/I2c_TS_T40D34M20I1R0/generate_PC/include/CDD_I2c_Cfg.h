/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : LPI2C
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

/**
*   @file
*
*   @addtogroup I2C_DRIVER_CONFIGURATION I2c Driver Configurations
*   @{
*/

#ifndef CDD_I2C_CFG_H
#define CDD_I2C_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
#include "CDD_I2c_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!]
#include "CDD_I2c_PBcfg.h"
[!ENDIF!]
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/* Inclusion of incompatible header files shall be avoided */

#define I2C_VENDOR_ID_CFG                    43
#define I2C_MODULE_ID_CFG                    255
#define I2C_AR_RELEASE_MAJOR_VERSION_CFG     4
#define I2C_AR_RELEASE_MINOR_VERSION_CFG     4
#define I2C_AR_RELEASE_REVISION_VERSION_CFG  0
#define I2C_SW_MAJOR_VERSION_CFG             2
#define I2C_SW_MINOR_VERSION_CFG             0
#define I2C_SW_PATCH_VERSION_CFG             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and I2c_Cfg header file are of the same module */
#if (I2C_MODULE_ID_CFG  != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_MODULE_ID)
    #error "CDD_I2c_Cfg.h and CDD_I2c_PBcfg.h have different vendor ids"
#endif
/* Check if current file and I2c_Types header file are of the same vendor */
#if (I2C_VENDOR_ID_CFG != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_VENDOR_ID)
    #error "CDD_I2c_Cfg.h and CDD_I2c_PBcfg.h have different vendor ids"
#endif
/* Check if current file and I2c_Types header file are of the same Autosar version */
#if (  (I2C_AR_RELEASE_MAJOR_VERSION_CFG    != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
       (I2C_AR_RELEASE_MINOR_VERSION_CFG    != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
       (I2C_AR_RELEASE_REVISION_VERSION_CFG != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_I2c_Cfg.h and CDD_I2c_PBcfg.h are different"
#endif
/* Check if current file and I2c_Types header file are of the same Software version */
#if ( (I2C_SW_MAJOR_VERSION_CFG != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MAJOR_VERSION) || \
      (I2C_SW_MINOR_VERSION_CFG != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_MINOR_VERSION) || \
      (I2C_SW_PATCH_VERSION_CFG != I2C[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCFG_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of CDD_I2c_Cfg.h and CDD_I2c_PBcfg.h are different"
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
[!IF "not(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!] [!//
/**
* @brief   macro contains all PB configuration macros.
*/
#define I2C_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!VAR "variantIndex"="0"!][!//
[!VAR "variantNumber"="variant:size()"!][!//
[!LOOP "variant:all()"!][!VAR "variantIndex"="$variantIndex + 1"!][!//
 I2C_CONFIG_[!"."!]_PB [!IF "$variantIndex < $variantNumber"!]\[!ENDIF!]
[!ENDLOOP!][!//
[!ELSE!][!//
 I2C_CONFIG_PB
[!ENDIF!]
[!ENDIF!]
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @brief          Precompile Support On.
* @details        VARIANT-PRE-COMPILE: Only parameters with "Pre-compile time"
*                 configuration are allowed in this variant.
*/
#define I2C_PRECOMPILE_SUPPORT [!IF "(IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size() <= 1)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief   Total number of I2c channel configured.
*/
#define I2C_HW_MAX_CONFIG           ([!"num:i(count(I2cGlobalConfig/I2cChannel/*))"!]U)


/**
* @brief   Total number of available hardware I2C channels.
*/
#define I2C_HW_MAX_MODULES          ([!"num:i(count(ecu:list('I2c.I2cGlobalConfig.I2cChannel.I2cHwChannel')))"!]U)

/**
* @brief   Total number of available hardware LPI2C channels.
*/
#define I2C_LPI2C_MAX_MODULES       ([!"num:i(count(ecu:list('I2c.I2cGlobalConfig.I2cChannel.I2cHwChannel')[contains(., 'LPI2C')]))"!]U)

/**
* @brief            This is the ID of the first FLEXIO channel.
*/
#define I2C_FLEXIO_FIRST_CHANNEL_U8   (I2C_LPI2C_MAX_MODULES)

/**
* @brief   Switches the Development Error Detection and Notification ON or OFF.
*/
#define I2C_DEV_ERROR_DETECT      [!IF "GeneralConfiguration/I2cDevErrorDetect"!](STD_ON)  /* Enable Development Error Detection */[!ELSE!](STD_OFF) /* Disable Development Error Detection */[!ENDIF!]

/**
* @brief          Support for version info API.
*/
#define I2C_VERSION_INFO_API      [!IF "GeneralConfiguration/I2cVersionInfoApi"!](STD_ON)  /* Enable API I2c_GetVersionInfo      */[!ELSE!](STD_OFF) /* Disable API I2c_GetVersionInfo     */[!ENDIF!]

[!NOCODE!]
[!IF "variant:size()<=1"!][!//
[!CODE!]
/**
* @brief          Link I2c channels symbolic names with I2c channel IDs.
*/
[!ENDCODE!]
[!FOR "index" = "0" TO "num:i(ecu:get('I2c.I2cGlobalConfig.I2cChannel'))-1"!]
    [!VAR "I2cEcuHwChannel" = "string((ecu:list('I2c.I2cGlobalConfig.I2cChannel.I2cHwChannel'))[$index+1])"!]
    [!VAR "ChannelFound" = "0"!]
    [!LOOP "I2cGlobalConfig/I2cChannel/*"!]
        [!VAR "HardwareChannel" = "string(I2cHwChannel)"!]
        [!IF "$I2cEcuHwChannel = $HardwareChannel"!]
            [!IF "$ChannelFound = 0"!]
                [!CODE!][!//

#define I2C_[!"$I2cEcuHwChannel"!]     [!"num:i($index)"!]U[!//

                [!ENDCODE!]
                [!VAR "ChannelFound" = "1"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDFOR!]
[!ENDIF!]
[!ENDNOCODE!]

[!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cMulticoreSupport='true'"!]
[!CODE!]
/**
* @brief          Unallocate coreid.
*/
#define I2C_UNALLOCATED_CORE_ID     ([!"num:i(255)"!]U)
[!ENDCODE!][!//
[!ELSE!][!//
[!CODE!]
/**
* @brief          spurious coreid.
*/
#define I2C_SPURIOUS_CORE_ID        ([!"num:i(0)"!]U)
[!ENDCODE!][!//
[!ENDIF!][!//

/**
* @brief          Symbolic names for configured channels.
*/
[!LOOP "I2cGlobalConfig/I2cChannel/*"!][!//
#define I2cConf_I2cChannel_[!"node:name(.)"!]         [!"I2cChannelId"!]
[!ENDLOOP!][!//

/**
* @brief          FLEXIO Channel Used
*/
#define I2C_FLEXIO_USED   [!IF "(GeneralConfiguration/I2cFlexIOUsed='true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

[!LOOP "I2cGlobalConfig/*/I2cChannel/*"!][!//
[!IF "contains(I2cHwChannel, 'FLEXIO')"!][!//
#define I2C_[!"node:value(I2cHwChannel)"!]_USED
#define I2C_[!"node:value(I2cHwChannel)"!]_ISR_USED
#define I2C_FLEXIO_0_PIN_[!"substring-after(I2cFlexIOConfiguration/I2cFlexIOSclPin,'FXIO_D')"!]_USED
#define I2C_FLEXIO_0_PIN_[!"substring-after(I2cFlexIOConfiguration/I2cFlexIOSdaPin,'FXIO_D')"!]_USED
[!ENDIF!][!//
[!ENDLOOP!][!//

/**
* @brief            Enable/Disable the API for reporting the Dem Error.
*/
#define I2C_DISABLE_DEM_REPORT_ERROR_STATUS    [!IF "(GeneralConfiguration/I2cDisableDemReportErrorStatus='false') or (node:exists(I2cGlobalConfig/I2cDemEventParameterRefs))"!](STD_OFF)[!ELSE!](STD_ON)[!ENDIF!]

/**
* @brief            Enable/Disable Multi Core Support.
*/
#define I2C_MULTICORE_SUPPORT   [!IF "(GeneralConfiguration/I2cMulticoreSupport='true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief            DMA is used for at least one channel (STD_ON/STD_OFF)
*/
[!CODE!][!//
#define I2C_DMA_USED   [!IF "(GeneralConfiguration/I2cDmaUsed='true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#if (I2C_MULTICORE_SUPPORT == STD_ON)
    #define I2c_GetCoreID()            OsIf_GetCoreID()
#else
    #define I2c_GetCoreID()            ((uint32)0UL)
#endif /* (I2C_MULTICORE_SUPPORT == STD_ON) */

/**
* @brief            TIMEOUT for sync transmissions
*/
#define I2C_TIMEOUT               [!"GeneralConfiguration/I2cTimeoutDuration"!]U
[!ENDCODE!][!//

[!IF "node:exists(GeneralConfiguration/I2cCallback)"!]
[!IF "node:exists(GeneralConfiguration/I2cErrorCallback)"!]
/**
* @brief            I2c slave bit error event
*/
#define  I2C_EVENT_BIT_ERROR_SLAVE  0x00U

/**
* @brief            I2c slave underrun event
*/
#define  I2C_EVENT_UNDERRUN_SLAVE  0x01U

/**
* @brief            I2c slave overrun event
*/
#define  I2C_EVENT_OVERRUN_SLAVE  0x02U
[!ENDIF!]
/**
* @brief            I2c slave rx buffer full
*/
#define  I2C_EVENT_RX_FULL_SLAVE  0x03U

/**
* @brief            I2c slave tx buffer empty
*/
#define  I2C_EVENT_TX_EMPTY_SLAVE  0x04U

/**
* @brief            I2c slave tx request 
*/
#define  I2C_EVENT_TX_REQ_SLAVE 0x05U

/**
* @brief            I2c slave rx request
*/
#define  I2C_EVENT_RX_REQ_SLAVE 0x06U

/**
* @brief            I2c slave stop transfer 
*/
#define  I2C_EVENT_STOP_SLAVE 0x07U
[!IF "node:exists(GeneralConfiguration/I2cErrorCallback)"!]
/**
* @brief            I2c NACK event
*/
#define  I2C_EVENT_NACK_MASTER  0x08U

/**
* @brief            I2c master arbitration lost event
*/
#define  I2C_EVENT_ARBITRATION_LOST_MASTER  0x09U

/**
* @brief            I2c master fifo error
*/
#define  I2C_MASTER_EVENT_ERROR_FIFO  0x0AU
[!ENDIF!]
/**
* @brief            I2c master end transfer 
*/
#define I2C_EVENT_END_TRANSFER_MASTER  0x0BU

/**
* @brief            I2c master pin low timeout
*/
#define I2C_EVENT_PIN_LOW_TIMEOUT_MASTER  0x0CU

/**
* @brief            I2c master dma transfer error event
*/
#define I2C_EVENT_DMA_TRANSFER_ERROR_MASTER 0x0DU

/**
* @brief            I2c slave dma transfer error event
*/
#define I2C_EVENT_DMA_TRANSFER_ERROR_SLAVE 0x0EU
[!ENDIF!]

[!IF "node:exists(GeneralConfiguration/I2cCallback)"!]
/**
* @brief            The callback configured by the user for i2c events.
*/

#define I2C_MODULE_CALLBACK(Event, Channel) ([!"normalize-space(GeneralConfiguration/I2cCallback)"!](Event, Channel))

#define I2C_START_SEC_CODE
#include "I2c_MemMap.h"

/**
* @brief            The callback configured by the user for i2c events.
*/
extern void I2C_MODULE_CALLBACK(uint8 Event, uint8 Channel);

#define I2C_STOP_SEC_CODE
#include "I2c_MemMap.h"
[!ENDIF!]

[!IF "node:exists(GeneralConfiguration/I2cErrorCallback)"!]
/**
* @brief            The callback configured by the user for i2c events.
*/
#define I2C_ERROR_CALLBACK(Event, Channel) ([!"normalize-space(GeneralConfiguration/I2cErrorCallback)"!](Event, Channel))

#define I2C_START_SEC_CODE
#include "I2c_MemMap.h"

/**
* @brief            The callback configured by the user for i2c events.
*/
extern void I2C_ERROR_CALLBACK(uint8 Event, uint8 Channel);

#define I2C_STOP_SEC_CODE
#include "I2c_MemMap.h"
[!ENDIF!]

[!MACRO "I2cExportDriverConfiguration"!]
[!NOCODE!][!//
[!VAR "configName" = "as:name(I2cGlobalConfig)"!][!//
[!CODE!][!//

/**
* @brief            Pre-Compile structure from CDD_I2c_Cfg.c file.
*/
#define I2C_CONF_PB \
[!ENDCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "variantIndex"="0"!][!//
    [!VAR "variantNumber"="variant:size()"!][!//
    [!LOOP "variant:all()"!][!VAR "variantIndex"="$variantIndex + 1"!][!//
    [!//
        [!CODE!][!WS4!]extern const I2c_ConfigType [!"$configName"!]_[!"."!];[!IF "$variantIndex < $variantNumber"!]\[!ENDIF!][!CR!][!ENDCODE!]
    [!ENDLOOP!][!//
[!ELSE!][!//
    [!CODE!][!WS4!]extern const I2c_ConfigType [!"$configName"!];[!CR!][!ENDCODE!]
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!]

[!IF "((IMPLEMENTATION_CONFIG_VARIANT != 'VariantPostBuild') and (variant:size()>1)) or (IMPLEMENTATION_CONFIG_VARIANT = 'VariantPostBuild')"!]
    [!CALL "I2cExportDriverConfiguration"!]
[!ENDIF!]

/**
* @brief        Variable storing number of maximum partitions using in configuration.
*/
[!NOCODE!]
[!VAR "MaxCoreId"="0"!]
[!IF "as:modconf('I2c')[1]/GeneralConfiguration/I2cMulticoreSupport='true'"!][!//
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
            [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
                [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
            [!ENDIF!]
    [!ENDLOOP!][!//
[!ENDIF!]
[!ENDNOCODE!]
#define I2C_MAX_CORE_ID     ((uint8)[!"num:i($MaxCoreId+1)"!]U)

/**
* @brief    the value initialization un init for each core.
*/
#define I2C_UNINIT_CORE \
{[!//
[!NOCODE!]
    [!VAR "FirstCore"="num:i(1)"!]
    [!FOR "Coreid"="0" TO "$MaxCoreId"!]
[!IF "$FirstCore = num:i(0)"!][!CODE!],[!ENDCODE!][!ELSE!][!VAR "FirstCore"="num:i(0)"!][!ENDIF!][!CODE!] I2C_UNINIT[!ENDCODE!][!//
    [!ENDFOR!]
[!ENDNOCODE!][!//
}

/**
* @brief    the value initialization null pointer for each core.
*/
#define I2C_NULL_PTR \
{[!//
[!NOCODE!]
    [!VAR "FirstCore"="num:i(1)"!]
    [!FOR "Coreid"="0" TO "$MaxCoreId"!]
[!IF "$FirstCore = num:i(0)"!][!CODE!],[!ENDCODE!][!ELSE!][!VAR "FirstCore"="num:i(0)"!][!ENDIF!][!CODE!] NULL_PTR[!ENDCODE!][!//
    [!ENDFOR!]
[!ENDNOCODE!][!//
}

[!NOCODE!][!//
[!VAR "HwMaxChannel" = "num:i(count(ecu:list('I2c.I2cGlobalConfig.I2cChannel.I2cHwChannel')))"!]
[!ENDNOCODE!][!//
/**
* @brief        the value initialization Hardware Map.
*/
#define I2C_HW_MAP_INIT \
{[!//
[!NOCODE!]
    [!VAR "FirstChannel"="num:i(1)"!]
    [!FOR "Channel"="0" TO "$HwMaxChannel - 1"!]
[!IF "$FirstChannel = num:i(0)"!][!CODE!],[!ENDCODE!][!ELSE!][!VAR "FirstChannel"="num:i(0)"!][!ENDIF!][!CODE!] -1[!ENDCODE!][!//
    [!ENDFOR!]
[!ENDNOCODE!][!//
}
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
/**
 * @brief Definition of the type of activation or procession mechanism of an I2C hw unit
 * @implements I2c_DataDirectionType_enum
 */
typedef enum
{
    I2C_SEND_DATA       = 0x0U, /**< @brief Used to send data to a slave */
    I2C_RECEIVE_DATA    = 0x1U  /**< @brief Used to receive data from a slave */
} I2c_DataDirectionType;

/**
 * @brief Definition of the hardware channel type
 * 
 */
typedef enum
{
    I2C_LPI2C_CHANNEL       = 0x0U, /**< @brief This is used for LPI2C channels */
    I2C_FLEXIO_CHANNEL      = 0x1U  /**< @brief This is used for FlexIO channels */
} I2c_HwChannelType;

/*==================================================================================================
*                                STRUCTURES AND OTHER S
==================================================================================================*/
/**
* @brief            DEM error reporting configuration.
* @details          This structure contains information DEM error reporting
*/
typedef struct
{
    Mcal_DemErrorType I2c_E_TimeoutFailureCfg;

} I2c_DemConfigType;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define I2C_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2c_MemMap.h"

extern const I2c_DemConfigType I2c_DemConfig;

#define I2C_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2c_MemMap.h"
/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* CDD_I2C_CFG_H */

/** @}*/

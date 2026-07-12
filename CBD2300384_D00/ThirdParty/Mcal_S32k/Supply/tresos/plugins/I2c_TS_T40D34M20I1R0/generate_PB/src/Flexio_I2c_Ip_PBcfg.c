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
*   @addtogroup FLEXIO_I2C_DRIVER_CONFIGURATION Flexio_I2c Driver Configurations
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCfg.h"
#include "Flexio_I2c_Ip.h"

#if(FLEXIO_I2C_IP_ENABLE == STD_ON)
#if (STD_ON == FLEXIO_I2C_IP_DMA_FEATURE_AVAILABLE)
#include "Dma_Ip.h"
#endif
#endif
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_MODULE_ID_C                     255
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1


/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and I2c header file are of the same vendor */
#if (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != FLEXIO_I2C_IP_VENDOR_ID)
    #error "Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_I2c_Ip.h have different vendor ids"
#endif
/* Check if current file and I2c header file are of the same Autosar version */
#if ((FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != FLEXIO_I2C_IP_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != FLEXIO_I2C_IP_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != FLEXIO_I2C_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCfg.c and Flexio_I2c_Ip.h are different"
#endif
/* Check if current file and I2c header file are of the same Software version */
#if ((FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != FLEXIO_I2C_IP_SW_MAJOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != FLEXIO_I2C_IP_SW_MINOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != FLEXIO_I2C_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_I2c_Ip.h are different"
#endif

/* Check if current file and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h header file are of the same vendor */
#if (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID )
    #error "Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h have different vendor ids"
#endif
/* Check if current file and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h header file are of the same Autosar version */
#if ((FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBCfg.c and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif
/* Check if current file and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h header file are of the same Software version */
#if ((FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION) || \
     (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION ) \
    )
    #error "Software Version Numbers of Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_I2c_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if(FLEXIO_I2C_IP_ENABLE == STD_ON)
        #if(STD_ON == FLEXIO_I2C_IP_DMA_FEATURE_AVAILABLE)
            /* Check if current file and Dma_Ip.h header file are of the same Autosar version */
            #if ( (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
                  (FLEXIO_I2C_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != DMA_IP_AR_RELEASE_MINOR_VERSION)    \
                 )
                #error "AutoSar Version Numbers of Flexio_I2c_Ip.c and Dma_Ip.h are different"
            #endif
        #endif /* (STD_ON == FLEXIO_I2C_IP_DMA_FEATURE_AVAILABLE) */
    #endif /* (FLEXIO_I2C_IP_ENABLE == STD_ON) */
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

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

#if(FLEXIO_I2C_IP_ENABLE == STD_ON)
#define I2C_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I2c_MemMap.h"

[!NOCODE!][!//
[!VAR "masterStateIdx" = "0"!]
[!VAR "slaveStateIdx" = "0"!]
[!LOOP "I2cGlobalConfig/I2cChannel/*"!] /* Loop through all configured channels */
    [!IF "contains(I2cHwChannel,'FLEXIO')"!] /* Select only FLEXIO_I2C channels */
        [!IF "contains(I2cMasterSlaveConfiguration,'MASTER')"!] /* Based on the master-slave configuration other structures will be generated */
[!CODE!]

/* Channel configuration for channel [!"I2cHwChannel"!] - configured as master */
Flexio_I2c_Ip_MasterConfigType Flexio_I2cMasterChannel[!"I2cChannelId"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!VAR "SclTimerId" = "num:i(substring-before(substring-after(node:value(I2cHwChannel),'FLEXIO_0_CH_'),'_'))"!][!//
[!VAR "CtlTimerId" = "num:i(substring-after(substring-after(node:value(I2cHwChannel),'FLEXIO_0_CH_'),'_'))"!][!//
[!VAR "instanceId" = "num:i(substring-before(substring-after(node:value(I2cHwChannel),'FLEXIO_'),'_'))"!][!//
[!VAR "SclPinId" = "num:i(substring-after(node:value(node:ref(I2cFlexIOConfiguration/SclFlexioRef)/FlexioMclPinId), '_'))"!][!//
[!VAR "SdaPinId" = "num:i(substring-after(node:value(node:ref(I2cFlexIOConfiguration/SdaFlexioRef)/FlexioMclPinId), '_'))"!][!//
    /* Slave address */
    0U,
    /* Transfer Type */
    [!"I2cFlexIOConfiguration/I2cAsyncMethod"!],
    /* Baud rate in hertz */
    (uint32)[!"I2cFlexIOConfiguration/I2cBaudRate"!],
    /* I2c flexio compare value */
    [!"I2cFlexIOConfiguration/I2cFlexIOCompareValue"!],
    /* I2cFlexIOChannelPrescaler */
[!IF "node:exists(I2cFlexIOConfiguration/I2cFlexIOChannelPrescaler)"!]
    FLEXIO_TMR_DECREMENT_ON_FLEXIO_CLK_[!"I2cFlexIOConfiguration/I2cFlexIOChannelPrescaler"!],
[!ELSE!]
    FLEXIO_TMR_DECREMENT_ON_FLEXIO_CLK_DIV_1,
[!ENDIF!]
    /* I2c flexio sda pin */
    [!"num:i($SdaPinId)"!]U,
    /* I2c flexio scl pin */
    [!"num:i($SclPinId)"!]U,
    /* Master Callback */
    NULL_PTR,
    /* I2c Master Callback Parameter */
    [!"I2cChannelId"!]U,
    /* FlexIO device instance number */
    [!"num:i($instanceId)"!]U,
    /* Index of first used internal resource instance (shifter and timer) */
    [!"num:i($SclTimerId)"!]U,
    [!IF "I2cFlexIOConfiguration/I2cAsyncMethod = 'FLEXIO_I2C_USING_DMA'"!]
    [!"node:value(node:ref(I2cFlexIOConfiguration/I2cDmaRxChannelRef)/dmaLogicChannel_LogicName)"!],
    [!"node:value(node:ref(I2cFlexIOConfiguration/I2cDmaTxChannelRef)/dmaLogicChannel_LogicName)"!],
    [!ELSE!]
    0U,
    0U,
    [!ENDIF!]
    &Flexio_I2c_Ip_MasterState[[!"num:i($masterStateIdx)"!]]
};
    [!VAR "masterStateIdx" = "num:i($masterStateIdx)+1"!]
[!ENDCODE!]
        [!ENDIF!] /* End of master-slave if */
    [!ENDIF!] /* End of Select only FLEXIO_I2C channels */
[!ENDLOOP!] /* End of the loop */

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
#endif /* FLEXIO_I2C_IP_ENABLE == STD_OFF */

#ifdef __cplusplus
}
#endif

/** @} */


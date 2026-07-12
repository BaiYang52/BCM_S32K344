[!CODE!][!//
/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K3XX
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
*   @file    modules.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Base - Project configuration.
*   @details Header file describing all Autosar MCAL modules present in the current project.
*
*   @addtogroup BASE_COMPONENT
*   @{
*/
/**
* @file        modules.h
*/

#ifndef MODULES_H
#define MODULES_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/** 
* @file        modules.h
* @brief Include Standard types & defines
*/
#include "StandardTypes.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/** 
@{
* @brief Parameters that shall be published within the modules header file.
*       The integration of incompatible files shall be avoided.
*/
#define MODULES_VENDOR_ID                       43
#define MODULES_MODULE_ID                       0
#define MODULES_AR_RELEASE_MAJOR_VERSION_H      4
#define MODULES_AR_RELEASE_MINOR_VERSION_H      4
#define MODULES_AR_RELEASE_REVISION_VERSION_H   0
#define MODULES_SW_MAJOR_VERSION_H              2
#define MODULES_SW_MINOR_VERSION_H              0
#define MODULES_SW_PATCH_VERSION_H              1
/**@}*/
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if source file and StandardTypes.h header file are of the same Autosar version */
    #if ((MODULES_AR_RELEASE_MAJOR_VERSION_H != STD_AR_RELEASE_MAJOR_VERSION) || \
         (MODULES_AR_RELEASE_MINOR_VERSION_H != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of modules.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** 
* @brief This constant used for other modules to check if ADC is present in the project.   
*/
#define USE_ADC_MODULE              ([!IF "node:exists(as:modconf("Adc")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if CAN is present in the project.   
*/
#define USE_CAN_MODULE              ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Can")/../.."!][!IF "text:match(node:name(.), "^Can$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if CAN_43_LLCE is present in the project.   
*/
#define USE_CAN_43_LLCE_MODULE      ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Can")/../.."!][!IF "text:match(node:name(.), "^Can_43_LLCE$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
 
/** 
* @brief This constant used for other modules to check if CRCU is present in the project.   
*/
#define USE_CRCU_MODULE              ([!IF "node:exists(as:modconf("Crcu")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if CRYPTO is present in the project.   
*/
#define USE_CRYPTO_MODULE              ([!IF "node:exists(as:modconf("Crypto")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if CSEC is present in the project.   
*/
#define USE_CSEC_MODULE              ([!IF "node:exists(as:modconf("Csec")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if DIO is present in the project.   
*/
#define USE_DIO_MODULE              ([!IF "node:exists(as:modconf("Dio")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if EEP is present in the project. 
*/
#define USE_EEP_MODULE              ([!IF "node:exists(as:modconf("Eep")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if ETH is present in the project. 
*/
#define USE_ETH_MODULE              ([!IF "node:exists(as:modconf("Eth")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if FEE is present in the project.   
*/
#define USE_FEE_MODULE              ([!IF "node:exists(as:modconf("Fee")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if FLS is present in the project.   
*/
#define USE_FLS_MODULE              ([!IF "node:exists(as:modconf("Fls")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if Fr is present in the project.   
*/
#define USE_FR_MODULE              ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Fr")/../.."!][!IF "text:match(node:name(.), "^Fr$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if Fr_43_LLCE is present in the project.   
*/
#define USE_FR_43_LLCE_MODULE       ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Fr")/../.."!][!IF "text:match(node:name(.), "^Fr_43_LLCE$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if GPT is present in the project.   
*/
#define USE_GPT_MODULE              ([!IF "node:exists(as:modconf("Gpt")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if I2C is present in the project.   
*/
#define USE_I2C_MODULE              ([!IF "node:exists(as:modconf("I2c")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if ICU is present in the project.   
*/
#define USE_ICU_MODULE              ([!IF "node:exists(as:modconf("Icu")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if LIN is present in the project.   
*/
#define USE_LIN_MODULE              ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Lin")/../.."!][!IF "text:match(node:name(.), "^Lin$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
 
/** 
* @brief This constant used for other modules to check if Lin_43_LLCE is present in the project.   
*/
#define USE_LIN_43_LLCE_MODULE      ([!VAR "MDL_DEF" = "0"!][!LOOP "as:modconf("Lin")/../.."!][!IF "text:match(node:name(.), "^Lin_43_LLCE$")"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!ENDLOOP!][!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if MCEM is present in the project.   
*/
#define USE_MCEM_MODULE              ([!IF "node:exists(as:modconf("Mcem")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if MCEM is present in the project.   
*/
#define USE_MCL_MODULE              ([!IF "node:exists(as:modconf("Mcl")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if MCU is present in the project.   
*/
#define USE_MCU_MODULE              ([!IF "node:exists(as:modconf("Mcu")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if OCU is present in the project. 
*/
#define USE_OCU_MODULE             ([!IF "node:exists(as:modconf("Ocu")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief This constant used for other modules to check if PORT is present in the project.   
*/
#define USE_PORT_MODULE             ([!IF "node:exists(as:modconf("Port")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if PWM is present in the project.   
*/
#define USE_PWM_MODULE              ([!IF "node:exists(as:modconf("Pwm")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if SENT is present in the project.   
*/
#define USE_SENT_MODULE              ([!IF "node:exists(as:modconf("Sent")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if SPI is present in the project.   
*/
#define USE_SPI_MODULE              ([!IF "node:exists(as:modconf("Spi")[1])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/** 
* @brief This constant used for other modules to check if WDG is present in the project.   
*/
#define USE_WDG_MODULE    ([!VAR "MDL_DEF" = "0"!][!//
[!LOOP "as:modconf("Wdg")/../.."!][!//
[!IF "text:match(node:name(.), '^Wdg$')"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])[!//


[!FOR "i" = "1" TO "10"!]
 /**
 * @brief This constant used for other modules to check if WDG Instance [!"$i"!] is present in the project.
 */
#define USE_WDG_INSTANCE[!"$i"!]     ([!VAR "MDL_DEF" = "0"!][!//
[!LOOP "as:modconf("Wdg")/../.."!][!//
[!IF "text:match(node:name(.), concat('^Wdg_43_Instance',string($i),'$'))"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
[!ENDFOR!]


/** 
* @brief This constant used for other modules to check if WDG for chip vr5510 is present in the project.   
*/
#define USE_WDG_43_VR5510_MODULE      ([!VAR "MDL_DEF" = "0"!][!//
[!LOOP "as:modconf("Wdg")/../.."!][!//
[!IF "text:match(node:name(.), '^Wdg_43_VR5510$')"!][!VAR "MDL_DEF" = "1"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!IF "$MDL_DEF = 1"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* MODULES_H */

/** @} */

[!ENDCODE!][!//


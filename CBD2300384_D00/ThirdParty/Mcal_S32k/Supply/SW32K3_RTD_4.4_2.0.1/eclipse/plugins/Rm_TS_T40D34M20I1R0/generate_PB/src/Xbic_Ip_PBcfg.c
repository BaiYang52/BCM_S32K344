/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

/**
*   @file Xbic_Ip_PBcfg.c
*
*   @addtogroup Xbic_IP Xbic IPV Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Xbic_Ip_Types.h"
#include "Xbic_Ip_Cfg.h"
#include "Xbic_Ip_Device_Registers.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                     43
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C      4
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C   0
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C              2
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C              0
#define RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C              1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Xbic_Ip_Types.h */
#if (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != RM_XBIC_IP_TYPES_VENDOR_ID)
    #error "Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Types.h have different vendor ids"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != RM_XBIC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != RM_XBIC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != RM_XBIC_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Types.h are different"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != RM_XBIC_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != RM_XBIC_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != RM_XBIC_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Types.h are different"
#endif

/* Checks against Xbic_Ip_Cfg.h */
#if (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != RM_XBIC_IP_CFG_VENDOR_ID)
    #error "Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != RM_XBIC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != RM_XBIC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != RM_XBIC_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Cfg.h are different"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != RM_XBIC_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != RM_XBIC_IP_CFG_SW_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != RM_XBIC_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Cfg.h are different"
#endif

/* Checks against Xbic_Ip_Device_Registers.h */
#if (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != RM_XBIC_IP_DEVICE_REGISTERS_VENDOR_ID)
    #error "Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Device_Registers.h have different vendor ids"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != RM_XBIC_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != RM_XBIC_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != RM_XBIC_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Device_Registers.h are different"
#endif
#if ((RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != RM_XBIC_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != RM_XBIC_IP_DEVICE_REGISTERS_SW_MINOR_VERSION) || \
     (RM_XBIC_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != RM_XBIC_IP_DEVICE_REGISTERS_SW_PATCH_VERSION))
    #error "Software Version Numbers of Xbic_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Xbic_Ip_Device_Registers.h are different"
#endif
/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
[!IF "RmGeneral/RmXbicConfigurable"!]
[!SELECT "RmConfigSet/Xbic_Configuration/XbicCheckingControl"!]
[!NOCODE!][!//

[!VAR "XBIC_0_MCR" = "num:hextoint('0xFFFF0000')"!][!//
[!VAR "XBIC_1_MCR" = "num:hextoint('0xFFFF0000')"!][!//
[!VAR "XBIC_2_MCR" = "num:hextoint('0xFFFF0000')"!][!//
[!VAR "XBIC_3_MCR" = "num:hextoint('0xFFFF0000')"!][!//
[!LOOP "./*"!][!//

    [!IF "string(./XbicInstance) = 'XBIC_0' or string(./XbicInstance) = 'XBIC_CE'"!]
        [!FOR "i" = "0" TO "7"!][!//
        [!IF "(node:value(concat("./SE", $i))) = 'false'"!]
            [!VAR "XBIC_0_MCR" = "bit:and($XBIC_0_MCR, bit:not( bit:shr(num:hextoint('0x80000000'),$i)))"!]
        [!ENDIF!]
        [!IF "(node:value(concat("./ME", $i))) = 'false'"!]
            [!VAR "XBIC_0_MCR" = "bit:and($XBIC_0_MCR, bit:not( bit:shr(num:hextoint('0x00800000'),$i)))"!]
        [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]

    [!IF "string(./XbicInstance) = 'XBIC_1' or string(./XbicInstance) = 'XBIC_SMU'"!]
        [!FOR "i" = "0" TO "7"!][!//
        [!IF "(node:value(concat("./SE", $i))) = 'false'"!]
            [!VAR "XBIC_1_MCR" = "bit:and($XBIC_1_MCR, bit:not( bit:shr(num:hextoint('0x80000000'),$i)))"!]
        [!ENDIF!]
        [!IF "(node:value(concat("./ME", $i))) = 'false'"!]
            [!VAR "XBIC_1_MCR" = "bit:and($XBIC_1_MCR, bit:not( bit:shr(num:hextoint('0x00800000'),$i)))"!]
        [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]

    [!IF "string(./XbicInstance) = 'XBIC_2'"!]
        [!FOR "i" = "0" TO "7"!][!//
        [!IF "(node:value(concat("./SE", $i))) = 'false'"!]
            [!VAR "XBIC_2_MCR" = "bit:and($XBIC_2_MCR, bit:not( bit:shr(num:hextoint('0x80000000'),$i)))"!]
        [!ENDIF!]
        [!IF "(node:value(concat("./ME", $i))) = 'false'"!]
            [!VAR "XBIC_2_MCR" = "bit:and($XBIC_2_MCR, bit:not( bit:shr(num:hextoint('0x00800000'),$i)))"!]
        [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]

    [!IF "string(./XbicInstance) = 'XBIC_3'"!]
        [!FOR "i" = "0" TO "7"!][!//
        [!IF "(node:value(concat("./SE", $i))) = 'false'"!]
            [!VAR "XBIC_3_MCR" = "bit:and($XBIC_3_MCR, bit:not( bit:shr(num:hextoint('0x80000000'),$i)))"!]
        [!ENDIF!]
        [!IF "(node:value(concat("./ME", $i))) = 'false'"!]
            [!VAR "XBIC_3_MCR" = "bit:and($XBIC_3_MCR, bit:not( bit:shr(num:hextoint('0x00800000'),$i)))"!]
        [!ENDIF!]
        [!ENDFOR!]
    [!ENDIF!]
[!ENDLOOP!][!//

[!ENDNOCODE!][!//
#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

const Xbic_Ip_ConfigType Xbic_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{   /*Value of MCR registers to enable(bit1)/disable(bit0) for Slave port EDC Error Detection (SEn) and Master Port For Feedback Integrity Check (MEn)*/
[!NOCODE!]
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
[!CODE!]    {                /*            SE0 SE1 SE2 SE3 SE4 SE5 SE6 SE7 ME0 ME1 ME2 ME3 ME4 ME5 ME6 ME7      Reverse     */
        [!"num:inttohex($XBIC_0_MCR)"!]U   /* XBIC0_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_0_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
    }
[!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32z27') or contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32s27') or contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32e27')"!][!//
[!CODE!]    {                /*               SE0 SE1 SE2 SE3 SE4 SE5 SE6 SE7 ME0 ME1 ME2 ME3 ME4 ME5 ME6 ME7      Reverse     */
        [!"num:inttohex($XBIC_0_MCR)"!]U, /* XBIC_CE_MCR:  [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_0_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
        [!"num:inttohex($XBIC_1_MCR)"!]U  /* XBIC_SMU_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_1_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
    }
[!ENDCODE!]
    [!ELSE!]
[!CODE!]    {                /*            SE0 SE1 SE2 SE3 SE4 SE5 SE6 SE7 ME0 ME1 ME2 ME3 ME4 ME5 ME6 ME7      Reverse     */
        [!"num:inttohex($XBIC_0_MCR)"!]U, /* XBIC0_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_0_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
        [!"num:inttohex($XBIC_1_MCR)"!]U, /* XBIC1_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_1_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
        [!"num:inttohex($XBIC_2_MCR)"!]U, /* XBIC2_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_2_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
        [!"num:inttohex($XBIC_3_MCR)"!]U  /* XBIC3_MCR: [!"text:replaceAll( text:replaceAll( text:replaceAll( num:inttobin($XBIC_3_MCR,32), '0', '0   '), '1', '1   '), '   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   ', '   0000000000000000')"!]*/
    }
[!ENDCODE!]
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
};

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!ENDSELECT!][!//
[!ENDIF!]
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */


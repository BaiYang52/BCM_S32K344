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
*   @file Xrdc_Ip_PBcfg.c
*
*   @addtogroup XRDC_IP XRDC IPV Driver
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
#include "Xrdc_Ip_Types.h"
#include "Xrdc_Ip_Cfg.h"
#include "Xrdc_Ip_Device_Registers.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Xrdc_Ip_Types.h */
#if (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_XRDC_IP_TYPES_VENDOR_ID)
    #error "Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Types.h have different vendor ids"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_XRDC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_XRDC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_XRDC_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Types.h are different"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_XRDC_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_XRDC_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_XRDC_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Types.h are different"
#endif

/* Checks against Xrdc_Ip_Cfg.h */
#if (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_XRDC_IP_CFG_VENDOR_ID)
    #error "Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_XRDC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_XRDC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_XRDC_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Cfg.h are different"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_XRDC_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_XRDC_IP_CFG_SW_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_XRDC_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Cfg.h are different"
#endif

/* Checks against Xrdc_Ip_Device_Registers.h */
#if (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_XRDC_IP_DEVICE_REGISTERS_VENDOR_ID)
    #error "Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Device_Registers.h have different vendor ids"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_XRDC_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_XRDC_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_XRDC_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Device_Registers.h are different"
#endif
#if ((RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_XRDC_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_XRDC_IP_DEVICE_REGISTERS_SW_MINOR_VERSION) || \
     (RM_XRDC_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_XRDC_IP_DEVICE_REGISTERS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Xrdc_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip_Device_Registers.h are different"
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!IF "RmGeneral/RmEnableXRDCSupport"!][!//

[!VAR "MasterDescList"="''"!]
[!VAR "MasterList"="''"!]
[!VAR "MasterDescriptor" = "0"!][!// MasterDescriptor - to calculate automatic descriptor number of master.

[!MACRO "XrdcGenerateMasterDescriptor","CurrentMasterInstance"!][!//
[!NOCODE!][!//
[!VAR "MasterDescriptor" = "0"!]
[!IF "contains(text:split($MasterList, ','), $CurrentMasterInstance)"!]
    [!VAR "tempMasterIndex"="0"!]
    [!VAR "tempDesc"="''"!]
    [!LOOP "text:split($MasterList, ',')"!]
        [!IF ". = $CurrentMasterInstance"!]
            [!BREAK!]
    [!ENDIF!]
        [!VAR "tempMasterIndex"="$tempMasterIndex + 1"!]
    [!ENDLOOP!]
    [!VAR "tempIndex"="0"!]
    [!LOOP "text:split($MasterDescList, ',')"!]
        [!IF "$tempMasterIndex = $tempIndex"!]
            [!VAR "tempDesc"="concat($tempDesc, ',',num:i(num:i(.)+1))"!]
            [!VAR "MasterDescriptor" = "num:i(.)"!]
        [!ELSE!]
            [!VAR "tempDesc"="concat($tempDesc, ',',.)"!]
        [!ENDIF!]
        [!VAR "tempIndex"="$tempIndex + 1"!]
    [!ENDLOOP!]
    [!VAR "MasterDescList"="$tempDesc"!]
[!ELSE!]
    [!VAR "MasterList"="concat($MasterList, ',', $CurrentMasterInstance)"!]
    [!VAR "MasterDescList"="concat($MasterDescList, ',','1')"!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "XrdcGenerateConfigStructures"!][!//
[!VAR "NumberOfConfigs"="0"!][!//
[!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
[!VAR "MemConfigInstances" = "num:i(count(Rm_XRDC_Memory_Config/*[starts-with(./XrdcMrcInstance,$prefix)]))"!]
[!IF "$MemConfigInstances != 0"!][!//
[!VAR "NumberOfConfigs"="$NumberOfConfigs + $MemConfigInstances"!][!//
static const Xrdc_Ip_MemConfigType Xrdc_Memory_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$MemConfigInstances"!]] =
{
[!NOCODE!][!//
[!VAR "comma" = "num:i(1)"!][!//
[!VAR "DescList"="''"!]
[!VAR "MrcList"="''"!]
[!LOOP "Rm_XRDC_Memory_Config/*[starts-with(./XrdcMrcInstance,$prefix)]"!][!//
[!VAR "MrcDescriptor" = "0"!][!// MrcDescriptor - to calculate automatic descriptor number of mrc.
[!VAR "MrcCurrentIndex" = "./@index"!]
[!VAR "MrcName" = "./XrdcMrcInstance"!][!//
[!VAR "MrcInstance" = "ecu:get(text:replace('Rm.XRDC.Mrc.MrcName','MrcName',$MrcName))"!][!//
[!IF "contains(text:split($MrcList, ','), $MrcInstance)"!]
    [!VAR "tempMrcIndex"="0"!]
    [!VAR "tempDesc"="''"!]
    [!LOOP "text:split($MrcList, ',')"!]
        [!IF ". = $MrcInstance"!]
            [!BREAK!]
    [!ENDIF!]
        [!VAR "tempMrcIndex"="$tempMrcIndex + 1"!]
    [!ENDLOOP!]
    [!VAR "tempDescIndex"="0"!]
    [!LOOP "text:split($DescList, ',')"!]
        [!IF "$tempMrcIndex = $tempDescIndex"!]
            [!VAR "tempDesc"="concat($tempDesc, ',',num:i(num:i(.)+1))"!]
            [!VAR "MrcDescriptor" = "num:i(.)"!]
        [!ELSE!]
            [!VAR "tempDesc"="concat($tempDesc, ',',.)"!]
    [!ENDIF!]
        [!VAR "tempDescIndex"="$tempDescIndex + 1"!]
    [!ENDLOOP!]
    [!VAR "DescList"="$tempDesc"!]
[!ELSE!]
    [!VAR "MrcList"="concat($MrcList, ',', $MrcInstance)"!]
    [!VAR "DescList"="concat($DescList, ',','1')"!]
[!ENDIF!]
[!CODE!][!//
    {
[!ENDCODE!][!//
        [!CODE!][!WS "8"!]/*Select Memory region controller [!"./XrdcMrcInstance"!] will be used to control the address range below */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"text:replace(ecu:get(text:concat('Rm.XRDC.Mrc.', ./XrdcMrcInstance)),$prefix,'XRDC')"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*Each memory region controller include 16 descriptors to write configuration. If multiple descriptors are configured the policy will be ORed by all the descriptors */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!"num:i($MrcDescriptor)"!]UL,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*Start address for [!"./XrdcMrcInstance"!] controller and descriptor [!"num:i($MrcDescriptor)"!] */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!"num:inttohex(./XrdcStartAddress)"!]UL,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*End address for [!"./XrdcMrcInstance"!] controller and descriptor [!"num:i($MrcDescriptor)"!] */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!"num:inttohex(./XrdcEndAddress)"!]UL,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*Enable Semaphore would require the domain must own the gate to have access right for this memory range */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!IF "./XrdcSema4Enable = 'true' "!]XRDC_SEMA4_ENABLE,[!ELSE!]XRDC_SEMA4_DISABLE,[!ENDIF!][!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*Number of the gate of sema4 for granting access*/[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!IF "./XrdcSema4Enable = 'true' "!][!"node:value(node:ref(XrdcSema42LogicChannel)/Sema42HardwareChannel)"!][!ELSE!]0[!ENDIF!]UL,[!CR!][!ENDCODE!]

[!//Calculate access policy for current MCR instance and MCR descriptor.
        /*Policy value for Domain 0-7*/
        [!VAR "FinalPolicy0Value" = "num:i(0)"!]
        /*Policy value for Domain 8-15*/
        [!VAR "FinalPolicy1Value" = "num:i(0)"!]
        [!LOOP "./../../Rm_XRDC_Domain_Assignment/*"!]
            [!VAR "CurrentDomain" = "./DomainID"!]
            [!VAR "DomainIdValue" = "num:i(substring-after($CurrentDomain,'DOMAIN'))"!]
            [!IF "$DomainIdValue > num:i(7)"!]
                [!VAR "FinalPolicyValue" = "$FinalPolicy1Value"!]
                [!VAR "DomainIDnumber" = "$DomainIdValue - num:i(8)"!]
            [!ELSE!]
                [!VAR "FinalPolicyValue" = "$FinalPolicy0Value"!]
                [!VAR "DomainIDnumber" = "num:i($DomainIdValue)"!]
            [!ENDIF!]
            [!LOOP "./Domain_Memory_Assignment/*[(node:refvalid(./Domain_Memory_Assignment) and $MrcName = node:ref(./Domain_Memory_Assignment)/XrdcMrcInstance) and ($MrcCurrentIndex = node:ref(./Domain_Memory_Assignment)/@index)]"!]
                [!VAR "SecurePolicy"  = "./Secure_Access_Policy"!]
                [!VAR "NonSecurePolicy"  = "./Non_Secure_Access_Policy"!]
                [!IF "$SecurePolicy = 'XRDC_NO_ACCESS'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(0,num:i($DomainIDnumber*3)))"!]
                [!ELSEIF " $SecurePolicy = 'XRDC_READ_ONLY'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(1,num:i($DomainIDnumber*3)))"!]
                [!ELSEIF " $SecurePolicy = 'XRDC_SUPERVISOR_ACCESS_ONLY'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(2,num:i($DomainIDnumber*3)))"!]
                [!ELSEIF " $SecurePolicy = 'XRDC_FULL_ACCESS' "!]
                    [!IF " $NonSecurePolicy = 'XRDC_NO_ACCESS'"!]
                    [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(3,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $NonSecurePolicy = 'XRDC_SUPERVISOR_READ_ONLY'"!]
                    [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(4,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $NonSecurePolicy = 'XRDC_READ_ONLY'"!]
                    [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(5,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $NonSecurePolicy = 'XRDC_SUPERVISOR_ACCESS_ONLY'"!]
                    [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(6,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF "$NonSecurePolicy = 'XRDC_FULL_ACCESS'"!]
                    [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(7,num:i($DomainIDnumber*3)))"!]
                    [!ENDIF!]
                [!ENDIF!]
                [!IF "num:i($DomainIdValue) > num:i(7)"!]
                    [!VAR "FinalPolicy1Value" = "$FinalPolicyValue"!]
                [!ELSE!]
                    [!VAR "FinalPolicy0Value" = "$FinalPolicyValue"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDLOOP!]
[!//End Calculate
        [!CODE!][!WS "8"!]/*Access policy for Domains 0-7 is calculated automatically*/[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!"num:inttohex($FinalPolicy0Value)"!]UL,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/*Access policy for Domains 8-15 is calculated automatically*/[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](uint32)[!"num:inttohex($FinalPolicy1Value)"!]UL[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]}[!ENDCODE!][!IF "(num:i($comma))<(num:i($MemConfigInstances))"!][!CODE!],[!ENDCODE!][!ENDIF!][!VAR "comma"="$comma + 1"!][!CR!]
[!ENDLOOP!]
[!ENDNOCODE!][!//
};
[!ENDIF!][!//

[!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
[!VAR "PeripheralConfigInstances" = "num:i(count(Rm_XRDC_Peripheral_Config/*[starts-with(./XrdcPeripheralSlot,$prefix)]))"!][!//
[!IF "$PeripheralConfigInstances != 0"!][!//
[!VAR "NumberOfConfigs"="$NumberOfConfigs + $PeripheralConfigInstances"!][!//
static const Xrdc_Ip_PeripheralConfigType Xrdc_Peripheral_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"$PeripheralConfigInstances"!]] =
{
[!VAR "comma" = "num:i(1)"!][!//
[!LOOP "Rm_XRDC_Peripheral_Config/*[starts-with(./XrdcPeripheralSlot,$prefix)]"!][!//
[!VAR "CurrentSlot" = "./XrdcPeripheralSlot"!][!//
[!CODE!][!//
    {
        /*Peripheral slot number of a block of peripherals. Checking Reference manual for chip specific*/
        (uint32)[!"ecu:get(text:concat('Rm.XRDC.PdacSlot.', ./XrdcPeripheralSlot))"!]UL,
        /*Enable Semaphore would require the domain must own the gate to have access right for this peripheral slot */
        (uint32)[!IF "./XrdcSema4Enable = 'true' "!]XRDC_SEMA4_ENABLE,[!ELSE!]XRDC_SEMA4_DISABLE,[!ENDIF!]
        /*Number of the gate of sema4 for granting access*/
        (uint32)[!IF "./XrdcSema4Enable = 'true' "!][!"node:value(node:ref(XrdcSema42LogicChannel)/Sema42HardwareChannel)"!][!ELSE!]0[!ENDIF!]UL,
[!NOCODE!][!//
        [!VAR "FinalPolicy0Value" = "num:i(0)"!]
        [!VAR "FinalPolicy1Value" = "num:i(0)"!]
        [!LOOP "./../../Rm_XRDC_Domain_Assignment/*"!]
            [!VAR "CurrentDomain" = "./DomainID"!]
            [!VAR "DomainIdValue" = "num:i(substring-after($CurrentDomain,'DOMAIN'))"!]
            [!IF "$DomainIdValue > num:i(7)"!]
                [!VAR "FinalPolicyValue" = "$FinalPolicy1Value"!]
                [!VAR "DomainIDnumber" = "$DomainIdValue - num:i(8)"!]
            [!ELSE!]
                [!VAR "FinalPolicyValue" = "$FinalPolicy0Value"!]
                [!VAR "DomainIDnumber" = "num:i($DomainIdValue)"!]
            [!ENDIF!]
            [!LOOP "./Domain_Peripheral_Assignment/*"!]
                [!IF "node:refvalid(./Domain_Peripheral_Assignment) and $CurrentSlot = node:ref(./Domain_Peripheral_Assignment)/XrdcPeripheralSlot"!]
                    [!VAR "SecurePolicy"  = "./Secure_Access_Policy"!]
                    [!VAR "NonSecurePolicy"  = "./Non_Secure_Access_Policy"!]
                    [!IF "$SecurePolicy = 'XRDC_NO_ACCESS'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(0,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $SecurePolicy = 'XRDC_READ_ONLY'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(1,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $SecurePolicy = 'XRDC_SUPERVISOR_ACCESS_ONLY'"!][!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(2,num:i($DomainIDnumber*3)))"!]
                    [!ELSEIF " $SecurePolicy = 'XRDC_FULL_ACCESS' "!]
                        [!IF " $NonSecurePolicy = 'XRDC_NO_ACCESS'"!]
                        [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(3,num:i($DomainIDnumber*3)))"!]
                        [!ELSEIF " $NonSecurePolicy = 'XRDC_SUPERVISOR_READ_ONLY'"!]
                        [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(4,num:i($DomainIDnumber*3)))"!]
                        [!ELSEIF " $NonSecurePolicy = 'XRDC_READ_ONLY'"!]
                        [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(5,num:i($DomainIDnumber*3)))"!]
                        [!ELSEIF " $NonSecurePolicy = 'XRDC_SUPERVISOR_ACCESS_ONLY'"!]
                        [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(6,num:i($DomainIDnumber*3)))"!]
                        [!ELSEIF "$NonSecurePolicy = 'XRDC_FULL_ACCESS'"!]
                        [!VAR "FinalPolicyValue" = "bit:or($FinalPolicyValue,bit:shl(7,num:i($DomainIDnumber*3)))"!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
                [!IF "num:i($DomainIdValue) > num:i(7)"!]
                    [!VAR "FinalPolicy1Value" = "$FinalPolicyValue"!]
                [!ELSE!]
                    [!VAR "FinalPolicy0Value" = "$FinalPolicyValue"!]
                [!ENDIF!]
            [!ENDLOOP!]
        [!ENDLOOP!]
[!ENDNOCODE!][!//
        /*Access policy for Domains 0-7 is calculated automatically*/
        (uint32)[!"num:inttohex($FinalPolicy0Value)"!]UL,
        /*Access policy for Domains 8-15 is calculated automatically*/
        (uint32)[!"num:inttohex($FinalPolicy1Value)"!]UL
    }[!IF "(num:i($comma))<(num:i($PeripheralConfigInstances))"!],[!ENDIF!][!CR!][!VAR "comma"="$comma + 1"!][!//
[!ENDCODE!][!//
[!ENDLOOP!][!//
};
[!ENDIF!][!//

[!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
[!VAR "DomainConfigInstances" = "num:i(0)"!][!//
[!LOOP "Rm_XRDC_Domain_Assignment/*"!][!//
[!LOOP "Domain_Master_Assignment/*"!][!//
[!IF "starts-with(./XrdcMasterInstance, $prefix)"!][!//
[!VAR "DomainConfigInstances" = "num:i($DomainConfigInstances) + num:i(1)"!][!//
[!VAR "DomainConfigInstances" = "num:i($DomainConfigInstances)"!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!NOCODE!][!//
[!IF "$DomainConfigInstances != 0"!][!//
[!VAR "NumberOfConfigs"="$NumberOfConfigs + $DomainConfigInstances"!][!//
[!CODE!][!//
static const Xrdc_Ip_DomainConfigType Xrdc_Domain_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($DomainConfigInstances)"!]] =
{
[!ENDCODE!][!//
[!VAR "comma" = "num:i(1)"!][!//
[!LOOP "Rm_XRDC_Domain_Assignment/*/Domain_Master_Assignment/*[starts-with(./XrdcMasterInstance, $prefix)]"!][!//
    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Valid DomainID*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"../../DomainID"!],[!CR!][!ENDCODE!]
    [!VAR "MasterInstance" = "./XrdcMasterInstance"!][!//
    [!VAR "MasterInstanceName" = "text:replace('Rm.XRDC.Master.Instance.List','Instance',text:tolower(substring-after($InstanceParam, 'XRDC_')))"!][!//
    [!FOR "counter" = "0" TO "num:i(count(ecu:list($MasterInstanceName)))"!][!//
        [!IF "$MasterInstance = ecu:list($MasterInstanceName)[num:i($counter + 1)]"!][!//
            [!CODE!][!WS "8"!]/*Select the MDA register respect the Master instance */[!CR!][!ENDCODE!][!//
            [!CODE!][!WS "8"!][!"ecu:list(text:replace('Rm.XRDC.Master.Mdac.Instance.List','Instance',text:tolower(substring-after($InstanceParam, 'XRDC_'))))[$counter+1]"!],[!CR!][!ENDCODE!][!//
            [!BREAK!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!//
    [!CALL "XrdcGenerateMasterDescriptor", "CurrentMasterInstance" = "$MasterInstance"!][!//
    [!CODE!][!WS "8"!]/*Attribute to specfiy the type of current master which is core or non core master*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!IF "contains(ecu:list('Rm.XRDC.CoreMaster.List'),./XrdcMasterInstance)"!]XRDC_CORE_MASTER[!ELSE!]XRDC_NONCORE_MASTER[!ENDIF!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*PID field only has meaning if the core is core master type and PID mode enable*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"./XrdcMasterPID"!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*PID mask*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"./XrdcMasterPIDMask"!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*PID mode enable - only valid if the master instance is core type*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"./XrdcPIDEnable"!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Three State Model to be done later if architecture support*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]0UL,[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Selection of descriptor for current master core. If master is non-core master this field should be omitted*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]XRDC_WORD[!"num:i($MasterDescriptor)"!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Domain ID bypass. If master is core master this field should be omitted*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!IF "./XrdcDIDBDisable = 'true' "!]XRDC_MDA_DID_BYPASS_NOT_USED,[!ELSE!]XRDC_MDA_DID_BYPASS_USED,[!ENDIF!][!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Domain Secure Mode*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"./XrdcMasterMode"!],[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!]/*Domain Priviledged Mode*/[!CR!][!ENDCODE!]
    [!CODE!][!WS "8"!][!"./XrdcMasterPriviledgeMode"!][!CR!][!ENDCODE!]
    [!CODE!][!WS "4"!]}[!IF "(num:i($comma))<($DomainConfigInstances)"!],[!CR!][!ENDIF!][!VAR "comma"="$comma + 1"!][!ENDCODE!]
[!ENDLOOP!][!//
[!CODE!]
};
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

[!IF "$NumberOfConfigs > 0"!][!//
[!IF "var:defined('postBuildVariant')"!][!//
[!VAR "ConfigName"="concat('Xrdc_Config_',$InstanceParam,'_',$postBuildVariant)"!][!//
[!ELSE!][!//
[!VAR "ConfigName"="concat('Xrdc_Config_',$InstanceParam)"!][!//
[!ENDIF!][!//
const Xrdc_Ip_InstanceConfigType [!"$ConfigName"!] =
{
    [!"$InstanceParam"!],
    [!IF "$DomainConfigInstances!=0"!]Xrdc_Domain_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!]NULL_PTR[!ENDIF!],
    (uint32)[!IF "$DomainConfigInstances!=0"!][!"$DomainConfigInstances"!]UL[!ELSE!]0UL[!ENDIF!],
    [!IF "$MemConfigInstances!=0"!]Xrdc_Memory_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!]NULL_PTR[!ENDIF!],
    (uint32)[!IF "$MemConfigInstances!=0"!][!"$MemConfigInstances"!]UL[!ELSE!]0UL[!ENDIF!],
    [!IF "$PeripheralConfigInstances!=0"!]Xrdc_Peripheral_Config_[!"$InstanceParam"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!ELSE!]NULL_PTR[!ENDIF!],
    (uint32)[!IF "$PeripheralConfigInstances!=0"!][!"$PeripheralConfigInstances"!]UL[!ELSE!]0UL[!ENDIF!]
};
[!VAR "GeneratedXrdcConfigs"="concat($GeneratedXrdcConfigs,',&',$ConfigName)"!]
[!VAR "GeneratedXrdcConfigsCount"="$GeneratedXrdcConfigsCount+1"!][!//
[!ENDIF!][!//
[!ENDMACRO!][!// [!MACRO "XrdcGenerateConfigStructures"!]
[!ENDIF!][!//

[!MACRO "XrdcGenerateInUsed"!][!//
[!NOCODE!][!//
[!VAR "InstanceString"="''"!][!//
[!VAR "InstanceCount"="0"!][!//
[!VAR "TotalInstanceCount"="count(ecu:list('Rm.XRDC.XRDCInstances.List'))"!][!//
[!LOOP "Rm_XRDC_Memory_Config/*"!][!//
    [!FOR "i" = "1" TO "$TotalInstanceCount"!][!//
    [!VAR "InstanceParam"="ecu:list('Rm.XRDC.XRDCInstances.List')[position() = $i]"!][!//
    [!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
    [!IF "starts-with(./XrdcMrcInstance, $prefix)"!][!//
        [!IF "$InstanceString =''"!][!//
            [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
            [!VAR "InstanceString" = "concat($InstanceString,$InstanceParam)"!][!//
        [!ELSE!][!//
            [!IF "contains($InstanceString, $InstanceParam) != 'true'"!][!//
                [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
                [!VAR "InstanceString" = "concat($InstanceString,', ',$InstanceParam)"!][!//
            [!ENDIF!]
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!ENDFOR!][!//
[!ENDLOOP!][!//
[!LOOP "Rm_XRDC_Peripheral_Config/*"!][!//
    [!FOR "i" = "1" TO "$TotalInstanceCount"!][!//
    [!VAR "InstanceParam"="ecu:list('Rm.XRDC.XRDCInstances.List')[position() = $i]"!][!//
    [!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
    [!IF "starts-with(./XrdcPeripheralSlot, $prefix)"!][!//
        [!IF "$InstanceString =''"!][!//
            [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
            [!VAR "InstanceString" = "concat($InstanceString,$InstanceParam)"!][!//
        [!ELSE!][!//
            [!IF "contains($InstanceString, $InstanceParam) != 'true'"!][!//
                [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
                [!VAR "InstanceString" = "concat($InstanceString,', ',$InstanceParam)"!][!//
            [!ENDIF!]
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!ENDFOR!][!//
[!ENDLOOP!][!//
[!LOOP "Rm_XRDC_Domain_Assignment/*"!][!//
[!LOOP "Domain_Master_Assignment/*"!][!//
    [!FOR "i" = "1" TO "$TotalInstanceCount"!][!//
    [!VAR "InstanceParam"="ecu:list('Rm.XRDC.XRDCInstances.List')[position() = $i]"!][!//
    [!VAR "prefix" = "ecu:get(text:concat('Rm.XRDC.Master.Prefix.',$InstanceParam))"!][!//
    [!IF "starts-with(./XrdcMasterInstance, $prefix)"!][!//
        [!IF "$InstanceString =''"!][!//
            [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
            [!VAR "InstanceString" = "concat($InstanceString,$InstanceParam)"!][!//
        [!ELSE!][!//
            [!IF "contains($InstanceString, $InstanceParam) != 'true'"!][!//
                [!VAR "InstanceCount"= "$InstanceCount + 1"!][!//
                [!VAR "InstanceString" = "concat($InstanceString,', ',$InstanceParam)"!][!//
            [!ENDIF!]
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!ENDFOR!][!//
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//

static const uint32 Xrdc_Instances_InUsed[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($InstanceCount)"!]] =
{
    [!"$InstanceString"!]
};
[!ENDMACRO!][!//
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                      BUFFER DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                   STATE STRUCTURE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
[!IF "RmGeneral/RmEnableXRDCSupport"!][!//

#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

[!SELECT "RmConfigSet"!][!//
[!CALL "XrdcGenerateInUsed"!][!//
[!VAR "GeneratedXrdcConfigs"="'DUMMY'"!][!//
[!VAR "GeneratedXrdcConfigsCount"="0"!][!//
[!FOR "i" = "1" TO "$InstanceCount"!][!//
[!VAR "InstanceParam"="normalize-space(text:split($InstanceString,',')[position() = $i])"!][!//
/* XRDC instance: [!"$InstanceParam"!] */[!//
[!CALL "XrdcGenerateConfigStructures"!][!//
[!ENDFOR!][!//
[!VAR "NumMaster" = "num:i(0)"!][!//
[!LOOP "Rm_XRDC_Domain_Assignment/*"!][!//
[!VAR "NumMaster"  = "num:i(count(Domain_Master_Assignment/*)) + $NumMaster"!][!//
[!ENDLOOP!][!//
const Xrdc_Ip_InstanceConfigType * const aXrdc_Config_Array[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($GeneratedXrdcConfigsCount)"!]] = {[!"text:replace($GeneratedXrdcConfigs,'DUMMY,', '')"!]};

const Xrdc_Ip_ConfigType Xrdc_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    aXrdc_Config_Array[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
    (uint32)[!"num:i($GeneratedXrdcConfigsCount)"!]UL,
    Xrdc_Instances_InUsed[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
    (uint32)[!"num:i($InstanceCount)"!]UL
};
[!ENDSELECT!][!// [!SELECT "RmConfigSet"!]
#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!ENDIF!][!//


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


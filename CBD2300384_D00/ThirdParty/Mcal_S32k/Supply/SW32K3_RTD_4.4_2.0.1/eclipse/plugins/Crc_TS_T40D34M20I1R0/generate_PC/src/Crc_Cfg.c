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
*   @file       Crc_Cfg.c
*   @implements Crc_Cfg.c_Artifact
*   @version    2.0.1
*
*   @brief      AUTOSAR Crc Configuration file for the driver.
*   @details    Precompile parameters and extern configuration.
*
*   @addtogroup CRC_CFG
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Crc_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRC_CFG_VENDOR_ID_C                      43
#define CRC_CFG_AR_RELEASE_MAJOR_VERSION_C       4
#define CRC_CFG_AR_RELEASE_MINOR_VERSION_C       4
#define CRC_CFG_AR_RELEASE_REVISION_VERSION_C    0
#define CRC_CFG_SW_MAJOR_VERSION_C               2
#define CRC_CFG_SW_MINOR_VERSION_C               0
#define CRC_CFG_SW_PATCH_VERSION_C               1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crc_Cfg.c and Crc_Cfg.h are of the same Autosar version */
#if (CRC_CFG_VENDOR_ID_C != CRC_CFG_VENDOR_ID)
    #error "Crc_Cfg.c and Crc_Cfg.h have different vendor ids"
#endif

/* Check if Crc_Cfg.c and CRC.h are of the same Autosar version */
#if ((CRC_CFG_AR_RELEASE_MAJOR_VERSION_C    != CRC_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_CFG_AR_RELEASE_MINOR_VERSION_C    != CRC_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CRC_CFG_AR_RELEASE_REVISION_VERSION_C != CRC_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crc_Cfg.c and Crc_Cfg.h are different"
#endif

/* Check if Crc_Cfg.c and CRC.h are of the same Software version */
#if ((CRC_CFG_SW_MAJOR_VERSION_C != CRC_CFG_SW_MAJOR_VERSION) || \
     (CRC_CFG_SW_MINOR_VERSION_C != CRC_CFG_SW_MINOR_VERSION) || \
     (CRC_CFG_SW_PATCH_VERSION_C != CRC_CFG_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crc_Cfg.c and Crc_Cfg.h are different"
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
                                       GLOBAL VARIABLES
==================================================================================================*/
[!NOCODE!]
[!VAR "EcucCorePartitionMax" = "num:i(count(as:modconf('EcuC')[1]/EcucHardware/*[1]/*[1]/*) - 1)"!]
[!VAR "AutosarCrcLibraryIsAvailable" = "'false'"!]
[!/* Check Any Channel Enable Autosar Library */!]
[!IF "(./CrcGeneral/Crc8Mode) or (./CrcGeneral/Crc8H2FMode) or (./CrcGeneral/Crc16Mode) or (./CrcGeneral/Crc16ARCMode) or (./CrcGeneral/Crc32Mode) or (./CrcGeneral/Crc32P4Mode) or (./CrcGeneral/Crc64Mode)"!]
    [!VAR "AutosarCrcLibraryIsAvailable" = "'true'"!]
[!ENDIF!]

[!IF "(./CrcGeneral/CrcMultiCoreEnable = 'true') and (ecu:get('Crc.Hardware.NumOfCoreSupport') > 1)"!]
    [!FOR "CoreID" = "0" TO "$EcucCorePartitionMax"!]
        [!VAR "Channel8BitSaeJ1850"   = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel8BitH2f"        = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel16BitCcitt"     = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel16BitArc"       = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel32BitEthernet"  = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel32BitE2ep4"     = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!VAR "Channel64BitEcma"      = "'CRC_IP_CHANNEL_INVALID_U32'"!]
        [!LOOP "CrcChannelConfig/*"!]
            [!IF "node:exists(./CrcPartitionRefOfChannel)"!][!//
                [!VAR "CrcLogicChannelName"    = "node:value(./CrcLogicChannelName)"!]
                [!VAR "CrcAutosarSelect"       = "node:value(./CrcAutosarSelect)"!]
                [!VAR "CrcChannelPartitionRef" = "node:value(./CrcPartitionRefOfChannel)"!]
                [!VAR "CrcChannelProtocolType" = "node:value(./CrcProtocolInfo/CrcProtocolType)"!]

                [!IF "$AutosarCrcLibraryIsAvailable = 'true'"!]
                    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]​
                        [!IF "$CrcChannelPartitionRef = node:value(./OsAppEcucPartitionRef)"!]​
                            [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                                [!SELECT "node:ref(./OsApplicationCoreRef)"!]​
                                    [!IF "$CoreID = node:value(./EcucCoreId)"!]​
                                        [!IF "($CrcAutosarSelect = 'AUTOSAR_CRC_8') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_8BIT_SAE_J1850')"!]
                                            [!VAR "Channel8BitSaeJ1850"  = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_8H2F') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_8BIT_H2F')"!]
                                            [!VAR "Channel8BitH2f"        = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_16') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_16BIT_CCITT_FALSE')"!]
                                            [!VAR "Channel16BitCcitt"     = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_16ARC') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_16BIT_ARC')"!]
                                            [!VAR "Channel16BitArc"       = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_32') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_32BIT_ETHERNET')"!]
                                            [!VAR "Channel32BitEthernet"  = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_32P4') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_32BIT_E2E_P4')"!]
                                            [!VAR "Channel32BitE2ep4"     = "$CrcLogicChannelName"!]
                                        [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_64') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_64BIT_ECMA')"!]
                                            [!VAR "Channel64BitEcma"           = "$CrcLogicChannelName"!]
                                        [!ENDIF!]
                                    [!ENDIF!]
                                [!ENDSELECT!]
                            [!ELSE!]
                                [!ERROR!]
                                [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores support, OsAppEcucPartitionRef and OsApplicationCoreRef of the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now: [!CR!]    + "OsAppEcucPartitionRef"([!"text:split($CrcChannelPartitionRef, '/')[last()]"!]) was configured [!CR!]    + The corresponding "OsApplicationCoreRef" was not configured.
                                [!CR!]2. How to resolve this error?
                                - Step 1: Go to Os component
                                - Step 2: Select "OsApplication"
                                - Step 3: Select configuration for [!"text:split($CrcChannelPartitionRef, '/')[last()]"!]
                                - Step 4: Configure "OsApplicationCoreRef"
                                [!ENDERROR!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDLOOP!]
                [!ENDIF!]
            [!ELSE!]
                [!ERROR!]
                    [!CR!] Error when generate core for ([!"./CrcLogicChannelName"!])
                    [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, you have to configure "Partition Ref Of Channel" Node.
                    [!CR!]2. How to resolve this error?
                    - Step 1: Go to Os/Ecuc component.
                    - Step 2: Configure at least one of Partition and the corresponding Core for it.
                    - Step 3: Go to Crc component. Select "Crc Channels Configuration" tag.
                    - Step 4: Select configuration for [!"./CrcLogicChannelName"!].
                    - Step 5: Enable "Partition Ref Of Channel" Node.
                    - Step 6: Configure "Partition Ref Of Channel" Node.
                [!ENDERROR!]
            [!ENDIF!]
        [!ENDLOOP!]

        [!IF "var:defined('AutosarChannel8BitSaeJ1850')"!]
            [!VAR "AutosarChannel8BitSaeJ1850" = "concat($AutosarChannel8BitSaeJ1850, ', ', $Channel8BitSaeJ1850)"!]
        [!ELSE!]
            [!VAR "AutosarChannel8BitSaeJ1850" = "$Channel8BitSaeJ1850"!]
        [!ENDIF!]

        [!IF "var:defined('AutosarChannel8BitH2f')"!]
            [!VAR "AutosarChannel8BitH2f" = "concat($AutosarChannel8BitH2f, ', ', $Channel8BitH2f)"!]
        [!ELSE!]
            [!VAR "AutosarChannel8BitH2f" = "$Channel8BitH2f"!]
        [!ENDIF!]

        [!IF "var:defined('AutosarChannel16BitCcittFalse')"!]
            [!VAR "AutosarChannel16BitCcittFalse" = "concat($AutosarChannel16BitCcittFalse, ', ', $Channel16BitCcitt)"!]
        [!ELSE!]
            [!VAR "AutosarChannel16BitCcittFalse" = "$Channel16BitCcitt"!]
        [!ENDIF!]

        [!IF "var:defined('AutosarChannel16BitArc')"!]
            [!VAR "AutosarChannel16BitArc" = "concat($AutosarChannel16BitArc, ', ', $Channel16BitArc)"!]
        [!ELSE!]
            [!VAR "AutosarChannel16BitArc" = "$Channel16BitArc"!]
        [!ENDIF!]

        [!IF "var:defined('AutosarChannel32BitEthernet')"!]
            [!VAR "AutosarChannel32BitEthernet" = "concat($AutosarChannel32BitEthernet, ', ', $Channel32BitEthernet)"!]
        [!ELSE!]
            [!VAR "AutosarChannel32BitEthernet" = "$Channel32BitEthernet"!]
        [!ENDIF!]

        [!IF "var:defined('AutosarChannel32BitE2ep4')"!]
            [!VAR "AutosarChannel32BitE2ep4" = "concat($AutosarChannel32BitE2ep4, ', ', $Channel32BitE2ep4)"!]
        [!ELSE!]
            [!VAR "AutosarChannel32BitE2ep4" = "$Channel32BitE2ep4"!]
        [!ENDIF!]
        [!IF "var:defined('AutosarChannel64BitEcma')"!]
            [!VAR "AutosarChannel64BitEcma" = "concat($AutosarChannel64BitEcma, ', ', $Channel64BitEcma)"!]
        [!ELSE!]
            [!VAR "AutosarChannel64BitEcma" = "$Channel64BitEcma"!]
        [!ENDIF!]
    [!ENDFOR!]

[!ELSE!] CrcMultiCoreEnable = false
    [!VAR "AutosarChannel8BitSaeJ1850"    = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel8BitH2f"          = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel16BitCcittFalse" = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel16BitArc"         = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel32BitEthernet"    = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel32BitE2ep4"       = "'CRC_IP_CHANNEL_INVALID_U32'"!]
    [!VAR "AutosarChannel64BitEcma"             = "'CRC_IP_CHANNEL_INVALID_U32'"!]

    [!LOOP "CrcChannelConfig/*"!]
        [!VAR "CrcLogicChannelName"    = "node:value(./CrcLogicChannelName)"!]
        [!VAR "CrcAutosarSelect"       = "node:value(./CrcAutosarSelect)"!]
        [!VAR "CrcChannelProtocolType" = "node:value(./CrcProtocolInfo/CrcProtocolType)"!]

        [!IF "$AutosarCrcLibraryIsAvailable = 'true'"!]
            [!IF "($CrcAutosarSelect = 'AUTOSAR_CRC_8') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_8BIT_SAE_J1850')"!]
                [!VAR "AutosarChannel8BitSaeJ1850"    = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_8H2F') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_8BIT_H2F')"!]
                [!VAR "AutosarChannel8BitH2f"          = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_16') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_16BIT_CCITT_FALSE')"!]
                [!VAR "AutosarChannel16BitCcittFalse" = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_16ARC') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_16BIT_ARC')"!]
                [!VAR "AutosarChannel16BitArc"         = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_32') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_32BIT_ETHERNET')"!]
                [!VAR "AutosarChannel32BitEthernet"    = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_32P4') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_32BIT_E2E_P4')"!]
                [!VAR "AutosarChannel32BitE2ep4"       = "$CrcLogicChannelName"!]
            [!ELSEIF "($CrcAutosarSelect = 'AUTOSAR_CRC_64') and ($CrcChannelProtocolType = 'CRC_PROTOCOL_64BIT_ECMA')"!]
                [!VAR "AutosarChannel64BitEcma"             = "$CrcLogicChannelName"!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDNOCODE!][!//

#define CRC_START_SEC_CONST_32
#include "Crc_MemMap.h"

/* CRC Autosar Channel */
const uint32 Crc_au32Crc8BitSaeJ1850[]     = {[!"$AutosarChannel8BitSaeJ1850"!]};
const uint32 Crc_au32Crc8BitH2f[]          = {[!"$AutosarChannel8BitH2f"!]};
const uint32 Crc_au32Crc16BitCcittFalse[]  = {[!"$AutosarChannel16BitCcittFalse"!]};
const uint32 Crc_au32Crc16BitArc[]         = {[!"$AutosarChannel16BitArc"!]};
const uint32 Crc_au32Crc32BitEthernet[]    = {[!"$AutosarChannel32BitEthernet"!]};
const uint32 Crc_au32Crc32BitE2ep4[]       = {[!"$AutosarChannel32BitE2ep4"!]};
const uint32 Crc_au32Crc64BitEcma[]        = {[!"$AutosarChannel64BitEcma"!]};

#define CRC_STOP_SEC_CONST_32
#include "Crc_MemMap.h"


#define CRC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"
[!IF "(./CrcGeneral/CrcMultiCoreEnable = 'true') and (ecu:get('Crc.Hardware.NumOfCoreSupport') > 1)"!][!//
[!NOCODE!]
    [!LOOP "CrcEcucPartitionRefArray/*/CrcEcucPartitionRef"!]
        [!VAR "CrcChannelPartitionRef" = "node:current()"!]
        [!VAR "PartitionShortName" = "substring-after(substring-after($CrcChannelPartitionRef, node:name(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1])), '/')"!]
        [!VAR "LogicChannelNames"!][!ENDVAR!]
        [!LOOP "../../../CrcChannelConfig/*"!]
            [!IF "./CrcPartitionRefOfChannel = $CrcChannelPartitionRef"!]
                [!VAR "LogicChannelNames" = "concat($LogicChannelNames, ./CrcLogicChannelName, ' ')"!]
            [!ENDIF!]
        [!ENDLOOP!]
        [!VAR "ChannelLength" = "num:i(count(text:split($LogicChannelNames, ' ')))"!]
        [!IF "$ChannelLength > '0'"!]
            [!VAR "ChannelID" = "num:i(0)"!]
[!CODE!]

/**
 * @brief  Crc IP Logic Channel of [!"$PartitionShortName"!]
 * */
const uint32 Crc_ChannelOf[!"$PartitionShortName"!][] =
{
[!LOOP "text:split($LogicChannelNames, ' ')"!][!//
    [!"."!][!IF "$ChannelID < $ChannelLength - 1"!],[!ENDIF!]
[!VAR "ChannelID"="num:i($ChannelID + 1)"!][!//
[!ENDLOOP!][!//
};

/**
 * @brief  Crc IP Logic Channel of [!"$PartitionShortName"!]
 * */
const Crc_PartitionType Crc_Config[!"$PartitionShortName"!] =
{
    /* uint32 ChannelMax    */ [!"$ChannelLength"!]U,
    /* uint32 * ChannelList */ &Crc_ChannelOf[!"$PartitionShortName"!][0]
};

[!ENDCODE!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDNOCODE!][!//

/**
 * @brief  Crc Partition Configuration
 * */
const Crc_PartitionType * const Crc_pxPartitionConfig[] =
{
[!FOR "CoreID" = "0" TO "$EcucCorePartitionMax"!][!//
[!NOCODE!]
    [!LOOP "CrcEcucPartitionRefArray/*/CrcEcucPartitionRef"!]
        [!VAR "PartitionPointer" = "'NULL_PTR'"!]
        [!VAR "PartitionRef" = "."!]
        [!VAR "PartitionShortName" = "substring-after(substring-after(., node:name(as:modconf('EcuC')[1]/EcucPartitionCollection/*[1])), '/')"!]

        [!// Find EcucCoreID follow PartitionRef of Channel
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]​
            [!IF "$PartitionRef = node:value(./OsAppEcucPartitionRef)"!]​
                [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!][!// Refer to EcucCoreDefinition Node​
                        [!IF "$CoreID = node:value(./EcucCoreId)"!]
                            [!VAR "PartitionPointer" = "concat('&Crc_Config', $PartitionShortName)"!]
                            [!BREAK!]
                        [!ENDIF!]
                    [!ENDSELECT!]
                [!ELSE!]
                    [!ERROR!]
                    [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores support, OsAppEcucPartitionRef and OsApplicationCoreRef of the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now: [!CR!]    + "OsAppEcucPartitionRef"([!"text:split($CrcChannelPartitionRef, '/')[last()]"!]) was configured [!CR!]    + The corresponding "OsApplicationCoreRef" was not configured.
                    [!CR!]2. How to resolve this error?
                    - Step 1: Go to Os component
                    - Step 2: Select "OsApplication"
                    - Step 3: Select configuration for [!"text:split($CrcChannelPartitionRef, '/')[last()]"!]
                    - Step 4: Configure "OsApplicationCoreRef"
                    [!ENDERROR!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDLOOP!]

        [!IF "$PartitionPointer != 'NULL_PTR'"!]
            [!BREAK!]
        [!ENDIF!]

    [!ENDLOOP!]
[!ENDNOCODE!][!//
    [!"$PartitionPointer"!][!IF "$CoreID < $EcucCorePartitionMax"!],[!ENDIF!]
[!ENDFOR!][!//
};
[!//
[!ELSE!][!// Multicore Disable
[!VAR "ChannelMax"   = "num:i(count(CrcChannelConfig/*))"!][!//
[!VAR "ChannelIndex" = "num:i(0)"!][!//
/**
 * @brief  Crc IP Logic Channel of Partition
 * */
static const uint32 Crc_ChannelOfPartition[] =
{
[!LOOP "CrcChannelConfig/*"!][!//
    [!"./CrcLogicChannelName"!][!IF "$ChannelIndex < $ChannelMax - 1"!],[!ENDIF!]
[!VAR "ChannelIndex"="num:i($ChannelIndex + 1)"!][!//
[!ENDLOOP!][!//
};

/**
 * @brief  Crc Partition Configuration
 * */
const Crc_PartitionType Crc_xConfigPartition =
{
    /* uint32 ChannelMax    */ [!"$ChannelMax"!]U,
    /* uint32 * ChannelList */ &Crc_ChannelOfPartition[0]
};
[!ENDIF!][!// End of Multicore Option

/**
 * @brief  Configure CRC driver initialization
 * */
const Crc_InitType Crc_xConfigInit =
{
    &CrcIp_xConfigInit
};

#define CRC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

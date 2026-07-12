[!NOCODE!][!//
/**
    @file    Trgmux_Ip_RegOperations.m
    @version 2.0.1

    @brief   AUTOSAR Mcl - Register operations.
    @details Register operations.

    Project RTD AUTOSAR 4.4
    Platform CORTEXM
    Peripheral DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
    Dependencies none

    ARVersion 4.4.0
    ARRevision ASR_REL_4_4_REV_0000
    ARConfVariant
    SWVersion 2.0.1
    BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

    (c) Copyright 2020 - 2022 NXP Semiconductors
    All Rights Reserved.
 */
[!//
[!MACRO "Trgmux_Ip_GetMac_NrOfLogicGroups"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicGroups" = "0"!][!//
[!LOOP "./MclConfig/trgmuxLogicGroup/*"!][!//
[!VAR "Mac_NrOfLogicGroups" = "num:i($Mac_NrOfLogicGroups + 1)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_NrOfLogicGroups)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!//
[!//
[!//
[!MACRO "Trgmux_Ip_GetNrOfLogicTriggers"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicTriggers" = "0"!][!//
[!LOOP "./MclConfig/trgmuxLogicGroup/*"!][!//
[!LOOP "./trgmuxLogicTrigger/*"!][!//
[!VAR "Mac_NrOfLogicTriggers" = "num:i($Mac_NrOfLogicTriggers + 1)"!][!//
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_NrOfLogicTriggers)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!//
[!//
[!//
[!MACRO "Trgmux_Ip_LogicTriggerConfig","Type"!][!//
[!NOCODE!][!//
[!VAR "Mac_IsMulticoreEnable"="num:i(0)"!][!//
[!IF "node:value(MclGeneral/MclEnableMulticoreSupport)='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1)"!][!//
[!VAR "Mac_IsMulticoreEnable" = "num:i(1)"!][!//
[!VAR "Mac_TotalNumberOfPartition"="num:i(0)"!][!//
[!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!][!//
[!VAR "Mac_TotalNumberOfPartition"="num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!CODE!]
/* TRGMUX Logic Trigger Configuration */
[!VAR "Mac_NrOfLogicTriggers" = "0"!][!//
[!VAR "Mac_LogicGroupLock" = "0"!][!//
[!LOOP "./MclConfig/trgmuxLogicGroup/*"!][!//
[!VAR "Mac_LogicGroupLock" = "node:value(trgmuxLogicGroup_Lock)"!][!//
[!LOOP "./trgmuxLogicTrigger/*"!][!//
[!VAR "Mac_NrOfLogicTriggers" = "num:i($Mac_NrOfLogicTriggers + 1)"!][!//
const Trgmux_Ip_LogicTriggerType Trgmux_Ip_LogicTrigger[!"num:i($Mac_NrOfLogicTriggers - 1)"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* uint8 LogicChannel; */         [!"node:value(trgmuxLogicTrigger_Name)"!],
    /* uint8 Output; */               [!"node:value(trgmuxLogicTrigger_Output)"!],
    /* uint8 Input; */                [!"node:value(trgmuxLogicTrigger_Input)"!],
    /* uint8 HwInstId; */             TRGMUX_IP_HW_INST_0,
    /* boolean Lock; */               (boolean)[!"text:toupper($Mac_LogicGroupLock)"!],
[!NOCODE!][!//
[!IF "$Mac_IsMulticoreEnable = num:i(1)"!][!//
    [!IF "node:exists(trgmuxLogicTrigger_EcucPartitionRef)"!][!//
        [!VAR "Mac_ReturnValue1" = "num:i(0)"!]
        [!VAR "Mac_ReturnValue2" = "num:i(0)"!]
        [!VAR "Mac_PartitionRef" = "node:value(trgmuxLogicTrigger_EcucPartitionRef)"!][!//
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                [!IF "$Mac_PartitionRef = node:value(./OsAppEcucPartitionRef)"!][!//
                    [!IF "node:refvalid(./OsApplicationCoreRef)"!]
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                            [!VAR "Mac_CoreUsed" ="node:value(./EcucCoreId)"!][!//
                        [!ENDSELECT!][!//
                        [!VAR "Mac_ReturnValue1" = "num:i($Mac_ReturnValue1 + 1)"!][!//
                    [!ELSE!]
                        [!VAR "Mac_ReturnValue2" = "num:i($Mac_ReturnValue2 + 1)"!][!//
                        [!ERROR!]
                            [!CR!] Error when generate core reference for Logic Channel
                            [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, OsAppEcucPartitionRef and OsApplicationCoreRef of the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now:OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!]) was added correctly. But OsApplicationCoreRef was not configured.
                            [!CR!]2. How to resolve this error?
                            - Step 1: Go to Os component
                            - Step 2: Select OsApplication
                            - Step 3: Configure OsApplicationCoreRef for OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!])
                        [!ENDERROR!]
                    [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "($Mac_ReturnValue1 = 0) and ($Mac_ReturnValue2 = 0)"!]
            [!ERROR!]
            [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores support, OsAppEcucPartitionRef and OsApplicationCoreRef of the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now: Both of OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!]) and OsApplicationCoreRef were not configured.
            [!CR!]2. How to resolve this error?
            - Step 1: Go to Os component
            - Step 2: Select OsApplication
            - Step 3: Create a new OsApplication configuration (by select + button)
            - Step 4: Configure OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!]) and OsApplicationCoreRef for the above OsApplication Configuration
            [!ENDERROR!]
        [!ENDIF!]
    [!ELSE!]
        [!ERROR!]
        [!CR!] Error when generate core reference for Logic Channel
        [!CR!]1. Why you got this error? [!CR!]-> When enable multi cores feature, users need to configure 'Ecuc Partition Ref' for corresponding Logic Channel. [!CR!]For now: 'Ecuc Partition Ref' Node of [!"node:value(./trgmuxLogicTrigger_Name)"!] was not configured.
        [!CR!]2. How to resolve this error?
        - Step 1: Go to 'Trgmux Logic Trigger'
        - Step 2: Select configuration has Logic Channel Name is '[!"node:value(./trgmuxLogicTrigger_Name)"!]'
        - Step 3: Enable 'Ecuc Partition Ref' node and Select the ECUC Partition Reference
        [!ENDERROR!]
    [!ENDIF!][!//
    [!IF "$Mac_CoreUsed != 255"!][!//
        [!CODE!]    /* uint8 CoreId; */               [!"num:i($Mac_CoreUsed)"!]U,[!CR!][!ENDCODE!]
    [!ELSE!]
        [!CODE!]    /* uint8 CoreId; */               0xFFU,[!CR!][!ENDCODE!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
};
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//
[!//
[!//
[!//
[!MACRO "Trgmux_Ip_Init","Type"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicTriggers"!][!CALL "Trgmux_Ip_GetNrOfLogicTriggers"!][!ENDVAR!][!//
[!CODE!]
/* TRGMUX Initialization Structure */
const Trgmux_Ip_InitType Trgmux_Ip_xTrgmuxInit[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!][!//
    [!IF "$Mac_NrOfLogicTriggers > num:i(0)"!][!//
[!CODE!][!//
    /* Trgmux_Ip_LogicTriggerType * paxLogicTrigger[]  */
    {
[!VAR "Mac_LogicTriggerIndex" = "0"!][!//
[!LOOP "./MclConfig/trgmuxLogicGroup/*"!][!//
[!LOOP "./trgmuxLogicTrigger/*"!][!//
[!VAR "Mac_LogicTriggerIndex" = "num:i($Mac_LogicTriggerIndex + 1)"!][!//
        &Trgmux_Ip_LogicTrigger[!"num:i($Mac_LogicTriggerIndex - 1)"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
[!ENDLOOP!][!//
    },
[!ENDCODE!][!//
    [!ELSE!][!//
[!CODE!][!//
    /* Trgmux_Ip_LogicTriggerType * paxLogicTrigger[]  */                       NULL_PTR,
[!ENDCODE!][!//
    [!ENDIF!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!ENDNOCODE!]

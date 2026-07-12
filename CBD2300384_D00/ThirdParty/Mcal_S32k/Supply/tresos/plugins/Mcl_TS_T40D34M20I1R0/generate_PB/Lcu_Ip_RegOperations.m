[!NOCODE!]
/**
    @file    Lcu_Ip_RegOperations.m
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

[!MACRO "lcuMac_PartitionRef"!][!//
[!NOCODE!][!//
[!VAR "Mac_CoreUsed" ="num:i(255)"!][!//
[!VAR "Mac_ReturnValue1" = "num:i(0)"!]
[!VAR "Mac_ReturnValue2" = "num:i(0)"!]
[!IF "node:exists(MclGeneral/MclEcucPartitionRef)"!][!//
    [!VAR "Mac_PartitionRef" = "node:value(MclGeneral/MclEcucPartitionRef)"!][!//
    [!IF "node:exists(as:modconf('Os')[1]/OsApplication)"!][!//
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
                        [!CR!] Error when generate core reference for MclEcucPartitionRef
                        [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, OsAppEcucPartitionRef and OsApplicationCoreRef of the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now:OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!]) was added correctly. But OsApplicationCoreRef was not configured.
                        [!CR!]2. How to resolve this error?
                        - Step 1: Go to Os component
                        - Step 2: Select OsApplication
                        - Step 3: Configure OsApplicationCoreRef for OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!])
                        [!ENDERROR!]
                    [!ENDIF!]
                [!ENDIF!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "($Mac_ReturnValue1 = 0) and ($Mac_ReturnValue2 = 0)"!]
            [!ERROR!]
            [!CR!] Error when generate core reference for MclEcucPartitionRef
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
        [!CR!] Error when generate core reference for MclEcucPartitionRef
        [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, the OsApplication must be added and configured synchronization with configuration of EcuC component.[!CR!]For now Os application is not added
        [!CR!]2. How to resolve this error?
        - Step 1: Add Os component
        - Step 2: Select OsApplication
        - Step 3: Create a new OsApplication configuration (by select + button)
        - Step 4: Configure OsAppEcucPartitionRef ([!"text:split($Mac_PartitionRef, '/')[last()]"!]) and OsApplicationCoreRef for the above OsApplication Configuration
        [!ENDERROR!]
    [!ENDIF!]
[!ELSE!]
    [!ERROR!]
    [!CR!] Error when generate core reference for Mcl Initialization Multicore Configuration
    [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, users need to configure 'Ecuc Partition Ref' for corresponding Mcl Initialization Multicore Configuration.
    [!CR!]2. How to resolve this error?
    - Step 1: Go to 'Mcl General Configuration'
    - Step 2: Enable 'Mcl Initialization Multicore Configuration' node and Select the ECUC Partition Reference
    [!ENDERROR!]
[!ENDIF!][!//
[!CODE!][!//
/* uint8 LcuCoreRef                                                         */ [!"$Mac_CoreUsed"!]U,
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "Lcu_Ip_GetNrOfLogicInstances"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicInstances" = "0"!][!//
[!LOOP "./MclConfig/lcuConfiguration/*/lcuInstanceCfg/*"!][!//
[!VAR "Mac_NrOfLogicInstances" = "num:i($Mac_NrOfLogicInstances + 1)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_NrOfLogicInstances)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!MACRO "Lcu_Ip_GetNrOfLogicInputs"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicInputs" = "0"!][!//
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuInputConfiguration/*"!][!//
[!VAR "Mac_NrOfLogicInputs" = "num:i($Mac_NrOfLogicInputs + 1)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_NrOfLogicInputs)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "Lcu_Ip_GetNrOfLogicOutputs"!][!//
[!NOCODE!][!//
[!VAR "Mac_NrOfLogicOutputs" = "0"!][!//
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!][!//
[!VAR "Mac_NrOfLogicOutputs" = "num:i($Mac_NrOfLogicOutputs + 1)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_NrOfLogicOutputs)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!// Generate configuration for Instance
[!MACRO "Lcu_Ip_Logic_Instance_Config","Type"!][!//
[!NOCODE!][!//
[!CODE!][!//

/* LCU Logic Instance Configuration */
[!LOOP "./MclConfig/lcuConfiguration/*/lcuInstanceCfg/*"!][!//
[!IF "lcuLogicCell_UsingForceSignal='true' or lcuLogicCell_UsingSyncSignal='true' or (../../../../../MclGeneral/MclEnableMulticoreSupport='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1))"!][!//
[!VAR "Mac_NrOfLogicCellCfg" = "0"!][!//
[!LOOP "lcuLogicCellCfg_lst/*"!][!//
[!VAR "Mac_CoreUsed" ="num:i(255)"!][!//
[!VAR "Mac_NrOfLogicCellCfg" = "num:i($Mac_NrOfLogicCellCfg + 1)"!][!//
/* LCU Logic Cell [!"@index"!] Configuration of Logic Instance [!"../../@index"!] */
const Lcu_Ip_LogicCellType LogicCell[!"@index"!]LogicInst[!"substring-after(node:value(./../../lcuLogicInstance_LogicName), 'LCU_LOGIC_INSTANCE_')"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* uint8 HwLcId         */ [!"node:value(lcuLogicCell_HwLcID)"!],
[!IF "../../lcuLogicCell_UsingForceSignal='true' or ../../lcuLogicCell_UsingSyncSignal='true'"!][!//
    /* uint8 ForceFilter    */ [!"node:value(lcuLogicCell_forceFilter)"!]U,
    /* uint8 ForcePolarity  */ [!IF "lcuLogicCell_usingForcePolarity='true'"!][!CALL "lcuLogicCell_ForcePolarity"!][!ELSE!]0[!ENDIF!]U,
    /* uint8 CombEn         */ [!IF "lcuLogicCell_usingCombinationalForcePath='true'"!][!CALL "lcuLogicCell_CFPEnable"!][!ELSE!]0[!ENDIF!]U,
    /* uint8 SwSyncSel      */ [!"node:value(lcuLogicCell_SwSyncSel)"!],
[!ELSE!][!//
    /* uint8 ForceFilter    */ 0U,
    /* uint8 ForcePolarity  */ 0U,
    /* uint8 CombEn         */ 0U,
    /* uint8 SwSyncSel      */ LCU_IP_SYNC_SEL_INPUT0,
[!ENDIF!][!//
[!NOCODE!][!//
[!IF "../../../../../../../MclGeneral/MclEnableMulticoreSupport='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1)"!][!//
    [!IF "node:exists(lcuLogicCell_EcucPartitionRef)"!][!//
        [!VAR "Mac_ReturnValue1" = "num:i(0)"!]
        [!VAR "Mac_ReturnValue2" = "num:i(0)"!]
        [!VAR "Mac_PartitionRef" = "node:value(lcuLogicCell_EcucPartitionRef)"!][!//
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
                            [!CR!] Error when generate core reference for Logic Cell
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
            [!CR!] Error when generate core reference for Logic Cell
            [!CR!]1.Why you got this error? [!CR!]-> When enable multi cores feature, users need to configure 'Ecuc Partition Ref' for corresponding Logic Cell. [!CR!]For now: 'Ecuc Partition Ref' Node of [!"node:value(./lcuLogicCell_HwLcID)"!] - [!"node:value(./../../lcuLogicInstance_LogicName)"!] was not configured.
            [!CR!]2. How to resolve this error?
            - Step 1: Go to 'Lcu Logic Instance'
            - Step 2: Select configuration has Logic Instance Name is '[!"node:value(./../../lcuLogicInstance_LogicName)"!]'
            - Step 3: Select 'Logic Cell Configuration List'
            - Step 4: Select configuration has Hardware Logic Cell is '[!"node:value(./lcuLogicCell_HwLcID)"!]'
            - Step 5: Enable 'Ecuc Partition Ref' node and Select the ECUC Partition Reference
            [!ENDERROR!]
    [!ENDIF!][!//
[!CODE!][!//
    /* uint8 CoreRef        */ [!"$Mac_CoreUsed"!]U,
[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
};
[!ENDLOOP!][!//

/* LCU Logic Cell Array Configuration of Logic Instance [!"@index"!] */
const Lcu_Ip_LogicCellType * const Lcu_Ip_ppxLogicCellLogicInst[!"@index"!]ConfigArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][LCU_LC_COUNT] =
{
[!LOOP "lcuLogicCellCfg_lst/*"!][!//
    &LogicCell[!"@index"!]LogicInst[!"substring-after(node:value(./../../lcuLogicInstance_LogicName), 'LCU_LOGIC_INSTANCE_')"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
};
[!ENDIF!][!//
/* LCU Logic Instance [!"@index"!] Configuration */
const Lcu_Ip_LogicInstanceType LcuLogicInstance[!"@index"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* uint8 HwInstId             */ [!"node:value(lcuLogicInstance_HwInstID)"!],
[!IF "lcuLogicCell_UsingForceSignal='true' or lcuLogicCell_UsingSyncSignal='true' or (../../../../../MclGeneral/MclEnableMulticoreSupport='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1))"!][!//
    /* uint8 NumLogicCellConfig   */ [!"num:i($Mac_NrOfLogicCellCfg)"!]U,
    /* ppxLogicCellConfigArray    */ &Lcu_Ip_ppxLogicCellLogicInst[!"@index"!]ConfigArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
[!ELSE!][!//
    /* uint8 NumLogicCellConfig   */ 0U,
    /* ppxLogicCellConfigArray    */ NULL_PTR,
[!ENDIF!][!//
[!IF "node:value(./lcuLogicInstance_OperationMode) = 'INTERRUPT'"!][!//
    /* uint8 OperationMode        */ LCU_IP_INTERRUPT_MODE,
[!ELSE!][!//
    /* uint8 OperationMode        */ LCU_IP_POLLING_MODE,
[!ENDIF!][!//

};

[!ENDLOOP!][!//
/* LCU Logic Instance Array Configuration */
const Lcu_Ip_LogicInstanceType * const Lcu_Ip_ppxLogicInstanceArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][LCU_IP_NOF_CFG_LOGIC_INSTANCES] =
{
[!LOOP "./MclConfig/lcuConfiguration/*/lcuInstanceCfg/*"!][!//
    &LcuLogicInstance[!"@index"!]Config[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
};
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Convert value about ForcePolarity
[!MACRO "lcuLogicCell_ForcePolarity"!][!//
[!NOCODE!][!//
[!VAR "Mac_ForcePolarityValue" = "0"!][!//
[!LOOP "lcuLogicCell_ForceSignalCfg/*"!][!//
[!IF "node:value(./lcuLogicCell_forcePol) = 'true'"!]
[!VAR "Mac_forcePol" = "1"!][!//
[!ELSE!]
[!VAR "Mac_forcePol" = "0"!][!//
[!ENDIF!]
[!VAR "Mac_ForceSignalIndex" = "substring-after(node:value(lcuLogicCell_ForceSignalSelect), 'LCU_IP_FORCE_SEL_INPUT')"!]
[!IF "$Mac_ForceSignalIndex = 0"!]
[!VAR "Mac_ConvertValue" = "1"!]
[!ELSE!]
[!VAR "Mac_ConvertValue" = "num:i($Mac_ForceSignalIndex * 2)"!]
[!ENDIF!]
[!VAR "Mac_ForcePolarityValue" = "num:i($Mac_ForcePolarityValue + $Mac_ConvertValue * $Mac_forcePol)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_ForcePolarityValue)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Convert value about CombEn
[!MACRO "lcuLogicCell_CFPEnable"!][!//
[!NOCODE!][!//
[!VAR "Mac_CombineEnable" = "0"!][!//
[!LOOP "lcuLogicCell_ForceSignalCfg/*"!][!//
[!IF "node:value(./lcuLogicCell_combEnable) = 'true'"!]
[!VAR "Mac_CombEnable" = "1"!][!//
[!ELSE!]
[!VAR "Mac_CombEnable" = "0"!][!//
[!ENDIF!]
[!VAR "Mac_ForceSignalIndex" = "substring-after(node:value(lcuLogicCell_ForceSignalSelect), 'LCU_IP_FORCE_SEL_INPUT')"!]
[!IF "$Mac_ForceSignalIndex = 0"!]
[!VAR "Mac_ConvertValue" = "1"!]
[!ELSE!]
[!VAR "Mac_ConvertValue" = "num:i($Mac_ForceSignalIndex * 2)"!]
[!ENDIF!]
[!VAR "Mac_CombineEnable" = "num:i($Mac_CombineEnable + $Mac_ConvertValue * $Mac_CombEnable)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_CombineEnable)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Generate configuration for Input
[!MACRO "Lcu_Ip_Logic_Input_Config","Type"!][!//
[!NOCODE!][!//
[!CODE!][!//
/* LCU Logic Input Configuration */
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuInputConfiguration/*"!][!//
const Lcu_Ip_LogicInputConfigType LogicInput[!"@index"!]Cfg[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* uint8 MuxSel         */ [!"node:value(lcuLogicInput_MuxSelect)"!],
    /* boolean SwSynMode    */ [!IF "node:value(lcuLogicInput_UsingSwOverride) = 'true'"!][!"node:value(lcuLogicInput_SwOverrideMode)"!][!ELSE!]LCU_IP_SW_SYNC_IMMEDIATE[!ENDIF!],
    /* uint8 SwValue        */ [!IF "node:value(lcuLogicInput_UsingSwOverride) = 'true'"!][!"node:value(lcuLogicInput_SwOverrideValue)"!][!ELSE!]LCU_IP_SW_OVERRIDE_LOGIC_LOW[!ENDIF!],
};

const Lcu_Ip_LogicInputType LogicInput[!"@index"!][!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    {
        /* uint8 HwInstId      */ [!"node:value(lcuLogicInput_HwInstID)"!],
        /* uint8 HwLcInputId   */ [!CALL "lcu_convert_input", "LC" = "node:value(lcuLogicInput_HwLcID)", "Input" = "node:value(lcuLogicInput_HwInputID)"!]U,
    },
    &LogicInput[!"@index"!]Cfg[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
};
[!ENDLOOP!][!//

const Lcu_Ip_LogicInputType * const Lcu_Ip_ppxLogicInputArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][LCU_IP_NOF_CFG_LOGIC_INPUTS] =
{
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuInputConfiguration/*"!][!//
    &LogicInput[!"@index"!][!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
};[!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Convert Hwinput to HwLcInputId
[!MACRO "lcu_convert_input","LC", "Input"!][!//
[!NOCODE!][!//
[!CODE!][!//
[!VAR "Mac_numIOPerLC" = "num:i(count(ecu:list('Mcl.MclConfigSet.LcuInput')))"!][!//
[!VAR "Mac_LcIndex" = "num:i(substring-after($LC, 'LCU_IP_HW_LC_'))"!][!//
[!VAR "Mac_IoIndex" = "num:i(substring-after($Input, 'LCU_IP_HW_INPUT_'))"!][!//
[!VAR "Mac_newIO" = "num:i($Mac_LcIndex * $Mac_numIOPerLC + $Mac_IoIndex)"!][!//
[!CODE!][!"num:i($Mac_newIO)"!][!ENDCODE!][!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Convert Hwoutput to HwLcOutputId
[!MACRO "lcu_convert_Output","LC", "Output"!][!//
[!NOCODE!][!//
[!CODE!][!//
[!VAR "Mac_numIOPerLC" = "num:i(count(ecu:list('Mcl.MclConfigSet.LcuInput')))"!][!//
[!VAR "Mac_LcIndex" = "num:i(substring-after($LC, 'LCU_IP_HW_LC_'))"!][!//
[!VAR "Mac_IoIndex" = "num:i(substring-after($Output, 'LCU_IP_HW_OUTPUT_'))"!][!//
[!VAR "Mac_newIO" = "num:i($Mac_LcIndex * $Mac_numIOPerLC + $Mac_IoIndex)"!][!//
[!CODE!][!"num:i($Mac_newIO)"!][!ENDCODE!][!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Generate configuration for Output
[!MACRO "Lcu_Ip_Logic_Output_Config","Type"!][!//
[!NOCODE!][!//
[!CODE!][!//
#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref LCU_IP_PBCFG_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

/* LCU Hardware output state */

#define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED
/* @violates @ref LCU_IP_PBCFG_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!][!//

Lcu_Ip_HwOutputStateType HwOutput[!"@index"!]State[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!];
[!ENDLOOP!][!//

#define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* @violates @ref Mcl_Dma_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"


#define MCL_START_SEC_VAR_INIT_UNSPECIFIED
/* @violates @ref LCU_IP_PBCFG_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

Lcu_Ip_HwOutputStateType * Lcu_Ip_ppxHwOutputStateArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][LCU_IP_NOF_CFG_LOGIC_OUTPUTS] =
{
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!][!//
    &HwOutput[!"@index"!]State[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
};

#define MCL_STOP_SEC_VAR_INIT_UNSPECIFIED
/* @violates @ref Mcl_Dma_h_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"


#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref LCU_IP_PBCFG_C_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

/* LCU Logic Output Configuration */
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!][!//
const Lcu_Ip_LogicOutputConfigType LogicOutput[!"@index"!]Cfg[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* boolean EnDebugMode   */ (boolean)[!"text:toupper(node:value(lcuLogicOutput_DebugMode))"!],
    /* uint16 LutControl     */ [!"node:value(lcuLogicOutput_LutControl)"!]U,
    /* uint16 LutRiseFilt    */ [!"node:value(lcuLogicOutput_LutRiseFilter)"!]U,
    /* uint16 LutFallFilt    */ [!"node:value(lcuLogicOutput_LutFallFilter)"!]U,
    /* boolean EnLutDma      */ (boolean)[!"text:toupper(node:value(lcuLogicOutput_LutDmaEnable))"!],
    /* boolean EnForceDma    */ (boolean)[!IF "node:value(lcuLogicOutput_UsingForceSignal) = 'true'"!][!"text:toupper(node:value(lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceDmaEnable))"!][!ELSE!]FALSE[!ENDIF!],
[!IF "(var:set('HwInsId',./lcuLogicOutput_HwInstID) and ./../../lcuInstanceCfg/*[lcuLogicInstance_HwInstID=$HwInsId]/lcuLogicInstance_OperationMode = 'INTERRUPT')"!][!//
    /* boolean EnLutInt      */ (boolean)[!"text:toupper(node:value(lcuLogicOutput_LutIntEnable))"!],
    /* boolean EnForceInt    */ (boolean)[!IF "node:value(lcuLogicOutput_UsingForceSignal) = 'true'"!][!"text:toupper(node:value(lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceIntEnable))"!][!ELSE!]FALSE[!ENDIF!],
[!ELSE!][!//
    /* boolean EnLutInt      */ (boolean)FALSE,
    /* boolean EnForceInt    */ (boolean)FALSE,
[!ENDIF!][!//
    /* boolean InvertOutput  */ (boolean)[!"text:toupper(node:value(lcuLogicOutput_InvertOutput))"!],
    /* uint8 ForceSignalSel  */ [!IF "node:value(lcuLogicOutput_UsingForceSignal) = 'true'"!][!CALL "logicOutputForceSignal"!][!ELSE!]0[!ENDIF!]U,
    /* uint8 ClearForceMode  */ [!IF "node:value(lcuLogicOutput_UsingForceSignal) = 'true'"!][!"node:value(lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceClearMode)"!][!ELSE!]LCU_IP_CLEAR_FORCE_SIGNAL_IMMEDIATE[!ENDIF!],
    /* uint8 ForceSyncSel    */ [!IF "node:value(lcuLogicOutput_UsingForceSignal) = 'true' and (node:value(lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceClearMode) = 'LCU_IP_CLEAR_FORCE_SIGNAL_AFTER_CLEAR_STATUS')"!][!"node:value(lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceSyncSelect)"!]
    [!ELSE!]LCU_IP_SYNC_SEL_INPUT0[!ENDIF!],
};

const Lcu_Ip_LogicOutputType LogicOutput[!"@index"!][!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    {
        /* uint8 HwInstId               */ [!"node:value(lcuLogicOutput_HwInstID)"!],
        /* uint8 HwLcOutputId           */ [!CALL "lcu_convert_Output", "LC" = "node:value(lcuLogicOutput_HwLcID)", "Output" = "node:value(lcuLogicOutput_HwOutputID)"!]U,
[!IF "(var:set('HwInsId',./lcuLogicOutput_HwInstID) and ./../../lcuInstanceCfg/*[lcuLogicInstance_HwInstID=$HwInsId]/lcuLogicInstance_OperationMode = 'INTERRUPT')"!][!//
        /* Lcu_Ip_Callback IntCallback  */ [!"node:value(lcuLogicOutput_InterruptCallback)"!],
[!ELSE!][!//
        /* Lcu_Ip_Callback IntCallback  */ NULL_PTR,
[!ENDIF!][!//

    },
    &LogicOutput[!"@index"!]Cfg[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
};
[!ENDLOOP!][!//

const Lcu_Ip_LogicOutputType * const Lcu_Ip_ppxLogicOutputArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][LCU_IP_NOF_CFG_LOGIC_OUTPUTS] =
{
[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!][!//
    &LogicOutput[!"@index"!][!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ENDLOOP!][!//
};[!//
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!// Caculate forse signal select
[!MACRO "logicOutputForceSignal"!][!//
[!NOCODE!][!//
[!VAR "Mac_ForceSigSelValue" = "0"!][!//
[!LOOP "lcuLogicOutput_ForceSignalConfiguration/lcuLogicOutput_ForceSignalSelect/*"!][!//
[!VAR "Mac_ForceSignalIndex" = "substring-after(node:value(lcuLogicOutput_ForceSignal), 'LCU_IP_FORCE_SEL_INPUT')"!]
[!IF "node:value(lcuLogicOutput_ForceSignalAffect) = 'true'"!]
[!VAR "ForceAffectValue" = "1"!][!//
[!ELSE!]
[!VAR "ForceAffectValue" = "0"!][!//
[!ENDIF!]

[!IF "$Mac_ForceSignalIndex = 0"!]
[!VAR "Mac_ConvertValue" = "1"!]
[!ELSE!]
[!VAR "Mac_ConvertValue" = "num:i($Mac_ForceSignalIndex * 2)"!]
[!ENDIF!]
[!VAR "Mac_ForceSigSelValue" = "num:i($Mac_ForceSigSelValue + $Mac_ConvertValue * $ForceAffectValue)"!][!//
[!ENDLOOP!][!//
[!CODE!][!"num:i($Mac_ForceSigSelValue)"!][!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!MACRO "Lcu_Ip_Init_Type_Config","Type"!][!//
[!NOCODE!][!//
[!CODE!][!//

/* LCU InitType Configuration */
const Lcu_Ip_InitType Lcu_Ip_xLcuInit[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /* Lcu_Ip_HwOutputStateType ** ppxHwOutputStateArray                        */ &Lcu_Ip_ppxHwOutputStateArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
    /* const Lcu_Ip_LogicInstanceType * const * ppxLogicInstanceConfigArray     */ &Lcu_Ip_ppxLogicInstanceArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
    /* const Lcu_Ip_LogicOutputConfigType * const pxLogicOutputResetConfigArray */ &Lcu_Ip_xLogicOutputResetConfig,
    /* const Lcu_Ip_LogicInputConfigType * const pxLogicInputResetConfigArray   */ &Lcu_Ip_xLogicInputResetConfig,
    /* const Lcu_Ip_LogicOutputType * const * ppxLogicOutputConfigArray         */ &Lcu_Ip_ppxLogicOutputArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
    /* const Lcu_Ip_LogicInputType * const * ppxLogicInputConfigArray           */ &Lcu_Ip_ppxLogicInputArray[!"$Type"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
[!IF "node:value(./MclGeneral/MclEnableMulticoreSupport)='true' and (num:i(ecu:get('Mcl.MultiCore.Support')) = 1)"!][!//
    [!CALL "lcuMac_PartitionRef"!]
[!ENDIF!][!//
};
[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!ENDNOCODE!]

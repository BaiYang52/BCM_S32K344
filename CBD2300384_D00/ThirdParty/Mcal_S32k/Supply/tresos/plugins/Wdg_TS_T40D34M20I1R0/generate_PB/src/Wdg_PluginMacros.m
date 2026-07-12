[!NOCODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Swt
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
*   @file
*
*   @addtogroup Wdg
*   @{
*/
[!// Constant that defines the maximum number of Wdg instances
[!VAR "WdgMaxNumberOfInstances"="num:i(ecu:get('Wdg.Num.Of.Instances'))"!]
[!// MACRO for checking that the Gpt callback has been set
[!MACRO "WdgCheckCbkNotification","WdgNo","WdgName"!]
[!NOCODE!]
    [!IF "node:exists(node:ref(node:value(concat($WdgNo,'/WdgSettingsConfig/','WdgExternalTriggerCounterRef')))/GptNotification)"!]
        [!IF "node:ref(node:value(concat($WdgNo,'/WdgSettingsConfig/','WdgExternalTriggerCounterRef')))/GptNotification != concat('Wdg_Cbk_GptNotification',$WdgName)"!]
            [!ERROR!] The notification handler for WDG[!"$WdgName"!] must be Wdg_Cbk_GptNotification[!"$WdgName"!][!ENDERROR!]
        [!ENDIF!]
    [!ELSE!]
        [!ERROR!] The channel notification for WDG[!"$WdgName"!] must be set up and the notification handler must be Wdg_Cbk_GptNotification[!"$WdgName"!][!ENDERROR!]
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]
[!// MACRO for defining core used
[!MACRO "WdgCoreConfig","WdgNo"!][!//
[!NOCODE!]
[!IF "node:exists($WdgNo)"!][!//
    [!IF "node:value(concat($WdgNo,'/WdgGeneral/WdgEnableMultiCoreSupport'))"!][!//
        [!VAR "coreused" = "65535"!][!//
        [!VAR "WdgCrtPart" = "node:value(concat($WdgNo,'/WdgGeneral/WdgEcucPartitionRef'))"!][!//
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
            [!IF "$WdgCrtPart = node:value(./OsAppEcucPartitionRef)"!][!//​
                [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//​
                    [!VAR "coreused" ="node:value(./EcucCoreId)"!][!//​
                [!ENDSELECT!][!//
            [!ENDIF!][!//​
        [!ENDLOOP!][!//
        [!IF "$coreused = 65535"!][!//
            [!ERROR!][!//
                "Os appication is not configured for [!"text:split($WdgCrtPart,'/')[5]"!]"[!//
            [!ENDERROR!][!//
        [!ELSE!][!CODE!][!"$coreused"!][!ENDCODE!][!ENDIF!][!//
    [!ELSE!]
        [!CODE!]0xFFFF[!ENDCODE!]
    [!ENDIF!][!//
[!ELSE!]
    [!CODE!]0xFFFF[!ENDCODE!]
[!ENDIF!][!//
[!ENDNOCODE!]
[!ENDMACRO!]
[!// MACRO for defining mode configuration
[!MACRO "WdgModeConfig","WdgMode","WdgNo"!][!//
    (uint8)           (SWT_IP_ALL_MAP_DISABLE[!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster0'))"!] | SWT_IP_MAP0_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster1'))"!] | SWT_IP_MAP1_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster2'))"!] | SWT_IP_MAP2_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster3'))"!] | SWT_IP_MAP3_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster4'))"!] | SWT_IP_MAP4_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster5'))"!] | SWT_IP_MAP5_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster6'))"!] | SWT_IP_MAP6_ENABLE[!ENDIF!][!/*
            */!][!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgMasterAccessProtectionforMaster7'))"!] | SWT_IP_MAP7_ENABLE[!ENDIF!]),[!/*
            */!]  /* u8MapEnBitmask */
    (boolean)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgResetOnInvalidAccess')) = 'BusError'"!]FALSE[!ELSE!]TRUE[!ENDIF!],         /* bEnResetOnInvalidAccess */
#if (defined(SWT_IP_HAS_STOP_MODE) && (SWT_IP_HAS_STOP_MODE == 1U))
    (boolean)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgRunsInStopMode'))"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* bEnRunInStopMode */
#endif 
    (boolean)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgRunsInDebugMode'))"!]TRUE[!ELSE!]FALSE[!ENDIF!],         /* bEnRunInDebugMode */
    (boolean)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowMode'))"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* bEnWindow */
    (boolean)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgOperationMode')) = 'Interrupt'"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* bEnInterrupt */
    [!IF "(node:exists(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode'))) "!][!/*
    */!](Swt_Ip_ServiceModeType)     [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode')) = 'FixedService'"!]SWT_IP_FS_SEQ_MODE[!/*
                      */!][!ELSEIF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode')) = 'KeyedService'"!]SWT_IP_KS_SEQ_MODE[!/*
                      */!][!ELSEIF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode')) = 'FixedAddress'"!]SWT_IP_FA_EXE_MODE[!/*
                      */!][!ELSEIF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode')) = 'Incremental'"!]SWT_IP_IA_EXE_MODE[!ENDIF!],        /* eServiceMode */[!/*                  
    */!][!ELSE!]
    (Swt_Ip_ServiceModeType)         [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgKeyedService'))"!]TRUE[!ELSE!]FALSE[!ENDIF!],        /* eServiceMode */
    [!ENDIF!] 
    (uint32)          [!"num:inttohex(num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgTimeoutPeriod')) * node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgClockValue')) * 1000),8)"!],      /* u32TimeoutValue */
    (uint32)          [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowMode'))"!][!/*
            */!][!"num:inttohex(num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowPeriod'))* node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgClockValue')) * 1000),8)"!][!/*
            */!][!ELSE!]0x0[!ENDIF!],          /* u32WindowValue */  
    (uint16)          [!IF "((node:exists(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode'))) and (node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceMode')) = 'FixedAddress')) or                      ((node:exists(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgKeyedService'))) and (node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgKeyedService'))))"!][!/*
            */!][!"num:inttohex(num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgServiceKeyValue'))),4)"!][!ELSE!][!"num:inttohex(0,4)"!][!ENDIF!],          /* u16InitialKey */
    (Swt_Ip_LockType)[!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgSoftLockConfiguration'))"!] SWT_IP_SOFTLOCK[!/*
            */!][!ELSEIF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgHardLockConfiguration'))"!] SWT_IP_HARDLOCK[!/*
            */!][!ELSE!] SWT_IP_UNLOCK[!ENDIF!],     /* lockConfig */
[!ENDMACRO!]
[!ENDNOCODE!]
[!// MACRO for calculating the trigger period
[!MACRO "WdgTriggerPeriod","WdgMode","WdgNo"!][!//
[!NOCODE!]
    [!VAR "WdgTimeoutVal" = "num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgTimeoutPeriod')) * node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgClockValue')) * 1000)"!]
    [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowMode'))"!][!/*
                  */!][!VAR "WdgWindowVal" = "num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowPeriod'))* node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgClockValue')) * 1000)"!][!/*
            */!][!ELSE!][!VAR "WdgWindowVal" = "0"!][!ENDIF!]
    [!VAR "WdgInternalClockVal" = "num:i(node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgClockValue')))"!]
    [!VAR "WdgTriggerSourceClock" = "num:i(node:value(node:ref(node:value(concat($WdgNo,'/WdgSettingsConfig/','WdgExternalTriggerCounterRef')))/GptChannelTickFrequency)div 1000)"!]
[!CODE!][!//
    (uint32) [!IF "node:value(concat($WdgNo,'/WdgSettingsConfig/',$WdgMode,'/WdgWindowMode'))"!][!/*
            */!][!"num:inttohex(num:i((($WdgTimeoutVal - ($WdgWindowVal div 2)) div $WdgInternalClockVal) * $WdgTriggerSourceClock))"!][!/*
            */!][!ELSE!][!"num:inttohex(num:i((($WdgTimeoutVal - ($WdgTimeoutVal div 2)) div $WdgInternalClockVal) * $WdgTriggerSourceClock))"!][!ENDIF!],
[!ENDCODE!]
[!ENDNOCODE!]
[!ENDMACRO!]
[!NOCODE!]
[!AUTOSPACING!]
/**
  @file    Icu_PluginMacros.m
  @version 2.0.1

  @brief   AUTOSAR Icu - plugin check.
  @details Version checks.

  Project RTD AUTOSAR 4.4
  Patform CORTEXM
  Peripheral Emios Siul2 Wkpu LpCmp
  Dependencies none

  ARVersion 4.4.0
  ARRevision ASR_REL_4_4_REV_0000
  ARConfVariant
  SWVersion 2.0.1
  BuildVersion S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707

  (c) Copyright 2020 - 2022 NXP Semiconductors
  All Rights Reserved.
*/
[!// MACRO for generating the notification function prototype
[!MACRO "ICU_GENERATE_NOTIFICATION_FUNCTIONS_PROTOTYPE"!][!//
[!NOCODE!]
    [!VAR "MacOuterLoopCounter" = "0"!]
    [!VAR "MacInnerLoopCounter" = "0"!]
    [!VAR "Found" = "'false'"!]
    [!SELECT "IcuConfigSet"!]
     [!LOOP "./IcuChannel/*"!]
        [!VAR "a" = "IcuMeasurementMode"!]
        [!IF "(contains($a,'ICU_MODE_TIMESTAMP') and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL_PTR'))"!]
            [!VAR "MacOuterLoopCounter" = "$MacOuterLoopCounter + 1"!]
            [!VAR "MacInnerLoopCounter" = "0"!]
            [!VAR "MacMatchcounter" = "0"!]
            [!VAR "MacNotification" = "IcuTimestampMeasurement/IcuTimestampNotification"!]
            [!LOOP "../../../*/IcuChannel/*"!] [!//All notifications from all configurations
                [!VAR "a" = "IcuMeasurementMode"!]
                [!IF "(contains($a,'ICU_MODE_TIMESTAMP') and (IcuTimestampMeasurement/IcuTimestampNotification != 'NULL_PTR'))"!]
                    [!VAR "MacInnerLoopCounter" = "$MacInnerLoopCounter + 1"!]
                    [!IF "$MacOuterLoopCounter >= $MacInnerLoopCounter"!]
                        [!IF "($MacNotification = IcuTimestampMeasurement/IcuTimestampNotification)"!]
                             [!VAR "MacMatchcounter" = "$MacMatchcounter + 1"!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$MacMatchcounter = 1"!]
                  [!IF "$Found = 'false'"!]
                      [!VAR "Found" = "'true'"!]
[!CODE!][!//
/**
* @brief External Notifications for Timestamp
*
*/
[!ENDCODE!]
                  [!ENDIF!]
[!CODE!][!//
extern void [!"IcuTimestampMeasurement/IcuTimestampNotification"!](void);
[!ENDCODE!]
            [!ENDIF!]
        [!ENDIF!]
     [!ENDLOOP!]
    [!ENDSELECT!]

    [!VAR "MacOuterLoopCounter" = "0"!]
    [!VAR "MacInnerLoopCounter" = "0"!]
    [!VAR "Found" = "'false'"!]
    [!SELECT "IcuConfigSet"!]
     [!LOOP "./IcuChannel/*"!]
        [!VAR "a" = "IcuMeasurementMode"!]
        [!IF "(contains($a,'ICU_MODE_SIGNAL_EDGE_DETECT') and node:exists(IcuSignalEdgeDetection/IcuSignalNotification))"!]
          [!IF "IcuSignalEdgeDetection/IcuSignalNotification != 'NULL_PTR'"!]
            [!VAR "MacOuterLoopCounter" = "$MacOuterLoopCounter + 1"!]
            [!VAR "MacInnerLoopCounter" = "0"!]
            [!VAR "MacMatchcounter" = "0"!]
            [!VAR "MacNotification" = "IcuSignalEdgeDetection/IcuSignalNotification"!]
            [!LOOP "../../../*/IcuChannel/*"!] [!//All notifications from all configurations
                [!VAR "a" = "IcuMeasurementMode"!]
                [!IF "contains($a,'ICU_MODE_SIGNAL_EDGE_DETECT') and node:exists(IcuSignalEdgeDetection/IcuSignalNotification)"!]
                  [!IF "(IcuSignalEdgeDetection/IcuSignalNotification != 'NULL_PTR')"!]
                    [!VAR "MacInnerLoopCounter" = "$MacInnerLoopCounter + 1"!]
                    [!IF "$MacOuterLoopCounter >= $MacInnerLoopCounter"!]
                        [!IF "($MacNotification = IcuSignalEdgeDetection/IcuSignalNotification)"!]
                             [!VAR "MacMatchcounter" = "$MacMatchcounter + 1"!]
                        [!ENDIF!]
                    [!ENDIF!]
                  [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$MacMatchcounter = 1"!]
                [!IF "$Found = 'false'"!]
                    [!VAR "Found" = "'true'"!]
[!CODE!][!//
/**
* @brief External Notifications for Edge Detection
*
*/
[!ENDCODE!]
                [!ENDIF!]
[!CODE!][!//
extern void [!"IcuSignalEdgeDetection/IcuSignalNotification"!](void);
[!ENDCODE!]
            [!ENDIF!]
          [!ENDIF!]
        [!ENDIF!]
     [!ENDLOOP!]
    [!ENDSELECT!]

    [!VAR "MacOuterLoopCounter" = "0"!]
    [!VAR "MacInnerLoopCounter" = "0"!]
    [!VAR "Found" = "'false'"!]
    [!SELECT "IcuConfigSet"!]
     [!LOOP "./IcuChannel/*"!]
        [!IF "node:exists(IcuOverflowNotification)"!]
          [!IF "(IcuOverflowNotification != 'NULL_PTR') and (IcuOverflowNotification != 'NULL')"!]
            [!VAR "MacOuterLoopCounter" = "$MacOuterLoopCounter + 1"!]
            [!VAR "MacInnerLoopCounter" = "0"!]
            [!VAR "MacMatchcounter" = "0"!]
            [!VAR "MacNotification" = "IcuOverflowNotification"!]
            [!LOOP "../../../*/IcuChannel/*"!] [!//All notifications from all configurations
                [!IF "node:exists(IcuOverflowNotification)"!]
                    [!IF "(IcuOverflowNotification != 'NULL_PTR')and (IcuOverflowNotification != 'NULL')"!]
                        [!VAR "MacInnerLoopCounter" = "$MacInnerLoopCounter + 1"!]
                        [!IF "$MacOuterLoopCounter >= $MacInnerLoopCounter"!]
                            [!IF "($MacNotification = IcuOverflowNotification)"!]
                                [!VAR "MacMatchcounter" = "$MacMatchcounter + 1"!]
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!IF "$MacMatchcounter = 1"!]
                [!IF "$Found = 'false'"!]
                    [!VAR "Found" = "'true'"!]
[!CODE!]/**
 *   @brief  Overflow notification handlers
 */
[!ENDCODE!]
                [!ENDIF!]
[!CODE!][!//
extern void [!"IcuOverflowNotification"!](void);
[!ENDCODE!]
            [!ENDIF!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDLOOP!]
    [!ENDSELECT!]
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!// MACRO for creating the configuration pointer(s)
[!MACRO "ICU_GENERATE_CONFIGURATION","VARIANT"="'PB'"!][!//
[!NOCODE!]
    [!FOR "PartitionIdx" = "1" TO "$IcuEcucPartitionRefNum"!]
        [!VAR "NumChannelInPartition" = "num:i(count(IcuConfigSet/IcuChannel/*))"!]
        [!IF "$IcuEcucPartitionRefCount != 0 "!]
            [!VAR "PartitionIndex" = "node:name(node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)]))"!]
            [!VAR "IcuEcucPartitionRefName" = "node:value(IcuGeneral/IcuEcucPartitionRef/*[num:i($PartitionIdx)])"!]
            [!VAR "NumChannelInPartition"  = "num:i(count(IcuConfigSet/IcuChannel/*/IcuChannelEcucPartitionRef/*[. = $IcuEcucPartitionRefName]))"!]
        [!ENDIF!]
        [!SELECT "IcuConfigSet"!]
            [!VAR "IdxInPartition" = "0"!]
[!CODE!]/*[!CR!][!ENDCODE!]
[!CODE!]*  @brief    [!"$VARIANT"!][!IF "$IcuEcucPartitionRefCount != 0 "!] Partition [!"$PartitionIndex"!][!ENDIF!] Configuration[!CR!][!ENDCODE!]
[!CODE!]*/[!CR!][!ENDCODE!]
[!CODE!]static const Icu_ChannelConfigType Icu_ChannelConfig_[!"$VARIANT"!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!][[!"num:i($NumChannelInPartition)"!]U]=[!CR!]{[!CR!][!ENDCODE!]
            [!VAR "NumIcuChannels" = "num:i(count(IcuChannel/*))"!]
            [!FOR "Idx" = "0" TO "$NumIcuChannels - 1"!]
            [!LOOP "IcuChannel/*"!]
                [!IF "num:i($Idx)=num:i(IcuChannelId)"!]
                [!IF "(($IcuEcucPartitionRefCount = 0) or (node:value(./IcuChannelEcucPartitionRef/*[1]) = $IcuEcucPartitionRefName))"!]
                    [!CODE!][!WS "4"!]/* [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuWakeupCapability))"!],    /* Wakeup capability */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"IcuDefaultStartEdge"!],    /* Edge used */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"IcuMeasurementMode"!],     /* Measurement mode */[!CR!][!ENDCODE!]

                    [!IF "IcuMeasurementMode='ICU_MODE_SIGNAL_EDGE_DETECT'"!]
                    [!CODE!][!WS "8"!](Icu_MeasurementSubModeType)0U, /* Icu_MeasurementSubModeType */[!CR!][!ENDCODE!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_TIMESTAMP'"!]
                    [!CODE!][!WS "8"!](Icu_MeasurementSubModeType)[!"IcuTimestampMeasurement/IcuTimestampMeasurementProperty"!], /* Icu_MeasurementSubModeType */[!CR!][!ENDCODE!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_SIGNAL_MEASUREMENT'"!]
                    [!CODE!][!WS "8"!](Icu_MeasurementSubModeType)[!"IcuSignalMeasurement/IcuSignalMeasurementProperty"!], /* Icu_MeasurementSubModeType */[!CR!][!ENDCODE!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_EDGE_COUNTER'"!]
                    [!CODE!][!WS "8"!](Icu_MeasurementSubModeType)[!"IcuDefaultStartEdge"!], /* Starting edge */[!CR!][!ENDCODE!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_SIGNAL_MEASUREMENT' or IcuMeasurementMode='ICU_MODE_EDGE_COUNTER'"!]
                    [!CODE!][!WS "8"!]NULL_PTR, /* Icu_Channel_Notification */[!CR!][!ENDCODE!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_TIMESTAMP'"!]
                        [!IF "IcuTimestampMeasurement/IcuTimestampNotification!='NULL_PTR'"!]
                            [!CODE!][!WS "8"!]&[!"IcuTimestampMeasurement/IcuTimestampNotification"!], /* Icu_Channel_Notification */[!CR!][!ENDCODE!]
                        [!ELSE!]
                            [!CODE!][!WS "8"!]NULL_PTR, /* Icu_Channel_Notification */[!CR!][!ENDCODE!]
                        [!ENDIF!]
                    [!ENDIF!]
                            
                    [!IF "IcuMeasurementMode='ICU_MODE_SIGNAL_EDGE_DETECT'"!]
                        [!IF "node:exists(IcuSignalEdgeDetection/IcuSignalNotification) and (node:value(IcuSignalEdgeDetection/IcuSignalNotification) != 'NULL_PTR')"!]
                            [!CODE!][!WS "8"!]&[!"IcuSignalEdgeDetection/IcuSignalNotification"!], /* Icu_Channel_Notification */[!CR!][!ENDCODE!]
                        [!ELSE!]
                            [!CODE!][!WS "8"!]NULL_PTR, /* Icu_Channel_Notification */[!CR!][!ENDCODE!]
                        [!ENDIF!]
                    [!ENDIF!]
[!CODE!][!//
#if ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON))
[!ENDCODE!]
                    [!// MCL DMA logical Channel
                    [!CODE!][!WS "8"!](Mcl_ChannelType)[!ENDCODE!]
                    [!IF "((IcuMeasurementMode = 'ICU_MODE_TIMESTAMP') or (IcuMeasurementMode = 'ICU_MODE_SIGNAL_MEASUREMENT')) and IcuDMAChannelEnable = 'true'"!]
                        [!IF "node:exists(IcuDMAChannelRef)"!]
                            [!CODE!][!WS "8"!][!"node:value(node:ref(IcuDMAChannelRef)/dmaLogicChannel_LogicName)"!], /* Mcl_DmaChannel */[!CR!][!ENDCODE!]
                            [!// Validation for Notification function
                            [!IF "node:exists(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback)"!]
                                [!IF "(node:value(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback) = 'NULL_PTR') or (node:value(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback) = 'NULL')"!]
                                    [!ERROR!] From variant "[!"variant:name( )"!]","[!"node:path(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback)"!]" need to be configured for DMA Channels that are linked to "[!"node:name(.)"!]". The name of the notification shall be &lt;IcuChannelName&gt;_MclDmaTransferCompletionNotif e.g [!"node:name(.)"!]_MclDmaTransferCompletionNotif[!ENDERROR!]
                                [!ELSEIF "node:value(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback) != concat(node:name(.),'_MclDmaTransferCompletionNotif')"!]
                                    [!ERROR!] From variant "[!"variant:name( )"!]","[!"node:path(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback)"!]" name in "[!"node:name(.)"!]" must be [!"node:name(.)"!]_MclDmaTransferCompletionNotif[!ENDERROR!]
                                [!ENDIF!]
                            [!ELSE!]
                                [!ERROR!] From variant "[!"variant:name( )"!]","[!"node:path(node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback)"!]" need to be configured for DMA Channels that are linked to "[!"node:name(.)"!]" [!ENDERROR!]
                            [!ENDIF!]
                        [!ELSE!]
                            [!ERROR!] From variant "[!"variant:name( )"!]", IcuDMAChannelRef need to be configured for DMA Channels that are linked to "[!"node:name(.)"!]" [!ENDERROR!]
                        [!ENDIF!]
                    [!ELSE!]
                        [!CODE!]NoMclDmaChannel, /* Mcl_DmaChannel */[!CR!][!ENDCODE!]
                    [!ENDIF!]
[!CODE!][!//
#endif
#if (ICU_OVERFLOW_NOTIFICATION_API == STD_ON)
[!ENDCODE!]
                    [!IF "node:exists(IcuOverflowNotification) and (IcuOverflowNotification!= 'NULL_PTR')"!]
                        [!CODE!][!WS "8"!]&[!"IcuOverflowNotification"!], /* Icu_Channel_OverflowNotification */[!CR!][!ENDCODE!]
                    [!ELSE!]
                        [!CODE!][!WS "8"!]NULL_PTR, /* Icu_Channel_OverflowNotification */[!CR!][!ENDCODE!]
                    [!ENDIF!]
[!CODE!][!//
#endif  /* ICU_OVERFLOW_NOTIFICATION_API */
#if (ICU_REPORT_WAKEUP_SOURCE == STD_ON)
[!ENDCODE!]
                    
                    [!IF "IcuWakeupCapability"!]
                        [!CODE!][!WS "8"!](Icu_WakeupValueType)((uint32)((uint32)1U<<[!"as:ref(IcuWakeup/IcuChannelWakeupInfo)/EcuMWakeupSourceId"!])), /* Icu_Channel_WakeupValue */[!CR!][!ENDCODE!]
                    [!ELSE!]
                        [!CODE!][!WS "8"!](Icu_WakeupValueType)0U, /*Icu_Channel_WakeupValue*/[!CR!][!ENDCODE!]
                    [!ENDIF!]
[!CODE!][!//
#endif /* (ICU_REPORT_WAKEUP_SOURCE == STD_ON) */
[!ENDCODE!]
                    [!CODE!][!WS "8"!]&Icu_Ipw_IpChannelConfig_[!"$VARIANT"!][!IF "$IcuEcucPartitionRefCount != 0 "!]_P_[!"$PartitionIndex"!][!ENDIF!][[!"num:i($IdxInPartition)"!]U] /* Ipw channel pointer */[!CR!][!ENDCODE!]
[!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!VAR "IdxInPartition" = "$IdxInPartition + 1"!]
                    [!IF "$IdxInPartition < num:i($NumChannelInPartition)"!][!CODE!],[!ENDCODE!][!ENDIF!]
                    [!CODE!][!CR!][!ENDCODE!]
                [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!]
            [!ENDFOR!]
[!CODE!]};[!CR!][!ENDCODE!]
        [!ENDSELECT!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!ENDMACRO!]


[!// MACRO for creating the defines for the interrupts used on hardware modules
[!MACRO "ECUC_MAX_PARTITION_NUMBER"!]
[!NOCODE!]
[!VAR "IcuEcucPartitionRefCount" = "num:i(count(as:modconf('Icu')[1]/IcuGeneral/IcuEcucPartitionRef/*))"!]
[!IF "$IcuEcucPartitionRefCount != 0 "!]
    [!VAR "IcuEcucPartitionRefNum" = "$IcuEcucPartitionRefCount"!] 
    [!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
        [!VAR "maxPartition" = "0"!] 
        [!LOOP "./IcuGeneral/IcuEcucPartitionRef/*"!]
            [!VAR "IcuCrtPart" = "node:value(.)"!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                [!IF "node:refvalid(./OsAppEcucPartitionRef)"!]
                    [!IF "$IcuCrtPart = node:value(./OsAppEcucPartitionRef)"!]
                        [!IF "node:refvalid(./OsApplicationCoreRef)"!]            
                            [!IF "$maxPartition < node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                                [!VAR "maxPartition" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!] 
                            [!ENDIF!]
                        [!ENDIF!]
                    [!ENDIF!]
                [!ENDIF!]
            [!ENDLOOP!] 
        [!ENDLOOP!]
    [!ELSE!]
        [!VAR "maxPartition" = "0"!] 
        [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
            [!IF "node:refvalid(./OsApplicationCoreRef)"!]            
                [!IF "$maxPartition < node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]
                    [!VAR "maxPartition" = "node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!] 
                [!ENDIF!]
            [!ENDIF!]            
        [!ENDLOOP!]  
    [!ENDIF!]
[!ELSE!]
    [!VAR "maxPartition" = "0"!] 
    [!VAR "IcuEcucPartitionRefNum" = "1"!] 
[!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!]

[!ENDNOCODE!]
[!NOCODE!][!//
/**   
  @file    Spi_RegOperations.m
  @version 2.0.1

  @brief   AUTOSAR Spi - File used by the C generation templates.
  @details This file contains macros used by the C generation templates.
*/
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPSPI
*   Dependencies         : 
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
==================================================================================================*/
[!//==================================================================================================
[!//                                List Of Notification Functions
[!//==================================================================================================
[!MACRO "Spi_ListOfNotificationFunctions"!][!//
[!NOCODE!]
[!CODE!][!//
/* Job start Notifications */
[!ENDCODE!][!//
    [!LOOP "SpiDriver/SpiJob/*"!][!//
        [!VAR "WhiteSpaceInstartJobNotification"="normalize-space(SpiJobStartNotification)"!][!//
        [!IF "(string-length($WhiteSpaceInstartJobNotification)>0) and (SpiJobStartNotification!='NULL_PTR') and (SpiJobStartNotification!='NULL')"!][!//
[!CODE!][!//
extern void [!"SpiJobStartNotification"!](void); /* start job Notification for Job '[!"name(.)"!]' */
[!ENDCODE!][!//
        [!ENDIF!][!//
[!ENDLOOP!][!//
[!//

[!CODE!][!//

/* Job End Notifications */
[!ENDCODE!][!//
    [!LOOP "SpiDriver/SpiJob/*"!][!//
        [!VAR "WhiteSpaceInJobEndNotification"="normalize-space(SpiJobEndNotification)"!][!//
        [!IF "(string-length($WhiteSpaceInJobEndNotification)>0) and (SpiJobEndNotification!='NULL_PTR') and (SpiJobEndNotification!='NULL')"!][!//
[!CODE!][!//
extern void [!"SpiJobEndNotification"!](void); /* End Notification for Job '[!"name(.)"!]' */
[!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    
[!CODE!][!//

/* Sequence End Notifications */
[!ENDCODE!][!//
    [!LOOP "SpiDriver/SpiSequence/*"!][!//
        [!VAR "WhiteSpaceInSeqEndNotification"="normalize-space(SpiSeqEndNotification)"!][!//
        [!IF "(string-length($WhiteSpaceInSeqEndNotification)>0)  and (SpiSeqEndNotification!='NULL_PTR') and (SpiSeqEndNotification!='NULL')"!][!//
[!CODE!][!//
extern void [!"SpiSeqEndNotification"!](void); /* End Notification for Sequence '[!"name(.)"!]' */
[!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//


[!//==================================================================================================
[!//               Buffers Descriptors for EB Channels and Allocate Buffers for IB Channels
[!//==================================================================================================
[!MACRO "Spi_BuffersDescriptorForEBchannel"!][!//
[!NOCODE!][!//
    [!LOOP "SpiDriver/SpiChannel/*"!][!//
      [!VAR "ThisChannelIsUsed" = "'true'"!]
      [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
          [!IF "not(node:exists(SpiChannelEcucPartitionRef))"!]
              [!VAR "ThisChannelIsUsed" = "'false'"!]
          [!ENDIF!]
      [!ENDIF!]
      [!IF "$ThisChannelIsUsed = 'true'"!]
        [!IF "SpiChannelType = 'EB'"!][!//
            [!IF "num:i(SpiDataWidth) > 8"!][!//
[!CODE!][!//
#if (CPU_TYPE == CPU_TYPE_64)
VAR_ALIGN(static Spi_BufferDescriptorType Spi_Buffer[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!], 8)
#else
/** Compiler_Warning: It is intended for aligning memory. */
VAR_ALIGN(static Spi_BufferDescriptorType Spi_Buffer[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!], 4)
#endif
[!ENDCODE!][!//
            [!ELSE!][!//
[!CODE!][!//
static Spi_BufferDescriptorType Spi_Buffer[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!];
[!ENDCODE!][!//
            [!ENDIF!][!//
[!CODE!][!//
/* Allocate Buffers for IB Channels (if any) */
[!ENDCODE!][!//
        [!ELSEIF "SpiChannelType = 'IB'"!][!//
            [!IF "num:i(SpiDataWidth) > 8"!][!//
[!CODE!][!//
#if (CPU_TYPE == CPU_TYPE_64)
VAR_ALIGN(static Spi_DataBufferType Spi_BufferTX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]], 8)
VAR_ALIGN(static Spi_DataBufferType Spi_BufferRX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]], 8)
#else
/** Compiler_Warning: It is intended for aligning memory. */
VAR_ALIGN(static Spi_DataBufferType Spi_BufferTX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]], 4)
VAR_ALIGN(static Spi_DataBufferType Spi_BufferRX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]], 4)
#endif
[!ENDCODE!][!//
            [!ELSE!][!//
[!CODE!][!//
static Spi_DataBufferType Spi_BufferTX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]];
static Spi_DataBufferType Spi_BufferRX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!][[!"SpiIbNBuffers"!]];
[!ENDCODE!][!//
            [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Buffers Descriptors for IB Channels
[!//==================================================================================================
[!MACRO "Spi_BuffersDescriptorForIBchannel"!][!//
[!NOCODE!]
[!CODE!][!//
    /*  Buffers Descriptors for IB Channels (if any) */
[!ENDCODE!][!//
    [!LOOP "SpiDriver/SpiChannel/*"!][!//
        [!IF "SpiChannelType = 'IB'"!][!//
        [!VAR "ThisChannelIsUsed" = "'true'"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!IF "not(node:exists(SpiChannelEcucPartitionRef))"!]
                [!VAR "ThisChannelIsUsed" = "'false'"!]
            [!ENDIF!]
        [!ENDIF!]
       [!IF "$ThisChannelIsUsed = 'true'"!]
[!CODE!][!//
static Spi_BufferDescriptorType Spi_Buffer[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!] =
{
    Spi_BufferTX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!],  /* pBufferTX */
    Spi_BufferRX[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!]   /* pBufferRX */
};
[!ENDCODE!][!//
        [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Channel Configuration
[!//==================================================================================================
[!MACRO "Spi_ChannelConfiguration"!][!//
[!NOCODE!][!//
    [!VAR "Count" = "count(SpiDriver/SpiChannel/*)"!][!//
    
    [!FOR "channelIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiChannel/*[SpiChannelId=$channelIndex]"!][!//
        [!VAR "ThisChannelIsUsed" = "'true'"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!IF "not(node:exists(SpiChannelEcucPartitionRef))"!]
                [!VAR "ThisChannelIsUsed" = "'false'"!]
            [!ENDIF!]
        [!ENDIF!]
       [!IF "$ThisChannelIsUsed = 'true'"!]
[!CODE!][!//
/* Channel Configuration of [!"name(.)"!]*/
static const Spi_ChannelConfigType Spi_ChannelConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
        /* [!"name(.)"!]*/
        [!"SpiChannelType"!], /* BufferType IB or EB */
        [!"SpiDataWidth"!]U, /* Frame size */
        [!IF "SpiTransferStart = 'LSB'"!][!//        
        (boolean)TRUE, /* Bite order */
        [!ELSE!][!//
        (boolean)FALSE, /* Bite order */
        [!ENDIF!]
#ifdef SPI_HALF_DUPLEX_MODE_SUPPORT
  #if (STD_ON == SPI_HALF_DUPLEX_MODE_SUPPORT)
        [!IF "node:exists(SpiChannelHalfDuplexDirection)"!]  
        SPI_[!"SpiChannelHalfDuplexDirection"!],
        [!ELSE!]
        SPI_FULL_DUPLEX,
        [!ENDIF!]
  #endif
#endif
[!ENDCODE!][!//
[!IF "node:exists(SpiDefaultData)"!][!//
[!CODE!][!//
        (uint32)[!"SpiDefaultData"!]U, /* u32DefaultTransmitValue (configured) */
[!ENDCODE!][!//
[!ELSE!][!//
[!CODE!][!//
        (uint32)1U, /* In the case SpiDefaultData is disabled. Set this value is 1U */
[!ENDCODE!][!//
[!ENDIF!][!//       
[!IF "SpiChannelType = 'IB'"!][!//
[!CODE!][!//
        [!"SpiIbNBuffers"!]U, /* Length (configured in SpiIbNBuffers) */
[!ENDCODE!][!//
[!ENDIF!][!//
[!IF "SpiChannelType = 'EB'"!][!//
[!CODE!][!//
        [!"SpiEbMaxLength"!]U, /* Length (configured in SpiEbMaxLength) */
[!ENDCODE!][!//
[!ENDIF!][!//
[!CODE!][!//
        &Spi_Buffer[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!]_[!ENDIF!][!"name(.)"!], /* pcBufferDescriptor */
[!ENDCODE!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPName"="99"!]
            [!IF "node:exists(SpiChannelEcucPartitionRef)"!]
                [!VAR "CurrentPName" = "(node:name(node:ref(SpiChannelEcucPartitionRef)))"!]
            [!ENDIF!]
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "$CurrentPName = node:name(node:ref(./OsAppEcucPartitionRef))"!][!//
                    [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                        [!VAR "CrtCore"="num:i(node:value(./EcucCoreId))"!]
            [!CODE!]        (uint32)[!"$CrtCore"!]U, /* Core has assigned */[!ENDCODE!]
                    [!ENDSELECT!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
        [!ELSE!]
            [!CODE!]        SPI_SPURIOUS_CORE_ID_U32,[!ENDCODE!]    
        [!ENDIF!]
[!CODE!]
        &Spi_axSpiChannelState[[!"SpiChannelId"!]U] /* pChannelState pointer */
};
[!ENDCODE!][!//
    [!ENDIF!]
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
    
    [!//--------------------------------------------------------------------------------
    [!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!IF "$SpiConfiguredPartitions = 0"!]
        [!VAR "TotalPartition" = "num:i(1)"!]
    [!ENDIF!]
    [!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPartition" = "node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)]))"!]
            [!VAR "CurrentPartitionUnderScore" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
        [!ELSE!]
            [!VAR "CurrentPartitionUnderScore" = "string(null)"!]
        [!ENDIF!]
[!CODE!][!//
/* Spi_apChannelConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Channel Configuration of [!"name(.)"!]*/
static const Spi_ChannelsCfgType Spi_aChannelConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartitionUnderScore"!][[!"num:i($Count)"!]] =
{
[!ENDCODE!][!//
    [!FOR "channelIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiChannel/*[SpiChannelId=$channelIndex]"!][!//
[!CODE!][!//
    {
    /* [!"name(.)"!] */
[!ENDCODE!]
            [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
                [!VAR "CurrentPName"="99"!]
                [!IF "node:exists(SpiChannelEcucPartitionRef)"!]  
                    [!VAR "CurrentPName" = "(node:name(node:ref(SpiChannelEcucPartitionRef)))"!]
                [!ENDIF!]
                [!IF "$CurrentPName = $CurrentPartition"!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_ChannelConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
                [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    NULL_PTR  /* Not allocated to this partition */
[!ENDCODE!]
                [!ENDIF!]
            [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_ChannelConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
            [!ENDIF!]
[!CODE!][!//
    }[!IF "SpiChannelId!=($Count - 1)"!],[!ENDIF!]
[!ENDCODE!][!//
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
[!CODE!]
};
[!ENDCODE!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Channel Assignment of Jobs
[!//==================================================================================================
[!MACRO "Spi_ChannelAssignmentOfJobs"!]
/* Channel to Job Assignment */
[!NOCODE!][!//
    [!LOOP "SpiDriver/SpiJob/*"!][!//
[!CODE!]
static const Spi_ChannelType Spi_[!"name(.)"!]_ChannelAssignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(count(SpiChannelList/*))"!]] = {[!//
[!ENDCODE!][!//
        [!LOOP "SpiChannelList/*"!][!//
            [!IF "@index > 0"!][!CODE!], [!ENDCODE!][!ENDIF!][!//
[!CODE!][!//
SpiConf_SpiChannel_[!"name(node:ref(SpiChannelAssignment))"!][!//
[!ENDCODE!][!//
        [!ENDLOOP!][!CODE!]};[!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Configuration of Jobs
[!//==================================================================================================
[!MACRO "Spi_JobConfiguration"!]
[!NOCODE!][!//
    [!VAR "spiPath" = "node:path(.)"!][!//
    [!VAR "Count" = "count(SpiDriver/SpiJob/*)"!][!//
    
    [!FOR "jobIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiJob/*[SpiJobId=$jobIndex]"!][!//
            [!VAR "WhiteSpaceInJobEndNotification1"="normalize-space(SpiJobEndNotification)"!]
            [!VAR "WhiteSpaceInstartJobNotification1"="normalize-space(SpiJobStartNotification)"!]
[!CODE!][!//
/* Job Configuration of [!"name(.)"!]*/
static const Spi_JobConfigType Spi_JobConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
        /* [!"name(.)"!] */
        (Spi_ChannelType)[!"num:i(count(SpiChannelList/*))"!]U, /* NumChannels field */
        Spi_[!"name(.)"!]_ChannelAssignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* List of Channels */
        [!IF "node:exists(SpiJobEndNotification)"!]
            [!IF "(string-length($WhiteSpaceInJobEndNotification1)>0) and (SpiJobEndNotification!='NULL_PTR') and (SpiJobEndNotification!='NULL')"!]&[!"SpiJobEndNotification"!][!ELSE!]NULL_PTR[!ENDIF!], /* End Notification */
        [!ELSE!]
            NULL_PTR,
        [!ENDIF!]
        [!IF "node:exists(SpiJobStartNotification) and (../../../SpiAutosarExt/SpiJobStartNotificationEnable = 'true') "!]
            [!IF "(string-length($WhiteSpaceInstartJobNotification1)>0) and (SpiJobStartNotification!='NULL_PTR') and (SpiJobStartNotification!='NULL')"!]&[!"SpiJobStartNotification"!][!ELSE!]NULL_PTR[!ENDIF!], /* Start Notification */
        [!ELSE!]
            NULL_PTR,
        [!ENDIF!]
        (sint8)[!"SpiJobPriority"!], /* Priority */
[!ENDCODE!][!//
        [!VAR "CurrentPName"="string(null)"!][!//
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!SELECT "node:ref(SpiDeviceAssignment)"!][!//
                [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                    [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                [!ENDIF!]
            [!ENDSELECT!]
            [!IF "$CurrentPName = string(null)"!]
        [!CODE!][!WS "8"!](uint32)99U, /* No Core has assigned */[!ENDCODE!]
            [!ELSE!]
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "$CurrentPName = node:name(node:ref(./OsAppEcucPartitionRef))"!][!//
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                            [!VAR "CrtCore"="num:i(node:value(./EcucCoreId))"!]
                            [!VAR "CurrentPName" = "concat('_',$CurrentPName)"!]
        [!CODE!][!WS "8"!](uint32)[!"num:i(node:value(./EcucCoreId))"!]U, /* Core has assigned */[!ENDCODE!]
                        [!ENDSELECT!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//
        [!ELSE!]
        [!CODE!][!WS "8"!]SPI_SPURIOUS_CORE_ID_U32,[!ENDCODE!]
        [!ENDIF!]
[!CODE!][!//
        &Spi_axSpiJobState[[!"SpiJobId"!]], /* JobState instance */
        [!"node:value(node:ref(SpiDeviceAssignment)/SpiHwUnit)"!], /* HWUnit index */
        /* External Device Settings */
[!ENDCODE!][!//
            [!SELECT "node:ref(SpiDeviceAssignment)"!][!//
[!CODE!][!//
        SPI_[!"name(.)"!], /* External Device */
        [!IF "(as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true') and ($CurrentPName = string(null))"!][!//
        NULL_PTR[!//
        [!ELSE!][!//
        &Spi_aExternalDeviceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPName"!][SPI_[!"name(.)"!]] /* pcExternalDeviceConfig - pointer to the external device configuration */[!//
        [!ENDIF!][!//
[!ENDCODE!][!//
            [!ENDSELECT!][!//
[!CODE!][!//
};
[!ENDCODE!]
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
    
    [!//--------------------------------------------------------------------------------
    [!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!IF "$SpiConfiguredPartitions = 0"!]
        [!VAR "TotalPartition" = "num:i(1)"!]
    [!ENDIF!]
    [!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPartition" = "node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)]))"!]
            [!VAR "CurrentPartitionUnderScore" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
        [!ELSE!]
            [!VAR "CurrentPartitionUnderScore" = "string(null)"!]
        [!ENDIF!]
[!CODE!][!//
/* Spi_apJobConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Job Configuration of [!"name(.)"!]*/
static const Spi_JobsCfgType Spi_aJobConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartitionUnderScore"!][[!"num:i($Count)"!]] =
{
[!ENDCODE!][!//
    [!FOR "jobIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiJob/*[SpiJobId=$jobIndex]"!][!//
[!CODE!][!//
    {
    /* [!"name(.)"!] */
[!ENDCODE!]
            [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
                [!VAR "CurrentPName"="99"!][!//
                [!SELECT "node:ref(SpiDeviceAssignment)"!][!//
                    [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                        [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                    [!ENDIF!]
                [!ENDSELECT!]
                [!IF "$CurrentPName = $CurrentPartition"!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_JobConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
                [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    NULL_PTR  /* Not allocated to this partition */
[!ENDCODE!]
                [!ENDIF!]
            [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_JobConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
            [!ENDIF!]
[!CODE!][!//
    }[!IF "SpiJobId!=($Count - 1)"!],[!ENDIF!]
[!ENDCODE!]
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
[!CODE!]
};
[!ENDCODE!]
    [!ENDFOR!][!//
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Job Assignment of Sequences
[!//==================================================================================================
[!MACRO "Spi_JobAssignmentOfSequences"!][!//
/* Job to Sequence Assignment */
[!NOCODE!][!//
    [!LOOP "SpiDriver/SpiSequence/*"!][!//
[!CODE!]
static const Spi_JobType Spi_[!"name(.)"!]_JobAssignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i(count(SpiJobAssignment/*))"!]] = {[!//
[!ENDCODE!][!//
        [!LOOP "SpiJobAssignment/*"!][!//
                [!IF "@index > 0"!][!CODE!],[!ENDCODE!][!ENDIF!][!//
[!CODE!]SpiConf_SpiJob_[!"name(node:ref(.))"!][!ENDCODE!][!//
        [!ENDLOOP!][!//
[!CODE!]};[!ENDCODE!]
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                 Configuration of Sequences
[!//==================================================================================================
[!MACRO "Spi_SequenceConfiguration"!]
[!NOCODE!][!//
    [!VAR "spiPath" = "node:path(.)"!][!//
    [!VAR "Count" = "count(SpiDriver/SpiSequence/*)"!][!//
    
    [!FOR "sequenceIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiSequence/*[SpiSequenceId=$sequenceIndex]"!][!//
[!CODE!][!//
/* Sequence Configuration of [!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] */
static const Spi_SequenceConfigType Spi_SequenceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
        /* [!"name(.)"!] */
[!ENDCODE!][!//
            [!VAR "WhiteSpaceInSeqEndNotification1"="normalize-space(SpiSeqEndNotification)"!]
[!CODE!][!//
        (Spi_JobType)[!"num:i(count(SpiJobAssignment/*))"!]U,
[!ENDCODE!][!//
            [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
                [!VAR "CurrentPName"="99"!][!//
                [!SELECT "SpiJobAssignment/*[1]"!][!//
                    [!SELECT "node:ref(node:current())"!][!//
                        [!SELECT "node:ref(SpiDeviceAssignment)"!][!//
                            [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                                [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                            [!ENDIF!]
                        [!ENDSELECT!][!//
                    [!ENDSELECT!][!//
                [!ENDSELECT!][!//
                [!IF "$CurrentPName = 99.0"!]
[!CODE!]        (uint32)99U, /* No Core has assigned */[!ENDCODE!]
                [!ELSE!]
                    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                        [!IF "$CurrentPName = node:name(node:ref(./OsAppEcucPartitionRef))"!][!//
                            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                                [!VAR "CrtCore"="num:i(node:value(./EcucCoreId))"!]
[!CODE!]        (uint32)[!"num:i(node:value(./EcucCoreId))"!]U, /* Core has assigned */[!ENDCODE!]
                            [!ENDSELECT!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDIF!][!//
            [!ELSE!]
[!CODE!]        SPI_SPURIOUS_CORE_ID_U32,[!ENDCODE!]
            [!ENDIF!]
[!CODE!]
        Spi_[!"name(.)"!]_JobAssignment[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!], /* List of Jobs */
        [!IF "(string-length($WhiteSpaceInSeqEndNotification1)>0) and (SpiSeqEndNotification!='NULL_PTR') and (SpiSeqEndNotification!='NULL')"!]&[!"SpiSeqEndNotification"!][!ELSE!]NULL_PTR[!ENDIF!], /* End Notification */
        [!IF "SpiInterruptibleSequence"!](uint8)TRUE[!ELSE!](uint8)FALSE[!ENDIF!] /* Interruptible */
        [!IF "node:exists(SpiEnableDmaFastTransfer)"!][!//
        #if ((SPI_DMA_USED == STD_ON) && (SPI_ENABLE_DMAFASTTRANSFER_SUPPORT == STD_ON))
        , [!IF "SpiEnableDmaFastTransfer"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!] /* Enable Dma fast transfer */
        #endif
        [!ENDIF!]
};
[!ENDCODE!]
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
    
    [!//--------------------------------------------------------------------------------
    [!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!IF "$SpiConfiguredPartitions = 0"!]
        [!VAR "TotalPartition" = "num:i(1)"!]
    [!ENDIF!]
    [!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPartition" = "node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)]))"!]
            [!VAR "CurrentPartitionUnderScore" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
        [!ELSE!]
            [!VAR "CurrentPartitionUnderScore" = "string(null)"!]
        [!ENDIF!]
[!CODE!][!//
/* Spi_apSequenceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Sequence Configuration of [!"name(.)"!]*/
static const Spi_SeqsConfigType Spi_aSequenceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartitionUnderScore"!][[!"num:i($Count)"!]] =
{
[!ENDCODE!][!//
    [!FOR "sequenceIndex" = "0" TO "num:i($Count)"!][!//
        [!SELECT "SpiDriver/SpiSequence/*[SpiSequenceId=$sequenceIndex]"!][!//
[!CODE!][!//
    {
    /* [!"name(.)"!] */
[!ENDCODE!]
            [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
                [!VAR "CurrentPName"="99"!][!//
                [!SELECT "SpiJobAssignment/*[1]"!][!//
                    [!SELECT "node:ref(node:current())"!][!//
                        [!SELECT "node:ref(SpiDeviceAssignment)"!][!//
                            [!IF "(count(SpiDeviceEcucPartitionRef/*))=1"!]
                                [!VAR "CurrentPName" = "node:name(node:ref(SpiDeviceEcucPartitionRef/*[1]))"!]
                            [!ENDIF!]
                        [!ENDSELECT!][!//
                    [!ENDSELECT!][!//
                [!ENDSELECT!][!//
                [!IF "$CurrentPName = $CurrentPartition"!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_SequenceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
                [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    NULL_PTR  /* Not allocated to this partition */
[!ENDCODE!]
                [!ENDIF!]
            [!ELSE!]
[!CODE!][!//
    /* [!"name(.)"!] */
    &Spi_SequenceConfig_[!"name(.)"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDCODE!]
            [!ENDIF!]
[!CODE!][!//
    }[!IF "$sequenceIndex!=($Count - 1)"!],[!ENDIF!]
[!ENDCODE!]
        [!ENDSELECT!][!//
    [!ENDFOR!][!//
[!CODE!][!//
};
[!ENDCODE!][!//
    [!ENDFOR!][!//
[!ENDNOCODE!][!//
[!ENDMACRO!][!//

[!//==================================================================================================
[!//                                    Spi Configuration
[!//==================================================================================================
[!MACRO "Spi_ConfigurationInfo"!]
[!NOCODE!]
    [!VAR "SpiDemErrorEnable" = "'false'"!]
    [!IF "SpiAutosarExt/SpiDisableDemReportErrorStatus ='false'"!][!//
        [!IF "node:exists(SpiDemEventParameterRefs)"!]
            [!VAR "SpiDemErrorEnable" = "'true'"!]
        [!ENDIF!]
    [!ENDIF!]
    [!VAR "MaxCoreId"="0"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucHardware/*[1]/EcucCoreDefinition/*"!][!//
        [!IF "$MaxCoreId < node:value(./EcucCoreId)"!]
            [!VAR "MaxCoreId"="node:value(./EcucCoreId)"!]
        [!ENDIF!]
    [!ENDLOOP!][!// 
    [!IF "var:defined('postBuildVariant')"!]
        [!VAR "postBuildVariantNameUnderscore" = "concat('_',$postBuildVariant)"!]
    [!ELSE!]
        [!VAR "postBuildVariantNameUnderscore" = "string(null)"!]
    [!ENDIF!]
    [!VAR "SpiConfiguredPartitions" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!VAR "TotalPartition" = "num:i(count(SpiGeneral/SpiEcucPartitionRef/*))"!]
    [!IF "$SpiConfiguredPartitions = 0"!]
        [!VAR "TotalPartition" = "num:i(1)"!]
    [!ENDIF!]
    [!FOR "PartitionCount" = "1" TO "$TotalPartition"!]
        [!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
            [!VAR "CurrentPartition" = "concat('_',node:name(node:ref(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])))"!]
        [!ELSE!]
            [!VAR "CurrentPartition" = "string(null)"!]
        [!ENDIF!]
[!CODE!]
/** Compiler_Warning: In some cases, the partition configuration may not be used by the application. */
[!IF "(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!]static [!ENDIF!]const Spi_ConfigType Spi_Config[!IF "(IMPLEMENTATION_CONFIG_VARIANT ='VariantPostBuild') or (variant:size()>1)"!][!"$postBuildVariantNameUnderscore"!][!ENDIF!][!"$CurrentPartition"!]=
{
    [!"num:i(count(SpiDriver/SpiExternalDevice/*))"!]U, /* u16MaxExternalDevice - number of external devices */
    [!"num:i(count(SpiDriver/SpiChannel/*) - 1)"!]U, /* Spi_Max_Channel - number of channels */
    [!"num:i(count(SpiDriver/SpiJob/*) - 1)"!]U, /* Spi_Max_Job - number of jobs */
    [!"num:i(count(SpiDriver/SpiSequence/*) - 1)"!]U, /* Spi_Max_Sequence - number of sequences */
[!ENDCODE!]
[!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
    [!VAR "CurrentPName" = "node:value(SpiGeneral/SpiEcucPartitionRef/*[num:i($PartitionCount)])"!]
    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
        [!IF "$CurrentPName = node:value(./OsAppEcucPartitionRef)"!][!//
            [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
[!CODE!]    (uint32)[!"num:i(node:value(./EcucCoreId))"!]U, /* u32SpiCoreUse - used core */[!ENDCODE!]
            [!ENDSELECT!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ELSE!]
[!CODE!]    SPI_SPURIOUS_CORE_ID_U32, /* u32SpiCoreUse */[!ENDCODE!]
[!ENDIF!]
[!CODE!]
    Spi_aChannelConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartition"!], /* pcChannelConfig */
    Spi_aJobConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartition"!], /* pcJobConfig */
    Spi_aSequenceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartition"!], /* pcSequenceConfig */
    Spi_aExternalDeviceConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartition"!], /* pcExternalDeviceConfig */
    Spi_aHwUnitConfigList[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][!"$CurrentPartition"!] /* pcHWUnitConfig */
#if (SPI_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
        [!IF "$SpiDemErrorEnable"!][!//
            [!IF "node:exists(as:modconf('Spi')[1]/SpiDemEventParameterRefs/SPI_E_HARDWARE_ERROR)"!][!//
                [!IF "node:exists(node:ref(as:modconf('Spi')[1]/SpiDemEventParameterRefs/SPI_E_HARDWARE_ERROR))"!][!//
    ,{ (uint32)STD_ON, (uint32)DemConf_DemEventParameter_[!"node:name(node:ref(as:modconf('Spi')[1]/SpiDemEventParameterRefs/SPI_E_HARDWARE_ERROR))"!]} /* SPI_E_HARDWARE_ERROR parameters*/ [!//
                [!ELSE!][!//
                    [!ERROR "Invalid reference for SPI_E_HARDWARE_ERROR"!][!//
                [!ENDIF!][!//
            [!ELSE!][!//
    ,{ (uint32)STD_OFF, (uint32)0U} /* SPI_E_HARDWARE_ERROR parameters*/[!//
            [!ENDIF!][!// 
        [!ELSE!][!//
    ,{ (uint32)STD_OFF, (uint32)0U}/* SPI_E_HARDWARE_ERROR parameters*/ [!//
        [!ENDIF!]
#endif /* SPI_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF */  
};
[!ENDCODE!]
    [!ENDFOR!]
[!ENDNOCODE!]
[!CODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile' or IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime') and (variant:size() <= 1)"!]
[!IF "as:modconf('Spi')[1]/SpiGeneral/SpiMulticoreSupport = 'true'"!]
const Spi_ConfigType * const Spi_PBCfgVariantPredefined[[!"num:i($MaxCoreId + 1)"!]]=
{
[!NOCODE!]
    [!VAR "MacNoPartitions" = "0"!][!//
    [!FOR "SpiCoreIdCount"="0" TO "$MaxCoreId"!]
        [!VAR "CoreIsMatch" = "'false'"!]
        [!VAR "Partitionname" = "0"!][!//
        [!LOOP "SpiGeneral/SpiEcucPartitionRef/*"!]
            [!VAR "MacPartNo" = "node:name(node:ref(.))"!]
            [!IF "num:i(count(as:modconf('Spi')[1]/SpiDriver/SpiExternalDevice/*[contains(./SpiDeviceEcucPartitionRef/*,$MacPartNo)])) > 0"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!]
                    [!IF "$MacPartNo = node:name(node:ref(./OsAppEcucPartitionRef))"!]
                        [!SELECT "node:ref(./OsApplicationCoreRef)"!][!//
                            [!IF "num:i($SpiCoreIdCount) = num:i(node:value(./EcucCoreId))"!]
                                [!VAR "CoreIsMatch" = "'true'"!]
                                [!VAR "Partitionname" = "$MacPartNo"!]
                            [!ENDIF!]
                        [!ENDSELECT!]
                    [!ENDIF!]
                [!ENDLOOP!]
            [!ENDIF!]
        [!ENDLOOP!][!//
        [!IF "$CoreIsMatch = 'true'"!]
[!CODE!]    
    &Spi_Config_[!"$Partitionname"!][!IF "$MacNoPartitions < num:i($MaxCoreId)"!],[!VAR "MacNoPartitions" = "$MacNoPartitions + 1"!][!ENDIF!]
[!ENDCODE!]
        [!ELSE!]
[!CODE!]
    NULL_PTR[!IF "$MacNoPartitions < num:i($MaxCoreId)"!],[!VAR "MacNoPartitions" = "$MacNoPartitions + 1"!][!ENDIF!]
[!ENDCODE!]
        [!ENDIF!]
    [!ENDFOR!]
[!ENDNOCODE!]
};
[!ELSE!]
const Spi_ConfigType * const Spi_PBCfgVariantPredefined[SPI_MAX_PARTITIONS]=
{
    &Spi_Config
};
[!ENDIF!]
[!ENDIF!]
[!ENDCODE!]
[!ENDMACRO!][!//

[!ENDNOCODE!]

/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : ADC_SAR
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

#ifndef ADC_IPW_H
#define ADC_IPW_H

/**
*   @file
*
*   @internal
*   @addtogroup adc_ipw Adc Ipw
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
#include "Adc_Ipw_Types.h"
#include "Adc_Ipw_Cfg.h"

#include "Adc_Sar_Ip.h"
#ifdef ADC_DMA_SUPPORTED
    #include "Dma_Ip.h"
#endif
#if (ADC_IPW_BCTU_AVAILABLE == STD_ON)
#if ((ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) || (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON) || (ADC_HW_TRIGGER_API == STD_ON))
    #include "Bctu_Ip.h"
#endif /* (ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) || */
#endif /* (ADC_IPW_BCTU_AVAILABLE == STD_ON) */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_IPW_VENDOR_ID                       43
#define ADC_IPW_AR_RELEASE_MAJOR_VERSION        4
#define ADC_IPW_AR_RELEASE_MINOR_VERSION        4
#define ADC_IPW_AR_RELEASE_REVISION_VERSION     0
#define ADC_IPW_SW_MAJOR_VERSION                2
#define ADC_IPW_SW_MINOR_VERSION                0
#define ADC_IPW_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Adc_Ipw.h file and Adc_Ipw_Types.h file are of the same vendor */
#if (ADC_IPW_VENDOR_ID != ADC_IPW_VENDOR_ID_TYPES)
    #error "Adc_Ipw.h and Adc_Ipw_Types.h have different vendor ids"
#endif

/* Check if Adc_Ipw.h file and Adc_Ipw_Types.h file are of the same Autosar version */
#if ((ADC_IPW_AR_RELEASE_MAJOR_VERSION != ADC_IPW_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (ADC_IPW_AR_RELEASE_MINOR_VERSION != ADC_IPW_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (ADC_IPW_AR_RELEASE_REVISION_VERSION != ADC_IPW_AR_RELEASE_REVISION_VERSION_TYPES) \
    )
    #error "AutoSar Version Numbers of Adc_Ipw.h and Adc_Ipw_Types.h are different"
#endif

/* Check if Adc_Ipw.h file and Adc_Ipw_Types.h file are of the same Software version */
#if ((ADC_IPW_SW_MAJOR_VERSION != ADC_IPW_SW_MAJOR_VERSION_TYPES) || \
     (ADC_IPW_SW_MINOR_VERSION != ADC_IPW_SW_MINOR_VERSION_TYPES) || \
     (ADC_IPW_SW_PATCH_VERSION != ADC_IPW_SW_PATCH_VERSION_TYPES) \
    )
  #error "Software Version Numbers of Adc_Ipw.h and Adc_Ipw_Types.h are different"
#endif

/* Check if Adc_Ipw.h file and Adc_Ipw_Cfg.h file are of the same vendor */
#if (ADC_IPW_VENDOR_ID != ADC_IPW_VENDOR_ID_CFG)
    #error "Adc_Ipw.h and Adc_Ipw_Cfg.h have different vendor ids"
#endif

/* Check if Adc_Ipw.h file and Adc_Ipw_Cfg.h file are of the same Autosar version */
#if ((ADC_IPW_AR_RELEASE_MAJOR_VERSION != ADC_IPW_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (ADC_IPW_AR_RELEASE_MINOR_VERSION != ADC_IPW_AR_RELEASE_MINOR_VERSION_CFG) || \
     (ADC_IPW_AR_RELEASE_REVISION_VERSION != ADC_IPW_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Adc_Ipw.h and Adc_Ipw_Cfg.h are different"
#endif

/* Check if Adc_Ipw.h file and Adc_Ipw_Cfg.h file are of the same Software version */
#if ((ADC_IPW_SW_MAJOR_VERSION != ADC_IPW_SW_MAJOR_VERSION_CFG) || \
     (ADC_IPW_SW_MINOR_VERSION != ADC_IPW_SW_MINOR_VERSION_CFG) || \
     (ADC_IPW_SW_PATCH_VERSION != ADC_IPW_SW_PATCH_VERSION_CFG) \
    )
  #error "Software Version Numbers of Adc_Ipw.h and Adc_Ipw_Cfg.h are different"
#endif

/* Check if Adc_Ipw.h file and Adc_Sar_Ip.h file are of the same vendor */
#if (ADC_IPW_VENDOR_ID != ADC_SAR_IP_VENDOR_ID)
    #error "Adc_Ipw.h and Adc_Sar_Ip.h have different vendor ids"
#endif

/* Check if Adc_Ipw.h file and Adc_Sar_Ip.h file are of the same Autosar version */
#if ((ADC_IPW_AR_RELEASE_MAJOR_VERSION != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_IPW_AR_RELEASE_MINOR_VERSION != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION) || \
     (ADC_IPW_AR_RELEASE_REVISION_VERSION != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Adc_Ipw.h and Adc_Sar_Ip.h are different"
#endif

/* Check if Adc_Ipw.h file and Adc_Sar_Ip.h file are of the same Software version */
#if ((ADC_IPW_SW_MAJOR_VERSION != ADC_SAR_IP_SW_MAJOR_VERSION) || \
     (ADC_IPW_SW_MINOR_VERSION != ADC_SAR_IP_SW_MINOR_VERSION) || \
     (ADC_IPW_SW_PATCH_VERSION != ADC_SAR_IP_SW_PATCH_VERSION) \
    )
  #error "Software Version Numbers of Adc_Ipw.h and Adc_Sar_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
#ifdef ADC_DMA_SUPPORTED
/* Check if Adc_Ipw.h file and Dma_Ip.h file are of the same Autosar version */
#if ((ADC_IPW_AR_RELEASE_MAJOR_VERSION != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_IPW_AR_RELEASE_MINOR_VERSION != DMA_IP_AR_RELEASE_MINOR_VERSION)    \
    )
    #error "AutoSar Version Numbers of Adc_Ipw.h and Dma_Ip.h are different"
#endif
#endif /* ADC_DMA_SUPPORTED */
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

#if (ADC_IPW_BCTU_AVAILABLE == STD_ON)
/* The next condition is true if CTU/BCTU Unit is used */
#if ((ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) || (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON) || (ADC_HW_TRIGGER_API == STD_ON))
/* Check if Adc_Ipw.h file and Bctu_Ip.h file are of the same vendor */
#if (ADC_IPW_VENDOR_ID != BCTU_IP_VENDOR_ID)
    #error "Adc_Ipw.h and Bctu_Ip.h have different vendor ids"
#endif

/* Check if Adc_Ipw.h file and Bctu_Ip.h file are of the same Autosar version */
#if ((ADC_IPW_AR_RELEASE_MAJOR_VERSION != BCTU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_IPW_AR_RELEASE_MINOR_VERSION != BCTU_IP_AR_RELEASE_MINOR_VERSION) || \
     (ADC_IPW_AR_RELEASE_REVISION_VERSION != BCTU_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Adc_Ipw.h and Bctu_Ip.h are different"
#endif

/* Check if Adc_Ipw.h file and Bctu_Ip.h file are of the same Software version */
#if ((ADC_IPW_SW_MAJOR_VERSION != BCTU_IP_SW_MAJOR_VERSION) || \
     (ADC_IPW_SW_MINOR_VERSION != BCTU_IP_SW_MINOR_VERSION) || \
     (ADC_IPW_SW_PATCH_VERSION != BCTU_IP_SW_PATCH_VERSION) \
    )
  #error "Software Version Numbers of Adc_Ipw.h and Bctu_Ip.h are different"
#endif
#endif /* (ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) ... */
#endif /* (ADC_IPW_BCTU_AVAILABLE == STD_ON) */
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#ifdef ADC_DMA_SUPPORTED
/* Assumption is that conversion results are less than 16bits, otherwise driver will treat results equal to 0xFFFF as invalid */
#define ADC_IPW_INVALID_DATA_RESULT_DMA     (0xFFFFU)
#endif /* ADC_DMA_SUPPORTED */

#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
/**
* @brief          Mask used to verify the DATA registers content.
*/
#define ADC_IPW_DATA_MASK_U16               (0xFFFFU)
#endif /* (ADC_ENABLE_LIMIT_CHECK == STD_ON) */
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define ADC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"
#if (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON)
/**
* @brief          Unit status structure
* @details        Structure for the unit status.
*
*/
extern Adc_ChansIdxMaskType Adc_axRuntimeChansEnMask[ADC_NO_OF_ENABLE_CH_DISABLE_CH_GROUPS];
#endif /* ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON */
#define ADC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

#if ((ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON) || (ADC_SETCHANNEL_API == STD_ON))
void Adc_Ipw_SetChansMask(Adc_ChansIdxMaskType * const ChansMask, const uint32 ChanIdx);
#endif /* ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON || ADC_SETCHANNEL_API == STD_ON */
#if (ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON)
void Adc_Ipw_ClearChansMask(Adc_ChansIdxMaskType * const ChansMask, const uint32 ChanIdx);
#endif /* ADC_ENABLE_CH_DISABLE_CH_NONAUTO_API == STD_ON */

void Adc_Ipw_ClearValidBit(Adc_HwUnitType Unit,
                           Adc_GroupType Group,
                           uint32 CoreId);

/**
* @brief          Functions mapping.
* @details        Middle layer versus Low layer functions mapping
*
*/
Std_ReturnType Adc_Ipw_Init(const Adc_Ipw_Config * IpwCfg, const uint32 CoreId);

#if (ADC_DEINIT_API == STD_ON)
Std_ReturnType Adc_Ipw_DeInit(uint32 CoreId);
#endif /* ADC_DEINIT_API == STD_ON */

#if (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)
#if (ADC_SOFTWARE_INJECTED_CONVERSIONS_USED == STD_ON)
void Adc_Ipw_StartInjectedConversion(Adc_HwUnitType Unit,
                                     uint32 CoreId);
#endif /* ADC_SOFTWARE_INJECTED_CONVERSIONS_USED == STD_ON */

void Adc_Ipw_StartNormalConversion(Adc_HwUnitType Unit,
                                   uint32 CoreId);

Std_ReturnType Adc_Ipw_StopCurrentConversion(Adc_HwUnitType Unit,
                                             Adc_GroupType Group,
                                             uint32 CoreId);
#endif /* ADC_ENABLE_START_STOP_GROUP_API == STD_ON */

#if (ADC_HW_TRIGGER_API == STD_ON)
Std_ReturnType Adc_Ipw_EnableHardwareTrigger(Adc_GroupType Group,
                                             Adc_HwUnitType Unit,
                                             uint32 CoreId);

void Adc_Ipw_DisableHardwareTrigger(Adc_GroupType Group,
                                    Adc_HwUnitType Unit,
                                    uint32 CoreId);

#if (ADC_IPW_BCTU_AVAILABLE == STD_ON)
void Adc_Ipw_GetCtuChanList(uint8 * ChanList,
                            uint8 ChannelCount,
                            Adc_GroupType Group,
                            Adc_HwUnitType LogicalHwUnitId,
                            uint32 CoreId);
#endif /* (ADC_IPW_BCTU_AVAILABLE == STD_ON) */

#if (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON)
/* CPR_RTD_00037.adc, CPR_RTD_00039.adc, CPR_RTD_00041.adc */
Std_ReturnType Adc_Ipw_EnableCtuTrigger(Adc_GroupType Group,
                                        Adc_HwTriggerTimerType TriggerSource,
                                        boolean FirstTrigger,
                                        uint32 CoreId);

void Adc_Ipw_DisableCtuTrigger(Adc_GroupType Group,
                               Adc_HwTriggerTimerType TriggerSource,
                               boolean LastTrigger,
                               uint32 CoreId);

Std_ReturnType Adc_Ipw_HwResultReadGroup(Adc_GroupType Group,
                                         Adc_ValueGroupType * DataPtr,
                                         uint32 CoreId);
#endif /* ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON */
#endif /* ADC_HW_TRIGGER_API == STD_ON */

#if (ADC_READ_GROUP_API == STD_ON)
Std_ReturnType Adc_Ipw_ReadGroup(const Adc_GroupType Group,
                                 Adc_ValueGroupType * DataPtr,
                                 boolean * Flag,
                                 uint32 CoreId);
#endif /* ADC_READ_GROUP_API == STD_ON */

#if (ADC_ENABLE_READ_RAW_DATA_API == STD_ON)
Std_ReturnType Adc_Ipw_GetConvData(Adc_HwUnitType Unit,
                                   Adc_ChannelType ChnIdx,
                                   uint32 CoreId,
                                   uint16 * Result);
#endif /* (ADC_ENABLE_READ_RAW_DATA_API == STD_ON) */

#if (ADC_SET_HW_UNIT_POWER_MODE_API == STD_ON)
Std_ReturnType Adc_Ipw_SetHwUnitPowerMode(Adc_HwUnitType Unit,
                                          Adc_SetPowerModeType SetHwUnitPowerMode,
                                          uint32 CoreId);
#endif /* ADC_SET_HW_UNIT_POWER_MODE_API == STD_ON */

#if ((ADC_POWER_STATE_SUPPORTED == STD_ON) || (ADC_SET_HW_UNIT_POWER_MODE_API == STD_ON))
#if (ADC_IPW_BCTU_AVAILABLE == STD_ON)
/* The next condition is true if CTU/BCTU Unit is used */
#if ((ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) || (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON) || (ADC_HW_TRIGGER_API == STD_ON))
Std_ReturnType Adc_Ipw_CtuSetPowerMode(Adc_HwUnitType CtuUnit,
                                       Adc_PowerStateType State);
#endif /* (ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON) || */
#endif /* (ADC_IPW_BCTU_AVAILABLE == STD_ON) */
#endif /* ((ADC_POWER_STATE_SUPPORTED == STD_ON) || (ADC_SET_HW_UNIT_POWER_MODE_API == STD_ON)) */

#if (ADC_CALIBRATION == STD_ON)
void Adc_Ipw_Calibrate(Adc_HwUnitType Unit,
                       Adc_CalibrationStatusType * Status,
                       uint32 CoreId);
#endif /* ADC_CALIBRATION == STD_ON */

#if (ADC_SELF_TEST == STD_ON)
Std_ReturnType Adc_Ipw_SelfTest(Adc_HwUnitType Unit,
                                uint32 CoreId);
#endif /* ADC_SELF_TEST == STD_ON */

#if (ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON)
Std_ReturnType Adc_Ipw_ConfigureThreshold(Adc_ThresholdControlIndexType ThresholdControlIndex,
                                          Adc_ValueGroupType LowValue,
                                          Adc_ValueGroupType HighValue,
                                          uint32 CoreId);
#endif /* ADC_ENABLE_CONFIGURE_THRESHOLD_NONAUTO_API == STD_ON */

#ifdef ADC_WDG_SUPPORTED
Std_ReturnType Adc_Ipw_EnableWdgNotification(Adc_HwUnitType Unit,
                                             Adc_ChannelType ChannelId,
                                             uint32 CoreId);

Std_ReturnType Adc_Ipw_DisableWdgNotification(Adc_HwUnitType Unit,
                                              Adc_ChannelType ChannelId,
                                              uint32 CoreId);
#endif /* ADC_WDG_SUPPORTED */

#if (ADC_DUAL_CLOCK_MODE == STD_ON)
Std_ReturnType Adc_Ipw_SetClockMode(Adc_SelectPrescalerType Prescaler,
                                    uint32 CoreId);
#endif /* (ADC_DUAL_CLOCK_MODE == STD_ON) */

#if (ADC_ENABLE_LIMIT_CHECK == STD_ON)
boolean Adc_Ipw_CheckConversionValuesInRange(Adc_ValueGroupType Value,
                                             Adc_HwUnitType LogicalHwUnitId,
                                             Adc_ChannelType ChannelIndex,
                                             uint32 CoreId);
#endif /* (ADC_ENABLE_LIMIT_CHECK == STD_ON) */

#if (ADC_ENABLE_QUEUING == STD_ON)
void Adc_Ipw_RemoveFromQueue(const Adc_HwUnitType Unit,
                             const Adc_QueueIndexType CurQueueIndex);
#endif /* (ADC_ENABLE_QUEUING == STD_ON) */

void Adc_Ipw_GetCmrRegister(Adc_HwUnitType Unit,
                            Adc_GroupType Group,
                            Adc_ChansIdxMaskType * CmrMask,
                            uint32 CoreId);

#if (ADC_POWER_STATE_SUPPORTED == STD_ON)
Std_ReturnType Adc_Ipw_SetPowerState(Adc_PowerStateType PowerState,
                                     uint32 CoreId);
#endif /* (ADC_POWER_STATE_SUPPORTED == STD_ON) */

#if (ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON)
void Adc_Ipw_EnableCtuControlMode(Adc_HwUnitType Unit,
                                  uint32 CoreId);

void Adc_Ipw_DisableCtuControlMode(Adc_HwUnitType Unit,
                                   uint32 CoreId);

#define Adc_Ipw_CtuReadFifoData(Instance, FifoIdx)                 Bctu_Ip_GetFifoData(Instance, FifoIdx)
#define Adc_Ipw_CtuEnableNotification(Instance, NotificationId)      Bctu_Ip_EnableNotifications(Instance, NotificationId)
#define Adc_Ipw_CtuDisableNotification(Instance, NotificationId)     Bctu_Ip_DisableNotifications(Instance, NotificationId)
#define Adc_Ipw_CtuEnableHwTrigger(Instance, TrigIdx)              Bctu_Ip_EnableHwTrigger(Instance, TrigIdx)
#define Adc_Ipw_CtuDisableHwTrigger(Instance, TrigIdx)             Bctu_Ip_DisableHwTrigger(Instance, TrigIdx)
#define Adc_Ipw_CtuStartConversion(Instance, TrigIdx)              Bctu_Ip_SwTriggerConversion(Instance, TrigIdx)
#define Adc_Ipw_CtuReadConvData(Instance, AdcIdx)                  Bctu_Ip_GetConvData(Instance, AdcIdx)
#define Adc_Ipw_CtuReadConvResult(Instance, AdcIdx, ConvResultPtr) Bctu_Ip_GetConvResult(Instance, AdcIdx, ConvResultPtr)
#define Adc_Ipw_CtuReadFifoResult(Instance, FifoIdx, ResultPtr)    Bctu_Ip_GetFifoResult(Instance, FifoIdx, ResultPtr)
#define Adc_Ipw_CtuSetFifoWatermark(Instance, FifoIdx, Watermark)    Bctu_Ip_SetFifoWatermark(Instance, FifoIdx, Watermark)
#define Adc_Ipw_CtuSetList(Instance, StartListElemIdx, ConvListItems, NumItems)   Bctu_Ip_ConfigChanList(Instance, StartListElemIdx, ConvListItems, NumItems)
#define Adc_Ipw_CtuSetListPointer(Instance, TrigIdx, StartListElemIdx) (void)Bctu_Ip_SetTriggerChnListAddr(Instance, TrigIdx, TRUE, StartListElemIdx)
#define Adc_Ipw_CtuStopLoopConversions(Instance, TrigIdx)          Bctu_Ip_StopLoopConversions(Instance, TrigIdx)
#endif /* ADC_ENABLE_CTU_CONTROL_MODE_API == STD_ON */

#if FEATURE_ADC_HAS_TEMPSENSE_CHN
#if (ADC_ENABLE_TEMPSENSE_API == STD_ON)
#define Adc_Ipw_TempSenseCalculateTemp(AdcInstance, TempInstance, Data)  Adc_Sar_Ip_TempSenseCalculateTemp(AdcInstance, TempInstance, Data)
#define Adc_Ipw_TempSenseEnable(Instance)                            Adc_Sar_Ip_TempSenseEnable(Instance)
#define Adc_Ipw_TempSenseDisable(Instance)                           Adc_Sar_Ip_TempSenseDisable(Instance)

Std_ReturnType Adc_Ipw_TempSenseGetTemp(Adc_HwUnitType Unit,
                                        const uint32 TempInstance,
                                        uint16 * const TempSenseVal,
                                        uint32 CoreId);
#endif /* (ADC_ENABLE_TEMPSENSE_API == STD_ON) */
#endif /* FEATURE_ADC_HAS_TEMPSENSE_CHN */

#if (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON)
boolean Adc_Ipw_MapCTUTriggerIndex(Adc_HwTriggerTimerType TriggerSource,
                                   uint32 CoreId,
                                   uint8 * TriggerIndex);
#endif /* (ADC_ENABLE_CTUTRIG_NONAUTO_API == STD_ON) */

uint32 Adc_GetCoreID(void);

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_IPW_H */

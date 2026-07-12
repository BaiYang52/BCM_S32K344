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
*   @file       Power_Ip_PMC.c
*   @version    2.0.1
*
*   @brief
*   @brief   POWER driver implementations.
*   @details POWER driver implementations.
*
*   @addtogroup POWER_DRIVER Power Ip Driver
*   @{
*/


#ifdef __cplusplus
extern "C"{
#endif





/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Power_Ip_Private.h"
#include "Power_Ip_PMC.h"

#if (defined(POWER_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == POWER_IP_ENABLE_USER_MODE_SUPPORT))
  #if (defined(MCAL_PMC_REG_PROT_AVAILABLE))
    #if (STD_ON == MCAL_PMC_REG_PROT_AVAILABLE)
      #define USER_MODE_REG_PROT_ENABLED      (STD_ON)
      #include "RegLockMacros.h"
    #endif /* (STD_ON == MCAL_PMC_REG_PROT_AVAILABLE) */
  #endif
#endif /* (STD_ON == POWER_IP_ENABLE_USER_MODE_SUPPORT) */


/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define POWER_IP_PMC_VENDOR_ID_C                      43
#define POWER_IP_PMC_AR_RELEASE_MAJOR_VERSION_C       4
#define POWER_IP_PMC_AR_RELEASE_MINOR_VERSION_C       4
#define POWER_IP_PMC_AR_RELEASE_REVISION_VERSION_C    0
#define POWER_IP_PMC_SW_MAJOR_VERSION_C               2
#define POWER_IP_PMC_SW_MINOR_VERSION_C               0
#define POWER_IP_PMC_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Power_Ip_PMC.c file and Power_Ip_Private.h file are of the same vendor */
#if (POWER_IP_PMC_VENDOR_ID_C != POWER_IP_PRIVATE_VENDOR_ID)
    #error "Power_Ip_PMC.c and Power_Ip_Private.h have different vendor ids"
#endif

/* Check if Power_Ip_PMC.c file and Power_Ip_Private.h file are of the same Autosar version */
#if ((POWER_IP_PMC_AR_RELEASE_MAJOR_VERSION_C != POWER_IP_PRIVATE_AR_RELEASE_MAJOR_VERSION) || \
     (POWER_IP_PMC_AR_RELEASE_MINOR_VERSION_C != POWER_IP_PRIVATE_AR_RELEASE_MINOR_VERSION) || \
     (POWER_IP_PMC_AR_RELEASE_REVISION_VERSION_C != POWER_IP_PRIVATE_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Power_Ip_PMC.c and Power_Ip_Private.h are different"
#endif

/* Check if Power_Ip_PMC.c file and Power_Ip_Private.h file are of the same Software version */
#if ((POWER_IP_PMC_SW_MAJOR_VERSION_C != POWER_IP_PRIVATE_SW_MAJOR_VERSION) || \
     (POWER_IP_PMC_SW_MINOR_VERSION_C != POWER_IP_PRIVATE_SW_MINOR_VERSION) || \
     (POWER_IP_PMC_SW_PATCH_VERSION_C != POWER_IP_PRIVATE_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Power_Ip_PMC.c and Power_Ip_Private.h are different"
#endif

/* Check if Power_Ip_PMC.c file and Power_Ip_PMC.h file are of the same vendor */
#if (POWER_IP_PMC_VENDOR_ID_C != POWER_IP_PMC_VENDOR_ID)
    #error "Power_Ip_PMC.c and Power_Ip_PMC.h have different vendor ids"
#endif

/* Check if Power_Ip_PMC.c file and Power_Ip_PMC.h file are of the same Autosar version */
#if ((POWER_IP_PMC_AR_RELEASE_MAJOR_VERSION_C != POWER_IP_PMC_AR_RELEASE_MAJOR_VERSION) || \
     (POWER_IP_PMC_AR_RELEASE_MINOR_VERSION_C != POWER_IP_PMC_AR_RELEASE_MINOR_VERSION) || \
     (POWER_IP_PMC_AR_RELEASE_REVISION_VERSION_C != POWER_IP_PMC_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Power_Ip_PMC.c and Power_Ip_PMC.h are different"
#endif

/* Check if Power_Ip_PMC.c file and Power_Ip_PMC.h file are of the same Software version */
#if ((POWER_IP_PMC_SW_MAJOR_VERSION_C != POWER_IP_PMC_SW_MAJOR_VERSION) || \
     (POWER_IP_PMC_SW_MINOR_VERSION_C != POWER_IP_PMC_SW_MINOR_VERSION) || \
     (POWER_IP_PMC_SW_PATCH_VERSION_C != POWER_IP_PMC_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Power_Ip_PMC.c and Power_Ip_PMC.h are different"
#endif

#if (defined(POWER_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == POWER_IP_ENABLE_USER_MODE_SUPPORT))
  #if (defined(MCAL_PMC_REG_PROT_AVAILABLE))
    #if (STD_ON == MCAL_PMC_REG_PROT_AVAILABLE)
        #ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
        /* Check if Power_Ip_PMC.c file and RegLockMacros.h file are of the same Autosar version */
            #if ((POWER_IP_PMC_AR_RELEASE_MAJOR_VERSION_C    != REGLOCKMACROS_AR_RELEASE_MAJOR_VERSION) || \
                (POWER_IP_PMC_AR_RELEASE_MINOR_VERSION_C    != REGLOCKMACROS_AR_RELEASE_MINOR_VERSION))
                #error "AutoSar Version Numbers of Power_Ip_PMC.c and RegLockMacros.h are different"
            #endif
        #endif
    #endif /* (STD_ON == MCAL_PMC_REG_PROT_AVAILABLE) */
  #endif
#endif /* (STD_ON == POWER_IP_ENABLE_USER_MODE_SUPPORT) */
/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
#if (defined(POWER_IP_VOLTAGE_ERROR_ISR_USED))
  #if (POWER_IP_VOLTAGE_ERROR_ISR_USED == STD_ON)

#define MCU_START_SEC_VAR_INIT_UNSPECIFIED
#include "Mcu_MemMap.h"

static volatile Power_Ip_PMC_StatusType Power_Ip_ePMCStatus = PMC_UNINIT;

#define MCU_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Mcu_MemMap.h"

  #endif
#endif
/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#define MCU_START_SEC_CODE

#include "Mcu_MemMap.h"


/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
#if (defined(POWER_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == POWER_IP_ENABLE_USER_MODE_SUPPORT))
  #if (defined(MCAL_PMC_REG_PROT_AVAILABLE))
    #if (STD_ON == MCAL_PMC_REG_PROT_AVAILABLE)
/**
* @brief            This function will enable writing in User mode by configuring REG_PROT
*/
void Power_Ip_PMC_SetUserAccessAllowed(void)
{
#if (defined(IP_PMC_BASE))
    SET_USER_ACCESS_ALLOWED(IP_PMC_BASE, PMC_PROT_MEM_U32);
#endif
}
    #endif
  #endif /* MCAL_PMC_REG_PROT_AVAILABLE */
#endif /* POWER_IP_ENABLE_USER_MODE_SUPPORT */


/**
* @brief            This function configure the Power Management Controller
* @details          The operating voltages are monitored by a set of on-chip supervisory circuits
*                   to ensure that this device works within the correct voltage range.
*
* @param[in]        ConfigPtr   Pointer to PMC configuration structure.
*
* @return           void
*
*/
void Power_Ip_PMC_PowerInit(const Power_Ip_PMC_ConfigType * ConfigPtr)
{
#if !(defined(POWER_IP_DERIVATIVE_001) || defined(POWER_IP_DERIVATIVE_006))
    uint32 StartTime;
    uint32 ElapsedTime;
    uint32 TimeoutTicks;
    boolean TimeoutOccurred = FALSE;
    uint32 LvscValue;
    uint32 LastMileRegValue;
    uint32 ConfigValue;
#endif
    (void)ConfigPtr;

    /* After the initial power ramp-up of the MCU, in PMC_LVSC register the POR flag
     * and the LVR flags are all set to 1. The GNG flags will have an arbitrary value.
     * Therefore, after the initial power ramp-up, all flags in PMC_LVSC must be
     * cleared by writing 0xFFFF_FFFF */

    if (PMC_LVSC_RAMP_UP_RESET_FLAGS_RWBITS_MASK == (IP_PMC->LVSC & PMC_LVSC_RAMP_UP_RESET_FLAGS_RWBITS_MASK))
    {
        IP_PMC->LVSC = PMC_LVSC_RWBITS_MASK;
    }
#if defined(POWER_IP_DERIVATIVE_001) || defined(POWER_IP_DERIVATIVE_006)
    /* Initialize the rest of the PMC module */
    IP_PMC->CONFIG = ConfigPtr->ConfigRegister;
#else
    /* Initialize the rest of the PMC module  */
    if (PMC_CONFIG_LAST_MILE_REG_ENABLE == (ConfigPtr->ConfigRegister & PMC_CONFIG_LMEN_MASK))
    {
        IP_PMC->CONFIG = (uint32)(ConfigPtr->ConfigRegister & (~(uint32)PMC_CONFIG_LMEN_MASK));

        /* If external BJT is using on the PCB board, the program needs to wait for the LVD15S bit to be cleared, then LMEN is enabled */
        if(PMC_CONFIG_LM_BASE_CONTROL_ENABLE == (ConfigPtr->ConfigRegister & PMC_CONFIG_LMBCTLEN_MASK))
        {
            Power_Ip_StartTimeout(&StartTime, &ElapsedTime, &TimeoutTicks, POWER_IP_TIMEOUT_VALUE_US);
            do
            {
                TimeoutOccurred = Power_Ip_TimeoutExpired(&StartTime, &ElapsedTime, TimeoutTicks);
    
                LvscValue = IP_PMC->LVSC;
            } while ( (PMC_LVSC_V15_ABOVE_LV != (LvscValue & PMC_LVSC_LVD15S_MASK)) && (!TimeoutOccurred) );
      
            if (!TimeoutOccurred)
            {
                ConfigValue = IP_PMC->CONFIG;
                ConfigValue = (uint32)(ConfigValue & (~(uint32)PMC_CONFIG_LMEN_MASK));
                ConfigValue = (uint32)(ConfigValue | (uint32)(PMC_CONFIG_LAST_MILE_REG_ENABLE & PMC_CONFIG_LMEN_MASK));
                IP_PMC->CONFIG = ConfigValue;
            }
            else
            {
                Power_Ip_ReportPowerErrors(POWER_IP_REPORT_TIMEOUT_ERROR, POWER_IP_ERR_CODE_RESERVED);
            }
        }
        else
        {
            ConfigValue = IP_PMC->CONFIG;
            ConfigValue = (uint32)(ConfigValue & (~(uint32)PMC_CONFIG_LMEN_MASK));
            ConfigValue = (uint32)(ConfigValue | (uint32)(PMC_CONFIG_LAST_MILE_REG_ENABLE & PMC_CONFIG_LMEN_MASK));
            IP_PMC->CONFIG = ConfigValue;
        }
    }
    else
    {
        IP_PMC->CONFIG = ConfigPtr->ConfigRegister;

        if (PMC_CONFIG_LAST_MILE_REG_AUTO_ENABLE == (ConfigPtr->ConfigRegister & PMC_CONFIG_LMAUTOEN_MASK))
        {
            Power_Ip_StartTimeout(&StartTime, &ElapsedTime, &TimeoutTicks, POWER_IP_TIMEOUT_VALUE_US);
            do
            {
                TimeoutOccurred = Power_Ip_TimeoutExpired(&StartTime, &ElapsedTime, TimeoutTicks);

                LastMileRegValue = (IP_PMC->CONFIG & PMC_CONFIG_LMSTAT_MASK);
            } while ((PMC_CONFIG_LAST_MILE_REG_ON != LastMileRegValue) && (!TimeoutOccurred));

            if(TimeoutOccurred)
            {
                Power_Ip_ReportPowerErrors(POWER_IP_REPORT_TIMEOUT_ERROR, POWER_IP_ERR_CODE_RESERVED);
            }
            else
            {
                /* do nothing */
            }
        }
    }
#endif
#if (defined(POWER_IP_VOLTAGE_ERROR_ISR_USED))
  #if (POWER_IP_VOLTAGE_ERROR_ISR_USED == STD_ON)
    /* make Status of PMC to initialized to check in the interrupt function */
    Power_Ip_ePMCStatus = PMC_INIT;
  #endif
#endif

}


#if (defined(POWER_IP_ENTER_LOW_POWER_MODE) && (POWER_IP_ENTER_LOW_POWER_MODE == STD_ON))
/**
* @brief            This function prepares the PMC module for Standby/Low Power entry.
* @details          If the Lastmile Regulator is enabled, clear both PMC_LVSC[LMEN]
*                   and PMC_LSVC[LMBCTLEN] simultaneously and notify the application
*                   of the regulator disablement. The LVD/HVD Interrupts are also
*                   disabled.
*
* @param[in]        void
*
* @return           void
*
*/
void Power_Ip_PMC_PrepareLowPowerEntry(void)
{
    uint32 TempValue = 0U;

    /* Disable LVD/HVD Interrupts */
    TempValue = IP_PMC->CONFIG;
    TempValue = (uint32)(TempValue & (uint32)( ~(PMC_CONFIG_LVDIE_MASK | PMC_CONFIG_HVDIE_MASK) ) );
    TempValue = (uint32)(TempValue | (uint32)(PMC_CONFIG_LVD_INTERRUPTS_DISABLE | PMC_CONFIG_HVD_INTERRUPTS_DISABLE));
    IP_PMC->CONFIG = TempValue;

#if !(defined(POWER_IP_DERIVATIVE_001) || defined(POWER_IP_DERIVATIVE_006))
    /* Disable LM Regulator, if enabled, and notify the application */
    if ((PMC_CONFIG_LAST_MILE_REG_ENABLE == ((uint32)(IP_PMC->CONFIG & PMC_CONFIG_LMEN_MASK))) || \
        (PMC_CONFIG_LAST_MILE_REG_AUTO_ENABLE == ((uint32)(IP_PMC->CONFIG & PMC_CONFIG_LMAUTOEN_MASK))))
    {
        TempValue = IP_PMC->CONFIG;
        TempValue = (uint32)(TempValue & (uint32)( ~(PMC_CONFIG_LMBCTLEN_MASK | PMC_CONFIG_LMEN_MASK | PMC_CONFIG_LMAUTOEN_MASK) ) );
        TempValue = (uint32)(TempValue | (uint32)(PMC_CONFIG_LM_BASE_CONTROL_DISABLE | PMC_CONFIG_LAST_MILE_REG_DISABLE | PMC_CONFIG_LAST_MILE_REG_AUTO_DISABLE));
        IP_PMC->CONFIG = TempValue;

        /* Notify the application of the regulator disablement to further signal the SBC, if needed */
#ifdef POWER_IP_PMC_NOTIFICATION
        POWER_IP_PMC_NOTIFICATION(POWER_IP_LAST_MILE_REGULATOR_DISABLED);
#else
        Power_Ip_ReportPowerErrors(POWER_IP_PMC_ERROR, POWER_IP_LAST_MILE_REGULATOR_DISABLED);
#endif
    }
#endif
}
#endif /* (POWER_IP_ENTER_LOW_POWER_MODE == STD_ON) */

#if (defined(POWER_IP_VOLTAGE_ERROR_ISR_USED))
  #if (POWER_IP_VOLTAGE_ERROR_ISR_USED == STD_ON)
/**
* @brief            This function handles the voltage error detection.
*
* @return           void
*
* @isr
* @implements Power_Ip_PMC_VoltageErrorIsr_Activity
*/
void Power_Ip_PMC_VoltageErrorIsr(void)
{
    uint32 RegValue;
    uint32 VoltageIsrEnabled;
    uint32 VoltageIsrStatus;
    uint32 EventReport;
    RegValue = IP_PMC->LVSC;

    VoltageIsrEnabled = RegValue & PMC_LVSC_OV_UV_IRQ_FLAGS_MASK32;

    IP_PMC->LVSC = VoltageIsrEnabled;

    if (PMC_UNINIT != Power_Ip_ePMCStatus)
    {

        VoltageIsrStatus = RegValue & PMC_LVSC_OV_UV_STATUS_FLAGS_MASK32;

        RegValue = IP_PMC->CONFIG;

        if (PMC_CONFIG_LVD_INTERRUPTS_DISABLE == (RegValue & PMC_CONFIG_LVDIE_MASK))
        {
            VoltageIsrEnabled = (VoltageIsrEnabled & (uint32)(~PMC_LVSC_UV_IRQ_FLAGS_MASK32));
        }

        if (PMC_CONFIG_HVD_INTERRUPTS_DISABLE == (RegValue & PMC_CONFIG_HVDIE_MASK))
        {
            VoltageIsrEnabled = (VoltageIsrEnabled & (uint32)(~PMC_LVSC_OV_IRQ_FLAGS_MASK32));
        }

        /* Align IRQ Flags with Status flags */
        VoltageIsrStatus = (VoltageIsrStatus >> 8U);
        EventReport = (VoltageIsrStatus & VoltageIsrEnabled);

        /* Check if there is any event to report */
        if ((uint32)0U != EventReport)
        {
            if (((EventReport & PMC_LVSC_UV_IRQ_FLAGS_MASK32) != (uint32)0U) && \
                ((EventReport & PMC_LVSC_OV_IRQ_FLAGS_MASK32) != (uint32)0U))
            {
#ifdef POWER_IP_ERROR_ISR_NOTIFICATION
                POWER_IP_ERROR_ISR_NOTIFICATION(POWER_IP_E_ISR_VOLTAGE_ERROR);
#else
                Power_Ip_ReportPowerErrors(POWER_IP_ISR_ERROR, POWER_IP_E_ISR_VOLTAGE_ERROR);
#endif
            }
            else if ((EventReport & PMC_LVSC_UV_IRQ_FLAGS_MASK32) != (uint32)0U)
            {
#ifdef POWER_IP_ERROR_ISR_NOTIFICATION
                POWER_IP_ERROR_ISR_NOTIFICATION(POWER_IP_E_ISR_LOW_VOLTAGE_ERROR);
#else
                Power_Ip_ReportPowerErrors(POWER_IP_ISR_ERROR, POWER_IP_E_ISR_LOW_VOLTAGE_ERROR);
#endif
            }
            else
            {
#ifdef POWER_IP_ERROR_ISR_NOTIFICATION
                POWER_IP_ERROR_ISR_NOTIFICATION(POWER_IP_E_ISR_HIGH_VOLTAGE_ERROR);
#else
                Power_Ip_ReportPowerErrors(POWER_IP_ISR_ERROR, POWER_IP_E_ISR_HIGH_VOLTAGE_ERROR);
#endif
            }

        }
    }
}
  #endif
#endif /* (POWER_IP_VOLTAGE_ERROR_ISR_USED == STD_ON) */

#define MCU_STOP_SEC_CODE

#include "Mcu_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

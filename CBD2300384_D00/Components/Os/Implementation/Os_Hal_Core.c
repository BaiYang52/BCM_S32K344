/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup  Os_Hal_Core
 *  \{
 *
 *  \file        Os_Hal_Core.c
 *  \brief       Core related primitives which don't have to be inlined.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */


#define OS_HAL_CORE_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Hal_Derivative.h"
#include "Os_Hal_Core.h"
#include "Os_Hal_Interrupt.h"

/* Os kernel module dependencies */

/* Os hal dependencies */
#include "Os_Hal_Os.h"
#include "Os_Hal_Cfg.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/* Check the version of OS header file */
#if (  (OS_HAL_SW_MAJOR_VERSION != (2u)) \
    || (OS_HAL_SW_MINOR_VERSION != (72u)) \
    || (OS_HAL_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Os_Hal_Core.c and Os_Hal_Os.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (OS_CFG_HAL_MAJOR_VERSION != (2u)) \
    || (OS_CFG_HAL_MINOR_VERSION != (53u)) )
# error "Version numbers of Os_Hal_Core.c and Os_Hal_Cfg.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_CoreInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Hal_CoreInit
(
  P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config
)
{
  /* #10 Initialize exception handling. */
  Os_Hal_Core_InitExceptions(Config);                                                                                   /* SBSW_OS_HAL_FC_CALLER */

  /* #20 Initialize interrupt handling. */
  Os_Hal_INTC_InitializeVectorTable(Config->VectorTable);                                                               /* SBSW_OS_HAL_FC_CALLER */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreInitHardware()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Hal_CoreInitHardware(P2CONST(Os_Hal_SystemConfigType, AUTOMATIC, OS_CONST) Config)
{
  /* #10 Nop. */
  OS_IGNORE_UNREF_PARAM(Config);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreFreeze()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_Hal_CoreFreeze,
(
  void
))                                                                                                                      /* COV_OS_HALARMCMCOREFREEZE */
{
  /* #10 Loop forever. */
  while( (1 == 1) )                                                                                                     /* COV_OS_HALARMCMCOREFREEZE */ /* FETA_OS_HAL_COREFREEZE */
  {
    Os_Hal_Nop();
  }
}


#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* START_COVERAGE_JUSTIFICATION

\ID COV_OS_HALARMCMCOREFREEZE
  \ACCEPT XX
  \REASON This function is called from Os_ErrKernelPanic() right after the panic hook has returned. As the panic hook
          is the last possible exit point for the tests, Os_Hal_CoreFreeze cannot be covered in tests. Because of the
          simplicity of the code, the performed code inspection results in sufficient quality.

Justifications for core:

\ID COV_OS_HALPLATFORMCLZAVAILABILITY
  \ACCEPT TX
  \REASON The condition is hardware dependent always true. The unnecessary code parts should be
          removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
          always just test the code for the respective platform. No risk as the remaining code shall be optimized
          away. (Other platforms may set ACCEPT XF with a similar justification)

\ID COV_OS_HALPLATFORMPRIVILEGEMODEUSERREADABILITY
  \ACCEPT TX
  \REASON The condition is hardware dependent always true. The unnecessary code parts should be
          removed by compiler optimization. As the tests run on each platform, the OS is to be released for, we
          always just test the code for the respective platform. No risk as the remaining code shall be optimized
          away.

\ID COV_OS_HALPLATFORMTHREADPOINTERUSERREADABILITY_XX
  \ACCEPT XX
  \REASON Due to the hardware ability to read the current privileges directly, this function is never called. The
          unnecessary code parts should be removed by compiler optimization. As the tests run on each platform, the OS
          is to be released for, we always just test the code for the respective platform. No risk as the remaining code
          shall be optimized away.

\ID COV_OS_HALPLATFORMBITLIBCLZ
  \ACCEPT XX
  \REASON This code is meant as library code for the software implementation of Os_Bit_CountLeadingZeros(). As this
          processor provides a hardware solution, this code is unused. However, the function is extremely simple,
          code inspected and unit tested. It is typically optimized away by the linker. Therefore, the risk of
          keeping this code is expected to be low.

\ID COV_OS_HALPLATFORMUNHANDLEDSYSCALL
  \ACCEPT TF
  \REASON This function is only reacheable on some platforms. However, unit test and code inspection have not found
          any risk in keeping this code. The function is fully tested on those platforms which have potentially
          unhandled system calls.

\ID COV_OS_HALPLATFORMAPPISPRIVILEGED
  \ACCEPT XX
  \REASON Platform is able to read privileges from hardware. Thus, this function is never used for determination of
          current privileges.

\ID COV_OS_HALPLATFORMGETEXCEPTIONCONTEXTAVAILABILITY
   \ACCEPT XF
   \REASON The feature "read exception context" is available on this platform.

\ID COV_OS_HALPLATFORMSETEXCEPTIONCONTEXTAVAILABILITY
   \ACCEPT TX
   \ACCEPT XX
   \REASON The feature "modify exception context" is not available on this platform.

\ID COV_OS_HALPLATFORM_NEED_NO_TRAP_IOC_MULTIQUEUE
  \ACCEPT XF
  \ACCEPT XX
  \REASON This path can never be reached on this platform. As IsTrapRequired is always false for multi-queue IOCs
          and the thread pointer is readable in user mode on this platform, no trap is necessary. However, the code is
          unit tested as well as tested on other platforms. Aditionally it is similar to the code used for the call of
          other OS API functios. So there is no risk in keeping this code.

\ID COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED
   \ACCEPT XF
   \ACCEPT XX
   \REASON Global to level switch is not supported on this platform.

\ID COV_OS_HALPLATFORMFRT64BITAVAILABILITY
   \ACCEPT XF
   \ACCEPT XX
   \REASON The feature "64-bit FRT timer" is not available on this platform.

\ID COV_OS_HALPLATFORMUNUSED
   \ACCEPT XX
   \REASON The feature is currently unused and will be removed with OSC-5157.
           The unnecessary code parts should be removed by compiler optimization.

\ID COV_OS_HALPLATFORMISRPOSTACTIONAVAILABILITY
 \ACCEPT XX
 \ACCEPT XF
 \REASON This derivative does not meet the hardware requirements to support interrupt post action handling.
         Functionality has been tested on another derivative.

END_COVERAGE_JUSTIFICATION */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_HAL_PWA_CALLER
 \DESCRIPTION    A write access is performed on a pointer, which is coming directly from the caller.
 \COUNTERMEASURE \N No measure is required, because the caller has to check the validity.

\ID SBSW_OS_HAL_FC_CALLER
 \DESCRIPTION    A function is called with one or more pointer parameters, which are coming directly from the caller.
 \COUNTERMEASURE \N No measure is required, because the caller has to check the validity.

\ID SBSW_OS_HAL_FC_LOCAL_VAR
 \DESCRIPTION    A function is called with one or more pointer parameters, which are pointing to local variables.
 \COUNTERMEASURE \N No measure is required, because local variables are always valid.

\ID SBSW_OS_HAL_FC_MPUCONFIG
 \DESCRIPTION    A function is called with a pointer parameter.
 \COUNTERMEASURE \N No measure required, because this pointer will not be used for write operation by the callee.

\ID SBSW_OS_HAL_FC_STACKCONFIG
 \DESCRIPTION    A function is called with a pointer parameter. The pointer references a mandatory structure inside the
                 CONST section. The array access is always valid because Os_Hal_CoreGetId always returns 0.
 \COUNTERMEASURE \N No measure required, because this pointer will not be used for write operation by the callee.

\ID SBSW_OS_HAL_INTC_INITIALIZEVECTORTABLE_001
 \DESCRIPTION    A write access is performed on the Vector Table Offset Register (VTOR). The validity of
                 OS_HAL_INTC_SCB_VTOR is checked by manual test. Additionally the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_IPR
 \DESCRIPTION    A write access is performed on the Interrupt Priority Registers (IPR). The validity of
                 OS_HAL_INTC_NVIC_IPR() is checked by manual test. Additionally, the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_ISER
 \DESCRIPTION    A write access is performed on the Interrupt Set-Enable Registers (ISER). The validity of
                 OS_HAL_INTC_NVIC_ISER() is checked by manual test. Additionally, the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_ICER
 \DESCRIPTION    A write access is performed on the Interrupt Clear-Enable Registers (ICER). The validity of
                 OS_HAL_INTC_NVIC_ICER() is checked by manual test. Additionally, the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_ICPR
 \DESCRIPTION    A write access is performed on the Interrupt Clear-Pending Registers (ICPR). The validity of
                 OS_HAL_INTC_NVIC_ICPR() is checked by manual test. Additionally, the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_ISPR
 \DESCRIPTION    A write access is performed on the Interrupt Set-Pending Registers (ISPR). The validity of
                 OS_HAL_INTC_NVIC_ISPR() is checked by manual test. Additionally, the Safety manual should state that
                 the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

 \ID SBSW_OS_HAL_INTC_NVIC_STEER
 \DESCRIPTION    A write access is performed on the Steer Module Registers (IRQSTEER). The validity of
                 OS_HAL_INTC_STEER_BASE and register write implementation in the static code is checked by manual test.
                 Additionally, the Safety manual should state that the functionality of each configured ISR has to be
                 checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_INTC_NVIC_CPUSS
 \DESCRIPTION    A write access is performed on the Interrupt Control CPUSS Registers (CPUSS). The validity of
                 OS_HAL_CPUSS_CM4_SYSTEM_INT_CTL() is checked by manual test. Additionally, the Safety manual should
                 state that the functionality of each configured ISR has to be checked for its correctness.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.
                 \S [CM_OS_HAL_ISRCONFIG_S]

\ID SBSW_OS_HAL_CTRL_CPUSS
 \DESCRIPTION    A write access is performed on System Cortex-M7 control register. The validity of
                 OS_HAL_CPUSS_CM7_CTL() is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORE_INITEXCEPTIONS_001
 \DESCRIPTION    A write access is performed on the System Handler Configuration Registers. The validity of the
                 base address OS_HAL_SCS_SH_BASE_ADDRESS is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MP_MPU_CTRL
 \DESCRIPTION    A write access is performed on the MPU Control Register (MPU_CTRL). The validity of the
                 address OS_HAL_MP_MPU_CTRL is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MP_MPU_RNR
 \DESCRIPTION    A write access is performed on the MPU Region Number Register (MPU_RNR). The validity of the
                 address OS_HAL_MP_MPU_RNR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MP_MPU_RASR
 \DESCRIPTION    A write access is performed on the MPU Region Attribute and Size Register (MPU_RASR).
                 The validity of the address OS_HAL_MP_MPU_RASR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MP_MPU_RBAR
 \DESCRIPTION    A write access is performed on the MPU Region Base Address Register (MPU_RBAR). The validity of the
                 address OS_HAL_MP_MPU_RBAR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORESETTHREAD_001
 \DESCRIPTION    A write access is performed to a Core2Thread pointer type. The Core2Thread object is derived
                 with an index from the Core2Thread array.
 \COUNTERMEASURE \S Safety manual should state that the generated OsCfg_Hal_Core2Thread object of type
                 Os_Hal_Core2ThreadConfigType in _Os_Hal_Core_Lcfg.c_ has to be checked for its correctness.
                 The array Core2Thread in the object OsCfg_Hal_Core2Thread contains all physical cores.
                 For each existing physical hardware core identifier there shall be one corresponding entry inside
                 the array which is indexed by the physical hardware core identifier provided directly by the
                 hardware registers. SMI-30843

\ID SBSW_OS_HAL_TIMER_SYSTICK_SYST_CSR
 \DESCRIPTION    A write access is performed on the SysTick Control and Status Register (SYST_CSR). The validity of the
                 address OS_HAL_TIMER_SYSTICK_SYST_CSR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_TIMER_SYSTICK_SYST_RVR
 \DESCRIPTION    A write access is performed on the SysTick Reload Value Register (SYST_RVR). The validity of the
                 address OS_HAL_TIMER_SYSTICK_SYST_RVR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_TIMER_SYSTICK_SYST_CVR
 \DESCRIPTION    A write access is performed on the SysTick Current Value Register (SYST_CVR). The validity of the
                 address OS_HAL_TIMER_SYSTICK_SYST_CVR is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CONTEXTSUSPEND
 \DESCRIPTION    A write access is performed on a pointer, which is coming directly from the caller. All passed pointer
                 reference members of the Os_Hal_ContextType. The pointer to this data type comes directly from the
                 caller.
 \COUNTERMEASURE \N No measure is required, because the caller has to check the validity.

\ID SBSW_OS_HAL_FC_POINTER2LOCAL
 \DESCRIPTION    Pass a reference to a local variable.
 \COUNTERMEASURE \N No measure required, as the passed pointer refers to a local variable and the called function
                    allows that the existence of the local variable ends on return.

\ID SBSW_OS_HAL_SMPU_ADDR
 \DESCRIPTION    Write address is performed on a pointer to an SMPU registers. (from Os_Hal_MemoryProtection_NXP.h;
                 Base addresses are defined within the derivative headers)
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_TCPWM_MODULETYPE
 \DESCRIPTION    A write access is performed on a member of the struct Os_Hal_TcpwmModuleType that is mapped to
                 TCPWM hardware registers addresses: their validity is ensured by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_TCPWM_COUNTER
 \DESCRIPTION    A write access is performed on the array Os_Hal_TcpwmCounterType.
                 The indexes used is statically defined and manual test ensures that they are between
                 0 and OS_HAL_TCPWM_COUNTER_COUNT - 1.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\CM CM_OS_HAL_ISRCONFIG_S
                 Safety manual should state that the functionality of each configured ISR has to be checked for its
                 correctness. This includes the correct call of the ISR handler as well as enabling, disabling,
                 reading the enable state, reading the pending state and clearing of the pending information of
                 the corresponding ISR sources. SMI-491

\ID SBSW_OS_HAL_TIMER_CONFIG
 \DESCRIPTION    A write access is performed on a pointer, which is part of the generated timer configuration.
 \COUNTERMEASURE \S Safety manual should state that the user shall assure the correctness of the FRT timer
                 configuration. If the FTM FRT timer is used by the OS the following configuration struct is generated
                 into Os_Hal_Timer_Lcfg.c:
                 CONST(Os_Hal_TimerFrtConfigType, OS_CONST) OsCfg_Hal_TimerFrt_<CounterName>
                 The following struct members must be checked (where <X> (0, 2, 4, 6) is the channel ID)
                 and <y> is the timer module):
                 ModuleControlRegisterAddress  = OS_HAL_TIMER_FTM<Y>_BASE
                 ModuleCountRegisterAddress    = OS_HAL_TIMER_FTM<Y>_BASE + OS_HAL_TIMER_FTM_COUNTREGISTER_OFFSET
                 ChannelControlRegisterAddress = OS_HAL_TIMER_FTM<Y>_BASE + OS_HAL_TIMER_FTM_CHANNEL<X>_OFFSET
                 ChannelCompareRegisterAddress = OS_HAL_TIMER_FTM<Y>_BASE + OS_HAL_TIMER_FTM_CHANNEL<X>_OFFSET
                                                                          + OS_HAL_TIMER_FTM_CHANNEL_CMP_OFFSET
                 SMI-333241

\ID SBSW_OS_HAL_FRT_SC
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_SC). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_CONF
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_CONF). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_QDCTRL
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_QDCTRL). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_C0SC
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_C0SC). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_C0V
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_C0V). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_CNT
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_CNT). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_MOD
 \DESCRIPTION    A write access is performed on the FRT Register (OS_HAL_TIMER_TPM_FRT_MOD). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_FRT_NXP_GPT_REG_ACCESS
 \DESCRIPTION    A write access is performed on an FRT NXP GPT timer register . The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_SPINLOCK_SEMA42_RSTGT
 \DESCRIPTION    A write access is performed on the SEMA42 Register (OS_HAL_SEMA42_RSTGT). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_SPINLOCK_SEMA42_GATE
 \DESCRIPTION    A write access is performed on the SEMA42 Register (OS_HAL_SEMA42_GATE). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_XSIGNAL_MSCM_IRCPGR
 \DESCRIPTION    A write access is performed on the MSCM Register (OS_HAL_MSCM_IRCPGR). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_XSIGNAL_MSCM_IRCPSR
 \DESCRIPTION    A write access is performed on the MSCM Register (OS_HAL_MSCM_IRCPSR). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_ADDR
 \DESCRIPTION    A write access is performed on the MC_ME Register (MC_ME_PRTN0_CORE_ADDR). The address
                 is generated in the corresponding core configuration of type Os_Hal_CoreConfigType.
 \COUNTERMEASURE \S Safety manual should state that the user shall assure the correctness of address
                 configuration. For all non-autostarted Autosar cores the address should be the
                 base address of the interrupt vector table, otherwise configured entry symbol for the
                 corresponding core is used (value of OsPhysicalCoreDefaultEntrySymbol). In the first case
                 the following struct shall be checked in Os_Hal_Core_Lcfg.c (where <X> is the logical core
                 ID of the checked core) CONST(Os_Hal_CoreConfigType, OS_CONST) OsCfg_Hal_Core_OsCoreX = {
                 X, (uint32)(&OsCfg_Hal_Core_OsCoreX_VectorTable)}. SMI-431712

\ID SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_PCONF
 \DESCRIPTION    A write access is performed on the MC_ME Register (MC_ME_PRTN0_CORE_PCONF). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORECONFIG_MC_ME_PRTN0_COREX_PUPD
 \DESCRIPTION    A write access is performed on the MC_ME Register (MC_ME_PRTN0_CORE_PUPD). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORECONFIG_MC_ME_CTL_KEY
 \DESCRIPTION    A write access is performed on the MC_ME Register (OS_HAL_MC_ME_CTL_KEY). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_CORECONFIG_MC_RGM_PRST_BASE
 \DESCRIPTION    A write access is performed on the MC_RGM Register (OS_HAL_RGM_PRST_BASE). The validity of the
                 address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MU0_A_ACR
 \DESCRIPTION    A write access is performed on the Messaging Unit Processor A Control Register
                (SBSW_OS_HAL_MU0_A_ACR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MU0_B_BSR
 \DESCRIPTION    A write access is performed on the Messaging Unit Processor B Status Register
                (SBSW_OS_HAL_MU0_B_BSR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MU0_B_BCR
 \DESCRIPTION    A write access is performed on the Messaging Unit Processor Control Register
                (SBSW_OS_HAL_MU0_B_BCR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_IPC_STRUCT_RELEASE
 \DESCRIPTION    A write access is performed on the IPC channel lock release register
                (OS_HAL_IPC_STRUCT_X_RELEASE). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_IPC_STRUCT_NOTIFY
 \DESCRIPTION    A write access is performed on the IPC notification register of the corresponding channel
                (OS_HAL_IPC_STRUCT_NOTIFY). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_IPC_INTR_STRUCT_INTR
 \DESCRIPTION    A write access is performed on the IPC interrupt status register of the corresponding interrupt
                (OS_HAL_IPC_INTR_STRUCT_INTR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_IPC_INTR_STRUCT_INTR_MASK
 \DESCRIPTION    A write access is performed on the IPC interrupt mask register of the corresponding interrupt
                (OS_HAL_IPC_INTR_STRUCT_INTR_MASK). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MU_CR
 \DESCRIPTION    A write access is performed on the Messaging Unit Processor A or Processor B Control Register
                (OS_HAL_MU_CR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_MU_BSR
 \DESCRIPTION    A write access is performed on the Messaging Unit Processor B Status Register
                (OS_HAL_MU_SR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_IOMUXC_LPSR_GPR_GPR
 \DESCRIPTION    A write access is performed on the IOMUX Controller LPSR GPR General purpose Register
                (OS_HAL_IOMUXC_LPSR_GPR_GPRn, n=[0,1]). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

\ID SBSW_OS_HAL_SRC_SCR
 \DESCRIPTION    A write access is performed on the System Reset Controller Control Register
                (OS_HAL_SRC_SCR). The validity of the address macro is checked by manual test.
 \COUNTERMEASURE \T TCASE-910391 ensures that the hardware access is correct.

SBSW_JUSTIFICATION_END */

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_OS_HAL_COREFREEZE
 \DESCRIPTION    OS core freeze loop being used in case of inconsistent internal state is recognized by
                 MICROSAR Classic OS.
 \COUNTERMEASURE \N None as the loop is endless by requirement. System reset will occur by a watchdog at this point.

\ID FETA_OS_HAL_SPINLOCK_AQUIRE
 \DESCRIPTION    The loop performs a busy wait until the spinlock is successfully acquired.
 \COUNTERMEASURE \N None as the busy wait is the intended behaviour.

\ID FETA_OS_HAL_SPINLOCK_INIT
 \DESCRIPTION    The loop performs a busy wait until the spinlock peripheral is properly initialized.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since
                    the OS has not been started and no other components are running.

\ID FETA_OS_HAL_XSIGNAL_INIT
 \DESCRIPTION    The loop performs a busy wait until the message unit peripheral is properly initialized.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since
                    the OS has not been started and no other components are running.

\ID FETA_OS_HAL_MPU
 \DESCRIPTION    The loop iterates over all configured MPU region descriptors.
 \COUNTERMEASURE \N None as the maximum number of iterations is fixed.

\ID FETA_OS_HAL_MCME_CLK
 \DESCRIPTION    The loop waits until the clock is initialized for the corresponding core.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since
                    the OS has not been started and no other components are running.

\ID FETA_OS_HAL_MCME_PROCESS
 \DESCRIPTION    The loop waits Until triggering the hardware process is accomplished.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since
                    the OS has not been started and no other components are running.

\ID FETA_OS_HAL_RGM
 \DESCRIPTION    The loop waits until a core reset is deasserted on the hardware.
 \COUNTERMEASURE \N None as freedom from interference with respect to timing is not violated since
                    the OS has not been started and no other components are running.

FETA_JUSTIFICATION_END */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core.c
 **********************************************************************************************************************/

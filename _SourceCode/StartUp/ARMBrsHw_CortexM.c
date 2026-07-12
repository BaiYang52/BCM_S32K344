/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  ARMBrsHw_CortexM.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for all platforms with ARM core Cortex-M
 *     Template:  This file is reviewed according to Brs_Template@Implementation[1.03.05]
 *
 *  \brief Description:  This is a global, platform-independent file for the ARM-BRS.
 *                       This file includes all non-platform dependent functions.
 *                       All the (platform depending) rest needs to be defined in BrsHw.c
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to ARMBrsHw_CortexM.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EXAMPLE CODE ONLY
 *  -------------------------------------------------------------------------------------------------------------------
 *  This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
 *  The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
 *  intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
 *  implementations must be tested with diligent care and must comply with all quality requirements which are necessary
 *  according to the state of the art before their use.
 *********************************************************************************************************************/

#define ARMBRSHW_CORTEXM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsHw.h"

#if !defined (BRS_ENABLE_FBL_SUPPORT)
  #include "Os.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (ARMBRSHW_CORTEXM_VERSION != 0x0204u)
  #error "Header and source file are inconsistent!"
#endif
#if (ARMBRSHW_CORTEXM_BUGFIX_VERSION != 0x00u)
  #error "Different versions of bugfix in Header and Source used!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)                  

#else
  #error "Unknown compiler specified!"
#endif

#if defined (BRS_CPU_CORE_CORTEX_M0)     || \
    defined (BRS_CPU_CORE_CORTEX_M0PLUS) || \
    defined (BRS_CPU_CORE_CORTEX_M23)
  /* primask is used instead of IPR in BrsHwDisableInterruptAtPowerOn() */

#elif defined (BRS_CPU_CORE_CORTEX_M3)  || \
      defined (BRS_CPU_CORE_CORTEX_M4)  || \
      defined (BRS_CPU_CORE_CORTEX_M4F) || \
      defined (BRS_CPU_CORE_CORTEX_M7)
  /* The processor architecture supports a maximum of 60*4=240 external interrupts (8 prio bits each) (NVIC_IPR0 - NVIC_IPR59) */
  #define BRSHW_MCORE_NVIC_MAX_IPR_NO 59

#elif defined (BRS_CPU_CORE_CORTEX_M33)
  /* The processor architecture supports a maximum of 120*4=480 external interrupts (8 prio bits each) (NVIC_IPR0 - NVIC_IPR119) */
  #define BRSHW_MCORE_NVIC_MAX_IPR_NO 119

#else
  #error "Your configured Cortex-M core architecture is not yet supported by this implementation."
#endif


/**********************************************************************************************************************
 *  DEFINITION + MACROS
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)
  #define __asm  asm
#endif

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONST VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL VARIABLES AND LOCAL HW REGISTERS
 *********************************************************************************************************************/
/* Needed for BASEPRI analyzation within BrsHwDisableInterruptAtPowerOn() */
volatile uint8 brsDisIntPrioTmp;
volatile uint8 brsDisIntITmp;

/**********************************************************************************************************************
 *  CONTROLLER CONFIGURATION REGISTERS
 *********************************************************************************************************************/
/* lowest 4 bit -> INTLINESNUM -> No. of last implemented NVIC_IPR-Register (4 int prios / 32bit-Register) */
#define BRSHW_MCORE_NVIC_ICTR    BRSHW_IOS(uint32, (NVIC_BASE + 0x4UL))
#define BRSHW_MCORE_NVIC_IPR(X)  BRSHW_IOS(uint32, (NVIC_BASE + 0x400UL+(4*X)))

/* Vector Table Offset Register (VTOR). */
#define BRSHW_VTOR               BRSHW_IOS(uint32, NVIC_VTOR)

/* Reset control registers */
#define BRSHW_MCORE_SCS_AIRCR    BRSHW_IOS(uint32, NVIC_BASE + 0xD0CUL)  /* Reset control registers for Cortex-M */

/**********************************************************************************************************************
 *  LOCAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONST VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  PROTOTYPES OF GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  PROTOTYPES OF LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*****************************************************************************/
/* @brief      Disable the global system interrupt.
 * @pre        Must be the first function call in main@BrsMain
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHwDisableInterruptAtPowerOn(void)
{
#if defined (BRS_CPU_CORE_CORTEX_M0) || defined (BRS_CPU_CORE_CORTEX_M0PLUS) || defined (BRS_CPU_CORE_CORTEX_M23)
  /* Global disable IRQs, EXCEPT HardFault and NMI! */
# if defined (BRS_COMP_IAR) 
  __asm("PUSH {R4}");
  __asm("MOVS R4, #1");
  __asm("MSR primask,  R4");
  __asm("POP {R4}");

# else
  #error "Cortex-M0 support not yet verified for your compiler!"
# endif

#else
  uint8 brsIdx;
  brsDisIntITmp = 0; /* prevent compiler warning */
  uint32 brsNvicIprTmpValue;
  uint8 brsNvicIprAmount;

  /* get pointer to brsDisIntITmp */
# if defined (BRS_COMP_IAR)
  __asm("PUSH {R2,R4}");
  /* Load pointer into R4 */
  asm("ORR R4, %0, %0 " : : "r"(&brsDisIntITmp) : );
  /* analyze amount of supported bits in priority registers and BASEPRI
   * (BASEPRI bits not supported are "Read All Zero, Ignore Writes") */
  /* BASEPRI = 255 */
  asm("MOV  R2, #255");
  asm("MSR  basepri, R2");
  /* brsDisIntITmp = BASEPRI */
  asm("MRS  R2, basepri");
  asm("STRB R2, [R4]");
  __asm("POP {R2,R4}");

# else
  __asm("PUSH {R2,R4}");
  __as1(LDR R4, =brsDisIntITmp)
  /* analyze amount of supported bits in priority registers and BASEPRI
   * (BASEPRI bits not supported are "Read All Zero, Ignore Writes") */
   /* BASEPRI = 255 */
  __as1(MOV R2, #255)
  __as1(MSR basepri, R2)
  /* brsDisIntITmp = BASEPRI */
  __as1(MRS R2, basepri)
  __as1(STRB R2, [R4])
  __asm("POP {R2,R4}");
# endif /*BRS_COMP_x*/

  brsDisIntPrioTmp = 0xFF;
  for (brsIdx = 0; brsIdx < 8; brsIdx++)
  {
    if (((uint8)(1 << brsIdx)) & brsDisIntITmp) 
    {
      brsDisIntPrioTmp = (uint8)(1 << brsIdx);
      break;
    }
  }
  if (brsDisIntPrioTmp == 0xFF)
  {
    /* BASEPRI not writable! */
    while (1);
  }

  /* get pointer to brsDisIntPrioTmp */
# if defined (BRS_COMP_IAR)
  __asm("PUSH {R2,R4}");
  /* Load pointer and end into R4. ORR is only a helper construct */
  asm("ORR R4, %0, %0 " : : "r"(&brsDisIntPrioTmp) : );
  asm("LDRB R2, [R4]");
  /* BASEPRI = brsDisIntPrioTmp */
  asm("MSR basepri, R2");
  __asm("POP {R2,R4}");

# else
  __asm("PUSH {R2,R4}");
  __as1(LDR R4, =brsDisIntPrioTmp)
  __as1(LDRB R2, [R4])
  /* BASEPRI = brsDisIntPrioTmp */
  __as1(MSR basepri, R2)
  __asm("POP {R2,R4}");
# endif /*BRS_COMP_x*/

  /* Every NVIC_IPR register configures the priority of 4 interrupts (8 bit each).
     Initialize brsNvicIprTmpValue to configure 4 interrupts at once, all with same priority */
  brsIdx = brsDisIntPrioTmp; /* brsIdx used, to prevent compiler warning because brsDisIntPrioTmp is volatile */
  brsNvicIprTmpValue = brsIdx + (brsIdx<<8) + (brsIdx<<16) + (brsIdx<<24);

  /* Evaluate amount of available NVIC_IPR registers within this Cortex-M core implementation.
     NVIC_ICTR holds total number of interrupt lines in groups of 32:
       b0000 = 0...32
       b0001 = 33...64
       ...
       => (NVIC_ICTR+1)*32 = number of external interrupts
     Every NVIC_IPR register configures 4 external interrupts
       => (NVIC_ICTR+1)*8 = number of IPR registers (32/4=8) */
  brsNvicIprAmount = ((BRSHW_MCORE_NVIC_ICTR & 0x0000000F)+1) *8;

  /* Check, if maximum value for NVIC_IPR was surpassed */
  if (brsNvicIprAmount > (BRSHW_MCORE_NVIC_MAX_IPR_NO + 1))
    brsNvicIprAmount = BRSHW_MCORE_NVIC_MAX_IPR_NO + 1;

  /* Set all interrupt priorities to BASEPRI priority -> disable all interrupts
   * (if an interrupts shall be enabled -> set its NVIC_IPR reg to 0) */
  for (brsIdx = 0; brsIdx < brsNvicIprAmount; brsIdx++)
  {
    BRSHW_MCORE_NVIC_IPR(brsIdx) = brsNvicIprTmpValue;
  }
#endif /*BRS_CPU_CORE_CORTEX_Mx*/
}

/*****************************************************************************/
/* @brief      Program the vector table base addresses
 * @pre        -
 * @param[in]  ExcVecLabel - address of the core exception table
 *             IntVecLabel - address of the interrupt vector table
 *             Use BRS_DEFINE_ADDRESS_UNUSED for unused values
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHw_ExceptionTable_Init(Brs_AddressOfConstType ExcVecLabel, Brs_AddressOfConstType IntVecLabel)
{
  /* Initialize VTOR register to start of ExcVec table. No separate IntVec table on cortexM! */
  if(ExcVecLabel!=BRS_DEFINE_ADDRESS_UNUSED)
  {
    BRSHW_VTOR = (uint32)ExcVecLabel;
  }
}

/*****************************************************************************/
/* @brief      Restart ECU (issue a software reset or jump to startup code)
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. ECU state handling
 *****************************************************************************/
void BrsHwSoftwareResetECU(void)
{
  BrsMain_SoftwareResetECU_Hook();

#if !defined (BRS_ENABLE_FBL_SUPPORT) /* FBL is always running in polling mode with interrupts disabled */
  DisableAllInterrupts();
#endif

  /* Vector Key 0x05FA has to be writen together with setting the SYSRESETREQ to 1,
   * otherwise the reset request is not accepted */

  BRSHW_MCORE_SCS_AIRCR = 0x05FA0004;

  while (1)
  {
    /* Wait until reset/watchdog reset occurs */
  }
}

/*****************************************************************************/
/* @brief      This API is used for the BRS time measurement support to get a
 *             default time value for all measurements with this platform to
 *             be able to compare time measurements on different dates based
 *             on this time result.
 * @pre        Should be called with interrupts global disabled
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. component testsuits for calibration
 *****************************************************************************/
void BrsHwTime100NOP(void)
{
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
  BRSHWNOP10();
}

/*****************************************************************************/
/* @brief      This API is used to enable an interrupt source in the core
 *             interrupt controller (NVIC).
 * @pre        -
 * @param[in]  Source to be enabled.
 * @param[in]  Priority level to be set.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_EnableInterrupt(uint32 Source, uint8 Priority)
{
  NVIC_ICPR((uint32)(Source/32)) = (1UL << (Source%32)); /* Clear pending interrupt source. */

  NVIC_IPR(Source) |= (uint32)((Priority) << (((Source) % 4uL) * 8uL)); /* Set interrupt priority. */

  NVIC_ISER((uint32)(Source/32)) = (1UL << (Source%32)); /* Enable interrupt source. */
}

/*****************************************************************************/
/* @brief      This API is used to disable an interrupt source in the core
 *             interrupt controller (NVIC).
 * @pre        -
 * @param[in]  Source to be disabled.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_DisableInterrupt(uint32 Source)
{
  NVIC_ICER((uint32)(Source/32)) = (1UL << (Source%32)); /* Disable interrupt source. */
}

/*****************************************************************************/
/* @brief      This API is used to trigger the given software interrupt source.
 * @pre        -
 * @param[in]  Source to be triggered.
 *             Some derivatives only support few software triggerable sources,
 *             check for their validity.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_TriggerSoftwareInterrupt(uint32 Source)
{
  NVIC_ISPR((uint32)(Source/32)) = (1UL << (Source%32)); /* Trigger software interrupt source. */
}

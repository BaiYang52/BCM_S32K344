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
/** \file  File:  BrsMain_Callout_Stubs.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  UseCase specific implementation templates for several hooks out of BrsMain.
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to BrsMain.h.
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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsMain.h"

#include "BrsHw.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (BRSMAIN_VERSION != 0x0210u)
  #error "Header and source file are inconsistent!"
#endif
#if (BRSMAIN_BUGFIX_VERSION != 0x01u)
  #error "Different versions of bugfix in Header and Source used!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DEFINITION + MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONST VARIABLES
 *********************************************************************************************************************/

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
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage zero.
 *****************************************************************************/
void BrsMain_MemoryInit_StageZero_Hook(uint32 coreId)
{
  /* nothing to be done up to now */
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage hard reset.
 *****************************************************************************/
void BrsMain_MemoryInit_StageHardReset_Hook(uint32 coreId)
{
#if defined (BRS_ENABLE_PREFER_PLL_WATCHDOG_INIT)
  /* If preferred feature is not acivated, initialization will take place in BrsMainInit(). */
# if (BRS_CPU_CORE_AMOUNT>1)
  if (coreId == BRSHW_INIT_CORE_ID)
# endif
  {
# if defined (BRSHW_PREINIT_AVAILABLE)
    BrsHwPreInitPowerOn();
# endif

# if defined (BRS_ENABLE_WATCHDOG)
    BrsHwWatchdogInitPowerOn();
# else
  #error "To use the feature of preferred PLL and Watchdog initialization, add your DrvWd initialization here manually."
# endif

# if defined (BRS_ENABLE_PLLCLOCKS)
    BrsHwPllInitPowerOn();
# else
  #error "To use the feature of preferred PLL and Watchdog initialization, add your DrvMcu initialization here manually."
# endif
  }
#endif /*BRS_ENABLE_PREFER_PLL_WATCHDOG_INIT*/
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage one.
 *****************************************************************************/
void BrsMain_MemoryInit_StageOne_Hook(uint32 coreId)
{
  /* nothing to be done up to now */
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage two.
 *****************************************************************************/
void BrsMain_MemoryInit_StageTwo_Hook(uint32 coreId)
{
  /* nothing to be done up to now */
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage three.
 *****************************************************************************/
void BrsMain_MemoryInit_StageThree_Hook(uint32 coreId)
{
  /* nothing to be done up to now */
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             after the memory initialization was done.
 *****************************************************************************/
void BrsMain_PreMainHook(uint32 coreId)
{
  /* nothing to be done up to now */
}

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwSoftwareResetECU(), before a
 *             SW reset is triggered. Implement code here to also reset
 *             peripherals, if necessary for your UseCase.
 *****************************************************************************/
void BrsMain_SoftwareResetECU_Hook(void)
{
  /* nothing to be done up to now */
}

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
/** \file  File:  BrsMain_Appl.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  Additional, Application-specific, implementation
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

#define _BRSMAIN_APPL_C_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsMain_Appl.h"

#if defined (BRS_ENABLE_CAN_SUPPORT)
  #include BRS_DRVCAN_HEADER_FILENAME
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
#if defined (BRS_ENABLE_CAN_SUPPORT) && (!defined (BRS_ENABLE_FBL_SUPPORT))
  #define CAN_START_SEC_VAR_NOINIT_8BIT
  /* to support CAN driver name infix, vBRS generator provides a wrapper
     for usage of the driver specific MemMap.h define in vBrsCfg.h */
  #include "vBrsCfg.h"
  static uint8 BrsAsrApplCanInterruptLockCtr;
  #define CAN_STOP_SEC_VAR_NOINIT_8BIT
  /* to support CAN driver name infix, vBRS generator provides a wrapper
     for usage of the driver specific MemMap.h define in vBrsCfg.h */
  #include "vBrsCfg.h"
#endif /*BRS_ENABLE_CAN_SUPPORT&&!BRS_ENABLE_FBL_SUPPORT*/

/**********************************************************************************************************************
 *  GLOBAL CONST VARIABLES
 *********************************************************************************************************************/
#if defined (BRS_FBL_INSTANCE_FBL)
/***********************************************************************************************************************
 * FBL Instance Presence Pattern
 *   This presence pattern is only required when downloading the FBL with debugger. When using an updater (or an EOL
 *   programmer), this presence pattern is not required as part of the .hex file and should be removed.
 **********************************************************************************************************************/
#define BRS_START_SEC_FBL_PP_CONST
#include "Brs_MemMap.h"
const uint8 FblPresencePatternArray[4] = {0x73u, 0x6Au, 0x29u, 0x3Eu};
#define BRS_STOP_SEC_FBL_PP_CONST
#include "Brs_MemMap.h"
#endif /*BRS_FBL_INSTANCE_FBL*/

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
/* @brief      Main initialization routine.
 *             Contains configuration specific initialization of BrsHw parts
 *             and BrsMain specific mechanisms.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from main
 *****************************************************************************/
void BrsMain_Appl_Init(void)
{
#if defined (BRS_ENABLE_CAN_SUPPORT) && (!defined (BRS_ENABLE_FBL_SUPPORT))
  /* UserDefined ExclusiveArea handling for CAN channels, according to AN-ISC-8-1149_ErrorHook_E_OS_DISABLED_INT.pdf */
  BrsAsrApplCanInterruptLockCtr = 0u;
#endif
}

#if defined (BRS_ENABLE_CAN_SUPPORT)
/***********************************************************************************************************************
 * UserDefined ExclusiveArea handling for CAN channels, according to AN-ISC-8-1149_ErrorHook_E_OS_DISABLED_INT.pdf
 **********************************************************************************************************************/
/* Macro is generated in vBrsCfg.h, to support drivers with infix.
   Sample w/o infix: void SchM_Enter_Can_CAN_EXCLUSIVE_AREA_0(void)
                     Can_DisableControllerInterrupts(0u);
   Sample w/ infix:  void SchM_Enter_Can_30_Mcan_CAN_30_MCAN_EXCLUSIVE_AREA_0(void)
                     Can_30_Mcan_DisableControllerInterrupts(0u);*/

#define CAN_START_SEC_CODE
/* to support CAN driver name infix, vBRS generator provides a wrapper
   for usage of the driver specific MemMap.h define in vBrsCfg.h */
#include "vBrsCfg.h"
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_0)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (++BrsAsrApplCanInterruptLockCtr == 1u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Disable)(0u);
  }
#endif
}

BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_0)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (--BrsAsrApplCanInterruptLockCtr == 0u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Enable)(0u);
  }
#endif
}

BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_6)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (++BrsAsrApplCanInterruptLockCtr == 1u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Disable)(0u);
  }
#endif
}

BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_6)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (--BrsAsrApplCanInterruptLockCtr == 0u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Enable)(0u);
  }
#endif
}
#define CAN_STOP_SEC_CODE
/* to support CAN driver name infix, vBRS generator provides a wrapper
   for usage of the driver specific MemMap.h define in vBrsCfg.h */
#include "vBrsCfg.h"

# if !defined (BRS_ENABLE_FBL_SUPPORT)
#define CANIF_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_0(void)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (++BrsAsrApplCanInterruptLockCtr == 1u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Disable)(0u);
  }
#endif
}

void SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_0(void)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (--BrsAsrApplCanInterruptLockCtr == 0u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Enable)(0u);
  }
#endif
}
#define CANIF_STOP_SEC_CODE
#include "MemMap.h"

#define CANSM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_4(void)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (++BrsAsrApplCanInterruptLockCtr == 1u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Disable)(0u);
  }
#endif
}

void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_4(void)
{
#if !defined (BRS_ENABLE_FBL_SUPPORT)
  if (--BrsAsrApplCanInterruptLockCtr == 0u)
  {
    BRS_DRVCAN_ControllerInterrupts_INFIX(Enable)(0u);
  }
#endif
}

void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_1(void)
{
  /* Protects against task interruptions -> no locking needed, because all main functions are on same task */
}

void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_1(void)
{
  /* Protects against task interruptions -> no locking needed, because all main functions are on same task */
}
#define CANSM_STOP_SEC_CODE
#include "MemMap.h"

#define COMM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1(void)
{
  /* Protects against task interruptions -> no locking needed, because all main functions are on same task */
}

void SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1(void)
{
  /* Protects against task interruptions -> no locking needed, because all main functions are on same task */
}
#define COMM_STOP_SEC_CODE
#include "MemMap.h"
# endif /*!BRS_ENABLE_FBL_SUPPORT*/
#endif /*BRS_ENABLE_CAN_SUPPORT*/

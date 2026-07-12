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
/** \file  File:  BrsMain_Appl.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  Header of additional, Application-specific, implementation
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

#ifndef _BRSMAIN_APPL_H_
#define _BRSMAIN_APPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsMain.h"

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL TYPE DEFINITIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
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
void BrsMain_Appl_Init(void);

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
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_0);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_0);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_6);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_6);
#define CAN_STOP_SEC_CODE
/* to support CAN driver name infix, vBRS generator provides a wrapper
   for usage of the driver specific MemMap.h define in vBrsCfg.h */
#include "vBrsCfg.h"

# if !defined (BRS_ENABLE_FBL_SUPPORT)
#define CANIF_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_0(void);
void SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_0(void);
#define CANIF_STOP_SEC_CODE
#include "MemMap.h"

#define CANSM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_4(void);
void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_4(void);
void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_1(void);
void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_1(void);
#define CANSM_STOP_SEC_CODE
#include "MemMap.h"

#define COMM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1(void);
void SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1(void);
#define COMM_STOP_SEC_CODE
#include "MemMap.h"
# endif /*!BRS_ENABLE_FBL_SUPPORT*/
#endif /*BRS_ENABLE_CAN_SUPPORT*/
#endif /*_BRSMAIN_APPL_H_*/

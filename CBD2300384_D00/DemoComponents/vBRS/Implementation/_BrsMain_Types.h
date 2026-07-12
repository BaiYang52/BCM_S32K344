/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  BrsMain_Types.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  Vector Basic Runtime System types header for vBrs generated structures.
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

#ifndef _BRSMAIN_TYPES_H_
#define _BRSMAIN_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MODULE VERSION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL TYPE DEFINITIONS
 *********************************************************************************************************************/

typedef enum
{
  NON_ASR = 0, /* No Autosar OS is configured for this core */
  ASR = 4096   /* An Autosar OS is configured for this core */
}brsCoreAsrType;

/* Data type for readable addresses. */
typedef P2CONST(void, TYPEDEF, AUTOMATIC) Brs_AddressOfConstType;

#define BRS_DEFINE_ADDRESS_UNUSED (Brs_AddressOfConstType)0xFFFFFFFF

/* Allocation of physical core to logical cores of a system. */
typedef struct
{
  /* Logical Id of configured core. */
  uint32 LogicalCoreId;

  /* Phyiscal Id of configured core. */
  uint32 PhysicalCoreId;

  /* End label of the startup stack. */
  Brs_AddressOfConstType StartupStackEndLabel;

  /* startup stack size */
  uint32 StartupStackSize;

  /* Reflects if core is an AUTOSAR core. */
  brsCoreAsrType CoreIsAsr;

  /* Start label of the exception vector table. */
  Brs_AddressOfConstType ExcVecLabel;

  /* Start label of the interrupt vector table. */
  Brs_AddressOfConstType IntVecLabel;
}brsMain_CoreType;

/* Reset reason, mainly used by BrsHwGetResetReason().
 * Necessary to e.g. identify hard reset during memory initialization in BrsMainStartup.
 */
typedef enum
{
  BRSMAIN_RESET_SW,
  BRSMAIN_RESET_OTHER
}brsMain_ResetReasonType;

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /*_BRSMAIN_TYPES_H_*/

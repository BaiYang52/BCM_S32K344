/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  Ea_PartitionHandler.c
 *        \brief  Ea Sub-component PartitionHandler source file
 *
 *      \details  PartitionHandler is responsible for management and initialization of partition relevant data.
 *                PartitionHandler provides public services to other EA sub-components to access partition relevant data.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_PartitionHandler.h"
#include "Ea_BlockHandler.h"
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

#define EA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_MemMap */

EA_LOCAL VAR (uint16, EA_APPL_DATA) EaPh_PartitionIndex;

#define EA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Ea_MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "Ea_MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  EaPh_Init()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaPh_Init(uint16 BlockNumber)
{
  /* #10 PartitionIndex has to be initialized with Partition BlockNumber is contained in */
  EaPh_PartitionIndex = EaBh_GetPartitionIndex(BlockNumber);

}


/**********************************************************************************************************************
  EaPh_GetPartitionStartAddress()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaPh_GetPartitionStartAddress(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionStartAddress;
}

/**********************************************************************************************************************
  EaPh_GetPartitionWriteAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionWriteAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionWriteAlignment;
}

/**********************************************************************************************************************
  EaPh_GetPartitionAddressAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionAddressAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionAddressAlignment;
}

/**********************************************************************************************************************
  EaPh_GetPartitionReadAlignment()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaPh_GetPartitionReadAlignment(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].PartitionReadAlignment;
}

/**********************************************************************************************************************
  EaPh_GetEepApiPtr()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_EepApiPtrType, EA_PUBLIC_CODE) EaPh_GetEepApiPtr(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].EepApiPtr;
}

/**********************************************************************************************************************
  EaPh_GetEepEraseValue()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint8, EA_PUBLIC_CODE) EaPh_GetEepEraseValue(void)
{
  return Ea_PartitionConfigList[EaPh_PartitionIndex].EepEraseValue;
}

#define EA_STOP_SEC_CODE
#include "Ea_MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* START_COVERAGE_JUSTIFICATION
    \ID COV_EA_CONDITION_ALWAYS_TRUE
    \ACCEPT TX
    \REASON Condition "LoopCounter < EA_NUMBER_OF_PARTITIONS" is never false in TscStandard.
            'break' in if-clause prevents loop condition from getting false because even if last partition
            contains BlockNumber, LoopCounter will not exceed the condition. Only invalid BlockNumbers are able to
            trigger this condition, and only if DET is disabled, since DET checks validity of BlockNumber before.
END_COVERAGE_JUSTIFICATION */


/**********************************************************************************************************************
 *  END OF FILE: Ea_PartitionHandler.c
 *********************************************************************************************************************/

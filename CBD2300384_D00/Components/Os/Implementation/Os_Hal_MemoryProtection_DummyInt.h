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
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_MemoryProtection  HAL Memory Protection
 *  \brief      Internal functions and data types related to memory protection.
 *  \details
 *  \{
 *
 *  \file
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_MEMORYPROTECTION_DUMMYINT_H
# define OS_HAL_MEMORYPROTECTION_DUMMYINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! MPU specific define for stack region configuration. */
# define OS_HAL_MPU_GETSTACKREGIONINFO(stackConfig)   (stackConfig.StackRegionSize)

/*! \brief    Attributes and size for the maximum stack.
 *  \details  Only dummy define as the MPU is not supported on this derivative. */
# define OS_HAL_MP_STACK_MAXSIZE_AND_ATTRIBUTES    (0xFFFFffffUL)

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Description of a single region in the ARM PMSAv7m implementation. */
struct Os_Hal_MpRegionType_Tag
{
  uint32 Dummy;  /*!< MPU not supported on this derivative. */
};

#endif /* OS_HAL_MEMORYPROTECTION_DUMMYINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtectionInt.h
 **********************************************************************************************************************/

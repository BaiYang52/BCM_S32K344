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
 *  \file        Os_Hal_Core_Types.h
 *  \brief       Core and multi-core related data types and defines.
 *

 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_CORE_TYPES_H
# define OS_HAL_CORE_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */
# include "Os_Thread_Types.h"

/* Os HAL dependencies */
# include "Os_Hal_Derivative.h"
# include "Os_Hal_Core_Cfg.h"
# include "Os_Hal_Entry_Cfg.h"
# include "Os_Hal_Interrupt_Types.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Defines the memory start address which is used to initialize MPU region for startup stack. */
# define OS_HAL_MEM_STARTADDR   (0x00000000uL)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/*! Data type which is used to perform efficient calculation of count leading zero bits.
 * Typically this type is mapped to the processors word type (unsigned int or Os_Hal_WordType).
 * The kernel uses this type to count the leading zeros in Os_Hal_CountLeadingZeros() respectively
 * Os_Bit_CountLeadingZeros(). */
typedef uint32 Os_Hal_BitFieldType;

/*! Data type for spinlocks. */
typedef uint32 Os_Hal_SpinlockType;

/*! Defines whether the lock succeeded or not. */
typedef Os_Hal_SpinlockType Os_Hal_SpinlockResultType;

struct Os_Hal_CoreConfigType_Tag;
typedef struct Os_Hal_CoreConfigType_Tag Os_Hal_CoreConfigType;

struct Os_Hal_Core2ThreadType_Tag;
typedef struct Os_Hal_Core2ThreadType_Tag Os_Hal_Core2ThreadType;

struct Os_Hal_Core2ThreadConfigType_Tag;
typedef struct Os_Hal_Core2ThreadConfigType_Tag Os_Hal_Core2ThreadConfigType;

struct Os_Hal_CoreAsrConfigType_Tag;
typedef struct Os_Hal_CoreAsrConfigType_Tag Os_Hal_CoreAsrConfigType;

struct Os_Hal_SystemConfigType_Tag;
typedef struct Os_Hal_SystemConfigType_Tag Os_Hal_SystemConfigType;

struct Os_Hal_XSigInterruptConfigType_Tag;
typedef struct Os_Hal_XSigInterruptConfigType_Tag Os_Hal_XSigInterruptConfigType;

typedef P2FUNC(void, OS_CODE, Os_Hal_ExceptionTableEntryType)(void);
typedef CONSTP2CONST(Os_Hal_ExceptionTableEntryType, TYPEDEF, OS_CONST) Os_Hal_ExceptionTableType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


#endif /* OS_HAL_CORE_TYPES_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core_Types.h
 **********************************************************************************************************************/

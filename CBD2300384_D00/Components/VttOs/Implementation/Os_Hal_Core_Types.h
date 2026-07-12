/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup  Os_Hal_Core
 * \{
 *
 * \file        Os_Hal_Core_Types.h
 * \brief       Core and multi-core related primitives.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_CORE_TYPES_H
# define OS_HAL_CORE_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_FeatureInt.h"

/* Os kernel module dependencies */
# include "Os_Core_Cfg.h"


/* Os HAL dependencies */

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! Defines whether the HAL provides a count leading zeros command (STD_ON) or not (STD_OFF). */
# define OS_HAL_COUNT_LEADING_ZEROS_HARDWARE    OS_HAL_TST_CLZINHWSUPPORT


/*! The required address width to subscript the bits of a Os_Hal_CoreBitFieldType.
 * The value can be selected from the following table:
 * typeof(Os_Hal_BitFieldType)      | OS_HAL_INDEXBITWIDTH
 * :------------------------------: | :----------------------------
 * uint8                            | 3
 * uint16                           | 4
 * uint32                           | 5
 * uint64                           | 6
 */
# define OS_HAL_INDEXBITWIDTH       (5u)



# define OS_HAL_SPINLOCKUNLOCKED (0xFFu)
# define OS_HAL_SPINLOCKLOCKED   (0x00u)

/*! Defines the value which is returned by \ref Os_Hal_SpinTryLock, if locking succeeded. */
# define OS_HAL_SPINLOCKSUCCEEDED    ((Os_Hal_SpinlockResultType)OS_HAL_SPINLOCKUNLOCKED)

/*! Defines for physical core startup behavior:
 * AUTOSTART:    The core runs directly after reset.
 * MANUAL:       The core needs to be started manually (e.g. by \ref StartCore()).
 * DISABLESTART  The core could not be started. This is used for unused cores.
 *               E.g. for derivatives with core ids 0 and 2 the core 1 is "disabled".
 */
#define OS_HAL_CORESTARTUP_AUTOSTART          1
#define OS_HAL_CORESTARTUP_MANUALSTART        2
#define OS_HAL_CORESTARTUP_DISABLESTART       3

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

/*! Forward declaration of Os_Hal_CANoeNameType */
typedef char Os_Hal_CANoeNameType;

/*! Forward declaration of Os_Hal_CoreNameType */
typedef P2CONST(Os_Hal_CANoeNameType, AUTOMATIC, OS_CONST)  Os_Hal_CoreNameType;

/*! Defines whether the lock succeeded or not. */
typedef Os_Hal_SpinlockType Os_Hal_SpinlockResultType;

/*! Forward declaration of Os_Hal_CoreConfigType */
struct Os_Hal_CoreConfigType_Tag;
typedef struct Os_Hal_CoreConfigType_Tag Os_Hal_CoreConfigType;

/*! Forward declaration of Os_Hal_Core2ThreadType */
struct Os_Hal_Core2ThreadType_Tag;
typedef struct Os_Hal_Core2ThreadType_Tag Os_Hal_Core2ThreadType;

/*! Forward declaration of Os_Hal_Core2ThreadConfigType */
struct Os_Hal_Core2ThreadConfigType_Tag;
typedef struct Os_Hal_Core2ThreadConfigType_Tag Os_Hal_Core2ThreadConfigType;

/*! Forward declaration of Os_Hal_CoreAsrConfigType */
struct Os_Hal_CoreAsrConfigType_Tag;
typedef struct Os_Hal_CoreAsrConfigType_Tag Os_Hal_CoreAsrConfigType;

/*! Forward declaration of Os_Hal_SystemConfigType */
struct Os_Hal_SystemConfigType_Tag;
typedef struct Os_Hal_SystemConfigType_Tag Os_Hal_SystemConfigType;

struct Os_Hal_XSigInterruptConfigType_Tag;
typedef struct Os_Hal_XSigInterruptConfigType_Tag Os_Hal_XSigInterruptConfigType;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/



#endif /* OS_HAL_CORE_TYPES_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core_Types.h
 **********************************************************************************************************************/


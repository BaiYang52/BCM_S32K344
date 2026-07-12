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
 * \addtogroup  Os_Hal_Context
 * \{
 *
 * \file        Os_Hal_Context_Types.h
 * \brief       Forward declarations of the HAL context module.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_CONTEXT_TYPES_H
# define OS_HAL_CONTEXT_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */

/* Os hal dependencies */
# include "Os_Hal_Core_Types.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef enum
{
  Os_Hal_ContextGlobalInterruptState_Disabled,
  Os_Hal_ContextGlobalInterruptState_Enabled
}Os_Hal_ContextGlobalInterruptStateType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/*! Abstraction of CANoe task identifier type. */
typedef sint32 Os_Hal_CANoeTaskIdType;

struct Os_Hal_ContextCallType_Tag;
typedef struct Os_Hal_ContextCallType_Tag Os_Hal_ContextCallType;

struct Os_Hal_CANoeTaskConfigType_Tag;
typedef struct Os_Hal_CANoeTaskConfigType_Tag Os_Hal_CANoeTaskConfigType;

/*! Forward declaration of Os_Hal_ContextType */
struct Os_Hal_ContextType_Tag;
typedef struct Os_Hal_ContextType_Tag Os_Hal_ContextType;

/*! Forward declaration of Os_Hal_ContextConfigType */
struct Os_Hal_ContextConfigType_Tag;
typedef struct Os_Hal_ContextConfigType_Tag Os_Hal_ContextConfigType;

/*! Forward declaration of Os_Hal_ContextStackConfigType */
struct Os_Hal_ContextStackConfigType_Tag;
typedef struct Os_Hal_ContextStackConfigType_Tag Os_Hal_ContextStackConfigType;

struct Os_Hal_ContextFpuContextType_Tag;
typedef struct Os_Hal_ContextFpuContextType_Tag Os_Hal_ContextFpuContextType;

typedef P2CONST(Os_Hal_CANoeNameType, AUTOMATIC, OS_CONST)  Os_Hal_ContextNameType;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_CONTEXT_TYPES_H */


/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context_Types.h
 **********************************************************************************************************************/

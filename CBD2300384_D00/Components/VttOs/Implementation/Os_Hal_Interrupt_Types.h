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
 *  \addtogroup Os_Hal_Interrupt
 *  \{
 *
 *  \file       Os_Hal_Interrupt_Types.h
 *  \brief
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_INTERRUPT_TYPES_H
# define OS_HAL_INTERRUPT_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */

/* Os Hal dependencies */
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

 /*! \brief      Disable all interrupt type.
 *  \details    Specifies whether disabling all interrupts is implemented by level or by flag.
 */
typedef enum
{
  OS_HAL_INT_DISABLE_ALL_BY_FLAG = 0, /*!< Disabling all interrupts is implemented by global disable flag. */
  OS_HAL_INT_DISABLE_ALL_BY_LEVEL = 1 /*!< Disabling all interrupts is implemented by level. */
} Os_Hal_IntDisableAllType;

/*! \brief Interrupt state
 *  \details  The kernel uses this data type to suspend/resume interrupts, to protect OS internal critical sections.
 *            VTT does not support a global disable flag. It always locks up to a certain level.
 */
typedef sint32 Os_Hal_IntGlobalStateType;

/*! \brief    Interrupt state.
 *  \details  The kernel uses this data type to suspend/resume interrupts, to protect OS internal critical sections.
 *            VTT does not support a global disable flag. It always locks up to a certain level.
 */
typedef sint32 Os_Hal_IntStateType;

/*! \brief    Interrupt level.
 *  \details  The kernel uses this data type to suspend/resume category 2 ISRs up to a certain level.
 *            This data type is needed to implement interrupt resources.
 */
typedef sint32 Os_Hal_IntLevelType;

/*! \brief    Interrupt state (with trap information).
 *  \details  The kernel uses this data type to restore the caller's interrupt state on OS service requests.
 */
struct Os_Hal_IntTrapStateType_Tag;
typedef struct Os_Hal_IntTrapStateType_Tag Os_Hal_IntTrapStateType;

struct Os_Hal_IntIsrConfigType_Tag;
/*! Hardware specific configuration for category 2 ISRs. */
typedef struct Os_Hal_IntIsrConfigType_Tag Os_Hal_IntIsrConfigType;

/*! Forward declaration of Os_Hal_IntIsrMapConfigType */
struct Os_Hal_IntIsrMapConfigType_Tag;
typedef struct Os_Hal_IntIsrMapConfigType_Tag Os_Hal_IntIsrMapConfigType;

/*! CANoe Interrupt Routing Mask */
typedef uint32 Os_Hal_IntRoutingMaskType;

typedef P2CONST(Os_Hal_CANoeNameType, AUTOMATIC, AUTOMATIC) Os_Hal_IntIsrNameType;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_INTERRUPT_TYPES_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Interrupt_Types.h
 **********************************************************************************************************************/


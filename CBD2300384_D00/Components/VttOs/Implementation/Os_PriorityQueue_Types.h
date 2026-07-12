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
 * \addtogroup Os_PriorityQueue
 * \{
 *
 * \file        Os_PriorityQueue_Types.h
 * \brief       Contains the static data type definitions of the priority queue.
 * \details     --no details--
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_PRIORITYQUEUE_TYPES_H
# define OS_PRIORITYQUEUE_TYPES_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */
# include "Os_Collection_Types.h"



/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Index of the root node */
# define  OS_PRIORITYQUEUE_ROOT_IDX              ((Os_PriorityQueueNodeIdxType)0)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Represents the index of an element in the queue.*/
typedef Os_CollectionNodeIdxType Os_PriorityQueueNodeIdxType;
typedef Os_CollectionNodeIdxType_IteratorType Os_PriorityQueueNodeIdx_IteratorType;

/*! The configuration data of a priority queue.
 *  The most significant bit of the Usage and Size member is reserved for overflow detection.
 *  It may not be used for node addressing. The valid range is from 0 .. (2^15)-1 */
typedef struct Os_CollectionConfigType_Tag Os_PriorityQueueConfigType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/




#endif /* OS_PRIORITYQUEUE_TYPES_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_PriorityQueue_Types.h
 **********************************************************************************************************************/

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
 * \addtogroup Os_ArrayList
 * \{
 *
 * \file        Os_ArrayList.c
 * \brief       General implementation of the Array List.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/


#define OS_ARRAYLIST_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */

/* Os module declarations */
#include "Os_ArrayList.h"

/* Os kernel module dependencies */


/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"


/* module specific MISRA deviations:
 */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_AR_DATA_INDEX_001
 \DESCRIPTION    Write access to a Data element. The Data element is derived from index usage of a Nodes object.
                 Nodes is derived from the collection of an ArrayList which is passed as argument. Precondition ensures
                 that the ArrayList pointer is valid. The compliance of the precondition is check during review.
                 Usage of Dyn member of Os_ArrayListConfigType's Collection is always in range for the following
                 reasons:
                 (Range is from zero to Size (i.e. number of jobs) + 1)
                 In Os_ArrayListInit usage is set to zero which is in range.
                 In Os_ArrayListInsert usage is incremented. As precondition ensures that every job can only be inserted
                 once, usage can't exceed number of jobs.
                 In Os_ArrayListDelete usage is decremented in case the given job is a node of the list. In this case
                 there's at least one job in the list, i.e. usage must not be zero and underflow can't happen.
                 In Os_ArrayListDelIterNode usage is decremented. As precondition ensures that usage must not be zero,
                 underflow can't happen. The compliance of the precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_ARRAYLIST_COLLECTION_NODES_DATA_M]
                 \R [CM_OS_ARRAYLIST_NODES_INDEX_R]

\ID SBSW_OS_AR_DATA_INDEX_002
 \DESCRIPTION    Write access to a Data element. The Data element is derived from an index of a Nodes object.
                 Nodes is derived from the collection of an ArrayList which is passed as argument. Precondition ensures
                 that the ArrayList pointer is valid. The compliance of the precondition is check during review.
                 It is ensured that the index is lower than usage. As usage is in range, index is also in range.
 \COUNTERMEASURE \M [CM_OS_ARRAYLIST_COLLECTION_NODES_DATA_M]
                 \R [CM_OS_ARRAYLIST_NODES_INDEX_R]

\ID SBSW_OS_AR_DATA_INDEX_003
 \DESCRIPTION    Write access to a Data element. The Data element is derived from index iterator of a Nodes object.
                 Nodes is derived from the collection of an ArrayList which is passed as argument. Precondition ensures
                 that the ArrayList pointer is valid. The compliance of the precondition is check during review.
                 Iterator of ListDyn member of Os_ArrayListConfigType is always in range for the following
                 reasons:
                 In Os_ArrayListInit iterator is set to zero which is in range.
                 In Os_ArrayListDelete iterator could be set to an index that is lower than usage which is in range.
                 In Os_ArrayListIncrementIter iterator is incremented. As precondition ensures that iterator is lower
                 than usage, overflow can't happen. The compliance of the precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_ARRAYLIST_COLLECTION_NODES_DATA_M]
                 \R [CM_OS_ARRAYLIST_NODES_INDEX_R]

\ID SBSW_OS_AR_ARRAYLISTGETDYN_001
 \DESCRIPTION    Write access to a Dyn element. The Dyn element is derived from the collection of an Array List which
                 is passed as argument. Precondition ensures that the ArrayList pointer is valid. The compliance of
                 the precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_ARRAYLIST_COLLECTION_DYN_M]

\ID SBSW_OS_AR_ARRAYLISTGETLISTDYN_001
 \DESCRIPTION    Write access to a Dyn element. The Dyn element is derived from an ArrayList which is passed as
                 argument. Precondition ensures that the ArrayList pointer is valid. The compliance of the
                 precondition is check during review.
 \COUNTERMEASURE \M [CM_OS_ARRAYLIST_LISTDYN_M]

SBSW_JUSTIFICATION_END */

/*
\CM CM_OS_ARRAYLIST_COLLECTION_NODES_DATA_M
      Verify that:
        1. Verify that the Nodes pointer of the collection of each array list is no NULL_PTR and
        2. the size of Nodes is equal Size + 1.
        3. Verify that the Nodes size is sufficient to handle all client data (in case of counters, the Nodes size
           has to be at least equal to the number of jobs + 1, handled by the counter).

\CM CM_OS_ARRAYLIST_COLLECTION_DYN_M
      Verify that the Dyn pointer of the collection of each ArrayList object is valid.

\CM CM_OS_ARRAYLIST_LISTDYN_M
      Verify that the ListDyn pointer of each ArrayList object is valid.

\CM CM_OS_ARRAYLISTGETITERNODE_R
      Implementation ensures that the returned pointer is not used for write access if it is a NULL_PTR.

\CM CM_OS_ARRAYLIST_NODES_INDEX_R
      Validity of statements is check during review.

SBSW_JUSTIFICATION_END */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_ArrayList.c
 **********************************************************************************************************************/

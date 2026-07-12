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
 *
 * \{
 * \file
 * \brief   Provides the API declaration.
 * \details --no details--
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/


#ifndef OS_ARRAYLIST_H
# define OS_ARRAYLIST_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_ArrayListInt.h"

/* Os kernel module dependencies */
# include "Os_Job.h"
# include "Os_Common_Types.h"
# include "Os_Error.h"

/* Os HAL dependencies */
# include "Os_Hal_Compiler.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_ArrayListInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListInit,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
))
{
  /* #10 Initialize the list usage counter with '0'. */
  ArrayList->Collection.Dyn->Usage = 0;                                                                                 /* SBSW_OS_AR_ARRAYLISTGETDYN_001 */

  /* #20 Initialize the list iterator with '0'.*/
  ArrayList->ListDyn->Iterator = 0;                                                                                     /* SBSW_OS_AR_ARRAYLISTGETLISTDYN_001 */
}


/***********************************************************************************************************************
 *  Os_ArrayListInsert()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListInsert,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList,
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) Data
))
{
  /* #10 Assert that the list has still capacity. */
  Os_Assert((Os_StdReturnType)(ArrayList->Collection.Dyn->Usage < ArrayList->Collection.Size));                         /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* #20 Append the element after the last used node. */
  ArrayList->Collection.Nodes[ArrayList->Collection.Dyn->Usage].Data = Data;                                            /* SBSW_OS_AR_DATA_INDEX_001 */

  /* #30 Increment the list usage counter. */
  ArrayList->Collection.Dyn->Usage++;                                                                                   /* SBSW_OS_AR_ARRAYLISTGETDYN_001 */
}


/***********************************************************************************************************************
 *  Os_ArrayListDelete()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE Os_CollectionDelResultType, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListDelete,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList,
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) Data
))
{
  Os_CollectionDelResultType result = OS_COLLECTIONDELETE_INEXISTENT;
  Os_ArrayListNodeIdx_IteratorType idx;

  /* #10 Search for the index (deleteIdx) of node which points to data. */
  for(idx = OS_ARRAYLIST_ROOT_IDX; idx < (Os_ArrayListNodeIdx_IteratorType) ArrayList->Collection.Dyn->Usage; idx++)    /* COV_OS_INVSTATE */ /* FETA_OS_LISTUSAGE */
  {
    if (ArrayList->Collection.Nodes[idx].Data == Data)
    {
      break;
    }
  }

  /* #20 If this node exists: */
  if (idx < (Os_ArrayListNodeIdx_IteratorType) ArrayList->Collection.Dyn->Usage)
  {
    /* #30 Decrement the number of nodes by one. */
    ArrayList->Collection.Dyn->Usage--;                                                                                 /* SBSW_OS_AR_ARRAYLISTGETDYN_001 */

    /* #40 If the deleted node is not the last node: */
    if (OS_LIKELY(idx < (Os_ArrayListNodeIdx_IteratorType) ArrayList->Collection.Dyn->Usage))
    {
      /* #50 Copy the data pointer of the last node into the deleted node. */
      ArrayList->Collection.Nodes[idx] = ArrayList->Collection.Nodes[ArrayList->Collection.Dyn->Usage];                 /* SBSW_OS_AR_DATA_INDEX_002 */

      /* #60 If the index of the deleted node is lower than the list iterator to work off jobs: */
      if (idx < (Os_ArrayListNodeIdx_IteratorType) ArrayList->ListDyn->Iterator)
      {
        /* #70 Set the list iterator to this index. */
        ArrayList->ListDyn->Iterator = (Os_ArrayListNodeIdxType) idx;                                                   /* SBSW_OS_AR_ARRAYLISTGETLISTDYN_001 */
      }
    }
    /* #80 Set result to OS_COLLECTIONDELETE_OK. */
    result = OS_COLLECTIONDELETE_OK;
  }

  /* #90 Return the result. */
  return result;
}


/***********************************************************************************************************************
 *  Os_ArrayListGetIterNode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE P2CONST(Os_JobConfigType, AUTOMATIC, OS_CODE), OS_CODE, OS_ALWAYS_INLINE,
Os_ArrayListGetIterNode,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
))
{
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) result = NULL_PTR;
  P2CONST(volatile Os_CollectionNodeIdxType, AUTOMATIC, OS_VAR_NOINIT) usage = &(ArrayList->Collection.Dyn->Usage);
  Os_ArrayListNodeIdxType iterator = ArrayList->ListDyn->Iterator;

  /* #10 If the list's iterator is lower than the list's usage: */
  if (iterator < *usage)
  {
    /* #20 Set result to the iterator data pointer. */
    result = ArrayList->Collection.Nodes[iterator].Data;
  }
  return result;
}


/***********************************************************************************************************************
 *  Os_ArrayListDelIterNode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListDelIterNode,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
))
{
  P2VAR(volatile Os_CollectionNodeIdxType, AUTOMATIC, OS_VAR_NOINIT) usage = &(ArrayList->Collection.Dyn->Usage);
  Os_ArrayListNodeIdxType iterator = ArrayList->ListDyn->Iterator;
  /*! Internal comment removed.
 *
 * */
  Os_CollectionNodeIdxType usageDecremented = *usage - (Os_CollectionNodeIdxType)1u;

  /* #10 Assert that the list's iterator is lower than the list's usage. */
  Os_Assert((Os_StdReturnType)(iterator < *usage));                                                                     /* PRQA S 4304 */ /* MD_Os_C90BooleanCompatibility */

  /* #20 Decrement the number of nodes by one. */
  *usage = usageDecremented;                                                                                            /* SBSW_OS_AR_ARRAYLISTGETDYN_001 */

  /* #30 Copy the data pointer of the last node into the deleted node. */
  ArrayList->Collection.Nodes[iterator] = ArrayList->Collection.Nodes[*usage];                                          /* SBSW_OS_AR_DATA_INDEX_003 */
}


/***********************************************************************************************************************
 *  Os_ArrayListIncrementIter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListIncrementIter,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
))
{
  /*! Internal comment removed.
 *
 * */
  Os_ArrayListNodeIdxType iteratorIncremented = ArrayList->ListDyn->Iterator + 1u;
  ArrayList->ListDyn->Iterator = iteratorIncremented;                                                                   /* SBSW_OS_AR_ARRAYLISTGETLISTDYN_001 */
}


/***********************************************************************************************************************
 *  Os_ArrayListResetIter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListResetIter,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
))
{
  ArrayList->ListDyn->Iterator = OS_ARRAYLIST_ROOT_IDX;                                                                 /* SBSW_OS_AR_ARRAYLISTGETLISTDYN_001 */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"


#endif /* OS_ARRAYLIST_H */

/* module specific MISRA deviations:
 *
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_ArrayList.h
 **********************************************************************************************************************/

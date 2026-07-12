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
 * \ingroup     Os_Kernel
 * \defgroup    Os_ArrayList Array List
 * \brief       Array List management.
 * \details
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


#ifndef OS_ARRAYLISTINT_H
# define OS_ARRAYLISTINT_H
                                                                                                                        /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_ArrayList_Types.h"

/* Os kernel module dependencies */
# include "Os_CollectionInt.h"
# include "Os_ErrorInt.h"

/* Os HAL dependencies */
# include "Os_Hal_CompilerInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Dynamic information of an array list. */
struct Os_ArrayListType_Tag
{
  /*! The iterator to work off jobs in the array list. */
  volatile Os_ArrayListNodeIdxType Iterator;
};

/*!
 * The configuration data of an array list. *
 * \extends Os_CollectionConfigType
 */
struct Os_ArrayListConfigType_Tag
{
  /*! The collection which holds the general data.
  * This attribute must come first! */
  Os_CollectionConfigType Collection;

  /*! Dynamic part of the array list. */
  P2VAR(Os_ArrayListType, AUTOMATIC, OS_CONST) ListDyn;
};

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"

/***********************************************************************************************************************
 *  Os_ArrayListInit()
 **********************************************************************************************************************/
/*! \brief          Initialize the given array list. This function has to be called for each list once.
 *                  Any other operation on a list instance may only be performed after calling Os_ArrayListInit.
 *  \details        This function is called for the first time in Init-Step3 for the timing protection counter. It is
 *                  called for all counters in Init-Step4.
 *
 *  \param[in,out]  ArrayList The array list to be initialized. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListInit,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
));


/***********************************************************************************************************************
 *  Os_ArrayListInsert()
 **********************************************************************************************************************/
/*! \brief          Inserts a new element.
 *  \details        --no details--
 *
 *  \param[in,out]  ArrayList  The array list. Parameter must not be NULL.
 *  \param[in]      Data       Pointer to the data element which shall be inserted. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Data element is not part of the list.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListInsert,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList,
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) Data
));


/***********************************************************************************************************************
 *  Os_ArrayListDelete()
 **********************************************************************************************************************/
/*! \brief          Searches for a node with a data pointer equal to the data parameter and deletes the node.
 *  \details        If there are multiple nodes with equal pointers, the first node found, will be deleted.
 *                  The deleted node does not have to be the one with the highest priority.
 *
 *  \param[in,out]  ArrayList  The array list. Parameter must not be NULL.
 *  \param[in]      Data       Pointer to the data element which shall be deleted. Parameter must not be NULL.
 *
 *  \retval         OS_COLLECTIONDELETE_OK          The deletion succeeded.
 *  \retval         OS_COLLECTIONDELETE_INEXISTENT  The element is not part of the list.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE Os_CollectionDelResultType, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListDelete,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList,
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) Data
));


/***********************************************************************************************************************
 *  Os_ArrayListGetIterNode()
 **********************************************************************************************************************/
/*! \brief          Returns the array list's node at the list's iterator index.
 *  \details        Returns the NULL_PTR if the iterator is not lower than the list's usage.
 *
 *  \param[in]      ArrayList  The array list. Parameter must not be NULL.
 *
 *  \return         Returns the data pointer of the element at the list's iterator index.
 *  \retval         NULL_PTR  If the list's iterator is not lower than the list's usage.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE P2CONST(Os_JobConfigType, AUTOMATIC, OS_CODE), OS_CODE, OS_ALWAYS_INLINE,
Os_ArrayListGetIterNode,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
));


/***********************************************************************************************************************
 *  Os_ArrayListDelIterNode()
 **********************************************************************************************************************/
/*! \brief          Deletes the array list's node at the list's iterator index.
 *  \details        --no details--
 *
 *  \param[in,out]  ArrayList  The array list. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            The list's iterator is lower than the list's usage.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListDelIterNode,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
));


/***********************************************************************************************************************
 *  Os_ArrayListIncrementIter()
 **********************************************************************************************************************/
/*! \brief          Increments the array list's iterator by one.
 *  \details        --no details--
 *
 *  \param[out]     ArrayList  The array list. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListIncrementIter,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
));


/***********************************************************************************************************************
 *  Os_ArrayListResetIter()
 **********************************************************************************************************************/
/*! \brief          Sets the array list's iterator to the root index of the list.
 *  \details        --no details--
 *
 *  \param[out]     ArrayList  The array list. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_ArrayListResetIter,
(
  P2CONST(Os_ArrayListConfigType, AUTOMATIC, OS_CONST) ArrayList
));


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"


#endif /* OS_ARRAYLISTINT_H */

/* module specific MISRA deviations:
 *
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_ArrayListInt.h
 **********************************************************************************************************************/

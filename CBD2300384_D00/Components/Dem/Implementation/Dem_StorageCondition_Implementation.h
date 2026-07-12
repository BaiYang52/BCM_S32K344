/* ********************************************************************************************************************
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
 */
/*! \addtogroup Dem_StorageCondition
 *  \{
 *  \file       Dem_StorageCondition_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the StorageCondition subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_STORAGECONDITION_IMPLEMENTATION_H)
#define DEM_STORAGECONDITION_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_StorageCondition_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_STORAGECONDITION_IMPLEMENTATION_FILENAME "Dem_StorageCondition_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*!
 * \defgroup Dem_Conditions_StorageCondition_PrivateProperties Private Properties
 * \{
 */

 /* ****************************************************************************
 * Dem_Conditions_StorageCondition_IsStorageConditionSupported
 *****************************************************************************/
 /*!
 * \brief         Tests if storage conditions are supported.
 *
 * \details       Tests if storage conditions are supported.
 *
 * \return        TRUE
 *                Storage conditions are supported.
 * \return        FALSE
 *                Storage conditions are not supported.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_IsStorageConditionSupported(
  void
);

/* ********************************************************************************************************************
*  UNIT PRIVATE PROPERTIES DEFINITIONS
*********************************************************************************************************************/

/* ****************************************************************************
% Dem_Conditions_StorageCondition_IsStorageConditionSupported
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_IsStorageConditionSupported(
  void
)
{
  return (DEM_CFG_SUPPORT_STORAGE_CONDITIONS == STD_ON);
}


/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "MemMap.h"    


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Conditions_StorageCondition_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_GetGlobalCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Conditions_StorageCondition_GetGlobalCount(
  void
  )
{
  return DEM_CFG_GLOBAL_STORAGE_COND_COUNT;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Conditions_StorageCondition_Private Private Methods
 * \{
 */


/* ****************************************************************************
 * Dem_Conditions_StorageCondition_TestActiveState
 *****************************************************************************/
/*!
 * \brief         Get current activation state of storage condition
 *
 * \details       Get current activation state of storage condition
 *
 * \param[in]     StorageConditionID
 *                Unique handle of the storage condition
 *
 * \return        Activation state of storage condition
 *
 * \pre           -
 * \config       -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_TestActiveState(
  CONST(uint8, AUTOMATIC)  StorageConditionID
  );



/* ****************************************************************************
 * Dem_Conditions_StorageCondition_SetActiveState
 *****************************************************************************/
/*!
 * \brief         Set current activation status of storage condition
 *
 * \details       Set current activation status of storage condition
 *
 * \param[in]     StorageConditionID
 *                Unique handle of the storage condition
 * \param[in]     ActiveStatus
 *                New activation state of the storage condition
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_StorageCondition_SetActiveState(
  CONST(uint8, AUTOMATIC)  StorageConditionID,
  CONST(boolean, AUTOMATIC)  ActiveStatus
  );


/* ****************************************************************************
 * Dem_StorageCondition_Group_GetNotFulfilledCount
 *****************************************************************************/
/*!
 * \brief         Get current internal counter of storage condition group
 *
 * \details       Get current internal counter of storage condition group
 *
 * \param[in]     StorageConditionGroup
 *                Unique handle of the storage condition group
 *
 * \return        Internal counter of storage condition group
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_StorageCondition_Group_GetNotFulfilledCount(
  CONST(Dem_StorageCondition_Group_IdentifierType, AUTOMATIC)  StorageConditionGroup
  );



/* ****************************************************************************
 * Dem_StorageCondition_Group_SetNotFulfilledCounter
 *****************************************************************************/
/*!
 * \brief         Set current internal counter of storage condition group
 *
 * \details       Set current internal counter of storage condition group
 *
 * \param[in]     StorageConditionGroup
 *                Unique handle of the storage condition group
 * \param[in]     Counter
 *                New internal counter of the storage condition group
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_SetNotFulfilledCounter(
  CONST(Dem_StorageCondition_Group_IdentifierType, AUTOMATIC)  StorageConditionGroup,
  CONST(uint8, AUTOMATIC)  Counter
  );




/* ****************************************************************************
 * Dem_Conditions_StorageCondition_GetInitValue
 *****************************************************************************/
/*!
 * \brief         Get the initial value of storage condition
 *
 * \details       Get the initial value of storage condition
 *
 * \param[in]     StorageConditionID
 *                Unique handle of the storage condition.
 *
 * \return        TRUE 
 *                The initial value of storage condition, is TRUE
 * \return        FALSE 
 *                The initial value of storage condition, is FALSE
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_GetInitValue(
  CONST(uint8, AUTOMATIC)  StorageConditionID
  );



/* ****************************************************************************
 * Dem_StorageCondition_Group_IterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_StorageCondition_Group_GetIter() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_StorageCondition_Group_GetIter() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageCondition_Group_IterExists(
  CONSTP2CONST(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );



/* ****************************************************************************
 * Dem_StorageCondition_Group_GetIter
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to Storage condition group descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageCondition_Group_IdentifierType, DEM_CODE)
Dem_StorageCondition_Group_GetIter(
  CONSTP2CONST(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_StorageCondition_Group_InitIter
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator
 *
 * \details       Initializes the iterator to enumerate storage condition groups
 *                referencing given storage condition
 *
 * \param[in]     StorageConditionID
 *                Unique handle of the storage condition.
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_InitIter(
  CONST(uint8, AUTOMATIC) StorageConditionID,
  CONSTP2VAR(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );



/* ****************************************************************************
 * Dem_StorageCondition_Group_NextIter
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_NextIter(
  CONSTP2VAR(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_TestActiveState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_TestActiveState(
  CONST(uint8, AUTOMATIC)  StorageConditionID
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionID)                                                                             /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  return Dem_Cfg_IsStorageConditionFulfilled(StorageConditionID);
# else
  return TRUE;
# endif
}


/* ****************************************************************************
 % Dem_Conditions_StorageCondition_SetActiveState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_StorageCondition_SetActiveState(
  CONST(uint8, AUTOMATIC)  StorageConditionID,
  CONST(boolean, AUTOMATIC)  ActiveStatus
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ActiveStatus)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionID)                                                                             /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
#  if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (StorageConditionID >= Dem_Cfg_GetSizeOfStorageConditionFulfilled())
  {
    Dem_Error_RunTimeCheckFailed(DEM_STORAGECONDITION_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#  endif
  {
    Dem_Cfg_SetStorageConditionFulfilled(StorageConditionID, ActiveStatus);                                                        /* SBSW_DEM_ARRAY_WRITE_STORAGECONDITIONFULFILLED */
  }
# endif
}


/* ****************************************************************************
 % Dem_StorageCondition_Group_GetNotFulfilledCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_StorageCondition_Group_GetNotFulfilledCount(
  CONST(Dem_StorageCondition_Group_IdentifierType, AUTOMATIC)  StorageConditionGroup
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionGroup)                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  return Dem_Cfg_GetStorageConditionGroupCounter(StorageConditionGroup);
# else
  return 0;
# endif
}


/* ****************************************************************************
 % Dem_StorageCondition_Group_SetNotFulfilledCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_SetNotFulfilledCounter(
  CONST(Dem_StorageCondition_Group_IdentifierType, AUTOMATIC)  StorageConditionGroup,
  CONST(uint8, AUTOMATIC)  Counter
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionGroup)                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Counter)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
#  if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (StorageConditionGroup >= Dem_Cfg_GetSizeOfStorageConditionGroupCounter())
  {
    Dem_Error_RunTimeCheckFailed(DEM_STORAGECONDITION_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#  endif
  {
    Dem_Cfg_SetStorageConditionGroupCounter(StorageConditionGroup, Counter);                                                     /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_STORAGECONDITIONGROUPCOUNTER */
  }
# endif
}

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_GetInitValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_GetInitValue(
  CONST(uint8, AUTOMATIC) StorageConditionID
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionID)                                                                             /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  return Dem_Cfg_IsFulfilledAtInitOfStorageConditionInitTable(StorageConditionID);                                                 /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
# else
  return TRUE;
# endif
}


/* ****************************************************************************
 % Dem_StorageCondition_Group_IterExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageCondition_Group_IterExists(
  CONSTP2CONST(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr                                                /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
# else
  return FALSE;
# endif
}


/* ****************************************************************************
 % Dem_StorageCondition_Group_GetIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageCondition_Group_IdentifierType, DEM_CODE)
Dem_StorageCondition_Group_GetIter(
  CONSTP2CONST(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr                                                /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  return Dem_Cfg_GetStorageConditionGroupTableInd(IterPtr->mIdx);
# else
  return 0;
# endif
}

/* ****************************************************************************
 % Dem_StorageCondition_Group_InitIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_InitIter(
  CONST(uint8, AUTOMATIC)  StorageConditionID,                                                                                   /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr                                                  /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StorageConditionID)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  IterPtr->mIdx = Dem_Cfg_GetStorageConditionGroupTableIndStartIdxOfStorageConditionTable(StorageConditionID);                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetStorageConditionGroupTableIndEndIdxOfStorageConditionTable(StorageConditionID);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# else
  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = 0u;                                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
}


/* ****************************************************************************
 % Dem_StorageCondition_Group_NextIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageCondition_Group_NextIter(
  CONSTP2VAR(Dem_StorageCondition_Group_IterType, AUTOMATIC, AUTOMATIC) IterPtr                              /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_HAS_STORAGE_CONDITIONS == STD_ON)
  IterPtr->mIdx += 1u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Conditions_StorageCondition_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
  Dem_Conditions_StorageCondition_PreInit(
  void
  )
{
  if (Dem_Conditions_StorageCondition_IsStorageConditionSupported() == TRUE)
  {
    Dem_StorageCondition_Group_IdentifierType lStorageConditionGroupCounter;
    uint8  lStorageConditionId;

    for (lStorageConditionId = 0u;
      lStorageConditionId < Dem_Cfg_GetSizeOfStorageConditionFulfilled();                                                            /* PRQA S 2994 */ /* MD_DEM_2994 */
      ++lStorageConditionId)
    {
      Dem_Conditions_StorageCondition_SetActiveState(lStorageConditionId, TRUE);
    }

    for (lStorageConditionGroupCounter = 0u;
      lStorageConditionGroupCounter < Dem_Cfg_GetSizeOfStorageConditionGroupCounter();                                             /* PRQA S 2994 */ /* MD_DEM_2994 */
      ++lStorageConditionGroupCounter
      )
    {
      Dem_StorageCondition_Group_SetNotFulfilledCounter(lStorageConditionGroupCounter, 0u);
    }
  }
}

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_StorageCondition_Init(
  void
  )
{
  if (Dem_Conditions_StorageCondition_IsStorageConditionSupported() == TRUE)
  {
    Dem_StorageCondition_Group_IdentifierType lStorageConditionGroupCounter;
    uint8 lStorageCondition;

    for (lStorageConditionGroupCounter = 0u;
      lStorageConditionGroupCounter < Dem_Cfg_GetSizeOfStorageConditionGroupCounter();                                             /* PRQA S 2994 */ /* MD_DEM_2994 */
      ++lStorageConditionGroupCounter
      )
    {
      Dem_StorageCondition_Group_SetNotFulfilledCounter(lStorageConditionGroupCounter, 0u);
    }

    for (lStorageCondition = 0u; lStorageCondition < Dem_Conditions_StorageCondition_GetGlobalCount(); ++lStorageCondition)        /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      boolean lStorageConditionInitState;
      Dem_StorageCondition_Group_IterType lStorageConditionIter;

      lStorageConditionInitState = Dem_Conditions_StorageCondition_GetInitValue(lStorageCondition);

      if (lStorageConditionInitState == FALSE)
      {
        for (Dem_StorageCondition_Group_InitIter(lStorageCondition, &lStorageConditionIter);                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_StorageCondition_Group_IterExists(&lStorageConditionIter) == TRUE;                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_StorageCondition_Group_NextIter(&lStorageConditionIter))                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          Dem_StorageCondition_Group_IdentifierType lStorageConditionGroup;

          lStorageConditionGroup = Dem_StorageCondition_Group_GetIter(&lStorageConditionIter);                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */

          Dem_StorageCondition_Group_SetNotFulfilledCounter(lStorageConditionGroup,
            (uint8)(Dem_StorageCondition_Group_GetNotFulfilledCount(lStorageConditionGroup) + 1u));
        }
      }
      Dem_Conditions_StorageCondition_SetActiveState(lStorageCondition, lStorageConditionInitState);
    }
  }
}                                                                                                                                   /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_TestEventStorageConditionsFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_StorageCondition_TestEventStorageConditionsFulfilled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Conditions_StorageCondition_IsStorageConditionSupported() == TRUE)
  {
    boolean lStorageConditionsSatisfied;
    Dem_StorageCondition_Group_IdentifierType lStorageConditionGroup;

    lStorageConditionGroup = Dem_Core_Event_GetStorageConditionGroupId(EventId);
    if (lStorageConditionGroup != DEM_STORAGECONDITION_NO_STORAGE_CONDITION_GROUP)
    {
      if (Dem_StorageCondition_Group_GetNotFulfilledCount(lStorageConditionGroup) > 0u)
      {
        lStorageConditionsSatisfied = FALSE;
      }
      else
      {
        lStorageConditionsSatisfied = TRUE;
      }
    }
    else
    {
      lStorageConditionsSatisfied = TRUE;
    }

    return lStorageConditionsSatisfied;
  }
  else
  {
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
    return TRUE;
  }
}

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_SetFulfilled
 *****************************************************************************/
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_StorageCondition_SetFulfilled(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  StorageConditionID
  )
{
  if (Dem_Conditions_StorageCondition_IsStorageConditionSupported() == TRUE)                                                    /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_Conditions_StorageCondition_TestActiveState(StorageConditionID) == FALSE)
    {
      Dem_StorageCondition_Group_IterType lStorageConditionIter;
      for (Dem_StorageCondition_Group_InitIter(StorageConditionID, &lStorageConditionIter);                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_StorageCondition_Group_IterExists(&lStorageConditionIter) == TRUE;                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_StorageCondition_Group_NextIter(&lStorageConditionIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_StorageCondition_Group_IdentifierType lStorageConditionGroup;

        lStorageConditionGroup = Dem_StorageCondition_Group_GetIter(&lStorageConditionIter);                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
        
        if (Dem_StorageCondition_Group_GetNotFulfilledCount(lStorageConditionGroup) > 0u)
        {
          Dem_StorageCondition_Group_SetNotFulfilledCounter(lStorageConditionGroup,
            (uint8)(Dem_StorageCondition_Group_GetNotFulfilledCount(lStorageConditionGroup) - 1u));
        }
      }
      Dem_Conditions_StorageCondition_SetActiveState(StorageConditionID, TRUE);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Conditions_StorageCondition_ResetFulfilled
  *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_StorageCondition_ResetFulfilled(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  StorageConditionID
  )
{
  if (Dem_Conditions_StorageCondition_IsStorageConditionSupported() == TRUE)                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_Conditions_StorageCondition_TestActiveState(StorageConditionID) == TRUE)
    {
      Dem_StorageCondition_Group_IterType lStorageConditionIter;
      for (Dem_StorageCondition_Group_InitIter(StorageConditionID, &lStorageConditionIter);                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_StorageCondition_Group_IterExists(&lStorageConditionIter) == TRUE;                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */ /* PRQA S 2994 */ /* MD_DEM_2994 */
        Dem_StorageCondition_Group_NextIter(&lStorageConditionIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_StorageCondition_Group_IdentifierType lStorageConditionGroup;

        lStorageConditionGroup = Dem_StorageCondition_Group_GetIter(&lStorageConditionIter);                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_StorageCondition_Group_SetNotFulfilledCounter(lStorageConditionGroup,
          (uint8)(Dem_StorageCondition_Group_GetNotFulfilledCount(lStorageConditionGroup) + 1u));
      }
      Dem_Conditions_StorageCondition_SetActiveState(StorageConditionID, FALSE);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_STORAGECONDITION_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_StorageCondition_Implementation.h
 *********************************************************************************************************************/

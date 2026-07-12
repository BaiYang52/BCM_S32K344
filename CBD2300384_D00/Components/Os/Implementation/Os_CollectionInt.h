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
 * \defgroup    Os_Collection Collection
 * \brief       Collection primitives.
 * \details
 *
 * \{
 * \file
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/


#ifndef OS_COLLECTIONINT_H
# define OS_COLLECTIONINT_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Collection_Types.h"

/* Os kernel module dependencies */
# include "Os_Job_Types.h"

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

/*! Defines the return codes for the collections delete operation. */
typedef enum
{
  OS_COLLECTIONDELETE_OK,              /*!< Deletion succeeded. */
  OS_COLLECTIONDELETE_INEXISTENT       /*!< The node to be deleted does not exist. */
}Os_CollectionDelResultType;


/*! Represents a single node of a collection.  */
struct Os_CollectionNodeType_Tag
{
  /*! A pointer to the managed data. */
  P2CONST(Os_JobConfigType, AUTOMATIC, OS_APPL_DATA) Data;
};


/*! The runtime data of a collection. */
struct Os_CollectionType_Tag
{
  /*! Number of nodes actually used. */
  Os_CollectionNodeIdxType Usage;
};


/*! The configuration data of a collection. */
struct Os_CollectionConfigType_Tag
{
  /*! The array which represents the collection. */
  P2VAR(Os_CollectionNodeType, AUTOMATIC, OS_CONST) Nodes;

  /*! Points to the runtime data of the collection. */
  P2VAR(Os_CollectionType, AUTOMATIC, OS_CONST) Dyn;

  /*! The total number of available nodes. */
  Os_CollectionNodeIdxType Size;
};

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_COLLECTIONINT_H */

/* module specific MISRA deviations:
 *
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_CollectionInt.h
 **********************************************************************************************************************/

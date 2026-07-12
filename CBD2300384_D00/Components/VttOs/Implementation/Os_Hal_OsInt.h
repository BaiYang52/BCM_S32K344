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
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_Os  HAL OS
 *  \{
 *
 *  \file       Os_Hal_OsInt.h
 *  \brief      HAL interfaces which are visible to the user.
 *  \details
 *  This file is included by Os.h. Therefore all symbols defined here are visible to the user.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_OSINT_H
# define OS_HAL_OSINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */
# include "Os_Lcfg.h"

/* Os HAL dependencies */
# include "Os_Hal_Os_Types.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! \brief    Provides function declaration for category 1 ISRs.
 *
 *  \param[in]  IsrName   The function name which contains the user code of the category 1 ISR.
 */
# define OS_HAL_ISR1_DECLARE(IsrName)      void IsrName(void)


/*! \brief    Provides function definition for category 1 ISRs.
 *  \details
 *  The kernel uses this macro to provide the platform independent OS_ISR1() interface.
 *
 *  \param[in]  IsrName   The function name which contains the user code of the category 1 ISR.
 */
# define OS_HAL_ISR1_DEFINE(IsrName)       void IsrName(void)


/*! \brief    Provides function declaration for category 0 ISRs.
 *
 *  \param[in]  IsrName   The function name which contains the user code of the category 0 ISR.
 */
# define OS_HAL_ISR0_DECLARE(IsrName)      void IsrName(void)


/*! \brief    Provides function definition for category 0 ISRs.
 *  \details
 *  The kernel uses this macro to provide the platform independent OS_ISR0() interface.
 *
 *  \param[in]  IsrName   The function name which contains the user code of the category 0 ISR.
 */
# define OS_HAL_ISR0_DEFINE(IsrName)       void IsrName(void)


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Set of hardware registers to be able to resume from an exception. */
struct Os_ExceptionContextType_Tag
{
  /*! Dummy attribute */
  uint32 Dummy;
};

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_OS_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_OsInt.h
 **********************************************************************************************************************/


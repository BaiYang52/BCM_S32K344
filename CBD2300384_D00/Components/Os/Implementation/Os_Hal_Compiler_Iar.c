/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup Os_Hal_Compiler
 *  \{
 *
 *  \file
 *  \brief        Compiler abstraction for Iar compiler.
 *  \details
 *  Internal comment removed.
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
 *

 *
 *
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */

#define OS_HAL_COMPILER_IAR_SOURCE

# ifdef CDK_CHECK_MISRA                                                                                                 /* COV_OS_STATICCODEANALYSIS */
#  ifndef OS_STATIC_CODE_ANALYSIS                                                                                       /* COV_OS_STATICCODEANALYSIS */
#    define OS_STATIC_CODE_ANALYSIS
#  endif
# endif


#ifndef OS_STATIC_CODE_ANALYSIS                                                                                         /* COV_OS_STATICCODEANALYSIS */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os hal module dependencies */
# include "Os_Hal_Compiler.h"

# if defined (OS_CFG_COMPILER_IAR)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_Hal_LoadExclusive()
 **********************************************************************************************************************/
/*! \brief        Load register exclusive
 *  \details      -
 *  \param[in]    Addr  Address that has to be read .
 *  \return       Content of the input memory address.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
uint32 Os_Hal_LoadExclusive(const volatile uint32* Addr)
{
  uint32 ret;
  asm volatile( "ldrex %0, [%1]"
  : "=r" (ret)
  : "r"  (Addr)
  :);
  return ret;
}


/***********************************************************************************************************************
 *  Os_Hal_StoreExclusive()
 **********************************************************************************************************************/
/*! \brief        Store register exclusive
 *  \details      Inline assembly allows to instruct the compiler not to use the same registers for the strex arguments:
 *                - %0 register that refers to ret variable (it will be written)
 *                - %1 register that refers to Addr variable (it will be written)
 *                - %2 register that contains the Value function argument.
 *  \param[out]   Addr  Address in which has to be written exclusively.
 *  \param[in]    Value Value that has to be written in the memory location Address.
 *  \return       ret   0 Operation successfull.
 *                      1 Operation unsuccessfull.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
uint32 Os_Hal_StoreExclusive(volatile uint32* Addr, uint32 Value)
{
  uint32 ret = 1;
  asm volatile( "strex %0, %2, [%1] "
  : "+&r" (ret) "+&r" (Addr)
  : "r" (Value)
  : );
  return ret;
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# endif /* if defined (OS_CFG_COMPILER_IAR) */

#endif /* #ifndef (CDK_CHECK_MISRA) */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Compiler_Arm.c
 *********************************************************************************************************************/

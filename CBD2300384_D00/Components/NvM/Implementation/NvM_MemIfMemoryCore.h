/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**!       \file  NvM_MemIfMemoryCore.h
 *        \brief  NvM_MemIfMemoryCore header file
 *      \details  Header of MemIf Memory Core unit of the NvM.
 *                This unit is placed within the (memory) Core where also the underlying MemIf is placed.
 *                The rest of the NvM itself is placed on another Core.
 *                ONLY needed if NvM and MemIf are placed on different Cores!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (NVM_MEMIFMEMORYCORE)
# define NVM_MEMIFMEMORYCORE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "NvM_Cfg.h"

# if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  NvM_MemIfMemoryCore_MainFunction()
 **********************************************************************************************************************/
/*! \brief       Process part of NvM which is located at the (memory) Core.
 *               Therefore, the core where the MemIf is placed.
 *  \details     Pull possible requests triggered by the NvM MemIfAbstraction within RAM area accessible by both Cores.
 *               Push current state of underlying devices to the NvM MemIfAbstraction into RAM area accessible by both 
 *               Cores.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, NVM_CODE) NvM_MemIfMemoryCore_MainFunction(void);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif  /* NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON */

#endif /* NVM_MEMIFMEMORYCORE */

/**********************************************************************************************************************
 *  END OF FILE: NvM_MemIfMemoryCore.h
 *********************************************************************************************************************/

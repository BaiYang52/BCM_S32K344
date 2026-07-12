/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actISipHash.h
 *        \brief  Interface for SipHash
 *
 *      \details  Implements SipHash according to 'SipHash: a short-input PRF' from Jean-Philippe Aumasson and Daniel
 *                Bernstein
 *                This file is part of the embedded systems library cvActLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ACTISIPHASH_H
# define ACTISIPHASH_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

# include "actITypes.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if defined ACT_PLATFORM_UINT64_AVAILABLE
/***********************************************************************************************************************
 *  actSipHashInit
 **********************************************************************************************************************/
/*! \brief         Initializes SipHash calculation.
 *  \details       This function initializes the SipHash calculation and stores the user key.
 *  \param[in,out] info  Pointer to context structure
 *  \param[in]     key  AES key to be used
 *  \param[in]     key_length  Size of AES key to be used
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *                 actEXCEPTION_NULL  Key is NULL
 *                 actEXCEPTION_LENGTH  Invalid key length
 *  \pre           info must be a valid workspace pointer
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSipHashInit(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key,
  actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));

/***********************************************************************************************************************
 *  actSipHashUpdate
 **********************************************************************************************************************/
/*! \brief         Updates SipHash calculation.
 *  \details       This function updates SipHash calculation.
 *  \param[in,out] info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *                 actEXCEPTION_NULL  Null pointer exception
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           info must be a valid workspace pointer
 *                 workspace must be initialized
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSipHashUpdate(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));

/***********************************************************************************************************************
 *  actSipHashFinalize
 **********************************************************************************************************************/
/*! \brief         Finalizes SipHash calculation.
 *  \details       This function finalizes the SipHash calculation and provides the calculated MAC.
 *  \param[in,out] info  Pointer to context structure
 *  \param[out]    mac  Pointer to computed MAC
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *  \pre           info must be a valid workspace pointer
 *                 workspace must be initialized
 *                 mac must be a valid pointer
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSipHashFinalize(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU64) mac,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTISIPHASH_H */

/**********************************************************************************************************************
 *  END OF FILE: actISipHash.h
 *********************************************************************************************************************/

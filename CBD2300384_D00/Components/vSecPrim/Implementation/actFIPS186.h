/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actFIPS186.h
 *        \brief  FIPS186 PRNG interface.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#ifndef ACTFIPS186_H
# define ACTFIPS186_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actISHA.h"

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actInitializeFIPS186()
 *********************************************************************************************************************/
/*! \brief       Initialize FIPS-186.2 PRNG
 *  \details     This function initializes the FIPS186 pseudo random number generator. It must be called at least
 *               once before using the generator.
 *  \param[in,out] info  In: Pointer to FIPS context structure Out: Initialized FIPS context structure.
 *  \param[in]     seed  Initial value of the generator. The seed is the 'true' random input of a PRNG. We need
 *                       a 160 bit seed.
 *  \pre         info must be a valid workspace pointer
 *               seed must be at least 20 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actInitializeFIPS186(VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info, VSECPRIM_P2CONST_PARA(actU8) seed);

 /**********************************************************************************************************************
 * actGetFIPS186
 *********************************************************************************************************************/
/*!
 * \brief          Provide random bytes.
 * \details        This function is used to provide random bytes based on the provided workspaces secret state.
 * \param[in,out]  info        (in) pointer to FIPS context structure
 *                             (out) updated FIPS context structure
 * \param[in,out]  output      (in) pointer to the data to be randomized
 *                             (out) the randomized data
 * \param[in]      length      length of the data to be randomized in bytes
 * \pre            info must be a valid workspace pointer
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actGetFIPS186(
  VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) output,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

 /**********************************************************************************************************************
 * actWriteFIPS186
 *********************************************************************************************************************/
/*!
 * \brief          Stir up entropy.
 * \details        This function is used to stir the internal seed state.
 * \param[in,out]  info        (in) pointer to FIPS context structure
 *                             (out) updated FIPS context structure
 * \param[in]      new_seed    Pointer to the new entropy source
 * \pre            Initialized workspace. new_seed must be of size 20 bytes.
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actWriteFIPS186(
   VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info, VSECPRIM_P2CONST_PARA(actU8) new_seed);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTFIPS186_H */

/**********************************************************************************************************************
 *  END OF FILE: actFIPS186.h
 *********************************************************************************************************************/

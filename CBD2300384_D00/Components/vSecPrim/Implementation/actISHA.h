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
/*!        \file  actISHA.h
 *        \brief  Interface for SHA-1 hash algorithm.
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

#ifndef ACTISHA_H
# define ACTISHA_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"

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
 *  actSHAInit()
 *********************************************************************************************************************/
/*! \brief          Initialize SHA.
 *  \details        This function initializes the SHA-1 algorithm.
 *  \param[in,out]  info Pointer to the provided workspace.
 *  \return         actOK - Initialization performed
 *  \pre            info must be a valid workspace pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHAInit(VSECPRIM_P2VAR_PARA(actSHASTRUCT) info);

/**********************************************************************************************************************
 *  actSHAUpdate()
 *********************************************************************************************************************/
/*! \brief       Update SHA.
 *  \details     This function hashes the given data and can be called arbitrarily often between an initialize and
 *               finalize of the SHA-1 algorithm. Uses any data already in the actSHASTRUCT structure and leaves
 *               any partial data block there.
 *  \param[in,out]  info  Pointer to the provided workspace.
 *  \param[in]   dataIn  Pointer to data to be hashed
 *  \param[in]   length  Length of data in bytes
 *  \param[in]   watchdog  Watchdog function pointer
 *  \return      actOK - Operation successful
 *               actEXCEPTION_LENGTH - Total input size exceeds 2^64 - 1 bit
 *  \pre         info must be a valid workspace pointer
 *               workspace must be initialized
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHAUpdate(
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHAFinalize()
 *********************************************************************************************************************/
/*! \brief       Finalize SHA.
 *  \details     This function finalizes the SHA algorithm and delivers the hash value.
 *  \param[in,out]  info  Pointer to the provided workspace.
 *  \param[in,out]  hash  In: Byte array to contain the hash value; Out: The final hash value.
 *  \param[in]   watchdog  Watchdog function pointer
 *  \return      actOK - Operation successful
 *  \pre         info must be a valid workspace pointer
 *               workspace must be initialized
 *               hash must be at least 20 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHAFinalize(
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHAGetDigest()
 *********************************************************************************************************************/
/*! \brief       Get Digest
 *  \details     Get the current (partial / internal) digest, WITHOUT FINALIZING the hash first.
 *  \param[in]       info  Pointer to hash context structure
 *  \param[in,out]   hash  In: Byte array to contain the digest value. Out: The current digest value.
 *  \param[in]       watchdog  Pointer to watchdog reset function
 *  \pre         info must be a valid workspace pointer
 *               workspace must be initialized
 *               hash must be at least 20 bytes
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actSHAGetDigest(
  VSECPRIM_P2CONST_PARA(actSHASTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTISHA_H */

/**********************************************************************************************************************
 *  END OF FILE: actISHA.h
 *********************************************************************************************************************/

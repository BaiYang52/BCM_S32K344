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
/*!        \file  actISHA2_64.h
 *        \brief  Interface for SHA-384 and SHA-512 hash algorithms.
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#ifndef ACTISHA2_64_H
# define ACTISHA2_64_H

# include "actITypes.h"

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actSHA384Init()
 *********************************************************************************************************************/
/*! \brief       Initialize SHA-384
 *  \details     This function initializes the SHA-384 algorithm.
 *  \param[in,out]   info  Pointer to the workspace.
 *  \return      actOK - always
 *  \pre         info must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA384Init(VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info);

/**********************************************************************************************************************
 *  actSHA512Init()
 *********************************************************************************************************************/
/*! \brief       Initialize SHA-512
 *  \details     This function initializes the SHA-512 algorithm.
 *  \param[in,out]   info  Pointer to the workspace.
 *  \return      actOK - always
 *  \pre         info must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512Init(VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info);

/**********************************************************************************************************************
 *  actSHA512_224Init()
 *********************************************************************************************************************/
/*! \brief       Initialize SHA-512-224
 *  \details     This function initializes the SHA-512-224 algorithm.
 *  \param[in,out]   info  Pointer to the workspace.
 *  \return      actOK - always
 *  \pre         info must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Init(VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info);

/**********************************************************************************************************************
 *  actSHA512_256Init()
 *********************************************************************************************************************/
/*! \brief       Initialize SHA-512-256
 *  \details     This function initializes the SHA-512-256 algorithm.
 *  \param[in,out]   info  Pointer to the workspace.
 *  \return      actOK - always
 *  \pre         info must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Init(VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info);

/**********************************************************************************************************************
 *  actSHA384Update()
 *********************************************************************************************************************/
/*! \brief       Update of SHA-384 calculation.
 *  \details     This function hashes the given data and can be called arbitrarily often between an initialize and
 *               finalize of the hash algorithm. Uses any data already in the actSHA384STRUCT structure and leaves
 *               any partial data block there.
 *  \param[in,out]   info  Pointer to the workspace
 *  \param[in]   dataIN  Pointer to data to be hashed
 *  \param[in]   length  Length of data in bytes
 *  \param[in]   watchdog  Pointer to the watchdog trigger function
 *  \return      actOK - OK
 *               actEXCEPTION_LENGTH - total input more than 2^64 - 1 bit
 *  \pre         actSHA384Init() has been called before calling this function
 *               dataIN != NULL is a valid pointer
 *               length >= 0
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA384Update(
  VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512Update()
 *********************************************************************************************************************/
/*! \brief       Update of SHA-512 calculation.
 *  \details     This function hashes the given data and can be called arbitrarily often between an initialize and
 *               finalize of the hash algorithm. Uses any data already in the actSHA512STRUCT structure and leaves
 *               any partial data block there.
 *  \param[in,out]   info  Pointer to the workspace
 *  \param[in]   dataIN  Pointer to data to be hashed
 *  \param[in]   length  Length of data in bytes
 *  \param[in]   watchdog  Pointer to the watchdog trigger function
 *  \return      actOK - OK
 *               actEXCEPTION_LENGTH - total input more than 2^64 - 1 bit
 *  \pre         actSHA512Init() has been called before calling this function
 *               dataIN != NULL is a valid pointer
 *               length >= 0
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512Update(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512_224Update()
 *********************************************************************************************************************/
/*! \brief       Update of SHA-512-224 calculation.
 *  \details     This function hashes the given data and can be called arbitrarily often between an initialize and
 *               finalize of the hash algorithm. Uses any data already in the actSHA512_224STRUCT structure and leaves
 *               any partial data block there.
 *  \param[in,out]   info  Pointer to the workspace
 *  \param[in]   dataIN  Pointer to data to be hashed
 *  \param[in]   length  Length of data in bytes
 *  \param[in]   watchdog  Pointer to the watchdog trigger function
 *  \return      actOK - OK
 *               actEXCEPTION_LENGTH - total input more than 2^64 - 1 bit
 *  \pre         actSHA512_224Init() has been called before calling this function
 *               dataIN != NULL is a valid pointer
 *               length >= 0
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Update(
  VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512_256Update()
 *********************************************************************************************************************/
/*! \brief       Update of SHA-512-256 calculation.
 *  \details     This function hashes the given data and can be called arbitrarily often between an initialize and
 *               finalize of the hash algorithm. Uses any data already in the actSHA512_256STRUCT structure and leaves
 *               any partial data block there.
 *  \param[in,out]   info  Pointer to the workspace
 *  \param[in]   dataIN  Pointer to data to be hashed
 *  \param[in]   length  Length of data in bytes
 *  \param[in]   watchdog  Pointer to the watchdog trigger function
 *  \return      actOK - OK
 *               actEXCEPTION_LENGTH - total input more than 2^64 - 1 bit
 *  \pre         actSHA512_256Init() has been called before calling this function
 *               dataIN != NULL is a valid pointer
 *               length >= 0
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Update(
  VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA384Finalize()
 *********************************************************************************************************************/
/*! \brief       Finalize SHA-384 calculation.
 *  \details     This function finalizes the hash algorithm and delivers the hash value.
 *  \param[in,out]  info  Pointer to workspace
 *  \param[out]     hash  Pointer where the hash shall be stored.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK - always
 *  \pre         hash must have at least size of 48 bytes
 *               actSHA384Init() has been called before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA384Finalize(
  VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512Finalize()
 *********************************************************************************************************************/
/*! \brief       Finalize SHA-512 calculation.
 *  \details     This function finalizes the hash algorithm and delivers the hash value.
 *  \param[in,out]  info  Pointer to workspace
 *  \param[out]     hash  Pointer where the hash shall be stored.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK - always
 *  \pre         hash must have at least size of 64 bytes
 *               actSHA512Init() has been called before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512_224Finalize()
 *********************************************************************************************************************/
/*! \brief       Finalize SHA-512-224 calculation.
 *  \details     This function finalizes the hash algorithm and delivers the hash value.
 *  \param[in,out]  info  Pointer to workspace
 *  \param[out]     hash  Pointer where the hash shall be stored.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK - always
 *  \pre         hash must have at least size of 28 bytes
 *               actSHA512_224Init() has been called before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA512_256Finalize()
 *********************************************************************************************************************/
/*! \brief       Finalize SHA-512-256 calculation.
 *  \details     This function finalizes the hash algorithm and delivers the hash value.
 *  \param[in,out]  info  Pointer to workspace
 *  \param[out]     hash  Pointer where the hash shall be stored.
 *  \param[in]      watchdog     pointer to the watchdog function
 *  \return      actOK - always
 *  \pre         hash must have at least size of 32 bytes
 *               actSHA512_256Init() has been called before calling this function
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
  } /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTISHA2_64_H */
/**********************************************************************************************************************
 *  END OF FILE: actISHA2_64.h
 *********************************************************************************************************************/

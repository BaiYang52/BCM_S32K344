/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2005 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIRMD160.h
 *        \brief  This file contains the interface for RMD160 hash algorithm.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

#ifndef ACTIRMD160_H
# define ACTIRMD160_H

# include "actITypes.h"

/* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actRMD160Init()
 *********************************************************************************************************************/
/*! \brief          This function initializes the RMD160 algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to hash context structure
 *                               (out) initialized hash context structure
 *  \return         actOK always
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRMD160Init(VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info);

/**********************************************************************************************************************
 *  actRMD160Update()
 *********************************************************************************************************************/
/*! \brief          This function hashes the given data.
 *  \details        This function hashes the given data and can be called arbitrarily
 *                  often between an initialize and finalize of the RMD160 algorithm.
 *                  Uses any data already in the actRMD160STRUCT structure and leaves
 *                  any partial data block there.
 *  \param[in,out]  info         (in) pointer to hash context structure
 *                               (out) actualized hash context structure
 *  \param[in]      dataIN       pointer to data to be hashed
 *  \param[in]      length       length of data in bytes
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 *                  actOK                 else
 *  \pre            actRMD160Init() is called once before calling this function
 *                  dataIN != NULL is a valid pointer
 *                  length >= 0
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRMD160Update(
   VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actRMD160Finalize()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the RMD160 algorithm and delivers the hash value.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to hash context structure
 *                               (out) finalized hash context structure
 *  \param[in,out]  hash         (in) byte array to contain the hash value
 *                               (out) the final hash value,
 *                               (big endian of length actHASH_SIZE_RMD160)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            actRMD160Init() is called once before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actRMD160Finalize(
   VSECPRIM_P2VAR_PARA(actRMD160STRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

#endif /* ACTIRMD160_H */


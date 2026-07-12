/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2020 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actKECCAKc.h
 *        \brief  Interface for KECCAK[c] function algorithm.
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

#ifndef ACTKECCAKC_H
# define ACTKECCAKC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define KECCAKc_PHASE                                                (0x0Fu)
# define KECCAKc_PHASE_UNDEFINED                                      (0x00u)
# define KECCAKc_PHASE_INITIAL                                        (0x01u)
# define KECCAKc_PHASE_ABSORB                                         (0x02u)
# define KECCAKc_PHASE_SQUEEZE                                        (0x03u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
    extern "C" {
# endif

/**********************************************************************************************************************
 *  actKECCAKcInit()
 *********************************************************************************************************************/
/*! \brief          Initialize the KECCAK[c] algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) initialized KECCAK[c] context structure
 *  \param[in]      strength     security strength (BITs) - half the capacity
 *  \param[in]      pad1         1st padding character;
 *                               the 2nd one is identical for SHA3-nnn and SHAKEnnn
 *  \return         actEXCEPTION_LENGTH   when strength is no multiple of 32
 *                  (2* strength MUST be a multiple of LANE size (64))
 *                  actOK                 otherwise
 *  \pre            info must be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKECCAKcInit(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  const actU32       strength,
  const actU8        pad1);

/**********************************************************************************************************************
 *  actKECCAKcAbsorb()
 *********************************************************************************************************************/
/*! \brief          Absorb data into the KECCAK[c] sponge.
 *                  Can be called arbitrarily often between initialize and squeeze.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) updated KECCAK[c] context structure
 *  \param[in]      data         pointer to data to be hashed
 *  \param[in]      dataLength   length of data (bytes)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_MODE   when called AFTER squeeze
 *                  actOK               otherwise
 *  \pre            actKECCAKcInit() is called once before calling this function
 *                  all pointers must be valid
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKECCAKcAbsorb(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dataLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actKECCAKcSqueeze()
 *********************************************************************************************************************/
/*! \brief          Squeeze the digest from the KECCAK[c] sponge.
 *                  Can be called arbitrarily often to deliver more result bits
 *                  (e.g. for SHAKExxx).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) finalized KECCAK[c] context structure
 *  \param[in,out]  digest       (in) byte array to hold the digest
 *                               (out) the final KECCAK[c] value
 *  \param[in]      digestSize   requested size of digest (BITs)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH   when digestLength is no multiple of 8
 *                  actOK                 otherwise
 *  \pre            actKECCAKcInit() is called once before calling this function
 *                  All pointers must be valid.
 *                  The pointer referenced by digest must provide at least digestSize many bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKECCAKcSqueeze(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) digest,
  const actLengthType digestSize,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actKECCAKcZEROPadToRate()
 *********************************************************************************************************************/
/*! \brief          ZERO-pad the accumulated input to a multiple of the rate (bytes).
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) updated KECCAK[c] context structure
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            actKECCAKcInit() is called once before calling this function
 *                  info must be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actKECCAKcZEROPadToRate(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus  /* COV_VSECPRIM_CPLUSPLUS XF */
    }  /*  extern "C"  */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTKECCAKC_H  */

/**********************************************************************************************************************
 *  END OF FILE: actKECCAKc.h
 *********************************************************************************************************************/


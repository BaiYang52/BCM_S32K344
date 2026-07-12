/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actIDRBG.h
 *        \brief  DRBG implementation.
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

#ifndef ACTIDRBG_H
# define ACTIDRBG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actHASHDRBG_SEEDING_PREFIX1                                  (0x00u)
# define actHASHDRBG_SEEDING_PREFIX2                                  (0x01u)
# define actHASHDRBG_GENERATE_PREFIX1                                 (0x02u)
# define actHASHDRBG_GENERATE_PREFIX2                                 (0x03u)

# define actDRBGSEED_INPUT_PARAM1                                     (0u)
# define actDRBGSEED_INPUT_PARAM2                                     (1u)
# define actDRBGSEED_INPUT_PARAM3                                     (2u)
# define actDRBGSEED_INPUT_PARAM4                                     (3u)

# define actDRBG_SIZEOF_SEEDINPUT                                     (4u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* DRBG Seed Input Element Type Structure */
typedef struct
{
  VSECPRIM_P2CONST_PARA(actU8) dataPtr; /**< Input data pointer */
  actLengthType length;                 /**< Length of input data */
} actAesCtrDrbgSeedInputElementType;

/* CTR DRBG: 3 input elements used
 * Hash DRBG: initial seeding (instantiation)  - 3 input elements used
 *            reseeding (instantiation)        - 4 input elements used
 *            random number generation         - 3 input elements used */

typedef struct
{
  actU8 elements;                              /**< Number of input elements */
  actAesCtrDrbgSeedInputElementType input[actDRBG_SIZEOF_SEEDINPUT]; /**< Input data information */
} actDrbgSeedInputType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# if (VSECPRIM_ACTCTRDRBG_ENABLED == STD_ON)

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actAesCtrDrbgInstantiation()
 **********************************************************************************************************************/
/*! \brief          This function implements the instantiation function specified in 10.2.1.3
 *  \details        -
 *  \param[in,out]  workSpace     (in) pointer to the DRBG workspace
 *                                (out) DRBG workspace, key and variable are modified
 *  \param[in]      dataInput     pointer to the input data
 *  \param[in]      reseedFlag    identifier if this is a reseed request
 *  \param[in]      watchdog      watchdog function pointer
 *  \return         actOK                      DRBG instantiation successful
 *                  actEXCEPTION_INPUT_LENGTH  the sum of the length of the elements in dataInput is greater than
 *                                             uint32max
 *  \pre            workSpace has to be a valid pointer to a variable of type actCTRDRBGSTRUCT.
 *                  dataInput has to be a valid pointer to a variable of type actDrbgSeedInputType and has to
 *                  include at least one element.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAesCtrDrbgInstantiation(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) dataInput,
  boolean reseedFlag,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actAesCtrDrbgGenerate()
 **********************************************************************************************************************/
/*! \brief         This function generates requestedLength many random bytes using the AES CTR DRBG
 *  \details       -
 *  \param[in,out] workSpace                (in) pointer to the DRBG workspace
 *                                          (out) DRBG workspace, wsAES, key and variable modified
 *  \param[in]     requestedLength          number of random bytes to be generated
 *  \param[in,out] outputBuffer             (in)  Pointer to the output buffer
 *                                          (out) output buffer, containing requestedLength random bytes
 *  \param[in]     additionalInputPtr       pointer to additional input buffer, optional parameter,
 *                                          possibly a null pointer
 *  \param[in]     additionalInputLength    length of the additional input in bytes, possibly 0
 *  \param[in]     watchdog                 watchdog function pointer
 *  \return        actOK                           generation was successful
 *                 actEXCEPTION_INPUT_LENGTH       additionalInputLength is greater than uint32max
 *                 actEXCEPTION_ENTROPY_EXHAUSTION entropy exhausted, DRBG needs reseeding
 *  \pre           All pointers have to be valid.
 *                 The buffer referenced by outputBuffer must provide at least requestedLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actAesCtrDrbgGenerate(
  VSECPRIM_P2VAR_PARA(actCTRDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif

# if (VSECPRIM_ACTHASHDRBG_ENABLED == STD_ON)

#  define VSECPRIM_START_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashDrbgInstantiation()
 **********************************************************************************************************************/
/*! \brief         This function instantiates the Hash DRBG based on SHA512 for initial or re-seeding.
 *  \details       -
 *  \param[in,out]  workSpace     (in) pointer to the DRBG workspace
 *                                (out) DRBG workspace, constant, variable and reseedCounter are modified
 *  \param[in]      seedMaterial  pointer to the input data
 *  \param[in]      watchdog      watchdog function pointer
 *  \pre            All pointers have to be valid.
 *                  include at least one element.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actHashDrbgInstantiation(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  VSECPRIM_P2CONST_PARA(actDrbgSeedInputType) seedMaterial,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actHashDrbgGenerate()
 **********************************************************************************************************************/
/*! \brief         This function generates requestedLength many random bytes using the Hash DRBG based on SHA512
 *  \details       -
 *  \param[in,out] workSpace                (in) pointer to the DRBG workspace
 *                                          (out) DRBG workspace, wsSHA512, constant, variable and
 *                                                reseedCounter modified
 *  \param[in]     requestedLength          number of random bytes to be generated
 *  \param[in,out] outputBuffer             (in)  Pointer to the output buffer
 *                                          (out) output buffer, containing requestedLength random bytes
 *  \param[in]     additionalInputPtr       pointer to additional input buffer, optional parameter,
 *                                          possibly a null pointer
 *  \param[in]     additionalInputLength    length of the additional input in bytes, possibly 0
 *  \param[in]     watchdog                 watchdog function pointer
 *  \return        actOK                           generation was successful
 *                 actEXCEPTION_ENTROPY_EXHAUSTION entropy exhausted, DRBG needs reseeding
 *  \pre           All pointers have to be valid.
 *                 The buffer referenced by outputBuffer must provide at least requestedLength bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHashDrbgGenerate(
  VSECPRIM_P2VAR_PARA(actHASHDRBGSTRUCT) workSpace,
  actLengthType requestedLength,
  VSECPRIM_P2VAR_PARA(actU8) outputBuffer,
  VSECPRIM_P2CONST_PARA(actU8) additionalInputPtr,
  actLengthType additionalInputLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

#  define VSECPRIM_STOP_SEC_CODE
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif

#endif /* ACTIDRBG_H */
/**********************************************************************************************************************
 *  END OF FILE: actIDRBG.h
 *********************************************************************************************************************/


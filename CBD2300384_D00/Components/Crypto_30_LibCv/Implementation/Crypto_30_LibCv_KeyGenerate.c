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
/*      \file  Crypto_30_LibCv_KeyGenerate.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of Key Generate Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_KEYGENERATE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_KeyGenerate.h"
#include "Crypto_30_LibCv_Curve.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_KEY_GENERATE_SIZEOF_ALGORITHM                 (1u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_SYMMETRIC_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate_Symmetric()
 *********************************************************************************************************************/
/*! \brief         Generation of the symmetric key values
 *  \details       Generates the symmetric key values and stores the key in the memory location pointed to by the key
 *                 value pointer. The write access right for the destination key element must be less than or equal
 *                 to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation.
 *  \return        E_OK                        Request successful.
 *                 E_NOT_OK                    Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Symmetric(uint32 cryptoKeyId);
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_SYMMETRIC_GENERIC_ENABLED == STD_ON */

#if ((CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic()
 *********************************************************************************************************************/
/*! \brief         Key value generation
 *  \details       Generates the key value for the key generate and stores the key in the memory location pointed
 *                 by the key value pointer. The write access right for the destination key element must be less
 *                 or equal than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     speedUpExtPtr           Pointer to precomputation structure
 *  \param[in]     keySize                 Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 all pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic(
  uint32 cryptoKeyId,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Generation of the key value
 *  \details       Generates the key value and stores the public key in the memory location pointed to by the public
 *                 value pointer. The write access right for the destination key element must be less or equal
 *                 than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     speedUpExtPtr           Pointer to precomputation structure
 *  \param[in]     keySize                 Contains the length of the key element in bytes.
 *  \param[in,out] workspace               Contains the workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON || CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON */

#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate_Ed25519()
 *********************************************************************************************************************/
/*! \brief         Key value generation
 *  \details       Generates the key values for the Ed25519 key generate algorithm by passing a suitable workspace to
 *                 Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws and stores the keys in the memory location
 *                 pointed to by the key value pointer. The write access right for the destination key elements must
 *                 be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation
 *  \return        E_OK                        Request successful.
 *                 E_NOT_OK                    Request failed.
 *                 CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Ed25519(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Generation of the key values
 *  \details       Generates the key values according to Ed25519 key generation algorithm and stores the keys in the
 *                 memory location pointed to by the key value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation
 *  \param[in,out] workspace               Contains the workspace.
 *  \return        E_OK                        Request successful.
 *                 E_NOT_OK                    Request failed.
 *                 CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 workspace has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Ed25519_Calculate_With_Ws()
 *********************************************************************************************************************/
/*! \brief          Calculate Ed25519 key pair
 *  \details        Calculates the private and public Ed25519 key pair.
 *  \param[in,out]  workspace               Contains the workspace.
 *  \param[out]     privateValuePtr         Contains the pointer to the data where the private value shall be stored.
 *  \param[in,out]  privateValueLengthPtr   Contains the pointer to the data where the private length value shall be stored.
 *  \param[out]     publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out]  publicValueLengthPtr    Contains the pointer to the data where the public length value shall be stored.
 *  \return         E_OK                    Request successful.
 *                  E_NOT_OK                Request failed.
 *  \pre            All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Ed25519_Calculate_With_Ws(
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValuePtr,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValueLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519_ENABLED */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_SYMMETRIC_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate_Symmetric()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Symmetric(uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_ErrorCode getBytesRetVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 kgkLength;
  uint8 keyBuffer[CRYPTO_30_LIBCV_KEY_GENERATE_MAX_LENGTH];
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Search Key Element */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    kgkLength = Crypto_30_LibCv_GetLengthOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex));

    /* #20 Check key length */
    if (kgkLength <= CRYPTO_30_LIBCV_KEY_GENERATE_MAX_LENGTH)
    {
      /* #30 Generate random values */
      getBytesRetVal = esl_getBytesRNG((eslt_Length)kgkLength, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyBuffer); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      if (getBytesRetVal == E_OK)
      {
        /* #40 Set random values to the key element */
        retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, (P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyBuffer, kgkLength); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }

      Crypto_30_LibCv_ClearData(keyBuffer, kgkLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
  }
  else
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_SYMMETRIC_GENERIC_ENABLED == STD_ON */

#if ((CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic(
  uint32 cryptoKeyId,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize)
{
  /* #10 Init Workspace */
  eslt_WorkSpaceEcP workspace;

  /* #20 Call Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws */
  return Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws(
    cryptoKeyId,
    domainPtr,
    domainExtPtr,
    speedUpExtPtr,
    keySize,
    &workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_MSR_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 privKey[CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE];
  uint8 pubKey[Crypto_30_LibCv_Math_Mul2(CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE)];
  uint32 doubleKeySize;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Calculate Keys */
  if (Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws(pubKey, privKey, domainPtr, domainExtPtr, speedUpExtPtr, keySize, workspace) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    doubleKeySize = Crypto_30_LibCv_Math_Mul2(keySize);

    /* #11 Store public and private key to key elements if calculation was successfully */
    retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, pubKey, doubleKeySize); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (retVal == E_OK)
    {
      retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, privKey, keySize); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
    /* #12 delete buffer content from RAM */

    Crypto_30_LibCv_ClearData(privKey, keySize); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    Crypto_30_LibCv_ClearData(pubKey, doubleKeySize); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON || CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON */

#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate_Ed25519()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Ed25519(
  uint32 cryptoKeyId)
{
  /* #10 Init Workspace */
  eslt_WorkSpaceEd25519 workspace;

  /* #20 Call Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws */
  return Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws(
    cryptoKeyId,
    &workspace); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 privKey[ESL_SIZEOF_Ed25519_SECRET_KEY];
  uint8 pubKey[ESL_SIZEOF_Ed25519_PUBLIC_KEY];
  eslt_Length privKeyLength = ESL_SIZEOF_Ed25519_SECRET_KEY;
  eslt_Length pubKeyLength = ESL_SIZEOF_Ed25519_PUBLIC_KEY;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Calculate Keys */
  if (Crypto_30_LibCv_Local_Ed25519_Calculate_With_Ws(workspace,
    privKey,
    &privKeyLength,
    pubKey,
    &pubKeyLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  {
    /* #11 Store public and private key to key elements if calculation was successfully */
    retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, pubKey, pubKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (retVal == E_OK)
    {
      retVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYGENERATE_KEY, privKey, privKeyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }

    /* #12 delete buffer content from RAM */
    Crypto_30_LibCv_ClearData(privKey, privKeyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    Crypto_30_LibCv_ClearData(pubKey, pubKeyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Ed25519_Calculate_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Ed25519_Calculate_With_Ws(
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValuePtr,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValueLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Init Workspace */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_SIZEOF_WS_Ed25519, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initGenerateKeyPairEdDSA(workspace, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #20 Calculate Keys */
    if (esl_generateKeyPairEdDSA(workspace,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))privateValuePtr,
      privateValueLengthPtr,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))publicValuePtr,
      publicValueLengthPtr) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
    {
      retVal = E_OK;
    }
  }
  return retVal;
}

#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519_ENABLED */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyGenerate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050,6080 1 */ /* MD_MSR_STCAL,MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate(uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyStorageType kgaIndex;
  uint32 kgaLength = CRYPTO_30_LIBCV_KEY_GENERATE_SIZEOF_ALGORITHM;

  /* ----- Implementation ------------------------------------------------- */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
# if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
  /* #5 Lock read access for the DefaultRandomKey */
  if (Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(Crypto_30_LibCv_GetDefaultRandomKey()) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
# endif
  {
    /* #10 Lock write access for the CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      /* #20 Read and check algorithm key element */
      if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYGENERATE_ALGORITHM, &kgaIndex, &kgaLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      {

      }
      else
      {
# if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_SYMMETRIC_GENERIC_ENABLED == STD_ON)
        if (Crypto_30_LibCv_GetKeyStorage(kgaIndex) == CRYPTO_30_LIBCV_KEY_GENERATE_SYMMETRIC)
        {
          /* #30 Execute KGA Symmetric */
          retVal = Crypto_30_LibCv_Local_KeyGenerate_Symmetric(cryptoKeyId);
        }
        else
# endif
# if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON)
          if (Crypto_30_LibCv_GetKeyStorage(kgaIndex) == CRYPTO_30_LIBCV_KEY_GENERATE_P256R1)
          {
            /* #40 Execute KGA ANSI_NIST_P256R1 */
            /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
            retVal = Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic(cryptoKeyId,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
              (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
              (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt,
              CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
          }
          else
# endif
# if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON)
            if (Crypto_30_LibCv_GetKeyStorage(kgaIndex) == CRYPTO_30_LIBCV_KEY_GENERATE_P384R1)
            {
              /* #50 Execute KGA NIST_SEC_P384R1 */
              /* NISTp384r1 = SECp384r1 */
              retVal = Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic(cryptoKeyId,
                (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1Domain,
                (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt,
                (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt,
                CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            }
            else
# endif
# if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519_ENABLED == STD_ON)
              if (Crypto_30_LibCv_GetKeyStorage(kgaIndex) == CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ED25519)
              {
                /* #60 Execute KGA ED25519 */
                retVal = Crypto_30_LibCv_Local_KeyGenerate_Ed25519(cryptoKeyId);
              }
              else
# endif
              {
                /* invalid algorithm */
              }

        if (retVal != E_OK)
        {
          retVal = E_NOT_OK;
        }
      }
      /* #70 Release key write lock */
      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(cryptoKeyId);
    }
# if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
    Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(Crypto_30_LibCv_GetDefaultRandomKey());
# endif
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON */

#if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_MSR_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE)  Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValuePtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Init Workspace */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initGenerateKeyEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, domainPtr, domainExtPtr, speedUpExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    if (esl_getLengthOfEcPprivateKey(domainPtr) == keySize) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
    {
      if (esl_getLengthOfEcPpublicKey_comp(domainPtr) == keySize) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
      {
        /* #20 Calculate Keys */
        if (esl_generateKeyEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace,
          (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))privateValuePtr, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))publicValuePtr,
          (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(publicValuePtr[keySize])) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
        {
          retVal = E_OK;
        }
      }
    }
  }
  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_KEYGENSYMGENERIC == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyGenSymGeneric()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenSymGeneric( uint32 objectId,
      P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
      Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Generate symmetric key */
    retVal = Crypto_30_LibCv_Local_KeyGenerate_Symmetric(job->cryptoKeyId);
  }

  CRYPTO_30_LIBCV_DUMMY_STATEMENT(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYGENSYMGENERIC == STD_ON */

#if (CRYPTO_30_LIBCV_KEYGENP256R1 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyGenP256R1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenP256R1( uint32 objectId,
                                                                                  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
                                                                                  Crypto_OperationModeType mode)
{

  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyGenP256R1(Crypto_30_LibCv_GetKeyGenP256R1IdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #20 Calculate public value (Key pair of private and public) according to ECC curve p256r1 */
    retVal = Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws(job->cryptoKeyId,
      (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
      (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
      (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt,
      CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE,
      workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYGENP256R1 == STD_ON */

#if (CRYPTO_30_LIBCV_KEYGENP384R1 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyGenP384R1()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenP384R1( uint32 objectId,
                                                                                  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
                                                                                  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyGenP384R1(Crypto_30_LibCv_GetKeyGenP384R1IdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to ECC curve p384r1 */
    retVal = Crypto_30_LibCv_Local_KeyGenerate_ECC_Generic_With_Ws(job->cryptoKeyId,
      (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1Domain,
      (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt,
      (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt,
      CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE,
      workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYGENP384R1 == STD_ON */

#if (CRYPTO_30_LIBCV_KEYGENED25519 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyGenEd25519()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenEd25519( uint32 objectId,
                                                                                   P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
                                                                                   Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyGenEd25519(Crypto_30_LibCv_GetKeyGenEd25519IdxOfObjectInfo(objectId));

    /* #10 Calculate Key pair of private and public according to Ed25519 */
    retVal = Crypto_30_LibCv_Local_KeyGenerate_Ed25519_With_Ws(job->cryptoKeyId, workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYGENED25519 == STD_ON */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyGenerate.c
 *********************************************************************************************************************/

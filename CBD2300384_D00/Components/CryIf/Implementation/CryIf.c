/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CryIf.c
 *        \brief  MICROSAR Crypto Interface (CRYIF)
 *
 *      \details  The Crypto Interface module provides a unique interface to manage different Crypto HW and SW
 *                solutions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define CRYIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "CryIf_Cbk.h"
#include "Csm_Cbk.h"

#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CryIf header file */
#if ((  CRYIF_SW_MAJOR_VERSION != (5u)) \
    || (CRYIF_SW_MINOR_VERSION != (1u)) \
    || (CRYIF_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of CryIf.c and CryIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define CRYIF_FIRST_KEY_PRIMITIVE                                     CRYPTO_RANDOMSEED

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define CryIf_Util_MemClr(dst, count)                                 CryIf_Util_MemSet(dst, 0u, count)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CRYIF_LOCAL) /* COV_CRYIF_ALWAYS_ON */
# define CRYIF_LOCAL                                                  static
#endif

#if !defined (CRYIF_LOCAL_INLINE) /* COV_CRYIF_ALWAYS_ON */
# define CRYIF_LOCAL_INLINE                                           LOCAL_INLINE
#endif
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CRYIF_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
CRYIF_LOCAL VAR(uint8, CRYIF_VAR_ZERO_INIT) CryIf_ModuleInitialized = CRYIF_UNINIT;
#endif

#define CRYIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  CryIf_KeyCopyForDifferentDrivers()
 *********************************************************************************************************************/
/*! \brief         Copies two key between two drivers.
 *  \details       Gets each key element of set source key and writes it to a matching key element of the target key.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key whose key element shall be the source
 *                                         element.
 *  \param[in]     targetCryIfKeyId        Holds the identifier of the key whose key element shall be the destination
 *                                         element.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL Std_ReturnType CryIf_KeyCopyForDifferentDrivers(uint32 cryIfKeyId, uint32 targetCryIfKeyId);

#if (CRYIF_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 *  CryIf_TranslateRedirectionKeys()
 *********************************************************************************************************************/
/*! \brief        Translates CryIf redirection keys to Crypto keys.
 *  \details       If the keyId is valid, it is translated to a crypto Key Id
 *  \param[in,out] keyId    Pointer to the cryIfKeyId.
 *  \return        CRYIF_E_NO_ERROR        Translation was successful.
 *  \return        CRYIF_E_PARAM_HANDLE    Translation was not successful.
 *  \pre           Parameter keyId must point to an accessible memory location and must be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different keyIds
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL FUNC(Std_ReturnType, CRYIF_CODE) CryIf_TranslateRedirectionKeys(P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) keyId);

/**********************************************************************************************************************
 *  CryIf_UpdateRedirectionInfo()
 *********************************************************************************************************************/
/*! \brief          Updates redirection info ob job objects.
 *  \details        If a redirection for the corresponding job is available, this function translates the CryIf KeyIds to Crypto KeyIds.
 *  \param[in]      job    Pointer to the corresponding job object
 *  \return         E_OK                    Redirection was successful.
 *  \return         E_NOT_OK                Redirection failed.
 *  \pre            Parameter job must point to a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL FUNC(Std_ReturnType, CRYIF_CODE) CryIf_UpdateRedirectionInfo(P2CONST(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job);
#endif /* (CRYIF_REDIRECTION == STD_ON) */

/**********************************************************************************************************************
 *  CryIf_Util_Max()
 *********************************************************************************************************************/
/*! \brief       Calculates the maximum value of the inputs.
 *  \details     The function calculates for the two given values the maximum and returns its value.
 *  \param[in]   a    First Value
 *  \param[in]   b    Second value
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL_INLINE FUNC(uint32, CRYIF_CODE) CryIf_Util_Max(uint32 a, uint32 b);

/**********************************************************************************************************************
 *  CryIf_Util_MemSet()
 *********************************************************************************************************************/
/*! \brief       Sets destination buffer to pattern.
 *  \details     The function sets the destination bytewise to a specified pattern.
 *  \param[out]  dst      Pointer to the destination buffer
 *  \param[in]   pattern  Pattern which shall be written to the destination buffer.
 *  \param[in]   count    Number of bytes which shall be set
 *  \pre         dst must point to a valid memory location. The buffer pointed to by dst has to have a capacity of at least count bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL_INLINE FUNC(void, CRYIF_CODE) CryIf_Util_MemSet(P2VAR(uint8, AUTOMATIC, AUTOMATIC) dst, uint8 pattern, uint32 count);

/**********************************************************************************************************************
 *  CryIf_Util_MemCpy()
 *********************************************************************************************************************/
/*! \brief       Copies values from one buffer to another.
 *  \details     The function copies a specified amount of bytes from a source buffer to a destination buffer.
 *  \param[out]  dst    Pointer to the destination buffer
 *  \param[in]   src    Pointer to the source buffer
 *  \param[in]   count  Number of bytes which shall be copied
 *  \pre         dst and src must point to valid memory locations. The buffer pointed to by dst has to have a capacity of at least count bytes.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CRYIF_LOCAL_INLINE FUNC(void, CRYIF_CODE) CryIf_Util_MemCpy(P2VAR(uint8, AUTOMATIC, AUTOMATIC) dst, P2CONST(uint8, AUTOMATIC, AUTOMATIC) src, uint32 count);

/**********************************************************************************************************************
 *  CryIf_KeyElementCopyPartial_Internal()
 *********************************************************************************************************************/
/*! \brief         Copies key element partially without using the Crypto Service.
 *  \details       This function copies key element partially without using the Crypto Service. This is necessary if either
 *                 the keys are lying in different Crypto Drivers or if the underyling Crypto Driver do not support KeyElementCopyPartial Service.
 *  \param[in]     cryptoFctIdxSrc            Internal array index of the crypto function table for the source key
 *  \param[in]     cryptoFctIdxDst            Internal array index of the crypto function table for the target key
 *  \param[in]     cryptoKeyId                Holds the identifier of the key whose key element shall be the source
 *                                            element.
 *  \param[in]     keyElementId               Holds the identifier of the key element which shall be the source for the
 *                                            copy operation.
 *  \param[in]     keyElementSourceOffset     This is the offset of the source key element indicating the start index
 *                                            of the copy operation.
 *  \param[in]     keyElementTargetOffset     This is the offset of the destination key element indicating the start index
 *                                            of the copy operation.
 *  \param[in]     keyElementCopyLength       Specifies the number of bytes that shall be copied.
 *  \param[in]     targetCryptoKeyId          Holds the identifier of the key whose key element shall be the destination
 *                                            element.
 *  \param[in]     targetKeyElementId         Holds the identifier of the key element which shall be the destination for
 *                                            the copy operation.
 *  \return        E_OK                       Request successful
 *                 E_NOT_OK                   Request failed
 *                 CRYPTO_E_BUSY              Request failed, Crypto Driver Object is busy
 *                 CRYPTO_E_KEY_READ_FAIL     Request failed because read access was denied
 *                 CRYPTO_E_KEY_WRITE_FAIL    Request failed because write access was denied
 *                 CRYPTO_E_KEY_NOT_AVAILABLE Request failed because the key is not available
 *                 CRYPTO_E_KEY_SIZE_MISMATCH Request failed, key element size does not match size of provided data.
 *                 CRYPTO_E_KEY_EMPTY         Request failed because of uniinitialzed source key element.
 *  \context       TASK
 *  \reentrant     TRUE, but not for the same cryIfKeyId
 *  \synchronous   TRUE
 *  \pre           cryptoFctIdxSrc and cryptoFctIdxDst must be valid indices of the crypto function table.
 *********************************************************************************************************************/
CRYIF_LOCAL_INLINE FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopyPartial_Internal(CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxSrc,
                                                                                         CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxDst,
                                                                                         uint32 cryptoKeyId,
                                                                                         uint32 keyElementId,
                                                                                         uint32 keyElementSourceOffset,
                                                                                         uint32 keyElementTargetOffset,
                                                                                         uint32 keyElementCopyLength,
                                                                                         uint32 targetCryptoKeyId,
                                                                                         uint32 targetKeyElementId);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CryIf_KeyCopyForDifferentDrivers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYIF_LOCAL Std_ReturnType CryIf_KeyCopyForDifferentDrivers(uint32 cryIfKeyId, uint32 targetCryIfKeyId)
{
  uint8 i, j;
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValSrc;
  Std_ReturnType retValDst;
  uint8 buffer[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
  uint32 bufferSize = CryIf_GetMaxSizeOfKeyElementOfGeneral();
  uint32 elementIdsSrc[CryIf_GetMaxNumberOfKeyElementsOfGeneral()];
  uint32 elementIdsSrcSize = CryIf_GetMaxNumberOfKeyElementsOfGeneral();
  uint32 elementIdsDst[CryIf_GetMaxNumberOfKeyElementsOfGeneral()];
  uint32 elementIdsDstSize = CryIf_GetMaxNumberOfKeyElementsOfGeneral();

  /* Get Key ids of both keys */
  retValSrc = CryIf_GetKeyElementIdsGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), elementIdsSrc, &elementIdsSrcSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
  retValDst = CryIf_GetKeyElementIdsGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))(CryIf_GetRefOfKey(targetCryIfKeyId), elementIdsDst, &elementIdsDstSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */

  if ((retValSrc == E_OK) && (retValDst == E_OK))
  {
    /* Iterate through both lists and copy compatible elements from source to destination */
    for (i = 0; i < elementIdsSrcSize; i++)
    {
      for (j = 0; j < elementIdsDstSize; j++)
      {
        if (elementIdsSrc[i] == elementIdsDst[j])
        {
          /* Copy compatible elements */
          if (E_OK == CryIf_GetKeyElementGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), elementIdsSrc[i], buffer, &bufferSize)) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
          {
            if (E_OK == CryIf_GetKeyElementSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))(CryIf_GetRefOfKey(targetCryIfKeyId), elementIdsDst[j], buffer, bufferSize)) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
            {
              retVal = E_OK;
            }
          }
          break;
        }
      }
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (CRYIF_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 *  CryIf_TranslateRedirectionKeys()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CRYIF_LOCAL FUNC(Std_ReturnType, CRYIF_CODE) CryIf_TranslateRedirectionKeys(P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) keyId)
{
  uint8 errorId = CRYIF_E_NO_ERROR;

# if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  if (*keyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
# endif /* (CRYIF_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Translate cryIfKeyId to CryptoKeyId */
    *keyId = CryIf_GetRefOfKey(*keyId); /* SBSW_CRYIF_API_CHECKED_ACCESS */
  }

  return errorId;
}

/**********************************************************************************************************************
 *  CryIf_UpdateRedirectionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYIF_LOCAL FUNC(Std_ReturnType, CRYIF_CODE) CryIf_UpdateRedirectionInfo(P2CONST(Crypto_JobType, AUTOMATIC, CRYIF_APPL_DATA) job)
{
  Std_ReturnType retVal = E_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* Check if redirection shall be applied */
  if(job->jobRedirectionInfoRef != NULL_PTR)
  {
    uint8 redirectionConfigValue = job->jobRedirectionInfoRef->redirectionConfig;

    /* Update key ids depending on redirection mask */
    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)
    {
      errorId = CryIf_TranslateRedirectionKeys(&job->jobRedirectionInfoRef->inputKeyId); /* SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)
    {
      errorId &= CryIf_TranslateRedirectionKeys(&job->jobRedirectionInfoRef->secondaryInputKeyId); /* SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)
    {
      errorId &= CryIf_TranslateRedirectionKeys(&job->jobRedirectionInfoRef->tertiaryInputKeyId); /* SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)
    {
      errorId &= CryIf_TranslateRedirectionKeys(&job->jobRedirectionInfoRef->outputKeyId); /* SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)
    {
      errorId &= CryIf_TranslateRedirectionKeys(&job->jobRedirectionInfoRef->secondaryOutputKeyId); /* SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB */
    }
  }

# if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    retVal = E_NOT_OK;

    /* ----- Development Error Report --------------------------------------- */
#  if (CRYIF_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError((uint16)CRYIF_MODULE_ID,
      CRYIF_INSTANCE_ID_DET,
      CRYIF_SID_PROCESS_JOB,
      errorId);
#  else
    CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
  }
# else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CRYIF_DEV_ERROR_DETECT == STD_ON) */

  return retVal;
}
#endif /* (CRYIF_REDIRECTION == STD_ON) */

/**********************************************************************************************************************
 * CryIf_Util_Max()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYIF_LOCAL_INLINE FUNC(uint32, CRYIF_CODE) CryIf_Util_Max(uint32 a, uint32 b)
{
  return (a > b)?(a):(b);
}

/**********************************************************************************************************************
 * CryIf_Util_MemSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYIF_LOCAL_INLINE FUNC(void, CRYIF_CODE) CryIf_Util_MemSet(P2VAR(uint8, AUTOMATIC, AUTOMATIC) dst, uint8 pattern, uint32 count)
{
  uint32_least i;

  for(i = 0; i < count; ++i)
  {
    dst[i] = pattern; /* SBSW_CRYIF_API_CHECKED_SIZE_ACCESS */
  }
}

/**********************************************************************************************************************
 * CryIf_Util_MemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYIF_LOCAL_INLINE FUNC(void, CRYIF_CODE) CryIf_Util_MemCpy(P2VAR(uint8, AUTOMATIC, AUTOMATIC) dst, P2CONST(uint8, AUTOMATIC, AUTOMATIC) src, uint32 count)
{
  uint32_least i;

  for(i = 0; i < count; ++i)
  {
    dst[i] = src[i]; /* SBSW_CRYIF_API_CHECKED_SIZE_ACCESS */
  }
}

/**********************************************************************************************************************
 * CryIf_KeyElementCopyPartial_Internal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
CRYIF_LOCAL_INLINE FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopyPartial_Internal(
      CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxSrc,
      CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxDst,
      uint32 cryptoKeyId,
      uint32 keyElementId,
      uint32 keyElementSourceOffset,
      uint32 keyElementTargetOffset,
      uint32 keyElementCopyLength,
      uint32 targetCryptoKeyId,
      uint32 targetKeyElementId)
{
  Std_ReturnType retVal;

  uint8 bufferSrc[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
  uint8 bufferDst[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
  uint32 bufferSizeSrc = CryIf_GetMaxSizeOfKeyElementOfGeneral();
  uint32 bufferSizeDst = CryIf_GetMaxSizeOfKeyElementOfGeneral();

  /* Check local buffer size vs copy length */
  if (((keyElementTargetOffset + keyElementCopyLength) <= CryIf_GetMaxSizeOfKeyElementOfGeneral()))
  {
    /* Get Source key element */
    retVal = CryIf_GetKeyElementGetOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyId, keyElementId, bufferSrc, &bufferSizeSrc); /* SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR */ /* SBSW_CRYIF_CSL_FPTR_CALL */

    /* Check if required data is inside the queried key element */
    if (retVal == E_OK)
    {
      if ((keyElementSourceOffset + keyElementCopyLength) <= bufferSizeSrc)
      {
        /* Get Destination key element */
        retVal = CryIf_GetKeyElementGetOfCryptoFunctions(cryptoFctIdxDst)(targetCryptoKeyId, targetKeyElementId, bufferDst, &bufferSizeDst); /* SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR */ /* SBSW_CRYIF_CSL_FPTR_CALL */
        switch (retVal)
        {
        case CRYPTO_E_KEY_EMPTY:
        {
          bufferSizeDst = 0u;
        }
        /* intentionally no break, since the processing of both cases is similar except of length assignment */

        case E_OK: /* PRQA S 2003 */ /* MD_CRYIF_NO_BREAK_IN_SWITCH */
        {
          /* Fill destination keyElement with zeroes if there are holes in the destination element */
          if (bufferSizeDst < keyElementTargetOffset)
          {
            CryIf_Util_MemClr(&bufferDst[bufferSizeDst], keyElementTargetOffset - bufferSizeDst); /* SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR */
          }

          /* Copy source window to destination window */
          CryIf_Util_MemCpy(&bufferDst[keyElementTargetOffset], &bufferSrc[keyElementSourceOffset], keyElementCopyLength); /* SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR */

          bufferSizeDst = CryIf_Util_Max(bufferSizeDst, keyElementTargetOffset + keyElementCopyLength);

          /* Set destination key element */
          retVal = CryIf_GetKeyElementSetOfCryptoFunctions(cryptoFctIdxDst)(targetCryptoKeyId, targetKeyElementId, bufferDst, bufferSizeDst); /* SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR */ /* SBSW_CRYIF_CSL_FPTR_CALL */
        }
          break;

        default:
          /* return retVal to caller */
          break;
        }

      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_CRYIF_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CryIf_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  CryIf_ModuleInitialized = CRYIF_UNINIT;
#endif
} /* CryIf_InitMemory() */

/**********************************************************************************************************************
 *  CryIf_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_INIT)
  {
    errorId = CRYIF_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
    CryIf_ModuleInitialized = (uint8)CRYIF_INIT;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_INIT, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* CryIf_Init() */

#if (CRYIF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CryIf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CRYIF_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (CRYIF_VENDOR_ID); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->moduleID = (CRYIF_MODULE_ID); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_major_version = (CRYIF_SW_MAJOR_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_minor_version = (CRYIF_SW_MINOR_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
    versioninfo->sw_patch_version = (CRYIF_SW_PATCH_VERSION); /* SBSW_CRYIF_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_GET_VERSION_INFO, errorId);
  }
# else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* CryIf_GetVersionInfo() */
#endif /* (CRYIF_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  CryIf_CallbackNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYIF_CODE) CryIf_CallbackNotification(P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job,
                                                  Std_ReturnType result)
{
  /* AUTOSAR 4.3.x standard deviation
   * According to ASR 4.3.x, this function receives job as P2CONST.
   * Also according to ASR 4.3.x, Csm_CallbackNotification expects job as a P2VAR.
   * This inconsistency in the standard was fixed in ASR 4.4.0 and later versions.
   * The MICROSAR CryIf does not offer support for the ASR 4.3.x CryIf_CallbackNotification interface,
   * as the standard is broken and a workaround (e.g., by creating a local clone of the job struct)
   * would involve heavy performance penalties.
   */

  Csm_CallbackNotification(job, result); /* SBSW_CRYIF_API_CALL_FORWARDING_PTR */
}

/**********************************************************************************************************************
 * CryIf_ProcessJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_ProcessJob(uint32 channelId,
                                                  P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'channelId' */
  else if (channelId >= CryIf_GetSizeOfChannel())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* Check if keyId is in range */
    if(job->jobPrimitiveInfo->cryIfKeyId < CryIf_GetSizeOfKey())
    {
      /* Check if service is an key primitive job */
      if (job->jobPrimitiveInfo->primitiveInfo->service < CRYIF_FIRST_KEY_PRIMITIVE)
      {
        job->cryptoKeyId = CryIf_GetRefOfKey( job->jobPrimitiveInfo->cryIfKeyId ); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */
      }
      else
      {
        job->cryptoKeyId = CryIf_GetRefOfKey( job->jobPrimitiveInputOutput.cryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */

        /* Check if service contains targetCryIfKeyId */
        if (job->jobPrimitiveInfo->primitiveInfo->service == (Crypto_ServiceInfoType)CRYPTO_KEYDERIVE)
        {
          job->targetCryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInputOutput.targetCryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */
        }
      }
#if (CRYIF_REDIRECTION == STD_ON)
# if (CRYIF_DEV_ERROR_DETECT)
      retVal = CryIf_UpdateRedirectionInfo(job); /* SBSW_CRYIF_FORWARDING_OF_JOB_PTR */
      if (retVal == E_OK)
# else
      (void)CryIf_UpdateRedirectionInfo(job); /* SBSW_CRYIF_FORWARDING_OF_JOB_PTR */
# endif /* (CRYIF_DEV_ERROR_DETECT) */
#endif /* (CRYIF_REDIRECTION == STD_ON) */
      {
        retVal = CryIf_GetProcessJobOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId))(CryIf_GetDriverObjectRefOfChannel(channelId), job); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      }
    }
    else
    {
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_PROCESS_JOB, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * CryIf_CancelJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CancelJob(uint32 channelId,
                                                 P2VAR(Crypto_JobType, AUTOMATIC, CRYIF_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'channelId' */
  else if (channelId >= CryIf_GetSizeOfChannel())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

        /* Check if keyId is in range */
    if (job->jobPrimitiveInfo->cryIfKeyId < CryIf_GetSizeOfKey())
    {
      /* Check if service is an key primitive job */
      if (job->jobPrimitiveInfo->primitiveInfo->service < CRYIF_FIRST_KEY_PRIMITIVE)
      {
        job->cryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInfo->cryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */
      }
      else
      {
        job->cryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInputOutput.cryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */

        /* Check if service contains targetCryIfKeyId */
        if (job->jobPrimitiveInfo->primitiveInfo->service == (Crypto_ServiceInfoType)CRYPTO_KEYDERIVE)
        {
          job->targetCryptoKeyId = CryIf_GetRefOfKey(job->jobPrimitiveInputOutput.targetCryIfKeyId); /* SBSW_CRYIF_WRITE_TO_JOB_POINTER */
        }
      }
#if (CRYIF_CANCELJOBWITHINFOTYPEOFCRYPTOFUNCTIONS == STD_ON)
      /* Check if canceljob service with jobinfotype parameter is enabled*/
      if (CryIf_GetCancelJobOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId)) == NULL_PTR)
      {
        /* CancelJob expects a non-const Crypto_JobInfoType, therefore a local non-const Crypto_JobInfoType must be created.
         * Since the call in synchronous, a pointer to a local struct is okay. */
        Crypto_JobInfoType jobInfo;
        jobInfo.jobId = job->jobInfo->jobId;
        jobInfo.jobPriority = job->jobInfo->jobPriority;

        retVal = CryIf_GetCancelJobWithInfoTypeOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId))(CryIf_GetDriverObjectRefOfChannel(channelId), &jobInfo); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      }
      else
#endif
      {
        retVal = CryIf_GetCancelJobOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfChannel(channelId))(CryIf_GetDriverObjectRefOfChannel(channelId), job); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      }
    }
    else
    {
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CANCEL_JOB, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * CryIf_KeyElementSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementSet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) keyPtr,
                                                     uint32 keyLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (keyPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'keyLength' */
  else if (keyLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyElementSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                     keyElementId,
                                                     keyPtr,
                                                     keyLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_SET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyElementSet() */

/**********************************************************************************************************************
 * CryIf_KeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetValid(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyValidSetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_SET_VALID, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyValidSet() */

/**********************************************************************************************************************
 * CryIf_KeyElementGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementGet(uint32 cryIfKeyId,
                                                     uint32 keyElementId,
                                                     P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) resultLengthPtr)
{

  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if ((resultPtr == NULL_PTR) || (resultLengthPtr == NULL_PTR))
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'resultLengthPtr' */
  else if (*resultLengthPtr == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyElementGetOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                   keyElementId,
                                                                                                   resultPtr,
                                                                                                   resultLengthPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_GET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyElementGet() */

/**********************************************************************************************************************
 * CryIf_KeyElementCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopy(uint32 cryIfKeyId,
                                                      uint32 keyElementId,
                                                      uint32 targetCryIfKeyId,
                                                      uint32 targetKeyElementId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxSrc = CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId);
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxDst = CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdSrc = CryIf_GetRefOfKey(cryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdDst = CryIf_GetRefOfKey(targetCryIfKeyId);

    /* ----- Implementation ----------------------------------------------- */
    if (cryptoFctIdxSrc == cryptoFctIdxDst)
    {
      /* Keys are in same crypto driver */
      retVal = CryIf_GetKeyElementCopyOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyIdSrc, keyElementId, cryptoKeyIdDst, targetKeyElementId); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      uint8 buffer[CryIf_GetMaxSizeOfKeyElementOfGeneral()];
      uint32 bufferSize = CryIf_GetMaxSizeOfKeyElementOfGeneral();
      /* Copy key element by getting element from source driver and set it to given key element in destination driver. */
      if (CryIf_GetKeyElementGetOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyIdSrc, keyElementId, buffer, &bufferSize) == E_OK) /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      {
        retVal = CryIf_GetKeyElementSetOfCryptoFunctions(cryptoFctIdxDst)(cryptoKeyIdDst, targetKeyElementId, buffer, bufferSize); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_COPY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyElementCopy() */

/**********************************************************************************************************************
 * CryIf_KeyElementCopyPartial_Stub()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopyPartial_Stub(uint32 cryIfKeyId,
                                                                  uint32 keyElementId,
                                                                  uint32 keyElementSourceOffset,
                                                                  uint32 keyElementTargetOffset,
                                                                  uint32 keyElementCopyLength,
                                                                  uint32 targetCryIfKeyId,
                                                                  uint32 targetKeyElementId)
{
  CRYIF_DUMMY_STATEMENT(cryIfKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(keyElementSourceOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(keyElementTargetOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(keyElementCopyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(targetCryIfKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYIF_DUMMY_STATEMENT(targetKeyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return E_NOT_OK;
} /* PRQA S 6060 */ /* MD_CRYIF_STPAR */

/**********************************************************************************************************************
 * CryIf_KeyElementCopyPartial()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopyPartial(uint32 cryIfKeyId,
                                                            uint32 keyElementId,
                                                            uint32 keyElementSourceOffset,
                                                            uint32 keyElementTargetOffset,
                                                            uint32 keyElementCopyLength,
                                                            uint32 targetCryIfKeyId,
                                                            uint32 targetKeyElementId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxSrc = CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId);
    CryIf_CryptoFunctionsIdxOfKeyType cryptoFctIdxDst = CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdSrc = CryIf_GetRefOfKey(cryIfKeyId);
    CryIf_RefOfKeyType cryptoKeyIdDst = CryIf_GetRefOfKey(targetCryIfKeyId);

    /* ----- Implementation ----------------------------------------------- */
    /* If KeyElementCopyPartial is supported by underlying driver and source- and target keys are in the same
       driver, call the KeyElementCopyPartial Service of the underyling key, otherwise use internal function */
    if((cryptoFctIdxSrc == cryptoFctIdxDst) && (CryIf_IsSupportsKeyElementCopyPartialOfCryptoFunctions(cryptoFctIdxDst)))
    {
      retVal = CryIf_GetKeyElementCopyPartialOfCryptoFunctions(cryptoFctIdxSrc)(cryptoKeyIdSrc, keyElementId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength, cryptoKeyIdDst, targetKeyElementId); /* SBSW_CRYIF_CSL_FPTR_CALL */
    }
    else
    {
      retVal = CryIf_KeyElementCopyPartial_Internal(cryptoFctIdxSrc, cryptoFctIdxDst, cryptoKeyIdSrc, keyElementId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength, cryptoKeyIdDst, targetKeyElementId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_ELEMENT_COPY_PARTIAL, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* PRQA S 6060 */ /* MD_CRYIF_STPAR */

/**********************************************************************************************************************
 * CryIf_KeyCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyCopy(uint32 cryIfKeyId,
                                               uint32 targetCryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if (CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId) == CryIf_GetCryptoFunctionsIdxOfKey(targetCryIfKeyId))
    {
      /* Keys are in same crypto driver */
      retVal = CryIf_GetKeyCopyOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), CryIf_GetRefOfKey(targetCryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
    }
    else
    {
      /* Keys are in different crypto drivers */
      retVal = CryIf_KeyCopyForDifferentDrivers(cryIfKeyId, targetCryIfKeyId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_COPY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyCopy() */

/**********************************************************************************************************************
 * CryIf_RandomSeed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_RandomSeed(uint32 cryIfKeyId,
                                                  P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) seedPtr,
                                                  uint32 seedLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (seedPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'seedLength' */
  else if (seedLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetRandomSeedOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                seedPtr,
                                                                                                seedLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_RANDOM_SEED, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_RandomSeed() */

/**********************************************************************************************************************
 * CryIf_KeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyGenerate(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyGenerateOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_GENERATE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyGenerate() */

/**********************************************************************************************************************
 * CryIf_KeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyDerive(uint32 cryIfKeyId,
                                                 uint32 targetCryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' and 'targetCryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (targetCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyDeriveOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), CryIf_GetRefOfKey(targetCryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_DERIVE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyDerive() */

/**********************************************************************************************************************
 * CryIf_KeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcPubVal(uint32 cryIfKeyId,
                                                             P2VAR(uint8, AUTOMATIC, CRYIF_APPL_VAR) publicValuePtr,
                                                             P2VAR(uint32, AUTOMATIC, CRYIF_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if ((publicValuePtr == NULL_PTR) || (publicValueLengthPtr == NULL_PTR))
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'publicValueLengthPtr' */
  else if (*publicValueLengthPtr == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyExchangeCalcPubValOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                           publicValuePtr,
                                                                                                           publicValueLengthPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_EXCHANGE_CALC_PUB_VAL, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 * CryIf_KeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyExchangeCalcSecret(uint32 cryIfKeyId,
                                                             P2CONST(uint8, AUTOMATIC, CRYIF_APPL_DATA) partnerPublicValuePtr,
                                                             uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check for value 'partnerPublicValueLength' */
  else if (partnerPublicValueLength == 0u)
  {
    errorId = CRYIF_E_PARAM_VALUE;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetKeyExchangeCalcSecretOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                           partnerPublicValuePtr,
                                                                                                           partnerPublicValueLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_KEY_EXCHANGE_CALC_SECRET, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_KeyExchangeCalcSecret() */

/**********************************************************************************************************************
 * CryIf_CertificateParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse(uint32 cryIfKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check parameter 'cryIfKeyId' */
  else if (cryIfKeyId >= CryIf_GetSizeOfKey())
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetCertificateParseOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId)); /* SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CERTIFICATE_PARSE, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_CertificateParse() */

/**********************************************************************************************************************
 * CryIf_CertificateVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify(uint32 cryIfKeyId,
                                                         uint32 verifyCryIfKeyId,
                                                         P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_VAR) verifyPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYIF_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CryIf_ModuleInitialized == (uint8)CRYIF_UNINIT)
  {
    errorId = CRYIF_E_UNINIT;
  }
  /* Check for null pointer */
  else if (verifyPtr == NULL_PTR)
  {
    errorId = CRYIF_E_PARAM_POINTER;
  }
  /* Check parameter 'cryIfKeyId' */
  else if ((cryIfKeyId >= CryIf_GetSizeOfKey())
    || (verifyCryIfKeyId >= CryIf_GetSizeOfKey()))
  {
    errorId = CRYIF_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = CryIf_GetCertificateVerifyOfCryptoFunctions(CryIf_GetCryptoFunctionsIdxOfKey(cryIfKeyId))(CryIf_GetRefOfKey(cryIfKeyId), /* SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR */
                                                                                                       CryIf_GetRefOfKey(verifyCryIfKeyId),
                                                                                                       verifyPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYIF_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYIF_MODULE_ID, CRYIF_INSTANCE_ID_DET, CRYIF_SID_CERTIFICATE_VERIFY, errorId);
  }
#else
  CRYIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* CryIf_CertificateVerify() */

#define CRYIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
  MD_CRYIF_STPAR:
    Reason:     Number of parameters is too high.
    Risk:       None
    Prevention: None

  MD_CRYIF_NO_BREAK_IN_SWITCH:
    Reason:     No break in a single case of a switch statement. Both cases only differ in one statement. Otherwise, the code must be duplicated or a function call must occur here.
    Risk:       If the code is refactored or adapted, this may lead to a problem if it is not considered.
    Prevention: Code Review.
 */

/* SBSW_JUSTIFICATION_BEGIN
  \ID SBSW_CRYIF_GETVERSIONINFO
    \DESCRIPTION The function CryIf_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_ID_AND_PTR
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with a pointer and an id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the CryIf and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the CryIf module.
                       The pointer is either forwarded and assumed to be valid or it is a local stack variable used for a synchronous call.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_ID_ONLY
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
                    The API is called with an Id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The Id is a SNV provided by the CryIf and is converted to the corresponding ID of lower layer using ComStackLib (CSL01).
                       It is checked when it was first passed to the CryIf module.

  \ID SBSW_CRYIF_API_CALL_FORWARDING_PTR
    \DESCRIPTION    The API call is forwarded and has as parameter one or more pointers.
    \COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

  \ID SBSW_CRYIF_WRITE_TO_JOB_POINTER
    \DESCRIPTION    The function does a write access on the job pointer.
    \COUNTERMEASURE \R The pointer is checked when it was first to the service.

  \ID SBSW_CRYIF_API_CHECKED_ACCESS
    \DESCRIPTION Write access to a pointer through a parameter. The API requires the pointer to be valid.
    \COUNTERMEASURE \R The caller has to assure that the API requirements are matched and the buffer is valid.

  \ID SBSW_CRYIF_API_CHECKED_SIZE_ACCESS
    \DESCRIPTION Write access to array given by pointer through a parameter. The API requires the array to have a certain size and it is assumed that the size is available in the given pointer.
    \COUNTERMEASURE \R The caller has to assure that the API requirements are matched and the size of the buffer is according to these requirements.

  \ID SBSW_CRYIF_FORWARDING_OF_REDIRECTION_POINTER_OF_JOB
    \DESCRIPTION A member of the redirection info of a job pointer is provided to the callee as an pointer.
    \COUNTERMEASURE \R The job pointer is assumed to be valid by API requirements, the redirection info pointer is checked locally.

  \ID SBSW_CRYIF_FORWARDING_OF_JOB_PTR
    \DESCRIPTION A job pointer is forwarded to the callee. The job pointer is passed to the caller directly.
    \COUNTERMEASURE \N The job pointer is assumed to be valid by API requirements.

  \ID SBSW_CRYIF_STACK_ARRAY_VARIABLE_AS_PTR
    \DESCRIPTION A pointer to an array located on the stack is given as parameter to a function including its size.
    \COUNTERMEASURE \N The function call is synchronous and the size of the array is given, therefore the stack array is valid throughout the function call.

  \ID SBSW_CRYIF_CSL_FPTR_CALL
    \DESCRIPTION    The API is called through a function pointer which is read using ComStackLib macros. The function pointer is selected from a ComStackLib table which is identified through an id.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).

SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

 \ID COV_CRYIF_ALWAYS_ON
    \ACCEPT TX
    \REASON CRYIF_LOCAL is always defined externally.

 COV_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  END OF FILE: CryIf.c
 *********************************************************************************************************************/

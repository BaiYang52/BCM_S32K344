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
/*      \file  Crypto_30_LibCv_Mac.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for MAC Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_MAC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_MacGenerate.h"
#include "Crypto_30_LibCv_MacVerify.h"

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
# include "actICMACAES.h"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL_CONSTANT MACROS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_MODULO_8(x)                                   ((x) & (8u - 1u))
#define CRYPTO_30_LIBCV_SIP_HASH_KEY_LENGTH                           (16u)
#define CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE                              (8u)
#define CRYPTO_30_LIBCV_SHA2_256_KEY_LENGTH                           (32u)
#define CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE                         (16u)

#define CRYPTO_30_LIBCV_CMAC_AES_MODE_128                             (0u)
#define CRYPTO_30_LIBCV_CMAC_AES_MODE_256                             (1u)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_CompareMac()
 *********************************************************************************************************************/
/*! \brief          Compares two MACs
 *  \details        This function compares two MAC values from MSB to LSB.
 *  \param[in]      mac1                  Holds a pointer to the first mac
 *  \param[in]      mac2                  Holds a pointer to the second mac
 *  \param[in]      lengthInBit           Number of bits which shall be compared from MSB to LSB.
 *  \return         CRYPTO_E_VER_NOT_OK   Macs are not similar
 *                  CRYPTO_E_VER_OK       Macs are similar
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Crypto_VerifyResultType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CompareMac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac1,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac2,
  uint32 lengthInBit);
#endif

#if ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_InitializeCmacAes()
 *********************************************************************************************************************/
/*! \brief          Handels the initialization of the CmacAesGenerate job.
 *  \details        This function initializes the provided workspace for the lower layer crypto library.
 *                  Based for the job operationmode start.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \param[in,out]  workspace          Pointer to the esl workspace.
 *  \param[in]      sheService         Holds the service for SHE key usage.
 *  \return         E_OK               Init was successful.
 *                  E_NOT_OK           Init failed.
 *  \pre            workspace has to be valid.
 *                  objectId has to be a valid driver object handle.
 *                  job has to be a valid job object handle.
 *                  sheService hast be be a valid SHE service.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitializeCmacAes(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  Crypto_30_LibCv_ServiceType sheService);
#endif /* ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)) */

#if (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA1ModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *                  job has to be a job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA1GenerateModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the tag value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA256ModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *                  job has to be a job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA256GenerateModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the tag value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA384ModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *                  job has to be a job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA384GenerateModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the tag value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA1VerifyModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers and checks if verification was successful.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA256VerifyModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers and checks if verification was successful.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacSHA384VerifyModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers and checks if verification was successful.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) || (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchGmacAesModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *                  job has to be a job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesModeStart(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchGmacAesModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesModeUpdate(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) || (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchGmacAesGenerateModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the tag value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesGenerateModeFinish(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchGmacAesVerifyModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers and checks if verification was successful.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesVerifyModeFinish(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace          Pointer to the esl workspace.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *                  job has to be a job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace          Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacRipeMd160GenerateModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the tag value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace          Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchHmacRipeMd160VerifyModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles operation mode FINISH.
 *  \details        This function writes the results in output buffers and checks if verification was successful.
 *  \param[in]      workSpace     Pointer to the esl workspace.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_CompareMac()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Crypto_VerifyResultType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CompareMac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac1,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) mac2,
  uint32 lengthInBit)
{
  Crypto_VerifyResultType result = CRYPTO_E_VER_OK;
  const uint32 numberOfFullBytesInMac = lengthInBit >> 3u;
  uint32 numberBitsInLastByte;
  uint8 verifyBitMask;
  
  uint32 i = 0u;

  /* # Compare full bytes of both MACs. */
  for (i = 0u; i < (numberOfFullBytesInMac); i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
  {
    if (mac1[i] != mac2[i])
    {
      /* Verification does not match */
      result = CRYPTO_E_VER_NOT_OK;
    }
  }

  /* # Compare remaining bits. */
  /* Compute number of bits to verify in the last byte */
  numberBitsInLastByte = CRYPTO_30_LIBCV_MODULO_8(lengthInBit);  /* Calculate modulo */
  if (numberBitsInLastByte != 0u)
  {
    /* Generate bitmask which contains all bits to be verified */
    verifyBitMask = (uint8)~((1u << (8u - numberBitsInLastByte)) - 1u);

    /* Verify the last masked byte */
    if ((mac1[numberOfFullBytesInMac] & verifyBitMask)
        != (mac2[numberOfFullBytesInMac] & verifyBitMask))
    {
      result = CRYPTO_E_VER_NOT_OK;
    }
  }

  return result;
}
#endif

#if ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_InitializeCmacAes()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitializeCmacAes(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  Crypto_30_LibCv_ServiceType sheService)
{
  uint32 cmacAesKeyLength;
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  Crypto_30_LibCv_SizeOfKeyStorageType cmacAesKeyIndex;

  eslt_ErrorCode retValCv;

  /* # Load key element from storage and initialize algorithm with keys. Dependent on roundkey feature, either the roundkey is loaded as key or the original key. */
# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE== STD_ON)
  Crypto_30_LibCv_SizeOfKeyElementsType macElementId;
  boolean reCalculate = FALSE;

  localRetVal = Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_MAC_KEY, &macElementId); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  /* Check if mac key has changed */
  if ((localRetVal != E_OK) || (!Crypto_30_LibCv_IsKeyElementStateByMask(macElementId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_MASK)))
  {
    reCalculate = TRUE;
  }
  /* Check if job is last job / workspace not changed */
  if (Crypto_30_LibCv_IsObjectWorkspaceUnchanged(objectId, job) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  {
    reCalculate = TRUE;
  }

  if (reCalculate == TRUE)
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif
  {
    /* Get key */
    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(job->cryptoKeyId, CRYPTO_KE_MAC_KEY, &cmacAesKeyIndex, &cmacAesKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE, sheService); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
    if (localRetVal == E_OK)
    {
      /* # Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->wsCMACAES.header), ESL_MAXSIZEOF_WS_CMACAES, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        if (cmacAesKeyLength == ESL_SIZEOF_AES128_KEY)
        {
          /* Store key in workspace */
          workspace->mode = CRYPTO_30_LIBCV_CMAC_AES_MODE_128; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
          retValCv = esl_initCMACAES128(
            &workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
            (eslt_Length)cmacAesKeyLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(cmacAesKeyIndex));
        }
        else
        {
          /* Store key in workspace */
          workspace->mode = CRYPTO_30_LIBCV_CMAC_AES_MODE_256; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
          retValCv = esl_initCMACAES256(
            &workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
            (eslt_Length)cmacAesKeyLength,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(cmacAesKeyIndex));
        }

# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
        /* # Set flag that roundkey is calculated */
        if (retValCv == ESL_ERC_NO_ERROR)
        {
          Crypto_30_LibCv_SetKeyElementStateByMask(macElementId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
        }
# endif
      }
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
      if (localRetVal == CRYPTO_E_KEY_NOT_VALID)
      {
        retVal = CRYPTO_E_KEY_NOT_VALID;
      }
    }
  }
# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
  else
  {
    retValCv = ESL_ERC_NO_ERROR;
  }
# endif

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) || (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)) */

#if (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_CmacAesGenerate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_CmacAesGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(Crypto_30_LibCv_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfCmacAesGenerate(Crypto_30_LibCv_GetCmacAesGenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceCMACAES)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_InitializeCmacAes(objectId, job, workspace, CRYPTO_30_LIBCV_SHE_SERVICE_MAC_GENERATE); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      if (workspace->mode == CRYPTO_30_LIBCV_CMAC_AES_MODE_128)
      {
      /* # Process plaintext data. */
        retValCv = esl_updateCMACAES128(&workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
      }
      else
      {
      /* # Process plaintext data. */
        retValCv = esl_updateCMACAES256(&workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
      }
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Byte macBuffer[CRYPTO_30_LIBCV_CMACAES_MAC_SIZE];
# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
      if (actCMACAESFinalize(
        (P2VAR(actCMACAESSTRUCT, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace->wsCMACAES.wsCMACAES, /* PRQA S 3305, 0310 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST */
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer, workspace->wsCMACAES.header.watchdog)
        == (actRETURNCODE)actOK) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
# else
      eslt_Length macBufferLength = CRYPTO_30_LIBCV_CMACAES_MAC_SIZE;
      if (workspace->mode == CRYPTO_30_LIBCV_CMAC_AES_MODE_128)
      {
        retValCv = esl_finalizeCMACAES128(&workspace->wsCMACAES, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      }
      else
      {
        retValCv = esl_finalizeCMACAES256(&workspace->wsCMACAES, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer, &macBufferLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
        if (macBufferLength != CRYPTO_30_LIBCV_CMACAES_MAC_SIZE)
        {
          retValCv = ESL_ERC_INPUT_INVALID;
        }
      }

      if (retValCv == ESL_ERC_NO_ERROR)
# endif
      {
        /* # Adapt output size. */
        if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > CRYPTO_30_LIBCV_CMACAES_MAC_SIZE)
        {
          *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = CRYPTO_30_LIBCV_CMACAES_MAC_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* # Write results in output buffers. */
        Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
        /* clear workspace buffer if actLib function is used */
        Crypto_30_LibCv_ClearData(((P2VAR(actCMACAESSTRUCT, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace->wsCMACAES.wsCMACAES)->buffer, actAES_BLOCK_SIZE); /* PRQA S 3305, 0310, 0315 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST, MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
# endif

        retValCv = ESL_ERC_NO_ERROR;
      }
      break;
    }

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_CMACAESGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_CmacAesVerify()
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
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_CmacAesVerify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(Crypto_30_LibCv_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfCmacAesVerify(Crypto_30_LibCv_GetCmacAesVerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceCMACAES)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_InitializeCmacAes(objectId, job, workspace, CRYPTO_30_LIBCV_SHE_SERVICE_MAC_VERIFY); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Process plaintext data */
      if (workspace->mode == CRYPTO_30_LIBCV_CMAC_AES_MODE_128)
      {
        retValCv = esl_updateCMACAES128(&workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
      }
      else
      {
        retValCv = esl_updateCMACAES256(&workspace->wsCMACAES, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
      }

      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {

      eslt_Byte macBuffer[CRYPTO_30_LIBCV_CMACAES_MAC_SIZE];
      uint32 macLength;
# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
      if (actCMACAESFinalize(
        (P2VAR(actCMACAESSTRUCT, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace->wsCMACAES.wsCMACAES, /* PRQA S 3305, 0310 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST */
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer, workspace->wsCMACAES.header.watchdog) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
        == (actRETURNCODE)actOK)
# else
      eslt_Length macBufferLength = CRYPTO_30_LIBCV_CMACAES_MAC_SIZE;
      if (workspace->mode == CRYPTO_30_LIBCV_CMAC_AES_MODE_128)
      {
        retValCv = esl_finalizeCMACAES128(&workspace->wsCMACAES, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      }
      else
      {
        retValCv = esl_finalizeCMACAES256(&workspace->wsCMACAES, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer, &macBufferLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
        if (macBufferLength != CRYPTO_30_LIBCV_CMACAES_MAC_SIZE)
        {
          retValCv = ESL_ERC_INPUT_INVALID;
        }
      }

      if (retValCv == ESL_ERC_NO_ERROR)
# endif
      {
        /* # Check if input length is not to large. */
        macLength = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength;
        /* size is given in bits */
        if (macLength <= (CRYPTO_30_LIBCV_CMACAES_MAC_SIZE << 3u))
        {

          /* # Check if verification was successful. */
          *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
            Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, macLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
          retValCv = ESL_ERC_NO_ERROR;
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
        }
# if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
        /* clear workspace buffer if actLib function is used */
        Crypto_30_LibCv_ClearData(((P2VAR(actCMACAESSTRUCT, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace->wsCMACAES.wsCMACAES)->buffer, actAES_BLOCK_SIZE); /* PRQA S 3305, 0310, 0315 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST, MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
# endif
      }
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_CMACAESVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_SIPHASHGENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_SipHashGenerate()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_SipHashGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceSipHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfSipHashGenerate(Crypto_30_LibCv_GetSipHashGenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceSipHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[1];

      Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SIPHASH, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        if (retVal == E_OK)
        {
          /* # Initialize algorithm. */
          retVal = E_NOT_OK;
          retValCv = esl_initSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                     (eslt_Length)keyElements[0u].keyElementLength,
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
        }
      }
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {

      /* # Process plaintext data */
      retValCv = esl_updateSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

      break;
    }
    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Byte macBuffer[CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE];

      retValCv = esl_finalizeSipHash(workspace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Adapt output size. */
        if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE)
        {
          *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }

        /* # Write results in output buffers. */
        Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      }

      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_SIPHASHGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_SipHashVerify()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_SipHashVerify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceSipHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfSipHashVerify(Crypto_30_LibCv_GetSipHashVerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceSipHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[1];

      Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_SIPHASH, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        if (retVal == E_OK)
        {
          /* # Initialize algorithm. */
          retVal = E_NOT_OK;
          retValCv = esl_initSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                     (eslt_Length)keyElements[0u].keyElementLength,
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
        }
      }
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Process plaintext data */
      retValCv = esl_updateSipHash(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Byte macBuffer[CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE];
      uint32 macLength;
      retValCv = esl_finalizeSipHash(workspace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Check if input length is not to large. */
        macLength = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength;
        if (macLength <= (CRYPTO_30_LIBCV_SIPHASH_MAC_SIZE << 3u))
        {

          /* # Check if verification was successful. */
          *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
            Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, macLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
        }
      }
      break;
    }

    default:   /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_SIPHASHVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA1ModeStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* # Initialize workspace header. */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (esl_initWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_HMACSHA1, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initHashMACSHA1(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                     (eslt_Length)keyElements[0u].keyElementLength,
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  eslt_ErrorCode retValCv;

  /* Update HMAC Calculation */
  /* # Process input data. */
  retValCv = esl_updateHashMACSHA1(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

  return retValCv;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA1GenerateModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 macBuffer[ESL_SIZEOF_SHA1_DIGEST];

  if (esl_finalizeHashMACSHA1(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  {
    /* # Adapt output size. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > ESL_SIZEOF_SHA1_DIGEST)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = ESL_SIZEOF_SHA1_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Write results in output buffers. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    /* Set return value */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha1Generate()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha1Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha1Generate(Crypto_30_LibCv_GetHmacSha1GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA1)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA1ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA1GenerateModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA1GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA256ModeStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* # Initialize workspace header. */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (esl_initWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_HMACSHA256, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initHashMACSHA256(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                       (eslt_Length)keyElements[0u].keyElementLength,
                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  eslt_ErrorCode retValCv;

  /* Update HMAC Calculation */
  /* # Process input data. */
  retValCv = esl_updateHashMACSHA256(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                     (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

  return retValCv;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA256GenerateModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 macBuffer[ESL_SIZEOF_SHA256_DIGEST];

  if (esl_finalizeHashMACSHA256(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  {
    /* # Adapt output size. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > ESL_SIZEOF_SHA256_DIGEST)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = ESL_SIZEOF_SHA256_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Write results in output buffers. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    /* Set return value */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha256Generate()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha256Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha256Generate(Crypto_30_LibCv_GetHmacSha256GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA256)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA256ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA256GenerateModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA256GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA384ModeStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384ModeStart(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* # Initialize workspace header. */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (esl_initWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_HMACSHA384, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initHashMACSHA384(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                       (eslt_Length)keyElements[0u].keyElementLength,
                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  eslt_ErrorCode retValCv;

  /* Update HMAC Calculation */
  /* # Process input data. */
  retValCv = esl_updateHashMACSHA384(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                     (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                     (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

  return retValCv;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA384GenerateModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 macBuffer[ESL_SIZEOF_SHA384_DIGEST];

  if (esl_finalizeHashMACSHA384(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  {
    /* # Adapt output size. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > ESL_SIZEOF_SHA384_DIGEST)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = ESL_SIZEOF_SHA384_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Write results in output buffers. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    /* Set return value */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha384Generate()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha384Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha384Generate(Crypto_30_LibCv_GetHmacSha384GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA384)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA384ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA384GenerateModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA384GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA1VerifyModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA1VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  eslt_ErrorCode retValCv;
  uint8 macBuffer[ESL_SIZEOF_SHA1_DIGEST];

  retValCv = esl_finalizeHashMACSHA1(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Check if input length is not to large. */
    /* size is given in bits */
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength <= Crypto_30_LibCv_Byte2Bit(ESL_SIZEOF_SHA1_DIGEST))
    {
      /* # Check if verification was successful. */
      *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha1Verify()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha1Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA1, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha1Verify(Crypto_30_LibCv_GetHmacSha1VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA1)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA1ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA1ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA1VerifyModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA1VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA256VerifyModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA256VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  eslt_ErrorCode retValCv;
  uint8 macBuffer[ESL_SIZEOF_SHA256_DIGEST];

  retValCv = esl_finalizeHashMACSHA256(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Check if input length is not to large. */
    /* size is given in bits */
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength <= Crypto_30_LibCv_Byte2Bit(ESL_SIZEOF_SHA256_DIGEST))
    {
      /* # Check if verification was successful. */
      *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha256Verify()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha256Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA256, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha256Verify(Crypto_30_LibCv_GetHmacSha256VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA256)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA256ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA256ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA256VerifyModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA256VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacSHA384VerifyModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacSHA384VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  eslt_ErrorCode retValCv;
  uint8 macBuffer[ESL_SIZEOF_SHA384_DIGEST];

  retValCv = esl_finalizeHashMACSHA384(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Check if input length is not to large. */
    /* size is given in bits */
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength <= Crypto_30_LibCv_Byte2Bit(ESL_SIZEOF_SHA384_DIGEST))
    {
      /* # Check if verification was successful. */
      *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacSha384Verify()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacSha384Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACSHA384, AUTOMATIC, CRYPTO_30_LIBCV_VAR_NOINIT) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacSha384Verify(Crypto_30_LibCv_GetHmacSha384VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACSHA384)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacSHA384ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA384ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacSHA384VerifyModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACSHA384VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) || (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchGmacAesModeStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesModeStart(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* # Initialize workspace header. */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CIPHER_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CIPHER_IV); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (esl_initWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_GMAC, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initGMAC(workSpace,
                              (eslt_Length)keyElements[0u].keyElementLength,
                              (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                              (eslt_Length)keyElements[1u].keyElementLength, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                              (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex));
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchGmacAesModeUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesModeUpdate(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  eslt_ErrorCode retValCv;

  /* Update GMAC Calculation */
  /* # Process input data. */
  retValCv = esl_updateGMAC(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                            (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

  return retValCv;
}
#endif /* (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) || (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchGmacAesGenerateModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesGenerateModeFinish(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* buffer holding Auth Tag */
  eslt_Byte authTagTempBuffer[CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE] = { 0u };

  /* # Write results in output buffer. */
  if (esl_finalizeGMAC(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&authTagTempBuffer[0]) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      == ESL_ERC_NO_ERROR)
  {
    /* # Adapt output size of auth tag. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }
    /* # Copy auth tag to output buffer. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, authTagTempBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    retValCv = ESL_ERC_NO_ERROR;
  }

  return retValCv;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_GmacAesGenerate()
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
 */
/* PRQA S 6030 4 */ /* MD_MSR_STCYC */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_GmacAesGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfGmacAesGenerate(Crypto_30_LibCv_GetGmacAesGenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceGMAC)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchGmacAesModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchGmacAesModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchGmacAesGenerateModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_GMACAESGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchGmacAesVerifyModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchGmacAesVerifyModeFinish(
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* buffer holding Auth Tag */
  eslt_Byte authTagTempBuffer[CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE] = { 0u };
  uint32 authTagTempBufferLength;

  /* # Write results in output buffer. */
  if (esl_finalizeGMAC(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&authTagTempBuffer[0]) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
      == ESL_ERC_NO_ERROR)
  {
    /* # Check if input length is not to large. */
    authTagTempBufferLength = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength;
    /* size is given in bits */
    if (authTagTempBufferLength <= (CRYPTO_30_LIBCV_GMAC_MAX_OUT_TAG_SIZE << 3u))
    {

      /* # Check if verification was successful. */
      *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, authTagTempBuffer, authTagTempBufferLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      retValCv = ESL_ERC_NO_ERROR;
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }
  }

  return retValCv;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_GmacAesVerify()
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
 */
/* PRQA S 6030 4 */ /* MD_MSR_STCYC */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_GmacAesVerify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceGMAC, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfGmacAesVerify(Crypto_30_LibCv_GetGmacAesVerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceGMAC)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchGmacAesModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchGmacAesModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchGmacAesVerifyModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_GMACAESVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* # Initialize workspace header. */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_MAC_KEY); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (esl_initWorkSpaceHeader(&(workSpace->header), ESL_MAXSIZEOF_WS_HMACRIPEMD160, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      /* # Initialize algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initHashMACRIPEMD160(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                         (eslt_Length)keyElements[0u].keyElementLength,
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex));
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  eslt_ErrorCode retValCv;

  /* Update HMAC Calculation */
  /* # Process input data. */
  retValCv = esl_updateHashMACRIPEMD160(workSpace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                       (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);

  return retValCv;
}
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) || (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacRipeMd160GenerateModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160GenerateModeFinish(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
{
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint8 macBuffer[ESL_SIZEOF_SHA1_DIGEST];

  if (esl_finalizeHashMACRIPEMD160(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  {
    /* # Adapt output size. */
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > ESL_SIZEOF_RIPEMD160_DIGEST)
    {
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = ESL_SIZEOF_RIPEMD160_DIGEST; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Write results in output buffers. */
    Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, macBuffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

    /* Set return value */
    retValCv = ESL_ERC_NO_ERROR;
  }
  else
  {
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = 0u; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacRipeMd160Generate()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacRipeMd160Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacRipeMd160Generate(Crypto_30_LibCv_GetHmacRipeMd160GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACRIPEMD160)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacRipeMd160GenerateModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchHmacRipeMd160VerifyModeFinish()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchHmacRipeMd160VerifyModeFinish(
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  eslt_ErrorCode retValCv;
  uint8 macBuffer[ESL_SIZEOF_RIPEMD160_DIGEST];

  retValCv = esl_finalizeHashMACRIPEMD160(workSpace, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) macBuffer); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Check if input length is not to large. */
    /* size is given in bits */
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength <= Crypto_30_LibCv_Byte2Bit(ESL_SIZEOF_RIPEMD160_DIGEST))
    {
      /* # Check if verification was successful. */
      *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr) = /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CompareMac(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, macBuffer, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_HmacRipeMd160Verify()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_HmacRipeMd160Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceHMACRIPEMD160, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfHmacRipeMd160Verify(Crypto_30_LibCv_GetHmacRipeMd160VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceHMACRIPEMD160)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchHmacRipeMd160ModeStart(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacRipeMd160ModeUpdate(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retValCv = Crypto_30_LibCv_DispatchHmacRipeMd160VerifyModeFinish(workspace, job);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      break;
    }

    default: /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_HMACRIPEMD160VERIFY == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Mac.c
 *********************************************************************************************************************/

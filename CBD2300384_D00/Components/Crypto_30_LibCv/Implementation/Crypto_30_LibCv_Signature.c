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
/*      \file  Crypto_30_LibCv_Signature.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for SIGNATURE Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_SIGNATURE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_Curve_Int.h"
#include "Crypto_30_LibCv_SignatureGenerate.h"
#include "Crypto_30_LibCv_SignatureVerify.h"
#include "Crypto_30_LibCv_Hash.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY                          32u

/* RSA */
#define CRYPTO_30_LIBCV_RSA_SALT_LENGTH_SIZE                          (2u)
#define CRYPTO_30_LIBCV_SIGNATURE_RSA_PSS_SALT_SIZE_AUTO_CALC         (-1)

/* Pre-Hash */
#define CRYPTO_30_LIBCV_PREHASH_DIGEST_MAXSIZE                        ESL_SIZEOF_SHA512_DIGEST

#define CRYPTO_E_PARAM_HANDLE_CUSTOM                                  (0x0Fu)

#define Crypto_30_LibCv_GetUint16(hiByte, loByte)                     ((uint16)((((uint16)(hiByte)) << 8) | ((uint16)(loByte))))

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON))
# if (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashStart()
 *********************************************************************************************************************/
/*!
 * \brief           pre-hashing start mode.
 * \details         This function dispatches and executes the pre-hashing for the required job.
 * \param[in,out]   wsHash        Pointer to the vSecPrim hash workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  wsHash has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashStart(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashUpdate()
 *********************************************************************************************************************/
/*!
 * \brief           pre-hashing update mode.
 * \details         This function dispatches and executes the pre-hashing for the required job.
 * \param[in,out]   wsHash        Pointer to the vSecPrim hash workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  wsHash has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashUpdate(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashFinish()
 *********************************************************************************************************************/
/*!
 * \brief           pre-hashing finish mode.
 * \details         This function dispatches and executes the pre-hashing for the required job. If no pre-hashing
 *                  is required the input data is copied to the hashPtr.
 * \param[in,out]   wsHash        Pointer to the vSecPrim hash workspace.
 * \param[out]      hashPtr       Pointer to the hash.
 * \param[in,out]   hashLengthPtr Pointer to the hash length.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  wsHash has to be valid.
 *                  hashPtr has to be valid pointer with the length of the hashLengthPtr value.
 *                  hashLengthPtr has to be valid pointer.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashFinish(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) hashPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) hashLengthPtr);
# endif /* (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON) */
#endif /* ((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) */

#if (CRYPTO_30_LIBCV_ECPGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job in start mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace     Pointer to the esl workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job in update mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace     Pointer to the esl workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature_SignData()
 *********************************************************************************************************************/
/*!
 * \brief           Calls the lower layer to compute the signature based on the provided domain parameters.
 * \details         This function retrieves the private key and passes the domain parameters, the key
 *                  and the message to the crypto library to compute the ECDSA signature.
 * \param[in,out]   job                      Pointer to the job which is dispatched.
 * \param[in,out]   workSpace                Pointer to the esl workspace.
 * \param[in]       EcDomainPtr              Pointer to the curve domain parameters
 * \param[in]       EcDomainExtPtr           Pointer to the curve domain extension parameters
 * \param[in]       EcSpeedUpExtPtr          Pointer to the curve speed up extension parameters
 * \param[in]       expectedKeyLength        Expected length of the private key for signature generation
 * \param[in]       messagePtr               Pointer to the plain message.
 * \param[in]       messageLength            Length of the plain message.
 * \return          E_OK                     Signature computation was successful.
 *                  E_NOT_OK                 Signature computation failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 *                  messagePtr has to be valid ptr with the length messageLength.
 *                  All other provided pointers have to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature_SignData(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcSpeedUpExtPtr,
  uint32 expectedKeyLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature_ConvertSignatureForOutputBuffer()
 *********************************************************************************************************************/
/*!
 * \brief           Processes the signature output of the lower layer.
 * \details         This function processes the signature output of the crypto library and writes the signature parts
 *                  to the job's output parameter.
 * \param[in,out]   job                      Pointer to the job which is dispatched.
 * \param[in]       sigLength                Maximum length of one signature component.
 * \param[in]       doubleSigLength          Twice the maximum length of one signature component.
 * \param[in]       sigLengthR               Length of signature component r.
 * \param[in]       sigLengthS               Length of signature component s.
 * \param[in]       expectedKeyLength        Expected length of the private key for signature generation
 * \param[in]       signatureR               Pointer to the buffer storing the r component of the signature.
 * \param[in]       signatureS               Pointer to the buffer storing the s component of the signature.
 * \return          E_OK                     Signature post-processing was successful.
 *                  E_NOT_OK                 Signature post-processing failed.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 *                  messagePtr has to be valid ptr with the length messageLength.
 *                  All other provided pointers have to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature_ConvertSignatureForOutputBuffer(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  eslt_Length sigLength,
  eslt_Length doubleSigLength,
  eslt_Length sigLengthR,
  eslt_Length sigLengthS,
  uint32 expectedKeyLength,
  P2CONST(eslt_Byte, AUTOMATIC, AUTOMATIC) signatureR,
  P2CONST(eslt_Byte, AUTOMATIC, AUTOMATIC) signatureS);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job in finish mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace                    Pointer to the esl workspace.
 * \param[in,out]   job                          Pointer to the job which shall be dispatched.
 * \param[in]       messagePtr                   Pointer to the plain message.
 * \param[in]       messageLength                Length of the plain message.
 * \return          E_OK                         Dispatching was successful.
 *                  E_NOT_OK                     Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 *                  messagePtr has to be valid ptr with the length messageLength.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job in finish mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace                   Pointer to the esl workspace.
 * \param[in,out]   job                         Pointer to the job which shall be dispatched.
 * \return          E_OK                        Dispatching was successful.
 *                  E_NOT_OK                    Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job in start mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace     Pointer to the esl workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             workSpace has to be valid.
 *                  job has to be a job object handle.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job in update mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace     Pointer to the esl workspace.
 * \param[in]       job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish_CvOperation()
 *********************************************************************************************************************/
/*!
 * \brief           Calls the lower layer to verify the signature based on the provided domain parameters.
 * \details         This function retrieves the public key for signature verification and passes the
 *                  domain parameters and the message to the crypto library to verify the ECDSA signature.
 * \param[in,out]   job                      Pointer to the job which is dispatched.
 * \param[in,out]   workSpace                Pointer to the esl workspace.
 * \param[in]       EcDomainPtr              Pointer to the curve domain parameters
 * \param[in]       EcDomainExtPtr           Pointer to the curve domain extension parameters
 * \param[in]       messagePtr               Pointer to the plain message.
 * \param[in]       messageLength            Length of the plain message.
 * \return          E_OK                     Signature verification was successful.
 *                  E_NOT_OK                 Signature verification failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 *                  messagePtr has to be valid ptr with the length messageLength.
 *                  All other provided pointers have to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish_CvOperation(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job in finish mode to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library.
 * \param[in,out]   workSpace     Pointer to the esl workspace.
 * \param[in,out]   job           Pointer to the job which shall be dispatched.
 * \return          E_OK          Dispatching was successful.
 *                  E_NOT_OK      Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             job has to be a job object handle.
 *                  workSpace has to be valid.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON) */

#if ((CRYPTO_30_LIBCV_RSAVERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureVerifyResult()
 *********************************************************************************************************************/
/*!
 * \brief          Dispatch the return value of the verification function.
 * \details        This functions handles the return value of the verification function and set the result to the corresponding result ptr.
 * \param[in,out]  job                    Pointer to the job which shall be dispatched.
 * \param[in]      retValCv               Return value of the verification function.
 * \param[in]      signatureInvalidValue  Expected value for a invalid signature.
 * \return         ESL_ERC_ERROR          Dispatching was successful and verification failed.
 *                 ESL_ERC_NO_ERROR       Dispatching was successful and verification successful.
 * \pre            job has to be a valid job object.
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureVerifyResult(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  eslt_ErrorCode retValCv,
  eslt_ErrorCode signatureInvalidValue);
#endif /* ((CRYPTO_30_LIBCV_RSAVERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) */

#if (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_SignatureRsaPssVerifyGetKeyElements()
 *********************************************************************************************************************/
/*!
 * \brief          Load all required key elements and handle return values for RSA Pss verify.
 * \details        This function load the required key elements and handles the return values.
 * \param[in]      cryptoKeyId          Id of the crypto key.
 * \param[out]     saltLengthPtr        Pointer to where the salt length should be stored.
 * \param[out]     SigExponentIndexPtr  Holds the index to the key data which shall be set as key element.
 * \param[in,out]  SigExponentLength    Contains the length of the key element in bytes.
 * \param[out]     SigModulusIndexPtr   Holds the index to the key data which shall be set as key element.
 * \param[in,out]  SigModulusLength     Contains the length of the key element in bytes.
 * \return         E_OK                 Read key elements success.
 *                 E_NOT_OK             Read key elements failed.
 * \pre            objectId has to be a valid driver object handle.
 *                 SigExponentIndexPtr has to be a valid ptr.
 *                 sigExponentLength has to be a valid ptr.
 *                 SigModulusIndexPtr has to be a valid ptr.
 *                 sigModulusLength has to be a valid ptr.
 * \context        TASK
 * \reentrant      TRUE
 * \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SignatureRsaPssVerifyGetKeyElements(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) saltLengthPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) SigExponentIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) SigExponentLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) SigModulusIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) SigModulusLength);
#endif /* (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateStart()
 *********************************************************************************************************************/
/*! \brief          Handle Operation mode start for RSA CRT PKCS1 signature generate.
 *  \details        This function dispatches the provided job to the lower layer crypto library.
 *  \param[in,out]  workspace           Pointer to the esl workspace.
 *  \param[in]      job                 Pointer to the job which shall be dispatched.
 *  \return         E_OK                Dispatching was successful.
 *                  E_NOT_OK            Dispatching failed.
 *  \pre            workspace has to be a valid eslt_WorkSpaceRSACRTsig handle.
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateStart(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateUpdate()
 *********************************************************************************************************************/
/*! \brief          Handle Operation mode start for RSA CRT PKCS1 signature generate.
 *  \details        This function dispatches the provided job to the lower layer crypto library.
 *  \param[in,out]  workspace           Pointer to the esl workspace.
 *  \param[in]      job                 Pointer to the job which shall be dispatched.
 *  \return         E_OK                Dispatching was successful.
 *                  E_NOT_OK            Dispatching failed.
 *  \pre            workspace has to be a valid eslt_WorkSpaceRSACRTsig handle.
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateUpdate(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateFinish()
 *********************************************************************************************************************/
/*! \brief          Handle Operation mode start for RSA CRT PKCS1 signature generate.
 *  \details        This function dispatches the provided job to the lower layer crypto library.
 *  \param[in,out]  workspace           Pointer to the esl workspace.
 *  \param[out]     job                 Pointer to the job which shall be dispatched.
 *  \return         E_OK                Dispatching was successful.
 *                  E_NOT_OK            Dispatching failed.
 *  \pre            workspace has to be a valid eslt_WorkSpaceRSACRTsig handle.
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateFinish(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_RSACRTGENERATE == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ((CRYPTO_30_LIBCV_RSAVERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureVerifyResult()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureVerifyResult(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
  eslt_ErrorCode retValCv,
  eslt_ErrorCode signatureInvalidValue)
{
  eslt_ErrorCode retVal = ESL_ERC_ERROR;
  /* # Handle Signature Verify Return Value. */
  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* # Verification Passed */
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    retVal = ESL_ERC_NO_ERROR;
  }
  else
  {
    /* # Verification Failed */
    if ((retValCv == signatureInvalidValue) || (retValCv == ESL_ERC_SIGNATURE_INVALID))
    {
      retVal = ESL_ERC_NO_ERROR;
    }
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_RSAVERIFY == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) */

#if (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_SignatureRsaPssVerifyGetKeyElements()
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
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SignatureRsaPssVerifyGetKeyElements(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) saltLengthPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) SigExponentIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) SigExponentLength,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) SigModulusIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) SigModulusLength)
{
  Std_ReturnType retVal;
  Std_ReturnType retValRequired, reValOptional;

  Crypto_30_LibCv_KeyElementGetType keyElements[2];

  Crypto_30_LibCv_SizeOfKeyStorageType SignatureSaltLengthIndex;
  uint32 SignatureSaltLengthSize = CRYPTO_30_LIBCV_RSA_SALT_LENGTH_SIZE;

  /* Check if key element is accessible */
  /* # Read required key elements */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  retValRequired = Crypto_30_LibCv_Local_GetElementsIndexJob(cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  reValOptional = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(cryptoKeyId, CRYPTO_KE_CUSTOM_RSA_SALT_LENGTH, &SignatureSaltLengthIndex, &SignatureSaltLengthSize, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # check results */
  if (retValRequired == E_OK)
  {
    *SigExponentIndexPtr = keyElements[0u].keyElementIndex; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    *SigExponentLength = keyElements[0u].keyElementLength; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    *SigModulusIndexPtr = keyElements[1u].keyElementIndex; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    *SigModulusLength = keyElements[1u].keyElementLength; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    /* # check if the salt length key element is available. */
    if (reValOptional == E_OK)
    {
      *saltLengthPtr = (uint32)Crypto_30_LibCv_GetUint16(Crypto_30_LibCv_GetKeyStorage(SignatureSaltLengthIndex), Crypto_30_LibCv_GetKeyStorage(SignatureSaltLengthIndex + 1u)); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      retVal = E_OK;
    }
    else if (reValOptional == CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      /* # if the salt length key element is not available use specific value to auto calculate salt length. */
      *saltLengthPtr = (uint32)(eslt_Length)CRYPTO_30_LIBCV_SIGNATURE_RSA_PSS_SALT_SIZE_AUTO_CALC; /* PRQA S 2890, 2895 */ /* MD_CRYPTO_30_LIBCV_3.1, MD_CRYPTO_30_LIBCV_3.1 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      retVal = E_OK;
    }
    else
    {
      /* invalid salt length */
      retVal = reValOptional;
    }
  }
  else
  {
    retVal = retValRequired;
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON) */

#if ((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON))
# if (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashStart()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashStart(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* Dispatch secondary algorithm for pre-hashing. */
  if(job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_NOT_SET)
  {
    /* Without pre-hashing */
    retVal = E_OK;
  }
  else
  {
    /* With pre-hashing */
    retVal = Crypto_30_LibCv_Dispatch_Hash_ModeStart(wsHash, job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashUpdate()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashUpdate(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* Dispatch secondary algorithm for pre-hashing. */
  if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_NOT_SET)
  {
    /* without pre hashing */
    if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength != 0u) && (!Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH)))
    {
      retVal = E_NOT_OK;
    }
    else
    {
      retVal = E_OK;
    }
  }
  else
  {
    /* With pre hashing */
    retVal = Crypto_30_LibCv_Dispatch_Hash_ModeUpdate(wsHash, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_PreHashFinish()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_PreHashFinish(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsHash,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) hashPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) hashLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* Dispatch secondary algorithm for pre-hashing. */
  retVal = Crypto_30_LibCv_Dispatch_Hash_ModeFinish(wsHash, hashLengthPtr, hashPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON) */
#endif /* ((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) */

#if (CRYPTO_30_LIBCV_ECPGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */

# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON)
  /* # Handle pre-hashing start. */
  retVal = Crypto_30_LibCv_PreHashStart(&(workSpace->wsPreHash), job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(workSpace); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON)
  /* # Handle pre-hashing update. */
  retVal = Crypto_30_LibCv_PreHashUpdate(&(workSpace->wsPreHash), job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(workSpace); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature_SignData()
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
 *
 *
 */
/* PRQA S 6050, 6060, 6080 10 */ /* MD_MSR_STCAL, MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature_SignData(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcSpeedUpExtPtr,
  uint32 expectedKeyLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  eslt_Length sigLength = 0u, doubleSigLength = 0u, sigLengthR = 0u, sigLengthS = 0u;
  uint32 sigKeyLength = 0u;

  eslt_Byte signatureR[CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY] = {0u};
  eslt_Byte signatureS[CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY] = {0u};

  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) privateKeyElementPtr;
# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
  eslt_Byte privateKeyElementBuffer[CRYPTO_30_LIBCV_MAX_SIZEOF_SIGNATURE_GENERATE_ECC_KEY] = { 0u };
# endif

  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  /* ----- Implementation ------------------------------------------------- */
  /* # Get private key for signature generation. */
  sigKeyLength = (uint32)(esl_getLengthOfEcPprivateKey(EcDomainPtr));  /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_SIGNATURE_KEY, sigKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    /* # Initialize ECC workspace header. */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(workSpace->wsEcP.header), ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* # Check if key element for private key is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_MAX); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (keyElements[0].keyElementLength < expectedKeyLength)
    {
      retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
    }

    if (retVal == E_OK)
    {
      retVal = E_NOT_OK;
      if (esl_initSignDSAEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(workSpace->wsEcP), EcDomainPtr, EcDomainExtPtr, EcSpeedUpExtPtr) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
      {
# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
        if (sigKeyLength == (keyElements[0].keyElementLength + 1u))
        {
          privateKeyElementBuffer[0] = 0u; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          Crypto_30_LibCv_CopyData(&privateKeyElementBuffer[1u], Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex), (keyElements[0].keyElementLength)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
          privateKeyElementPtr = privateKeyElementBuffer;
        }
        else
# endif
        {
          privateKeyElementPtr = Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex);
        }

        /* # Calculate signature. */
        /* # get esl size of signature length for r and s (signature = [signature_r|signature_s]) */
        sigLength = esl_getLengthOfEcPsignature_comp(EcDomainPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
        doubleSigLength = (eslt_Length)Crypto_30_LibCv_Math_Mul2(sigLength);
        if (*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) >= doubleSigLength)
        {
          sigLengthS = sigLength;
          sigLengthR = sigLength;

          /* # Finalize and store output. */
          retValCv = esl_signDSAEcP_prim(&(workSpace->wsEcP), /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            (eslt_Length)messageLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))messagePtr,
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))privateKeyElementPtr,
            (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&sigLengthR, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))signatureR,
            (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&sigLengthS, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))signatureS);

          if (retValCv == ESL_ERC_NO_ERROR)
          {
            retVal = Crypto_30_LibCv_EccPrimeGenerateSignature_ConvertSignatureForOutputBuffer(job, sigLength, doubleSigLength, sigLengthR, sigLengthS, expectedKeyLength, signatureR, signatureS); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
          }
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature_ConvertSignatureForOutputBuffer()
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
/* PRQA S 6060 10 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature_ConvertSignatureForOutputBuffer(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_3 */
  eslt_Length sigLength,
  eslt_Length doubleSigLength,
  eslt_Length sigLengthR,
  eslt_Length sigLengthS,
  uint32 expectedKeyLength,
  P2CONST(eslt_Byte, AUTOMATIC, AUTOMATIC) signatureR,
  P2CONST(eslt_Byte, AUTOMATIC, AUTOMATIC) signatureS)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
  boolean outputIsSet = FALSE; /* PRQA S 2981 */ /* MD_MSR_RetVal */
# endif

  /* ----- Implementation ------------------------------------------------- */
  if ((sigLengthR == sigLength) && (sigLengthS == sigLength))
  {
# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
    /* # Check if the return length is one greater than expected, the SECp160r1 can return 161 bits. */
    if (sigLength == (expectedKeyLength + 1u))
    {
      /* # Check the first bytes and delete the zero padding. */
      if ((signatureR[0] == 0u) && (signatureS[0] == 0u))
      {
        Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, &signatureR[1u], (eslt_Length)(sigLengthR - 1u)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        Crypto_30_LibCv_CopyData(&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[sigLengthR - 1u], &signatureS[1u], (eslt_Length)(sigLengthS - 1u)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (eslt_Length)(doubleSigLength - 2u); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

        outputIsSet = TRUE;
      }
      else
      {
        outputIsSet = FALSE;
      }
    }
    else
    {
      outputIsSet = FALSE;
    }

    /* # If the first bytes are not equal zero, copy the signature to the output ptr. */
    if (outputIsSet == FALSE)
# else
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(expectedKeyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif
    {
      Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, signatureR, (sigLengthR)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      Crypto_30_LibCv_CopyData(&job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr[sigLengthR], signatureS, (sigLengthS)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = doubleSigLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_EccPrimeGenerateSignature()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_EccPrimeGenerateSignature(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, retValDispatching = E_NOT_OK;
  uint32 expectedKeyLength = 0u;
  uint32 messageLengthCut = 0u;
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr = Crypto_30_LibCv_EccCurveEmptyDomain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr = Crypto_30_LibCv_EccCurveEmptyDomain;
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveEmptyDomain;

  /* ----- Implementation ------------------------------------------------- */
  /* # Choose domain parameters according to configured primitive. */
  switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
  {
# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P160R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveSecP160R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveSecP160R1DomainExt;
    EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveSecP160R1SpeedUpExt;
    expectedKeyLength = CRYPTO_30_LIBCV_SIZEOF_ECC_160_KEY_PRIVATE - 1u;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P256R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt;
    EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt;
    expectedKeyLength = CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P384R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP384R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt;
    EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt;
    expectedKeyLength = CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P521R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP521R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt;
    EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveNistSecP521R1SpeedUpExt;
    expectedKeyLength = CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON) */

  default:
    /* not reachable, misra only. */
    break;
  }

  if (retValDispatching == E_OK)
  {
    /* # Adapt message length according to maximum length. */
    messageLengthCut = messageLength;
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON)
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily != CRYPTO_ALGOFAM_NOT_SET) /* COV_CRYPTO_30_LIBCV_ECDSA_PREHASHING_ENABLED */
    {
      if (messageLength > esl_getMaxLengthOfEcPmessage(EcDomainPtr))  /* COV_CRYPTO_30_LIBCV_ECDSA_CONFIGURATION */ /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
      {
        messageLengthCut = esl_getMaxLengthOfEcPmessage(EcDomainPtr);  /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
      }
    }
# endif

    /* # # Call ECC algorithm. */
    retVal = Crypto_30_LibCv_EccPrimeGenerateSignature_SignData(job, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
      workSpace,
      EcDomainPtr,
      EcDomainExtPtr,
      EcSpeedUpExtPtr,
      expectedKeyLength,
      messagePtr,
      messageLengthCut);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint32 messageLength;
  const uint8 *messagePtr;

# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON))
  uint8 messageBuf[CRYPTO_30_LIBCV_PREHASH_DIGEST_MAXSIZE];
  Std_ReturnType localRetVal;
# endif

  /* ----- Implementation ------------------------------------------------- */
  /* # Handle pre-hashing finish. */
# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON))
  retVal = E_NOT_OK;
  messageLength = CRYPTO_30_LIBCV_PREHASH_DIGEST_MAXSIZE;
  if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_NOT_SET) /* COV_CRYPTO_30_LIBCV_ECDSA_PREHASHING_ENABLED */
  {
# endif
    /* without pre hashing */
    messagePtr = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr;
    messageLength = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength;
# if ((CRYPTO_30_LIBCV_SIGNATURE_GENERATE_PRE_HASH == STD_ON))
    localRetVal = E_OK;
  }
  else
  {
    localRetVal = Crypto_30_LibCv_PreHashFinish(&(workSpace->wsPreHash), messageBuf, &messageLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    messagePtr = messageBuf;
  }

  if (localRetVal == E_OK)
# endif
  {
    /* # Pass on potentially pre-hashed message for signature computation. */
    retVal = Crypto_30_LibCv_EccPrimeGenerateSignature(workSpace, job, messagePtr, messageLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
  /* # Handle pre-hashing start. */
  retVal = Crypto_30_LibCv_PreHashStart(&(workSpace->wsPreHash), job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(workSpace); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
  /* # Handle pre-hashing update. */
  retVal = Crypto_30_LibCv_PreHashUpdate(&(workSpace->wsPreHash), job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(workSpace); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish_CvOperation()
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
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 8 */ /* MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish_CvOperation(
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) messagePtr,
  uint32 messageLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  uint32 sigKeyLength = 0u;
  Crypto_30_LibCv_KeyElementGetType keyElements[1];

  /* ----- Implementation ------------------------------------------------- */
  /* # Retrieve the public key. */
  /* get esl size of key length for x and y component of the public key (key = [key_x|key_y]) */
  sigKeyLength = (uint32)(Crypto_30_LibCv_Math_Mul2((uint32)esl_getLengthOfEcPpublicKey_comp(EcDomainPtr)));  /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_SIGNATURE_KEY, sigKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # Initialize ECC workspace header. */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(workSpace->wsEcP.header), ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (retVal == E_OK)
    {
      /* # Initialize ECC algorithm. */
      retVal = E_NOT_OK;
      retValCv = esl_initVerifyDSAEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(workSpace->wsEcP), EcDomainPtr, EcDomainExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Check if value is even */
        if (Crypto_30_LibCv_Math_IsEven(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength))
        {
          /* # Calculate and verify signature. */
          retValCv = esl_verifyDSAEcP_prim(&(workSpace->wsEcP), (eslt_Length)messageLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))messagePtr,  /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
            (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex + Crypto_30_LibCv_Math_Div2(sigKeyLength)),
            (eslt_Length)Crypto_30_LibCv_Math_Div2(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength), (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
            (eslt_Length)Crypto_30_LibCv_Math_Div2(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength), (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr[(eslt_Length)Crypto_30_LibCv_Math_Div2(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength)]));

          retValCv = Crypto_30_LibCv_DispatchSignatureVerifyResult(job, retValCv, ESL_ERC_ECC_SIGNATURE_INVALID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */

          if (retValCv == ESL_ERC_NO_ERROR)
          {
            retVal = E_OK;
          }
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish()
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
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) workSpace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, retValDispatching = E_NOT_OK;
  uint32 messageLength;
  const uint8* messagePtr;

# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
  Std_ReturnType localRetVal;
# endif

  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr = Crypto_30_LibCv_EccCurveEmptyDomain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr = Crypto_30_LibCv_EccCurveEmptyDomain;

# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
  uint8 messageBuf[CRYPTO_30_LIBCV_PREHASH_DIGEST_MAXSIZE];
# endif

  /* ----- Implementation ------------------------------------------------- */
  *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

  switch (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode)
  {

# if (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P160R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveSecP160R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveSecP160R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P256R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P384R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP384R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON)
  case CRYPTO_ALGOMODE_CUSTOM_P521R1:
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP521R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif

  default:
    /* not reachable, misra only. */
    break;
  }

  if (retValDispatching == E_OK)
  {
    /* # Handle pre-hashing finish. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
    messageLength = CRYPTO_30_LIBCV_PREHASH_DIGEST_MAXSIZE;
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_NOT_SET) /* COV_CRYPTO_30_LIBCV_ECDSA_PREHASHING_ENABLED */
# endif
    {
      /* without pre hashing */
      messagePtr = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr;
      messageLength = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength;
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
      localRetVal = E_OK;
# endif
    }
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_PRE_HASH == STD_ON)
    else
    {
      localRetVal = Crypto_30_LibCv_PreHashFinish(&(workSpace->wsPreHash), messageBuf, &messageLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      messagePtr = messageBuf;
    }
    /* # Adapt message length according to maximum length. */
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily != CRYPTO_ALGOFAM_NOT_SET) /* COV_CRYPTO_30_LIBCV_ECDSA_PREHASHING_ENABLED */
    {
      if (messageLength > esl_getMaxLengthOfEcPmessage(EcDomainPtr)) /* COV_CRYPTO_30_LIBCV_ECDSA_CONFIGURATION */ /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
      {
        messageLength = esl_getMaxLengthOfEcPmessage(EcDomainPtr);  /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
      }
    }

    if (localRetVal == E_OK)
# endif
    {
      /* # Call ECC algorithm. */
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish_CvOperation(job, workSpace, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR */
                                                                                 EcDomainPtr, EcDomainExtPtr,
                                                                                 messagePtr, messageLength);
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateStart()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateStart(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  Crypto_30_LibCv_KeyElementGetType keyElements[5];

  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_PRIME_P); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PRIME_Q); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 2u, CRYPTO_KE_CUSTOM_RSA_EXPONENT_DP); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 3u, CRYPTO_KE_CUSTOM_RSA_EXPONENT_DQ); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 4u, CRYPTO_KE_CUSTOM_RSA_INVERSE_QI); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  /* # Initialize workspace header. */
  if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_CRT_SIG, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  {
    /* Check if key element is accessible */
    /* # Read key elements. */
    retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 5u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    if (retVal == E_OK)
    {
      retVal = E_NOT_OK;
      /* # Initialize algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
      if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_initSignRSACRTSHA1_V15(
          workspace,
          (eslt_Length)keyElements[0u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
          (eslt_Length)keyElements[1u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex),
          (eslt_Length)keyElements[2u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[2u].keyElementIndex),
          (eslt_Length)keyElements[3u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[3u].keyElementIndex),
          (eslt_Length)keyElements[4u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[4u].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_initSignRSACRTSHA256_V15(
          workspace,
          (eslt_Length)keyElements[0u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
          (eslt_Length)keyElements[1u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1u].keyElementIndex),
          (eslt_Length)keyElements[2u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[2u].keyElementIndex),
          (eslt_Length)keyElements[3u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[3u].keyElementIndex),
          (eslt_Length)keyElements[4u].keyElementLength,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[4u].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
      }
# endif
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateUpdate()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateUpdate(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  /* # Update algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
  if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA1)
#  endif
  {
    retValCv = esl_updateSignRSACRTSHA1_V15(
      workspace,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
  else
#  endif
  {
    retValCv = esl_updateSignRSACRTSHA256_V15(
      workspace,
      (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
# endif

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RsaPkcs1CrtGenerateFinish()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RsaPkcs1CrtGenerateFinish(
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;

  eslt_Length outputLength;
  outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

  /* # Generate Signature based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
  if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA1)
#  endif
  {
    retValCv = esl_finalizeSignRSACRTSHA1_V15(
      workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
  }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_CUSTOM_RSASSA_PKCS1_v1_5_CRT_SHA1 == STD_ON)
  else
#  endif
  {
    retValCv = esl_finalizeSignRSACRTSHA256_V15(
      workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
      (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
  }
# endif

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
    *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSACRTGENERATE == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Ed25519Generate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Ed25519Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  eslt_Length outputLength;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfEd25519Generate(Crypto_30_LibCv_GetEd25519GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceEd25519)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* # Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_Ed25519, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Initialize algorithm with or without pre - hashing based on secondary algorithm family. */
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
        {
          /* Init EdDsa with pre-hashing */
          retValCv = esl_initSignEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
                                       ESL_Curve25519,
                                       ESL_INSTANCE_Ed25519ph,
                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))NULL_PTR,
                                       0u);
        }
        else
        {
          /* Init EdDsa without pre-hashing */
          retValCv = esl_initSignEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
                                       ESL_Curve25519,
                                       ESL_INSTANCE_Ed25519,
                                       (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))NULL_PTR,
                                       0u);
        }
      }

      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Process prehashing if necessary. */
      if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
      {
        /* Prehash Calculation */
        retValCv = esl_updateEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength);
      }
      else
      {
        /* Nothing to do if no prehashing is used */
        if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength != 0u) && (!Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH)))
        {
          retVal = E_NOT_OK;
        }
        else
        {
          retVal = E_OK;
        }
      }
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[1];

      uint32 sigGenKeyLength = CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY;
      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_SIGNATURE_KEY, sigGenKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* Check if key element is accessible */
      /* # Read key elements. */
      retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      if (retVal == E_OK)
      {
        /* # Initialize algorithm. */
        retVal = E_NOT_OK;
        outputLength = (eslt_Length)*(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);

        /* # Generate Signature based on loaded signature key. */
        retValCv = esl_signEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                 (eslt_Length)((job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512) ? (0u) : (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength)),
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))NULL_PTR,
                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength);

        *(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr) = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
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

#endif /* (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Ed25519Verify()
 **********************************************************************************************************************/
/*!
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
 */
/* PRQA S 6030, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Ed25519Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceEd25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfEd25519Verify(Crypto_30_LibCv_GetEd25519VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceEd25519)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      /* # Initialize workspace header. */
      retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_SIZEOF_WS_Ed25519, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Initialize algorithm with or without pre-hashing based on secondary algorithm family. */
        if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
        {
          /* Init EdDsa with pre-hashing */
          retValCv = esl_initVerifyEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
                                         ESL_Curve25519,
                                         ESL_INSTANCE_Ed25519ph,
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))NULL_PTR,
                                         0u);
        }
        else
        {
          /* Init EdDsa without pre-hashing */
          retValCv = esl_initVerifyEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
                                         ESL_Curve25519,
                                         ESL_INSTANCE_Ed25519,
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))NULL_PTR,
                                         0u);
        }
      }
      break;
    }

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Process prehashing if necessary. */
      if (job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512)
      {
        /* Prehash Calculation */
        retValCv = esl_updateEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength);
      }
      else
      {
        /* Nothing to do if no prehashing is used */
        if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength != 0u) && (!Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH)))
        {
          retVal = E_NOT_OK;
        }
        else
        {
          retVal = E_OK;
        }
      }
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[1];

      uint32 sigGenKeyLength = CRYPTO_30_LIBCV_SIZEOF_SIGNATURE_KEY;
      Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_SIGNATURE_KEY, sigGenKeyLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      /* Check if key element is accessible */
      /* # Read key elements. */
      retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      if (retVal == E_OK)
      {
        /* # Verify Signature based on loaded signature key. */
        retVal = E_NOT_OK;
        retValCv = esl_verifyEdDSA(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                   (eslt_Length)((job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily == CRYPTO_ALGOFAM_SHA2_512) ? (0u) : (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength)),
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr,
                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength);

        if (retValCv == ESL_ERC_NO_ERROR)
        {
          *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
        else
        {
          if ((retValCv == ESL_ERC_SIGNATURE_INVALID) || (retValCv == ESL_ERC_ECC_SIGNATURE_INVALID))
          {
            retValCv = ESL_ERC_NO_ERROR;
          }
          *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
        }
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
#endif /* (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Generate()
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
 *
 *
 *
 *
 *
 */
 /* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
#  endif
# endif
  P2VAR(eslt_WorkSpaceRSAsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPkcs1Generate(Crypto_30_LibCv_GetRsaPkcs1GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAsig)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_SIG, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          retVal = E_NOT_OK;
          /* # Initialize algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
          if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
          {
            retValCv = esl_initSignRSASHA1_V15(workspace,
                                               (eslt_Length)keyElements[1].keyElementLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex),
                                               (eslt_Length)keyElements[0].keyElementLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
          else
#  endif
          {
            retValCv = esl_initSignRSASHA256_V15(workspace,
                                                 (eslt_Length)keyElements[1].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex),
                                                 (eslt_Length)keyElements[0].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
        }
      }
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Update algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_updateSignRSASHA1_V15(workspace,
                                             (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                             (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_updateSignRSASHA256_V15(workspace,
                                               (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      eslt_Length outputLength;
      outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);
      /* # Generate Signature based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_finalizeSignRSASHA1_V15(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                               &outputLength,
                                               (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_finalizeSignRSASHA256_V15(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                 &outputLength,
                                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
      }
# endif

      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = (uint32)outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */

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
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Verify()
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
#  endif
# endif
  P2VAR(eslt_WorkSpaceRSAver, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPkcs1Verify(Crypto_30_LibCv_GetRsaPkcs1VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAver)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_VER, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PUBLIC_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          retVal = E_NOT_OK;
          /* # Initialize algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
          if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
          {
            retValCv = esl_initVerifyRSASHA1_V15(workspace,
                                                 (eslt_Length)keyElements[0].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                 (eslt_Length)keyElements[1].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
          else
#  endif
          {
            retValCv = esl_initVerifyRSASHA256_V15(workspace,
                                                   (eslt_Length)keyElements[0].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                   (eslt_Length)keyElements[1].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
        }
      }
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Update algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_updateVerifyRSASHA1_V15(workspace,
                                               (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_updateVerifyRSASHA256_V15(workspace,
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* # Verify Signature based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_finalizeVerifyRSASHA1_V15(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr);

        retValCv = Crypto_30_LibCv_DispatchSignatureVerifyResult(job, retValCv, ESL_ERC_RSA_SIGNATURE_INVALID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PKCS1_v1_5_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_finalizeVerifyRSASHA256_V15(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr);

        retValCv = Crypto_30_LibCv_DispatchSignatureVerifyResult(job, retValCv, ESL_ERC_RSA_SIGNATURE_INVALID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
# endif
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
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPSSGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPssGenerate()
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPssGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
#  endif
# endif
  P2VAR(eslt_WorkSpaceRSAPSSsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPssGenerate(Crypto_30_LibCv_GetRsaPssGenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSAPSSsig)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[2];

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_RSA_PSS_SIG, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /* # Read key elements. */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_MODULUS); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 1u, CRYPTO_KE_CUSTOM_RSA_PRIVATE_EXPONENT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

        retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          retVal = E_NOT_OK;
          /* # Initialize algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
          if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
          {
            retValCv = esl_initSignRSASHA1_PSS(workspace,
                                               (eslt_Length)keyElements[0].keyElementLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                               (eslt_Length)keyElements[1].keyElementLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
          else
#  endif
          {
            retValCv = esl_initSignRSASHA256_PSS(workspace,
                                                 (eslt_Length)keyElements[0].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0].keyElementIndex),
                                                 (eslt_Length)keyElements[1].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[1].keyElementIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
        }
      }
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Update algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_updateSignRSASHA1_PSS(workspace,
                                             (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                             (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_updateSignRSASHA256_PSS(workspace,
                                               (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      Crypto_30_LibCv_KeyElementGetType keyElements[1];

      /* # Load salt key element. */
      Crypto_30_LibCv_Local_ElementGetterSetId(keyElements, 0u, CRYPTO_KE_CUSTOM_RSA_SALT); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(job->cryptoKeyId, keyElements, 1u, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

      if (retVal == E_OK)
      {
        eslt_Length outputLength;
        outputLength = (eslt_Length)(*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr);
        retVal = E_NOT_OK;

        /* # Generate Signature based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
        if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
        {
          retValCv = esl_finalizeSignRSASHA1_PSS(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                 (eslt_Length)keyElements[0u].keyElementLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                 (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
        }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_GENERATE_RSA_RSASSA_PSS_SHA1 == STD_ON)
        else
#  endif
        {
          retValCv = esl_finalizeSignRSASHA256_PSS(workspace, /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                   (eslt_Length)keyElements[0u].keyElementLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(keyElements[0u].keyElementIndex),
                                                   (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputLength,
                                                   (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);
        }
# endif
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
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
#endif /* (CRYPTO_30_LIBCV_RSAPSSGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPssVerify()
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPssVerify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;

  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
  Crypto_AlgorithmSecondaryFamilyType secAlgoFam = job->jobPrimitiveInfo->primitiveInfo->algorithm.secondaryFamily;
#  endif
# endif
  P2VAR(Crypto_30_LibCv_WorkSpaceRsaPssVer, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPssVerify(Crypto_30_LibCv_GetRsaPssVerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(Crypto_30_LibCv_WorkSpaceRsaPssVer)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      Crypto_30_LibCv_SizeOfKeyStorageType SigExponentIndex;
      uint32 SigExponentLength;
      Crypto_30_LibCv_SizeOfKeyStorageType SigModulusIndex;
      uint32 SigModulusLength;

      /* # Initialize workspace header. */
      if (esl_initWorkSpaceHeader(&(workspace->wsRsa.header), ESL_MAXSIZEOF_WS_RSA_PSS_VER, CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* PRQA S 3395 */ /* MD_CRYPTO_30_LIBCV_3395 */ /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        /* Check if key element is accessible */
        /*  # Read key elements. */
        retVal = Crypto_30_LibCv_SignatureRsaPssVerifyGetKeyElements(job->cryptoKeyId, &workspace->saltLength, &SigExponentIndex, &SigExponentLength, &SigModulusIndex, &SigModulusLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
        if (retVal == E_OK)
        {
          retVal = E_NOT_OK;
          /* # Initialize algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
          if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
          {
            retValCv = esl_initVerifyRSASHA1_PSS(&(workspace->wsRsa),
                                                 (eslt_Length)SigModulusLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(SigModulusIndex),
                                                 (eslt_Length)SigExponentLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(SigExponentIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
          else
#  endif
          {
            retValCv = esl_initVerifyRSASHA256_PSS(&(workspace->wsRsa),
                                                   (eslt_Length)SigModulusLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(SigModulusIndex),
                                                   (eslt_Length)SigExponentLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(SigExponentIndex)); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR */
          }
# endif
        }
      }
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      /* # Update algorithm based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_updateVerifyRSASHA1_PSS(&(workspace->wsRsa),
                                               (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_updateVerifyRSASHA256_PSS(&(workspace->wsRsa),
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
# endif
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      /* # Verify Signature based on secondary algorithm family. */
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
      if (secAlgoFam == CRYPTO_ALGOFAM_SHA1)
#  endif
      {
        retValCv = esl_finalizeVerifyRSASHA1_PSS(&(workspace->wsRsa), /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                 (eslt_Length)workspace->saltLength,
                                                 (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
                                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr);

        retValCv = Crypto_30_LibCv_DispatchSignatureVerifyResult(job, retValCv, ESL_ERC_RSA_SIGNATURE_INVALID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
# endif
# if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA2_256 == STD_ON)
#  if (CRYPTO_30_LIBCV_SIGNATURE_VERIFY_RSA_RSASSA_PSS_SHA1 == STD_ON)
      else
#  endif
      {
        retValCv = esl_finalizeVerifyRSASHA256_PSS(&(workspace->wsRsa), /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
                                                   (eslt_Length)workspace->saltLength,
                                                   (eslt_Length)job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength,
                                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr);

        retValCv = Crypto_30_LibCv_DispatchSignatureVerifyResult(job, retValCv, ESL_ERC_RSA_SIGNATURE_INVALID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
# endif
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
#endif /* (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1CrtGenerate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1CrtGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  P2VAR(eslt_WorkSpaceRSACRTsig, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfRsaPkcs1CrtGenerate(Crypto_30_LibCv_GetRsaPkcs1CrtGenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(eslt_WorkSpaceRSACRTsig)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    retVal = Crypto_30_LibCv_RsaPkcs1CrtGenerateStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    retVal = Crypto_30_LibCv_RsaPkcs1CrtGenerateUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    retVal = Crypto_30_LibCv_RsaPkcs1CrtGenerateFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    break;

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    /* Not supported State */
    break;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP160Generate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP160Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP160Generate(Crypto_30_LibCv_GetEcP160GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP160Verify()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP160Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP160Verify(Crypto_30_LibCv_GetEcP160VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP256Generate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP256Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP256Generate(Crypto_30_LibCv_GetEcP256GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP256Verify()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP256Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP256Verify(Crypto_30_LibCv_GetEcP256VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP384Generate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP384Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  /* NISTp384r1 = SECp384r1 */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP384Generate(Crypto_30_LibCv_GetEcP384GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP384Verify()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP384Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  /* NISTp384r1 = SECp384r1 */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP384Verify(Crypto_30_LibCv_GetEcP384VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP521Generate()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP521Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP521Generate(Crypto_30_LibCv_GetEcP521GenerateIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeGenerateFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON) */

#if (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP521Verify()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP521Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  workspace = Crypto_30_LibCv_GetWorkspaceOfEcP521Verify(Crypto_30_LibCv_GetEcP521VerifyIdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SetLengthOfSaveAndRestoreWorkspace(objectId, sizeof(vSecPrimType_WorkSpaceHash)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH */

  /* # Distinguish modes. */
  switch (mode)
  {
    /* # Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyStart(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }
    /* # Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyUpdate(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    /* # Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchSignatureEccPrimeVerifyFinish(workspace, job); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      break;
    }

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    {
      /* Not supported State */
      break;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Signature.c
 *********************************************************************************************************************/

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
/*      \file  Crypto_30_LibCv_SignatureVerify.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for SIGNATURE Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_SIGNATUREVERIFY_H)
# define CRYPTO_30_LIBCV_SIGNATUREVERIFY_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_LibCv_Cfg.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Ed25519Verify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature Ed25519 verify job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-137762
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Ed25519Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ED25519VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPKCS1VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Verify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature RSA PKCS1 verify job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-137763
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPKCS1VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPssVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature RSA PSS verify job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
                    CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-137966
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPssVerify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPSSVERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP160Verify()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job based on curve SECP160 to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 * \param[in]       objectId    Holds the identifier of the Crypto Driver Object.
 * \param[in,out]   job         Pointer to the job which shall be dispatched.
 * \param[in]       mode        Operation mode in which the job currently is.
 * \return          E_OK        Dispatching was successful.
 *                  E_NOT_OK    Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace 	        CREQ-244685
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP160Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP160VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP256Verify()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job based on curve SECP256 to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 * \param[in]       objectId    Holds the identifier of the Crypto Driver Object.
 * \param[in,out]   job         Pointer to the job which shall be dispatched.
 * \param[in]       mode        Operation mode in which the job currently is.
 * \return          E_OK        Dispatching was successful.
 *                  E_NOT_OK    Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace 	        CREQ-139644, CREQ-141042, CREQ-141044
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP256Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP256VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP384Verify()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job based on curve SECP384 to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 * \param[in]       objectId    Holds the identifier of the Crypto Driver Object.
 * \param[in,out]   job         Pointer to the job which shall be dispatched.
 * \param[in]       mode        Operation mode in which the job currently is.
 * \return          E_OK        Dispatching was successful.
 *                  E_NOT_OK    Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace 	        CREQ-158944
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP384Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP384VERIFY == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP521Verify()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime verify job based on curve SECP521 to the lower layer.
 * \details         This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 * \param[in]       objectId    Holds the identifier of the Crypto Driver Object.
 * \param[in,out]   job         Pointer to the job which shall be dispatched.
 * \param[in]       mode        Operation mode in which the job currently is.
 * \return          E_OK        Dispatching was successful.
 *                  E_NOT_OK    Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID      Request failed, the key is not valid.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 * \pre             objectId has to be a valid driver object handle.
 *                  job has to be a job object handle.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace 	        CREQ-313251, CREQ-322901
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP521Verify(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP521VERIFY == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_SIGNATUREVERIFY_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_SignatureVerify.h
 *********************************************************************************************************************/

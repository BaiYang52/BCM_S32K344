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
/*      \file  Crypto_30_LibCv_SignatureGenerate.h
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

#if !defined (CRYPTO_30_LIBCV_SIGNATUREGENERATE_H)
# define CRYPTO_30_LIBCV_SIGNATUREGENERATE_H

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

# if (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Ed25519Generate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature Ed25519 generate job to the lower layer.
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
 *  \trace          CREQ-137758
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Ed25519Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ED25519GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPKCS1GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Generate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature RSA PKCS1 generate job to the lower layer.
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
 *  \trace          CREQ-137967
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPKCS1GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPSSGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPssGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature RSA PSS generate job to the lower layer.
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
 *  \trace          CREQ-137761
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPssGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPSSGENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1CrtGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the signature RSA CRT PKCS1 generate job to the lower layer.
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
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-149659
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1CrtGenerate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPKCS1CRTGENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP160Generate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job based on curve SECP160 to the lower layer.
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
 * \trace 	        CREQ-244684
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP160Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP160GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP256Generate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job based on curve SECP256 to the lower layer.
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
 * \trace 	        CREQ-139645, CREQ-141041, CREQ-141043
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP256Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP256GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP384Generate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job based on curve SECP384 to the lower layer.
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
 * \trace 	        CREQ-158943, CREQ-158945
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP384Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP384GENERATE == STD_ON) */

# if (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_EcP521Generate()
 *********************************************************************************************************************/
/*!
 * \brief           Dispatches the signature Ecc prime generate job based on curve SECP521 to the lower layer.
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
 * \trace 	        CREQ-313250, CREQ-322902
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_EcP521Generate(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_ECP521GENERATE == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_SIGNATUREGENERATE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_SignatureGenerate.h
 *********************************************************************************************************************/

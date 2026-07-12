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
/*      \file  Crypto_30_LibCv_Decrypt.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for CIPHER Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_DECRYPT_H)
# define CRYPTO_30_LIBCV_DECRYPT_H

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

# if (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_AesDecrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherAesDecrypt job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-137765, CREQ-317644, CREQ-158941
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_AesDecrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_AESDECRYPT == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaPkcs1Decrypt job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-137764, CREQ-190479
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaPkcs1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAPKCS1DECRYPT == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha1Decrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaOaep Decrypt Sha1 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-168409
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA1DECRYPT == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepSha256Decrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaOaep Decrypt Sha256 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-168409
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepSha256Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAOAEPSHA256DECRYPT == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepCrtSha1Decrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaOaep Crt Decrypt Sha1 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-168872
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepCrtSha1Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA1DECRYPT == STD_ON) */

# if (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_RsaOaepCrtSha256Decrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RsaOaep Crt Decrypt Sha256 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_KEY_NOT_VALID Request failed, the key is not valid.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-168872
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_RsaOaepCrtSha256Decrypt(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_RSAOAEPCRTSHA256DECRYPT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_DECRYPT_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Decrypt.h
 *********************************************************************************************************************/

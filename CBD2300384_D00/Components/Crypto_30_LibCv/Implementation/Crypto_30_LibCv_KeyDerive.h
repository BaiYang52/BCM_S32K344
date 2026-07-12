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
/*      \file   Crypto_30_LibCv_KeyDerive.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for Key Derive Services.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_KEYDRIVE_H
# define CRYPTO_30_LIBCV_KEYDRIVE_H

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

# if (CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyDerive()
 *********************************************************************************************************************/
/*! \brief         Derives a key
 *  \details       Dispatch the key derivation.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]     targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in,out] writeBlock              Holds the result if the NvM need to be written.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           cryptoKeyId is a valid key id.
 *                 targetCryptoKeyId is a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock);
# endif /* (CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON) */

# if (CRYPTO_30_LIBCV_KEYDERIVENISTFIPS186ERB == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveNistFips186Erb()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveNistFips186Erb job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-145592
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNistFips186Erb(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif/* CRYPTO_30_LIBCV_KEYDERIVENISTFIPS186ERB() */

# if (CRYPTO_30_LIBCV_KEYDERIVEISO15118 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveISO15118()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveISO15118 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-145594
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveISO15118(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* Crypto_30_LibCv_Dispatch_KeyDeriveISO15118 */

# if (CRYPTO_30_LIBCV_KEYDERIVENIST80056AONEPASS == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveNist80056AOnePass()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveNist80056AOnePass job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-145593
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNist80056AOnePass(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* Crypto_30_LibCv_Dispatch_KeyDeriveNist80056AOnePass */

# if (CRYPTO_30_LIBCV_KEYDERIVENIST800108CNT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveNist800108Cnt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveNist800108Cnt job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-132407
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveNist800108Cnt(
   uint32 objectId,
   P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
   Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVENIST800108CNT */

# if (CRYPTO_30_LIBCV_KEYDERIVEX963SHA256 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA256()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveX963SHA256 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-191710
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEX963SHA256 */

# if (CRYPTO_30_LIBCV_KEYDERIVEX963SHA512 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA512()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveX963SHA512 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-191710
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveX963SHA512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEX963SHA512 */

# if (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA1()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveHmacSha1 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-248610
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA1(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA1 */

# if (CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA256()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveHmacSha256 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-248610
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveKDF2HMACSHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEKDF2HMACSHA256 */

# if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA256 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA256()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveHKDFHmacSha256 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-266053
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA256(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA256 */

# if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA384 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA384()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveHKDFHmacSha384 job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-266053
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHMACSHA384(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVEHKDFHMACSHA384 */

# if (CRYPTO_30_LIBCV_KEYDERIVEHKDFHASHOPTION1 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHashOption1()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDeriveHKDFHashOneStep job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *  \trace          CREQ-304740
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHashOption1(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* Crypto_30_LibCv_Dispatch_KeyDeriveHKDFHashOption1 */

# if (CRYPTO_30_LIBCV_KEYDERIVESPAKE2P == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyDeriveSpake2P()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeyDerive Spake2+ preamble job to the lower layer.
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
 *  \reentrant      TRUE, for different crypto keys
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyDeriveSpake2P(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYDERIVESPAKE2P */

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_KEYDRIVE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyDerive.h
 *********************************************************************************************************************/

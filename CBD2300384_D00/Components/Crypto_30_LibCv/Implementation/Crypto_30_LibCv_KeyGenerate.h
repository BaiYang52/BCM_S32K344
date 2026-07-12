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
/*      \file   Crypto_30_LibCv_KeyGenerate.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for Key Generate Services.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_KEYGENERATE_H
# define CRYPTO_30_LIBCV_KEYGENERATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_LibCv_Cfg.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP256R1_ENABLED == STD_ON) \
     || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON))
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED      STD_ON
# else
#  define CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED      STD_OFF
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyGenerate()
 *********************************************************************************************************************/
/*! \brief         Dispatches the key generation.
 *  \details       Dispatches the key value generation dependent on the given algorithm.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key generation
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyGenerate(uint32 cryptoKeyId);
# endif /* (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON) */

# if (CRYPTO_30_LIBCV_KEYGENSYMGENERIC == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyGenSymGeneric()
 *********************************************************************************************************************/
/*! \brief          Dispatches the key generate symmetric Generic job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId                    Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                         Pointer to the job which shall be dispatched.
 *  \param[in]      mode                        Operation mode in which the job currently is.
 *  \return         E_OK                        Dispatching was successful.
 *                  E_NOT_OK                    Dispatching failed.
 *                  CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                  CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                  CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                              data.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-265506
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenSymGeneric(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYGENSYMGENERIC == STD_ON */

# if (CRYPTO_30_LIBCV_KEYGENP256R1 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyGenP256R1()
 *********************************************************************************************************************/
/*! \brief          Dispatches the key generate P256R1 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId                    Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                         Pointer to the job which shall be dispatched.
 *  \param[in]      mode                        Operation mode in which the job currently is.
 *  \return         E_OK                        Dispatching was successful.
 *                  E_NOT_OK                    Dispatching failed.
 *                  CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                  CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                  CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                              data.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-240342
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenP256R1(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYGENP256R1 == STD_ON */

# if (CRYPTO_30_LIBCV_KEYGENP384R1 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyGenP384R1()
 *********************************************************************************************************************/
/*! \brief          Dispatches the key generate P384R1 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId                    Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                         Pointer to the job which shall be dispatched.
 *  \param[in]      mode                        Operation mode in which the job currently is.
 *  \return         E_OK                        Dispatching was successful.
 *                  E_NOT_OK                    Dispatching failed.
 *                  CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                  CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                  CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                              data.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-240342
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenP384R1(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYGENP384R1 == STD_ON) */

# if (CRYPTO_30_LIBCV_KEYGENED25519 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeyGenEd25519()
 *********************************************************************************************************************/
/*! \brief          Dispatches the key generate ED25519 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId                    Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                         Pointer to the job which shall be dispatched.
 *  \param[in]      mode                        Operation mode in which the job currently is.
 *  \return         E_OK                        Dispatching was successful.
 *                  E_NOT_OK                    Dispatching failed.
 *                  CRYPTO_E_BUSY               Request failed, Crypto Driver Object is busy.
 *                  CRYPTO_E_KEY_WRITE_FAIL     Request failed, write access was denied.
 *                  CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                              data.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-240342
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyGenEd25519(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYGENED25519 == STD_ON) */

# if ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws()
 *********************************************************************************************************************/
/*! \brief          Calculate Ecc
 *  \details        Calculate Ecc for ECDHE for the given values.
 *  \param[out]     publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *                                          The publicValuePtr contains the x- and y-value pair.
 *  \param[out]     privateValuePtr         Contains the pointer to the data where the private value shall be stored.
 *  \param[in]      domainPtr               Pointer to domain parameter structure
 *  \param[in]      domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]      speedUpExtPtr           Pointer to precomputation structure
 *  \param[in]      keySize                 Contains the length of the key element in bytes.
 *  \param[in,out]  workspace               Contains the workspace.
 *  \return         E_OK                    Request successful.
 *                  E_NOT_OK                Request failed.
 *  \pre            privateValuePtr has to be a valid pointer with keySize.
 *                  publicValuePtr has to be a valid pointer with size 2*keySize.
 *                  all pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE)  Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) privateValuePtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

# endif /* ((CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_ANSI_NIST_SEC_P256R1_ENABLED == STD_ON)\
     || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM_NIST_SEC_P384R1_ENABLED == STD_ON)) */

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_GENERATE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyGenerate.h
 *********************************************************************************************************************/

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
/*      \file   Crypto_30_LibCv_RandomSeed.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for Random Seed Services.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CRYPTO_30_LIBCV_RANDOMSEED_H
# define CRYPTO_30_LIBCV_RANDOMSEED_H

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

# if ((CRYPTO_30_LIBCV_FIPS186 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_RandomSeed()
 *********************************************************************************************************************/
/*! \brief         Initialize the seed
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           cryptoKeyId must identify a valid key
 *                 entropyPtr need to be a valid pointer
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength);
# endif /* ((CRYPTO_30_LIBCV_FIPS186 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)) */

# if (CRYPTO_30_LIBCV_KEYSEEDDRBGAES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeySeedDRBGAES()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeySeedDRBGAES job to the lower layer.
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
 *  \trace          CREQ-149660
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedDRBGAES(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_KEYSEEDDRBGAES */

# if (CRYPTO_30_LIBCV_KEYSEEDFIPS186 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySeedFips186()
 **********************************************************************************************************************/
/*! \brief          Dispatches the KeySeedFips186 job to the lower layer.
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
 *  \trace          CREQ-139102
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedFips186(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif/* CRYPTO_30_LIBCV_KEYSEEDFIPS186 */

# if (CRYPTO_30_LIBCV_KEYSEEDDRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_KeySeedDRBGHashSha512()
 *********************************************************************************************************************/
/*! \brief          Dispatches the KeySeedDRBGHashSha512 job to the lower layer.
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
 *  \trace          CREQ-149660
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedDRBGHashSha512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* Crypto_30_LibCv_KeySeedDRBGHashSha512 */

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_RANDOMSEED_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_RandomSeed.h
 *********************************************************************************************************************/

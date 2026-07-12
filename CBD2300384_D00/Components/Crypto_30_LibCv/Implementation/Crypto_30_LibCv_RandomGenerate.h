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
/*      \file  Crypto_30_LibCv_RandomGenerate.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_RANDOMGENERATE_H)
# define CRYPTO_30_LIBCV_RANDOMGENERATE_H

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

# if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Fips186()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RandomFips186 job to the lower layer.
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Fips186(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_FIPS186 == STD_ON */

# if (CRYPTO_30_LIBCV_DRBGAES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_DRBGAES()
 **********************************************************************************************************************/
/*! \brief          Dispatches the Nist 800-90A DRBG Aes job to the lower layer.
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_DRBGAES(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_DRBGAES == STD_ON */

# if (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_DRBGHashSha512()
 **********************************************************************************************************************/
/*! \brief          Dispatches the Nist 800-90A DRBG Hash job to the lower layer.
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_DRBGHashSha512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_RANDOMGENERATE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_RandomGenerate.h
 *********************************************************************************************************************/

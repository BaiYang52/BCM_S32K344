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
/*      \file  Crypto_30_LibCv_Hash.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for HASH Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_HASH_H)
# define CRYPTO_30_LIBCV_HASH_H

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

# if (CRYPTO_30_LIBCV_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Hash()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Hash job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library.
 *                  It will evaluate the correct hash based on the given job.
 *                  Based on the jobs operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-139111, CREQ-139110, CREQ-178100, CREQ-139109, CREQ-321212, CREQ-257656, CREQ-321213, 
 *                  CREQ-247953, CREQ-266807, CREQ-321214, CREQ-321215, CREQ-321216
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif /* (CRYPTO_30_LIBCV_HASH == STD_ON) */

# if ((CRYPTO_30_LIBCV_HASH == STD_ON) || (((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) && (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON)))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Hash_ModeStart()
 *********************************************************************************************************************/
/*!\brief           Handles hash operation mode START.
 *  \details        This function initializes both the workspace header and algorithm.
 *  \param[in]      workSpace          Pointer to the vSecPrim workspace.
 *  \param[in]      hashAlgoFam        Algorithm family of the hash.
 *  \return         E_OK               Start operation was successful.
 *                  E_NOT_OK           Start operation failed.
 *  \pre            workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeStart(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  Crypto_AlgorithmFamilyType hashAlgoFam);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Hash_ModeUpdate()
 *********************************************************************************************************************/
/*!\brief           Handles hash operation mode UPDATE.
 *  \details        This function processes the input data.
 *  \param[in]      workSpace          Pointer to the vSecPrim workspace.
 *  \param[in]      inputLength        Length of the input data in bytes.
 *  \param[in]      inputPtr           Pointer to the input data which should be hashed.
 *  \return         E_OK               Update operation was successful.
 *                  E_NOT_OK           Update operation failed.
 *  \pre            job has to be a job object handle.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeUpdate(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  uint32 inputLength,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) inputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Hash_ModeFinish()
 *********************************************************************************************************************/
/*!\brief           Handles hash operation mode FINISH.
 *  \details        This function writes the results in output buffers, adapts the output size of the hash value and
 *                  copies the data to the output location.
 *  \param[in]      workSpace          Pointer to the vSecPrim workspace.
 *  \param[in,out]  outputLengthPtr    Pointer to the output length of the hash.
 *                                     If the output length is smaller than the hash output,
 *                                     the hash output will be truncated accordingly.
 *  \param[out]     outputPtr          Pointer to the output buffer of the hash.
 *  \return         E_OK               Finish operation was successful.
 *                  E_NOT_OK           Finish operation failed.
 *  \pre            outputLengthPtr and outputPtr has to be valid.
 *                  workSpace has to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Hash_ModeFinish(
  P2VAR(vSecPrimType_WorkSpaceHash, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workSpace,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputPtr);
# endif /* ((CRYPTO_30_LIBCV_HASH == STD_ON) || (((CRYPTO_30_LIBCV_ECPGENERATE == STD_ON) || (CRYPTO_30_LIBCV_ECPVERIFY == STD_ON)) && (CRYPTO_30_LIBCV_SIGNATURE_PRE_HASH == STD_ON))) */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_HASH_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Hash.h
 *********************************************************************************************************************/

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
/*      \file  Crypto_30_LibCv_Random.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation for RANDOM Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_RANDOM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_RandomGenerate.h"
#include "Crypto_30_LibCv_RandomSeed.h"

#include "ESLib_types.h"
#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
# include "actFIPS186.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_RANDOM_SEED_SIZEOF_ALGORITHM                  (1u)

/* FIPS */
#define CRYPTO_30_LIBCV_FIPS_SEED_LENGTH                              (20u)

/* Reseed Interval */
#define CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN                     (CRYPTO_30_LIBCV_SIZEOF_UINT32) /* (sizeof(uint32)) */

/* HASHDRBG SHA512 SEED LEN */
#define CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN         (2u * ESL_HASHDRBG_SHA512_SEED_LEN)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomFips186Finish()
 *********************************************************************************************************************/
/*! \brief          Dispatches the RandomFips186 job to the lower layer.
 *  \details        This function dispatches the provided job to the lower layer crypto library. Based on the jobs
 *                  operation mode, it triggers different stages of the service.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomFips186Finish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_RandomSeed_Fips()
 *********************************************************************************************************************/
/*! \brief         Initialize the seed for FIPS 186.2
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[out]    writeBlock              Determines if the block need to be written.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-139102
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_Fips(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock);
#endif /* CRYPTO_30_LIBCV_FIPS186 == STD_ON */

#if (CRYPTO_30_LIBCV_DRBGAES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_AesCtrDrbgSeedFirst()
 **********************************************************************************************************************/
/*! \brief         Initial Seed for NIST800-90A DRBG AES128
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[out]    wsDRBG                  Pointer to the DRBG workspace
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[in]     modeDRBG                Contains the DRBG mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceCTRDRBG pointer.
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 modeDRBG has to be a valid
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgSeedFirst(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_AesCtrDrbgReseed()
 **********************************************************************************************************************/
/*! \brief          Reseed for NIST800-90A DRBG AES128
 *  \details        This function generates the internal seed state using the provided entropy source.
 *                  Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]      cryptoKeyId   Holds the identifier of the key for which a new seed shall be generated.
 *  \param[out]     wsDRBG        Pointer to the DRBG workspace
 *  \param[in]      entropyPtr    Holds a pointer to the memory location which contains the
 *                                data to feed the entropy.
 *  \param[in]      entropyLength Contains the length of the entropy in bytes.
 *  \param[in]      modeDRBG      indication which AES size the DRBG shall be based on and whether the seeding and
 *                                generating functions shall use a derivation function or not
 *  \return         E_OK          Request was successful.
 *                  E_NOT_OK      Request failed.
 *  \pre            cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                  wsDRBG has to be initialized and a valid eslt_WorkSpaceCTRDRBG pointer.
 *                  entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                  modeDRBG has to be a valid
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgReseed(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare()
 **********************************************************************************************************************/
/*! \brief          Prepare  Nist 800-90A DRBG Aes execution for esl_getBytesRng.
 *  \details        Check input parameter and prepare generation.
 *  \param[out]     ws                            Pointer to the workspace.
 *  \param[in]      cryptoKeyId                   Identifier of the crypto key.
 *  \param[out]     keyElements                   Struct to get element informations.
 *  \param[out]     seedStateElementIndexPtr      Holds the element index for Seed state.
 *  \param[out]     seedCounterElementIndexPtr    Holds the element index for Seed counter.
 *  \param[out]     persist                       Determines igf the block need to be written.
 *  \param[in]      seedLength                    Contains the seed length.
 *  \param[in]      modeDRBG                      Contains the DRBG mode.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy exhausted, random seed need to be called.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            all pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *                  seedLength has to be a valid length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare(
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedStateElementIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedCounterElementIndexPtr,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist,
  uint32 seedLength,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes()
 **********************************************************************************************************************/
/*! \brief         Initialize the seed for NIST800-90A DRBG AES128
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[out]    writeBlock              Determines if the block need to be written.
 *  \param[in]     modeDRBG                Contains the DRBG mode.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 seedStateElementIndex need to be a valid index.
 *                 modeDRBG has to be a valid
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-149660
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  eslt_DRBGMode modeDRBG);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws()
 **********************************************************************************************************************/
/*! \brief         Initialize the seed for NIST800-90A DRBG AES128
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[out]    writeBlock              Determines if the block need to be written.
 *  \param[in]     modeDRBG                Contains the DRBG mode.
 *  \param[in,out] wsDRBG                  Pointer to the DRBG workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 seedStateElementIndex need to be a valid index.
 *                 modeDRBG has to be a valid
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceCTRDRBG pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  eslt_DRBGMode modeDRBG,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG);

/**********************************************************************************************************************
 * Crypto_30_LibCv_AesCtrDrbgSeed_With_Ws()
 **********************************************************************************************************************/
/*! \brief         Seed for NIST800-90A DRBG AES128
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the index of the seed state
 *  \param[in]     reseedCtElementIndex    Holds the index of the reseed counter
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[in]     modeDRBG                Contains the DRBG mode.
 *  \param[in,out] wsDRBG                  Pointer to the DRBG workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 seedStateElementIndex need to be a valid index.
 *                 reseedCtElementIndex need to be a valid index.
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 modeDRBG has to be a valid
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceCTRDRBG pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgSeed_With_Ws(
  uint32 cryptoKeyId, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG);

# if (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_AES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetRandomNistDrbgAes()
 **********************************************************************************************************************/
/*! \brief          Nist 800-90A DRBG Aes execution for esl_getBytesRng.
 *  \details        Generate random number. This includes a retry mechanism for generation collision.
 *  \param[in]      cryptoKeyId                   Identifier of the crypto key.
 *  \param[out]     requestBuffer                 Buffer to safe the generated data.
 *  \param[in]      requestLength                 Length of the requested random data.
 *  \param[out]     persist                       Determines igf the block need to be written.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy exhausted, random seed need to be called.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            All pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetRandomNistDrbgAes(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
# endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgAesFinish()
 **********************************************************************************************************************/
/*! \brief          Prepare  Nist 800-90A DRBG Aes execution.
 *  \details        Check input parameter and prepare generation.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgAesFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Generate_NistDrbgAes()
 **********************************************************************************************************************/
/*! \brief          Generate random number.
 *  \details        Check input parameter and prepare generation.
 *  \param[in]      cryptoKeyId        Identifier of the crypto key.
 *  \param[out]     requestBuffer      Buffer to safe the generated data.
 *  \param[in]      requestLength      Length of the requested random data.
 *  \param[in,out]  wsDRBG             Pointer to the DRBG workspace.
 *  \return         All return values of esl_getBytesCTRDRBG.
 *  \pre            All pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Generate_NistDrbgAes(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState()
 **********************************************************************************************************************/
/*! \brief          Get and set NistDrbgAes state.
 *  \details        Get state parameter and set seed status.
 *  \param[in]      seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]      seedCounterElementIndex Holds the identifier of the key element which shall be set
 *  \param[in,out]  wsDRBG                  Pointer to the DRBG workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength()
 **********************************************************************************************************************/
/*! \brief          Set NistDrbgAes mode and seed length.
 *  \details        Get correct data from the cryptoKeyId and set NistDrbgAes mode and seed length.
 *  \param[in]      cryptoKeyId        Identifier of the crypto key.
 *  \param[in,out]  modeDRBG           Contains the DRBG mode.
 *  \param[in,out]  seedLength         Contains the seed length.
 *  \return         E_OK               Request was successful.
 *                  E_NOT_OK           Request failed.
 *  \pre            All pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength(
  uint32 cryptoKeyId,
  P2VAR(eslt_DRBGMode, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) modeDRBG,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) seedLength); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
#endif /* CRYPTO_30_LIBCV_DRBGAES == STD_ON */

#if (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_HashDrbgSeedFirst()
 **********************************************************************************************************************/
/*! \brief         Initial Seed for NIST800-90A DRBG HASH SHA2-512
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[out]    wsDRBG                  Pointer to the DRBG workspace
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceHASHDRBG pointer.
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgSeedFirst(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_HashDrbgReseed()
 **********************************************************************************************************************/
/*! \brief          Reseed for NIST800-90A DRBG HASH SHA2-512
 *  \details        This function generates the internal seed state using the provided entropy source.
 *                  Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]      cryptoKeyId   Holds the identifier of the key for which a new seed shall be generated.
 *  \param[out]     wsDRBG        Pointer to the DRBG workspace
 *  \param[in]      entropyPtr    Holds a pointer to the memory location which contains the
 *                                data to feed the entropy.
 *  \param[in]      entropyLength Contains the length of the entropy in bytes.
 *  \return         E_OK          Request was successful.
 *                  E_NOT_OK      Request failed.
 *  \pre            cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                  wsDRBG has to be initialized and a valid eslt_WorkSpaceHASHDRBG pointer.
 *                  reseedCnt has to be a valid pointer with the length CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN.
 *                  entropyPtr need to be a valid pointer with at least the length of entropyLength
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgReseed(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash()
 **********************************************************************************************************************/
/*! \brief         Initialize the seed for NIST800-90A DRBG HASH SHA2-512
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[out]    writeBlock              Determines if the block need to be written.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 seedStateElementIndex need to be a valid index.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *  \trace         CREQ-149660
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws()
 **********************************************************************************************************************/
/*! \brief         Initialize the seed for NIST800-90A DRBG HASH SHA2-512
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[out]    writeBlock              Determines if the block need to be written.
 *  \param[in,out] wsDRBG                  Pointer to the DRBG workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 seedStateElementIndex need to be a valid index.
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceHASHDRBG pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG);

/**********************************************************************************************************************
 * Crypto_30_LibCv_HashDrbgSeed_With_Ws()
 **********************************************************************************************************************/
/*! \brief         Seed for NIST800-90A DRBG HASH SHA2-512
 *  \details       This function generates the internal seed state using the provided entropy source.
 *                 Furthermore, this function can be used to update the seed state with new entropy
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the index of the seed state
 *  \param[in]     reseedCtElementIndex    Holds the index of the reseed counter
 *  \param[in]     entropyPtr              Holds a pointer to the memory location which contains the
 *                                         data to feed the entropy.
 *  \param[in]     entropyLength           Contains the length of the entropy in bytes.
 *  \param[in,out] wsDRBG                  Pointer to the DRBG workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *                 entropyPtr need to be a valid pointer with at least the length of entropyLength
 *                 seedStateElementIndex need to be a valid index.
 *                 reseedCtElementIndex need to be a valid index.
 *                 wsDRBG has to be initialized and a valid eslt_WorkSpaceHASHDRBG pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgSeed_With_Ws(
  uint32 cryptoKeyId, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState()
 **********************************************************************************************************************/
/*! \brief          Get and set NistDrbgHash state.
 *  \details        Get state parameter and set seed status.
 *  \param[in]      seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]      seedCounterElementIndex Holds the identifier of the key element which shall be set
 *  \param[in,out]  wsDRBG                  Pointer to the DRBG workspace.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgHashFinish()
 **********************************************************************************************************************/
/*! \brief          Prepare  Nist 800-90A DRBG Hash execution.
 *  \details        Check input parameter and prepare generation.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgHashFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare()
 **********************************************************************************************************************/
/*! \brief          Prepare  Nist 800-90A DRBG Hash execution for esl_getBytesHASHDRBG.
 *  \details        Check input parameter and prepare generation.
 *  \param[out]     ws                            Pointer to the workspace.
 *  \param[in]      cryptoKeyId                   Identifier of the crypto key.
 *  \param[out]     keyElements                   Struct to get element informations.
 *  \param[out]     seedStateElementIndexPtr      Holds the element index for Seed state.
 *  \param[out]     seedCounterElementIndexPtr    Holds the element index for Seed counter.
 *  \param[out]     persist                       Determines igf the block need to be written.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy exhausted, random seed need to be called.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            all pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare(
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedStateElementIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedCounterElementIndexPtr,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Generate_NistDrbgHash()
 **********************************************************************************************************************/
/*! \brief          Generate random number.
 *  \details        Check input parameter and prepare generation.
 *  \param[in]      cryptoKeyId        Identifier of the crypto key.
 *  \param[out]     requestBuffer      Buffer to safe the generated data.
 *  \param[in]      requestLength      Length of the requested random data.
 *  \param[in,out]  wsDRBG             Pointer to the DRBG workspace.
 *  \return         All return values of esl_getBytesHASHDRBG.
 *  \pre            All pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Generate_NistDrbgHash(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */

# if (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetRandomNistDrbgHash()
 **********************************************************************************************************************/
/*! \brief          Nist 800-90A DRBG Hash execution for esl_getBytesRng.
 *  \details        Generate random number. This includes a retry mechanism for generation collision.
 *  \param[in]      cryptoKeyId                   Identifier of the crypto key.
 *  \param[out]     requestBuffer                 Buffer to safe the generated data.
 *  \param[in]      requestLength                 Length of the requested random data.
 *  \param[out]     persist                       Determines igf the block need to be written.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *                  CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy exhausted, random seed need to be called.
 *                  CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre            All pointers need to be valid.
 *                  cryptoKeyId has to be a valid key id.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetRandomNistDrbgHash(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist); /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
# endif
#endif /* CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON */

#if ((CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON))
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Random_Check_For_Write_Once()
 *********************************************************************************************************************/
/*! \brief         Check the key element for write once
 *  \details       This function checks the key element for write once
 *  \param[in]     cryptoKeyId             Holds the identifier of the key for which a new seed shall be generated.
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in,out] reseedCtElementIndex    Holds the identifier of the key element which shall be set
 *  \return        TRUE                    Write once inactive.
 *                 FALSE                   Write once active.
 *  \pre           cryptoKeyId must identify a valid key
 *                 seedStateElementIndex need to be a valid index.
 *                 reseedCtElementIndex need to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Random_Check_For_Write_Once(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) reseedCtElementIndex);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Set_NistDrbgState()
 *********************************************************************************************************************/
/*! \brief         Set the key element state and the reseed counter
 *  \details       This function sets the key element state and the reseed counter
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     seedCounterElementIndex Holds the identifier of the key element which shall be set
 *  \param[in]     retValCv                Holds the return value
 *  \param[in]     seedLength              Holds the seed length
 *  \param[in]     expectedSeedLength      Holds the expected seed length
 *  \param[in]     reseedCntBuf            Holds the reseed counter
 *  \param[in]     seedStatus              Holds the current seed status
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           seedStateElementIndex need to be a valid index.
 *                 reseedCtElementIndex need to be a valid index.
 *                 retValCv need to be a valid value.
 *                 seedLength need to be a valid length.
 *                 expectedSeedLength need to be a valid length.
 *                 reseedCntBuf need to be a valid buffer.
 *                 seedStatus need to be a valid status.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Set_NistDrbgState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  eslt_ErrorCode retValCv,
  eslt_Length seedLength,
  eslt_Length expectedSeedLength,
  uint32 reseedCntBuf,
  eslt_DRBGSeedStatusType seedStatus);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist()
 *********************************************************************************************************************/
/*! \brief         Check the key element for persist
 *  \details       This function checks the key element for persist
 *  \param[in]     seedStateElementIndex   Holds the identifier of the key element which shall be set
 *  \param[in]     seedCounterElementIndex Holds the identifier of the key element which shall be set
 *  \param[out]    persist                 Holds the return value
 *  \pre           seedStateElementIndex need to be a valid index.
 *                 reseedCtElementIndex need to be a valid index.
 *                 persist need to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist);
#endif /* ((CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)) */

#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_IsRngCalculationValid()
 **********************************************************************************************************************/
/*! \brief          Check if the Rng Calculation is valid.
 *  \details        Check if the Rng counter has the expected value.
 *  \param[in]      localCount      holds the start counter.
 *  \return         TRUE            Same counter value.
 *                  FALSE           Different counter value.
 *  \pre            This function need to be protected by an exclusive area
 *                  (Crypto_30_LibCv_RandomSourceGenerateCount is accessed).
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_IsRngCalculationValid(uint16 localCount);
#endif /* CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON */

#if (CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyValueChangedCallout()
 **********************************************************************************************************************/
/*! \brief          Call key value changed callout.
 *  \details        -
 *  \param[in]      cryptoKeyId           Holds the identifier of the key.
 *  \pre            cryptoKeyId must identify a valid key.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyValueChangedCallout(uint32 cryptoKeyId);
#endif /* CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomFips186Finish()
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
 */
/* PRQA S 6030, 6050, 6080 3 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomFips186Finish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode retValCv;
  P2VAR(eslt_WorkSpaceFIPS186, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfFips186(Crypto_30_LibCv_GetFips186IdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SizeOfKeyStorageType fips186KeyIndex;
  uint32 fips186KeyLength;
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) randomSeedBuffer = &Crypto_30_LibCv_GetRandomSeedBuffer(Crypto_30_LibCv_GetRandomSeedBufferStartIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  /* #50 Initialize workspace header. */
  retValCv = esl_initWorkSpaceHeader(&(workspace->header), ESL_MAXSIZEOF_WS_FIPS186, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;

    /* Get index of keyElement SEED_STATE */
    if (Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* #60 Distinguish the different modes for seeding and restoring seed state. */
      /* Check if randomSeed was called on the given key element */
      if (!Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_MASK))
      {
        /* Get new seed value */
        retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &fips186KeyIndex, &fips186KeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
        if (retVal == E_OK)
        {
          retVal = E_NOT_OK;
          /* check if initial seed is required */
          /* #70 Seed if RandomSeed was called on the given key. (first time) */
          if (!Crypto_30_LibCv_Uint8CheckMask(randomSeedBuffer[0], CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
          {
            retValCv = esl_initFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                       (eslt_Length)fips186KeyLength,
                                       (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(fips186KeyIndex),
                                       NULL_PTR);
            /* mark new seed as initiated */
            Crypto_30_LibCv_Uint8SetMask(randomSeedBuffer[0], CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
          }
          else
          {
            /* #80 Reseed if RandomSeed was called on the given key and restore the old seed state. */
            /* Restore workspace - This is done manually since workspace is not kept forever due to shared memory (union) */
            retValCv = esl_initFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                       (eslt_Length)fips186KeyLength,
                                       (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(fips186KeyIndex),
                                       (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&randomSeedBuffer[1]);
          }
          /* mark new seed as used */
          Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
          if (Crypto_30_LibCv_IsKeyElementPersist(elementIndex) == TRUE)
          {
            Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = TRUE; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
          }
# endif
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          /* keep retVal */
        }
      }
      else
      {
        if (Crypto_30_LibCv_Uint8CheckMask(randomSeedBuffer[0], CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
        {
          /* #85 If the rng is not seeded only restore old seed state. */
          /* Restore workspace - This is done manually since workspace is not kept forever due to shared memory (union) */
          retValCv = esl_initFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
                                     (eslt_Length)CRYPTO_30_LIBCV_FIPS_SEED_LENGTH,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&randomSeedBuffer[1],
                                     NULL_PTR);
        }
        else
        {
          retValCv = ESL_ERC_ERROR;
          retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
        }
      }
    }
    else
    {
      retValCv = ESL_ERC_ERROR;
    }

    /* #90 Generate Random Numbers. */
    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retValCv = esl_getBytesFIPS186(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                     (eslt_Length)*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
                                     (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr);

      /* #95 Save workspace. */
      /* Save internal state of key random method to continue with the next good value. */
      Crypto_30_LibCv_CopyData(&randomSeedBuffer[1], ((P2VAR(actFIPS186STRUCT, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))workspace->wsFIPS186)->X_KEY, CRYPTO_30_LIBCV_FIPS_SEED_LENGTH); /* PRQA S 3305, 0310, 0315 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST, MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ACT_RANDOM_WORKSPACE_ARRAY_ACCESS */
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }
  else if (retVal == CRYPTO_E_KEY_NOT_VALID)
  {
    retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
  }
  else
  {
    /* keep retVal */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_Fips()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_Fips(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Deny service for write once keys. */
  if (Crypto_30_LibCv_IsKeyElementWriteOnce(seedStateElementIndex))
  {
    retVal = E_NOT_OK;
  }
  /* #10 Seed key element and update seed state */
  else if (Crypto_30_LibCv_Local_KeyElementSetInternal(cryptoKeyId, seedStateElementIndex, entropyPtr, entropyLength, CRYPTO_30_LIBCV_WA_ALLOWED) == E_OK) /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
  {
    /* The elementIndex do not need to be checked. The Code is only reachable if the key element is available. (Crypto_30_LibCv_Local_KeyElementSearch) */
    Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_NORMAL_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    *writeBlock = Crypto_30_LibCv_SetKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK);  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    retVal = E_OK;
  }
  else
  {
    *writeBlock = FALSE;  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_FIPS186 == STD_ON) */

#if (CRYPTO_30_LIBCV_DRBGAES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_AesCtrDrbgReseed()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgReseed(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyStorageType additionalInputIndex = 0u, seedIndex, reseedCounter;
  uint32 reseedCnt, additionalInputLength = 0u;
  uint32 seedLength = ESL_CTRDRBG_AES128_SEEDLEN;
  uint32 reseedCntLength = CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN;

  /* ----- Implementation ------------------------------------------------- */

  if ((modeDRBG == ESL_DRBGMODE_CTRDRBG_AES256_NODF) || (modeDRBG == ESL_DRBGMODE_CTRDRBG_AES256_DF))
  {
    seedLength = ESL_CTRDRBG_AES256_SEEDLEN;
  }

  if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &seedIndex, &seedLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  {
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, &reseedCounter, &reseedCntLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    {
      /* #1 Reseed: restore seed state */
      Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCnt, Crypto_30_LibCv_GetAddrKeyStorage(reseedCounter)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      retValCv = esl_restoreStateCTRDRBG(wsDRBG,
                                        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(seedIndex),
                                        (eslt_Length) seedLength,
                                        (eslt_DRBGReseedCounterType) reseedCnt,
                                        (eslt_Byte) ESL_DRBG_SEED_STATUS_SEEDED,
                                        modeDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #5 check if additional input is available. (optional feature) */
        localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT, &additionalInputIndex, &additionalInputLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      }
    }

    if ((localRetVal == E_OK) && (additionalInputLength > 0u))
    {
      /* #10 reseed via esl_seedCTRDRBG */
      retValCv = esl_seedCTRDRBG(wsDRBG,
                                (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) entropyPtr,
                                (eslt_Length) entropyLength,
                                NULL_PTR,
                                0u,
                                NULL_PTR,
                                0u,
                                (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(additionalInputIndex),
                                (eslt_Length) additionalInputLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else if ((localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (localRetVal == E_OK))
    {
      retValCv = esl_seedCTRDRBG(wsDRBG,
                                (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) entropyPtr,
                                (eslt_Length) entropyLength,
                                NULL_PTR,
                                0u,
                                NULL_PTR,
                                0u,
                                NULL_PTR,
                                0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else
    {
      /* request failed */
      retValCv = ESL_ERC_ERROR;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_AesCtrDrbgSeedFirst()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgSeedFirst(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal = E_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyStorageType personalizationStrIndex, nonceIndex = 0u;
  uint32 personalizationStrLength, nonceLength = 0u;

  /* ----- Implementation ------------------------------------------------- */

  if ((modeDRBG == ESL_DRBGMODE_CTRDRBG_AES128_DF) || (modeDRBG == ESL_DRBGMODE_CTRDRBG_AES256_DF))
  {
    /* #10 Get Nonce */
    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_NONCE, &nonceIndex, &nonceLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  }

  if (localRetVal == E_OK)
  {
    /* #15 Get Private String (recommended feature) */
    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_PERSONALIZATION_STRING, &personalizationStrIndex, &personalizationStrLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if ((localRetVal == E_OK) && (personalizationStrLength > 0u))
    {
      /* #20 seed with personal string via esl_seedCTRDRBG */
      retValCv = esl_seedCTRDRBG(wsDRBG,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) entropyPtr,
        (eslt_Length)entropyLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(nonceIndex),
        (eslt_Length)nonceLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(personalizationStrIndex),
        (eslt_Length)personalizationStrLength,
        NULL_PTR,
        0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else if ((localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (localRetVal == E_OK))
    {
      /* #25 Personal string in not available in the key so seed without personal string. */
      retValCv = esl_seedCTRDRBG(wsDRBG,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) entropyPtr,
        (eslt_Length) entropyLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(nonceIndex),
        (eslt_Length) nonceLength,
        NULL_PTR,
        0u,
        NULL_PTR,
        0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else
    {
      /* request failed */
    }

    /* #30 set algorithm initiated */
    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_AesCtrDrbgSeed_With_Ws()
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
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_AesCtrDrbgSeed_With_Ws(
  uint32 cryptoKeyId, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  eslt_DRBGMode modeDRBG,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  /* #1 Initialize workspace header. */
  retValCv = esl_initWorkSpaceHeader(&(wsDRBG->header), ESL_MAXSIZEOF_WS_CTRDRBG, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  retValCv |= esl_initCTRDRBG(wsDRBG, modeDRBG); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* #5 already initiated / Reseeding */
    if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
    {
      retVal = Crypto_30_LibCv_AesCtrDrbgReseed(cryptoKeyId, wsDRBG, entropyPtr, entropyLength, modeDRBG); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
    }
    /* #10 need to be initiated */
    else
    {
      retVal = Crypto_30_LibCv_AesCtrDrbgSeedFirst(cryptoKeyId, wsDRBG, entropyPtr, entropyLength, modeDRBG); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
    }

    if (retVal == E_OK)
    {
      /* #20 Get current DRBG state and save the seed status */
      retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState(
        seedStateElementIndex,
        reseedCtElementIndex,
        wsDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
  }

  if ((retValCv != E_OK) || (retVal != E_OK))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    Crypto_30_LibCv_ClearKeyElementStateByMask(reseedCtElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    /* #30 Call the callout if the functionality is enabled and trigger that the key value is changed */
    /* Key validity changed callout is called in Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws */
# if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
    Crypto_30_LibCv_KeyValueChangedCallout(cryptoKeyId);
# endif
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare(
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ws, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedStateElementIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedCounterElementIndexPtr,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist,
  uint32 seedLength,
  eslt_DRBGMode modeDRBG) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* # load seed state and reseed counter */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_RANDOM_SEED_STATE, seedLength); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 1u, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    /* Get index of keyElement SEED_STATE and CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER */
    (void)Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, seedStateElementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    (void)Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, seedCounterElementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  /* # Initialize workspace header. */
    if (esl_initWorkSpaceHeader(&(ws->header), ESL_MAXSIZEOF_WS_CTRDRBG, CRYPTO_30_LIBCV_WATCHDOG_PTR) != ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
    {
      retVal = E_NOT_OK;
    }
    else
    {
      /* # Initialize CTR DRBG workspace. */
      if (esl_initCTRDRBG(ws, modeDRBG) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist(*seedStateElementIndexPtr, *seedCounterElementIndexPtr, persist); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else if (retVal == CRYPTO_E_KEY_NOT_VALID)
  {
    retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
  }
  else
  {
    /* keep retVal */
  }

  return retVal;
}

# if (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_AES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetRandomNistDrbgAes()
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
 *
 *
 */
/* PRQA S 6010, 6050 5 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetRandomNistDrbgAes(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_WorkSpaceCTRDRBG ws;
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex = 0u, seedCounterElementIndex = 0u;
  Crypto_30_LibCv_DefaultRandomMaxRetriesOfGeneralType retry;
  uint16 localCount;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];
  uint32 reseedCntBuf;
  eslt_Byte seedStatus;
  eslt_DRBGMode modeDRBG = 0u;
  eslt_Length seedLength = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* # Get Algorithm from the algorithm key element to set the correct DRBG mode */
  if (Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength(cryptoKeyId, &modeDRBG, &seedLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* # Prepare DRBG and check key data. */
    retVal = Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare(&ws, cryptoKeyId, keyElements, &seedStateElementIndex, &seedCounterElementIndex, persist, (uint32)seedLength, modeDRBG); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  }

  if (retVal == E_OK)
  {
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    /* # Calculate random number until calculation is valid or max reties exhausted. */
    for (retry = 0u; retry < CRYPTO_30_LIBCV_DEFAULT_RANDOM_MAX_RETRIES; retry++)
    {
      /* # Copy counter and RNG paramter to local buffers. */
      localCount = Crypto_30_LibCv_RandomSourceGenerateCount;
      Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCntBuf, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedCounterElementIndex)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
      {
        seedStatus = ESL_DRBG_SEED_STATUS_SEEDED;
      }
      else
      {
        seedStatus = ESL_DRBG_SEED_STATUS_UNSEEDED;
      }

      retValCv = esl_restoreStateCTRDRBG(
        (P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &ws, /* PRQA S 2982 */ /* MD_MSR_RetVal */
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
        seedLength,
        reseedCntBuf,
        seedStatus,
        modeDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Generate random number DRBG. */
        retValCv = Crypto_30_LibCv_Local_Generate_NistDrbgAes(
          cryptoKeyId,
          requestBuffer,
          requestLength,
          &ws); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      /* # Check if rng counter has changed. */
      if ((Crypto_30_LibCv_IsRngCalculationValid(localCount)) && (retValCv == ESL_ERC_NO_ERROR))
      {
        retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState(
          seedStateElementIndex,
          seedCounterElementIndex,
          &ws); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        /* break loop */
        break;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (retVal != E_OK)
    {
      /* # clear output buffer if request failed. */
      Crypto_30_LibCv_ClearData(requestBuffer, requestLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      *persist = FALSE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    }
  }

  if ((retValCv != ESL_ERC_NO_ERROR) || (retVal != E_OK))
  {
    if (retValCv == ESL_ERC_ENTROPY_EXHAUSTION)
    {
      retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_AES == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgAesFinish()
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
/* PRQA S 6050 3 */ /* MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgAesFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex = 0u, seedCounterElementIndex = 0u;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];
  eslt_DRBGReseedCounterType reseedCntBuf;
  eslt_DRBGMode modeDRBG = 0u;
  eslt_Byte seedStatus;
  eslt_Length seedLength = 0u;
  boolean persist = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 load workspace */
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfDRBGAES(Crypto_30_LibCv_GetDRBGAESIdxOfObjectInfo(objectId));

  if (Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength(job->cryptoKeyId, &modeDRBG, &seedLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* #20 Load seed state, seed counter and check key data. */
    localRetVal = Crypto_30_LibCv_DispatchRandomNistDrbgAesGeneratePrepare(workspace, job->cryptoKeyId, keyElements, &seedStateElementIndex, &seedCounterElementIndex, &persist, (uint32)seedLength, modeDRBG); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  }

  if (localRetVal == E_OK)
  {
    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCntBuf, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedCounterElementIndex)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
    {
      seedStatus = ESL_DRBG_SEED_STATUS_SEEDED;
    }
    else
    {
      seedStatus = ESL_DRBG_SEED_STATUS_UNSEEDED;
    }

    /* #30 Set state to current seed state. */
    retValCv = esl_restoreStateCTRDRBG(
      workspace,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
      seedLength,
      reseedCntBuf,
      seedStatus,
      modeDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      /* #31 generate random number via esl_getBytesHASHDRBG. */
      retValCv = Crypto_30_LibCv_Local_Generate_NistDrbgAes(
        job->cryptoKeyId,
        job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
        workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #32 Set current state. */
        retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState(
          seedStateElementIndex,
          seedCounterElementIndex,
          workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
    }
  }
  else
  {
    retVal = localRetVal;
  }

  if (retValCv == ESL_ERC_ENTROPY_EXHAUSTION)
  {
    retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
  }

  if (retVal != E_OK)
  {
    /* # clear output buffer if request failed */
    Crypto_30_LibCv_ClearData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
  else
  {
    Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = persist; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
# if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
    /* Call the callout if the functionality is enabled and trigger that the key value is changed */
    Crypto_30_LibCv_KeyValueChangedCallout(job->cryptoKeyId);
# endif
    /* Call the callout if the functionality is enabled and trigger that the key is set to valid */
# if(CRYPTO_30_LIBCV_KEYVALIDITYSETCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
    Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(job->cryptoKeyId, TRUE);
# endif
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Generate_NistDrbgAes()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Generate_NistDrbgAes(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyStorageType additionalInputIndex;
  uint32 additionalInputLength = 0u;

  /* #10 Load additional input. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT, &additionalInputIndex, &additionalInputLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

  /* #20 Generate random number DRBG. */
  if ((localRetVal == E_OK) && (additionalInputLength > 0u))
  {
    retValCv = esl_getBytesCTRDRBG(
      wsDRBG,
      (eslt_Length) requestLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) requestBuffer,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorage(additionalInputIndex),
      (eslt_Length) additionalInputLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
  else if ((localRetVal == E_OK) || (localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE))
  {
    retValCv = esl_getBytesCTRDRBG(
      wsDRBG,
      (eslt_Length) requestLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) requestBuffer,
      NULL_PTR,
      0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
  else
  {
    retValCv = ESL_ERC_ERROR;
  }

  return retValCv;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Get_And_Set_NistDrbgAesState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv;
  eslt_DRBGReseedCounterType reseedCntBuf;
  eslt_DRBGSeedStatusType seedStatus;
  eslt_DRBGMode modeDRBG;
  eslt_Length seedLength = ESL_CTRDRBG_AES128_SEEDLEN;
  eslt_Length expectedSeedLength = ESL_CTRDRBG_AES128_SEEDLEN;

  if ((wsDRBG->wsDRBG.mode == ESL_DRBGMODE_CTRDRBG_AES256_NODF) || (wsDRBG->wsDRBG.mode == ESL_DRBGMODE_CTRDRBG_AES256_DF))
  {
    seedLength = ESL_CTRDRBG_AES256_SEEDLEN;
    expectedSeedLength = ESL_CTRDRBG_AES256_SEEDLEN;
  }

  /* #10 Get current DRBG state and save the seed state. */
  retValCv = esl_getStateCTRDRBG(
    wsDRBG,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
    (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &seedLength,
    (P2VAR(eslt_DRBGReseedCounterType, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &reseedCntBuf,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &seedStatus,
    (P2VAR(eslt_DRBGMode, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &modeDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

  retVal = Crypto_30_LibCv_Local_Set_NistDrbgState(
     seedStateElementIndex,
     seedCounterElementIndex,
     retValCv,
     seedLength,
     expectedSeedLength,
     reseedCntBuf,
     seedStatus); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  eslt_DRBGMode modeDRBG)
{
  /* #10 Initialize workspace */
  eslt_WorkSpaceCTRDRBG wsDRBG;

  /* Call Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws */
  return Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws( /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
    cryptoKeyId,
    seedStateElementIndex,
    entropyPtr,
    entropyLength,
    writeBlock,
    modeDRBG,
    &wsDRBG);
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  eslt_DRBGMode modeDRBG,
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Deny service for write once keys. */
  if (Crypto_30_LibCv_Local_Random_Check_For_Write_Once(cryptoKeyId, seedStateElementIndex, &reseedCtElementIndex) != TRUE) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* retVal is already set */
  }
  /* #10 seed with new seed value */
  else if (Crypto_30_LibCv_AesCtrDrbgSeed_With_Ws(cryptoKeyId, seedStateElementIndex, reseedCtElementIndex, entropyPtr, entropyLength, modeDRBG, wsDRBG) == E_OK) /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
  {
    /* The elementIndex do not need to be checked. The Code is only reachable if the key element is available. (Crypto_30_LibCv_Local_KeyElementSearch) */
    Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    *writeBlock = Crypto_30_LibCv_SetKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    retVal = E_OK;
  }
  else
  {
    *writeBlock = FALSE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength(
  uint32 cryptoKeyId,
  P2VAR(eslt_DRBGMode, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) modeDRBG,
  P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) seedLength) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyElementsType algorithmElementIndex = 0u;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Search algorithm index */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_ALGORITHM, &algorithmElementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* #20 Set corrrect mode and seed length */
    if (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(algorithmElementIndex)) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128)
    {
      *modeDRBG = ESL_DRBGMODE_CTRDRBG_AES128_NODF; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *seedLength = ESL_CTRDRBG_AES128_SEEDLEN; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
    else if (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(algorithmElementIndex)) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128_DF)
    {
      *modeDRBG = ESL_DRBGMODE_CTRDRBG_AES128_DF; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *seedLength = ESL_CTRDRBG_AES128_SEEDLEN; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
    else if (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(algorithmElementIndex)) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256)
    {
      *modeDRBG = ESL_DRBGMODE_CTRDRBG_AES256_NODF; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *seedLength = ESL_CTRDRBG_AES256_SEEDLEN; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
    else if (Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(algorithmElementIndex)) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256_DF)
    {
      *modeDRBG = ESL_DRBGMODE_CTRDRBG_AES256_DF; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *seedLength = ESL_CTRDRBG_AES256_SEEDLEN; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
    else
    {
      /* Return value already set */
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_DRBGAES == STD_ON) */

#if (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock)
{
  /* #10 Initialize workspace */
  eslt_WorkSpaceHASHDRBG wsDRBG;

  /* #20 Call Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws */
  return Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws( /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
    cryptoKeyId,
    seedStateElementIndex,
    entropyPtr,
    entropyLength,
    writeBlock,
    &wsDRBG);
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) writeBlock,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Deny service for write once keys. */
  if (Crypto_30_LibCv_Local_Random_Check_For_Write_Once(cryptoKeyId, seedStateElementIndex, &reseedCtElementIndex) != TRUE) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* retVal is already set */
  }
  /* #20 seed with new seed value. */
  else if (Crypto_30_LibCv_HashDrbgSeed_With_Ws(cryptoKeyId, seedStateElementIndex, reseedCtElementIndex, entropyPtr, entropyLength, wsDRBG) == E_OK) /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
  {
    /* The elementIndex do not need to be checked. The Code is only reachable if the key element is available. (Crypto_30_LibCv_Local_KeyElementSearch) */
    Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALUE_USED_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
    *writeBlock = Crypto_30_LibCv_SetKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    retVal = E_OK;
  }
  else
  {
    *writeBlock = FALSE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_HashDrbgSeed_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgSeed_With_Ws(
  uint32 cryptoKeyId, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType reseedCtElementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv;

  /* ----- Implementation ------------------------------------------------- */
  /* #1 Initialize workspace header. */
  retValCv = esl_initWorkSpaceHeader(&(wsDRBG->header), ESL_MAXSIZEOF_WS_HASHDRBG, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
  retValCv |= esl_initHASHDRBG(wsDRBG, ESL_DRBGMODE_HASHDRBG_SHA512); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    /* #5 already initiated / Reseeding */
    if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
    {
      retVal = Crypto_30_LibCv_HashDrbgReseed(cryptoKeyId, wsDRBG, entropyPtr, entropyLength); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
    }
    /* #10 need to be initiated */
    else
    {
      retVal = Crypto_30_LibCv_HashDrbgSeedFirst(cryptoKeyId, wsDRBG, entropyPtr, entropyLength); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
    }

    if (retVal == E_OK)
    {
      /* #20 Get current DRBG state and save the seed status */
      retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState(
        seedStateElementIndex,
        reseedCtElementIndex,
        wsDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
  }

  if ((retValCv != E_OK) || (retVal != E_OK))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    Crypto_30_LibCv_ClearKeyElementStateByMask(reseedCtElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
# if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
      /* Call the callout if the functionality is enabled and trigger that the key value is changed */
      /* Key validity changed callout is called in Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws */
      Crypto_30_LibCv_KeyValueChangedCallout(cryptoKeyId);
# endif
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_HashDrbgSeedFirst()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgSeedFirst(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyStorageType personalizationStrIndex, nonceIndex = 0u;
  uint32 personalizationStrLength, nonceLength = 0u;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Get Nonce index */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_NONCE, &nonceIndex, &nonceLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRetVal == E_OK)
  {
    /* #15 Get Private String (recommended feature) */
    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_PERSONALIZATION_STRING, &personalizationStrIndex, &personalizationStrLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if ((localRetVal == E_OK) && (personalizationStrLength > 0u))
    {
      /* #20 seed with personal string via esl_seedHASHDRBG */
      retValCv = esl_seedHASHDRBG(wsDRBG,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))entropyPtr,
        (eslt_Length)entropyLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(nonceIndex),
        (eslt_Length)nonceLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(personalizationStrIndex),
        (eslt_Length)personalizationStrLength,
        NULL_PTR,
        0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else if ((localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (localRetVal == E_OK))
    {
      /* #25 Personal string in not available in the key so seed without personal string. */
      retValCv = esl_seedHASHDRBG(wsDRBG,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))entropyPtr,
        (eslt_Length)entropyLength,
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(nonceIndex),
        (eslt_Length)nonceLength,
        NULL_PTR,
        0u,
        NULL_PTR,
        0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else
    {
      /* request failed */
    }

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_HashDrbgReseed()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_HashDrbgReseed(
  uint32 cryptoKeyId,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) wsDRBG,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal = E_NOT_OK;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyStorageType additionalInputIndex = 0u, seedIndex, reseedCounter;
  uint32 seedLength = CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN;
  uint32 reseedCntLength = CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN;
  uint32 reseedCnt, additionalInputLength = 0u;

  /* ----- Implementation ------------------------------------------------- */

  if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &seedIndex, &seedLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  {
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, &reseedCounter, &reseedCntLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    {
      /* #1 Reseed: restore seed state */
      Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCnt, Crypto_30_LibCv_GetAddrKeyStorage(reseedCounter)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      retValCv = esl_restoreStateHASHDRBG(wsDRBG,
                                         (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(seedIndex),
                                         (eslt_Length)seedLength,
                                         reseedCnt,
                                         (eslt_Byte)ESL_DRBG_SEED_STATUS_SEEDED,
                                         (eslt_DRBGMode)ESL_DRBGMODE_HASHDRBG_SHA512); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #5 check if additional input is available. (optional feature) */
        localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT, &additionalInputIndex, &additionalInputLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      }
    }

    if ((localRetVal == E_OK) && (additionalInputLength > 0u))
    {
      /* #10 reseed via esl_seedHASHDRBG */
      retValCv = esl_seedHASHDRBG(wsDRBG,
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))entropyPtr,
                                 (eslt_Length)entropyLength,
                                 NULL_PTR,
                                 0u,
                                 NULL_PTR,
                                 0u,
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(additionalInputIndex),
                                 (eslt_Length)additionalInputLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else if ((localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE) || (localRetVal == E_OK))
    {
      retValCv = esl_seedHASHDRBG(wsDRBG,
                                 (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))entropyPtr,
                                 (eslt_Length)entropyLength,
                                 NULL_PTR,
                                 0u,
                                 NULL_PTR,
                                 0u,
                                 NULL_PTR,
                                 0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
    }
    else
    {
      /* request failed */
      retValCv = ESL_ERC_ERROR;
    }
  }

  if (retValCv == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv;
  uint32 reseedCntBuf;
  eslt_Length seedLength = CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN;
  eslt_DRBGSeedStatusType seedStatus;
  eslt_DRBGMode modeDRBG;

  /* #10 Get current DRBG state and save the seed state. */
  retValCv = esl_getStateHASHDRBG(wsDRBG,
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
    &seedLength,
    &reseedCntBuf,
    &seedStatus,
    &modeDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

  retVal = Crypto_30_LibCv_Local_Set_NistDrbgState(
     seedStateElementIndex,
     seedCounterElementIndex,
     retValCv,
     seedLength,
     CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN,
     reseedCntBuf,
     seedStatus); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgHashFinish()
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
/* PRQA S 6050 3 */ /* MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgHashFinish(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR;
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex, seedCounterElementIndex;
  uint32 reseedCntBuf;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];
  boolean persist = FALSE;
  eslt_DRBGSeedStatusType seedStatus;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 load workspace */
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfDRBGHashSha512(Crypto_30_LibCv_GetDRBGHashSha512IdxOfObjectInfo(objectId));

  /* #20 Load seed state, seed counter and check key data. */
  localRetVal = Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare(workspace, job->cryptoKeyId, keyElements, &seedStateElementIndex, &seedCounterElementIndex, &persist); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if (localRetVal == E_OK)
  {
    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCntBuf, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedCounterElementIndex)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
    {
      seedStatus = ESL_DRBG_SEED_STATUS_SEEDED;
    }
    else
    {
      seedStatus = ESL_DRBG_SEED_STATUS_UNSEEDED;
    }

    /* #30 Set state to current seed state. */
    retValCv = esl_restoreStateHASHDRBG(workspace,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
      (eslt_Length)CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN,
      (eslt_DRBGReseedCounterType)reseedCntBuf,
      seedStatus,
      ESL_DRBGMODE_HASHDRBG_SHA512); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

    if (retValCv == ESL_ERC_NO_ERROR)
    {
      /* #31 generate random number via esl_getBytesHASHDRBG. */
      retValCv = Crypto_30_LibCv_Local_Generate_NistDrbgHash(
        job->cryptoKeyId,
        job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
        *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
        workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* #32 Set current state. */
        retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState(
          seedStateElementIndex,
          seedCounterElementIndex,
          workspace); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }
    }
  }
  else
  {
    retVal = localRetVal;
  }

  if (retValCv == ESL_ERC_ENTROPY_EXHAUSTION)
  {
    retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
  }

  if (retVal != E_OK)
  {
    /* clear output buffer if request failed */
    Crypto_30_LibCv_ClearData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
  else
  {
    Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = persist; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
# if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
    /* Call the callout if the functionality is enabled and trigger that the key value is changed */
    Crypto_30_LibCv_KeyValueChangedCallout(job->cryptoKeyId);
# endif
    /* Call the callout if the functionality is enabled and trigger that the key is set to valid */
# if(CRYPTO_30_LIBCV_KEYVALIDITYSETCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
    Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(job->cryptoKeyId, TRUE);
# endif
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare(
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ws, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedStateElementIndexPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) seedCounterElementIndexPtr,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* # load seed state and reseed counter */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 0u, CRYPTO_KE_RANDOM_SEED_STATE, CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(keyElements, 1u, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  retVal = Crypto_30_LibCv_Local_GetElementsIndexJob(cryptoKeyId, keyElements, 2u, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    /* Get index of keyElement SEED_STATE and CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER */
    (void)Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, seedStateElementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    (void)Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, seedCounterElementIndexPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  /* # Initialize workspace header */
    if (esl_initWorkSpaceHeader(&(ws->header), ESL_MAXSIZEOF_WS_HASHDRBG, CRYPTO_30_LIBCV_WATCHDOG_PTR) != ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
    {
      retVal = E_NOT_OK;
    }
    else
    {
      /* # Initialize workspace */
      if (esl_initHASHDRBG(ws, ESL_DRBGMODE_HASHDRBG_SHA512) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
      {
        Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist(*seedStateElementIndexPtr, *seedCounterElementIndexPtr, persist); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else if (retVal == CRYPTO_E_KEY_NOT_VALID)
  {
    retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
  }
  else
  {
    /* keep retVal */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Generate_NistDrbgHash()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Generate_NistDrbgHash(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsDRBG) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType localRetVal;
  eslt_ErrorCode retValCv = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyStorageType additionalInputIndex;
  uint32 additionalInputLength = 0u;

  /* #10 Load additional input. */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_ADDITIONAL_INPUT, &additionalInputIndex, &additionalInputLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

  /* #20 Generate random number DRBG. */
  if ((localRetVal == E_OK) && (additionalInputLength > 0u))
  {
    retValCv = esl_getBytesHASHDRBG(wsDRBG,
      (eslt_Length)requestLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))requestBuffer,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(additionalInputIndex),
      (eslt_Length)additionalInputLength); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
  else if ((localRetVal == E_OK) || (localRetVal == CRYPTO_E_KEY_NOT_AVAILABLE))
  {
    retValCv = esl_getBytesHASHDRBG(wsDRBG,
      (eslt_Length)requestLength,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) requestBuffer,
      NULL_PTR,
      0u); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
  }
  else
  {
    retValCv = ESL_ERC_ERROR;
  }

  return retValCv;
}

# if (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetRandomNistDrbgHash()
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
 *
 */
/* PRQA S 6010, 6050 5 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetRandomNistDrbgHash(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) requestBuffer,
  uint32 requestLength,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ------------------------------------------------ */
  eslt_WorkSpaceHASHDRBG wsDRBG;
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode retValCv = ESL_ERC_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex = 0u, seedCounterElementIndex = 0u;
  Crypto_30_LibCv_DefaultRandomMaxRetriesOfGeneralType retry;
  uint16 localCount;
  Crypto_30_LibCv_KeyElementGetType keyElements[2];
  uint32 reseedCntBuf;
  eslt_DRBGSeedStatusType seedStatus;

  /* ----- Implementation ------------------------------------------------- */

  /* # Prepare DRBG and check key data. */
  retVal = Crypto_30_LibCv_DispatchRandomNistDrbgHashGeneratePrepare(&wsDRBG, cryptoKeyId, keyElements, &seedStateElementIndex, &seedCounterElementIndex, persist); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    /* # Calculate random number until calculation is valid or max reties exhausted. */
    for (retry = 0u; retry < CRYPTO_30_LIBCV_DEFAULT_RANDOM_MAX_RETRIES; retry++)
    {
      /* # Copy counter and RNG paramter to local buffers. */
      localCount = Crypto_30_LibCv_RandomSourceGenerateCount;
      Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&reseedCntBuf, Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedCounterElementIndex)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      if (Crypto_30_LibCv_IsKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK))
      {
        seedStatus = ESL_DRBG_SEED_STATUS_SEEDED;
      }
      else
      {
        seedStatus = ESL_DRBG_SEED_STATUS_UNSEEDED;
      }

      retValCv = esl_restoreStateHASHDRBG(
        (P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) &wsDRBG, /* PRQA S 2982 */ /* MD_MSR_RetVal */
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedStateElementIndex),
        (eslt_Length)CRYPTO_30_LIBCV_RANDOM_HASH_INTERNAL_STATE_BUFFER_LEN,
        reseedCntBuf,
        seedStatus,
        ESL_DRBGMODE_HASHDRBG_SHA512); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      if (retValCv == ESL_ERC_NO_ERROR)
      {
        /* # Generate random number DRBG. */
        retValCv = Crypto_30_LibCv_Local_Generate_NistDrbgHash(
          cryptoKeyId,
          requestBuffer,
          requestLength,
          &wsDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
      }

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      /* # Check if rng counter has changed. */
      if ((Crypto_30_LibCv_IsRngCalculationValid(localCount)) && (retValCv == ESL_ERC_NO_ERROR))
      {
        retVal = Crypto_30_LibCv_Local_Get_And_Set_NistDrbgHashState(
          seedStateElementIndex,
          seedCounterElementIndex,
          &wsDRBG); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        /* break loop */
        break;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (retVal != E_OK)
    {
      /* # clear output buffer if request failed. */
      Crypto_30_LibCv_ClearData(requestBuffer, requestLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      *persist = FALSE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    }
  }

  if ((retValCv != ESL_ERC_NO_ERROR) || (retVal != E_OK))
  {
    if (retValCv == ESL_ERC_ENTROPY_EXHAUSTION)
    {
      retVal = CRYPTO_E_ENTROPY_EXHAUSTION;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_HASH == STD_ON) */
#endif /* (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON) */

#if((CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Random_Check_For_Write_Once()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Random_Check_For_Write_Once(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) reseedCtElementIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean retVal = FALSE;

  /* ----- Implementation ------------------------------------------------- */

  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER, reseedCtElementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* retVal is already set */
  }
  /* #10 Deny service for write once keys. */
  else if (Crypto_30_LibCv_IsKeyElementWriteOnce(*reseedCtElementIndex))
  {
    retVal = FALSE;
  }
  else if (Crypto_30_LibCv_IsKeyElementWriteOnce(seedStateElementIndex))
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Set_NistDrbgState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 9 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Set_NistDrbgState(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  eslt_ErrorCode retValCv,
  eslt_Length seedLength,
  eslt_Length expectedSeedLength,
  uint32 reseedCntBuf,
  eslt_DRBGSeedStatusType seedStatus) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if ((retValCv == ESL_ERC_NO_ERROR) && (seedLength == expectedSeedLength))
  {
    Crypto_30_LibCv_SetKeyElementWrittenLength(seedStateElementIndex, seedLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (seedStatus == ESL_DRBG_SEED_STATUS_UNSEEDED)
    {
      Crypto_30_LibCv_ClearKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    }
    else
    {
      Crypto_30_LibCv_SetKeyElementStateByMask(seedStateElementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_SEED_INIT_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    }

    /* #10 save reseed counter. */
    Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(seedCounterElementIndex), reseedCntBuf); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    Crypto_30_LibCv_SetKeyElementWrittenLength(seedCounterElementIndex, CRYPTO_30_LIBCV_RANDOM_RESEED_COUNTER_LEN);
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Check_Random_KeyElement_Persist(
  Crypto_30_LibCv_SizeOfKeyElementsType seedStateElementIndex,
  Crypto_30_LibCv_SizeOfKeyElementsType seedCounterElementIndex,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) persist) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Implementation ------------------------------------------------- */
# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
  if (Crypto_30_LibCv_IsKeyElementPersist(seedStateElementIndex) || Crypto_30_LibCv_IsKeyElementPersist(seedCounterElementIndex))
  {
    *persist = TRUE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
  else
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(seedStateElementIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(seedCounterElementIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif
  {
    *persist = FALSE; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
}

#endif /* ((CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON)) */

#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_IsRngCalculationValid()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_IsRngCalculationValid(uint16 localCount)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean validCalculation;

  /* ----- Implementation ------------------------------------------------- */
  /* Check if the Rng Counter is the expected value */
  if (localCount == Crypto_30_LibCv_RandomSourceGenerateCount)
  {
    validCalculation = TRUE;
    Crypto_30_LibCv_RandomSourceGenerateCount++;
  }
  else
  {
    validCalculation = FALSE;
  }

  return validCalculation;
}
#endif /* (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_KeyValueChangedCallout()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyValueChangedCallout(uint32 cryptoKeyId)
{
  /* ----- Implementation ------------------------------------------------- */
  /* Call key value changed callout */
  Crypto_30_LibCv_GetKeyValueChangedCalloutFctNameOfConfigurableCallouts()(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE);
  Crypto_30_LibCv_GetKeyValueChangedCalloutFctNameOfConfigurableCallouts()(cryptoKeyId, CRYPTO_KE_CUSTOM_RANDOM_RESEED_COUNTER);
}
#endif /* (CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_Fips186()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_Fips186(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Distinguish modes. */
  switch (mode)
  {
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = E_OK;
      break;
    }
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = E_OK;
      break;
    }

    /* #70 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchRandomFips186Finish(objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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
#endif /* (CRYPTO_30_LIBCV_FIPS186 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYSEEDFIPS186 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySeedFips186()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedFips186(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;
  boolean writeBlock = FALSE;
  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    if (Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* #10 Call Crypto_30_LibCv_Local_RandomSeed_Fips */
      retVal = Crypto_30_LibCv_Local_RandomSeed_Fips(job->cryptoKeyId, elementIndex, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, &writeBlock); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */

      /* #20 If key was derived successfully, mark it as valid */
      Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = writeBlock; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYSEEDFIPS186() */

#if (CRYPTO_30_LIBCV_DRBGAES == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_DRBGAES()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_DRBGAES(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = E_OK;
      break;
    }
    /* #15 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = E_OK;
      break;
    }

    /* #20 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchRandomNistDrbgAesFinish(objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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
#endif /* (CRYPTO_30_LIBCV_DRBGAES == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYSEEDDRBGAES == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySeedDRBGAES()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedDRBGAES(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;
  boolean writeBlock = FALSE;
  eslt_DRBGMode modeDRBG = 0u;
  eslt_Length seedLength = 0u;
  P2VAR(eslt_WorkSpaceCTRDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    workspace = Crypto_30_LibCv_GetWorkspaceOfKeySeedDRBGAES(Crypto_30_LibCv_GetKeySeedDRBGAESIdxOfObjectInfo(objectId));

    if (Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* #10 Set the correct DRBG mode */
      if (Crypto_30_LibCv_RandomNistDrbgAes_Set_Mode_And_SeedLength(job->cryptoKeyId, &modeDRBG, &seedLength) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        /* #20 Call Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws */
        retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes_With_Ws(job->cryptoKeyId, elementIndex, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, &writeBlock, modeDRBG, workspace); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */

        /* #30 If key was derived successfully, mark it as valid */
        Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = writeBlock; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYSEEDDRBGAES() */

#if (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Dispatch_DRBGHashSha512()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_DRBGHashSha512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #5 Distinguish modes. */
  switch (mode)
  {
    /* #10 Handle operationmode START. */
    case CRYPTO_OPERATIONMODE_START:
    {
      retVal = E_OK;
      break;
    }
    /* #15 Handle operationmode UPDATE. */
    case CRYPTO_OPERATIONMODE_UPDATE:
    {
      retVal = E_OK;
      break;
    }

    /* #20 Handle operationmode FINISH. */
    case CRYPTO_OPERATIONMODE_FINISH:
    {
      retVal = Crypto_30_LibCv_DispatchRandomNistDrbgHashFinish(objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
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
#endif /* (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON) */

#if (CRYPTO_30_LIBCV_KEYSEEDDRBGHASHSHA512 == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySeedDRBGHashSha512()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySeedDRBGHashSha512(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceHASHDRBG, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeySeedDRBGHashSha512(Crypto_30_LibCv_GetKeySeedDRBGHashSha512IdxOfObjectInfo(objectId));
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;
  boolean writeBlock = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    if (Crypto_30_LibCv_Local_KeyElementSearch(job->cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* #10 Call Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws */
      retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash_With_Ws(job->cryptoKeyId, elementIndex, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength, &writeBlock, workspace); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */

      /* #20 If key was derived successfully, mark it as valid */
      Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = writeBlock; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_KeySeedDRBGHashSha512() */

#if ((CRYPTO_30_LIBCV_FIPS186 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RandomSeed()
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
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex, algorithmElementIndex;
  uint32 seedAlgorithmLength = CRYPTO_30_LIBCV_RANDOM_SEED_SIZEOF_ALGORITHM;
  Crypto_30_LibCv_SizeOfKeyStorageType seedAlgorithmStorageIndex;
  boolean writeBlock = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Check if the seed state key element is available */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* #15 Lock write access for the CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #20 Get Algorithm from the algorithm key element */
      if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_ALGORITHM, &algorithmElementIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        if (Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(algorithmElementIndex, &seedAlgorithmStorageIndex, (P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&seedAlgorithmLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_OTHER) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        {
# if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
          /* #25 handle RNG_FIPS_186_2 */
          if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_FIPS_186_2_SHA1)
          {
            retVal = Crypto_30_LibCv_Local_RandomSeed_Fips(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
          }
          else
# endif
# if (CRYPTO_30_LIBCV_DRBGAES == STD_ON)
            /* #30 handle NIST 800-90A DRBG CTR AES128 */
            if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128)
            {
              retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock, ESL_DRBGMODE_CTRDRBG_AES128_NODF); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
            }
            else if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES128_DF)
            {
              retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock, ESL_DRBGMODE_CTRDRBG_AES128_DF); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
            }
            /* #35 handle NIST 800-90A DRBG CTR AES256 */
            else if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256)
            {
              retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock, ESL_DRBGMODE_CTRDRBG_AES256_NODF); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
            }
            else if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_CTR_DRBG_AES256_DF)
            {
              retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgAes(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock, ESL_DRBGMODE_CTRDRBG_AES256_DF); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
            }
            else
# endif
# if (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)
              /* #40 handle NIST 800-90A DRBG HASH SHA2-512 */
              if (Crypto_30_LibCv_GetKeyStorage(seedAlgorithmStorageIndex) == CRYPTO_30_LIBCV_RNG_NIST_800_90A_HASH_DRBG_SHA_512)
              {
                retVal = Crypto_30_LibCv_Local_RandomSeed_NistDrbgHash(cryptoKeyId, elementIndex, entropyPtr, entropyLength, &writeBlock); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
              }
              else
# endif
              {
                /* invalid random seed algorithm */
              }
        }
      }
      Crypto_30_LibCv_Local_KeyWriteLockRelease(cryptoKeyId);
# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
      if (writeBlock)
      {
        Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(cryptoKeyId));
      }
# else
      CRYPTO_30_LIBCV_DUMMY_STATEMENT(writeBlock); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

    }
  }
  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_FIPS186 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON)) */

#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
/**********************************************************************************************************************
 * esl_getBytesRNG()
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
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
FUNC(eslt_ErrorCode, CRYPTO_30_LIBCV_CODE) esl_getBytesRNG(
  const eslt_Length targetLength,
  P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) target)
{
  eslt_ErrorCode retVal = ESL_ERC_ERROR;
  Std_ReturnType rngResult = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  boolean triggerPersist = FALSE;

# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  if (target != NULL_PTR)
# endif
  {
# if(CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_AES == STD_ON)
    /* # Generate Drbg Aes Random Number */
    rngResult = Crypto_30_LibCv_GetRandomNistDrbgAes(Crypto_30_LibCv_GetDefaultRandomKey(), target, targetLength, &triggerPersist); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
# endif
# if(CRYPTO_30_LIBCV_RANDOM_SOURCE_DRBG_HASH == STD_ON)
    /* # Generate Drbg Hash Random Number */
    rngResult = Crypto_30_LibCv_GetRandomNistDrbgHash(Crypto_30_LibCv_GetDefaultRandomKey(), target, targetLength, &triggerPersist); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
# endif

    /* # Persist Key if necessary */
# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    if (triggerPersist)
    {
      Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(Crypto_30_LibCv_GetDefaultRandomKey()));
    }
# endif
  }

  if (rngResult == E_OK)
  {
    retVal = ESL_ERC_NO_ERROR;
# if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
      /* Call the callout if the functionality is enabled and trigger that the key value is changed */
      Crypto_30_LibCv_KeyValueChangedCallout(Crypto_30_LibCv_GetDefaultRandomKey());
# endif
      /* Call the callout if the functionality is enabled and trigger that the key is set to valid */
# if(CRYPTO_30_LIBCV_KEYVALIDITYSETCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
      Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(Crypto_30_LibCv_GetDefaultRandomKey(), TRUE);
# endif
  }
  else if (rngResult == CRYPTO_E_ENTROPY_EXHAUSTION)
  {
    errorId = CRYPTO_E_RE_ENTROPY_EXHAUSTED;
  }
  else
  {
    errorId = CRYPTO_E_RE_GET_BYTES_RNG_ERROR;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
    /* #90 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportRuntimeError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
      CRYPTO_30_LIBCV_INSTANCE_ID,
      CRYPTO_30_LIBCV_SID_ESL_GETBYTESRNG,
      errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Random.c
 *********************************************************************************************************************/

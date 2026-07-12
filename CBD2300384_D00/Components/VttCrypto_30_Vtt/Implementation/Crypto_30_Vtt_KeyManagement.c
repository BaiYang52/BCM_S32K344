/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_Vtt_KeyManagement.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define CRYPTO_30_VTT_KEYMANAGEMENT_SOURCE
/* PRQA S 0777, 0779, 3453 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_19.7 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_Vtt.h"
#include "Crypto_30_Vtt_Services.h"

#if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
# include "actBigNum.h"
#endif

#if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
# include "actIX25519.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if !defined (CRYPTO_30_VTT_LOCAL) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
# define CRYPTO_30_VTT_LOCAL static
#endif

#if !defined (CRYPTO_30_VTT_LOCAL_INLINE) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
# define CRYPTO_30_VTT_LOCAL_INLINE LOCAL_INLINE
#endif

/* PRQA S 3453 1 */ /*  MD_MSR_19.7 */
#define Crypto_30_Vtt_GetKeyElementWrittenLength(Index) ((uint32)(((uint32)Crypto_30_Vtt_GetKeyStorage(Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))) << 8u) | Crypto_30_Vtt_GetKeyStorage(Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))+1)))
#define CRYPTO_30_VTT_SIZEOF_SHE_UPDATE_KEY             (64u)
#define CRYPTO_30_VTT_SIZEOF_SHE_KEY                    (16u)
#define CRYPTO_30_VTT_SIZEOF_SHE_DATA_BUFFER            (48u)

#define CRYPTO_30_VTT_SIZEOF_SHE_M1_SIZE                (16u)
#define CRYPTO_30_VTT_SIZEOF_SHE_M2_SIZE                (32u)
#define CRYPTO_30_VTT_SIZEOF_SHE_M2_START_INDEX         (CRYPTO_30_VTT_SIZEOF_SHE_M1_SIZE)
#define CRYPTO_30_VTT_SIZEOF_SHE_M3_SIZE                (16u)
#define CRYPTO_30_VTT_SIZEOF_SHE_M3_START_INDEX         (CRYPTO_30_VTT_SIZEOF_SHE_M2_START_INDEX + CRYPTO_30_VTT_SIZEOF_SHE_M2_SIZE)

/* KDF */
#define CRYPTO_30_VTT_KEY_DERIVE_SIZEOF_ALGORITHM       (1u)

/* KDF ISO15118 */
#define CRYPTO_30_VTT_ISO15118_SIZEOF_IV               (16u)
#define CRYPTO_30_VTT_ISO15118_PRIV_KEY_START          (CRYPTO_30_VTT_ISO15118_SIZEOF_IV)
#define CRYPTO_30_VTT_ISO15118_SIZEOF_IV_AND_PRIV_KEY  (48u)
#define CRYPTO_30_VTT_ISO15118_SIZEOF_OTHER_INFO       (3u)

/* KDF NIST 800-56A */
#define CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE          (4u)
#define CRYPTO_30_VTT_NIST80056A_OTHER_INFO_MAXSIZE    (5u)

/* Nist 800-108 Nist/Fips 186.4 */
#define CRYPTO_30_VTT_SIZEOF_KDF_PRF_INPUT_BUFFER       (256u + 12u)
#define CRYPTO_30_VTT_SIZEOF_KDF_MAX_PARENT_KEY         (256u)
#define CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY         (256u)
#define CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8  (256u + 8u)
#define CRYPTO_30_VTT_SIZEOF_KDF_SALT_SYMMETRIC         (6u)
#define CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC        (38u)
#define CRYPTO_30_VTT_SIZEOF_KDF_PRIME                  (32u)
#define CRYPTO_30_VTT_SIZEOF_KDF_CONTEXT                (4u)
#define CRYPTO_30_VTT_SIZEOF_KDF_TARGET_KEY_LENGTH      (2u)
#define CRYPTO_30_VTT_SIZEOF_KDF_LABEL_LENGTH           (4u)
#define CRYPTO_30_VTT_SIZEOF_KDF_ALGORITHM_LENGTH       (1u)
#define CRYPTO_30_VTT_SIZEOF_KDF_PRIME_ASYM_EXTEND      (8u)

/* ECDHE */
#define CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY              (32u)

/* max size of ECC key -> corresponding to curve -> p256r1 = 32Byte */
#define CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE                 (CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)

#define CRYPTO_30_VTT_INDEXOF_SHE_M1_IDS                (15u)
#define CRYPTO_30_VTT_SHE_MASTER_KEY_ID                 (1u)

#define CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM     (1u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_VTT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         Copy key element
 *  \details       Copies a key element to another key element in the same crypto driver.
 *  \param[in]     elementIndexSrc              Holds the element index of the source key element. Has to be in bounds.
 *  \param[in]     elementIndexDst              Holds the element index of the destination key element. Has to be in bounds.
 *  \param[in]     cryptoKeyIdDst               Holds the identifier of the key.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_EXTRACT_DENIED  Request failed, not allowed to extract key material.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *  \pre           elementIndexSrc has to be a valid element storage index.
 *                 elementIndexDst has to be a valid element storage index.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementCopy(
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexDst,
  uint32 cryptoKeyIdDst);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SetKeyElementWrittenLength()
 *********************************************************************************************************************/
/*! \brief         Sets the written length of a key element.
 *  \details       Sets the written length of a key element in the key storage.
 *  \param[in]     keyElementId            Holds the identifier of the key element whose written length shall be set.
 *  \param[in]     keyElementLength        Written length of the key element
 *  \pre           keyElementId has to hold a valid identifier of the key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SetKeyElementWrittenLength(
  uint32 keyElementId,
  uint32 keyElementLength);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SetKeyState()
 *********************************************************************************************************************/
/*! \brief         Sets the state of a key.
 *  \details       The function sets the key state and respectively the key states of all associated key elements.
 *  \param[in]     cryptoKeyId           Holds the identifier of the key.
 *  \param[in]     mask                  State of the key, consistent of different flags.
 *  \pre           cryptoKeyId has to hold a valid identifier of a key
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SetKeyState(
  uint32 cryptoKeyId,
  uint8 mask);

/**********************************************************************************************************************
*  Crypto_30_Vtt_ClearKeyState()
*********************************************************************************************************************/
/*! \brief         Clears the state of a key.
 *  \details       The function clears the key state and respectively the key states of all associated key elements.
 *  \param[in]     cryptoKeyId           Holds the identifier of the key.
 *  \param[in]     mask                  State of the key, consistent of different flags.
 *  \pre           cryptoKeyId has to hold a valid identifier of a key
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_ClearKeyState(
  uint32 cryptoKeyId,
  uint8 mask);

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
  || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray()
*********************************************************************************************************************/
/*! \brief         Copy uint32 to c array.
 *  \details       The function copies data from a uint32 variable to a big endian c array with the given bit length.
 *  \param[in]     source           source value
 *  \param[out]    target           target Buffer
 *  \pre           target has to be a valid array with the length 4Byte / 32Bit.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray(
  uint32 source,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) target);
#endif

#if (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_CompareData_IsSmaller()
*********************************************************************************************************************/
/*! \brief         Check if the target value is smaller than the reference value
 *  \details       Compare data big-endian bytewise and check if the target value is smaller than the reference value.
 *  \param[in]     targetData     target value array
 *  \param[in]     referenceData  reference value array
 *  \param[in]     dataLength     compare length
 *  \pre           targetData has to be a valid array with the given dataLength.
 *  \pre           referenceData has to be a valid array with the given dataLength.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CompareData_IsSmaller(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) referenceData,
  uint32 dataLength);
#endif

#if ((CRYPTO_30_VTT_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementSet()
*********************************************************************************************************************/
/*! \brief         Sets a key element. This Api is only for internal usage.
 *  \details       Sets the given key element bytes to the key identified by cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength);
#endif

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementSetInternal()
*********************************************************************************************************************/
/*! \brief         Sets a key element. This Api is only for internal usage.
 *  \details       Sets the given key element bytes to the key identified by cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the index of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSetInternal(
  uint32 cryptoKeyId,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength,
  Crypto_30_Vtt_WriteOfKeyElementInfoType writeAccess);

#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
/**********************************************************************************************************************
*  Crypto_30_Vtt_KeyElementSetInternalVirtual()
*********************************************************************************************************************/
/*! \brief         Copies data to key storage for virtual key element.
 *  \details       Copies data of the keyPtr to the internal virtual key storage identified by the elementIndex.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[in]     keyPtr                  Holds the data which shall be copied to the key storage
 *  \param[in]     keyLength               Specifies the number of bytes which shall be copied.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 cryptoKeyId has to be a valid identifier to a keyElement in the keyStorage.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementSetInternalVirtual(
  uint32 cryptoKeyId,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength);
#endif

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementGet_Standard()
*********************************************************************************************************************/
/*!
 *
 * \brief       Gets content of the non-virtual key element.
 * \details     Gets content of the non-virtual key element from the key storage.
 * \param[in]   resultPtr        Pointer to the memory location where the data shall be stored.
 * \param[in]   resultLengthPtr  Pointer to a variable which contains the size of the available buffer.
 *                               After calling this function, the parameter contains the number of bytes written to the buffer.
 * \param[in]   elementIndex     Index of the Key element.
 * \return      E_OK
 *              E_NOT_OK
 *              CRYPTO_E_SMALL_BUFFER
 * \pre         elementIndex is a valid parameter
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet_Standard(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultLengthPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex);

#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementGet_Virtual()
*********************************************************************************************************************/
/*!
 *
 * \brief       Gets content of the virtual key element.
 * \details     Gets content of the virtual key element from the key storage.
 * \param[in]   cryptoKeyId      Holds the identifier of the key whose key element shall be set.
 * \param[in]   resultPtr        Pointer to the memory location where the data shall be stored.
 * \param[in]   resultLengthPtr  Pointer to a variable which contains the size of the available buffer.
 *                               After calling this function, the parameter contains the number of bytes written to the buffer.
 * \param[in]   elementIndex     Index of the Key element.
 * \return      E_OK
 *              E_NOT_OK
 *              CRYPTO_E_SMALL_BUFFER
 * \pre         elementIndex is a valid parameter
 *              cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet_Virtual(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultLengthPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex);
#endif

#if (CRYPTO_30_VTT_SUPPORT_SHE_KEY_UPDATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel()
 *********************************************************************************************************************/
/*! \brief         The function derives a key from given input data.
 *  \details       The function derives a key from given input data. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to input data from which the key is derived
 *  \param[in]     length                  Length of input data.
 *  \param[out]    outputPtr               Pointer to derived key
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes.
 *                 Length of the buffer provided by dataPtr has to be a multiple of 16.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateCmac()
 *********************************************************************************************************************/
/*! \brief         Generates Cmac based on given data and given key.
 *  \details       The function calculates the cmac based on the given data and the key. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data from which the mac is calculated
 *  \param[in]     length                  Length of input data
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to derived key
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the provided dataPtr has to be a multiple of 16 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateCmac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) dataPtr,
  uint32 length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdate()
 *********************************************************************************************************************/
/*! \brief         Updates key element based on SHE key update mechanism.
 *  \details       Interprets the given key buffer as M1M2M3 of the SHE key update mechanism and extracts relevant
                   information for setting the key element. This is used during the SHE key updated protocol.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the identifier of the key element which shall be set
 *  \param[in]     keyPtr                  Holds the pointer to the key buffer which shall be used to update the key element
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as elementIndex must identify a valid key - key element pair
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateDecryptKey()
 *********************************************************************************************************************/
/*! \brief         Decrypts data based on given key.
 *  \details       The function decrypts the given data using the provided key. AES128 is used as underlying algorithm.
                   This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data which shall be decrypted
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the decrypted data will be copied to
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be 32 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the buffer provided by outputPtr has to be 32 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateDecryptKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateEncryptKey()
 *********************************************************************************************************************/
/*! \brief         Encrypts data based on given key.
 *  \details       The function encrypts the given data using the provided key. AES128 is used as underlying algorithm.
                   This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data which shall be encrypted
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the encrypted data will be copied to
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be 32 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the buffer provided by outputPtr has to be 32 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateEncryptKey(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
*  Crypto_30_Vtt_SheKeyUpdateCheckM1Ids()
*********************************************************************************************************************/
/*! \brief        Determines the authentication key element based on M1.
 *  \details       The function checks if the authentication key element should be the element itself
                   or the SHE master key. This is done by processing ID byte of M1.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     elementIndex            Index of the key element
 *  \param[out]    authElementIndex        Pointer to the Index of the authentication key element
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the key provided by keyPtr has to be 16 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateCheckM1Ids(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_Vtt_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) authElementIndex);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateVerifyM3()
 *********************************************************************************************************************/
/*! \brief         Verifies M3.
 *  \details       The function verifies the M3.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     AuthElementIndex        Index of the authenticcation key element
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *                 Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType AuthElementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateExtractKey()
 *********************************************************************************************************************/
/*! \brief         Extracts new Key.
 *  \details       The function provides the new key.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     elementIndex            Index of the key element.
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *                 Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateExtractKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_SheKeyUpdateProofM4M5()
 *********************************************************************************************************************/
/*! \brief         Calculates M4 and M5
 *  \details       The function calculate the proof elements M4 and M5.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[in]     outputElement           Index of the destination key element
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateProofM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength, P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

#endif

#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcPubVal()
*********************************************************************************************************************/
/*! \brief         Dispatches the public value calculation.
 *  \details       Dispatches the public value calculation dependent on the given algorithm.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr has to be a valid pointer.
 *                 publicValueLengthPtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr);

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_X25519()
*********************************************************************************************************************/
/*! \brief         Calculation of the public value for x25519
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed by the public value pointer. The write access right for the destination
 *                 key element must be less or equal than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr has to be a valid pointer.
 *                 publicValueLengthPtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_X25519(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr);
# endif

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_ANSI()
*********************************************************************************************************************/
/*! \brief         Calculation of the public value for ANSI X9.63 with curve P256R1
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed by the public value pointer. The write access right for the destination
 *                 key element must be less or equal than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr has to be a valid pointer.
 *                 publicValueLengthPtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_ANSI(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr);
# endif
#endif

#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcSecret()
*********************************************************************************************************************/
/*! \brief         Dispatch the shared secret calculation.
 *  \details       Dispatch the shared secret calculation depending on the given algorithm.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the length of th partner public value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcSecret_X25519()
*********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less or equal than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPublicValuePtr   Holds the pointer to the memory location which contains the partners
 *                                         public value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer with size ESL_SIZEOF_X25519_PUBLIC_KEY.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret_X25519(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr);
# endif

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyExchangeCalcSecret_ANSI()
*********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less or equal than WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the Length of the partnerPublicValuePtr.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *                 CRYPTO_E_BUSY            Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER    Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer with size 2*CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY,.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret_ANSI(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);
# endif
#endif

#if (CRYPTO_30_VTT_KEY_DERIVE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive()
*********************************************************************************************************************/
/*! \brief        Derives a key
 *  \details      Dispatch the key derivation.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif

#if ((CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_Nist800108NistFips1864()
*********************************************************************************************************************/
/*! \brief        Derive Key for Nist 800-108 Nist/Fips 186.4
 *  \details      Dispatch and derive the key according to Nist 800-108 Nist/Fips 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           The required key is busy.
 *  \pre          cryptoKeyId has to be a valid crypto key.
 *                targetCryptoKeyId has to be a valid crypto key.
 *                deriveAlgorithm has to be a valid (CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256
 *                or CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB).
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-132407, CREQ-145592
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist800108NistFips1864(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_Kdf_HandleInputParams()
 *********************************************************************************************************************/
/*! \brief        Gets and checks input paremeters of the KDF
 *  \details      Gets and checks input paremeters of the KDF according to Nist 800-108 Nist/Fips 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \param[out]   parentKeyLength         Length of the parent key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           The required key is busy.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_VTT_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) parentKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt()
 *********************************************************************************************************************/
/*! \brief        Gets and checks the salt input parameter of the KDF
 *  \details      Gets and checks the salt input parameter of the KDF according to Nist 800-108 Nist/Fips 186-4.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[out]   prfInputBuffer          Input for the PRF
 *  \param[out]   prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by prfInputBuffer has to be CRYPTO_30_VTT_SIZEOF_KDF_PRF_INPUT_BUFFER Bytes.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt(CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
 *********************************************************************************************************************/
/*! \brief        Gets and checks the target key length from the salt input parameter of the KDF
 *  \details      Gets and checks the target key length from the salt input parameter of the KDF according to
 *                Nist 800-108 Nist/Fips 186-4.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \param[in]    deriveAlgorithm         The algorithm from the Algorithm key element
 *  \param[in]    saltBuffer              The salt from the Salt key element (needs to be defined in the calling function because primeMinus1PtrPtr points into that buffer)
 *  \param[out]   targetKeyLength         Length of the target key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by deriveAlgorithm has to be 1 Byte.
 *                Length of the buffer provided by saltBuffer has to be CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC Bytes.
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_Kdf_CallPRF()
 *********************************************************************************************************************/
/*! \brief        Calls the PRF function of the KDF
 *  \details      Calls the PRF function of the KDF according to Nist 800-108 Nist/Fips 186-4.
 *  \param[in]    prfInputBuffer          Input for the PRF
 *  \param[in]    prfInputBufferLength    Written Length of the PRF intput
 *  \param[out]   targetKey               Buffer for the target key
 *  \param[in]    targetKeyLength         Length of the target key
 *  \param[in]    parentKeyLength         Length of the parent key
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by targetKey has to be CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_CallPRF(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  CONST(uint32, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength, CONST(uint32, AUTOMATIC) parentKeyLength);
#endif /* #if ((CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)) */

#if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_Local_Kdf_AsymDevideByPrimeMinusOne()
 *********************************************************************************************************************/
/*! \brief        Divides the given target key by the given prime-1 and adds one to the result
 *  \details      Divides the given target key by the given prime-1 and adds one to the result according to Nist/Fips 186-4.
 *  \param[out]   targetKey               Buffer for the target key
 *  \param[in]    targetKeyLength         Length of the target key
 *  \param[in]    primeMinus1Ptr       Pointer to the position of the prime minus-1 data within the salt
 *  \param[in]    primeMinus1Length       Length of the prime-1
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *  \pre          Length of the buffer provided by targetKey has to be CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8 Bytes.
*********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_AsymDevideByPrimeMinusOne(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr,
  CONST(uint32, AUTOMATIC) primeMinus1Length);
#endif /* (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON) */

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
  || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
  || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_EcP_CalculateSharedSecret()
*********************************************************************************************************************/
/*! \brief          Calculate shared secret
 *  \details        Calculate shared secret for ECDHE Prime for the given values.
 *  \param[in]      privateKeyPtr           Pointer to the private key array.
 *  \param[in]      privateKeyLength        Length of the private key array.
 *  \param[in]      partnerPubKeyPtr        Pointer to the partner public key array.
 *  \param[in]      partnerPubKeyLength     Length of the partner public key array.
 *  \param[out]     sharedSecretPtr         Pointer to the shared secret.
 *  \return         E_OK                    Request successful.
 *                  E_NOT_OK                Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  partnerPubKeyPtr has to be a valid pointer with the length partnerPubKeyLength.
 *                  sharedSecretPtr has to be a valid pointer with the length 2*esl_getLengthOfEcPsecret_comp(EcDomainPtr).
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_EcP_CalculateSharedSecret(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr);
#endif

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
  || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_Nist80056A_SingleStepKdfHash()
*********************************************************************************************************************/
/*! \brief          Calculate single step KDF.
 *  \details        Calculate the single step kdf with HASH/Sha256 corresponding to NIST 800 56A.
 *  \param[in]      sharedSecretPtr       Pointer to the shared secret.
 *  \param[in]      sharedSecretLength    Length of the shared secret.
 *  \param[in]      otherInfoPtr          Pointer to the other-information.
 *  \param[in]      otherInfoLength       Length of the other-information.
 *  \param[out]     derivedKeyPtr         Pointer to the derived key.
 *  \param[in,out]  derivedKeyLengthPtr   Pointer to the length of the derived key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            sharedSecretPtr has to be a valid pointer with the length sharedSecretLength.
 *                  otherInfoPtr has to be a valid pointer with the length otherInfoLength.
 *                  derivedKeyPtr has to be a valid pointer with the length *derivedKeyLengthPtr.
 *                  derivedKeyLengthPtr has to be a valid length pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_SingleStepKdfHash(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint32 sharedSecretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S()
*********************************************************************************************************************/
/*! \brief          Calculate one pass c1e1s KDF.
 *  \details        Calculate the the one pass c1e1s corresponding to NIST 800 56A.
 *  \param[in]      privateKeyPtr         Pointer to the private key.
 *  \param[in]      privateKeyLength      Length of the private key.
 *  \param[in]      partnerPubKeyPtr      Pointer to the partner public key.
 *  \param[in]      partnerPubKeyLength   Length of the partner public key.
 *  \param[in]      otherInfoPtr          Pointer to the other-information.
 *  \param[in]      otherInfoPtrLength    Length of the other-information.
 *  \param[out]     derivedKeyPtr         Pointer to the derived key.
 *  \param[in,out]  derivedKeyLengthPtr   Pointer to the length of the derived key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  partnerPubKeyPtr has to be a valid pointer with the length partnerPubKeyLength.
 *                  otherInfoPtr has to be a valid pointer with the length otherInfoLength.
 *                  derivedKeyPtr has to be a valid pointer with the length *derivedKeyLengthPtr.
 *                  derivedKeyLengthPtr has to be a valid length pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoPtrLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr);
#endif

#if (CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey()
*********************************************************************************************************************/
/*! \brief        Calculate one pass c1e1s KDF.
*  \details       Calculate the the one pass c1e1s corresponding to NIST 800 56A and load all required key elements.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-145593
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif

#if (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_ISO15118_CheckKey()
*********************************************************************************************************************/
/*! \brief          Check key according to ISO15118.
 *  \details        Check if the key pair is valid according to ISO15118.
 *  \param[in]      privateKeyPtr         Pointer to the private key.
 *  \param[in]      privateKeyLength      Length of the private key.
 *  \param[in]      pubKeyPtr             Pointer to the public key.
 *  \param[in]      pubKeyLength          Length of the public key.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            privateKeyPtr has to be a valid pointer with the length privateKeyLength.
 *                  pubKeyPtr has to be a valid pointer with the length pubKeyLength.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118_CheckKey(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) pubKeyPtr,
  uint32 pubKeyLength);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_ISO15118_Aes128Decrypt()
*********************************************************************************************************************/
/*! \brief          Decrypt Aes128 data.
 *  \details        Decrypt Aes128 data corresponding to ISO15118(extract private key).
 *  \param[in]      ivPtr                   Pointer to the initial vector.
 *  \param[in]      keyPtr                  Pointer to the encryption key.
 *  \param[in]      encryptedMessagePtr     Pointer to the encrypted message.
 *  \param[in]      encryptedMessageLength  Length of the encrypted message.
 *  \param[out]     plainMessagePtr         Pointer to the plain message.
 *  \param[in,out]  plainMessageLengthPtr   Length of theplain message.
 *  \return         E_OK                  Request successful.
 *                  E_NOT_OK              Request failed.
 *  \pre            encryptedMessagePtr has to be a valid pointer with the length encryptedMessageLength.
 *                  plainMessagePtr has to be a valid pointer with the length plainMessageLengthPtr.
 *                  plainMessageLengthPtr has to be a length valid pointer.
 *                  ivPtr has to be a length valid pointer to a valid IV with the length CRYPTO_30_VTT_ISO15118_SIZEOF_IV.
 *                  keyPtr has to be a length valid pointer to a valid AES128 key with the length ESL_SIZEOF_AES128_KEY.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118_Aes128Decrypt(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) ivPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) encryptedMessagePtr,
  uint32 encryptedMessageLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) plainMessagePtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) plainMessageLengthPtr);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyDerive_ISO15118()
*********************************************************************************************************************/
/*! \brief        Derives a ISO15118 key.
 *  \details      Install and update the key according to ISO15118. Calculate the session key an decrypt the new private key.
 *  \param[in]    cryptoKeyId             Holds the identifier of the key which is used for key derivation.
 *  \param[in]    targetCryptoKeyId       Holds the identifier of the key which is used to store the derived key.
 *  \return       E_OK                    Request successful.
 *                E_NOT_OK                Request failed.
 *                CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre          cryptoKeyId is a valid key id.
 *                targetCryptoKeyId is a valid key id.
 *  \context      TASK
 *  \reentrant    TRUE, for different crypto keys
 *  \synchronous  TRUE
 *  \trace        CREQ-145594
 *********************************************************************************************************************/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Crypto_30_Vtt_SetKeyElementWrittenLength()
 **********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SetKeyElementWrittenLength(uint32 keyElementId, uint32 keyElementLength)
{

  /* #10 Set high- and low-byte for keyElement written length */
  Crypto_30_Vtt_SetKeyStorage(Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementId), (uint8)(keyElementLength >> 8)); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  Crypto_30_Vtt_SetKeyStorage(Crypto_30_Vtt_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementId) + 1, (uint8)keyElementLength); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_SetKeyState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SetKeyState(uint32 cryptoKeyId, uint8 mask)
{
  Crypto_30_Vtt_KeyElementsIterType elementIndex;

  /* #10 Set key state for all key elements */

  /* Loop over all elements of the source key */
  for (elementIndex = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    Crypto_30_Vtt_SetKeyElementStateByMask(elementIndex, mask); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}

/**********************************************************************************************************************
* Crypto_30_Vtt_ClearKeyState()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_ClearKeyState(uint32 cryptoKeyId, uint8 mask)
{
  Crypto_30_Vtt_KeyElementsIterType elementIndex;

  /* #10 Clear key state for all key elements */

  /* Loop over all elements of the source key */
  for (elementIndex = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    Crypto_30_Vtt_ClearKeyElementStateByMask(elementIndex, mask); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
 || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray()
*********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray(
  uint32 source,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) target)
{
  uint8_least byteIdx;

  /* Iterate over all bytes. Start at most significant byte. */
  for (byteIdx = sizeof(uint32); byteIdx > 0; byteIdx--)
  {
    /* #11 Calculate the corresponding byte by shifting the uint32 value and set the byte in the target buffer. */
    target[byteIdx-1] = (uint8)(source & 0xFFu); /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    source >>= 8;
  }
}
#endif /* ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)) */

#if (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_CompareData_IsSmaller()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CompareData_IsSmaller(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) referenceData,
  uint32 dataLength)
{
  uint32_least byteIdx;
  Std_ReturnType retVal = E_NOT_OK;
  /* #10 Copy data bytewise to the target buffer */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++)
  {
    if (targetData[byteIdx] < referenceData[byteIdx]) /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    {
      retVal = E_OK;
      break;
    }
  }
  return retVal;
}
#endif /* (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementCopy()
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
*/
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementCopy(
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexDst,
  uint32 cryptoKeyIdDst)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndexSrc;
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndexDst;
  uint32 i;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Check if the access flags are matching the conditions to copy a key element */
  /* Read != DENIED and Destination Protection >= Source Protection */
  if (((Crypto_30_Vtt_GetReadOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndexSrc))) == CRYPTO_30_VTT_RA_DENIED)
    || ((Crypto_30_Vtt_GetReadOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndexSrc))) > (Crypto_30_Vtt_GetReadOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndexDst)))))
  {
    retVal = CRYPTO_E_KEY_READ_FAIL;
  }
  /* WRITE <= Internal Copy */
  else if ((Crypto_30_Vtt_GetWriteOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndexDst))) == CRYPTO_30_VTT_WA_DENIED)
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
  }
  else
  {
    /* Are both key elements non-virtual ?*/
    /* #30 Handle not virtual key elements */
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALIDXOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    if ((Crypto_30_Vtt_IsKeyElementVirtual(elementIndexSrc) == FALSE)
      && (Crypto_30_Vtt_IsKeyElementVirtual(elementIndexDst) == FALSE))
#endif
    {
      /* Check if the available data is not bigger than the maximum size of the destination element */
      /* #35 Copy key element data if the size is matching */
      if ((Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc) == Crypto_30_Vtt_GetKeyElementLength(elementIndexDst))
        || ((Crypto_30_Vtt_IsKeyElementPartial(elementIndexDst) == TRUE)
        && (Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_Vtt_GetKeyElementLength(elementIndexDst))))
      {
        i = 0u;
        keyStorageIndexSrc = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc);
        keyStorageIndexDst = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);
        /* Copy the source element to the destination element */
        while (i < Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc)) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        {
          Crypto_30_Vtt_SetKeyStorage(keyStorageIndexDst, Crypto_30_Vtt_GetKeyStorage(keyStorageIndexSrc)); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

          keyStorageIndexSrc++;
          keyStorageIndexDst++;
          i++;
          CANoeAPI_ConsumeTicks(1);
        }
        /* store the written length in the destination element */
        Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndexDst, i);
        Crypto_30_Vtt_ClearKeyState(cryptoKeyIdDst, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
        retVal = E_OK;
      }
      else
      {
        /* The destination element is to short to store the available data of the source element */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    /* #40 Handle virtual key elements */
    /* #45 Both key elements are virtual */
    /* Are both elements virtual (This use-case should normally not happen) */
    else if ((Crypto_30_Vtt_IsKeyElementVirtual(elementIndexSrc) == TRUE)
      && (Crypto_30_Vtt_IsKeyElementVirtual(elementIndexDst) == TRUE))
    {
      /* #50 Check if the available data is not bigger than the maximum size of the destination element */
      if (Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_Vtt_GetKeyElementLength(elementIndexDst))
      {
        /* #55 Copy the data from the source element to the destination element */
        keyStorageIndexSrc = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc) + (Crypto_30_Vtt_KeyStorageIterType)Crypto_30_Vtt_GetKeyElementVirtualOffset(elementIndexSrc);
        keyStorageIndexDst = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexDst) + (Crypto_30_Vtt_KeyStorageIterType)Crypto_30_Vtt_GetKeyElementVirtualOffset(elementIndexSrc);
        for (i = 0u; i < Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc); i++)
        {
          Crypto_30_Vtt_SetKeyStorage(keyStorageIndexDst, Crypto_30_Vtt_GetKeyStorage(keyStorageIndexSrc));
          /* Increment indices */
          keyStorageIndexSrc++;
          keyStorageIndexDst++;
        }
        /* #60 store the written length in the destination element */
        Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndexDst, i);
        Crypto_30_Vtt_ClearKeyState(cryptoKeyIdDst, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
        retVal = E_OK;
      }
      else
      {
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
    /* #65 Source key element is virtual, destination key element is non-virtual */
    else if ((Crypto_30_Vtt_IsKeyElementVirtual(elementIndexSrc) == TRUE)
      && (Crypto_30_Vtt_IsKeyElementVirtual(elementIndexDst) == FALSE))
    {
      /* #70 Copy the data from the source element to the destination element */
      keyStorageIndexSrc = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc) + (Crypto_30_Vtt_KeyStorageIterType)Crypto_30_Vtt_GetKeyElementVirtualOffset(elementIndexSrc);
      keyStorageIndexDst = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);
      for (i = 0u; ((i < Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc)) && (Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndexDst))); i++)
      {
        Crypto_30_Vtt_SetKeyStorage(keyStorageIndexDst, Crypto_30_Vtt_GetKeyStorage(keyStorageIndexSrc));
        keyStorageIndexSrc++;
        keyStorageIndexDst++;
      }
      /* #75 store the written length in the destination element */
      Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndexDst, i);
      Crypto_30_Vtt_ClearKeyState(cryptoKeyIdDst, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
      retVal = E_OK;
    }
    /* #80 Source key element is non-virtual, destination key element is virtual */
    else
    {
      /* copy from non-virtual key element to virtual key element not possible */
      retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
    }
#endif
  }

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementGet_Standard()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet_Standard(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultLengthPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex)
{
  Std_ReturnType retVal;
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex;

  /* #20 Handle non-virtual key element. */
  /* Check if the buffer for the result is large enough.
  Depending on the actual length of the element the data is stored or an error is generated if the buffer is to small */

  /* #25 Copy Key element to outputbuffer if size is adequate */
  if (Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndex) <= *resultLengthPtr)
  {
    keyStorageIndex = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex);
    for (keyPtrIndex = 0u; keyPtrIndex < Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndex); keyPtrIndex++) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
    {
      resultPtr[keyPtrIndex] = Crypto_30_Vtt_GetKeyStorage(keyStorageIndex); /* SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK */
      keyStorageIndex++;
    }
    /* [SWS_Crypto_00092] */
    *resultLengthPtr = keyPtrIndex; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_WITH_DET_CHECK */
    retVal = E_OK;
  }
  else
  {
    /* [SWS_Crypto_00093] */
    retVal = CRYPTO_E_SMALL_BUFFER;
  }

  return retVal;
}

#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementGet_Virtual()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet_Virtual(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultLengthPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_Vtt_KeyElementsIterType virtualElementIndex;
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex;

  /* #35 Copy Key element to outputbuffer if size is adequate */
  /* Check if the buffer for the result is great enough.
  Depending on the partial access property of the element the data is stored or an error is generated if the buffer is to small */
  if ((Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndex) <= *resultLengthPtr)
    || ((Crypto_30_Vtt_GetKeyElementWrittenLength(elementIndex) > *resultLengthPtr)
    && (Crypto_30_Vtt_IsKeyElementPartial(elementIndex) == FALSE)))
  {
    /* store the handle for the virtual key element for later use */
    virtualElementIndex = elementIndex;

    /* Iterate over the whole key to find the root element of the virtual key element */
    for (elementIndex = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
    {
      /* Check if the element is the root of the virtual key element */
      if (Crypto_30_Vtt_GetKeyElementVirtualRoot(virtualElementIndex) == Crypto_30_Vtt_GetIdOfKeyElements(elementIndex))
      {
        /* Copy the data from the root element to the result buffer */
        keyStorageIndex = (Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex) + (Crypto_30_Vtt_KeyStorageIterType)Crypto_30_Vtt_GetKeyElementVirtualOffset(virtualElementIndex));
        for (keyPtrIndex = 0u; keyPtrIndex < Crypto_30_Vtt_GetKeyElementWrittenLength(virtualElementIndex); keyPtrIndex++)
        {
          /* write until buffer is full or no more data available */
          if (*resultLengthPtr > keyPtrIndex)
          {
            resultPtr[keyPtrIndex] = Crypto_30_Vtt_GetKeyStorage(keyStorageIndex);
          }
          keyStorageIndex++;
        }
        /* [SWS_Crypto_00092] */
        *resultLengthPtr = keyPtrIndex;
      }
      retVal = E_OK;
    }
  }
  else
  {
    /* [SWS_Crypto_00093] */
    retVal = CRYPTO_E_SMALL_BUFFER;
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (CRYPTO_30_VTT_SUPPORT_SHE_KEY_UPDATE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel()
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
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  /*
    AES-MP(x i): OUT_i  = ENC_(ECB,OUT_i-1) (x i)  XOR x_i  XOR OUT_i-1;  i > 0;   OUT_0  = 0;
  */
  eslt_WorkSpaceAES128Block workSpaceAes128Block;
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  uint8 Out[CRYPTO_30_VTT_SIZEOF_SHE_KEY];
  uint8 Out_last[CRYPTO_30_VTT_SIZEOF_SHE_KEY];
  uint8 tempCalcByte;
  boolean exitLoop = FALSE;
  eslt_ErrorCode eslRetVal;

  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) tempPlainPtr = dataPtr;

  /* #10 initialize all parameters */
  /* Init Out_i-1 (Out_last) */
  for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
  {
    Out[i] = 0u; /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
    Out_last[i] = 0u; /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
  }

  /* #15 initialize workspace */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block.header, /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
    ESL_MAXSIZEOF_WS_AES128,
    CRYPTO_30_VTT_WATCHDOG_PTR) == ESL_ERC_NO_ERROR)
  {
    /* #20 Calculate Block-Hash until data size is reached */
    while ((length > 0u) && (exitLoop == FALSE))
    {
      /* Set Key */
      /* #13 Set old Hash as round Key */
      eslRetVal = esl_initEncryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))Out_last); /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS */

      /* Encrypt */
      /* #14 Calculate new Hash */
      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        eslRetVal = esl_encryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, tempPlainPtr, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))Out); /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
      }

      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        /* #25 Exor new and old hash */
        for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
        {
          /* Calculate Out and Out_last */
          tempCalcByte = (uint8)(Out_last[i] ^ Out[i] ^ tempPlainPtr[i]);
          Out_last[i] = tempCalcByte; /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
          Out[i] = tempCalcByte; /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
        }
        /* Store new data pointer */
        tempPlainPtr = &tempPlainPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY];
        /* Update length */
        length -= CRYPTO_30_VTT_SIZEOF_SHE_KEY;
      }
      else
      {
        exitLoop = TRUE;
      }
      CANoeAPI_ConsumeTicks(1);
    }
  }

  /* #30 Save Result */
  if (length == 0u)
  {
    retVal = E_OK;

    for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
    {
      outputPtr[i] = Out[i]; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_SheKeyUpdateCmac()
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
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateCmac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) dataPtr,
  uint32 length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceCMACAES workSpaceCmacAes;
  uint32 tempLength = length;

  /* #10 Init work space */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceCmacAes.header, /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
    ESL_MAXSIZEOF_WS_CMACAES,
    CRYPTO_30_VTT_WATCHDOG_PTR) == ESL_ERC_NO_ERROR)
  {
    /* #15 Init CMAC */
    if (esl_initCMACAES128((P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceCmacAes, /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
      CRYPTO_30_VTT_CMACAES_MAX_KEY_SIZE,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))keyPtr) == ESL_ERC_NO_ERROR)
    {
      /* #20 Calculate Block-CMAC until data size is reached */
      while (tempLength > 0u)
      {
        /* #25 Update Calculation */
        if (esl_updateCMACAES128((P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceCmacAes, /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
          CRYPTO_30_VTT_CMACAES_MAC_SIZE,
          (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&dataPtr[length - tempLength]) == ESL_ERC_NO_ERROR)
        {
          tempLength -= CRYPTO_30_VTT_CMACAES_MAC_SIZE;
        }
        else
        {
          break;
        }
        CANoeAPI_ConsumeTicks(1);
      }
      if (tempLength == 0u)
      {
        /* Finalize */
        if (ESL_ERC_NO_ERROR == esl_finalizeCMACAES128((P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceCmacAes, /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_OUTPUT_PTR */
          outputPtr))
        {
          retVal = E_OK;
        }
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_SheKeyUpdateDecryptKey()
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
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateDecryptKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceAES128Block workSpaceAes128Block;
  uint8_least i;
  eslt_ErrorCode eslRet;

  /* #10 Init work space */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block.header, ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */

  /* #15 Init AES decryption */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initDecryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))keyPtr); /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
  }

  /* #20 Decrypt first and second data blocks */
  /* Decrypt first block */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_decryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))dataPtr, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))outputPtr); /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
  }
  /* Decrypt second block */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_decryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&dataPtr[CRYPTO_30_VTT_AES_BLOCK_SIZE], (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&outputPtr[CRYPTO_30_VTT_AES_BLOCK_SIZE]); /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 copy result to output buffer */
    for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
    {
      outputPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY + i] = (uint8)(outputPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY + i] ^ dataPtr[i]); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
    }
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_SheKeyUpdateEncryptKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateEncryptKey(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  P2CONST (uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceAES128Block workSpaceAes128Block;

  /* #10 Init work space */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&(workSpaceAes128Block.header), /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
                                                  ESL_MAXSIZEOF_WS_AES128,
                                                  CRYPTO_30_VTT_WATCHDOG_PTR) == ESL_ERC_NO_ERROR)
  {
    /* #15 Init AES encryption */
    if (esl_initEncryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))keyPtr) == ESL_ERC_NO_ERROR)
    {
      /* #20 encrypt data block */
      /* Encrypt block */
      if (esl_encryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workSpaceAes128Block, /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))dataPtr,
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))outputPtr) == ESL_ERC_NO_ERROR)
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_SheKeyUpdateCheckM1Ids()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateCheckM1Ids (
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_Vtt_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) authElementIndex)
{
  Std_ReturnType retVal;
  /* #5 Set authentication key element to itself if ids in M1 are similar */
  if ((keyPtr[CRYPTO_30_VTT_INDEXOF_SHE_M1_IDS] & 0x0Fu) == ((keyPtr[CRYPTO_30_VTT_INDEXOF_SHE_M1_IDS] >> 4) & 0x0Fu))
  {
    *authElementIndex = elementIndex; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
    retVal = E_OK;
  }
# if (CRYPTO_30_VTT_SHEMASTERKEYREFOFGENERAL == STD_ON)
  /* #10 Set authentication key element to the SHE master key if the id given in M1 matches the master key id. */
  else if ((keyPtr[CRYPTO_30_VTT_INDEXOF_SHE_M1_IDS] & 0x0Fu) == CRYPTO_30_VTT_SHE_MASTER_KEY_ID)
  {
    retVal = Crypto_30_Vtt_Local_KeyElementSearch(Crypto_30_Vtt_GetSheMasterKeyRefOfGeneral(), CRYPTO_KE_MAC_KEY, authElementIndex); /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
  }
# endif
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_SheKeyUpdateVerifyM3()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType AuthElementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_VTT_SIZEOF_SHE_KEY]; /* K2 */
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(AuthElementIndex);

  /* #10 Generate K2 */
  /* KDF AuthKey|Key_Update_Mac_C */
  for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
  {
    KDFbufferPtr[i] = Crypto_30_Vtt_GetKeyStorage(keyStorageIndex + i); /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
  }

  if (Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {

    /* #15 Calculate M3 / CMAC and verify */
    /* M3 proof:  M3* = outputBufferPtr */
    if (Crypto_30_Vtt_SheKeyUpdateCmac(keyPtr, CRYPTO_30_VTT_SIZEOF_SHE_M3_START_INDEX, key, outputBufferPtr) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
    {
      retVal = E_OK;
      /* Check (M3 = M3*) */
      for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_M3_SIZE; i++)
      {
        if (keyPtr[i + CRYPTO_30_VTT_SIZEOF_SHE_M3_START_INDEX] != outputBufferPtr[i])
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_SheKeyUpdateExtractKey()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateExtractKey(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_VTT_SIZEOF_SHE_KEY];

  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Generate K1 */
  /* Set Key_Update_Enc_C constant */
  KDFbufferPtr[17u] = 0x01u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */

  if (Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_M2_SIZE; i++)
    {
      outputBufferPtr[i] = 0u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    }
    /* #15 Decrypt C,K,F */
    if (Crypto_30_Vtt_SheKeyUpdateDecryptKey(&keyPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY], key, outputBufferPtr) == E_OK) /* SBSW_CRYPTO_30_VTT_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK */
    {
      /* #20 Store new Key and key length. */
      for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
      {
        Crypto_30_Vtt_SetKeyStorage(keyStorageIndex + i, outputBufferPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY + i]); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

        /*  KDF Key|Key_Update_Enc_C */
        KDFbufferPtr[i] = outputBufferPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY + i]; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
      }

      Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndex, CRYPTO_30_VTT_SIZEOF_SHE_KEY);
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_SheKeyUpdateProofM4M5()
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
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdateProofM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  Crypto_30_Vtt_SizeOfKeyElementsType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_VTT_SIZEOF_SHE_KEY]; /* Key3 and Key4 */
  uint8 dataBuffer[CRYPTO_30_VTT_SIZEOF_SHE_DATA_BUFFER];

  /* #10 Generate K3 */
  if (Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    /* #15 Generate M4* */
    outputBufferPtr[3] |= 0x08u; /* Add leading zero for padding */ /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    outputBufferPtr[3] &= 0xF8u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */

    for (i = 4u; i < 16u; i++)
    {
      /* Add padding */
      outputBufferPtr[i] = 0u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    }

    /* #20 Copy first block of M4 */
    for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_KEY; i++)
    {
      dataBuffer[i] = keyPtr[i]; /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_ACCESS_WITH_SIZE */
    }

    /* #25 Generate and store second block of M4 */
    if (Crypto_30_Vtt_SheKeyUpdateEncryptKey(outputBufferPtr, key, &dataBuffer[CRYPTO_30_VTT_SIZEOF_SHE_KEY]) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
    {
      /* Set Key_Update_Mac_C constant */
      KDFbufferPtr[CRYPTO_30_VTT_SIZEOF_SHE_KEY+1u] = 0x02u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */

      /* 30 Generate K4 */
      if (Crypto_30_Vtt_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_AND_KEY */
      { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        /* Generate M5 */
        retVal = Crypto_30_Vtt_SheKeyUpdateCmac(dataBuffer, 32u, key, &dataBuffer[32]); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

        /* #35 Copy M4 & M5 */
        for (i = 0u; i < CRYPTO_30_VTT_SIZEOF_SHE_DATA_BUFFER; i++)
        {
          Crypto_30_Vtt_SetKeyStorage(Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(outputElement) + i, dataBuffer[i]); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
        }

        /* #40 Store written length */
        Crypto_30_Vtt_SetKeyElementWrittenLength(outputElement, CRYPTO_30_VTT_SIZEOF_SHE_DATA_BUFFER); /* SBSW_CRYPTO_30_VTT_CSL01 */
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_SheKeyUpdate()
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
 */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr)
{
  /* #10 Init KDF Parameters */
  /* KDF Buffer */
  uint8 KDFbuffer[32] =
  {
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x01u, 0x02u, 0x53u, 0x48u, 0x45u, 0x00u, 0x80u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xB0u
  };

  /* #15 Init Workspace */
  /* Working Buffers */
  uint8 outputBuffer[32];

  Std_ReturnType retVal = E_NOT_OK;
  uint8 outputElementAvailable = FALSE;
  uint8 updateKey = TRUE;

  Crypto_30_Vtt_KeyElementsIterType outputElement;

  /* #20 Determine proof output slot */
  for (outputElement = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId);
    outputElement < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId);
    outputElement++)
  {
    if ((Crypto_30_Vtt_GetIdOfKeyElements(outputElement) == CRYPTO_KE_MAC_PROOF)
      || (Crypto_30_Vtt_GetIdOfKeyElements(outputElement) == CRYPTO_KE_CIPHER_PROOF))
    {
      if ((Crypto_30_Vtt_GetKeyStorageEndIdxOfKeyElements(outputElement) - Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(outputElement)) >= (uint16)48u)
      {
        outputElementAvailable = TRUE;
      }
      else
      {
        updateKey = FALSE;
      }
      break;
    }
  }

  if (updateKey == TRUE)
  {
    Crypto_30_Vtt_KeyElementsIterType authElementIndex;
    /*#21 Check key ID */
    if (Crypto_30_Vtt_SheKeyUpdateCheckM1Ids(keyPtr, elementIndex, &authElementIndex) == E_OK) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
    {
      /* #25 Verify M3 */
      if (Crypto_30_Vtt_SheKeyUpdateVerifyM3(keyPtr, authElementIndex, KDFbuffer, sizeof(KDFbuffer), outputBuffer) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
      {
        /* #30 Extract Key */
        retVal = Crypto_30_Vtt_SheKeyUpdateExtractKey(keyPtr, elementIndex, KDFbuffer, sizeof(KDFbuffer), outputBuffer); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */

        if (retVal == E_OK)
        {
          /* #35 Generate M4 & M5, if proof slot is available */
          if (outputElementAvailable == TRUE)
          {
            /* Set Key_Update_Enc_C constant */
            KDFbuffer[17u] = 0x01u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */

            retVal = Crypto_30_Vtt_SheKeyUpdateProofM4M5(keyPtr, outputElement, KDFbuffer, sizeof(KDFbuffer), outputBuffer); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
          }
        }
      }
    }
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* CRYPTO_30_VTT_SUPPORT_SHE_KEY_UPDATE == STD_ON */

#if ((CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON))
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt_TargetKeyLength()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength)
{
  Std_ReturnType retVal = E_OK;

  uint32 configuredTargetKeyLength = 0u;

  /* #1 Get configured target key length */
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;
  if (Crypto_30_Vtt_Local_KeyElementSearch(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
  {
    /* Get Configured Target Key Length */
    configuredTargetKeyLength = Crypto_30_Vtt_GetKeyElementLength(elementIndex);
  }
  else
  {
    retVal = E_NOT_OK;
  }

  /* #2 Compare the configured length against the target key length given in salt */
  /* The keyLength (k) in salt is given in bytes and Big Endian Format. This has to be converted to size in bits and little endian format later */
  (*targetKeyLength) = ((uint16)((((uint16)(saltBuffer[4])) << 8) | ((uint16)(saltBuffer[5] & 0x00FFu)))); /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

  /* Check valid target key length */
  if ((*targetKeyLength > CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY) || ((*targetKeyLength) > configuredTargetKeyLength))
  {
    retVal = E_NOT_OK;
  }

# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
#  if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
  /* #3 For Asym keys check that length is supported by used prime numbers */
  if (deriveAlgorithm == CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB)
#  else
  CRYPTO_30_VTT_DUMMY_STATEMENT_CONST(deriveAlgorithm); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#  endif
  {
    if ((*targetKeyLength) != CRYPTO_30_VTT_SIZEOF_KDF_PRIME)
    {
      retVal = E_NOT_OK;
    }

    /* #4 For Asymmetric key derivation target key has to be extended by 8 bytes, because it is divided by prime-1 in the second step */
    (*targetKeyLength) += CRYPTO_30_VTT_SIZEOF_KDF_PRIME_ASYM_EXTEND; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */
  }
# else
  CRYPTO_30_VTT_DUMMY_STATEMENT_CONST(deriveAlgorithm); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength)
{
  Std_ReturnType retVal;

  uint32 saltLength;

  /* #1 Load Salt / Context from the salt key element */
  /* 4 byte Context | 2 byte target Key Length in Big Endian (k) | 32 byte curve specific Prime-1 */
  /* For Asym keys the salt contains also the 32 byte prime-1 */
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
#  if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
  if (deriveAlgorithm == CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB)
#  endif
  {
    /* Asym key */
    saltLength = CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC;
  }
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
#  if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
  else
#  endif
  {
    /* sym key */
    saltLength = CRYPTO_30_VTT_SIZEOF_KDF_SALT_SYMMETRIC;
  }
# endif

  retVal = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_SALT, saltBuffer, &saltLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

  /* #2 Check expected salt length according to the algorithm */
  if (retVal == E_OK)
  {
# if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
#  if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    if (deriveAlgorithm == CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256)
#  endif
    {
      if (saltLength < CRYPTO_30_VTT_SIZEOF_KDF_SALT_SYMMETRIC)
      {
        retVal = E_NOT_OK;
      }
    }
#  if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    else
#  endif
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    {
      /* CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB */
      if (saltLength < CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC)
      {
        retVal = E_NOT_OK;
      }
    }
# endif
  }

  /* #4 Copy the 4 byte Context into the PRF input buffer */
  if (retVal == E_OK)
  {
    Crypto_30_Vtt_CopyData(&(prfInputBuffer[*prfInputBufferLength]), saltBuffer, CRYPTO_30_VTT_SIZEOF_KDF_CONTEXT); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    (*prfInputBufferLength) += CRYPTO_30_VTT_SIZEOF_KDF_CONTEXT; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt_TargetKeyLength(targetCryptoKeyId, deriveAlgorithm, saltBuffer, targetKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  }
  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_Kdf_HandleInputParams()
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
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_HandleInputParams(
  CONST(uint32, AUTOMATIC) cryptoKeyId,
  CONST(uint32, AUTOMATIC) targetCryptoKeyId,
  CONST(uint8, AUTOMATIC) deriveAlgorithm,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) saltBuffer,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) targetKeyLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) parentKeyLength)
{
  Std_ReturnType retVal;

  uint32 labelLength = CRYPTO_30_VTT_SIZEOF_KDF_LABEL_LENGTH;

  (*parentKeyLength) = CRYPTO_30_VTT_SIZEOF_KDF_MAX_PARENT_KEY; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

  /* #2 Get Parent Key from the password key element and write it to the PRF input buffer */

  retVal = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, prfInputBuffer, parentKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  (*prfInputBufferLength) += *parentKeyLength; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

  /* #3 Get Label from the Label key element and write it to the PRF input buffer */
  if (retVal == E_OK)
  {
    /* Skip one byte for iteration counter */
    (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

    retVal = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_LABEL, &(prfInputBuffer[*prfInputBufferLength]), &labelLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
    (*prfInputBufferLength) += labelLength; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */
  }

  /* #4 Add constant byte 0x00 to the PRF input buffer */
  prfInputBuffer[*prfInputBufferLength] = 0x00u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
  (*prfInputBufferLength)++; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

  /* #5 Get and check the Salt element */
  if (retVal == E_OK)
  {
    retVal = Crypto_30_Vtt_Local_Kdf_HandleInputParams_Salt(cryptoKeyId, targetCryptoKeyId, deriveAlgorithm, prfInputBuffer, prfInputBufferLength, saltBuffer, targetKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  }

  /* #6 Convert target key length to bits and little endian and copy it to the input buffer */
  if (retVal == E_OK)
  {
    /* Set k in bits and little endian (lo byte first) */
    prfInputBuffer[*prfInputBufferLength] = (uint8)((uint16)((*targetKeyLength << 3u)) & 0x00FFu); /* lo byte */ /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    prfInputBuffer[*prfInputBufferLength + 1] = (uint8)((uint16)((*targetKeyLength) >> 5u) & 0x00FFu); /* hi byte */ /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    (*prfInputBufferLength) += CRYPTO_30_VTT_SIZEOF_KDF_TARGET_KEY_LENGTH; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_Kdf_CallPRF()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_CallPRF(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) prfInputBuffer,
  CONST(uint32, AUTOMATIC) prfInputBufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength,
  CONST(uint32, AUTOMATIC) parentKeyLength)
{
  Std_ReturnType retVal = E_OK;
  uint32_least derivedLength;
  uint8 i = 1u;

  /* #1 Iterate over the target key length */
  for (derivedLength = 0u; derivedLength < targetKeyLength; derivedLength += ESL_SIZEOF_SHA256_DIGEST)
  {
    eslt_WorkSpaceSHA256 workspaceSHA256;
    uint8 hash[ESL_SIZEOF_SHA256_DIGEST] = { 0u };

    /* #11 Set the iteration counter i in the PRF input buffer. Inserted after parent key */
    prfInputBuffer[parentKeyLength] = i; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
    i++;

    /* #12 Call SHA-256 as PRF */
    retVal = E_NOT_OK;
    /* CRYPTO_OPERATIONMODE_START: */
    if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workspaceSHA256.header, ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_VTT_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
    {
      if (esl_initSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workspaceSHA256) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE */
      {
        /* CRYPTO_OPERATIONMODE_UPDATE: */
        if (esl_updateSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workspaceSHA256, (eslt_Length)prfInputBufferLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))prfInputBuffer) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
        {
          /* CRYPTO_OPERATIONMODE_FINISH: */
          if (esl_finalizeSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&workspaceSHA256, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))hash) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS */
          {
            retVal = E_OK;
          }
        }
      }
    }

    /* #13 Copy the PRF result to the end of the target key buffer */
    if (retVal == E_OK)
    {
      if ((derivedLength + ESL_SIZEOF_SHA256_DIGEST) <= targetKeyLength)
      {
        Crypto_30_Vtt_CopyData(&(targetKey[derivedLength]), hash, ESL_SIZEOF_SHA256_DIGEST); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
      else
      {
        Crypto_30_Vtt_CopyData(&(targetKey[derivedLength]), hash, (uint16)(targetKeyLength - derivedLength)); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
      }
    }
    else
    {
      break;
    }
    /* END Calculate Hash */
  }
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_Kdf_AsymDevideByPrimeMinusOne()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_Kdf_AsymDevideByPrimeMinusOne(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) targetKey,
  CONST(uint32, AUTOMATIC) targetKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr,
  CONST(uint32, AUTOMATIC) primeMinus1Length)
{
  Std_ReturnType retVal = E_OK;
  /* #1 Use the library function actBNReduce to divide big numbers */
  /*  r = a mod b
  **
  **  int actBNReduce(actBNDIGIT* a,
  **                  actBNLENGTH a_length,
  **                  actBNDIGIT* b,
  **                  actBNLENGTH b_length,
  **                  actBNDIGIT* r,
  **                  void (*watchdog) (void));
  **
  **   BigNum reduce (r = a mod b).
  **
  ** input:                                              (length in digits)
  ** - a:       first number (will be destroyed)              a_length+1
  ** - b:       second number (will be returned const         b_length
  **            but touched by the normalization)
  ** - r:       result buffer                                 b_length+1
  **
  ** output:
  ** - a:       the normalized result a mod b                 b_length
  ** - b:       second number                                 b_length
  ** - r:       the result a mod b                            b_length
  ** - returns: -1    if exact b_length <= 1 (error)
  **             0    else (success) */

#  if (actBN_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)targetKeyLength;
  actBNDIGIT b[256];
  actBNLENGTH b_length = (actBNLENGTH)primeMinus1Length;
  actBNDIGIT r[264];
#  elif (actBN_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264 >> 1]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)(targetKeyLength >> 1u);
  actBNDIGIT b[256 >> 1];
  actBNLENGTH b_length = (actBNLENGTH)(primeMinus1Length >> 1u);
  actBNDIGIT r[264 >> 1];
#  elif (actBN_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
  actBNDIGIT a[264 >> 2]; /* 256 + 8 */
  actBNLENGTH a_length = (actBNLENGTH)(targetKeyLength >> 2u);
  actBNDIGIT b[256 >> 2];
  actBNLENGTH b_length = (actBNLENGTH)(primeMinus1Length >> 2u);
  actBNDIGIT r[264 >> 2];
#  else
#   error "Value of actBN_BYTES_PER_DIGIT is not supported"
#  endif

  uint8 one = 1u;
  int res; /* PRQA S 5013 */ /* MD_CRYPTO_30_VTT_5013 */

  /* #11 Set the so far calculated target key as dividend */
  actBNSetOctetString(a, a_length, targetKey, (int)targetKeyLength); /* PRQA S 5013 */ /* MD_CRYPTO_30_VTT_5013 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #12 Set the prime-1 as divisor */
  actBNSetOctetString(b, b_length, primeMinus1Ptr, (int)primeMinus1Length); /* PRQA S 5013 */ /* MD_CRYPTO_30_VTT_5013 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

  /* #13 Call actBNReduce and store the value in targetKey */
  res = actBNReduce(a, a_length, b, b_length, r, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  if (res == 0) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
  {
    /* #2 Add one and return the result */
    actBNSetOctetString(a, a_length, &one, 1u); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    (void)actBNAdd(r, a, r, a_length); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    actBNOctetString(targetKey, (int)targetKeyLength, r, a_length); /* PRQA S 5013 */ /* MD_CRYPTO_30_VTT_5013 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}
# endif

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDeriveNist800108NistFips1864()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist800108NistFips1864(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId,
  uint8 deriveAlgorithm)
{
  Std_ReturnType retVal;
  uint32 targetKeyLength;

  uint32 parentKeyLength;
  uint32 prfInputBufferLength = 0u;
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
  uint16 primeMinus1Length;
# endif

  /* Input buffer for the Pseudo Random Function. */
  /* Buffer size is 256 byte maximum parent key length + 12 byte (1 byte iteration | 4 byte Label | 1 byte 0x00 | 6 byte of salt (2 byte TypeID | 2 byte training Counter | 2 byte target Key Length in littel endian (k))) */
  uint8 prfInputBuffer[CRYPTO_30_VTT_SIZEOF_KDF_PRF_INPUT_BUFFER];
  uint8 saltBuffer[CRYPTO_30_VTT_SIZEOF_KDF_SALT_ASYMMETRIC];
  uint8 targetKey[CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8]; /* 256+8 for asym KDF */
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) primeMinus1Ptr;
# endif

  Crypto_30_Vtt_ClearData(targetKey, CRYPTO_30_VTT_SIZEOF_KDF_MAX_TARGET_KEY_PLUS_8); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  /* #2 Handle input parameter */
  retVal = Crypto_30_Vtt_Local_Kdf_HandleInputParams(cryptoKeyId, targetCryptoKeyId, deriveAlgorithm, prfInputBuffer, &prfInputBufferLength, saltBuffer, &targetKeyLength, &parentKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  /* #3 Call PRF */
  if (retVal == E_OK)
  {
    retVal = Crypto_30_Vtt_Local_Kdf_CallPRF(prfInputBuffer, prfInputBufferLength, targetKey, targetKeyLength, parentKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  }

  /* #4 Handle the results of the PRF according to the algorithm */
  if (retVal == E_OK)
  {
# if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
#  if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    if (deriveAlgorithm == CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256)
#  endif
    {
      /* #41 Symmetric Key Derive finished here. Copy calculated key to key element */
      retVal = Crypto_30_Vtt_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_30_VTT_KE_TARGET_KEY, targetKey, targetKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    }
#  if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    else
#  endif
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    {
      /* CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB */
      /* #42 For Asymmetric Key Derive: */

      /* #421 Set the prime minus-1 pointer to the start address of the value within the salt buffer and set the data length */
      primeMinus1Ptr = &(saltBuffer[CRYPTO_30_VTT_SIZEOF_KDF_SALT_SYMMETRIC]); /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */
      primeMinus1Length = CRYPTO_30_VTT_SIZEOF_KDF_PRIME; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_INLINE_STACK */

      /* #424 Calculate private key for asymmetric keys only by dividing key by given prime-1 */
      retVal = Crypto_30_Vtt_Local_Kdf_AsymDevideByPrimeMinusOne(targetKey, targetKeyLength, primeMinus1Ptr, primeMinus1Length); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

      if (retVal == E_OK)
      {
        /* Skip the first 8 additional bytes */
        /* 428 Asymmetric Key Derive finished. Copy calculated key to key element */
        retVal = Crypto_30_Vtt_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_30_VTT_KE_TARGET_KEY, &(targetKey[8]), targetKeyLength - 8u); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
      }
    }
# endif
  }

  /* #5 If key was derived successfully, mark it as valid */
  if (retVal == E_OK)
  {
    Crypto_30_Vtt_SetKeyState(targetCryptoKeyId, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK);
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif /* ((CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)) */

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
  || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
  || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON))
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_EcP_CalculateSharedSecret()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_EcP_CalculateSharedSecret(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  eslt_WorkSpaceEcP localWsEcP;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_VTT_CONST) EcDomainPtr = Crypto_30_Vtt_EccCurveNistAnsiSecP256R1Domain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_VTT_CONST) EcDomainExtPtr = Crypto_30_Vtt_EccCurveNistAnsiSecP256R1DomainExt;

  /* ----- Implementation ----------------------------------------------- */
  /* #0 check key size */
  if (privateKeyLength != esl_getLengthOfEcPprivateKey(EcDomainPtr)) /* SBSW_CRYPTO_30_VTT_ESL_CONST_WORKSPACE */
  {
    /* invalid private key length */
  }
  else if (partnerPubKeyLength != (uint32)Crypto_30_Vtt_Math_Mul2(esl_getLengthOfEcPpublicKey_comp(EcDomainPtr))) /* SBSW_CRYPTO_30_VTT_ESL_CONST_WORKSPACE */
  {
    /* invalid partner public key length */
  }
  else
  {
    /* #1 init  EcP workspaces */
    eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP.header, ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      eslRet = esl_initGenerateSharedSecretDHEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP, EcDomainPtr, EcDomainExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE */
    }

    if (eslRet == ESL_ERC_NO_ERROR)
    {

      /* #2 Verify that the key is valid. */
      /* This is already implemented in actECPointIsOnCurve and is part of esl_generateSharedSecretDHEcP_prim */

      /* #3 generate shared Secret */
      eslRet = esl_generateSharedSecretDHEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP,/* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_VTT_0310, MD_CRYPTO_30_VTT_0311 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
                                                  (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))privateKeyPtr,
                                                  (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&partnerPubKeyPtr[0], (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&partnerPubKeyPtr[esl_getLengthOfEcPprivateKey(EcDomainPtr)], /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
                                                  (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&sharedSecretPtr[0], (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&sharedSecretPtr[esl_getLengthOfEcPsecret_comp(EcDomainPtr)]); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
      if (eslRet == ESL_ERC_NO_ERROR)
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
}
#endif

#if ((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) \
 || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_Nist80056A_SingleStepKdfHash()
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
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_SingleStepKdfHash(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint32 sharedSecretLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr)
{
  /* This check is not required while the sharedSecret size and hash digest size are equal but is required by IS15118 (marked with \fr)*/
  /* \fr uint32 repeatSteps; */
  /*\fr uint32 counter; */
  uint32 derivedLength = 0u;
  eslt_WorkSpaceSHA256 localWsSha256;
  uint8 concatinateBuf[CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE + CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE + CRYPTO_30_VTT_NIST80056A_OTHER_INFO_MAXSIZE];
  Std_ReturnType retVal = E_NOT_OK;
  /* #1 init workspace */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsSha256.header, ESL_MAXSIZEOF_WS_SHA256, CRYPTO_30_VTT_WATCHDOG_PTR) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
  {
    /* Single-step KDF Option 1 Hash */
    /* Implementation according to NIST SP 56A: Revision 2, May 2013 (5.8.1.1  The Single-Step KDF Specification) */

    /* #5 (Spec-Step: 1) calculate repeat Steps. */
    /* repeat Steps = KeyLength(bit)/HashLength(bit) */
    /* This check is not required while the sharedSecret size and hash digest size are equal */
    /* \fr repeatSteps = Crypto_30_Vtt_Byte2Bit(sharedSecretLength) / Crypto_30_Vtt_Byte2Bit(ESL_SIZEOF_SHA256_DIGEST);*/

    /* #10 (Spec-Step: 2) check max repeat Steps. */
    /* This check is not required while the sharedSecret size and hash digest size are equal */
    /* \fr if ((repeatSteps * ESL_SIZEOF_SHA256_DIGEST) <= *derivedKeyLengthPtr) */
    {

      /* #15 add shared secret to string. */
      /* (counter || Z || OtherInfo) */
      Crypto_30_Vtt_CopyData(&concatinateBuf[CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE], sharedSecretPtr, sharedSecretLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

      /* #15 add otherInformation to string. */
      /* OtherInfo = (AlgorithmID || PartyUInfo(Tranmitter) || PartyVInfo(Receiver)) */
      Crypto_30_Vtt_CopyData(&concatinateBuf[CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE + sharedSecretLength], otherInfoPtr, otherInfoLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */

      /* #20 (Spec-Step: 4) check max input length hash */
      /* not required */

      /* #25 (Spec-Step: 3) init Counter */
      /* create the concatenated shared secret (counter || Z || OtherInfo) */
      /* Initialize a 32-bit counter as 1 (big-endian) */
      /* #30 (Spec-Step: 5 + 5.2) process repeat steps + increment counter */
      /* This check is not required while the sharedSecret size and hash digest size are equal */
      /* \fr for (counter = 1u; counter <= repeatSteps; counter++)*/
      {
        /*retVal = E_NOT_OK;*/

        /* #35 copy counter to string. */
        /* \fr Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray(counter, concatinateBuf, 32u);  SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
        Crypto_30_Vtt_Local_CopyUint32ToBigEndianArray(1u, concatinateBuf); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

        /* #40 (Spec-Step: 5.1) Hash(counter || Z || OtherInfo)*/
        if (esl_initSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsSha256) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE */
        {
          if (esl_updateSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsSha256, (eslt_Length)(CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE + sharedSecretLength + otherInfoLength), (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))concatinateBuf) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
          {
            if (esl_finalizeSHA256((P2VAR(eslt_WorkSpaceSHA256, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsSha256, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&derivedKeyPtr[derivedLength]) == ESL_ERC_NO_ERROR) /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
            {
              derivedLength += ESL_SIZEOF_SHA256_DIGEST;
              retVal = E_OK;
            }
          }
        }
        /* This check is not required while the sharedSecret size and hash digest size are equal */
        /* \fr if (retVal != E_OK)
        {
          break;
        }*/
      }
    }
  }

  /* #45 Set derived key length. */
  if (retVal == E_OK)
  {
    *derivedKeyLengthPtr = derivedLength; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
  }

  /* #50 clear buffer */
  Crypto_30_Vtt_ClearData(concatinateBuf, CRYPTO_30_VTT_NIST80056A_COUNTER_SIZE + CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE + CRYPTO_30_VTT_NIST80056A_OTHER_INFO_MAXSIZE); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) otherInfoPtr,
  uint32 otherInfoPtrLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) derivedKeyPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) derivedKeyLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sharedSecret[CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY * 2];
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (Crypto_30_Vtt_Local_EcP_CalculateSharedSecret(privateKeyPtr, privateKeyLength, partnerPubKeyPtr, partnerPubKeyLength, sharedSecret) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  {
      /* #1  Single-step KDF Option 1 Hash */
      /* use only shared secret X for concatenation */
      retVal = Crypto_30_Vtt_Local_KeyDerive_Nist80056A_SingleStepKdfHash(sharedSecret, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY, otherInfoPtr, otherInfoPtrLength, derivedKeyPtr, derivedKeyLengthPtr); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  }

  /* #5 destroy all working copies. */
  Crypto_30_Vtt_ClearData(sharedSecret, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY * 2); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif /*((CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) || (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON))*/

#if (CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_Nist80056AConcatinateHashAndLoadOtherInfo()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */

  uint32 partnerPubKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2;
  uint32 privKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE;
  uint32 otherInfoLength = CRYPTO_30_VTT_NIST80056A_OTHER_INFO_MAXSIZE;
  uint32 derivedKeyBufLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE;

  uint8 partnerPubKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2];
  uint8 privKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE];
  uint8 otherInfo[CRYPTO_30_VTT_NIST80056A_OTHER_INFO_MAXSIZE];
  uint8 derivedKeyBuf[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE];
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType localRet;

  /* ----- Implementation ----------------------------------------------- */

  /* #1 read other info from key element. */
  localRet = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYDERIVATION_ADDITIONAL_INFO, otherInfo, &otherInfoLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY, &partnerPubKey[0], &partnerPubKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &privKey[0], &privKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRet == E_OK)
  {
    /* #5 calculate Nist 800 56A */
    if (Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S(privKey, privKeyLength, partnerPubKey, partnerPubKeyLength, otherInfo, otherInfoLength, derivedKeyBuf, &derivedKeyBufLength) == E_OK) /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
    {
      if (Crypto_30_Vtt_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, derivedKeyBuf, derivedKeyBufLength) == E_OK) /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
      {
        retVal = E_OK;
      }
    }
  }

  /* #10 destroy all working copies. */
  Crypto_30_Vtt_ClearData(privKey, privKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(partnerPubKey, partnerPubKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(otherInfo, otherInfoLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(derivedKeyBuf, derivedKeyBufLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON) */

#if (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_ISO15118_CheckKey()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118_CheckKey(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) pubKeyPtr,
  uint32 pubKeyLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sharedSecret[CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY * 2];
  Std_ReturnType retVal = E_NOT_OK;

  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_CONST) basePointOrder = &Crypto_30_Vtt_EccCurveNistAnsiSecP256R1Domain[192];

  /* ----- Implementation ----------------------------------------------- */
  /* #1 check if the private key is smaller than the order of the curve base point */

  if (Crypto_30_Vtt_Local_CompareData_IsSmaller(privateKeyPtr, basePointOrder, privateKeyLength) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
  {

    /* #5 verify key pair with esLib */
    if (Crypto_30_Vtt_Local_EcP_CalculateSharedSecret(privateKeyPtr, privateKeyLength, pubKeyPtr, pubKeyLength, sharedSecret) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    {
      retVal = E_OK;
    }
  }

  /* #10 destroy all working copies. */
  Crypto_30_Vtt_ClearData(sharedSecret, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY * 2); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_ISO15118_Aes128Decrypt()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118_Aes128Decrypt(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) ivPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) encryptedMessagePtr,
  uint32 encryptedMessageLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) plainMessagePtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) plainMessageLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  eslt_WorkSpaceAES128   localWsAes128;
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  uint32 outputDataLen;
  uint32 totalOutputDataLen = 0;

  /* #0 initialize the AES workspaces */
  if (ESL_ERC_NO_ERROR == esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsAes128.header, ESL_MAXSIZEOF_WS_AES128, CRYPTO_30_VTT_WATCHDOG_PTR)) /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */
  {

    /* #5 initialize the decryption */
    eslRet = esl_initDecryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsAes128, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))keyPtr, /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE */
                                   ESL_BM_CBC, ESL_PM_PKCS5, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))ivPtr);

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      outputDataLen = *plainMessageLengthPtr;
      /* #10 decrypt the actual private key */
      eslRet = esl_decryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsAes128,
                                 (eslt_Length)encryptedMessageLength, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))encryptedMessagePtr,
                                 (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&outputDataLen, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))plainMessagePtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */  /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      if (eslRet == ESL_ERC_NO_ERROR)
      {
        /* #15 save the decrypted length */
        totalOutputDataLen += outputDataLen;
        /* reset the OutputDataLen */
        outputDataLen = (*plainMessageLengthPtr - totalOutputDataLen);

      /* #20 add padding to get least block */
        eslRet = esl_decryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsAes128,  /* SBSW_CRYPTO_30_VTT_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                   (eslt_Length)1u, (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))encryptedMessagePtr,
                                   (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&outputDataLen, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&plainMessagePtr[totalOutputDataLen]); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */

        if (eslRet == ESL_ERC_NO_ERROR)
        {
          /* #25 update the decrypted length */
          totalOutputDataLen += outputDataLen;
        }
      }
    }
    if (eslRet == ESL_ERC_NO_ERROR)
    {
      /* #30 provide the total output data len to the application */
      *plainMessageLengthPtr = totalOutputDataLen; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
      retVal = E_OK;
    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive_ISO15118()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive_ISO15118(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 encryptedIvAndPrivKeyLength = CRYPTO_30_VTT_ISO15118_SIZEOF_IV_AND_PRIV_KEY;

  uint32 privKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE;
  uint32 partnerPubKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2;

  uint32 derivedKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE;

  uint32 newPrivateKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE;
  uint32 newPubKeyLength = CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2;

  uint8 otherInfo[CRYPTO_30_VTT_ISO15118_SIZEOF_OTHER_INFO];
  uint8 encryptedIvAndPrivKey[CRYPTO_30_VTT_ISO15118_SIZEOF_IV_AND_PRIV_KEY];

  uint8 privKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE];
  uint8 partnerPubKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2];

  uint8 derivedKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE];

  uint8 newPrivateKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE];
  uint8 newPubKey[CRYPTO_30_VTT_ECDHE_KEY_MAXSIZE * 2];

  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType localRet;
  /* ----- Implementation ----------------------------------------------- */

  /* #5 Get KeyElements for CertificateInstallation/Update (ISO 15118) */
  localRet = Crypto_30_Vtt_Local_KeyElementGet(targetCryptoKeyId, CRYPTO_KE_CUSTOM_SCC_CONTRACT_PUBLIC_KEY, &newPubKey[0], &newPubKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_Vtt_Local_KeyElementGet(targetCryptoKeyId, CRYPTO_KE_CUSTOM_SCC_IV_AND_ENCRYPTED_PRIVATE_KEY, &encryptedIvAndPrivKey[0], &encryptedIvAndPrivKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  localRet |= Crypto_30_Vtt_Local_KeyElementGet(targetCryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY, &partnerPubKey[0], &partnerPubKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  /* load old partner public value and private key */
  localRet |= Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &privKey[0], &privKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

  if ((localRet == E_OK) && (encryptedIvAndPrivKeyLength == CRYPTO_30_VTT_ISO15118_SIZEOF_IV_AND_PRIV_KEY))
  {
    /* [V2G2-818]
    * The algorithm ID shall be one character 0x01. The sender name ID U shall be one character "U" = 0x55 , the receiver name ID V shall be
    * one character "V" = 0x56. A symmetric encryption key of exactly 128 bits shall be derived. */

    /* #10 Set OtherInfo = (AlgorithmID || PartyUInfo(Tranmitter) || PartyVInfo(Receiver)) */
    otherInfo[0] = 0x01u; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
    otherInfo[1] = 0x55u; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
    otherInfo[2] = 0x56u; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */

    /* #15 Execute Concatenation KDF */
    localRet = Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S(privKey, privKeyLength, partnerPubKey, partnerPubKeyLength, otherInfo, CRYPTO_30_VTT_ISO15118_SIZEOF_OTHER_INFO, derivedKey, &derivedKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
    if (localRet == E_OK)
    {
      /* #20 decrypt new key */
      localRet = Crypto_30_Vtt_Local_KeyDerive_ISO15118_Aes128Decrypt(encryptedIvAndPrivKey, derivedKey, &encryptedIvAndPrivKey[CRYPTO_30_VTT_ISO15118_PRIV_KEY_START], encryptedIvAndPrivKeyLength - CRYPTO_30_VTT_ISO15118_PRIV_KEY_START, newPrivateKey, &newPrivateKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
      if (localRet == E_OK)
      {
        /* #25 Verify new key pair */
        /* The best way the verify that the keys are valid is to generate and verify a signature. This need to be implemented by upper layer e.g. TLS */
        if (Crypto_30_Vtt_Local_KeyDerive_ISO15118_CheckKey(newPrivateKey, newPrivateKeyLength, newPubKey, newPubKeyLength) == E_OK) /* PRQA S 3354 */ /* MD_CRYPTO_30_VTT_3354 */ /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
        {
          /* #30 Store new key pair */
          if (Crypto_30_Vtt_Local_KeyElementSet(targetCryptoKeyId, CRYPTO_KE_KEYDERIVATION_PASSWORD, &newPrivateKey[0], newPrivateKeyLength) == E_OK)  /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
          {
            retVal = E_OK;
          }
        }
      }
    }
  }

  /* #40 clear data */
  Crypto_30_Vtt_ClearData(encryptedIvAndPrivKey, encryptedIvAndPrivKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(privKey, privKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(partnerPubKey, partnerPubKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(derivedKey, derivedKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(newPrivateKey, newPrivateKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  Crypto_30_Vtt_ClearData(newPubKey, newPubKeyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

#if (CRYPTO_30_VTT_KEY_DERIVE_ALGORITHM == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyDerive()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  uint32 deriveAlgorithmLength = CRYPTO_30_VTT_KEY_DERIVE_SIZEOF_ALGORITHM;
  uint8 deriveAlgorithm[CRYPTO_30_VTT_KEY_DERIVE_SIZEOF_ALGORITHM];

  Std_ReturnType retVal;

  /* #1 Get Algorithm from the algorithm key element */
  retVal = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYDERIVATION_ALGORITHM, deriveAlgorithm, &deriveAlgorithmLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

  if ((retVal == E_OK) && (deriveAlgorithmLength == CRYPTO_30_VTT_KEY_DERIVE_SIZEOF_ALGORITHM))
  {
    /* #5 Dispatch key derivation according the given algorithm */
# if (CRYPTO_30_VTT_KDF_ALGO_SYM_NIST_800_108_CNT_MODE_SHA256_ENABLED == STD_ON)
    if (deriveAlgorithm[0] == CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256)
    {
      retVal = Crypto_30_Vtt_Local_KeyDerive_Nist800108NistFips1864(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_VTT_KDF_ALGO_KDF_SYM_NIST_800_108_CNT_MODE_SHA256);
    }
    else
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_ASYM_NIST_FIPS_186_4_ERB_ENABLED == STD_ON)
    if (deriveAlgorithm[0] == CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB)
    {
      retVal = Crypto_30_Vtt_Local_KeyDerive_Nist800108NistFips1864(cryptoKeyId, targetCryptoKeyId, CRYPTO_30_VTT_KDF_ALGO_KDF_ASYM_NIST_FIPS_186_4_ERB);
    }
    else
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)
    if (deriveAlgorithm[0] == CRYPTO_30_VTT_KDF_ALGO_KDF_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256)
    {
      retVal = Crypto_30_Vtt_Local_KeyDerive_Nist80056A_OnePass_C1E1S_AndLoadKey(cryptoKeyId, targetCryptoKeyId);
    }
    else
# endif
# if (CRYPTO_30_VTT_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
    if (deriveAlgorithm[0] == CRYPTO_30_VTT_KDF_ALGO_KDF_ISO_15118_CERTIFICATE_HANDLING)
    {
      retVal = Crypto_30_Vtt_Local_KeyDerive_ISO15118(cryptoKeyId, targetCryptoKeyId);
    }
    else
# endif
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  /* #10 If key was derived successfully, mark it as valid */
  if (retVal == E_OK)
  {
    Crypto_30_Vtt_SetKeyState(targetCryptoKeyId, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK);
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif  /* (CRYPTO_30_VTT_SUPPORT_KEY_DERIVE == STD_ON) */

#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_X25519()
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

CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_X25519(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 publicKey[ESL_SIZEOF_X25519_PUBLIC_KEY];
  eslt_Length publicKeyLength = ESL_SIZEOF_X25519_PUBLIC_KEY;
  eslt_WorkSpaceX25519 localWsX25519;
  eslt_ErrorCode eslRet;
  Std_ReturnType retValKeyElementSetPrivateKey = E_NOT_OK;
  Std_ReturnType retValKeyElementSetPublicKey = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

    /* #20 Init Workspace */
    eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519.header, ESL_SIZEOF_WS_X25519, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initECDH((P2VAR(eslt_WorkSpace, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 Calculate Keys*/

    if (esl_generateEphemeralKeyPairECDH((P2VAR(eslt_WorkSpace, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519, publicKey, &publicKeyLength) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS */
    {
      /* #30 Store public and private key to key elements if calculation was successfully */
      retValKeyElementSetPrivateKey = Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(((actX25519Struct*)localWsX25519.wsX25519)->privateKey), ESL_SIZEOF_X25519_PRIVATE_KEY); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
      retValKeyElementSetPublicKey = Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))publicKey, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    }

    if ((retValKeyElementSetPrivateKey == E_OK) && (retValKeyElementSetPublicKey == E_OK))
    {
      /* #35 Copy public key to output buffer and overwrite local copy if store key was successfully */
      /* [SWS_Crypto_00109] return public key */
      if (*publicValueLengthPtr >= ESL_SIZEOF_X25519_PUBLIC_KEY) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
      { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */

        Crypto_30_Vtt_CopyData(publicValuePtr, publicKey, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK */
        Crypto_30_Vtt_ClearData(publicKey, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK */
        *publicValueLengthPtr = ESL_SIZEOF_X25519_PUBLIC_KEY; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_WITH_DET_CHECK */
        retVal = E_OK;
      }
      else
        /* #40 If buffer is too small return with error */
      { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        retVal = CRYPTO_E_SMALL_BUFFER;
      }
    }
  }
  return retVal;
}
# endif

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_ANSI()
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

CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_ANSI(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 privKey[CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY];
  uint8 publicKey[Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)];
  eslt_WorkSpaceEcP localWsEcP;
  eslt_ErrorCode eslRet;
  Std_ReturnType retValKeyElementSetPrivateKey = E_NOT_OK;
  Std_ReturnType retValKeyElementSetPublicKey = E_NOT_OK;

  /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_VTT_CONST) EcDomainPtr = Crypto_30_Vtt_EccCurveNistAnsiSecP256R1Domain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_VTT_CONST) EcDomainExtPtr = Crypto_30_Vtt_EccCurveNistAnsiSecP256R1DomainExt;
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_VTT_CONST) EcSpeedUpExtPtr = Crypto_30_Vtt_EccCurveNistAnsiSecP256R1SpeedUpExt;

  /* ----- Implementation ------------------------------------------------- */
  /* #20 Init Workspace */
    eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP.header, ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initGenerateKeyEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP, EcDomainPtr, EcDomainExtPtr, EcSpeedUpExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    if (esl_getLengthOfEcPprivateKey(EcDomainPtr) == CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY) /* SBSW_CRYPTO_30_VTT_ESL_CONST_WORKSPACE */
    {
      if (esl_getLengthOfEcPpublicKey_comp(EcDomainPtr) == CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY) /* SBSW_CRYPTO_30_VTT_ESL_CONST_WORKSPACE */
      {
        /* #25 Calculate Keys*/

        if (esl_generateKeyEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsEcP, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))privKey, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))publicKey, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&(publicKey[CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY])) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS */
        {
          /* #30 Store public and private key to key elements if calculation was successfully */
          retValKeyElementSetPrivateKey = Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, publicKey, Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
          retValKeyElementSetPublicKey = Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, privKey, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
        }

        if ((retValKeyElementSetPrivateKey == E_OK) && (retValKeyElementSetPublicKey == E_OK))
        {
          /* #35 Copy public key to output buffer and overwrite local copy if store key was successfully */
          /* [SWS_Crypto_00109] return public key */
          if (*publicValueLengthPtr >= Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)) /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */

            Crypto_30_Vtt_CopyData(publicValuePtr, publicKey, Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)); /* SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK */

            /* delete buffer content from RAM */
            Crypto_30_Vtt_ClearData(privKey, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
            Crypto_30_Vtt_ClearData(publicKey, Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

            *publicValueLengthPtr = Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY); /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_WITH_DET_CHECK */
            retVal = E_OK;
          }
          else
            /* #40 If buffer is too small return with error */
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            retVal = CRYPTO_E_SMALL_BUFFER;
          }
        }
      }
    }
  }

  return retVal;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcPubVal()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 kea[CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM];
  uint32 keaLength = CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Lock write access for the CryptoKey */
  if (Crypto_30_Vtt_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
  {
    /* #20 Read and check algorithm key element. */
    if (Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_ALGORITHM, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))kea, &keaLength) != E_OK) /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
    {

    }
    else if (keaLength != CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM)
    {

    }
    else
    {
# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
      if (kea[0] == CRYPTO_30_VTT_KEY_EXCHANGE_X25519)
      {
        /* #30 execute KEA x25519 / RFC 7748 */
        retVal = Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_X25519(cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_VTT_FORWARDING_PTR */
      }
      else
# endif

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
      if (kea[0] == CRYPTO_30_VTT_KEY_EXCHANGE_ANSIP256R1)
      {
        /* #40 execute KEA ANSIp256r1 / ANSI X9.63 */
        retVal = Crypto_30_Vtt_Local_KeyExchangeCalcPubVal_ANSI(cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_VTT_FORWARDING_PTR */
      }
      else
# endif
      {
        /* invalid algorithm */
      }

    }
    /* #50 Release key write lock. */
    Crypto_30_Vtt_Local_KeyWriteLockRelease(cryptoKeyId);
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcSecret_X25519()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret_X25519(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  uint8 keyExchangePrivateKey[ESL_SIZEOF_X25519_PRIVATE_KEY];
  uint8 sharedSecret[ESL_SIZEOF_X25519_SHARED_SECRET];
  uint32 keyExchangePrivateKeyLength = ESL_SIZEOF_X25519_PRIVATE_KEY;
  eslt_Length sharedSecretLength_eslt = ESL_SIZEOF_X25519_SHARED_SECRET;
  eslt_WorkSpaceX25519 localWsX25519;
  eslt_ErrorCode eslRet;

  /* #20 Initialize workspace */
  /* initialize Ed25519 */
    eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519.header, ESL_SIZEOF_WS_X25519, CRYPTO_30_VTT_WATCHDOG_PTR); /* SBSW_CRYPTO_30_VTT_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initECDH((P2VAR(eslt_WorkSpace, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 Load private key */
    /* copy content of key elements to local buffers */
    Std_ReturnType localRetVal;

    localRetVal = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, keyExchangePrivateKey, &keyExchangePrivateKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

    if (E_OK == localRetVal)
    {
      /* import private key from key elements */
      if (esl_importStaticPrivateKeyECDH((P2VAR(eslt_WorkSpace, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519, keyExchangePrivateKey) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_VTT_0310 */ /* SBSW_CRYPTO_30_VTT_WORKSPACE_STACK_WITH_STACK_BUFFERS */
      {
        /* #30 generate secret by using public key from key element */
        eslRet = esl_generateSharedSecretECDH((P2VAR(eslt_WorkSpace, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))&localWsX25519, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))partnerPublicValuePtr, sharedSecret, &sharedSecretLength_eslt); /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_VTT_0310, MD_CRYPTO_30_VTT_0311 */ /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

        if (eslRet == ESL_ERC_NO_ERROR)
        {
          /* #35 Store calculated sharedSecret */
          if (Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, sharedSecret, (uint32)sharedSecretLength_eslt) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
          {
            retVal = E_OK;
          }
        }

        /* #40 delete buffer content from RAM */
        Crypto_30_Vtt_ClearData(sharedSecret, ESL_SIZEOF_X25519_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
      }
    }
    Crypto_30_Vtt_ClearData(keyExchangePrivateKey, ESL_SIZEOF_X25519_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  }
  return retVal;
}/* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif

# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcSecret_ANSI()
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
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret_ANSI(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 keyExchangePrivateKey[CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY];
  uint8 sharedSecret[Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)];
  uint32 keyExchangePrivateKeyLength = CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY;

  /* ----- Implementation ------------------------------------------------- */

  /* #25 Load private key */
  /* copy content of key elements to local buffers */
  if (Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, keyExchangePrivateKey, &keyExchangePrivateKeyLength) != E_OK) /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  {
    /* return E_NOT_OK */
  }
  else
  {
    /* #30 generate secret by using public key from key element */

    if (Crypto_30_Vtt_Local_EcP_CalculateSharedSecret(keyExchangePrivateKey, keyExchangePrivateKeyLength, partnerPublicValuePtr, partnerPublicValueLength, sharedSecret) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    {
      /* #35 Store calculated sharedSecret */
      if (Crypto_30_Vtt_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, sharedSecret, Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)) == E_OK) /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
      {
        retVal = E_OK;
      }
    }
    /* #40 delete buffer content from RAM */
    Crypto_30_Vtt_ClearData(sharedSecret, Crypto_30_Vtt_Math_Mul2(CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY)); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
  }
  Crypto_30_Vtt_ClearData(keyExchangePrivateKey, CRYPTO_30_VTT_SIZEOF_ECDHE_256_KEY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

  return retVal;
}/* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
# endif

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyExchangeCalcSecret()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 kea[CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM];
  uint32 keaLength = CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM;

  /* #10 Lock write access for the CryptoKey */
  if (Crypto_30_Vtt_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #20 Read and check algorithm key element. */
    if (Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_ALGORITHM, kea, &keaLength) != E_OK) /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
    {

    }
    else if (keaLength != CRYPTO_30_VTT_KEY_EXCHANGE_SIZEOF_ALGORITHM)
    {

    }
    else
    {
# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
      if (kea[0] == CRYPTO_30_VTT_KEY_EXCHANGE_X25519)
      {
        /* #30 execute KEA x25519 / RFC 7748 */
        if (partnerPublicValueLength == ESL_SIZEOF_X25519_PUBLIC_KEY)
        {
          retVal = Crypto_30_Vtt_Local_KeyExchangeCalcSecret_X25519(cryptoKeyId, partnerPublicValuePtr); /* SBSW_CRYPTO_30_VTT_FORWARDING_PTR */
        }
      }
      else
# endif
# if (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
      if ((kea[0] == CRYPTO_30_VTT_KEY_EXCHANGE_ANSIP256R1))
      {
        /* #40 execute KEA ANSIp256r1 / ANSI X9.63 */
         retVal = Crypto_30_Vtt_Local_KeyExchangeCalcSecret_ANSI(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_PTR */
      }
      else
# endif
      {
        /* invalid algorithm */
      }
    }
    /* #50 Release key write lock. */
    Crypto_30_Vtt_Local_KeyWriteLockRelease(cryptoKeyId);
  }

  return retVal;
}/* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_VTT_USE_VSTD_LIB == STD_OFF)
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_CopyData_Implementation()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CopyData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) sourceData,
  uint32 dataLength)
{
  uint32_least byteIdx;
  /* #10 Copy data bytewise to the target buffer */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++)
  {
    targetData[byteIdx] = sourceData[byteIdx]; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
  }
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_ClearData_Implementation()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_ClearData_Implementation(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataBuf,
  uint32 dataLength)
{
  uint32_least byteIdx;
  /* #10 Clear data bytewise */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++)
  {
    dataBuf[byteIdx] = 0x00u; /* SBSW_CRYPTO_30_VTT_API_CHECKED_SIZE_ACCESS */
  }
}
#endif /* (CRYPTO_30_VTT_USE_VSTD_LIB == STD_OFF) */

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyReadLockGetNotProtected()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (Crypto_30_Vtt_GetKeyLock(cryptoKeyId) >= CRYPTO_30_VTT_KEY_LOCK_FREE)
  {
    Crypto_30_Vtt_IncKeyLock(cryptoKeyId); /* SBSW_CRYPTO_30_VTT_CSL01 */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId)
{
  if (Crypto_30_Vtt_GetKeyLock(cryptoKeyId) > CRYPTO_30_VTT_KEY_LOCK_FREE) /* COV_CRYPTO_30_VTT_KEY_LOCKING_RELEASE */
  {
    Crypto_30_Vtt_DecKeyLock(cryptoKeyId); /* SBSW_CRYPTO_30_VTT_CSL01 */
  }
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyReadLockGet()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockGet(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal;

  SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
  retVal = Crypto_30_Vtt_Local_KeyReadLockGetNotProtected(cryptoKeyId);
  SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyReadLockRelease()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockRelease(
  uint32 cryptoKeyId)
{
  SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
  Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected(cryptoKeyId);
  SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyWriteLockGet()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyWriteLockGet(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
  if (Crypto_30_Vtt_GetKeyLock(cryptoKeyId) == CRYPTO_30_VTT_KEY_LOCK_FREE)
  {
    Crypto_30_Vtt_SetKeyLock(cryptoKeyId, CRYPTO_30_VTT_KEY_LOCK_WRITE);  /* SBSW_CRYPTO_30_VTT_CSL01 */
    retVal = E_OK;
  }
  SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyWriteLockRelease()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyWriteLockRelease(
  uint32 cryptoKeyId)
{

  SchM_Enter_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();
  if (Crypto_30_Vtt_GetKeyLock(cryptoKeyId) == CRYPTO_30_VTT_KEY_LOCK_WRITE) /* COV_CRYPTO_30_VTT_KEY_LOCKING_RELEASE */
  {
    Crypto_30_Vtt_SetKeyLock(cryptoKeyId, CRYPTO_30_VTT_KEY_LOCK_FREE); /* SBSW_CRYPTO_30_VTT_CSL01 */
  }
  SchM_Exit_VTTCrypto_30_Vtt_CRYPTO_30_VTT_EXCLUSIVE_AREA_0();

}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementSearch()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_Vtt_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_Vtt_KeyElementsIterType elementIdx;

  /* #10 Iterate over all elements in the key to find a matching key element */
  for (elementIdx = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIdx < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIdx++)
  {
    if (keyElementId == Crypto_30_Vtt_GetIdOfKeyElements(elementIdx))
    {
      retVal = E_OK;
      *elementIndex = elementIdx; /* SBSW_CRYPTO_30_VTT_VARIABLE_ACCESS_PTR_FORM_STACK */
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyElementIdsGet()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementIdsGet(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyElementIdsLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00161] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (keyElementIdsPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (keyElementIdsLengthPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    Crypto_30_Vtt_KeyElementsIterType elementIndex = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId);
    uint8 idx = 0u;
    retVal = E_OK;

    /* #20 Iterate over all Key Element Ids */
    while (elementIndex < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId))
    {
      /* #25 Copy Key Element Id, if the buffer is to small return with error */
      if (idx < *(keyElementIdsLengthPtr))
      {
        keyElementIdsPtr[idx] = Crypto_30_Vtt_GetIdOfKeyElements(elementIndex); /* SBSW_CRYPTO_30_VTT_ARRAY_ACCESS_WITH_SIZE_CHECK */
      }
      else
      {
        /* provided array is too small to store all element ids */
        retVal = CRYPTO_E_SMALL_BUFFER;
        /* [SWS_Crypto_00164] Check that provided buffer is big enough */
        errorId = CRYPTO_E_SMALL_BUFFER;
        break;
      }

      idx++;
      elementIndex++;
      CANoeAPI_ConsumeTicks(1);
    }

    /* #30 save written data length */
    *keyElementIdsLengthPtr = idx; /* SBSW_CRYPTO_30_VTT_PTR_ACCESS_WITH_DET_CHECK */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_ELEMENT_IDS_GET,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyCopy()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyCopy(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal, retValBuf;
  Crypto_30_Vtt_KeyElementsIterType elementIndexSrc;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexDst;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00156] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00157] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00158] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
    /* ----- Implementation ------------------------------------------------- */
  {
    /* #16 Lock write access for the target CryptoKey */
    if (Crypto_30_Vtt_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #17 Lock read access for the source CryptoKey */
      if (Crypto_30_Vtt_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
      {
        retVal = CRYPTO_E_BUSY;
      }
      else
      {
        retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
        /* #20 Loop over all elements of the source key */
        for (elementIndexSrc = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndexSrc < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndexSrc++)
        {
          /* #25 Loop over all elements of the destination key to find a matching element for the current source element */
          if (E_OK == Crypto_30_Vtt_Local_KeyElementSearch(targetCryptoKeyId, Crypto_30_Vtt_GetIdOfKeyElements(elementIndexSrc), &elementIndexDst)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
          {
            /* #30 copy key element with Crypto_30_Vtt_Local_KeyElementCopy */
            retValBuf = Crypto_30_Vtt_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
            if (retVal != E_OK)
            {
              retVal = retValBuf;
            }
          }
        }
        Crypto_30_Vtt_Local_KeyReadLockRelease(cryptoKeyId);
      }
      Crypto_30_Vtt_Local_KeyWriteLockRelease(targetCryptoKeyId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_COPY,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyElementCopy()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementCopy(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexSrc;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndexDst;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00149] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00150] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00151] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* #20 Find source element. Otherwise return with Error. */
    if (E_OK != Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndexSrc)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
    {
      /* ----- Implementation ------------------------------------------------- */
      /* [SWS_Crypto_00152] if keyElementId is out of valid range return E_NOT_OK */
      /* [SWS_Crypto_00153] if targetKeyElementId is out of valid range return E_NOT_OK */
      retVal = E_NOT_OK;
    }
    else
    {
      /* ----- Implementation ------------------------------------------------- */
      /* #25 Find destination element. Otherwise return with Error. */
      if (E_OK == Crypto_30_Vtt_Local_KeyElementSearch(targetCryptoKeyId, targetKeyElementId, &elementIndexDst)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
      {
        /* [SWS_Crypto_00152] Check that keyElementId is in valid range */
        /* [SWS_Crypto_00153] Check that targetKeyElementId is in valid range */
        errorId = CRYPTO_E_NO_ERROR;

        /* #30 Lock write access for the target CryptoKey */
        if (Crypto_30_Vtt_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
        {
          retVal = CRYPTO_E_BUSY;
        }
        else
        {
          /* #35 Lock read access for the source CryptoKey if necessary */
          /* #40 copy key element with Crypto_30_Vtt_Local_KeyElementCopy */
          if (cryptoKeyId == targetCryptoKeyId)
          {
            retVal = Crypto_30_Vtt_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
          }
          else if (Crypto_30_Vtt_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
          {
            retVal = CRYPTO_E_BUSY;
          }
          else
          {
            retVal = Crypto_30_Vtt_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
            Crypto_30_Vtt_Local_KeyReadLockRelease(cryptoKeyId);
          }
          Crypto_30_Vtt_Local_KeyWriteLockRelease(targetCryptoKeyId);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_ELEMENT_COPY,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_Vtt_KeyElementSetInternalStandard()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
*/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementSetInternalStandard(Crypto_30_Vtt_SizeOfKeyElementsType elementIndex, P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr, uint32 keyLength)
{
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex = 0u;

  keyStorageIndex = Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Copy keyPtr to key storage */
  while (keyPtrIndex < keyLength)
  { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
    Crypto_30_Vtt_SetKeyStorage(keyStorageIndex, keyPtr[keyPtrIndex]); /* SBSW_CRYPTO_30_VTT_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    keyStorageIndex++;
    keyPtrIndex++;
    CANoeAPI_ConsumeTicks(1);
  }

  Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndex, keyPtrIndex);
}

#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
/**********************************************************************************************************************
* Crypto_30_Vtt_KeyElementSetInternalVirtual()
**********************************************************************************************************************/
/*!
*
* Internal comment removed.
 *
 *
 *
 *
*/
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementSetInternalVirtual(uint32 cryptoKeyId, Crypto_30_Vtt_SizeOfKeyElementsType elementIndex, P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr, uint32 keyLength)
{
  Crypto_30_Vtt_KeyElementsIterType virtualElementIndex;
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_Vtt_KeyStorageIterType keyStorageIndex;
  uint32_least keyPtrIndex;

  virtualElementIndex = elementIndex;
  /* set root element only with the virtual key element */
  /* #10 find root key element */
  for (elementIndex = Crypto_30_Vtt_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_Vtt_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++)
  {
    if (Crypto_30_Vtt_GetKeyElementInfoVirtualIdxOfKeyElementInfo(virtualElementIndex) == Crypto_30_Vtt_GetIdOfKeyElements(elementIndex))
    {
      /* #15 check if the length is matching  */
      if (Crypto_30_Vtt_GetKeyElementLength(elementIndex) >= (Crypto_30_Vtt_GetKeyElementVirtualOffset(virtualElementIndex) + Crypto_30_Vtt_GetKeyElementWrittenLength(virtualElementIndex)))
      {
        /* #20 copy key to root key element  */
        keyStorageIndex = (Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex) + (Crypto_30_Vtt_KeyStorageIterType)Crypto_30_Vtt_GetKeyElementVirtualOffset(virtualElementIndex));
        for (keyPtrIndex = 0u; keyPtrIndex < Crypto_30_Vtt_GetKeyElementWrittenLength(virtualElementIndex); keyPtrIndex++)
        {
          Crypto_30_Vtt_SetKeyStorage(keyStorageIndex, keyPtr[keyPtrIndex]);
          keyStorageIndex++;
        }
        Crypto_30_Vtt_SetKeyElementWrittenLength(elementIndex, keyPtrIndex);
        retVal = E_OK;
      }
    }
  }

  CRYPTO_30_VTT_DUMMY_STATEMENT_CONST(keyLength); /* PRQA S 3112 */ /* MD_MSR_14.2 */

  return retVal;
}
#endif

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementSetInternal()
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
CRYPTO_30_VTT_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSetInternal(
  uint32 cryptoKeyId,
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength,
  Crypto_30_Vtt_WriteOfKeyElementInfoType writeAccess)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* #15 Handle non-virtual key element */
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    if (Crypto_30_Vtt_IsKeyElementVirtual(elementIndex) == FALSE)
#endif
    {
      /* #20 Copy data if there is enough free space */
      /* Check if the available data is not bigger than the maximum size of the destination element */
      if ((keyLength == Crypto_30_Vtt_GetKeyElementLength(elementIndex))
        || ((Crypto_30_Vtt_IsKeyElementPartial(elementIndex) == TRUE) && (keyLength <= Crypto_30_Vtt_GetKeyElementLength(elementIndex))))
      {
        if (writeAccess >= (Crypto_30_Vtt_GetWriteOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
        {
          Crypto_30_Vtt_KeyElementSetInternalStandard(elementIndex, keyPtr, keyLength); /* SBSW_CRYPTO_30_VTT_FORWARDING_PTR */
          retVal = E_OK;
        }
        else
        {
          retVal = CRYPTO_E_KEY_WRITE_FAIL;
        }
      }
      else
      {
        /* Key length is to large */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH; /* [SWS_Crypto_00146] */
      }
    }
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    /* #35 Else handle virtual key element */
    else
    {
      /* #40 Copy key element data if data size match or partial access is enabled  */
      if ((((uint32)(Crypto_30_Vtt_GetKeyStorageEndIdxOfKeyElements(elementIndex) - Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements(elementIndex))) == keyLength)
        || (Crypto_30_Vtt_IsKeyElementPartial(elementIndex) == TRUE))
      {
        if (writeAccess >= (Crypto_30_Vtt_GetWriteOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
        {
          retVal = Crypto_30_Vtt_KeyElementSetInternalVirtual(cryptoKeyId, elementIndex, keyPtr, keyLength);
        }
        else
        {
          retVal = CRYPTO_E_KEY_WRITE_FAIL;
        }
      }
      else
      {
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH; /* [SWS_Crypto_00146] */
      }
    }
#else
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

#if ((CRYPTO_30_VTT_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON))
/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementSet()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* [SWS_Crypto_91004] */
CRYPTO_30_VTT_LOCAL FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Find key element, otherwise return with Error */
  if (E_OK != Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  else
  {
    /* #15 Handle non-virtual and virtual Key */
    retVal = Crypto_30_Vtt_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, keyPtr, keyLength, CRYPTO_30_VTT_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */

    if (retVal == E_OK)
    {
      Crypto_30_Vtt_ClearKeyState(cryptoKeyId, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
* Crypto_30_Vtt_KeyElementSet()
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
/* [SWS_Crypto_91004] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized == CRYPTO_30_VTT_UNINIT)
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00075] */
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00076] */
  }
  else if (keyPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* [SWS_Crypto_00078] */
  }
  else if (keyLength == 0u)
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* [SWS_Crypto_00079] */
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* ----- Local Variables ------------------------------------------------ */
    Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;

    /* ----- Implementation ------------------------------------------------- */

    /* #20 Find key element, otherwise return with Error */
    if (E_OK != Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
    {
      /* [SWS_Crypto_00077] check if keyElementId is in valid range */
      errorId = CRYPTO_E_PARAM_HANDLE;
      retVal = E_NOT_OK;
    }
    /* #23 Lock write access for the CryptoKey */
    else if (Crypto_30_Vtt_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #25 Handle non-virtual and virtual key elements */
      retVal = Crypto_30_Vtt_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, keyPtr, keyLength, CRYPTO_30_VTT_WA_ALLOWED); /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */
      if (retVal == CRYPTO_E_KEY_SIZE_MISMATCH)
      {
        /* [SWS_Crypto_00146] */
#if (CRYPTO_30_VTT_SUPPORT_SHE_KEY_UPDATE == STD_ON)
        /* #30 Else: if key size matching update using SHE key update protocol */
        /* Key update using SHE key update protocol */
# if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
        if ((Crypto_30_Vtt_IsKeyElementVirtual(elementIndex) == FALSE) &&
          (keyLength == CRYPTO_30_VTT_SIZEOF_SHE_UPDATE_KEY) && (Crypto_30_Vtt_GetKeyElementLength(elementIndex) == CRYPTO_30_VTT_SIZEOF_SHE_KEY))
# else
        if ((keyLength == CRYPTO_30_VTT_SIZEOF_SHE_UPDATE_KEY) && (Crypto_30_Vtt_GetKeyElementLength(elementIndex) == CRYPTO_30_VTT_SIZEOF_SHE_KEY))
# endif
        {
          if (CRYPTO_30_VTT_WA_ENCRYPTED == Crypto_30_Vtt_GetWriteOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex)))
          {
            retVal = Crypto_30_Vtt_SheKeyUpdate(cryptoKeyId, elementIndex, keyPtr); /* SBSW_CRYPTO_30_VTT_FORWARDING_OF_KEYPTR */
          }
          else
          {
            retVal = CRYPTO_E_KEY_WRITE_FAIL;
          }
        }
#endif
      }

      if (retVal == E_OK)
      {
        Crypto_30_Vtt_ClearKeyState(cryptoKeyId, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK & CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK);
      }
      Crypto_30_Vtt_Local_KeyWriteLockRelease(cryptoKeyId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_ELEMENT_SET,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_Vtt_KeyValidSet()
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
/* [SWS_Crypto_91005] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyValidSet(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00082] */
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00083] */
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #17 Lock write access for the CryptoKey */
    if (Crypto_30_Vtt_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #20 Set the given KeyId valid */
      Crypto_30_Vtt_SetKeyState(cryptoKeyId, CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK);
      VttCntrl_WriteCryptoNvFile();
      Crypto_30_Vtt_Local_KeyWriteLockRelease(cryptoKeyId);
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_VALID_SET,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
* Crypto_30_Vtt_Local_KeyElementGet()
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
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) resultPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Search Key Element */
  if (E_OK != Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  /* #15 Check if the Key element is valid */
  else if (FALSE == Crypto_30_Vtt_IsKeyElementValid(elementIndex))
  {
    /* [SWS_Crypto_00039] */
    retVal = CRYPTO_E_KEY_NOT_VALID;
  }
  else
  {
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    /* #20 Check if the key element is virtual */
    if (Crypto_30_Vtt_IsKeyElementVirtual(elementIndex) == TRUE) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
    {
      /* #30 Handle virtual key element*/
      retVal = Crypto_30_Vtt_Local_KeyElementGet_Virtual(cryptoKeyId, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))resultPtr, (P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))resultLengthPtr, elementIndex); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    }
    else
#endif
    {
      /* #40 Handle non-virtual key element */
      retVal = Crypto_30_Vtt_Local_KeyElementGet_Standard((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))resultPtr, (P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))resultLengthPtr, elementIndex); /* SBSW_CRYPTO_30_VTT_FORWARDING_STACK_BUFFER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_ReadTwoKeyElements()
*********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_ReadTwoKeyElements(
  uint32 cryptoKeyId,
  uint32 firstKeyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) firstKey,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) firstKeyLength,
  uint32 secondKeyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) secondKey,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) secondKeyLength)
{
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValKey1, retValKey2;

  /* Check if key element is accessible */
  /* #1 Read the requested key elements to the given buffer */
  retValKey1 = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, firstKeyElementId, firstKey, firstKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */
  retValKey2 = Crypto_30_Vtt_Local_KeyElementGet(cryptoKeyId, secondKeyElementId, secondKey, secondKeyLength); /* SBSW_CRYPTO_30_VTT_STACK_ARRAY_VARIABLE_AS_PTR */

  /* #5 check both results and set return value */
  if ((retValKey1 == E_OK) && (retValKey2 == E_OK))
  {
    retVal = E_OK;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
* Crypto_30_Vtt_KeyElementGet()
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
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal;
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00085] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00086] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (resultPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00088] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00089] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*resultLengthPtr == 0u)
  {
    /* [SWS_Crypto_00090] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #20 Search Key Element */
    if (E_OK != Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex)) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
    {
      /* [SWS_Crypto_00087] Check that keyElementId is in valid range */
      errorId = CRYPTO_E_PARAM_HANDLE;
      /* Info: This return value is according to the Spec although CRYPTO_E_KEY_NOT_AVAILABLE would make more sense */
      retVal = E_NOT_OK;
    }
    /* #21 Try to get read lock */
    else if (Crypto_30_Vtt_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #25 Check if the Key element is valid */
      if (FALSE == Crypto_30_Vtt_IsKeyElementValid(elementIndex))
      {
        /* [SWS_Crypto_00039] */
        /* Info: This return value is according to the Spec although CRYPTO_E_KEY_NOT_VALID would make more sense */
        retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
      }
      else
      {

        /* #40 Check if read access is allowed */
        if (CRYPTO_30_VTT_RA_ALLOWED != (Crypto_30_Vtt_GetReadOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
        {
          retVal = CRYPTO_E_KEY_READ_FAIL;
        }
        else
        {
#if (CRYPTO_30_VTT_KEYELEMENTINFOVIRTUALUSEDOFKEYELEMENTINFO == STD_ON) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
          /* #50 Check if the key element is virtual */
          if (Crypto_30_Vtt_IsKeyElementVirtual(elementIndex) == TRUE) /* COV_CRYPTO_30_VTT_VIRTUALKEY */
          {
            /* #60 Handle virtual key element */
            retVal = Crypto_30_Vtt_Local_KeyElementGet_Virtual(cryptoKeyId, resultPtr, resultLengthPtr, elementIndex); /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */
          }
          else
#endif
          {
            /* #70 Handle non-virtual key element*/
            retVal = Crypto_30_Vtt_Local_KeyElementGet_Standard(resultPtr, resultLengthPtr, elementIndex); /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */
          }
        }
      }
      Crypto_30_Vtt_Local_KeyReadLockRelease(cryptoKeyId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_ELEMENT_GET,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Crypto_30_Vtt_RandomSeed()
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
/* [SWS_Crypto_91013] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00128] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00129] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (entropyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00130] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (entropyLength == 0u)
  {
    /* [SWS_Crypto_00131] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    Crypto_30_Vtt_SizeOfKeyElementsType elementIndex;

    /* #20 Generate and Store Random Seed */
    if (Crypto_30_Vtt_Local_KeyElementSearch(cryptoKeyId, CRYPTO_KE_RANDOM_SEED_STATE, &elementIndex) == E_OK) /* SBSW_CRYPTO_30_VTT_STACK_VARIABLE_AS_PTR */
    {
      /* #22 Lock write access for the CryptoKey */
      if (Crypto_30_Vtt_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
      {
        retVal = CRYPTO_E_BUSY;
      }
      else
      {
        if (Crypto_30_Vtt_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, entropyPtr, entropyLength, CRYPTO_30_VTT_WA_ALLOWED) == E_OK) /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */
        {
          /* The elementIndex do not need to be checked. The Code is only reachable if the key element is available. (Crypto_30_Vtt_Local_KeyElementSearch) */
          Crypto_30_Vtt_SetKeyElementStateByMask(elementIndex, CRYPTO_30_VTT_KEYELEMENTSTATE_SEED_MASK | CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_VTT_RNG_SEED */
          retVal = E_OK;
        }
        Crypto_30_Vtt_Local_KeyWriteLockRelease(cryptoKeyId);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_RANDOM_SEED,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Crypto_30_Vtt_KeyGenerate()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

/* [SWS_Crypto_91007] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyGenerate(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00094] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00095] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_GENERATE,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyDerive()
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
 */
/* [SWS_Crypto_91008] */
/* PRQA S 0715 KEYDERIVE0715 */ /* MD_MSR_1.1_715 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00097] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00098] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_?????] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
#if (CRYPTO_30_VTT_KEY_DERIVE_ALGORITHM == STD_ON)
    /* #30 Lock write access for the target CryptoKey */
    if (Crypto_30_Vtt_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #35 Lock read access for the source CryptoKey if necessary */
      /* #40 derive key element with Crypto_30_Vtt_Local_KeyDerive */
      if (cryptoKeyId == targetCryptoKeyId)
      {
        retVal = Crypto_30_Vtt_Local_KeyDerive(cryptoKeyId, targetCryptoKeyId);
      }
      else if (Crypto_30_Vtt_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
      {
        retVal = CRYPTO_E_BUSY;
      }
      else
      {
        retVal = Crypto_30_Vtt_Local_KeyDerive(cryptoKeyId, targetCryptoKeyId);
        Crypto_30_Vtt_Local_KeyReadLockRelease(cryptoKeyId);
      }
      Crypto_30_Vtt_Local_KeyWriteLockRelease(targetCryptoKeyId);
    }
#else
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(targetCryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID, CRYPTO_30_VTT_INSTANCE_ID, CRYPTO_30_VTT_SID_KEY_DERIVE, errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:KEYDERIVE0715 */

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyExchangeCalcPubVal()
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
/* [SWS_Crypto_91009] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValuePtr, /* PRQA S 3673 */ /* MD_CRYPTO_30_VTT_3673_API_REQUIREMENT */
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) publicValueLengthPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_VTT_3673_API_REQUIREMENT */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00103] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00104] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (publicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00105] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (publicValueLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00106] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*publicValueLengthPtr == 0u)
  {
    /* [SWS_Crypto_00107] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  /* vismaw: the retVal is already captured in the methods body */
  /* [SWS_Crypto_00110] */
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
    /* #20 Calculate Public Value */
    retVal = Crypto_30_Vtt_Local_KeyExchangeCalcPubVal(cryptoKeyId, publicValuePtr, publicValueLengthPtr);  /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */

# if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
    if (retVal == CRYPTO_E_SMALL_BUFFER)
    {
      /* [SWS_Crypto_00110] */
      errorId = CRYPTO_E_PARAM_VALUE;
    }
# endif
#else
    retVal = E_NOT_OK;
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(publicValuePtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(publicValueLengthPtr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_EXCHANGE_CALC_PUB_VAL,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Crypto_30_Vtt_KeyExchangeCalcSecret()
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
/* [SWS_Crypto_91010] */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00111] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00112] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00113] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (partnerPublicValueLength == 0u)
  {
    /* [SWS_Crypto_00115] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  {
    /* ----- Runtime Error Check ------------------------------------------------- */
#if  (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM == STD_ON)
    /* #20 Calculate Secret Value */
    retVal = Crypto_30_Vtt_Local_KeyExchangeCalcSecret(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength);  /* SBSW_CRYPTO_30_VTT_PTR_FORWARDING_WITH_DET_CHECK */
#else
    retVal = E_NOT_OK;
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(partnerPublicValuePtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(partnerPublicValueLength); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #45 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_KEY_EXCHANGE_CALC_SECRET,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Crypto_30_Vtt_CertificateParse()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_CertificateParse(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00168] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00169] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */

    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_CERTIFICATE_PARSE,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_Vtt_CertificateVerify()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_CertificateVerify(
  uint32 cryptoKeyId,
  uint32 verifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_VTT_3673_2 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_Vtt_ModuleInitialized != CRYPTO_30_VTT_INITIALIZED)
  {
    /* [SWS_Crypto_00172] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (cryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00173] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (verifyCryptoKeyId >= Crypto_30_Vtt_GetSizeOfKey())
  {
    /* [SWS_Crypto_00174] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (verifyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00175] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ------------------------------------------------- */
    CRYPTO_30_VTT_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(verifyCryptoKeyId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    CRYPTO_30_VTT_DUMMY_STATEMENT(verifyPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_VTT_MODULE_ID,
      CRYPTO_30_VTT_INSTANCE_ID,
      CRYPTO_30_VTT_SID_CERTIFICATE_VERIFY,
      errorId);
  }
#else
  CRYPTO_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  return retVal;
}

#define CRYPTO_30_VTT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_Vtt_KeyManagement.c
 *********************************************************************************************************************/

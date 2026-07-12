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
/*      \file  Crypto_30_LibCv_InternalApi.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Dispatcher for Key Management Functions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_INTERNALAPI_H)
# define CRYPTO_30_LIBCV_INTERNALAPI_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

typedef struct
{
  boolean isKeyValid;            /* 1 key is valid, 0 key is invalid */
  boolean isSheKey;              /* 1 key is an she key, 0 key is not a she key */
  boolean write_protection;      /* 0 rewriting and erasing enabled, 1 rewriting and erasing disabled */
  /* The following flags are only relevant for SHE keys */
  boolean boot_protection;       /* 0 key usage is not limited according to result of secure boot, 1 usage is limited */
  boolean debugger_protection;   /* 0 use of a debug interface does not impose a limitation on key usage, 1 key can not be used if debugger is attached */
  boolean key_usage;             /* 0 key can be used for AES ECB+CBC encrypt/decrypt, 1 key can be used for CMAC generate/verify */
  boolean disable_wildcard;      /* 0 key update by wildcard UID is enabled, 1 key update by wildcard is disabled */
  boolean cmac_usage;            /* 0 key can be used to execute CMAC generate, 1 key can not be used to execute CMAC generate but only for CMAC verify */
} Crypto_30_LibCv_KeyFlagType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CRYPTO_30_LIBCV_START_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyElementGetStorageIndex()
 *********************************************************************************************************************/
/*! \brief         The key element storage index can be provided for all read access rights values.
 *  \details       This interface shall be used to get a key element of the key identified by the
 *                 cryptoKeyId and store the key element in the memory location pointed to by the result pointer.
 *                 The key element will be searched and checked vor validity.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be provided.
 *  \param[in]     keyElementId            Holds the Id of the key element which shall be provided.
 *  \param[in,out] resultIndexPtr          Holds the pointer to the storage index which shall be provided.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[in]     lengthCheck             Length check mode.
 *  \param[in]     serviceType             SHE service Type.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_SMALL_BUFFER       Request failed, the provided buffer is too small to store the result.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           The crypto key has to be locked for read access.
 *                 resultIndexPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *                 lengthCheck has to be a valid parameter.
 *                 serviceType has to be a valid parameter.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementGetStorageIndex(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_GetKeyFlags()
 *********************************************************************************************************************/
/*! \brief       Retrieving information about key flags
 *  \details     The function checks the key for validity and its key extensions and sets the output buffer.
 *  \param[in]   cryptoKeyId            Holds the identifier of the key
 *  \param[in]   keyElementId           Holds the identifier of the key element
 *  \param[out]  keyFlagsPtr            Contains the pointer to the data where the key flag data shall be stored
 *  \return      E_OK                   Request successful.
 *               E_NOT_OK               Request failed.
 *  \pre         The crypto key has to be locked for read access.
 *               keyFlagsPtr has to be a valid ptr.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_GetKeyFlags(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_KeyFlagType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyFlagsPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyReadLockGetNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key read access success.
 *                  E_NOT_OK           Lock key read access failed.
 * \pre             The caller must ensure that this call is protected by a exclusive area which prohibits
 *                  multiple calls to LibCv Api's.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyReadLockReleaseNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Release key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Release key read access success.
 *                  E_NOT_OK           Request failed, the key is not available.
 * \pre             The caller must ensure that this call is protected by a exclusive area which prohibits
 *                  multiple calls to LibCv Api's.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId);

# if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_GetKeyListRedirection()
 **********************************************************************************************************************/
/*! \brief         Create list of keys for redirection.
 *  \details       A key is only referenced once in the list with the highest access.
 *  \param[in]     job       Pointer to the job which shall checked.
 *  \param[in,out] keyList   Pointer to the keyList.
 *  \pre           job has to be a valid job object.
 *                 keyList has to be a valid keyList object with at least 6 elements if redirection is used, otherwise 1.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_GetKeyListRedirection(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_RedirectPreRedirKeys()
 **********************************************************************************************************************/
/*! \brief         Pre redirect keys and in- and out-buffer.
 *  \details       Set the in- and out-buffer to the redirected keys.
 *  \param[in,out] redirectBuffer               Pointer to the redirection buffer.
 *  \param[in,out] job                          Pointer to the configuration of the job.
 *  \return        E_OK                         Request success.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_EMPTY           Request failed, key is empty.
 *  \pre           Lock has to be reserved for the job object.
 *                 redirectBuffer has to be a valid pointer.
 *                 job has to be a job object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_RedirectPreRedirKeys(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_RedirectPostRestoreBuffer()
 **********************************************************************************************************************/
/*! \brief         Post redirect keys and in- and out-buffer.
 *  \details       Save result and restore in- and out-buffer.
 *  \param[in,out] redirectBuffer   Pointer to the redirection buffer.
 *  \param[in,out] job              Pointer to the configuration of the job.
 *  \param[in]     result           Current update result
 *  \return        E_OK             Request successful.
 *                 E_NOT_OK         Redirect Keys are not available.
 *  \pre           Lock has to be reserved for the job object.
 *                 redirectBuffer has to be a valid pointer.
 *                 job has to be a job object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_RedirectPostRestoreBuffer(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Std_ReturnType result);
# endif /* (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListAddKey()
 **********************************************************************************************************************/
/*! \brief         Add Key to key list.
 *  \details       Add key to keyList if the key is not in keyList. If the Key is in KeyList update keyAccess.
 *  \param[in,out] keyList       Pointer to the keyList.
 *  \param[in]     cryptoKeyId   KeyId of the key which shall be add.
 *  \param[in]     keyAccess     Key access right of the key which shall be add.
 *  \return        E_OK          Request successful.
 *                 E_NOT_OK      Request failed.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListAddKey(
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList,
  uint32 cryptoKeyId,
  uint8 keyAccess);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListPreLockKeys()
 **********************************************************************************************************************/
/*! \brief         Lock keys in key list.
 *  \details       Lock keys in key list according to their read/write access.
 *  \param[in]     keyList   Pointer to the keyList.
 *  \return        E_OK      Request success.
 *                 E_NOT_OK  Request failed, since key is already locked or not available.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListPreLockKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListPostFreeKeys()
 **********************************************************************************************************************/
/*! \brief         Free keys in key list.
 *  \details       Free keys in key list according to their read/write access.
 *  \param[in]     keyList   Pointer to the keyList.
 *  \return        E_OK      Request success.
 *                 E_NOT_OK  Request failed, since key is not available.
 *  \pre           keyList has to be a valid keyList object.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListPostFreeKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyElementSearch()
 *********************************************************************************************************************/
/*! \brief         Searches the key elementIndex.
 *  \details       Searches the key elementIndex of the given keyElementId in the given cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[out]    elementIndex            Holds the pointer to the found key element index.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key or key element not found.
 *  \pre           elementIndex has to be a valid element index Ptr.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyWriteLockGetNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Try to get key write lock.
 * \details         Try to get key write lock. Only one write lock can be allocated, if there is no read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key write access success.
 *                  E_NOT_OK           Lock key write access failed.
 * \pre             The caller has to ensure that this call is protected by a exclusive area.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyWriteLockGetNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyWriteLockReleaseNotProtected()
 *********************************************************************************************************************/
/*!
 * \brief           Release key write lock.
 * \details         Release key write lock. Only one write lock can be allocated.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Request success.
 *                  E_NOT_OK           Request failed as key is not available.
 * \pre             The caller has to ensure that this call is protected by a exclusive area.
 *                  The caller has to ensure that the key is locked for write access.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyWriteLockReleaseNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Api_KeyElementSetInternalStandard()
 *********************************************************************************************************************/
/*! \brief         Copies data to key storage.
 *  \details       Copies data of the keyPtr to the internal key storage identified by the elementIndex.
 *                 If keyLength is zero the old value is deleted.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[in]     keyPtr                  Holds the data which shall be copied to the key storage
 *  \param[in]     keyLength               Specifies the number of bytes which shall be copied.
 *  \return        E_OK                    Request success.
 *                 E_NOT_OK                Request failed as elementIndex is not valid.
 *  \pre           keyLength has to be smaller or equal in length to available key storage space.
 *                 keyPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementSetInternalStandard(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CODE
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CRYPTO_30_LIBCV_INTERNALAPI_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_LIBCV_INTERNALAPI.H
 *********************************************************************************************************************/

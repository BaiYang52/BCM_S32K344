/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_Vtt_Services.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for service dispatch function prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_VTT_SERVICE_H)
# define CRYPTO_30_VTT_SERVICE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Types.h"
# include "Crypto_30_Vtt_Cfg.h"

# if !defined (CRYPTO_30_VTT_LOCAL) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
#  define CRYPTO_30_VTT_LOCAL static
# endif

# if !defined (CRYPTO_30_VTT_LOCAL_INLINE) /* COV_CRYPTO_30_VTT_LOCAL_DEFINE */
#  define CRYPTO_30_VTT_LOCAL_INLINE LOCAL_INLINE
# endif

# if (CRYPTO_30_VTT_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/* Masks for KeyElementState */
# define CRYPTO_30_VTT_KEYELEMENTSTATE_CLEAR_MASK               (0xFFu)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_MASK        (0x04u)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_AES_ROUNDKEY_INV_MASK    (0xFBu)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_SEED_MASK                (0x02u)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_SEED_INV_MASK            (0xFDu)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK               (0x01u)
# define CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_INV_MASK           (0xFEu)

# ifndef CRYPTO_30_VTT_WATCHDOG_PTR /* COV_CRYPTO_30_VTT_WATCHDOG_FCTN_PTR */
#  define CRYPTO_30_VTT_WATCHDOG_PTR                (0u)
# endif

# if (CRYPTO_30_VTT_RSAPKCS1GENERATE == STD_ON) || (CRYPTO_30_VTT_RSAPSSGENERATE == STD_ON)
#  define CRYPTO_30_VTT_RSAGENERATE STD_ON
# else
#  define CRYPTO_30_VTT_RSAGENERATE STD_OFF
# endif

# if (CRYPTO_30_VTT_RSAPKCS1VERIFY == STD_ON) || (CRYPTO_30_VTT_RSAPSSVERIFY == STD_ON)
#  define CRYPTO_30_VTT_RSAVERIFY STD_ON
# else
#  define CRYPTO_30_VTT_RSAVERIFY STD_OFF
# endif

/* General Target Key Element -> always id 1 */
# define CRYPTO_30_VTT_KE_TARGET_KEY        (1u)

/* Size definitions */
# define CRYPTO_30_VTT_CMACAES_MAX_KEY_SIZE (16u)
# define CRYPTO_30_VTT_CMACAES_MAC_SIZE     (16u)
# define CRYPTO_30_VTT_AES_BLOCK_SIZE       (16u)

# define CRYPTO_30_VTT_KEY_LOCK_FREE        (2u)
# define CRYPTO_30_VTT_KEY_LOCK_WRITE       (1u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/* Macros for virtual elements */
# define Crypto_30_Vtt_GetKeyElementVirtualOffset(Index)        (*Crypto_30_Vtt_GetAddrKeyStorage(Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_SetKeyElementVirtualOffset(Index, Value) (*Crypto_30_Vtt_GetAddrKeyStorage(Crypto_30_Vtt_GetKeyStorageStartIdxOfKeyElements((Index)))= (Value)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_GetKeyElementVirtualRoot(Index)          (Crypto_30_Vtt_GetIdOfKeyElements(Crypto_30_Vtt_GetKeyElementInfoVirtualIdxOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((Index))))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_IsKeyElementVirtual(Index)               (Crypto_30_Vtt_IsKeyElementInfoVirtualUsedOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Macros for all elements */
# define Crypto_30_Vtt_IsKeyElementPartial(Index)               (Crypto_30_Vtt_IsPartialOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_GetKeyElementLength(Index)               (Crypto_30_Vtt_GetLengthOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_HasKeyElementInitValue(Index)            (Crypto_30_Vtt_IsInitValueUsedOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_GetKeyElementInitValueLength(Index)      (Crypto_30_Vtt_GetInitValueEndIdxOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements((elementIndex))) - Crypto_30_Vtt_GetInitValueStartIdxOfKeyElementInfo(Crypto_30_Vtt_GetKeyElementInfoIdxOfKeyElements(elementIndex))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_IsKeyElementValid(Index)                 ((Crypto_30_Vtt_GetKeyStorage(Crypto_30_Vtt_GetKeyStorageValidIdxOfKeyElements((Index))) & CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK) ==  CRYPTO_30_VTT_KEYELEMENTSTATE_VALID_MASK) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_SetKeyElementState(Index, Value)         (Crypto_30_Vtt_SetKeyStorage(Crypto_30_Vtt_GetKeyStorageValidIdxOfKeyElements((Index)), (Value))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Crypto_30_Vtt_GetKeyElementState(Index)                (Crypto_30_Vtt_GetKeyStorage(Crypto_30_Vtt_GetKeyStorageValidIdxOfKeyElements((Index)))) /* PRQA S 3453 */ /* MD_MSR_19.7) */
# define Crypto_30_Vtt_ClearKeyElementStateByMask(Index, Mask)  (Crypto_30_Vtt_SetKeyElementState((Index), (uint8)(Crypto_30_Vtt_GetKeyElementState((Index)) & (Mask))))  /* PRQA S 3453 */ /* MD_MSR_19.7) */
# define Crypto_30_Vtt_SetKeyElementStateByMask(Index, Mask)    (Crypto_30_Vtt_SetKeyElementState((Index), (uint8)(Crypto_30_Vtt_GetKeyElementState((Index)) | (Mask))))  /* PRQA S 3453 */ /* MD_MSR_19.7) */

/* EcP Curves */
# if ((CRYPTO_30_VTT_SIGNATURE_GENERATE_ECCNIST_CUSTOM_P256R1_SHA2_256 == STD_ON) || (CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_ECCANSI_CUSTOM_P256R1_SHA2_256 == STD_ON) || (CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_ECCSEC_CUSTOM_P256R1_SHA2_256 == STD_ON))
#  define CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_P256R1 STD_ON
# else
#  define CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_P256R1 STD_OFF
# endif

# if ((CRYPTO_30_VTT_SIGNATURE_VERIFY_ECCNIST_CUSTOM_P256R1_SHA2_256 == STD_ON) || (CRYPTO_30_VTT_SIGNATURE_VERIFY_CUSTOM_ECCANSI_CUSTOM_P256R1_SHA2_256 == STD_ON) || (CRYPTO_30_VTT_SIGNATURE_VERIFY_CUSTOM_ECCSEC_CUSTOM_P256R1_SHA2_256 == STD_ON))
#  define CRYPTO_30_VTT_SIGNATURE_VERIFY_CUSTOM_P256R1 STD_ON
# else
#  define CRYPTO_30_VTT_SIGNATURE_VERIFY_CUSTOM_P256R1 STD_OFF
# endif

/* Mathematic Operation Macros */
# define Crypto_30_Vtt_Math_Mul2(Value) ((Value) << 1)
# define Crypto_30_Vtt_Math_Div2(Value) ((Value) >> 1)

# define Crypto_30_Vtt_Byte2Bit(Byte) ((Byte) << 3)

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

# define CRYPTO_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (CRYPTO_30_VTT_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
extern VAR(uint8, CRYPTO_30_VTT_VAR_ZERO_INIT) Crypto_30_Vtt_ModuleInitialized;
# endif

# define CRYPTO_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define CRYPTO_30_VTT_START_SEC_CONST_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_P256R1 == STD_ON) || (CRYPTO_30_VTT_SIGNATURE_VERIFY_CUSTOM_P256R1 == STD_ON) || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED== STD_ON)
/* NIST_ANSI_SEC_p256r1 START *********************************************************************************************/
/* NISTp256r1 = ANSIp256r1 = SECp256r1 */
extern CONST(uint8, CRYPTO_30_VTT_CONST) Crypto_30_Vtt_EccCurveNistAnsiSecP256R1Domain[227];  /* PRQA S 3218 */ /* MD_CRYPTO_30_VTT_8.7 */
#  if (actBN_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_VTT_CONST) Crypto_30_Vtt_EccCurveNistAnsiSecP256R1DomainExt[219];   /* PRQA S 3218 */ /* MD_CRYPTO_30_VTT_8.7 */
#  elif (actBN_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_VTT_CONST) Crypto_30_Vtt_EccCurveNistAnsiSecP256R1DomainExt[221];   /* PRQA S 3218 */ /* MD_CRYPTO_30_VTT_8.7 */
#  elif (actBN_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_VTT_ACTBN_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_VTT_CONST) Crypto_30_Vtt_EccCurveNistAnsiSecP256R1DomainExt[225];   /* PRQA S 3218 */ /* MD_CRYPTO_30_VTT_8.7 */
#  endif
#  if (CRYPTO_30_VTT_SIGNATURE_GENERATE_CUSTOM_P256R1 == STD_ON) || (CRYPTO_30_VTT_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED== STD_ON)
extern CONST(uint8, CRYPTO_30_VTT_CONST) Crypto_30_Vtt_EccCurveNistAnsiSecP256R1SpeedUpExt[547];   /* PRQA S 3218 */ /* MD_CRYPTO_30_VTT_8.7 */
#  endif
/* NIST_ANSI_SEC_p256r1 END *********************************************************************************************/
# endif

# define CRYPTO_30_VTT_STOP_SEC_CONST_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CRYPTO_30_VTT_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (CRYPTO_30_VTT_USE_VSTD_LIB == STD_OFF)
/**********************************************************************************************************************
*  Crypto_30_Vtt_CopyData()
*********************************************************************************************************************/
/*! \brief         Copies data from source to destination.
 *  \details       Copies data from source to destination for the given length. Exemplary implementation.
 *  \param[in]     sourceData           Pointer to data which shall be copied
 *  \param[in]     dataLength           Number of bytes which shall be copied
 *  \param[out]    targetData           Pointer to buffer where the data shall be copied to
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_CopyData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) sourceData,
  uint32 dataLength);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_ClearData_Implementation()
*********************************************************************************************************************/
/*! \brief         Clears data buffer.
 *  \details       Clears data buffer for the given length (overwrite with 0x00). Exemplary implementation.
 *  \param[in,out] dataBuf                 Pointer to data which shall be cleared
 *  \param[in]     dataLength              Number of bytes which shall be cleared
 *  \pre           Length of the buffer provided by targetDataBuf can hold at least the number of bytes given in dataLength
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_ClearData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) dataBuf,
  uint32 dataLength);
# endif /* # (CRYPTO_30_VTT_USE_VSTD_LIB == STD_OFF) */

# if (CRYPTO_30_VTT_SERVICE_HASH == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchHash()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Hash job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchHash(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchMacGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the MacGenerate job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchMacGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_MAC_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchMacVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the MacVerify job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchMacVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_ENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchCipherEncrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherEncrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchCipherEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_DECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchCipherDecrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the CipherDecrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchCipherDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADENCRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchAeadEncrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AeadEncrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchAeadEncrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_AEADDECRYPT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchAeadDecrypt()
 *********************************************************************************************************************/
/*! \brief          Dispatches the AeadDecrypt job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchAeadDecrypt(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_GENERATE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchSignatureGenerate()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureGenerate job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchSignatureGenerate(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_SIGNATURE_VERIFY == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchSignatureVerify()
 *********************************************************************************************************************/
/*! \brief          Dispatches the SignatureVerify job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchSignatureVerify(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

# if (CRYPTO_30_VTT_SERVICE_RANDOM == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_DispatchRandom()
 *********************************************************************************************************************/
/*! \brief          Dispatches the Random job to primitive layer.
 *  \details        This function dispatches the provided job to the primitives.
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      mode               Operation mode in which the job currently is.
 *  \param[in,out]  job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_DispatchRandom(uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job,
  Crypto_OperationModeType mode);
# endif

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementSearch()
*********************************************************************************************************************/
/*! \brief         Searches the key elementIndex.
 *  \details       Searches the key elementIndex of the given keyElementId in the given cryptoKeyId.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[out]    elementIndex            Holds the pointer to the found key element index.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key element not found.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 elementIndex has to be a valid element index Ptr.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_Vtt_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyElementGet()
*********************************************************************************************************************/
/*! \brief         The key element can be provided for all read access rights values.
 *  \details       This interface shall be used internal to get a key element of the key identified by the cryptoKeyId and store
 *                 the key element in the memory location pointed by the result pointer.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[in]     resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *                 resultPtr has to be a valid ptr.
 *                 resultLengthPtr has to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) resultPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr);

/**********************************************************************************************************************
*  Crypto_30_Vtt_KeyElementSetInternalStandard()
*********************************************************************************************************************/
/*! \brief         Copies data to key storage.
 *  \details       Copies data of the keyPtr to the internal key storage identified by the elementIndex.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[in]     keyPtr                  Holds the data which shall be copied to the key storage
 *  \param[in]     keyLength               Specifies the number of bytes which shall be copied.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_KeyElementSetInternalStandard(
  Crypto_30_Vtt_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) keyPtr,
  uint32 keyLength);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_ReadTwoKeyElements()
*********************************************************************************************************************/
/*!
 * \brief           Load the given key elements and handle return values.
 * \details         This function load the given key elements and handles the return values.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \param[in]       firstKeyElementId  Key Element 1.
 * \param[in,out]   firstKey           Holds the pointer to the key data which shall be set as key element.
 * \param[in,out]   firstKeyLength     Contains the length of the key element in bytes.
 * \param[in]       secondKeyElementId Key Element 2.
 * \param[in,out]   secondKey          Holds the pointer to the key data which shall be set as key element.
 * \param[in,out]   secondKeyLength    Contains the length of the key element in bytes.
 * \return          E_OK               Read key elements success.
 *                  E_NOT_OK           Read key elements failed.
 * \pre             cryptoKeyId has to be a valid identifier to a key in the keyStorage.
 *                  The crypto key has to be locked for read access.
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_ReadTwoKeyElements(
  uint32 cryptoKeyId,
  uint32 firstKeyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) firstKey,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) firstKeyLength,
  uint32 secondKeyElementId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) secondKey,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) secondKeyLength);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyReadLockGet()
*********************************************************************************************************************/
/*!
 * \brief           Try to get key read lock.
 * \details         Try to get a key read lock. Increment the key read lock, if there is no write lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key read access success.
 *                  E_NOT_OK           Lock key read access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockGet(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyReadLockRelease()
*********************************************************************************************************************/
/*!
 * \brief           Release key read lock.
 * \details         Release key read lock. Decrement the key read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockRelease(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyReadLockGetNotProtected()
*********************************************************************************************************************/
/*!
 * \brief           Try to get key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key read access success.
 *                  E_NOT_OK           Lock key read access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected()
*********************************************************************************************************************/
/*!
 * \brief           Release key read lock.
 * \details         This access is not protected by a exclusive area.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 *                  The caller has to ensure that this call is protected by a exclusive area.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyWriteLockGet()
*********************************************************************************************************************/
/*!
 * \brief           Try to get key write lock.
 * \details         Try to get key write lock. Only one write lock can be allocated, if there is no read lock.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \return          E_OK               Lock key write access success.
 *                  E_NOT_OK           Lock key write access failed.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyWriteLockGet(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
*  Crypto_30_Vtt_Local_KeyWriteLockRelease()
*********************************************************************************************************************/
/*!
 * \brief           Release key write lock.
 * \details         Release key write lock. Only one write lock can be allocated.
 * \param[in]       cryptoKeyId        Id of the crypto key.
 * \pre             cryptoKeyId        Has to hold a valid identifier of a key.
 * \context         TASK
 * \reentrant       TRUE, for different keys
 * \synchronous     TRUE
 *********************************************************************************************************************/
extern FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Local_KeyWriteLockRelease(
  uint32 cryptoKeyId);

# define CRYPTO_30_VTT_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRYPTO_30_VTT_SERVICE_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_VTT_SERVICES.H
 *********************************************************************************************************************/

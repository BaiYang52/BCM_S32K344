/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file   Csm_Rte.h
 *      \brief  MICROSAR Crypto Service Manager (CSM) RTE
 *
 *      \details  Description of the MICROSAR Crypto Service Manager (CSM) service APIs used only by RTE
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CSM_RTE_H)
# define CSM_RTE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm.h"

 /* Usage check */
# if !defined CSM_RTE_SOURCE
#  error "This is a private header and should only be included by Csm_Rte.c"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# if (CSM_RTE_PORTS == STD_ON)

#  define CSM_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Csm_CancelJob43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob43x(uint32 jobId, Crypto_OperationModeType mode);

 /**********************************************************************************************************************
 *  Csm_CancelJobNoOpMode()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpMode(uint32 jobId);

/**********************************************************************************************************************
 *  Csm_HashDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Hash().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerate43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/

extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacVerify43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
  uint32 macLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
  uint32 macLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_Encrypt43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_EncryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_Decrypt43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncrypt43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  uint32 plaintextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  uint32 plaintextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecrypt43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  uint32 ciphertextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  uint32 tagLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  uint32 ciphertextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  uint32 tagLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerate43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerify43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify43x(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRef(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRef(uint32 jobId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

 /**********************************************************************************************************************
 *  Csm_KeyElementGet43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementGet().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet43x(uint32 keyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr);

/**********************************************************************************************************************
 *  Csm_CertificateParse43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CertificateParse().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse43x(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_CertificateVerify43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CertificateVerify().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify43x(uint32 keyId,
  uint32 verifyKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_KeyCopy43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyCopy().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy43x(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyDerive43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyDerive().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive43x(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyElementCopy43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementCopy().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy43x(uint32 keyId,
  uint32 keyElementId,
  uint32 targetKeyId,
  uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcPubVal().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal43x(uint32 keyId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcSecret().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret43x(uint32 keyId,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_KeyGenerate43x()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyGenerate().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate43x(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_SaveContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SaveContextJobWithVoidPtr().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJobWithVoidPtr(uint32 jobId,
  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr);

/**********************************************************************************************************************
 *  Csm_RestoreContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RestoreContextJobWithVoidPtr().
 *  \details       See original function for behaviour, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Declared and called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJobWithVoidPtr(uint32 jobId,
  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
  uint32 contextBufferLength);

#  define CSM_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (CSM_RTE_PORTS == STD_ON) */

#endif /* CSM_RTE_H */
/**********************************************************************************************************************
 *  END OF FILE: CSM_RTE_H
 *********************************************************************************************************************/

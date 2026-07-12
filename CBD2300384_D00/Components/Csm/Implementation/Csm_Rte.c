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
/*!        \file  Csm_Rte.c
 *        \brief  MICROSAR Crypto Service Manager (CSM) RTE
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM) service APIs used only by RTE
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CSM_RTE_SOURCE
/* PRQA S 6060 EOF */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm_Rte.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

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
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CSM_RTE_PORTS == STD_ON)

# define CSM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Csm_KeyElementGet43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_KeyElementGet43x( uint32 keyId,
                                                       uint32 keyElementId,
                                                       P2VAR( uint8, AUTOMATIC, CSM_APPL_VAR ) keyPtr,
                                                       P2VAR( uint32, AUTOMATIC, CSM_APPL_VAR ) keyLengthPtr )
{
  /* #10 Call Csm_KeyElementGet */
  return Csm_KeyElementGet( keyId, keyElementId, keyPtr, keyLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_KeyElementGet43x() */

/**********************************************************************************************************************
 *  Csm_HashDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_HashDataRef( uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_Hash */
  return Csm_Hash(  jobId,
                    mode,
                    dataPtr,
                    dataLength,
                    resultPtr,
                    resultLengthPtr  ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_HashDataRef() */

/**********************************************************************************************************************
 *  Csm_MacGenerate43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_MacGenerate43x(  uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr )
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate( jobId,
                          mode,
                          dataPtr,
                          dataLength,
                          macPtr,
                          macLengthPtr  ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_MacGenerate43x() */

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_MacGenerateDataRef( uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr )
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate( jobId,
                          mode,
                          dataPtr,
                          dataLength,
                          macPtr,
                          macLengthPtr  ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_MacGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_MacVerify43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_MacVerify43x(  uint32 jobId,
                                                    Crypto_OperationModeType mode,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                    uint32 dataLength,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                    uint32 macLength,
                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify( jobId,
                        mode,
                        dataPtr,
                        dataLength,
                        macPtr,
                        macLength,
                        verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_MacVerify43x() */

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_MacVerifyDataRef(  uint32 jobId,
                                                        Crypto_OperationModeType mode,
                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                        uint32 dataLength,
                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                        uint32 macLength,
                                                        P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify( jobId,
                        mode,
                        dataPtr,
                        dataLength,
                        macPtr,
                        macLength,
                        verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_MacVerifyDataRef() */

/**********************************************************************************************************************
 *  Csm_Encrypt43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_Encrypt43x( uint32 jobId,
                                                 Crypto_OperationModeType mode,
                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                 uint32 dataLength,
                                                 P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                 P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt( jobId,
                      mode,
                      dataPtr,
                      dataLength,
                      resultPtr,
                      resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_Encrypt43x() */

/**********************************************************************************************************************
 *  Csm_EncryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_EncryptDataRef( uint32 jobId,
                                                     Crypto_OperationModeType mode,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                     uint32 dataLength,
                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt( jobId,
                      mode,
                      dataPtr,
                      dataLength,
                      resultPtr,
                      resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_EncryptDataRef() */

/**********************************************************************************************************************
 *  Csm_Decrypt43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_Decrypt43x(  uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt( jobId,
                      mode,
                      dataPtr,
                      dataLength,
                      resultPtr,
                      resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_Decrypt43x() */

/**********************************************************************************************************************
 *  Csm_DecryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_DecryptDataRef(  uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt( jobId,
                      mode,
                      dataPtr,
                      dataLength,
                      resultPtr,
                      resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_DecryptDataRef() */

/**********************************************************************************************************************
 *  Csm_AEADEncrypt43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_AEADEncrypt43x(  uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                      uint32 plaintextLength,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                      uint32 associatedDataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr )
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(  jobId,
                           mode,
                           plaintextPtr,
                           plaintextLength,
                           associatedDataPtr,
                           associatedDataLength,
                           ciphertextPtr,
                           ciphertextLengthPtr,
                           tagPtr,
                           tagLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_AEADEncrypt43x() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_AEADEncryptDataRef(  uint32 jobId,
                                                          Crypto_OperationModeType mode,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                          uint32 plaintextLength,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                          uint32 associatedDataLength,
                                                          P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                          P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                          P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                          P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr )
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(  jobId,
                           mode,
                           plaintextPtr,
                           plaintextLength,
                           associatedDataPtr,
                           associatedDataLength,
                           ciphertextPtr,
                           ciphertextLengthPtr,
                           tagPtr,
                           tagLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_AEADEncryptDataRef() */

/**********************************************************************************************************************
 *  Csm_AEADDecrypt43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_AEADDecrypt43x( uint32 jobId,
                                                     Crypto_OperationModeType mode,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                     uint32 ciphertextLength,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                     uint32 associatedDataLength,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                     uint32 tagLength,
                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                     P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt( jobId,
                          mode,
                          ciphertextPtr,
                          ciphertextLength,
                          associatedDataPtr,
                          associatedDataLength,
                          tagPtr,
                          tagLength,
                          plaintextPtr,
                          plaintextLengthPtr,
                          verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_AEADDecrypt43x() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_AEADDecryptDataRef( uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                         uint32 ciphertextLength,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                         uint32 associatedDataLength,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                         uint32 tagLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                         P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt( jobId,
                          mode,
                          ciphertextPtr,
                          ciphertextLength,
                          associatedDataPtr,
                          associatedDataLength,
                          tagPtr,
                          tagLength,
                          plaintextPtr,
                          plaintextLengthPtr,
                          verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_AEADDecryptDataRef() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerate43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_SignatureGenerate43x(  uint32 jobId,
                                                            Crypto_OperationModeType mode,
                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                            uint32 dataLength,
                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate( jobId,
                                mode,
                                dataPtr,
                                dataLength,
                                resultPtr,
                                resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_SignatureGenerate43x() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_SignatureGenerateDataRef(  uint32 jobId,
                                                                Crypto_OperationModeType mode,
                                                                P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                uint32 dataLength,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate( jobId,
                                mode,
                                dataPtr,
                                dataLength,
                                resultPtr,
                                resultLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_SignatureGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_SignatureVerify43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_SignatureVerify43x(  uint32 jobId,
                                                          Crypto_OperationModeType mode,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                          uint32 dataLength,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                          uint32 signatureLength,
                                                          P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify( jobId,
                              mode,
                              dataPtr,
                              dataLength,
                              signaturePtr,
                              signatureLength,
                              verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_SignatureVerify43x() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_SignatureVerifyDataRef(  uint32 jobId,
                                                              Crypto_OperationModeType mode,
                                                              P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                              uint32 dataLength,
                                                              P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                              uint32 signatureLength,
                                                              P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr )
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify( jobId,
                              mode,
                              dataPtr,
                              dataLength,
                              signaturePtr,
                              signatureLength,
                              verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_SignatureVerifyDataRef() */

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_RandomGenerateDataRef( uint32 jobId,
                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr )
{
  /* #10 Call Csm_RandomGenerate */
  return Csm_RandomGenerate( jobId,
                             resultPtr,
                             resultLengthPtr  ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_RandomGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_CancelJob43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_CancelJob43x( uint32 jobId, Crypto_OperationModeType mode )
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob( jobId, mode );

} /* Csm_CancelJob43x() */

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_CancelJobNoOpMode( uint32 jobId )
{
  /* #10 Call Csm_CancelJob with arbitrary operation mode (which is never used) */
  return Csm_CancelJob( jobId, CRYPTO_OPERATIONMODE_SINGLECALL);

} /* Csm_CancelJobNoOpMode() */

/**********************************************************************************************************************
 *  Csm_CertificateParse43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse43x( uint32 keyId )
{
  /* #10 Call Csm_CertificateParse */
  return Csm_CertificateParse( keyId );

} /* Csm_CertificateParse43x() */

/**********************************************************************************************************************
 *  Csm_CertificateVerify43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify43x( uint32 keyId,
                                                         uint32 verifyKeyId,
                                                         P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_CertificateVerify */
  return Csm_CertificateVerify( keyId,
                                verifyKeyId,
                                verifyPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_CertificateVerify43x() */

/**********************************************************************************************************************
 *  Csm_KeyCopy43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy43x( uint32 keyId,
                                               uint32 targetKeyId )
{
  /* #10 Call Csm_KeyCopy */
  return Csm_KeyCopy( keyId, targetKeyId );

} /* Csm_KeyCopy43x() */

/**********************************************************************************************************************
 *  Csm_KeyDerive43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive43x( uint32 keyId, uint32 targetKeyId )
{
  /* #10 Call Csm_KeyDerive */
  return Csm_KeyDerive( keyId, targetKeyId );
} /* Csm_KeyDerive43x() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopy43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy43x( uint32 keyId,
                                                      uint32 keyElementId,
                                                      uint32 targetKeyId,
                                                      uint32 targetKeyElementId )
{
  /* #10 Call Csm_KeyElementCopy */
  return Csm_KeyElementCopy( keyId,
                             keyElementId,
                             targetKeyId,
                             targetKeyElementId );
} /* Csm_KeyElementCopy43x() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal43x( uint32 keyId,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr )
{
  /* #10 Call Csm_KeyExchangeCalcPubVal */
  return Csm_KeyExchangeCalcPubVal( keyId,
                                    publicValuePtr,
                                    publicValueLengthPtr ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_KeyExchangeCalcPubVal43x() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret43x( uint32 keyId,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                             uint32 partnerPublicValueLength )
{
  /* #10 Call Csm_KeyExchangeCalcSecret */
  return Csm_KeyExchangeCalcSecret( keyId,
                                    partnerPublicValuePtr,
                                    partnerPublicValueLength ); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_KeyExchangeCalcSecret43x() */

/**********************************************************************************************************************
 *  Csm_KeyGenerate43x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate43x( uint32 keyId )
{
  /* #10 Call Csm_KeyGenerate */
  return Csm_KeyGenerate( keyId );

} /* Csm_KeyGenerate43x() */

/**********************************************************************************************************************
 *  Csm_SaveContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJobWithVoidPtr(uint32 jobId,
  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr)
{
  /* #10 Call Csm_SaveContextJob */
  return Csm_SaveContextJob(jobId,
    (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))contextBufferPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
    (P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR))contextBufferLengthPtr); /* PRQA S 0316 */ /* MD_CSM_0316 */ /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_SaveContextJobWithVoidPtr() */

/**********************************************************************************************************************
 *  Csm_RestoreContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJobWithVoidPtr(uint32 jobId,
  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
  uint32 contextBufferLength)
{
  /* #10 Call Csm_RestoreContextJob */
  return Csm_RestoreContextJob(jobId,
    (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))contextBufferPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
    contextBufferLength);/* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */

} /* Csm_RestoreContextJobWithVoidPtr() */

# define CSM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (CSM_RTE_PORTS == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: CSM.C
 *********************************************************************************************************************/

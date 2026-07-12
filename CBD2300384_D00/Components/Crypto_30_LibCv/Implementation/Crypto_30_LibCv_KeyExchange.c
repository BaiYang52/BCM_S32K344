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
/*      \file  Crypto_30_LibCv_KeyExchange.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of Key Exchange Services
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_KEYEXCHANGE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_KeyExchange.h"
#include "Crypto_30_LibCv_KeyGenerate.h"
#include "Crypto_30_LibCv_Custom.h"
#include "Crypto_30_LibCv_Curve.h"

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
# include "actIX25519.h"
#endif

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON)
# include "ESLib_SPAKE2PConstants.h"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ECDHE */
#define CRYPTO_30_LIBCV_ECDHE_256_ID                                  (1u)
#define CRYPTO_30_LIBCV_ECDHE_384_ID                                  (2u)
#define CRYPTO_30_LIBCV_ECDHE_521_ID                                  (3u)

/* Spake2+ */
#define CRYPTO_30_LIBCV_SPAKE2P_MODE_NORMAL                           (0u)
#define CRYPTO_30_LIBCV_SPAKE2P_MODE_CHANGED_VERIFICATION             (1u)

#define CRYPTO_30_LIBCV_SPAKE2P_STATE_UNINIT                          (0u)
#define CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_PUBVAL                     (1u)
#define CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_SECRET                     (2u)
#define CRYPTO_30_LIBCV_SPAKE2P_STATE_VERIFICATION                    (3u)

/* Elliptic curve with burmester desmedt */
#define CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_NUM_ECU_LENGTH                (1u)
#define CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_ECU_ID_LENGTH                 (1u)
#define CRYPTO_30_LIBCV_ECDHE_BD_MIN_NUM_ECU                          (3u)

#define CRYPTO_30_LIBCV_ECBD_STATE_UNINIT                             (0u)
#define CRYPTO_30_LIBCV_ECBD_STATE_CALC_PUBVAL                        (1u)
#define CRYPTO_30_LIBCV_ECBD_STATE_CALC_INTERMEDIATE                  (2u)
#define CRYPTO_30_LIBCV_ECBD_STATE_REC_INTERMEDIATE                   (3u)
#define CRYPTO_30_LIBCV_ECBD_STATE_CALC_SECRET                        (4u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_A_Pre()
 **********************************************************************************************************************/
/*! \brief         Set Preamble Spake2+ Party A
 *  \details       The service stores w0 and w1 in the workspace and sets a preamble flag in the workspace.
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[in]     w0Ptr                   Pointer to w0
 *  \param[in]     w0Length                Length of w0
 *  \param[in]     w1Ptr                   Pointer to w1
 *  \param[in]     w1Length                Length of w1
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be a valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_A_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w1Ptr,
  uint32 w1Length);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_B_Pre()
 **********************************************************************************************************************/
/*! \brief         Set Preamble Spake2+ Party B
 *  \details       The service stores w0 and l in the workspace and sets a preamble flag in the workspace.
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *  \param[in]     w0Ptr                   Pointer to w0
 *  \param[in]     w0Length                Length of w0
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointers need to be a valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_B_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Pre()
 **********************************************************************************************************************/
/*! \brief         Set Preamble Spake2+
 *  \details       Set Preamble Spake2+ for Party A or Party B
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointers need to be a valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Public()
 **********************************************************************************************************************/
/*! \brief         Calculation of the public value according to Spake2+
 *  \details       -
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointers need to be a valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Public(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_Calc()
 **********************************************************************************************************************/
/*! \brief         Calculation of the public value according to Spake2+
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     mode                    Contain the Spake2+ calculation Mode
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointers need to be a valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_Calc(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  uint8 mode);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P()
 **********************************************************************************************************************/
/*! \brief         Calculation of the public value according to Spake2+
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     mode                    Contain the Spake2+ calculation Mode
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointers need to be a valid.
 *  \note          This service locks the long term workspace if possible.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  uint8 mode);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead()
 **********************************************************************************************************************/
/*! \brief         Read single data element.
 *  \details       [info-a | info-b | aad] each [DataLength 4-Byte | Data X-Byte]
 *  \param[in]     infoPtr            Holds the pointer to the memory location which contains the info.
 *  \param[in]     infoLength         Holds the Length of the infoPtr.
 *  \param[in,out] readPos            Holds the current read position.
 *  \param[out]    dataPos            Holds the position of the data.
 *  \param[out]    dataLength         Holds the Length of the dataPos.
 *  \return        E_OK               Request successful.
 *                 E_NOT_OK           Request failed.
 *  \pre           All pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) infoPtr,
  uint32 infoLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) readPos,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dataPos,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dataLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_AdditionalInfo()
 **********************************************************************************************************************/
/*! \brief         Set additional info for secret for Spake2+
 *  \details       [info-a | info-b | aad] each [DataLength 4-Byte | Data X-Byte]
 *  \param[in,out] ws                 Contains the workspace.
 *  \param[in]     infoPtr            Holds the pointer to the memory location which contains the info.
 *  \param[in]     infoLength         Holds the Length of the infoPtr.
 *  \param[in]     infoElementRetVal  Holds the retval from the key element operation of info data
 *  \return        E_OK               Request successful.
 *                 E_NOT_OK           Request failed.
 *  \pre           All pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_AdditionalInfo(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) infoPtr,
  uint32 infoLength,
  Std_ReturnType infoElementRetVal);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Secret()
 **********************************************************************************************************************/
/*! \brief         Calculation of the secret for Spake2+
 *  \details       Secret is calculation.
 *  \param[in,out] ws                       Contains the workspace.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the Length of the partnerPublicValuePtr.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Secret(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Verification()
 **********************************************************************************************************************/
/*! \brief         Key confirmation for Spake2+
 *  \details       -
 *  \param[in,out] ws                      Contains the workspace.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerConfirmationValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          confiramtion value.
 *  \param[in]     partnerConfirmationValueLength Holds the Length of the partnerConfirmationValuePtr.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Verification(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerConfirmationValuePtr,
  uint32 partnerConfirmationValueLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P()
 **********************************************************************************************************************/
/*! \brief         Calculation of the secret for Spake2+
 *  \details       Depeneding on the state either secret is calculation or key confirmation is executed.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public/confirmation value.
 *  \param[in]     partnerPublicValueLength Holds the Length of the partnerPublicValuePtr.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 All pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value for x25519
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     speedUpExtPtr           Pointer to precomputation structure
 *  \param[in]     keySize                 Contains the length of the key element in bytes.
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the Length of the partnerPublicValuePtr.
 *  \param[in]     keySize                  Contains the length of the key element in bytes.
 *  \param[in]     keaId                    Key exchange algorithm identifier.
 *  \param[in,out] workspace                Contains the workspace.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer with size 2*CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength,
  uint32 keySize,
  uint8 keaId,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \param[in]     domainPtr               Pointer to domain parameter structure
 *  \param[in]     domainExtPtr            Pointer to domain parameter extension structure
 *  \param[in]     speedUpExtPtr           Pointer to precomputation structure
 *  \param[in]     keySize                 Contains the length of the key element in bytes.
 *  \param[in,out] workspace               Contains the workspace.
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId                    Holds the identifier of the key which shall be used for the key exchange
 *                                                protocol.
 *  \param[in]     partnerIntermediateValuePtr    Holds the pointer to the memory location which contains the intermediate
 *                                                value. The value must contain the following format
 *                                                [EcuId (1Byte) | Partner intermediate value X | Partner intermediate value Y].
 *  \param[in]     partnerIntermediateValueLength Holds the Length of the partnerIntermediateValuePtr.
 *  \return        E_OK                           Request successful.
 *                 E_NOT_OK                       Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerIntermediateValuePtr has to be a valid pointer with size (1 + CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY).
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerIntermediateValuePtr,
  uint32 partnerIntermediateValueLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_First()
 *********************************************************************************************************************/
/*! \brief         First calc secret
 *  \details       Init calc secret
 *  \param[in,out] wsPtr            Pointer to the Crypto_30_LibCv_WorkSpaceECBD workspace.
 *  \return        E_OK             Request successful.
 *                 E_NOT_OK         Request failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_First(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Update()
 *********************************************************************************************************************/
/*! \brief         Update calc secret with intermediate value of next key exchange partner.
 *  \details       -
 *  \param[in,out] wsPtr            Pointer to the Crypto_30_LibCv_WorkSpaceECBD workspace.
 *  \param[in]     partnerIntermediateValuePtr    Holds the pointer to the memory location which contains the intermediate
 *                                                value. The value must contain the following format
 *                                                [EcuId (1Byte) | Partner intermediate value X | Partner intermediate value Y].
 *  \param[in]     partnerIntermediateValueLength Holds the Length of the partnerIntermediateValuePtr.
 *  \return        E_OK                           Request successful.
 *                 E_NOT_OK                       Request failed.
 *  \pre           partnerIntermediateValuePtr has to be a valid pointer with size (1 + CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY).
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Update(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerIntermediateValuePtr,
  uint32 partnerIntermediateValueLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Finish()
 *********************************************************************************************************************/
/*! \brief         Calculate shared secret
 *  \details       -
 *  \param[in,out] wsPtr            Pointer to the Crypto_30_LibCv_WorkSpaceECBD workspace.
 *  \param[in]     cryptoKeyId      Holds the identifier of the key which shall be used for the key exchange protocol.
 *  \return        E_OK             Request successful.
 *                 E_NOT_OK         Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Finish(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON) */

#endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) */

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPublicValuePtr   Holds the pointer to the memory location which contains the partners
 *                                         public value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer with size ESL_SIZEOF_X25519_PUBLIC_KEY.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Calculation of the public value for x25519
 *  \details       Calculates the public value for the key exchange and stores the public key in the
 *                 memory location pointed to by the public value pointer. The write access right for the destination
 *                 key element must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \param[in,out] wsX25519                Contains the workspace.
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsX25519);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[in]     partnerPublicValuePtr   Holds the pointer to the memory location which contains the partners
 *                                         public value.
 *  \param[in,out] wsX25519                Contains the workspace.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsX25519);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic()
 *********************************************************************************************************************/
/*! \brief         Calculation of the secret
 *  \details       Calculates the shared secret key for the key exchange with the key material of the key identified
 *                 by the cryptoKeyId and the partner public key. The shared secret key is stored
 *                 as a key element in the same key. The write access right for the destination key element
 *                 must be less than or equal to WA_INTERNAL_COPY.
 *  \param[in]     cryptoKeyId              Holds the identifier of the key which shall be used for the key exchange
 *                                          protocol.
 *  \param[in]     partnerPublicValuePtr    Holds the pointer to the memory location which contains the partners
 *                                          public value.
 *  \param[in]     partnerPublicValueLength Holds the Length of the partnerPublicValuePtr.
 *  \param[in]     keySize                  Contains the length of the key element in bytes.
 *  \param[in]     keaId                    Key exchange algorithm identifier.
 *  \return        E_OK                     Request successful.
 *                 E_NOT_OK                 Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 partnerPublicValuePtr has to be a valid pointer with size 2*CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength,
  uint32 keySize,
  uint8 keaId);
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GetPartnerPub()
 *********************************************************************************************************************/
/*! \brief         Load left and right public value
 *  \details       Load the left and right public value is ready to use.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    leftStorageIndexPtr     Pointer to the storage index of the left public value.
 *  \param[out]    rightStorageIndexPtr    Pointer to the storage index of the right public value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 all pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, for different keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GetPartnerPub(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyStorageIndexType, AUTOMATIC, AUTOMATIC) leftStorageIndexPtr,
  P2VAR(Crypto_30_LibCv_KeyStorageIndexType, AUTOMATIC, AUTOMATIC) rightStorageIndexPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair_Init()
 *********************************************************************************************************************/
/*! \brief         Init CalcPubVal elliptic curve burmester desmedt.
 *  \details       -
 *  \param[in,out] wsPtr                   Pointer to the Crypto_30_LibCv_WorkSpaceECBD workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 all pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair_Init(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair()
 *********************************************************************************************************************/
/*! \brief         Generate own public and private key.
 *  \details       Generate own public and private key according to the given curve.
 *  \param[in,out] wsPtr                   Pointer to the Crypto_30_LibCv_WorkSpaceECBD workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr need to have at lest the length of publicValueLengthPtr.
 *                 all pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, true for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_CalcIntermediate()
 *********************************************************************************************************************/
/*! \brief         Generate intermediate value.
 *  \details       Generate  intermediate value according to the given partner public values.
 *  \param[in,out] wsPtr                   Pointer to the eslt_WorkSpaceECBD workspace.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the intermediate value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr need to have at lest the length of publicValueLengthPtr.
 *                 Element index need to be valid index.
 *                 all pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE, true for different workspaces.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_CalcIntermediate(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace()
 *********************************************************************************************************************/
/*! \brief         Delete workspace if it is locked for key.
 *  \details       -
 *  \param[in]     longWsIdx             Holds the valid identifier of the long term workspace.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace(
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys()
 *********************************************************************************************************************/
/*! \brief         Delete all required key elements in key.
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys(
  uint32 cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime()
 *********************************************************************************************************************/
/*! \brief         Dispatch public value genartion.
 *  \details       Generate own public and private key according to the given curve or
 *                 intermediate value according to the given partner public values.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key which shall be used for the key exchange
 *                                         protocol.
 *  \param[out]    publicValuePtr          Contains the pointer to the data where the public value shall be stored.
 *  \param[in,out] publicValueLengthPtr    Holds a pointer to the memory location in which the public value length
 *                                         information is stored. On calling this function, this parameter shall
 *                                         contain the size of the buffer provided by publicValuePtr. When the request
 *                                         has finished, the actual length of the returned value shall be stored.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId has to be a valid crypto key.
 *                 publicValuePtr need to have at lest the length of publicValueLengthPtr.
 *                 all pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr);

# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON) */
#endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON))

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_EcP_CalculateSharedSecret_With_Ws()
 *********************************************************************************************************************/
 /*! \brief          Calculate shared secret
  *  \details        Calculate shared secret for ECDHE Prime for the given values.
  *  \param[in]      privateKeyPtr           Pointer to the private key array.
  *  \param[in]      partnerPubKeyPtr        Pointer to the partner public key array.
  *  \param[out]     sharedSecretPtr         Pointer to the shared secret.
  *  \param[in,out]  workspace               Contains workspace.
  *  \param[in]      EcDomainPtr             Pointer to the curve domain parameters.
  *  \param[in]      EcDomainExtPtr          Pointer to the extended curve domain parameters.
  *  \return         E_OK                    Request successful.
  *                  E_NOT_OK                Request failed.
  *  \pre            privateKeyPtr has to be a valid pointer with a valid length.
  *                  partnerPubKeyPtr has to be a valid pointer with a valid length.
  *                  sharedSecretPtr has to be a valid pointer with the length 2*esl_getLengthOfEcPsecret_comp(EcDomainPtr).
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_EcP_CalculateSharedSecret_With_Ws(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr);

#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON)

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_A_Pre()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_A_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w1Ptr,
  uint32 w1Length)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* ----- Implementation ------------------------------------------------- */

  /* Set Preamble Members for A */
  eslRet = esl_setPreambleDataPartyASPAKE2P(ws, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))w0Ptr, (eslt_Length)w0Length,
    (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))w1Ptr, (eslt_Length)w1Length);

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_B_Pre()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_B_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) w0Ptr,
  uint32 w0Length)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode eslRet;
  Crypto_30_LibCv_SizeOfKeyStorageType  lElementIndex;
  uint32  lElementLength;
  /* ----- Implementation ------------------------------------------------- */

  /* Get L */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_L, &lElementIndex, &lElementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  if (localRetVal == E_OK)
  {
    /* Set Preamble Members for B */
    eslRet = esl_setPreambleDataPartyBSPAKE2P(ws, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))w0Ptr, (eslt_Length)w0Length,
                                              (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(lElementIndex), (eslt_Length)lElementLength);

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Pre()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Pre(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  Crypto_30_LibCv_SizeOfKeyStorageType w0ElementIndex, w1ElementIndex;
  uint32 w0ElementLength, w1ElementLength;

  /* ----- Implementation ------------------------------------------------- */

  /* Get w0 */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_W0, &w0ElementIndex, &w0ElementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRetVal == E_OK)
  {
    /* Get w1 to determine if this is Party A or Party B */
    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_W1, &w1ElementIndex, &w1ElementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (localRetVal != CRYPTO_E_KEY_NOT_AVAILABLE)
    {
      if (localRetVal == E_OK)
      {
        /* Party A */
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_A_Pre(ws, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                                                           Crypto_30_LibCv_GetAddrKeyStorage(w0ElementIndex), w0ElementLength,
                                                                           Crypto_30_LibCv_GetAddrKeyStorage(w1ElementIndex), w1ElementLength);
      }
    }
    else
    {
      /* Party B */
      retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_B_Pre(ws, cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                                                         Crypto_30_LibCv_GetAddrKeyStorage(w0ElementIndex), w0ElementLength);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Public()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Public(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  eslt_Length reqLength;

  /* ----- Implementation ------------------------------------------------- */
  /* Set Sizes for Chipher Suite */
  reqLength = CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC;

  /* Check Length Calc Public Value  */
  /* Public Value = [pubVal_x | pubVal_y] */
  if ((Crypto_30_LibCv_Math_Mul2((uint32)reqLength)) > *publicValueLengthPtr)
  {
    retVal = CRYPTO_E_SMALL_BUFFER;
  }
  else
  {
    eslRet = esl_calcPubValSPAKE2P(ws, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                  (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))publicValuePtr,
                                  (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&publicValuePtr[reqLength],
                                  (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&reqLength);

    if (eslRet == ESL_ERC_NO_ERROR)
    {
      *publicValueLengthPtr = Crypto_30_LibCv_Math_Mul2((uint32)reqLength); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
    else
    {
      Crypto_30_LibCv_ClearData(publicValuePtr, *publicValueLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_Calc()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_Calc(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  uint8 mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  eslt_SPAKE2PMode spakeMode;

  /* ----- Implementation ------------------------------------------------- */

  /* Init Workspace */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&ws->header, ESL_SIZEOF_WS_SPAKE2P, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (mode == CRYPTO_30_LIBCV_SPAKE2P_MODE_NORMAL)
  {
    spakeMode = ESL_SPAKE2P_MODE_CIPHERSUITE_8_1;
  }
  else
  {
    spakeMode = ESL_SPAKE2P_MODE_CIPHERSUITE_8_2;
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initSPAKE2P(ws,
                             domainPtr,
                             domainExtPtr,
                             (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))esl_SPAKE2P_pointM_P256,
                             (eslt_Length)sizeof(esl_SPAKE2P_pointM_P256),
                             (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))esl_SPAKE2P_pointN_P256,
                             (eslt_Length)sizeof(esl_SPAKE2P_pointN_P256),
                             spakeMode); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {

    /* Set Preamble */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Pre(ws, cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */

    if (retVal == E_OK)
    {
      /* Calc Public Value */
      retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2_Public(ws, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050, 6060 8 */ /* MD_MSR_STCAL, MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  uint8 mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  P2VAR(Crypto_30_LibCv_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws;
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx;

  /* ----- Implementation ------------------------------------------------- */
  /* Lock long term workspace */
  if (Crypto_30_LibCv_Local_LongWsLockGet(cryptoKeyId, &longWsIdx) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {

    ws = (P2VAR(Crypto_30_LibCv_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsSPAKE2P;
    ws->state = CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_PUBVAL;  /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */

    /* Calculate public value */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P_Calc(cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
                                                                     publicValuePtr, publicValueLengthPtr,
                                                                     &ws->wsSpake,
                                                                     domainPtr,
                                                                     domainExtPtr,
                                                                     mode);
    if (retVal != E_OK)
    {
      /* Release long term workspace */
      Crypto_30_LibCv_Local_LongWsLockRelease(cryptoKeyId, longWsIdx);
    }
  }
  else
  {
    retVal = CRYPTO_E_BUSY;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) infoPtr,
  uint32 infoLength,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) readPos,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dataPos,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dataLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint32 length;
  /* ----- Implementation ------------------------------------------------- */
  *dataLength = 0u; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

  /* [DataLength 4-Byte | Data X-Byte] */
  if ((infoLength >= (*readPos + CRYPTO_30_LIBCV_SIZEOF_UINT32)) && !Crypto_30_LibCv_IsUint32Overflow(*readPos, CRYPTO_30_LIBCV_SIZEOF_UINT32))
  {
    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&length, &infoPtr[*readPos]); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    *readPos += CRYPTO_30_LIBCV_SIZEOF_UINT32; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

    if ((infoLength >= (*readPos + length)) && !Crypto_30_LibCv_IsUint32Overflow(*readPos, length))
    {
      *dataLength = length; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *dataPos = *readPos; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *readPos += length; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_AdditionalInfo()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_AdditionalInfo(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) infoPtr,
  uint32 infoLength,
  Std_ReturnType infoElementRetVal)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode eslRet;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) aPtr = NULL_PTR;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) bPtr = NULL_PTR;
  P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) aadPtr = NULL_PTR;
  uint32 aLength, bLength, aadLength, readPos, dataPos;
  /* ----- Implementation ------------------------------------------------- */

  readPos = 0u;
  dataPos = 0u;
  if (infoElementRetVal == E_OK)
  {
    /* Read info-a */
    localRetVal = Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead(infoPtr, infoLength, &readPos, &dataPos, &aLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    if (aLength != 0u)
    {
      aPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&infoPtr[dataPos]; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }

    /* Read info-b */
    localRetVal |= Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead(infoPtr, infoLength, &readPos, &dataPos, &bLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    if (bLength != 0u)
    {
      bPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&infoPtr[dataPos]; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }

    /* Read aad */
    localRetVal |= Crypto_30_LibCv_Local_Spake2P_AdditionalInfoRead(infoPtr, infoLength, &readPos, &dataPos, &aadLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    if (aadLength != 0u)
    {
      aadPtr = (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&infoPtr[dataPos]; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }

    if (localRetVal == E_OK)
    {
      eslRet = esl_setAdditionalInformationSPAKE2P(ws, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                                   aPtr, (eslt_Length)aLength,
                                                   bPtr, (eslt_Length)bLength,
                                                   aadPtr, (eslt_Length)aadLength);
      if (eslRet == ESL_ERC_NO_ERROR)
      {
        retVal = E_OK;
      }
    }

  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Secret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Secret(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal;
  eslt_ErrorCode eslRet;
  eslt_Length halfLength, secretLength, verificationLength;
  Crypto_30_LibCv_SizeOfKeyStorageType elementIndex;
  uint32 elementLength;
  uint8 secret[ESL_SIZEOF_SHA256_DIGEST /2u];
  uint8 verification[CRYPTO_30_LIBCV_CMACAES_MAC_SIZE];

  /* ----- Implementation ------------------------------------------------- */
  secretLength = ESL_SIZEOF_SHA256_DIGEST /2u;
  verificationLength = CRYPTO_30_LIBCV_CMACAES_MAC_SIZE;

  /* Set Additional Info (only if required) */
  localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_ADDITIONAL_INFO, &elementIndex, &elementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

  if (localRetVal != CRYPTO_E_KEY_NOT_AVAILABLE)
  {
    localRetVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_AdditionalInfo(ws, Crypto_30_LibCv_GetAddrKeyStorage(elementIndex), elementLength, localRetVal); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
  }
  else
  {
    localRetVal = E_OK;
  }

  /* Calc Secret */
  if (((Crypto_30_LibCv_Math_Mul2((uint32)CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PUBLIC)) == partnerPublicValueLength) && (localRetVal == E_OK))  /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
  {
    halfLength = (eslt_Length)Crypto_30_LibCv_Math_Div2(partnerPublicValueLength);
    {
      eslRet = esl_calcSharedSecretSPAKE2P(ws,
                                           partnerPublicValuePtr, &partnerPublicValuePtr[halfLength], halfLength,
                                           secret, &secretLength,
                                           verification, &verificationLength); /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */

      if (eslRet == ESL_ERC_NO_ERROR)
      {
        /* Store calculated sharedSecret */
        localRetVal = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, secret, (uint32)secretLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        localRetVal |= Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_VERIFICATION, verification, (uint32)verificationLength);/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        if (localRetVal == E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_NOT_OK;
        }
      }

      /* delete buffer content from RAM */
      Crypto_30_LibCv_ClearData(secret, ESL_SIZEOF_SHA256_DIGEST /2u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      Crypto_30_LibCv_ClearData(verification, CRYPTO_30_LIBCV_CMACAES_MAC_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Verification()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Verification(
  P2VAR(eslt_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws,
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerConfirmationValuePtr,
  uint32 partnerConfirmationValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRetVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  uint8 verification;

  /* ----- Implementation ------------------------------------------------- */
  verification = CRYPTO_E_VER_NOT_OK;

  /* Check verification */
  eslRet = esl_confirmKeySPAKE2P(ws, partnerConfirmationValuePtr, (eslt_Length)partnerConfirmationValueLength); /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  if (eslRet == ESL_ERC_INCORRECT_MAC)
  {
    localRetVal = E_OK;
  }
  else if (eslRet == ESL_ERC_NO_ERROR)
  {
    localRetVal = E_OK;
    verification = CRYPTO_E_VER_OK;
  }
  else
  {
      /* Nothing to do */
  }

  /* Store verification result */
  if (Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_VERIFICATION_RESULT, &verification, 1u) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    retVal = localRetVal;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  P2VAR(Crypto_30_LibCv_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) ws;
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx;

  /* ----- Implementation ------------------------------------------------- */
  /* Check if long term workspace is locked */
  if (Crypto_30_LibCv_Local_LongWsIsLock(cryptoKeyId, &longWsIdx) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {

    ws = (P2VAR(Crypto_30_LibCv_WorkSpaceSPAKE2P, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsSPAKE2P;
    if (ws->state == CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_PUBVAL)
    {
      /* Calc Secret */
      ws->state = CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_SECRET; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Secret(&ws->wsSpake, cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
    }
    else /*  (ws->state == CRYPTO_30_LIBCV_SPAKE2P_STATE_CALC_SECRET) */
    {
      /* Verification */
      ws->state = CRYPTO_30_LIBCV_SPAKE2P_STATE_VERIFICATION; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
      retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P_Verification(&ws->wsSpake, cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR */
    }

    /* release lock if request is finished */
    if (retVal != E_OK)
    {
      Crypto_30_LibCv_ClearData(ws, sizeof(Crypto_30_LibCv_WorkSpaceSPAKE2P)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
      Crypto_30_LibCv_Local_LongWsLockRelease(cryptoKeyId, longWsIdx);
    }
    else if (ws->state == CRYPTO_30_LIBCV_SPAKE2P_STATE_VERIFICATION)
    {
      Crypto_30_LibCv_Local_LongWsLockRelease(cryptoKeyId, longWsIdx);
    }
    else
    {
      /* keep workspace locked, request is continued */
    }

  }
  return retVal;
}

#endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Init Workspace */
  eslt_WorkSpaceX25519 localWsX25519;

  /* #20 Call Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws */
  return Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws(cryptoKeyId, publicValuePtr, publicValueLengthPtr, &localWsX25519); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws()
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
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsX25519)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 publicKeyBuf[ESL_SIZEOF_X25519_PUBLIC_KEY];
  eslt_Length publicKeyLength = ESL_SIZEOF_X25519_PUBLIC_KEY;
  eslt_ErrorCode eslRet;
  Std_ReturnType retValKeyElementSetPrivateKey = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retValKeyElementSetPublicKey = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */

  /* #20 Init Workspace */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&wsX25519->header, ESL_SIZEOF_WS_X25519, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initECDH(wsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 Calculate Keys */

    if (esl_generateEphemeralKeyPairECDH(wsX25519, publicKeyBuf, &publicKeyLength) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
    {
      /* #30 Store public and private key to key elements if calculation was successfully */
      retValKeyElementSetPrivateKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))(((actX25519Struct*)wsX25519->wsX25519)->privateKey), ESL_SIZEOF_X25519_PRIVATE_KEY);  /* PRQA S 3305, 0310 */ /* MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST, MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      retValKeyElementSetPublicKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, (P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))publicKeyBuf, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }

    if ((retValKeyElementSetPrivateKey == E_OK) &&
      (retValKeyElementSetPublicKey == E_OK))
    {
      /* #35 Copy public key to output buffer and overwrite local copy if store key was successfully */
      /* [SWS_Crypto_00109] return public key */
      if (*publicValueLengthPtr >= ESL_SIZEOF_X25519_PUBLIC_KEY)
      {

        Crypto_30_LibCv_CopyData(publicValuePtr, publicKeyBuf, ESL_SIZEOF_X25519_PUBLIC_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
        Crypto_30_LibCv_ClearData(publicKeyBuf, ESL_SIZEOF_X25519_PUBLIC_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
        *publicValueLengthPtr = ESL_SIZEOF_X25519_PUBLIC_KEY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
        retVal = E_OK;
      }
      else
        /* #40 If buffer is too small return with error */
      {
        retVal = CRYPTO_E_SMALL_BUFFER;
      }
    }
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize)
{
  /* #10 Init Workspace */
  eslt_WorkSpaceEcP workspace;

  /* #20 Call Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws */
  return Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                     publicValuePtr,
                                                                     publicValueLengthPtr,
                                                                     domainPtr,
                                                                     domainExtPtr,
                                                                     speedUpExtPtr,
                                                                     keySize,
                                                                     &workspace);
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws()
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
/* PRQA S 6050, 6060, 6080 9 */ /* MD_MSR_STCAL, MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) domainExtPtr,
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) speedUpExtPtr,
  uint32 keySize,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 privKey[CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE];
  uint8 pubKey[Crypto_30_LibCv_Math_Mul2(CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE)];
  uint32 doubleKeySize;
  Std_ReturnType retValKeyElementSetPrivateKey = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retValKeyElementSetPublicKey = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Calculate Keys */
  if (Crypto_30_LibCv_Local_Ecc_Calculate_With_Ws(pubKey, privKey, domainPtr, domainExtPtr, speedUpExtPtr, keySize, workspace) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    doubleKeySize = Crypto_30_LibCv_Math_Mul2(keySize);

    /* #11 Store public and private key to key elements if calculation was successfully */
    retValKeyElementSetPublicKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, pubKey, doubleKeySize); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    retValKeyElementSetPrivateKey = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, privKey, keySize); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if ((retValKeyElementSetPrivateKey == E_OK) &&
      (retValKeyElementSetPublicKey == E_OK))
    {
      /* #12 Copy public key to output buffer and overwrite local copy if store key was successfully */
      /* [SWS_Crypto_00109] return public key */
      if (*publicValueLengthPtr >= doubleKeySize)
      {

        Crypto_30_LibCv_CopyData(publicValuePtr, pubKey, doubleKeySize); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */

        /* #13 delete buffer content from RAM */
        Crypto_30_LibCv_ClearData(privKey, keySize); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
        Crypto_30_LibCv_ClearData(pubKey, doubleKeySize); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        *publicValueLengthPtr = doubleKeySize; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
        retVal = E_OK;
      }
      else
        /* #14 If buffer is too small return with error */
      {
        retVal = CRYPTO_E_SMALL_BUFFER;
      }
    }
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GetPartnerPub()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GetPartnerPub(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyStorageIndexType, AUTOMATIC, AUTOMATIC) leftStorageIndexPtr,
  P2VAR(Crypto_30_LibCv_KeyStorageIndexType, AUTOMATIC, AUTOMATIC) rightStorageIndexPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  /*  predecessor = left neighbor / successor = right neighbor */
  uint32 keyLengthLeft = CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY;
  uint32 keyLengthRight = CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY;

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex( /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    cryptoKeyId,
    CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY,
    leftStorageIndexPtr,
    (P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&keyLengthLeft,
    CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK)
  {
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex( /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      cryptoKeyId,
      CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY_2,
      rightStorageIndexPtr,
      (P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&keyLengthRight,
      CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) == E_OK)
    {
        retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair_Init()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair_Init(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 numberDevicesLength = CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_NUM_ECU_LENGTH;
  uint32 deviceIdLength = CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_ECU_ID_LENGTH;
  Crypto_30_LibCv_KeyStorageIndexType numberDevicesIndex; /* limited to uint8 */
  Crypto_30_LibCv_KeyStorageIndexType deviceIdIndex; /* limited to uint8 */
  Std_ReturnType retVal = E_NOT_OK, localRet;
  uint8 numEcu, ecuId;

  /* NISTp224r1 = ANSIp224r1 = SECp224r1 */
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP224R1Domain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt;
  P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcSpeedUpExtPtr = Crypto_30_LibCv_EccCurveNistSecP224R1SpeedUpExt;

  /* ----- Implementation ------------------------------------------------- */
  /* Check length for key and parameter access. */
  if (esl_getLengthOfEcPprivateKey(EcDomainPtr) == CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
  {
    if (esl_getLengthOfEcPpublicKey_comp(EcDomainPtr) == CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
    {

      /* Get and check ecu id and number. */
      localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_NUM_ECU, /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
                                                                 &numberDevicesIndex,
                                                                 &numberDevicesLength,
                                                                 CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL);
      localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_ECU_ID, /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
                                                                  &deviceIdIndex,
                                                                  &deviceIdLength,
                                                                  CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL);
      if (localRet == E_OK)
      {
        numEcu = Crypto_30_LibCv_GetKeyStorage(numberDevicesIndex);
        ecuId = Crypto_30_LibCv_GetKeyStorage(deviceIdIndex);
        if ((ecuId <= numEcu) && (numEcu >= CRYPTO_30_LIBCV_ECDHE_BD_MIN_NUM_ECU))
        {
          wsPtr->ecuNum = numEcu; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
          /* Init Workspace */
          if (ESL_ERC_NO_ERROR == esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(wsPtr->wsBD.header), ESL_MAXSIZEOF_WS_ECBD, CRYPTO_30_LIBCV_WATCHDOG_PTR)) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE */
          {
            if (ESL_ERC_NO_ERROR == esl_initECBD(&wsPtr->wsBD, (eslt_Size32)numEcu, (eslt_Size32)ecuId, EcDomainPtr, EcDomainExtPtr, EcSpeedUpExtPtr)) /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
            {
              retVal = E_OK;
            }
          }
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)

{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType localRet = E_NOT_OK;
  uint8 privKey[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE];
  uint8 publicKey[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY];

  /* ----- Implementation ------------------------------------------------- */
  /* Init workspace */
  if (Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair_Init(wsPtr, cryptoKeyId) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */
  {
    if (esl_generateEphemeralKeyPairECBD( /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER */
      &wsPtr->wsBD,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))privKey,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))publicKey,
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(publicKey[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC]))
      == ESL_ERC_NO_ERROR)
    {
      /* Store public and private key to key elements if calculation was successfully */
      localRet = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                                     publicKey,
                                                     CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY);
      localRet |= Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                                      privKey,
                                                      CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE);
    }

    if (localRet == E_OK)
    {
      /* Copy public key to output buffer and overwrite local copy if store key was successfully */
      /* [SWS_Crypto_00109] return public key */
      if (*publicValueLengthPtr >= CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY)
      {
        Crypto_30_LibCv_CopyData(publicValuePtr, publicKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        *publicValueLengthPtr = CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

        retVal = E_OK;
      }
      else
      {
        /* If buffer is too small return with error */
        retVal = CRYPTO_E_SMALL_BUFFER;
      }

      /* delete buffer content from RAM */
      Crypto_30_LibCv_ClearData(privKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PRIVATE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
      Crypto_30_LibCv_ClearData(publicKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_CalcIntermediate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_CalcIntermediate(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  /*  predecessor = left neighbor / successor = right neighbor */
  Crypto_30_LibCv_KeyStorageIndexType leftPublicStorageIndex;
  Crypto_30_LibCv_KeyStorageIndexType rightPublicStorageIndex;

  uint8 intermediateKey[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY];

  /* ----- Implementation ------------------------------------------------- */
  /* Load partner public values and generate intermediate value.*/
  if (Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GetPartnerPub(cryptoKeyId, &leftPublicStorageIndex, &rightPublicStorageIndex) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {

    if (esl_calculateIntermediateECBD(wsPtr, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER */
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(leftPublicStorageIndex),
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&((Crypto_30_LibCv_GetAddrKeyStorage(leftPublicStorageIndex))[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC]),
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Crypto_30_LibCv_GetAddrKeyStorage(rightPublicStorageIndex),
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&((Crypto_30_LibCv_GetAddrKeyStorage(rightPublicStorageIndex))[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC]),
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))intermediateKey,
        (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(intermediateKey[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC])) == ESL_ERC_NO_ERROR)
    {
      if (Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_INTERMEDIATE, intermediateKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      {
        /* Copy intermediate key to output buffer and overwrite local copy if store key was successfully */
        /* [SWS_Crypto_00109] return public key */
        if (*publicValueLengthPtr >= CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY)
        {
          Crypto_30_LibCv_CopyData(publicValuePtr, intermediateKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
          *publicValueLengthPtr = CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

          /* delete buffer content from RAM */
          Crypto_30_LibCv_ClearData(intermediateKey, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          retVal = E_OK;
        }
        else
        {
          /* If buffer is too small return with error */
          retVal = CRYPTO_E_SMALL_BUFFER;
        }
      }
    }

  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace(
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx)
{
  /* Delete workspace. */
  Crypto_30_LibCv_ClearData(&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsECBD, sizeof(Crypto_30_LibCv_WorkSpaceECBD)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys(
  uint32 cryptoKeyId)
{
  uint8 keyBuf = 0;
  /* key element delete need to be enabled */

  /* Delete required key elements. */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_OWNPUBKEY, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_PARTNER_PUB_KEY_2, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_CUSTOM_KEYEXCHANGE_INTERMEDIATE, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  (void)Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, &keyBuf, 0u); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) localWsEcBPPtr = NULL_PTR;
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx;

  /* ----- Implementation ------------------------------------------------- */
  /* Try to lock workspace */
  if (Crypto_30_LibCv_Local_LongWsIsLock(cryptoKeyId, &longWsIdx) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    /* Workspace is already locked. */
    localWsEcBPPtr = (P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsECBD;
  }
  else if (Crypto_30_LibCv_Local_LongWsLockGet(cryptoKeyId, &longWsIdx) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    /* New Locked Workspace. */
    localWsEcBPPtr = (P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsECBD;
    localWsEcBPPtr->state = CRYPTO_30_LIBCV_ECBD_STATE_CALC_PUBVAL; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
  }
  else
  {
    retVal = CRYPTO_E_BUSY;
  }

  if (localWsEcBPPtr != NULL_PTR)
  {
    switch (localWsEcBPPtr->state)
    {
      /* Calculate own key pair */
      default: /* CRYPTO_30_LIBCV_ECBD_STATE_CALC_PUBVAL */
      {
        /* reset state members */
        localWsEcBPPtr->ecuCount = 0u; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
        localWsEcBPPtr->ecuNum = 0u; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
        /* calculate private and public key */
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_GenKeyPair(localWsEcBPPtr, cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        localWsEcBPPtr->state = CRYPTO_30_LIBCV_ECBD_STATE_CALC_INTERMEDIATE; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
        break;
      }

      /* Calculate intermediate value */
      case CRYPTO_30_LIBCV_ECBD_STATE_CALC_INTERMEDIATE:
      {
        /* calculate intermediate */
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime_CalcIntermediate(&localWsEcBPPtr->wsBD, cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
        localWsEcBPPtr->ecuCount = 1u; /* own intermediate value stored */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
        localWsEcBPPtr->state = CRYPTO_30_LIBCV_ECBD_STATE_REC_INTERMEDIATE; /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
        break;
      }

    }

    /* delete working products if three was a failure */
    if (retVal != E_OK)
    {
      Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace(longWsIdx);
      Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys(cryptoKeyId);
      Crypto_30_LibCv_Local_LongWsLockRelease(cryptoKeyId, longWsIdx);
    }
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcPubVal()
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
 */
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyStorageType keaIndex;
  uint32 keaLength = CRYPTO_30_LIBCV_KEY_EXCHANGE_SIZEOF_ALGORITHM;

  /* ----- Implementation ------------------------------------------------- */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
# if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
  /* #5 Lock read access for the DefaultRandomKey */
  if (Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(Crypto_30_LibCv_GetDefaultRandomKey()) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
# endif
  {
    /* #10 Lock write access for the CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      /* #20 Read and check algorithm key element. */
      if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_ALGORITHM, &keaIndex, &keaLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
      {

      }
      else
      {
        switch (Crypto_30_LibCv_GetKeyStorage(keaIndex))
        {
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_X25519:
            /* #30 execute KEA x25519 / RFC 7748 */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519(cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP256R1_ENABLED == STD_ON)
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP256R1:
            /* #50 execute KEA SECp256r1 / ANSI X9.63 */
            /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                         (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt,
                                                                         CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_ANSIP256R1:
            /* #50 execute KEA SECp256r1 / ANSI X9.63 */
            /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                         (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt,
                                                                         CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON)
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP384R1:
            /* #60 execute KEA SECp384r1 */
            /* NISTp384r1 = SECp384r1 */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt,
                                                                         (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt,
                                                                         CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON)
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP521R1:
            /* #70 execute KEA SECp521r1 */
            /* NISTp521r1 = SECp521r1 */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1Domain,
              (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt,
              (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1SpeedUpExt,
              CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON)
            /* #80 execute KEA Spake2+ */
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8:
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                         CRYPTO_30_LIBCV_SPAKE2P_MODE_NORMAL); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
          case CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8_1:
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P(cryptoKeyId, publicValuePtr, publicValueLengthPtr,
              (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                         CRYPTO_30_LIBCV_SPAKE2P_MODE_CHANGED_VERIFICATION); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA */
            break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
            case CRYPTO_30_LIBCV_KEY_EXCHANGE_NISTP224R1_BD:
              /* #90 execute KEA NISTp224r1 BD */
              retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime(cryptoKeyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
            break;
# endif
          default:
                /* invalid algorithm */
            break;
        }
      }
      /* #100 Release key write lock. */
      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(cryptoKeyId);
    }
# if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
    Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(Crypto_30_LibCv_GetDefaultRandomKey());
# endif
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) */

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr)
{
  /* #10 Initialize workspace */
  eslt_WorkSpaceX25519 localWsX25519;

  /* #20 Call Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws */
  return Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws(cryptoKeyId, partnerPublicValuePtr, &localWsX25519); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws()
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
/* PRQA S 6050, 6080 4 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsX25519)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  Crypto_30_LibCv_SizeOfKeyStorageType keyExchangePrivateKeyIndex;
  uint32 keyExchangePrivateKeyLength = ESL_SIZEOF_X25519_PRIVATE_KEY;
  eslt_Length sharedSecretLength_eslt = ESL_SIZEOF_X25519_SHARED_SECRET;
  uint8 sharedSecret[ESL_SIZEOF_X25519_SHARED_SECRET];

  /* ----- Implementation ------------------------------------------------- */
  /* #20 Initialize workspace */
  /* initialize Ed25519 */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&wsX25519->header, ESL_SIZEOF_WS_X25519, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initECDH(wsX25519, ESL_Curve25519); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    /* #25 Load private key */
    /* copy content of key elements to local buffers */
    Std_ReturnType localRetVal;

    localRetVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY, &keyExchangePrivateKeyIndex, &keyExchangePrivateKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */

    if (localRetVal == E_OK)
    {
      /* import private key from key elements */
      if (esl_importStaticPrivateKeyECDH(wsX25519, Crypto_30_LibCv_GetAddrKeyStorage(keyExchangePrivateKeyIndex)) == ESL_ERC_NO_ERROR) /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */
      {
        /* #30 generate secret by using public key from key element */
        eslRet = esl_generateSharedSecretECDH(wsX25519, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))partnerPublicValuePtr, sharedSecret, &sharedSecretLength_eslt); /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        if (eslRet == ESL_ERC_NO_ERROR)
        {
          /* #35 Store calculated sharedSecret */
          if (Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, sharedSecret, (uint32)sharedSecretLength_eslt) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
          {
            retVal = E_OK;
          }
        }

        /* #40 delete buffer content from RAM */
        Crypto_30_LibCv_ClearData(sharedSecret, ESL_SIZEOF_X25519_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }
    }
  }
  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength,
  uint32 keySize,
  uint8 keaId)
{
  /* #10 Init workspace */
  eslt_WorkSpaceEcP workspace;

  /* #20 Call Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws */
  return  Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(cryptoKeyId,  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                      partnerPublicValuePtr,
                                                                      partnerPublicValueLength,
                                                                      keySize,
                                                                      keaId,
                                                                      &workspace);
}
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws()
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
/* PRQA S 6050, 6060, 6080 7 */ /* MD_MSR_STCAL, MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength,
  uint32 keySize,
  uint8 keaId,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyStorageType keyExchangePrivateKeyIndex;
  uint32 keyExchangePrivateKeyLength;
  uint8 sharedSecret[Crypto_30_LibCv_Math_Mul2(CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE)];

  /* ----- Implementation ------------------------------------------------- */
  Crypto_30_LibCv_ClearData(sharedSecret, Crypto_30_LibCv_Math_Mul2((uint32)CRYPTO_30_LIBCV_ECC_KEY_MAXSIZE)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* #25 Load private key */
  /* copy content of key elements to local buffers */
  if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(
    cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_PRIVKEY,
    &keyExchangePrivateKeyIndex, &keyExchangePrivateKeyLength,
    CRYPTO_30_LIBCV_LENGTH_CHECK_NONE) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  {
    /* return E_NOT_OK */
  }
  else
  {
    /* #30 generate secret by using public key from key element */
    if (Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws(
      Crypto_30_LibCv_GetAddrKeyStorage(keyExchangePrivateKeyIndex),
      keyExchangePrivateKeyLength,
      partnerPublicValuePtr, partnerPublicValueLength,
      sharedSecret,
      keaId, workspace) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    {
      /* #35 Store calculated sharedSecret */
      if (Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, sharedSecret, Crypto_30_LibCv_Math_Mul2(keySize)) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      {
        retVal = E_OK;
      }
    }
    /* #40 delete buffer content from RAM */
    Crypto_30_LibCv_ClearData(sharedSecret, (uint32)Crypto_30_LibCv_Math_Mul2(keySize)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_First()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_First(
  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

  if (esl_initSharedSecretECBD(&wsPtr->wsBD) == ESL_ERC_NO_ERROR)  /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Update()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Update(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerIntermediateValuePtr,
  uint32 partnerIntermediateValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */

  /* add intermediate value for ecu x */
  /* EcuId (1Byte) | Partner intermediate value X | Partner intermediate value Y */
  if (partnerIntermediateValueLength == (CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_ECU_ID_LENGTH + CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY))
  {
    if (esl_updateSharedSecretECBD(wsPtr, /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      (eslt_Size32)partnerIntermediateValuePtr[0],
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&partnerIntermediateValuePtr[CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_ECU_ID_LENGTH],
        (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&partnerIntermediateValuePtr[CRYPTO_30_LIBCV_SIZEOF_ECDHE_BD_ECU_ID_LENGTH + CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC])
        == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Finish()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Finish(
  P2VAR(eslt_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) wsPtr,
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 sharedSecret[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY];
  Std_ReturnType retVal = E_NOT_OK, localRet;

  /* ----- Implementation ------------------------------------------------- */

  /* # Retrieve shared secret */
  if (esl_retrieveSharedSecretECBD(wsPtr, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN */
    (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))sharedSecret,
           (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&sharedSecret[CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC])
           == ESL_ERC_NO_ERROR)
  {
    /* Store calculated sharedSecret */
    localRet = Crypto_30_LibCv_Local_KeyElementSet(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
                                                   sharedSecret, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY);
    if (localRet == E_OK)
    {
      retVal = E_OK;
    }

    /* Delete working buffer from RAM */
    Crypto_30_LibCv_ClearData(sharedSecret, CRYPTO_30_LIBCV_SIZEOF_ECC_224_KEY_PUBLIC_XY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }
  else
  {
    /* retVal already set */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerIntermediateValuePtr,
  uint32 partnerIntermediateValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */

  Std_ReturnType retVal = E_NOT_OK;

  P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) localWsEcBPPtr;
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx;
  boolean release = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* Is workspace already locked for key */
  if (Crypto_30_LibCv_Local_LongWsIsLock(cryptoKeyId, &longWsIdx) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    localWsEcBPPtr = (P2VAR(Crypto_30_LibCv_WorkSpaceECBD, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&Crypto_30_LibCv_GetLongTermWs(longWsIdx).wsECBD;

    if (localWsEcBPPtr->state == CRYPTO_30_LIBCV_ECBD_STATE_REC_INTERMEDIATE)
    {
      /* distinguish mode dependent on the number of received ecus */
      /* own value already set by Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime */
      if (localWsEcBPPtr->ecuCount == 1u)
      {
        /* init workspace at first call */
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_First(localWsEcBPPtr); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      }
      else
      {
        retVal = E_OK;
      }

      if (retVal == E_OK)
      {
        /* Update always workspace with new intermediate value.*/
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Update(&localWsEcBPPtr->wsBD, partnerIntermediateValuePtr, partnerIntermediateValueLength); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
      }

      if ((retVal == E_OK) && (localWsEcBPPtr->ecuCount == (localWsEcBPPtr->ecuNum - 1u))) /* own value not required */
      {
        /* Finalize and generate shared secret, if it is the last intermediate value. */
        retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime_Finish(&localWsEcBPPtr->wsBD, cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE */
        Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace(longWsIdx);
        release = TRUE;
      }
    }

    if (retVal != E_OK)
    {
      /* delete workspace if there was an error */
      Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteWorkspace(longWsIdx);
      Crypto_30_LibCv_Local_KeyExchange_BD_Prime_DeleteKeys(cryptoKeyId);
      release = TRUE;
    }
    else
    {
      /* update for added value */
      localWsEcBPPtr->ecuCount = (uint8)(localWsEcBPPtr->ecuCount + 1u); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS */
    }

    if (release)
    {
      /* unlock if necessary */
      Crypto_30_LibCv_Local_LongWsLockRelease(cryptoKeyId, longWsIdx);
    }
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyExchangeCalcSecret()
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
/* PRQA S 6030, 6050, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyStorageType keaIndex;
  uint32 keaLength = CRYPTO_30_LIBCV_KEY_EXCHANGE_SIZEOF_ALGORITHM;

  /* #10 Lock write access for the CryptoKey */
  if (Crypto_30_LibCv_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #20 Read and check algorithm key element. */
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(cryptoKeyId, CRYPTO_KE_KEYEXCHANGE_ALGORITHM, &keaIndex, &keaLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
    {

    }
    else
    {
      switch (Crypto_30_LibCv_GetKeyStorage(keaIndex))
      {
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_X25519_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_X25519:
            /* #30 execute KEA x25519 / RFC 7748 */
          if (partnerPublicValueLength == ESL_SIZEOF_X25519_PUBLIC_KEY)
          {
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519(cryptoKeyId, partnerPublicValuePtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          }
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_ANSIP256R1_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_ANSIP256R1:
          /* #40 execute KEA ANSIp256r1 / ANSI X9.63 */
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength, CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_256_ID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP256R1_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP256R1:
          /* #50 execute KEA SECp256r1 / ANSI X9.63 */
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength, CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_256_ID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP384R1:
          /* #60 execute KEA SECp384r1 */
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength, CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_384_ID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_SECP521R1:
          /* #70 execute KEA SECp521r1 */
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength, CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_521_ID); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SPAKE2_PLUS_CIPHERSUITE_8_ENABLED == STD_ON)
          /* #80 execute KEA Spake2+ */
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8:
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_SPAKE2_PLUS_CIPHERSUITE_8_1:
          retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_NISTP224R1_BD_ENABLED == STD_ON)
        case CRYPTO_30_LIBCV_KEY_EXCHANGE_NISTP224R1_BD:
            /* #90 execute KEA NISTp224r1 BD */
            retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          break;
# endif
        default:
              /* invalid algorithm */
          break;
      }
    }
    /* #100 Release key write lock. */
    Crypto_30_LibCv_Local_KeyWriteLockRelease(cryptoKeyId);
  }

  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON */

#if ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_Dispatch_EcP_CalculateSharedSecret_With_Ws(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  uint32 privateKeyLength,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  uint32 partnerPubKeyLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  uint8 keaId,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValDispatching = E_NOT_OK;
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr = Crypto_30_LibCv_EccCurveEmptyDomain;
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr = Crypto_30_LibCv_EccCurveEmptyDomain;

  switch (keaId)
  {
# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_P256R1_DOMAIN == STD_ON)
  case CRYPTO_30_LIBCV_ECDHE_256_ID:
    /* NISTp256r1 = ANSIp256r1 = SECp256r1 */
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_P256R1_DOMAIN == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON)
  case CRYPTO_30_LIBCV_ECDHE_384_ID:
    /* NISTp384r1 = SECp384r1 */
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP384R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP384R1_ENABLED == STD_ON) */

# if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON)
  case CRYPTO_30_LIBCV_ECDHE_521_ID:
    /* NISTp521r1 = SECp521r1 */
    EcDomainPtr = Crypto_30_LibCv_EccCurveNistSecP521R1Domain;
    EcDomainExtPtr = Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt;
    retValDispatching = E_OK;
    break;
# endif /* (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_SECP521R1_ENABLED == STD_ON) */

  default:
    /* not reachable, misra only. */
    break;
  }

  /* ----- Implementation ----------------------------------------------- */
  if (retValDispatching == E_OK)
  {
    /* Check key sizes */
    if (privateKeyLength != esl_getLengthOfEcPprivateKey(EcDomainPtr)) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
    {
      /* invalid private key length */
    }
    else if (partnerPubKeyLength != (uint32)Crypto_30_LibCv_Math_Mul2((uint32)esl_getLengthOfEcPpublicKey_comp(EcDomainPtr))) /* SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE */
    {
      /* invalid partner public key length */
    }
    else
    {
      retVal = Crypto_30_LibCv_Local_EcP_CalculateSharedSecret_With_Ws(privateKeyPtr,
        partnerPubKeyPtr,
        sharedSecretPtr,
        workspace,
        EcDomainPtr,
        EcDomainExtPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_EcP_CalculateSharedSecret_With_Ws()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
 /* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_EcP_CalculateSharedSecret_With_Ws(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) privateKeyPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) partnerPubKeyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sharedSecretPtr,
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace,
  P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainPtr,
  P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) EcDomainExtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;

  /* #1 init  EcP workspaces */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workspace->header, ESL_MAXSIZEOF_WS_ECP, CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initGenerateSharedSecretDHEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, EcDomainPtr, EcDomainExtPtr); /* PRQA S 0310 */ /* MD_CRYPTO_30_LIBCV_0310 */ /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE */
  }

  if (eslRet == ESL_ERC_NO_ERROR)
  {

    /* #2 Verify that the key is valid. */
    /* This is already implemented in actECPointIsOnCurve and is part of esl_generateSharedSecretDHEcP_prim */

    /* #3 generate shared Secret */
    eslRet = esl_generateSharedSecretDHEcP_prim((P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))workspace, /* PRQA S 0310, 0311 */ /* MD_CRYPTO_30_LIBCV_0310, MD_CRYPTO_30_LIBCV_0311 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))privateKeyPtr,
      (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&partnerPubKeyPtr[0], (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&partnerPubKeyPtr[esl_getLengthOfEcPprivateKey(EcDomainPtr)], /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
      (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&sharedSecretPtr[0], (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&sharedSecretPtr[esl_getLengthOfEcPsecret_comp(EcDomainPtr)]); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    if (eslRet == ESL_ERC_NO_ERROR)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_KDF_ALGO_NIST_800_56_A_ONE_PASS_C1E1S_SINGLE_STEP_KDF_SHA256_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)\
    || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM_GENERIC_ENABLED == STD_ON)) */

#if (CRYPTO_30_LIBCV_KEYX25519SECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyX25519Secret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX25519Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyX25519Secret(Crypto_30_LibCv_GetKeyX25519SecretIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode ==CRYPTO_OPERATIONMODE_FINISH)
  {
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength == ESL_SIZEOF_X25519_PUBLIC_KEY)
    {
      /* #10 Calculate shared secret according to X25519 */
      retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_X25519_With_Ws(job->cryptoKeyId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyX25519Secret() */

#if (CRYPTO_30_LIBCV_KEYP256R1SECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP256R1Secret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP256R1Secret(Crypto_30_LibCv_GetKeyP256R1SecretIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate shared secret according to ECC curve p256r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(job->cryptoKeyId,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_256_ID, workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP256R1Secret() */

#if (CRYPTO_30_LIBCV_KEYP384R1SECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP384R1Secret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP384R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP384R1Secret(Crypto_30_LibCv_GetKeyP384R1SecretIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate shared secret according to ECC curve p384r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(job->cryptoKeyId,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_384_ID, workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP384R1Secret() */

#if (CRYPTO_30_LIBCV_KEYP521R1SECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP521R1Secret()
 **********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP521R1Secret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP521R1Secret(Crypto_30_LibCv_GetKeyP521R1SecretIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate shared secret according to ECC curve p521r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Generic_With_Ws(job->cryptoKeyId,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength,
      CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE, CRYPTO_30_LIBCV_ECDHE_521_ID, workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP521R1Secret() */

#if (CRYPTO_30_LIBCV_KEYSPAKE2PSECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySpake2PSecret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySpake2PSecret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT_CONST(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate shared secret according to Spake2+ */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_Spake2P(job->cryptoKeyId,
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeySpake2PSecret() */

#if (CRYPTO_30_LIBCV_KEYP224R1BDSECRET == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP224R1BDSecret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP224R1BDSecret(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT_CONST(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate shared secret according to ECBD */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret_BD_Prime(job->cryptoKeyId,
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYP224R1BDSECRET */

#if (CRYPTO_30_LIBCV_KEYX25519PUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyX25519PubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyX25519PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceX25519, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyX25519PubVal(Crypto_30_LibCv_GetKeyX25519PubValIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to X25519 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_X25519_With_Ws( job->cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
                                                                         workspace); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyX25519PubVal() */

#if (CRYPTO_30_LIBCV_KEYP256R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP256R1PubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP256R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP256R1PubVal(Crypto_30_LibCv_GetKeyP256R1PubValIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to ECC curve p256r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(job->cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
                                                                         (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                         (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt,
                                                                         CRYPTO_30_LIBCV_SIZEOF_ECC_256_KEY_PRIVATE,
                                                                         workspace);
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP256R1PubVal() */

#if (CRYPTO_30_LIBCV_KEYP384R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP384R1PubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP384R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP384R1PubVal(Crypto_30_LibCv_GetKeyP384R1PubValIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to ECC curve p384r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(job->cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                                         job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
                                                                         (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1Domain,
                                                                         (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt,
                                                                         (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt,
                                                                         CRYPTO_30_LIBCV_SIZEOF_ECC_384_KEY_PRIVATE,
                                                                         workspace);
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP384R1PubVal() */

#if (CRYPTO_30_LIBCV_KEYP224R1BDPUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP224R1BDPubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP224R1BDPubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ------------------------------------------------- */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT_CONST(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to ECBD curve p224r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_BD_Prime(job->cryptoKeyId,
                                                                  job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                                  job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYP224R1BDPUBVAL */

#if (CRYPTO_30_LIBCV_KEYP521R1PUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeyP521R1PubVal()
 **********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeyP521R1PubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  P2VAR(eslt_WorkSpaceEcP, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfKeyP521R1PubVal(Crypto_30_LibCv_GetKeyP521R1PubValIdxOfObjectInfo(objectId));

  /* ----- Implementation ------------------------------------------------- */
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Calculate public value (Key pair of private and public) according to ECC curve p521r1 */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Generic_With_Ws(job->cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
      (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1Domain,
      (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt,
      (P2CONST(eslt_EccSpeedUpExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistSecP521R1SpeedUpExt,
      CRYPTO_30_LIBCV_SIZEOF_ECC_521_KEY_PRIVATE,
      workspace);
  }
  return retVal;
}
#endif /* Crypto_30_LibCv_Dispatch_KeyP521R1PubVal() */

#if (CRYPTO_30_LIBCV_KEYSPAKE2PPUBVAL == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySpake2PPubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySpake2PPubVal(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  uint8 spakeMode;

  /* ----- Implementation ------------------------------------------------- */
  CRYPTO_30_LIBCV_DUMMY_STATEMENT_CONST(objectId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    if (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == CRYPTO_ALGOMODE_NOT_SET)
    {
      spakeMode = CRYPTO_30_LIBCV_SPAKE2P_MODE_NORMAL;
    }
    else
    {
      spakeMode = CRYPTO_30_LIBCV_SPAKE2P_MODE_CHANGED_VERIFICATION;
    }

    /* #10 Calculate public value according to Spake2+ */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal_Spake2P(job->cryptoKeyId, /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                                 job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr,
                                                                 (P2CONST(eslt_EccDomain, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain,
                                                                 (P2CONST(eslt_EccDomainExt, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR)) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt,
                                                                 spakeMode);
  }
  return retVal;
}
#endif /* CRYPTO_30_LIBCV_KEYSPAKE2PPUBVAL */

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyExchange.c
 *********************************************************************************************************************/

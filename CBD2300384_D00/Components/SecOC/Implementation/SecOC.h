/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SecOC.h
 *        \brief  MICROSAR  Secure Onboard Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Secure Onboard Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-08-14  vishho  ESCAN00077857 AR4-667: CONC_607_SecureOnboardCommunication
 *  01.00.01  2014-11-07  vishho  ESCAN00079423 include of PduR_SecOC.h misses and leads to compile errors
 *            2014-11-07  vishho  ESCAN00079424 endif misses in MemMap Inc file
 *  01.00.02  2015-03-30  vishho  ESCAN00082132 Buffer overflow when truncated Freshness value is copied to Secured Pdu
 *  01.00.02  2015-03-30  vishho  ESCAN00082134 < overwrites its internal Key data buffer.
 *  01.01.00  2015-06-17  vishho  ESCAN00083471 FEAT-1475: SecOC-Extensions, TP, CSM and encryption
 *  01.01.00  2015-06-19  vishho  ESCAN00082484 Authentication of valid secured PDUs fails
 *  01.01.01  2015-10-27  vishho  ESCAN00086110 Compiler error: error #20: identifier "Cal_VerifyResultType" is undefined
 *  01.01.02  2015-11-17  vishho  ESCAN00086559 DET occurs if SecOC_StartOfReception is called with a NULL_PTR
 *  01.01.02  2015-11-17  vishho  ESCAN00086367 RTE VerificationStatus service is not called
 *  01.01.02  2015-11-20  vishho  ESCAN00086659 Truncated freshness value is written in Little Endian instead of Big Endian
 *  01.01.02  2015-12-08  vishho  ESCAN00086690 Compiler error: SecOC.c: Det.h: No such file or directory
 *  01.02.00  2016-02-25  visms   ESCAN00087833 FEAT-1631: Trigger Transmit API with SduLength In/Out according to ASR4.2.2
 *  02.00.00  2016-07-07  vishho  ESCAN00090889 FEAT-1574: SecOC extension
 *            2016-08-11  visms   ESCAN00091423 FEAT-1779: Release SecOC
 *            2016-08-12  visms   ESCAN00091447 Rx Authentication fails Using Csm
 *  03.00.00  2016-09-12  vishho  FEATC-305: FEAT-2001: SecOC
 *            2016-11-15  vishho  FEATC-457: SecOC
 *            2016-11-15  vishho  FEATC-379: SecOC Release
 *  03.00.01  2016-11-28  vishho  ESCAN00093038 Wrong MAC values
 *  03.00.02  2016-12-21  vishho  ESCAN00093402 Compiler error: 'SecOC_KeyMgt_isKeyAssigned' undefined; assuming extern returning int
 *  03.01.00  2017-02-20  vishho  FEATC-1168: SecOC use Parameter Names like in 4.3
 *  04.00.00  2017-02-24  visms   FEATC-852: FEAT-2365: Support standalone distribution of AR4.3 SecOC
 *  04.01.00  2017-03-07  vishho  ESCAN00094186 BETA version - the BSW module is in BETA state
 *            2017-03-07  vishho  ESCAN00094276 SecOCVerificationStatusCallout is not called with SECOC_FRESHNESSFAILURE if GetRxFreshnessValue returns E_NOT_OK
 *            2017-03-08  vishho  ESCAN00094308 Transmission trigger of an Authentic Tx Pdu will not lead to an Secured Pdu transmission
 *            2017-03-09  vishho  ESCAN00094318 Reception of authentic PDU not indicated by SecOC if CSM 4.3 is used asynchronous
 *            2017-03-16  vishho  ESCAN00094379 Message Linker evaluation leads to read from invalid pointer
 *  05.00.00  2017-04-06  visfrm  STORYC-233: Remove Cal and Key Interface, Refactor against ASR4.3 SWS
 *            2017-04-11  visfrm  STORYC-492: API Pattern, ReviewHelper, MISRA
 *  05.01.00  2017-05-11  visfrm  STORYC-491: 100% C/DC
 *            2017-05-11  visfrm  STORYC-493: 100% Variant Coverage
 *  05.01.01  2017-05-11  visfrm  ESCAN00093747 Compiler error: 'SecOC_<Get>SecuredPduDataContainerTruncatedFreshnessValueEndIdxOf<Rx|Tx>PduInfo<Type>' : undeclared identifier
 *  05.02.00  2017-05-16  vishho  STORYC-763: SecOC_VerificationStatusCallout informs only final result
 *  05.03.00  2017-06-12  vishho  ESCAN00095505: Compiler warning: '<=' : signed/unsigned mismatch
 *            2017-06-28  vishho  ESCAN00095661: Compiler error: Percompiler encapsulation of EcuM_BswErrorHook is missing
 *            2017-07-24  vishho  STORYC-767: Add pass override until further notice for SecOC_VerifyStatusOverride
 *            2017-07-25  vishho  STORYC-919: Development Mode
 *  05.04.00  2017-08-11  vishho  STORYC-2141: SecOC Backlog cleanup
 *  06.00.00  2017-11-07  vishho  STORYC-2140: Support of dynamic length Secured/Authentic Pdu
 *            2017-11-09  vishho  ESCAN00097331: DET occurs during initialization if configuration variant is Post Build
 *            2017-11-16  vishho  ESCAN00097306: Freshness Value Manager function is called within exclusive area
 *  06.01.00  2017-11-29  vishho  STORYC-2139: Special CAN FD Tx Padding logic
 *            2017-12-04  vishho  STORYC-2944: SecOC VerificationRetry in same MainFunction Cycle
 *            2017-12-20  vishho  STORYC-2138: Rx Container length calculation and Padding remove
 *  06.02.00  2018-01-03  vishho  ESCAN00097866: Compiler warning: conversion from 'uint32' to 'SecOC_SecuredPduDataContainerIterType', possible loss of data
 *            2018-01-25  visgut  STORYC-3381: Configuration of secured area within a PDU (AR4.3.1 - RfC77090)
 *  06.02.01  2018-03-21  vishho  ESCAN00097900: Reception error occurs if Secured PDU with CAN FD padding is received
 *            2018-03-23  vishho  ESCAN00098774: Compiler warning: unused parameter 'result' in  function 'SecOC_TpTxConfirmation'
 *            2018-04-05  vishho  ESCAN00098991: Compiler warning: parameter <parameter name> not used
 *  07.00.00  2018-05-14  visssg  STORYC-4403    MISRA-C:2012 SecOC
 *            2018-06-04  vishho  ESCAN00099184: Async Generation of MAC fails
 *  07.01.00  2018-06-15  vishho  STORYC-5433: Rx Tp Upper Layer interface
 *            2018-06-15  vishho  STORYC-5434: Tx Tp Upper Layer interface
 *  07.02.00  2018-07-09  vishho  ESCAN00099957: Incorrect pointer is passed to PduR_SecOCCopyTxData
 *            2018-07-09  vishho  STORYC-5436: ASIL Release Tp Upper Layer interface
 *            2018-07-18  visbbk  STORYC-5576: [SEC] SecOC: Provide PDU payload within message authentication failed callout
 *            2018-07-24  visbbk  STORYC-5921: SecOC: Development mode for empty KeySlot
 *            2018-07-25  visfrm  STORYC-5765: Tx Secure PDU Processing Callout in SecOC
 *            2018-07-25  visfrm  STORYC-5766: Rx Secure PDU Processing Callout in SecOC
 *            2018-07-25  vishho  ESCAN00099634: Compiler error: implicit declaration of function 'SecOC_GetStartIdxOfTxSecuredPdu'
 *            2018-07-31  vishho  ESCAN00094994: Compiler error: 'SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOf<Rx/Tx>PduInfo' undefined
 *  07.03.00  2018-08-09  vishho  ESCAN00100351: SecOC passes incorrect length to Upper Layer Tp
 *            2018-08-09  vishho  STORYC-5436: ASIL Release Tp Upper Layer interface
 *            2018-08-10  visbbk  ESCAN00100357: Build and Verify Attempts counter is not resetted for Rx TP PDUs
 *  07.03.01  2018-08-30  vishho  ESCAN00100288: Verification errors occur and valid pdus are rejected.
 *            2018-09-05  vishho  ESCAN00100641: Compiler warning: 'id' : unreferenced formal parameter
 *  08.00.00  2018-10-02  visbbk  STORYC-6552: Support SecOCSecuredRxPduVerification parameter according ASR4.3.1
 *            2018-10-24  vishho  STORYC-6394: Support non-byte aligned Message Linker with Secured PDU Collections
 *  08.01.00  2018-11-29  vishho  STORYC-6930: Extension of the VerifyStatusOverride-API during runtime to ignore the reception of the Crypto-PDU.
 *            2018-12-18  visgut  STORYC-7038: SecOC Immediate Rx/Tx handling
 *            2018-12-18  vishho  ESCAN00101649: TriggerTransmit not functional for Split Pdus
 *  08.02.00  2019-03-12  vishho  STORYC-7871: Deny Retry in SecOc
 *  08.02.01  2019-04-12  vishho  ESCAN00102809: SduLength not checked for minimum length in SecOC_RxIndication could lead to arbitrary ECU behavior
 *            2019-04-12  vishho  ESCAN00102812: SduLength not checked for minimum length in SecOC_StartOfReception could lead to arbitrary ECU behavior
 *  08.03.00  2019-06-11  visbbk  STORYC-6476: SecOC - SecOC_VerifyStatusOverride and Verifiation Stauts Callout API update
 *            2019-06-11  viscpe  STORYC-6478: RfC  80038 - Option to send wrong Authentication Information
 *            2019-06-17  vishho  ESCAN00103440: negative return values of Csm_MacGenerate are not handled correctly
 *  08.04.00  2019-07-04  visbbk  STORYC-8098: Make Retry Transmit configurable
 *            2019-07-16  viscpe  TASK-121854: COM-72 Support SecOCAuthPduHeaderLength in SecOC
 *  09.00.00  2019-10-23  vishho  COM-1021: SecOC shall add Padding to Cryptographic PDU to be byte-aligend
 *  09.01.00  2019-11-20  vishho  COM-1229 SecOC: Add implementation for SECOC_AUTHENTICATIONBUILDFAILURE
 *            2019-11-22  vishho  COM-981 Support aligned data in SecOC module
 *            2019-11-28  vishho  ESCAN00105062: Reception of CANFD Container PDU can lead to endless loop in SecOC
 *  09.01.01  2020-02-04  vishho  ESCAN00105566: SecOC does not accept PduInfo->SduDataPtr == NULL_PTR if PduInfo->SduLength == 0 for SecOC_Transmit
 *  09.01.02  2020-03-16  vishho  ESCAN00105847: After the MAC verification of a received Split PDU fails the following receptions this Split PDUs will fail as well
 *            2020-03-26  vishho  ESCAN00105889: Truncated freshness value is not shifted as expected by Freshness Value Manager
 *  10.00.00  2020-04-28  vishho  COM-1238 Support MetaData in SecOC
 *            2020-05-06  vishho  ESCAN00106125: Reception of CANFD Container PDU with no padding can lead to endless loop in SecOC
 *            2020-05-06  vishho  ESCAN00106264: Manipulated length of IpduM Container Headers leads to memory corruption
 *  10.01.00  2020-05-27  visfrm  COM-1477 Support unsigned long long as least Type
 *                        vismij  COM-1516 Support Helix QAC 2019-2
 *  10.02.00  2020-07-24  visssg  COM-1747 Adapt ServiceIDs to AR19-11 Spec
 *            2020-07-29  vishho  COM-1617 Create timeout in SecOC for calls to Csm
 *            2020-07-29  visssg  ESCAN00106590 Compiler warning: 'initializing' : conversion from 'uint32' to 'SecOC_SecuredPduDataContainerIterType', possible loss of data
 *  10.03.00  2020-08-18  vishho  COM-1233 Introduce Queuing support in SecOC
 *            2020-09-08  vishho  ESCAN00107318: Compiler error: identifier storedState not declared
 *  10.03.01  2020-09-14  vishho  ESCAN00107375: Compiler error: identifier sduLength not declared
 *  11.00.00  2020-11-25  vishho  COM-2173 [AR-3215] Change of interface for SecOC_VerificationStatusService
 *            2020-12-07  vishho  COM-2013 Remove the AUTHOR IDENTITY
 *  11.00.01  2021-06-18  vishho  ESCAN00109151: IllegalStateException value 'xxxxx' of 'AuthenticPduDataContainerAuthenticPduEndiIdx' is not in range of datatype 'UTINT16'
 *            2021-06-29  visms   COM-2230 Support SECOC_LOCAL_INLINE
 *  12.00.00  2022-01-04  vishho  COM-2560 Implement 'Provide received data in StartOfReception' in Il_AsrSecOC
 *            2022-01-10  vishho  COM-2795 Support SecOC_MemMap.h
 *            2022-01-13  vismvi  COM-2645 Support Helix QAC 2021.1
 *  12.00.01  2022-01-17  vishho  ESCAN00111117: Compiler error: SecOC_MemMap.h(4295): fatal error C1189: #error:  No MemMap section found in SecOC_MemMap.h
 *********************************************************************************************************************/

#ifndef SECOC_H
# define SECOC_H

/**********************************************************************************************************************
 * MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* *INDENT-ON* */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "SecOC_Cfg.h"
# include "SecOC_Lcfg.h"
# include "SecOC_PBcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define SECOC_MODULE_ID                           150u
# define SECOC_VENDOR_ID                           30u
# define SECOC_INSTANCE_ID_DET                     0u

/*!
  \defgroup  SecOCARReleaseVersion SecOC AUTOSAR release version number
  \brief  Version number of AUTOSAR release on which the appropriate implementation is based on.
  \{
*/
# define SECOC_AR_RELEASE_MAJOR_VERSION            4u
# define SECOC_AR_RELEASE_MINOR_VERSION            3u
# define SECOC_AR_RELEASE_REVISION_VERSION         0u
/*!
  \}
*/

/*!
  \defgroup  SecOCSWVersion SecOC version number
  \brief  Version number of this implementation.
  \{
*/
# define SECOC_SW_MAJOR_VERSION                    12u
# define SECOC_SW_MINOR_VERSION                    0u
# define SECOC_SW_PATCH_VERSION                    1u
/*!
  \}
*/

/*!
  \defgroup  SecOCApiIds SecOC Module Service Identifiers
  \brief  Unique identifier of a SECOC module service.
  \{
*/
/*! Service ID: SecOC_Init() */
# define SECOC_SID_INIT                            0x01u
/*! Service ID: SecOC_GetVersionInfo() */
# define SECOC_SID_GET_VERSION_INFO                0x02u
/*! Service ID: SecOC_MainFunctionTx() */
# define SECOC_SID_MAIN_FUNCTION_TX                0x03u
/*! Service ID: SecOC_SendDefaultAuthenticationInformation() */
# define SECOC_SID_SEND_DEFAULT_AUTHENTICATION_INFORMATION            0x04u
/*! Service ID: SecOC_DeInit() */
# define SECOC_SID_DE_INIT                         0x05u
/*! Service ID: SecOC_MainFunctionRx() */
# define SECOC_SID_MAIN_FUNCTION_RX                0x06u
/*! Service ID: SecOC_RxIndication() */
# define SECOC_SID_VERIFY_STATUS_OVERRIDE          0x0bu
/*! Service ID: SecOC_TxConfirmation() */
# define SECOC_SID_TX_CONFIRMATION                 0x40u
/*! Service ID: SecOC_TriggerTransmit() */
# define SECOC_SID_TRIGGER_TRANSMIT                0x41u
/*! Service ID: SecOC_RxIndication() */
# define SECOC_SID_RX_INDICATION                   0x42u
/*! Service ID: SecOC_CopyTxData() */
# define SECOC_SID_COPY_TX_DATA                    0x43u
/*! Service ID: SecOC_CopyRxData() */
# define SECOC_SID_COPY_RX_DATA                    0x44u
/*! Service ID: SecOC_TpRxIndication() */
# define SECOC_SID_TP_RX_INDICATION                0x45u
/*! Service ID: SecOC_StartOfReception() */
# define SECOC_SID_START_OF_RECEPTION              0x46u
/*! Service ID: SecOC_TpTxConfirmation() */
# define SECOC_SID_TP_TX_CONFIRMATION              0x48u
/*! Service ID: SecOC_Transmit() */
# define SECOC_SID_TRANSMIT                        0x49u
/*! Service ID: SecOC_CancelTransmit() */
# define SECOC_SID_CANCEL_TRANSMIT                 0x4au
/* Service ID: SecOC_SPduTxConfirmation() */
# define SECOC_SID_SPDU_TX_CONFIRMATION            0x4du
/* Service ID: SecOC_GetRxFreshnessAuthData() */
# define SECOC_SID_RX_FRESHNESS_AUTH_DATA          0x4eu
/* Service ID: SecOC_GetRxFreshness() */
# define SECOC_SID_RX_FRESHNESS                    0x4fu
/* Service ID: SecOC_VerificationStatusCallout() */
# define SECOC_SID_VERIFICATION_STATUS_CALLOUT     0x50u
/* Service ID: SecOC_GetTxFreshnessTruncData() */
# define SECOC_SID_TX_FRESHNESS_TRUNC_DATA         0x51u
/* Service ID: SecOC_GetTxFreshness() */
# define SECOC_SID_TX_FRESHNESS                    0x52u
/*! Service ID: SecOC_SetDevelopmentMode() */
# define SECOC_SID_SET_DEVELOPMENT_MODE            0x80u
/*!
  \}
*/

/*!
  \defgroup  SecOCDetErrorCodes SecOC Det Error Codes
  \brief  SecOC development error IDs.
  \trace  SPEC-2697664
  \{
*/
/*! this code is used to detect that no error occurred. Det_ReportError() is not called. */
# define SECOC_E_NO_ERROR                          0u
/*! Error code: An API service was called with a NULL pointer */
# define SECOC_E_PARAM_POINTER                     1u
/*! Error code: API service used without module initialization or PduR_Init called  */
# define SECOC_E_UNINIT                            2u
/*! Error code: Invalid I-PDU identifier */
# define SECOC_E_INVALID_PDU_SDU_ID                3u
/*! Error code: Crypto service failed */
# define SECOC_E_CRYPTO_FAILURE                    4u
/*! Error code: An API service was called with an invalid parameter. */
# define SECOC_E_PARAM                             7u
/*! Error code: SecOC was not able to get the required freshness value form the Freshness Value Manager. */
# define SECOC_E_FRESHNESS_FAILURE                 8u
/*! Error code: Provided authentic PDU does not fit in the secured area. */
# define SECOC_E_SECURED_AREA                      9u
/*! Error code: Pdu is dropped because of ongoing validation. */
# define SECOC_E_BUFFER_BUSY                       10u
/*! Error code: The call to Csm_CancelJob was not successful. */
# define SECOC_E_CSM_CANCEL_JOB_FAILED             11u
/*!
  \}
*/

#if !defined (SECOC_LOCAL_INLINE)
# define SECOC_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# if(SECOC_USE_INIT_POINTER == STD_ON)
#  define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
#  include "SecOC_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Pointer to the current configuration */
extern P2CONST(SecOC_ConfigType, SECOC_VAR_CLEARED, SECOC_PBCFG) SecOC_ConfigDataPtr;
#  define SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#  include "SecOC_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define SECOC_START_SEC_CODE
# include "SecOC_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Internal functions */

# if (SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxCsmCallback
**********************************************************************************************************************/
/*! \brief      Static part of the Rx Csm Callback.
 *  \details    Process the Rx Csm Callback. This is an internal function.
 *  \pre        -
 *  \param[in]  rxPduId    Rx Pdu Id
 *  \param[in]  result     result of the Csm callback
 *  \context      TASK|ISR2
 *  \synchronous  TRUE
 *  \reentrant    TRUE
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_RxCsmCallback(PduIdType rxHandleId, Csm_ResultType result);
# endif

# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxCsmCallback
**********************************************************************************************************************/
/*! \brief      Static part of the Tx Csm Callback.
 *  \details    Process the Tx Csm Callback.  This is an internal function.
 *  \pre        -
 *  \param[in]  txPduId    Tx Pdu Id
 *  \param[in]  result     result of the Csm callback
 *  \context      TASK|ISR2
 *  \synchronous  TRUE
 *  \reentrant    TRUE
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_TxCsmCallback(PduIdType txHandleId, Csm_ResultType result);
# endif

/* Service functions */
/***********************************************************************************************************************
 *  SecOC_InitMemory()
 **********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function can be used to initialize the
 *              variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre        Module must not be initialized
 *  \pre        Function shall be called from task level
 *  \context    TASK
 *  \synchronous TRUE
 *  \reentrant  FALSE
 **********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_InitMemory(void);

/**********************************************************************************************************************
 * SecOC_Init()
 **********************************************************************************************************************/
/*! \brief      Initialization function
 *  \details    This function initializes the module SecOC. It initializes all variables and sets the module state to
 *              initialized.
 *  \param[in]  config   Configuration structure for initializing the module
 *  \pre        Interrupts have to be disabled.
 *  \pre        The module has to be uninitialized.
 *  \context    TASK
 *  \synchronous TRUE
 *  \reentrant  FALSE
 *  \note       Specification of module initialization
 **********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Init(P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_INIT_DATA) config);

/**********************************************************************************************************************
 * SecOC_DeInit()
 **********************************************************************************************************************/
/*! \brief      Deinitialization function
 *  \details    This function stops the secure onboard communication. All buffered I-PDU are removed and
 *              have to be optained again, if needed, after SecOC_Init has been called. By a call to SecOC_DeInit
 *              the AUTOSAR SecOC module is put into an not initialized state (SecOC_UNINIT).
 *  \trace      SPEC-2697665, SPEC-2697679
 *  \pre        Interrupts have to be disabled.
 *  \pre        The module has to be uninitialized.
 *  \context    TASK
 *  \synchronous TRUE
 *  \reentrant  FALSE
 *  \note       Specification of module initialization
 **********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_DeInit(void);

# if (SECOC_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  SecOC_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief      Returns the version information
 *  \details    SecOC_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out] versioninfo             Pointer to where to store the version information
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from task level
 *  \context    TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  FALSE
 **********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo);
# endif

/***********************************************************************************************************************
 *  SecOC_Transmit()
 **********************************************************************************************************************/
/*! \brief      Transmits an Authentic I-PDU.
 *  \details    Function is called by the PduR to request authentication and transmission of an Authentic I-PDU.
 *  \param[in]  id              ID of the Authentic I-PDU to be transmitted.
 *  \param[in]  info            A pointer to a structure with Authentic I-PDU related data that shall be transmitted:
 *                              data length and pointer to I-SDU buffer
 *  \return     Std_ReturnType
 *              E_OK:      request is accepted by the SecOC module. transmission is continued.
 *              E_NOT_OK:  request is not accepted by the SecOC module. transmission is aborted.
 *  \trace      SPEC-2697665, SPEC-2697666
 *  \pre        Module must be initialized
 *  \context    TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different PduIds.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_Transmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info);

# if (SECOC_RXSECUREDPDUINFO == STD_ON)
/***********************************************************************************************************************
 *  SecOC_VerifyStatusOverride()
 **********************************************************************************************************************/
/*!
 * \brief      Sets verification status override.
 * \details    This service provides the ability to override the VerifyStatus with "Fail" or "Pass" when using a specific Freshness Value
 *             to verify authenticity of data making up an I-PDU. Using this interface, VerifyStatus may be overridden
 *             1. Indefinitely for received I-PDUs which use the specific Freshness Value for authentication verification
 *             2. For a number of sequentially received I-PDUs which use the specific Freshness Value for authentication verification.
 * \param[in]  freshnessValueID           ID of the Freshness Value which when used to authenticate data, results in SecOC_VerifyStatus
 *                                        equal to OverrideStatus independent of the actual authentication status.
 * \param[in]  overrideStatus             0  = Override VerifyStatus to "Fail" until further notice
 *                                        1  = Override VerifyStatus to "Fail" until NumberOfMessagesToOverride is reached
 *                                        2  = Cancel Override of VerifyStatus
 *                                        40 = Override VerifyStatus to "Pass" until further notice.
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 *                                        41 = Override VerifyStatus to "Pass" until NumberOfMessagesToOverride is reached.
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 *                                      0x41 = Until NumberOfMessagesToOverride is reached, authenticator verification is not performed
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 *                                      0x40 = Override VerifyStatus to "Pass" until further notice.
 *                                             Verification is performed and PDU is forwarded to upper layer.
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 *                                      0x42 = Override VerifyStatus to "Pass" until NUmberOfMessagesToOverwrite is reached.
 *                                             Verification is performed and PDU is forwarded to upper layer.
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 *                                      0x43 = Authenticator verification is not performed until further notice.
 *                                             Only available if SecOCEnableForcedPassOverride is set to TRUE
 * \param[in]  numberOfMessagesToOverride Number of sequential VerifyStatus to override when using a specific counter
 *                                        for authentication verification. This is only considered when OverrideStatus is equal to 1 or 41.
 * \return     Std_ReturnType
 *              E_OK:      request successful
 *              E_NOT_OK:  request failed
 * \pre        Module must be initialized
 * \context    TASK
 * \synchronous TRUE
 * \reentrant  TRUE, for different FreshnessValueIDs.
 * \trace      SPEC-2697665
 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_VerifyStatusOverride(uint16 freshnessValueID, uint8 overrideStatus, uint8 numberOfMessagesToOverride);
# endif

# if(SECOC_DEVMODEENABLED == STD_ON)
/***********************************************************************************************************************
 *  SecOC_SetDevelopmentMode()
 **********************************************************************************************************************/
/*!
 * \brief      Enables/Disables the development mode.
 * \details    -
 * \param[in]  enableDevMode        TRUE  - Development Mode is enabled.
 *                                  FALSE - Development Mode is disabled
 * \pre        Module must be initialized
 * \context    TASK
 * \synchronous TRUE
 * \reentrant  FALSE
 */
FUNC(void, SECOC_CODE) SecOC_SetDevelopmentMode(boolean enableDevMode);
# endif

# if(SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
/***********************************************************************************************************************
 *  SecOC_SendDefaultAuthenticationInformation()
 **********************************************************************************************************************/
/*!
 * \brief       The service provides the ability to enable the sending of un-authenticated PDU to lower layer.
 * \details     -
 * \param[in]   FreshnessValueID                      ID of the Freshness Value for which sending  SecOCDefaultAuthenticationInformationPattern should be enabled.
 * \param[in]   sendDefaultAuthenticationInformation  FALSE - sending SecOCDefaultAuthenticationInformationPattern shall be disabled for given FreshnessValueID.
 *                                                    TRUE  - sending SecOCDefaultAuthenticationInformationPattern shall be enabled for given FreshnessValueID.
 *  \return     Std_ReturnType
 *                E_OK: request successful.
 *                E_NOT_OK: request failed.
 * \pre         Module must be initialized
 * \context     TASK
 * \synchronous TRUE
 * \reentrant   TRUE, for different FreshnessValueIDs
 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_SendDefaultAuthenticationInformation(uint16 freshnessValueID, boolean sendDefaultAuthenticationInformation);
# endif

/***********************************************************************************************************************
 *  SecOC_MainFunctionRx()
 **********************************************************************************************************************/
/*! \brief      Scheduled Main Function for the Rx path
 *  \details    This function performs the processing of the SecOC module's authentication and verification processing for the rx path.
 *  \trace      SPEC-2968994
 *  \pre        -
 *  \context    TASK
 *  \synchronous TRUE
 *  \reentrant  FALSE
 **********************************************************************************************************************/
/* function defined in file 'SchM_SecOC.h' */

/***********************************************************************************************************************
 *  SecOC_MainFunctionTx()
 **********************************************************************************************************************/
/*! \brief      Scheduled Main Function for the Tx path
 *  \details    This function performs the processing of the SecOC module's authentication and verification processing for the tx path.
 *  \trace      SPEC-2968948
 *  \pre        -
 *  \context    TASK
 *  \synchronous TRUE
 *  \reentrant  FALSE
 **********************************************************************************************************************/
/* function defined in file 'SchM_SecOC.h' */

# define SECOC_STOP_SEC_CODE
# include "SecOC_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*!
* \exclusivearea SECOC_EXCLUSIVE_AREA_RXSTATE
* Ensures consistency of global RAM variables.
* \protects Global RAM variables used of Rx path.
* \usedin SecOC_MainFunctionRx, SecOC_VerifyStatusOverride
* \exclude All functions provided by SecOC.
* \length SHORT depending on the API and the call graph.
* \endexclusivearea
*/

/*!
* \exclusivearea SECOC_EXCLUSIVE_AREA_TXSTATE
* Ensures consistency of global RAM variables.
* \protects Global RAM variables used of Tx path.
* \usedin SecOC_Transmit, SecOC_TriggerTransmit
* \exclude All functions provided by SecOC.
* \length SHORT depending on the API and the call graph.
* \endexclusivearea
*/


#endif /* SECOC_H */

/**********************************************************************************************************************
 *  END OF FILE: SecOC.h
 *********************************************************************************************************************/

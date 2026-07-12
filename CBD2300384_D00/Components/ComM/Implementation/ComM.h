/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**         \file  ComM.h
 *         \brief  Communication Manager ASR4
 *
 *       \details  Header of the Autosar Communication Manager
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
  REVISION HISTORY
-----------------------------------------------------------------------------------------------------------------------
  Version   Date        Author     Change Id     Description
-----------------------------------------------------------------------------------------------------------------------
  01.00.00  2012-06-14  vismih     ESCAN00058535 Implementation according to ASR 4.0.3
  01.00.01  2012-10-18  vismih     ESCAN00062348 Compiler error: Cannot open include file 'Nm.h'
                                   ESCAN00061391 Compiler warning: unused variable 'ComM_FullComRequestors'
                                   ESCAN00061400 Compiler warning: unused variable 'ComM_StateChange'
  01.01.00  2012-12-07  vismih     ESCAN00061388 DET errors reported when shutting down communication on LIN channel with Nm
                                   ESCAN00062059 AR4-278: Add Port Interface of ComM_CurrentChannelRequest
                                   ESCAN00063511 Compiler warning: more than one declaration of 'Rte_ModeType_ComMMode'
  01.02.00  2013-04-12  vismih     ESCAN00059545 BETA version - no series production allowed
                                   ESCAN00064311 AR4-325: Implement Post-Build Loadable
                                   ESCAN00065236 BswM gets notified about bus mode, which is not yet provided by the BusSM
                                   ESCAN00066281 Various small improvements and optimizations
                                   ESCAN00066282 NM Message Reception in Silent Mode leads to DET error and missing NM Tx Message
                                   ESCAN00066290 ECU does not send Nm messages on a channel while ComM channel user requests FULL_COM
                                   ESCAN00066311 Shutdown synchronisation is shortened or delayed depending on timing conditions
                                   ESCAN00067357 Partial Network does not enter No Communication mode again when Pnc Prepare Sleep Timer is 0 and external wake-up occurred
                                   ESCAN00067386 Network with partial networking does not enter sleep
                                   ESCAN00067423 User of a Partial Network is not notified about a new communication mode
  02.00.00 2013-09-27   vismih     ESCAN00068132 DET error occurs on passive startup of NM (woken up by another NM message during prepare sleep)
                                   ESCAN00068871 Missing DET check inside the function ComM_RequestComMode
                                   ESCAN00068896 Mode Limitation API does not return the correct return value if the request is not accepted
                                   ESCAN00068902 Prevent Wake-up API does not return the correct return value if the request is not accepted
                                   ESCAN00069043 AR4-478: Support EthSM
                                   ESCAN00069394 Added PC-lint error inhibition options
                                   ESCAN00070962 MISRA deviation: MISRA-C:2004 Code Metrics
                                   ESCAN00071329 AR4-438: Support for J1939
                                   ESCAN00071488 MISRA deviation: MISRA-C:2004 Rule 14.3
  02.01.00 2014-02-07   vismih     ESCAN00072395 Removed unused memory sections and class
                                   ESCAN00072736 LIN communication channel does not enter COMM_NO_COMMUNICATON
                                   ESCAN00072762 Report Det error if diagnostic communication is requested or released on a channel with Nm PASSIVE
                                   ESCAN00073325 Performance improvement for gateways with Partial Networking
                                   ESCAN00073488 Code optimizations to improve run-time behavior and ROM usage
  03.00.00 2014-05-16   vismih     ESCAN00075473 Changed access on generated variables and constants due to introduction of ComStackLib
                                   ESCAN00075701 Memory class qualifiers were conformed to the Vector standard
                                   ESCAN00076098 Compiler error: 'Nm_NetworkRequest' undefined
  03.01.00 2014-07-11   vismih     ESCAN00077029 Adapted generation and usage of PNC Signals, FullCommReqeust variables for Post-Build Selectable usecase
                        vispmo     ESCAN00076156 Improve Include Hierarchy of ComM
                                   ESCAN00076857 AR4-776: Add Support of COMM_BUS_TYPE_INTERNAL
                                   ESCAN00077600 ECU does not enter sleep when a PNC is requested and released after an external wake-up
                                   ESCAN00077624 Support Mode Limitation without ComMGlobalNvMBlockDescriptor
                                   ESCAN00077950 ComM_Inhibition shall not have STATIC linkage to be writeable by NvM
  03.02.00 2015-01-26   vismih     ESCAN00080440 MISRA deviation: MISRA-C:2004 Rule 5.1 and 19.7
                                   ESCAN00080884 Support variance for ComMUser
  04.00.00 2015-02-18   vismih     ESCAN00076865 FEAT-111: add support for VAP partial networking use case
                                   ESCAN00081330 FEAT-1114: Realization OSEK-NM
  04.00.01 2015-03-26   vismih     ESCAN00082101 Compiler error: the macro 'COMM_PNC_PS_TIMER' is not defined
  05.00.00 2015-06-21   vismih     ESCAN00080279 FEAT-427: SafeBSW Step 1
                                   ESCAN00081767 FEAT-1275: SafeBSW Step 2
                                   ESCAN00083603 FEAT-1440: Partial Network routing/coordination limitation
  05.00.01 2015-11-05   vismih     ESCAN00085722 Compiler warning: 'channel' unreferenced formal parameter
                                   ESCAN00086538 FEAT-1275: SafeBSW Step 2 (completion)
  06.00.00 2015-11-27   vismih     ESCAN00085680 FEAT-1492: Support Ford SDLC: NM Gateway and Coordination
                                   ESCAN00086618 ComM state does not reflect a pending channel wake-up
                                   ESCAN00086661 ComM state does not reflect a pending user communication request
                                   ESCAN00086915 The return value of NmIf APIs shall not be evaluated
  07.00.00 2016-02-26   vismih     ESCAN00087541 Compiler warning: condition is always true
                                   ESCAN00087791 FEAT-1688: SafeBSW Step 4
                                   ESCAN00088563 FEAT-598: Extended RAM Check
  07.00.01 2016-04-13   vismih     ESCAN00089461 Compiler warning: CanSM_RamCheckStatus is undefined
                                   ESCAN00089740 FEAT-1688: SafeBSW Step 5
  07.00.02 2016-05-11   vismih     ESCAN00090228 FEAT-1688: SafeBSW Step 5 (completion)
  07.00.03 2016-06-24   vismih     ESCAN00090163 Inhibition Counter value is higher than expected
                                   ESCAN00090714 Extended generation of channel-based variable arrays by ComStackLib
  07.01.00 2016-08-05   vismih     ESCAN00091346 FEAT-1948: provided new APIs ComM_GetDcmRequestStatus() and ComM_GetMinFullComModeTimerStatus()
  08.00.00 2016-10-06   vismih     FEATC-60: FEAT-2039: Support Reset After Forcing NoComm according to ASR4.2.2
                                   FEATC-268: FEAT-1454: Configuration of Switch Ports (switchable per PNC)
                                   ESCAN00090162 Pnc To Channel Routing Limitation is not considered by ComM_GetState()
                                   ESCAN00091475 Channel with Nm Variant LIGHT does not enter NO COMMUNICATION after a wake-up on bus event
                                   ESCAN00092198 ComM_GetCurrentComMode() and ComM_GetRequestedComMode() return COMM_FULL_COMMUNICATION if user is not attached to any Channel or PNC
  08.01.00 2017-01-13   vismih     ESCAN00092692 ComM_MainFunction() shall not report Det error if ComM is not initialized
                                   ESCAN00093096 Compiler warning: truncating assignment
                                   ESCAN00094086 Improved QAC and PClint compliance
  08.01.01 2017-08-08   vismih     ESCAN00094464 NvM data is not stored after calling ComM_DeInit()
                                   STORYC-2178: Consistently used ComStackLib types to support uint16 indexes
  09.00.00 2017-10-26   vismih     ESCAN00093702: PNC Wake-up Indication does not work as specified in AUTOSAR
  09.00.01 2018-03-12   vismih     ESCAN00096807: Compiler warning: possible loss of data due to implicit cast from uint16 to uint8
  10.00.00 2018-09-20   vislsi     STORYC-6128: MISRA-C:2012 Compliance
  10.00.01 2019-06-06   vislsi     ESCAN00102988: [Partial Network only] ECU is blocked until Watch-Dog reset when a Partial Network is requested and released via ComM_RequestComMode
  11.00.00 2019-07-10   vislsi     NMM-49: [ComM] Support PNC Ids > 63
  12.00.00 2019-09-13   vispps     NMM-2: LIN Slave Support
  12.01.00 2019-11-29   vispps     NMM-820: Support Managing and Managed Channel
  12.02.00 2019-12-10   vislsi     NMM-972: PassiveGateway - sending PN-0-vector
                                   ESCAN00105150 Compiler error: 'COMM_EXISTS_LINSLAVE_NMTYPEOFCHANNEL' is not defined as a preprocessor macro
                                   ESCAN00104770 ComM releases channels where PNC Gateway is set to Passive too early which can lead to delayed ECU shutdown
  13.00.00 2020-02-11   vislsi     NMM-971: PncGatewayType "NONE" within ComMPncGateway
                        vispps     ESCAN00092764 Change BETA Feature to Safe - PNC WakeUp Indication (FEAT-1454)
  13.00.01 2020-07-09   vislsi     ESCAN00106564 ComM shuts down communication while FrNm is still in state SYNCHRONIZE
                                   NMM-1216 : MISRA-C: 2012 Compliance for Helix extension
  14.00.00 2020-10-14   vislsi     NMM-1237 : R22_ESCAN00106344_Ccl_Asr4ComMCfg5_Vector
                        vispps     NMM-1136: Multi-Partition Support
  14.01.00 2021-04-27   vispps     NMM-1412: Finalize Multi-Partition Support
                        vispps     ESCAN00109748: Communication request via PNC is limited on a channel unexpectedly
                        vispps     ESCAN00108491: Channel does not wake up when expected by a passive wakeup
                        smarcelin  NMM-1455: Support for ComM_GetCurrentPNCComMode
  14.01.01 2021-10-08   smarcelin  ESCAN00105892: Configuration tool throws validation errors if any PNC references a managed channel
  14.02.00 2021-11-24   smarcelin  ESCAN00110796: Error codes of Interface Operation ComM_GetCurrentPNCComMode are not according to AUTOSAR
           2021-11-29   vispps     NMM-1528: Added support for ASR MemMap include structure
  15.00.00 2022-01-17   smarcelin  NMM-1841: Support for Synchronized PNC Shutdown
  15.00.01 2022-03-28   vispps     ESCAN00111302: [Multi-Partition] PNC state changes unexpectedly
  16.00.00 2022-05-13   vispps     NMM-2078: Finite Execution Time Analysis
           2022-05-20   vispps     ESCAN00111917: [Multi-Partition] ComM MainFunction takes too long
           2022-05-24   vispps     ESCAN00111975: [Multi-Partition] State of PNC To Channel Routing Limitation changes unexpectedly
  16.00.01 2022-10-26   smarcelin  ESCAN00112732: [Multi-Partition] Critical Section is never left
  16.01.00 2023-01-02   smarcelin  NMM-2533: Support satellite partitions on SWCs
**********************************************************************************************************************/

#ifndef COMM_H
# define COMM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComM_Lcfg.h"                          /* Include ComM configuration header */
# include "ComM_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : Ccl_Asr4ComMCfg5 CQComponent : Implementation */
# define CCL_ASR4COMMCFG5_VERSION                (0x1601u)
# define CCL_ASR4COMMCFG5_RELEASE_VERSION        (0x00u)

/* Vendor and module identification */
# define COMM_VENDOR_ID                          (30u)
# define COMM_MODULE_ID                          (12u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define COMM_SW_MAJOR_VERSION                   (CCL_ASR4COMMCFG5_VERSION >> 8u)
# define COMM_SW_MINOR_VERSION                   (CCL_ASR4COMMCFG5_VERSION & 0x00FFu)
# define COMM_SW_PATCH_VERSION                   (CCL_ASR4COMMCFG5_RELEASE_VERSION)

/* AUTOSAR Software specification version information */
# define COMM_AR_RELEASE_MAJOR_VERSION           (4u)
# define COMM_AR_RELEASE_MINOR_VERSION           (0u)
# define COMM_AR_RELEASE_REVISION_VERSION        (3u)

# define COMM_INSTANCE_ID_DET                    (0u)

/* ----- Error codes ----- */
# define COMM_E_NO_ERROR                         (0x00u) /*!< used to check if no error occurred - use a value unequal to any error code */
# define COMM_E_NOT_INITED                       (0x01u) /*!< Error code: API service used without module initialization */
# define COMM_E_WRONG_PARAMETERS                 (0x02u) /*!< Error code: API service used with wrong parameters */
# define COMM_E_ERROR_IN_PROV_SERVICE            (0x03u) /*!< Error code: Provided API services of other module returns with an error */
# define COMM_E_NOSUPPORTED_MODECHANGE           (0x04u) /*!< Error code: ComM is trying to perform a mode change which is not supported */
# define COMM_E_DIAGNOSTIC_NOT_SUPPORTED         (0x05u) /*!< Error code: Diagnostic communication is requested or released on a channel where diagnostic is not supported */
# define COMM_E_ALREADY_INITIALIZED              (0x06u) /*!< Error code: The service ComM_Init() is called while the module is already initialized  */
# define COMM_E_WRONG_GENERATED_DATA             (0x07u) /*!< Error code: Check of generated data has failed */
# define COMM_E_NO_PREINIT                       (0x08u) /*!< Error code: API service is used without module pre-initialization */
# define COMM_E_INVALID_PARTITION                (0x09u) /*!< Error code: The current partition index is out of range */
# define COMM_E_NO_POSTINIT                      (0x10u) /*!< Error code: API service is used without module post-initialization */

/* ----- API service IDs ----- */
# define COMM_SID_INIT                           (0x01u) /*!< Service ID: ComM_Init() */
# define COMM_SID_DEINIT                         (0x02u) /*!< Service ID: ComM_DeInit() */
# define COMM_SID_GETSTATUS                      (0x03u) /*!< Service ID: ComM_GetStatus() */
# define COMM_SID_GETINHIBITIONSTATUS            (0x04u) /*!< Service ID: ComM_GetInhibitionStatus() */
# define COMM_SID_REQUESTCOMMODE                 (0x05u) /*!< Service ID: ComM_RequestComMode() */
# define COMM_SID_GETMAXCOMMODE                  (0x06u) /*!< Service ID: ComM_GetMaxComMode() */
# define COMM_SID_GETREQUESTEDCOMMODE            (0x07u) /*!< Service ID: ComM_GetRequestedComMode() */
# define COMM_SID_GETCURRENTCOMMODE              (0x08u) /*!< Service ID: ComM_GetCurrentComMode() */
# define COMM_SID_PREVENTWAKEUP                  (0x09u) /*!< Service ID: ComM_PreventWakeUp() */
# define COMM_SID_LIMITCHANNELTONOCOMMODE        (0x0Bu) /*!< Service ID: ComM_LimitChannelToNoComMode() */
# define COMM_SID_LIMITECUTONOCOMMODE            (0x0Cu) /*!< Service ID: ComM_LimitECUToNoComMode() */
# define COMM_SID_READINHIBITCOUNTER             (0x0Du) /*!< Service ID: ComM_ReadInhibitCounter() */
# define COMM_SID_RESETINHIBITCOUNTER            (0x0Eu) /*!< Service ID: ComM_ResetInhibitCounter() */
# define COMM_SID_SETECUGROUPCLASSIFICATION      (0x0Fu) /*!< Service ID: ComM_SetECUGroupClassification() */
# define COMM_SID_GETVERSIONINFO                 (0x10u) /*!< Service ID: ComM_GetVersionInfo() */
# define COMM_SID_COMMUNICATIONALLOWED           (0x35u) /*!< Service ID: ComM_CommunicationAllowed() */
# define COMM_SID_LIMITPNCTOCHANNELROUTING       (0x36u) /*!< Service ID: ComM_LimitPncToChannelRouting() */
# define COMM_SID_GETDCMREQUESTSTATUS            (0x37u) /*!< Service ID: ComM_GetDcmRequestStatus() */
# define COMM_SID_GETMINFULLCOMMODETIMERSTATUS   (0x38u) /*!< Service ID: ComM_GetMinFullComModeTimerStatus() */
# define COMM_SID_GETSTATE                       (0x39u) /*!< Service ID: ComM_GetState() */
# define COMM_SID_MAINFUNCTION                   (0x60u) /*!< Service ID: ComM_MainFunction() */
# define COMM_SID_MAINFUNCTION_SATELLITE         (0x61u) /*!< Service ID: ComM_MainFunction_Satellite() */
# define COMM_SID_PREINIT                        (0x62u) /*!< Service ID: ComM_PreInit() */
# define COMM_SID_POSTINIT                       (0x63u) /*!< Service ID: ComM_PostInit() */
# define COMM_SID_GETCURRENTPNCCOMMODE           (0x6Au) /*!< Service ID: ComM_GetCurrentPNCComMode() */

/* ----- Callback function service IDs ----- */
# define COMM_SID_CBK_NM_NETWORKSTARTINDICATION         (0x15u) /*!< Service ID: ComM_Nm_NetworkStartIndication() */
# define COMM_SID_CBK_NM_NETWORKMODE                    (0x18u) /*!< Service ID: ComM_Nm_NetworkMode() */
# define COMM_SID_CBK_NM_PREPAREBUSSLEEPMODE            (0x19u) /*!< Service ID: ComM_Nm_PrepareBusSleepMode() */
# define COMM_SID_CBK_NM_BUSSLEEPMODE                   (0x1Au) /*!< Service ID: ComM_Nm_BusSleepMode() */
# define COMM_SID_CBK_NM_RESTARTINDICATION              (0x1Bu) /*!< Service ID: ComM_Nm_RestartIndication() */
# define COMM_SID_CBK_NM_STATECHANGENOTIFICATION        (0x1Cu) /*!< Service ID: ComM_Nm_StateChangeNotification() */
# define COMM_SID_CBK_DCM_ACTIVEDIAGNOSTIC              (0x1Fu) /*!< Service ID: ComM_DCM_ActiveDiagnostic() */
# define COMM_SID_CBK_DCM_INACTIVEDIAGNOSTIC            (0x20u) /*!< Service ID: ComM_DCM_InactiveDiagnostic() */
# define COMM_SID_CBK_ECUM_WAKEUPINDICATION             (0x2Au) /*!< Service ID: ComM_EcuM_WakeUpIndication() */
# define COMM_SID_CBK_ECUM_PNCWAKEUPINDICATION          (0x2Bu) /*!< Service ID: ComM_EcuM_PNCWakeUpIndication() */
# define COMM_SID_CBK_BUSSMMODEINDICATION               (0x33u) /*!< Service ID: ComM_BusSM_ModeIndication() */
# define COMM_SID_CBK_BUSSMBUSSLEEPMODE                 (0x34u) /*!< Service ID: ComM_BusSM_BusSleepMode() */
# define COMM_SID_CBK_COMCBK                            (0x40u) /*!< Service ID: ComM_COMCbk_<signal_name>() */
# define COMM_SID_CBK_NM_FORWARDSYNCHRONIZEDPNCSHUTDOWN (0x6Bu) /*!< Service ID: ComM_Nm_ForwardSynchronizedPncShutdown() */

/* advanced return type of the communication manager */
# define COMM_E_MODE_LIMITATION                  (Std_ReturnType)(2u) /*!< Mode is limited and cannot be requested by a ComM User */
# define COMM_E_UNINIT                           (Std_ReturnType)(5u) /*!< API called but ComM is not initialized */
# define E_MULTIPLE_PNC_ASSIGNED                 (Std_ReturnType)(3u) /*!< Multiple PNCs are assigned to the affected user */
# define E_NO_PNC_ASSIGNED                       (Std_ReturnType)(4u) /*!< No PNC is assigned to the affected user */

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
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 * ComM_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Function to perform channel-specific state transitions and state change notifications.
 *  \details     This function must be called cyclically with the configured channel-specific cycle time.
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       SPEC-19840
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_MainFunction(NetworkHandleType Channel);

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ComM_PreInit()
 *********************************************************************************************************************/
/*! \brief       Pre-Initialization function of the ComM
 *  \details     -
 *  \param[in]   ConfigPtr              Pointer to configuration data
 *  \pre         ComM must not be initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-102775
 *  \trace       SPEC-5254, SPEC-5139
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_PreInit(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA) ConfigPtr);

/**********************************************************************************************************************
 * ComM_PostInit()
 *********************************************************************************************************************/
/*! \brief       Post-Initialization function of the ComM
 *  \details     -
 *  \param[in]   -
 *  \pre         ComM must be initialized on each partition.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-102775
 *  \trace       SPEC-5254, SPEC-5139
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_PostInit( void );

/**********************************************************************************************************************
 * ComM_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function of the ComM
 *  \details     -
 *  \param[in]   ConfigPtr              Pointer to configuration data
 *  \pre         ComM must not be initialized
 *  \pre         The NVRAM Manager module has to be initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-102775
 *  \trace       SPEC-5254, SPEC-5139
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_Init(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA) ConfigPtr);

/**********************************************************************************************************************
 * ComM_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for initialization of ComM_InitStatus
 *  \details     Service to initialize module global variable ComM_InitStatus at power up. This function can be used to initialize the
 *               variables in *_INIT_* sections in case they are not initialized by the startup code.
 *               This function has to be called once during start-up and before ComM_Init() is called.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-102775
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_InitMemory(void);

/**********************************************************************************************************************
 * ComM_DeInit()
 *********************************************************************************************************************/
/*! \brief       This function stores non-volatile values in NVRAM. Additionally it de-initializes ComM and sets the
 *               initialization status to COMM_UNINIT if all channels are in state COMM_NO_COM_NO_PENDING_REQUEST.
 *  \details     -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       CREQ-102778
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_DeInit(void);

/**********************************************************************************************************************
 * ComM_GetStatus()
 *********************************************************************************************************************/
/*! \brief       Queries the initialization status of the module.
 *  \details     -
 *  \param[in]   Status                  Valid pointer where the current ComM state shall be stored
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-102775
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus(P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_VAR) Status);

/**********************************************************************************************************************
 * ComM_GetState()
 *********************************************************************************************************************/
/*! \brief       Queries the current communication state of the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[out]  State                   Valid pointer where the current ComM state shall be stored
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The function is used by EcuM fixed and by BswM.
 *  \trace       CREQ-102779
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetState( NetworkHandleType Channel, P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_VAR) State);

/* PRQA S 3451, 3449 EXTERNDECLARATIONS */ /* MD_ComM_3451_3449 */
/**********************************************************************************************************************
 * ComM_RequestComMode()
 *********************************************************************************************************************/
/*! \brief       Requesting of a Communication Mode by a user.
 *  \details     -
 *  \param[in]   User                    Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[in]   ComMode                 The requested communication mode (COMM_FULL_COMMUNICATION or COMM_NO_COMMUNICATION)
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *               COMM_E_MODE_LIMITATION  COMM_FULL_COMMUNICATION is requested but mode limitation is active
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       CREQ-1267
 *  \trace       SPEC-5108, SPEC-5440, SPEC-5434, SPEC-5358, SPEC-5112, SPEC-5082, SPEC-5099, SPEC-5094, SPEC-5089
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_RequestComMode(ComM_UserHandleType User, ComM_ModeType ComMode);

/**********************************************************************************************************************
 * ComM_GetMaxComMode()
 *********************************************************************************************************************/
/*! \brief       Function to query the maximum allowed communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[out]  ComMode                Valid pointer where to store the maximal communication mode of the given user
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Invalid parameter
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1267
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetMaxComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode);

/**********************************************************************************************************************
 * ComM_GetRequestedComMode()
 *********************************************************************************************************************/
/*! \brief       Function to query the requested communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[out]  ComMode                Valid pointer where to store the requested communication mode of the given user
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Invalid parameter
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1267
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetRequestedComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode);

/**********************************************************************************************************************
 * ComM_GetCurrentComMode()
 *********************************************************************************************************************/
/*! \brief       Function to query the current communication mode of the corresponding user.
 *  \details     -
 *  \param[in]   User                   Valid handle of the User, can be found in ComM_Cfg.h
 *  \param[out]  ComMode                Valid pointer where to store the current communication mode of the given user
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Invalid parameter
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-5091
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode);

/**********************************************************************************************************************
 * ComM_GetCurrentPNCComMode()
 *********************************************************************************************************************/
/*! \brief       Function to query the current communication mode of the corresponding PNC the user is assigned to.
 *  \details     -
 *  \param[in]   User                          Valid handle of the User, can be found in ComM_Lcfg.h
 *  \param[out]  ComMode                       Valid pointer where to store the current communication mode of the given user
 *  \return      E_OK                          Request is accepted
 *               E_NOT_OK                      Invalid parameter
 *               E_MULTIPLE_PNC_ASSIGNED       Multiple PNCs are assigned to the user
 *               E_NO_PNC_ASSIGNED             No PNC is assigned to the affected user
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-292564
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetCurrentPNCComMode(ComM_UserHandleType User, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode);

/**********************************************************************************************************************
 * ComM_GetInhibitionStatus()
 *********************************************************************************************************************/
/*! \brief       This function gets the current inhibition status of the given channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                  Valid pointer where the inhibition status shall be stored
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetInhibitionStatus( NetworkHandleType Channel, P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_VAR) Status);

/**********************************************************************************************************************
 * ComM_PreventWakeUp()
 *********************************************************************************************************************/
/*! \brief       Requests or releases Prevent Wake-up for a ComM channel
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                 TRUE: request Wake-up Inhibition
 *                                      FALSE: release Wake-up Inhibition
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Request is ignored if one of the following occurs
 *                                      - Channel parameter is invalid or
 *                                      - 'Wake-Up Inhibition Enabled' is de-activated in the module configuration or
 *                                      - 'ECU Group Classification' does not support Prevent Wake-Up
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-5113
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_PreventWakeUp( NetworkHandleType Channel, boolean Status);

/**********************************************************************************************************************
 * ComM_LimitChannelToNoComMode()
 *********************************************************************************************************************/
/*! \brief       Requests or releases Mode Limitation for a ComM channel
 *  \details     -
 *  \param[in]   Channel                Valid channel identifier (network handle), can be found in ComM_Cfg.h
 *  \param[in]   Status                 TRUE: request Mode Limitation
 *                                      FALSE: release Mode Limitation
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               - the channel has Nm Variant NONE or
 *                                      - ECU Group Classification supports Mode Limitation
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-5453, SPEC-5433
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_LimitChannelToNoComMode( NetworkHandleType Channel, boolean Status);

/**********************************************************************************************************************
 * ComM_LimitECUToNoComMode()
 *********************************************************************************************************************/
/*! \brief       Requests or releases Mode Limitation for all ComM channels
 *  \details     -
 *  \param[in]   Status                 TRUE: request Mode Limitation
 *                                      FALSE: release Mode Limitation
 *  \return      E_OK                   Request is accepted
 *  \return      E_NOT_OK               Request is ignored if one of the following occurs
 *                                      - The service is called on another partition than the master partition
 *                                      - Mode Limitation to NO_COM is disabled or
 *                                      - ECU Group Classification does not support Mode Limitation or
 *                                      - The API ComM_LimitChannelToNoComMode returned E_NOT_OK for at least one channel
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode(boolean Status);

/**********************************************************************************************************************
 * ComM_ReadInhibitCounter()
 *********************************************************************************************************************/
/*! \brief       Queries the inhibition counter, which is number of rejected Full Communication user requests.
 *  \details     -
 *  \param[out]  CounterValue           Valid pointer where the value of inhibition counter shall be stored
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Invalid parameter
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1301
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ReadInhibitCounter(P2VAR(uint16, AUTOMATIC, COMM_APPL_VAR) CounterValue);

/**********************************************************************************************************************
 * ComM_ResetInhibitCounter()
 *********************************************************************************************************************/
/*! \brief       Resets the inhibition counter, which is number of rejected Full Communication user requests.
 *  \details     -
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               The service is called on another partition than the master partition
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1301
 *  \trace       SPEC-5099
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ResetInhibitCounter(void);

/**********************************************************************************************************************
 * ComM_SetECUGroupClassification()
 *********************************************************************************************************************/
/*! \brief       Sets the value of ECU group classification status during runtime. The value is stored non-volatile.
 *  \details     -
 *  \param[in]   Status                 The value of ECU group classification. Defines Mode Inhibition types the ECU is affected by:
 *                                      0 - ECU is not affected by mode inhibition
 *                                      1 - ECU is affected by Wake-up Inhibition only
 *                                      2 - ECU is affected by Mode Limitation only
 *                                      3 - ECU is affected by both inhibition types
 *  \return      E_OK                   Request is accepted
 *               E_NOT_OK               Invalid parameter
 *               COMM_E_UNINIT          ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1302
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_SetECUGroupClassification(ComM_InhibitionStatusType Status);
/* PRQA L:EXTERNDECLARATIONS */

# if (COMM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * ComM_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Provides the version information of the module.
 *  \details     -
 *  \param[in]   Versioninfo             Valid pointer where the version information shall be stored
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
 *  \trace       CREQ-102776
*********************************************************************************************************************/
extern FUNC(void, COMM_CODE) ComM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, COMM_APPL_VAR) Versioninfo);
# endif

/* PRQA S 3451, 3449 EXTERNDECLARATIONS */ /* MD_ComM_3451_3449 */
# if (COMM_PNC_TO_CHANNEL_ROUTING_LIMITATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 * ComM_LimitPncToChannelRouting()
 *********************************************************************************************************************/
/*! \brief       Service to set the status of PNC to Channel Routing Limitation
 *  \details     The function stores the limitation status for the given PNC and Channel. The status will be used in combination with
                 some other conditions (current Nm state, receiving of ERA signal) to decide whether the routing of PNC information on
                 the channel is active or not. The decision and corresponding actions are taken in the next ComM_MainFunction().
 *  \param[in]   Pnc                    Valid PNC identifier
 *  \param[in]   Channel                Valid channel identifier (network handle)
 *  \param[in]   Status                 TRUE: limitation of the PNC on the Channel shall be activated
 *                                      FALSE: limitation of the PNC on the Channel shall be de-activated (default status after initialization)
 *  \return      E_OK                   parameters are correct, ComM accepted the request
 *               E_NOT_OK               parameters are incorrect, ComM discarded the request
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous FALSE
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_LimitPncToChannelRouting( PNCHandleType Pnc, NetworkHandleType Channel, boolean Status );
# endif
/* PRQA L:EXTERNDECLARATIONS */

#if (COMM_DCM_INDICATION == STD_ON)
/**********************************************************************************************************************
 * ComM_GetDcmRequestStatus()
 *********************************************************************************************************************/
/*! \brief       Queries the status of DCM active diagnostic request of the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  Status                  Valid pointer where the request status shall be stored
 *                                       TRUE: DCM indicated active diagnostic
 *                                       FALSE: otherwise
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_DCM_INDICATION == STD_ON)
 *  \trace       CREQ-102677
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetDcmRequestStatus(NetworkHandleType Channel, P2VAR(boolean, AUTOMATIC, COMM_APPL_VAR) Status);
# endif

# if (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 * ComM_GetMinFullComModeTimerStatus()
 *********************************************************************************************************************/
/*! \brief       Queries the status of Min Full Com Mode Timer of the corresponding channel.
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  Status                  Valid pointer where the timer status shall be stored
 *                                       TRUE: Min Full Com Mode Timer is running
 *                                       FALSE: otherwise
 *  \return      E_OK                    Request is accepted
 *               E_NOT_OK                Invalid parameter
 *               COMM_E_UNINIT           ComM is not initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_MINFULLCOMTIMEOFCHANNEL == STD_ON)
 *  \trace       CREQ-1306
*********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetMinFullComModeTimerStatus(NetworkHandleType Channel, P2VAR(boolean, AUTOMATIC, COMM_APPL_VAR) Status);
# endif

#if (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
/***********************************************************************************************************************
 *  ComM_InternalBusSmModeIndication()
 **********************************************************************************************************************/
/*! \brief       Request the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL).
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  ComMode                 Valid pointer to the communication mode which shall be requested
 *  \return      E_OK                    Request is accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_InternalBusSmModeIndication(NetworkHandleType Channel, VAR(ComM_ModeType, AUTOMATIC) ComMode);

/***********************************************************************************************************************
 *  ComM_InternalGetCurrentBusSmMode()
 **********************************************************************************************************************/
/*! \brief       Query the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL).
 *  \details     -
 *  \param[in]   Channel                 Valid channel identifier (network handle)
 *  \param[out]  ComMode                 Valid pointer where the communication mode shall be stored
 *  \return      E_OK                    Request is accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (COMM_BUS_TYPE_INTERNAL_PRESENT == STD_ON)
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, COMM_CODE) ComM_InternalGetCurrentBusSmMode(NetworkHandleType Channel, P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_VAR) ComMode);
#endif

# define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_0
 * Protects the ComM active channel state user request status.
 * \protects ComM_ActiveComMode, ComM_UserReqFullCom, ComM_UserReqPncFullCom
 * \usedin ComM_CalculateHighestComMode, ComM_CalculateHighestPNComMode, ComM_TF_SiCom_NoCom, ComM_RequestComMode, 
 *         ComM_GetRequestedComMode, ComM_GetCurrentComMode, ComM_PreventWakeUp, ComM_LimitChannelToNoComMode,
 *         ComM_PncProcessRxSignalEra, ComM_SyncUserFullComRequests
 * \exclude -
 * \length LONG This exclusive area covers calls to several sub-functions.
 * \endexclusivearea
 */

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_1
 * Protects the ComM active channel state from becoming inconsistent while execution of the main function.
 * \protects ComM_ActiveComMode
 * \usedin ComM_MainFunction
 * \exclude Nm_MainFunction, BusNm_MainFunction (e.g. CanNm_MainFunction), BusSM_MainFunction (e.g. CanSM_MainFunction)
 * \length LONG This exclusive area covers calls to several sub-functions.
 * \endexclusivearea
 */

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_2
 * Protects the synchronization of the ComM channel state variables from becoming inconsistent while being accessed on multiple partitions.
 * \protects ComM_ActiveComMode, ComM_ActiveComModeSyncCounter, ComM_BusComModeReq, ComM_BusComModeReqSyncCounter,
             ComM_BusComModeReqFrShutdown, ComM_BusComModeReqFrShutdownSyncCounter, ComM_EcuMWakeUpIndicationSyncCounter,
             ComM_NmStartUpIndicationSyncCounter
 * \usedin ComM_EcuM_WakeUpIndication, ComM_BusSM_ModeIndication, ComM_Nm_PrepareBusSleepMode,
 *         ComM_BusSM_BusSleepMode, ComM_Nm_NetworkMode, ComM_Nm_BusSleepMode,
 *         ComM_ProcessPassiveWakeUp, ComM_UpdateDataFromSatellites, ComM_SyncBusComModeRequests
 * \exclude -
 * \length LONG This exclusive area covers calls to several sub-functions.
 * \endexclusivearea
 */

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_3
 * Protects the synchronization of DCM requests and state changes due to BusSM indications from becoming inconsistent while being
   accessed on multiple partitions.
 * \protects ComM_DcmRequestActive, ComM_DcmRequestActiveSyncCounter, ComM_StateChange, ComM_StateChangeSyncCounter
 * \usedin ComM_DCM_ActiveDiagnostic, ComM_DCM_InactiveDiagnostic, ComM_BusSM_ModeIndication, ComM_UpdateDataFromSatellites
 * \exclude -
 * \length SHORT no calls inside and at max 2 bytes are copied within this exclusive area.
 * \endexclusivearea
 */

 /*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_4
 * Protects the synchronization of PNC signal values and PNC to Channel Routing states from becoming inconsistent while being
   accessed on multiple partitions.
 * \protects ComM_PncSignalValues, ComM_PncSignalValuesSyncCounter, ComM_PncToChannelRoutingState, ComM_PncToChannelRoutingStateSyncCounter
 * \usedin ComM_PncProcessRxSignalEra, ComM_PncProcessSignalRxEira, ComM_Nm_StateChangeNotifiaction,
 *         ComM_PncSetBitInSignal, ComM_PncClearBitInSignal, ComM_UpdateDataFromSatellites, ComM_Nm_StateChangeNotification
 * \exclude -
 * \length SHORT no calls inside and at max 2 bytes are copied within this exclusive area.
 * \endexclusivearea
 */

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_5
 * Protects the synchronization of extern PNC requests from becoming inconsistent while being accessed on multiple partitions.
 * \protects ComM_BusPncComModeReq, ComM_BusPncComModeReqSyncCounter
 * \usedin ComM_PncProcessRxSignalEira, ComM_SyncBusPncComModeRequests
 * \exclude -
 * \length MEDIUM no calls inside, updating several EIRA RX signal values (depending on the configuration).
 * \endexclusivearea
 */

/*!
 * \exclusivearea COMM_EXCLUSIVE_AREA_6
 * Protects the synchronization of forwarding the BusSM requests and COM Signal updates to the respective partition from 
   becoming inconsistent while being accessed on multiple partitions.
 * \protects ComM_ForwardComSendSignalSyncCounter, ComM_ForwardingComSendSignalIndex, ComM_ForwardRequestBusSmModeSyncCounter,
 *           ComM_ForwardingBusSmMode
 * \usedin ComM_PncRoutingLimitationHandleChannels, ComM_PncModeSendSignal, ComM_TF_NoCom_NetReq,
 *         ComM_TF_NoCom_FullReadySleep, ComM_TF_Full_SiCom, ComM_TF_ReadyS_NetReq, ComM_TF_SiCom_NoCom, ComM_TF_NetReq_ReadyS.
 *         ComM_MainFunction_Satellite
 * \exclude -
 * \length MEDIUM no calls inside, updating several EIRA RX signal values (depending on the configuration).
 * \endexclusivearea
 */

#endif  /* COMM_H */

/**********************************************************************************************************************
  END OF FILE: ComM.H
**********************************************************************************************************************/


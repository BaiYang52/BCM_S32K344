/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!     \file      FiM.h
 *      \brief     Header for Autosar 4 module FiM
 *
 *      \details   Static header file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *            2012-08-22  visjk   ---           3.01.00: AR 3 FiM version that this module was originally based on
 *                        visktw  ---           ditto
 *  1.00.00   2012-10-11  vistne  ESCAN00061742 AR4-148: FiM AR4 Support
 *                        vistne  ESCAN00061816 AR4-220: Remove STATIC
 *  1.01.00   2013-03-15  vistne  ESCAN00063875 Add Offline Calibration Support
 *                        vistne  ESCAN00063883 FiM include hierarchy does not conform to AR standard
 *                        vistne  ESCAN00064191 Support Cyclic Event Evaluation
 *                        vistne  ESCAN00064193 Support of FiM_GetFunctionPendingStatus
 *  1.02.00   2013-06-28  vistne  ESCAN00067461 Include of Dem.h was moved to different file
 *                        vistne  ESCAN00067956 Support absence of RTE generated files
 *  2.00.00   2013-11-01  vistne  ESCAN00070055 Compiler error: FiM.c(XXX):'FiM_FidPendingCounter':undeclared identifier
 *                        vistne  ESCAN00070532 Possible out of bounds access to EventIdTable
 *                        vistne  ESCAN00070624 New Binary Search Algorithm for EventIds
 *                        vistne  ESCAN00070687 New Direct Table Access for EventIds
 *  2.01.00   2014-03-07  vistne  ESCAN00071946 Harmonize usage of boolean and Boolean
 *                        vistne  ESCAN00072079 Pending status of FID is influenced by event even if corresponding
 *                                              event/FID link has invalid inhibition configuration
 *                        vistne  ESCAN00072092 AR4-580: FiM Inhibition Config Optimizations
 *                        vistne  ESCAN00072428 AR4-580: FiM Postbuild Loadable
 *  3.00.00   2014-10-31  vistne  ESCAN00078673 Introduce DET code FIM_E_INITIALIZATION_NOT_COMPLETED
 *                        vistne  ESCAN00078676 FiM_GetFunctionPermission and FiMGetFunctionPendingStatus return
 *                                              E_NOT_OK in all error cases
 *  3.01.00   2015-03-20  vistne  ESCAN00080166 Return value for FiM_DemTriggerOnEventStatus does not comply to
 *                                              AUTOSAR-Standard
 *                        vistne  ESCAN00080369 FiM_GetFunctionPermission and FiMGetFunctionPendingStatus do
 *                                              plausibility checks only if FIM_DEV_ERROR_DETECT is enabled
 *                        vistne  ESCAN00080605 Ignore events when Dem_GetEventStatus does not return E_OK
 *                        vistne  ESCAN00080284 FEAT-427: SafeBSW Step I
 *  4.00.00   2016-01-08  vistne  ESCAN00083200 Remove inclusion of v_ver.h
 *                        vistne  ESCAN00083639 Compiler warning: narrowing or signed-to-unsigned type conversion found
 *                        vistne  ESCAN00085144 FEAT-1527: SafeBSW Step 3
 *                        vistne  ESCAN00086406 Reduce number of entries into critical sections
 *  4.01.00   2016-07-27  vistne  ESCAN00089387 FEAT-1734: SafeBSW Step 5
 *  4.02.00   2016-11-18  vistne  FEATC-12      FEAT-1920: FiM - SafeBSW Step6
 *                        vistne  ESCAN00091090 Incorrect initialization status detection
 *                        vistne  ESCAN00091787 Removed Critical Sections in FiM_GetFunctionPermission and
 *                                              FiM_GetFunctionPendingStatus
 *  5.00.00   2017-10-27  vistne  STORY-12      Rework for AR 4.3 OnDemand Calculation - Beta, Component Only
 *                                STORY-2524    Rework for AR 4.3 OnDemand Calculation - Beta, DemIntegrated
 *  5.01.00   2017-12-22  vistne  STORY-2522    Rework for AR 4.3 OnDemand Calculation (incl. OBD) - Release
 *  6.00.00   2018-08-24  vistne  STORY-3099    Extend FiM for Multi Core Usage - Dev
 *                        vistne  ESCAN00098975 OS protection hook may be triggered by FiM_SetSearchEventIdTableFct
 *  6.01.00   2018-09-28  vistne  STORY-7813    Implement FiM_PostInit - Dev
 *  6.02.00   2018-11-23  vistne  STORY-3100    Extend FiM for Multi Core Usage - Prod
 *  6.03.00   2018-12-21  vistne  STORY-9585    Remove Compiler Warning in FiM.c: conversion from const
 *                                              FiM_EventToFidPendingTableType to FiM_FunctionIdType
 *  6.04.00   2019-03-15  vistne  STORY-3100    Extend FiM for Multi Core Usage - Prod
 *  6.04.01   2019-04-02  vistne  ESCAN00102724 Source Files contain Beta Disclaimer
 *  7.00.00   2020-04-09  vistne  DCM-2359      FiM: Introduce 2nd Exclusive Area for Core Local Synchronization
 *                        vistne  DIAG-312      FiM Satellite in ASIL partition for partitioning solution
 *  8.00.00   2020-08-07  vistne  DCM-2391      Update FiM's Inhibition Configuration according AR 4.3
 *  9.00.00   2021-02-12  vistne  DCM-3609      Affected FIDs Handling for Cyclic FID Calculation:
 *                                              Design, Implementation, Test, Release
 *                                DCM-3715      FiM: Units - Design, Implementation
 *                                              Removed Author Identity
 *  9.01.00   2022-12-16  vistne  DEM-7763      ESCAN00111496: Endless loop during initialization of FiM
 *                        vistne  DEM-7765      ESCAN00109734: Missing MemMap guard for definition of FiM_ConfigDataPtr
 *                        vistne  DEM-7767      ESCAN00112991: Undefined behavior if APIs FiM_DemTriggerOnMonitorStatus
 *                                              or FiM_DemInitSatellite are invoked
 *                        vistne  DEM-7382      ESCAN00112480: Compiler warning: enumerated type mixed with another type
 *                                              (ApplicationType)
 ***********************************************************************************************************************/
#if !defined(FIM_H)
#define FIM_H


/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "FiM_Cfg.h"
#include "FiM_Types.h"
#include "FiM_Lcfg.h"
#include "FiM_PBcfg.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Vendor and module identification */
#define FIM_VENDOR_ID                               (30u) /* Vector Informatik GmbH */
#define FIM_MODULE_ID                               (11u) /* FIM */
/* AUTOSAR Software Specification Version Information - decimal encoding */
#define FIM_AR_RELEASE_MAJOR_VERSION                (4u)
#define FIM_AR_RELEASE_MINOR_VERSION                (3u)
#define FIM_AR_RELEASE_REVISION_VERSION             (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define FIM_SW_MAJOR_VERSION                        (9u) /* decimal coded version number         */
#define FIM_SW_MINOR_VERSION                        (1u)
#define FIM_SW_PATCH_VERSION                        (0u) /* decimal coded release version number */

#define FIM_INSTANCE_ID_DET                         (0x00u)

/*! * \defgroup  FimDetSids API Service IDs for reporting errors to the DET
 * API Service IDs for reporting errors to the DET with Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID, ServiceId,
 * ErrorId)
 * \{ */
#define FIM_SID_FIM_INIT                            (0x00u) /*!< Service ID: FiM_Init() */
#define FIM_SID_FIM_GETFUNCTIONPERMISSION           (0x01u) /*!< Service ID: FiM_GetFunctionPermission() */
#define FIM_SID_FIM_DEMTRIGGERONMONITORSTATUS       (0x02u) /*!< Service ID: FiM_DemTriggerOnMonitorStatus() */
#define FIM_SID_FIM_DEMINIT                         (0x03u) /*!< Service ID: FiM_DemInit() */
#define FIM_SID_FIM_GETVERSIONINFO                  (0x04u) /*!< Service ID: FiM_GetVersionInfo() */
#define FIM_SID_FIM_MAINFUNCTION                    (0x05u) /*!< Service ID: FiM_MainFunction() */
/* Additional API Service IDs for reporting errors to the DET */
# if (FIM_FEATURE_OBD == STD_ON)
#  define FIM_SID_FIM_GETFUNCTIONPENDINGSTATUS      (0x80u) /*!< Service ID: FiM_GetFunctionPendingStatus() */
#  define FIM_SID_FIM_DEMTRIGGERONEVENTSTATUS       (0x82u) /*!< Service ID: FiM_DemTriggerOnEventStatus() */
#endif
#define FIM_SID_FIM_DEMINITMASTER                   (0x83u) /*!< Service ID: FiM_DemInitMaster() */
#define FIM_SID_FIM_DEMINITSATELLITE                (0x84u) /*!< Service ID: FiM_DemInitSatellite() */
#define FIM_SID_FIM_POSTINIT                        (0x85u) /*!< Service ID: FiM_PostInit() */
/*! \} */

/*! * \defgroup  FimDetErrId ErrorIds for reporting errors to the DET
 * ErrorIds for reporting errors to the DET
 * \{ */
#define FIM_E_NO_ERROR                              (0x00u) /*!< used to check if no error occurred */
#define FIM_E_UNINIT                                (0x01u) /*!< API function called before the FiM module has been fully initialized (AR4.3) */
#define FIM_E_FID_OUT_OF_RANGE                      (0x02u) /*!< FiM_GetFunctionPermission/FiM_GetFunctionPendingStatus called with wrong FID (AR4.3) */
#define FIM_E_EVENTID_OUT_OF_RANGE                  (0x03u) /*!< Dem calls FiM with invalid EventId (AR4.3) */
#define FIM_E_PARAM_POINTER                         (0x04u) /*!< API is invoked with NULL pointer (AR4.3) */
#define FIM_E_INIT_FAILED                           (0x05u) /*!< Invalid configuration set selection (e.g.) (AR4.3) */
/*! \} */

/*! * \defgroup  IdxInhModTable Indexes for inhibition configuration.
 * Indexes for inhibition configuration.
 * \{ */
#define FIM_INHCODE_INVALID                         (0u)
#define FIM_INHCODE_LAST_FAILED                     (1u)
#define FIM_INHCODE_NOT_TESTED                      (2u)
#define FIM_INHCODE_TESTED                          (3u)
#define FIM_INHCODE_TESTED_AND_FAILED               (4u)
#define FIM_INHCODE_NOT_TESTED_OR_FAILED            (5u)
#define FIM_INHCODE_PENDING                         (6u)
#define FIM_INHCODE_NUMBER                          (7u)
/*! \} */

/*! Invalid DEM event id. */
#define FIM_DEM_EVENT_INVALID                       (0u)

/*! Minimum list length necessary to apply binary search algorithm. */
#if !defined FIM_BINSEARCH_MINLISTLENGTH                                                  /* COV_MSR_COMPATIBILITY TX */
# define FIM_BINSEARCH_MINLISTLENGTH                (5u)
#endif

/*! FiM is in uninitialized state */
#define FIM_UNINITIALIZED                           (0U)
/*! FiM is in initialized state (FiM_Init and FiM_DemInit were executed) */
#define FIM_INITIALIZED                             (1U)
/*! FiM is in pre-initialized state (FiM_Init was executed but not FiM_DemInit) */
#define FIM_PREINITIALIZED                          (2U)

#if (FIM_DATA_FIXED == STD_ON)
#  error "Invalid configuration: Calibration not supported"
#endif

#if (FIM_CYCLIC_EVENT_EVALUATION == STD_ON)
#  error "Invalid configuration: FIM_CYCLIC_EVENT_EVALUATION not supported"
#endif


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
extern P2CONST(FiM_ConfigType, FIM_VAR_NOINIT, FIM_INIT_DATA)  FiM_ConfigDataPtr;
#endif

#define FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  FiM_InitMemory()
 **********************************************************************************************************************/
/*! \brief                    Use this function to initialize static RAM variables in single partition systems (master
 *                            and one satellite).
 *                            - Extension to Autosar -
 *  \details                  Use when the start-up code is not used to initialize RAM.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \trace                    CREQ-103737
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_InitMemory(void);


/***********************************************************************************************************************
 *  FiM_Init()
 **********************************************************************************************************************/
/*! \brief                    Initializes FiM
 *
 *  \details                  This function shall be used during the startup phase of the ECU before
 *                            the DEM is initialized to setup the FIM's internal variables for operation.
 *                            The component state is set to pre-initialized if no error occurred.
 *                            The FiM is not yet ready for operation afterwards.
 *
 *  \param[in]  FiMConfigPtr  Root pointer to FiM's configuration.
 *
 *  \pre                      FiM_InitMemory has been called unless FiM_InitializationStatus is initialized by
 *                            start-up code.
 *  \context                  TASK,
 *                            if any part of the FiM runs in a trusted partition, this function may only be called from
 *                            one of these trusted partitions
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22910
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_Init
                                  (P2CONST(FiM_ConfigType,
                                   AUTOMATIC, FIM_INIT_DATA) FiMConfigPtr);

/***********************************************************************************************************************
 *  FiM_PostInit()
 **********************************************************************************************************************/
/*! \brief                    Calculates common status variable after FiM has been initialized.
 *                            - Extension to Autosar -
 *
 *  \details                  This function calculates the cumulated initialization status of all satellites.
 *
 *  \pre                      FiM master and all satellites have been initialized
 *                            (FiM_DemInitMaster/FiM_DemInitSatellite have been called and finished successfully)
 *  \context                  TASK,
 *                            if any part of the FiM runs in a trusted partition, this function may only be called from
 *                            one of these trusted partitions
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM23396
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_PostInit(void);


/***********************************************************************************************************************
 *  FiM_DemInit()
 **********************************************************************************************************************/
/*! \brief                    This service (re-)initializes the FiM master and the first satellite with
 *                            current status of DEM.
 *
 *  \details                  This is a compatibility function for single core systems where only one master and one
 *                            satellite exist and are on the same OS partition.
 *                            Function shall be called by DEM to (re-)initialize the FIM when
 *                            - the DEM has been initialized and FIM can query monitor and event status for initial FID
 *                              calculation or
 *                            - the DEM detects a status change of a certain number of events.
 *                            CAUTION: During the complete runtime of FiM_DemInit, FiM_DemTriggerOnEventStatus
 *                            must not be called!
 *                            NOTE: Interruption by FiM_DemTriggerOnMonitorStatus is ok.
 *                            CAUTION: The FiM loops through all configured events or FIDs and calls 
 *                            Dem_GetEventUdsStatus and Dem_GetMonitorStatus for each of them.
 *                            Depending on the configuration, FiM_DemInit can take a considerable time.
 *  \pre                      FiM needs to be at least pre-initialized.
 *  \pre                      May only be called for single partition systems (master and only one satellite).
 *  \context                  TASK, master partition in single partition systems
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22910, DSGN-FiM22867, DSGN-FiM22936, DSGN-FiM23363
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemInit(void);

/***********************************************************************************************************************
 *  FiM_DemInitMaster()
 **********************************************************************************************************************/
/*! \brief                    This service (re-)initializes the FiM master with current status of DEM.
 *                            - Extension to Autosar -
 *
 *  \details                  Function shall be called by DEM to (re-)initialize the FIM master when
 *                            - the DEM has been initialized and FIM can query event status for initial FID
 *                              calculation or
 *                            - the DEM detects a status change of a certain number of events.
 *                            CAUTION: During the complete runtime of FiM_DemInitMaster, FiM_DemTriggerOnEventStatus
 *                            must not be called!
 *                            CAUTION: The FiM loops through all configured events and inhibition configurations and
 *                            calls Dem_GetEventUdsStatus for each of them.
 *                            Depending on the configuration, FiM_DemInitMaster can take a considerable time.
 *  \pre                      FiM master needs to be at least pre-initialized.
 *  \context                  TASK, master partition
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22910, DSGN-FiM22867
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemInitMaster(void);

/***********************************************************************************************************************
 *  FiM_DemInitSatellite()
 **********************************************************************************************************************/
/*! \brief                    This service (re-)initializes a FiM satellite with current status of DEM.
 *                            - Extension to Autosar -
 *
 *  \details                  Function shall be called by DEM to (re-)initialize a FIM satellite when
 *                            - the DEM has been initialized and FIM satellite can query monitor status
 *                              for initial FID calculation or
 *                            - the DEM detects a status change of a certain number of events.
 *                            NOTE: Interruption by FiM_DemTriggerOnMonitorStatus is ok.
 *                            CAUTION: The FiM loops through all configured events and inhibition configurations and
 *                            calls Dem_GetMonitorStatus for each of them.
 *                            Depending on the configuration, FiM_DemInitSatellite can take a considerable time.
 *
 *  \param[in]  applicationId OS application ID of satellite to be initialized
 *  \pre                      FiM satellite needs to be at least pre-initialized.
 *  \context                  TASK, satellite partition of calling DEM satellite
 *  \reentrant                FALSE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22910, DSGN-FiM22936, DSGN-FiM23363, DSGN-FiM22867
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemInitSatellite(ApplicationType applicationId);

/***********************************************************************************************************************
 *  FiM_GetFunctionPermission()
 **********************************************************************************************************************/
/*! \brief      This function shall be used to query the permission of the FID.
 *
 *  \details    - This function  reports the permission state of the functionality assigned to the FID.
 *              - If development error detection is enabled:
 *                - Permission will be set to FALSE, if the FIM is not initialized or if the FID is not valid.
 *                - If development error reporting is enabled, an error will additionally be reported to the DET.
 *
 *  \param[in]  FID           Function ID that the query is for (identifies a functionality).
 *                            Min.:   0
 *                            Max.:   Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 *  \param[out] Permission    Must point to a variable of type boolean.
 *                            TRUE:   FID has permission to run 
 *                            FALSE:  FID has no permission to run, i.e. shall not be executed 
 *  \return                   E_OK:     The request is accepted, Permission could be obtained and is valid
 *                            E_NOT_OK: The request is not accepted, e.g. initialization of FIM not completed 
 *                                      Permission could not be obtained.
 *  \pre                      -
 *  \context                  TASK|ISR2, any partition
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \trace                    DSGN-FiM22861
 **********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPermission
                                  (FiM_FunctionIdType FID,
                                   P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission
                                  );

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_GetFunctionPendingStatus()
 **********************************************************************************************************************/
/*! \brief      This function shall be used to query the pending status of the events connected to the FID.
 *              - Extension to Autosar -
 *
 *  \details    - This function is used in context of IUMPR calculation for OBD.
 *              - It reports the pending status of the event assigned to the FID.
 *              - If development error detection is enabled:
 *                - pending status will be set to FALSE, if the FIM is not initialized or if the FID is not valid,
 *                - if development error reporting is enabled, an error will additionally be reported to the DET.
 *
 *  \param[in]  FID           Function ID that the query is for (identifies a functionality).
 *                            Min.:   0
 *                            Max.:   Result of configuration of FIDs in FIM (Max is either 255 or 65535)
 *  \param[out] pendingStatus Must point to a variable of type boolean.
 *                            TRUE:   any event connected to FID has status bit DEM_UDS_STATUS_PDTC set
 *                            FALSE:  no event connected to FID has status bit DEM_UDS_STATUS_PDTC set
 *  \return                   E_OK:     The request is accepted, pending status could be obtained and is valid
 *                            E_NOT_OK: The request is not accepted, e.g. initialization of FIM not completed 
 *                                      Pending status could not be obtained.
 *  \pre                      -
 *  \context                  TASK|ISR2, any partition
 *  \reentrant                TRUE
 *  \synchronous              TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON (OBD license available and OBD enabled at compile time)
 *  \trace                    DSGN-FiM23008
 **********************************************************************************************************************/
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPendingStatus
                                  (FiM_FunctionIdType FID, 
                                   P2VAR(boolean, AUTOMATIC, FIM_DEM_DATA) pendingStatus
                                  );
#endif


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  FiM_DemTriggerOnEventStatus()
 **********************************************************************************************************************/
/*! \brief      Function is provided to DEM in order to call FIM upon status changes.
 *
 *  \details                    This function is provided to DEM in order to call FIM upon status changes.
 *                              CAUTION: This function may not be interrupted by a function call of the same kind
 *                              with the same event id! Interruption of FiM_DemTriggerOnEventStatus by
 *                              FiM_DemTriggerOnEventStatus is only allowed if the event ids of both calls differ.
 *  \param[in]  EventId         Identification of an Event by assigned event number.
 *                              The Event Number is configured in the DEM. 
 *                              Min.: 1 (0: Indication of no Event or Failure) 
 *                              Max.: depending on configuration of Event Numbers in DEM (maximum possible is 65535) 
 *  \param[in]  EventStatusOld  extended event status before change
 *  \param[in]  EventStatusNew  extended event status after change
 *  \pre                        -
 *  \context                    TASK|ISR2, master partition
 *  \reentrant                  TRUE (but not for the same event id!)
 *  \synchronous                TRUE
 *  \trace                      DSGN-FiM22911, DSGN-FiM23259
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemTriggerOnEventStatus
                                  (Dem_EventIdType EventId,
                                   Dem_UdsStatusByteType EventStatusOld,
                                   Dem_UdsStatusByteType EventStatusNew
                                  );
#endif

/***********************************************************************************************************************
 *  FiM_DemTriggerOnMonitorStatus()
 **********************************************************************************************************************/
/*! \brief      Function is provided to DEM in order to call FIM upon monitor status changes.
 *
 *  \details                    This function is provided to DEM in order to call FIM upon monitor status changes.
 *  \param[in]  EventId         Identification of an Event by assigned event number.
 *                              The Event Number is configured in the DEM. 
 *                              Min.: 1 (0: Indication of no Event or Failure) 
 *                              Max.: depending on configuration of Event Numbers in DEM (maximum possible is 65535)
 *  \pre                        -
 *  \context                    TASK|ISR2, satellite partition of DEM satellite that EventId is mapped to
 *  \reentrant                  TRUE
 *  \synchronous                TRUE
 *  \trace                      DSGN-FiM22911, DSGN-FiM22960
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_DemTriggerOnMonitorStatus
                                  (Dem_EventIdType EventId
                                  );

#if (FIM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  FiM_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief                      This function returns the version information of the FiM.
 *
 *  \details                    The version information contains vendor ID, moduleID, major/minor/patch version number. 
 *                              The version numbers are decimal coded.
 *  \param[out] versioninfo     Pointer to where to store the version information of this module. Must point to a
 *                              variable of type Std_VersionInfoType.
 *  \pre                        -
 *  \context                    TASK|ISR2, any partition
 *  \reentrant                  TRUE
 *  \synchronous                TRUE
 *  \config                     FIM_VERSION_INFO_API == STD_ON
 *  \trace                      CREQ-103738
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_GetVersionInfo
                                  (P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo
                                  );   
#endif

/***********************************************************************************************************************
 *  FiM_MainFunction()
 **********************************************************************************************************************/
/*! \brief                      Main function.
 *
 *  \details                    If cyclic FID calculation is configured:
 *                                - function calculates permission states of FIDs and must be scheduled cyclically
 *                              otherwise
 *                                - function does nothing and therefore needs not be scheduled.
 *  \pre                        -
 *  \context                    TASK, master partition
 *  \reentrant                  FALSE
 *  \synchronous                TRUE
 *  \trace                      DSGN-FiM23377
 **********************************************************************************************************************/
FUNC(void, FIM_CODE)            FiM_MainFunction(void);

#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/*!
 * \exclusivearea FIM_EXCLUSIVE_AREA_0
 *                Ensures consistency of the inhibition status array.
 *                Must synchronize across all processor cores on which a FiM Satellite is located.
 * \protects      FiM_InhStatusTable_Sat_<n>
 * \usedin        FiM_DemInit
 *                FiM_Init (only if CompareAndSwap is not implemented by application)
 *                FiM_DemInitSatellite
 *                FiM_MainFunction (only if CompareAndSwap is not implemented by application)
 *                FiM_DemTriggerOnMonitorStatus (only if CompareAndSwap is not implemented by application)
 * \exclude       -
 * \length        SHORT during runtime          protects a short code section for (re-)setting a bit
 *                                              (only if CompareAndSwap is not implemented by application)
 * \length        MEDIUM during initialization  protects resetting all inhibition states
 * \endexclusivearea
 *
 * \exclusivearea FIM_EXCLUSIVE_AREA_1
 *                Ensures consistency of the FID Pending counters.
 *                Must synchronize locally on one processor core.
 * \protects      FiM_InhStatusTable_Sat_<n>, FiM_FidPendingCounter
 * \usedin        FiM_DemInit
 *                FiM_DemInitMaster
 *                FiM_DemTriggerOnEventStatus
 * \exclude       -
 * \length        SHORT during runtime          protects a short code section for inc-/decrementing a counter
 * \length        MEDIUM during initialization  protects resetting all counters
 * \endexclusivearea
 */


#endif /* FIM_H */

/***********************************************************************************************************************
 *  END OF FILE: FiM.h
 **********************************************************************************************************************/

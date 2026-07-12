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
/**        \file  Dcm_Diag.h
 *         \unit  Diag
 *        \brief  Contains public inline function definitions of Diag unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_DIAG_H)
# define DCM_DIAG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_DiagTypes.h"
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Total number of diagnostic entities that can be verified (use always the one with highest ID as reference) */
# define DCM_DIAG_NUM_CHK_LVLS                                       (DCM_DIAG_CHK_LVL_SUBFUNC + 1u)

/*! Bitmap encoding for the enumerator values below (used to build the AR standard confirmation status values e.g. DCM_RES_POS_OK !) */
# define DCM_DIAG_RES_ANY_OK                                         ((Dcm_ConfirmationStatusType)0x00u) /*!< The response (positive/negative) was successfully sent */
# define DCM_DIAG_RES_ANY_NOT_OK                                     ((Dcm_ConfirmationStatusType)0x01u) /*!< The response (positive/negative) transmission failed */
# define DCM_DIAG_RES_POS_ANY                                        ((Dcm_ConfirmationStatusType)0x00u) /*!< It is a positive response */
# define DCM_DIAG_RES_NEG_ANY                                        ((Dcm_ConfirmationStatusType)0x02u) /*!< It is a negative response */

/*! Diagnostic kernel application notification levels reached during diagnostic service processing.
    This levels are used to determine which finalization callouts shall be performed later (e.g. service execution confirmation, post-handlers etc.) */
# define DCM_DIAG_APPL_NOTIFICATION_NONE                             ((Dcm_DiagApplNotificationType)0x00u) /*!< No application call was made */
# define DCM_DIAG_APPL_NOTIFICATION_OEM                              ((Dcm_DiagApplNotificationType)0x01u) /*!< Manufacturer specific request indication(s) reached (Xxx_Indication()) */
# define DCM_DIAG_APPL_NOTIFICATION_SYS                              ((Dcm_DiagApplNotificationType)0x02u) /*!< System supplier specific request indication(s) reached (Xxx_Indication()) */
# define DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION                     ((Dcm_DiagApplNotificationType)0x04u) /*!< Call the DCM confirmation after service processing */
# define DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR                    ((Dcm_DiagApplNotificationType)0x08u) /*!< Call the post processor function */
# define DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR                ((Dcm_DiagApplNotificationType)0x10u) /*!< Call the fast post processor function */


/*! Diagnostic kernel request queued flags. These are used to keep track of any overlapping diagnostic request in processing with a new one just won a request prioritization
    Especially in case of split-task this mechanism provides a look-ahead whether a new request is already queue to be processed within the low-priority Dcm_DiagTaskWorker() */
# define DCM_DIAG_QUEUE_FLAG_NONE                                    ((Dcm_DiagProcessorFlagType)0x00u) /*!< There is no diagnostic request queued to be processed */
# define DCM_DIAG_QUEUE_FLAG_IS_WAITING                              ((Dcm_DiagProcessorFlagType)0x01u) /*!< There is already a diagnostic request queued (waiting) to be processed */
# define DCM_DIAG_QUEUE_FLAG_IS_ACTIVE                               ((Dcm_DiagProcessorFlagType)0x02u) /*!< There diagnostic request in started processing */

/*! Diagnostic service specific properties */
# define DCM_DIAG_SVC_CFG_PROP_HAS_SUBFUNC                           ((DcmCfg_DiagServicePropertiesType)0x01u) /*!< Has a sub-function parameter */
# define DCM_DIAG_SVC_CFG_PROP_CALL_POST_HDLR_ALWAYS                 ((DcmCfg_DiagServicePropertiesType)0x02u) /*!< Requires the post-handler (Dcm_ServiceXXPostHandler()) to be called always once the service evaluation has started */
# define DCM_DIAG_SVC_CFG_PROP_CALL_SVC_DISPATCHER                   ((DcmCfg_DiagServicePropertiesType)0x04u) /*!< Requires a service dispatching (internal and external service processing) */
# define DCM_DIAG_SVC_CFG_PROP_NEEDS_LOCK                            ((DcmCfg_DiagServicePropertiesType)0x08u) /*!< Requires a service lock */

/*! Getter for the diagnostic session specific P2 timings */
# define Dcm_DiagGetP2Time(sesStateIdx)                              (Dcm_CfgWrapStateSessionInfo((sesStateIdx))->P2ServerTime.P2)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_DiagGetP2StarTime(sesStateIdx)                          (Dcm_CfgWrapStateSessionInfo((sesStateIdx))->P2ServerTime.P2Star)               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Specifies the maximum buffer size required for the final unsolicited response (i.e. after ECU re-programming)
  Note: Currently the longest possible response is for SID 0x10, that has: SID*1 + SF*1 + P2*2 + P2Ex*2 = 6 Bytes */
# define DCM_DIAG_START_UP_FBL_RES_SIZE                              6u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_DIAG_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Diag.h
 *********************************************************************************************************************/

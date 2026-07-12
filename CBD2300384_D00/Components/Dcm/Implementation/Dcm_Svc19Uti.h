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
/**        \file  Dcm_Svc19Uti.h
 *         \unit  Svc19Uti
 *        \brief  Contains public inline function definitions of Service 0x19 Utilities unit.
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
#if !defined(DCM_SVC19UTI_H)
# define DCM_SVC19UTI_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc19UtiTypes.h"
# if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_DemIfTypes.h"
#  include "Dcm_Svc19.h"
#  include "Dcm_Debug.h"
#  include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  if (DCM_SVC_19_01_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_07_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_11_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_12_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED        STD_ON
#  else
#   define DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED        STD_OFF
#  endif

/* DEM API usage optimization */
#  if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETNUMFILTEREDDTC_ENABLED                  STD_ON
#  else
#   define DCM_SVC_19_UTI_GETNUMFILTEREDDTC_ENABLED                  STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_SETDTCFILTER_ENABLED                       STD_ON
#  else
#   define DCM_SVC_19_UTI_SETDTCFILTER_ENABLED                       STD_OFF
#  endif

#  if (DCM_SVC_19_0B_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_0C_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_0D_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_0E_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED             STD_ON
#  else
#   define DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED             STD_OFF
#  endif

#  if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETDTCSEVAM_ENABLED                        STD_ON
#  else
#   define DCM_SVC_19_UTI_GETDTCSEVAM_ENABLED                        STD_OFF
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_SETDTCFILTER_INFO_CLASS_ENABLED            STD_ON
#  else
#   define DCM_SVC_19_UTI_SETDTCFILTER_INFO_CLASS_ENABLED            STD_OFF
#  endif

#  if (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED                   STD_ON
#  else
#   define DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED                   STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED                  STD_ON
#  else
#   define DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED                  STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON) || \
      (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED                     STD_ON
#  else
#   define DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED                     STD_OFF
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETDTCSTATUSAVAILABILITYMASK_ENABLED       STD_ON
#  else
#   define DCM_SVC_19_UTI_GETDTCSTATUSAVAILABILITYMASK_ENABLED       STD_OFF
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETDTCOCCURTIME_ENABLED                    STD_ON
#  else
#   define DCM_SVC_19_UTI_GETDTCOCCURTIME_ENABLED                    STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED                STD_ON
#  else
#   define DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED                STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETDTCSAM_ENABLED                          STD_ON
#  else
#   define DCM_SVC_19_UTI_GETDTCSAM_ENABLED                          STD_OFF
#  endif

#  if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON ) || \
      (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON ) || \
      (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_ON )
#   define DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED              STD_ON
#  else
#   define DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED              STD_OFF
#  endif

#  if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON) && \
      (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_OFF)
#   define DCM_SVC_19_UTI_COPY_LINEAR_DATA_ENABLED                   STD_ON
#  else
#   define DCM_SVC_19_UTI_COPY_LINEAR_DATA_ENABLED                   STD_OFF
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON) || \
      (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_CHAINING_ENABLED                           STD_ON

#   if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
       (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON) || \
       (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON)
#    define DCM_SVC_19_UTI_CHAIN_END_ENABLED                         STD_ON
#   else
#    define DCM_SVC_19_UTI_CHAIN_END_ENABLED                         STD_OFF
#   endif
#  else
#   define DCM_SVC_19_UTI_CHAINING_ENABLED                           STD_OFF
#   define DCM_SVC_19_UTI_CHAIN_END_ENABLED                          STD_OFF
#  endif

#  if ((DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON) || \
      (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)) && \
      (DCM_DEMIF_421_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_ABSTRACT_RECORD_ITER_ENABLED               STD_ON
#  else
#   define DCM_SVC_19_UTI_ABSTRACT_RECORD_ITER_ENABLED               STD_OFF
#  endif

/*! List of DEM filter configurations */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01                      ((Dcm_Svc19UtiDemSetFilterClassType)0u) /*!< For SF 0x01, 0x02, 0x14, 0x0A */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_02                      ((Dcm_Svc19UtiDemSetFilterClassType)1u) /*!< For SF 0x07, 0x08 */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_03                      ((Dcm_Svc19UtiDemSetFilterClassType)2u) /*!< For SF 0x11, 0x0F */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_04                      ((Dcm_Svc19UtiDemSetFilterClassType)3u) /*!< For SF 0x12, 0x13 */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_05                      ((Dcm_Svc19UtiDemSetFilterClassType)4u) /*!< For SF 0x15, 0x55 */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_06                      ((Dcm_Svc19UtiDemSetFilterClassType)5u) /*!< For SF 0x17 */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_07                      ((Dcm_Svc19UtiDemSetFilterClassType)6u) /*!< For SF 0x42 */

/*! Total number of DEM filter configurations */
#  define DCM_SVC_19_UTI_DEM_SETFILTER_NUM_CLASSES                   (DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_07+1u)

/*! Service 0x19 paged-buffer updater dispatching items IDs */
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_RECORD                 ((Dcm_Svc19UtiSubUpdaterType)0u)
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY               ((Dcm_Svc19UtiSubUpdaterType)1u)
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_FDC                    ((Dcm_Svc19UtiSubUpdaterType)2u)
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY_WWHOBD        ((Dcm_Svc19UtiSubUpdaterType)3u)
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_DTC                    ((Dcm_Svc19UtiSubUpdaterType)4u)
#  define DCM_SVC_19_UTI_UPDATE_GET_EXT_RECORD                       ((Dcm_Svc19UtiSubUpdaterType)5u)
#  define DCM_SVC_19_UTI_UPDATE_SELECT_FF_RECORD                     ((Dcm_Svc19UtiSubUpdaterType)6u)
#  define DCM_SVC_19_UTI_UPDATE_GET_FF_RECORD                        ((Dcm_Svc19UtiSubUpdaterType)7u)
#  define DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_EXT_RECORD             ((Dcm_Svc19UtiSubUpdaterType)8u)

/*! API Dcm_Svc19UtiReportDtcByStatusMaskData DTC status mask usage parametrization */
#  define DCM_SVC_19_UTI_USE_MASK_REQ                                ((Dcm_Svc19UtiDemSetFilterMaskUsageType)0u) /*!< Extract and use the mask from request data */
#  define DCM_SVC_19_UTI_USE_MASK_ALL                                ((Dcm_Svc19UtiDemSetFilterMaskUsageType)1u) /*!< Do report all supported DTCs */

/*! DTC extended record number wildcard types */
#  define DCM_SVC_19_UTI_RECNUM_ALL                                  ((uint8)0xFFu) /*!< All supported extended data records */
#  define DCM_SVC_19_UTI_RECNUM_OBD_ALL                              ((uint8)0xFEu) /*!< Report only OBD related extended data records */
/*! DTC (extended/freeze frame) record list end marker (0xFF is not a valid single record ID) */
#  define DCM_SVC_19_UTI_RECLIST_END                                 ((uint8)0xFFu)

/*! Abstract DTC record iterator result values */
#  define DCM_SVC_19_UTI_REC_ITER_RSLT_OK                            ((Dcm_Svc19UtiRecordIterResultType)0) /*!< Next DTC record found and can be used */
#  define DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS               ((Dcm_Svc19UtiRecordIterResultType)1) /*!< Iteration ends here */

/*! Additional DTCOriginType */
#  define DCM_SVC_19_UTI_DEM_DTC_ORIGIN_USER_MEMORY                  ((uint8)0xFFu) /*!< User defined memory DTCOrigin from request */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#  if (DCM_UC_SVC_19_PATCH_SUPPORT_ENABLED == STD_ON)
#   define Dcm_Svc19UtiPatchDtcStatusSetForFilter(dtcStatusMaskPtr)  { if((*(dtcStatusMaskPtr) & 0x0Cu) == 0x0Cu) {*(dtcStatusMaskPtr) |= 0x20u; }  } /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
/*! Per default - patching of DTC status masks is not required */
#   define Dcm_Svc19UtiPatchDtcStatusSetForFilter(dtcStatusMaskPtr)  /* not used */
#  endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#  if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON)
/*! Service 0x19 paged-buffer updater dispatching item ID type */
typedef uint8 Dcm_Svc19UtiSubUpdaterType;
#  endif

#  if (DCM_SVC_19_UTI_SETDTCFILTER_ENABLED == STD_ON)
/*! DEM filter configuration selector type */
typedef uint8 Dcm_Svc19UtiDemSetFilterClassType;
#  endif
#  if (DCM_SVC_19_UTI_GETDTCSTATUSAVAILABILITYMASK_ENABLED == STD_ON)
/*! DEM DTC status availability Mask pointer type */
typedef uint8 Dcm_Svc19UtiDemDtcStatusAvailMaskType;
typedef P2VAR(Dcm_Svc19UtiDemDtcStatusAvailMaskType, TYPEDEF, AUTOMATIC) Dcm_Svc19UtiDemDtcStatusAvailMaskPtrType;
#  endif
#  if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON)
/*! Report DTC by status mask engine mask usage type */
typedef uint8 Dcm_Svc19UtiDemSetFilterMaskUsageType;
#  endif
#  if (DCM_SVC_19_UTI_ABSTRACT_RECORD_ITER_ENABLED == STD_ON)
/*! Abstract DTC record iterator result type */
typedef uint8 Dcm_Svc19UtiRecordIterResultType;
#  endif
#  if (DCM_SVC_19_UTI_SETDTCFILTER_INFO_CLASS_ENABLED == STD_ON)
/*! Descriptor of a DEM filter configuration */
struct DCM_SVC19UTIDEMDTCFILTERINFOTYPE_TAG
{
#   if (DCM_DEMIF_421_ENABLED == STD_ON)
  Dem_DTCKindType                Kind;             /*!< DTC kind */
#   endif
  Dem_DTCOriginType              Origin;           /*!< DTC origin */
  Dcm_DemIfFilterWithSeverityType  ConsiderSeverity; /*!< DTC severity */
};
typedef struct DCM_SVC19UTIDEMDTCFILTERINFOTYPE_TAG Dcm_Svc19UtiDemDtcFilterInfoType;
#  endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectDTC()
 *********************************************************************************************************************/
/*! \brief          Wrapper function for Dem_SelectDTC API.
 *  \details        Performs the DTC selection using the respective DEM API.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Selection API never fails
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectDTC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_SETDTCFILTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetDTCFilter()
 *********************************************************************************************************************/
/*! \brief          Wrapper function for Dem_SetDTCFilter API.
 *  \details        Sets the DTC filter using the respective DEM API.
 *  \param[out]     ErrorCode               The NRC
 *  \param[in]      DsfClass                DEM SetFilter class type
 *  \param[in]      DtcStatusMask           DTC status mask
 *  \param[in]      DtcSeverityMask         DTC severity mask
 *  \param[in]      FilterForFDC            Filter for fault detection counter
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Filter successully set
 *  \return         DCM_E_NOT_OK            Setting filter failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetDTCFilter(
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19UtiDemSetFilterClassType DsfClass,
  uint8 DtcStatusMask,
  Dem_DTCSeverityType DtcSeverityMask,
  Dcm_DemIfFilterForFDCType FilterForFDC,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETDTCSTATUSAVAILABILITYMASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask()
 *********************************************************************************************************************/
/*! \brief          Wrapper function for Dem_GetDTCStatusAvailabilityMask API.
 *  \details        Returns the supported DTC status availability mask using the respective DEM API.
 *  \param[out]     pDtcStatusAvailMask     Pointer to the DTCStatusMask
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK        Filter successully set
 *  \return         DCM_E_NOT_OK    Setting filter failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(
  Dcm_Svc19UtiDemDtcStatusAvailMaskPtrType pDtcStatusAvailMask,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiStartReporting()
 *********************************************************************************************************************/
/*! \brief          Dispatches the current update job to the corresponding sub-updater function.
 *  \details        This is shared utility function of the Repeater and PagedBuffer.
 *  \param[in]      pContext        Pointer to the context
 *  \param[in]      pMsgContext     Pointer to current message context
 *  \param[in]      subUpdater      The reporting engine to be invoked
 *  \param[in]      reportSize      Calculated report size (used only for paged buffer)
 *  \param[in,out]  pRepContext     Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Let the page-buffer do the job
 *  \return         DCM_E_OK                Send response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiStartReporting(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  uint8 subUpdater,
  Dcm_MsgLenType reportSize,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCopyData()
 *********************************************************************************************************************/
/*! \brief          Dispatches the current update job to the corresponding sub-updater function.
 *  \details        This is shared utility function of the Repeater and PagedBuffer.
 *  \param[in]      pContext                    Pointer to the context
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_NOT_OK                Error occured
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCopyData(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_COPY_LINEAR_DATA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCopyLinearData()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        Utility to copy linear data.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_PENDING_LIMITER   Runtime limit reached, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCopyLinearData(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemDisableRecordUpdate()
 *********************************************************************************************************************/
/*! \brief          Disables the update of records of a particular DTC.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemDisableRecordUpdate(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetStatusOfDtc()
 *********************************************************************************************************************/
/*! \brief          Reads the DTC status bits.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetStatusOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSizeOfFFRecord()
 *********************************************************************************************************************/
/*! \brief          Calculates the size of one or multiple freeze-frame data record(s).
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Let the page-buffer do the job
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSizeOfFFRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SELECT_EDR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectExtRecord()
 *********************************************************************************************************************/
/*! \brief          Selects the extended data record(s).
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Let the page-buffer do the job
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectExtRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSizeOfExtRecord()
 *********************************************************************************************************************/
/*! \brief          Calculates the size of one or multiple extended data record(s).
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Let the page-buffer do the job
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSizeOfExtRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETNUMFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNumFltrdDtc()
 *********************************************************************************************************************/
/*! \brief          Retrieves the number of filtered DTCs.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNumFltrdDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*! \brief          Initiates reading filtered DTCs severity.
 *  \details        Depending of enabled/disabled paged buffer, starts corresponding reporting process.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER       Stop polling
 *  \return         DCM_E_LOOP                Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSeverityOfDtc()
 *********************************************************************************************************************/
/*! \brief          Reads the next filtered DTCs severity.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSeverityOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetFuncUnitOfDtc()
 *********************************************************************************************************************/
/*! \brief          Reads the functional unit of the requested DTC.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetFuncUnitOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );

#  endif

#  if (DCM_DEMIF_SELECT_FFR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectFFData()
 *********************************************************************************************************************/
/*! \brief          Selects a freeze-frame data record.
 *  \details        -
 *  \param[in]      pContext              Pointer to the context
 *  \param[in,out]  pRepContext           Current repeater proxy context
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_LOOP            Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectFFData(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdFDC()
 *********************************************************************************************************************/
/*! \brief          Initiates reading filtered DTCs FDC.
 *  \details        Depending of enabled/disabled paged buffer, starts corresponding reporting process.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER       Stop polling
 *  \return         DCM_E_LOOP                Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdFDC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter()
 *********************************************************************************************************************/
/*! \brief         Initiates reading of DTCs filtered by ReadinessGroupIdentifier.
 *  \details        -
 *  \param[in]      readinessGroupNumber    readiness group identifier
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK       Setting the filter was successfully
 *  \return         DCM_E_NOT_OK   Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter(
  Dem_EventOBDReadinessGroupType readinessGroupNumber,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
#  endif

#  if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetExtDataRecordFilter()
 *********************************************************************************************************************/
/*! \brief          Initiates reading of filtered extended data records.
 *  \details        -
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK       Setting the filter was successfully
 *  \return         DCM_E_NOT_OK   Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetExtDataRecordFilter(
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
#  endif

#  if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiWwhObdDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*! \brief          Initiates reading filtered DTCs status mask.
 *  \details        Depending of enabled/disabled paged buffer, starts corresponding reporting process.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Stop polling
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiWwhObdDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdDtc()
 *********************************************************************************************************************/
/*! \brief          Initiates reading filtered DTCs status mask.
 *  \details        Depending of enabled/disabled paged buffer, starts corresponding reporting process.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER       All data copied
 *  \return         DCM_E_LOOP                Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiPutNumFilteredDtc()
 *********************************************************************************************************************/
/*! \brief          Write caclucalted by DEM number of filtered DTCs into the response buffer.
 *  \details        -
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                This value is always returned
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiPutNumFilteredDtc(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber()
 *********************************************************************************************************************/
/*! \brief          Initiates reading of DTCs filtered by ExtendedDataRecordNumber.
 *  \details        -
 *  \param[in]      ExtendedDataRecordNumber    The number of the extended data record.
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK       Setting the filter was successfully
 *  \return         DCM_E_NOT_OK   Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber(
  uint8 ExtendedDataRecordNumber,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportNumDtcByStatusMaskData()
 *********************************************************************************************************************/
/*! \brief          Compose response for any service 0x19 sub-function reporting number of DTC by status mask.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      dsfClass                DEM SetFilter class type
 *  \param[in]      dtcSeverity             If severity specific search - define the severity level
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportNumDtcByStatusMaskData(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19UtiDemSetFilterClassType dsfClass,
  Dem_DTCSeverityType dtcSeverity,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportDtcByStatusMaskData()
 *********************************************************************************************************************/
/*! \brief          Compose response for any service 0x19 sub-function reporting DTC and status mask.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      dsfClass                DEM SetFilter class type
 *  \param[in]      maskUsage               Specifies how to treat the requested status mask (if any)
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportDtcByStatusMaskData(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19UtiDemSetFilterClassType dsfClass,
  Dcm_Svc19UtiDemSetFilterMaskUsageType maskUsage,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportExtDataRecordByDtcNum()
 *********************************************************************************************************************/
/*! \brief          Compose response for any service 0x19 sub-function reporting DTC and its extended data record(s).
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      dtcOrigin               The DTC origin to process
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportExtDataRecordByDtcNum(
  Dcm_ContextPtrType pContext,
  Dem_DTCOriginType dtcOrigin,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_CHAINING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiChainStart()
 *********************************************************************************************************************/
/*! \brief          Initiates a service processing chain of activities.
 *  \details        -
 *  \param[in]      pContext      Pointer to the context
 *  \param[in]      funcChain     The function chain
 *  \param[in,out]  pRepContext   Current repeater proxy context
 *  \return         DCM_E_LOOP    Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiChainStart(
  Dcm_ContextPtrType pContext,
  P2CONST(Dcm_RepeaterProgressType, AUTOMATIC, DCM_CONST) funcChain,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_CHAINING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiChainNext()
 *********************************************************************************************************************/
/*! \brief          Switches to the next processing activity in the chain specified by Dcm_Svc19UtiChainStart.
 *  \details        -
 *  \param[in]      pContext      Pointer to the context
 *  \param[in,out]  pRepContext   Current repeater proxy context
 *  \return         DCM_E_LOOP    Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiChainNext(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportNumDtcByOccurenceTime()
 *********************************************************************************************************************/
/*! \brief          Compose response for any service 0x19 sub-function reporting number DTC by occurence time.
 *  \details        -
 *  \param[in]     pContext             Pointer to the context
 *  \param[in]      dtcReqType              The DTC request type
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportNumDtcByOccurenceTime(
  Dcm_ContextPtrType pContext,
  Dem_DTCRequestType dtcReqType,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportDtcFFRByDtc()
 *********************************************************************************************************************/
/*! \brief          Compose response for any service 0x19 sub-function reporting DTC and its freze-frame data record(s).
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      dtcOrigin      The DTC origin to process
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_LOOP     Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportDtcFFRByDtc(
  Dcm_ContextPtrType pContext,
  Dem_DTCOriginType dtcOrigin,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CONST_8
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x42 */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_42DemChain[2];
#  endif

#  if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x14 */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_14DemChain[2];
#  endif

#  if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x08 */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_08DemChain[2];
#  endif

#  if (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x09 */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_09DemChain[];                                                                       /* PRQA S 3684 */ /* MD_Dcm_UnknownSize_3684 */
#  endif

#  if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
      (DCM_SVC_19_05_SUPPORT_ENABLED             == STD_ON)
/*! List of actions for acquiring DTC status and reporting it together with the DTC itself. This is also a termination activity chain triggering the final positive response */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainGetDtcStatusEnd[];                                                             /* PRQA S 3684 */ /* MD_Dcm_UnknownSize_3684 */
#  endif

#  if (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON)
/*! DEM API "GetOBDFreezeFrameData" result to NRC map */
extern CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetOBDFreezeFrameData[1];
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
      (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of a SF reporting DTCs filtered by status mask */
extern CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainReadDtcByStatus[2];
#  endif

#  if (DCM_SVC_19_UTI_GETDTCSAM_ENABLED == STD_ON) || \
      (DCM_SVC_19_UTI_GETDTCSEVAM_ENABLED == STD_ON)
/*! DEM API with Std_ReturnType result to NRC map */
extern CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapStdReturnType[3];
#  endif
#  define DCM_STOP_SEC_CONST_8
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC19UTI_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc19Uti.h
 *********************************************************************************************************************/

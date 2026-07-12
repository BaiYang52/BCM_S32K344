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
/**        \file  Dcm_Svc19Uti.c
 *         \unit  Svc19Uti
 *        \brief  Contains the implementation of Service 0x19 Utilities unit.
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
#define DCM_SVC19UTI_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc19Uti.h"
#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_DEMIF_421_ENABLED == STD_ON)
/*! Vector DCM to Vector DEM Interface check */
#  if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#   if ( defined(DCM_SVC_19_NUM_EXTDATA_RECID) && !defined (DCM_SVC_19_LST_EXTDATA_RECID)) || \
       (!defined(DCM_SVC_19_NUM_EXTDATA_RECID) &&  defined (DCM_SVC_19_LST_EXTDATA_RECID))
#    error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#   else
#    if (defined(DCM_SVC_19_NUM_EXTDATA_RECID) && defined (DCM_SVC_19_LST_EXTDATA_RECID))
/* the DCM knows the extended data records of the DEM */
#    else
#     if ( defined(DEM_DCM_NUM_EXTDATA_RECID) && !defined (DEM_DCM_LST_EXTDATA_RECID)) || \
         (!defined(DEM_DCM_NUM_EXTDATA_RECID) &&  defined (DEM_DCM_LST_EXTDATA_RECID))
#      error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#     else
#      if (defined(DEM_DCM_NUM_EXTDATA_RECID) && defined (DEM_DCM_LST_EXTDATA_RECID))
/* use the information provided by DEM */
#       define DCM_SVC_19_NUM_EXTDATA_RECID                          DEM_DCM_NUM_EXTDATA_RECID
#       define DCM_SVC_19_LST_EXTDATA_RECID                          DEM_DCM_LST_EXTDATA_RECID
#      else
#       error "Missing information for the supported DTC Extended Data Records! See DCM TechRef!"
#      endif
#     endif
#    endif
#   endif
/*! Actual complete extended data record list sizes */
#   define DCM_SVC_19_UTI_EXTDATA_RECID_SIZE                         (DCM_SVC_19_NUM_EXTDATA_RECID     + 1u)
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#   if ( defined(DCM_SVC_19_NUM_OBD_EXTDATA_RECID) && !defined (DCM_SVC_19_LST_OBD_EXTDATA_RECID)) || \
       (!defined(DCM_SVC_19_NUM_OBD_EXTDATA_RECID) &&  defined (DCM_SVC_19_LST_OBD_EXTDATA_RECID))
#    error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#   else
#    if (defined(DCM_SVC_19_NUM_OBD_EXTDATA_RECID) && defined (DCM_SVC_19_LST_OBD_EXTDATA_RECID))
/* the DCM knows the OBD extended data records of the DEM */
#    else
#     if ( defined(DEM_DCM_NUM_OBD_EXTDATA_RECID) && !defined (DEM_DCM_LST_OBD_EXTDATA_RECID)) || \
         (!defined(DEM_DCM_NUM_OBD_EXTDATA_RECID) &&  defined (DEM_DCM_LST_OBD_EXTDATA_RECID))
#      error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#     else
#      if ( defined(DEM_DCM_NUM_OBD_EXTDATA_RECID) && defined (DEM_DCM_LST_OBD_EXTDATA_RECID))
/* use the information provided by DEM */
#       define DCM_SVC_19_NUM_OBD_EXTDATA_RECID                      DEM_DCM_NUM_OBD_EXTDATA_RECID
#       define DCM_SVC_19_LST_OBD_EXTDATA_RECID                      DEM_DCM_LST_OBD_EXTDATA_RECID
#      else
#       define DCM_SVC_19_NUM_OBD_EXTDATA_RECID                      0u
#       define DCM_SVC_19_LST_OBD_EXTDATA_RECID                      /* empty */
#      endif
#     endif
#    endif
#   endif
/*! Actual OBD extended data record list sizes */
#   define DCM_SVC_19_UTI_OBD_EXTDATA_RECID_SIZE                     (DCM_SVC_19_NUM_OBD_EXTDATA_RECID + 1u)

#   if (DCM_SVC_19_NUM_OBD_EXTDATA_RECID > 0u)
#    define DCM_SVC_19_UTI_OBD_EXT_RECORD_ENABLED                    STD_ON
#   endif
#  endif

#  if defined(DCM_SVC_19_UTI_OBD_EXT_RECORD_ENABLED)
/* already set */
#  else
#   define DCM_SVC_19_UTI_OBD_EXT_RECORD_ENABLED                     STD_OFF
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
#   if ( defined(DCM_SVC_19_NUM_FRZFRAME_RECID) && !defined (DCM_SVC_19_LST_FRZFRAME_RECID)) || \
       (!defined(DCM_SVC_19_NUM_FRZFRAME_RECID) &&  defined (DCM_SVC_19_LST_FRZFRAME_RECID))
#    error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#   else
#    if (defined(DCM_SVC_19_NUM_FRZFRAME_RECID) && defined (DCM_SVC_19_LST_FRZFRAME_RECID))
/* the DCM knows the OBD extended data records of the DEM */
#    else
#     if ( defined(DEM_DCM_NUM_FRZFRAME_RECID) && !defined (DEM_DCM_LST_FRZFRAME_RECID)) || \
         (!defined(DEM_DCM_NUM_FRZFRAME_RECID) &&  defined (DEM_DCM_LST_FRZFRAME_RECID))
#      error "Inconsistent setup! Check your user configuration file upon DCM TechRef!"
#     else
#      if ( defined(DEM_DCM_NUM_FRZFRAME_RECID) && defined (DEM_DCM_LST_FRZFRAME_RECID))
/* use the information provided by DEM */
#       define DCM_SVC_19_NUM_FRZFRAME_RECID                         DEM_DCM_NUM_FRZFRAME_RECID
#       define DCM_SVC_19_LST_FRZFRAME_RECID                         DEM_DCM_LST_FRZFRAME_RECID
#      else
#       error "Missing information for the supported DTC Freeze Frame Records! See DCM TechRef!"
#      endif
#     endif
#    endif
#   endif
/*! Actual complete freeze frame data record list sizes */
#   define DCM_SVC_19_UTI_FRZFRAME_RECID_SIZE                        (DCM_SVC_19_NUM_FRZFRAME_RECID    + 1u)
#  endif
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_19_UTI_ABSTRACT_RECORD_ITER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Sv19UtiRecordIterNext()
 *********************************************************************************************************************/
/*! \brief          Returns the next DTC record or end of iteration result.
 *  \details        -
 *  \param[in,out]  pRepContext     Current repeater proxy context
 *  \return         DCM_SVC_19_UTI_REC_ITER_RSLT_OK                Next record has been found
 *  \return         DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS   End of iteration (record ID shall not be evaluated from now on)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            A non-empty list of records has to be passed to this utility
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc19UtiRecordIterResultType, DCM_CODE) Dcm_Svc19UtiRecordIterNext(
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdExtRecord()
 *********************************************************************************************************************/
/*! \brief          Read the next filtered extended data record.
 *  \details        -
 *  \param[in,out]  pDataContext          Pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \param[in,out]  pRepContext           Current repeater proxy context
 *  \return         DCM_E_OK              All data copied
 *  \return         DCM_E_PENDING         Retry later
 *  \return         DCM_E_BUFFERTOOLOW    Retry later, because of no available space any more
 *  \return         DCM_E_NOT_OK          Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdExtRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdRecord()
 *********************************************************************************************************************/
/*! \brief          Read the next filtered DTC and its record number.
 *  \details        -
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \return         DCM_E_NOT_OK                Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        Calls the GetNextFilteredDTCAndSeverity function.
 *  \param[in]      pContext                    Pointer to the context
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdFDC()
 *********************************************************************************************************************/
/*! \brief          Read the next filtered DTC and its FDC.
 *  \details        -
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \return         DCM_E_NOT_OK                Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdFDC(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdDtc()
 *********************************************************************************************************************/
/*! \brief          Read the next filtered DTC and its status mask.
 *  \details        -
 *  \param[in]      pContext                    Pointer to the context
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \return         DCM_E_NOT_OK                Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdDtc(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiWwhObdCpyDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of readDtcInfo.
 *  \details        Calls the GetNextFilteredDTCAndSeverity function.
 *  \param[in]      pContext                    Pointer to the context
 *  \param[in,out]  pDataContext                Pointer to the data context
 *  \param[out]     ErrorCode                   The NRC
 *  \param[in,out]  pRepContext                 Current repeater proxy context
 *  \return         DCM_E_OK                    All data copied
 *  \return         DCM_E_PENDING               Retry later
 *  \return         DCM_E_PENDING_LIMITER       Runtime limit reached, retry later
 *  \return         DCM_E_BUFFERTOOLOW          Retry later, because of no available space any more
 *  \return         DCM_E_DATA_READY_PADDING    Force padding of remaining bytes
 *  \return         DCM_E_NOT_OK                Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiWwhObdCpyDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetExtRecord()
 *********************************************************************************************************************/
/*! \brief          Read the next DTCs extended data record.
 *  \details        -
 *  \param[in,out]  pDataContext          Pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \param[in,out]  pRepContext           Current repeater proxy context
 *  \return         DCM_E_OK              All data copied
 *  \return         DCM_E_PENDING         Retry later
 *  \return         DCM_E_BUFFERTOOLOW    Retry later, because of no available space any more
 *  \return         DCM_E_NOT_OK          Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetExtRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetFFRecord()
 *********************************************************************************************************************/
/*! \brief          Read the next DTCs freeze-frame data record.
 *  \details        -
 *  \param[in,out]  pDataContext          Pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \param[in,out]  pRepContext           Current repeater proxy context
 *  \return         DCM_E_OK              All data copied
 *  \return         DCM_E_PENDING         Retry later
 *  \return         DCM_E_BUFFERTOOLOW    Retry later, because of no available space any more
 *  \return         DCM_E_NOT_OK          Error occured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetFFRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON) || \
     (DCM_SVC_19_05_SUPPORT_ENABLED             == STD_ON)
/*! List of actions for acquiring DTC status and reporting it together with the DTC itself. This is also a termination activity chain triggering the final positive response */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainGetDtcStatusEnd[] =                                                                   /* PRQA S 3218, 1514 */ /* MD_Dcm_CodingRule_3218, MD_Dcm_ObjectOnlyAccessedOnce */
{
#  if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC,
#  endif
  DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC
  , DCM_SVC19_PROGRESS_CHAIN_END
};
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
/*! List of actions for freeze-frame data reporting prolog */
DCM_LOCAL CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainReadFFRByDtc[] =                                                            /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC,
#  endif
  DCM_SVC19_PROGRESS_DEMCHAIN_DISABLERECORDUPDATE
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC
#  if (DCM_DEMIF_SELECT_FFR_ENABLED == STD_ON)
  , DCM_SVC19_PROGRESS_DEMCHAIN_SELECTFFREC
#  endif
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFFFREC
};
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
/*! List of actions for extended data reporting prolog */
DCM_LOCAL CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainReadExtRecords[] =                                                          /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC,
#  endif
  DCM_SVC19_PROGRESS_DEMCHAIN_DISABLERECORDUPDATE
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC
#  if (DCM_DEMIF_SELECT_EDR_ENABLED == STD_ON)
  , DCM_SVC19_PROGRESS_DEMCHAIN_SELECTEXTDATAREC
#  endif
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFEXTDATAREC
};
# endif

# if (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x09 */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_09DemChain[] =                                                                             /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
{
#  if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
  DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC,
#  endif
  DCM_SVC19_PROGRESS_DEMCHAIN_GETSEVERITYOFDTC
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETFUNCUNITOFDTC
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC
  , DCM_SVC19_PROGRESS_CHAIN_END
};
# endif

# if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
/*! List of actions for complete processing of a SF reporting the number of filtered DTCs */
DCM_LOCAL CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainDtcCount[2] =                                                               /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC
  , DCM_SVC19_PROGRESS_DEMCHAIN_PUTNUMFLTRDDTC
};
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
     (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of a SF reporting DTCs filtered by status mask */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19UtiDemChainReadDtcByStatus[2] =                                                                  /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
{
  DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC /* let this API to be called also in linear-buffer case in order to check for the last time if any DEM state error has occurred */
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDDTC
};
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x08 */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_08DemChain[2] =                                                                            /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
{
  DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDSEVERITY
};
# endif

# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x14 */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_14DemChain[2] =                                                                            /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
{
  DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC
  , DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDFDC
};
# endif

# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/*! List of actions for complete processing of SF 0x42 */
CONST(Dcm_RepeaterProgressType, DCM_CONST) Dcm_Svc19Uti19_42DemChain[2] =                                                                            /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC /* let this API to be called also in linear-buffer case in order to check for the last time if any DEM state error has occurred */
  , DCM_SVC19_PROGRESS_DEMCHAIN_WWHOBD_GETNEXTFLTRDSEVERITY
};
# endif

# if (DCM_DEMIF_421_ENABLED == STD_ON)
#  if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
/*! List of all (UDS and OBD) extended data records DEM supports */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiExtDataRecords[DCM_SVC_19_UTI_EXTDATA_RECID_SIZE] =                                                    /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#   if (DCM_SVC_19_NUM_EXTDATA_RECID > 0u)
  DCM_SVC_19_LST_EXTDATA_RECID,
#   endif
  DCM_SVC_19_UTI_RECLIST_END /* end marker */
};
#  endif

#  if (DCM_SVC_19_UTI_OBD_EXT_RECORD_ENABLED == STD_ON)
/*! List of all OBD extended data records DEM supports */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiExtDataRecordsObd[DCM_SVC_19_UTI_OBD_EXTDATA_RECID_SIZE] =                                             /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_SVC_19_LST_OBD_EXTDATA_RECID,
  DCM_SVC_19_UTI_RECLIST_END /* end marker */
};
#  endif

#  if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
/*! List of all freeze frame data records DEM supports */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiFFDataRecords[DCM_SVC_19_UTI_FRZFRAME_RECID_SIZE] =                                                    /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#   if (DCM_SVC_19_NUM_FRZFRAME_RECID > 0u)
  DCM_SVC_19_LST_FRZFRAME_RECID,
#   endif
  DCM_SVC_19_UTI_RECLIST_END /* end marker */
};
#  endif
# endif

# if (DCM_SVC_19_UTI_GETDTCSAM_ENABLED == STD_ON) || \
     (DCM_SVC_19_UTI_GETDTCSEVAM_ENABLED == STD_ON)
/*! DEM API with Std_ReturnType result to NRC map */
CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapStdReturnType[3] =                                                                                         /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
{
  E_NOT_OK, DCM_E_CONDITIONSNOTCORRECT,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETDTCOCCURTIME_ENABLED == STD_ON)
/*! DEM API "GetDTCByOccurrenceTime" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetDTCByOccurrenceTime[] =                                                                       /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if defined (DCM_DEMIF_RET_OCCURR_N_OK)
  DCM_DEMIF_RET_OCCURR_N_OK, DCM_E_GENERALREJECT,
#  endif
#  if defined (DCM_DEMIF_RET_OCCURR_NO_SUCH_ELEMENT)
  DCM_DEMIF_RET_OCCURR_NO_SUCH_ELEMENT, DCM_E_POSITIVERESPONSE,
#  endif
#  if defined (DCM_DEMIF_RET_OCCURR_NOT_AVAILABLE)
  DCM_DEMIF_RET_OCCURR_NOT_AVAILABLE, DCM_E_POSITIVERESPONSE,
#  endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON)
/*! DEM API "GetOBDFreezeFrameData" result to NRC map */
CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetOBDFreezeFrameData[1] =                                                                                 /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
/*! DEM API "DisableDTCRecordUpdate", "EnableDTCRecordUpdate" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapAnyAbleRecordUpdate[] =                                                                          /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTCORIGIN, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTC, DCM_E_REQUESTOUTOFRANGE,
#  if defined (DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_CONDITION)
  /* Let it be handled in default case (PANIC NRC + DET error) since this means wrong DCM implementation */
  /* DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_CONDITION, DCM_E_PANIC, */
#  endif
#  if defined (DCM_DEMIF_RET_DISABLE_DTC_REC_UP_BUSY)
  DCM_DEMIF_RET_DISABLE_DTC_REC_UP_BUSY, DCM_E_CONDITIONSNOTCORRECT,
#  endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED == STD_ON)
/*! DEM API "GetStatusOfDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetStatusOfDTC[] =                                                                               /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DEMIF_RET_STATUS_WRONG_DTC, DCM_E_REQUESTOUTOFRANGE,
#  if defined (DCM_DEMIF_RET_STATUS_FAILED)
  DCM_DEMIF_RET_STATUS_FAILED, DCM_E_CONDITIONSNOTCORRECT,
#  endif
  DCM_DEMIF_RET_STATUS_WRONG_DTCORIGIN, DCM_E_CONDITIONSNOTCORRECT,
#  if defined (DCM_DEMIF_RET_STATUS_BUSY)
  DCM_DEMIF_RET_STATUS_BUSY, DCM_E_CONDITIONSNOTCORRECT,
#  endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/*! DEM API "GetSeverityOfDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetSeverityOfDTC[] =                                                                             /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTC, DCM_E_REQUESTOUTOFRANGE,
#  if defined (DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTCORIGIN)
  DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTCORIGIN, DCM_E_REQUESTOUTOFRANGE,
#  endif
#  if defined (DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_BUSY)
  DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_BUSY, DCM_E_CONDITIONSNOTCORRECT,
#  endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/*! DEM API "GetFunctionalUnitOfDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetFunctionalUnitOfDTC[] =                                                                       /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if defined (DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTCORIGIN)
  DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTCORIGIN, DCM_E_REQUESTOUTOFRANGE,
#  endif
  DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTC, DCM_E_REQUESTOUTOFRANGE,
#  if defined (DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_BUSY)
  DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_BUSY, DCM_E_CONDITIONSNOTCORRECT,
#  endif
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED == STD_ON)
/*! DEM API "GetSizeOfExtendedDataRecordByDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetSizeOfExtendedDataRecordByDTC[] =                                                             /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if defined (DCM_DEMIF_RET_SIZEOFEDR_N_OK)
  /* Let it be handled in default case (PANIC NRC + DET error) since this means wrong DCM implementation */
  /* DCM_DEMIF_RET_SIZEOFEDR_N_OK,     DCM_E_PANIC, */
#  endif
  DCM_DEMIF_RET_SIZEOFEDR_W_DTC, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_SIZEOFEDR_W_RNUM, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_SIZEOFEDR_W_ORIGIN, DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED == STD_ON)
/*! DEM API "GetSizeOfFreezeFrameByDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetSizeOfFreezeFrameByDTC[] =                                                                    /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
#  if defined (DCM_DEMIF_RET_SIZEOFFFR_N_OK)
  /* Let it be handled in default case (PANIC NRC + DET error) since this means wrong DCM implementation */
  /* DCM_DEMIF_RET_SIZEOFFFR_N_OK,     DCM_E_PANIC, */
#  endif
  DCM_DEMIF_RET_SIZEOFFFR_W_DTC, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_SIZEOFFFR_W_RNUM, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_SIZEOFFFR_W_ORIGIN, DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON)
/*! DEM API "GetFreezeFrameDataByDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetFreezeFrameDataByDTC[7] =                                                                     /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DEMIF_RET_FFDATABYDTC_W_DTC, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_FFDATABYDTC_W_ORIGIN, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_FFDATABYDTC_W_RNUM, DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif

# if (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON)
/*! DEM API "GetExtendedDataRecordByDTC" result to NRC map */
DCM_LOCAL CONST(uint8, DCM_CONST) Dcm_Svc19UtiNrcMapGetExtendedDataRecordByDTC[7] =                                                                  /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTC, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTCORIGIN, DCM_E_REQUESTOUTOFRANGE,
  DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_NUMBER, DCM_E_REQUESTOUTOFRANGE,
  E_OK /* end marker  - E_OK shall never be used as an error code */
};
# endif
# define DCM_STOP_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CONST_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_19_UTI_SETDTCFILTER_INFO_CLASS_ENABLED == STD_ON)
/*! A set of all required DEM filter configurations */
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
DCM_LOCAL CONST(Dcm_Svc19UtiDemDtcFilterInfoType, DCM_CONST) Dcm_Svc19UtiDemDtcFilterInfo[DCM_SVC_19_UTI_DEM_SETFILTER_NUM_CLASSES] =                /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  { DEM_DTC_KIND_ALL_DTCS, DEM_DTC_ORIGIN_PRIMARY_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_NO }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01: SF 0x01, 0x02, 0x14, 0x0A */
  , { DEM_DTC_KIND_ALL_DTCS, DEM_DTC_ORIGIN_PRIMARY_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_YES } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_02: SF 0x07, 0x08 */
  , { DEM_DTC_KIND_ALL_DTCS, DEM_DTC_ORIGIN_MIRROR_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_NO }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_03: SF 0x11, 0x0F */
  , { DEM_DTC_KIND_EMISSION_REL_DTCS, DEM_DTC_ORIGIN_PRIMARY_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_NO }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_04: SF 0x12, 0x13 */
  , { DEM_DTC_KIND_ALL_DTCS, DEM_DTC_ORIGIN_PERMANENT_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_NO }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_05: SF 0x15, 0x55 */
  , { DEM_DTC_KIND_ALL_DTCS, DEM_DTC_ORIGIN_SECONDARY_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_NO }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_06: SF 0x17 */
  , { DEM_DTC_KIND_EMISSION_REL_DTCS, DEM_DTC_ORIGIN_PRIMARY_MEMORY, DCM_DEMIF_FILTER_WITH_SEVERITY_YES } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_07: SF 0x42 */
};
#  else
DCM_LOCAL CONST(Dcm_Svc19UtiDemDtcFilterInfoType, DCM_CONST) Dcm_Svc19UtiDemDtcFilterInfo[DCM_SVC_19_UTI_DEM_SETFILTER_NUM_CLASSES] =                /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  { DEM_DTC_ORIGIN_PRIMARY_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_01: SF 0x01, 0x02, 0x14, 0x0A */
  , { DEM_DTC_ORIGIN_PRIMARY_MEMORY, TRUE }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_02: SF 0x07, 0x08 */
  , { DEM_DTC_ORIGIN_MIRROR_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_03: SF 0x11, 0x0F */
  , { DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_04: SF 0x12, 0x13 */
  , { DEM_DTC_ORIGIN_PERMANENT_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_05: SF 0x15, 0x55 */
#   if (DCM_DEMIF_431_ENABLED == STD_ON)
  , { DCM_SVC_19_UTI_DEM_DTC_ORIGIN_USER_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_06: SF 0x17 */ /* 0x17: mem selection from request */
#   else
  , { DEM_DTC_ORIGIN_SECONDARY_MEMORY, FALSE } /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_06: SF 0x17 */
#   endif
  , { DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY, TRUE }  /* DCM_SVC_19_UTI_DEM_SETFILTER_CLASS_07: SF 0x42 */
};
#  endif
# endif
# define DCM_STOP_SEC_CONST_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_19_UTI_ABSTRACT_RECORD_ITER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiRecordIterNext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc19UtiRecordIterResultType, DCM_CODE) Dcm_Svc19UtiRecordIterNext(
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_Svc19UtiRecordIterResultType lRecIterResult;

  lRecIterResult = DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS;

  if (pRepContext->RecordNumList != NULL_PTR)
  {
    Dcm_UtiNextItemByPtr(pRepContext->RecordNumList, 1u);                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->RecordNum = pRepContext->RecordNumList[0];                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    if (pRepContext->RecordNum != DCM_SVC_19_UTI_RECLIST_END)
    {
      lRecIterResult = DCM_SVC_19_UTI_REC_ITER_RSLT_OK;
    }
  }

  return lRecIterResult;
}
# endif

# if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdExtRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdExtRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_BUFFERTOOLOW;

  while (pDataContext->AvailLen > 4u)
  {
    Std_ReturnType  lDemResult;
    uint32          lDtc;
    uint8           lDtcStatus;
    uint16          lRecordSize;

    /* Move four byte up to reserve place for the DTCAndStatusRecord */
    Dcm_UtiCommitData(pDataContext, 4u);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

    lRecordSize = (uint16)pDataContext->AvailLen;

    lDemResult = Dem_GetNextFilteredExtendedDataRecord(pRepContext->DemClientId
                                                      ,&lDtc
                                                      ,&lDtcStatus
                                                      ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                      ,&lRecordSize);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Move back to original position to write now the data */
    Dcm_UtiRevertData(pDataContext, 4);                                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (lDemResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, lDtcStatus);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiCommitData(pDataContext, lRecordSize);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(lDemResult, ErrorCode);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  uint8                                   recordNumber;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  lStdResult = DCM_E_BUFFERTOOLOW;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  while (pDataContext->AvailLen >= 4u)                                                                                                               /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    demResult = Dcm_DemIf(GetNextFilteredRecord)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                                 pRepContext->DemClientId,
#  endif
                                                 &lDtc
                                                ,&recordNumber);                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, recordNumber);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  uint8                                   dtcStatus;
  uint8                                   dtcFuncUnit;
  Dem_DTCSeverityType                     dtcSeverity;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  lStdResult = DCM_E_BUFFERTOOLOW;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  while (pDataContext->AvailLen >= 6u)                                                                                                               /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    demResult = Dcm_DemIf(GetNextFilteredDTCAndSeverity)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                                         pRepContext->DemClientId,
#  endif
                                                         &lDtc
                                                        ,&dtcStatus
                                                        ,&dtcSeverity
                                                        ,&dtcFuncUnit);                                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU8(pDataContext, (Dcm_MsgItemType)dtcSeverity);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, dtcFuncUnit);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, dtcStatus);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdFDC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdFDC(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  sint8                                   dtcFDC;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  lStdResult = DCM_E_BUFFERTOOLOW;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  while (pDataContext->AvailLen >= 4u)                                                                                                               /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    demResult = Dcm_DemIf(GetNextFilteredDTCAndFDC)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                                    pRepContext->DemClientId,
#  endif
                                                    &lDtc, &dtcFDC);                                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, (uint8)dtcFDC);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetNxtFltrdDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetNxtFltrdDtc(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  uint8                                   dtcStatus;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  lStdResult = DCM_E_BUFFERTOOLOW;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  while (pDataContext->AvailLen >= 4u)                                                                                                               /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    demResult = Dcm_DemIf(GetNextFilteredDTC)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                              pRepContext->DemClientId,
#  endif
                                              &lDtc, &dtcStatus);                                                                                    /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, dtcStatus);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiWwhObdCpyDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiWwhObdCpyDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint32                                  lDtc;
  uint8                                   dtcStatus;
  uint8                                   dtcFuncUnit;
  Dem_DTCSeverityType                     dtcSeverity;
  Dcm_DemIfReturnGetNextFilteredItemType  demResult;
  Std_ReturnType                          lStdResult;

  lStdResult = DCM_E_BUFFERTOOLOW;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  while (pDataContext->AvailLen >= 5u)                                                                                                               /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    demResult = Dcm_DemIf(GetNextFilteredDTCAndSeverity)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                                         pRepContext->DemClientId,
#  endif
                                                         &lDtc
                                                        ,&dtcStatus
                                                        ,&dtcSeverity
                                                        ,&dtcFuncUnit);                                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
    if (demResult == DCM_DEMIF_RET_FILTERED_OK)
    {
      Dcm_UtiProvideDataAsU8(pDataContext, (Dcm_MsgItemType)dtcSeverity);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU24(pDataContext, lDtc);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideDataAsU8(pDataContext, dtcStatus);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      lStdResult = Dcm_DemIfConvDemRsltGetNextFilteredItem(demResult, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    }

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON)
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetExtRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetExtRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  demResult;
  Std_ReturnType  lStdResult;
  uint16          dcmBufferSize;
  boolean         readNextData = TRUE;

  do
  {
    dcmBufferSize = (uint16)pDataContext->AvailLen;

    demResult = Dcm_DemIf(GetNextExtendedDataRecord)(pRepContext->DemClientId
                                                    ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                    ,&dcmBufferSize);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

    switch (demResult)
    {
    case DCM_DEMIF_RET_GET_EXT_DATA_REC_OK:
      Dcm_UtiCommitData(pDataContext, dcmBufferSize);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_OK;
      break;
    case DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_NUMBER:
      lStdResult = DCM_E_OK;
      readNextData = FALSE;
      break;
    case DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_BUFFERSIZE:
      lStdResult = DCM_E_BUFFERTOOLOW;
      readNextData = FALSE;
      break;
    case DCM_DEMIF_RET_GET_EXT_DATA_REC_PENDING:
      lStdResult = DCM_E_PENDING;/* signal retry */
      readNextData = FALSE;
      break;
    default:
      *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetExtendedDataRecordByDTC);                                           /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      readNextData = FALSE;
      break;
    }
  } while (readNextData == TRUE);

  /* If record data processing finished in this call (with or without success), enable DTC record update immediately */
  if ((lStdResult == DCM_E_OK) || (lStdResult == DCM_E_NOT_OK))
  {
    (void)Dcm_DemIf(EnableDTCRecordUpdate)(pRepContext->DemClientId);
  }

  return lStdResult;
}
#  else
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetExtRecord()
 *********************************************************************************************************************/
/*!
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
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetExtRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Dem_ReturnGetExtendedDataRecordByDTCType demResult;
  Std_ReturnType                           lStdResult;
  uint16                                   dcmBufferSize;
  Dcm_Svc19UtiRecordIterResultType         dcmNextRecordResult;

  do
  {
    /* Move one byte up to reserve place for the record number */
    Dcm_UtiCommitData(pDataContext, 1);                                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

    dcmBufferSize = (uint16)pDataContext->AvailLen;

    demResult = Dcm_DemIf(GetExtendedDataRecordByDTC)(pRepContext->Dtc
                                                     ,pRepContext->Origin
                                                     ,pRepContext->RecordNum
                                                     ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                     ,&dcmBufferSize);                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Move back to original position to write now the data */
    Dcm_UtiRevertData(pDataContext, 1);                                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (demResult == DCM_DEMIF_RET_GET_EXT_DATA_REC_OK)
    {
      if (dcmBufferSize != 0u)/* consider only stored records */
      {
        Dcm_UtiProvideDataAsU8(pDataContext, pRepContext->RecordNum);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_UtiCommitData(pDataContext, dcmBufferSize);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      }

      dcmNextRecordResult = Dcm_Svc19UtiRecordIterNext(pRepContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_OK;
    }
    else if (demResult == DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_NUMBER)
    {
      if (pRepContext->RecordNumList == NULL_PTR)/* single record query */
      {
        dcmNextRecordResult = DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS;
        *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetExtendedDataRecordByDTC);                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
      else
      {
        dcmNextRecordResult = Dcm_Svc19UtiRecordIterNext(pRepContext);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_OK;
      }
    }
    else
    {
      dcmNextRecordResult = DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS;
      switch (demResult)
      {
      case DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_BUFFERSIZE:
        lStdResult = DCM_E_BUFFERTOOLOW;
        break;
      case DCM_DEMIF_RET_GET_EXT_DATA_REC_PENDING:
        lStdResult = DCM_E_PENDING;/* signal retry */
        break;
      default:
        *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetExtendedDataRecordByDTC);                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        break;
      }
    }

    /* If no more records to be reported or current iteration detected an error, stop iteration */
    if (dcmNextRecordResult == DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS)
    {
      break;
    }
  } while (pDataContext->AvailLen != 0u);

  if ( (pDataContext->AvailLen == 0u)
     && (dcmNextRecordResult != DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS))
  {
    lStdResult = DCM_E_BUFFERTOOLOW;
  }

  /* If record data processing finished in this call (with or without success), enable DTC record update immediately */
  if ((lStdResult == DCM_E_OK) || (lStdResult == DCM_E_NOT_OK))
  {
    (void)Dcm_DemIf(EnableDTCRecordUpdate)();
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#  endif
# endif

# if (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON)
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetFFRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetFFRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  demResult;
  Std_ReturnType  lStdResult;
  uint16          dcmBufferSize;
  boolean         readNextData = TRUE;

  do
  {
    dcmBufferSize = (uint16)pDataContext->AvailLen;

    demResult = Dcm_DemIf(GetNextFreezeFrameData)(pRepContext->DemClientId
                                                 ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                 ,&dcmBufferSize);                                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */

    switch (demResult)
    {
    case DCM_DEMIF_RET_FFDATABYDTC_OK:
      if (dcmBufferSize != 0u)/* consider only stored records */
      {
        Dcm_UtiCommitData(pDataContext, dcmBufferSize);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      lStdResult = DCM_E_OK;
      break;
    case DCM_DEMIF_RET_FFDATABYDTC_W_RNUM:
      lStdResult = DCM_E_OK;
      readNextData = FALSE;
      break;
    case DCM_DEMIF_RET_FFDATABYDTC_W_BUFSIZE:
      lStdResult = DCM_E_BUFFERTOOLOW;
      readNextData = FALSE;
      break;
    case DCM_DEMIF_RET_FFDATABYDTC_PENDING:
      lStdResult = DCM_E_PENDING;/* signal retry */
      readNextData = FALSE;
      break;
    default:
      *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetFreezeFrameDataByDTC);                                              /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;/* finish processing */
      readNextData = FALSE;
      break;
    }
  } while (readNextData == TRUE);

  /* If record data processing finished in this call (with or without success), enable DTC record update immediately */
  if ((lStdResult == DCM_E_OK) || (lStdResult == DCM_E_NOT_OK))
  {
    (void)Dcm_DemIf(EnableDTCRecordUpdate)(pRepContext->DemClientId);
  }

  return lStdResult;
}
#  else
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCpyDemGetFFRecord()
 *********************************************************************************************************************/
/*!
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
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCpyDemGetFFRecord(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Dem_ReturnGetFreezeFrameDataByDTCType demResult;
  Std_ReturnType                        lStdResult;
  uint16                                dcmBufferSize;
  Dcm_Svc19UtiRecordIterResultType      dcmNextRecordResult;

  do
  {
    dcmBufferSize = (uint16)pDataContext->AvailLen;

    demResult = Dcm_DemIf(GetFreezeFrameDataByDTC)(pRepContext->Dtc
                         ,pRepContext->Origin
                         ,pRepContext->RecordNum
                         ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
                         ,&dcmBufferSize);                                                                                                           /* SBSW_DCM_COMB_PTR_FORWARD */
    if (demResult == DCM_DEMIF_RET_FFDATABYDTC_OK)
    {
      Dcm_UtiCommitData(pDataContext, dcmBufferSize);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      dcmNextRecordResult = Dcm_Svc19UtiRecordIterNext(pRepContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_OK;
    }
    else if (demResult == DCM_DEMIF_RET_FFDATABYDTC_W_RNUM)
    {
      if (pRepContext->RecordNumList == NULL_PTR)/* single record query */
      {
        dcmNextRecordResult = DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS;
        *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetFreezeFrameDataByDTC);                                            /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
      else
      {
        dcmNextRecordResult = Dcm_Svc19UtiRecordIterNext(pRepContext);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_OK;
      }
    }
    else
    {
      switch (demResult)
      {
      case DCM_DEMIF_RET_FFDATABYDTC_W_BUFSIZE:
        lStdResult = DCM_E_BUFFERTOOLOW;
        break;
      case DCM_DEMIF_RET_FFDATABYDTC_PENDING:
        lStdResult = DCM_E_PENDING;/* signal retry */
        break;
      default:
        *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetFreezeFrameDataByDTC);                                            /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;/* finish processing */
        break;
      }
      /* Finished for this cycle or error occurred */
      dcmNextRecordResult = DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS;
    }

    if (dcmNextRecordResult == DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS)
    {
      break;
    }
  } while (pDataContext->AvailLen != 0u);

  if ( (pDataContext->AvailLen == 0u)
     && (dcmNextRecordResult != DCM_SVC_19_UTI_REC_ITER_RSLT_NO_MORE_RECORDS))
  {
    lStdResult = DCM_E_BUFFERTOOLOW;
  }

  /* If record data processing finished in this call (with or without success), enable DTC record update immediately */
  if ((lStdResult == DCM_E_OK) || (lStdResult == DCM_E_NOT_OK))
  {
    (void)Dcm_DemIf(EnableDTCRecordUpdate)();
  }

  return lStdResult;
}
#  endif
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectDTC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectDTC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DemIf(SelectDTC)(pRepContext->DemClientId
                                   ,pRepContext->Dtc
                                   ,DEM_DTC_FORMAT_UDS
                                   ,pRepContext->Origin);

  switch (lStdResult)
  {
  case E_OK:
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lStdResult, Dcm_DemIfNrcMapSelectDTC);                                                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_SETDTCFILTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetDTCFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetDTCFilter(
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19UtiDemSetFilterClassType DsfClass,
  uint8 DtcStatusMask,
  Dem_DTCSeverityType DtcSeverityMask,
  Dcm_DemIfFilterForFDCType FilterForFDC,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Dem_DTCOriginType lDtcOrigin;
  Dcm_DemIfReturnSetFilterType demResultSetFilter;
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_431_ENABLED == STD_ON)
  if (Dcm_Svc19UtiDemDtcFilterInfo[DsfClass].Origin == DCM_SVC_19_UTI_DEM_DTC_ORIGIN_USER_MEMORY)
  {
    /* Origin has to be extracted of the request */
    lDtcOrigin = pRepContext->Origin;
  }
  else
#  endif
  {
    lDtcOrigin = Dcm_Svc19UtiDemDtcFilterInfo[DsfClass].Origin;
  }

  demResultSetFilter = Dcm_DemIf(SetDTCFilter)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
                                               pRepContext->DemClientId,
#  endif
                                               DtcStatusMask
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
                                              ,Dcm_Svc19UtiDemDtcFilterInfo[DsfClass].Kind
#  endif
                                              ,DEM_DTC_FORMAT_UDS
                                              ,lDtcOrigin
                                              ,Dcm_Svc19UtiDemDtcFilterInfo[DsfClass].ConsiderSeverity
                                              ,DtcSeverityMask
                                              ,FilterForFDC);

  if (demResultSetFilter == DCM_DEMIF_RET_FILTER_ACCEPTED)
  {
    lStdResult = DCM_E_OK;
  }
  else
  {
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResultSetFilter, Dcm_DemIfNrcMapSetDTCFilter);                                                     /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_SVC_19_UTI_GETDTCSTATUSAVAILABILITYMASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(
  Dcm_Svc19UtiDemDtcStatusAvailMaskPtrType pDtcStatusAvailMask,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Gather the configured DEM Status Availability Mask */
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  lStdResult = Dcm_DemIf(GetDTCStatusAvailabilityMask)(pDtcStatusAvailMask);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  else
  lStdResult = Dcm_DemIf(GetDTCStatusAvailabilityMask)(pRepContext->DemClientId
                                                      ,pDtcStatusAvailMask);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif

  if (lStdResult != DCM_E_OK)
  {
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lStdResult, Dcm_Svc19UtiNrcMapStdReturnType);                                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiStartReporting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiStartReporting(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint8 subUpdater,
  Dcm_MsgLenType reportSize,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(reportSize);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->SubUpdater = subUpdater;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */

#  if (DCM_DEMIF_PAGED_BUFFER_ENABLED == STD_ON)
  Dcm_PagedBufferStart(pMsgContext, reportSize);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  return DCM_E_STOP_REPEATER;
#  else
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  Dcm_RepeaterNextStep(pContext, DCM_SVC19_PROGRESS_UTI_COPYLINEARDATA);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  return DCM_E_LOOP;
#  endif
}
# endif

# if (DCM_SVC_19_UTI_REPORT_VIA_SUBUPDATER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCopyData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCopyData(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  switch (pRepContext->SubUpdater)
  {
#  if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_DTC:
    lStdResult = Dcm_Svc19UtiCpyDemGetNxtFltrdDtc(pContext, pDataContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_RECORD:
    lStdResult = Dcm_Svc19UtiCpyDemGetNxtFltrdRecord(pDataContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_FDC:
    lStdResult = Dcm_Svc19UtiCpyDemGetNxtFltrdFDC(pDataContext, ErrorCode, pRepContext);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY:
    lStdResult = Dcm_Svc19UtiCpyDemGetNxtFltrdSeverity(pContext, pDataContext, ErrorCode, pRepContext);                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY_WWHOBD:
    lStdResult = Dcm_Svc19UtiWwhObdCpyDemGetNxtFltrdSeverity(pContext, pDataContext, ErrorCode, pRepContext);                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_EXT_RECORD:
    lStdResult = Dcm_Svc19UtiCpyDemGetExtRecord(pDataContext, ErrorCode, pRepContext);                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_FF_RECORD:
    lStdResult = Dcm_Svc19UtiCpyDemGetFFRecord(pDataContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
#  if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_EXT_RECORD:
    lStdResult = Dcm_Svc19UtiCpyDemGetNxtFltrdExtRecord(pDataContext, ErrorCode, pRepContext);                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

# if (DCM_SVC_19_UTI_COPY_LINEAR_DATA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiCopyLinearData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiCopyLinearData(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType           lStdResult;
  Dcm_DiagDataContextType  lDataContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiInitDataContext(&lDataContext, Dcm_UtiGetResData(pMsgContext), pMsgContext->resMaxDataLen);                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

  lStdResult = Dcm_Svc19UtiCopyData(pContext, &lDataContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Commit copied data */
  Dcm_UtiProvideResData(pMsgContext, lDataContext.Usage);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  if ( (lStdResult != DCM_E_PENDING)
     && (lStdResult != DCM_E_PENDING_LIMITER)
     && (lStdResult != DCM_E_NOT_OK))
  {
    if (lStdResult == DCM_E_BUFFERTOOLOW)
    {
      *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      lStdResult = DCM_E_OK;
    }
  }
  return lStdResult;
}
# endif
# if (DCM_SVC_19_UTI_ANYABLERECORDUPDATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemDisableRecordUpdate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemDisableRecordUpdate(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnDisableDTCRecordUpdateType demResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResult = Dcm_DemIf(DisableDTCRecordUpdate)(pRepContext->Dtc
                                               ,pRepContext->Origin);
#  else
  demResult = Dcm_DemIf(DisableDTCRecordUpdate)(pRepContext->DemClientId);
#  endif
  switch (demResult)
  {
  case DCM_DEMIF_RET_DISABLE_DTC_REC_UP_OK:
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_DISABLE_DTC_REC_UP_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapAnyAbleRecordUpdate);                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_GETSTATUSOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetStatusOfDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetStatusOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnGetStatusOfDTCType demResult;
  uint8 dtcStatusMask;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResult = Dcm_DemIf(GetStatusOfDTC)(pRepContext->Dtc
                                       ,pRepContext->Origin
                                       ,&dtcStatusMask);                                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
  demResult = Dcm_DemIf(GetStatusOfDTC)(pRepContext->DemClientId
                                       ,&dtcStatusMask);                                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif
  switch (demResult)
  {
  case DCM_DEMIF_RET_STATUS_OK:
    Dcm_UtiProvideResDataAsU24(pMsgContext, pRepContext->Dtc);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcStatusMask);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  if defined (DCM_DEMIF_RET_STATUS_NO_SUCH_ELEMENT)
  case DCM_DEMIF_RET_STATUS_NO_SUCH_ELEMENT:
    /* Skip committing data (SWS_Dcm_01409) */
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
  case DCM_DEMIF_RET_STATUS_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetStatusOfDTC);                                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_GETSIZEOFFFRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSizeOfFFRecord()
 *********************************************************************************************************************/
/*!
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSizeOfFFRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnGetSizeOfFFRByDTCType demResult;
  uint16 dataLen;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResult = Dcm_DemIf(GetSizeOfFreezeFrameByDTC)(pRepContext->Dtc
                                                  ,pRepContext->Origin
                                                  ,pRepContext->RecordNum
                                                  ,&dataLen);                                                                                        /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
  demResult = Dcm_DemIf(GetSizeOfFreezeFrameSelection)(pRepContext->DemClientId
                                                      ,&dataLen);                                                                                    /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif
  switch (demResult)
  {
  case DCM_DEMIF_RET_SIZEOFFFR_OK:
    if (dataLen != 0u) /* consider only stored records */
    {
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
      if (pRepContext->RecordNumList != NULL_PTR)
      {
        pRepContext->RecordNum = pRepContext->RecordNumList[0]; /* take first one */                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugAssert((DCM_SVC_19_UTI_RECNUM_ALL != pRepContext->RecordNum), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                               /* COV_DCM_RTM_DEV_DEBUG XF */
      }
#  endif

      lStdResult = Dcm_Svc19UtiStartReporting(pContext
                                            ,pMsgContext
                                            ,DCM_SVC_19_UTI_UPDATE_GET_FF_RECORD
                                            ,dataLen
                                            ,pRepContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      /* Otherwise, proceed with response transmission with no further data */
      lStdResult = DCM_E_OK;
    }
    break;
  case DCM_DEMIF_RET_SIZEOFFFR_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetSizeOfFreezeFrameByDTC);                                              /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_DEMIF_SELECT_EDR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectExtRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectExtRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnSelectExtRecordType lDemResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lDemResult = Dcm_DemIf(SelectExtendedDataRecord)(pRepContext->DemClientId
                                                  ,pRepContext->RecordNum);

  switch (lDemResult)
  {
  case DCM_DEMIF_RET_SELECT_EXT_DATAREC_OK:
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_SELECT_EXT_DATAREC_BUSY:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSizeOfExtRecord()
 *********************************************************************************************************************/
/*!
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSizeOfExtRecord(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;
  Dcm_DemIfReturnGetSizeOfEDRByDTCType demResult;
  uint16 dataLen;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResult = Dcm_DemIf(GetSizeOfExtendedDataRecordByDTC)(pRepContext->Dtc
                                                         ,pRepContext->Origin
                                                         ,pRepContext->RecordNum
                                                         ,&dataLen);                                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
  demResult = Dcm_DemIf(GetSizeOfExtendedDataRecordSelection)(pRepContext->DemClientId
                                                             ,&dataLen);                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif
  switch (demResult)
  {
  case DCM_DEMIF_RET_SIZEOFEDR_OK:
    if (dataLen != 0u) /* consider only stored records */
    {
#  if (DCM_DEMIF_421_ENABLED == STD_ON)
      if (pRepContext->RecordNumList != NULL_PTR)
      {
        pRepContext->RecordNum = pRepContext->RecordNumList[0];/* take first one */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugAssert((DCM_SVC_19_UTI_RECNUM_ALL != pRepContext->RecordNum), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                               /* COV_DCM_RTM_DEV_DEBUG XF */
      }
#  endif
      lStdReturn = Dcm_Svc19UtiStartReporting(pContext
                                            ,pMsgContext
                                            ,DCM_SVC_19_UTI_UPDATE_GET_EXT_RECORD
                                            ,dataLen
                                            ,pRepContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      /* Otherwise, proceed with response transmission with no further data */
      lStdReturn = DCM_E_OK;
    }
    break;
  case DCM_DEMIF_RET_SIZEOFEDR_PENDING:
    lStdReturn = DCM_E_PENDING;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetSizeOfExtendedDataRecordByDTC);                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
    break;
  }
  return lStdReturn;
}
# endif

# if (DCM_SVC_19_UTI_GETNUMFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNumFltrdDtc()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNumFltrdDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnGetNumberOfFilteredDTCType demResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResult = Dcm_DemIf(GetNumberOfFilteredDTC)(&pRepContext->NumDtc);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
#  else
  demResult = Dcm_DemIf(GetNumberOfFilteredDTC)(pRepContext->DemClientId
                                               ,&pRepContext->NumDtc);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif
  switch (demResult)
  {
  case DCM_DEMIF_RET_NUMBER_OK:
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_NUMBER_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:/* DEM_NUMBER_FAILED or unknown */
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_DemIfNrcMapGetNumberOfFilteredDTC);                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiStartReporting(pContext
                                  ,pMsgContext
                                  ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY
                                  ,(Dcm_MsgLenType)((Dcm_MsgLenType)(pRepContext->NumDtc) * 6u)
                                  ,pRepContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetSeverityOfDtc()
 *********************************************************************************************************************/
/*!
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetSeverityOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnGetSeverityOfDTCType demResultSeverity;
  uint8 dtcSeverity;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  demResultSeverity = Dcm_DemIf(GetSeverityOfDTC)(pRepContext->Dtc
                                                 ,&dtcSeverity);                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
  demResultSeverity = Dcm_DemIf(GetSeverityOfDTC)(pRepContext->DemClientId
                                                 ,&dtcSeverity);                                                                                     /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif

  switch (demResultSeverity)
  {
#  if defined (DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_NOSEVERITY)
  case DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_NOSEVERITY:
    dtcSeverity = 0; /* set to NO SEVERITY as specified in ISO14229-1 and go on with the remained part */
#  endif
    /* fall through */
  case DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_OK:                                                                                                         /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSeverity);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResultSeverity, Dcm_Svc19UtiNrcMapGetSeverityOfDTC);                                               /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_GETSEVERITYOFDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetFuncUnitOfDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetFuncUnitOfDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8 functionalUnit;
  Dcm_DemIfReturnGetFunctionalUnitOfDTCType lDemResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  lDemResult = Dcm_DemIf(GetFunctionalUnitOfDTC)(pRepContext->Dtc
                                                ,&functionalUnit);                                                                                   /* SBSW_DCM_POINTER_FORWARD_STACK */
#  else
  lDemResult = Dcm_DemIf(GetFunctionalUnitOfDTC)(pRepContext->DemClientId
                                                ,&functionalUnit);                                                                                   /* SBSW_DCM_POINTER_FORWARD_STACK */
#  endif

  switch (lDemResult)
  {
  case DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_OK:
    Dcm_UtiProvideResDataAsU8(pMsgContext, functionalUnit);                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  if defined (DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_PENDING)
  case DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_PENDING:
    lStdResult = DCM_E_PENDING;
    break;
#  endif
  default:
    *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(lDemResult, Dcm_Svc19UtiNrcMapGetFunctionalUnitOfDTC);                                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_DEMIF_SELECT_FFR_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSelectFFData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSelectFFData(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DemIfReturnSelectFFDataType lDemResult;

  lDemResult = Dcm_DemIf(SelectFreezeFrameData)(pRepContext->DemClientId
                                               ,pRepContext->RecordNum);

  switch (lDemResult)
  {
  case DCM_DEMIF_RET_SELECT_FF_DATA_OK:
    lStdResult = Dcm_Svc19UtiChainNext(pContext, pRepContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_DEMIF_RET_SELECT_FF_DATA_BUSY:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdFDC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdFDC(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiStartReporting(pContext
                                  ,pMsgContext
                                  ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_FDC
                                  ,(Dcm_MsgLenType)((Dcm_MsgLenType)(pRepContext->NumDtc) * 4u)
                                  ,pRepContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetReadinessGroupIdentifierFilter(
  Dem_EventOBDReadinessGroupType readinessGroupNumber,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lDemResult;

  lDemResult = Dem_SetDTCFilterByReadinessGroup(pRepContext->DemClientId
                                               ,readinessGroupNumber
                                               ,DEM_DTC_FORMAT_UDS);

  switch (lDemResult)
  {
  case E_OK:
    lStdResult = DCM_E_OK;
    break;
  case E_NOT_OK:
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_16_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetExtDataRecordFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetExtDataRecordFilter(
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lDemResult;

  lDemResult = Dem_SetExtendedDataRecordFilter(pRepContext->DemClientId
                                              ,pRepContext->RecordNum
                                              ,DEM_DTC_ORIGIN_PRIMARY_MEMORY
                                              ,DEM_DTC_FORMAT_UDS);

  switch (lDemResult)
  {
  case E_OK:
    lStdResult = DCM_E_OK;
    break;
  case DEM_BUSY:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  case E_NOT_OK:
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiWwhObdDemGetNxtFltrdSeverity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiWwhObdDemGetNxtFltrdSeverity(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiStartReporting(pContext
                                  ,pMsgContext
                                  ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_SEVERITY_WWHOBD
                                  ,(Dcm_MsgLenType)((Dcm_MsgLenType)(pRepContext->NumDtc) * 5u)
                                  ,pRepContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemGetNxtFltrdDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemGetNxtFltrdDtc(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return Dcm_Svc19UtiStartReporting(pContext
                                  ,pMsgContext
                                  ,DCM_SVC_19_UTI_UPDATE_GET_NXT_FLTRD_DTC
                                  ,(Dcm_MsgLenType)((Dcm_MsgLenType)(pRepContext->NumDtc) * 4u)
                                  ,pRepContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiPutNumFilteredDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiPutNumFilteredDtc(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_UtiProvideResDataAsU16(pMsgContext, pRepContext->NumDtc);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

  return DCM_E_OK;
}
# endif

# if (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiDemSetDTCFilterByExtendedDataRecordNumber(
  uint8 ExtendedDataRecordNumber,
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lDemResult;

  lDemResult = Dem_SetDTCFilterByExtendedDataRecordNumber(pRepContext->DemClientId, ExtendedDataRecordNumber, DEM_DTC_FORMAT_UDS);

  switch (lDemResult)
  {
  case E_OK:
    lStdResult = DCM_E_OK;
    break;
  case E_NOT_OK:
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_19_UTI_READ_DTC_CNT_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportNumDtcByStatusMaskData()
 *********************************************************************************************************************/
/*!
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
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportNumDtcByStatusMaskData(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19UtiDemSetFilterClassType dsfClass,
  Dem_DTCSeverityType dtcSeverity,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8 dtcSAM;
  uint8 dtcStatusMask;

  /* Gather the configured DEM Status Availability Mask */
  lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == E_OK)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcStatusMask);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */

    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_DemIf(GetTranslationType)());                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  else
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_DemIf(GetTranslationType)(pRepContext->DemClientId));                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif

    Dcm_Svc19UtiPatchDtcStatusSetForFilter(&dtcStatusMask);                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

    dtcStatusMask &= dtcSAM;
    if (dtcStatusMask != 0u)
    {
      lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                              ,dsfClass
                                              ,dtcStatusMask
                                              ,dtcSeverity
                                              ,DCM_DEMIF_FILTER_FOR_FDC_NO
                                              ,pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      if (lStdResult == DCM_E_OK)
      {
        lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainDtcCount, pRepContext);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
      }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilter */
    }
    else
    {
      Dcm_UtiProvideResDataAsU16(pMsgContext, 0);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_OK;
    }
  }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportDtcByStatusMaskData()
 *********************************************************************************************************************/
/*!
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
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportDtcByStatusMaskData(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19UtiDemSetFilterClassType dsfClass,
  Dcm_Svc19UtiDemSetFilterMaskUsageType maskUsage,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8 dtcStatusMask;
  uint8 dtcSAM;

  /* Gather the configured DEM Status Availability Mask */
  lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == E_OK)
  {
    lStdResult = DCM_E_NOT_OK;

    /* If a requested DTC status mask shall be considered: */
    if (maskUsage == DCM_SVC_19_UTI_USE_MASK_REQ)
    {
      Dcm_UtiConsumeReqDataAsU8(pMsgContext, &dtcStatusMask);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

      Dcm_Svc19UtiPatchDtcStatusSetForFilter(&dtcStatusMask);                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

      dtcStatusMask &= dtcSAM;

      if (dtcStatusMask == 0u)
      {
        /* If no supported (relevant) status bit remains, omit DEM SetFilter API invokation
        * (i.e. send final positive response only with SAM)
        */
        lStdResult = DCM_E_OK;
      }
      /* Otherwise, prepare a DEM filter to consider relevant DTC status bits */
    }
    else
    {
      /* Otherwise, prepare a DEM filter to report all supported DTCs */
      dtcStatusMask = 0u;
    }

    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* If DEM filtering still required: */
    if (lStdResult != DCM_E_OK)
    {
      lStdResult = Dcm_Svc19UtiDemSetDTCFilter(ErrorCode
                                              ,dsfClass
                                              ,dtcStatusMask
                                              ,0 /* parameter not used */
                                              ,DCM_DEMIF_FILTER_FOR_FDC_NO
                                              ,pRepContext);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      if (lStdResult == DCM_E_OK)
      {
        lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadDtcByStatus, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
      }/* Otherwise, Already set by Dcm_Svc19UtiDemSetDTCFilter */
    }/* Otherwise, send the final response with no additional data */
  }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportExtDataRecordByDtcNum()
 *********************************************************************************************************************/
/*!
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportExtDataRecordByDtcNum(                                                                              /* PRQA S 2889 */ /* MD_Dcm_Optimize_2889 */
  Dcm_ContextPtrType pContext,
  Dem_DTCOriginType dtcOrigin,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pRepContext->Origin = dtcOrigin;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_UtiConsumeReqDataAsU24(pMsgContext, &(pRepContext->Dtc));                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pRepContext->RecordNum));                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  if (pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_ALL)
  {
    pRepContext->RecordNumList = Dcm_Svc19UtiExtDataRecords;                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else if (pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_OBD_ALL)
  {
#   if (DCM_SVC_19_UTI_OBD_EXT_RECORD_ENABLED == STD_ON)
    pRepContext->RecordNumList = Dcm_Svc19UtiExtDataRecordsObd;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
#   else
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* the ECU does not support any OBD extended data records - reject request */                              /* SBSW_DCM_PARAM_PTR_WRITE */
    return DCM_E_NOT_OK;
#   endif
  }
  else
  {
    /* Single record */
    pRepContext->RecordNumList = NULL_PTR;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  }
#  endif
  return Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadExtRecords, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif

# if (DCM_SVC_19_UTI_CHAINING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiChainStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiChainStart(
  Dcm_ContextPtrType pContext,
  P2CONST(Dcm_RepeaterProgressType, AUTOMATIC, DCM_CONST) funcChain,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  pRepContext->FuncChain = funcChain;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextStep(pContext, *pRepContext->FuncChain);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  return DCM_E_LOOP;/* execute it immediately */
}
# endif

# if (DCM_SVC_19_UTI_CHAINING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiChainNext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiChainNext(
  Dcm_ContextPtrType pContext,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_UtiNextItemByPtr(pRepContext->FuncChain, 1u);                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextStep(pContext, *pRepContext->FuncChain);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  return DCM_E_LOOP;/* take next item from list */
}
# endif

# if (DCM_SVC_19_UTI_READ_DTC_BY_OCCUR_TIME_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportNumDtcByOccurenceTime()
 *********************************************************************************************************************/
/*!
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportNumDtcByOccurenceTime(
  Dcm_ContextPtrType pContext,
  Dem_DTCRequestType dtcReqType,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  uint8 dtcSAM;

  /* Gather the configured DEM Status Availability Mask */
  lStdResult = Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask(&dtcSAM, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == E_OK)
  {
    Dcm_DemIfReturnGetDTCByOccurrenceTimeType demResult;

    Dcm_UtiProvideResDataAsU8(pMsgContext, dtcSAM);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    demResult = Dcm_DemIf(GetDTCByOccurrenceTime)(
#  if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
      pRepContext->DemClientId,
#  endif
      dtcReqType
      , &pRepContext->Dtc);                                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    switch (demResult)
    {
    case DCM_DEMIF_RET_OCCURR_OK:
      pRepContext->Origin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainGetDtcStatusEnd, pRepContext);                                               /* SBSW_DCM_COMB_PTR_FORWARD */
      break;
    default:
      *ErrorCode = Dcm_DemIfGetNrcForDemRetValue(demResult, Dcm_Svc19UtiNrcMapGetDTCByOccurrenceTime);                                               /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }/* Otherwise, Already set by Dcm_Svc19UtiDemGetDTCStatusAvailabilityMask */
  return lStdResult;
}
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc19UtiReportDtcFFRByDtc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc19UtiReportDtcFFRByDtc(
  Dcm_ContextPtrType pContext,
  Dem_DTCOriginType dtcOrigin,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc19RepeaterProxyContextPtrType pRepContext
  )
{
  pRepContext->Origin = dtcOrigin;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiConsumeReqDataAsU24(pMsgContext, &(pRepContext->Dtc));                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pRepContext->RecordNum));                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

#  if (DCM_DEMIF_421_ENABLED == STD_ON)
  if (pRepContext->RecordNum == DCM_SVC_19_UTI_RECNUM_ALL)
  {
    pRepContext->RecordNumList = Dcm_Svc19UtiFFDataRecords;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Single record */
    pRepContext->RecordNumList = NULL_PTR;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  }
#  endif
  return Dcm_Svc19UtiChainStart(pContext, Dcm_Svc19UtiDemChainReadFFRByDtc, pRepContext);                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc19Uti.c
 *********************************************************************************************************************/

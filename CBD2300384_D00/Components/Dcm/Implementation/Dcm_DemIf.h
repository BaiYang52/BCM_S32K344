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
/**        \file  Dcm_DemIf.h
 *         \unit  DemIf
 *        \brief  Contains public inline function definitions of Dem Interface unit.
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
#if !defined(DCM_DEMIF_H)
# define DCM_DEMIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_DemIfTypes.h"
# include "Dcm_Uti.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_SVC_04_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_14_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_CLEAR_DTC_ENABLED                                STD_ON
# else
#  define DCM_DEMIF_CLEAR_DTC_ENABLED                                STD_OFF
# endif

# if (DCM_SVC_19_02_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0A_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0F_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_13_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_15_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_17_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED           STD_ON
# else
#  define DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED           STD_OFF
# endif

# if (DCM_SVC_19_06_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_10_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_19_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED      STD_ON
# else
#  define DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED      STD_OFF
# endif

# if (DCM_SVC_19_04_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_18_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED               STD_ON
# else
#  define DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED               STD_OFF
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED                STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED                STD_OFF
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_FFR_BY_DTC_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED                       STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED                       STD_OFF
# endif

# if (DCM_DEMIF_SVC_19_READ_DTC_BY_STATUS_MASK_ENABLED == STD_ON) || \
     (DCM_SVC_19_1A_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_55_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_56_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED                STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED                STD_OFF
# endif

# if (DCM_SVC_19_08_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_42_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDSEV_ENABLED          STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDSEV_ENABLED          STD_OFF
# endif

# if (DCM_SVC_19_14_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDFDC_ENABLED          STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDFDC_ENABLED          STD_OFF
# endif

# if (DCM_SVC_19_03_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDRECORD_ENABLED             STD_ON
# else
#  define DCM_DEMIF_SVC_19_GETNEXTFILTEREDRECORD_ENABLED             STD_OFF
# endif

# if ((DCM_DEMIF_430_ENABLED == STD_ON) || \
     (DCM_DEMIF_431_ENABLED == STD_ON)) && \
     ((DCM_SVC_04_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_14_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_04_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_05_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_06_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_09_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0B_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0C_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0D_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_0E_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_10_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_18_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_19_SUPPORT_ENABLED == STD_ON))
#  define DCM_DEMIF_SELECT_DTC_ENABLED                               STD_ON
# else
#  define DCM_DEMIF_SELECT_DTC_ENABLED                               STD_OFF
# endif

# if ((DCM_DEMIF_430_ENABLED == STD_ON) || \
     (DCM_DEMIF_431_ENABLED == STD_ON)) && \
     (DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED == STD_ON)
#  define DCM_DEMIF_SELECT_FFR_ENABLED                               STD_ON
# else
#  define DCM_DEMIF_SELECT_FFR_ENABLED                               STD_OFF
# endif

# if ((DCM_DEMIF_430_ENABLED == STD_ON) || \
     (DCM_DEMIF_431_ENABLED == STD_ON)) && \
     (DCM_DEMIF_SVC_19_GETSIZEOFEXTRECORD_ENABLED == STD_ON)
#  define DCM_DEMIF_SELECT_EDR_ENABLED                               STD_ON
# else
#  define DCM_DEMIF_SELECT_EDR_ENABLED                               STD_OFF
# endif

# if (DCM_SVC_04_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_14_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) || \
     (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
#  define DCM_DEMIF_RETVAL_TO_NRC_MAP_ENABLED                        STD_ON
# else
#  define DCM_DEMIF_RETVAL_TO_NRC_MAP_ENABLED                        STD_OFF
# endif

# if (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON) || \
     (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTC_ENABLED == STD_ON) || \
     (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDSEV_ENABLED == STD_ON) || \
     (DCM_DEMIF_SVC_19_GETNEXTFILTEREDDTCANDFDC_ENABLED == STD_ON) || \
     (DCM_DEMIF_SVC_19_GETNEXTFILTEREDRECORD_ENABLED == STD_ON)
#  define DCM_DEMIF_GETNEXTFILTEREDITEM_ENABLED                      STD_ON
# else
#  define DCM_DEMIF_GETNEXTFILTEREDITEM_ENABLED                      STD_OFF
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if (DCM_DEMIF_421_ENABLED == STD_ON)
/*! AR 4.2.1 DEM API function name substitutions - generic rename */
#  define Dcm_DemIf(name)                                            Dem_Dcm ## name                                                                 /* PRQA S 0342 */ /* MD_MSR_Rule20.10_0342 */

/*! AR 4.2.1 DEM API function name substitutions - special cases */
#  define Dcm_DemIfCancelOperation                                   Dem_DcmCancelOperation
#  define Dcm_DemIfGetOBDFreezeFrameData(recNum, dtcOrigin, dtc, buffer, bufferSize) (Dem_DcmGetOBDFreezeFrameData((dtc), (buffer), (bufferSize)))   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_DemIfControlDTCStatusChangedNotification(active)       Dem_DcmControlDTCStatusChangedNotification((active))                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# elif (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
/* functions - generic rename */
#  define Dcm_DemIf(name)                                            Dem_ ## name                                                                    /* PRQA S 0342 */ /* MD_MSR_Rule20.10_0342 */

/* functions - special cases */
#  define Dcm_DemIfCancelOperation()                                 /* Not used */
#  define Dcm_DemIfGetOBDFreezeFrameData(recNum, dtcOrigin, dtc, buffer, bufferSize) (Dem_DcmGetOBDFreezeFrameData((dtc), (buffer), (bufferSize)))   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_DemIfControlDTCStatusChangedNotification(active)       /* Not used */
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfGetDTCOfOBDFreezeFrame()
 *********************************************************************************************************************/
 /*! \brief          Wraps the DEM API call Dem_DcmGetDTCOfOBDFreezeFrame() for the correct API version.
 *  \details        -
 *  \param[in]      frameNumber               Unique identifier for a freeze frame record
 *  \param[in]      dtcFormat                 Output format of the DTC value
 *  \param[out]     dtc                       Diagnostic trouble code in OBD format
 *  \return         E_OK                      operation was successful
 *  \return         E_NOT_OK                  no DTC available
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfGetDTCOfOBDFreezeFrame(
  uint8 frameNumber,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dtc,
  Dem_DTCFormatType dtcFormat
  );
# endif

# if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfReadDataOfOBDFreezeFrame()
 *********************************************************************************************************************/
/*! \brief          Wraps the DEM API call Dem_DcmReadDataOfOBDFreezeFrame() for the correct API version.
 *  \details        -
 *  \param[in]      pid                       Identifier for PID
 *  \param[in]      dataElementIndexOfPid     Data element index of this PID
 *  \param[in,out]  destBuffer                Buffer where PID shall be written
 *  \param[in,out]  bufSize                   Max number of writable bytes
 *  \return         E_OK                      Freeze frame data successfully reported
 *  \return         E_NOT_OK                  Freeze frame data report failed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfReadDataOfOBDFreezeFrame(
  uint8 pid,
  uint8 dataElementIndexOfPid,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) destBuffer,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, AUTOMATIC) bufSize
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DEMIF_RETVAL_TO_NRC_MAP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfGetNrcForDemRetValue()
 *********************************************************************************************************************/
/*! \brief          Converts a DEM API return value to a concrete NRC.
 *  \details        -
 *  \param[in]      demResult           DEM API returned value
 *  \param[in]      demResult2NrcMap    Pointer to a DEM API specific map (serialized array of pair values)
 *  \return         The associated NRC for the given DEM result value
 *  \return         The panic NRC
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DemIfGetNrcForDemRetValue(
  uint8 demResult,
  Dcm_Uint8ConstDataPtrType demResult2NrcMap
  );
# endif

# if (DCM_DEMIF_GETNEXTFILTEREDITEM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfConvDemRsltGetNextFilteredItem()
 *********************************************************************************************************************/
/*! \brief          Converts a DEM API return value to a concrete DCM return value.
 *  \details        -
 *  \param[in]      demResult                 DEM API returned value
 *  \param[out]     ErrorCode                 The NRC
 *  \return         DCM_E_DATA_READY_PADDING  Force padding of remaining bytes
 *  \return         DCM_E_PENDING             Retry later
 *  \return         DCM_E_BUFFERTOOLOW        Retry later, because of no available space any more
 *  \return         DCM_E_NOT_OK              Error occured
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfConvDemRsltGetNextFilteredItem(
  Dcm_DemIfReturnGetNextFilteredItemType demResult,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) || \
     (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
/*! Dem GetNumberOfFilteredDTC API return value to NRC mapping */
extern CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapGetNumberOfFilteredDTC[];                                                                              /* PRQA S 3684 */ /* MD_Dcm_UnknownSize_3684 */
# endif

# if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) || \
     (DCM_OBDDTCMGR_SUPPORT_ENABLED == STD_ON)
/*! Dem SetDTCFilter API return value to NRC mapping */
extern CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapSetDTCFilter[3];
# endif

# if (DCM_DEMIF_SELECT_DTC_ENABLED == STD_ON)
/*! Dem SelectDTC API return value to NRC mapping */
extern CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapSelectDTC[3];
# endif

# if (DCM_DEMIF_CLEAR_DTC_ENABLED == STD_ON)
/*! Dem ClearDTC API return value to NRC mapping */
extern CONST(uint8, DCM_CONST) Dcm_DemIfNrcMapClearDTC[];                                                                                            /* PRQA S 3684 */ /* MD_Dcm_UnknownSize_3684 */
# endif
# define DCM_STOP_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if !defined(DCM_NOUNIT_DEMIF)                                                                                                                      /* COV_DCM_NOUNIT TX */
#  if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfGetDTCOfOBDFreezeFrame()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfGetDTCOfOBDFreezeFrame(                                                                    /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  uint8 frameNumber,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) dtc,
  Dem_DTCFormatType dtcFormat
  )
{
  Std_ReturnType lDemResult;
  DCM_IGNORE_UNREF_PARAM(dtcFormat);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lDemResult = Dem_DcmGetDTCOfOBDFreezeFrame(frameNumber
    , dtc
#   if (DCM_DEMIF_421_ENABLED == STD_OFF)
    , dtcFormat
#   endif
    );                                                                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

  return lDemResult;
}
#  endif

#  if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DemIfReadDataOfOBDFreezeFrame()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DemIfReadDataOfOBDFreezeFrame(                                                                  /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  uint8 pid,
  uint8 dataElementIndexOfPid,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) destBuffer,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, AUTOMATIC) bufSize
  )
{
  Std_ReturnType lDemResult;

  /* Calculate maximum possible buffer size */
#   if (DCM_DEMIF_421_ENABLED == STD_ON)
  uint8 lBufSize;
  lBufSize = (uint8)Dcm_UtiMathMin(*bufSize, 0xFFu);
#   else
  uint16 lBufSize;
  lBufSize = (uint16)Dcm_UtiMathMin(*bufSize, 0xFFFFu);
#   endif

  lDemResult = Dem_DcmReadDataOfOBDFreezeFrame(pid
    , dataElementIndexOfPid
    , destBuffer
    , &lBufSize);                                                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  *bufSize = lBufSize;                                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  return lDemResult;
}
#  endif
# endif /* !defined(DCM_NOUNIT_DEMIF) */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* !defined(DCM_DEMIF_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_DemIf.h
 *********************************************************************************************************************/

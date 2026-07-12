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
/**        \file  Dcm_DemIfTypes.h
 *         \unit  DemIf
 *        \brief  Contains native types and forward declarations of Dem Interface unit.
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
#if !defined(DCM_DEMIFTYPES_H)
# define DCM_DEMIFTYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Types.h"
# if (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
#  include "Dem.h"
# endif
# if (DCM_DEMIF_421_ENABLED == STD_ON) || (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
#  include "Dem_Dcm.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Standard ISO 14229-1:2013 (UDS) DTC status bit representation */
# define DCM_DTC_BIT_DONT_CARE                                       ((uint8)0x00u)
# define DCM_DTC_BIT_TST_FLD                                         ((uint8)0x01u)
# define DCM_DTC_BIT_TST_FLD_CYCL                                    ((uint8)0x02u)
# define DCM_DTC_BIT_PNDG_DTC                                        ((uint8)0x04u)
# define DCM_DTC_BIT_CONF_DTC                                        ((uint8)0x08u)
# define DCM_DTC_BIT_TST_NOT_COMPL_CLR                               ((uint8)0x10u)
# define DCM_DTC_BIT_TST_FLD_CLR                                     ((uint8)0x20u)
# define DCM_DTC_BIT_TST_NOT_COMPL_CYCL                              ((uint8)0x40u)
# define DCM_DTC_BIT_WARN_IND_REQ                                    ((uint8)0x80u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if (DCM_DEMIF_421_ENABLED == STD_ON)
/*! AR 4.2.1+ DEM API compliant return values */

/*! Return values for Dem_ReturnGetFreezeFrameByDTCType */
#  define DCM_DEMIF_RET_FFDATABYDTC_OK                               DEM_GET_FFDATABYDTC_OK
#  define DCM_DEMIF_RET_FFDATABYDTC_W_DTC                            DEM_GET_FFDATABYDTC_WRONG_DTC
#  define DCM_DEMIF_RET_FFDATABYDTC_W_ORIGIN                         DEM_GET_FFDATABYDTC_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_FFDATABYDTC_W_RNUM                           DEM_GET_FFDATABYDTC_WRONG_RECORDNUMBER
#  define DCM_DEMIF_RET_FFDATABYDTC_W_BUFSIZE                        DEM_GET_FFDATABYDTC_WRONG_BUFFERSIZE
#  define DCM_DEMIF_RET_FFDATABYDTC_PENDING                          DEM_GET_FFDATABYDTC_PENDING

/*! Return values for Dcm_DemIfReturnGetFreezeFrameDataByRecordType */
#  define DCM_DEMIF_RET_FFBYRECORD_OK                                E_OK
#  define DCM_DEMIF_RET_FFBYRECORD_NO_DTC                            E_NOT_OK

/*! Return values for Dcm_DemIfReturnGetSizeOfFFRByDTCType (ED) */
#  define DCM_DEMIF_RET_SIZEOFEDR_OK                                 DEM_GETSIZEBYDTC_OK
#  define DCM_DEMIF_RET_SIZEOFEDR_W_DTC                              DEM_GETSIZEBYDTC_WRONG_DTC
#  define DCM_DEMIF_RET_SIZEOFEDR_W_ORIGIN                           DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_SIZEOFEDR_W_RNUM                             DEM_GETSIZEBYDTC_WRONG_RECNUM
#  define DCM_DEMIF_RET_SIZEOFEDR_PENDING                            DEM_GETSIZEBYDTC_PENDING

/*! Return values for Dcm_DemIfReturnGetSizeOfFFRByDTCType (FF) */
#  define DCM_DEMIF_RET_SIZEOFFFR_OK                                 DEM_GETSIZEBYDTC_OK
#  define DCM_DEMIF_RET_SIZEOFFFR_W_DTC                              DEM_GETSIZEBYDTC_WRONG_DTC
#  define DCM_DEMIF_RET_SIZEOFFFR_W_ORIGIN                           DEM_GETSIZEBYDTC_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_SIZEOFFFR_W_RNUM                             DEM_GETSIZEBYDTC_WRONG_RECNUM
#  define DCM_DEMIF_RET_SIZEOFFFR_PENDING                            DEM_GETSIZEBYDTC_PENDING

/*! Return values for Dem_ReturnGetDTCByOccurrenceType */
#  define DCM_DEMIF_RET_OCCURR_OK                                    DEM_OCCURR_OK
#  define DCM_DEMIF_RET_OCCURR_NOT_AVAILABLE                         DEM_OCCURR_NOT_AVAILABLE

/*! Return values for Dcm_DemIfReturnGetNextFilteredElementType */
#  define DCM_DEMIF_RET_FILTERED_OK                                  DEM_FILTERED_OK
#  define DCM_DEMIF_RET_FILTERED_NO_ITEMS                            DEM_FILTERED_NO_MATCHING_ELEMENT
#  define DCM_DEMIF_RET_FILTERED_PENDING                             DEM_FILTERED_PENDING
#  define DCM_DEMIF_RET_FILTERED_BUFFER_TOO_SMALL                    DEM_FILTERED_BUFFER_TOO_SMALL

/*! Return values for Dcm_DemIfReturnGetSeverityOfDTCType */
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_OK                       DEM_GET_SEVERITYOFDTC_OK
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTC                DEM_GET_SEVERITYOFDTC_WRONG_DTC
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_NOSEVERITY               DEM_GET_SEVERITYOFDTC_NOSEVERITY
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_PENDING                  DEM_GET_SEVERITYOFDTC_PENDING

/*! Return values for Dcm_DemIfReturnGetFunctionalUnitOfDTCType */
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_OK                   DEM_GET_FUNCTIONALUNITOFDTC_OK
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTC            DEM_GET_FUNCTIONALUNITOFDTC_WRONG_DTC

/*! Return values for Dcm_DemIfReturnSetFilterType */
#  define DCM_DEMIF_RET_FILTER_ACCEPTED                              DEM_FILTER_ACCEPTED
#  define DCM_DEMIF_RET_WRONG_FILTER                                 DEM_WRONG_FILTER

/*! Return values for Dem_ReturnClearDTCType */
#  define DCM_DEMIF_RET_CLEAR_OK                                     DEM_CLEAR_OK
#  define DCM_DEMIF_RET_CLEAR_W_DTC                                  DEM_CLEAR_WRONG_DTC
#  define DCM_DEMIF_RET_CLEAR_W_ORIGIN                               DEM_CLEAR_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_CLEAR_FAILED                                 DEM_CLEAR_FAILED
#  define DCM_DEMIF_RET_CLEAR_PENDING                                DEM_CLEAR_PENDING
#  define DCM_DEMIF_RET_CLEAR_CLEAR_BUSY                             DEM_CLEAR_BUSY
#  define DCM_DEMIF_RET_CLEAR_MEMORY_ERROR                           DEM_CLEAR_MEMORY_ERROR

/*! Return values for Dem_GetStatusOfDTCType */
#  define DCM_DEMIF_RET_STATUS_OK                                    DEM_STATUS_OK
#  define DCM_DEMIF_RET_STATUS_PENDING                               DEM_STATUS_PENDING
#  define DCM_DEMIF_RET_STATUS_WRONG_DTC                             DEM_STATUS_WRONG_DTC
#  define DCM_DEMIF_RET_STATUS_FAILED                                DEM_STATUS_FAILED
#  define DCM_DEMIF_RET_STATUS_WRONG_DTCORIGIN                       DEM_STATUS_WRONG_DTCORIGIN

/* Std_ReturnType */
#  define DCM_DEMIF_RET_NUMBER_OK                                    DEM_NUMBER_OK
#  define DCM_DEMIF_RET_NUMBER_PENDING                               DEM_NUMBER_PENDING

/* Dem_ReturnControlDTCSettingType */
#  define DCM_DEMIF_RET_CONTROL_DTC_SETTING_OK                       DEM_CONTROL_DTC_SETTING_OK
#  define DCM_DEMIF_RET_CONTROL_DTC_SETTING_N_OK                     DEM_CONTROL_DTC_SETTING_N_OK
#  define DCM_DEMIF_RET_CONTROL_DTC_WRONG_DTCGROUP                   DEM_CONTROL_DTC_WRONG_DTCGROUP

/* Return values for Dem_ReturnGetExtendedDataRecoredByDTCType */
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_OK                          DEM_RECORD_OK
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTC                   DEM_RECORD_WRONG_DTC
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTCORIGIN             DEM_RECORD_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_NUMBER                DEM_RECORD_WRONG_NUMBER
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_BUFFERSIZE            DEM_RECORD_WRONG_BUFFERSIZE
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_PENDING                     DEM_RECORD_PENDING

#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_OK                        DEM_DISABLE_DTCRECUP_OK
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTC                 DEM_DISABLE_DTCRECUP_WRONG_DTC
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTCORIGIN           DEM_DISABLE_DTCRECUP_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_PENDING                   DEM_DISABLE_DTCRECUP_PENDING

#  define DCM_DEMIF_FILTER_FOR_FDC_NO                                DEM_FILTER_FOR_FDC_NO
#  define DCM_DEMIF_FILTER_FOR_FDC_YES                               DEM_FILTER_FOR_FDC_YES
#  define DCM_DEMIF_FILTER_WITH_SEVERITY_NO                          DEM_FILTER_WITH_SEVERITY_NO
#  define DCM_DEMIF_FILTER_WITH_SEVERITY_YES                         DEM_FILTER_WITH_SEVERITY_YES

/*! AR 4.2.1 DEM API compliant return types */
#  define Dcm_DemIfFilterForFDCType                                  Dem_FilterForFDCType
#  define Dcm_DemIfFilterWithSeverityType                            Dem_FilterWithSeverityType
#  define Dcm_DemIfReturnClearDTCType                                Dem_ReturnClearDTCType
#  define Dcm_DemIfReturnControlDTCSettingType                       Dem_ReturnControlDTCSettingType
#  define Dcm_DemIfReturnDisableDTCRecordUpdateType                  Dem_ReturnDisableDTCRecordUpdateType
#  define Dcm_DemIfReturnGetDTCByOccurrenceTimeType                  Dem_ReturnGetDTCByOccurrenceTimeType
#  define Dcm_DemIfReturnGetFreezeFrameDataByRecordType              Std_ReturnType
#  define Dcm_DemIfReturnGetFunctionalUnitOfDTCType                  Dem_ReturnGetFunctionalUnitOfDTCType
#  define Dcm_DemIfReturnGetNextFilteredElementType                  Dem_ReturnGetNextFilteredElementType
#  define Dcm_DemIfReturnGetNextFilteredItemType                     Dem_ReturnGetNextFilteredElementType
#  define Dcm_DemIfReturnGetNumberOfFilteredDTCType                  Dem_ReturnGetNumberOfFilteredDTCType
#  define Dcm_DemIfReturnGetSeverityOfDTCType                        Dem_ReturnGetSeverityOfDTCType
#  define Dcm_DemIfReturnGetSizeOfDataByDTCType                      Dem_ReturnGetSizeOfDataByDTCType
#  define Dcm_DemIfReturnGetSizeOfEDRByDTCType                       Dem_ReturnGetSizeOfDataByDTCType
#  define Dcm_DemIfReturnGetSizeOfFFRByDTCType                       Dem_ReturnGetSizeOfDataByDTCType
#  define Dcm_DemIfReturnGetStatusOfDTCType                          Dem_ReturnGetStatusOfDTCType
#  define Dcm_DemIfReturnSetFilterType                               Dem_ReturnSetFilterType

# elif (DCM_DEMIF_430_ENABLED == STD_ON) || (DCM_DEMIF_431_ENABLED == STD_ON)
/* return values */

/* Define DEM_BUSY here, because DEM_BUSY is not supported by older versions of MICROSAR DEM. */
#  ifndef DEM_BUSY
#   define DEM_BUSY                                                  ((Std_ReturnType)(22U))
#  endif

/* Std_ReturnType */
#  define DCM_DEMIF_RET_FFDATABYDTC_OK                               E_OK
#  define DCM_DEMIF_RET_FFDATABYDTC_W_DTC                            DEM_WRONG_DTC
#  define DCM_DEMIF_RET_FFDATABYDTC_W_ORIGIN                         DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_FFDATABYDTC_W_RNUM                           DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_FFDATABYDTC_W_BUFSIZE                        DEM_BUFFER_TOO_SMALL
#  define DCM_DEMIF_RET_FFDATABYDTC_PENDING                          DEM_PENDING

/* Std_ReturnType */
#  define DCM_DEMIF_RET_FFBYRECORD_OK                                E_OK
#  define DCM_DEMIF_RET_FFBYRECORD_NO_DTC                            E_NOT_OK

/* Std_ReturnType */
#  define DCM_DEMIF_RET_SIZEOFEDR_OK                                 E_OK
#  define DCM_DEMIF_RET_SIZEOFEDR_N_OK                               E_NOT_OK
#  define DCM_DEMIF_RET_SIZEOFEDR_W_DTC                              DEM_WRONG_DTC
#  define DCM_DEMIF_RET_SIZEOFEDR_W_ORIGIN                           DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_SIZEOFEDR_W_RNUM                             DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_SIZEOFEDR_PENDING                            DEM_PENDING

/* Std_ReturnType */
#  define DCM_DEMIF_RET_SIZEOFFFR_OK                                 E_OK
#  define DCM_DEMIF_RET_SIZEOFFFR_N_OK                               E_NOT_OK
#  define DCM_DEMIF_RET_SIZEOFFFR_W_DTC                              DEM_WRONG_DTC
#  define DCM_DEMIF_RET_SIZEOFFFR_W_ORIGIN                           DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_SIZEOFFFR_W_RNUM                             DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_SIZEOFFFR_PENDING                            DEM_PENDING

/* Std_ReturnType */
#  define DCM_DEMIF_RET_OCCURR_OK                                    E_OK
#  define DCM_DEMIF_RET_OCCURR_N_OK                                  E_NOT_OK
#  define DCM_DEMIF_RET_OCCURR_NO_SUCH_ELEMENT                       DEM_NO_SUCH_ELEMENT

/* Std_ReturnType */
#  define DCM_DEMIF_RET_FILTERED_OK                                  E_OK
#  define DCM_DEMIF_RET_FILTERED_NO_ITEMS                            DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_FILTERED_PENDING                             DEM_PENDING
#  define DCM_DEMIF_RET_FILTERED_BUFFER_TOO_SMALL                    DEM_BUFFER_TOO_SMALL

/* Std_ReturnType */
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_OK                       E_OK
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTC                DEM_WRONG_DTC
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_WRONG_DTCORIGIN          DEM_WRONG_DTCORIGIN
/* # define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_NOSEVERITY */            /* not used anymore */
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_PENDING                  DEM_PENDING
#  define DCM_DEMIF_RET_GET_SEVERITY_OF_DTC_BUSY                     DEM_BUSY

/* Std_ReturnType */
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_OK                   E_OK
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_PENDING              DEM_PENDING
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTC            DEM_WRONG_DTC
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_WRONG_DTCORIGIN      DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_GET_FUNCTIONALUNITOFDTC_BUSY                 DEM_BUSY

/* Std_ReturnType */
#  define DCM_DEMIF_RET_FILTER_ACCEPTED                              E_OK
#  define DCM_DEMIF_RET_WRONG_FILTER                                 E_NOT_OK

/* Std_ReturnType */
#  define DCM_DEMIF_RET_SELECT_BUSY                                  DEM_BUSY

/* Std_ReturnType */
#  define DCM_DEMIF_RET_SELECT_EXT_DATAREC_OK                        E_OK
#  define DCM_DEMIF_RET_SELECT_EXT_DATAREC_BUSY                      DEM_BUSY

/* Std_ReturnType */
#  define DCM_DEMIF_RET_SELECT_FF_DATA_OK                            E_OK
#  define DCM_DEMIF_RET_SELECT_FF_DATA_BUSY                          DEM_BUSY

/* Std_ReturnType */
#  define DCM_DEMIF_RET_CLEAR_OK                                     E_OK
#  define DCM_DEMIF_RET_CLEAR_PENDING                                DEM_PENDING
#  define DCM_DEMIF_RET_CLEAR_CLEAR_BUSY                             DEM_CLEAR_BUSY
#  define DCM_DEMIF_RET_CLEAR_BUSY                                   DEM_BUSY
#  define DCM_DEMIF_RET_CLEAR_MEMORY_ERROR                           DEM_CLEAR_MEMORY_ERROR
#  define DCM_DEMIF_RET_CLEAR_FAILED                                 DEM_CLEAR_FAILED
#  define DCM_DEMIF_RET_CLEAR_W_DTC                                  DEM_WRONG_DTC
#  define DCM_DEMIF_RET_CLEAR_W_ORIGIN                               DEM_WRONG_DTCORIGIN

/* Std_ReturnType */
#  define DCM_DEMIF_RET_STATUS_OK                                    E_OK
#  define DCM_DEMIF_RET_STATUS_PENDING                               DEM_PENDING
#  define DCM_DEMIF_RET_STATUS_WRONG_DTC                             DEM_WRONG_DTC
#  define DCM_DEMIF_RET_STATUS_WRONG_DTCORIGIN                       DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_STATUS_NO_SUCH_ELEMENT                       DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_STATUS_BUSY                                  DEM_BUSY
/* # define DCM_DEMIF_RET_STATUS_FAILED */                             /* not used anymore */

/* Std_ReturnType */
#  define DCM_DEMIF_RET_NUMBER_OK                                    E_OK
#  define DCM_DEMIF_RET_NUMBER_PENDING                               DEM_PENDING

/* Std_ReturnType */
#  define DCM_DEMIF_RET_CONTROL_DTC_SETTING_OK                       E_OK
#  define DCM_DEMIF_RET_CONTROL_DTC_SETTING_PENDING                  DEM_PENDING

/* Return values for Dem_ReturnGetExtendedDataRecoredByDTCType */
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_OK                          E_OK
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTC                   DEM_WRONG_DTC
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_DTCORIGIN             DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_NUMBER                DEM_NO_SUCH_ELEMENT
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_WRONG_BUFFERSIZE            DEM_BUFFER_TOO_SMALL
#  define DCM_DEMIF_RET_GET_EXT_DATA_REC_PENDING                     DEM_PENDING

#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_OK                        E_OK
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_CONDITION           DEM_E_WRONG_CONDITION
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTC                 DEM_WRONG_DTC
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_WRONG_DTCORIGIN           DEM_WRONG_DTCORIGIN
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_PENDING                   DEM_PENDING
#  define DCM_DEMIF_RET_DISABLE_DTC_REC_UP_BUSY                      DEM_BUSY

#  define DCM_DEMIF_FILTER_FOR_FDC_NO                                FALSE
#  define DCM_DEMIF_FILTER_FOR_FDC_YES                               TRUE
#  define DCM_DEMIF_FILTER_WITH_SEVERITY_NO                          FALSE
#  define DCM_DEMIF_FILTER_WITH_SEVERITY_YES                         TRUE

/* types */
#  define Dcm_DemIfFilterForFDCType                                  boolean
#  define Dcm_DemIfFilterWithSeverityType                            boolean
#  define Dcm_DemIfReturnClearDTCType                                Std_ReturnType
#  define Dcm_DemIfReturnControlDTCSettingType                       Std_ReturnType
#  define Dcm_DemIfReturnDisableDTCRecordUpdateType                  Std_ReturnType
#  define Dcm_DemIfReturnGetDTCByOccurrenceTimeType                  Std_ReturnType
#  define Dcm_DemIfReturnGetFreezeFrameDataByRecordType              Std_ReturnType
#  define Dcm_DemIfReturnGetFunctionalUnitOfDTCType                  Std_ReturnType
#  define Dcm_DemIfReturnGetNextFilteredElementType                  Std_ReturnType
#  define Dcm_DemIfReturnGetNextFilteredItemType                     Std_ReturnType
#  define Dcm_DemIfReturnGetNumberOfFilteredDTCType                  Std_ReturnType
#  define Dcm_DemIfReturnGetSeverityOfDTCType                        Std_ReturnType
#  define Dcm_DemIfReturnGetSizeOfDataByDTCType                      Std_ReturnType
#  define Dcm_DemIfReturnGetSizeOfEDRByDTCType                       Dcm_DemIfReturnGetSizeOfDataByDTCType
#  define Dcm_DemIfReturnGetSizeOfFFRByDTCType                       Dcm_DemIfReturnGetSizeOfDataByDTCType
#  define Dcm_DemIfReturnGetStatusOfDTCType                          Std_ReturnType
#  define Dcm_DemIfReturnSetFilterType                               Std_ReturnType
#  define Dcm_DemIfReturnSelectExtRecordType                         Std_ReturnType
#  define Dcm_DemIfReturnSelectFFDataType                            Std_ReturnType

# endif

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

#endif /* !defined(DCM_DEMIFTYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_DemIfTypes.h
 *********************************************************************************************************************/

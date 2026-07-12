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
/**        \file  Dcm_Svc19.h
 *         \unit  Svc19
 *        \brief  Contains public inline function definitions of Service 0x19 unit.
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
#if !defined(DCM_SVC19_H)
# define DCM_SVC19_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc19Types.h"
# if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_Repeater.h"
#  include "Dcm_DiagTypes.h"
#  include "Dcm_DemIf.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  if (DCM_DEMIF_SVC_19_READ_DTC_EXT_DATA_BY_DTC_NUM_ENABLED == STD_ON)
#   define DCM_SVC_19_UTI_GETEXTRECORD_ENABLED                       STD_ON
#  else
#   define DCM_SVC_19_UTI_GETEXTRECORD_ENABLED                       STD_OFF
#  endif

#  if ((DCM_DEMIF_SVC_19_GETFFRECORD_ENABLED   == STD_ON) || \
      (DCM_SVC_19_UTI_GETEXTRECORD_ENABLED == STD_ON)) && \
      (DCM_DEMIF_421_ENABLED == STD_ON)
#   define DCM_SVC_19_RECORD_ITER_ENABLED                            STD_ON
#  else
#   define DCM_SVC_19_RECORD_ITER_ENABLED                            STD_OFF
#  endif

#  if defined (DCM_UC_SVC_19_PATCH_SUPPORT_ENABLED)
/* take this value */
#  else
#   define DCM_UC_SVC_19_PATCH_SUPPORT_ENABLED                       DCM_SVC_19_PATCH_SUPPORT_ENABLED
#  endif

/*! Service 0x19 processing steps */
#  define DCM_SVC19_PROGRESS_SUBFUNCTION                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
#  define DCM_SVC19_PROGRESS_UTI_COPYLINEARDATA                      ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

#  define DCM_SVC19_PROGRESS_DEMCHAIN_SELECT_DTC                     ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 10u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_DISABLERECORDUPDATE            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 11u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETSTATUSOFDTC                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 12u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_SELECTFFREC                    ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 13u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFFFREC                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 14u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_SELECTEXTDATAREC               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 15u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETSIZEOFEXTDATAREC            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 16u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETNUMFLTRDDTC                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 17u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDSEVERITY           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 18u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETSEVERITYOFDTC               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 19u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETFUNCUNITOFDTC               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 20u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDDTC                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 21u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_PUTNUMFLTRDDTC                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 22u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_GETNEXTFLTRDFDC                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 23u))
#  define DCM_SVC19_PROGRESS_DEMCHAIN_WWHOBD_GETNEXTFLTRDSEVERITY    ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 24u))

#  define DCM_SVC19_PROGRESS_CHAIN_END                               DCM_REPEATER_PROGRESS_USERS_LAST /*!< Service 0x19 final processing step */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Service 0x19 temporary processing context */
struct DCM_19_REPEATERPROXYCONTEXTTYPE_TAG
{
  P2CONST(Dcm_RepeaterProgressType, TYPEDEF, DCM_CONST)  FuncChain;       /*!< Pointer to valid list of processing DEM API list */
#  if (DCM_SVC_19_RECORD_ITER_ENABLED == STD_ON)
  Dcm_Uint8ConstDataPtrType                              RecordNumList;   /*!< Pointer to DEM (extended/freeze-frame) record list */
#  endif
  uint32                                                 Dtc;             /*!< Requested DTC for futher DEM interaction */
  uint16                                                 NumDtc;          /*!< Read number of DTCs reported by DEM to be passed to next processing stage */
  Dem_DTCOriginType                                      Origin;          /*!< Requested DTC origin */
  uint8                                                  SubUpdater;      /*!< Current paged-buffer updater within this diagnostic service */
  uint8                                                  RecordNum;       /*!< Either the requested or next record number to be read out from DEM */
  Dcm_DiagSubServiceRefMemType                           SubSvcRef;       /*!< A validly dispatched sub-function requested for this diagnostic service */
  uint8                                                  DemClientId;     /*!< DEM ClientId */
};
typedef struct DCM_19_REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc19RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc19RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)   Dcm_Svc19RepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc19RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc19RepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_19_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC19_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc19.h
 *********************************************************************************************************************/

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
/**        \file  Dcm_Svc29.h
 *         \unit  Svc29
 *        \brief  Contains public inline function definitions of Service 0x29 unit.
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
#if !defined(DCM_SVC29_H)
# define DCM_SVC29_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc29Types.h"
# if (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_DiagTypes.h"
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Authentication with PKI Certificate Exchange (APCE) handling */
#  if (DCM_SVC_29_01_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
#   define DCM_SVC_29_APCE_INTERNAL                                  STD_ON
#  else
#   define DCM_SVC_29_APCE_INTERNAL                                  STD_OFF
#  endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Service 0x29 temporary processing context */
struct DCM_SVC29REPEATERPROXYCONTEXTTYPE_TAG
{
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
  Dcm_AuthMgrConnContextType    ConnContext;          /*!< Connection specific context */
  KeyM_CertElementIteratorType  CertElementIterator;  /*!< Iteratorator for accessing certificate elements */
  Dcm_CfgNetAuthInfoRefMemType  AuthInfoHdl;          /*!< Reference to Dcm_CfgAuthMgrNetConnInfo[] entry */
#  endif
#  if (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  uint8                         ClientCertIdx;        /*!< Reference to Dcm_CfgAuthMgrTransmitCertificateInfo[] entry */
#  endif
  Dcm_DiagSubServiceRefMemType  SubSvcRef;            /*!< Reference to Dcm_CfgSvc29SubFuncInfo[] entry */
  uint8                         SubSvcId;             /*!< Subfunction id for this context */
};
typedef struct DCM_SVC29REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc29RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc29RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)   Dcm_Svc29RepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc29RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc29RepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrModeSwitchAuthentication()
 *********************************************************************************************************************/
/*! \brief          Mode switching for authentication state change.
 *  \details        -
 *  \param[in]      authInfoRef     The authentication info table reference
 *  \param[in]      mode            The next authentication mode
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_AuthMgrModeSwitchAuthentication(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  uint8 mode
  );
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_29_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC29_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc29.h
 *********************************************************************************************************************/

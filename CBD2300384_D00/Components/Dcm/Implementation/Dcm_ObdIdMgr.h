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
/**        \file  Dcm_ObdIdMgr.h
 *         \unit  ObdIdMgr
 *        \brief  Contains public inline function definitions of OBD ID Manager unit.
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
#if !defined(DCM_OBDIDMGR_H)
# define DCM_OBDIDMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_ObdIdMgrTypes.h"
# if (DCM_OBDIDMGR_SUPPORT_ENABLED == STD_ON) || (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_VarMgr.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Specifies whether at least one diagnostic service needs the request parser utility */
#  if (DCM_SVC_01_SUPPORT_ENABLED == STD_ON) || \
      (DCM_OBDIDMGR_OBDMID_SUPPORT_BY_DCM_ENABLED == STD_ON) || \
      (DCM_SVC_08_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_09_SUPPORT_ENABLED == STD_ON)
#   define DCM_OBDIDMGR_REQ_PARSER_ENABLED                           STD_ON
#  else
#   define DCM_OBDIDMGR_REQ_PARSER_ENABLED                           STD_OFF
#  endif

#  define DCM_OBDIDMGR_MAX_NUMBER_OF_IDS                             (6u) /*!< Maximum number of requested IDs */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* Check if it is an available ID at all: (id MOD 0x20) == 0 */
#  define Dcm_ObdIdMgrIsAvailabilityId(obdId)                        (((obdId) & 0x1Fu) == 0u)                                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Request parser temporary processing context */
struct DCM_OBDIDMGRDESCTYPE_TAG
{
  Dcm_Uint8ConstDataPtrType    LookUpTable;         /*!< IN: The OBD ID lookup table */
  Dcm_VarRefMemPtrType         LookUpFilterTable;   /*!< IN: List of OBD ID related filters */
  Dcm_ObdMaskConstDataPtrType  MaskData;            /*!< IN: The calibrated OBD ID mask */
  Dcm_ObdIdMgrHandleMemType    Handle;              /*!< OUT: ID specific handle */
};
typedef struct DCM_OBDIDMGRDESCTYPE_TAG Dcm_ObdIdMgrDescType;
typedef P2VAR(Dcm_ObdIdMgrDescType, TYPEDEF, DCM_VAR_NOINIT) Dcm_ObdIdMgrDescPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if (DCM_SVC_22_OBD_CALIBRATION_ENABLED == STD_ON) || \
      (DCM_SVC_31_OBD_CALIBRATION_ENABLED == STD_ON) || \
      (DCM_OBDUDSIDMGR_CALIBRATION_ENABLED   == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrIsIdEnabled()
 *********************************************************************************************************************/
/*! \brief          Returns the calibrated state of an OBD ID.
 *  \details        -
 *  \param[in]      obdId              The OBD ID to be verified.
 *  \param[in]      pMaskData    A pointer to the OBD ID availability range mask list
 *  \return         TRUE               OBD ID is still enabled
 *  \return         FALSE              OBD ID was calibrated to be disabled
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_ObdIdMgrIsIdEnabled(
  uint8 obdId,
  P2CONST(Dcm_ObdIdMgrMaskValueTableType, AUTOMATIC, DCM_CONST) pMaskData
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_ObdIdMgrGetAvailabilityMask()
 *********************************************************************************************************************/
/*! \brief          A utility to provide a lookup result of an OBD ID and returning the corresponding AID mask value.
 *  \details        -
 *  \param[in]      id     The OBD ID to look for
 *  \param[in,out]  pParserDesc
 *                  IN     LookUpTable
 *                  IN     LookUpFilterTable
 *                  IN     MaskData
 *                  OUT    Handle
 *  \return         The generated 32bit mask value (variant independent/calibratable)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            The requested id has to be supported and to be an availability one.
 *********************************************************************************************************************/
FUNC(uint32, DCM_CODE) Dcm_ObdIdMgrGetAvailabilityMask(
  uint8 id,
  Dcm_ObdIdMgrDescPtrType pParserDesc
  );

/**********************************************************************************************************************
 *  Dcm_ObdIdMgrIdLookUp()
 *********************************************************************************************************************/
/*! \brief          Look up for a given OBD ID.
 *  \details        Considers any variant handling/calibration.
 *  \param[in]      id     The OBD ID to look for
 *  \param[in,out]  pParserDesc
 *                  IN     LookUpTable
 *                  IN     LookUpFilterTable
 *                  IN     MaskData
 *                  OUT    Handle
 *  \return         DCM_E_OK        Look up was successful
 *  \return         DCM_E_NOT_OK    Look up was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgrIdLookUp(
  uint8 id,
  Dcm_ObdIdMgrDescPtrType pParserDesc
  );

#  if (DCM_OBDIDMGR_REQ_PARSER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ObdIdMgrProcessSupportedIdRequest()
 *********************************************************************************************************************/
/*! \brief          Validates and processes any OBD ID availability ID request.
 *  \details        Processes a list of IDs and in case of availability ID request writes the response data.
 *  \param[in,out]  pMsgContext               Current request context
 *  \param[in]      pParserDesc
 *                  IN     LookUpTable
 *                  IN     LookUpFilterTable
 *                  IN     MaskData
 *                  OUT    Handle
 *  \param[out]     ErrorCode                 The NRC
 *  \return         DCM_E_OK        Request processing was successful
 *  \return         DCM_E_NOT_OK    Request was not valid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ObdIdMgrProcessSupportedIdRequest(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_ObdIdMgrDescPtrType pParserDesc,
  Dcm_NegativeResponseCodePtrType ErrorCode
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

# endif /* (DCM_OBDIDMGR_SUPPORT_ENABLED == STD_ON) || (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_OBDIDMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_ObdIdMgr.h
 *********************************************************************************************************************/

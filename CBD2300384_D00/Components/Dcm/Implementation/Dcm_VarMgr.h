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
/**        \file  Dcm_VarMgr.h
 *         \unit  VarMgr
 *        \brief  Contains public inline function definitions of Variant Manager unit.
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
#if !defined(DCM_VARMGR_H)
# define DCM_VARMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_VarMgrTypes.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Variant reference data type (derived from the execution precondition references since it is part of the pre-conditions and located in the same table) */
typedef P2CONST(Dcm_CfgStateRefMemType, TYPEDEF, DCM_CONST) Dcm_VarRefMemPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrInit()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        Initialization of variant manager sub-module.
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \return         TRUE         Configuration root pointer is valid
 *  \return         FALSE        Configuration root pointer is invalid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_VarMgrInit(
  Dcm_ConfigPtrType configPtr
  );
# endif

/**********************************************************************************************************************
 *  Dcm_VarMgrGetActiveCfgVariantId()
 *********************************************************************************************************************/
/*! \brief          Returns currently set variant.
 *  \details        Returns currently active config variant id.
 *  \return         Currently active config variant id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgVarMgrDiagVariantIdMemType, DCM_CODE) Dcm_VarMgrGetActiveCfgVariantId(void);

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*! \brief          Checks if a diagnostic object is active in at least one variant.
 *  \details        -
 *  \param[in]      varInfoTable    Reference to the variant information table
 *  \return         TRUE        Diagnostic object is active in at least one variant
 *  \return         FALSE       Diagnostic object is not active
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_VarMgrIsEnabledInActiveVariants(
  P2CONST(Dcm_CfgVarMgrBitSetBaseType, TYPEDEF, DCM_CONST) varInfoTable
  );
# endif

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgSetSingle()
 *********************************************************************************************************************/
/*! \brief          Sets the status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[in]      State         DCM_VSG_ENABLED:  Set VSG to active
 *                                DCM_VSG_DISABLED: Set VSG to inactive
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_VarMgrVsgSetSingle(
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  );
# endif

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgIsActive()
 *********************************************************************************************************************/
/*! \brief          Returns status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[out]     State         DCM_VSG_ENABLED:  VSG is active
 *                                DCM_VSG_DISABLED: VSG is inactive
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_VarMgrVsgIsActive(
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_VARMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_VarMgr.h
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Wdg_30_TLE4278G.c
 *        \brief  Implementation of module Wdg_30_TLE4278G.c
 *
 *      \details  This file contains the implementation of module Wdg_30_TLE4278G
 *
 *********************************************************************************************************************/

#define WDG_30_TLE4278G_SOURCE

/**********************************************************************************************************************
 * INCLUDES
 ********************************************************************************************************************/
#include "Wdg_30_TLE4278G.h"
#include "Wdg_30_TLE4278G_Cbk.h"
#include "Wdg_30_TLE4278G_Types.h"

#if (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON)
#include "Det.h"
#endif /* (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON) */

/**********************************************************************************************************************
 * INCLUDES OF SUBMODULES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Timer.h"
#include "Wdg_30_TLE4278G_Mode.h"
#include "Wdg_30_TLE4278G_TrgCnd.h"
#include "Wdg_30_TLE4278G_LL.h"

/**********************************************************************************************************************
 * VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Wdg_30_TLE4278G header file */
#if (  (WDG_30_TLE4278G_SW_MAJOR_VERSION != (4u)) \
    || (WDG_30_TLE4278G_SW_MINOR_VERSION != (2u)))
#error "Version numbers of Wdg_30_TLE4278G.c and Wdg_30_TLE4278G.h are inconsistent!"
#endif

/* Check the compatibility of generated data */
#if (  (WDG_30_TLE4278G_CFG_COMP_MAJOR_VERSION != (4u)) \
    || (WDG_30_TLE4278G_CFG_COMP_MINOR_VERSION != (3u)) )
# error "Version numbers of Wdg_30_TLE4278G.c and Wdg_30_TLE4278G_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (WDG_30_TLE4278G_LOCAL) /* COV_WDG_30_TLE4278G_COMPATIBILITY */
# define WDG_30_TLE4278G_LOCAL static
#endif

#if !defined (WDG_30_TLE4278G_LOCAL_INLINE) /* COV_WDG_30_TLE4278G_COMPATIBILITY */
# define WDG_30_TLE4278G_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 * LOCAL VARIABLES
 *********************************************************************************************************************/

#if (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)

#define WDG_30_TLE4278G_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Stores the module state */
WDG_30_TLE4278G_LOCAL VAR(Wdg_30_TLE4278G_StateType, WDG_30_TLE4278G_VAR_INIT) Wdg_30_TLE4278G_moduleState = WDG_30_TLE4278G_UNINIT;

#define WDG_30_TLE4278G_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */

#define WDG_30_TLE4278G_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Stores the config pointer */
P2CONST(Wdg_30_TLE4278G_ConfigType, WDG_30_TLE4278G_VAR_NOINIT, WDG_30_TLE4278G_CONST) Wdg_30_TLE4278G_ConfigPtr;

#define WDG_30_TLE4278G_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * LOCAL MACROS
 *********************************************************************************************************************/
#if (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)
/* PRQA S 3453 2 */ /* MD_MSR_FctLikeMacro */
# define Wdg_30_TLE4278G_SetDriverState(Mode) Wdg_30_TLE4278G_moduleState = (Mode)
# define Wdg_30_TLE4278G_CheckDriverState(Mode) ((boolean)(Wdg_30_TLE4278G_moduleState == ((Wdg_30_TLE4278G_StateType)Mode)))
#else
# define Wdg_30_TLE4278G_SetDriverState(Mode)
# define Wdg_30_TLE4278G_CheckDriverState(Mode)
#endif
/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Init()
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
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Init(
  P2CONST(Wdg_30_TLE4278G_ConfigType, AUTOMATIC, WDG_30_TLE4278G_CONST) ConfigPtr
)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = WDG_30_TLE4278G_E_NO_ERROR; /* PRQA S 3205 */ /* MD_Wdg_30_TLE4278G_3205 */
/* ----- Development Error Checks ------------------------------------- */
#if(WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that ConfigPtr parameter is not NULL_PTR */
  if(NULL_PTR == ConfigPtr)
  {
    errorId = WDG_30_TLE4278G_E_PARAM_POINTER;
  }
  /* #20 Check if default mode is supported */
  else if (Wdg_30_TLE4278G_Mode_IsModeAllowed(ConfigPtr->DefaultMode) == FALSE)  
  {  /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    errorId = WDG_30_TLE4278G_E_PARAM_CONFIG;
  }
  else
#endif /* WDG_30_TLE4278G_DEV_ERROR_DETECT */
  /* DEM check */
  /* #30 Report EDisableRejected to DEM if default mode is WDGIF_OFF_MODE and WDG_30_TLE4278G_DISALE_ALLOWED is disabled */
#if(WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_OFF)
  if (ConfigPtr->DefaultMode == WDGIF_OFF_MODE)
  {
    Dem_ReportErrorStatus(ConfigPtr->EDisableRejected, DEM_EVENT_STATUS_FAILED);
  }
  else
#endif /* (WDG_30_TLE4278G_DISABLE_ALLOWED == STD_OFF) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Set global ConfigPtr */
    Wdg_30_TLE4278G_ConfigPtr = ConfigPtr;

    /* #45 Initialize sub-component Wdg_30_TLE4278G_Timer */
    if(Wdg_30_TLE4278G_Timer_Init() == E_OK)
    {

      /* #50 If initializon of sub-component Wdg_30_TLE4278G_Timer was successful, set watchdog default */
      if(Wdg_30_TLE4278G_Mode_SetMode(Wdg_30_TLE4278G_ConfigPtr->DefaultMode) == E_OK)
      {
        Wdg_30_TLE4278G_SetDriverState(WDG_30_TLE4278G_IDLE);
      }

    }
    else
    {
      /* #60 Otherwise report EModeFailed to DEM */
      Dem_ReportErrorStatus(ConfigPtr->EModeFailed, DEM_EVENT_STATUS_FAILED);
    }

  }


#if (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON) 
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != WDG_30_TLE4278G_E_NO_ERROR)
  {
    (void)Det_ReportError(WDG_30_TLE4278G_MODULE_ID, WDG_30_TLE4278G_INSTANCE_ID, WDG_30_TLE4278G_SID_INIT, errorId);
  }
#else 
  WDG_30_TLE4278G_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif /* (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON) */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Cbk_GptNotificationTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Cbk_GptNotificationTrigger(void)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = WDG_30_TLE4278G_E_NO_ERROR; /* PRQA S 3205 */ /* MD_Wdg_30_TLE4278G_3205 */

/* ----- Development Error Checks ------------------------------------- */
#if (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check driver state of the component */
  if (Wdg_30_TLE4278G_CheckDriverState(WDG_30_TLE4278G_IDLE) == FALSE)
  {
    errorId = WDG_30_TLE4278G_E_DRIVER_STATE;
  }
  else
#endif
  {
  /* ----- Implementation ----------------------------------------------- */
    Wdg_30_TLE4278G_SetDriverState(WDG_30_TLE4278G_BUSY);

    /* #20 Forward call to sub-component Wdg_30_TLE4278G_Timer */
    Wdg_30_TLE4278G_Timer_TriggerCbk();


    Wdg_30_TLE4278G_SetDriverState(WDG_30_TLE4278G_IDLE);
  }
#if (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON)
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != WDG_30_TLE4278G_E_NO_ERROR)
  {
    (void)Det_ReportError(WDG_30_TLE4278G_MODULE_ID, WDG_30_TLE4278G_INSTANCE_ID, WDG_30_TLE4278G_SID_CBK_GPT_TRIGGER, errorId);
  }
#else 
  WDG_30_TLE4278G_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_SetTriggerCondition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_SetTriggerCondition(uint16 timeout)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = WDG_30_TLE4278G_E_NO_ERROR; /* PRQA S 3205 */ /* MD_Wdg_30_TLE4278G_3205 */

  /* ----- Development Error Checks ------------------------------------- */
#if (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)  
  /* #10 Check if value of given parameter timeout does not exceed the maximum allowed value */
  if (WDG_30_TLE4278G_MAX_TIMEOUT < timeout)
  {
    errorId = WDG_30_TLE4278G_E_PARAM_TIMEOUT;
  }
  else
#endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */
  {
  /* ----- Implementation ------------------Y----------------------------- */

    /* #20 Set trigger condition by calling Wdg_30_TLE4278G_TrgCnd_setCondition */
    Wdg_30_TLE4278G_TrgCnd_SetCondition(timeout, Wdg_30_TLE4278G_Mode_GetMode());

  }
#if (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON)
  /* ----- Development Error Report --------------------------------------- */
  if (errorId != WDG_30_TLE4278G_E_NO_ERROR)
  {
    (void)Det_ReportError(WDG_30_TLE4278G_MODULE_ID, WDG_30_TLE4278G_INSTANCE_ID, WDG_30_TLE4278G_SID_SET_TRIGGER_CONDITION, errorId);
  }
#else 
  WDG_30_TLE4278G_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */
}

/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_SetMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_SetMode(WdgIf_ModeType Mode)
{
  uint8 errorId = WDG_30_TLE4278G_E_NO_ERROR; /* PRQA S 3205 */ /* MD_Wdg_30_TLE4278G_3205 */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
/* ----- Development Error Checks ------------------------------------- */
#if (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)

  /* #10 Check if component state is valid */
  if (Wdg_30_TLE4278G_CheckDriverState(WDG_30_TLE4278G_IDLE) == FALSE)
  {
    errorId = WDG_30_TLE4278G_E_DRIVER_STATE;
  }
  /* #20 Check if passed parameter mode is supported */
  else if (Wdg_30_TLE4278G_Mode_IsModeAllowed(Mode) == FALSE)
  {  /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    errorId = WDG_30_TLE4278G_E_PARAM_MODE;
  }
  else
#endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */

  /* #30 Report EDisableRejected to DEM if default mode is WDGIF_OFF_MODE and WDG_30_TLE4278G_DISALE_ALLOWED is disabled */
#if(WDG_30_TLE4278G_IS_MODE_SUPPORTED(WDG_30_TLE4278G_MODE_OFF) && WDG_30_TLE4278G_DISABLE_ALLOWED == STD_OFF)
  if(Mode == WDGIF_OFF_MODE)
  {
    Dem_ReportErrorStatus(Wdg_30_TLE4278G_ConfigPtr->EDisableRejected, DEM_EVENT_STATUS_FAILED);
  }
  else
#endif /* (WDG_30_TLE4278G_DISABLE_ALLOWED == STD_ON) */
  /* End DEM Check */
  /* ----- Implementation ----------------------------------------------- */
  {
    Wdg_30_TLE4278G_SetDriverState(WDG_30_TLE4278G_BUSY);
    
    /* #40 Initiate mode switch by calling Wdg_30_TLE4278G_Mode_setMode */
    retVal = Wdg_30_TLE4278G_Mode_SetMode(Mode);

    Wdg_30_TLE4278G_SetDriverState(WDG_30_TLE4278G_IDLE);

  }
#if(WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON)
  /* ----- Development Error Report --------------------------------------- */
  if (errorId != WDG_30_TLE4278G_E_NO_ERROR)
  {
    (void)Det_ReportError(WDG_30_TLE4278G_MODULE_ID, WDG_30_TLE4278G_INSTANCE_ID, WDG_30_TLE4278G_SID_SET_MODE, errorId);
  }
#else 
  WDG_30_TLE4278G_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif /* (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON) */
  return retVal;
}

#if (STD_ON == WDG_30_TLE4278G_VERSION_INFO_API)
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, WDG_30_TLE4278G_APPL_DATA) versioninfo
)
{
/* ----- Local Variables -----------------------------------------------------------------------------*/
  uint8 errorId = WDG_30_TLE4278G_E_NO_ERROR; /* PRQA S 3205 */ /* MD_Wdg_30_TLE4278G_3205 */
/* ----- Development Error Checks ------------------------------------- */
# if(WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check passed parameter for NULL pointer */
  if(versioninfo == NULL_PTR)
  {
    errorId = WDG_30_TLE4278G_E_PARAM_POINTER;
  }
  else
# endif /* (WDG_30_TLE4278G_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #20 Return version information */
    versioninfo->vendorID = (WDG_30_TLE4278G_VENDOR_ID); /* SBSW_WDG_30_TLE4278G_01 */
    versioninfo->moduleID = (WDG_30_TLE4278G_MODULE_ID); /* SBSW_WDG_30_TLE4278G_01 */
# if (WDG_30_TLE4278G_ASR3X_COMPATIBILITY == STD_ON) 
    versioninfo->instanceID = 0; /* SBSW_WDG_30_TLE4278G_01 */
# endif
    versioninfo->sw_major_version = (WDG_30_TLE4278G_SW_MAJOR_VERSION); /* SBSW_WDG_30_TLE4278G_01 */
    versioninfo->sw_minor_version = (WDG_30_TLE4278G_SW_MINOR_VERSION); /* SBSW_WDG_30_TLE4278G_01 */
    versioninfo->sw_patch_version = (WDG_30_TLE4278G_SW_PATCH_VERSION); /* SBSW_WDG_30_TLE4278G_01 */
  }
/* ----- Development Error Report --------------------------------------- */
# if(WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON)
  if(errorId != WDG_30_TLE4278G_E_NO_ERROR)
  {
    (void)Det_ReportError(WDG_30_TLE4278G_MODULE_ID, WDG_30_TLE4278G_INSTANCE_ID, WDG_30_TLE4278G_SID_GET_VERSION_INFO, errorId);
  }
# else
  WDG_30_TLE4278G_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* (WDG_30_TLE4278G_DEV_ERROR_REPORT == STD_ON) */
}
#endif /* (STD_ON == WDG_30_TLE4278G_VERSION_INFO_API) */


#if(STD_ON == WDG_30_TLE4278G_ASR3X_COMPATIBILITY)
/**********************************************************************************************************************
 *  Wdg_30_TLE4278G_Trigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Trigger(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call Wdg_30_TLE4278G_SetTriggerCondition with configured timeout */
  Wdg_30_TLE4278G_SetTriggerCondition(WDG_30_TLE4278G_ASR3X_TIMEOUT);
}

#endif

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 MD_Wdg_30_TLE4278G_3205:
 MISRA 2012 rule 2.2
 Reason:      The identifier 'errorId' is not used and could be removed.
 Risk:        There's no risk.
 Prevention:  There are pre-processed variants in which the 'errorId' is used for error detection. Therefore
              the assignment is necessary.
*/

/* SBSW_JUSTIFICATION_BEGIN
   \ID SBSW_WDG_30_TLE4278G_01
    \DESCRIPTION The function Wdg_30_TLE4278G_GetVersionInfo writes to the object referenced by parameter versioninfo.
                 Additionally, versioninfo is checked if it's valid.
    \COUNTERMEASURE \N The caller ensures that the pointers passed to the parameters versioninfo is valid.
SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_WDG_30_TLE4278G_COMPATIBILITY
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]

END_COVERAGE_JUSTIFICATION */ 


/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G.c
 *********************************************************************************************************************/

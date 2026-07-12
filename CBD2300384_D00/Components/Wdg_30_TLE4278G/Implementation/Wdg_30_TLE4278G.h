/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Wdg_30_TLE4278G.h
 *        \brief  Wdg_30_TLE4278G header file
 *
 *      \details  This component is a watchdog driver which can be configured to support common external DIO watchdog
 *                devices (Infineon TLE4278, Infineon TLE7368, SII S19400 and compatible devices).
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  2.00.00   2015-01-22  virmid  -             Initial version of Wdg_30_TLE4278G (ASR 4.0.3)
 *  2.00.01   2015-05-29  virmid ESCAN00083201  SetMode accepts requests of current mode
 *  2.01.00   2015-06-09  virmid ESCAN00083141  Readded support for TLE4278G / general DIO toggled Wdgs
 *  2.01.01   2015-09-01  virmid ESCAN00084928  Write correct version info in ASR3 compatibility mode
 *  2.01.02   2015-09-16  virmid ESCAN00085282  Renamed MemMep section makers
 *                               ESCAN00085286  Removed DET check (replaced by GenTool validation)
 *  2.01.03   2016-02-22  virmid  -             Updated version check due to breaking change (R14) in GenTool
 *  3.00.00   2016-09-19  virmid  -             Development according to SafeBSW process, support of S19400
 *  4.00.00   2017-04-04  virmid ESCAN00094613  Generic support of IO-based external watchdog peripherals
 *  4.01.00   2019-04-07  virmcn CTM-502        MISRA 2012 Compliance
 *  4.02.00   2021-10-04  virwsl CTM-3276       Update to new core (3.04.01).
 *********************************************************************************************************************/

#if !(defined WDG_30_TLE4278G_H)
#define WDG_30_TLE4278G_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Wdg_30_TLE4278G_Cfg.h"
#include "SchM_Wdg_30_TLE4278G.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- vendor, module and instance identification ----- */
# define WDG_30_TLE4278G_VENDOR_ID                           (30u)
# define WDG_30_TLE4278G_MODULE_ID                           (0x66u)
# define WDG_30_TLE4278G_INSTANCE_ID                         (0u)

/* ----- AUTOSAR Software specification version information ----- */
# define WDG_30_TLE4278G_AR_RELEASE_MAJOR_VERSION            (0x04u)
# define WDG_30_TLE4278G_AR_RELEASE_MINOR_VERSION            (0x00u)
# define WDG_30_TLE4278G_AR_RELEASE_REVISION_VERSION         (0x03u)

/* ----- Software version information ----- */
# define WDG_30_TLE4278G_SW_MAJOR_VERSION                    (4u)
# define WDG_30_TLE4278G_SW_MINOR_VERSION                    (2u)
# define WDG_30_TLE4278G_SW_PATCH_VERSION                    (0u)

/* ----- API service IDs ----- */
# define WDG_30_TLE4278G_SID_INIT                            (0x00u)
# define WDG_30_TLE4278G_SID_SET_MODE                        (0x01u)
# define WDG_30_TLE4278G_SID_SET_TRIGGER_CONDITION           (0x03u)
# define WDG_30_TLE4278G_SID_GET_VERSION_INFO                (0x04u)
# define WDG_30_TLE4278G_SID_CBK_GPT_TRIGGER                 (0x05u)
# define WDG_30_TLE4278G_SID_TRIGGER                         (0x07u)

/* ----- Error codes ----- */
#define WDG_30_TLE4278G_E_NO_ERROR                           (0x00u)
#define WDG_30_TLE4278G_E_DRIVER_STATE                       (0x10u)
#define WDG_30_TLE4278G_E_PARAM_MODE                         (0x11u)
#define WDG_30_TLE4278G_E_PARAM_CONFIG                       (0x12u)
#define WDG_30_TLE4278G_E_PARAM_TIMEOUT                      (0x13u)
#define WDG_30_TLE4278G_E_PARAM_POINTER                      (0x14u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! critical section macros */
#if (STD_OFF == WDG_30_TLE4278G_ASR3X_COMPATIBILITY)
/*! ASR4 project/ environment */
# define Wdg_30_TLE4278G_EnterCritical(Section)              SchM_Enter_Wdg_30_TLE4278G_##Section() /* PRQA S 0342 */ /* MD_Wdg_30_TLE4278G_342 */
# define Wdg_30_TLE4278G_ExitCritical(Section)               SchM_Exit_Wdg_30_TLE4278G_##Section() /* PRQA S 0342 */ /* MD_Wdg_30_TLE4278G_342 */
#else
/*! ASR3 project/ environment */
# define Wdg_30_TLE4278G_EnterCritical(Section)              SchM_Enter_Wdg_30_TLE4278G(Section) /* PRQA S 3453 */ /* MD_MSR_MemMap */
# define Wdg_30_TLE4278G_ExitCritical(Section)               SchM_Exit_Wdg_30_TLE4278G(Section) /* PRQA S 3453 */ /* MD_MSR_MemMap */
#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define WDG_30_TLE4278G_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \param[in]   ConfigPtr   Configuration structure for initializing the module.
 *  \pre         The module is uninitialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This function initializes the module Wdg_30_TLE4278G. It initializes all variables and sets the module state to
 *               initialized.
 *  \trace       CREQ-105559, DSGN-Wdg22909
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Init(P2CONST(Wdg_30_TLE4278G_ConfigType, AUTOMATIC, WDG_30_TLE4278G_CONST) ConfigPtr);

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_SetTriggerCondition()
 *********************************************************************************************************************/
/*! \brief       Sets the watchdog timeout
 *  \param[in]   timeout     Duration of timeout period in milliseconds.
 *  \pre         The module is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This API is used by WdgIf to set the timeout period in which the watchdog driver is allowed to trigger
 *               the watchdog hardware.
 *               In case the value 0 is passed as timeout parameter the module will cause a reset as soon as possible.
 *  \trace       CREQ-105561, DSGN-Wdg22900
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_SetTriggerCondition(uint16 timeout);

/**********************************************************************************************************************
 * Wdg_30_TLE4278G_SetMode()
 *********************************************************************************************************************/
/*! \brief       Changes the mode of the watchdog module / hardware.
 *  \param[in]   Mode     Mode to which the module / hardware should be set.
 *  \return      E_OK if mode switch executed successfully
 *               E_NOT_OK if mode switch failed
 *  \pre         The module is initialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This API is used by WdgIf to set the mode of the watchdog to a given mode.
 *  \note        In case a the mode change of the hardware fails the internal mode remains unchanged and the mode of
 *               the hardware is undefined.
 *  \trace       CREQ-105560, CREQ-106194, CREQ-106195, CREQ-106196, DSGN-Wdg22902, DSGN-Wdg22983
 *********************************************************************************************************************/
FUNC(Std_ReturnType, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_SetMode(WdgIf_ModeType Mode);

# if (WDG_30_TLE4278G_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * Wdg_30_TLE4278G_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information of this module.
 *  \param[out]  versioninfo    Pointer to where to store the version information.
 *  \pre         The module is initialized.
 *  \config      WDG_30_TLE4278G_VERSION_INFO_API == STD_ON
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \details     This API returns version information, vendor ID and AUTOSAR module ID of the component. \n
 *               If ASR3 compatibility is enabled for this module also the instance id is returned. \n
 *               The values are decimal-coded. \n
 *               If the given output parameter is a NULL_PTR the API throws DET WDG_30_TLE4278G_E_PARAM_VERSIONINFO.
 *  \trace       CREQ-105608
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, WDG_30_TLE4278G_APPL_DATA) versioninfo);
# endif

# if (WDG_30_TLE4278G_ASR3X_COMPATIBILITY == STD_ON)
/**********************************************************************************************************************
 * Wdg_30_TLE4278G_Trigger()
 *********************************************************************************************************************/
/*! \brief       Triggers the watchdog in ASR3 watchdog stacks.
 *  \pre         ASR3 compatibility must be enabled.
 *  \config      WDG_30_TLE4278G_ASR3X_COMPATIBILITY == STD_ON
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \details     This API provides a wrapper-functionality that allows this module to be used in ASR3-conform watchdog
 *               stacks. It has to be called cyclically from WdgIf to reset the trigger condition with a configured
 *               timeout.
 *  \trace       DSGN-Wdg22984
 *********************************************************************************************************************/
FUNC(void, WDG_30_TLE4278G_CODE) Wdg_30_TLE4278G_Trigger(void);
# endif

#define WDG_30_TLE4278G_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
*\exclusivearea WDG_30_TLE4278G_EXCLUSIVE_AREA_0
* Ensures consistency during mode change
* \protects Wdg_30_TLE4278G_Mode_currentMode, Wdg_30_TLE4278G_Timer_permissionCounter, mode consistency
* \usedin Wdg_30_TLE4278G_Mode_SetMode
* \exclude Wdg_30_TLE4278G_Mode_DoModeChange
* \length SHORT Hardware mode is changed, Timer is reconfigured.
*\endexclusivearea

*\exclusivearea WDG_30_TLE4278G_EXCLUSIVE_AREA_1
* Ensures consistency during modification of trigger condition
* \protects Wdg_30_TLE4278G_TrgCnd_ConditionFlag, Wdg_30_TLE4278G_TrgCnd_ExpiredFlag
* \usedin Wdg_30_TLE4278G_TrgCnd_SetCondition, Wdg_30_TLE4278G_TrgCnd_ResetCondition, Wdg_30_TLE4278G_TrgCnd_InvalidateCondition
* \exclude All functions provided by sub-component Wdg_30_TLE4278G_TrgCnd
* \length SHORT Variables are set
*\endexclusivearea
*/

/* Justification for module-specific MISRA deviations:
 MD_Wdg_30_TLE4278G_342:
 Misra Rule 19.13
 Reason: Glue operator used to abstract AUTOSAR SchM access.
 Risk: Compiler does not support the glue operator.
 Prevention: Module supports only ANSI Compiler. Integration tests verify that the code compiles.
*/

#endif /* WDG_30_TLE4278G_H */
/**********************************************************************************************************************
 *  END OF FILE: Wdg_30_TLE4278G.h
 *********************************************************************************************************************/

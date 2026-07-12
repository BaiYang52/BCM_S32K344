/***********************************************************************************************************************
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
/*!        \file  Arti.c
 *        \brief  Arti source file
 *      \details  -
 *
 *  \unit General
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#define ARTI_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Arti.h"

#if (ARTI_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of Arti header file */
#if (  (ARTI_SW_MAJOR_VERSION != (1u)) \
    || (ARTI_SW_MINOR_VERSION != (1u)) \
    || (ARTI_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Arti.c and Arti.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ARTI_CFG_MAJOR_VERSION != (1u)) \
    || (ARTI_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of Arti.c and Arti_Cfg.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if !defined (ARTI_LOCAL)
# define ARTI_LOCAL                                                    static
#endif

#if !defined (ARTI_LOCAL_INLINE)
# define ARTI_LOCAL_INLINE                                             LOCAL_INLINE
#endif

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define ARTI_START_SEC_CODE
#include "Arti_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Arti_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ARTI_CODE) Arti_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ARTI_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ARTI_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ARTI_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    errorId = ARTI_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID = (ARTI_VENDOR_ID);
    VersionInfo->moduleID = (ARTI_MODULE_ID);
    VersionInfo->sw_major_version = (ARTI_SW_MAJOR_VERSION);
    VersionInfo->sw_minor_version = (ARTI_SW_MINOR_VERSION);
    VersionInfo->sw_patch_version = (ARTI_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ARTI_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ARTI_E_NO_ERROR)
  {
    (void)Det_ReportError(ARTI_MODULE_ID, ARTI_INSTANCE_ID_DET, ARTI_SID_GET_VERSION_INFO, errorId);
  }
#else
  (void) errorId; /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#endif
}

#define ARTI_STOP_SEC_CODE
#include "Arti_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* VCA_JUSTIFICATION_BEGIN

\ID VCA_ARTI_TraceVariableIndex
 \DESCRIPTION The macros in the generated file Arti_Lcfg.h may contain code that accesses variables via an array index.
              As the range of the index is not checked, an array-out-of-bounds write access may occur.
              A runtime range check is not used as the affected code parts may be called in performance-critical sections.
              Mind that the VCA is not able to analyze macros which are only called by other components. Thus, this justification
              is not referenced anywhere in the code, because the VCA is not able to detect that it is needed.
 \COUNTERMEASURE \S SMI-978292

VCA_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  END OF FILE: Arti.c
 **********************************************************************************************************************/

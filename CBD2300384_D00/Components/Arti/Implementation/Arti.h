/***********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Arti.h
 *        \brief  Arti header file
 *      \details  -
 *
 * \unit General
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2022-05-25  visdqk  -             Initial version.
 *  01.01.00  2022-08-05  virleh  OSC-8924      SafeBSW release.
 **********************************************************************************************************************/

#if !defined (ARTI_H)
# define ARTI_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "Std_Types.h"
# include "Arti_Cfg.h"
# include "Arti_Lcfg.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/* Vendor and module identification */
# define ARTI_VENDOR_ID                           (30u)
# define ARTI_MODULE_ID                           (0x05u)

/* AUTOSAR Software specification version information (based on AUTOSAR internal versioning scheme, e.g. 4.5.0) */
# define ARTI_AR_RELEASE_MAJOR_VERSION            (4u)
# define ARTI_AR_RELEASE_MINOR_VERSION            (6u)
# define ARTI_AR_RELEASE_REVISION_VERSION         (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define ARTI_SW_MAJOR_VERSION                    (1u)
# define ARTI_SW_MINOR_VERSION                    (1u)
# define ARTI_SW_PATCH_VERSION                    (0u)

# define ARTI_INSTANCE_ID_DET                     (0x00u)

/* ----- API service IDs ----- */
# define ARTI_SID_GET_VERSION_INFO                (0x10u) /*!< Service ID: Mip_GetVersionInfo() */

/* ----- Error codes ----- */
# define ARTI_E_NO_ERROR                          (0x00u) /*!< No error */
# define ARTI_E_PARAM_POINTER                     (0x0Cu) /*!< API used with invalid pointer parameter (NULL) */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
# define ARTI_START_SEC_CODE
# include "Arti_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Arti_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Arti_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfo     Pointer to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, ARTI_CODE) Arti_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, ARTI_APPL_VAR) VersionInfo);

# define ARTI_STOP_SEC_CODE
# include "Arti_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



#endif /* ARTI_H */

/***********************************************************************************************************************
 *  END OF FILE: Arti.h
 **********************************************************************************************************************/

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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E.h
 *        \brief  E2E header file
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2015-06-23  virgmi  ESCAN00083544  SafeBSW Implementation
 *            2015-07-02  virgmi  ESCAN00083770  AUTOSAR deviations were solved
 *  1.01.00   2015-10-20  virgmi  ESCAN00085972  FEAT-1610: Support for JLR E2E profile
 *  1.01.01   2016-03-11  virgmi  -              Updates due to safety process
 *  1.02.00   2016-11-25  virgmi  FEAT-2252      Added E2E profile 7
 *  2.00.00   2018-01-15  virgmi  STORY-3626     E2E Library according to ASR 4.2.2
 *  2.00.01   2018-16-08  viroto  ESCAN00100148  Too early acceptance of E2E protected message.
 *  2.00.02   2018-10-29  vircre  ESCAN00100867  E2E P01 does not recognize ReceiveCounter >= 15 as erroneous situation.
 *  2.01.00   2019-05-13  virsrl  STORY-6175     MISRA-C:2012 compliance.
 *  2.02.00   2019-08-16  virsrl  MWDG-99        E2E shall provide the JLR safety signal implementation according to
 *                                               specification version 6.0.0.
 *  2.03.00   2019-10-16  virsrl  MWDG-1925      E2ELib shall support AUTOSAR 4.3.0 profile 11
 *  2.04.00   2021-02-18  visso   RTE-4007       Rework TechRef according to templates and remove full name from sources
 **********************************************************************************************************************/

#if !defined (E2E_H)
# define E2E_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "E2E_SM.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define E2E_VENDOR_ID                     (30u)
# define E2E_MODULE_ID                     (207u)

# define E2E_AR_RELEASE_MAJOR_VERSION      (4u)
# define E2E_AR_RELEASE_MINOR_VERSION      (2u)
# define E2E_AR_RELEASE_REVISION_VERSION   (2u)

# define E2E_SW_MAJOR_VERSION              (2u)
# define E2E_SW_MINOR_VERSION              (3u)
# define E2E_SW_PATCH_VERSION              (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

typedef P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_BytePtrType;

# define E2E_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * \brief       Retrieves the version information of the E2E module.
 * \details     Stores version information, i.e. Module Id, Vendor Id, Vendor specific version numbers to structure pointed by VersionInfo
 * \param[in]   VersionInfo
 * \pre         -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 * \trace CREQ-1090
 */
FUNC(void, E2E_CODE) E2E_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, E2E_APPL_DATA) VersionInfo);


# define E2E_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E.h
 *********************************************************************************************************************/

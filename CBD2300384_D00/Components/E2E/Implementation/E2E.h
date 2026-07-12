/**********************************************************************************************************************
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
 *  2.03.01   2020-03-13  virsrl  ESCAN00105816  Compiler error for undefined symbol E2E_DATA.
 *  3.00.00   2020-11-05  virsrl  MWDG-4079      Implement profile P44.
 *  4.00.00   2021-07-22  virljs  MWDG-5455      Support E2E Profile 8
 *  4.00.01   2021-08-30  virljs  ESCAN00109888  Compiler error: declaration may not appear after executable statement in block
 *  5.00.00   2021-10-08  vireno  MWDG-5527      Support Profile 22
 *  6.00.00   2022-01-24  virbka  MWDG-5808      Generate E2E_MemMap.h
 *            2022-01-27  vireno  MWDG-6482      Update E2ELib to new window size attributes.
 *            2022-02-14  vireno  MWDG-6638      Adapt E2ELib to the fixed E2EProtocol R19-11 Specification
 *            2022-02-17  vireno  MWDG-5650      E2E shall implement the SM according to ASR20-11
 *  6.00.01   2022-03-21  vireno  ESCAN00111416  E2ELib causes the compiler error "'>' signed/unsigned mismatch"
 *  6.00.02   2022-08-02  vireno  ESCAN00112124  Profile 22: At most 16 errors before OK is returned
 *            2022-08-04  vireno  MWDG-6787      Finite Execution Time Analysis on E2ELib
 *  6.00.03   2022-09-05  vireno  ESCAN00112661  Compiler error: Too many identifiers
 **********************************************************************************************************************/

#if !defined (E2E_H)
# define E2E_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Crc.h"
# include "E2E_SM.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define E2E_VENDOR_ID                     (30u)
# define E2E_MODULE_ID                     (207u)

# define E2E_AR_RELEASE_MAJOR_VERSION      (4u)
# define E2E_AR_RELEASE_MINOR_VERSION      (2u)
# define E2E_AR_RELEASE_REVISION_VERSION   (2u)

# define E2E_SW_MAJOR_VERSION              (6u)
# define E2E_SW_MINOR_VERSION              (0u)
# define E2E_SW_PATCH_VERSION              (3u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

typedef P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_BytePtrType;

# define E2E_START_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

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
# include "E2E_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E.h
 *********************************************************************************************************************/

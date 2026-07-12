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
/*!        \file  wdgm_verifier.h
 *        \brief  wdgm_verifier header file
 *
 *      \details  This is the header file of WdgMVerifier
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2016-07-27  virchl                First Verifier version of migrated WdgM
 *  02.01.00  2017-03-30  virchl                Support status reporting via mode switch ports
 *  02.02.00  2018-07-27  virchl  STORYC-5087   WdgM Verifier is now able to be fully build-able through customer
 *  03.00.00  2020-11-30  virbka  MWDG-4234     WdM: Remove Initial Checkpoint ID Verifier Test
 *  03.00.01  2021-08-24  virsrl  ESCAN00107964 Verifier test case 106 fails when trigger mode configuration per
 *                                              core differ
 *  03.00.02  2021-12-08  virsrl  ESCAN00110497 WdM Verifier test 110 may not check all local transitions or even 
 *                                              leads to verifier error
 *********************************************************************************************************************/

#ifndef WDGM_VERIFIER_H
#define WDGM_VERIFIER_H

#include "wdgm_verifier_types.h"
#include "wdgm_verifier_version.h"

int verify(const WdgM_ConfigType * main_struct, const verifier_main_struct_t * verifier_info);

void print_WdgMVerifier_version(void);


#endif  /* WDGM_VERIFIER_H */


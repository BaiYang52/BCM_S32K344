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
/**        \file  
 *        \brief  String Header for RTE Analyzer
 *
 *      \details  This header provides the string.h prototypes
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-07-31  visso                 Initial creation
 *  01.01.00  2021-02-18  visso   RTE-4007      Rework TechRef according to templates and remove full name from sources
 *********************************************************************************************************************/
#ifndef _STRING_H
# define _STRING_H

typedef unsigned int size_t;

void * memset ( void * ptr, int value, size_t num );
void * memcpy ( void * destination, const void * source, size_t num );


#endif

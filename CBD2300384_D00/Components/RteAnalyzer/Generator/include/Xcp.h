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
 *        \brief  Xcp Header for RTE Analyzer
 *
 *      \details  This header provides the prototypes of the XCP module
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-07-31  visso                 Initial creation
 *  01.01.00  2017-03-25  visso                 Fixed configuration inclusion
 *  01.02.00  2021-02-18  visso   RTE-4007      Rework TechRef according to templates and remove full name from sources
 *********************************************************************************************************************/
#ifndef _XCP_H
# define _XCP_H 1

# include "RteAnalyzer/Source/Xcp_Cfg.h"

FUNC(uint8, XCP_CODE) Xcp_Event(uint32 event);



#endif

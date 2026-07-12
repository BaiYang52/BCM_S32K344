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
 *        \brief  Det Header for RTE Analyzer
 *
 *      \details  This header provides the prototypes of the DET module
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

#ifndef DET_H
# define DET_H

# include "Std_Types.h"

FUNC(Std_ReturnType, DET_CODE) Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId ); /* PRQA S 850 */ /* MD_DET_0850 */

#endif /* DET_H */


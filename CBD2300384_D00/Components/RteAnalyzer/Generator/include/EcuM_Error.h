/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  
 *        \brief  EcuM Error Header for RTE Analyzer
 *
 *      \details  This header provides the prototypes of the ECUM module
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-09-11  visso                 Initial creation
 *  01.01.00  2021-02-18  visso   RTE-4007      Rework TechRef according to templates and remove full name from sources
 *********************************************************************************************************************/

/* ---- Protection against multiple inclusion ----------------------------- */
#ifndef _ECUM_ERROR_H
# define _ECUM_ERROR_H

void EcuM_BswErrorHook(uint16 BswModuleId, uint8 ErrorId);


#define ECUM_BSWERROR_NULLPTR                       (0x00u)
#define ECUM_BSWERROR_COMPATIBILITYVERSION          (0x01u)
#define ECUM_BSWERROR_MAGICNUMBER                   (0x02u)
#define ECUM_NO_BSWERROR                            (0xFFu)

#endif

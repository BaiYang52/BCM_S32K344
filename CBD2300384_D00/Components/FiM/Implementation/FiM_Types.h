/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------**/
/*!     \file      FiM_Types.h
 *      \brief     Header for the AUTOSAR 4 FiM types
 *
 *      \details   Static header file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *            see FiM.h     
***********************************************************************************************************************/

#if !defined(FIM_TYPES_H)
#define FIM_TYPES_H

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/

#include "Std_Types.h"
#include "FiM_Cfg.h"


/*
 * File Rte_Type.h contains the typedefs and defines which are required as PortInterface argument
 * and also used in the FiM. If we are not using the RTE, the types will be defined by ourself below
   \trace DSGN-FiM22931 
 */
#if (FIM_CFG_USE_RTE == STD_ON)
#include "Rte_Type.h"
#else
/*! Type for function id.
   If RTE usage is not configured in FiM the RTE shouldn't have knowledge about FiM types;
   this switch is present in case the RTE has the types defined for some reason;
   if you have a different RTE generator that does not generate Rte_TypeDef...
   then include a user config file that defines it to your needs */
#if !defined (Rte_TypeDef_FiM_FunctionIdType)
typedef uint16  FiM_FunctionIdType;
#endif
#endif

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Type for FID (Pending) counter. Needs to allow for atomic write on target platform.
    \trace DSGN-FiM23391 */
typedef uint16  FiM_FidCounterType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* FIM_TYPES_H */

/***********************************************************************************************************************
 *  END OF FILE: FiM_Types.h
 **********************************************************************************************************************/
 

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
*  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VttCntrl_Eep.h
*        \brief   Declares the public API of the virtual EEPROM controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual EEPROM controller that enables to read and write bytes to a simulated EEPROM device.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-24  vismsl  -             Initial version extracted from VttCntrl generator.
 *  03.00.03  2015-08-26  visdap  -             Removed macro VTTEEP_EEPROM_TOTALSIZE from static header.
 *  03.00.04  2015-09-22  visdap  ESCAN00085394 Added pragmas to protect VttCntrl during simulated
 *                                              ECU reset.
 *  03.00.05  2016-01-12  vismsl  -             Reset behaviour of EEPROM fixed.
 *  03.00.06  2016-01-18  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *  03.00.07  2022-03-17  virbmz  MWDG-6674     VTTCntrl Fls/ Eep 64 bit support
*********************************************************************************************************************/

#if !defined(VTTCNTRL_EEP_H)
# define VTTCNTRL_EEP_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Eep_Cfg.h"

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTEEP_SW_MAJOR_VERSION (3u)
#define VTTEEP_SW_MINOR_VERSION (0u)
#define VTTEEP_SW_PATCH_VERSION (7u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

uint8 VttEep_Read(VttEep_AddressType address);
void VttEep_Write(VttEep_AddressType address, uint8 value);

void VttEep_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_EEP_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Eep.h
 *********************************************************************************************************************/

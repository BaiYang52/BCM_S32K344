/**********************************************************************************************************************
*  COPYRIGHT
*  -------------------------------------------------------------------------------------------------------------------
*  \verbatim
*  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
*
*                This software is copyright protected and proprietary to Vector Informatik GmbH.
*                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
*                All other rights remain with Vector Informatik GmbH.
*  \endverbatim
*  -------------------------------------------------------------------------------------------------------------------
*  FILE DESCRIPTION
*  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VttCntrl_Crypto.h
*        \brief   Declares the public API of the virtual ADC controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Crypto controller.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-12-14  virvchl STORYC-2154   Creation of VTT Crypto (ASR 4.3)
*********************************************************************************************************************/

#if !defined (VTTCNTRL_CYRYPTO_H)
# define VTTCNTRL_CYRYPTO_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "Crypto_30_Vtt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
# define VTTCRYPTO_SW_MAJOR_VERSION (1u)
# define VTTCRYPTO_SW_MINOR_VERSION (0u)
# define VTTCRYPTO_SW_PATCH_VERSION (0u)

# define VTTCRYPTO_FALSE  0
# define VTTCRYPTO_TRUE   1

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef uint32 VttCrypto_NvRamAdressType;
extern VttCrypto_NvRamAdressType VttCyrpto_InitialNvRamAddress;


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

void VttCntrl_OpenCryptoNvFile(void);
void VttCntrl_ReadCryptoNvFile(void);
void VttCntrl_WriteCryptoNvFile(void);

#endif  /* VTTCNTRL_CYRYPTO_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Cyrpto.h
 *********************************************************************************************************************/

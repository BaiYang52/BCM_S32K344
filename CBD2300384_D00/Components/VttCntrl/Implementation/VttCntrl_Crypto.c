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
/**      \file    VttCntrl_Crypto.c
*        \brief   Implements the public API of the virtual Crypto controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Crypto controller.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the module's header file.
*
*  FILE VERSION
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the VERSION CHECK below.
*********************************************************************************************************************/

#define VTTCRYPTO_SOURCE
#define VTTCNTRL_CRYPTO_SOURCE


/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/


#include "VttCntrl_Crypto.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the Crypto controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()


/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Crypto header file. */
#if (  (VTTCRYPTO_SW_MAJOR_VERSION != (1u)) \
    || (VTTCRYPTO_SW_MINOR_VERSION != (0u)) \
    || (VTTCRYPTO_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of VttCntrl_Adc.c and VttCntrl_Adc.h are inconsistent"
#endif


/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/


/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/


/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC static
#endif


/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/


/**********************************************************************************************************************
*  GLOBAL DATA
*********************************************************************************************************************/

/*! \brief Base address of the non-volatile RAM (NvRAM) file that stores the Crypto data. 
*          Technically, the NvRAM file is memory-mapped for fast random-access. */
VttCrypto_NvRamAdressType VttCyrpto_InitialNvRamAddress = 0;


/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/* Intentionally left empty. */


/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/* Intentionally left empty. */


/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/
void VttCntrl_OpenCryptoNvFile(void)
{
  uint32 existingSize, totalSize = Crypto_30_Vtt_GetSizeOfKeyStorageOfPCConfig();
  if (VttCyrpto_InitialNvRamAddress == 0)
  {
    VttCyrpto_InitialNvRamAddress = (VttCrypto_NvRamAdressType) CANoeAPI_OpenNvRamInstance("Crypto", totalSize, &existingSize);
    if (existingSize != totalSize)
    {
      VttCntrl_WriteCryptoNvFile();
    }
  }
  else
  {
    /* File already exists */
  }
}

void VttCntrl_ReadCryptoNvFile(void)
{
  int index = 0;
  
  if (VttCyrpto_InitialNvRamAddress == 0)
  {
    return;
  }
  else
  {
    for(index = 0; index < Crypto_30_Vtt_GetSizeOfKeyStorageOfPCConfig() - 1; index++)
    {
      Crypto_30_Vtt_SetKeyStorage(index, * (Crypto_30_Vtt_KeyStorageType*) (VttCyrpto_InitialNvRamAddress + index));
      CANoeAPI_ConsumeTicks(1);
    }
  }
}

void VttCntrl_WriteCryptoNvFile(void)
{
  int index = 0;
  
  if (VttCyrpto_InitialNvRamAddress == 0)
  {
    return;
  }
  else
  {
    for(index = 0; index < Crypto_30_Vtt_GetSizeOfKeyStorageOfPCConfig() - 1; index++)
    {
      *( (Crypto_30_Vtt_KeyStorageType*)(VttCyrpto_InitialNvRamAddress + index)) = Crypto_30_Vtt_GetKeyStorage(index);;
      CANoeAPI_ConsumeTicks(1);
    }
  }
}

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Crypto.c
*********************************************************************************************************************/

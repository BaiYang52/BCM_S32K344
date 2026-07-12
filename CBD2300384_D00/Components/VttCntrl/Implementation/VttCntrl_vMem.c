/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  VttCntrl_vMem.c
 *        \brief  Implements the public API of the virtual memory controller integrated in the vVIRTUALtarget.
 *
 *      \details  See VttCntrl_vMem.h
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

#define VTTVMEM_SOURCE
#define VTTCNTRL_VMEM_SOURCE
/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_vMem.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the memory controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of VttCntrl_vMem header file. */
#if (  (VTTVMEM_SW_MAJOR_VERSION != (1u)) \
    || (VTTVMEM_SW_MINOR_VERSION != (0u)) \
    || (VTTVMEM_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of VttCntrl_Fls.c and VttCntrl_Fls.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTVMEM_CFG_MAJOR_VERSION != (0x01u)) \
    || (VTTVMEM_CFG_MINOR_VERSION != (0x00u)))
  #error "Version numbers of VttCntrl_vMem.c and VttCntrl_vMem_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
#define STATIC static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal ECU state. */
STATIC uint8 VttvMem_EcuState = CANOEAPI_ECUSTATE_INITIAL;

/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/*! \brief Base RAM address of the non-volatile RAM (NvRAM) file that stores the memory data.
*          Technically, the NvRAM file is memory-mapped for fast random-access. */
extern vMem_30_Vtt_AddressType VttvMem_NvRamAddress;

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/* The implementaton of virtual memory controller (initialization, deinitialization and REST handling)
 * is generated via Vtt_Cntrl generator into VttCntrl_vMem_Cfg.c
 */
extern void VttvMem_Init(void);
extern void VttvMem_Deinit(void);
extern void VttvMem_OnReset(void);

STATIC void VttvMem_OnSwitchOn(void);
STATIC void VttvMem_OnSwitchOff(void);
STATIC void VttvMem_OnStopMeasurement(void);
STATIC void VttvMem_OnUnload(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttvMem_OnSwitchOn()
**********************************************************************************************************************/
/*! \brief     OnSwitchOn callout of virtual memory controller.
*
*         Tasks:
*            - Sets the base RAM address of the NvRAM file to zero.
*            - Initializes the virtual memory controller.
*
*  \context    Called by VttvMem_OnStateChange handler when the virtual ECU is powered on.
**********************************************************************************************************************/
STATIC void VttvMem_OnSwitchOn(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttvMem_NvRamAddress = 0;
  VttvMem_Init();

} /* VttvMem_OnSwitchOn() */

/***********************************************************************************************************************
*  VttvMem_OnSwitchOff()
**********************************************************************************************************************/
/*! \brief     OnSwitchOff callout of virtual memory controller.
*
*         Tasks:
*            - De-initializes the virtual memory controller.
*
*  \context    Called by VttvMem_OnStateChange handler right before the virtual ECU is powered off.
**********************************************************************************************************************/
STATIC void VttvMem_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttvMem_Deinit();

} /* VttvMem_OnSwitchOff() */

/***********************************************************************************************************************
*  VttvMem_OnStopMeasurement()
**********************************************************************************************************************/
/*! \brief     OnStopMeasurement callout of virtual memory controller.
*
*         Tasks:
*            - De-initializes the virtual memory controller.
*
*  \context    Called by VttvMem_OnStateChange handler right simulation of the virtual ECU is stopped.
**********************************************************************************************************************/
STATIC void VttvMem_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttvMem_Deinit();

} /* VttvMem_OnStopMeasurement() */

/***********************************************************************************************************************
*  VttvMem_OnUnload()
**********************************************************************************************************************/
/*! \brief     OnUnload callout of virtual memory controller.
*
*         Tasks:
*            - De-initializes the virtual memory controller.
*
*  \context    Called by VttvMem_OnStateChange when the DLL of the ECU is unloaded in CANoe.
**********************************************************************************************************************/
STATIC void VttvMem_OnUnload(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttvMem_Deinit();

} /* VttvMem_OnUnload() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttvMem_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual memory controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttvMem_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;

  /* ----- Implementation ----------------------------------------------- */
  VttvMem_EcuState = newState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    break;
  case CANOEAPI_ECUACTION_SWITCHON:
    {
      VttvMem_OnSwitchOn();
    }    
    break;
  case CANOEAPI_ECUACTION_RESET:
    {
      VttvMem_OnReset();
    }
    break;
  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;
  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  case CANOEAPI_ECUACTION_SWITCHOFF:
    {
      VttvMem_OnSwitchOff();
    }    
    break;
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    {
      VttvMem_OnStopMeasurement();
    }
    break;
  case CANOEAPI_ECUACTION_UNLOAD:
    {
      VttvMem_OnUnload();
    }
    break;
  }

} /* VttvMem_OnStateChange() */

/**********************************************************************************************************************
*  VttvMem_Read()
**********************************************************************************************************************/
/*! \brief     Reads a byte from the memory.
*  \param[in]  address        Memory address of the vMem to be read from.
*  \return     0 if the initialization of the memory failed, otherwise the byte value stored at given memory address.
*  \context    Called by Fls driver.
*********************************************************************************************************************/
uint8 VttvMem_Read(vMem_30_Vtt_AddressType address)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (VttvMem_NvRamAddress == 0)
  {
    return 0;
  }

  /* Compute the effective memory address and read the byte value from the memory. */
  return *((uint8*)(VttvMem_NvRamAddress + address));

} /* VttvMem_Read() */

/**********************************************************************************************************************
*  VttvMem_Write()
**********************************************************************************************************************/
/*! \brief     Writes a byte to the memory.
*
*  \param[in]  address      Memory address of the vMem to be written to.
*  \param[in]  value        Value (byte) that shall be stored at the given memory address of memory.
*
*  \context    Called by Fls driver.
*  \note      If the initialization of the memory failed, the write action is skipped.
*********************************************************************************************************************/
void VttvMem_Write(vMem_30_Vtt_AddressType address, uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
    if (VttvMem_NvRamAddress == 0)
    {
        return;
    }

  /* Compute the effective memory address and write given byte value to the memory. */
    *((uint8*)(VttvMem_NvRamAddress + address)) = value;

} /* VttvMem_Write() */

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_vMem.c
*********************************************************************************************************************/

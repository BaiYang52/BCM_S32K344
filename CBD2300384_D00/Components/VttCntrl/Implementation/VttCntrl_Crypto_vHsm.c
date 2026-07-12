/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VttCntrl_Crypto_vHsm.h
*        \brief   Implements the public API of the Crypto vHSM integrated in the vVIRTUALtarget.
*
*      \details   Passes IPC address to vHSM vVIRTUALtarget instance.
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

#define VTTCRYPTO_VHSM_SOURCE
#define VTTCNTRL_CRYPTO_VHSM_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Crypto_vHsm.h"
#include "Crypto_30_vHsm_Ipc.h"

/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the virtual CRYPTO_VHSM is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Cryptov_Hsm header file. */
#if (  (VTTCRYPTO_VHSM_SW_MAJOR_VERSION != (12u)) \
    || (VTTCRYPTO_VHSM_SW_MINOR_VERSION != (1u)) \
    || (VTTCRYPTO_VHSM_SW_PATCH_VERSION != (0u)) )
  #error "Vendor specific version numbers of VttCntrl_Crypto_vHsm.c and VttCntrl_Crypto_vHsm.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

#ifndef VTTCNTRL_CRYPTO_VHSM_IRQ
# define VTTCNTRL_CRYPTO_VHSM_IRQ IRQ_Hsm2Htf
#endif

#ifndef VTTCNTRL_CRYPTO_VHSM_HT2HSMF_VALUE
# define VTTCNTRL_CRYPTO_VHSM_HT2HSMF_VALUE 1u
#endif

#ifndef VTTCNTRL_CRYPTO_VHSM_ISR_FCT_NAME
#define VTTCNTRL_CRYPTO_VHSM_ISR_FCT_NAME Os_Isr_vHsmResponseIsr
#endif

extern void VTTCNTRL_CRYPTO_VHSM_ISR_FCT_NAME(void);

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
sint32 VttCntrl_Crypto_vHsm_Handle_IpcAddress;
sint32 VttCntrl_Crypto_vHsm_Handle_Ht2Hsmf;
/**********************************************************************************************************************
*  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttCntrl_Crypto_vHsm_OnLoad(void);
STATIC void VttCntrl_Crypto_vHsm_OnSwitchOn(void);

void VttCntrl_Crypto_vHsm_ChangeHandlerUInt32(sint32 param, uint32 value);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttCntrl_Crypto_vHsm_OnSwitchOn()
**********************************************************************************************************************/
/*! \brief     OnSwitchOn callout of virtual CRYPTO_VHSM.
*
*         Tasks:
*            - Allocate a system variable for storing the last occurred reset reason.
*
*  \context    Called by VttCntrl_Crypto_vHsm_OnStateChange handler when Node DLL is switched on.
**********************************************************************************************************************/
STATIC void VttCntrl_Crypto_vHsm_OnSwitchOn(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_SysVar_SetUInt64(VttCntrl_Crypto_vHsm_Handle_IpcAddress, (uint64)(Crypto_30_vHsm_Ipc_Memory));

}  /* VttCntrl_Crypto_vHsm_OnSwitchOn() */


/***********************************************************************************************************************
*  VttCntrl_Crypto_vHsm_OnLoad()
**********************************************************************************************************************/
/*! \brief     OnLoad callout of virtual CRYPTO_VHSM.
*
*         Tasks:
*            - Allocate a system variable for storing the last occurred reset reason.
*
*  \context    Called by VttCntrl_Crypto_vHsm_OnStateChange handler when Node DLL is loaded.
**********************************************************************************************************************/
STATIC void VttCntrl_Crypto_vHsm_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint32  VttCntrl_Crypto_vHsm_Handle_Hsm2Htf;
  /* ----- Implementation ----------------------------------------------- */
  /* Handle not needed as value is set correctly on load */
  VttCntrl_Crypto_vHsm_Handle_IpcAddress = CANoeAPI_SysVar_DeclareUInt64("Crypto_vHsm", "IpcAddress", (uint64)(Crypto_30_vHsm_Ipc_Memory));

  VttCntrl_Crypto_vHsm_Handle_Ht2Hsmf = CANoeAPI_SysVar_DeclareUInt32("Crypto_vHsm", "Ht2Hsmf", 0U);
  VttCntrl_Crypto_vHsm_Handle_Hsm2Htf = CANoeAPI_SysVar_DeclareUInt32("Crypto_vHsm", "Hsm2Htf", 0U);


  CANoeAPI_SysVar_SetHandlerUInt32(VttCntrl_Crypto_vHsm_Handle_Hsm2Htf, VttCntrl_Crypto_vHsm_ChangeHandlerUInt32, VttCntrl_Crypto_vHsm_Handle_Hsm2Htf);
}  /* VttCntrl_Crypto_vHsm_OnLoad() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttCntrl_Crypto_vHsm_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual CRYPTO_VHSM about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttCntrl_Crypto_vHsm_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  (void)oldState;
  (void)newState;

  /* ----- Implementation ----------------------------------------------- */
  switch (action)
  {
  case CANOEAPI_ECUACTION_LOAD:
  {
    VttCntrl_Crypto_vHsm_OnLoad();
    break;
  }
  case CANOEAPI_ECUACTION_INITMEASUREMENT:
    VttCntrl_Crypto_vHsm_OnSwitchOn();
    break;
  default:
    /* nothing to do*/
    break;
  }

} /* VttCntrl_Crypto_vHsm_OnStateChange() */


/***********************************************************************************************************************
*  VttCntrl_Crypto_vHsm_ChangeHandlerUInt32()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual CRYPTO_VHSM about a changed system variable.
*
*  \param[in]  param         Number declared by CRYPTO_VHSM to indentify the system variable
*  \param[in]  value         Value of the system variable
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttCntrl_Crypto_vHsm_ChangeHandlerUInt32(sint32 param, uint32 value)
{
  (void)value;
  (void)param;

  VttCntrl_Base_SetCheckedInterrupt(VTTCNTRL_CRYPTO_VHSM_IRQ);
}


/***********************************************************************************************************************
*  Ht2Hsmf
**********************************************************************************************************************/
/*! \brief     Interrupt service routine triggered when a Ht2Hsmf ISR is triggered. This method transfers it to vHsm.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
void Ht2Hsmf(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_SysVar_SetUInt32(VttCntrl_Crypto_vHsm_Handle_Ht2Hsmf, (uint32)(VTTCNTRL_CRYPTO_VHSM_HT2HSMF_VALUE));

} /* VttSysVarIsr_0() */


/***********************************************************************************************************************
*  Hsm2Htf
**********************************************************************************************************************/
/*! \brief     Interrupt service routine triggered from HSM forwarded to real Crypto vHSM Driver.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
ISR(Hsm2Htf)
{
  VTTCNTRL_CRYPTO_VHSM_ISR_FCT_NAME();
}


/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Crypto_vHsm.c
*********************************************************************************************************************/

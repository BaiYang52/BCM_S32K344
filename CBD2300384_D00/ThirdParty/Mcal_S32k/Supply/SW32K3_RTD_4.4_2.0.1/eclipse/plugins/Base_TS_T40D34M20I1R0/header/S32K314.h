/*
** ###################################################################
**     Processor:           S32K314
**     Reference manual:    S32K3xx RM Rev.3
**     Version:             rev. 1.1, 2021-10-27
**     Build:               b211027
**
**     Abstract:
**         Peripheral Access Layer for S32K314
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2021 NXP
**
**     NXP Confidential. This software is owned or controlled by NXP and may only be
**     used strictly in accordance with the applicable license terms. By expressly
**     accepting such terms or by downloading, installing, activating and/or otherwise
**     using the software, you are agreeing that you have read, and that you agree to
**     comply with and are bound by, such license terms. If you do not agree to be
**     bound by the applicable license terms, then you may not retain, install,
**     activate or otherwise use the software. The production use license in
**     Section 2.3 is expressly granted for this software.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 Taru Shree 03-Sep-21  Initial release based on 
**                                      RM Rev.2.
**
**     - rev. 1.1 Taru Shree 27-Oct-21  1)Initial release based on 
**                                      RM Rev.3.
**                                      2)EMAC-
**                                 	    MAC_VLAN_INCL register 
**                                      array with MAC_VLAN_INCL, 
**                                      MAC_VLAN_INCL0, MAC_VLAN_INCL1, 
**                                      MAC_VLAN_INCL2, MAC_VLAN_INCL3, 
**                                      MAC_VLAN_INCL4, MAC_VLAN_INCL5, 
**                                      MAC_VLAN_INCL6 and MAC_VLAN_INCL7 
**                                      registers is added.
**                                      3)XBIC_PERI and XBIC_EDMA instances 
**                                      positions are interchanged.
**
** ###################################################################
*/

/*!
 * @file S32K314.h
 * @version 1.1
 * @date 2021-10-27
 * @brief Peripheral Access Layer for S32K314
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/* Prevention from multiple including the same memory map */
#if !defined(S32K314_H_)  /* Check if memory map has not been already included */
#define S32K314_H_

/* ----------------------------------------------------------------------------
   -- IP Header Files
   ---------------------------------------------------------------------------- */

/* IP Header Files List */
#include "S32K314_ADC.h"
#include "S32K314_AXBS_LITE.h"
#include "S32K314_BCTU.h"
#include "S32K314_CMU_FC.h"
#include "S32K314_CMU_FM.h"
#include "S32K314_CONFIGURATION_GPR.h"
#include "S32K314_CRC.h"
#include "S32K314_DCM.h"
#include "S32K314_DCM_GPR.h"
#include "S32K314_DMA.h"
#include "S32K314_DMAMUX.h"
#include "S32K314_DMA_TCD.h"
#include "S32K314_EIM.h"
#include "S32K314_EMAC.h"
#include "S32K314_EMIOS.h"
#include "S32K314_ERM.h"
#include "S32K314_FCCU.h"
#include "S32K314_FIRC.h"
#include "S32K314_FLASH.h"
#include "S32K314_FLEXCAN.h"
#include "S32K314_FLEXIO.h"
#include "S32K314_FXOSC.h"
#include "S32K314_INTM.h"
#include "S32K314_JDC.h"
#include "S32K314_LCU.h"
#include "S32K314_LPCMP.h"
#include "S32K314_LPI2C.h"
#include "S32K314_LPSPI.h"
#include "S32K314_LPUART.h"
#include "S32K314_MCM_CM7.h"
#include "S32K314_MC_CGM.h"
#include "S32K314_MC_ME.h"
#include "S32K314_MC_RGM.h"
#include "S32K314_MDM_AP.h"
#include "S32K314_MSCM.h"
#include "S32K314_MU.h"
#include "S32K314_PFLASH.h"
#include "S32K314_PIT.h"
#include "S32K314_PLL.h"
#include "S32K314_PMC.h"
#include "S32K314_PRAMC.h"
#include "S32K314_QUADSPI.h"
#include "S32K314_QUADSPI_ARDB.h"
#include "S32K314_RTC.h"
#include "S32K314_SAI.h"
#include "S32K314_SDA_AP.h"
#include "S32K314_SELFTEST_GPR.h"
#include "S32K314_SEMA42.h"
#include "S32K314_SIRC.h"
#include "S32K314_SIUL2.h"
#include "S32K314_STCU.h"
#include "S32K314_STM.h"
#include "S32K314_SWT.h"
#include "S32K314_SXOSC.h"
#include "S32K314_TEMPSENSE.h"
#include "S32K314_TRGMUX.h"
#include "S32K314_TSPC.h"
#include "S32K314_VIRT_WRAPPER.h"
#include "S32K314_WKPU.h"
#include "S32K314_XBIC.h"
#include "S32K314_XRDC.h"
#include "S32K314_NVIC.h"
#include "S32K314_SCB.h"
#include "S32K314_SYSTICK.h"
#include "S32K314_MPU.h"

#endif  /* #if !defined(S32K314_H_) */

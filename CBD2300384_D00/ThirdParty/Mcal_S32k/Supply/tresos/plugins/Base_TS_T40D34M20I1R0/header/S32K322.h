/*
** ###################################################################
**     Processor:           S32K322_M7
**     Reference manual:    S32K3xx RM Rev.3
**     Version:             rev. 1.0, 2021-12-03
**     Build:               b211203
**
**     Abstract:
**         Peripheral Access Layer for S32K322_M7
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
**
**     - rev. 1.0 Taru Shree 03-Dec-21  Initial release based on 
**                                      RM S32K3xx Rev.3.
**
**
** ###################################################################
*/

/*!
 * @file S32K322.h
 * @version 1.0
 * @date 2021-12-03
 * @brief Peripheral Access Layer for S32K322
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/* Prevention from multiple including the same memory map */
#if !defined(S32K322_H_)  /* Check if memory map has not been already included */
#define S32K322_H_

/* ----------------------------------------------------------------------------
   -- IP Header Files
   ---------------------------------------------------------------------------- */

/* IP Header Files List */
#include "S32K322_ADC.h"
#include "S32K322_AXBS_LITE.h"
#include "S32K322_BCTU.h"
#include "S32K322_CMU_FC.h"
#include "S32K322_CMU_FM.h"
#include "S32K322_CONFIGURATION_GPR.h"
#include "S32K322_CRC.h"
#include "S32K322_DCM.h"
#include "S32K322_DCM_GPR.h"
#include "S32K322_DMA.h"
#include "S32K322_DMAMUX.h"
#include "S32K322_DMA_TCD.h"
#include "S32K322_EIM.h"
#include "S32K322_EMAC.h"
#include "S32K322_EMIOS.h"
#include "S32K322_ERM.h"
#include "S32K322_FCCU.h"
#include "S32K322_FIRC.h"
#include "S32K322_FLASH.h"
#include "S32K322_FLEXCAN.h"
#include "S32K322_FLEXIO.h"
#include "S32K322_FXOSC.h"
#include "S32K322_I3C.h"
#include "S32K322_INTM.h"
#include "S32K322_JDC.h"
#include "S32K322_LCU.h"
#include "S32K322_LPCMP.h"
#include "S32K322_LPI2C.h"
#include "S32K322_LPSPI.h"
#include "S32K322_LPUART.h"
#include "S32K322_MCM_CM7.h"
#include "S32K322_MC_CGM.h"
#include "S32K322_MC_ME.h"
#include "S32K322_MC_RGM.h"
#include "S32K322_MDM_AP.h"
#include "S32K322_MSCM.h"
#include "S32K322_MU.h"
#include "S32K322_PFLASH.h"
#include "S32K322_PIT.h"
#include "S32K322_PLL.h"
#include "S32K322_PMC.h"
#include "S32K322_PRAMC.h"
#include "S32K322_QUADSPI.h"
#include "S32K322_QUADSPI_ARDB.h"
#include "S32K322_RTC.h"
#include "S32K322_SAI.h"
#include "S32K322_SDA_AP.h"
#include "S32K322_SEMA42.h"
#include "S32K322_SIRC.h"
#include "S32K322_SIUL2.h"
#include "S32K322_STCU.h"
#include "S32K322_STM.h"
#include "S32K322_SWT.h"
#include "S32K322_SXOSC.h"
#include "S32K322_TEMPSENSE.h"
#include "S32K322_TRGMUX.h"
#include "S32K322_TSPC.h"
#include "S32K322_VIRT_WRAPPER.h"
#include "S32K322_WKPU.h"
#include "S32K322_XBIC.h"
#include "S32K322_XRDC.h"
#include "S32K322_NVIC.h"
#include "S32K322_SCB.h"
#include "S32K322_SYSTICK.h"
#include "S32K322_MPU.h"
#endif  /* #if !defined(S32K322_H_) */

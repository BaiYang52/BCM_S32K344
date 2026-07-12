/*
** ###################################################################
**     Processor:           S32K342_M7
**     Reference manual:    S32K3xx RM Rev.3
**     Version:             rev. 1.4, 2021-12-10
**     Build:               b211210
**
**     Abstract:
**         Peripheral Access Layer for S32K342_M7
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
**     - rev. 1.0 Taru Shree 11-Jan-21  Initial release based on 
**                                      S32K3xx RM Rev.2 Draft A.
**
**     - rev. 1.1 Taru Shree 08-Mar-21  1)Initial release based on 
**                                      S32K3xx RM Rev.2 Draft B.  
**                                      2)FLASH-
**                                      -ADR register:
**                                       A3 and A4 bitfields are 
**                                       removed.
**                                      -AMCR, AMCRS, APEADR, ADATA, 
**                                       ASPELOCK And ASSPELOCK 
**                                       registers are removed.
**                                      3)MDM_AP-
**                                      -MDMAPSTTS register: 
**                                       HSEHLT, HSEBPDPSLP, HSEBSLPNG 
**                                       and CM0PDBGRST bitfields are 
**                                       removed.
**                                      -MDMAPCTL register:
**                                       HSEBDBGREQ And CM0PDBGRSRT bitfields 
**                                       are removed.
**                                      4)PFLASH-
**                                      -PFCPGM_APEADR_L and PFCPGM_APEADR_P 
**                                       registers are removed.   
**                                      5)SAI-
**                                      BCS bitfield is removed from
**                                      TCR2 and RCR2 registers.
**                                      6)VIRT_WRAPPER-
**                                      A register array is renamed as REG_A.
**                                      7)FLASH_ALT and PFLASH_ALT instances 
**                                      are removed.
**                                      8)In address definitions, instances 
**                                      are prefixed with "IP".
**                                      9)Following interrupt names are changed-
**                                      -From HSE_B_0_IRQn to HSE_MU0_TX_IRQn.
**                                      -From HSE_B_1_IRQn to HSE_MU0_RX_IRQn.
**                                      -From HSE_B_2_IRQn to HSE_MU0_ORED_IRQn.
**                                      -From HSE_B_3_IRQn to HSE_MU1_TX_IRQn.
**                                      -From HSE_B_4_IRQn to HSE_MU1_RX_IRQn.
**                                      -From HSE_B_5_IRQn to HSE_MU1_ORED_IRQn.
**
**     - rev. 1.2 Taru Shree 23-Aug-21  1)Initial release based on 
**                                      S32K3xx RM Rev.2.  
**                                      2)ADC-
**                                      -PCDR registers are converted into array.
**                                      -AMSIO register: HSEN bitfield’s size is 
**                                       changed from 1 to 2.
**                                      3)DCM- 
**                                      -DCMMISC register: MRKLSTRCHK bitfield is 
**                                       removed.
**                                      -DCMROD3 register: CM7_1_LOCKUP, 
**                                       PRAM1_ECC_ERR, CM7_1_DCDATA_ECC_ERR, 
**                                       CM7_1_DCTAG_ECC_ERR and CM7_1_ICDATA_ECC_ERR 
**                                       bitfields are removed.
**                                      -DCMROD4 register: CM7_1_ICTAG_ECC_ERR, 
**                                       PRAM1_FCCU_ALARM, PF2_CODE_ECC_ERR, 
**                                       PF2_DATA_ECC_ERR and FLASH_ECC_ERR bitfields 
**                                       are removed.
**                                      -DCMROD5 register: CM7_1_AHBP_RDATA_EDC_ERR and 
**                                       CM7_1_AHBM_RDATA_EDC_ERR bitfields are removed.
**                                      -DCMROF20 register: CM7_TCM_WS_EN bitfield is 
**                                       removed.
**                                      -DCMRWD3 register: CM7_1_LOCKUP_EN, 
**                                       PRAM1_ECC_ERR_EN, CM7_1_DCDATA_ECC_ERR_EN, 
**                                       CM7_1_DCTAG_ECC_ERR_EN and 
**                                       CM7_1_ICDATA_ECC_ERR_EN bitfields are removed.
**                                      -DCMRWD4 register: CM7_1_ICTAG_ECC_ERR_EN, 
**                                       PRAM1_FCCU_ALARM_EN, PF2_CODE_ECC_ERR_EN, 
**                                       PF2_DATA_ECC_ERR_EN and FLASH_ECC_ERR_EN 
**                                       bitfields are removed.
**                                      -DCMRWD5 register: CM7_1_AHBP_RDATA_EDC_ERR_EN 
**                                       and CM7_1_AHBM_RDATA_EDC_ERR_EN bitfields 
**                                       are removed.
**                                      -DCMRWD6 register: EMIOS2_DBG_DIS_CM7_0, 
**                                       LPSPI4_DBG_DIS_CM7_0, LPSPI5_DBG_DIS_CM7_0, 
**                                       FLEXCAN4_DBG_DIS_CM7_0 and FLEXCAN5_DBG_DIS_CM7_0 
**                                       bitfields are removed.
**                                      -DCMRWD8 register: All bitfields are removed.
**                                      -DCMRWD9 register: All bitfields are removed.
**                                      -DCMRWF4 register: MUX_MODE_EN_ADC2_S8, 
**                                       MUX_MODE_EN_ADC2_S9 and CM7_1_CPUWAIT bitfields 
**                                       are removed.
**                                      4)EMAC-
**                                      MAC_VLAN_TAG, MAC_VLAN_TAG_CTRL, 
**                                      MAC_VLAN_TAG_FILTER0, MAC_VLAN_TAG_FILTER1, 
**                                      MAC_VLAN_TAG_FILTER2 and MAC_VLAN_TAG_FILTER3 
**                                      registers are added.
**                                      5)I3C-
**                                      SCAPABILITIES2 register: V1_1 bitfield is 
**                                      removed.
**                                      6)LPI2C-
**                                      SRDR register: RADDR bitfield is added.
**                                      7)MDM_AP-
**                                      -MDMAPCTL register: DBGRSTSLOWPAD, 
**                                       DBGRSTFASTPAD and TRIUOVRD bitfields 
**                                       are added.
**                                      -MDMAPWIREN register: LWPRSTPRVT bitfield 
**                                       is added.
**                                      8)SIUL2-
**                                      -UDR1 register: 
**                                       -uint16_t is changed to uint32_t bits.
**                                       -UDR16, UDR17…UDR31 bitfields are added.
**                                      -UDR0 register:	
**                                       -uint16_t is changed to uint32_t bits.
**                                       -UDR16, UDR17…UDR31 bitfields are added.
**                                      -UDR3 register:
**                                       -uint16_t is changed to uint32_t bits.
**                                       -UDR16, UDR17…UDR31 bitfields are added.
**                                      -UDR2 register:	
**                                       -uint16_t is changed to uint32_t bits.
**                                       -UDR16, UDR17…UDR31 bitfields are added.
**                                      -UDR5, UDR4, UDR7 and UDR6 registers are removed.	
**
** - rev. 1.3 Jalaj Awasthi 22-Nov-21     1)Initial release based on 
**                                        S32K3xx RM Rev.3.
**
** - rev. 1.4 Jalaj Awasthi 10-Dec-21     1)Initial release based on 
**                                        S32K3xx RM Rev.3 (Updated).
**                                        2)CMP module name is changed to LPCMP
**                                        3)FlexCAN_3 module name is changed to FLEXCAN
**                                        4)MUB module name is changed to MU
**
** ###################################################################
*/

/*!
 * @file S32K342.h
 * @version 1.4
 * @date 2021-12-10
 * @brief Peripheral Access Layer for S32K342
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/* Prevention from multiple including the same memory map */
#if !defined(S32K342_H_)  /* Check if memory map has not been already included */
#define S32K342_H_

/* ----------------------------------------------------------------------------
   -- IP Header Files
   ---------------------------------------------------------------------------- */

/* IP Header Files List */
#include "S32K342_ADC.h"
#include "S32K342_AXBS_LITE.h"
#include "S32K342_BCTU.h"
#include "S32K342_CMU_FC.h"
#include "S32K342_CMU_FM.h"
#include "S32K342_CONFIGURATION_GPR.h"
#include "S32K342_CRC.h"
#include "S32K342_DCM.h"
#include "S32K342_DCM_GPR.h"
#include "S32K342_DMA.h"
#include "S32K342_DMAMUX.h"
#include "S32K342_DMA_TCD.h"
#include "S32K342_EIM.h"
#include "S32K342_EMAC.h"
#include "S32K342_EMIOS.h"
#include "S32K342_ERM.h"
#include "S32K342_FCCU.h"
#include "S32K342_FIRC.h"
#include "S32K342_FLASH.h"
#include "S32K342_FLEXCAN.h"
#include "S32K342_FLEXIO.h"
#include "S32K342_FXOSC.h"
#include "S32K342_I3C.h"
#include "S32K342_INTM.h"
#include "S32K342_JDC.h"
#include "S32K342_LCU.h"
#include "S32K342_LPCMP.h"
#include "S32K342_LPI2C.h"
#include "S32K342_LPSPI.h"
#include "S32K342_LPUART.h"
#include "S32K342_MCM_CM7.h"
#include "S32K342_MC_CGM.h"
#include "S32K342_MC_ME.h"
#include "S32K342_MC_RGM.h"
#include "S32K342_MDM_AP.h"
#include "S32K342_MSCM.h"
#include "S32K342_MU.h"
#include "S32K342_PFLASH.h"
#include "S32K342_PIT.h"
#include "S32K342_PLL.h"
#include "S32K342_PMC.h"
#include "S32K342_PRAMC.h"
#include "S32K342_QUADSPI.h"
#include "S32K342_QUADSPI_ARDB.h"
#include "S32K342_RTC.h"
#include "S32K342_SAI.h"
#include "S32K342_SDA_AP.h"
#include "S32K342_SEMA42.h"
#include "S32K342_SIRC.h"
#include "S32K342_SIUL2.h"
#include "S32K342_STCU.h"
#include "S32K342_STM.h"
#include "S32K342_SWT.h"
#include "S32K342_SXOSC.h"
#include "S32K342_TEMPSENSE.h"
#include "S32K342_TRGMUX.h"
#include "S32K342_TSPC.h"
#include "S32K342_VIRT_WRAPPER.h"
#include "S32K342_WKPU.h"
#include "S32K342_XBIC.h"
#include "S32K342_XRDC.h"
#include "S32K342_NVIC.h"
#include "S32K342_SCB.h"
#include "S32K342_SYSTICK.h"
#include "S32K342_MPU.h"
#endif  /* #if !defined(S32K342_H_) */

/*
** ###################################################################
**     Processor:           S32K312_M7
**     Reference manual:    S32K3xx RM Rev.3
**     Version:             rev. 1.3, 2021-10-28
**     Build:               b211028
**
**     Abstract:
**         Peripheral Access Layer for S32K312_M7
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
**     - rev. 1.0 Taru Shree 05-Oct-20  Initial release based on 
**                                      RM Rev.1.
**
**     - rev. 1.1 Taru Shree 12-Mar-20  1)Initial release based on
**                                      RM Rev.2 Draft B.
**                                      2)ADC-
**                                      -ICDR registers are converted into array.
**                                      -AMSIO and CAL2 registers are added.
**                                      3)DCM- 
**                                      -DCMSTAT Register: 
**                                       -DCMOTAA and DCMOTAR bitfields are removed.
**                                       -DCMERR and DCMUTS bitfields are added.
**                                      -DCMMISC Register: DCMDBGE bitfield is added.
**                                      4)FLASH-
**                                      -ADR Register:
**                                       A3 and A4 bitfields are removed.
**                                      -AMCR, AMCRS, APEADR, ADATA, ASPELOCK and ASSPELOCK 
**                                       registers are removed.
**                                      5)MDM_AP-
**                                      -MDMAPSTTS Register:
**                                       HSEHLT, HSEBPDPSLP, HSEBSLPNG And CM0PDBGRST 
**                                       bitfields are removed.
**                                      -MDMAPCTL register: 
**                                       HSEBDBGREQ and CM0PDBGRSRT bitfield are removed.
**                                      6)MSCM-
**                                      -CP1TYPE, CP1NUM, CP1REV, CP1CFG0, CP1CFG1, CP1CFG2
**                                       and CP1CFG3 registers are removed.
**                                      -IRCPmISRn register: 
**                                       CP1_INT bitfield is removed.
**                                      -XN_CTRL register is added.
**                                      -ENEDC register is updated.
**                                      -IRSPRC register: 
**                                       M7_1 Bitfield is removed.
**                                      7)PFLASH-
**                                      PFCPGM_APEADR_L and PFCPGM_APEADR_P registers are 
**                                      removed.
**                                      8)VIRT_WRAPPER-
**                                      A register array is renamed as REG_A.
**                                      9)XRDC instance is added.
**                                      10)FLASH_ALT And PFLASH_ALT instances are removed.
**                                      11)Following interrupt names are
**                                      changed-
**                                      -From HSE_B_0_IRQn to HSE_MU0_TX_IRQn.
**                                      -From HSE_B_1_IRQn to HSE_MU0_RX_IRQn.
**                                      -From HSE_B_2_IRQn to HSE_MU0_ORED_IRQn.
**                                      -From HSE_B_3_IRQn to HSE_MU1_TX_IRQn.
**                                      -From HSE_B_4_IRQn to HSE_MU1_RX_IRQn.
**                                      -From HSE_B_5_IRQn to HSE_MU1_ORED_IRQn. 
**
**     - rev. 1.2 Taru Shree 18-Aug-21  1)Initial release based on
**                                      RM Rev.2.
**                                      2)ADC-                                 
**                                      -PCDR registers are converted into    
**                                       array.                               
**                                      -AMSIO register: HSEN bitfield’s size 
**                                       changed from HSEN 1 to 2.            
**                                      3)DCM-                                
**                                      DCMMISC register: MRKLSTRCHK bitfield 
**                                      is removed.                           
**                                      4)DCM_GPR-                            
**                                      -DCMROD3 register: CM7_1_LOCKUP,      
**                                       CM7_RCCU1_ALARM, CM7_RCCU2_ALARM,    
**                                       DMA_SYS_GSKT_ALARM,                  
**                                       DMA_PERIPH_GSKT_ALARM,               
**                                       DMA_AXBS_ALARM, QSPI_GSKT_ALARM,     
**                                       AIPS2_GSKT_ALARM, TCM_AXBS_ALARM,    
**                                       EMAC_GSKT_ALARM, PERIPH_AXBS_ALARM,  
**                                       PRAM1_ECC_ERR, CM7_1_DCDATA_ECC_ERR, 
**                                       CM7_1_DCTAG_ECC_ERR and              
**                                       CM7_1_ICDATA_ECC_ERR bitfields are   
**                                       removed.                             
**                                      -DCMROD4 register:                    
**                                       CM7_1_ICTAG_ECC_ERR,                      
**                                       CM7_1_ITCM_ECC_ERR,                  
**                                       CM7_1_DTCM0_ECC_ERR,                 
**                                       CM7_1_DTCM1_ECC_ERR,                 
**                                       PRAM1_FCCU_ALARM, PF2_CODE_ECC_ERR,  
**                                       PF2_DATA_ECC_ERR and FLASH_ECC_ERR   
**                                       bitfields are removed.               
**                                      -DCMROD5 register: TCM_RDATA_EDC_ERR, 
**                                       EMAC_RDATA_EDC_ERR,                  
**                                       CM7_1_AHBP_RDATA_EDC_ERR and         
**                                       CM7_1_AHBM_RDATA_EDC_ERR bitfields   
**                                       are removed.                         
**                                      -DCMROF1 register: All bitfields are  
**                                       removed.                             
**                                      -DCMROF19 register:	LOCKSTEP_EN       
**                                       bitfield is removed.                 
**                                      -DCMROF20 register: LMAUTO_DIS,       
**                                       CM7_TCM_WS_EN, DMA_AXBS_IAHB_BYP and 
**                                       QSPI_IAHB_BYP bitfields are removed. 
**                                      -DCMRWD3 register: CM7_1_LOCKUP_EN,   
**                                       CM7_RCCU1_ALARM_EN,                  
**                                       CM7_RCCU2_ALARM_EN,                  
**                                       DMA_SYS_GSKT_ALARM_EN,               
**                                       DMA_PERIPH_GSKT_ALARM_EN,            
**                                       DMA_AXBS_ALARM_EN,                   
**                                       QSPI_GSKT_ALARM_EN,                  
**                                       AIPS2_GSKT_ALARM_EN,                 
**                                       TCM_AXBS_ALARM_EN,                   
**                                       EMAC_GSKT_ALARM_EN,                  
**                                       PERIPH_AXBS_ALARM_EN,                
**                                       PRAM1_ECC_ERR_EN,                    
**                                       CM7_1_DCDATA_ECC_ERR_EN,             
**                                       CM7_1_DCTAG_ECC_ERR_EN and           
**                                       CM7_1_ICDATA_ECC_ERR_EN bitfields are
**                                       removed.                             
**                                      -DCMRWD4 register:                    
**                                       CM7_1_ICTAG_ECC_ERR_EN,              
**                                       CM7_1_ITCM_ECC_ERR_EN,               
**                                       CM7_1_DTCM0_ECC_ERR_EN,              
**                                       CM7_1_DTCM1_ECC_ERR_EN,              
**                                       PRAM1_FCCU_ALARM_EN,                 
**                                       PF2_CODE_ECC_ERR_EN,                 
**                                       PF2_DATA_ECC_ERR_EN and              
**                                       FLASH_ECC_ERR_EN bitfields are       
**                                       removed.                             
**                                      -DCMRWD5 register:                    
**                                       TCM_RDATA_EDC_ERR_EN,                
**                                       EMAC_RDATA_EDC_ERR_EN,               
**                                       CM7_1_AHBP_RDATA_EDC_ERR_EN and      
**                                       CM7_1_AHBM_RDATA_EDC_ERR_EN bitfields
**                                       are removed.                         
**                                      -DCMRWD6 register:                    
**                                       EMIOS2_DBG_DIS_CM7_0,                
**                                       SWT1_DBG_DIS_CM7_0,                  
**                                       STM1_DBG_DIS_CM7_0,                  
**                                       PIT2_DBG_DIS_CM7_0,                  
**                                       LPSPI4_DBG_DIS_CM7_0,                
**                                       LPSPI5_DBG_DIS_CM7_0,                
**                                       SAI0_DBG_DIS_CM7_0 and               
**                                       SAI1_DBG_DIS_CM7_0 bitfields are     
**                                       removed.                             
**                                      -DCMRWD8 register: All bitfields are  
**                                       removed.                             
**                                      -DCMRWD9 register: All bitfields are  
**                                       removed.                             
**                                      -DCMRWF1 register:	                  
**                                       CAN_TIMESTAMP_SEL, CAN_TIMESTAMP_EN, 
**                                       RMII_MII_SEL, VDD_HV_B_IO_CTRL_LATCH,
**                                       VDD_HV_B_VLT_DVDR_EN and             
**                                       VDD_1_5_VLT_DVDR_EN bitfields are    
**                                       removed.                             
**                                      -DCMRWF4 register:                    
**                                       MUX_MODE_EN_ADC2_S8,                 
**                                       MUX_MODE_EN_ADC2_S9 and              
**                                       CM7_1_CPUWAIT bitfields are          
**                                       removed.                             
**                                      5)I3C-                                
**                                      -SCONFIG register: DDROK bitfield is  
**                                       renamed as HDROK.                    
**                                      -SSTATUS register: HDRMATCH bitfield  
**                                       is renamed as HDRMATCHED.            
**                                      -SCAPABILITIES2 register: V1_1        
**                                       bitfield is removed.                 
**                                      6)LPI2C-                              
**                                      SRDR register: RADDR bitfield is      
**                                      added.                                
**                                      7)LPUART-                             
**                                      BAUD register: RIDMAE bitfield is     
**                                      removed.                              
**                                      8)MDM_AP-                             
**                                      -MDMAPSTTS register: CM71HLT,         
**                                       CM71DPSLP, CM71SLPNG and             
**                                       CM71DBGRSTRD bitfields are removed.  
**                                      -MDMAPCTL register:	CM71DBGREQ,       
**                                       DBGRSTSLOWPAD, DBGRSTFASTPAD,        
**                                       TRIUOVRD and CM71DBGRSRT bitfields   
**                                       are removed.                         
**                                      -MDMAPWIREN register: LWPRSTPRVT      
**                                       bitfield is removed.                 
**                                      9)SIUL2-                              
**                                      -UDR1 register:	                      
**                                       -uint16_t is changed to uint32_t.      
**                                       -UDR16, UDR17…UDR31 bitfields are   
**                                        added.                              
**                                      -UDR0 register:              	      
**                                       -uint16_t is changed to uint32_t.  
**                                       -UDR16, UDR17…UDR31 bitfields are    
**                                        added.                              
**                                      -UDR3 register:	                      
**                                       -uint16_t is changed to uint32_t.  
**                                       -UDR16, UDR17…UDR31 bitfields are    
**                                        added.                              
**                                      -UDR2 register:	                      
**                                       -uint16_t is changed to uint32_t.  
**                                       -UDR16, UDR17…UDR31 bitfields are    
**                                        added.                              
**                                      -UDR5, UDR4, UDR7 and UDR6 registers  
**                                       are removed.                         
**                                      10)XRDC-                              
**                                      PID register: LMNUM and ELK22H        
**                                      bitfields are removed.              
**
**     - rev. 1.3 Taru Shree 28-Oct-21  1)Initial release based on
**                                      RM Rev.3.
**                                      2)ADC-
**	                                    CTR1 register: 
**                                      -TSENSOR_SEL bitfield is removed.
**                                      -INPSAMP bitfield’s size is changed from 7 
**                                       to 8.
**                                      3)I3C-	
**                                      -SDYNADDR register is added.	
**                                      -SCONFIG register: HDROK bitfield is renamed 
**                                       as DDROK.
**                                      -SSTATUS register: HDRMATCHED bitfield is 
**                                       renamed as HDRMATCH.
**
** ###################################################################
*/

/*!
 * @file S32K312.h
 * @version 1.3
 * @date 2021-10-28
 * @brief Peripheral Access Layer for S32K312
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/* Prevention from multiple including the same memory map */
#if !defined(S32K312_H_)  /* Check if memory map has not been already included */
#define S32K312_H_

/* ----------------------------------------------------------------------------
   -- IP Header Files
   ---------------------------------------------------------------------------- */

/* IP Header Files List */
#include "S32K312_ADC.h"
#include "S32K312_BCTU.h"
#include "S32K312_CMU_FC.h"
#include "S32K312_CMU_FM.h"
#include "S32K312_CONFIGURATION_GPR.h"
#include "S32K312_CRC.h"
#include "S32K312_DCM.h"
#include "S32K312_DCM_GPR.h"
#include "S32K312_DMA.h"
#include "S32K312_DMAMUX.h"
#include "S32K312_DMA_TCD.h"
#include "S32K312_EIM.h"
#include "S32K312_EMIOS.h"
#include "S32K312_ERM.h"
#include "S32K312_FCCU.h"
#include "S32K312_FIRC.h"
#include "S32K312_FLASH.h"
#include "S32K312_FLEXCAN.h"
#include "S32K312_FLEXIO.h"
#include "S32K312_FXOSC.h"
#include "S32K312_I3C.h"
#include "S32K312_INTM.h"
#include "S32K312_JDC.h"
#include "S32K312_LCU.h"
#include "S32K312_LPCMP.h"
#include "S32K312_LPI2C.h"
#include "S32K312_LPSPI.h"
#include "S32K312_LPUART.h"
#include "S32K312_MCM_CM7.h"
#include "S32K312_MC_CGM.h"
#include "S32K312_MC_ME.h"
#include "S32K312_MC_RGM.h"
#include "S32K312_MDM_AP.h"
#include "S32K312_MSCM.h"
#include "S32K312_MU.h"
#include "S32K312_PFLASH.h"
#include "S32K312_PIT.h"
#include "S32K312_PLL.h"
#include "S32K312_PMC.h"
#include "S32K312_PRAMC.h"
#include "S32K312_RTC.h"
#include "S32K312_SDA_AP.h"
#include "S32K312_SIRC.h"
#include "S32K312_SIUL2.h"
#include "S32K312_STCU.h"
#include "S32K312_STM.h"
#include "S32K312_SWT.h"
#include "S32K312_SXOSC.h"
#include "S32K312_TEMPSENSE.h"
#include "S32K312_TRGMUX.h"
#include "S32K312_TSPC.h"
#include "S32K312_VIRT_WRAPPER.h"
#include "S32K312_WKPU.h"
#include "S32K312_XBIC.h"
#include "S32K312_XRDC.h"
#include "S32K312_NVIC.h"
#include "S32K312_SCB.h"
#include "S32K312_SYSTICK.h"
#include "S32K312_MPU.h"

#endif  /* #if !defined(S32K312_H_) */

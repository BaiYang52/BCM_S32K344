/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  BrsHw.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for platform NXP S32K3xx
 *     Template:  This file is reviewed according to Brs_Template@Implementation[1.03.05]
 *
 *  \brief Description:  This is the hardware specific code file for Vector Basic Runtime System (BRS).
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to BrsHw.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EXAMPLE CODE ONLY
 *  -------------------------------------------------------------------------------------------------------------------
 *  This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
 *  The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
 *  intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
 *  implementations must be tested with diligent care and must comply with all quality requirements which are necessary
 *  according to the state of the art before their use.
 *********************************************************************************************************************/

#define _BRSHW_C_
#define BRSHW_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsHw.h"

#if !defined (BRS_ENABLE_FBL_SUPPORT)
  #include "Os.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (BRSHW_VERSION != 0x0201u)
  #error "Header and source file are inconsistent!"
#endif
#if (BRSHW_BUGFIX_VERSION != 0x00u)
  #error "Different versions of bugfix in Header and Source used!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR) 

#else
  #error "Unknown compiler specified!"
#endif

/**********************************************************************************************************************
 *  DEFINITION + MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONST VARIABLES
 *********************************************************************************************************************/
/*
 * Description: These constants are used to propagate the Versions over
 *              module boundaries.The version numbers are BCD coded. E.g. a sub
 *              version of 23 is coded with 0x23, a bug fix version of 9 is
 *              coded 0x09.
 */
uint8 const kBrsHwMainVersion   = (uint8)(BRSHW_VERSION >> 8);
uint8 const kBrsHwSubVersion    = (uint8)(BRSHW_VERSION & 0xFF);
uint8 const kBrsHwBugfixVersion = (uint8)(BRSHW_BUGFIX_VERSION);

/**********************************************************************************************************************
 *  LOCAL VARIABLES AND LOCAL HW REGISTERS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CONTROLLER CONFIGURATION REGISTERS
 *********************************************************************************************************************/
#if defined (BRS_ENABLE_PLLCLOCKS)
# if defined (BRS_PLL_GROUP_B) || \
     defined (BRS_PLL_GROUP_C) || \
     defined (BRS_PLL_GROUP_D)
  /* Fast Crystal Oscillator Digital Controller (FXOSC) */
  #define FXOSC_BASEADDR  0x402D4000ul
  #define FXOSC_CTRL                       BRSHW_IOS(uint32, FXOSC_BASEADDR)
  #define FXOSC_STAT                       BRSHW_IOS(uint32, FXOSC_BASEADDR + 0x04ul)

  #define FXOSC_CLK_WAIT                   ((uint8)BRS_OSC_CLK * 16u)
  #define FXOSC_CTRL_CONFG_VAL             0x019D00D0ul
  #define FXOSC_CTRL_OSCON_EN              0
  #define FXOSC_STAT_OSC_STAT              31u

  /* PLL Digital Interface (PLLDIG) */
  struct PLLDIG_reg {
    uint32 PLLCR;  /* PLL Control Register */
    uint32 PLLSR;  /* PLL Status Register */
    uint32 PLLDV;  /* PLL Divider Register */
    uint32 PLLFM;  /* PLL Frequency Modulation Register */
    uint32 PLLFD;  /* PLL Fractional Divide Register */
  };

  #define CORE_PLL_BASEADDR  0x402E0000ul
  #define CORE_PLL                         (*(volatile struct PLLDIG_reg *) CORE_PLL_BASEADDR)
  #define CORE_PLL_PLLODIV_0               BRSHW_IOS(uint32, CORE_PLL_BASEADDR + 0x80ul)
  #define CORE_PLL_PLLODIV_1               BRSHW_IOS(uint32, CORE_PLL_BASEADDR + 0x84ul)

# if defined (BRS_DERIVATIVE_S32K358)
  #define CORE_PLL_AUX_BASEADDR  0x402E4000ul
  #define CORE_PLL_AUX                     (*(volatile struct PLLDIG_reg *) CORE_PLL_AUX_BASEADDR)
  #define CORE_PLL_AUX_PLLODIV_0           BRSHW_IOS(uint32, CORE_PLL_AUX_BASEADDR + 0x80ul)
  #define CORE_PLL_AUX_PLLODIV_1           BRSHW_IOS(uint32, CORE_PLL_AUX_BASEADDR + 0x84ul)
  #define CORE_PLL_AUX_PLLODIV_2           BRSHW_IOS(uint32, CORE_PLL_AUX_BASEADDR + 0x88ul)
# endif

  #define PLLDIG_PLLODIV_DE                31u
  #define PLLDIG_PLLCR_PLLPD               31u
  #define PLLDIG_PLLDV_ODIV2               25u
  #define PLLDIG_PLLDV_MFN_CLEAR_VAL       0x7FFFul

  /* Settings for 120MHz PLL for different OSC_CLKs */
# if (BRS_OSC_CLK == 40)
  #define PLLDIG_PLLDV_MFI_VAL             96
# elif (BRS_OSC_CLK == 16)
  #define PLLDIG_PLLDV_MFI_VAL             60
# elif (BRS_OSC_CLK == 8)
  #define PLLDIG_PLLDV_MFI_VAL             120
# endif /* BRS_OSC_CLK == xy */

  #define PLLDIG_PLLDV_RDIV                12u
  #define PLLDIG_PLLiDIV_DIV               16u
  #define PLLDIG_PLLSR_LOCK                2u
  #define PLLDIG_PLLDV_CLEAR               0x70FFul
  #define PLLDIG_ODIV2_CLEAR               0x00FFFFFFul

# if defined (BRS_DERIVATIVE_GROUP_S32K31X) || \
     defined (BRS_DERIVATIVE_GROUP_MWCT201XX)
  /*Set ODIV2 Divider for PLLODIV2_CLK == 240MHz*/
  #define PLLDIG_ODIV2_VAL                 0x4u

# elif defined (BRS_DERIVATIVE_GROUP_S32K32X) || \
       defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX) || \
       defined (BRS_DERIVATIVE_GROUP_S32K34X) || \
       defined (BRS_DERIVATIVE_GROUP_S32K3X8)
  /*Set ODIV2 Divider for PLLODIV2_CLK == 480MHz*/
  #define PLLDIG_ODIV2_VAL                 0x2u  

# elif defined (BRS_DERIVATIVE_GROUP_S32K38X) || \
       defined (BRS_DERIVATIVE_GROUP_S32K39X)
  /*Set ODIV2 Divider for PLLODIV2_CLK == 960MHz*/
  #define PLLDIG_ODIV2_VAL                 0x1u
#endif

  /* Mode Entry Module */
  #define MC_ME_BASE_ADDR  0x402DC000ul
  #define MC_ME_CTL_KEY                    BRSHW_IOS(uint32, MC_ME_BASE_ADDR)
  #define MC_ME_MODE_CONF                  BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x4ul)
  #define MC_ME_MODE_UPD                   BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x8ul)
  #define MC_ME_MODE_STAT                  BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0xCul)
  #define MC_ME_MAIN_COREID                BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x10ul)
  #define MC_ME_PRTN0_PCONF                BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x100ul)
  #define MC_ME_PRTN0_PUPD                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x104ul)
  #define MC_ME_PRTN0_STAT                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x108ul)
  #define MC_ME_PRTN0_COFB1_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x114ul)
  #define MC_ME_PRTN0_COFB1_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x134ul)
  #define MC_ME_PRTN0_CORE0_PCONF          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x140ul)
  #define MC_ME_PRTN0_CORE0_PUPD           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x144ul)
  #define MC_ME_PRTN0_CORE0_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x148ul)
  #define MC_ME_PRTN0_CORE0_ADDR           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x14Cul)
  #define MC_ME_PRTN0_CORE1_PCONF          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x160ul)
  #define MC_ME_PRTN0_CORE1_PUPD           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x164ul)
  #define MC_ME_PRTN0_CORE1_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x168ul)
  #define MC_ME_PRTN0_CORE1_ADDR           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x16Cul)
  #define MC_ME_PRTN0_CORE2_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x188ul)
  #define MC_ME_PRTN0_CORE2_ADDR           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x18Cul)
  #define MC_ME_PRTN1_PCONF                BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x300ul)
  #define MC_ME_PRTN1_PUPD                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x304ul)
  #define MC_ME_PRTN1_STAT                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x308ul)
  #define MC_ME_PRTN1_COFB0_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x310ul)
  #define MC_ME_PRTN1_COFB1_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x314ul)
  #define MC_ME_PRTN1_COFB2_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x318ul)
  #define MC_ME_PRTN1_COFB3_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x31Cul)
  #define MC_ME_PRTN1_COFB0_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x330ul)
  #define MC_ME_PRTN1_COFB1_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x334ul)
  #define MC_ME_PRTN1_COFB2_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x338ul)
  #define MC_ME_PRTN1_COFB3_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x33Cul)
  #define MC_ME_PRTN2_PCONF                BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x500ul)
  #define MC_ME_PRTN2_PUPD                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x504ul)
  #define MC_ME_PRTN2_STAT                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x808ul)
  #define MC_ME_PRTN2_COFB0_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x510ul)
  #define MC_ME_PRTN2_COFB1_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x514ul)
  #define MC_ME_PRTN2_COFB2_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x518ul)
  #define MC_ME_PRTN2_COFB0_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x530ul)
  #define MC_ME_PRTN2_COFB1_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x534ul)
  #define MC_ME_PRTN2_COFB2_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x538ul)

# if defined (BRS_DERIVATIVE_GROUP_S32K38X) || defined (BRS_DERIVATIVE_GROUP_S32K39X)
  #define MC_ME_PRTN3_PCONF                BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x700ul)
  #define MC_ME_PRTN3_PUPD                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x704ul)
  #define MC_ME_PRTN3_STAT                 BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x708ul)
  #define MC_ME_PRTN3_COFB0_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x710ul)
  #define MC_ME_PRTN3_COFB1_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x714ul)
  #define MC_ME_PRTN3_COFB2_STAT           BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x718ul)
  #define MC_ME_PRTN3_COFB0_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x730ul)
  #define MC_ME_PRTN3_COFB1_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x734ul)
  #define MC_ME_PRTN3_COFB2_CLKEN          BRSHW_IOS(uint32, MC_ME_BASE_ADDR + 0x738ul)
# endif

  /* Clock Generation Module (MC_CGM) */
  #define MC_CGM_0_BASEADDR  0x402D8000ul
  #define MC_CGM_0_PCFS_SDUR               BRSHW_IOS(uint32, MC_CGM_0_BASEADDR)
  #define MC_CGM_0_PCFS_DIVC8              BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x58ul)  /* offset calculated as register missing in manual, 8*C-8 */
  #define MC_CGM_0_PCFS_DIVE8              BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x5Cul)  /* offset calculated as register missing in manual, 8*C-4 */
  #define MC_CGM_0_PCFS_DIVS8              BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x60ul)  /* offset calculated as register missing in manual, 8*C */
  #define MC_CGM_0_PCFS_DIVC22             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x100ul) /* offset calculated as register missing in manual, 0x16*C-8 */
  #define MC_CGM_0_PCFS_DIVE22             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x104ul) /* offset calculated as register missing in manual, 0x16*C-4 */
  #define MC_CGM_0_PCFS_DIVS22             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x108ul) /* offset calculated as register missing in manual, 0x16*C */
  #define MC_CGM_0_PCFS_DIVC23             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x10Cul) /* offset calculated as register missing in manual, 0x17*C-8 */
  #define MC_CGM_0_PCFS_DIVE23             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x110ul) /* offset calculated as register missing in manual, 0x17*C-4 */
  #define MC_CGM_0_PCFS_DIVS23             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x114ul) /* offset calculated as register missing in manual, 0x17*C */
  #define MC_CGM_0_PCFS_DIVC24             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x118ul) /* offset calculated as register missing in manual, 0x18*C-8 */
  #define MC_CGM_0_PCFS_DIVE24             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x11Cul) /* offset calculated as register missing in manual, 0x18*C-4 */
  #define MC_CGM_0_PCFS_DIVS24             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x120ul) /* offset calculated as register missing in manual, 0x18*C */
  #define MC_CGM_0_PCFS_DIVC25             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x124ul) /* offset calculated as register missing in manual, 0x19*C-8 */
  #define MC_CGM_0_PCFS_DIVE25             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x128ul) /* offset calculated as register missing in manual, 0x19*C-4 */
  #define MC_CGM_0_PCFS_DIVS25             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x12Cul) /* offset calculated as register missing in manual, 0x19*C */
  #define MC_CGM_0_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x300ul)
  #define MC_CGM_0_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x304ul)
  #define MC_CGM_0_MUX_0_DC(n)             BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x308ul + (n * 0x4ul))
  #define MC_CGM_0_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x33Cul)
  #define MC_CGM_1_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x340ul)
  #define MC_CGM_1_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x344ul)
  #define MC_CGM_1_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x348ul)
  #define MC_CGM_1_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x37Cul)
  #define MC_CGM_3_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x3C0ul)
  #define MC_CGM_3_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x3C4ul)
  #define MC_CGM_3_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x3C8ul)
  #define MC_CGM_3_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x3FCul)
  #define MC_CGM_4_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x400ul)
  #define MC_CGM_4_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x404ul)
  #define MC_CGM_4_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x408ul)
  #define MC_CGM_4_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x43Cul)
  #define MC_CGM_5_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x440ul)
  #define MC_CGM_5_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x444ul)
  #define MC_CGM_5_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x448ul)
  #define MC_CGM_5_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x47Cul)
  #define MC_CGM_6_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x480ul)
  #define MC_CGM_6_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x484ul)
  #define MC_CGM_6_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x488ul)
  #define MC_CGM_6_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x4BCul)
  
# if !defined (BRS_PLL_GROUP_C)
  #define MC_CGM_2_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x380ul)
  #define MC_CGM_2_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x384ul)
  #define MC_CGM_2_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x388ul)
  #define MC_CGM_2_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x3BCul)
  #define MC_CGM_7_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x4C0ul)
  #define MC_CGM_7_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x4C4ul)
  #define MC_CGM_7_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x4C8ul)
  #define MC_CGM_7_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x4FCul)
  #define MC_CGM_8_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x500ul)
  #define MC_CGM_8_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x504ul)
  #define MC_CGM_8_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x508ul)
  #define MC_CGM_8_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x53Cul)
  #define MC_CGM_9_MUX_CSC                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x540ul)
  #define MC_CGM_9_MUX_CSS                 BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x544ul)
  #define MC_CGM_9_MUX_DC_1                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x548ul)
  #define MC_CGM_9_MUX_DIV_UPD_STAT        BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x57Cul)
  #define MC_CGM_14_MUX_CSC                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x680ul)
  #define MC_CGM_14_MUX_CSS                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x684ul)
  #define MC_CGM_14_MUX_DC_6               BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6A0ul)
  #define MC_CGM_14_MUX_DIV_UPD_STAT       BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6BCul)
  #define MC_CGM_15_MUX_CSC                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6C0ul)
  #define MC_CGM_15_MUX_CSS                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6C4ul)
  #define MC_CGM_15_MUX_DC_6               BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6E0ul)
  #define MC_CGM_15_MUX_DIV_UPD_STAT       BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x6FCul)
  #define MC_CGM_16_MUX_CSC                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x700ul)
  #define MC_CGM_16_MUX_CSS                BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x704ul)
  #define MC_CGM_16_MUX_DC_6               BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x720ul)
  #define MC_CGM_16_MUX_DIV_UPD_STAT       BRSHW_IOS(uint32, MC_CGM_0_BASEADDR + 0x73Cul)
# endif

  #define MC_CGM_MUX_CSC_CLK_SW            2u
  #define MC_CGM_MUX_CSC_SELCTL            24u  
  #define MC_CGM_MUX_CSS_SWIP              16u
  #define MC_CGM_0_MUX_DC_DE               31u
  #define MC_CGM_0_MUX_DC_DIV              16u  
  #define MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK (uint32)1u 
  #define MC_CGM_0_PCFS_DIVCx_CHG_VAL      0x03E70000ul
  #define MC_CGM_0_PCFS_DIVx_INIT_VAL      0x000003E7ul

# else
  #error "Unknown BRS_PLL_GROUP configured. Check the Cfg5 BRS PreConfig for your Derivative."
# endif /*BRS_PLL_GROUP_x*/
#endif /*BRS_ENABLE_PLLCLOCKS*/

#if defined (BRS_ENABLE_PORT)
# if defined (BRS_PORT_GROUP_B) /*S32K3xx*/
  /* System Integration Unit Lite2 (SIUL2) */
  #define SIUL2_BASEADDR  0x40290000ul
  /* MSCR0 - MSCR219 */
  #define SIUL2_MSCR(r)              BRSHW_IOS(uint32, (SIUL2_BASEADDR + 0x0240ul + (r * 0x04ul)))
  /* IMCR0 - IMCR408 */
  #define SIUL2_IMCR(r)              BRSHW_IOS(uint32, (SIUL2_BASEADDR + 0x0A40ul + (r * 0x04ul)))
  /* GPDO0 - GPDO219 */
  #define SIUL2_GPDO(p,n)            BRSHW_IOS(uint8, (SIUL2_BASEADDR + 0x1300ul + ((p * 32u + n) ^ 3u)))
  /* GPDI0 - GPDI219 */
  #define SIUL2_GPDI(p,n)            BRSHW_IOS(uint8, (SIUL2_BASEADDR + 0x1500ul + ((p * 32u + n) ^ 3u)))

  #define SIUL2_MSCR_SSS_MASK        (uint32)0x00000007ul
  #define SIUL2_IMCR_SSS_MASK        (uint32)0x00000007ul
  #define SIUL2_MSCR_SMC             5u
  #define SIUL2_MSCR_PUS             11u
  #define SIUL2_MSCR_PUE             13u
  #define SIUL2_MSCR_SRE             14u
  #define SIUL2_MSCR_IBE             19u
  #define SIUL2_MSCR_OBE             21u

# else
  #error "Unknown BRS_PORT_GROUP configured. Check the Cfg5 BRS PreConfig for your Derivative."
# endif /*BRS_PORT_GROUP_x*/
#endif /*BRS_ENABLE_PORT*/

#if defined (BRS_RESET_GROUP_B)
  #define BRSHW_MC_RGM_BASE  0x4028C000uL
  /* Destructive resets */
  #define BRSHW_MC_RGM_DES                    BRSHW_IOS(uint32, (BRSHW_MC_RGM_BASE + 0x0u))
  #define BRSHW_MC_RGM_DES_RESET_MASK         (0x6006D759uL)
  #define BRSHW_MC_RGM_DES_RESET_WO_POR_MASK  (0x6006D758uL)
  /* Functional resets */
  #define BRSHW_MC_RGM_FES                    BRSHW_IOS(uint32, (BRSHW_MC_RGM_BASE + 0x8u))
  #define BRSHW_MC_RGM_FES_SW_FUNC            (0x601102D9uL)
  #define BRSHW_MC_RGM_FES_RESET_MASK         (0x601102D9uL)

#else
   #error "Unknown BRS_RESET_GROUP configured. Check the Cfg5 BRS PreConfig for your Derivative."
#endif /*BRS_RESET_GROUP_x*/

#if defined (BRS_DERIVATIVE_GROUP_S32K31X) || \
    defined (BRS_DERIVATIVE_GROUP_MWCT201XX) || \
    defined (BRS_DERIVATIVE_GROUP_S32K32X) || \
    defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX) || \
    defined (BRS_DERIVATIVE_GROUP_S32K34X) || \
    defined (BRS_DERIVATIVE_GROUP_S32K3X8) || \
    defined (BRS_DERIVATIVE_GROUP_S32K38X) || \
    defined (BRS_DERIVATIVE_GROUP_S32K39X)
  /* PFLASH */
  #define BRSHW_PFCR_BASE  0x40268000uL
  #define BRSHW_PFCR4                         BRSHW_IOS(uint32, (BRSHW_PFCR_BASE + 0x10uL))
  #define BRSHW_PFCR4_ECC_DISABLE_MASK        (0x00000001uL)
  
  /* Miscellaneous System Control Module (MSCM) */
  #define MSCM_BASEADDR                      0x40260000ul
  /* Processor x Number Register. */
  #define MSCM_CPUXNUM                       BRSHW_IOS(uint32, MSCM_BASEADDR + 0x04ul)
#else
  #error "Disable ECC not implemented or available for this derivative."
#endif /*BRS_DERIVATIVE_x*/

/**********************************************************************************************************************
 *  LOCAL VARIABLES
 *********************************************************************************************************************/
typedef volatile const struct
{
  const unsigned int  Header;                     /* Header of boot header structure */
  const unsigned int  BootConfig;                 /* Boot Configuration Word */
  const unsigned int  Reserved3;                  /* Reserved */
  const unsigned int* CM7_0_StartAddress;         /* Start address of application on CM7_0 core */
  const unsigned int  Reserved4;                  /* Reserved */
  const unsigned int* CM7_1_StartAddress;         /* Start address of application on CM7_1 core */
  const unsigned int  Reserved5;                  /* Reserved */
  const unsigned int* CM7_2_StartAddress;         /* Start address of application on CM7_2 core */
  const unsigned int* XRDCConfig_StartAddress;    /* Address of XRDC configuration data */
  const unsigned int* LCConfig;                   /* Address of LC configuration */
  const unsigned int  Reserved1;                  /* Reserved */
  const unsigned int* HseFwHeader_StartAddress;   /* Start address of HSE-FW image */
  const unsigned char Reserved[192];              /* Reserved for future use */
  const unsigned char CMAC[16];                   /* CMAC */
} IVTField;

typedef struct
{
  unsigned int    Header;
  unsigned int    MDAConfig_ProcessorCore0;
  unsigned int    MDAConfig_eDMA_AHB;
  unsigned int    MDAConfig_TestPort_AHB;
  unsigned int    MDAConfig_ProcessorCore1;
  unsigned int    MDAConfig_ENET_AHB;
  unsigned int    Reserved0[10];
  unsigned int    PDAC[14][2];
  unsigned int    Reserved2[20];
  unsigned char   CMAC[16];
} xrdc_config_t;

typedef unsigned int lc_config_t;

/******************************************************************************
 * LC Configuration
 ******************************************************************************/
const lc_config_t lc_config = 0xffffffff;

/******************************************************************************
 * XRDC Configuration
 ******************************************************************************/
const xrdc_config_t xrdc_config =
{
  .Header = 0xffffffff
};

#if defined (BRS_COMP_IAR)
extern void Brs_ExcVect$$Base(void);
#endif

/**********************************************************************************************************************
 *  LOCAL CONST VARIABLES
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)
#pragma location = ".IVTField"
#endif

#if !defined (BRS_COMP_IAR)
volatile const IVTField IVTField0 = {
#else
__root const IVTField IVTField0 = {
#endif
  .Header = 0x5AA55AA5,
#if defined (BRS_DERIVATIVE_GROUP_S32K31X) || \
    defined (BRS_DERIVATIVE_GROUP_MWCT201XX)
  .BootConfig = 0x00000001, /*Enable CM7_0 core*/
#elif defined (BRS_DERIVATIVE_GROUP_S32K32X) || \
      defined (BRS_DERIVATIVE_GROUP_S32K34X) || \
      defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX)
  .BootConfig = 0x00000003, /*Enable CM7_0 & CM7_1 core*/
#elif defined (BRS_DERIVATIVE_GROUP_S32K3X8) || \
      defined (BRS_DERIVATIVE_GROUP_S32K38X) || \
      defined (BRS_DERIVATIVE_GROUP_S32K39X)
  .BootConfig = 0x00000007, /*Enable CM7_0, CM7_1 core & CM7_2 core*/
#endif

#if defined (BRS_COMP_IAR)
  .CM7_0_StartAddress = (void *)Brs_ExcVect$$Base,
  .CM7_1_StartAddress = (void *)Brs_ExcVect$$Base,
  .CM7_2_StartAddress = (void *)Brs_ExcVect$$Base,
#endif
  .XRDCConfig_StartAddress  = (const unsigned int*)&xrdc_config,
  .LCConfig                 = (const unsigned int*)&lc_config,
  .HseFwHeader_StartAddress = (const unsigned int*)0U,
};


/**********************************************************************************************************************
 *  PROTOTYPES OF GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* part of ArmCommon */

/**********************************************************************************************************************
 *  PROTOTYPES OF LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)
/*coreRegisterInit() is part of ARMStartup_CortexM.c. Declaration here is necessary for IAR compiler*/
  extern __task void coreRegisterInit(void);
#endif

/**********************************************************************************************************************
 *  FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 /*Enabling of MSCM clock at beginning of startup code*/
#if !defined (BRS_COMP_IAR)
  BRS_SECTION_CODE(brsStartup)
  BRS_GLOBAL(brsPreAsmStartupHook)
#else
  #pragma location = ".brsStartup"
__stackless void brsPreAsmStartupHook (void)
{
#endif
  BRS_MULTILINE_ASM_BEGIN()
  BRS_LABEL(brsPreAsmStartupHook)
  /*Check if clock for MSCM module has already been enabled*/
  __as1(ldr r0, =0x402DC310)
  __as1(ldr r1, [r0])
  __as1(ldr r2, =(1 << 24))
  __as2(and r1, r1, r2)
#if !defined (BRS_COMP_IAR)
  BRS_BRANCH_NOT_EQUAL(R1,0,coreRegisterInit)
#else
  BRS_BRANCH_NOT_EQUAL(R1,#0,EndOfInit)
#endif

  /*Enable clock for MSCM module in PRTN1_COFB0_CLKEN register of MC_ME module*/
  __as1(ldr r0, =0x402DC330)
  __as1(ldr r1, [r0])
  __as1(ldr r2, =(1 << 24))
  __as2(orr r1, r1, r2)
  __as1(str r1, [r0])

  /*Write PRTN1_PUPD register to trigger the hardware process*/
  __as1(ldr r0, =0x402DC304)
  __as1(ldr r1, [r0])
  __as1(ldr r2, =1)
  __as2(orr r1, r1, r2)
  __as1(str r1, [r0])

  /*Write Key to CTL_KEY register to trigger changes*/
  __as1(ldr r0, =0x402DC000)
  __as1(ldr r1, =0x5AF0)
  __as1(str r1, [r0])
  __as1(ldr r1, =0xA50F)
  __as1(str r1, [r0])

  /*Check PRTN1_COFB0_STAT and wait until clock is gated*/
  BRS_LABEL(WaitForMSCMClock)
  __as1(ldr r0, =0x402DC310)
  __as1(ldr r1, [r0])
  __as1(ldr r2, =(1 << 24))
  __as2(and r1, r1, r2)
  BRS_BRANCH_EQUAL(R1,#0,WaitForMSCMClock)
#if !defined (BRS_COMP_IAR)
  BRS_BRANCH(coreRegisterInit)
  BRS_MULTILINE_ASM_END()
  BRS_GLOBAL_END()
  BRS_SECTION_CODE(text)
#else
  BRS_LABEL(EndOfInit)
  BRS_MULTILINE_ASM_END()
  BRS_EXTERN_BRANCH(coreRegisterInit)
}
#endif

#if defined (BRS_ENABLE_WATCHDOG)
/*****************************************************************************/
/* @brief      This function must be used to initialize the Watchdog.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwWatchdogInitPowerOn(void)
{
  /* Nothing to be done, watchdog disabled after reset. */
}
#endif /*BRS_ENABLE_WATCHDOG*/

#if defined (BRS_ENABLE_PLLCLOCKS)
void BrsHwModeEntryConfig(void)
{
#if defined (BRS_PLL_GROUP_B)
  /*Disable Core 1 clock */
  MC_ME_PRTN0_CORE1_PCONF &= ~(1 << 0);
  /*Trigger the update*/
  MC_ME_PRTN0_CORE1_PUPD |= (1 << 0);
  /*Write CTL Key Register to start the update process*/
  MC_ME_CTL_KEY &= 0x5AF0;
  MC_ME_CTL_KEY &= 0xA50F;
  /*Wait until update process is finished*/
  while(MC_ME_PRTN0_CORE1_PUPD & (1 << 0));
  /*Is Core1 clock still active? */
  while(MC_ME_PRTN0_CORE1_STAT & (1 << 0));
#elif defined (BRS_PLL_GROUP_C)
  /*Disable Core 0 clock */
  MC_ME_PRTN0_CORE0_PCONF &= ~(1 << 0);
  /*Trigger the update*/
  MC_ME_PRTN0_CORE0_PUPD |= (1 << 0);
  /*Write CTL Key Register to start the update process*/
  MC_ME_CTL_KEY &= 0x5AF0;
  MC_ME_CTL_KEY &= 0xA50F;
  /*Wait until update process is finished*/
  BRSHWNOP10();
  while(MC_ME_PRTN0_CORE0_PUPD & (1 << 0));
  /*Is Core0 clock still active? */
  BRSHWNOP10();
  while(MC_ME_PRTN0_CORE0_STAT & (1 << 0));
#endif  
  /* Enable IP clock */
  MC_ME_PRTN0_PCONF |= (1 << 0);
  /* Enable IP clock */
  MC_ME_PRTN1_PCONF |= (1 << 0);
# if !defined (BRS_PLL_GROUP_C)
  /* Enable IP clock */
  MC_ME_PRTN2_PCONF |= (1 << 0);
# endif
  /* Enable BCTU */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 1);
  /* Enable eMIOS0 */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 2);
  /* Enable eMIOS1 */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 3);
  /* Enable eMIOS2 */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 4);
  /* Enable PIT0 */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 12);
  /* Enable PIT1 */
  MC_ME_PRTN0_COFB1_CLKEN |= (1 << 13);
  /* Enable System crossbar switch*/
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 0);
  /* Enable System AXBS */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 1);
  /* Enable Peripheral AXBS */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 2);
  /* Enable ERM */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 23);
  /* Enable MSCM */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 24);
  /* Enable RAM Controller 0 */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 25);
  /* Enable Flash Controller */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 26);
  /* Enable Software Watchdog 0 Controller */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 28);
  /* Enable System Timer Module 0 */
  MC_ME_PRTN1_COFB0_CLKEN |= (1 << 29);
  /* Enable Interrupt Monitor */
  MC_ME_PRTN1_COFB0_CLKEN |= (1u << 31);
  /* Enable Reset generation module */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 3);
  /* Enable SIUL_VIRTWRAPPER_PDAC0 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 4);
  /* Enable SIUL_VIRTWRAPPER_PDAC0 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 5);
  /* Enable SIUL_VIRTWRAPPER_PDAC1 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 6);
  /* Enable SIUL_VIRTWRAPPER_PDAC1 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 7);
  /* Enable SIUL_VIRTWRAPPER_PDAC2 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 8);
  /* Enable SIUL_VIRTWRAPPER_PDAC2 */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 9);
  /* Enable SIUL_VIRTWRAPPER */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 10);
  /* Enable System Status and Configuration module */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 11);
  /* Enable 8-40 MHz Fast External Crystal Oscillator */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 12);
  /* Enable FXOSC Module */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 21);
  /* Enable Clock Generation Module */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 22);
  /* Enable Mode Entry Module */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 23);
  /* Enable peripheral clock for PLL */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 24);
  /* Enable Power Management Controller */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 26);
  /* Enable Flash Memory */
  MC_ME_PRTN1_COFB1_CLKEN |= (1 << 27);

# if defined (BRS_ENABLE_CAN_CHANNEL_0)
  /* Enable FlexCAN0 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 1);
# endif
# if defined (BRS_ENABLE_CAN_CHANNEL_1)
  /* Enable FlexCAN1 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 2);
# endif
# if defined (BRS_ENABLE_CAN_CHANNEL_2)
  /* Enable FlexCAN2 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 3);
# endif
# if defined (BRS_ENABLE_CAN_CHANNEL_3)
  /* Enable FlexCAN3 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 4);
# endif
# if defined (BRS_ENABLE_CAN_CHANNEL_4)
  /* Enable FlexCAN4 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 5);
# endif
# if defined (BRS_ENABLE_CAN_CHANNEL_5)
  /* Enable FlexCAN5 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 6);
# endif

# if defined (BRS_ENABLE_LIN_CHANNEL_0)
  /* Enable LINFlex0 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 10);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_1)
  /* Enable LINFlex1 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 11);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_2)
  /* Enable LINFlex2 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 12);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_3)
  /* Enable LINFlex3 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 13);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_4)
  /* Enable LINFlex4 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 14);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_5)
  /* Enable LINFlex5 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 15);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_6)
  /* Enable LINFlex6 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 16);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_7)
  /* Enable LINFlex7 clock */
  MC_ME_PRTN1_COFB2_CLKEN |= (1 << 17);
# endif

# if !defined (BRS_PLL_GROUP_C)
# if defined (BRS_ENABLE_LIN_CHANNEL_8)
  /* Enable LINFlex8 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 3);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_9)
  /* Enable LINFlex9 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 4);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_10)
  /* Enable LINFle10 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 5);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_11)
  /* Enable LINFlex11 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 6);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_12)
  /* Enable LINFlex12 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 7);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_13)
  /* Enable LINFlex13 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 8);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_14)
  /* Enable LINFlex14 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 9);
# endif
# if defined (BRS_ENABLE_LIN_CHANNEL_15)
  /* Enable LINFlex15 clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 10);
# endif

# if defined (BRS_ENABLE_ETHERNET_SUPPORT)
  /* Enable Ethernet (EMAC) clock */
  MC_ME_PRTN2_COFB1_CLKEN |= (1 << 0);
# endif
#endif

  /* Trigger update */
  MC_ME_PRTN0_PUPD |= (1 << 0);
  MC_ME_PRTN1_PUPD |= (1 << 0);
# if !defined (BRS_PLL_GROUP_C)
  MC_ME_PRTN2_PUPD |= (1 << 0);
# endif
# if defined (BRS_DERIVATIVE_GROUP_S32K38X) || defined (BRS_DERIVATIVE_GROUP_S32K39X)
  MC_ME_PRTN3_PUPD |= (1 << 0);
# endif

  /*Write CTL Key Register to start the update process*/
  MC_ME_CTL_KEY &= 0x5AF0;
  MC_ME_CTL_KEY &= 0xA50F;
    
  /*Wait until update process is finished*/
# if defined (BRS_DERIVATIVE_GROUP_S32K31X) || defined (BRS_DERIVATIVE_GROUP_MWCT201XX)
  while((MC_ME_PRTN0_PUPD & (1 << 0)) || (MC_ME_PRTN1_PUPD & (1 << 0)));
# elif defined (BRS_DERIVATIVE_GROUP_S32K32X) || defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX) || \
       defined (BRS_DERIVATIVE_GROUP_S32K34X) || defined (BRS_DERIVATIVE_GROUP_S32K3X8)
  while((MC_ME_PRTN0_PUPD & (1 << 0)) || (MC_ME_PRTN1_PUPD & (1 << 0)) || (MC_ME_PRTN2_PUPD & (1 << 0)));
# elif defined (BRS_DERIVATIVE_GROUP_S32K38X) || defined (BRS_DERIVATIVE_GROUP_S32K39X)
  while((MC_ME_PRTN0_PUPD & (1 << 0)) || (MC_ME_PRTN1_PUPD & (1 << 0)) || (MC_ME_PRTN2_PUPD & (1 << 0)) || (MC_ME_PRTN3_PUPD & (1 << 0)));
# endif
}

/*****************************************************************************/
/* @brief      This function must be used to initialize the PLL.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPllInitPowerOn(void)
{
#if (BRS_TIMEBASE_CLOCK > BRS_CPU_MAX_FREQUENCY)
  #error "The selected derivative does not support frequencies above BRS_CPU_MAX_FREQUENCY (please find the defined value within your vBaseEnv preconfig)!"
#endif

#if ((BRS_OSC_CLK != 8) && (BRS_OSC_CLK != 16) && (BRS_OSC_CLK != 40))
 #error "Only 8MHz, 16MHz or 40MHz as oscillator frequencies are actually supported. Please configure a supported value in Cfg5"
#endif

#if defined (BRS_DERIVATIVE_GROUP_S32K31X) || \
    defined (BRS_DERIVATIVE_GROUP_MWCT201XX)
# if (BRS_TIMEBASE_CLOCK != 120)
  #error "Only 120MHz as TIMEBASE_CLOCK supported by the configured derivative. Please configure a supported value in Cfg5"
# endif
# if (BRS_PERIPH_CLOCK != 120)
  #error "Only 120MHz as PERIPHERAL_CLOCK supported by the configured derivative. Please configure a supported value equal to TIMEBASE_CLOCK in Cfg5"
# endif

#elif defined (BRS_DERIVATIVE_GROUP_S32K32X)   || \
      defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX) || \
      defined (BRS_DERIVATIVE_GROUP_S32K34X)
# if ((BRS_TIMEBASE_CLOCK != 120) && (BRS_TIMEBASE_CLOCK != 160))
  #error "Only 120MHz or 160MHz as TIMEBASE_CLOCK supported by the configured derivative. Please configure a supported value in Cfg5"
# endif
# if ((BRS_PERIPH_CLOCK != 120) && (BRS_PERIPH_CLOCK != 160))
  #error "Only 120MHz or 160MHz as PERIPHERAL_CLOCK supported by the configured derivative. Please configure a supported value equal to TIMEBASE_CLOCK in Cfg5"
# endif

#elif defined (BRS_DERIVATIVE_GROUP_S32K3X8)
# if ((BRS_TIMEBASE_CLOCK != 120) && (BRS_TIMEBASE_CLOCK != 160) && (BRS_TIMEBASE_CLOCK != 240))
  #error "Only 120MHz, 160MHz or 240MHz as TIMEBASE_CLOCK supported by the configured derivative. Please configure a supported value in Cfg5"
# endif
# if ((BRS_PERIPH_CLOCK != 120) && (BRS_PERIPH_CLOCK != 160) && (BRS_PERIPH_CLOCK != 240))
  #error "Only 120MHz, 160MHz or 240MHz as PERIPHERAL_CLOCK supported by the configured derivative. Please configure a supported value equal to TIMEBASE_CLOCK in Cfg5"
# endif

#elif defined (BRS_DERIVATIVE_GROUP_S32K38X) || \
      defined (BRS_DERIVATIVE_GROUP_S32K39X)
# if ((BRS_TIMEBASE_CLOCK != 120) && (BRS_TIMEBASE_CLOCK != 160) && (BRS_TIMEBASE_CLOCK != 240) && (BRS_TIMEBASE_CLOCK != 320))
  #error "Only 120MHz, 160MHz, 240MHz or 320MHz as TIMEBASE_CLOCK supported by the configured derivative. Please configure a supported value in Cfg5"
# endif
# if ((BRS_PERIPH_CLOCK != 120) && (BRS_PERIPH_CLOCK != 160) && (BRS_PERIPH_CLOCK != 240) && (BRS_PERIPH_CLOCK != 320))
  #error "Only 120MHz, 160MHz, 240MHz or 320MHz as PERIPHERAL_CLOCK supported by the configured derivative. Please configure a supported value equal to TIMEBASE_CLOCK in Cfg5"
# endif
#endif

  uint32 i;

  BrsHwModeEntryConfig();

  /* 1.0 Set FXOSC to power down mode. */
  FXOSC_CTRL &= ~(1 << FXOSC_CTRL_OSCON_EN);

  /* 1.1 Wait until FXOSC is disabled. */
  while(FXOSC_STAT & ~(1UL << FXOSC_STAT_OSC_STAT));

  /* 1.2 Set GM_SE and EOCV to 0.776 uA/V and 157 counts respectively. Select FXOSC operation mode. */
  for(i = 0u; i < FXOSC_CLK_WAIT; i++)
  {
    __asm("NOP");
  }
  FXOSC_CTRL = FXOSC_CTRL_CONFG_VAL;

  /* 1.3 Enable FXOSC in crystal clock operation mode. */
  FXOSC_CTRL |= (1 << FXOSC_CTRL_OSCON_EN);

  /* 1.4 Wait for stable FXOSC clocking. */
  while(!(FXOSC_STAT & (1UL << FXOSC_STAT_OSC_STAT)));

  /* 2.0.0 Select FIRC_CLK as the source clock for Cortex-M cores (CORE_CLK). */
  MC_CGM_0_MUX_CSC &= ~(0x0Ful << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 2.0.1 Request clock switching. */
  MC_CGM_0_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 2.0.2 Wait until clock source switching is completed. */
  while(MC_CGM_0_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));

  /* 2.1.5 Disable CORE_PLL divider #0 */
  /* PLLDIG_PLLODIV_0 is Reserved on S32K2TV */
  CORE_PLL_PLLODIV_0 &= ~(1ul << PLLDIG_PLLODIV_DE);

  /* 2.1.5 Disable CORE_PLL divider #1 */
  CORE_PLL_PLLODIV_1 &= ~(1ul << PLLDIG_PLLODIV_DE);

  /* 2.1.1 Disable analog PLL block of CORE_PLL. */
  CORE_PLL.PLLCR |= (1ul << PLLDIG_PLLCR_PLLPD);
  
# if defined (BRS_DERIVATIVE_S32K358)
  /* Power down PLL_AUX */
  CORE_PLL_AUX.PLLCR |= (1 << PLLDIG_PLLCR_PLLPD);
# endif
  
  /*Make sure that PLL fractional mode is disabled (only integer mode needed)*/
  CORE_PLL.PLLFD &= ~(0x7 << 28);
  
  /*Make sure that frequency modulation is bypassed*/
  CORE_PLL.PLLFM |= (1 << 30);

  /* CORE_PLL frequency is configured according to the following equation:
   * f(pll_VCO)  = f(pll_ref)/PLLDV[RDIV] * (PLLDV[MFI] + PLLFD[MFN]/18432)
   * f(pll_VCO)  = 960MHz for all derivative groups
   * f(pll_ref)  = 16MHz (FXOSC_CLK)
   * PLLDV[RDIV] = 1
   * PLLFD[MFN]  = 0 (for integer mode)
   * PLLDV[MFI]  = 60 (For FXOSC_CLK == 16MHz)
   * Other values for MFI not mentioned here, supported OSC_CLKs are, 8, 16 and 40 MHz.
   */
  /* 2.1.2 Set VCO frequency to 960MHz. */
  CORE_PLL.PLLDV &= ~(uint32)PLLDIG_PLLDV_CLEAR;
# if ((BRS_OSC_CLK == 8) || (BRS_OSC_CLK == 16))
  CORE_PLL.PLLDV |= (PLLDIG_PLLDV_MFI_VAL | (1ul << PLLDIG_PLLDV_RDIV));
# elif (BRS_OSC_CLK == 40)
  CORE_PLL.PLLDV |= (PLLDIG_PLLDV_MFI_VAL | (4ul << PLLDIG_PLLDV_RDIV));
# endif
  
  /*Make sure that MFN value is set to 0*/
  CORE_PLL.PLLFD &= ~PLLDIG_PLLDV_MFN_CLEAR_VAL;

  /* 2.1.3 Set Output frequency divider for PLL clock. */
  /*Clear ODIV2 value*/
  CORE_PLL.PLLDV &= PLLDIG_ODIV2_CLEAR;
  /*Write ODIV2 value*/
  CORE_PLL.PLLDV |= (PLLDIG_ODIV2_VAL << PLLDIG_PLLDV_ODIV2);

  /* 2.1.4 Set PLL Output Divider. */
  /*F[VCO] = F[OSC_CLK] * (PLLDV[MFI] / PLLDV[RDIV]) */
  /*F[clkout] = F[VCO]/(PLLDV[ODIV2] * (PLLODIV_n[DIV] + 1)) */
  /*PLL_PHI0_CLK distributes CORE_CLK*/
# if defined (BRS_PLL_GROUP_D)
#  if (PLLDIG_ODIV2_VAL == 1) /*PLL[ODIV2] == VCO == 960MHz*/
#   if (BRS_TIMEBASE_CLOCK == 120)
  CORE_PLL_PLLODIV_0 |= (uint32)(7ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 8 for PLL_PHI0_CLK = 120MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(7ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 8 for PLL_PHI1_CLK = 120MHz */
#   elif (BRS_TIMEBASE_CLOCK == 160)
  CORE_PLL_PLLODIV_0 |= (uint32)(5ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 6 for PLL_PHI0_CLK = 160MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(5ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 6 for PLL_PHI1_CLK = 160MHz */
#   elif (BRS_TIMEBASE_CLOCK == 240)
  CORE_PLL_PLLODIV_0 |= (uint32)(3ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 3 for PLL_PHI0_CLK = 240MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(3ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 3 for PLL_PHI1_CLK = 240MHz */
#   elif (BRS_TIMEBASE_CLOCK == 320)
  CORE_PLL_PLLODIV_0 |= (uint32)(2ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 4 for PLL_PHI0_CLK = 320MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(3ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 4 for PLL_PHI1_CLK = 240MHz */
#   endif
#  endif

# elif defined (BRS_PLL_GROUP_B)
#  if (PLLDIG_ODIV2_VAL == 2) /*PLL[ODIV2] == VCO/2 == 480MHz*/
#   if (BRS_TIMEBASE_CLOCK == 120)
  CORE_PLL_PLLODIV_0 |= (uint32)(3ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 4 for PLL_PHI0_CLK = 120MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(3ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 4 for PLL_PHI1_CLK = 120MHz */
#   elif (BRS_TIMEBASE_CLOCK == 160)
  CORE_PLL_PLLODIV_0 |= (uint32)(2ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 3 for PLL_PHI0_CLK = 160MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(2ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 3 for PLL_PHI1_CLK = 160MHz */
#   elif (BRS_TIMEBASE_CLOCK == 240)
  CORE_PLL_PLLODIV_0 |= (uint32)(1ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 2 for PLL_PHI0_CLK = 240MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(1ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 2 for PLL_PHI1_CLK = 240MHz */
#   endif
#  endif

# elif defined (BRS_PLL_GROUP_C)
#  if (PLLDIG_ODIV2_VAL == 4) /*PLL[ODIV2] == VCO/4 == 240MHz*/
#   if (BRS_TIMEBASE_CLOCK == 120)
  CORE_PLL_PLLODIV_0 |= (uint32)(1ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 2 for PLL_PHI0_CLK = 120MHz */
  CORE_PLL_PLLODIV_1 |= (uint32)(4ul << PLLDIG_PLLiDIV_DIV); /* Divide PLLDV by 5 for PLL_PHI1_CLK =  48MHz */
#   endif
#  endif
# endif

  /* 2.1.5 Enable CORE_PLL divider #0 and #1. */
  CORE_PLL_PLLODIV_0 |= (uint32)(1ul << PLLDIG_PLLODIV_DE);
  CORE_PLL_PLLODIV_1 |= (uint32)(1ul << PLLDIG_PLLODIV_DE);

  /* 2.1.6 Enable analog PLL block of CORE_PLL. */
  CORE_PLL.PLLCR &= ~(uint32)(1ul << PLLDIG_PLLCR_PLLPD);

  /* 2.1.7 Wait until PLL lock is acquired. */
  while (!(CORE_PLL.PLLSR & (uint32)(1ul << PLLDIG_PLLSR_LOCK)));

  /* 3.0 Set PCFS start, change and end values. */
  MC_CGM_0_PCFS_DIVC8  = MC_CGM_0_PCFS_DIVCx_CHG_VAL;
  MC_CGM_0_PCFS_DIVE8  = MC_CGM_0_PCFS_DIVx_INIT_VAL;
  MC_CGM_0_PCFS_DIVS8  = MC_CGM_0_PCFS_DIVx_INIT_VAL;

# if !defined (BRS_PLL_GROUP_D)
  /*CORE_CLK is driven by PLL_PHI0_CLK frequency directly, 
    therefor no further dividing by MC_CGM_0_MUX0_DC0[DIV] necessary */
# else
  /*Clear divider value for MUX_0_DC_0 (CORE_CLK) */
  MC_CGM_0_MUX_0_DC(0u) &= ~(0x3 << 16);
  
  /*Set division value of MUX_0_DC_0 for CORE_CLK*/
  /* if BRS_TIMEBASE_CLOCK == 120 --> CORE_CLK == 120MHz */
  /* if BRS_TIMEBASE_CLOCK == 160 --> CORE_CLK == 160MHz */
  /* if BRS_TIMEBASE_CLOCK == 240 --> CORE_CLK == 120MHz */
  /* if BRS_TIMEBASE_CLOCK == 320 --> CORE_CLK == 160MHz */
#  if ((BRS_TIMEBASE_CLOCK == 320) || (BRS_TIMEBASE_CLOCK == 240))
   MC_CGM_0_MUX_0_DC(0u) |= (0x1 << 16);
#  elif ((BRS_TIMEBASE_CLOCK == 160) || (BRS_TIMEBASE_CLOCK == 120))
  /*Divider value is already cleared, nothing to be done here*/
#  endif
# endif

  /*Make sure divider is enabled */
  MC_CGM_0_MUX_0_DC(0u) |= (1ul << 31);
  
  /*Wait until update is finished. */
  while(MC_CGM_0_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

# if defined (BRS_PLL_GROUP_D)
  /*Clear divider value for MUX_0_DC_7 (CM7_CORE_CLK) */
  MC_CGM_0_MUX_0_DC(7u) &= ~(0x3 << 16);
  
  /*Set division value of MUX_0_DC_7 for CM7_CORE_CLK*/
  /*CM7_CORE_CLK is driven directly from PLL_PHI0_CLK frequency, therefor no dividing necessary*/
  
  /*Make sure divider is enabled */
  MC_CGM_0_MUX_0_DC(7u) |= (1ul << 31);
  
  /*Wait until update is finished. */
  while(MC_CGM_0_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);
# endif
    
  /* 3.2 Select PLL_PHI0_CLK as the source clock for Cortex-M cores (CORE_CLK). */
  MC_CGM_0_MUX_CSC |= (0x08ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 3.3 Request clock switching to PCFS. */
  MC_CGM_0_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 3.4 Wait until clock source switching is completed. */
  while(MC_CGM_0_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));
  
  /*Clear divider value for MUX_0_DC_1 (AIPS_PLAT_CLK) */
  MC_CGM_0_MUX_0_DC(1u) &= ~(0x7 << 16);

  /* 3.1.0 Set division value of MUX_0_DC_1 for AIPS_PLAT_CLK */
  /* if BRS_TIMEBASE_CLOCK == 120 --> AIPS_PLAT_CLK == 60MHz */
  /* if BRS_TIMEBASE_CLOCK == 160 --> AIPS_PLAT_CLK == 80MHz */
  /* if BRS_TIMEBASE_CLOCK == 240 --> AIPS_PLAT_CLK == 60MHz */
  /* if BRS_TIMEBASE_CLOCK == 320 --> AIPS_PLAT_CLK == 80MHz */
#  if ((BRS_TIMEBASE_CLOCK == 120) || (BRS_TIMEBASE_CLOCK == 160))
  MC_CGM_0_MUX_0_DC(1u) |= (1ul << MC_CGM_0_MUX_DC_DIV);
#  elif ((BRS_TIMEBASE_CLOCK == 240) ||(BRS_TIMEBASE_CLOCK == 320))
  MC_CGM_0_MUX_0_DC(1u) |= (3ul << MC_CGM_0_MUX_DC_DIV);
#  endif
  
  /*Make sure divider is enabled */
  MC_CGM_0_MUX_0_DC(1u) |= (1ul << 31);

  /* 3.1.1 Wait until update is finished. */
  while(MC_CGM_0_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);
  
  /*Clear divider value for MUX_0_DC_2 (AIPS_SLOW_CLK) */
  MC_CGM_0_MUX_0_DC(2u) &= ~(0xF << 16);

  /* 3.1.2 Set division value of DIV #0.1 for AIPS_SLOW_CLK */
  /* if BRS_TIMEBASE_CLOCK == 120 --> AIPS_SLOW_CLK == 30MHz */
  /* if BRS_TIMEBASE_CLOCK == 160 --> AIPS_SLOW_CLK == 40MHz */
  /* if BRS_TIMEBASE_CLOCK == 240 --> AIPS_SLOW_CLK == 30MHz */
  /* if BRS_TIMEBASE_CLOCK == 320 --> AIPS_SLOW_CLK == 40MHz */
#  if ((BRS_TIMEBASE_CLOCK == 120) || (BRS_TIMEBASE_CLOCK == 160))
  MC_CGM_0_MUX_0_DC(2u) |= (3ul << MC_CGM_0_MUX_DC_DIV);
#  elif ((BRS_TIMEBASE_CLOCK == 240) ||(BRS_TIMEBASE_CLOCK == 320))
  MC_CGM_0_MUX_0_DC(2u) |= (7ul << MC_CGM_0_MUX_DC_DIV);
#endif

  /*Make sure divider is enabled */
  MC_CGM_0_MUX_0_DC(2u) |= (1ul << 31);

  /* 3.1.3 Wait until update is finished. */
  while(MC_CGM_0_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);
  
  /*Clear divider value for MUX_0_DC_3 (HSE_CLK) */
  MC_CGM_0_MUX_0_DC(3u) &= ~(0x7 << 16);

  /* Set division value of DIV #0.1 for HSE_CLK*/
# if defined (BRS_PLL_GROUP_D)
  /* if BRS_TIMEBASE_CLOCK == 120 --> HSE_CLK == 120MHz */
  /* if BRS_TIMEBASE_CLOCK == 160 --> HSE_CLK == 80MHz */
  /* if BRS_TIMEBASE_CLOCK == 240 --> HSE_CLK == 120MHz */
  /* if BRS_TIMEBASE_CLOCK == 320 --> HSE_CLK == 80MHz */
#  if (BRS_TIMEBASE_CLOCK == 120)
  /*Nothing to do, divider value is already cleared*/
#  elif ((BRS_TIMEBASE_CLOCK == 160) || (BRS_TIMEBASE_CLOCK == 240))
  MC_CGM_0_MUX_0_DC(3u) |= (1ul << MC_CGM_0_MUX_DC_DIV);
#  elif (BRS_TIMEBASE_CLOCK == 320)
  MC_CGM_0_MUX_0_DC(3u) |= (3ul << MC_CGM_0_MUX_DC_DIV);
#  endif
  
# elif defined (BRS_PLL_GROUP_B) || defined (BRS_PLL_GROUP_C)
  /* if BRS_TIMEBASE_CLOCK == 120 --> HSE_CLK == 120MHz */
  /* if BRS_TIMEBASE_CLOCK == 160 --> HSE_CLK ==  80MHz */
  /* if BRS_TIMEBASE_CLOCK == 240 --> HSE_CLK == 120MHz */
#  if (BRS_TIMEBASE_CLOCK == 120)
  /*Nothing to do, divider value is already cleared*/
#  elif ((BRS_TIMEBASE_CLOCK == 240) || (BRS_TIMEBASE_CLOCK == 160))
  MC_CGM_0_MUX_0_DC(3u) |= (1ul << MC_CGM_0_MUX_DC_DIV);
#  endif
# endif

  /*Make sure divider is enabled */
  MC_CGM_0_MUX_0_DC(3u) |= (1ul << 31);

  /* Wait until update is finished. */
  while(MC_CGM_0_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 3.5 Set division value of MC_CGM_MUX1 DIV#0. */
  MC_CGM_1_MUX_DC_1 = ((1ul << MC_CGM_0_MUX_DC_DIV) | (1ul << MC_CGM_0_MUX_DC_DE));   /* Rli Only division by 2 allowed, DE Bit must be set */

  /* 3.6 Wait until update is finished. */
  while(MC_CGM_1_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 3.7 Select AIPS_PLAT_CLK as the source clock for STM0_CLK. */
  MC_CGM_1_MUX_CSC |= (0x16ul << MC_CGM_MUX_CSC_SELCTL);

  /* 3.8 Request clock switching to PCFS. */
  MC_CGM_1_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 3.9 Wait until clock source switching is completed. */
  while(MC_CGM_1_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));

# if !defined (BRS_PLL_GROUP_C)
  /* 3.10 Set division value of MC_CGM_MUX2 DIV#0. */
  MC_CGM_2_MUX_DC_1 = ((1ul << MC_CGM_0_MUX_DC_DIV) | (1ul << MC_CGM_0_MUX_DC_DE));  /* Rli Only division by 1 allowed, DE Bit must be set */

  /* 3.11 Wait until update is finished. */
  while(MC_CGM_2_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 3.12 Select AIPS_PLAT_CLK as the source clock for STM1_CLK. */
  MC_CGM_2_MUX_CSC |= (0x16ul << MC_CGM_MUX_CSC_SELCTL);

  /* 3.13 Request clock switching to PCFS. */
  MC_CGM_2_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 3.14 Wait until clock source switching is completed. */
  while(MC_CGM_2_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));
# endif

# if defined (BRS_ENABLE_CAN_SUPPORT)
  MC_CGM_3_MUX_DC_1 = ((1ul << MC_CGM_0_MUX_DC_DIV) | (1ul << MC_CGM_0_MUX_DC_DE));  /* Rli CAN clock must be devided by 2, DE bit must be set */
  while(MC_CGM_3_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 4.0.0 Select AIPS_SLOW_CLK as the source clock for FLEXCAN0, FLEXCAN1, FLEXCAN2. */
  MC_CGM_3_MUX_CSC |= (0x16ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 4.0.1 Request clock switching to PCFS. */
  MC_CGM_3_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 4.0.2 Wait until update is finished. */
  while(MC_CGM_3_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));
  MC_CGM_4_MUX_DC_1 = ((1ul << MC_CGM_0_MUX_DC_DIV) | (1ul << MC_CGM_0_MUX_DC_DE)); /* Rli CAN clock must be devided by 2, DE bit must be set */
  while(MC_CGM_4_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 4.0.0 Select AIPS_SLOW_CLK as the source clock for FLEXCAN3, FLEXCAN4, FLEXCAN5. */
  MC_CGM_4_MUX_CSC |= (0x16ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 4.0.1 Request clock switching to PCFS. */
  MC_CGM_4_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 4.0.2 Wait until update is finished. */
  while(MC_CGM_4_MUX_CSS & (uint32)(1ul << MC_CGM_MUX_CSS_SWIP));
# endif /*BRS_ENABLE_CAN_SUPPORT*/

# if !defined (BRS_PLL_GROUP_C)
#  if defined (BRS_ENABLE_ETHERNET_SUPPORT)
  /* 6.0.0 Select  ENET_RX_CLK as the source clock for for ENET_CLK_RX. */
  MC_CGM_7_MUX_CSC |= (0x19ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 6.0.1 Request clock switching to PCFS. */
  MC_CGM_7_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 6.0.2 Wait until update is finished. */
  while(MC_CGM_7_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 6.1.0 Select  ENET_RMII_TX_CLK as the source clock for for ENET_CLK_TX. */
  MC_CGM_8_MUX_CSC |= (0x18ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 6.1.1 Request clock switching to PCFS. */
  MC_CGM_8_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 6.1.2 Wait until update is finished. */
  while(MC_CGM_8_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);

  /* 6.2.0 Select  ENET_RX_CLK as the source clock for for ENET_CLK_TS. */
  MC_CGM_9_MUX_CSC |= (0x19ul << MC_CGM_MUX_CSC_SELCTL);
  __asm("NOP");

  /* 6.2.1 Request clock switching to PCFS. */
  MC_CGM_9_MUX_CSC |= (1ul << MC_CGM_MUX_CSC_CLK_SW);

  /* 6.2.2 Wait until update is finished. */
  while(MC_CGM_9_MUX_DIV_UPD_STAT & MC_CGM_MUX_DIV_UPD_DIV_STAT_MASK);
#  endif /*BRS_ENABLE_ETHERNET_SUPPORT*/
# endif /*!BRS_PLL_GROUP_C*/
}
#endif /*BRS_ENABLE_PLLCLOCKS*/

#if defined (BRS_ENABLE_PORT)
/*******************************************************************************
 *  Port Pin initialization helper functions for usage of BrsHw_Ports.h
 *******************************************************************************/
/*****************************************************************************/
/* @brief      This function configures the port pin configuration registers
 *             (pull-up, open drain)
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized,
 *             n - brsHw_Port_ConfType, for port pin configuration.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() locally.
*****************************************************************************/
void BrsHwInitPortConfig(brsHw_Port_PortType p, brsHw_Port_ConfType n)
{
  uint32 padId = (p.portGroup * 32u) + p.portNumber;

  if(padId <= 219u)
  {
    /* Port Output Driver configuration. */
    if(n.PortOutputControl == PORT_OUTPUT_DRIVER_ENABLED)
    {
      SIUL2_MSCR(padId) |= (1ul << SIUL2_MSCR_OBE);
    }
    else
    {
      SIUL2_MSCR(padId) &= ~(uint32)(1ul << SIUL2_MSCR_OBE);
    }
    
    /* Pull enable configuration. */
    if(n.PortPullEnable == PORT_PULL_ENABLED)
    {
      SIUL2_MSCR(padId) |= (1ul << SIUL2_MSCR_PUE);
    }
    else
    {
      SIUL2_MSCR(padId) &= ~(uint32)(1ul << SIUL2_MSCR_PUE);
    }

    /* Pull select configuration. */
    if(n.PortPullSelect == PORT_PULL_UP)
    {
      SIUL2_MSCR(padId) |= (1ul << SIUL2_MSCR_PUS);
    }
    else
    {
      SIUL2_MSCR(padId) &= ~(uint32)(1ul << SIUL2_MSCR_PUS);
    }

    /* Safe mode control configuration. */
    if(n.SafeModeControl == PORT_SAFEMODETYPE_ENABLED)
    {
      SIUL2_MSCR(padId) |= (1ul << SIUL2_MSCR_SMC);
    }
    else
    {
      SIUL2_MSCR(padId) &= ~(uint32)(1ul << SIUL2_MSCR_SMC);
    }

    /* Slew rate control configuration. */
    if(n.SlewRateControl == PORT_CONF_SLOW)
    {
      SIUL2_MSCR(padId) |= (1ul << SIUL2_MSCR_SRE);
    }
    else
    {
      SIUL2_MSCR(padId) &= ~(uint32)(1ul << SIUL2_MSCR_SRE);
    }
  }
}

/*****************************************************************************/
/* @brief      This function configures a port pin as input pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 *             m - brsHw_Port_MultiplexedType, for port pin configuration.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInput(brsHw_Port_PortType p, brsHw_Port_MultiplexedType m)
{
  uint32 padId = (p.portGroup * 32u) + p.portNumber;

  if(padId <= 219u)
  {
    SIUL2_MSCR(padId) &= ~((uint32)(1ul << SIUL2_MSCR_OBE));
    SIUL2_MSCR(padId) &= ~SIUL2_MSCR_SSS_MASK;
    /* Enable Input Buffer for corresponding pad */
    SIUL2_MSCR(padId) |= ((uint32)(1ul << SIUL2_MSCR_IBE));
    /*Note: The Value of the IMCR register indexes has an offset of 512 between the RM and the IO-Signal_Multiplexing sheet!*/
    m.muxMSCRValue -= 512;
    SIUL2_IMCR(m.muxMSCRValue) &= ~SIUL2_IMCR_SSS_MASK;
    SIUL2_IMCR(m.muxMSCRValue) |= (uint32)(m.muxMultiplexedAlternative);
  }
}

/*****************************************************************************/
/* @brief      This function configures a port pin as output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 *             n - brsHw_Port_ConfType, for port pin configuration.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortOutput(brsHw_Port_PortType p, brsHw_Port_ConfType n)
{
  uint32 padId = (p.portGroup * 32u) + p.portNumber;

  if(padId <= 219u)
  {
    /*Disable Input Buffer and clear SSS values for the corresponding pad*/
    SIUL2_MSCR(padId) &= ~((1ul << SIUL2_MSCR_IBE) | SIUL2_MSCR_SSS_MASK);
    SIUL2_MSCR(padId) |= (p.portAlternative);
    BrsHwInitPortConfig(p,n);
  }
}

/*****************************************************************************/
/* @brief      This function configures a port pin as input and output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 *             n - brsHw_Port_ConfType, for port pin configuration.
 *             m - brsHw_Port_MultiplexedType, for port pin configuration.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInputOutput(brsHw_Port_PortType p, brsHw_Port_ConfType n, brsHw_Port_MultiplexedType m)
{
  uint32 padId = (p.portGroup * 32u) + p.portNumber;

  if(p.portAlternative == BRSHW_PORT_ALT_0 && padId <= 219u)
  {
    BrsHwInitPortInput(p,m);
    BrsHwInitPortOutput(p,n);
  }
}

# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_START_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif
/*****************************************************************************/
/* @brief      This function sets the output level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as output pin with
 *             GPIO functionality.
 * @param[in]  p     - brsHw_Port_PortType, to be set,
 *             Level - level, port pin has to be set to
 *                     (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. BrsMainTogglePin()).
 *****************************************************************************/
void BrsHwPort_SetLevel(brsHw_Port_PortType p, uint8 Level)
{
  SIUL2_GPDO(p.portGroup, p.portNumber) = (Level != BRSHW_PORT_LOGIC_LOW) ? (boolean)STD_HIGH : (boolean)STD_LOW;
}

/*****************************************************************************/
/* @brief      This function reads the input level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as input pin with
 *             GPIO functionality.
 * @param[in]  p - brsHw_Port_PortType, to be read.
 * @param[out] -
 * @return     Level, read from port pin
 *             (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @context    Function is provided to external modules.
 *****************************************************************************/
uint8 BrsHwPort_GetLevel(brsHw_Port_PortType p)
{
  uint32 temp;

  temp = (uint32)((uint8)1u & (uint8)(SIUL2_GPDI(p.portGroup, p.portNumber)));

  return ((uint32)1u == temp) ? BRSHW_PORT_LOGIC_HIGH : BRSHW_PORT_LOGIC_LOW;
}
# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_STOP_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif

/*****************************************************************************/
/* @brief      This function must be used to initialize the used ports.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPortInitPowerOn(void)
{
#if defined (BRS_ENABLE_SUPPORT_LEDS)
  BrsHwInitPortOutput(BRSHW_PORT_LED, BRSHW_PORT_CONF_DIO);

  /* Set LED on EVB demo board to show the system is alive */
  BrsMainTogglePin(BRSMAIN_TOGGLEPIN_LED);
#endif

#if defined (BRS_ENABLE_SUPPORT_TOGGLE_WD_PIN)
  BrsHwInitPortOutput(BRSHW_PORT_TOGGLE_WD, BRSHW_PORT_CONF_DIO);
#endif

#if defined (BRS_ENABLE_SUPPORT_TOGGLE_CUSTOM_PIN)
  BrsHwInitPortOutput(BRSHW_PORT_TOGGLE_CUSTOM, BRSHW_PORT_CONF_DIO);
#endif

/*******************************************************************************
 *  CAN driver
 *******************************************************************************/
#if defined (BRS_ENABLE_CAN_SUPPORT)
# if defined (BRS_ENABLE_CAN_CHANNEL_0)
  BrsHwInitPortOutput(BRSHW_PORT_CAN0_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN0_RX, BRSHW_PORT_MUX_CAN0_RX);
#  if defined(_BRSHW_PORT_CAN0_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN0_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN0_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN0_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN0_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN0_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_0*/

# if defined (BRS_ENABLE_CAN_CHANNEL_1)
  BrsHwInitPortOutput(BRSHW_PORT_CAN1_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN1_RX, BRSHW_PORT_MUX_CAN1_RX);
#  if defined(_BRSHW_PORT_CAN1_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN1_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN1_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN1_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN1_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN1_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_1*/

# if defined (BRS_ENABLE_CAN_CHANNEL_2)
  BrsHwInitPortOutput(BRSHW_PORT_CAN2_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN2_RX, BRSHW_PORT_MUX_CAN2_RX);
#  if defined(_BRSHW_PORT_CAN2_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN2_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN2_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN2_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN2_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN2_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_2*/

# if defined (BRS_ENABLE_CAN_CHANNEL_3)
  BrsHwInitPortOutput(BRSHW_PORT_CAN3_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN3_RX, BRSHW_PORT_MUX_CAN3_RX);
#  if defined(_BRSHW_PORT_CAN3_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN3_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN3_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN3_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN3_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN3_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_3*/

# if defined (BRS_ENABLE_CAN_CHANNEL_4)
  BrsHwInitPortOutput(BRSHW_PORT_CAN4_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN4_RX, BRSHW_PORT_MUX_CAN4_RX);
#  if defined(_BRSHW_PORT_CAN4_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN4_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN4_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN4_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN4_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN4_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_4*/

# if defined (BRS_ENABLE_CAN_CHANNEL_5)
  BrsHwInitPortOutput(BRSHW_PORT_CAN5_TX, BRSHW_PORT_CONF_CAN_TX);
  BrsHwInitPortInput(BRSHW_PORT_CAN5_RX, BRSHW_PORT_MUX_CAN5_RX);
#  if defined(_BRSHW_PORT_CAN5_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_CAN5_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN5_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
#  if defined (_BRSHW_PORT_CAN5_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_CAN5_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_CAN5_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_CAN_CHANNEL_5*/
#endif /*BRS_ENABLE_CAN_SUPPORT*/

/*******************************************************************************
 *  LIN driver
 *******************************************************************************/
#if defined (BRS_ENABLE_LIN_SUPPORT)
# if defined (BRS_ENABLE_LIN_CHANNEL_0)
  BrsHwInitPortOutput(BRSHW_PORT_LIN0_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN0_RX,BRSHW_PORT_MUX_LIN0_RX);
#  if defined(_BRSHW_PORT_LIN0_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN0_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN0_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_0*/

# if defined (BRS_ENABLE_LIN_CHANNEL_1)
  BrsHwInitPortOutput(BRSHW_PORT_LIN1_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN1_RX, BRSHW_PORT_MUX_LIN1_RX);
#  if defined(_BRSHW_PORT_LIN1_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN1_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN1_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_1*/

# if defined (BRS_ENABLE_LIN_CHANNEL_2)
  BrsHwInitPortOutput(BRSHW_PORT_LIN2_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN2_RX, BRSHW_PORT_MUX_LIN2_RX);
#  if defined(_BRSHW_PORT_LIN2_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN2_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN2_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_2*/

# if defined (BRS_ENABLE_LIN_CHANNEL_3)
  BrsHwInitPortOutput(BRSHW_PORT_LIN3_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN3_RX, BRSHW_PORT_MUX_LIN3_RX);
#  if defined(_BRSHW_PORT_LIN3_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN3_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN3_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_3*/

# if defined (BRS_ENABLE_LIN_CHANNEL_4)
  BrsHwInitPortOutput(BRSHW_PORT_LIN4_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN4_RX, BRSHW_PORT_MUX_LIN4_RX);
#  if defined(_BRSHW_PORT_LIN4_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN4_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN4_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_4*/

# if defined (BRS_ENABLE_LIN_CHANNEL_5)
  BrsHwInitPortOutput(BRSHW_PORT_LIN5_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN5_RX, BRSHW_PORT_MUX_LIN5_RX);
#  if defined(_BRSHW_PORT_LIN5_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN5_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN5_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_5*/

# if defined (BRS_ENABLE_LIN_CHANNEL_6)
  BrsHwInitPortOutput(BRSHW_PORT_LIN6_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN6_RX, BRSHW_PORT_MUX_LIN6_RX);
#  if defined(_BRSHW_PORT_LIN6_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN6_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN6_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_6*/

# if defined (BRS_ENABLE_LIN_CHANNEL_7)
  BrsHwInitPortOutput(BRSHW_PORT_LIN7_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN7_RX, BRSHW_PORT_MUX_LIN7_RX);
#  if defined(_BRSHW_PORT_LIN7_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN7_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN7_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_7*/

# if defined (BRS_ENABLE_LIN_CHANNEL_8)
  BrsHwInitPortOutput(BRSHW_PORT_LIN8_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN8_RX, BRSHW_PORT_MUX_LIN8_RX);
#  if defined(_BRSHW_PORT_LIN8_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN8_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN8_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_8*/

# if defined (BRS_ENABLE_LIN_CHANNEL_9)
  BrsHwInitPortOutput(BRSHW_PORT_LIN9_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN9_RX, BRSHW_PORT_MUX_LIN9_RX);
#  if defined(_BRSHW_PORT_LIN9_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN9_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN9_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_9*/

# if defined (BRS_ENABLE_LIN_CHANNEL_10)
  BrsHwInitPortOutput(BRSHW_PORT_LIN10_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN10_RX, BRSHW_PORT_MUX_LIN10_RX);
#  if defined(_BRSHW_PORT_LIN10_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN10_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN10_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_10*/

# if defined (BRS_ENABLE_LIN_CHANNEL_11)
  BrsHwInitPortOutput(BRSHW_PORT_LIN11_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN11_RX, BRSHW_PORT_MUX_LIN11_RX);
#  if defined(_BRSHW_PORT_LIN11_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN11_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN11_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_11*/

# if defined (BRS_ENABLE_LIN_CHANNEL_12)
  BrsHwInitPortOutput(BRSHW_PORT_LIN12_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN12_RX, BRSHW_PORT_MUX_LIN12_RX);
#  if defined(_BRSHW_PORT_LIN12_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN12_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN12_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_12*/

# if defined (BRS_ENABLE_LIN_CHANNEL_13)
  BrsHwInitPortOutput(BRSHW_PORT_LIN13_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN13_RX, BRSHW_PORT_MUX_LIN13_RX);
#  if defined(_BRSHW_PORT_LIN13_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN13_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN13_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_13*/

# if defined (BRS_ENABLE_LIN_CHANNEL_14)
  BrsHwInitPortOutput(BRSHW_PORT_LIN14_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN14_RX, BRSHW_PORT_MUX_LIN14_RX);
#  if defined(_BRSHW_PORT_LIN14_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN14_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN14_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_14*/

# if defined (BRS_ENABLE_LIN_CHANNEL_15)
  BrsHwInitPortOutput(BRSHW_PORT_LIN15_TX, BRSHW_PORT_CONF_LIN_TX);
  BrsHwInitPortInput(BRSHW_PORT_LIN15_RX, BRSHW_PORT_MUX_LIN15_RX);
#  if defined(_BRSHW_PORT_LIN15_TRCV_STB)
  BrsHwInitPortOutput(BRSHW_PORT_LIN15_TRCV_STB, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_LIN15_TRCV_STB, BRSHW_PORT_LOGIC_HIGH);
#  endif
# endif /*BRS_ENABLE_LIN_CHANNEL_15*/
#endif /*BRS_ENABLE_LIN_SUPPORT*/

/*******************************************************************************
 *  ETHERNET driver
 *******************************************************************************/
#if defined (BRS_ENABLE_ETHERNET_SUPPORT)
  /* -- Transmitter signals -- */
  BrsHwInitPortInput(BRSHW_PORT_ETH0_REFCLK, BRSHW_PORT_MUX_ETH0_REFCLK);
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXD0, BRSHW_PORT_CONF_ETHERNET_TX);
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXD1, BRSHW_PORT_CONF_ETHERNET_TX);
# if defined (_BRSHW_PORT_ETH0_TXD2)
  /* Transmit signal 2 is obsolete within RMII */
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXD2, BRSHW_PORT_CONF_ETHERNET_TX);
# endif
# if defined (_BRSHW_PORT_ETH0_TXD3)
  /* Transmit signal 3 is obsolete within RMII */
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXD3, BRSHW_PORT_CONF_ETHERNET_TX);
# endif
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXEN, BRSHW_PORT_CONF_ETHERNET_TX);
# if defined (_BRSHW_PORT_ETH0_TXER)
  /* Transmit Error signal is only optional within MII and obsolete within RMII */
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TXER);
# endif

  /* -- Receiver signals -- */
# if defined (_BRSHW_PORT_ETH0_RXCLK)
  /* Receive clock signal is obsolete within RMII */
  BrsHwInitPortInput(BRSHW_PORT_ETH0_RXCLK, BRSHW_PORT_MUX_ETH0_RXCLK);
# endif
  BrsHwInitPortInput(BRSHW_PORT_ETH0_RXD0, BRSHW_PORT_MUX_ETH0_RXD0);
  BrsHwInitPortInput(BRSHW_PORT_ETH0_RXD1, BRSHW_PORT_MUX_ETH0_RXD1);
# if defined (_BRSHW_PORT_ETH0_RXD2)
  /* Receive signal 2 is obsolete within RMII */
  BrsHwInitPortInput(BRSHW_PORT_ETH0_RXD2, BRSHW_PORT_MUX_ETH0_RXD2);
# endif
# if defined (_BRSHW_PORT_ETH0_RXD3)
  /* Receive signal 3 is obsolete within RMII */
  BrsHwInitPortInput(BRSHW_PORT_ETH0_RXD3, BRSHW_PORT_MUX_ETH0_RXD3);
# endif
  BrsHwInitPortInput(BRSHW_PORT_ETH0_CRSDV, BRSHW_PORT_MUX_ETH0_CRSDV);

  /* -- Management signals -- */
  BrsHwInitPortInputOutput(BRSHW_PORT_ETH0_MDIO, BRSHW_PORT_CONF_ETHERNET_TX, BRSHW_PORT_MUX_ETH0_MDIO);
# if defined (_BRSHW_PORT_ETH0_MDC)
  /* Management data clock not always needed */
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_MDC, BRSHW_PORT_CONF_DIO);
# endif

# if defined (_BRSHW_PORT_ETH0_TRCV_EN)
  BrsHwInitPortOutput(BRSHW_PORT_ETH0_TRCV_EN, BRSHW_PORT_CONF_DIO);
  BrsHwPort_SetLevel(BRSHW_PORT_ETH0_TRCV_EN, BRSHW_PORT_LOGIC_HIGH);
# endif
#endif /*BRS_ENABLE_ETHERNET_SUPPORT*/
}
#endif /*BRS_ENABLE_PORT*/

/* BrsHwDisableInterruptAtPowerOn() is part of ArmCommon */

/* BrsHw_ExceptionTable_Init() is part of ArmCommon */

/* BrsHwSoftwareResetECU() is part of ArmCommon */

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
/* This code is only needed for the first instance/executable in the system */
#define BRS_START_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
/*****************************************************************************/
/* @brief      Get reset reason
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Reset reason
 * @context    Function is called from BrsMainStartup to determine if reset
 *             was triggered through software call (BrsHwSoftwareResetECU()).
 *             The result is stored by BrsMainStartup in the global variable
 *             brsMain_ResetReason. It should only be called once, during
 *             startup. The old API name BrsHwGetResetReason() is remapped
 *             to BrsMainGetResetReason.
 *****************************************************************************/
brsMain_ResetReasonType BrsHwGetResetReasonStartup(void)
{
  /* BRSHW_MC_RGM_FES and BRSHW_MC_RGM_DES registers can be written only either
     in supervisor mode or test mode. "Not checked here" */
  uint32 fesRegVal = 0x0uL;
  uint32 desRegVal = 0x0uL;

  if((BRSHW_MC_RGM_FES & BRSHW_MC_RGM_FES_SW_FUNC) && 
     ((uint32)0u == (BRSHW_MC_RGM_DES & BRSHW_MC_RGM_DES_RESET_WO_POR_MASK)))
  {
    /* Clear software functional reset flag. */
    BRSHW_MC_RGM_FES |= BRSHW_MC_RGM_FES_SW_FUNC;
    while((uint32)0u != (BRSHW_MC_RGM_FES & BRSHW_MC_RGM_FES_SW_FUNC));
    
    /* Software reset is occurred either standalone or along with other functional
       reset reason, multiple reset reasons are not checked. */
    return BRSMAIN_RESET_SW;
  }
  else
  {
    /* Clear functional reset events. */
    fesRegVal = BRSHW_MC_RGM_FES & BRSHW_MC_RGM_FES_RESET_MASK;
    if ((uint32)0u != fesRegVal)
    {
      do {
        BRSHW_MC_RGM_FES  = (uint32)fesRegVal;
      } while ((uint32)0u != (BRSHW_MC_RGM_FES & BRSHW_MC_RGM_FES_RESET_MASK));
    }

    /* Clear destructive reset events. */
    desRegVal = BRSHW_MC_RGM_DES & BRSHW_MC_RGM_DES_RESET_MASK;
    do {
      BRSHW_MC_RGM_DES  = (uint32)desRegVal;
    } while ((uint32)0u != (BRSHW_MC_RGM_DES & BRSHW_MC_RGM_DES_RESET_MASK));

    return BRSMAIN_RESET_OTHER;
  }
}
#define BRS_STOP_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
#endif /*BRS_FIRST_EXECUTION_INSTANCE*/

/* BrsHwTime100NOP() is part of ArmCommon */

#if defined (BRS_ENABLE_SAFECTXSUPPORT)
/*****************************************************************************/
/* @brief      This API is used to enable hardware access in untrusted mode
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function must be called after all depending peripheral modules
 *             are supplied by proper clocks AND before the OS is started.
 *****************************************************************************/
void BrsHw_EnableHwAccess(void)
{
  /* nothing to be done for this platform yet (or never tested) */
  #error "Hardware access in UserMode not yet supported for your specific derivative!"
}
#endif /*BRS_ENABLE_SAFECTXSUPPORT*/

/*****************************************************************************/
/* @brief      This API is used to read the core ID of the actual running core
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Core ID of the actual running core
 * @context    Function is e.g. called from main@BrsMain, to only call HW-init
 *             code once, on the boot core.
 *             In MultiCore setups, additional BRSHW_INIT_CORE_ID must be
 *             declared inside BrsHw.h, to configure the proper core ID value
 *             of that boot core.
 *****************************************************************************/
uint32 BrsHw_GetCore(void)
{
  /* only SingleCore supported up to now */
  return MSCM_CPUXNUM;
}

/* BrsHw_EnableInterrupt() is part of ArmCommon */

/* BrsHw_DisableInterrupt() is part of ArmCommon */

/* BrsHw_TriggerSoftwareInterrupt() is part of ArmCommon */

#if defined (BRS_ENABLE_FBL_SUPPORT)
/*****************************************************************************/
/* @brief      This API is used to disable the flash ECC error reporting,
 *             if this is necessary on the platform to grant access to it.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from FBL boot code, before flash is accessed.
 *****************************************************************************/
void BrsHwDisableEccErrorReporting(void)
{
  /* Disable all ECC Reporting for flash */
  BRSHW_PFCR4 |= BRSHW_PFCR4_ECC_DISABLE_MASK;
}
#endif /*BRS_ENABLE_FBL_SUPPORT*/

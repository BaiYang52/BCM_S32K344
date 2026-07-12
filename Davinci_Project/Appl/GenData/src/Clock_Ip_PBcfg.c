/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file       Clock_Ip_PBcfg.c
*   @version    2.0.1
*
*   @brief   AUTOSAR Mcu - Post-Build(PB) configuration file code template.
*   @details Code template for Post-Build(PB) configuration file generation.
*
*   @addtogroup CLOCK_DRIVER_CONFIGURATION Clock Ip Driver
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Clock_Ip_PBcfg.h"
#include "StandardTypes.h"
#include "Clock_Ip.h"
#include "Clock_Ip_Private.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CLOCK_IP_PBCFG_VENDOR_ID_C                      43
#define CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C       4
#define CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C       4
#define CLOCK_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C    0
#define CLOCK_IP_PBCFG_SW_MAJOR_VERSION_C               2
#define CLOCK_IP_PBCFG_SW_MINOR_VERSION_C               0
#define CLOCK_IP_PBCFG_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_PBcfg.h file are of the same vendor */
#if (CLOCK_IP_PBCFG_VENDOR_ID_C != CLOCK_IP_PBCFG_VENDOR_ID)
    #error "Clock_Ip_PBcfg.c and Clock_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_PBcfg.h file are of the same Autosar version */
#if ((CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C != CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C != CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C != CLOCK_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip_PBcfg.h are different"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_PBcfg.h file are of the same Software version */
#if ((CLOCK_IP_PBCFG_SW_MAJOR_VERSION_C != CLOCK_IP_PBCFG_SW_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_MINOR_VERSION_C != CLOCK_IP_PBCFG_SW_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_PATCH_VERSION_C != CLOCK_IP_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip_PBcfg.h are different"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip.h file are of the same vendor */
#if (CLOCK_IP_PBCFG_VENDOR_ID_C != CLOCK_IP_VENDOR_ID)
    #error "Clock_Ip_PBcfg.c and Clock_Ip.h have different vendor ids"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip.h file are of the same Autosar version */
#if ((CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C != CLOCK_IP_AR_RELEASE_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C != CLOCK_IP_AR_RELEASE_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C != CLOCK_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip.h are different"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip.h file are of the same Software version */
#if ((CLOCK_IP_PBCFG_SW_MAJOR_VERSION_C != CLOCK_IP_SW_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_MINOR_VERSION_C != CLOCK_IP_SW_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_PATCH_VERSION_C != CLOCK_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip.h are different"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_Private.h file are of the same vendor */
#if (CLOCK_IP_PBCFG_VENDOR_ID_C != CLOCK_IP_PRIVATE_VENDOR_ID)
    #error "Clock_Ip_PBcfg.c and Clock_Ip_Private.h have different vendor ids"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_Private.h file are of the same Autosar version */
#if ((CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C != CLOCK_IP_PRIVATE_AR_RELEASE_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C != CLOCK_IP_PRIVATE_AR_RELEASE_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_REVISION_VERSION_C != CLOCK_IP_PRIVATE_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip_Private.h are different"
#endif

/* Check if Clock_Ip_PBcfg.c file and Clock_Ip_Private.h file are of the same Software version */
#if ((CLOCK_IP_PBCFG_SW_MAJOR_VERSION_C != CLOCK_IP_PRIVATE_SW_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_MINOR_VERSION_C != CLOCK_IP_PRIVATE_SW_MINOR_VERSION) || \
     (CLOCK_IP_PBCFG_SW_PATCH_VERSION_C != CLOCK_IP_PRIVATE_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Clock_Ip_PBcfg.c and Clock_Ip_Private.h are different"
#endif


#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if Clock_Ip_PBcfg.c file and StandardTypes.h file are of the same Autosar version */
#if ((CLOCK_IP_PBCFG_AR_RELEASE_MAJOR_VERSION_C    != STD_AR_RELEASE_MAJOR_VERSION) || \
     (CLOCK_IP_PBCFG_AR_RELEASE_MINOR_VERSION_C    != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Clock_Ip_PBcfg.c and StandardTypes.h are different"
#endif
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/


#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"








/* *************************************************************************
 * Configuration structure for Clock Configuration 0
 * ************************************************************************* */
 /*! @brief User Configuration structure clock_Cfg_0 */
const Clock_Ip_ClockConfigType Mcu_aClockConfigPB[2U] = {

    {
        0U,                                 /* clkConfigId */

        (NULL_PTR),                                 /* Register data if register value optimization is enabled */

        2U,                                 /* ircoscsCount */
        2U,                                 /* xoscsCount */
        1U,                                 /* pllsCount */
        13U,                                /* selectorsCount */
        22U,                                /* dividersCount */
        1U,                                 /* dividerTriggersCount */
        0U,                                /* fracDivsCount */
        2U,                                /* extClksCount */
        0U,                                 /* gatesCount */
        0U,                                 /* pcfsCount */
        4U,                                /* cmusCount */
        6U,                                /* configureFrequenciesCount */

        /* IRCOSC initialization. */
        {
            #if CLOCK_IP_IRCOSCS_NO > 0U
            {
                FIRC_STANDBY_CLK,
                0U,
                0U,
                0U,
                0U,
                0U,
            },
            #endif
            #if CLOCK_IP_IRCOSCS_NO > 1U
            {
                SIRC_STANDBY_CLK,
                0U,
                0U,
                0U,
                0U,
                0U,
            },
            #endif
        },

        /* XOSC initialization. */
        {
            #if CLOCK_IP_XOSCS_NO > 0U
            {
                FXOSC_CLK,                    /* Clock name associated to xosc */
                16000000U,                    /* External oscillator frequency. */
                1U,                           /* Enable xosc. */
                49U,                         /* Startup stabilization time. */
                0U,                           /* XOSC bypass option */
                1U,                           /* Comparator enable */
                12U,                          /* Crystal overdrive protection */
                0U,                     /* Gain value */
                0U,                     /* Monitor type */
            },
            #endif
            #if CLOCK_IP_XOSCS_NO > 1U
            {
                SXOSC_CLK,                    /* Clock name associated to xosc */
                0U,                    /* External oscillator frequency. */
                0U,                           /* Enable xosc. */
                125U,                         /* Startup stabilization time. */
                0U,                           /* XOSC bypass option */
                0U,                           /* Comparator enable */
                0U,                          /* Crystal overdrive protection */
                0U,                     /* Gain value */
                0U,                     /* Monitor type */
            },
            #endif

        },

        /* PLL initialization. */
        {
            #if CLOCK_IP_PLLS_NO > 0U
            {
                PLL_CLK,                  /*!<     name;                    */
                1U,                           /*!<     enable;                  */
                RESERVED_CLK,                    /*!<     inputReference           */
                0U,                           /*!<     bypass;                  */
                2U,                           /*!<     predivider;              */
                0U,                           /*!<     multiplier;              */
                0U,                           /*!<     postdivider;              */
                0U,                           /*!<     numeratorFracLoopDiv;    */
                120U,                          /*!<     mulFactorDiv;            */
                0U,                           /*!<     modulation;              */
                1U,                           /*!<     modulationType;          */
                0U,                           /*!<     modulationPeriod;        */
                1U,                           /*!<     incrementStep;           */
                0U,                           /*!<     sigmaDelta;              */
                0U,                           /*!<     ditherControl;           */
                0U,                           /*!<     ditherControlValue;      */
                0U,                           /*!<     Monitor type */
                {                             /*!<     Dividers */
                        0U,
                        0U,
                        0U,
                },
            },
            #endif

        },


        /* SELECTOR initialization. */
        {

            #if CLOCK_IP_SELECTORS_NO > 0U
            {
                SCS_CLK,                     /* Clock name associated to selector */
                PLL_PHI0_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 1U
            {
                STMA_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 2U
            {
                STMB_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 3U
            {
                FLEXCANA_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 4U
            {
                FLEXCANB_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 5U
            {
                CLKOUT_STANDBY_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 6U
            {
                CLKOUT_RUN_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 7U
            {
                EMAC_RX_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 8U
            {
                EMAC_TX_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 9U
            {
                EMAC_TS_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 10U
            {
                QSPI_SFCK_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 11U
            {
                RTC_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 12U
            {
                TRACE_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
        },

        /* DIVIDER initialization. */
        {
        
            #if CLOCK_IP_DIVIDERS_NO > 0U
            {
                FIRC_POSTDIV_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 1U
            {
                PLL_POSTDIV_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 2U
            {
                PLL_PHI0_CLK,
                3U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 3U
            {
                PLL_PHI1_CLK,
                3U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 4U
            {
                CORE_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 5U
            {
                AIPS_PLAT_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 6U
            {
                AIPS_SLOW_CLK,
                4U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 7U
            {
                HSE_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 8U
            {
                DCM_CLK,
                4U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 9U
            {
                LBIST_CLK,
                4U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 10U
            {
                QSPI_MEM_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 11U
            {
                STMA_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 12U
            {
                STMB_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 13U
            {
                FLEXCANA_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 14U
            {
                FLEXCANB_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 15U
            {
                CLKOUT_STANDBY_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 16U
            {
                CLKOUT_RUN_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 17U
            {
                EMAC_RX_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 18U
            {
                EMAC_TX_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 19U
            {
                EMAC_TS_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 20U
            {
                QSPI_SFCK_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 21U
            {
                TRACE_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
        },

        /* DIVIDER TRIGGER Initialization. */
        {
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 0U
            {
                CORE_CLK,                     /* divider name */
                COMMON_TRIGGER_DIVIDER_UPDATE,      /* trigger value */
                CORE_CLK,                     /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 1U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 2U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 3U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 4U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 5U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 6U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
        },

        /* FRACTIONAL DIVIDER initialization. */
        {
            {
                RESERVED_CLK,
                0U,
                {
                    0U,
                    0U,
                },
            },
        },

        /* EXTERNAL CLOCKS initialization. */
        {
            #if CLOCK_IP_EXT_CLKS_NO  > 0U
            {
                EMAC_MII_RX_CLK,
                25000000U,
            },
            #endif

            #if CLOCK_IP_EXT_CLKS_NO  > 1U
            {
                EMAC_MII_RMII_TX_CLK,
                50000000U,
            },
            #endif

        },

        /* CLOCK GATE initialization. */
        {
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
        },

        /* PCFS initialization. */
        {
           #if CLOCK_IP_PCFS_NO > 0U
           {
                RESERVED_CLK,
                0U,
                0U,
                RESERVED_CLK,
                0U,
           },
           #endif
        },

        /* Clock monitor */ 
        {     

            #if CLOCK_IP_CMUS_NO > 0U
            {
                FXOSC_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_0_FXOSC_CLK */
                (                                           /* IER for CMU_FC_0_FXOSC_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                16000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 1U
            {
                CORE_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_3_CORE_CLK */
                (                                           /* IER for CMU_FC_3_CORE_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                160000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 2U
            {
                AIPS_PLAT_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_4_AIPS_PLAT_CLK */
                (                                           /* IER for CMU_FC_4_AIPS_PLAT_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                80000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 3U
            {
                HSE_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_5_HSE_CLK */
                (                                           /* IER for CMU_FC_5_HSE_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                80000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif
        },
        
        /* Specific peripheral initialization. */
        {
            0U,
            {
                {
                    RESERVED_VALUE,
                    0U,
                },

            },
        },
        
        /* Configured frequency values. */
        {
            {
                CLOCK_IS_OFF,
                0U,
            },
            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 1U
            {
                FIRC_CLK,
                48000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 2U
            {
                FXOSC_CLK,
                16000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 3U
            {
                CORE_CLK,
                160000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 4U
            {
                AIPS_PLAT_CLK,
                80000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 5U
            {
                AIPS_SLOW_CLK,
                40000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 6U
            {
                ,
                0U,
            },
            #endif

        },
    },

    {
        1U,                                 /* clkConfigId */

        (NULL_PTR),                                 /* Register data if register value optimization is enabled */

        2U,                                 /* ircoscsCount */
        2U,                                 /* xoscsCount */
        1U,                                 /* pllsCount */
        13U,                                /* selectorsCount */
        22U,                                /* dividersCount */
        1U,                                 /* dividerTriggersCount */
        0U,                                /* fracDivsCount */
        2U,                                /* extClksCount */
        0U,                                 /* gatesCount */
        0U,                                 /* pcfsCount */
        4U,                                /* cmusCount */
        6U,                                /* configureFrequenciesCount */

        /* IRCOSC initialization. */
        {
            #if CLOCK_IP_IRCOSCS_NO > 0U
            {
                FIRC_STANDBY_CLK,
                0U,
                0U,
                0U,
                0U,
                0U,
            },
            #endif
            #if CLOCK_IP_IRCOSCS_NO > 1U
            {
                SIRC_STANDBY_CLK,
                0U,
                0U,
                0U,
                0U,
                0U,
            },
            #endif
        },

        /* XOSC initialization. */
        {
            #if CLOCK_IP_XOSCS_NO > 0U
            {
                FXOSC_CLK,                    /* Clock name associated to xosc */
                0U,                    /* External oscillator frequency. */
                0U,                           /* Enable xosc. */
                49U,                         /* Startup stabilization time. */
                0U,                           /* XOSC bypass option */
                1U,                           /* Comparator enable */
                12U,                          /* Crystal overdrive protection */
                0U,                     /* Gain value */
                0U,                     /* Monitor type */
            },
            #endif
            #if CLOCK_IP_XOSCS_NO > 1U
            {
                SXOSC_CLK,                    /* Clock name associated to xosc */
                0U,                    /* External oscillator frequency. */
                0U,                           /* Enable xosc. */
                125U,                         /* Startup stabilization time. */
                0U,                           /* XOSC bypass option */
                0U,                           /* Comparator enable */
                0U,                          /* Crystal overdrive protection */
                0U,                     /* Gain value */
                0U,                     /* Monitor type */
            },
            #endif

        },

        /* PLL initialization. */
        {
            #if CLOCK_IP_PLLS_NO > 0U
            {
                PLL_CLK,                  /*!<     name;                    */
                0U,                           /*!<     enable;                  */
                RESERVED_CLK,                    /*!<     inputReference           */
                0U,                           /*!<     bypass;                  */
                2U,                           /*!<     predivider;              */
                0U,                           /*!<     multiplier;              */
                0U,                           /*!<     postdivider;              */
                0U,                           /*!<     numeratorFracLoopDiv;    */
                120U,                          /*!<     mulFactorDiv;            */
                0U,                           /*!<     modulation;              */
                1U,                           /*!<     modulationType;          */
                0U,                           /*!<     modulationPeriod;        */
                1U,                           /*!<     incrementStep;           */
                0U,                           /*!<     sigmaDelta;              */
                0U,                           /*!<     ditherControl;           */
                0U,                           /*!<     ditherControlValue;      */
                0U,                           /*!<     Monitor type */
                {                             /*!<     Dividers */
                        0U,
                        0U,
                        0U,
                },
            },
            #endif

        },


        /* SELECTOR initialization. */
        {

            #if CLOCK_IP_SELECTORS_NO > 0U
            {
                SCS_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 1U
            {
                STMA_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 2U
            {
                STMB_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 3U
            {
                FLEXCANA_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 4U
            {
                FLEXCANB_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 5U
            {
                CLKOUT_STANDBY_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 6U
            {
                CLKOUT_RUN_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 7U
            {
                EMAC_RX_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 8U
            {
                EMAC_TX_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 9U
            {
                EMAC_TS_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 10U
            {
                QSPI_SFCK_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 11U
            {
                RTC_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
            #if CLOCK_IP_SELECTORS_NO > 12U
            {
                TRACE_CLK,                     /* Clock name associated to selector */
                FIRC_CLK,                       /* Name of the selected input source */
            },
            #endif
        
        },

        /* DIVIDER initialization. */
        {
        
            #if CLOCK_IP_DIVIDERS_NO > 0U
            {
                FIRC_POSTDIV_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 1U
            {
                PLL_POSTDIV_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 2U
            {
                PLL_PHI0_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 3U
            {
                PLL_PHI1_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                    
            #if CLOCK_IP_DIVIDERS_NO > 4U
            {
                CORE_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 5U
            {
                AIPS_PLAT_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 6U
            {
                AIPS_SLOW_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 7U
            {
                HSE_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 8U
            {
                DCM_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 9U
            {
                LBIST_CLK,
                4U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 10U
            {
                QSPI_MEM_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 11U
            {
                STMA_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 12U
            {
                STMB_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 13U
            {
                FLEXCANA_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 14U
            {
                FLEXCANB_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 15U
            {
                CLKOUT_STANDBY_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 16U
            {
                CLKOUT_RUN_CLK,
                2U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 17U
            {
                EMAC_RX_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 18U
            {
                EMAC_TX_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 19U
            {
                EMAC_TS_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 20U
            {
                QSPI_SFCK_CLK,
                0U,
                {
                    0U,
                }
            },
            #endif
                        
            #if CLOCK_IP_DIVIDERS_NO > 21U
            {
                TRACE_CLK,
                1U,
                {
                    0U,
                }
            },
            #endif
                        
        },

        /* DIVIDER TRIGGER Initialization. */
        {
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 0U
            {
                CORE_CLK,                     /* divider name */
                COMMON_TRIGGER_DIVIDER_UPDATE,      /* trigger value */
                CORE_CLK,                     /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 1U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 2U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 3U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 4U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 5U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
            #if CLOCK_IP_DIVIDER_TRIGGERS_NO > 6U
            {
                RESERVED_CLK,          /* divider name */
                IMMEDIATE_DIVIDER_UPDATE,                    /* trigger value */
                RESERVED_CLK,          /* input source name */
            },
            #endif
        },

        /* FRACTIONAL DIVIDER initialization. */
        {
            {
                RESERVED_CLK,
                0U,
                {
                    0U,
                    0U,
                },
            },
        },

        /* EXTERNAL CLOCKS initialization. */
        {
            #if CLOCK_IP_EXT_CLKS_NO  > 0U
            {
                EMAC_MII_RX_CLK,
                25000000U,
            },
            #endif

            #if CLOCK_IP_EXT_CLKS_NO  > 1U
            {
                EMAC_MII_RMII_TX_CLK,
                50000000U,
            },
            #endif

        },

        /* CLOCK GATE initialization. */
        {
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
            {
                RESERVED_CLK,
                0U,
            },
    
        },

        /* PCFS initialization. */
        {
           #if CLOCK_IP_PCFS_NO > 0U
           {
                RESERVED_CLK,
                0U,
                0U,
                RESERVED_CLK,
                0U,
           },
           #endif
        },

        /* Clock monitor */ 
        {     

            #if CLOCK_IP_CMUS_NO > 0U
            {
                FXOSC_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_0_FXOSC_CLK */
                (                                           /* IER for CMU_FC_0_FXOSC_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                0U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 1U
            {
                CORE_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_3_CORE_CLK */
                (                                           /* IER for CMU_FC_3_CORE_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                48000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 2U
            {
                AIPS_PLAT_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_4_AIPS_PLAT_CLK */
                (                                           /* IER for CMU_FC_4_AIPS_PLAT_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                48000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif

            #if CLOCK_IP_CMUS_NO > 3U
            {
                HSE_CLK,            /* Clock name associated to clock monitor. */
                0U,                   /*Enable/disable clock monitor CMU_FC_5_HSE_CLK */
                (                                           /* IER for CMU_FC_5_HSE_CLK */
                CMU_FC_IER_FLLIE(0U) |
                CMU_FC_IER_FHHIE(0U) |
                CMU_FC_IER_FLLAIE(0U) |
                CMU_FC_IER_FHHAIE(0U)
                ),
                48000000U,
                {                                               
                    0U,          /* Start index in register values array */
                    0U,          /* End index in register values array */
                },
            },
            #endif
        },
        
        /* Specific peripheral initialization. */
        {
            0U,
            {
                {
                    RESERVED_VALUE,
                    0U,
                },

            },
        },
        
        /* Configured frequency values. */
        {
            {
                CLOCK_IS_OFF,
                0U,
            },
            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 1U
            {
                FIRC_CLK,
                48000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 2U
            {
                FXOSC_CLK,
                0U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 3U
            {
                CORE_CLK,
                48000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 4U
            {
                AIPS_PLAT_CLK,
                48000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 5U
            {
                AIPS_SLOW_CLK,
                24000000U,
            },
            #endif

            #if CLOCK_IP_CONFIGURED_FREQUENCIES_NO > 6U
            {
                ,
                0U,
            },
            #endif

        },
    },

};


#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */


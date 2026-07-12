/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : GMAC
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

#ifndef GMAC_IP_DEVICE_REGISTERS_H
#define GMAC_IP_DEVICE_REGISTERS_H

/**
*   @file
*
*   @addtogroup GMAC_DRIVER_CONFIGURATION GMAC Driver Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcal.h"
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')"!][!//
[!CODE!][!WS "0"!]#include "S32G274A_GMAC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32G274A_SRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!// 
[!CODE!][!WS "0"!]#include "S32G399A_GMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32G399A_SRC.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!// 
[!CODE!][!WS "0"!]#include "S32K344_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K344_DCM_GPR.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!// 
[!CODE!][!WS "0"!]#include "S32K342_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K342_DCM_GPR.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!// 
[!CODE!][!WS "0"!]#include "S32K342_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K342_DCM_GPR.h"[!CR!][!ENDCODE!][!//  
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!// 
[!CODE!][!WS "0"!]#include "S32K324_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K324_DCM_GPR.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!// 
[!CODE!][!WS "0"!]#include "S32K322_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K322_DCM_GPR.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!// 
[!CODE!][!WS "0"!]#include "S32K314_EMAC.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "Emac_Ip_Wrapper.h"[!CR!][!ENDCODE!][!// 
[!CODE!][!WS "0"!]#include "S32K314_DCM_GPR.h"[!CR!][!ENDCODE!][!// 
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!// 
[!CODE!][!WS "0"!]#include "S32R45_GMAC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32R45_SRC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32R45_SRC_1.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!// 
[!CODE!][!WS "0"!]#include "S32R41_GMAC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "S32R41_SRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85')"!][!// 
[!CODE!][!WS "0"!]#include "SAF85XX_GMAC.h"[!CR!][!ENDCODE!][!//
[!CODE!][!WS "0"!]#include "SAF85XX_SRC.h"[!CR!][!ENDCODE!][!//
    [!ENDIF!][!// 
[!ENDIF!][!// 
[!ENDNOCODE!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define GMAC_IP_DEVICE_REGISTERS_VENDOR_ID                    43
#define GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION     4
#define GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION     4
#define GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION  0
#define GMAC_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION             2
#define GMAC_IP_DEVICE_REGISTERS_SW_MINOR_VERSION             0
#define GMAC_IP_DEVICE_REGISTERS_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')"!][!//
    [!CODE!]
    /* Checks against Emac_Ip_Wrapper.h */
    #if (GMAC_IP_DEVICE_REGISTERS_VENDOR_ID != EMAC_IP_WRAPPER_VENDOR_ID)
        #error "Gmac_Ip_Device_Registers.h and Emac_Ip_Wrapper.h have different vendor ids"
    #endif
    #if (( GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION    != EMAC_IP_WRAPPER_AR_RELEASE_MAJOR_VERSION) || \
        ( GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION    != EMAC_IP_WRAPPER_AR_RELEASE_MINOR_VERSION) || \
        ( GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION != EMAC_IP_WRAPPER_AR_RELEASE_REVISION_VERSION))
        #error "AUTOSAR Version Numbers of Gmac_Ip_Device_Registers.h and Emac_Ip_Wrapper.h are different"
    #endif
    #if (( GMAC_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION != EMAC_IP_WRAPPER_SW_MAJOR_VERSION) || \
        ( GMAC_IP_DEVICE_REGISTERS_SW_MINOR_VERSION != EMAC_IP_WRAPPER_SW_MINOR_VERSION) || \
        ( GMAC_IP_DEVICE_REGISTERS_SW_PATCH_VERSION != EMAC_IP_WRAPPER_SW_PATCH_VERSION))
        #error "Software Version Numbers of Gmac_Ip_Device_Registers.h and Emac_Ip_Wrapper.h are different"
    #endif
    [!ENDCODE!]
    [!ENDIF!][!// 
[!ENDIF!][!// 
[!ENDNOCODE!][!//

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (GMAC_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Gmac_Ip_Device_Registers.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
[!CODE!]

/** @cond DRIVER_INTERNAL_USE_ONLY */
#define DMA_CH0_CONTROL_ADDR16              0x1100U
#define DMA_CH1_CONTROL_ADDR16              0x1180U
#define DMA_CH2_CONTROL_ADDR16              0x1200U
#define DMA_CH3_CONTROL_ADDR16              0x1280U
#define DMA_CH4_CONTROL_ADDR16              0x1300U

#define MTL_TXQ0_OPERATION_MODE_ADDR16      0x0D00U
#define MTL_TXQ1_OPERATION_MODE_ADDR16      0x0D40U
#define MTL_TXQ2_OPERATION_MODE_ADDR16      0x0D80U
#define MTL_TXQ3_OPERATION_MODE_ADDR16      0x0DC0U
#define MTL_TXQ4_OPERATION_MODE_ADDR16      0x0E00U

typedef struct
{
  volatile uint32 DMA_CONTROL;
  volatile uint32 DMA_TX_CONTROL;
  volatile uint32 DMA_RX_CONTROL;
  volatile uint8 RESERVED_1[8];
  volatile uint32 DMA_TXDESC_LIST_ADDRESS;
  volatile uint8 RESERVED_2[4];
  volatile uint32 DMA_RXDESC_LIST_ADDRESS;
  volatile uint32 DMA_TXDESC_TAIL_POINTER;
  volatile uint8 RESERVED_3[4];
  volatile uint32 DMA_RXDESC_TAIL_POINTER;
  volatile uint32 DMA_TXDESC_RING_LENGTH;
  volatile uint32 DMA_RXDESC_RING_LENGTH;
  volatile uint32 DMA_INTERRUPT_ENABLE;
  volatile uint32 DMA_RX_INTERRUPT_WATCHDOG_TIMER;
  volatile uint32 DMA_SLOT_FUNCTION_CONTROL_STATUS;
  volatile uint8 RESERVED_4[4];
  volatile uint32 DMA_CURRENT_APP_TXDESC;
  volatile uint8 RESERVED_5[4];
  volatile uint32 DMA_CURRENT_APP_RXDESC;
  volatile uint8 RESERVED_6[4];
  volatile uint32 DMA_CURRENT_APP_TXBUFFER;
  volatile uint8 RESERVED_7[4];
  volatile uint32 DMA_CURRENT_APP_RXBUFFER;
  volatile uint32 DMA_STATUS;
  volatile uint32 DMA_MISS_FRAME_CNT;
  volatile uint32 DMA_RXP_ACCEPT_CNT;
  volatile uint32 DMA_RX_ERI_CNT;
  volatile uint8 RESERVED_8[16];
} Gmac_Ip_ChannelType;
/** @endcond */
[!ENDCODE!]

/* @internal @brief Macro for different between header files */
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')"!][!//
        [!CODE!]
        #define MAC_VLAN_TAG_DATA_REG       MAC_VLAN_TAG.MAC_VLAN_TAG_DATA  
        #define MAC_VLAN_TAG_CTRL_REG       MAC_VLAN.MAC_VLAN_TAG_CTRL
        #define MAC_VLAN_INCL_REG           MAC_VLAN_INCL.MAC_VLAN_INCL
        [!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3')"!][!//
    [!CODE!]
    #define MAC_VLAN_TAG_DATA_REG       MAC_VLAN_TAG_DATA.MAC_VLAN_TAG_DATA  
    #define MAC_VLAN_TAG_CTRL_REG       MAC_VLAN_TAG_CTRL
    #define MAC_VLAN_INCL_REG           MAC_VLAN_INCL.MAC_VLAN_INCL
    [!ENDCODE!]
    [!ELSE!][!//
    [!CODE!]
    #define MAC_VLAN_TAG_DATA_REG       MAC_VLAN_TAG_DATA  
    #define MAC_VLAN_TAG_CTRL_REG       MAC_VLAN_TAG_CTRL
    #define MAC_VLAN_INCL_REG           MAC_VLAN_INCL
    [!ENDCODE!]
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/* @internal @brief Base addresses for the DMA channels */
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3'))"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_CH_BASE \
    { { \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH1_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH2_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH3_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH4_CONTROL_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_CH_BASE \
    { { \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH1_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH2_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH3_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH4_CONTROL_ADDR16) \
    }, \
    { \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH1_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH2_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH3_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH4_CONTROL_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_CH_BASE \
    { { \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH1_CONTROL_ADDR16) \
    }, \
    { \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
        (Gmac_Ip_ChannelType *)(IP_GMAC_1_BASE + (uint32)DMA_CH1_CONTROL_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85'))"!][!//
        [!CODE!]
        #define DEV_REG_GMAC_CH_BASE \
        { { \
            (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH0_CONTROL_ADDR16), \
            (Gmac_Ip_ChannelType *)(IP_GMAC_0_BASE + (uint32)DMA_CH1_CONTROL_ADDR16) \
        } }
        [!ENDCODE!]
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

[!CODE!]
/** @cond DRIVER_INTERNAL_USE_ONLY */
typedef struct
{
  volatile uint32 MTL_TXQ_OPERATION_MODE;
  volatile const uint32 MTL_TXQ_UNDERFLOW;
  volatile const uint32 MTL_TXQ_DEBUG;
  uint8 RESERVED_1[4];
  volatile uint32 MTL_TXQ_ETS_CONTROL;        /* Not available for Q0 */
  volatile const uint32 MTL_TXQ_ETS_STATUS;
  volatile uint32 MTL_TXQ_QUANTUM_WEIGHT;     
  volatile uint32 MTL_TXQ_SENDSLOPECREDIT;    /* Not available for Q0 */
  volatile uint32 MTL_TXQ_HICREDIT;           /* Not available for Q0 */
  volatile uint32 MTL_TXQ_LOCREDIT;           /* Not available for Q0 */
  uint8 RESERVED_2[4];
  volatile uint32 MTL_Q_INTERRUPT_CONTROL_STATUS;
  volatile uint32 MTL_RXQ_OPERATION_MODE;
  volatile const uint32 MTL_RXQ_MISSED_PACKET_OVERFLOW_CNT;
  volatile const uint32 MTL_RXQ_DEBUG;
  volatile uint32 MTL_RXQ_CONTROL;
} Gmac_Ip_QueueType;
/** @endcond */
[!ENDCODE!]

/*! @internal @brief Base addresses for the MTL queues */
[!NOCODE!][!// Include specific header file 
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!// 
    [!IF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g2')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32g3'))"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_QUEUE_BASE \
    { { \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ2_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ3_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ4_OPERATION_MODE_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r45')"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_QUEUE_BASE \
    { { \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ2_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ3_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ4_OPERATION_MODE_ADDR16) \
    }, \
    { \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ2_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ3_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ4_OPERATION_MODE_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32r41')"!][!//
    [!CODE!]
    #define DEV_REG_GMAC_QUEUE_BASE \
    { { \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16) \
    }, \
    { \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
        (Gmac_Ip_QueueType *)(IP_GMAC_1_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16) \
    } }
    [!ENDCODE!]
    [!ELSEIF "(contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k3')) or (contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 'saf85'))"!][!//
        [!CODE!]
        #define DEV_REG_GMAC_QUEUE_BASE \
        { { \
            (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ0_OPERATION_MODE_ADDR16), \
            (Gmac_Ip_QueueType *)(IP_GMAC_0_BASE + (uint32)MTL_TXQ1_OPERATION_MODE_ADDR16) \
        } }
        [!ENDCODE!]
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* GMAC_IP_DEVICE_REGISTERS_H */

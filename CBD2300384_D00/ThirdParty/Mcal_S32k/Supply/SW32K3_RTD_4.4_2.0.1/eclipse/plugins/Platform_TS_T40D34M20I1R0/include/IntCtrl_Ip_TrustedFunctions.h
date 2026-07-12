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

#ifndef INTCTRL_IP_TRUSTEDFUNCTIONS_H_
#define INTCTRL_IP_TRUSTEDFUNCTIONS_H_

/**
*   @file
*
*   @defgroup   IntCtrl_Ip Interrupt Controller IP
*   @ingroup    Platform
*
*   @addtogroup IntCtrl_Ip
*   @{
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "IntCtrl_Ip_CfgDefines.h"
#include "Devassert.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_VENDOR_ID                          43
#define PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_MAJOR_VERSION                   2
#define PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_MINOR_VERSION                   0
#define PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_PATCH_VERSION                   1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and IntCtrl_Ip_CfgDefines header file are of the same vendor */
#if (PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_VENDOR_ID != PLATFORM_INTCTRL_IP_CFG_DEFINES_VENDOR_ID)
    #error "IntCtrl_Ip_TrustedFunctions.h and IntCtrl_Ip_CfgDefines.h have different vendor ids"
#endif

/* Check if current file and IntCtrl_Ip_CfgDefines header file are of the same Software version */
#if ((PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_MAJOR_VERSION != PLATFORM_INTCTRL_IP_CFG_DEFINES_SW_MAJOR_VERSION) || \
     (PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_MINOR_VERSION != PLATFORM_INTCTRL_IP_CFG_DEFINES_SW_MINOR_VERSION) || \
     (PLATFORM_INTCTRL_IP_TRUSTEDFUNCTIONS_SW_PATCH_VERSION != PLATFORM_INTCTRL_IP_CFG_DEFINES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of IntCtrl_Ip_TrustedFunctions.h and IntCtrl_Ip_CfgDefines.h are different"
#endif

/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#define PLATFORM_START_SEC_CODE
#include "Platform_MemMap.h"

#if ((STD_ON == INTCTRL_PLATFORM_ENABLE_USER_MODE_SUPPORT) && (defined (MCAL_ENABLE_USER_MODE_SUPPORT)))
/**
 * @internal
 * @brief         Installs a handler for an IRQ.
 */
extern void IntCtrl_Ip_InstallHandlerPrivileged(IRQn_Type eIrqNumber,
                               const IntCtrl_Ip_IrqHandlerType pfNewHandler,
                               IntCtrl_Ip_IrqHandlerType* const pfOldHandler);
/**
 * @internal
 * @brief         Enables an interrupt request.
 */                               
extern void IntCtrl_Ip_EnableIrqPrivileged(IRQn_Type eIrqNumber);
/**
 * @internal
 * @brief         Disables an interrupt request.
 */
extern void IntCtrl_Ip_DisableIrqPrivileged(IRQn_Type eIrqNumber);
/**
 * @internal
 * @brief         Sets the priority for an interrupt request.
 */
extern void IntCtrl_Ip_SetPriorityPrivileged(IRQn_Type eIrqNumber, uint8 u8Priority);
/**
 * @internal
 * @brief         Gets the priority for an interrupt request.
 */
extern uint8 IntCtrl_Ip_GetPriorityPrivileged(IRQn_Type eIrqNumber);
/**
 * @internal
 * @brief         Clears the pending flag for an interrupt request.
 */
extern void IntCtrl_Ip_ClearPendingPrivileged(IRQn_Type eIrqNumber);


#if (INT_CTRL_IP_STANDALONE_APIS == STD_ON)
/**
 * @internal
 * @brief         Sets the pending flag for an interrupt request.
 */
extern void IntCtrl_Ip_SetPendingPrivileged(IRQn_Type eIrqNumber);
/**
 * @internal
 * @brief         Gets the pending flag for an interrupt request.
 */
extern boolean IntCtrl_Ip_GetPendingPrivileged(IRQn_Type eIrqNumber);
#if (INT_CTRL_IP_CORTEXM == STD_ON)
/**
 * @internal
 * @brief         Gets the active flag for an interrupt request.
 */    
extern boolean IntCtrl_Ip_GetActivePrivileged(IRQn_Type eIrqNumber);
#endif
#endif
#if ((INT_CTRL_IP_MSCM_SYSTEM_INTERRUPT_ROUTER == STD_ON) && (INT_CTRL_IP_ROUTING_CONTROL_REGISTER == STD_ON))
/**
 * @internal
 * @brief         Sets the target cores for an interrupt request.
 */    
extern void IntCtrl_Ip_SetTargetCoresPrivileged(IRQn_Type eIrqNumber, uint8 u8TargetCores);
#endif

#if (INT_CTRL_IP_MSI_AVAILABLE == STD_ON)
/**
 * @internal
 * @brief         Get directed cpu Interrupt interrupt flag.
 */    
extern void IntCtrl_Ip_GenerateDirectedCpuInterruptPrivileged(IRQn_Type eIrqNumber, IntCtrl_Ip_IrqTargetType eCpuTarget);
/**
 * @internal
 * @brief         Clear directed cpu Interrupt interrupt flag.
 */
extern void IntCtrl_Ip_ClearDirectedCpuInterruptPrivileged(IRQn_Type eIrqNumber);
/**
 * @internal
 * @brief         Get directed cpu Interrupt interrupt flag.
 */
extern boolean IntCtrl_Ip_GetDirectedCpuInterruptPrivileged(IRQn_Type eIrqNumber);
#endif

#endif /* FTM_IP_USER_MODE_SUPPORT_IS_AVAILABLE == STD_ON */

#define PLATFORM_STOP_SEC_CODE
#include "Platform_MemMap.h"

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* INTCTRL_IP_TRUSTEDFUNCTIONS_H_ */

/** @} */

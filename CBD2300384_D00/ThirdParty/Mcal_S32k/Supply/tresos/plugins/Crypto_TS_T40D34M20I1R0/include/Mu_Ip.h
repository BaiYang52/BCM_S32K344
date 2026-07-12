/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : HSE
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

#ifndef MU_IP_H
#define MU_IP_H

/**
*   @file
*
*   @internal
*   @addtogroup CRYPTO
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Hse_Ip_Cfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define MU_IP_VENDOR_ID_H                       43
#define MU_IP_SW_MAJOR_VERSION_H                2
#define MU_IP_SW_MINOR_VERSION_H                0
#define MU_IP_SW_PATCH_VERSION_H                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Mu_Ip header file and Hse_Ip_Cfg header file are of the same vendor */
#if (MU_IP_VENDOR_ID_H != HSE_IP_CFG_VENDOR_ID_H)
    #error "Mu_Ip.h and Hse_Ip_Cfg.h have different vendor ids"
#endif

/* Check if Mu_Ip header file and Hse_Ip_Cfg header file are of the same Software version */
#if ((MU_IP_SW_MAJOR_VERSION_H != HSE_IP_CFG_SW_MAJOR_VERSION_H) || \
     (MU_IP_SW_MINOR_VERSION_H != HSE_IP_CFG_SW_MINOR_VERSION_H) || \
     (MU_IP_SW_PATCH_VERSION_H != HSE_IP_CFG_SW_PATCH_VERSION_H)    \
    )
    #error "Software Version Numbers of Mu_Ip.h and Hse_Ip_Cfg.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/*! @brief Mask for valid bits in MU RCR register */
#define MU_IP_RCR_REG_VALID_BITS_MASK_U32   (0x0000FFFFUL)
/*! @brief Mask for valid bits in MU RSR register */
#define MU_IP_RSR_REG_VALID_BITS_MASK_U32   (0x0000FFFFUL)
/*! @brief Mask for valid bits in MU TSR register */
#define MU_IP_TSR_REG_VALID_BITS_MASK_U32   (0x0000FFFFUL)

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"

/*!
 * @brief Writes the specified TR register.
 * @param base MU base pointer.
 * @param u8Channel MU channel number.
 * @param u32Value Value to be written.
 */
static inline void Mu_Ip_SetTxRegister(MU_Type *base, uint8 u8Channel, uint32 u32Value)
{
    base->TR[u8Channel] = u32Value;
}

/*!
 * @brief Returns the value of the specified RR register.
 * @param base MU base pointer.
 * @param u8Channel MU channel number.
 * @return RR reg value
 */
static inline uint32 Mu_Ip_GetRxRegister(const MU_Type *base, uint8 u8Channel)
{
    return base->RR[u8Channel];
}

/*!
 * @brief Returns the flag status register.
 * @param base MU base pointer.
 * @return FSR reg value
 */
static inline uint32 Mu_Ip_GetFlagStatusRegister(const MU_Type *base)
{
    return base->FSR;
}

/*!
 * @brief Configures the Rx interrupt for the specified channel.
 * @param base MU base pointer.
 * @param u8Channel MU channel number.
 * @param bEnable TRUE - enable interrupt, FALSE - disable interrupt
 */
static inline void Mu_Ip_SetRxIrqEnable(MU_Type *base, uint8 u8Channel, boolean bEnable)
{
    uint32 u32Mask = ((uint32)(1UL << u8Channel));

    if (bEnable)
    {
        base->RCR |= u32Mask;
    }
    else
    {
        base->RCR &= (~u32Mask);
    }
}

/*!
 * @brief Configures the general purpose interrupts.
 * @param base MU base pointer.
 * @param u32IrqMask Mask of the interrupt bits to be enabled.
 */
static inline void Mu_Ip_SetGlobalIrqEnable(MU_Type *base, uint32 u32IrqMask)
{
    base->GIER = u32IrqMask;
}

/*!
 * @brief Reads the general purpose interrupts.
 * @param base MU base pointer.
 * @return GIER reg value.
 */
static inline uint32 Mu_Ip_GetGlobalIrqEnable(const MU_Type *base)
{
    return base->GIER;
}

/*!
 * @brief Returns the general purpose interrupt flags.
 * @param base MU base pointer.
 * @return GSR reg value.
 */
static inline uint32 Mu_Ip_GetGlobalIrqFlags(const MU_Type *base)
{
    return base->GSR;
}

/*!
 * @brief Clears the general purpose interrupt flags.
 * @param base MU base pointer.
 * @param u32IrqMask mask of the flags to be cleared.
 */
static inline void Mu_Ip_ClearGlobalIrqFlags(MU_Type *base, uint32 u32IrqMask)
{
    base->GSR = u32IrqMask;
}

/*!
 * @brief Checks whether the response has been received for the specified channel.
 * @param base MU base pointer.
 * @param u8Channel MU channel number.
 * @return TRUE - response received, FALSE - response pending
 */
static inline boolean Mu_Ip_IsResponseReady(const MU_Type *base, uint8 u8Channel)
{
    uint32 u32Mask = ((uint32)(1UL << u8Channel));

    return (((base->RSR & u32Mask) > 0U) ? TRUE : FALSE);
}

/*!
 * @brief Returns the Rx control register value.
 * @param base MU base pointer.
 * @return RCR reg value.
 */
static inline uint32 Mu_Ip_GetRxControlRegister(const MU_Type *base)
{
    return (base->RCR & MU_IP_RCR_REG_VALID_BITS_MASK_U32);
}

/*!
 * @brief Returns the Rx status register value.
 * @param base MU base pointer.
 * @return RSR reg value.
 */
static inline uint32 Mu_Ip_GetRxStatusRegister(const MU_Type *base)
{
    return (base->RSR & MU_IP_RSR_REG_VALID_BITS_MASK_U32);
}

/*!
 * @brief Returns the Tx status register value.
 * @param base MU base pointer.
 * @return TSR reg value.
 */
static inline uint32 Mu_Ip_GetTxStatusRegister(const MU_Type *base)
{
    return (base->TSR & MU_IP_TSR_REG_VALID_BITS_MASK_U32);
}

#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* MU_IP_H */


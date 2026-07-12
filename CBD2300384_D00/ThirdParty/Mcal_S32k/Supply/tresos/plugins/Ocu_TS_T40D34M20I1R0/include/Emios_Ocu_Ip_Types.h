/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : eMios
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
====================================================================================================*/

#ifndef EMIOS_OCU_IP_TYPES_H
#define EMIOS_OCU_IP_TYPES_H

/**
*   @file         Emios_Ocu_Ip_Types.h
*
*   @addtogroup   ocu_ip Ocu IPL
*   @brief        Ocu eMIOS driver header file specific to OCU driver
*   @details      eMios specific defines which need to be exported to external application
*
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

#include "StandardTypes.h"
#include "Emios_Ocu_Ip_CfgDefines.h"


/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_OCU_IP_TYPES_VENDOR_ID                       43
#define EMIOS_OCU_IP_TYPES_AR_RELEASE_MAJOR_VERSION        4
#define EMIOS_OCU_IP_TYPES_AR_RELEASE_MINOR_VERSION        4
#define EMIOS_OCU_IP_TYPES_AR_RELEASE_REVISION_VERSION     0
#define EMIOS_OCU_IP_TYPES_SW_MAJOR_VERSION                2
#define EMIOS_OCU_IP_TYPES_SW_MINOR_VERSION                0
#define EMIOS_OCU_IP_TYPES_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (EMIOS_OCU_IP_TYPES_VENDOR_ID != EMIOS_OCU_IP_CFGDEFINES_VENDOR_ID)
    #error "Emios_Ocu_Ip_Types.h and Emios_Ocu_Ip_CfgDefines.h have different vendor ids"
#endif

/* Check if  header file and Emios_Ocu_Ip_CfgDefines.h file are of the same Autosar version */
#if ((EMIOS_OCU_IP_TYPES_AR_RELEASE_MAJOR_VERSION != EMIOS_OCU_IP_CFGDEFINES_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_OCU_IP_TYPES_AR_RELEASE_MINOR_VERSION != EMIOS_OCU_IP_CFGDEFINES_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_OCU_IP_TYPES_AR_RELEASE_REVISION_VERSION != EMIOS_OCU_IP_CFGDEFINES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Ocu_Ip_Types.h and Emios_Ocu_Ip_CfgDefines.h are different"
#endif

/* Check if header file and Emios_Ocu_Ip_CfgDefines.h file are of the same Software version */
#if ((EMIOS_OCU_IP_TYPES_SW_MAJOR_VERSION != EMIOS_OCU_IP_CFGDEFINES_SW_MAJOR_VERSION) || \
     (EMIOS_OCU_IP_TYPES_SW_MINOR_VERSION != EMIOS_OCU_IP_CFGDEFINES_SW_MINOR_VERSION) || \
     (EMIOS_OCU_IP_TYPES_SW_PATCH_VERSION != EMIOS_OCU_IP_CFGDEFINES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Ocu_Ip_Types.h and Emios_Ocu_Ip_CfgDefines.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if this header file and StandardTypes.h file are of the same Autosar version */
    #if ((EMIOS_OCU_IP_TYPES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
        (EMIOS_OCU_IP_TYPES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Ocu_Ip_Types.h and StandardTypes.h are different"
    #endif

#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @{
* @brief eMios register define used to configure counter clock source.
*/

#define OCU_EMIOS_OUTPUT_DISABLED        (0UL)

#define OCU_EMIOS_OUTPUT_ENABLE          (1UL)

#ifndef OCU_EMIOS_PIN_STATE_SHIFT
#define OCU_EMIOS_PIN_STATE_SHIFT       (25U)
#define OCU_EMIOS_PIN_STATE_MASK        (0x01UL << OCU_EMIOS_PIN_STATE_SHIFT)
#endif

#ifndef OCU_EMIOS_PIN_ACTION_SHIFT
#define OCU_EMIOS_PIN_ACTION_SHIFT      (23U)
#define OCU_EMIOS_PIN_ACTION_MASK       (0x03UL << OCU_EMIOS_PIN_ACTION_SHIFT)
#endif

#ifndef EMIOS_OCU_BUS_SELECT_PARAM_SHIFT
#define EMIOS_OCU_BUS_SELECT_PARAM_SHIFT    (21U)
#define EMIOS_OCU_BUS_SELECT_PARAM_MASK     (0x03UL << EMIOS_OCU_BUS_SELECT_PARAM_SHIFT)
#endif

#ifndef OCU_EMIOS_CLOCK_DIV_SHIFT
#define OCU_EMIOS_CLOCK_DIV_SHIFT       (5U)
#define OCU_EMIOS_CLOCK_DIV_MASK        (0x0FUL << OCU_EMIOS_CLOCK_DIV_SHIFT)
#endif

#ifndef OCU_EMIOS_OUTPIN_USED_SHIFT
#define OCU_EMIOS_OUTPIN_USED_SHIFT     (10U)
#define OCU_EMIOS_OUTPIN_USED_MASK      (0x01UL << OCU_EMIOS_OUTPIN_USED_SHIFT)
#endif

#ifndef OCU_EMIOS_ODIS_SHIFT
#define OCU_EMIOS_ODIS_SHIFT        (0U)
#define OCU_EMIOS_ODIS_MASK         (0x01UL << OCU_EMIOS_ODIS_SHIFT)
#endif

#ifndef OCU_EMIOS_DBG_INFO_SHIFT
#define OCU_EMIOS_DBG_INFO_SHIFT     (0U)
#define OCU_EMIOS_DBG_INFO_MASK      (0x01UL << OCU_EMIOS_DBG_INFO_SHIFT)
#endif

/**@}*/

/*!
* @{
*   eMios unified channel prescaller control
*/
#define EMIOS_PRESCALER_DIVIDE1    (0x0UL)

#define EMIOS_PRESCALER_DIVIDE2    (0x01UL)

#define EMIOS_PRESCALER_DIVIDE3    (0x02UL)

#define EMIOS_PRESCALER_DIVIDE4    (0x03UL)

#define EMIOS_PRESCALER_DIVIDE5    (0x04UL)

#define EMIOS_PRESCALER_DIVIDE6    (0x05UL)

#define EMIOS_PRESCALER_DIVIDE7    (0x06UL)

#define EMIOS_PRESCALER_DIVIDE8    (0x07UL)

#define EMIOS_PRESCALER_DIVIDE9    (0x08UL)

#define EMIOS_PRESCALER_DIVIDE10   (0x09UL)

#define EMIOS_PRESCALER_DIVIDE11   (0x0AUL)

#define EMIOS_PRESCALER_DIVIDE12   (0x0BUL)

#define EMIOS_PRESCALER_DIVIDE13   (0x0CUL)

#define EMIOS_PRESCALER_DIVIDE14   (0x0DUL)

#define EMIOS_PRESCALER_DIVIDE15   (0x0EUL)

#define EMIOS_PRESCALER_DIVIDE16   (0x0FUL)

#define EMIOS_FREEZE_ENABLE         (1U)
/*! @}*/

/*!
* @{
* @brief eMios channels defines
*/
/*
 * eMios channel encoding
 *
 * [              3 bit | 5 bit                ]
 * [ module id: 7 ... 0 | channel id: 31 ... 0 ]
 */
#define EMIOS_MOD_MASK      (0xE0U)
#define EMIOS_MOD_SHIFT     (5U)
#define EMIOS_CH_MASK       (0x1FU)
#define EMIOS_CH_SHIFT      (0U)
/*! @}*/

/*!
* @{
* @brief eMios Channels defines
*/
#define EMIOS_CHANNEL_0     (0U)
#define EMIOS_CHANNEL_1     (1U)
#define EMIOS_CHANNEL_2     (2U)
#define EMIOS_CHANNEL_3     (3U)
#define EMIOS_CHANNEL_4     (4U)
#define EMIOS_CHANNEL_5     (5U)
#define EMIOS_CHANNEL_6     (6U)
#define EMIOS_CHANNEL_7     (7U)
#define EMIOS_CHANNEL_8     (8U)
#define EMIOS_CHANNEL_9     (9U)
#define EMIOS_CHANNEL_10    (0x0AU)
#define EMIOS_CHANNEL_11    (0x0BU)
#define EMIOS_CHANNEL_12    (0x0CU)
#define EMIOS_CHANNEL_13    (0x0DU)
#define EMIOS_CHANNEL_14    (0x0EU)
#define EMIOS_CHANNEL_15    (0x0FU)
#define EMIOS_CHANNEL_16    (0x10U)
#define EMIOS_CHANNEL_17    (0x11U)
#define EMIOS_CHANNEL_18    (0x12U)
#define EMIOS_CHANNEL_19    (0x13U)
#define EMIOS_CHANNEL_20    (0x14U)
#define EMIOS_CHANNEL_21    (0x15U)
#define EMIOS_CHANNEL_22    (0x16U)
#define EMIOS_CHANNEL_23    (0x17U)

#define EMIOS_0_CH_0    (EMIOS_CHANNEL_0)

#define EMIOS_0_CH_1    (EMIOS_CHANNEL_1)

#define EMIOS_0_CH_2    (EMIOS_CHANNEL_2)

#define EMIOS_0_CH_3    (EMIOS_CHANNEL_3)

#define EMIOS_0_CH_4    (EMIOS_CHANNEL_4)

#define EMIOS_0_CH_5    (EMIOS_CHANNEL_5)

#define EMIOS_0_CH_6    (EMIOS_CHANNEL_6)

#define EMIOS_0_CH_7    (EMIOS_CHANNEL_7)

#define EMIOS_0_CH_8    (EMIOS_CHANNEL_8)

#define EMIOS_0_CH_9    (EMIOS_CHANNEL_9)

#define EMIOS_0_CH_10   (EMIOS_CHANNEL_10)

#define EMIOS_0_CH_11   (EMIOS_CHANNEL_11)

#define EMIOS_0_CH_12   (EMIOS_CHANNEL_12)

#define EMIOS_0_CH_13   (EMIOS_CHANNEL_13)

#define EMIOS_0_CH_14   (EMIOS_CHANNEL_14)

#define EMIOS_0_CH_15   (EMIOS_CHANNEL_15)

#define EMIOS_0_CH_16   (EMIOS_CHANNEL_16)

#define EMIOS_0_CH_17   (EMIOS_CHANNEL_17)

#define EMIOS_0_CH_18   (EMIOS_CHANNEL_18)

#define EMIOS_0_CH_19   (EMIOS_CHANNEL_19)

#define EMIOS_0_CH_20   (EMIOS_CHANNEL_20)

#define EMIOS_0_CH_21   (EMIOS_CHANNEL_21)

#define EMIOS_0_CH_22   (EMIOS_CHANNEL_22)

#define EMIOS_0_CH_23   (EMIOS_CHANNEL_23)

#define EMIOS_1_CH_0    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_0))

#define EMIOS_1_CH_1    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_1))

#define EMIOS_1_CH_2    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_2))

#define EMIOS_1_CH_3    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_3))

#define EMIOS_1_CH_4    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_4))

#define EMIOS_1_CH_5    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_5))

#define EMIOS_1_CH_6    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_6))

#define EMIOS_1_CH_7    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_7))

#define EMIOS_1_CH_8    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_8))

#define EMIOS_1_CH_9    ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_9))

#define EMIOS_1_CH_10   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_10))

#define EMIOS_1_CH_11   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_11))

#define EMIOS_1_CH_12   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_12))

#define EMIOS_1_CH_13   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_13))

#define EMIOS_1_CH_14   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_14))

#define EMIOS_1_CH_15   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_15))

#define EMIOS_1_CH_16   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_16))

#define EMIOS_1_CH_17   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_17))

#define EMIOS_1_CH_18   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_18))

#define EMIOS_1_CH_19   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_19))

#define EMIOS_1_CH_20   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_20))

#define EMIOS_1_CH_21   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_21))

#define EMIOS_1_CH_22   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_22))

#define EMIOS_1_CH_23   ((uint8)((1U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_23))

#define EMIOS_2_CH_0    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_0))

#define EMIOS_2_CH_1    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_1))

#define EMIOS_2_CH_2    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_2))

#define EMIOS_2_CH_3    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_3))

#define EMIOS_2_CH_4    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_4))

#define EMIOS_2_CH_5    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_5))

#define EMIOS_2_CH_6    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_6))

#define EMIOS_2_CH_7    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_7))

#define EMIOS_2_CH_8    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_8))

#define EMIOS_2_CH_9    ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_9))

#define EMIOS_2_CH_10   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_10))

#define EMIOS_2_CH_11   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_11))

#define EMIOS_2_CH_12   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_12))

#define EMIOS_2_CH_13   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_13))

#define EMIOS_2_CH_14   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_14))

#define EMIOS_2_CH_15   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_15))

#define EMIOS_2_CH_16   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_16))

#define EMIOS_2_CH_17   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_17))

#define EMIOS_2_CH_18   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_18))

#define EMIOS_2_CH_19   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_19))

#define EMIOS_2_CH_20   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_20))

#define EMIOS_2_CH_21   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_21))

#define EMIOS_2_CH_22   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_22))

#define EMIOS_2_CH_23   ((uint8)((2U << EMIOS_MOD_SHIFT) + EMIOS_CHANNEL_23))
/*! @}*/

/** @brief invalid eMios channel number */
#define OCU_EMIOS_INVALID_CHANNEL_NUM   255U

/**
* @brief   Bus Select: MC up counter
*/
#define MASTER_MODE_UP_COUNTER_U32                     (0x00000010UL)

/**
* @brief   Bus Select: MC up/down counter
*/
#define MASTER_MODE_UP_DOWN_COUNTER_U32                (0x00000014UL)

/**
* @brief   Bus Select: MCB up
*/
#define MASTER_MODE_UP_BUFFERED_COUNTER_U32            (0x00000050UL)

/**
* @brief   Bus Select: MCB up/down
*/
#define MASTER_MODE_UP_DOWN_BUFFERED_COUNTER_U32       (0x00000054UL)
/**
* @brief   Initialization option to set the channel prescaler value to 1
*/
#define MCL_MASTER_BUS_PRESCALER_1_U32                                 (0UL)

/**
* @brief   Initialization option to set the channel prescaler value to 2
*/
#define MCL_MASTER_BUS_PRESCALER_2_U32                                 (0x04000000UL)

/**
* @brief   Initialization option to set the channel prescaler value to 3
*/
#define MCL_MASTER_BUS_PRESCALER_3_U32                                 (0x08000000UL)

/**
* @brief   Initialization option to set the channel prescaler value to 4
*/
#define MCL_MASTER_BUS_PRESCALER_4_U32                                 (0x0C000000UL)


/**
 * @brief   Edge Pin Action values
 * @details Automatic action (by hardware) to be performed on a pin attached to an OCU channel.
 */
/** @brief The channel pin will be set HIGH upon compare match. */
#define OCU_EMIOS_SET_LOW     0U
/** @brief The channel pin will be set LOW upon compare match. */
#define OCU_EMIOS_SET_HIGH    1U
/** @brief The channel pin will be set to the opposite of its current level HIGH upon compare match. */
#define OCU_EMIOS_SET_TOGGLE  2U
/** @brief The channel pin will remain at its current level upon compare match. */
#define OCU_EMIOS_SET_DISABLE 3U

/**
 * @brief      Pin State levels
 * @details    Output state of the pin linked to an OCU channel.
 *
 */
/** @brief Ocu Pin level is logic low */
#define OCU_EMIOS_LOW     0U
/** @brief Ocu Pin level is logic high */
#define OCU_EMIOS_HIGH    1U

/**
 * @brief      Prescaler values
 * @details    Possible values of prescallers used to configure base-clock timers
 *
 */
/** @brief Selected value is the default/primary prescaler */
#define OCU_EMIOS_PRIMARY_PRESCALER       0U
/** @brief Selected value is the alternative configured prescaler */
#define OCU_EMIOS_ALTERNATIVE_PRESCALER   1U

/**
 * @brief      Ocu Return values
 * @details    Return information after setting a new threshold value.
 *
 */
/** @brief The compare match will occur inside the current Reference Interval. */
#define OCU_EMIOS_CM_IN_REF_INTERVAL  0U
/** @brief The compare match will not occur inside the current Reference Interval. */
#define OCU_EMIOS_CM_OUT_REF_INTERVAL 1U


/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/**
* @brief UC modes of operation - Output compare mode
* @implements : Emios_Ocu_Ip_OperationModeType_enum
*/
typedef enum
{
    EMIOS_MODE_GPIO_IN                  = 0x00U,   /**< General Purpose Input
                                                    *   In GPIO mode, all input capture and output compare functions of the Unified Channel are disabled,
                                                    *   and the internal counter (EMIOSCNTn register) is cleared and disabled. */
    EMIOS_MODE_SAOC                     = 0x03U,   /**< Single-Action Output Compare */
    EMIOS_MODE_DAOC_FSET_TRAILING_MATCH = 0x06U,   /**< Double-Action Output Compare, FLAG set at Trailing edge match */
    EMIOS_MODE_DAOC_FSET_BOTH_MATCH     = 0x07U,    /**< Double-Action Output Compare, FLAG set at both Leading & Trailing edge match */
    EMIOS_MODE_MCB_COUNTER_UP           = 0x50U     /**< Modulus Counter up mode */
} Emios_Ocu_Ip_OperationModeType;

/**
* @brief      Ocu Count direction
* @details    This enum specifies the count direction for the whole Ocu driver.
*
*/
typedef enum
{
    OCU_EMIOS_UPCOUNTING = 0 /** @brief Counter in UP Counting */
#if (STD_ON == OCU_EMIOS_DOWNCOUNTING_SUPPORED)
    ,OCU_EMIOS_DOWNCOUNTING    /** @brief Counter in DOWN Counting. */
#endif
} Emios_Ocu_Ip_CountDirectionType;

/**
 * @brief   Counter bus select.
 * @details Select either one of counter bus or the internal counter to be used by the Unified Channel.
 *
 */
typedef enum
{
    EMIOS_OCU_BUS_A                 = 0x00U,    /**< Global counter bus A */
    EMIOS_OCU_BUS_B                 = 0x01U,    /**< Local counter bus */
    EMIOS_OCU_BUS_C                 = 0x01U,    /**< Local counter bus */
    EMIOS_OCU_BUS_D                 = 0x01U,    /**< Local counter bus */
    EMIOS_OCU_BUS_F                 = 0x02U,    /**< Global counter bus F */
    EMIOS_OCU_BUS_INTERNAL_COUNTER  = 0x03U     /**< Internal counter bus */
} Emios_Ocu_Ip_BusSelectType;

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
 * @brief      Edge Pin Action type
 * @details    Automatic action (by hardware) to be performed on a pin attached to an OCU channel.
 *
 */
typedef uint8 Emios_Ocu_Ip_PinActionType;

/**
 * @brief      Pin State level
 * @details    Output state of the pin linked to an OCU channel.
 *
 */
typedef uint8 Emios_Ocu_Ip_PinStateType;

/**
 * @brief      Prescaler type
 * @details    Specifies the possible values of prescallers used to configure base-clock timers
 *
 */
typedef uint8 Emios_Ocu_Ip_SelectPrescalerType;

/**
 * @brief      Ocu Return Type
 * @details    Return information after setting a new threshold value.
 *
 */
typedef uint8 Emios_Ocu_Ip_ReturnType;

#if((STD_ON == OCU_GET_COUNTER_API)||(STD_ON == OCU_SET_RELATIVE_THRESHOLD_API)||(STD_ON == OCU_SET_ABSOLUTE_THRESHOLD_API))
/**
* @brief        FTM Value type (the value of the period is platform dependent and thus configurable)
* @details      Type for reading the counter and writing the threshold values (in number of ticks)
*
*/
typedef uint16 Emios_Ocu_Ip_ValueType;
#endif

/** Redefine eMIOS Register Layout Typedef from header file to comply with coding guidelines */
typedef eMIOS_Type Emios_Ocu_Ip_xRegLayoutType;

/** @brief      Channel notification typedef */
typedef void (*Emios_Ocu_Ip_CallbackType)(uint16 CallbackParam);

#if (STD_ON == OCU_NOTIFICATION_SUPPORTED)
/**
 * @brief internal context structure
 *
 * This structure is used by the IPL driver for internal logic.
 * The content is populated on Init
 */
typedef struct
{
    boolean                     mChannelInit;
    Emios_Ocu_Ip_CallbackType   mCallbackFunc;
    uint16                      mCallbackParam;
} Emios_Ocu_Ip_ChStateType;
#endif

/** @brief    eMios IP channel specific configuration structure for the Ocu functionality */
typedef struct
{
/*
 * eMios channel encoding
 *
 * [              3 bit | 5 bit                ]
 * [ module id: 7 ... 0 | channel id: 31 ... 0 ]
 */
    /** @brief Assigned eMios Hw Channel Id */
    const uint16 HwChannel;
    /** @brief Compare match threshold for the current channel */
    const uint16 DefaultThreshold;
    /** @brief Maximum counter value for the current channel */
    const uint16 MaxCounterValue;
#if (STD_ON == OCU_SET_CLOCK_MODE_API)
    /** @brief eMios channel parameters */
    /** bits 4 .. 7 --> Prescaler configuration */
    const uint32 AltControlValue;
#endif
    /** @brief eMios configuration parameter */
    /** bit 24        --> Set Edge Polarity bit
        bit 23        --> Edge action on match bit
        bits 21 .. 22 --> Set Counter Bus bits
        bit 10        --> Set pin used
        bits 5 .. 8   --> Prescaler configuration
        bit 1         --> Set or Disable output
        bit 0         --> Set or Disable timer functionality in freeze mode */
    const uint32 ControlValue;
    /** @brief Assigned eMios Hw Channel Id of counter bus */
    const uint8 HwChannelCounterBus;
#if (STD_ON == OCU_NOTIFICATION_SUPPORTED)
    /** @brief  Channel notification function */
    Emios_Ocu_Ip_CallbackType   mCallbackFunc;
    /** @brief  Channel notification parameter */
    uint16                      mCallbackParam;
#endif
} Emios_Ocu_Ip_ChannelConfigType;

/** @brief      eMios IP specific configuration structure type */
typedef struct
{
    /** @brief Number of eMios channels in the Ocu configuration */
    const uint8 NumChannels;
    /** @brief Pointer to the channels configured for eMios */
    const Emios_Ocu_Ip_ChannelConfigType (*pcxChannelsConfig)[];
} Emios_Ocu_Ip_ModuleConfigType;

#ifdef __cplusplus
}
#endif

/** @} */

#endif

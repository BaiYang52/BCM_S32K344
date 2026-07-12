/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I2S
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

#ifndef FLEXIO_I2S_IP_HWACCESS_H
#define FLEXIO_I2S_IP_HWACCESS_H

/**
*   @file
*
*   @addtogroup flexio_i2s_ip FLEXIO_I2S IPL
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
#if (STD_ON == FLEXIO_I2S_IP_ENABLE)
#include "Flexio_Mcl_Ip_HwAccess.h"
#endif /* (STD_ON == FLEXIO_I2S_IP_ENABLE) */
/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_I2S_IP_HWACCESS_VENDOR_ID                    43
#define FLEXIO_I2S_IP_HWACCESS_MODULE_ID                    255
#define FLEXIO_I2S_IP_HWACCESS_AR_RELEASE_MAJOR_VERSION     4
#define FLEXIO_I2S_IP_HWACCESS_AR_RELEASE_MINOR_VERSION     4
#define FLEXIO_I2S_IP_HWACCESS_AR_RELEASE_REVISION_VERSION  0
#define FLEXIO_I2S_IP_HWACCESS_SW_MAJOR_VERSION             2
#define FLEXIO_I2S_IP_HWACCESS_SW_MINOR_VERSION             0
#define FLEXIO_I2S_IP_HWACCESS_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#if (STD_ON == FLEXIO_I2S_IP_ENABLE)

#define I2S_START_SEC_CODE
#include "I2s_MemMap.h"

/*
 * This function sets the following configurations for the specified shifter:
 * Start bit, Stop bit, input source
 */
static inline void Flexio_I2s_Ip_SetShifterConfig(FLEXIO_Type *BaseAddr,
                                                uint8 shifter,
                                                Flexio_Mcl_Ip_ShifterStartType Start,
                                                Flexio_Mcl_Ip_ShifterStopType Stop,
                                                Flexio_Mcl_Ip_ShifterSourceType Source)
{
    BaseAddr->SHIFTCFG[shifter] = FLEXIO_SHIFTCFG_SSTART(Start)
                                | FLEXIO_SHIFTCFG_SSTOP(Stop)
                                | FLEXIO_SHIFTCFG_INSRC(Source);
}

/*
 * This function configures the control settings for the specified shifter: Mode settings,
 * pin settings and Timer settings.
 */
static inline void Flexio_I2s_Ip_SetShifterControl(FLEXIO_Type *BaseAddr,
                                                uint8 shifter,
                                                Flexio_Mcl_Ip_ShifterModeType Mode,
                                                uint8 pin,
                                                Flexio_Mcl_Ip_PinPolarityType PinPolarity,
                                                Flexio_Mcl_Ip_PinConfigType PinConfig,
                                                uint8 Timer,
                                                Flexio_Mcl_Ip_TimerPolarityType TimerPolarity)
{
    BaseAddr->SHIFTCTL[shifter] = FLEXIO_SHIFTCTL_SMOD(Mode)
                                | FLEXIO_SHIFTCTL_PINPOL(PinPolarity)
                                | FLEXIO_SHIFTCTL_PINSEL(pin)
                                | FLEXIO_SHIFTCTL_PINCFG(PinConfig)
                                | FLEXIO_SHIFTCTL_TIMPOL(TimerPolarity)
                                | FLEXIO_SHIFTCTL_TIMSEL(Timer);
}

/*
 * This function configures the Mode for the specified shifter.
 */
static inline void Flexio_I2s_Ip_SetShifterMode(FLEXIO_Type *BaseAddr,
                                             uint8 shifter,
                                             Flexio_Mcl_Ip_ShifterModeType Mode)
{
    uint32 tmp = BaseAddr->SHIFTCTL[shifter];
    tmp &= ~(FLEXIO_SHIFTCTL_SMOD_MASK);
    tmp |= FLEXIO_SHIFTCTL_SMOD(Mode);
    BaseAddr->SHIFTCTL[shifter] = tmp;
}

/*
 * This function configures the pin assigned to the specified specified shifter.
 * It does not change the other pin-related settings.
 */
static inline void Flexio_I2s_Ip_SetShifterPinConfig(FLEXIO_Type *BaseAddr,
                                                  uint8 shifter,
                                                  Flexio_Mcl_Ip_PinConfigType config)
{
    uint32 tmp = BaseAddr->SHIFTCTL[shifter];
    tmp &= ~(FLEXIO_SHIFTCTL_PINCFG_MASK);
    tmp |= FLEXIO_SHIFTCTL_PINCFG(config);
    BaseAddr->SHIFTCTL[shifter] = tmp;
}

/*
 * This function writes data in the specified shifter buffer. The data can be
 * written in any of the four ways allowed by the hardware - see description of type
 * Flexio_Mcl_Ip_ShifterBufferModeType for details.
 */
static inline void Flexio_I2s_Ip_WriteShifterBuffer(FLEXIO_Type *BaseAddr,
                                                 uint8 shifter,
                                                 uint32 Value,
                                                 Flexio_Mcl_Ip_ShifterBufferModeType Mode)
{
    switch (Mode)
    {
        case FLEXIO_SHIFTER_RW_MODE_NORMAL:
            BaseAddr->SHIFTBUF[shifter] = Value;
            break;
        case FLEXIO_SHIFTER_RW_MODE_BIT_SWAP:
            BaseAddr->SHIFTBUFBIS[shifter] = Value;
            break;
        default:
            /* bad write Mode */
            break;
    }
}

/*
 * This function reads data from the specified shifter buffer. The data can be
 * read in any of the four ways allowed by the hardware - see description of type
 * Flexio_Mcl_Ip_ShifterBufferModeType for details.
 */
static inline uint32 Flexio_I2s_Ip_ReadShifterBuffer(const FLEXIO_Type *BaseAddr,
                                                    uint8 shifter,
                                                    Flexio_Mcl_Ip_ShifterBufferModeType Mode)
{
    uint32 data;

    switch (Mode)
    {
        case FLEXIO_SHIFTER_RW_MODE_NORMAL:
            data = BaseAddr->SHIFTBUF[shifter];
            break;
        case FLEXIO_SHIFTER_RW_MODE_BIT_SWAP:
            data = BaseAddr->SHIFTBUFBIS[shifter];
            break;
        default:
            /* bad read Mode */
            data = 0U;
            break;
    }
    return data;
}

/*
 * This function configures the compare value for the specified Timer.
 * The Timer compare value is loaded into the Timer counter when the Timer is first enabled,
 * when the Timer is reset and when the Timer decrements down to zero.
 * In dual 8-bit counters baud/bit Mode, the lower 8-bits configure the baud rate divider
 * and the upper 8-bits configure the number of bits in each word.
 * In dual 8-bit counters PWM Mode, the lower 8-bits configure the high period of the output
 * and the upper 8-bits configure the low period.
 * In 16-bit counter Mode, the compare value can be used to generate the baud rate divider
 * (if shift clock source is Timer output) or the number of bits in each word (when the shift
 * clock source is a pin or trigger input).
 */
static inline void Flexio_I2s_Ip_SetTimerCompare(FLEXIO_Type *BaseAddr,
                                             uint8 Timer,
                                             uint16 Value)
{
    BaseAddr->TIMCMP[Timer] = (uint32)Value;
}

/*
 * This function sets the following configurations for the specified Timer:
 * Start bit, Stop bit, enable condition, disable condition, reset condition,
 * decrement condition, initial output
 */
static inline void Flexio_I2s_Ip_SetTimerConfig(FLEXIO_Type *BaseAddr,
                                             uint8 Timer,
                                             Flexio_Mcl_Ip_TimerStartType Start,
                                             Flexio_Mcl_Ip_TimerStopType Stop,
                                             Flexio_Mcl_Ip_TimerEnableType enable,
                                             Flexio_Mcl_Ip_TimerDisableType disable,
                                             Flexio_Mcl_Ip_TimerResetType reset,
                                             Flexio_Mcl_Ip_TimerDecrementType decrement,
                                             Flexio_Mcl_Ip_TimerOutputType output)
{

    BaseAddr->TIMCFG[Timer] = FLEXIO_TIMCFG_TSTART(Start)
                            | FLEXIO_TIMCFG_TSTOP(Stop)
                            | FLEXIO_TIMCFG_TIMENA(enable)
                            | FLEXIO_TIMCFG_TIMDIS(disable)
                            | FLEXIO_TIMCFG_TIMRST(reset)
                            | FLEXIO_TIMCFG_TIMDEC(decrement)
                            | FLEXIO_TIMCFG_TIMOUT(output);
}

/*
 * This function configures the control settings for the specified Timer: Mode settings,
 * pin settings and trigger settings.
 */
static inline void Flexio_I2s_Ip_SetTimerControl(FLEXIO_Type *BaseAddr,
                                              uint8 Timer,
                                              uint8 trigger,
                                              Flexio_Mcl_Ip_TriggerPolarityType triggerPolarity,
                                              Flexio_Mcl_Ip_TriggerSourceType triggerSource,
                                              uint8 pin,
                                              Flexio_Mcl_Ip_PinPolarityType PinPolarity,
                                              Flexio_Mcl_Ip_PinConfigType PinConfig,
                                              Flexio_Mcl_Ip_TimerModeType Mode)
{
    BaseAddr->TIMCTL[Timer] = FLEXIO_TIMCTL_TIMOD(Mode)
                            | FLEXIO_TIMCTL_PINSEL(pin)
                            | FLEXIO_TIMCTL_PINPOL(PinPolarity)
                            | FLEXIO_TIMCTL_PINCFG(PinConfig)
                            | FLEXIO_TIMCTL_TRGSRC(triggerSource)
                            | FLEXIO_TIMCTL_TRGPOL(triggerPolarity)
                            | FLEXIO_TIMCTL_TRGSEL(trigger);
}

/*
 * This function configures the Mode for the specified Timer.
 * In 8-bit counter Mode, the lower 8-bits of the counter and compare register are used to
 * configure the baud rate of the Timer shift clock and the upper 8-bits are used to configure
 * the shifter bit count.
 * In 8-bit PWM Mode, the lower 8-bits of the counter and compare
 * register are used to configure the high period of the Timer shift clock and the upper
 * 8-bits are used to configure the low period of the Timer shift clock. The shifter bit count
 * is configured using another Timer or external signal.
 * In 16-bit counter Mode, the full 16-bits of the counter and compare register are used to
 * configure either the baud rate of the shift clock or the shifter bit count.
 */
static inline void Flexio_I2s_Ip_SetTimerMode(FLEXIO_Type *BaseAddr,
                                             uint8 Timer,
                                             Flexio_Mcl_Ip_TimerModeType Mode)
{
    uint32 tmp = BaseAddr->TIMCTL[Timer];
    tmp &= ~(FLEXIO_TIMCTL_TIMOD_MASK);
    tmp |= FLEXIO_TIMCTL_TIMOD(Mode);
    BaseAddr->TIMCTL[Timer] = tmp;
}

/*
 * Returns the state of the error interrupt for corresponding shifter.
 */
static inline boolean Flexio_I2s_Ip_GetShifterErrorInterrupt(const FLEXIO_Type * BaseAddr, uint8 Shifter)
{
    boolean RetStatus;
    uint32 RegValue = (uint32)BaseAddr->SHIFTEIEN;

    RegValue = (RegValue >> Shifter) & 1U;
    if(RegValue != 0U)
    {
        RetStatus = TRUE;
    }
    else
    {
        RetStatus = FALSE;
    }
    return RetStatus;
}

/*
 * Returns the state of the interrupt for corresponding shifter.
 */
static inline boolean Flexio_I2s_Ip_GetShifterInterrupt(const FLEXIO_Type * BaseAddr, uint8 Shifter)
{
    boolean RetStatus;
    uint32 RegValue = (uint32)BaseAddr->SHIFTSIEN;

    RegValue = (RegValue >> Shifter) & 1U;
    if(RegValue != 0U)
    {
        RetStatus = TRUE;
    }
    else
    {
        RetStatus = FALSE;
    }
    return RetStatus;
}

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

#define I2S_STOP_SEC_CODE
#include "I2s_MemMap.h"

#endif /* (STD_ON == FLEXIO_I2S_IP_ENABLE) */

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FLEXIO_I2S_IP_HWACCESS_H */

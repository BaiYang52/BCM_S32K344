/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 0.8.1
*   Build Version        : S32K3_RTD_0_8_1_D2011_ASR_REL_4_4_REV_0000_20201127
*
*   (c) Copyright 2020 NXP Semiconductors
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
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
#include "Mcal.h"

#include "Osif.h"

#include "Clock_Ip.h"

#include "IntCtrl_Ip.h"

#include "Lcu_Ip.h"

#include <string.h>

/* Local constants */
#define MOTOR_0_INPUT_LIST_DIMENSION    ((uint8)6U)
#define MOTOR_0_OUTPUT_LIST_DIMENSION   ((uint8)6U)
#define MOTOR_1_INPUT_LIST_DIMENSION    ((uint8)6U)
#define MOTOR_1_OUTPUT_LIST_DIMENSION   ((uint8)6U)

/* MOTOR 0 Inputs and Ouputs */
Lcu_Ip_SyncInputValueType Motor0_InputList[MOTOR_0_INPUT_LIST_DIMENSION] =
{
    {MOTOR0_LOGIC_INPUT_0, FALSE},
    {MOTOR0_LOGIC_INPUT_1, FALSE},
    {MOTOR0_LOGIC_INPUT_2, FALSE},
    {MOTOR0_LOGIC_INPUT_3, FALSE},
    {MOTOR0_LOGIC_INPUT_4, FALSE},
    {MOTOR0_LOGIC_INPUT_5, FALSE}
};

Lcu_Ip_SyncOutputValueType Motor0_OutputList[MOTOR_0_OUTPUT_LIST_DIMENSION] =
{
    {MOTOR0_LOGIC_OUTPUT_0, FALSE},
    {MOTOR0_LOGIC_OUTPUT_1, FALSE},
    {MOTOR0_LOGIC_OUTPUT_2, FALSE},
    {MOTOR0_LOGIC_OUTPUT_3, FALSE},
    {MOTOR0_LOGIC_OUTPUT_4, FALSE},
    {MOTOR0_LOGIC_OUTPUT_5, FALSE}
};

/* MOTOR 1 Inputs and Ouputs */
Lcu_Ip_SyncInputValueType Motor1_InputList[MOTOR_1_INPUT_LIST_DIMENSION] =
{
    {MOTOR1_LOGIC_INPUT_0, FALSE},
    {MOTOR1_LOGIC_INPUT_1, FALSE},
    {MOTOR1_LOGIC_INPUT_2, FALSE},
    {MOTOR1_LOGIC_INPUT_3, FALSE},
    {MOTOR1_LOGIC_INPUT_4, FALSE},
    {MOTOR1_LOGIC_INPUT_5, FALSE},
};

Lcu_Ip_SyncOutputValueType Motor1_OutputList[MOTOR_1_OUTPUT_LIST_DIMENSION] =
{
    {MOTOR1_LOGIC_OUTPUT_0, FALSE},
    {MOTOR1_LOGIC_OUTPUT_1, FALSE},
    {MOTOR1_LOGIC_OUTPUT_2, FALSE},
    {MOTOR1_LOGIC_OUTPUT_3, FALSE},
    {MOTOR1_LOGIC_OUTPUT_4, FALSE},
    {MOTOR1_LOGIC_OUTPUT_5, FALSE}
};

volatile uint8 g_Output0CallbackCounter = 0U;


boolean Motor0Control(void)
{
    boolean Status = TRUE;
    uint32 Index = 0U;
    uint8  OutputCheck = 0U;

    g_Output0CallbackCounter = 0U;

    /* Motor 0 */
    /* Set input signals software override values */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Lcu_Ip_SetSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Lcu_Ip_SetSyncInputSwOverrideEnable(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Enable Motor 0 specific outputs */
    Motor0_OutputList[0U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[1U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[2U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[3U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[4U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[5U].Value = LCU_IP_OUTPUT_ENABLE;
    Lcu_Ip_SetSyncOutputEnable(&Motor0_OutputList[0U], MOTOR_0_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 0 */
    Lcu_Ip_GetSyncLogicOutput(Motor0_OutputList, MOTOR_0_OUTPUT_LIST_DIMENSION);

    /* Check Motor 0 Outputs have the corect values. */
    OutputCheck = 0U;
    for(Index = 0U; Index < MOTOR_0_OUTPUT_LIST_DIMENSION ; Index++)
    {
        OutputCheck |= (Motor0_OutputList[Index].Value << Index);
    }
    /* Verify that the output value equal 0x21U or 00010101b */
    if(0x15U != OutputCheck)
    {
        Status = FALSE;
    }

    /* Verify that: the callback counter equal 1. Because after LUT event occur callback function was executed */
    if(1U != g_Output0CallbackCounter)
    {
        Status = FALSE;
    }

    /* Disable input signal for MOTOR0_LOGIC_INPUT_0 by using API Set Input SW Override */
    /* Set input signals software override values */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Lcu_Ip_SetSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    Lcu_Ip_GetSyncLogicOutput(Motor0_OutputList, MOTOR_0_OUTPUT_LIST_DIMENSION);

    /* Check Motor 0 Outputs have the corect values. */
    OutputCheck = 0U;
    for(Index = 0U; Index < MOTOR_0_OUTPUT_LIST_DIMENSION ; Index++)
    {
        OutputCheck |= (Motor0_OutputList[Index].Value << Index);
    }
    /* Verify that the output value equal 0x2AU or 00101010b */
    if(0x2AU != OutputCheck)
    {
        Status = FALSE;
    }

    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Lcu_Ip_SetSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Lcu_Ip_SetSyncInputSwOverrideEnable(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    return Status;
}



boolean Motor1Control(void)
{
    boolean Status = TRUE;
    uint32 Index = 0U;
    uint8  OutputCheck = 0U;

    /* Motor 1 */
    /* Set Sw Override value for Motor 1 */
    Motor1_InputList[0U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor1_InputList[1U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor1_InputList[2U].Value = LCU_IP_SW_OVERRIDE_LOGIC_HIGH;
    Motor1_InputList[3U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor1_InputList[4U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Motor1_InputList[5U].Value = LCU_IP_SW_OVERRIDE_LOGIC_LOW;
    Lcu_Ip_SetSyncInputSwOverrideValue(&Motor1_InputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Set Output Lut control value for Motor 1 */
    Motor1_OutputList[0U].Value = 0x1000U;
    Motor1_OutputList[1U].Value = 0x1000U;
    Motor1_OutputList[2U].Value = 0x0002U;
    Motor1_OutputList[3U].Value = 0x0000U;
    Motor1_OutputList[4U].Value = 0x0000U;
    Motor1_OutputList[5U].Value = 0x0000U;
    Lcu_Ip_SetSyncOutputLutControl(&Motor1_OutputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor1_InputList[0U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[1U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[2U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[3U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[4U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[5U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Lcu_Ip_SetSyncInputSwOverrideEnable(&Motor1_InputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Enable Output for all outputs motor 1 */
    Motor1_OutputList[0U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[1U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[2U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[3U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[4U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[5U].Value = LCU_IP_OUTPUT_ENABLE;
    Lcu_Ip_SetSyncOutputEnable(&Motor1_OutputList[0U], MOTOR_1_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 1 */
    Lcu_Ip_GetSyncLogicOutput(Motor1_OutputList, MOTOR_1_OUTPUT_LIST_DIMENSION);

    OutputCheck = 0U;
    for(Index = 0U; Index < MOTOR_0_OUTPUT_LIST_DIMENSION ; Index++)
    {
        OutputCheck |= (Motor1_OutputList[Index].Value << Index);
    }

    /* Verify that the output value equal 0x07U or 000111b */
    if(0x07U != OutputCheck)
    {
        Status = FALSE;
    }

    /* Set Output Polarity for Motor 1 */
    Motor1_OutputList[0U].Value = LCU_IP_OUTPOL_INVERTED;
    Motor1_OutputList[1U].Value = LCU_IP_OUTPOL_NOT_INVERTED;
    Motor1_OutputList[2U].Value = LCU_IP_OUTPOL_INVERTED;
    Motor1_OutputList[3U].Value = LCU_IP_OUTPOL_NOT_INVERTED;
    Motor1_OutputList[4U].Value = LCU_IP_OUTPOL_NOT_INVERTED;
    Motor1_OutputList[5U].Value = LCU_IP_OUTPOL_NOT_INVERTED;
    Lcu_Ip_SetSyncOutputPolarity(&Motor1_OutputList[0U], MOTOR_1_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 1 */
    Lcu_Ip_GetSyncLogicOutput(Motor1_OutputList, MOTOR_1_OUTPUT_LIST_DIMENSION);

    OutputCheck = 0U;
    for(Index = 0U; Index < MOTOR_0_OUTPUT_LIST_DIMENSION ; Index++)
    {
        OutputCheck |= (Motor1_OutputList[Index].Value << Index);
    }

    /* Verify that the output value equal 0x38U or ‭01000‬b */
    if(0x02U != OutputCheck)
    {
        Status = FALSE;
    }

    return Status;
}

void lcuCallback(Lcu_Ip_Event Event)
{
    (void)Event;
    g_Output0CallbackCounter++;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    bool Status = TRUE;


    Clock_Ip_InitClock(&Clock_Ip_aClockConfig[0]);

    /*Enable the DMA0 interrupt vector */
    IntCtrl_Ip_Init(&IntCtrlConfig_0);
    IntCtrl_Ip_InstallHandler(LCU0_IRQn, Lcu0_Ip_IRQHandler, NULL_PTR);
    IntCtrl_Ip_EnableIrq(LCU0_IRQn);

    Lcu_Ip_Init(&Lcu_Ip_xLcuInitPB);

    if(TRUE == Status)
    {
        Status = Motor0Control();
    }

    if(TRUE == Status)
    {
        Status = Motor1Control();
    }

    if (FALSE == Status)
    {
        DevAssert(FALSE);
    }

    Lcu_Ip_Deinit();

    return (0U);
}

/* END main */
/*!
** @}
*/

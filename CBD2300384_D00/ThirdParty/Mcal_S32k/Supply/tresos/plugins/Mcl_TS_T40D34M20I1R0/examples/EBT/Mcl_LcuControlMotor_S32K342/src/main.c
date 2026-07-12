/*
*   (c) Copyright 2020 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
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
#include "Mcl_Example_EBT_OVER_LCU.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define MOTOR_0_INPUT_LIST_DIMENSION    ((uint8)6U)
#define MOTOR_0_OUTPUT_LIST_DIMENSION   ((uint8)6U)
#define MOTOR_1_INPUT_LIST_DIMENSION    ((uint8)6U)
#define MOTOR_1_OUTPUT_LIST_DIMENSION   ((uint8)6U)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/* MOTOR 0 Inputs and Ouputs */
Mcl_LcuSyncInputValueType Motor0_InputList[MOTOR_0_INPUT_LIST_DIMENSION] =
{
    {MOTOR0_LOGIC_INPUT_0, FALSE},
    {MOTOR0_LOGIC_INPUT_1, FALSE},
    {MOTOR0_LOGIC_INPUT_2, FALSE},
    {MOTOR0_LOGIC_INPUT_3, FALSE},
    {MOTOR0_LOGIC_INPUT_4, FALSE},
    {MOTOR0_LOGIC_INPUT_5, FALSE}
};

Mcl_LcuSyncOutputValueType Motor0_OutputList[MOTOR_0_OUTPUT_LIST_DIMENSION] =
{
    {MOTOR0_LOGIC_OUTPUT_0, FALSE},
    {MOTOR0_LOGIC_OUTPUT_1, FALSE},
    {MOTOR0_LOGIC_OUTPUT_2, FALSE},
    {MOTOR0_LOGIC_OUTPUT_3, FALSE},
    {MOTOR0_LOGIC_OUTPUT_4, FALSE},
    {MOTOR0_LOGIC_OUTPUT_5, FALSE}
};

/* MOTOR 1 Inputs and Ouputs */
Mcl_LcuSyncInputValueType Motor1_InputList[MOTOR_1_INPUT_LIST_DIMENSION] =
{
    {MOTOR1_LOGIC_INPUT_0, FALSE},
    {MOTOR1_LOGIC_INPUT_1, FALSE},
    {MOTOR1_LOGIC_INPUT_2, FALSE},
    {MOTOR1_LOGIC_INPUT_3, FALSE},
    {MOTOR1_LOGIC_INPUT_4, FALSE},
    {MOTOR1_LOGIC_INPUT_5, FALSE},
};

Mcl_LcuSyncOutputValueType Motor1_OutputList[MOTOR_1_OUTPUT_LIST_DIMENSION] =
{
    {MOTOR1_LOGIC_OUTPUT_0, FALSE},
    {MOTOR1_LOGIC_OUTPUT_1, FALSE},
    {MOTOR1_LOGIC_OUTPUT_2, FALSE},
    {MOTOR1_LOGIC_OUTPUT_3, FALSE},
    {MOTOR1_LOGIC_OUTPUT_4, FALSE},
    {MOTOR1_LOGIC_OUTPUT_5, FALSE}
};
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
volatile uint8 g_Output0CallbackCounter = 0U;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
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
    Mcl_SetLcuSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Mcl_SetLcuSyncInputSwOverrideEnable(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Enable Motor 0 specific outputs */
    Motor0_OutputList[0U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[1U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[2U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[3U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[4U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor0_OutputList[5U].Value = LCU_IP_OUTPUT_ENABLE;
    Mcl_SetLcuSyncOutputEnable(&Motor0_OutputList[0U], MOTOR_0_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 0 */
    Mcl_GetLcuSyncLogicOutput(Motor0_OutputList, MOTOR_0_OUTPUT_LIST_DIMENSION);

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
    Mcl_SetLcuSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    Mcl_GetLcuSyncLogicOutput(Motor0_OutputList, MOTOR_0_OUTPUT_LIST_DIMENSION);

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
    Mcl_SetLcuSyncInputSwOverrideValue(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor0_InputList[0U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[1U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[2U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[3U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[4U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Motor0_InputList[5U].Value = LCU_IP_SW_OVERRIDE_DISABLE;
    Mcl_SetLcuSyncInputSwOverrideEnable(&Motor0_InputList[0U], MOTOR_0_INPUT_LIST_DIMENSION);

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
    Mcl_SetLcuSyncInputSwOverrideValue(&Motor1_InputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Set Output Lut control value for Motor 1 */
    Motor1_OutputList[0U].Value = 0x1000U;
    Motor1_OutputList[1U].Value = 0x1000U;
    Motor1_OutputList[2U].Value = 0x0002U;
    Motor1_OutputList[3U].Value = 0x0000U;
    Motor1_OutputList[4U].Value = 0x0000U;
    Motor1_OutputList[5U].Value = 0x0000U;
    Mcl_SetLcuSyncOutputLutControl(&Motor1_OutputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Set input signals by enabling software override */
    Motor1_InputList[0U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[1U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[2U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[3U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[4U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Motor1_InputList[5U].Value = LCU_IP_SW_OVERRIDE_ENABLE;
    Mcl_SetLcuSyncInputSwOverrideEnable(&Motor1_InputList[0U], MOTOR_1_INPUT_LIST_DIMENSION);

    /* Enable Output for all outputs motor 1 */
    Motor1_OutputList[0U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[1U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[2U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[3U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[4U].Value = LCU_IP_OUTPUT_ENABLE;
    Motor1_OutputList[5U].Value = LCU_IP_OUTPUT_ENABLE;
    Mcl_SetLcuSyncOutputEnable(&Motor1_OutputList[0U], MOTOR_1_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 1 */
    Mcl_GetLcuSyncLogicOutput(Motor1_OutputList, MOTOR_1_OUTPUT_LIST_DIMENSION);

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
    Mcl_SetLcuSyncOutputPolarity(&Motor1_OutputList[0U], MOTOR_1_OUTPUT_LIST_DIMENSION);

    /* Get state of output 0 motor 1 */
    Mcl_GetLcuSyncLogicOutput(Motor1_OutputList, MOTOR_1_OUTPUT_LIST_DIMENSION);

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
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void lcuCallback(Lcu_Ip_Event Event)
{
    (void)Event;
    g_Output0CallbackCounter++;
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
int main(void)
{
    boolean Status = TRUE;

    /* Initialize Mcu Driver */
#if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
#elif (MCU_PRECOMPILE_SUPPORT == STD_OFF)
    Mcu_Init(&Mcu_Config_VS_0);
#endif /* (MCU_PRECOMPILE_SUPPORT == STD_ON) */
    Mcu_InitClock(McuClockSettingConfig_Default);

#if (MCU_NO_PLL == STD_OFF)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }

    Mcu_DistributePllClock();
#endif

    Mcu_SetMode(McuModeSettingConf_Default);

    /* Initialize Platform Driver */
    Platform_Init(NULL_PTR);
    Platform_InstallIrqHandler(LCU0_IRQn, Lcu0_Ip_IRQHandler, NULL);
    /* Initialize MCL Driver */
    Mcl_Init(NULL_PTR);

    if(TRUE == Status)
    {
        Status = Motor0Control();
    }

    if(TRUE == Status)
    {
        Status = Motor1Control();
    }

    /* De-Initialize MCL driver */
    Mcl_DeInit();

    Exit_Example(Status);

    return(0U);
}


#ifdef __cplusplus
}
#endif

/** @} */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *             File:  Rte_Hook.h
 *           Config:  BCM_S32K344.dpa
 *      ECU-Project:  BCM_S32K344
 *
 *        Generator:  MICROSAR RTE Generator Version 4.32.0
 *                    RTE Core Version 4.32.0
 *          License:  CBD2300384
 *
 *      Description:  Header file containing definitions for VFB tracing
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Names of available VFB-Trace-Hooks
 **********************************************************************************************************************
 *
 * Not configured:
 *
 *  Rte_Runnable_BswM_BswM_MainFunction_Return
 *  Rte_Runnable_BswM_BswM_MainFunction_Start
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_1000ms_Return
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_1000ms_Start
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_10ms_Return
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_10ms_Start
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_Init_Return
 *  Rte_Runnable_CtAppSwc_RCtAppSwc_Init_Start
 *  Rte_Runnable_EcuM_EcuM_MainFunction_Return
 *  Rte_Runnable_EcuM_EcuM_MainFunction_Start
 *  Rte_Runnable_EcuM_GetBootTarget_Return
 *  Rte_Runnable_EcuM_GetBootTarget_Start
 *  Rte_Runnable_EcuM_GetLastShutdownTarget_Return
 *  Rte_Runnable_EcuM_GetLastShutdownTarget_Start
 *  Rte_Runnable_EcuM_GetShutdownCause_Return
 *  Rte_Runnable_EcuM_GetShutdownCause_Start
 *  Rte_Runnable_EcuM_GetShutdownTarget_Return
 *  Rte_Runnable_EcuM_GetShutdownTarget_Start
 *  Rte_Runnable_EcuM_SelectBootTarget_Return
 *  Rte_Runnable_EcuM_SelectBootTarget_Start
 *  Rte_Runnable_EcuM_SelectShutdownCause_Return
 *  Rte_Runnable_EcuM_SelectShutdownCause_Start
 *  Rte_Runnable_EcuM_SelectShutdownTarget_Return
 *  Rte_Runnable_EcuM_SelectShutdownTarget_Start
 *  Rte_Runnable_Os_OsCore0_swc_GetCounterValue_Return
 *  Rte_Runnable_Os_OsCore0_swc_GetCounterValue_Start
 *  Rte_Runnable_Os_OsCore0_swc_GetElapsedValue_Return
 *  Rte_Runnable_Os_OsCore0_swc_GetElapsedValue_Start
 *  Rte_Task_Activate
 *  Rte_Task_Dispatch
 *  Rte_Task_Terminate
 *  Rte_Task_WaitEvent
 *  Rte_Task_WaitEventRet
 *  SchM_EnterHook_BswM_BSWM_EXCLUSIVE_AREA_0_Return
 *  SchM_EnterHook_BswM_BSWM_EXCLUSIVE_AREA_0_Start
 *  SchM_EnterHook_Det_DET_EXCLUSIVE_AREA_0_Return
 *  SchM_EnterHook_Det_DET_EXCLUSIVE_AREA_0_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_00_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_00_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_01_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_01_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_02_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_02_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_03_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_03_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_04_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_04_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_05_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_05_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_06_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_06_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_07_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_07_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_08_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_08_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_09_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_09_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_10_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_10_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_11_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_11_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_12_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_12_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_13_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_13_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_14_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_14_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_15_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_15_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_16_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_16_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_17_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_17_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_18_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_18_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_19_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_19_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_20_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_20_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_21_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_21_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_22_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_22_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_23_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_23_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_24_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_24_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_25_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_25_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_26_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_26_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_27_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_27_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_28_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_28_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_29_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_29_Start
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_30_Return
 *  SchM_EnterHook_Dio_DIO_EXCLUSIVE_AREA_30_Start
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_0_Return
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_0_Start
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_1_Return
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_1_Start
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_2_Return
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_2_Start
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_3_Return
 *  SchM_EnterHook_EcuM_ECUM_EXCLUSIVE_AREA_3_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_00_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_00_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_01_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_01_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_02_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_02_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_03_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_03_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_04_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_04_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_05_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_05_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_06_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_06_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_07_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_07_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_08_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_08_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_09_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_09_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_10_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_10_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_11_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_11_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_12_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_12_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_13_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_13_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_14_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_14_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_15_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_15_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_16_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_16_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_17_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_17_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_18_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_18_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_19_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_19_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_20_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_20_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_21_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_21_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_22_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_22_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_23_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_23_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_24_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_24_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_25_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_25_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_26_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_26_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_27_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_27_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_28_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_28_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_29_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_29_Start
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_30_Return
 *  SchM_EnterHook_Mcu_MCU_EXCLUSIVE_AREA_30_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_00_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_00_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_01_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_01_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_02_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_02_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_03_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_03_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_04_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_04_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_05_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_05_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_06_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_06_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_07_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_07_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_08_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_08_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_09_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_09_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_10_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_10_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_11_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_11_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_12_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_12_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_13_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_13_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_14_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_14_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_15_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_15_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_16_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_16_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_17_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_17_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_18_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_18_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_19_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_19_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_20_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_20_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_21_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_21_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_22_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_22_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_23_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_23_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_24_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_24_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_25_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_25_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_26_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_26_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_27_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_27_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_28_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_28_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_29_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_29_Start
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_30_Return
 *  SchM_EnterHook_Port_PORT_EXCLUSIVE_AREA_30_Start
 *  SchM_ExitHook_BswM_BSWM_EXCLUSIVE_AREA_0_Return
 *  SchM_ExitHook_BswM_BSWM_EXCLUSIVE_AREA_0_Start
 *  SchM_ExitHook_Det_DET_EXCLUSIVE_AREA_0_Return
 *  SchM_ExitHook_Det_DET_EXCLUSIVE_AREA_0_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_00_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_00_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_01_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_01_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_02_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_02_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_03_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_03_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_04_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_04_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_05_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_05_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_06_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_06_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_07_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_07_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_08_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_08_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_09_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_09_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_10_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_10_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_11_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_11_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_12_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_12_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_13_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_13_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_14_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_14_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_15_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_15_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_16_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_16_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_17_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_17_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_18_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_18_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_19_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_19_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_20_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_20_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_21_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_21_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_22_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_22_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_23_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_23_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_24_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_24_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_25_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_25_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_26_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_26_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_27_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_27_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_28_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_28_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_29_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_29_Start
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_30_Return
 *  SchM_ExitHook_Dio_DIO_EXCLUSIVE_AREA_30_Start
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_0_Return
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_0_Start
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_1_Return
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_1_Start
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_2_Return
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_2_Start
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_3_Return
 *  SchM_ExitHook_EcuM_ECUM_EXCLUSIVE_AREA_3_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_00_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_00_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_01_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_01_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_02_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_02_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_03_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_03_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_04_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_04_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_05_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_05_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_06_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_06_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_07_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_07_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_08_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_08_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_09_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_09_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_10_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_10_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_11_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_11_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_12_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_12_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_13_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_13_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_14_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_14_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_15_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_15_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_16_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_16_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_17_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_17_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_18_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_18_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_19_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_19_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_20_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_20_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_21_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_21_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_22_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_22_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_23_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_23_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_24_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_24_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_25_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_25_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_26_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_26_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_27_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_27_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_28_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_28_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_29_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_29_Start
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_30_Return
 *  SchM_ExitHook_Mcu_MCU_EXCLUSIVE_AREA_30_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_00_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_00_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_01_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_01_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_02_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_02_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_03_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_03_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_04_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_04_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_05_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_05_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_06_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_06_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_07_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_07_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_08_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_08_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_09_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_09_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_10_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_10_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_11_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_11_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_12_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_12_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_13_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_13_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_14_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_14_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_15_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_15_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_16_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_16_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_17_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_17_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_18_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_18_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_19_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_19_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_20_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_20_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_21_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_21_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_22_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_22_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_23_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_23_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_24_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_24_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_25_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_25_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_26_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_26_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_27_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_27_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_28_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_28_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_29_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_29_Start
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_30_Return
 *  SchM_ExitHook_Port_PORT_EXCLUSIVE_AREA_30_Start
 *  SchM_Schedulable_BswM_BswM_MainFunction_Return
 *  SchM_Schedulable_BswM_BswM_MainFunction_Start
 *  SchM_Schedulable_EcuM_EcuM_MainFunction_Return
 *  SchM_Schedulable_EcuM_EcuM_MainFunction_Start
 *
 *********************************************************************************************************************/

/* double include prevention */
#ifndef RTE_HOOK_H
# define RTE_HOOK_H

# include "Os.h" /* PRQA S 0828, 0883 */ /* MD_MSR_Dir1.1, MD_Rte_Os */

# include "Rte_Type.h"
# include "Rte_Cfg.h"

# ifndef RTE_VFB_TRACE
#  define RTE_VFB_TRACE (0)
# endif

#endif /* RTE_HOOK_H */

/**********************************************************************************************************************
 MISRA 2012 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_Os:
     Reason:     This justification is used as summary justification for all deviations caused by the MICROSAR OS
                 which is for testing of the RTE. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

*/

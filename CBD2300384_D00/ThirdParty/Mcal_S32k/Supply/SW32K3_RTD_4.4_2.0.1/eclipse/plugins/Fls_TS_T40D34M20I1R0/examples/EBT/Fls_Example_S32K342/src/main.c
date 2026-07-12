/*
*   (c) Copyright 2022 NXP
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
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcu.h"
#include "Port.h"
#include "SchM_Fls.h"
#include "Fls.h"

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*
   FlsSectorList | Fls Sector Type | Fls Sector Size | Fls Number | Fls Logical    | Fls Physical  |
                 |                 |                 | Of Sector  | Start Address  | Start Address |
   ------------- | --------------- | --------------- | -----------| -------------- | ------------- |
   FlsSector_0   | External        | 4096 (0x1000)   |     1      |     0x0000     |      0x1000   |
   FlsSector_1   | External        | 4096 (0x1000)   |     1      |     0x1000     |      0x2000   |
   FlsSector_2   | External        | 4096 (0x1000)   |     1      |     0x2000     |      0x3000   |
   FlsSector_3   | External        | 4096 (0x1000)   |     1      |     0x3000     |      0x4000   |
   FlsSector_4   | Internal        | 8192 (0x2000)   |     1      |     0x4000     |  0x10080000   |
   FlsSector_5   | Internal        | 8192 (0x2000)   |     1      |     0x6000     |  0x100A0000   |
*/

#define FLS_BUF_SIZE                 1024U

#define LOGICAL_START_ADDR           0x0000U
#define PHYSICAL_START_ADDR          0x1000U    /* The HW start address coresponding to the logical address 0 */

#define EXTERNAL_SECTOR_SIZE         0x1000U
#define NUMBER_OF_EXTERNAL_SECTOR    4U

#define INTERNAL_SECTOR_SIZE         0x2000U
#define NUMBER_OF_INTERNAL_SECTOR    2U

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint8 TxBuffer[FLS_BUF_SIZE];
uint8 RxBuffer[FLS_BUF_SIZE];
uint8 RxBuffer_AHB[FLS_BUF_SIZE];

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
void Fls_InitBuffers(void);

void Fls_Test_InternalSector(void);
void Fls_Test_ExternalSector(void);

void Fls_ExampleAssert(boolean Condition);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/**
 * @brief Implement a simple assert macro
 *
 * @param  Condition
 * @return void
 */
void Fls_ExampleAssert(boolean Condition)
{
    while (!Condition)
    {
        /* Loop forever */
    }
}


void Fls_InitBuffers(void)
{
    uint32 Index;

    for (Index = 0U; Index < FLS_BUF_SIZE; Index++)
    {
        TxBuffer[Index] = (uint8)Index;
        RxBuffer[Index] = 0U;
        RxBuffer_AHB[Index] = 0U;
    }
}


void Fls_Test_ExternalSector(void)
{
    /* The start address of external memory section */
    Fls_AddressType TargetAddress = LOGICAL_START_ADDR;
    Std_ReturnType Status;
    uint32 Index;

    /* Initialize Tx and Rx Buffer */
    Fls_InitBuffers();


    /* Write to external memory */
    Status = Fls_Write(TargetAddress, TxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Compare data in external sector with TxBuffer */
    Status = Fls_Compare(TargetAddress, TxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Read data from external memory */
    Status = Fls_Read(TargetAddress, RxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Read data from external memory via AHB command */

    /* Table of AHB addresses for QuadSPI instances */
    const uint32 Qspi_Ip_AhbAddress[QuadSPI_INSTANCE_COUNT] = QuadSPI_AHB_PTRS;
    /* Get AHB address of QSPI_0 */
    uint8* pAHBExAddr = (uint8*)(Qspi_Ip_AhbAddress[0U] + PHYSICAL_START_ADDR);

    for (uint32 i = 0U; i < FLS_BUF_SIZE; i++)
    {
        RxBuffer_AHB[i] = pAHBExAddr[i];
    }


    /* Verify data */
    for (Index = 0U; Index < FLS_BUF_SIZE; Index++)
    {
        Fls_ExampleAssert(TxBuffer[Index] == RxBuffer[Index]);
        Fls_ExampleAssert(TxBuffer[Index] == RxBuffer_AHB[Index]);
    }
}


void Fls_Test_InternalSector(void)
{
    /* The start address of internal memory section */
    Fls_AddressType TargetAddress = LOGICAL_START_ADDR + NUMBER_OF_EXTERNAL_SECTOR * EXTERNAL_SECTOR_SIZE;
    Std_ReturnType Status;
    uint32 Index;

    /* Initialize Tx and Rx Buffer */
    Fls_InitBuffers();


    /* Write to internal FLS */
    Status = Fls_Write(TargetAddress, TxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Compare data in internal sector with TxBuffer */
    Status = Fls_Compare(TargetAddress, TxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Read data from internal sector */
    Status = Fls_Read(TargetAddress, RxBuffer, FLS_BUF_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Verify data */
    for (Index = 0U; Index < FLS_BUF_SIZE; Index++)
    {
        Fls_ExampleAssert(TxBuffer[Index] == RxBuffer[Index]);
    }
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Main function of the example
* @details      Initializes the used drivers and uses the Fls driver to erase, write and read the memory
*/
int main(void)
{
    Std_ReturnType Status;

    /* Initialize clock */
    #if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
    #elif (MCU_PRECOMPILE_SUPPORT == STD_OFF)
    Mcu_Init(&Mcu_Config_VS_0);
    #endif

    Mcu_InitClock(McuClockSettingConfig_0);
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();
    Mcu_SetMode(McuModeSettingConf_0);


    /* Initialize Qspi's pins */
    Port_Init(NULL_PTR);


    /* Initialize Fls driver */
    Fls_Init(NULL_PTR);
    /* Check Fls driver status */
    Fls_ExampleAssert(MEMIF_IDLE == Fls_GetStatus());


    /* Erase all configured sectors */
    Status = Fls_Erase(LOGICAL_START_ADDR, NUMBER_OF_EXTERNAL_SECTOR * EXTERNAL_SECTOR_SIZE + NUMBER_OF_INTERNAL_SECTOR * INTERNAL_SECTOR_SIZE);
    /* Check return status */
    Fls_ExampleAssert(E_OK == Status);
    /* Perform the job  */
    while (MEMIF_IDLE != Fls_GetStatus())
    {
        Fls_MainFunction();
    }
    /* Check job result */
    Fls_ExampleAssert(MEMIF_JOB_OK == Fls_GetJobResult());


    /* Run test with external sectors */
    Fls_Test_ExternalSector();

    /* Run test with internal sectors */
    Fls_Test_InternalSector();


    /* If we get here it means the test has passed */
    Exit_Example(TRUE);
    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

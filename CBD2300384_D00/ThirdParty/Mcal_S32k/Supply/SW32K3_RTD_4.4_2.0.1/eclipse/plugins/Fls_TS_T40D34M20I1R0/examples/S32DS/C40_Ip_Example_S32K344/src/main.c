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
#include "Clock_Ip.h"
#include "C40_Ip.h"

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define FLS_MASTER_ID                0U
#define FLS_BUF_SIZE                 80U
#define FLS_SECTOR_ADDR              0x00600000U
#define FLS_SECTOR_TEST              C40_CODE_ARRAY_0_BLOCK_2_S256


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
uint8 TxBuffer[FLS_BUF_SIZE];
uint8 RxBuffer[FLS_BUF_SIZE];


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
void Fls_InitBuffers(void);
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
    }
}


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Main function of the example
* @details      Initializes IP C40 driver and erase, write, read internal flash memory
*/
int main(void)
{
    uint32 Index;
    C40_Ip_StatusType C40Status;
    Clock_Ip_StatusType ClockStatus;

    /* Initialize clock */
    ClockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    Fls_ExampleAssert(ClockStatus == CLOCK_IP_SUCCESS);

    /* Initialize C40 driver */
    C40Status = C40_Ip_Init(&C40ConfigSet_VS_0_InitCfg);
    Fls_ExampleAssert(STATUS_C40_IP_SUCCESS == C40Status);

    /* Initialize buffers */
    Fls_InitBuffers();

    /* Unlock sector if needed */
    if (STATUS_C40_IP_SECTOR_PROTECTED == C40_Ip_GetLock(FLS_SECTOR_TEST))
    {
        C40_Ip_ClearLock(FLS_SECTOR_TEST, FLS_MASTER_ID);
    }

    /* Erase sector */
    C40_Ip_MainInterfaceSectorErase(FLS_SECTOR_TEST, FLS_MASTER_ID);
    do
    {
        C40Status = C40_Ip_MainInterfaceSectorEraseStatus();
    }
    while (STATUS_C40_IP_BUSY == C40Status);
    /* Check job result */
    Fls_ExampleAssert(STATUS_C40_IP_SUCCESS == C40Status);

    /* Write data */
    C40_Ip_MainInterfaceWrite(FLS_SECTOR_ADDR, FLS_BUF_SIZE, TxBuffer, FLS_MASTER_ID);
    do
    {
        C40Status = C40_Ip_MainInterfaceWriteStatus();
    }
    while (STATUS_C40_IP_BUSY == C40Status);
    /* Check job result */
    Fls_ExampleAssert(STATUS_C40_IP_SUCCESS == C40Status);

    /* Compare the data in memory with TxBuffer */
    C40Status = C40_Ip_Compare(FLS_SECTOR_ADDR, FLS_BUF_SIZE, TxBuffer);
    /* Check job result */
    Fls_ExampleAssert(STATUS_C40_IP_SUCCESS == C40Status);

    /* Read data */
    C40Status = C40_Ip_Read(FLS_SECTOR_ADDR, FLS_BUF_SIZE, RxBuffer);
    /* Check job result */
    Fls_ExampleAssert(STATUS_C40_IP_SUCCESS == C40Status);

    /* Verify data */
    for (Index = 0U; Index < FLS_BUF_SIZE; Index++)
    {
        Fls_ExampleAssert(TxBuffer[Index] == RxBuffer[Index]);
    }

    /* If we get here it means the test has passed */
    Exit_Example(TRUE);
    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

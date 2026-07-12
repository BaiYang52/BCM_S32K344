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
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mpu_M7_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define T_REGION                        (7U)
#define T_MEMMANAGE_VECTOR_OFFSET       (4U)
#define SRAM_REGION_1_ADDR              (0x20440000UL)
#define SRAM_REGION_1                   (*(volatile uint32*)SRAM_REGION_1_ADDR)
#define PATTERN_0                       (0x5A5A5A5AUL)
#define PATTERN_1                       (0x6B6B6B6BUL)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
volatile boolean gMpuErrorDetected = FALSE;
volatile boolean gHardfaultOccured = FALSE;
Mpu_M7_Ip_ErrorDetailsType gMpuErrorDetails;
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void Delay_Rm(volatile uint32 u32Cycles);
static void SetHandler_Rm(uint32 offset, uint32 newHandler);
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/* MemoryManagement Handler */
void MemManage_Handler_Rm(void)
{
    /* Get error details */
    gHardfaultOccured = TRUE;
    /*Get error detail from Mpu*/
    gMpuErrorDetected =  Mpu_M7_Ip_GetErrorDetails(&gMpuErrorDetails);
    /* Clear all errors */
    S32_SCB->CFSR |= MPU_M7_IP_SCB_CFSR_MMFSR_MASK_WITHOUT_VALID;
    /* Restore access rights to the region */
    Mpu_M7_Ip_SetAccessRight(T_REGION, MPU_M7_PRIV_RWX_UNPRIV_RWX);
}
/* Install exception handlers */
static void SetHandler_Rm(uint32 offset, uint32 newHandler)
{
    uint32 * vector = (uint32 *)S32_SCB->VTOR;
    vector[offset] = newHandler;
}
/* Dummy delay function */
static void Delay_Rm(volatile uint32 u32Cycles)
{
    while(u32Cycles--);
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Initializes Mpu M7 drivers.
*               Override exception handler.
*               Perform violation access.
*               Verify error information.
*/
int main(void)
{
    boolean Status = TRUE;

    /*Initialize MPU M7*/
    Mpu_M7_Ip_Init(&MPU_M7_ModuleConfig_0);

    /*Custom interrupt handler to handle hardfault interrupt when performing violates access */
    SetHandler_Rm(T_MEMMANAGE_VECTOR_OFFSET, (uint32)MemManage_Handler_Rm);

    /* Delay to waiting for sure that new exception handlers available*/
    Delay_Rm(0x1000UL);

    /* Perform access by write to SRAM_REGION_1*/
    SRAM_REGION_1 = PATTERN_0;
    Delay_Rm(0x1000UL);
    /*Check if hardfault interrupt occurred. It should not occur*/
    if(gHardfaultOccured == TRUE)
    {
        Status = FALSE;
    }
    /* Remove access rights of the MPU region */
    Mpu_M7_Ip_SetAccessRight(T_REGION, MPU_M7_PRIV_UNPRIV_NONE);

    /* Perform violates access by write to SRAM_REGION_1, this should trigger an hardfault interrupt and MPU error as the access is blocked */
    SRAM_REGION_1 = PATTERN_1;

    /*Wait hardfault happen*/
    Delay_Rm(0x1000UL);

    /*Check if hardfault interrupt occurred*/
    if(gHardfaultOccured == TRUE)
    {
        /* Check if get error successfully*/
        if(gMpuErrorDetected == TRUE)
        {
            /*Check error address and error type*/
            if((gMpuErrorDetails.u32Address != SRAM_REGION_1_ADDR) || (gMpuErrorDetails.eType != MPU_M7_MEMMAN_ERROR_DATA_ACCESS))
            {
                Status = FALSE;
            }
        }
        else
        {
            Status = FALSE;
        }
    }
    else
    {
        Status = FALSE;
    }
    Exit_Example(Status);
    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

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

#include "CDD_Rm.h"

#if defined(S32K342)
#include "S32K342_SIUL2.h"
#elif defined(S32K344)
#include "S32K344_SIUL2.h"
#elif defined(S32K312)
#include "S32K312_SIUL2.h"
#endif

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define T_REGION                             (7U)
#define T_HF_VECTOR_OFFSET                   (3U)
#define T_MEMMANAGE_VECTOR_OFFSET            (4U)
#define XRDC_INSTANCE                        (XRDC_INSTANCE0)

#if defined(S32K344)
#define SRAM_REGION_ADDR                     (0x20440000UL)
#elif defined(S32K342)
#define SRAM_REGION_ADDR                     (0x2040c000UL)
#elif defined(S32K312)
#define SRAM_REGION_ADDR                     (0x20410000UL)
#endif
#define SRAM_REGION                          (*(volatile uint32*)SRAM_REGION_ADDR)

#define IP_SIUL2_MIRROR1_PDAC1_M7_0_0        ((SIUL2_Type *)(IP_SIUL2_BASE + 0x8000))
#define IP_SIUL2_MIRROR2_PDAC1_M7_0_1        ((SIUL2_Type *)(IP_SIUL2_BASE + 0xC000))

#define TEST_ADDR                            (uint32)(&IP_SIUL2_MIRROR2_PDAC1_M7_0_1->MSCR[2])

#define PATTERN                              (0x5A5A5A5AUL)
#define PATTERN1                             (0x6B6B6B6BUL)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
volatile Std_ReturnType gMpuErrorDetected = E_NOT_OK;
volatile Std_ReturnType gXrdcErrorDetected = E_NOT_OK;
volatile boolean gMpuMemfaultOccured = FALSE;
Rm_Mpu_M7_ErrorDetailsType gMpuErrorDetails;
Rm_XrdcDomainIDErrStatusType gXrdcErrorDetails;

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/* Dummy delay function */
void Delay_Rm(volatile uint32 u32Cycles)
{
    while(u32Cycles--);
}

/* Hardfault Handler */
void HardFault_Handler_Rm(void)
{
    Rm_XrdcGetDomainIDErrorStatus(XRDC_INSTANCE, &gXrdcErrorDetails);
    gXrdcErrorDetected = E_OK;
}

/* MemoryManagement Handler */
void MemManage_Handler_Rm(void)
{
    gMpuMemfaultOccured = TRUE;
    /* Get error details */
    gMpuErrorDetected =  Rm_Mpu_M7_GetErrorDetails(&gMpuErrorDetails);
    /* Clear all errors */
    S32_SCB->CFSR |= MPU_M7_IP_SCB_CFSR_MMFSR_MASK_WITHOUT_VALID;
    /* Restore access rights to the region */
    Rm_Mpu_M7_SetAccessRight(T_REGION, MPU_M7_PRIV_RWX_UNPRIV_RWX);
}

/* Install exception handlers */
uint32 SetHandler_Rm(uint32 offset, uint32 newHandler)
{
    uint32 * vector = (uint32 *)S32_SCB->VTOR;
    uint32 oldHandler = vector[offset];
    vector[offset] = newHandler;
    return oldHandler;
}

/* Validate Xrdc Error Details */
boolean CheckXrdcError(void)
{
    boolean bReturnCode = FALSE;
    uint32 u32DomainId;
    uint32 u32ErrorPort;
    /* Get current domain ID from XRDC */
    u32DomainId = Rm_XrdcGetDomainID(0);

    if(gXrdcErrorDetails.DomainIDAccessError != u32DomainId)
    {
        return FALSE;
    }

    for(u32ErrorPort = 0; u32ErrorPort < sizeof(gXrdcErrorDetails.ErrorStatus)/sizeof(Xrdc_Ip_ErrorStatusType); u32ErrorPort++)
    {
        if(gXrdcErrorDetails.ErrorStatus[u32ErrorPort].u32AddError == TEST_ADDR)
        {
            bReturnCode = TRUE;
            /* Check that the error was only reported once */
            if(gXrdcErrorDetails.ErrorStatus[u32ErrorPort].ErrState != XRDC_IP_SINGLE_ACCESS)
            {
                bReturnCode = FALSE;
            }
            /* Check that the error was reported on a read operation */
            if(gXrdcErrorDetails.ErrorStatus[u32ErrorPort].ErrAccess != XRDC_IP_WRITE_ACCESS)
            {
                bReturnCode = FALSE;
            }
            /* Check that the error was reported on a secure data access */
            if(gXrdcErrorDetails.ErrorStatus[u32ErrorPort].ErrAttribute != XRDC_IP_SECUPRIV_DATA_ACCESS)
            {
                bReturnCode = FALSE;
            }
            break;
        }
    }

    return bReturnCode;
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Rm_Init to init XRDC, Virt Wrapper, Mpu M7.
*               Example for XRDC and Virt Wrapper:
*                   RM_Init configures Virt Wrapper with MSCR 1 access by MIRROR1_PDAC1_M7_0_0 and MSCR 2 access by MIRROR2_PDAC1_M7_0_1.
*                   Also configures XRDC to have access right for IP_SIUL2_MIRROR2_PDAC1_M7_0_1 and to have not access IP_SIUL2_MIRROR2_PDAC1_M7_0_1.
*                   Try to write to MSCR 1 from IP_SIUL2_MIRROR1_PDAC1_M7_0_0.
*                   Verify that MSCR 1 from IP_SIUL2_BASE (PDAC0) has been synchronized.
*                   Try to write to MSCR 2 from IP_SIUL2_MIRROR2_PDAC1_M7_0_1 to trigger and an exception. XRDC should capture error and error address inside hardfault handlers
*                   Verify detail error for XRDC
*               Example for Mpu M7:
*                   RM_Init configure MPU to have access right for SRAM REGION.
*                   Try to write PATTERN1 to SRAM_REGION.
*                   Verify that no memfault handler occur.
*                   Try to use Rm_Mpu_M7_SetAccessRight to take away right access of core for SRAM_REGION and Try to write PATTERN to SRAM_REGION. This should trigger an exception for memory management.
*                   Verify that memfault handler occur and detail error for MPU is correct
*/
volatile boolean Status = TRUE;
int main(void)
{
    /* Initialize RM CDD */
    Rm_Init(NULL_PTR);

    /* Install exception handlers */
    (void)SetHandler_Rm(T_HF_VECTOR_OFFSET, (uint32)HardFault_Handler_Rm);
    (void)SetHandler_Rm(T_MEMMANAGE_VECTOR_OFFSET, (uint32)MemManage_Handler_Rm);

    /* Delay to waiting for new exception handlers available*/
    Delay_Rm(0x1000UL);

    /******************* Example for XRDC and Virt Wrapper ******************/
    /*  1: Configutation: Vitual wrapper: MSCR 1 access by MIRROR 1 - PDAC1_M7_0_0,  XRDC: full access to Virtual Wrapper PDAC1_M7_0_0
    Check point: write to MSCR 1 by MIRROR1_PDAC1_M7_0_0  -> write success - no XRDC error
        2: Configutation: Vitual wrapper: MSCR 2 access by MIRROR 2 - PDAC1_M7_0_1,  XRDC: no access to Virtual Wrapper PDAC1_M7_0_1
    Check point: write to MSCR 2 by MIRROR2_PDAC1_M7_0_1  -> write fail - XRDC error */

    /*Write to MSCR 1 by MIRROR1_PDAC1_M7_0_0 */
    IP_SIUL2_MIRROR1_PDAC1_M7_0_0->MSCR[1] = SIUL2_MSCR_IBE(1);
    Delay_Rm(0x1000UL);

    /* Verify that MSCR 1 from IP_SIUL2_BASE (PDAC0) has been synchronized.*/
    if(IP_SIUL2->MSCR[1] != SIUL2_MSCR_IBE(1))
    {
       Status = FALSE;
    }

    /* Write to MSCR 2 by MIRROR2_PDAC1_M7_0_1 */
    IP_SIUL2_MIRROR2_PDAC1_M7_0_1->MSCR[2] = SIUL2_MSCR_IBE(1);
    Delay_Rm(0x1000UL);

    /* Check the value is not writen*/
    if(IP_SIUL2->MSCR[2] == SIUL2_MSCR_IBE(1))
    {
       Status = FALSE;
    }

    /* Check hardfault interrupt occurred and get error XRDC */
    if(gXrdcErrorDetected == E_OK)
    {
        if(CheckXrdcError() == FALSE)
        {
            Status = FALSE;
        }
    }
    else
    {
        Status = FALSE;
    }

    /********************* Example for Mpu M7 *********************/
    /* Perform access by write to SRAM_REGION*/
    SRAM_REGION = PATTERN1;
    Delay_Rm(0x1000UL);

    /*Check if memfault interrupt occurred. It should not occur*/
    if(gMpuMemfaultOccured == TRUE)
    {
       Status = FALSE;
    }
    /* Remove access rights of the MPU region */
    Rm_Mpu_M7_SetAccessRight(T_REGION, MPU_M7_PRIV_UNPRIV_NONE);

    /* Try to write at SRAM_REGION, this should trigger a memfault interrupt and an MPU error as the access is blocked */
    SRAM_REGION = PATTERN;
    Delay_Rm(0x1000UL);

    /*Check if memfault interrupt occurred*/
    if(gMpuMemfaultOccured == TRUE)
    {
        /* Check if get error successfully*/
        if(gMpuErrorDetected == E_OK)
        {
            /*Check error address and error type*/
            if((gMpuErrorDetails.u32Address != SRAM_REGION_ADDR) || (gMpuErrorDetails.eType != MPU_M7_MEMMAN_ERROR_DATA_ACCESS))
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

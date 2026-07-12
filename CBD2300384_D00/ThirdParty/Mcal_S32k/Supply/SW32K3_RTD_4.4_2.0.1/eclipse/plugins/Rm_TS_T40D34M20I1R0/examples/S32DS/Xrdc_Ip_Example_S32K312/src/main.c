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

#include "S32K312_SCB.h"
#include "Xrdc_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define XRDC_INSTANCE                   (XRDC_INSTANCE0)

#define TS_PIT_0_BASEADDR               ((uint32)0x400B0000UL)

#define TS_PIT_0                        (*(volatile uint32*)TS_PIT_0_BASEADDR)

#define PATTERN                         (0x5A5A5A5AUL)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

volatile boolean gXrdcErrorDetected = FALSE;

Xrdc_Ip_DomainIDErrStatusType gXrdcErrorDetails;

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
/* Set Hardfault Handler */
void HardFault_Handler(void)
{
    gXrdcErrorDetected = E_OK;
}

/* Dummy delay function */
void Delay_Rm(volatile uint32 u32Cycles)
{
    while(u32Cycles--);
}

/* Validate Xrdc Error Details */
boolean CheckXrdcError(void)
{
    boolean bReturnCode = FALSE;
    uint32 u32DomainId;
    uint32 u32ErrorPort;

    /* Get current domain ID from XRDC */
    u32DomainId = Xrdc_Ip_GetDomainID(XRDC_INSTANCE);

    if(gXrdcErrorDetails.DomainIDAccessError != u32DomainId)
    {
        return FALSE;
    }

    for(u32ErrorPort = 0; u32ErrorPort < sizeof(gXrdcErrorDetails.ErrorStatus)/sizeof(Xrdc_Ip_ErrorStatusType); u32ErrorPort++)
    {
        if(gXrdcErrorDetails.ErrorStatus[u32ErrorPort].u32AddError == TS_PIT_0_BASEADDR)
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
* @details      Initializes the XRDC Ip driver.
*               Override exception handler.
*               Perform violation access.
*               Verify error information.
*/
int main(void)
{
    boolean Status = TRUE;

    /* Initialize Xrdc Ip driver */
    Xrdc_Ip_Init(&Xrdc_Config_VS_0);

    /* Try to write at PIT 0, this should trigger an Xrdc error */
    TS_PIT_0 = PATTERN;

    /* Delay to waiting for that exception handlers occurred*/
    Delay_Rm(0x1000UL);

    if(gXrdcErrorDetected == E_OK)
    {
        Xrdc_Ip_GetDomainIDErrorStatus(XRDC_INSTANCE, &gXrdcErrorDetails);
        Status = CheckXrdcError();
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

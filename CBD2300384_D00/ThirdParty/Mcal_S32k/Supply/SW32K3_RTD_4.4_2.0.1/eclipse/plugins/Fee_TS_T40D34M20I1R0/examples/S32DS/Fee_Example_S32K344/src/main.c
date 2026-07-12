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
#include "Schm_Fls.h"
#include "Schm_Fee.h"
#include "Fls.h"
#include "Fee.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
const uint8 DataBlock0[4] = {0x00,0x01,0x02,0x03};

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint8 DataReceive[2] = {0x0,0x0};

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Initializez the used drivers and uses the Gpt
*               and Dio drivers to toggle a LED periodically
*/
int main(void) 
{
    MemIf_StatusType status = MEMIF_IDLE;

    /* Initialize Fls driver */
    Fls_Init(NULL_PTR);
    /* Initialize Fee driver */
    Fee_Init(NULL_PTR);
    /*Perform init Fee driver*/
    do
    {
        Fls_MainFunction();
        Fee_MainFunction();
        status = Fee_GetStatus();
    } while (status != MEMIF_IDLE);

    /*Write data to block 0*/
    Fee_Write(FeeConf_FeeBlockConfiguration_FeeBlockConfiguration_0, DataBlock0);

    /*Perform write data to Block 0*/
    do
    {
        Fls_MainFunction();
        Fee_MainFunction();
        status = Fee_GetStatus();
    } while (status != MEMIF_IDLE);

    /*Read data block 0, from offset 2 and length is 2*/
    Fee_Read(FeeConf_FeeBlockConfiguration_FeeBlockConfiguration_0, 2, DataReceive, 2);

    /*Perform read data form Block 0*/
    do
    {
        Fls_MainFunction();
        Fee_MainFunction();
        status = Fee_GetStatus();
    } while (status != MEMIF_IDLE);
    /*Check Data*/
    if((DataReceive[0] != DataBlock0[2])|| (DataReceive[1] != DataBlock0[3]))
    {
        Exit_Example(FALSE);
    }
    else
    {
        Exit_Example(TRUE);
    }

    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

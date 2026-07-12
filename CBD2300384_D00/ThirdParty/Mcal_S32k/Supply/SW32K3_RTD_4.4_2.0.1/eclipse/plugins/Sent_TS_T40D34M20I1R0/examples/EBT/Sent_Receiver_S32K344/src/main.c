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
#include "Mcl.h"
#include "Port.h"
#include "Mcu.h"
#include "CDD_Sent.h"
#include "SchM_Sent.h"
#include "check_example.h"
#include "sys_init.h"
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


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint8 Sent_RxIndication = 0U;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void SentFastNotification(Flexio_Sent_Ip_StatusType status, Flexio_Sent_FastMsgType *pSent_FastMsg)
{
    Sent_RxIndication++;
    (void)status;
    (void)pSent_FastMsg;
}

int main(void) 
{
    uint32 u32TimeOut = 0xFFFF;

    Mcu_Init(NULL_PTR);
    /* Initialize Mcu Clock */
    Mcu_InitClock(0);
    while (Mcu_GetPllStatus() != MCU_PLL_LOCKED){};
    /* Use PLL clock */
    Mcu_DistributePllClock();
    Mcu_SetMode(0U);

    /* Initialize Port driver */
    Port_Init(NULL_PTR);
    /* Initialize Mcl driver */
    Mcl_Init(NULL_PTR);
    /* Initialize Sent driver */
    Sent_Init(NULL_PTR);

    while((!Sent_RxIndication) && (--u32TimeOut))
    {
        /* Call Sent_GetFastChannelMsgData to get Fast message on channel 0 */
        Sent_GetFastChannelMsgData(0U);
    }

    Sent_DeInit();
    
    Exit_Example(TRUE);
    
    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

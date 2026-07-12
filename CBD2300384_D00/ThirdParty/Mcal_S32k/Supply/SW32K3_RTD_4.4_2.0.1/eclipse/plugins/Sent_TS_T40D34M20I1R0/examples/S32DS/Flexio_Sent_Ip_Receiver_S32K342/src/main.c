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
#include "Mcal.h"
#include "Siul2_Port_Ip.h"
#include "Clock_Ip.h"
#include "Flexio_Sent_Ip.h"
#include "Flexio_Mcl_Ip.h"
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define clockConfig &Clock_Ip_aClockConfig[0]

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
FLEXIO_SENT_IP_CONFIG_EXT
extern const Flexio_Ip_InstanceConfigType Flexio_Ip_xFlexioInit;

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

    Clock_Ip_Init(clockConfig);
    Clock_Ip_GetPllStatus();
    /* Distribute clock */
    Clock_Ip_DistributePll();

    /* Init Mcl */
    Flexio_Mcl_Ip_InitDevice(&Flexio_Ip_xFlexioInit);
    /* Initialize Port driver */
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
    /* Initialize Sent driver */
    Flexio_Sent_Ip_Init(0, Flexio_aCtrlConfigPB_BOARD_INITPERIPHERALS);

    while((!Sent_RxIndication) && (--u32TimeOut))
    {
        /* Call Flexio_Sent_Ip_GetFastChannelMsgData to get Fast message on FLEXIO instance 0, channel 0 */
        Flexio_Sent_Ip_GetFastChannelMsgData(0U,0U);
    }

    return (0U);
}

#ifdef __cplusplus
}
#endif

/** @} */

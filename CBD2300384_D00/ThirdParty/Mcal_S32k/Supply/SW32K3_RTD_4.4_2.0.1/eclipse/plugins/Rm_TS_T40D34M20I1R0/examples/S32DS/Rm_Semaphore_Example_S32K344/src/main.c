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
#include "sys_init.h"
#include "CDD_Rm.h"
#include "CDD_Mcl.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define DATA_LENGTH                        (16U)
#define DMA_CH0_CONFIG_LIST_DIMENSION      ((uint32)2U)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED_NO_CACHEABLE
#include "Rm_MemMap.h"

uint8 Src_data[DATA_LENGTH]  = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8 Dest_data[DATA_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED_NO_CACHEABLE
#include "Rm_MemMap.h"
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

Mcl_DmaChannelTransferListType DmaCh0_TransferList[DMA_CH0_CONFIG_LIST_DIMENSION] =
{
    {MCL_DMA_CH_SET_SOURCE_ADDRESS, (uint32)&Src_data[0U]},
    {MCL_DMA_CH_SET_DESTINATION_ADDRESS, (uint32)&Dest_data[0U]},
};

void PerformDmaTransfer(void)
{
    /* Clear error for Channel 0 */
    Mcl_SetDmaChannelCommand(DMA_LOGIC_CH_0, MCL_DMA_CH_ACK_ERROR);
    /* Set Logic Channel 0 -> Source and Destination Address */
    Mcl_SetDmaChannelTransferList(DMA_LOGIC_CH_0, DmaCh0_TransferList, DMA_CH0_CONFIG_LIST_DIMENSION);
    /* Start transfer for logic Channel 0 */
    Mcl_SetDmaChannelCommand(DMA_LOGIC_CH_0, MCL_DMA_CH_START_SERVICE);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Main function of the example
* @details      Rm_Init to init Domain0 (CM7_0) and Domain1 (DMA) and provide both of them access to SRAM with Semaphore validation enable.
*               Rm_Init also enable Sema42.
*               Try to lock gate 1 of Sema42 instance 0 with Domain0, DMA can not transfer.
*               Reset all Sema42 gates, data is transfered successfully
*/
int main(void) 
{
    boolean Status = TRUE;
    uint32 i;

    /* Initialize the clock */
    sys_init();

    /* Initialize MCL Driver */
    Mcl_Init(NULL_PTR);

    /* Disable Cache data */
    Mcl_CacheDisable(MCL_CACHE_DATA);

    /* Initialize RM CDD */
    Rm_Init(NULL_PTR);

    /* Lock Logical channel 1 of Sema42. Domain0 will owns Sema42 Logical channel 1*/
    Rm_SemaphoreLockGate(RM_SEMA42_LOGIC_CH_1);

    /* Use DMA to transfer data from array Src_data to Dest_data. DMA belongs to domain 1, cannot transfer */
    PerformDmaTransfer();

    /* Check data is not transfered successfully */
    for( i=0; i< DATA_LENGTH; i++)
    {
        if(Dest_data[i] != 0)
        {
            Status = FALSE;
        }    
    }

    /*Reset all gates then lock Logical channel 1 of Sema42 to achive write access to SRAM*/
    Rm_SemaphoreUnlockGate(RM_SEMA42_LOGIC_CH_1);

    /* Use DMA to transfer data from array Src_data to Dest_data */
    PerformDmaTransfer();
    Delay_Rm(0x1000UL);

    /* Check data is transfered successfully */
    for( i=0; i< DATA_LENGTH; i++)
    {
        if(Dest_data[i] != Src_data[i])
        {
            Status = FALSE;
        }    
    }

    Exit_Example(Status);

    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */

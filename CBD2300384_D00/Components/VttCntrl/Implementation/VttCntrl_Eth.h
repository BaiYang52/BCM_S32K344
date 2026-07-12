/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**      \file    VttCntrl_Fr.h
*        \brief   Declares the public API of the virtual Ethernet controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Ethernet controller that enables to send and receive Ethernet frames.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2015-11-03  visdap  -             Updated header comment.
 *  04.00.01  2016-01-20  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *  04.01.00  2016-06-23  visfer  -             Added frame length checks - compability with R16 DrvEth_Vtt
 *  04.02.00  2016-11-22  visfer  ESCAN00092941 Introduce CANoe system variable to control link state for component test
 *  09.00.00  2018-01-30  visjth  STORY-3149    Removed Channel Mapping
 *  11.00.00  2018-04-12  visfer  ESCAN00099102 Frame transmission is unreliable or gets stuck
 *  11.00.01  2018-05-02  visjth  OI-7932       Fixed compiler warnings
 *  11.00.02  2020-03-10  vismha  ESCAN00100695 Fatal Error Unhandled IRQ  due to VTT Ethernet Polling Mode
 *  12.03.00  2021-11-03  vistoc  ETHCIF-1      Introduce queue handling on basis of common QoS functionality
 *                                              (VLAN PCP) in DrvEth_VttEthAsr
 *  12.04.00  2021-11-26  vistoc  ETHCIF-662    [DrvEth_Vtt] HW Timestamping
 *  12.04.01  2022-10-24  visdrr  ETHPLAY-482   Remove handling of CANoe system variable "EthTestApp::TrcvLinkState"
 *                                              from VttCntrl_Eth.c
*********************************************************************************************************************/

#if !defined (VTTCNTRL_ETH_H)
# define VTTCNTRL_ETH_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

# include "VttCntrl_Eth_Cfg.h"
# include "Eth_GeneralTypes.h"
# include "Eth_30_Vtt_Types.h"
# include "Eth_30_Vtt_LL_Types.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
# define VTTETH_SW_MAJOR_VERSION                                      (12u)
# define VTTETH_SW_MINOR_VERSION                                      (04u)
# define VTTETH_SW_PATCH_VERSION                                      (01u)

/*! \brief Offset of IRQ numbers in os.h for Ethernet controller interrupts. */
# define ETH_IRQ_OFFSET                                               IRQ_EthIsrRx_0

/*! \brief Size of Ethernet frame buffer. */
# define VTTETH_FRAME_BUFFER_SIZE                                     1520

/*! \brief Possible return values for VttEth_GetState. */
# define VTTETH_STATE_UNKNOWN                                         (0u)
# define VTTETH_STATE_NOT_CONNECTED                                   (1u)
# define VTTETH_STATE_CONNECTED                                       (2u)
# define VTTETH_STATE_CHANNEL_UNKNOWN                                 (3u)

/*! \brief The same packet handler is triggered for Tx and Rx frames. Distinguished by direction variable. */
# define ETH_DIRECTION_RX                                             0
# define ETH_DIRECTION_TX                                             1

# define VTTETH_TX_INTERRUPT_NUM                                      (4u)
# define VTTETH_RX_INTERRUPT_NUM                                      (4u)

# define VTTETH_NANOSECONDS_PER_SECOND                                (1000000000uL)
/*! \brief The value of the nano second, when the field "second" in the time stamping reach the max value */
# define VTTETH_NANOSECONDS_WHEN_SECOND_MAX                           (4294967295999999999uLL)

# define VTTETH_HW_TIME_ERROR                                         (0xFFFFFFFFFFFFFFFFuLL)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

 /*! \brief Internal Ethernet controller structure. */
typedef struct
{
  uint8 Dir;
  uint32 PacketSize;
  uint8 PacketData[VTTETH_FRAME_BUFFER_SIZE];
  Eth_ModeType CtrlMode;
  sint32 IsrTx_No[VTTETH_TX_INTERRUPT_NUM];
  sint32 IsrRx_No[VTTETH_RX_INTERRUPT_NUM];
  uint32 Status;
} VttEth_CntrlType;

/*! \brief Internal controller structure associated with each Ethernet channel. */
extern VttEth_CntrlType VttEth_Cntrl[VTTETH_CHANNELS];

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttEth_SetControllerMode(uint8 channel, Eth_ModeType CtrlMode);
void VttEth_GetControllerMode(uint8 channel, Eth_ModeType* CtrlModePtr);
void VttEth_SetAllChannelsOnDownMode(void);
void VttEth_SetDescriptorBaseAddress(Eth_30_Vtt_RxDescriptorType* RxDescriptorBaseAddress, Eth_30_Vtt_TxDescriptorType* TxDescriptorBaseAddress);
void VttEth_SetRxDescriptorBaseAddress(Eth_30_Vtt_RxDescriptorType* RxDescriptorBaseAddress, uint8_least rxDescrRingIdx);
void VttEth_SetTxDescriptorBaseAddress(Eth_30_Vtt_TxDescriptorType* TxDescriptorBaseAddress, uint8_least txDescrRingIdx);

void VttEth_OutputEthernetPacket(uint8 channel, uint32 packetSize, const uint8 *packetData);
uint32 VttEth_GetStatus(uint8 channel);
VttEth_CntrlType* VttEth_GetData(uint8 channel);

void VttEth_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

boolean VttEth_GetRxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx);
void VttEth_SetRxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver);
boolean VttEth_GetTxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx);
void VttEth_SetTxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver);
boolean VttEth_GetRxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, uint8_least rxDescrRingIdx);
void VttEth_SetRxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver, uint8_least rxDescrRingIdx);
boolean VttEth_GetTxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, uint8_least txDescrRingIdx);
void VttEth_SetTxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver, uint8_least txDescrRingIdx);
uint32 VttEth_GetRxInterruptRaw(uint8 channel);
void VttEth_ResetRxInterruptRaw(uint8 channel, uint8 descrRingIdx);
uint32 VttEth_GetTxInterruptRaw(uint8 channel);
void VttEth_ResetTxInterruptRaw(uint8 channel, uint8 descrRingIdx);
void VttEth_SetUntaggedQueue(uint8 channel, uint8 rxUntaggedQueue, uint8 txUntaggedQueue);
void VttEth_SetVlanPrio2RxQueueMapping(uint8 channel, uint8 vlanPrio, uint8 rxQueue);
void VttEth_SetVlanPrio2TxQueueMapping(uint8 channel, uint8 vlanPrio, uint8 txQueue);
void VttEth_SetVlanID2RxQueueMapping(uint8 channel, uint8 vlanID, uint8 rxQueue);
void VttEth_SetVlanID2TxQueueMapping(uint8 channel, uint8 vlanID, uint8 txQueue);
void VttEth_SetQoSEnabled(void);
void VttEth_SetVLANIDBasedQueuingEnabled(void);

#if (ETH_30_VTT_ENABLE_TIMESYNC == STD_ON)
Std_ReturnType VttEth_GetHwTime(Eth_TimeStampType *hwTime);
void VttEth_SetHwTime(const Eth_TimeStampType *hwTime);
void VttEth_SetRxDescriptorTimestamp(uint8 channel, uint16 descriptorIdx, uint64 hwTimeInNanoseconds, uint8_least rxDescrRingIdx);
void VttEth_SetTxDescriptorTimestamp(uint8 channel, uint16 descriptorIdx, uint64 hwTimeInNanoseconds, uint8_least txDescrRingIdx);
Std_ReturnType VttEth_IsCANoeTimerOK(void);
#endif

#endif /* VTTCNTRL_ETH_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Eth.h
 *********************************************************************************************************************/

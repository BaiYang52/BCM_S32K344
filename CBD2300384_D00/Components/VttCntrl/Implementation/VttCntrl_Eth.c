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
/**      \file    VttCntrl_Eth.c
*        \brief   Implements the public API of the virtual Ethernet controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Ethernet controller that enables to send and receive Ethernet frames.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the module's header file.
*
*  FILE VERSION
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the VERSION CHECK below.
*********************************************************************************************************************/

#define VTTETH_SOURCE
#define VTTCNTRL_ETH_SOURCE

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Eth.h"
#include <string.h>  /* Declares function memcpy */
#include "Eth_30_Vtt_Lcfg.h"
#include "Eth_30_Vtt_CfgAccess_Int.h"
/**********************************************************************************************************************
*  PRAGMAS
*********************************************************************************************************************/

/*! \brief Ensure that the internal state of the Ethernet controller is not cleared during a simulated ECU reset. */
#pragma bss_seg()
#pragma data_seg()

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* Check the version of VttCntrl_Eth header file */
#if (  (VTTETH_SW_MAJOR_VERSION != (12u)) \
    || (VTTETH_SW_MINOR_VERSION != (04u)) \
    || (VTTETH_SW_PATCH_VERSION != (01u)) )
# error "Vendor specific version numbers of VttCntrl_Eth.c and VttCntrl_Eth.h are inconsistent"
#endif

/* Check the version of the configuration header file. */
#if (  (VTTETH_CFG_SW_MAJOR_VERSION != (0x02u)) \
    || (VTTETH_CFG_SW_MINOR_VERSION != (0x00u)))
# error "Version numbers of VttCntrl_Eth.c and VttCntrl_Eth_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
*  GLOBAL VARIABLES
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/
#define VTTCNTRL_FRAME_TYPE_UPPER_BYTE_OFFSET                         (12u)
#define VTTCNTRL_FRAME_TYPE_LOWER_BYTE_OFFSET                         (13u)
#define VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET                           (14u)
#define VTTCNTRL_VLAN_PCP_BYTE_MASK                                   (0xE0u)
#define VTTCNTRL_VLAN_ID_BYTE_MASK                                    (0x0FFFu)
#define VTTCNTRL_VLAN_PCP_BYTE_SHIFT                                  (5u)
#define VTTCNTRL_VLAN_ETHER_TYPE                                      (0x8100u)

#define RX_QUEUE_NUM                                                  (VTTETH_CHANNELS * 4)
#define TX_QUEUE_NUM                                                  (VTTETH_CHANNELS * 4)

#define VLAN_PRIO_NUM                                                 (8u)
#define VLAN_ID_NUM                                                   (4096u)

#define IRQ_OFFSET_DIFF_1                                             (1u)
#define IRQ_OFFSET_DIFF_2                                             (2u)
#define IRQ_OFFSET_DIFF_3                                             (3u)
#define IRQ_OFFSET_DIFF_4                                             (4u)
#define IRQ_OFFSET_DIFF_5                                             (5u)
#define IRQ_OFFSET_DIFF_6                                             (6u)
#define IRQ_OFFSET_DIFF_7                                             (7u)

#define VTTETH_CNTRL_IRQ_INDEX_0                                      (0u)
#define VTTETH_CNTRL_IRQ_INDEX_1                                      (1u)
#define VTTETH_CNTRL_IRQ_INDEX_2                                      (2u)
#define VTTETH_CNTRL_IRQ_INDEX_3                                      (3u)

#define VTTETH_CNTRL_IRQ_NUM                                          (8u)

#define VTT_DESCRIPTOR_INDEX_0                                        (0u)
#define VTT_DESCRIPTOR_INDEX_1                                        (1u)
#define VTT_DESCRIPTOR_INDEX_2                                        (2u)
#define VTT_DESCRIPTOR_INDEX_3                                        (3u)

/**********************************************************************************************************************
*  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#if !defined (STATIC)
# define STATIC                                                       static
#endif

/**********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/*! \brief Internal controller structure associated with each Ethernet channel. */
VttEth_CntrlType VttEth_Cntrl[VTTETH_CHANNELS];

/*! \brief Internal ECU state. */
STATIC uint8 vttEthEcuState = CANOEAPI_ECUSTATE_INITIAL;

STATIC VttEth_CntrlType vttData;
/* VttEth_*Descriptor is set by VttEth driver. Values must be lost during a reset. */
STATIC uint8 vttEthCurrentRxDescriptor[VTTETH_CHANNELS][RX_QUEUE_NUM];
STATIC uint8 vttEthCurrentTxDescriptor[VTTETH_CHANNELS][TX_QUEUE_NUM];
STATIC Eth_30_Vtt_RxDescriptorType* vttEthRxDescriptorBaseAddress[RX_QUEUE_NUM];
STATIC Eth_30_Vtt_TxDescriptorType* vttEthTxDescriptorBaseAddress[TX_QUEUE_NUM];
STATIC volatile uint32 pendingRxInterruptRaw;
STATIC volatile uint32 pendingTxInterruptRaw;

STATIC uint8 untaggedRxQueue[VTTETH_CHANNELS];
STATIC uint8 untaggedTxQueue[VTTETH_CHANNELS];
STATIC uint8 vlanPrio2RxQueue[VTTETH_CHANNELS][VLAN_PRIO_NUM];
STATIC uint8 vlanPrio2TxQueue[VTTETH_CHANNELS][VLAN_PRIO_NUM];
STATIC uint8 vlanID2RxQueue[VTTETH_CHANNELS][VLAN_ID_NUM];
STATIC uint8 vlanID2TxQueue[VTTETH_CHANNELS][VLAN_ID_NUM];

STATIC boolean isQosEnabled;
STATIC boolean isVlanIDBasedQueuingEnabled;

STATIC uint64 hwTimeInNanoseconds;

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

STATIC void VttEth_OnEthernetStatusHandler(uint8 channel, uint32 status);
STATIC void VttEth_OnEthernetPacketHandler2(sint64 eofTimestamp, uint8 channel, uint8 dir, uint32 packetSize, const uint8 *packetData);

STATIC void VttEth_AssignIRQ(void);
STATIC void VttEth_SetHandler(void);

STATIC void VttEth_OnReset(void);
STATIC void VttEth_OnStartMeasurement(void);
STATIC void VttEth_OnStopMeasurement(void);
STATIC void VttEth_OnLoad(void);
STATIC void VttEth_OnGoToSleep(void);
STATIC void VttEth_OnWakeup(void);
STATIC void VttEth_OnSwitchOff(void);

/**********************************************************************************************************************
*  LOCAL FUNCTIONS
**********************************************************************************************************************/

/***********************************************************************************************************************
*  VttEth_IncCurrentRxDescriptor
**********************************************************************************************************************/
/*! \brief     Increase the position of the current Rx Descriptor.
*  \return     void
*  \context    Function is called by VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_IncCurrentRxDescriptor(uint8 channel, uint8_least rxDescrRingIdx)
{
  if(vttEthRxDescriptorBaseAddress[rxDescrRingIdx][vttEthCurrentRxDescriptor[channel][rxDescrRingIdx]].LastElement == TRUE)
  {
    vttEthCurrentRxDescriptor[channel][rxDescrRingIdx] = 0;
  }
  else
  {
    vttEthCurrentRxDescriptor[channel][rxDescrRingIdx]++;
  }
} /* VttEth_IncCurrentRxDescriptor() */

/***********************************************************************************************************************
*  VttEth_IncCurrentTxDescriptor
**********************************************************************************************************************/
/*! \brief     Increase the position of the current Tx Descriptor.
*  \return     void
*  \context    Function is called by VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_IncCurrentTxDescriptor(uint8 channel, uint8_least txDescrRingIdx)
{
  if(vttEthTxDescriptorBaseAddress[txDescrRingIdx][vttEthCurrentTxDescriptor[channel][txDescrRingIdx]].LastElement == TRUE)
  {
    vttEthCurrentTxDescriptor[channel][txDescrRingIdx] = 0;
  }
  else
  {
    vttEthCurrentTxDescriptor[channel][txDescrRingIdx]++;
  }
} /* VttEth_IncCurrentTxDescriptor() */

/***********************************************************************************************************************
*  VttEth_OnEthernetPacketHandler
**********************************************************************************************************************/
/*! \brief     Callback function of CAnoe.
*  \param[in]  channel                 Channel index
*  \param[in]  dir                     Direction of packet (either Tx or Rx)
*  \param[in]  packetSize              Size of packet.
*  \param[in]  packetData              Pointer to data.
*  \return     no return
*  \pre        Input parameter are verified by CANoe.
*  \context    Function is called by CANoe.
**********************************************************************************************************************/
STATIC void VttEth_OnEthernetPacketHandler2(sint64 eofTimestamp, uint8 channel, uint8 dir, uint32 packetSize, const uint8 *packetData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 *payload;
  static uint8 CANoeWriteCounter = 0u;
  uint8 rxQueue = 0u;
  uint8 txQueue = 0u;
  uint16 frameType;
  uint8 vlanPrio;
  uint16 vlanId;


  /* ----- Implementation ----------------------------------------------- */
  if (eofTimestamp > -1) {
    hwTimeInNanoseconds = (uint64)eofTimestamp;
  }
  else
  {
    hwTimeInNanoseconds = VTTETH_HW_TIME_ERROR;
  }

  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  if(VttEth_Cntrl[channel].CtrlMode == ETH_MODE_ACTIVE)
  {
    if(dir == ETH_DIRECTION_RX)
    {
      /* Get the frame type */
      frameType = (uint16)packetData[VTTCNTRL_FRAME_TYPE_UPPER_BYTE_OFFSET] << 8u;
      frameType |= (uint16)packetData[VTTCNTRL_FRAME_TYPE_LOWER_BYTE_OFFSET];

      if (frameType == VTTCNTRL_VLAN_ETHER_TYPE)
      {
        if (isQosEnabled == TRUE)
        {
          /* Get the VLAN Prio of the received Ethernet frame */
          vlanPrio = (uint8)((packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET] & VTTCNTRL_VLAN_PCP_BYTE_MASK) >> VTTCNTRL_VLAN_PCP_BYTE_SHIFT);

          /* Choose the queue according to the VLAN Prio */
          rxQueue = vlanPrio2RxQueue[channel][vlanPrio];
        }

        if (isVlanIDBasedQueuingEnabled == TRUE)
        {
          /* Get the VLAN ID of the received Ethernet frame */
          vlanId = (uint16)(((uint16)packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET]) << 8u);
          vlanId |= (uint16)packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET + 1U];
          vlanId &= (uint16)VTTCNTRL_VLAN_ID_BYTE_MASK;

          /* Choose the queue according to the VLAN ID */
          rxQueue = vlanID2RxQueue[channel][vlanId];
        }
      }
      else
      {
        rxQueue = untaggedRxQueue[channel];
      }

      if(VttEth_GetRxDescriptorOwnedByDriverMultiQueues(channel, vttEthCurrentRxDescriptor[channel][rxQueue], rxQueue) == FALSE)
      {
        if(packetSize > vttEthRxDescriptorBaseAddress[rxQueue][vttEthCurrentRxDescriptor[channel][rxQueue]].AllowedPacketSize)
        {
          /* Frame is too long - dropped */
        }
        else
        {
          vttEthRxDescriptorBaseAddress[rxQueue][vttEthCurrentRxDescriptor[channel][rxQueue]].ReceivedPacketSize = (uint16)packetSize;
          payload = (uint8*)(Eth_30_Vtt_Cfg_GetRxDescr(channel, 0, vttEthCurrentRxDescriptor[channel][rxQueue] + Eth_30_Vtt_GetRxDescrStartIdxOfRxDescrHandling(rxQueue))->DataPtr);

          /* Copy Ethernet frame data into Ethernet RxBuffer as DMA would do */
          memcpy(&payload[2], packetData, packetSize);
          VttEth_SetRxDescriptorOwnedByDriverMultiQueues(channel, vttEthCurrentRxDescriptor[channel][rxQueue], TRUE, rxQueue);
#if (ETH_30_VTT_ENABLE_TIMESYNC == STD_ON)
          VttEth_SetRxDescriptorTimestamp(channel, vttEthCurrentRxDescriptor[channel][rxQueue], hwTimeInNanoseconds, rxQueue);
#endif
          pendingRxInterruptRaw |= (uint32)1u << rxQueue;

          VttEth_IncCurrentRxDescriptor(channel, rxQueue);

          if( Eth_30_Vtt_Cfg_IsRxInterruptModeEnabled(channel) == TRUE )
          {
            /* Inform Ethernet driver about Rx event. */
            VttCntrl_Base_SetCheckedInterrupt(VttEth_Cntrl[channel].IsrRx_No[rxQueue]);
          }
        }
      }
      else
      {
        /* Descriptor ring is full - frame is dropped. */
        if (CANoeWriteCounter < 100u) /* prevent Write-Window overrun */
        {
          CANoeWriteCounter++;
          CANoeAPI_Printf("VttEth was not able to process all received frames. At least one ethernet frame is lost.\nPlease increase the number of Ethernet buffers in DaVinci Configurator Pro.");
          if (CANoeWriteCounter == 100u)
          {
            CANoeAPI_Printf("The previous error message was reported 100 times, further messages are omitted.");
          }
        }
      }
    }

    if(dir == ETH_DIRECTION_TX)
    {
      /* Get the frame type */
      frameType = (uint16)packetData[VTTCNTRL_FRAME_TYPE_UPPER_BYTE_OFFSET] << 8u;
      frameType |= (uint16)packetData[VTTCNTRL_FRAME_TYPE_LOWER_BYTE_OFFSET];

      if (frameType == VTTCNTRL_VLAN_ETHER_TYPE)
      {
        if (isQosEnabled == TRUE)
        {
          /* Get the VLAN Prio of the received Ethernet frame */
          vlanPrio = (uint8)((packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET] & VTTCNTRL_VLAN_PCP_BYTE_MASK) >> VTTCNTRL_VLAN_PCP_BYTE_SHIFT);

          /* Choose the queue according to the VLAN Prio */
          txQueue = vlanPrio2TxQueue[channel][vlanPrio];
        }

        if (isVlanIDBasedQueuingEnabled == TRUE)
        {
          /* Get the VLAN ID of the received Ethernet frame */
          vlanId = (uint16)(((uint16)packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET]) << 8u);
          vlanId |= (uint16)packetData[VTTCNTRL_VLAN_TCI_UPPER_BYTE_OFFSET + 1U];
          vlanId &= (uint16)VTTCNTRL_VLAN_ID_BYTE_MASK;

          /* Choose the queue according to the VLAN ID */
          txQueue = vlanID2TxQueue[channel][vlanId];
        }
      }
      else
      {
       txQueue = untaggedTxQueue[channel];
      }

      pendingTxInterruptRaw |= (uint32)1u << txQueue;
      VttEth_SetTxDescriptorOwnedByDriverMultiQueues(channel, vttEthCurrentTxDescriptor[channel][txQueue], TRUE, txQueue);
#if (ETH_30_VTT_ENABLE_TIMESYNC == STD_ON)
      VttEth_SetTxDescriptorTimestamp(channel, vttEthCurrentTxDescriptor[channel][txQueue], hwTimeInNanoseconds, txQueue);
#endif
      VttEth_IncCurrentTxDescriptor(channel, txQueue);

      if( Eth_30_Vtt_Cfg_IsTxInterruptModeEnabled(channel) == TRUE )
      {
        /* Inform Ethernet driver about Tx event. */
        VttCntrl_Base_SetCheckedInterrupt(VttEth_Cntrl[channel].IsrTx_No[txQueue]);
      }
    }
  }
} /* VttEth_OnEthernetPacketHandler() */

/***********************************************************************************************************************
*  VttEth_OnEthernetStatusHandler
**********************************************************************************************************************/
/*! \brief     Tracks status changes of Ethernet channels.
*  \context    Called by CANoeEmu.
**********************************************************************************************************************/
STATIC void VttEth_OnEthernetStatusHandler(uint8 channel, uint32 status)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }
  VttEth_Cntrl[channel].Status = status;

} /* VttEth_OnEthernetStatusHandler() */

/***********************************************************************************************************************
*  VttEth_SetHandler
**********************************************************************************************************************/
/*! \brief     Register callback functions in CANoe.
*  \return     no return
*  \pre        -
*  \context    Function shall be called by VTT Ethernet controller during loading.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_SetHandler(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_RegisterEthernetPacket2(&VttEth_OnEthernetPacketHandler2);
  CANoeAPI_RegisterEthernetStatus(&VttEth_OnEthernetStatusHandler);

} /* VttEth_SetHandler() */

/***********************************************************************************************************************
*  VttEth_AssignIRQ
**********************************************************************************************************************/
/*! \brief     Store the IRQ numbers given by the OS in the Controller Structure.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_AssignIRQ(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();

  for(channel = 0u; channel < VTTETH_CHANNELS; channel++)
  {
    uint8 channelOffset = channel * VTTETH_CNTRL_IRQ_NUM;
    VttEth_Cntrl[channel].IsrRx_No[VTTETH_CNTRL_IRQ_INDEX_0] = ETH_IRQ_OFFSET + channelOffset;
    VttEth_Cntrl[channel].IsrTx_No[VTTETH_CNTRL_IRQ_INDEX_0] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_1;
    VttEth_Cntrl[channel].IsrRx_No[VTTETH_CNTRL_IRQ_INDEX_1] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_2;
    VttEth_Cntrl[channel].IsrTx_No[VTTETH_CNTRL_IRQ_INDEX_1] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_3;
    VttEth_Cntrl[channel].IsrRx_No[VTTETH_CNTRL_IRQ_INDEX_2] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_4;
    VttEth_Cntrl[channel].IsrTx_No[VTTETH_CNTRL_IRQ_INDEX_2] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_5;
    VttEth_Cntrl[channel].IsrRx_No[VTTETH_CNTRL_IRQ_INDEX_3] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_6;
    VttEth_Cntrl[channel].IsrTx_No[VTTETH_CNTRL_IRQ_INDEX_3] = ETH_IRQ_OFFSET + channelOffset + IRQ_OFFSET_DIFF_7;
  }

  CANoeAPI_AtomicEnd();

} /* VttEth_AssignIRQ() */

/***********************************************************************************************************************
*  VttEth_OnLoad
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \param[in]  -
*  \return     no return
*  \pre        -
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnLoad(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  VttEth_AssignIRQ();

  /* Set the Modes of all Controller on DOWN. */
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    VttEth_SetControllerMode(channel, ETH_MODE_DOWN);
  }

} /* VttEth_OnLoad() */

/***********************************************************************************************************************
*  VttEth_OnGoToSleep
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnGoToSleep(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnGoToSleep() */

/***********************************************************************************************************************
*  VttEth_OnWakeup
**********************************************************************************************************************/
/*! \brief     Callout of Vtt Eth Controller State Machine.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnWakeup(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

} /* VttEth_OnWakeup() */

/***********************************************************************************************************************
*  VttEth_OnStartMeasurement
**********************************************************************************************************************/
/*! \brief     Initialization during the start of a measurement in CANoe.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       A message is only sent, when the ECU is in state Processing (Emulation of RUN Mode).
**********************************************************************************************************************/
STATIC void VttEth_OnStartMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 channel;

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetHandler();

  /* Set all channels to have link up */
  for (channel = 0; channel < VTTETH_CHANNELS; ++channel)
  {
    VttEth_Cntrl[channel].Status = 0;
  }

} /* VttEth_OnStartMeasurement() */

/***********************************************************************************************************************
*  VttEth_OnReset
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note
**********************************************************************************************************************/
STATIC void VttEth_OnReset(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnReset() */

/***********************************************************************************************************************
*  VttEth_OnSwitchOff
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnSwitchOff(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnSwitchOff() */

/***********************************************************************************************************************
*  VttEth_OnStopMeasurement
**********************************************************************************************************************/
/*! \brief     Is called by State Machine of VTT.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
STATIC void VttEth_OnStopMeasurement(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  VttEth_SetAllChannelsOnDownMode();

} /* VttEth_OnStopMeasurement() */

/**********************************************************************************************************************
*  GLOBAL FUNCTIONS
**********************************************************************************************************************/
/***********************************************************************************************************************
*  VttEth_OnStateChange()
**********************************************************************************************************************/
/*! \brief     Called by CANoeEmu to inform virtual Ethernet controller about state change of ECU.
*
*  \param[in]  action           State change action, refer to the CANoeAPI header for more information.
*  \param[in]  oldState         Previous state of ECU, refer to the CANoeAPI header for more information.
*  \param[in]  newState         Next state of ECU, refer to the CANoeAPI header for more information.
*
*  \context    Called by CANoeEmu.
*  \note       For possible values of action, oldState and newState, refer to the CANoeAPI header.
**********************************************************************************************************************/
void VttEth_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  /* ----- Local Variables ---------------------------------------------- */
  vttEthEcuState = newState;
  oldState = oldState;

  /* ----- Implementation ----------------------------------------------- */
  switch(action)
  {
    case CANOEAPI_ECUACTION_LOAD:
      {
        VttEth_OnLoad();
      }
      break;
    case CANOEAPI_ECUACTION_STARTMEASUREMENT:
      {
        VttEth_OnStartMeasurement();
      }
      break;
    case CANOEAPI_ECUACTION_RESET:
      {
        VttEth_OnReset();
      }
      break;
    case CANOEAPI_ECUACTION_GOTOSLEEP:
      {
        VttEth_OnGoToSleep();
      }
      break;
    case CANOEAPI_ECUACTION_WAKEUP:
      {
        VttEth_OnWakeup();
      }
      break;
    case CANOEAPI_ECUACTION_SWITCHOFF:
      {
        VttEth_OnSwitchOff();
      }
      break;
    case CANOEAPI_ECUACTION_STOPMEASUREMENT:
      {
        VttEth_OnStopMeasurement();
      }
      break;
    case CANOEAPI_ECUACTION_UNLOAD:
    case CANOEAPI_ECUACTION_SWITCHON:
    break;
  }

} /* VttEth_OnStateChange() */

/***********************************************************************************************************************
*  VttEth_SetDescriptorBaseAddress
**********************************************************************************************************************/
/*! \brief     Set Descriptor Base Address
*  \param[in]  Descriptor Base Address
*  \return     no return
*  \pre        -
*  \context    Function is called by DrvEth_Vtt
*  \note       -
**********************************************************************************************************************/
void VttEth_SetDescriptorBaseAddress(Eth_30_Vtt_RxDescriptorType* RxDescriptorBaseAddress, Eth_30_Vtt_TxDescriptorType* TxDescriptorBaseAddress)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  vttEthRxDescriptorBaseAddress[0] = RxDescriptorBaseAddress;
  vttEthTxDescriptorBaseAddress[0] = TxDescriptorBaseAddress;

} /* VttEth_SetDescriptorBaseAddress() */

/***********************************************************************************************************************
*  VttEth_SetTxDescriptorBaseAddress
**********************************************************************************************************************/
/*! \brief     Set Tx Descriptor Base Address
*  \param[in]  Tx Descriptor Base Address
*  \return     no return
*  \pre        -
*  \context    Function is called by DrvEth_Vtt
*  \note       -
**********************************************************************************************************************/
void VttEth_SetTxDescriptorBaseAddress(Eth_30_Vtt_TxDescriptorType* TxDescriptorBaseAddress, uint8_least txDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  vttEthTxDescriptorBaseAddress[txDescrRingIdx] = TxDescriptorBaseAddress;

} /* VttEth_SetTxDescriptorBaseAddress() */

/***********************************************************************************************************************
*  VttEth_SetRxDescriptorBaseAddress
**********************************************************************************************************************/
/*! \brief     Set Rx Descriptor Base Address
*  \param[in]  Descriptor Base Address
*  \return     no return
*  \pre        -
*  \context    Function is called by DrvEth_Vtt
*  \note       -
**********************************************************************************************************************/
void VttEth_SetRxDescriptorBaseAddress(Eth_30_Vtt_RxDescriptorType* RxDescriptorBaseAddress, uint8_least rxDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  vttEthRxDescriptorBaseAddress[rxDescrRingIdx] = RxDescriptorBaseAddress;

} /* VttEth_SetRxDescriptorBaseAddress() */

/***********************************************************************************************************************
*  VttEth_SetControllerMode
**********************************************************************************************************************/
/*! \brief     Set the Mode of a particular Eth Controller.
*  \param[in]  channel                 Channel index
*              CtrlMode                Mode for the given Controller.
*  \return     no return
*  \context    Function is called by Eth Driver.
*  \note       Two modes are defined for an Eth Controller.
*              - ETH_MODE_DOWN
*              - ETH_MODE_ACTIVE
**********************************************************************************************************************/
void VttEth_SetControllerMode(uint8 channel, Eth_ModeType CtrlMode)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  CANoeAPI_AtomicBegin();
  VttEth_Cntrl[channel].CtrlMode = CtrlMode;
  if(VttEth_Cntrl[channel].CtrlMode == ETH_MODE_DOWN)
  {
    vttEthCurrentRxDescriptor[channel][VTT_DESCRIPTOR_INDEX_0] = 0;
    vttEthCurrentRxDescriptor[channel][VTT_DESCRIPTOR_INDEX_1] = 0;
    vttEthCurrentRxDescriptor[channel][VTT_DESCRIPTOR_INDEX_2] = 0;
    vttEthCurrentRxDescriptor[channel][VTT_DESCRIPTOR_INDEX_3] = 0;
    vttEthCurrentTxDescriptor[channel][VTT_DESCRIPTOR_INDEX_0] = 0;
    vttEthCurrentTxDescriptor[channel][VTT_DESCRIPTOR_INDEX_1] = 0;
    vttEthCurrentTxDescriptor[channel][VTT_DESCRIPTOR_INDEX_2] = 0;
    vttEthCurrentTxDescriptor[channel][VTT_DESCRIPTOR_INDEX_3] = 0;
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_SetControllerMode() */

/***********************************************************************************************************************
*  VttEth_GetControllerMode
**********************************************************************************************************************/
/*! \brief     Get the Mode of a particular Eth Controller.
*  \param[in]  channel                 Channel index
*  \param[out] CtrlModePtr             Mode for the given Controller.
*  \return     no return
*  \context    Function is called by Eth Driver.
*  \note       Two modes are defined for an Eth Controller.
*              - ETH_MODE_DOWN
*              - ETH_MODE_ACTIVE
**********************************************************************************************************************/
void VttEth_GetControllerMode(uint8 channel, Eth_ModeType* CtrlModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  CANoeAPI_AtomicBegin();
  *CtrlModePtr = VttEth_Cntrl[channel].CtrlMode;
  CANoeAPI_AtomicEnd();

} /* VttEth_GetControllerMode() */

/***********************************************************************************************************************
*  VttEth_OutputEthernetPacket
**********************************************************************************************************************/
/*! \brief     Sends a message by using CANoeAPI_OutputEthernetPacket.
*  \param[in]  channel                 Channel index
*  \param[in]  packetSize              Size of packet.
*  \param[out] packetData              Pointer to data.
*  \return     no return
*  \pre        Input parameter are verified by Eth Driver.
*  \context    Function is called by Eth Driver.
**********************************************************************************************************************/
void VttEth_OutputEthernetPacket(uint8 channel, uint32 packetSize, const uint8 *packetData)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return;
  }

  if((vttEthEcuState == CANOEAPI_ECUSTATE_PROCESSING) && (VttEth_Cntrl[channel].CtrlMode == ETH_MODE_ACTIVE))
  {
    CANoeAPI_OutputEthernetPacket(channel, packetSize, packetData);
  }

} /* VttEth_OutputEthernetPacket() */

/***********************************************************************************************************************
*  VttEth_SetAllChannelsOnDownMode
**********************************************************************************************************************/
/*! \brief     Sets all Ethernet channels on Down Mode.
*  \return     no return
*  \context    Function is called by State Machine of VTT.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetAllChannelsOnDownMode(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  int channel;

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  for(channel = 0; channel < VTTETH_CHANNELS; channel++)
  {
    VttEth_Cntrl[channel].CtrlMode = ETH_MODE_DOWN;
  }
  CANoeAPI_AtomicEnd();

} /* VttEth_SetAllChannelsOnDownMode() */

/***********************************************************************************************************************
*  VttEth_GetStatus
**********************************************************************************************************************/
/*! \brief     Returns the status of the given Ethernet channel.
*  \return     current channel status.
*              Possible states are:  status unknown  = VTTETH_STATE_UNKNOWN
*                                    not connected   = VTTETH_STATE_NOT_CONNECTED
*                                    connected       = VTTETH_STATE_CONNECTED
*                                    channel unknown = VTTETH_STATE_CHANNEL_UNKNOWN
*  \context    Function is called by EthTrcv_Vtt.
*  \note       -
**********************************************************************************************************************/
uint32 VttEth_GetStatus(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel >= VTTETH_CHANNELS)
  {
    return VTTETH_STATE_CHANNEL_UNKNOWN;
  }
  return VttEth_Cntrl[channel].Status;

} /* VttEth_GetStatus() */

/***********************************************************************************************************************
*  VttEth_GetData
**********************************************************************************************************************/
/*! \brief     Returns the data of the corresponding Ethernet channel.
*  \return     data structure
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
VttEth_CntrlType *VttEth_GetData(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if(channel < VTTETH_CHANNELS)
  {
    CANoeAPI_AtomicBegin();
    vttData = VttEth_Cntrl[channel];
    CANoeAPI_AtomicEnd();
  }
  return &vttData;

} /* VttEth_GetData() */

/***********************************************************************************************************************
*  VttEth_GetRxDescriptorOwnedByDriver
**********************************************************************************************************************/
/*! \brief     Gets the specified descriptor's 'owned by driver' field.
*              The access is synchronized between CANoe and VTT threads.
*  \return     see above
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
boolean VttEth_GetRxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ownedByDriver;
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  ownedByDriver = vttEthRxDescriptorBaseAddress[0][descriptorIdx].OwnedByDriver;
  CANoeAPI_AtomicEnd();
  return ownedByDriver;

} /* VttEth_GetRxDescriptorOwnedByDriver() */

/***********************************************************************************************************************
*  VttEth_SetRxDescriptorOwnedByDriver
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor as 'owned by driver'.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetRxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthRxDescriptorBaseAddress[0][descriptorIdx].OwnedByDriver = ownedByDriver;
  CANoeAPI_AtomicEnd();

} /* VttEth_SetRxDescriptorOwnedByDriver() */

/***********************************************************************************************************************
*  VttEth_GetTxDescriptorOwnedByDriver
**********************************************************************************************************************/
/*! \brief     Gets the specified descriptor's 'owned by driver' field.
*              The access is synchronized between CANoe and VTT threads.
*  \return     see above
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
boolean VttEth_GetTxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ownedByDriver;
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  ownedByDriver = vttEthTxDescriptorBaseAddress[0][descriptorIdx].OwnedByDriver;
  CANoeAPI_AtomicEnd();
  return ownedByDriver;

} /* VttEth_GetTxDescriptorOwnedByDriver() */

/***********************************************************************************************************************
*  VttEth_GetTxDescriptorOwnedByDriver
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor as 'owned by driver'.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetTxDescriptorOwnedByDriver(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthTxDescriptorBaseAddress[0][descriptorIdx].OwnedByDriver = ownedByDriver;
  CANoeAPI_AtomicEnd();

} /* VttEth_GetTxDescriptorOwnedByDriver() */

/***********************************************************************************************************************
*  VttEth_GetRxDescriptorOwnedByDriverMultiQueues
**********************************************************************************************************************/
/*! \brief     Gets the specified descriptor's 'owned by driver' field.
*              The access is synchronized between CANoe and VTT threads.
*  \return     see above
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
boolean VttEth_GetRxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, uint8_least rxDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ownedByDriver;
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  ownedByDriver = vttEthRxDescriptorBaseAddress[rxDescrRingIdx][descriptorIdx].OwnedByDriver;
  CANoeAPI_AtomicEnd();
  return ownedByDriver;

} /* VttEth_GetRxDescriptorOwnedByDriverMultiQueues() */

/***********************************************************************************************************************
*  VttEth_SetRxDescriptorOwnedByDriverMultiQueues
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor as 'owned by driver'.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetRxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver, uint8_least rxDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthRxDescriptorBaseAddress[rxDescrRingIdx][descriptorIdx].OwnedByDriver = ownedByDriver;
  CANoeAPI_AtomicEnd();

} /* VttEth_SetRxDescriptorOwnedByDriverMultiQueues() */

/***********************************************************************************************************************
*  VttEth_GetTxDescriptorOwnedByDriverMultiQueues
**********************************************************************************************************************/
/*! \brief     Gets the specified descriptor's 'owned by driver' field.
*              The access is synchronized between CANoe and VTT threads.
*  \return     see above
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
boolean VttEth_GetTxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, uint8_least txDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ownedByDriver;
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  ownedByDriver = vttEthTxDescriptorBaseAddress[txDescrRingIdx][descriptorIdx].OwnedByDriver;
  CANoeAPI_AtomicEnd();
  return ownedByDriver;

} /* VttEth_GetTxDescriptorOwnedByDriverMultiQueues() */

/***********************************************************************************************************************
*  VttEth_SetTxDescriptorOwnedByDriverMultiQueues
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor as 'owned by driver'.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetTxDescriptorOwnedByDriverMultiQueues(uint8 channel, uint16 descriptorIdx, boolean ownedByDriver, uint8_least txDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthTxDescriptorBaseAddress[txDescrRingIdx][descriptorIdx].OwnedByDriver = ownedByDriver;
  CANoeAPI_AtomicEnd();

} /* VttEth_SetTxDescriptorOwnedByDriverMultiQueues() */

  /***********************************************************************************************************************
  *  VttEth_GetRxInterruptRaw
  **********************************************************************************************************************/
  /*! \brief     Gets the rx raw interrupt.
  *  \return     void
  *  \context    Function is called by VTT Ether Driver or VttCntrl.
  *  \note       -
  **********************************************************************************************************************/
uint32 VttEth_GetRxInterruptRaw(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  return pendingRxInterruptRaw;

} /* VttEth_GetRxInterruptRaw() */

/***********************************************************************************************************************
*  VttEth_ResetRxInterruptRaw
**********************************************************************************************************************/
/*! \brief     Reset the rx raw interrupt.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_ResetRxInterruptRaw(uint8 channel, uint8 descrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  pendingRxInterruptRaw &= ~((uint32)1u << descrRingIdx);

} /* VttEth_ResetRxInterruptRaw() */

/***********************************************************************************************************************
*  VttEth_GetTxInterruptRaw
**********************************************************************************************************************/
/*! \brief     Gets the tx raw interrupt.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
uint32 VttEth_GetTxInterruptRaw(uint8 channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  return pendingTxInterruptRaw;

} /* VttEth_GetTxInterruptRaw() */

/***********************************************************************************************************************
*  VttEth_ResetTxInterruptRaw
**********************************************************************************************************************/
/*! \brief     Reset the tx raw interrupt.
*  \return     void
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_ResetTxInterruptRaw(uint8 channel, uint8 descrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  pendingTxInterruptRaw &= ~((uint32)1u << descrRingIdx);

} /* VttEth_ResetRxInterruptRaw() */

/***********************************************************************************************************************
*  VttEth_SetUntaggedQueue
**********************************************************************************************************************/
/*! \brief     Set the untagged Rx/Tx queue.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetUntaggedQueue(uint8 channel, uint8 rxUntaggedQueue, uint8 txUntaggedQueue)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  untaggedRxQueue[channel] = rxUntaggedQueue;
  untaggedRxQueue[channel] = txUntaggedQueue;

} /* VttEth_SetUntaggedQueue() */

/***********************************************************************************************************************
*  VttEth_SetVlanPrio2RxQueueMapping
**********************************************************************************************************************/
/*! \brief     Set VLAN prios to Rx queue mapping.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetVlanPrio2RxQueueMapping(uint8 channel, uint8 vlanPrio, uint8 rxQueue)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  vlanPrio2RxQueue[channel][vlanPrio] = rxQueue;
} /* VttEth_SetVlanPrio2RxQueueMapping() */

/***********************************************************************************************************************
*  VttEth_SetVlanPrio2TxQueueMapping
**********************************************************************************************************************/
/*! \brief     Set VLAN prios to Tx queue mapping.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetVlanPrio2TxQueueMapping(uint8 channel, uint8 vlanPrio, uint8 txQueue)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  vlanPrio2TxQueue[channel][vlanPrio] = txQueue;
} /* VttEth_SetVlanPrio2TxQueueMapping() */

/***********************************************************************************************************************
*  VttEth_SetVlanID2RxQueueMapping
**********************************************************************************************************************/
/*! \brief     Set VLAN IDs to Rx queue mapping.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetVlanID2RxQueueMapping(uint8 channel, uint8 vlanID, uint8 rxQueue)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  vlanID2RxQueue[channel][vlanID] = rxQueue;
} /* VttEth_SetVlanID2RxQueueMapping() */

/***********************************************************************************************************************
*  VttEth_SetVlanID2TxQueueMapping
**********************************************************************************************************************/
/*! \brief     Set VLAN IDs to Tx queue mapping.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetVlanID2TxQueueMapping(uint8 channel, uint8 vlanID, uint8 txQueue)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  vlanID2TxQueue[channel][vlanID] = txQueue;
} /* VttEth_SetVlanID2TxQueueMapping() */

/***********************************************************************************************************************
*  VttEth_SetQoSEnabled
**********************************************************************************************************************/
/*! \brief     Set QoS as enabled.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetQoSEnabled(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  isQosEnabled = TRUE;
} /* VttEth_SetQoSEnabled() */

/***********************************************************************************************************************
*  VttEth_SetVLANIDBasedQueuingEnabled
**********************************************************************************************************************/
/*! \brief     Set VLAN ID based queuing as enabled.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetVLANIDBasedQueuingEnabled(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  isVlanIDBasedQueuingEnabled = TRUE;
} /* VttEth_SetVLANIDBasedQueuingEnabled() */

#if (ETH_30_VTT_ENABLE_TIMESYNC == STD_ON)
/***********************************************************************************************************************
*  VttEth_GetHwTime
**********************************************************************************************************************/
/*! \brief     Get the hardware time
*  \details    Retrieves the current time of the Eth controller’s timer.
*              The time stamp of transmission or reception of Ethnert frame in CANoe will be used here as the value
*              of the Eth controller’s timer (This time stamp is same as the time stamp in the CANoe Trace window).
*  \return     E_OK in case the timestamp is reabable, E_NOT_OK otherwise.
*  \context    Function is called by VTT Ether Driver.
*  \note       -
**********************************************************************************************************************/
Std_ReturnType VttEth_GetHwTime(Eth_TimeStampType *hwTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (VttEth_IsCANoeTimerOK() == E_OK)
  {
    hwTime->nanoseconds = (uint32)(hwTimeInNanoseconds % VTTETH_NANOSECONDS_PER_SECOND);
    hwTime->seconds = (uint32)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_PER_SECOND);
    hwTime->secondsHi = (uint16)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_WHEN_SECOND_MAX);
    retVal = E_OK;
  }

  return retVal;
} /* VttEth_GetHwTime() */

/***********************************************************************************************************************
*  VttEth_SetHwTime
**********************************************************************************************************************/
/*! \brief     Set the hardware time
*  \details    Set the current time of the Eth controller’s timer.
*              The time stamp of transmission or reception of Ethnert frame in CANoe will be used here as the value
*              of the Eth controller’s timer (This time stamp is same as the time stamp in the CANoe Trace window).
*              This time stamp based on a CANoe internal timer, it starts with the value 0 when the measurement starts
*              in CANoe. A set/reset while the measurement is not possible.
*  \return     void
*  \context    Function is called by VTT Ether Driver.
*  \note       Currently nothing has been done in this function, because set/reset timer in CANoe while the measurement
*              is not supported.
**********************************************************************************************************************/
void VttEth_SetHwTime(const Eth_TimeStampType *hwTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(hwTime);
  /* ----- Implementation ----------------------------------------------- */
} /* VttEth_SetHwTime() */

/***********************************************************************************************************************
*  VttEth_SetRxDescriptorTimestamp
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor's time stamp fields: nanoseconds, seconds, secondHi.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetRxDescriptorTimestamp(uint8 channel, uint16 descriptorIdx, uint64 hwTimeInNanoseconds, uint8_least rxDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthRxDescriptorBaseAddress[rxDescrRingIdx][descriptorIdx].nanoseconds = (uint32)(hwTimeInNanoseconds % VTTETH_NANOSECONDS_PER_SECOND);
  vttEthRxDescriptorBaseAddress[rxDescrRingIdx][descriptorIdx].seconds = (uint32)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_PER_SECOND);
  vttEthRxDescriptorBaseAddress[rxDescrRingIdx][descriptorIdx].secondsHi = (uint16)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_WHEN_SECOND_MAX);
  CANoeAPI_AtomicEnd();

} /* VttEth_SetRxDescriptorTimestamp() */

/***********************************************************************************************************************
*  VttEth_SetTxDescriptorTimestamp
**********************************************************************************************************************/
/*! \brief     Sets the specified descriptor's time stamp fields: nanoseconds, seconds, secondHi.
*              The access is synchronized between CANoe and VTT threads.
*  \return     void
*  \context    Function is called by VTT Ether VttCntrl.
*  \note       -
**********************************************************************************************************************/
void VttEth_SetTxDescriptorTimestamp(uint8 channel, uint16 descriptorIdx, uint64 hwTimeInNanoseconds, uint8_least txDescrRingIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  VTTCNTRL_IGNORE_UNREF_PARAM(channel);

  /* ----- Implementation ----------------------------------------------- */
  CANoeAPI_AtomicBegin();
  vttEthTxDescriptorBaseAddress[txDescrRingIdx][descriptorIdx].nanoseconds = (uint32)(hwTimeInNanoseconds % VTTETH_NANOSECONDS_PER_SECOND);
  vttEthTxDescriptorBaseAddress[txDescrRingIdx][descriptorIdx].seconds = (uint32)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_PER_SECOND);
  vttEthTxDescriptorBaseAddress[txDescrRingIdx][descriptorIdx].secondsHi = (uint16)(hwTimeInNanoseconds / VTTETH_NANOSECONDS_WHEN_SECOND_MAX);
  CANoeAPI_AtomicEnd();

} /* VttEth_SetTxDescriptorTimestamp() */

/***********************************************************************************************************************
*  VttEth_IsCANoeTimerOK
**********************************************************************************************************************/
/*! \brief     Check if the timer in CANoe works without error
*  \return     E_OK - Timer in CANoe works without error
*  \return     E_NOT_OK - Error with the timer in CANoe
*  \context    Function is called by VTT Ether Driver or VttCntrl.
*  \note       -
**********************************************************************************************************************/
Std_ReturnType VttEth_IsCANoeTimerOK(void)
{
/* ----- Local Variables ---------------------------------------------- */
Std_ReturnType retVal = E_NOT_OK;

/* ----- Implementation ----------------------------------------------- */
CANoeAPI_AtomicBegin();
if (hwTimeInNanoseconds != VTTETH_HW_TIME_ERROR)
{
  retVal = E_OK;
}
CANoeAPI_AtomicEnd();

return retVal;
} /* VttEth_IsCANoeTimerOK() */
#endif

/**********************************************************************************************************************
*  END OF FILE: VttCntrl_Eth.c
*********************************************************************************************************************/

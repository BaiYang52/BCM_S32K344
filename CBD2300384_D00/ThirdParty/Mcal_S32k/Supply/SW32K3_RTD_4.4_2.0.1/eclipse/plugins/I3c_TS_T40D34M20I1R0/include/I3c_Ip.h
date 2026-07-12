/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

#ifndef I3C_IP_H
#define I3C_IP_H

/**
*   @file
*
*   @addtogroup i3c_ip
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "I3c_Ip_Cfg.h"
#include "I3c_Ip_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_IP_VENDOR_ID                    43
#define I3C_IP_MODULE_ID                    255
#define I3C_IP_AR_RELEASE_MAJOR_VERSION     4
#define I3C_IP_AR_RELEASE_MINOR_VERSION     4
#define I3C_IP_AR_RELEASE_REVISION_VERSION  0
#define I3C_IP_SW_MAJOR_VERSION             2
#define I3C_IP_SW_MINOR_VERSION             0
#define I3C_IP_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and I3c_Ip_Cfg header file are of the same vendor */
#if (I3C_IP_VENDOR_ID != I3C_IP_VENDOR_ID_CFG)
    #error "I3c_Ip.h and I3c_Ip_Cfg.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip_Cfg header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION     != I3C_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION     != I3C_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION  != I3C_IP_AR_RELEASE_REVISION_VERSION_CFG))
    #error "AutoSar Version Numbers of I3c_Ip.h and I3c_Ip_Cfg.h are different"
#endif

/* Check if current file and I3c_Ip_Cfg header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION != I3C_IP_SW_MAJOR_VERSION_CFG) || \
     (I3C_IP_SW_MINOR_VERSION != I3C_IP_SW_MINOR_VERSION_CFG) || \
     (I3C_IP_SW_PATCH_VERSION != I3C_IP_SW_PATCH_VERSION_CFG))
    #error "Software Version Numbers of I3c_Ip.h and I3c_Ip_Cfg.h are different"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same vendor */
#if (I3C_IP_VENDOR_ID != I3C_IP_VENDOR_ID_TYPES)
    #error "I3c_Ip.h and I3c_Ip_Types.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION     != I3C_IP_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION     != I3C_IP_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION  != I3C_IP_AR_RELEASE_REVISION_VERSION_TYPES))
    #error "AutoSar Version Numbers of I3c_Ip.h and I3c_Ip_Types.h are different"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION != I3C_IP_SW_MAJOR_VERSION_TYPES) || \
     (I3C_IP_SW_MINOR_VERSION != I3C_IP_SW_MINOR_VERSION_TYPES) || \
     (I3C_IP_SW_PATCH_VERSION != I3C_IP_SW_PATCH_VERSION_TYPES))
    #error "Software Version Numbers of I3c_Ip.h and I3c_Ip_Types.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"

/**
* @brief   Initialize the I3C master module.
*
* @details This function initializes the I3C master module.
*
* @param[in] Instance  I3C instance
* @param[in] Config    Pointer to the configuration structure
*
*/
void I3c_Ip_MasterInit(const uint8 Instance,
                       const I3c_Ip_MasterConfigType * const Config);

/**
* @brief   Deinitialize the I3C master module.
*
* @details This function deinitializes the master module. The driver can't be used until initialized again.
*
* @param[in] Instance   I3C instance
*
*/
void I3c_Ip_MasterDeInit(const uint8 Instance);

/**
* @brief   Return the current status of the I3C master transfer.
*
* @details This function can be called during a non-blocking transmission to check the status of the transfer.
*          In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*          only if BytesRemaining pointer is different than NULL_PTR.
*
* @param[in]  Instance       I3C instance
* @param[out] BytesRemaining Pointer to remaining bytes
*
*/
I3c_Ip_StatusType I3c_Ip_MasterGetTransferStatus(const uint8 Instance,
                                                 uint32 * const BytesRemaining);

/**
* @brief   Perform a blocking send transaction on the I3C bus.
*
* @details This function is used to send a block of data to the configured slave's address.
*          It only returns when the transfer is complete.
*
* @param[in] Instance       I3C instance
* @param[in] TxBuffer       Pointer to the Data buffer
* @param[in] TxBufferSize   Buffer size in bytes
* @param[in] TransferOption Transfer options
*
*/
I3c_Ip_StatusType I3c_Ip_MasterSendBlocking(const uint8 Instance,
                                            const uint8 * TxBuffer,
                                            const uint32 TxBufferSize,
                                            const I3c_Ip_TransferConfigType * const TransferOption);

/**
* @brief   Perform a blocking receive transaction on the I3C bus.
*
* @details This function is used to receive a block of data from the configured slave's address.
*          It only returns when the transfer is complete.
*
* @param[in] Instance       I3C instance
* @param[in] RxBuffer       Pointer to the Data buffer
* @param[in] RxBufferSize   Buffer size in bytes
* @param[in] TransferOption Transfer options
*
*/
I3c_Ip_StatusType I3c_Ip_MasterReceiveBlocking(const uint8 Instance,
                                               uint8 * RxBuffer,
                                               const uint32 RxBufferSize,
                                               const I3c_Ip_TransferConfigType * const TransferOption);

/**
* @brief   Perform a non-blocking send transaction on the I3C bus.
*
* @details This function is used to send a block of data to the configured slave's address.
*          It returns immediately. The rest of the transfer is handled by the interrupt service routine.
*          Use I3c_Ip_MasterGetTransferStatus() to check the progress of the transfer.
*
* @param[in] Instance       I3C instance
* @param[in] TxBuffer       Pointer to the Data buffer
* @param[in] TxBufferSize   Buffer size in bytes
* @param[in] TransferOption Transfer options
*
*/
I3c_Ip_StatusType I3c_Ip_MasterSend(const uint8 Instance,
                                    const uint8 * TxBuffer,
                                    const uint32 TxBufferSize,
                                    const I3c_Ip_TransferConfigType * const TransferOption);

/**
* @brief   Perform a non-blocking receive transaction on the I3C bus.
*
* @details This function is used to receive a block of data from the configured slave's address.
*          It returns immediately. The rest of the transfer is handled by the interrupt service routine.
*          Use I3c_Ip_MasterGetTransferStatus() to check the progress of the transfer.
*
* @param[in] Instance       I3C instance
* @param[in] RxBuffer       Pointer to the Data buffer
* @param[in] RxBufferSize   Buffer size in bytes
* @param[in] TransferOption Transfer options
*
*/
I3c_Ip_StatusType I3c_Ip_MasterReceive(const uint8 Instance,
                                       uint8 * RxBuffer,
                                       const uint32 RxBufferSize,
                                       const I3c_Ip_TransferConfigType * const TransferOption);

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/**
* @brief   Perform a Dynamic Address Assignment on the I3C bus.
*
* @details This function is used to emit ENTDAA command and assign dynamic addresses to the slaves which need one.
*          It shall be used either at bus initialization, or after one or more devices have been added to the I3C bus (hot-join devices).
*          Only the slaves with no dynamic address will be assigned one.
*
* @param[in] Instance         I3C instance
* @param[in] AddressList      Pointer to the addresses which will be assigned. If this is NULL_PTR, then the address list configured
*                             in the initialization config structure will be used.
* @param[in] AddressCount     Number of addresses in the list. This parameter is used only if AddressList is different than NULL_PTR.
* @param[out] SlaveDeviceList Pointer to slave devices characteristics structure containing: DA, BCR, DCR, Vendor Id, Part Number.
*                             If this pointer is NULL_PTR, the function shall not store in the list pointed by SlaveDeviceList, the device characteristics
*                             for the slave devices to which the dynamic addresses have been assigned. Note that when this parameter is not NULL_PTR, it is
*                             the caller's responsibility to correctly dimension the size of the list which will be populated by the function.
*
*/
I3c_Ip_StatusType I3c_Ip_MasterProcessDAA(const uint8 Instance,
                                          const uint8 * const AddressList,
                                          const uint8 AddressCount,
                                          I3c_Ip_SlaveDeviceType * const SlaveDeviceList);
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)*/

/**
* @brief   Initialize the I3C slave module.
*
* @details This function initializes the I3C slave module.
*
* @param[in] Instance   I3C instance
* @param[in] Config     Pointer to the configuration structure
*
*/
void I3c_Ip_SlaveInit(const uint8 Instance,
                      const I3c_Ip_SlaveConfigType * const Config);

/**
* @brief   Deinitialize the I3C slave module.
*
* @details This function deinitializes the slave module. The driver can't be used until initialized again.
*
* @param[in] Instance  I3C instance
*
*/
void I3c_Ip_SlaveDeInit(const uint8 Instance);

/**
* @brief   Return the current status of the I3C slave transfer.
*
* @details This function can be called during a non-blocking transmission to check the status of the transfer.
*          In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*          only if BytesRemaining pointer is different than NULL_PTR.
*
* @param[in] Instance       I3C instance
* @param[in] BytesRemaining Pointer to remaining bytes
*
*/
I3c_Ip_StatusType I3c_Ip_SlaveGetTransferStatus(const uint8 Instance,
                                                uint32 * const BytesRemaining);

/**
* @brief   The main master assigns its own Dynamic Address before handing off the mastership.
*
* @details This function is used to set a Dynamic Address for the main master before handing off the mastership.
*          This address should be written before switching to Slave mode and should not be changed once in Slave mode.
*
* @param[in] Instance   I3C instance
* @param[in] Address    Dynamic Address for the main master to set before becoming a slave
* @param[in] Valid      If TRUE, a Dynamic Address is assigned
*
*/
void I3c_Ip_MasterWriteOwnDynamicAddress(const uint8 Instance,
                                         const uint8 Address,
                                         const boolean Valid);

/**
* @brief   Configure a slave which was the main master and just handed off the mastership.
*
* @details This function is used to configure a slave at runtime, after it was the main master and handed off the mastership.
*
* @param[in] Instance       I3C instance
* @param[in] AddressType    Address type: Dynamic Address or Static Address
* @param[in] Address        7-bit or 10-bit address
* @param[in] Nack           If TRUE, the slave will NACK all messages
* @param[in] Enable         If TRUE, the map entry is enabled.
*
*/
void I3c_Ip_SlaveSetMapAddress(const uint8 Instance,
                               const boolean AddressType,
                               const uint16 Address,
                               const boolean Nack,
                               const boolean Enable);

/**
* @brief   Returns the master error register.
*
* @details This function is used to get the entire error/warning register for the current master.
*          This may be called from the Error/Warning notification and dedicated defines should be used
*          to filter the errors/warnings which occurred.
*          Defines that can be used:
*                   - I3C_IP_MASTER_ERROR_NACK
*                   - I3C_IP_MASTER_ERROR_WRITE_ABORT
*                   - I3C_IP_MASTER_ERROR_TERM
*                   - I3C_IP_MASTER_ERROR_PARITY
*                   - I3C_IP_MASTER_ERROR_CRC
*                   - I3C_IP_MASTER_ERROR_OREAD
*                   - I3C_IP_MASTER_ERROR_OWRITE
*                   - I3C_IP_MASTER_ERROR_MSG
*                   - I3C_IP_MASTER_ERROR_INVALID_REQUEST
*                   - I3C_IP_MASTER_ERROR_TIMEOUT
*
* @param[in] Instance   I3C instance
* @return               Master error register
*
*/
uint32 I3c_Ip_MasterGetError(const uint8 Instance);

/**
* @brief   Returns the slave error register.
*
* @details This function is used to get the entire error/warning register for the current slave.
*          This may be called from the Error/Warning notification and dedicated defines should be used
*          to filter the errors/warnings which occurred.
*          Defines that can be used:
*                   - I3C_IP_SLAVE_ERROR_RX_OVERRUN
*                   - I3C_IP_SLAVE_ERROR_TX_UNDERRUN
*                   - I3C_IP_SLAVE_ERROR_TX_UNDERRUN_NACK
*                   - I3C_IP_SLAVE_ERROR_TERM
*                   - I3C_IP_SLAVE_ERROR_INVALID_START
*                   - I3C_IP_SLAVE_ERROR_SDR_PARITY
*                   - I3C_IP_SLAVE_ERROR_HDR_PARITY_ERROR
*                   - I3C_IP_SLAVE_ERROR_HDR_CRC
*                   - I3C_IP_SLAVE_ERROR_S0S1
*                   - I3C_IP_SLAVE_ERROR_OREAD
*                   - I3C_IP_SLAVE_ERROR_OWRITE
*
* @param[in] Instance   I3C instance
* @return               Slave error register
*
*/
uint32 I3c_Ip_SlaveGetError(const uint8 Instance);

#if (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)
/**
* @brief   Returns the slave Dynamic Address and the cause of assignment.
*
* @details This function is used to get the slave's dynamic address when it was assigned, re-assigned, or reset.
*          This may be called from the DA Changed notification.
*
* @param[in]  Instance          I3C instance
* @param[out] DynamicAddress    Dynamic Address of the slave, if assigned
* @param[out] Cause             Cause of the Dynamic Address change
*
*/
void I3c_Ip_SlaveGetDynamicAddressAndCause(const uint8 Instance,
                                           uint8 * const DynamicAddress,
                                           I3c_Ip_SlaveDACauseType * const Cause);
#endif /* (STD_ON == I3C_IP_ENABLE_I3C_FEATURES)*/

/**
* @brief   Provide a buffer for the slave's data transfer.
*
* @details This function is used to set the slave's buffer for the transfer data. It also set the default RX FIFO watermark.
*          It could be called from the user callback provided at initialization.
*
* @param[in] Instance   I3C instance
* @param[in] DataBuffer Pointer to the data buffer
* @param[in] BufferSize Data buffer size in bytes
*
*/
void I3c_Ip_SlaveSetBuffer(const uint8 Instance,
                           uint8 * DataBuffer,
                           const uint8 BufferSize);

/**
* @brief   Enables the slave Tx Send event if used in I2C mode.
*
* @details This function is used to enable Slave TX interrupt.
*          This is used to fill the Slave TX FIFO before master request.
*          Note that before calling this function, I3c_Ip_SlaveSetBuffer() should be called.
*
* @param[in] Instance   I3C instance
*
*/
void I3c_Ip_SlavePrepareTxFifo(const uint8 Instance);

#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/**
*@brief Perform a Slave Request on the I3C bus.
*
*@details This function is used to request an In-Band Interrupt, Hot-Join, or Master Request.
*         IbiData and ExtData parameters are used only if the Event is an In-Band Interrupt.
*         If the Event is not an In-Band Interrupt, these parameters are ignored in the API's implementation.
*         IbiData and ExtData are ignored if the In-Band Interrupt has no mandatory byte.
*         If the In-Band Interrupt has only one mandatory byte, ExtData is ignored.
*         If Event is I3C_IP_SLAVE_NORMAL_MODE, it will cancel the previous event, if it hasn't started yet.
*         After a Slave request has been initialized, I3C_IP_SLAVE_EVENT_REQUEST callback can be used to check when the request was sent.
*         In addition, SSTATUS[EVDET] can be checked to see Master's decision regarding the request(ACK or NACK).
*         SSTATUS[EVDET] can be also checked to see if the master hasn't issued a START yet,
*         or if the Slave device is still waiting for Bus-Available or Bus-Idle(HJ) condition.
*
* @param[in] Instance   I3C instance
* @param[in] Event      Type of the event: IBI, HJ, MR
* @param[in] IbiData    If IBI, the mandatory byte
* @param[in] ExtData    If IBI has more than one mandatory byte, pointer to the extra bytes
*
*/
void I3c_Ip_SlaveRequestEvent(const uint8 Instance,
                              const I3c_Ip_SlaveRequestType Event,
                              const uint8 IbiData,
                              const uint8 * ExtData);
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/

/**
* @brief   Set the baud rate for all subsequent I3C communications.
*
* @details This function is used to set the baud rate for all subsequent I3C communications.
*          Using this function, the baud rates will be set as close as possible to the target ones.
*          After calling this function, I3c_Ip_MasterGetBaudRate() shall be called to check which baud rates were actually set.
*
* @param[in] Instance           I3C instance
* @param[in] FunctionalClock    Value of the Functional Clock (Hz)
* @param[in] BaudRates          Pointer to the Baud rates structure: PP, OD, I2C (Hz)
* @param[in] BusType            Type of the bus: I2C, SDR, DDR
*
*/
I3c_Ip_StatusType I3c_Ip_MasterSetBaudRate(const uint8 Instance,
                                           const uint32 FunctionalClock,
                                           const I3c_Ip_MasterBaudRateType * BaudRates,
                                           const I3c_Ip_BusType BusType);

/**
* @brief   Get the baud rates of the I3C module.
*
* @details This function is used to get the baud rates of the I3C module.
*
* @param[in]  Instance           I3C instance
* @param[in]  FunctionalClock    Value of the Functional Clock (Hz)
* @param[out] BaudRates          Pointer to the Baud rates structure: PP, OD, I2C (Hz)
*
*/
void I3c_Ip_MasterGetBaudRate(const uint8 Instance,
                              const uint32 FunctionalClock,
                              I3c_Ip_MasterBaudRateType * BaudRates);
#if (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS)
/**
* @brief   Set IBI options for the following Slave Requests.
*
* @details This function is used to set IBI options for the following Slave Requests.
*          If this function is not used, then default IBI options set in configurator will be used.
*          Note that for In-Band interrupts with data bytes, this function must be used to set the buffer
*          in which IBI data bytes will be stored.
*
* @param[in]  Instance      I3C instance
* @param[in]  IbiOptions    Pointer to the structure containing the In-Band interrupt options for the following Slave Requests.
*
*/
void I3c_Ip_MasterSetIbiOptions(const uint8 Instance,
                                const I3c_Ip_MasterIbiOptionType * const IbiOptions);

/**
* @brief   Set Manual In-Band Interrupt Response.
*
* @details This function is used by the master to take a decision (ACK/NACK) for an In-Band Interrupt which needs manual intervention.
*          Note that for Hot-Join and Master-Request, this function must be always called in I3C_IP_MASTER_EVENT_HOT_JOIN and I3C_IP_MASTER_EVENT_MASTER_REQUEST
*          callbacks.
*
* @param[in]  Instance      I3C instance
* @param[in]  IbiResponse   IBI Response: ACK/NACK
*
*/
void I3c_Ip_MasterManualIbiResponse(const uint8 Instance,
                                    const I3c_Ip_IbiResponseType IbiResponse);
#endif /* (STD_ON == I3C_IP_ENABLE_SLAVE_REQUESTS )*/

/**
* @brief   Returns the master status register.
*
* @details This function is used to get the status register for the current master.
*          Dedicated defines can be used to filter and check each status bit field: I3C_IP_MASTER_STATUS
*          For I3C_IP_MASTER_STATUS_IBIADDR, I3C_IP_MASTER_STATUS_IBITYPE, and I3C_IP_MASTER_STATUS_STATE
*          the following defines can be used to shift the masked values to get the actual value of the bit
*          fields (they have more than 1 bit length):
*                   - I3C_IP_MASTER_STATUS_IBIADDR_SHIFT
*                   - I3C_IP_MASTER_STATUS_IBITYPE_SHIFT
*                   - I3C_IP_MASTER_STATUS_STATE_SHIFT
*
* @param[in] Instance   I3C instance
* @return               Master status register
*
*/
uint32 I3c_Ip_MasterGetStatus(const uint8 Instance);

/**
* @brief   Returns the master status register.
*
* @details This function is used to get the status register for the current slave device.
*          Dedicated defines can be used to filter and check each status bit field: I3C_IP_SLAVE_STATUS
*          For I3C_IP_SLAVE_STATUS_TIMECTRL, I3C_IP_SLAVE_STATUS_ACTSTATE, and I3C_IP_SLAVE_STATUS_EVDET
*          the following defines can be used to shift the masked values to get the actual value of the bit
*          fields (they have more than 1 bit length):
*                   - I3C_IP_SLAVE_STATUS_TIMECTRL_SHIFT
*                   - I3C_IP_SLAVE_STATUS_ACTSTATE_SHIFT
*                   - I3C_IP_SLAVE_STATUS_EVDET_SHIFT
*
* @param[in] Instance   I3C instance
* @return               Slave status register
*
*/
uint32 I3c_Ip_SlaveGetStatus(const uint8 Instance);


#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* I3C_IP_H */

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

#ifndef I3C_H
#define I3C_H

/**
*   @file
*
*   @addtogroup i3c
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
#include "CDD_I3c_Cfg.h"
#include "CDD_I3c_Types.h"
#include "I3c_Ip.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_VENDOR_ID                    43
#define I3C_MODULE_ID                    255
#define I3C_AR_RELEASE_MAJOR_VERSION     4
#define I3C_AR_RELEASE_MINOR_VERSION     4
#define I3C_AR_RELEASE_REVISION_VERSION  0
#define I3C_SW_MAJOR_VERSION             2
#define I3C_SW_MINOR_VERSION             0
#define I3C_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and CDD_I3c_Cfg header file are of the same vendor */
#if (I3C_VENDOR_ID != I3C_VENDOR_ID_CFG)
    #error "CDD_I3c.h and CDD_I3c_Cfg.h have different vendor ids"
#endif

/* Check if current file and CDD_I3c_Cfg header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION     != I3C_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (I3C_AR_RELEASE_MINOR_VERSION     != I3C_AR_RELEASE_MINOR_VERSION_CFG) || \
     (I3C_AR_RELEASE_REVISION_VERSION  != I3C_AR_RELEASE_REVISION_VERSION_CFG))
    #error "AutoSar Version Numbers of CDD_I3c.h and CDD_I3c_Cfg.h are different"
#endif

/* Check if current file and CDD_I3c_Cfg header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION != I3C_SW_MAJOR_VERSION_CFG) || \
     (I3C_SW_MINOR_VERSION != I3C_SW_MINOR_VERSION_CFG) || \
     (I3C_SW_PATCH_VERSION != I3C_SW_PATCH_VERSION_CFG))
    #error "Software Version Numbers of CDD_I3c.h and CDD_I3c_Cfg.h are different"
#endif

/* Check if current file and CDD_I3c_Types header file are of the same vendor */
#if (I3C_VENDOR_ID != I3C_VENDOR_ID_TYPES)
    #error "CDD_I3c.h and CDD_I3c_Types.h have different vendor ids"
#endif

/* Check if current file and CDD_I3c_Types header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION     != I3C_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (I3C_AR_RELEASE_MINOR_VERSION     != I3C_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (I3C_AR_RELEASE_REVISION_VERSION  != I3C_AR_RELEASE_REVISION_VERSION_TYPES))
    #error "AutoSar Version Numbers of CDD_I3c.h and CDD_I3c_Types.h are different"
#endif

/* Check if current file and CDD_I3c_Types header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION != I3C_SW_MAJOR_VERSION_TYPES) || \
     (I3C_SW_MINOR_VERSION != I3C_SW_MINOR_VERSION_TYPES) || \
     (I3C_SW_PATCH_VERSION != I3C_SW_PATCH_VERSION_TYPES))
    #error "Software Version Numbers of CDD_I3c.h and CDD_I3c_Types.h are different"
#endif

/* Check if current file and I3c_Ip header file are of the same vendor */
#if (I3C_VENDOR_ID != I3C_IP_VENDOR_ID)
    #error "CDD_I3c.h and I3c_Ip.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION     != I3C_IP_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_AR_RELEASE_MINOR_VERSION     != I3C_IP_AR_RELEASE_MINOR_VERSION) || \
     (I3C_AR_RELEASE_REVISION_VERSION  != I3C_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_I3c.h and I3c_Ip.h are different"
#endif

/* Check if current file and I3c_Ip header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION != I3C_IP_SW_MAJOR_VERSION) || \
     (I3C_SW_MINOR_VERSION != I3C_IP_SW_MINOR_VERSION) || \
     (I3C_SW_PATCH_VERSION != I3C_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_I3c.h and I3c_Ip.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/**
* @brief   APIs service IDs.
*
* @details Service IDs for the I3C driver APIs.
*
*/
#define I3C_INIT_ID                         (0x00U) /**< I3c_Init() ID */
#define I3C_DEINIT_ID                       (0x01U) /**< I3c_Deinit() ID */
#define I3C_SYNCTRANSFER_ID                 (0x02U) /**< I3c_SyncTransmit() ID */
#define I3C_ASYNCTRANSFER_ID                (0x03U) /**< I3c_AsyncTransmit() ID */
#define I3C_GETTRANSFERSTATUS_ID            (0x04U) /**< I3c_GetTransferStatus() ID */
#define I3C_PREPARESLAVEBUFFER_ID           (0x05U) /**< I3c_PrepareSlaveBuffer() ID */
#define I3C_GETVERSIONINFO_ID               (0x06U) /**< I3c_GetVersionInfo() ID */
#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
#define I3C_ASSIGNDYNAMICADDRESS_ID         (0x07U) /**< I3c_AssignDynamicAddress() ID */
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES)*/
#define I3C_GETERROR_ID                     (0x08U) /**< I3c_GetError() ID */
#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
#define I3C_GETDYNAMICADDRESSINFO_ID        (0x09U) /**< I3c_GetDynamicAddressInfo() ID */
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES)*/
#define I3C_PREPARESLAVETXFIFO_ID           (0x0AU) /**< I3c_PrepareSlaveTransmission() ID */
#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
#define I3C_SLAVEREQUESTEVENT_ID            (0x0BU) /**< I3c_SlaveRequestEvent() ID */
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/
#define I3C_MASTERSETBAUDRATE_ID            (0x0CU) /**< I3c_MasterSetBaudRate() ID */
#define I3C_MASTERGETBAUDRATE_ID            (0x0DU) /**< I3c_MasterGetBaudRate() ID */
#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
#define I3C_MASTERSETIBIOPTIONS_ID          (0x0EU) /**< I3c_MasterSetIbiOptions() ID */
#define I3C_MASTERMANUALIBIRESPONSE_ID      (0x0FU) /**< I3c_MasterManualIbiResponse() ID */
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/
#define I3C_GETSTATUS_ID                    (0x10U) /**< I3c_GetStatus() ID */

#if (STD_ON == I3C_DEV_ERROR_DETECT)
/**
* @brief   Det error codes.
*
* @details Error codes that the I3C driver uses for generating Det errors
*
*/
#define I3C_E_NOT_INITIALIZED       (0x01U) /**< API service used without module initialization */
#define I3C_E_INIT_FAILED           (0x02U) /**< API service called with invalid configuration pointer */
#define I3C_E_INVALID_POINTER       (0x03U) /**< API service called with an invalid pointer */
#define I3C_E_INVALID_MODE          (0x04U) /**< API service called with an invalid pointer */
#define I3C_E_ALREADY_INITIALIZED   (0x05U) /**< Init API service used when the module has been already initialized */
#define I3C_E_PARAM_CONFIG          (0x06U) /**< API service called with wrong assigned resource (core partition)*/
#define I3C_E_INVALID_BUFFER_SIZE   (0x07U) /**< Number of bytes is exceeded, if a limit exists for the channel */

#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
#define I3C_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "I3c_MemMap.h"

#ifdef I3C_PRECOMPILE_SUPPORT
    extern const I3c_ConfigType * const I3c_ConfigVariantPredefined[I3C_MAX_CORE_ID];
#else
    I3C_CONFIG_EXT
#endif

#define I3C_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "I3c_MemMap.h"

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"

/**
* @brief   Initialize the I3C module.
*
* @details This function initializes the I3C module:
*               - Maps the logical channel to the hardware channel
*               - Initializes the channel
*
* @param[in] Config Pointer to the configuration structure
*
*/
void I3c_Init(const I3c_ConfigType * const Config);

/**
* @brief   Deinitialize the I3C module.
*
* @details This function deinitializes the I3C module to the reset values.
*          The driver must be initialized before calling I3c_DeInit().
*
*/
void I3c_DeInit(void);

/**
* @brief   Send or receive a blocking message on the I3C bus.
*
* @details This function is used to send or receive a message, depending on the direction of the transfer.
*          The message is sent or read in a blocking manner. It only returns when the transfer is complete.
*
* @param[in] Channel     I3C Channel
* @param[in] DataBuffer  Pointer to the Data buffer
* @param[in] BufferSize  Buffer size in bytes
* @param[in] Request     Pointer to the transfer information
*
* @return   Std_ReturnType
*
*/
Std_ReturnType I3c_SyncTransfer(const uint8 Channel,
                                uint8 * DataBuffer,
                                const uint32 BufferSize,
                                const I3c_RequestType * const Request);

/**
* @brief   Send or receive a non-blocking message on the I3C bus.
*
* @details This function is used to send or receive a message, depending on the direction of the transfer.
*          The message is sent or read in a non-blocking manner. It returns immediately.
*          The rest of the transmission is handled by the interrupt service routine.
*
* @param[in] Channel     I3C Channel
* @param[in] DataBuffer  Pointer to the Data buffer
* @param[in] BufferSize  Buffer size in bytes
* @param[in] Request     Pointer to the transfer information
*
* @return   Std_ReturnType
*
*/
Std_ReturnType I3c_AsyncTransfer(const uint8 Channel,
                                 uint8 * DataBuffer,
                                 const uint32 BufferSize,
                                 const I3c_RequestType * const Request);

/**
* @brief    Get the status of an I3C channel.
*
* @details  This function is used to get the status of a transfer and check for errors.
*           In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*           only if BytesRemaining pointer is different than NULL_PTR.
*
* @param[in]  Channel        I3C Channel
* @param[out] BytesRemaining Remaining bytes to be transferred.
* @return   I3c_StatusType
*
*/
I3c_StatusType I3c_GetTransferStatus(const uint8 Channel,
                                     uint32 * const BytesRemaining);

/**
* @brief   Prepare the Tx or Rx data buffer for a slave channel.
*
* @details Prepares the Tx or Rx buffer that will be used to send data or receive data when requested by the master.
*
* @param[in] Channel     I3C Channel
* @param[in] DataBuffer  Pointer to the data buffer
* @param[in] BufferSize  Data buffer size in bytes
*
* @return   Std_ReturnType
*
*/
Std_ReturnType I3c_PrepareSlaveBuffer(const uint8 Channel,
                                      uint8 * DataBuffer,
                                      const uint8 BufferSize);

#if (STD_ON == I3C_VERSION_INFO_API)
/**
* @brief   Returns the version information of the I3C module.
*
* @details The version information includes:
*
*          - Two bytes for the Vendor ID
*          - Two bytes for the Module ID
*          - One byte for the Channel ID
*          - Three bytes version number. The numbering shall be vendor specific and it consists of:
*               - The major, the minor and the patch version number of the module;
*               - The AUTOSAR specification version number shall not be included.
*                 The AUTOSAR specification version number is checked during compile time and therefore not required in this API.

*
* @param[in] VersionInfo Pointer to the version info structure
*
*/
void I3c_GetVersionInfo(Std_VersionInfoType * VersionInfo);
#endif /* (STD_ON == I3C_VERSION_INFO_API) */

/**
* @brief   Returns the error register, either for the master or the slave, depending on the configured channel.
*
* @details This function is used to get the entire error/warning register for the current channel.
*          This may be called from the Error/Warning notification and dedicated defines should be used
*          to filter the errors/warnings which occurred.
*
* @param[in] Channel I3C Channel
* @return   Master/Slave error register
*
*/
uint32 I3c_GetError(const uint8 Channel);

#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
/**
* @brief   Returns the slave Dynamic Address and the cause of assignment.
*
* @details This function is used to get the slave's dynamic address when it was assigned, re-assigned, or reset.
*          This may be called from the DA Changed notification.
*
* @param[in]  Channel           I3C Channel
* @param[out] DynamicAddress    Dynamic Address of the slave, if assigned
* @param[out] Cause             Cause of the Dynamic Address change
*
*/
Std_ReturnType I3c_GetDynamicAddressInfo(const uint8 Channel,
                                         uint8 * DynamicAddress,
                                         I3c_DynamicAddressChangeType * Cause);

/**
* @brief   Perform a Dynamic Address Assignment on the I3C bus.
*
* @details This function is used to emit ENTDAA command and assign dynamic addresses to the newly added slaves.
*          It shall be used after one or more device have been added to the I3C bus, after the bus was initialized.
*          Only the slaves with no dynamic address will be assigned one.
*
* @param[in]  Channel           I3C Channel
* @param[in]  AddressList       Pointer to the addresses which will be assigned. If this is NULL_PTR, then the address list configured
*                               in the initialization config structure will be used.
* @param[in]  AddressCount      Number of addresses in the list
* @param[out] SlaveDeviceList   Pointer to slave devices characteristics structure containing: DA, BCR, DCR, Vendor Id, Part Number.
*                               If this pointer is NULL_PTR, the function shall not store in the list pointed by SlaveDeviceList, the device characteristics
*                               for the slave devices to which the dynamic addresses have been assigned. Note that when this parameter is not NULL_PTR, it is
*                               the caller's responsibility to correctly dimension the size of the list which will be populated by the function.
*
*/
I3c_StatusType I3c_AssignDynamicAddress(const uint8 Channel,
                                        const uint8 * AddressList,
                                        const uint8 AddressCount,
                                        I3c_SlaveDeviceType * const SlaveDeviceList);
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES)*/

/**
* @brief   Enables the slave Tx Send event if used in I2C mode.
*
* @details This function is used to enable Slave TX interrupt.
*          This is used to fill the Slave TX FIFO before master request.
*          Note that before calling this function, I3c_PrepareSlaveBuffer() should be called.
*
* @param[in] Channel I3C Channel
*/
Std_ReturnType I3c_PrepareSlaveTransmission(const uint8 Channel);

#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
/**
*@brief Perform a Slave Request on the I3C bus.
*
*@details This function is used to request an In-Band Interrupt, Hot-Join, or Master Request.
*         IbiData and ExtData parameters are used only if the Event is an In-Band Interrupt.
*         If the Event is not an In-Band Interrupt, these parameters are ignored in the API's implementation.
*         IbiData and ExtData are ignored if the In-Band Interrupt has no mandatory byte.
*         If the In-Band Interrupt has only one mandatory byte, ExtData is ignored.
*         If Event is I3C_SLAVE_NORMAL_MODE, it will cancel the previous event, if it hasn't started yet.
*
* @param[in] Channel   I3C Channel
* @param[in] Event     Type of the event: IBI, HJ, MR
* @param[in] IbiData   If IBI, the mandatory byte
* @param[in] ExtData   If IBI has more than one mandatory byte, pointer to the extra bytes
*
*/
Std_ReturnType I3c_SlaveRequestEvent(const uint8 Channel,
                                     const I3c_SlaveRequestType Event,
                                     const uint8 IbiData,
                                     const uint8 * ExtData);
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/

/**
* @brief   Set the baud rate for all subsequent I3C communications.
*
* @details This function is used to set the baud rate for all subsequent I3C communications.
*          Using this function, the baud rates will be set as close as possible to the target ones.
*          After calling this function, I3c_MasterGetBaudRate() shall be called to check which baud rates were actually set.
*
* @param[in] Channel           I3C Channel
* @param[in] FunctionalClock   Value of the Functional Clock (Hz)
* @param[in] BaudRates         Pointer to the Baud rates structure: PP, OD, I2C (Hz)
* @param[in] BusType           Type of the bus: I2C, SDR, DDR
*
*/
Std_ReturnType I3c_MasterSetBaudRate(const uint8 Channel,
                                     const uint32 FunctionalClock,
                                     const I3c_MasterBaudRateType * BaudRates,
                                     const I3c_BusType BusType);


/**
* @brief   Get the baud rates of the I3C module.
*
* @details This function is used to get the baud rates of the I3C module.
*
* @param[in]  Channel           I3C Channel
* @param[in]  FunctionalClock   Value of the Functional Clock (Hz)
* @param[out] BaudRates         Pointer to the Baud rates structure: PP, OD, I2C (Hz)
*
*/
Std_ReturnType I3c_MasterGetBaudRate(const uint8 Channel,
                                     const uint32 FunctionalClock,
                                     I3c_MasterBaudRateType * BaudRates);
#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
/**
* @brief   Set IBI options for the following Slave Requests.
*
* @details This function is used to set IBI options for the following Slave Requests.
*          If this function is not used, then default IBI options set in configurator will be used.
*          Note that for In-Band interrupts with data bytes, this function must be used to set the buffer
*          in which IBI data bytes will be stored.
*
* @param[in]  Channel       I3C Channel
* @param[in]  IbiOptions    Pointer to the structure containing the In-Band interrupt options for the following Slave Requests.
*
*/
Std_ReturnType I3c_MasterSetIbiOptions(const uint8 Channel,
                                       const I3c_MasterIbiOptionType * const IbiOptions);

/**
* @brief   ACK/NACK an In-Band Interrupt when Manual Intervention is needed.
*
* @details This function is used by the master to take a decision (ACK/NACK) for an In-Band Interrupt which needs manual intervention.
*          Note that for Hot-Join and Master-Request, this function must be always called in I3C_IP_MASTER_EVENT_HOT_JOIN and I3C_IP_MASTER_EVENT_MASTER_REQUEST callbacks.
*
* @param[in] Channel        I3C Channel
* @param[in] IbiResponse    IBI Response: ACK/NACK
*
*/
Std_ReturnType I3c_MasterManualIbiResponse(const uint8 Channel,
                                           const I3c_IbiResponseType IbiResponse);
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/
/**
* @brief   Returns the error register, either for the master or the slave, depending on the configured channel.
*
* @details This function is used to get the entire status register for the current channel.
*          This may be called from the provided notification and dedicated defines should be used
*          to check which specific status is set.
*
* @param[in] Channel I3C Channel
* @return    Master/Slave status register
*
*/
uint32 I3c_GetStatus(const uint8 Channel);

#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* I3C_H */

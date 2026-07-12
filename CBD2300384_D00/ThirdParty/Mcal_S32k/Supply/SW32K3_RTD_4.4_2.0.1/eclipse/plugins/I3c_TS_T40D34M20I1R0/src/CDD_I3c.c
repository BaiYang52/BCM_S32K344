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
#include "CDD_I3c.h"
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    #include "Det.h"
#endif

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_VENDOR_ID_C                      43
#define I3C_AR_RELEASE_MAJOR_VERSION_C       4
#define I3C_AR_RELEASE_MINOR_VERSION_C       4
#define I3C_AR_RELEASE_REVISION_VERSION_C    0
#define I3C_SW_MAJOR_VERSION_C               2
#define I3C_SW_MINOR_VERSION_C               0
#define I3C_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and CDD_I3c header file are of the same vendor */
#if (I3C_VENDOR_ID_C != I3C_VENDOR_ID)
    #error "CDD_I3c.c and CDD_I3c.h have different vendor ids"
#endif

/* Check if current file and CDD_I3c header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_C     != I3C_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_AR_RELEASE_MINOR_VERSION_C     != I3C_AR_RELEASE_MINOR_VERSION) || \
     (I3C_AR_RELEASE_REVISION_VERSION_C  != I3C_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of CDD_I3c.c and CDD_I3c.h are different"
#endif

/* Check if current file and CDD_I3c header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_C != I3C_SW_MAJOR_VERSION) || \
     (I3C_SW_MINOR_VERSION_C != I3C_SW_MINOR_VERSION) || \
     (I3C_SW_PATCH_VERSION_C != I3C_SW_PATCH_VERSION))
    #error "Software Version Numbers of CDD_I3c.c and CDD_I3c.h are different"
#endif

#if (STD_ON == I3C_DEV_ERROR_DETECT)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if source file and Det header file are of the same AutoSar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CDD_I3c.c and Det.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */
#endif /* STD_ON == I3C_DEV_ERROR_DETECT */

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
#define I3C_START_SEC_VAR_INIT_UNSPECIFIED
#include "I3c_MemMap.h"

static const I3c_ConfigType * I3c_apxInitConfig[I3C_MAX_CORE_ID] = I3C_NULL_PTR;

#define I3C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "I3c_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define I3C_START_SEC_CODE
#include "I3c_MemMap.h"

#if (STD_ON == I3C_DEV_ERROR_DETECT)
static inline void I3c_ReportDetError(uint8 ServiceId,
                                      uint8 ErrorId);

static inline Std_ReturnType I3c_CheckInit(const I3c_ConfigType * const Config,
                                           uint8 CoreId);

static inline Std_ReturnType I3c_CheckApiCall(uint8 CoreId,
                                              uint8 ServiceId);

static inline Std_ReturnType I3c_CheckTransfer(const uint8 Channel,
                                               const I3c_RequestType * const Request,
                                               const uint8 * const DataBuffer,
                                               uint8 CoreId,
                                               uint8 ServiceId);

static inline Std_ReturnType I3c_CheckModeAndBuffer(const uint8 Channel,
                                                    const uint8 * const Buffer,
                                                    I3c_MasterSlaveModeType Mode,
                                                    uint8 CoreId,
                                                    uint8 ServiceId);
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
#if (STD_ON == I3C_DEV_ERROR_DETECT)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_ReportDetError
* Description   : This function reports the DET error
*
* END**************************************************************************/
static inline void I3c_ReportDetError(uint8 ServiceId,
                                      uint8 ErrorId)
{
    (void)Det_ReportError((uint16)I3C_MODULE_ID, (uint8)0U, ServiceId, ErrorId);
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_CheckInit
* Description   : This function checks the parameters of initialization of the I3C module. It returns:
*               - E_OK: The parameters are valid
*               - E_NOT_OK: The parameters are invalid
*
* END**************************************************************************/
static inline Std_ReturnType I3c_CheckInit(const I3c_ConfigType * const Config,
                                           uint8 CoreId)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_OK;

#ifdef I3C_PRECOMPILE_SUPPORT
    if (NULL_PTR != Config)
#else
    if (NULL_PTR == Config)
#endif /* I3C_PRECOMPILE_SUPPORT */
    {
        ReturnValue = (Std_ReturnType)E_NOT_OK;
        I3c_ReportDetError((uint8)I3C_INIT_ID, (uint8)I3C_E_INIT_FAILED);
    }
    else
    {
#ifdef I3C_PRECOMPILE_SUPPORT
        if((NULL_PTR == I3c_ConfigVariantPredefined[CoreId]) || (CoreId != I3c_ConfigVariantPredefined[CoreId]->I3c_CoreId))
#else
        if(CoreId != Config->I3c_CoreId)
#endif /* I3C_PRECOMPILE_SUPPORT */
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError(I3C_INIT_ID, (uint8)I3C_E_PARAM_CONFIG);
        }
        else
        {
            if (I3c_apxInitConfig[CoreId] != NULL_PTR)
            {
                ReturnValue = (Std_ReturnType)E_NOT_OK;
                I3c_ReportDetError((uint8)I3C_INIT_ID, (uint8)I3C_E_ALREADY_INITIALIZED);
            }
        }
    }
    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_CheckApiCall
* Description   : This function checks if the current API has been called before I3C module was initialized.
*                 It also checks for valid Core ID. It returns:
*               - E_OK: The Core ID is valid and the API was called after the I3C module was initialized
*               - E_NOT_OK: The Core ID is invalid or the API was called before the I3C module was initialized
*
* END**************************************************************************/
static inline Std_ReturnType I3c_CheckApiCall(uint8 CoreId,
                                              uint8 ServiceId)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_OK;

    if (I3c_apxInitConfig[CoreId] == NULL_PTR)
    {
        ReturnValue = (Std_ReturnType)E_NOT_OK;
        I3c_ReportDetError(ServiceId, (uint8)I3C_E_NOT_INITIALIZED);
    }
    else
    {
#ifdef I3C_PRECOMPILE_SUPPORT
        if((NULL_PTR == I3c_ConfigVariantPredefined[CoreId]) || (CoreId != I3c_ConfigVariantPredefined[CoreId]->I3c_CoreId))
#else
        if(CoreId != I3c_apxInitConfig[CoreId]->I3c_CoreId)
#endif /* I3C_PRECOMPILE_SUPPORT */
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError(ServiceId, (uint8)I3C_E_PARAM_CONFIG);
        }
    }

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_CheckTransfer
* Description   : This function checks if the current I3C module is in Master mode for initializing the transfer.
*                 It also checks for valid pointers to the transfer options structure and to the data buffer.
*               - E_OK: The I3C module is configured as a Master and the pointer to the transfer options structure and to the data buffer are valid
*               - E_NOT_OK: The I3C module is configured as a Slave or the pointer to the transfer options structure or to the data buffer are invalid
*
* END**************************************************************************/
static inline Std_ReturnType I3c_CheckTransfer(const uint8 Channel,
                                               const I3c_RequestType * const Request,
                                               const uint8 * const DataBuffer,
                                               uint8 CoreId,
                                               uint8 ServiceId)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_OK;

    if ((NULL_PTR == Request) || (NULL_PTR == DataBuffer))
    {
        ReturnValue = (Std_ReturnType)E_NOT_OK;
        I3c_ReportDetError(ServiceId, (uint8)I3C_E_INVALID_POINTER);
    }
    else
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError(ServiceId, (uint8)I3C_E_INVALID_MODE);
        }
    }

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_CheckSlaveMode
* Description   : This function checks if the current I3C module is in correct mode.
*                 It also checks for valid pointer given as parameter
*               - E_OK: The I3C module is configured in the correct mode and the pointer is valid
*               - E_NOT_OK: The I3C module is not configured in the correct mode or the pointer is invalid
*
* END**************************************************************************/
static inline Std_ReturnType I3c_CheckModeAndBuffer(const uint8 Channel,
                                                    const uint8 * const Buffer,
                                                    I3c_MasterSlaveModeType Mode,
                                                    uint8 CoreId,
                                                    uint8 ServiceId)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_OK;

    if (NULL_PTR == Buffer)
    {
        ReturnValue = (Std_ReturnType)E_NOT_OK;
        I3c_ReportDetError(ServiceId, (uint8)I3C_E_INVALID_POINTER);
    }
    else
    {
        if (I3C_MASTER_MODE == Mode)
        {
            if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
            {
                ReturnValue = (Std_ReturnType)E_NOT_OK;
                I3c_ReportDetError(ServiceId, (uint8)I3C_E_INVALID_MODE);
            }
        }
        else
        {
            if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
            {
                ReturnValue = (Std_ReturnType)E_NOT_OK;
                I3c_ReportDetError(ServiceId, (uint8)I3C_E_INVALID_MODE);
            }
        }
    }

    return ReturnValue;
}

#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
/*FUNCTION**********************************************************************
*
* Function Name : I3c_Init
* Description   : This function initializes the I3C module:
*               - Maps the logical channel to the hardware channel
*               - Initializes the channel
*
* @implements     I3c_Init_Activity
* END**************************************************************************/
void I3c_Init(const I3c_ConfigType * const Config)
{
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    uint8 ChLoop;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    Std_ReturnType CheckStatus = I3c_CheckInit(Config, CoreId);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
#ifdef I3C_PRECOMPILE_SUPPORT
        (void)Config;
        I3c_apxInitConfig[CoreId] = I3c_ConfigVariantPredefined[CoreId];;
#else
        I3c_apxInitConfig[CoreId] = Config;
#endif /* I3C_PRECOMPILE_SUPPORT */
        for (ChLoop = (uint8)0U; ChLoop < I3C_HW_MAX_CONFIG; ChLoop++)
        {
            if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->MasterSlaveModeConfig)
            {
                I3c_Ip_MasterInit(I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->I3c_HwUnit, I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->HwChannelConfig->MasterConfig);
            }
            else
            {
                I3c_Ip_SlaveInit(I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->I3c_HwUnit, I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->HwChannelConfig->SlaveConfig);
            }
        }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_DeInit
* Description   : This function deinitializes the I3C module to the reset values.
*                 The driver must be initialized before calling I3c_DeInit().
*
* @implements     I3c_DeInit_Activity
* END**************************************************************************/
void I3c_DeInit(void)
{
    uint8 ChLoop;
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_DEINIT_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        for (ChLoop = (uint8)0U; ChLoop < I3C_HW_MAX_CONFIG; ChLoop++)
        {
            if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->MasterSlaveModeConfig)
            {
                I3c_Ip_MasterDeInit(I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->I3c_HwUnit);
            }
            else
            {
                I3c_Ip_SlaveDeInit(I3c_apxInitConfig[CoreId]->I3c_HwConfig[ChLoop]->I3c_HwUnit);
            }
        }
        I3c_apxInitConfig[CoreId] = NULL_PTR;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_SyncTransfer
* Description   : This function is used to send or receive a message, depending on the direction of the transfer.
*                 The message is sent or read in a blocking manner. It only returns when the transfer is complete.
*
* @implements     I3c_SyncTransfer_Activity
* END**************************************************************************/
Std_ReturnType I3c_SyncTransfer(const uint8 Channel,
                                uint8 * DataBuffer,
                                const uint32 BufferSize,
                                const I3c_RequestType * const Request)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;
    I3c_Ip_TransferConfigType IpRequest;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_SYNCTRANSFER_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        CheckStatus = I3c_CheckTransfer(Channel, Request, DataBuffer, CoreId, (uint8)I3C_SYNCTRANSFER_ID);
        if ((Std_ReturnType)E_OK == CheckStatus)
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, NULL_PTR))
            {
                IpRequest.SlaveAddress = Request->SlaveAddress;
                IpRequest.SendStop = Request->SendStop;
                IpRequest.Direction = (I3c_Ip_DirectionType)Request->Direction;
                IpRequest.TransferSize = (I3c_Ip_TransferSizeType)Request->TransferSize;
                IpRequest.BusType = (I3c_Ip_BusType)Request->BusType;

                if (I3C_SEND == Request->Direction)
                {
                    ReturnValue = (Std_ReturnType)I3c_Ip_MasterSendBlocking(Channel, DataBuffer, BufferSize, &IpRequest);
                }
                else
                {
                    ReturnValue = (Std_ReturnType)I3c_Ip_MasterReceiveBlocking(Channel, DataBuffer, BufferSize, &IpRequest);
                }
            }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_AsyncTransfer
* Description   : This function is used to send or receive a message, depending on the direction of the transfer.
*                 The message is sent or read in a non-blocking manner. It returns immediately.
*                 The rest of the transfer is handled by the interrupt service routine.
*
* @implements     I3c_AsyncTransfer_Activity
* END**************************************************************************/
Std_ReturnType I3c_AsyncTransfer(const uint8 Channel,
                                 uint8 * DataBuffer,
                                 const uint32 BufferSize,
                                 const I3c_RequestType * const Request)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;
    I3c_Ip_TransferConfigType IpRequest;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_ASYNCTRANSFER_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        CheckStatus = I3c_CheckTransfer(Channel, Request, DataBuffer, CoreId, (uint8)I3C_ASYNCTRANSFER_ID);
        if ((Std_ReturnType)E_OK == CheckStatus)
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, NULL_PTR))
            {
                IpRequest.SlaveAddress = Request->SlaveAddress;
                IpRequest.SendStop = Request->SendStop;
                IpRequest.Direction = (I3c_Ip_DirectionType)Request->Direction;
                IpRequest.TransferSize = (I3c_Ip_TransferSizeType)Request->TransferSize;
                IpRequest.BusType = (I3c_Ip_BusType)Request->BusType;

                if (I3C_SEND == Request->Direction)
                {
                    ReturnValue = (Std_ReturnType)I3c_Ip_MasterSend(Channel, DataBuffer, BufferSize, &IpRequest);
                }
                else
                {
                    ReturnValue = (Std_ReturnType)I3c_Ip_MasterReceive(Channel, DataBuffer, BufferSize, &IpRequest);
                }
            }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_PrepareSlaveBuffer
* Description   : Prepares the Tx or Rx buffer that will be used to send data or receive data when requested by the master.
*
* @implements     I3c_PrepareSlaveBuffer_Activity
* END**************************************************************************/
Std_ReturnType I3c_PrepareSlaveBuffer(const uint8 Channel,
                                      uint8 * DataBuffer,
                                      const uint8 BufferSize)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_PREPARESLAVEBUFFER_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        CheckStatus = I3c_CheckModeAndBuffer(Channel, DataBuffer, I3C_SLAVE_MODE, CoreId, (uint8)I3C_PREPARESLAVEBUFFER_ID);
        if ((Std_ReturnType)E_OK == CheckStatus)
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            I3c_Ip_SlaveSetBuffer(Channel, DataBuffer, BufferSize);
            ReturnValue = (Std_ReturnType)E_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_GetTransferStatus
* Description   : This function is used to get the status of a transfer and check for errors.
*                 In addition, if the transfer is still in progress, this functions returns the remaining bytes to be transferred,
*                 only if BytesRemaining pointer is different than NULL_PTR.
*
* @implements     I3c_GetTransferStatus_Activity
* END**************************************************************************/
I3c_StatusType I3c_GetTransferStatus(const uint8 Channel,
                                     uint32 * const BytesRemaining)
{
    I3c_StatusType ReturnValue = I3C_STATUS_SUCCESS;
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_GETTRANSFERSTATUS_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, BytesRemaining);
        }
        else
        {
            ReturnValue = (I3c_StatusType)I3c_Ip_SlaveGetTransferStatus(Channel, BytesRemaining);
        }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_AssignDynamicAddress
* Description   : This function is used to emit ENTDAA command and assign dynamic addresses to the newly added slaves.
*                 It shall be used after one or more device have been added to the I3C bus, after the bus was initialized.
*                 Only the slaves with no dynamic address will be assigned one.
*
* @implements     I3c_AssignDynamicAddress_Activity
* END**************************************************************************/
I3c_StatusType I3c_AssignDynamicAddress(const uint8 Channel,
                                        const uint8 * AddressList,
                                        const uint8 AddressCount,
                                        I3c_SlaveDeviceType * const SlaveDeviceList)
{
    I3c_StatusType ReturnValue = I3C_STATUS_BUSY;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_ASSIGNDYNAMICADDRESS_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = I3C_STATUS_ERROR;
            I3c_ReportDetError((uint8)I3C_ASSIGNDYNAMICADDRESS_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, NULL_PTR))
            {
                ReturnValue = (I3c_StatusType)I3c_Ip_MasterProcessDAA(Channel, AddressList, AddressCount, SlaveDeviceList);
            }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_PrepareSlaveTransmission
* Description   : This function is used to enable Slave TX interrupt.
*                 This is used to fill the Slave TX FIFO before master request.
*                 Note that before calling this function, I3c_Ip_SlaveSetBuffer() should be called.
* @implements     I3c_PrepareSlaveTransmission_Activity
* END**************************************************************************/
Std_ReturnType I3c_PrepareSlaveTransmission(const uint8 Channel)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_PREPARESLAVETXFIFO_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_PREPARESLAVETXFIFO_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            I3c_Ip_SlavePrepareTxFifo(Channel);
            ReturnValue = (Std_ReturnType)E_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_GetDynamicAddressInfo
* Description   : This function is used to get the slave's dynamic address when it was assigned, re-assigned, or reset.
*                 This may be called from the DA Changed notification.
*
* @implements     I3c_GetDynamicAddressInfo_Activity
* END**************************************************************************/
Std_ReturnType I3c_GetDynamicAddressInfo(const uint8 Channel,
                                         uint8 * DynamicAddress,
                                         I3c_DynamicAddressChangeType * Cause)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;
    I3c_Ip_SlaveDACauseType IpCause;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_GETDYNAMICADDRESSINFO_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_SlaveGetTransferStatus(Channel, NULL_PTR))
        {
            I3c_Ip_SlaveGetDynamicAddressAndCause(Channel, DynamicAddress, &IpCause);
            switch (IpCause)
            {
                case I3C_IP_SLAVE_DA_CAUSE_AUTOMAP:
                    *Cause = I3C_DA_CAUSE_AUTOMAP;
                    break;
                case I3C_IP_SLAVE_DA_CAUSE_RSTDAA:
                    *Cause = I3C_DA_CAUSE_RSTDAA;
                    break;
                case I3C_IP_SLAVE_DA_CAUSE_SET:
                    *Cause = I3C_DA_CAUSE_SETD;
                    break;
                case I3C_IP_SLAVE_DA_CAUSE_ENTDAA:
                    *Cause = I3C_DA_CAUSE_ENTDAA;
                    break;
                default:
                    *Cause = I3C_DA_NOT_CONFIGURED;
                    break;
            }
            ReturnValue = (Std_ReturnType)E_OK;
        }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_GetError
* Description   : This function returns the error register, either for the master or the slave, depending on the configured channel
*
* @implements     I3c_GetError_Activity
* END**************************************************************************/
uint32 I3c_GetError(const uint8 Channel)
{
    uint32 ReturnValue = 0U;
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_GETERROR_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = I3c_Ip_MasterGetError(Channel);
        }
        else
        {
            ReturnValue = I3c_Ip_SlaveGetError(Channel);
        }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

#if (STD_ON == I3C_VERSION_INFO_API)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_GetVersionInfo
* Description   : This function returns the version information of the I3C module.
*
* @implements     I3c_GetVersionInfo_Activity
* END**************************************************************************/
void I3c_GetVersionInfo(Std_VersionInfoType * VersionInfo)
{
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    if (NULL_PTR == VersionInfo)
    {
        I3c_ReportDetError((uint8)I3C_GETVERSIONINFO_ID, (uint8)I3C_E_INVALID_POINTER);
    }
    else
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        (VersionInfo)->vendorID = (uint16)I3C_VENDOR_ID;
        (VersionInfo)->moduleID = (uint16)I3C_MODULE_ID;
        (VersionInfo)->sw_major_version = (uint8)I3C_SW_MAJOR_VERSION;
        (VersionInfo)->sw_minor_version = (uint8)I3C_SW_MINOR_VERSION;
        (VersionInfo)->sw_patch_version = (uint8)I3C_SW_PATCH_VERSION;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
}
#endif /* (STD_ON == I3C_VERSION_INFO_API) */

#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_SlaveRequestEvent
* Description   : This function is used to request an In-Band Interrupt, Hot-Join, or Master Request.
*
* @implements     I3c_SlaveRequestEvent_Activity
* END**************************************************************************/
Std_ReturnType I3c_SlaveRequestEvent(const uint8 Channel,
                                     const I3c_SlaveRequestType Event,
                                     const uint8 IbiData,
                                     const uint8 * ExtData)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_SLAVEREQUESTEVENT_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_SLAVEREQUESTEVENT_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            I3c_Ip_SlaveRequestEvent(Channel, (I3c_Ip_SlaveRequestType)Event, IbiData, ExtData);
            ReturnValue = (Std_ReturnType)E_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_MasterSetBaudRate
* Description   : This function is used to set the baud rate for all subsequent I3C communications.
*                 Using this function, the baud rates will be set as close as possible to the target ones.
*                 After calling this function, I3c_MasterGetBaudRate() shall be called to check which baud rates were actually set.
*
* @implements     I3c_MasterSetBaudRate_Activity
* END**************************************************************************/
Std_ReturnType I3c_MasterSetBaudRate(const uint8 Channel,
                                     const uint32 FunctionalClock,
                                     const I3c_MasterBaudRateType * BaudRates,
                                     const I3c_BusType BusType)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_MASTERSETBAUDRATE_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_MASTERSETBAUDRATE_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, NULL_PTR))
            {
                ReturnValue = (Std_ReturnType)I3c_Ip_MasterSetBaudRate(Channel, FunctionalClock, BaudRates, (I3c_Ip_BusType)BusType);
            }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_MasterGetBaudRate
* Description   : This function is used to get the baud rates of the I3C module
*
* @implements     I3c_MasterGetBaudRate_Activity
* END**************************************************************************/
Std_ReturnType I3c_MasterGetBaudRate(const uint8 Channel,
                                     const uint32 FunctionalClock,
                                     I3c_MasterBaudRateType * BaudRates)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_MASTERGETBAUDRATE_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_MASTERGETBAUDRATE_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            if (I3C_STATUS_BUSY != (I3c_StatusType)I3c_Ip_MasterGetTransferStatus(Channel, NULL_PTR))
            {
                I3c_Ip_MasterGetBaudRate(Channel, FunctionalClock, BaudRates);
                ReturnValue = (Std_ReturnType)E_OK;
            }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
/*FUNCTION**********************************************************************
*
* Function Name : I3c_MasterSetIbiOptions
* Description   : This function is used to set IBI options for the following Slave Requests.
*                 If this function is not used, then default IBI options set in configurator will be used.
*                 Note that for In-Band interrupts with data bytes, this function must be used to set the buffer
*                 in which IBI data bytes will be stored.
*
* @implements     I3c_MasterSetIbiOptions_Activity
* END**************************************************************************/
Std_ReturnType I3c_MasterSetIbiOptions(const uint8 Channel,
                                       const I3c_MasterIbiOptionType * const IbiOptions)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_MASTERSETIBIOPTIONS_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_MASTERSETIBIOPTIONS_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            I3c_Ip_MasterSetIbiOptions(Channel, IbiOptions);
            ReturnValue = (Std_ReturnType)E_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

/*FUNCTION**********************************************************************
*
* Function Name : I3c_MasterManualIbiResponse
* Description   : This function is used by the master to take a decision (ACK/NACK) for an In-Band Interrupt which needs manual intervention.
*                 Note that for Hot-Join and Master-Request, this function must be always called in I3C_IP_MASTER_EVENT_HOT_JOIN and I3C_IP_MASTER_EVENT_MASTER_REQUEST
*                 callbacks.
*
* @implements     I3c_MasterManualIbiResponse_Activity
* END**************************************************************************/
Std_ReturnType I3c_MasterManualIbiResponse(const uint8 Channel,
                                           const I3c_IbiResponseType IbiResponse)
{
    Std_ReturnType ReturnValue = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_MASTERMANUALIBIRESPONSE_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
        if (I3C_MASTER_MODE != I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = (Std_ReturnType)E_NOT_OK;
            I3c_ReportDetError((uint8)I3C_MASTERMANUALIBIRESPONSE_ID, (uint8)I3C_E_INVALID_MODE);
        }
        else
        {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
            I3c_Ip_MasterManualIbiResponse(Channel, (I3c_Ip_IbiResponseType)IbiResponse);
            ReturnValue = (Std_ReturnType)E_OK;
#if (STD_ON == I3C_DEV_ERROR_DETECT)
        }
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS) */

/*FUNCTION**********************************************************************
*
* Function Name : I3c_GetStatus
* Description   : This function returns the status register, either for the master or the slave, depending on the configured channel
*
* @implements     I3c_GetStatus_Activity
* END**************************************************************************/
uint32 I3c_GetStatus(const uint8 Channel)
{
    uint32 ReturnValue = 0U;
    uint8 CoreId;
    CoreId = (uint8)I3c_GetCoreID();

#if (STD_ON == I3C_DEV_ERROR_DETECT)
    Std_ReturnType CheckStatus = I3c_CheckApiCall(CoreId, (uint8)I3C_GETSTATUS_ID);
    if ((Std_ReturnType)E_OK == CheckStatus)
    {
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */
        if (I3C_MASTER_MODE == I3c_apxInitConfig[CoreId]->I3c_HwConfig[Channel]->MasterSlaveModeConfig)
        {
            ReturnValue = I3c_Ip_MasterGetStatus(Channel);
        }
        else
        {
            ReturnValue = I3c_Ip_SlaveGetStatus(Channel);
        }
#if (STD_ON == I3C_DEV_ERROR_DETECT)
    }
#endif /* (STD_ON == I3C_DEV_ERROR_DETECT) */

    return ReturnValue;
}

#define I3C_STOP_SEC_CODE
#include "I3c_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

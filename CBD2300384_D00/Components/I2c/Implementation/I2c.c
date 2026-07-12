/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file    I2c.c
 *        \brief    I2c source file
 *
 *      \details    The I2c driver is responsible for managing the communication within a bus network. It is capable
 *                  of managing the communication within a multi-master network and can handle communication issues
 *                  like de-synchronization.
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
#define I2C_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "I2c.h"
#include "I2c_ConfigMgr.h"
#include "I2c_TransProc.h"
#include "I2c_PerMgr.h"

#if (I2C_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of I2c header file */
#if (  (I2C_CORE_MAJOR_VERSION != (4u)) \
    || (I2C_CORE_MINOR_VERSION != (1u)) \
    || (I2C_CORE_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of I2c.c and I2c.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (I2C_LOCAL) /* COV_I2C_LOCAL */
# define I2C_LOCAL static
#endif

#if !defined (I2C_LOCAL_INLINE) /* COV_I2C_LOCAL */
# define I2C_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define I2C_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Current global component status. */
I2C_LOCAL VAR(I2c_ComponentStatusType, I2C_VAR) I2c_ComponentStatus = I2C_UNINIT;

#define I2C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define I2C_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (I2C_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  I2c_DetCheckDataLength()
 *********************************************************************************************************************/
/*! \brief          Checks if the requested data transmission length is valid.
 *  \details        The length shall not be 0 and it shall not exceed the defined maximum data transmission length.
 *  \param[in]      Length
 *  \return         E_OK:         - The data transmission length is valid.
 *  \return         E_NOT_OK:     - The data transmission length is invalid.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_DetCheckDataLength
(
  I2c_DataLengthType length
);

/**********************************************************************************************************************
 *  I2c_DetCheckTransBuffer()
 *********************************************************************************************************************/
/*! \brief          Checks if at least one of the buffer pointers is a NULL_PTR.
 *  \details        The services SetupEB() and SetupEBDynamic() shall be called with at least one NULL_PTR for one
 *                  of the transmission buffers.
 *  \param[in]      rxBufferPtr
 *  \param[in]      txBufferPtr
 *  \return         E_OK:         - At least one of the transmission buffer pointers is a NULL_PTR.
 *  \return         E_NOT_OK:     - Both transmission buffer pointers are no NULL_PTR.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_DetCheckTransBuffer
(
  I2c_DataConstPtrType txBufferPtr,
  I2c_DataPtrType rxBufferPtr
);
#endif /* I2C_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  I2c_SetupChParameters()
 *********************************************************************************************************************/
/*! \brief          Setup channel-specific parameters for transmission.
 *  \details        This service sets the transfer direction (according to the given buffer pointers) and length for
 *                  a channel.
 *  \param[in]      Pointer to the channel descriptor.
 *  \param[in]      Pointer to txBuffer.
 *  \param[in]      Pointer to rxBuffer.
 *  \param[in]      Length of the buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_SetupChParameters
(
  I2c_ConstChannelDescPtrType constChDescPtr,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
);

/*************************************************************************************************
 * LOCAL FUNCTIONS
 *************************************************************************************************/
#if (I2C_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  I2c_DetCheckDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_DetCheckDataLength
(
  I2c_DataLengthType length
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the length is either 0 or does exceed the configured maximum of data transmission length. */
  if (length == (I2c_DataLengthType)0)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* I2c_DetCheckDataLength */

/**********************************************************************************************************************
 *  I2c_DetCheckTransBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_DetCheckTransBuffer
(
  I2c_DataConstPtrType txBufferPtr,
  I2c_DataPtrType rxBufferPtr /* PRQA S 3673 */ /* MD_I2C_8.13 */
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If both transmission buffer pointers are no NULL_PTR. */
  if ((txBufferPtr != NULL_PTR) && (rxBufferPtr != NULL_PTR))
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* I2c_DetCheckTransBuffer */
#endif /* I2C_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  I2c_SetupChParameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_SetupChParameters
(
  I2c_ConstChannelDescPtrType constChDescPtr,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_VarChannelDescPtrType varChannelDescPtr = constChDescPtr->VarChannelDescPtr;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 Set the requested buffer length. */
  varChannelDescPtr->Length = length; /* SBSW_I2C_PTR_WRITE_N */

  /* #20 If the transmit buffer has been configured. */
  if (txDataBufferPtr != NULL_PTR)
  {
    varChannelDescPtr->TransDir = I2C_WRITE; /* SBSW_I2C_PTR_WRITE_N */
    varChannelDescPtr->BufPtr = (I2c_DataPtrType)txDataBufferPtr; /* PRQA S 0311 */ /* MD_I2C_11.8 */ /* SBSW_I2C_PTR_WRITE_N */
  }
  /* #30 If the receive buffer has been configured. */
  else if (rxDataBufferPtr != NULL_PTR)
  {
    varChannelDescPtr->TransDir = I2C_READ; /* SBSW_I2C_PTR_WRITE_N */
    varChannelDescPtr->BufPtr = rxDataBufferPtr; /* SBSW_I2C_PTR_WRITE_N */
  }
  /* #40 Otherwise no buffer has been configured, the channel's job is a write job. */
  else
  {
    varChannelDescPtr->TransDir = I2C_WRITE; /* SBSW_I2C_PTR_WRITE_N */
    varChannelDescPtr->BufPtr = NULL_PTR; /* SBSW_I2C_PTR_WRITE_N */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_Init
(
  I2c_ConfigPtrType configPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_HwUnitIndexType hwUnitIndex;
  I2c_SequenceType sequenceIndex;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr;
  I2c_ConstSeqDescPtrType seqDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
#if (I2C_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if the module is already initialized. */
  if (I2c_ComponentStatus != I2C_UNINIT)
  {
    errorId = I2C_E_ALREADY_INITIALIZED; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the given configPtr in invalid. */
  else if (configPtr == NULL_PTR)
  {
    errorId = I2C_E_PARAM_CONFIG; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
#endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    I2c_ConfigPtr = configPtr;

    /* #30 Initialize the HwUnits. */
    for (hwUnitIndex = 0; hwUnitIndex < I2c_ConfigPtr->NumOfHwUnits; hwUnitIndex++)
    {
      hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDesc(hwUnitIndex);

      /* #40 Reset the HwUnit's queue parameters. */
      I2c_QueueMgr_ResetQueue(hwUnitDescPtr); /* SBSW_I2C_HW_PTR_PASS_R */
      hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.CancelProcess = FALSE; /* SBSW_I2C_HWREF_WRITE_R_02 */
      hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.SequenceInProcess = FALSE; /* SBSW_I2C_HWREF_WRITE_R_02 */

      /* #50 Set timeout handling default value. */
      I2c_TransProcessor_SetHandling(I2C_BURST);

      /* #60 Call the Peripheral Manager's Init(). */
      I2c_PerMgr_Init(hwUnitDescPtr->HwUnitId);
    }

    /* #70 Set the sequences' result to OK. */
    for(sequenceIndex = 0; sequenceIndex < I2c_ConfigPtr->NumOfSeq; sequenceIndex++)
    {
      seqDescPtr = I2c_ConfigMgr_GetSeqDesc(sequenceIndex);

      seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_OK; /* SBSW_I2C_SEQ_PTR_WRITE_01 */
    }

    /* #80 Set the component's status IDLE. */
    I2c_ComponentStatus = I2C_IDLE;
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
#if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_INIT, errorId);
  }
#else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* I2c_Init() */

/**********************************************************************************************************************
 *  I2c_DeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_DeInit(void)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_HwUnitIndexType hwUnitIndex;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
#if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the module is busy. */
  else if (I2c_ComponentStatus == I2C_BUSY)
  {
    errorId = I2C_E_BUSY; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
#endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    retVal = E_OK;

    /* #30 Set the module's status to UNINIT. */
    I2c_ComponentStatus = I2C_UNINIT;

    /* #40 Call the Peripheral Manager's DeInit() service. */
    for(hwUnitIndex = 0; hwUnitIndex < I2c_ConfigPtr->NumOfHwUnits; hwUnitIndex++)
    {
      I2c_PerMgr_DeInit(hwUnitIndex);
    }

    /* #50 De-initialize the ConfigPtr. */
    I2c_ConfigPtr = NULL_PTR;
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
#if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_DEINIT, errorId);
  }
#else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  return retVal;
} /* I2c_DeInit */

#if (I2C_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_GetVersionInfo
(
  I2c_VersionInfoPtrType versionInfoPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  if (versionInfoPtr == NULL_PTR)
  {
    errorId = I2C_E_PARAM_VINFO_PTR; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    versionInfoPtr->vendorID = (I2C_VENDOR_ID); /* SBSW_I2C_VERSION_INFO_PTR */
    versionInfoPtr->moduleID = (I2C_MODULE_ID); /* SBSW_I2C_VERSION_INFO_PTR */
    versionInfoPtr->sw_major_version = (I2C_SW_MAJOR_VERSION); /* SBSW_I2C_VERSION_INFO_PTR */
    versionInfoPtr->sw_minor_version = (I2C_SW_MINOR_VERSION); /* SBSW_I2C_VERSION_INFO_PTR */
    versionInfoPtr->sw_patch_version = (I2C_SW_PATCH_VERSION); /* SBSW_I2C_VERSION_INFO_PTR */
  }

  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_GET_VERSION_INFO, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
} /* I2c_GetVersionInfo */
#endif

/**********************************************************************************************************************
 *  I2c_SetupEB()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_SetupEB
(
  I2c_ChannelType channelId,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_ConstChannelDescPtrType channelDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the channel ID is valid. */
  else if (channelId >= I2c_ConfigPtr->NumOfCh)
  {
    errorId = I2C_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #30 Check data length. */
  else if (I2c_DetCheckDataLength(length) == E_NOT_OK)
  {
    errorId = I2C_E_PARAM_LENGTH; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #40 Check if both buffer are configured. */
  else if (I2c_DetCheckTransBuffer(txDataBufferPtr, rxDataBufferPtr) == E_NOT_OK) /* SBSW_I2C_BUF_PTR_PASS_N */
  {
    errorId = I2C_E_PARAM_TRANS_BUFFER; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);

    /* #50 Call the local function to set the buffer specific parameters. */
    retVal = E_OK;
    I2c_SetupChParameters(channelDescPtr, txDataBufferPtr, rxDataBufferPtr, length); /* SBSW_I2C_SETUPCH_DOUBLE */
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_SETUP_EB, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return retVal;
} /* I2c_SetupEB */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  I2c_SetupEBDynamic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_SetupEBDynamic
(
  I2c_ChannelType channelId,
  I2c_AddressType address,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_ConstChannelDescPtrType channelDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the channel ID is valid. */
  else if (channelId >= I2c_ConfigPtr->NumOfCh)
  {
    errorId = I2C_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #30 Check if the address is within the 10-bit range. */
  else if (address > I2C_10_BIT_ADDRESS)
  {
    errorId = I2C_E_PARAM_ADDRESS; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #40 Check data length. */
  else if (I2c_DetCheckDataLength(length) == E_NOT_OK)
  {
    errorId = I2C_E_PARAM_LENGTH; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #50 Check if both buffer are configured. */
  else if (I2c_DetCheckTransBuffer(txDataBufferPtr, rxDataBufferPtr) == E_NOT_OK) /* SBSW_I2C_BUF_PTR_PASS_N */
  {
    errorId = I2C_E_PARAM_TRANS_BUFFER; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);
    retVal = E_OK;

    /* #60 Set the requested slave address. */
    channelDescPtr->VarChannelDescPtr->SlaveDevAddr = address; /* SBSW_I2C_CH_PTR_WRITE_02 */

    /* #70 Call the local function to set the buffer specific parameters. */
    I2c_SetupChParameters(channelDescPtr, txDataBufferPtr, rxDataBufferPtr, length); /* SBSW_I2C_SETUPCH_DOUBLE */
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_SETUP_EB_DYNAMIC, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return retVal;
} /* I2c_SetupEBDynamic */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  I2c_AsyncTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_AsyncTransmit
(
  I2c_SequenceType sequenceId
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_ConstSeqDescPtrType seqDescPtr;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the sequence ID is valid. */
  else if (sequenceId >= I2c_ConfigPtr->NumOfSeq)
  {
    errorId = I2C_E_PARAM_SEQUENCE; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */

    seqDescPtr = I2c_ConfigMgr_GetSeqDesc(sequenceId);

    hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);

    I2c_EnterCritical(I2C_EXCLUSIVE_AREA_0);

    I2c_ComponentStatus = I2C_BUSY;

    /* #30 Call the Transition Processor's service to transmit the sequence. */
    retVal = I2c_TransProcessor_RegisterSeq(seqDescPtr); /* SBSW_I2C_PTR_PASS_R */

    /* #40 Call the service to start the HwUnit's transmission process. */
    I2c_TransProcessor_ProcessHwUnit(hwUnitDescPtr); /* SBSW_I2C_HW_PTR_PASS_N_01 */

    I2c_ExitCritical(I2C_EXCLUSIVE_AREA_0);
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_ASYNC_TRANSMIT, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return retVal;
} /* I2c_AsyncTransmit */

#if (I2C_CANCEL_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_Cancel
(
  I2c_SequenceType sequenceId
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_ConstSeqDescPtrType seqDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the sequence ID is valid. */
  else if (sequenceId >= I2c_ConfigPtr->NumOfSeq)
  {
    errorId = I2C_E_PARAM_SEQUENCE; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    seqDescPtr = I2c_ConfigMgr_GetSeqDesc(sequenceId);
    retVal = E_OK;

    /* #30 Call the Transition Processor's service to cancel a sequence's ongoing process. */
    I2c_TransProcessor_CancelSeq(seqDescPtr); /* SBSW_I2C_PTR_PASS_R */
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_CANCEL, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* I2c_Cancel */
#endif /* I2C_CANCEL_API == STD_ON */

/**********************************************************************************************************************
 *  I2c_SetHandling()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, I2C_CODE) I2c_SetHandling
(
  I2c_HandlingType handling
)
{
  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 Call the TransProcessor's SetHandling() service. */
  I2c_TransProcessor_SetHandling(handling);
} /* I2c_SetHandling */

/**********************************************************************************************************************
 * I2c_GetResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(I2c_ChannelResultType, I2C_CODE) I2c_GetResult
(
  I2c_ChannelType channelId
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ChannelResultType retVal;
  I2c_ConstChannelDescPtrType channelDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = I2C_CH_RESULT_OK;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the channel ID is valid. */
  else if (channelId >= I2c_ConfigPtr->NumOfCh)
  {
    errorId = I2C_E_PARAM_CHANNEL; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);

    retVal = channelDescPtr->VarChannelDescPtr->Result;
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_GET_RESULT, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return retVal;
} /* I2c_GetResult */

/**********************************************************************************************************************
 *  I2c_GetSequenceResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(I2c_SeqResultType, I2C_CODE) I2c_GetSequenceResult
(
  I2c_SequenceType sequenceId
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_SeqResultType retVal;
  I2c_ConstSeqDescPtrType seqDescPtr;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  retVal = I2C_SEQ_FAILED;
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  /* #20 Check if the sequence ID is valid. */
  else if (sequenceId >= I2c_ConfigPtr->NumOfSeq)
  {
    errorId = I2C_E_PARAM_SEQUENCE; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    seqDescPtr = I2c_ConfigMgr_GetSeqDesc(sequenceId);

    retVal = seqDescPtr->VarSeqDescPtr->SeqResult;
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_GET_SEQUENCE_RESULT, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  return retVal;
} /* I2c_GetSequenceResult */

#if (I2C_GET_STATUS_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(I2c_ComponentStatusType, I2C_CODE) I2c_GetStatus(void)
{
  /* ----------- Implementation ------------------------------------------------------------------ */
  return I2c_ComponentStatus;
} /* I2c_GetStatus */
#endif

/**********************************************************************************************************************
 *  I2c_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_MainFunction(void)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_HwUnitIndexType hwUnitIndex;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr;
  boolean allHwUnitsIdle = TRUE;
  uint8 errorId = I2C_E_NO_ERROR; /* PRQA S 2983 */ /* MD_I2C_2983 */

  /* ----------- Development Error Checks -------------------------------------------------------- */
# if (I2C_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if the module is uninitialized. */
  if (I2c_ComponentStatus == I2C_UNINIT)
  {
    errorId = I2C_E_UNINIT; /* PRQA S 2983 */ /* MD_I2C_2983 */
  }
  else
# endif
  {
  /* ----------- Implementation ------------------------------------------------------------------ */
    /* #20 Call the Transmission Processor's ProcessHwUnit() service for each HwUnit. */
    for(hwUnitIndex = 0; hwUnitIndex < I2c_ConfigPtr->NumOfHwUnits; hwUnitIndex++)
    {
      hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDesc(hwUnitIndex);

      I2c_TransProcessor_ProcessHwUnit(hwUnitDescPtr); /* SBSW_I2C_HW_PTR_PASS_R */

      /* #30 If the HwUnit is BUSY. */
      if(hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.SequenceInProcess == TRUE)
      {
        allHwUnitsIdle = FALSE;
      }

      /* #40 Call the Peripheral Manager's MainFunction. */
      I2c_PerMgr_MainFunction(hwUnitIndex);
    }

    /* #50 In case all HwUnits are idle, set the components status to idle. */
    if(allHwUnitsIdle == TRUE)
    {
      I2c_ComponentStatus = I2C_IDLE;
    }
  }
  /* ----------- Development Error Report -------------------------------------------------------- */
# if (I2C_DEV_ERROR_REPORT == STD_ON)
  if (errorId != I2C_E_NO_ERROR)
  {
    (void) Det_ReportError(I2C_MODULE_ID, I2C_INSTANCE_ID_DET, I2C_SID_MAIN_FUNCTION, errorId);
  }
# else
  I2C_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
} /* I2c_MainFunction */

#define I2C_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

 MD_I2C_8.13: MISRA 2012 rule 8.13
 Reason:      The data buffer pointer "rxBufferPtr" is not modified within this function and can be of
              type 'pointer to const'.
 Risk:        In this function is no risk. The element, referenced by the pointer, is never modified.
 Prevention:  No prevention necessary. The rxBufferPtr is of the type 'pointer to var' wihtin the
              whole driver. An additional explicit cast to 'pointer to const' is an unnecessary overhead.

 MD_I2C_11.8: MISRA 2012 rule 11.8
 Reason:      A cast shall not remove any const or volatile qualification from the type pointed to by a pointer.
 Risk:        The element, the pointer is pointing to, can be modified after the 'const'
              qualifier has been removed.
 Prevention:  The pointer is only used internally and is not passed to any other module.
              A modification of the element is not performed within the I2c module.

 MD_I2C_3219:
 Reason:      This function is inlined and therefore it has to be implemented here. The function is not used
              by all implementation files which include this header file.
 Risk:        This function may not be used by any implementation file.
 Prevention:  The reviewer has to ensure the each static inline function is used at leased once.

 MD_I2C_1532:
 Reason:      This function is only referenced in one translation unit - but not the one in which it is defined.
 Risk:        There is no risk.
 Prevention:  This function provides access to this logically separated component's sub-module. Therefore this function
              is defined within this translation unit, but only called by other component's sub-modules.

 MD_I2C_2983: MISRA 2012 rule 2.2
 Reason:      The variable 'errorId' is never subsequently used, so the assignment is redundant.
 Risk:        There's no risk.
 Prevention:  There are pre-processed variants in which the 'errorId' is used for error detection. Therefore
              the assignment is necessary.

 MD_I2C_342: Misra Rule 19.13
 Reason:     Glue operator used to abstract AUTOSAR SchM access.
 Risk:       Compiler does not support the glue operator.
 Prevention: Module supports only ANSI Compiler. Integration tests verify that the code compiles.

 MD_I2C_3415:
 Reason:     Right hand operand of '&&' or '||' is an expression with possible side effects.
 Risk:       The function in the right hand operand of '&&' will not be called.
 Prevention: None, it is intentional that the function on the right hand operand will not be called
             if the left hand operand didn't pass.
*/

/*

  The following list describes all verifications of the configuration structure. These justifications
  make sure that every reference starting from the ConfigPtr is valid.

  ID CONFIG_JUSTIFICATION_HW_DESC_LIST
  VERIFICATION \M Verify that the size of the I2c_Cfgx_HardwareDescriptorList[] equals the value of
                  I2c_ConfigPtr->NumOfHwUnits.
                  Verify that each reference within the I2c_Cfgx_HardwareDescriptorList[] is a valid and
                  that each HwUnit ID is within the valid range of HwUnit IDs.

  ID CONFIG_JUSTIFICATION_SEQ_DESC_LIST
  VERIFICATION \M Verify that the size of I2c_Cfgx_SequenceDescriptorList[] equals the value of I2c_ConfigPtr->NumOfSeq.
                  Verify that each reference within the I2c_Cfgx_SequenceDescriptorList[] (VarSeqDescPtr, BusCfgPtr) is
                  valid. Verify that each sequence ID is within the valid range of sequence IDs.

  ID CONFIG_JUSTIFICATION_CH_DESC_LIST
  VERIFICATION \M Verify that the size of the I2c_Cfgx_ChannelDescriptorList[] equals the value of
                  I2c_ConfigPtr->NumOfCh.
                  Verify that each reference within the I2c_Cfgx_ChannelDescriptorList[] is a valid and
                  that each channel ID is within the valid range of channel IDs.

  ID CONFIG_JUSTIFICATION_BUS_DESC_LIST
  VERIFICATION \M Verify that the size of the I2c_Cfgx_BusDescriptorList[] equals the value of
                  I2c_ConfigPtr->NumOfBus.
                  Verify that each reference to I2c_Cfgx_HardwareDescriptorList[] is a valid.

  ID CONFIG_JUSTIFICATION_SEQ_CH_LIST
  VERIFICATION \M Verify that the size of the I2c_SequenceDescriptor's ChannelList equals
                  to the I2c_SequenceDescriptor's NumOfChannels parameter.
                  And that each entry within the ChannelList is a value within the valid range of channel IDs.

Access to configuration structures within the module are performed using the submodule 'I2c_ConfigMgr'.
The sub-module's functions implement boundary checks to ensure that the statically verified boundaries are not violated:

- I2c_ConfigMgr_GetChannelDesc ensures that the return-value points to an element with index < I2c_ConfigPtr->NumOfChl.
- I2c_ConfigMgr_GetHwUnitDesc ensures that the return-value points to an element with index < I2c_ConfigPtr->NumOfHwUnits.
- I2c_ConfigMgr_GetHwUnitDescViaSeq ensures that the return-value is derived from an element with index < I2c_ConfigPtr->NumOfSeq.
  (Validity of I2c_ConstHwUnitDescPtrType is ensured by CONFIG_JUSTIFICATION_SEQ_DESC_LIST / CONFIG_JUSTIFICATION_BUS_DESC_LIST)
- I2c_ConfigMgr_GetHwUnitDesc ensures that the returned pointer points to an element with index < I2c_ConfigPtr->NumOfHwUnits.

If an invalid index parameter is passed, NULL_PTR is returned.

*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_I2C_VERSION_INFO_PTR
  \DESCRIPTION The versionInfoPtr is accessed an written.
  \COUNTERMEASURE \N The function caller has to ensure, that the pointer is valid.
                    A DET check ensures, that the pointer is no NULL_PTR.

  \ID SBSW_I2C_PTR_WRITE_N
  \DESCRIPTION The element, this pointer is pointing to, is accessed and written.
  \COUNTERMEASURE \N The function caller ensures, that the pointer is valid. The pointer is initially received
                     from the ConfigManager and has been checked to be no NULL_PTR.

  \ID SBSW_I2C_PTR_PASS_N
  \DESCRIPTION A pointer is forwarded to a function as parameter. The pointer is initially received
               from the ConfigManager and has been checked to be valid.
  \COUNTERMEASURE \N The function caller ensures, that the pointer is valid.

  \ID SBSW_I2C_PTR_PASS_R
  \DESCRIPTION A pointer is passed to a function as parameter.
  \COUNTERMEASURE \R If DET is enabled, the passed ID that is used to receive the pointer
                  is checked to be valid.

  \ID SBSW_I2C_SEQ_PTR_WRITE_01
  \DESCRIPTION The element, the varSeqDescPtr is pointing to, is accessed and written. The pointer is referenced by the
               seqDescPtr which is received from the ConfigManager which checks the requested index for
               being within the valid range.
  \COUNTERMEASURE \R The for-loop ensures that the requested index, passed to the ConfigManager,
                  is smaller than I2c_ConfigPtr->NumOfSeq.

  \ID SBSW_I2C_SEQ_PTR_WRITE_02
  \DESCRIPTION The element, the varSeqDescPtr is pointing to, is accessed and written. The pointer is referenced by the
               seqDescPtr.
  \COUNTERMEASURE \R The seqDescPtr is checked to be no NULL_PTR.

  \ID SBSW_I2C_SEQ_PTR_WRITE_03
  \DESCRIPTION The element, the varSeqDescPtr is pointing to, is accessed and written. The pointer is referenced by the
               seqDescPtr which is received from the ConfigManager.
  \COUNTERMEASURE \N The index, passed to the ConfigManager, is the sequence ID, stored as the
                  'currently processed sequence'.
                  This parameter is only written within the I2c_TransProcessor_InitSeqTransmission() service with a
                  valid sequence ID (See SBSW_I2C_PTR_PASS_R above).

  \ID SBSW_I2C_SEQ_PTR_WRITE_04
  \DESCRIPTION The element, the varSeqDescPtr is pointing to, is accessed and written. The pointer is referenced by the
               seqDescPtr.
  \COUNTERMEASURE \N The seqDescPtr is initially received from the ConfigManager and is in each case checked to be valid.

  \ID SBSW_I2C_SEQ_PTR_WRITE_05
  \DESCRIPTION The element, the varSeqDescPtr is pointing to, is accessed and written. The pointer is referenced by the
               seqDescPtr which is initially received from the ConfigManager
               and has been checked for not being a NULL_PTR.
  \COUNTERMEASURE \N The caller has to make sure, that the pointer is valid.

  \ID SBSW_I2C_SEQ_FUNC_CALL
  \DESCRIPTION Function pointer call of the seqDescPtr. The seqDescPtr is initially received from the ConfigManager.
  \COUNTERMEASURE \N The index, passed to the ConfigManager, is the sequence ID,
                  stored as the 'currently processed sequence'.
                     This parameter is only written within the I2c_TransProcessor_InitSeqTransmission() service with a
                     valid sequence ID (See CONFIG_JUSTIFICATION above).

  \ID SBSW_I2C_SEQ_PTR_PASS_01
  \DESCRIPTION The seqDescPtr is passed to a function. The pointer is initially received from the ConfigManager.
  \COUNTERMEASURE \N The index, passed to the ConfigManager, is the sequence ID, stored as the 'currently processed sequence'.
                     This parameter is only written within the I2c_TransProcessor_InitSeqTransmission() service with a
                     valid sequence ID (See CONFIG_JUSTIFICATION above).

  \ID SBSW_I2C_SEQ_PTR_PASS_02
  \DESCRIPTION The seqDescPtr is passed to a function. The pointer is initially received from the ConfigManager and has
               been checked to be no NULL_PTR.
  \COUNTERMEASURE \N The caller has to make sure, that the pointer is valid.

  \ID SBSW_I2C_CH_PTR_WRITE_01
  \DESCRIPTION The element, the varChannelDescPtr is pointing to, is accessed and written.
               The varChDescPtr is referenced by the constChDescPtr
               which is received from the ConfigManager. The ConfigManager ensures,
               that the requested channel index is within the valid range.
  \COUNTERMEASURE \R The for-loop ensures that the requested index,
                  passed to the ConfigManager, is smaller than I2c_ConfigPtr->NumOfCh.

  \ID SBSW_I2C_CH_PTR_WRITE_02
  \DESCRIPTION The element, the varChannelDescPtr is pointing to, is accessed and written.
               The varChDescPtr is referenced by the constChDescPtr
               which is received from the ConfigManager. The ConfigManager ensures,
               that the requested channel index is within the valid range.
  \COUNTERMEASURE \R The pointer is checked to be no NULL_PTR.

  \ID SBSW_I2C_CH_PTR_WRITE_03
  \DESCRIPTION The element, the varChannelDescPtr is pointing to, is accessed and written.
               The varChDescPtr is referenced by the constChDescPtr
               which is received from the ConfigManager.
               The ConfigManager ensures, that the requested channel index is within the valid range.
  \COUNTERMEASURE \R The index, passed to the ConfigManager, is received from the sequence's channelList,
                  which contains only valid entries (See CONFIG_JUSTIFICATION above).
                  The for-loop ensures that the channelList is only accessed at valid entries.

  \ID SBSW_I2C_CH_PTR_WRITE_04
  \DESCRIPTION The element, the varChannelDescPtr is pointing to, is accessed and written.
               The varChDescPtr is referenced by the constChDescPtr which is received from the ConfigManager.
               The ConfigManager ensures, that the requested channel index is within the valid range.
  \COUNTERMEASURE \R The index, passed to the ConfigManager, is received from the sequence's channelList,
                  which contains only valid entries (See CONFIG_JUSTIFICATION above).
                  The index to access the sequence's channelList is checked to be smaller than the channelList's size.

  \ID SBSW_I2C_CH_PTR_WRITE_05
  \DESCRIPTION The element, the 'varChannelDescPtr' is pointing to, is accessed and written.
               The 'varChDescPtr' is referenced by the constChDescPtr which is received from the ConfigManager.
               The ConfigManager ensures, that the requested 'channelIndex' is within the valid range.
               The 'channelIndex' shall always be smaller than 'seqDescPtr->NumOfChannels' to be valid.
  \COUNTERMEASURE \R The sequence's 'LeadingChannel' and the 'NumOfChainedChs' are only written
                  in the 'I2c_TransProcessor_ProcessChannels()' service.
                  The 'LeadingChannel' receives its value from the 'NextChannel',
                  which is checked to be smaller than 'seqDescPtr->NumOfChannels'.
                  The value of 'numOfChainedChs' is limited by the 'NextChannel' value,
                  because 'numOfChainedChs' can only be incremented
                  as long as the 'NextChannel' is smaller than 'seqDescPtr->NumOfChannels'.
                  In case 'NextChannel' is 'seqDescPtr->NumOfChannels - 1',
                  the 'numOfChainedChs's value can only be '1'. In this case the maximum of
                  ('LeadingChannel' + 'numOfChainedChs') within the 'I2c_TransProcessor_ReportResult()' service is
                  'seqDescPtr->NumOfChannels'. Thus the 'channelIndex', passed to the ConfigManager,
                  will never exceed the size of the sequence's channelList.

  \ID SBSW_I2C_CH_PTR_PASS
  \DESCRIPTION The channelDescPtr is forwarded to a function. The pointer is initially received from the ConfigManager.
  \COUNTERMEASURE \R The index, passed to the ConfigManager, is received from the sequence's channelList,
                  which contains only valid entries (See CONFIG_JUSTIFICATION above).
                  The index to access the sequence's channelList is checked to be smaller than the channelList's size.

  \ID SBSW_I2C_TRANSBUF_ARRAY_R_01
  \DESCRIPTION Write access on the TransBufferDesc[] array.
  \COUNTERMEASURE \R The for-loop ensures, that the channelIndex does not exceed the size of the TransBufferDesc[].

  \ID SBSW_I2C_TRANSBUF_ARRAY_R_02
  \DESCRIPTION Write access on the TransBufferDesc[] array.
  \COUNTERMEASURE \R The modulo operator ensures that the TransBufferDesc[] is only accessed at valid entries.

  \ID SBSW_I2C_BUF_PTR_PASS_N
  \DESCRIPTION The buffer pointer txDataBufferPtr and rxDataBufferPtr are forwarded to a local function.
  \COUNTERMEASURE \N Within the I2c's context both pointers are allowed to be NULL_PTRs.

  \ID SBSW_I2C_ARRAY_MOD_R
  \DESCRIPTION An array is accessed and written.
  \COUNTERMEASURE \R The modulo operator ensures that there is no write access at an invalid entry.

  \ID SBSW_I2C_HWREF_WRITE_N_01
  \DESCRIPTION A pointer is accessed and written. The pointer is referenced by the hwUnitDescPtr.
  \COUNTERMEASURE \N The hwUnitDescPtr is initially received from the ConfigManager via a valid sequence ID
                     (See CONFIG_JUSTIFICATION above).

  \ID SBSW_I2C_HWREF_WRITE_R_02
  \DESCRIPTION A pointer is accessed and written. The pointer is referenced by the hwUnitDescPtr which is received
               from the ConfigManager.
  \COUNTERMEASURE \R The for-loop ensures, that the index, passed to the ConfigManager,
                  is smaller than I2c_ConfigPtr->NumOfHwUnits.

  \ID SBSW_I2C_HWREF_WRITE_R_03
  \DESCRIPTION A pointer is accessed and written. The pointer is referenced by the hwUnitDescPtr
               which is initially received from the ConfigManager.
  \COUNTERMEASURE \N There are several locations, where the hwUnitDescPtr
                  can be initially received from the ConfigManager and has been checked to be valid.

  \ID SBSW_I2C_HWREF_PASS_R_01
  \DESCRIPTION A pointer is passed to a function. The pointer is referenced by the hwUnitDescPtr.
  \COUNTERMEASURE \N The hwUnitDescPtr is initially received from the ConfigManager via a valid sequence ID
                     (See CONFIG_JUSTIFICATION above).

  \ID SBSW_I2C_HWREF_PASS_R_02
  \DESCRIPTION A pointer is passed to a function. The pointer is referenced by
               the hwUnitDescPtr which is initially received from the ConfigManager.
  \COUNTERMEASURE \N There are several locations, where the hwUnitDescPtr
                  can be initially received from the ConfigManager and has been checked to be valid.

  \ID SBSW_I2C_HW_PTR_PASS_N_01
  \DESCRIPTION The hwUnitDescPtr is passed to a function.
  \COUNTERMEASURE \N The hwUnitDescPtr is initially received from the ConfigManager via the passed sequence ID.
                  This sequence ID is checked if the DET is enabled.

  \ID SBSW_I2C_HW_PTR_PASS_N_02
  \DESCRIPTION The hwUnitDescPtr is passed to a function. This pointer is initially received from the ConfigManager.
  \COUNTERMEASURE \N There are several locations, where the hwUnitDescPtr
                  can be initially received from the ConfigManager and has been checked to be valid.

  \ID SBSW_I2C_HW_PTR_PASS_R
  \DESCRIPTION The hwUnitDescPtr is passed to a function. The hwUnitDescPtr is received from the ConfigManager.
  \COUNTERMEASURE \R The for-loop ensures, that the index, passed to the ConfigManager,
                     is smaller than I2c_ConfigPtr->NumOfHwUnits.

  \ID SBSW_I2C_ARRAY_ENDINDEX
  \DESCRIPTION Write access on the PrioQueue array.
  \COUNTERMEASURE \N The EndIndex, which is used to access the array,
                  is only modified within the I2c_QueueMgr_IncrementEndIndex() service.
                  The service ensures, that the EndIndex is smaller than the define I2C_PRIO_QUEUE_BUFFER_SIZE.

  \ID SBSW_I2C_SETUPCH_DOUBLE
  \DESCRIPTION A channelDescPtr is passed to a function and a txDataBufferPtr
               and a rxDataBufferPtr are forwarded to a function.
  \COUNTERMEASURE \R [SBSW_I2C_PTR_PASS_R] [SBSW_I2C_BUF_PTR_PASS_N]

  \ID SBSW_I2C_HW_CH_DOUBLE
  \DESCRIPTION A hwUnitDescPtr is forwarded to a function and a channelDescPtr is passed to a function.
  \COUNTERMEASURE \R [SBSW_I2C_PTR_PASS_N] [SBSW_I2C_CH_PTR_PASS]

 SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION
  \ID COV_I2C_LOCAL
  \ACCEPT XF
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]
  END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: I2c.c
 *********************************************************************************************************************/

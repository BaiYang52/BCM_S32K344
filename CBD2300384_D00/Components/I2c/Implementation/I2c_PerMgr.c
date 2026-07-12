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
/*        \file  I2c_PerMgr.c
 *        \brief  I2c peripheral manager
 *
 *      \details  This sub-module controls the specific I2c peripherals of the target platform. It takes requests for
 *                simple data transfers from the higher layers and executes them on the specific hardware. The result of
 *                a data transfer which depends on successful transmission or a detected error event is reported back to
 *                the layers above. In addition, the module offers mechanisms for the handling of error states to the
 *                layers above.
 *
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
#include "I2c_Regs.h"
#include "I2c_PerMgr.h"
#include "I2c_TransProc.h"
#include "I2c_PerMgr_RegAccess_Int.h"
#include "I2c_ConfigMgr.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of the configuration header file */
#if (  (I2C_CFG_MAJOR_VERSION != (2u)) \
    || (I2C_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of I2c.c and I2c_Cfg.h are inconsistent!"
#endif

/* Check the version of I2c_PerMgr header file */
#if (  (I2C_SW_MAJOR_VERSION != (2u)) \
    || (I2C_SW_MINOR_VERSION != (0u)) \
    || (I2C_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of I2c_PerMgr.c and I2c_PerMgr.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define I2C_ADDR_BITS_OFFSET     (0x01u) /*!< Offset of address bits in I2c header frame (LSH of 1). */
#define I2C_BYTE_MASK            (0xFFu) /*!< Selects a byte */
#define I2C_MAX7BIT_ADDR         (0x7Fu) /*!< Maximum address which can be addressed with 7bit addressing. */
#define I2C_SHIFT_BYTE_UP2LOW    (8u)    /*!< Shift upper byte to lower byte (8bit LSH). */
#define I2C_EX_ADDRESS_ID        (0xF0u) /*!< Special purpose address which marks Extended addressing. */
#define I2C_FREEBUS_CLK_EDGES    (18u)   /*!< No. of clock edges to generate 9 clock pulses to free the bus. */
#define I2C_MAX_TX_RX_BUFFER     (3u)    /*!< Maximum size of the RX and TX FIFO buffers. */
#define I2C_FREEBUS_IDLE_TICKS   (50u)   /*!< Value to adjust busy waiting time between clock pulses to free the bus. */
#define I2C_MAX_RX_BYTES         (256u)  /*!< Maximum number of RX bytes for one reception. Value is given by the data sheet. */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 I2c_LocalFunctionMacros_Tag */ /* MISRA2012-Dir4.9 */
/*! Macros to calculate address bytes. */
#define I2c_GetStdAddrBits(addr)       (uint8)((addr)  << (I2C_ADDR_BITS_OFFSET))
#define I2c_GetExtAddrBitsFirst(addr)  (uint8)(I2C_EX_ADDRESS_ID|I2c_GetStdAddrBits(((addr) >> I2C_SHIFT_BYTE_UP2LOW)))
#define I2c_GetExtAddrBitsSecond(addr) (uint8)((addr) & I2C_BYTE_MASK)
/* PRQA L:I2c_LocalFunctionMacros_Tag */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (I2C_LOCAL) /* COV_I2C_COMPATIBILITY */
# define I2C_LOCAL static
#endif

#if !defined (I2C_LOCAL_INLINE) /* COV_I2C_COMPATIBILITY */
# define I2C_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define I2C_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  I2c_PerMgr_StartTrans()
 *********************************************************************************************************************/
/*! \brief      Start a transmission.
 *  \details    -
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    TASK
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_StartTrans(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  I2c_PerMgr_StopTrans()
 *********************************************************************************************************************/
/*! \brief      Stop a transmission.
 *  \details    -
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    ISR
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_StopTrans(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  I2c_PerMgr_TxData()
 *********************************************************************************************************************/
/*! \brief      Transmits data to slave.
 *  \details    This function handles the transmission of data from a buffer to a slave device.
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    ISR
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_TxData(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  I2c_PerMgr_RxData()
 *********************************************************************************************************************/
/*! \brief      Receives data from slave.
 *  \details    This function handles the reception of data from a slave device to a buffer.
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    ISR
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_RxData(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  I2c_PerMgr_TxHeader()
 *********************************************************************************************************************/
/*! \brief      Transmits the TX header to slave.
 *  \details    -
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    TASK
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_TxHeader(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  I2c_PerMgr_RxHeader()
 *********************************************************************************************************************/
/*! \brief      Transmits the RX header to slave.
 *  \details    -
 *  \param[in]  hwUnitId           Index of hardware unit.
 *  \context    TASK
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_RxHeader(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 I2c_PerMgr_BuildHeaderFrames()
 **********************************************************************************************************************/
/*! \brief      Assembles the header frames.
 *  \details    Depending on the addressing type (7-/10-bit) and transfer direction, this function builds the
 *              corresponding header frames and saves them for subsequent transmission.
 *  \param[in]  hwUnitId           Index of hardware unit used to store the header frames.
 *  \param[in]  addr               Slave device address for address frame.
 *  \param[in]  dir                Sets the transmission direction (READ/WRITE).
 *  \context    TASK
 *  \pre        -
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_BuildHeaderFrames(I2c_HwUnitType hwUnitId, I2c_AddressType addr,
    I2c_TransDirectionType dir);

/**********************************************************************************************************************
   I2c_PerMgr_IsBusReady()
 **********************************************************************************************************************/
/*! \brief      Check if the bus status is either free or in "master busy" mode.
 *  \details    In case of a repeated start the bus status must be "master busy".
 *  \param[in]  hwUnitId            Id of the hardware unit.
 *  \return     TRUE                The bus is ready.
 *  \return     FALSE               The bus is not ready.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(boolean, I2C_CODE) I2c_PerMgr_IsBusReady(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
   I2c_PerMgr_FinishTrans()
 **********************************************************************************************************************/
/*! \brief      This function performs steps to finalize/finish the transmission.
 *  \details    Disables interrupts and sets results and flags.
 *  \param[in]  hwUnitId            Id of the hardware unit.
 *  \pre        -
 *  \context    ISR
 *  \reentrant  FALSE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_FinishTrans(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  I2c_PerMgr_FinishTrans()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_FinishTrans(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ------ Implementation ----------------------------------------------------------------------------------------- */
  /* #10 Disable and clear interrupts. */
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MIER, I2C_REG_MASK_MIER_DISABLE_ALL);
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_ALL_FLAGS);

  /* #20 Report result and set flags. */
  I2c_TransProcessor_ReportResult(hwUnitId, hwRuntimeData->Result);
  hwRuntimeData->State = IDLE;
  transDataPtr->RepeatedStart = FALSE;

} /* I2c_PerMgr_FinishTrans */

/**********************************************************************************************************************
 *  I2c_PerMgr_IsBusReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(boolean, I2C_CODE) I2c_PerMgr_IsBusReady(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------------------------------------------------- */
  boolean isBusReady = FALSE;
  uint16 loopCount = 0;
  I2c_ConstTransDataPtrType transData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);
  I2c_PerMgr_RegWidthType tmpStatusReg;

  /* ------ Implementation ----------------------------------------------------------------------------------------- */
  /* #10 Wait configured time until bus is ready. */
  while((loopCount < I2C_HW_LOOP_MAX) && (isBusReady == FALSE))
  {
    tmpStatusReg = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MSR);

    /* #20 If Repeated Start is set and the bus status is in "master busy" mode return TRUE. */
    if((transData->RepeatedStart == TRUE) && ((tmpStatusReg & I2C_REG_MASK_MSR_MBF) == I2C_REG_MASK_MSR_MBF))
    {
      isBusReady = TRUE;
    }
    /* #30 Or if the bus is free return TRUE. */
    else if((tmpStatusReg & I2C_REG_MASK_MSR_BBF) == 0u)
    {
      isBusReady = TRUE;
    }
    else
    {
      isBusReady = FALSE;
    }

    loopCount++;
  }

  return isBusReady;

} /* I2c_PerMgr_IsBusReady */

/**********************************************************************************************************************
 *  I2c_PerMgr_BuildHeaderFrames()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_BuildHeaderFrames(I2c_HwUnitType hwUnitId, I2c_AddressType addr,
    I2c_TransDirectionType dir)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);

  /* ----- Implementation ----------------------------------------------- */
  hwRuntimeData->HeaderFrameBufferSize = 0;
  hwRuntimeData->HeaderFrameBufferIdx = 0;

  /* #10 Check address length */
  if (addr > I2C_MAX7BIT_ADDR)
  {
    /* #20 Create extended header frame for TX */
    if (dir == I2C_WRITE)
    {
      hwRuntimeData->HeaderFrameBuffer[0] = I2c_GetExtAddrBitsFirst(addr);
      hwRuntimeData->HeaderFrameBuffer[1] = I2c_GetExtAddrBitsSecond(addr);
      hwRuntimeData->HeaderFrameBufferSize = 2;
    }
    /* #30 Create extended header frame for RX */
    else
    {
      hwRuntimeData->HeaderFrameBuffer[0] = I2c_GetExtAddrBitsFirst(addr);
      hwRuntimeData->HeaderFrameBuffer[1] = I2c_GetExtAddrBitsSecond(addr);
      hwRuntimeData->HeaderFrameBuffer[2] = I2c_GetExtAddrBitsFirst(addr) | (uint8) I2C_READ;
      hwRuntimeData->HeaderFrameBufferSize = 3;
    }
  }
  /* #40 Create standard header frame for TX or RX */
  else
  {
    hwRuntimeData->HeaderFrameBuffer[0] = I2c_GetStdAddrBits(addr) | (uint8) dir;
    hwRuntimeData->HeaderFrameBufferSize = 1;
  }

} /* I2c_PerMgr_BuildHeaderFrames() */

/**********************************************************************************************************************
 *  I2c_PerMgr_StartTrans()
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
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_StartTrans(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);
  I2c_ChannelIndexType channelIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Build and store address frames. */
  I2c_PerMgr_BuildHeaderFrames(hwUnitId, transDataPtr->SlaveAddress, transDataPtr->TransDir);

  /* #20 Reset FIFOs and flags. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RTF);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RRF);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_TDF);

  /* #30 Check if bus is ready. */
  if(I2c_PerMgr_IsBusReady(hwUnitId) == TRUE)
  {
    if(transDataPtr->TransDir == I2C_WRITE)
    {
      /* #40 Send header and enable interrupts for TX. */
      I2c_PerMgr_TxHeader(hwUnitId);
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MIER, I2C_REG_MASK_MIER_TDIE |
            I2C_REG_MASK_MIER_ALF | I2C_REG_MASK_MIER_NDIE | I2C_REG_MASK_MIER_SDIE | I2C_REG_MASK_MIER_PLTIE |
            I2C_REG_MASK_MIER_FEIE); /* The description of FEIE in the data sheet is wrong. "1" means enabled. Already reported to NXP (2021-07-22). */
    }
    else
    {
      /* #45 Get the data lengths of all chained channels and add them up. */
      for(channelIndex = 0u; channelIndex < transDataPtr->NumOfChannels; channelIndex++)
      {
        transDataPtr->RemainingTotalLength += (transDataPtr->TransBufferDesc[channelIndex].DataLength);
      }

      /* #50 Send header and enable interrupts for RX. */
      I2c_PerMgr_RxHeader(hwUnitId);
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MIER, I2C_REG_MASK_MIER_RDIE |
            I2C_REG_MASK_MIER_ALF | I2C_REG_MASK_MIER_NDIE | I2C_REG_MASK_MIER_SDIE | I2C_REG_MASK_MIER_PLTIE |
            I2C_REG_MASK_MIER_FEIE); /* The description of FEIE in the data sheet is wrong. "1" means enabled. Already reported to NXP (2021-07-22). */
    }
  }
  /* #60 Otherwise, bus is not ready and report result. */
  else
  {
    hwRuntimeData->State = IDLE;
    I2c_TransProcessor_ReportResult(hwUnitId, I2C_CH_RESULT_BUSFAIL);
  }
} /* I2c_PerMgr_StartTrans() */

/**********************************************************************************************************************
 *  I2c_PerMgr_TxHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_TxHeader(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_ConstVarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 First header byte requires a START condition. */
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR | hwRuntimeData->HeaderFrameBuffer[0]);

  /* #20 If extended address is used. */
  if(hwRuntimeData->HeaderFrameBufferSize == 2u)
  {
    /* #30 Transmit second address byte. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_TRANSMIT | hwRuntimeData->HeaderFrameBuffer[1]); /* PRQA S 2986 */ /* MD_I2c_PerMgr_2986 */
  }
} /* I2c_PerMgr_TxHeader() */

/**********************************************************************************************************************
 *  I2c_PerMgr_RxHeader()
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
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_RxHeader(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_ConstVarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_ConstTransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If extended header is used */
  if(transDataPtr->SlaveAddress > I2C_MAX7BIT_ADDR)
  {
    /* #20 Transmit header for extended master reception. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR | hwRuntimeData->HeaderFrameBuffer[0]);
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_TRANSMIT | hwRuntimeData->HeaderFrameBuffer[1]); /* PRQA S 2986 */ /* MD_I2c_PerMgr_2986 */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR | hwRuntimeData->HeaderFrameBuffer[2]);
  }
  /* #30 Otherwise */
  else
  {
    /* #40 Transmit header or normal data reception. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR | hwRuntimeData->HeaderFrameBuffer[0]);
  }

  /* #50 Request data from slave. */
  if(transDataPtr->RemainingTotalLength > I2C_MAX_RX_BYTES)
  {
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_RECEIVE | (I2C_MAX_RX_BYTES - 1u));
  }
  else
  {
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_RECEIVE | (transDataPtr->RemainingTotalLength - 1u));
  }
} /* I2c_PerMgr_RxHeader() */

/**********************************************************************************************************************
 *  I2c_PerMgr_TxData()
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
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_TxData(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_ConstTransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);
  I2c_ConstTransBufferDescPtrType curTransBufferDesc = &transDataPtr->TransBufferDesc[hwRuntimeData->BufDescrIdx];
  I2c_DataLengthType remainingBytes;
  I2c_DataLengthType txElements;

  /* ----- Implementation ----------------------------------------------- */
  remainingBytes = curTransBufferDesc->DataLength - hwRuntimeData->CurBufIdx;

  /* #10 If remaining bytes are left, continue sending data. */
  if (remainingBytes > 0u)
  {
    /* #20 Adapt TX FIFO watermark. */
    if (remainingBytes > I2C_MAX_TX_RX_BUFFER)
    {
      txElements = I2C_MAX_TX_RX_BUFFER;
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MFCR, (I2c_PerMgr_RegWidthType)txElements);
    }
    else
    {
      txElements = remainingBytes;
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MFCR, txElements - 1uL);
    }
    /* #30 Transmit data by filling TX buffer. */
    while(txElements > 0u)
    {
      if (curTransBufferDesc->SingleValue == TRUE)
      {
        I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, curTransBufferDesc->BufPtr[0u]);
      }
      else
      {
        I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, curTransBufferDesc->BufPtr[hwRuntimeData->CurBufIdx]);
      }

      hwRuntimeData->CurBufIdx++;
      txElements--;
    }

    /* #40 Reset TX FIFO watermark. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MFCR, 0u);

    /*  #50 If buffer is full and a further descriptor available
     *  reset buffer index and increment descriptor index.
     */
    if ((hwRuntimeData->CurBufIdx == curTransBufferDesc->DataLength)
        && (((hwRuntimeData->BufDescrIdx) + 1u) < (transDataPtr->NumOfChannels)))
    {
      hwRuntimeData->CurBufIdx = 0u;
      hwRuntimeData->BufDescrIdx++;
    }
  }
  else
  {
    /* #60 If the current channel is the last channel of buffer. */
    if (((hwRuntimeData->BufDescrIdx) + 1u) == (transDataPtr->NumOfChannels))
    {
      /* #70 Stop transmission. */
      I2c_PerMgr_StopTrans(hwUnitId);
    }
  }
} /* I2c_PerMgr_TxData() */


/**********************************************************************************************************************
 *  I2c_PerMgr_RxData()
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
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_RxData(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);
  I2c_ConstTransBufferDescPtrType curTransBufferDesc = &transDataPtr->TransBufferDesc[hwRuntimeData->BufDescrIdx];
  I2c_DataLengthType remainingBytes;
  I2c_DataLengthType remainingBytesPerTransmission;
  I2c_DataLengthType dynamicRxWatermark;
  I2c_PerMgr_RegWidthType tmpRxData;

  /* ----- Implementation ----------------------------------------------- */
  /*
   * The reading from the RX FIFO register must not be interruptible.
   * Because after reading the register, the next byte is automatically set in the register.
   * The Exclusive Area ensures that no other interrupt can occur during reading.
   * Otherwise, the register could be accessed twice and the first byte get lost.
   */
  /* #10 Get first data from RX FIFO. */
  SchM_Enter_I2c_I2C_EXCLUSIVE_AREA_0();
  tmpRxData = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MRDR);
  SchM_Exit_I2c_I2C_EXCLUSIVE_AREA_0();

  /* #20 While RX FIFO is not empty. */
  while((tmpRxData & I2C_REG_MASK_MRDR_RXEMPTY) == 0u)
  {
    if (hwRuntimeData->CurBufIdx < curTransBufferDesc->DataLength)
    {
      /* #30 Assign data to buffer. */
      curTransBufferDesc->BufPtr[hwRuntimeData->CurBufIdx] = (I2c_DataType)(tmpRxData & 0xFFu);
      hwRuntimeData->CurBufIdx++;
      transDataPtr->ReceivedBytes++;
    }

    /*  #40 If buffer is full and a further descriptor available
     *  reset buffer index, increment descriptor index and get next buffer.
     */
    if ((hwRuntimeData->CurBufIdx == curTransBufferDesc->DataLength)
        && (((hwRuntimeData->BufDescrIdx) + 1u) < (transDataPtr->NumOfChannels)))
    {
      hwRuntimeData->CurBufIdx = 0;
      hwRuntimeData->BufDescrIdx++;
      curTransBufferDesc = &transDataPtr->TransBufferDesc[hwRuntimeData->BufDescrIdx];
    }

    /* #50 Read further data from RX FIFO */
    SchM_Enter_I2c_I2C_EXCLUSIVE_AREA_0();
    tmpRxData = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MRDR);
    SchM_Exit_I2c_I2C_EXCLUSIVE_AREA_0();
  }

  remainingBytes = curTransBufferDesc->DataLength - hwRuntimeData->CurBufIdx;
  remainingBytesPerTransmission = I2C_MAX_RX_BYTES - transDataPtr->ReceivedBytes;

  dynamicRxWatermark = (remainingBytes > remainingBytesPerTransmission) ? remainingBytesPerTransmission : remainingBytes;

  /* #60 Adapt RX FIFO watermark for the next RX interrupt. */
  if (dynamicRxWatermark > 0u)
  {
    if(dynamicRxWatermark > I2C_MAX_TX_RX_BUFFER)
    {
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MFCR, (I2c_PerMgr_RegWidthType)I2C_MAX_TX_RX_BUFFER << 16u);
    }
    else
    {
      I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MFCR, (dynamicRxWatermark - 1uL) << 16u);
    }
  }
  else
  {
    I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MFCR, (I2c_PerMgr_RegWidthType)I2C_MAX_TX_RX_BUFFER << 16u);
  }

  /* #70 If no more bytes left for the current channel and last channel of transBuffer.*/
  if ((((hwRuntimeData->BufDescrIdx) + 1u) == (transDataPtr->NumOfChannels)) && (remainingBytes == 0u))
  {
    /* #80 Stop transmission. */
    I2c_PerMgr_StopTrans(hwUnitId);
  }

  /* #90 If the maximum number of RX bytes were received, start the next transmission by sending a new RX header. */
  if(transDataPtr->ReceivedBytes == I2C_MAX_RX_BYTES)
  {
    transDataPtr->ReceivedBytes = 0u;

    /* The result of the next calculation can not be negative, because RxData function is only called after the RxHeader function,
     * which checks the RemainingTotalLength is greater than I2C_MAX_RX_BYTES
     */
    transDataPtr->RemainingTotalLength -= I2C_MAX_RX_BYTES;
    I2c_PerMgr_RxHeader(hwUnitId);
  }
} /* I2c_PerMgr_RxData() */

/**********************************************************************************************************************
 *  I2c_PerMgr_StopTrans()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_PerMgr_StopTrans(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If Hold Bus is enabled. */
  if (transDataPtr->HoldBus == TRUE)
  {
    /* #20 Disable interrupts and set flags for next transmission. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MIER, I2C_REG_MASK_MIER_DISABLE_ALL);
    hwRuntimeData->State = IDLE;
    hwRuntimeData->ReportResult = TRUE;
    hwRuntimeData->Result = I2C_CH_RESULT_OK;
    transDataPtr->RepeatedStart = TRUE;
  }
  /* #30 Otherwise. */
  else
  {
    /* #40 Generate STOP condition. */
    I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MTDR, I2C_REG_MASK_MTDR_CMD_GEN_STOP);
  }

} /* I2c_PerMgr_StopTrans() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  I2c_PerMgr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_Init(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwCfgPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId]);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enable I2C peripheral. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

  /*
   * Enable the debug mode. This is necessary for debugging the code. If the debug mode is disabled and
   * the application will be downloaded to the target, the target will have to be reseted at the first time. Otherwise,
   * the data will no be transmitted. After the reset, it works normally.
   * So with the enabled debug mode, no reset must be performed. This is important for test automation.
   */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR,
      ((hwCfgPtr->DebugModeEnabled == TRUE) ? I2C_REG_MASK_MCR_DBGEN : 0u));

  /* #20 Initialize flags. */
 (&hwCfgPtr->varHwUnitDescPtr->PerData)->State = IDLE;
 transDataPtr->RepeatedStart = FALSE;

} /* I2c_PerMgr_Init() */

/**********************************************************************************************************************
 *  I2c_PerMgr_DeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_DeInit(I2c_HwUnitType hwUnitId)
{
  /* #10 Disable I2C peripheral. */
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_DBGEN);
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

  /* #20 Reset I2C peripheral. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);

  /* #30 Set state to UNINIT. */
  (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData)->State = UNINIT;
} /* I2c_PerMgr_DeInit */

/**********************************************************************************************************************
 *  I2c_PerMgr_IsHwUnitIdle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, I2C_CODE) I2c_PerMgr_IsHwUnitIdle(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------------------------------------------------- */
  I2c_ConstVarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  boolean isHwUnitIdle = FALSE;

  /* ----- Implementation ----------------------------------------------------------------------------------------- */
  /* #10 Check if hardware is able to accept new jobs. */
  if(hwRuntimeData->State == IDLE)
  {
    isHwUnitIdle = TRUE;
  }
  /* #20 Return the state. */
  return isHwUnitIdle;

} /* I2c_PerMgr_IsHwUnitIdle() */

/**********************************************************************************************************************
 *  I2c_PerMgr_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_MainFunction(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If hardware is busy. */
  if (hwRuntimeData->State == BUSY)
  {
    /* #20 If timeout is elapsed. */
    if (hwRuntimeData->Timeout.CntVal >= hwRuntimeData->Timeout.LimitVal)
    {
      /* #30 Report BUSFAIL and set to IDLE state. */
      I2c_TransProcessor_ReportResult(hwUnitId, I2C_CH_RESULT_BUSFAIL);
      hwRuntimeData->State = IDLE;
    }
    hwRuntimeData->Timeout.CntVal++;
  }
} /* I2c_PerMgr_MainFunction() */

/**********************************************************************************************************************
 *  I2c_PerMgr_FreeBus()
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
FUNC(void, I2C_CODE) I2c_PerMgr_FreeBus(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwCfgPtr= (&I2c_ConfigPtr->HwCfgPtr[hwUnitId]);
  uint8 loop;
  uint8 valueToTogglePin = 0x01u;
  uint16 ticks;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable I2c peripheral. */
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

  /* #20 Change Pin Mode of SCL to GPIO. */
  I2C_PortSetPinMode(hwCfgPtr->SclPulseRecover.PortSclPin, hwCfgPtr->SclPulseRecover.PortDioMode);

  /* #30 Start generating the pulses. */
  SchM_Enter_I2c_I2C_EXCLUSIVE_AREA_0();
  for(loop = 1u; loop <= I2C_FREEBUS_CLK_EDGES; loop++)
  {
    I2C_DioWriteChannel(hwCfgPtr->SclPulseRecover.DioSclPin, loop & valueToTogglePin);
    for(ticks = 0u; ticks < I2C_FREEBUS_IDLE_TICKS; ticks++)
    {
      (void)0u;
    }
  }
  SchM_Exit_I2c_I2C_EXCLUSIVE_AREA_0();

  /* #40 Switch back Pin Mode of SCL. */
  I2C_PortSetPinMode(hwCfgPtr->SclPulseRecover.PortSclPin, hwCfgPtr->SclPulseRecover.PortI2cMode);

  /* #50 Enable I2c peripheral. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

} /* I2c_PerMgr_FreeBus() */

/**********************************************************************************************************************
 *  I2c_PerMgr_ConfigHwUnit()
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
FUNC(void, I2C_CODE) I2c_PerMgr_ConfigHwUnit(I2c_HwUnitType hwUnitId, I2c_BaudrateType baudrate,
    I2c_TimeoutType timeout)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset master logic in order to set new clock configuration. */
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);

  /* #20 Clear prescaler. */
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCFGR1, I2C_REG_MASK_MCFGR1_PRESCALE);

  /* #30 Set values for prescale, clock and timeout. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCFGR1, baudrate.preScale);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCCR0, baudrate.masterClockControlReg);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCFGR3, baudrate.pinLowTimeoutReg);

  /* #40 Re-enable I2C peripheral. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

  /* #50 Store passed timeout. */
  hwRuntimeData->Timeout.LimitVal = timeout;

} /* I2c_PerMgr_ConfigHwUnit() */

/**********************************************************************************************************************
 *  I2c_PerMgr_TransData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_TransData(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_TransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all values for transmission. */
  hwRuntimeData->State = BUSY;
  hwRuntimeData->Result = I2C_CH_RESULT_OK;
  hwRuntimeData->HeaderFrameBufferIdx = 0u;
  hwRuntimeData->BufDescrIdx = 0u;
  hwRuntimeData->CurBufIdx = 0u;
  hwRuntimeData->ReportResult = FALSE;
  transDataPtr->RemainingTotalLength = 0u;
  transDataPtr->ReceivedBytes = 0u;

  /* #20 Reset timeout for main function. */
  hwRuntimeData->Timeout.CntVal = 0u;

  /* #30 Start the transmission. */
  I2c_PerMgr_StartTrans(hwUnitId);

} /* I2c_PerMgr_TransData() */

/**********************************************************************************************************************
 *  I2c_PerMgr_DetachBus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_DetachBus(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_PerMgr_RegWidthType tmpRegMCFGR1;
  I2c_PerMgr_RegWidthType tmpRegMCFGR3;
  I2c_PerMgr_RegWidthType tmpRegMCCR0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get current values of important configuration registers. */
  tmpRegMCFGR1 = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MCFGR1);
  tmpRegMCFGR3 = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MCFGR3);
  tmpRegMCCR0 = I2c_PerMgr_Reg_Read(hwUnitId, I2C_REG_OFFSET_MCCR0);

  /* #20 Reset peripheral and enable again. */
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);
  I2c_PerMgr_Reg_ClearBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_RST);
  I2c_PerMgr_Reg_SetBitMask(hwUnitId, I2C_REG_OFFSET_MCR, I2C_REG_MASK_MCR_MEN);

  /* #30 Restore configuration registers. */
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MCFGR1, tmpRegMCFGR1);
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MCFGR3, tmpRegMCFGR3);
  I2c_PerMgr_Reg_Write(hwUnitId, I2C_REG_OFFSET_MCCR0, tmpRegMCCR0);

  (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData)->State = IDLE;
} /* I2c_PerMgr_DetachBus() */

/**********************************************************************************************************************
 *  I2c_PerMgr_IrqHandler()
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
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_PerMgr_IrqHandler(I2c_HwUnitType hwUnitId)
{
  /* ----- Local Variables ---------------------------------------------- */
  I2c_VarPerMgrPtrType hwRuntimeData = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->PerData);
  I2c_ConstTransDataPtrType transDataPtr = (&I2c_ConfigPtr->HwCfgPtr[hwUnitId].varHwUnitDescPtr->TransData);

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If NACK is detected
   *       Stop transmission, report NACK failure and finish transmission.
   */
  if (I2c_PerMgr_Reg_IsBitMaskSet(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_NDF) == TRUE)
  {
    /* FYI: Detected NACKs may generate the STOP condition automatically. */
    hwRuntimeData->Result = I2C_CH_RESULT_NACKFAIL;
    I2c_PerMgr_FinishTrans(hwUnitId);
  }
  else
  {
    /*   - #20 Otherwise
     *           If Arbitration-Loss is detected, report Arbitration failure and finish transmission.
     */
    if (I2c_PerMgr_Reg_IsBitMaskSet(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_ALF) == TRUE)
    {
      hwRuntimeData->Result = I2C_CH_RESULT_ARBFAIL;
      I2c_PerMgr_FinishTrans(hwUnitId);
    }
    /*   - #30 Otherwise
     *           If FIFO Error is detected, report Bus failure and finish transmission.
     */
    else if (I2c_PerMgr_Reg_IsBitMaskSet(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_FEF ) == TRUE)
    {
      hwRuntimeData->Result = I2C_CH_RESULT_BUSFAIL;
      I2c_PerMgr_FinishTrans(hwUnitId);
    }
    /*   - #40 Otherwise
     *           If Pin Low Timeout is detected, report Bus failure and finish transmission.
     */
    else if (I2c_PerMgr_Reg_IsBitMaskSet(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_PLTF ) == TRUE)
    {
      hwRuntimeData->Result = I2C_CH_RESULT_BUSFAIL;
      I2c_PerMgr_FinishTrans(hwUnitId);
    }
    /* - #50 Otherwise
     *         If Stop condition is detected, finish transmission.
     */
    else if (I2c_PerMgr_Reg_IsBitMaskSet(hwUnitId, I2C_REG_OFFSET_MSR, I2C_REG_MASK_MSR_SDF) == TRUE)
    {
      I2c_PerMgr_FinishTrans(hwUnitId);
    }
    /*     - #60 Otherwise
     *             Start processing data.
     */
    else
    {
      if (transDataPtr->TransDir == I2C_WRITE)
      {
        I2c_PerMgr_TxData(hwUnitId);
      }
      else
      {
        I2c_PerMgr_RxData(hwUnitId);
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* I2c_PerMgr_IrqHandler() */


/* Justification for module-specific MISRA deviations:
 MD_I2c_PerMgr_2986: Rule 2.2
 Reason:     This operation is redundant. The value of the result is always that of the right-hand operand.
             The transmission of data need additional command bytes. One command byte for sending data is 0x00.
             It would be possible to remove this command byte, because an OR operation do not change the result.
             But readability will be decreased.
 Risk:       Decrease of performance.
 Prevention: None.

 */

/* SBSW_JUSTIFICATION_BEGIN
 SBSW_JUSTIFICATION_END
 */

/* START_COVERAGE_JUSTIFICATION

 \ID COV_I2C_COMPATIBILITY
   \ACCEPT XF
   \REASON [COV_MSR_COMPATIBILITY]

 END_COVERAGE_JUSTIFICATION
 */

/**********************************************************************************************************************
 *  END OF FILE: I2c_PerMgr.c
 *********************************************************************************************************************/

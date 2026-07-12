/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_P11.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 11
 *
 *      \details
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P11.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*!
 * Helper struct to combine information for the P11Check() function.
 */
typedef struct
{
  boolean IsNewDataAvbl;
  uint8 ReceivedNibble;
  uint8 ReceivedCounter;
  uint8 ReceivedCrc;
  uint8 ComputedCrc;
} E2E_P11CheckedData;

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#define E2E_P11_MAX_COUNTER_VALUE 0xEu

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (E2E_P11_LOCAL) /* COV_E2E_COMPATIBILITY */
# define E2E_P11_LOCAL static
#endif

#if !defined (E2E_P11_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
# define E2E_P11_LOCAL_INLINE LOCAL_INLINE
#endif

#define E2E_P11_BITOFFSET_TO_BYTEOFFSET(bit_offset_val) ((bit_offset_val) >> 3u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P11ProtectVerifyInputs
 *********************************************************************************************************************/
/*!
 * \brief      Verify inputs for P11Protect() function.
 * \details    -
 * \param[in]  ConfigPtr  Pointer to static configuration.
 * \param[in]  StatePtr   Pointer to port/data communication state.
 * \param[in]  DataPtr    Pointer to data to be transmitted.
 * \param[in]  Length     Length of the data in bytes.
 * \return     Standard return value, indicating valid/invalid input.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P11ProtectVerifyInputs(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length);

/**********************************************************************************************************************
 *  E2E_P11CheckVerifyInputs
 *********************************************************************************************************************/
/*!
 * \brief      Verify inputs for P11Check() function.
 * \details    -
 * \param[in]  ConfigPtr          Pointer to static configuration.
 * \param[in]  StatePtr           Pointer to port/data communication state.
 * \param[in]  DataPtr            Pointer to data to be transmitted.
 * \param[in]  Length             Length of the data in bytes.
 * \param[out] NewDataAvailable   Out-variable indicating information if new data is available.
 * \return     Standard return value, indicating valid/invalid input.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P11CheckVerifyInputs(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(boolean, AUTOMATIC, E2E_VAR) NewDataAvailable);

/**********************************************************************************************************************
 *  E2E_P11CheckReadNewData
 *********************************************************************************************************************/
/*!
 * \brief      Helper function for P11Check() to read new data.
 * \details    Reads required data into CheckedDataPtr structure.
 * \param[in]  ConfigPtr          Pointer to static configuration.
 * \param[in]  DataPtr            Pointer to data to be transmitted.
 * \param[in]  Length             Length of the data in bytes.
 * \param[out] CheckedDataPtr     Pointer to structure holding read data.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11CheckReadNewData(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(E2E_P11CheckedData, AUTOMATIC, E2E_VAR) CheckedDataPtr);

/**********************************************************************************************************************
 *  E2E_P11CheckValidateReceivedData
 *********************************************************************************************************************/
/*!
 * \brief         Helper function for P11Check() to check for correct data.
 * \details       Updates StatePtr counter and status information.
 * \param[in]     ConfigPtr       Pointer to static configuration.
 * \param[in,out] StatePtr        Pointer to port/data communication state.
 * \param[in]     CheckedDataPtr  Pointer to structure holding read data.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11CheckValidateReceivedData(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(E2E_P11CheckedData, AUTOMATIC, E2E_DATA) CheckedDataPtr);

/**********************************************************************************************************************
 *  E2E_P11ComputeCRC
 *********************************************************************************************************************/
/*!
 * \brief      Compute CRC8 value.
 * \details    -
 * \param[in]  ConfigPtr          Pointer to static configuration.
 * \param[in]  DataPtr            Pointer to data to be transmitted.
 * \param[in]  Length             Length of the data in bytes.
 * \return     The computed CRC8 value.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(uint8, E2E_CODE) E2E_P11ComputeCRC(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length);

/**********************************************************************************************************************
 *  E2E_P11WriteNibble
 *********************************************************************************************************************/
/*!
 * \brief      Writes given Nibble to data buffer.
 * \details    Uses NibbleBitwiseOffset to write Nibble to correct position in data buffer.
 * \param[in]  Nibble               4-bit information to be written.
 * \param[in]  NibbleBitwiseOffset  Bitwise offset where Nibble shall be written in DataPtr.
 * \param[in]  DataPtr              Data buffer to be used.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11WriteNibble(
    CONST(uint8, AUTOMATIC) Nibble,
    CONST(uint16, AUTOMATIC) NibbleBitwiseOffset,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);

/**********************************************************************************************************************
 *  E2E_P11ReadNibble
 *********************************************************************************************************************/
/*!
 * \brief      Reads out a Nibble of given data buffer.
 * \details    Uses NibbleBitwiseOffset to read Nibble from correct position in data buffer.
 * \param[in]  DataPtr              Data buffer to read from.
 * \param[in]  NibbleBitwiseOffset  Bitwise offset pointing to nibble.
 * \return     Read Nibble.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_P11_LOCAL FUNC(uint8, E2E_CODE) E2E_P11ReadNibble(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_CONST) DataPtr,
    CONST(uint16, AUTOMATIC) NibbleBitwiseOffset);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P11ProtectVerifyInputs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
E2E_P11_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P11ProtectVerifyInputs(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length)
{
  Std_ReturnType retVal = E2E_E_INPUTERR_NULL;

  if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
  {
    if (Length == E2E_P11_BITOFFSET_TO_BYTEOFFSET(ConfigPtr->DataLength))
    {
      retVal = E2E_E_OK;
    }
    else
    {
      retVal = E2E_E_INPUTERR_WRONG;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11CheckVerifyInputs
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
 *
 *
 */
E2E_P11_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P11CheckVerifyInputs(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_CONST) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_CONST) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(boolean, AUTOMATIC, E2E_VAR) NewDataAvailable)
{
  Std_ReturnType retVal = E2E_E_INPUTERR_NULL;

  /* #1000 Set NewDataAvailable to FALSE */
  *NewDataAvailable = FALSE; /* SBSW_E2E_02 */

  /* #2000 If trivial NULL pointer checks succeed */
  if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
  {
    const boolean isDataValid = (DataPtr != NULL_PTR);
    const boolean isLengthValid = (Length != 0u);

    retVal = E2E_E_INPUTERR_WRONG;

    /* #2100 If DataPtr/Length validation succeeds */
    if ((isDataValid && isLengthValid) || (!isDataValid && !isLengthValid))
    {
      retVal = E2E_E_OK;

      if (isDataValid)
      {
        /* #2110 If DataPtr is valid and data length verification succeeds */
        if (Length == E2E_P11_BITOFFSET_TO_BYTEOFFSET(ConfigPtr->DataLength))
        {
          *NewDataAvailable = TRUE; /* SBSW_E2E_02 */
        }
        /* #2120 If DataPtr is valid and data length verification fails */
        else
        {
          retVal = E2E_E_INPUTERR_WRONG;
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11CheckReadNewData
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
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11CheckReadNewData(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(E2E_P11CheckedData, AUTOMATIC, E2E_VAR) CheckedDataPtr)
{
  /* #100 If new data is available */
  if (CheckedDataPtr->IsNewDataAvbl)
  {
    /* #110 If DataIDMode is NIBBLE */
    if (ConfigPtr->DataIDMode == E2E_P11_DATAID_NIBBLE)
    {
      /* #111 Read DataIDNibble */
      CheckedDataPtr->ReceivedNibble = E2E_P11ReadNibble(DataPtr, ConfigPtr->DataIDNibbleOffset); /* SBSW_E2E_02 */ /* SBSW_E2E_03 */
    }

    /* #120 Read Counter */
    CheckedDataPtr->ReceivedCounter = E2E_P11ReadNibble(DataPtr, ConfigPtr->CounterOffset); /* SBSW_E2E_02 */ /* SBSW_E2E_03 */

    /* #130 Read CRC */
    CheckedDataPtr->ReceivedCrc = DataPtr[E2E_P11_BITOFFSET_TO_BYTEOFFSET(ConfigPtr->CRCOffset)]; /* SBSW_E2E_02 */

    /* #140 Compute CRC */
    CheckedDataPtr->ComputedCrc = E2E_P11ComputeCRC(ConfigPtr, DataPtr, Length); /* SBSW_E2E_02 */ /* SBSW_E2E_03 */
  }
}

/**********************************************************************************************************************
 *  E2E_P11CheckValidateReceivedData
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
 *
 *
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11CheckValidateReceivedData(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(E2E_P11CheckedData, AUTOMATIC, E2E_DATA) CheckedDataPtr)
{
  /* #100 If new data is available */
  if (CheckedDataPtr->IsNewDataAvbl)
  {
    /* #110 Validate CRC, test DataIDMode and DataNibble correctness (if configured) */
    if ((CheckedDataPtr->ReceivedCrc == CheckedDataPtr->ComputedCrc)
        && (
            (
                (ConfigPtr->DataIDMode == E2E_P11_DATAID_NIBBLE)
                && (CheckedDataPtr->ReceivedNibble == (uint8)((ConfigPtr->DataID >> 8u) & 0x0Fu))
            )
            || (ConfigPtr->DataIDMode != E2E_P11_DATAID_NIBBLE)
           )
    )
    {
      /* #121 Calculate counter delta (take limit into account) */
      uint8 deltaCounter = (CheckedDataPtr->ReceivedCounter - StatePtr->Counter) & 0x0Fu;
      if (StatePtr->Counter > CheckedDataPtr->ReceivedCounter)
      {
        deltaCounter--;
      }

      /* #122 Evaluate counter delta:
       *   delta == 0                 => Status=REPEATED
       *   delta == 1                 => Status=OK
       *   delta <= MaxDeltaCounter   => Status=OKSOMELOST
       *   OTHER                      => WRONGSEQUENCE */
      if (deltaCounter == 0u)
      {
        StatePtr->Status = E2E_P11STATUS_REPEATED; /* SBSW_E2E_03 */
      }
      else if (deltaCounter == 1u)
      {
        StatePtr->Status = E2E_P11STATUS_OK; /* SBSW_E2E_03 */
      }
      else if (deltaCounter <= ConfigPtr->MaxDeltaCounter)
      {
        StatePtr->Status = E2E_P11STATUS_OKSOMELOST; /* SBSW_E2E_03 */
      }
      else
      {
        StatePtr->Status = E2E_P11STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
      }

      /* #123 Sync Counter with ReceivedCounter */
      StatePtr->Counter = CheckedDataPtr->ReceivedCounter; /* SBSW_E2E_03 */
    }
    else
    {
      StatePtr->Status = E2E_P11STATUS_ERROR; /* SBSW_E2E_03 */
    }
  }
  else
  {
    StatePtr->Status = E2E_P11STATUS_NONEWDATA; /* SBSW_E2E_03 */
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  E2E_P11ComputeCRC
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
E2E_P11_LOCAL FUNC (uint8, E2E_CODE) E2E_P11ComputeCRC(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length)
{
  uint32 crcLength;
  uint16 crcOffset;
  uint8 localDataID[2u];
  uint8 computedCrc;
  const boolean isCrcFirstCall = FALSE;

  /* #10 Calculate byte offset */
  crcOffset = E2E_P11_BITOFFSET_TO_BYTEOFFSET(ConfigPtr->CRCOffset);

  switch (ConfigPtr->DataIDMode)
  {
    case E2E_P11_DATAID_BOTH:
      localDataID[0] = (uint8)ConfigPtr->DataID; /* SBSW_E2E_05 */
      localDataID[1] = (uint8)(ConfigPtr->DataID >> 8u); /* SBSW_E2E_05 */
      crcLength = 2u;
      break;

    case E2E_P11_DATAID_NIBBLE:
      localDataID[0] = (uint8)ConfigPtr->DataID; /* SBSW_E2E_05 */
      localDataID[1] = 0u; /* SBSW_E2E_05 */
      crcLength = 2u;
      break;

    default: /* COV_E2E_MISRA */
      /* Unreachable code - nevertheless ensure safe buffer/size settings */
      localDataID[0] = 0u; /* SBSW_E2E_05 */
      crcLength = 0u;
      break;
  }

  /* #20 Compute CRC8 for DataID */
  computedCrc = Crc_CalculateCRC8(localDataID, crcLength, 0xFFu, isCrcFirstCall); /* SBSW_E2E_02 */

  /* #30 compute CRC8 for rest of data */
  if (crcOffset > 0u)
  {
    /* Compute data preceding CRC (CRC value is not the first byte) */
    computedCrc = Crc_CalculateCRC8(DataPtr, crcOffset, computedCrc, isCrcFirstCall); /* SBSW_E2E_02 */

    /* Compute rest of data after CRC position (CRC value is not the last byte) */
    if (Length > (crcOffset + 1u))
    {
      crcLength = (uint32)Length - crcOffset - 1u;
      computedCrc = Crc_CalculateCRC8(&DataPtr[crcOffset + 1u], crcLength, computedCrc, isCrcFirstCall); /* SBSW_E2E_02 */
    }
  }
  else
  {
    crcLength  = (uint32)Length - 1u;
    computedCrc = Crc_CalculateCRC8(&DataPtr[1u], crcLength, computedCrc, isCrcFirstCall); /* SBSW_E2E_02 */
  }

  return computedCrc;
}

/**********************************************************************************************************************
 *  E2E_P11WriteNibble
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_P11_LOCAL FUNC(void, E2E_CODE) E2E_P11WriteNibble(
    CONST(uint8, AUTOMATIC) Nibble,
    CONST(uint16, AUTOMATIC) NibbleBitwiseOffset,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr)
{
  uint16 bytewiseOffset;
  uint8 relativeBitwiseOffset;
  uint8 nibbleMask;

  /* Determine byte offset */
  bytewiseOffset = E2E_P11_BITOFFSET_TO_BYTEOFFSET(NibbleBitwiseOffset);

  /* Determine bitwise offset to byte */
  relativeBitwiseOffset = (uint8)(NibbleBitwiseOffset & 0x7u);

  /* Determine nibble mask (low nibble or high nibble) */
  nibbleMask = ((relativeBitwiseOffset == 0u) ? 0x0Fu : 0xF0u );

  /* Clear nibble via mask and write data */
  DataPtr[bytewiseOffset] = (uint8)(
      (DataPtr[bytewiseOffset] & ~nibbleMask)
      | ((Nibble << relativeBitwiseOffset) & nibbleMask)); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P11ReadNibble
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_P11_LOCAL FUNC(uint8, E2E_CODE) E2E_P11ReadNibble(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_CONST) DataPtr,
    CONST(uint16, AUTOMATIC) NibbleBitwiseOffset)
{
  uint16 bytewiseOffset;
  uint8 relativeBitwiseOffset;

  /* Determine byte offset */
  bytewiseOffset = E2E_P11_BITOFFSET_TO_BYTEOFFSET(NibbleBitwiseOffset);

  /* Determine bitwise offset to byte */
  relativeBitwiseOffset = (uint8)(NibbleBitwiseOffset & 0x7u);

  /* Read nibble */
  return (uint8)((DataPtr[bytewiseOffset] >> relativeBitwiseOffset) & 0x0Fu);
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P11Protect()
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P11Protect(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    VAR(uint16, AUTOMATIC) Length)
{
  /* #10 Verify inputs of the protect function */
  Std_ReturnType retVal = E2E_P11ProtectVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length); /* SBSW_E2E_03 */

  if (retVal == E2E_E_OK)
  {
    uint16 bytewiseOffset;
    uint8 computedCrc;

    /* #21 Write DataIDNibble (if configured): low nibble of DataID high byte to DataIDNibbleOffset */
    if (ConfigPtr->DataIDMode == E2E_P11_DATAID_NIBBLE)
    {
      uint8 lowNibbleHighByte = (uint8)((ConfigPtr->DataID & 0x0F00u) >> 8u);
      E2E_P11WriteNibble(lowNibbleHighByte, ConfigPtr->DataIDNibbleOffset, DataPtr); /* SBSW_E2E_03 */
    }

    /* #22 Write Counter: low nibble of counter to counterOffset */
    E2E_P11WriteNibble((StatePtr->Counter & 0x0Fu), ConfigPtr->CounterOffset, DataPtr); /* SBSW_E2E_03 */

    /* #23 Compute CRC */
    computedCrc = E2E_P11ComputeCRC(ConfigPtr, DataPtr, Length); /* SBSW_E2E_03 */

    /* #24 Write CRC */
    bytewiseOffset = E2E_P11_BITOFFSET_TO_BYTEOFFSET(ConfigPtr->CRCOffset);
    DataPtr[bytewiseOffset] = computedCrc; /* SBSW_E2E_03 */

    /* #25 Increment Counter */
    /* Ensure to reset when max value reached */
    StatePtr->Counter = (uint8)((StatePtr->Counter + 1u) % (E2E_P11_MAX_COUNTER_VALUE + 1u)); /* SBSW_E2E_03 */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11ProtectInit(P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
  Std_ReturnType retVal = E2E_E_INPUTERR_NULL;

  if (StatePtr != NULL_PTR)
  {
    StatePtr->Counter = 0u; /* SBSW_E2E_03 */
    retVal = E2E_E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11Check()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11Check(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    VAR(uint16, AUTOMATIC) Length)
{
  E2E_P11CheckedData checkedData;
  Std_ReturnType retVal;

  retVal = E2E_P11CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length, &(checkedData.IsNewDataAvbl)); /* SBSW_E2E_03 */

  if (retVal == E2E_E_OK)
  {
    E2E_P11CheckReadNewData(ConfigPtr, DataPtr, Length, &checkedData); /* SBSW_E2E_03 */

    E2E_P11CheckValidateReceivedData(ConfigPtr, StatePtr, &checkedData); /* SBSW_E2E_03 */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11CheckInit(P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_CONST) StatePtr)
{
  Std_ReturnType retVal = E2E_E_INPUTERR_NULL;

  if (StatePtr != NULL_PTR)
  {
    StatePtr->Counter = 0x0Eu; /* SBSW_E2E_03 */
    StatePtr->Status = E2E_P11STATUS_ERROR; /* SBSW_E2E_03 */

    retVal = E2E_E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  E2E_P11MapStatusToSM()
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
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P11MapStatusToSM(
    VAR(Std_ReturnType, AUTOMATIC) CheckReturn,
    VAR(E2E_P11CheckStatusType, AUTOMATIC) Status)
{
  E2E_PCheckStatusType retVal = E2E_P_ERROR;

  if (CheckReturn == E2E_E_OK)
  {
    switch (Status)
    {
      case E2E_P11STATUS_REPEATED:      retVal = E2E_P_REPEATED;      break;
      case E2E_P11STATUS_NONEWDATA:     retVal = E2E_P_NONEWDATA;     break;
      case E2E_P11STATUS_WRONGSEQUENCE: retVal = E2E_P_WRONGSEQUENCE; break;

      /* Intentional fall through as same behavior */
      case E2E_P11STATUS_OK:
      case E2E_P11STATUS_OKSOMELOST:
        retVal = E2E_P_OK;
        break;

      /* Intentional fall through as same behavior */
      case E2E_P11STATUS_ERROR:
      default:
        retVal = E2E_P_ERROR;
        break;
    }
  }

  return retVal;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  END OF FILE: E2E_P11.c
 *********************************************************************************************************************/

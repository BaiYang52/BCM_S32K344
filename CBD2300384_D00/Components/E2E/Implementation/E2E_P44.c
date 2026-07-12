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
 *         \file  E2E_P44.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 44
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults 
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for 
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P44.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if !defined(E2EP44_LOCAL) /* COV_E2E_COMPATIBILITY */
#  define E2EP44_LOCAL static
#endif

#if !defined(E2EP44_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
#  define E2EP44_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/
typedef P2CONST(E2E_P44ProtectStateType, AUTOMATIC, E2E_APPL_DATA) E2E_P44ConstProtectStatePtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  PROTECT HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P44Protect
 * \details    All relevant input parameters are checked in order to proceed with E2E_P44Protect service
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \param[in]  Length     length of the data in bytes
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P44ProtectVerifyInputs(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ConstProtectStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length
);

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteLength()
 *********************************************************************************************************************/
/*!
 * \brief      Write Length of message to the E2E Header
 * \details    Write Length of message to the E2E Header in big endian format
 * \param[out] DataPtr  Pointer to the data to be protected
 * \param[in]  Length   Length of the data in bytes
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteLength(
    E2E_P44DataPtrType DataPtr,
    const E2E_P44LengthType Length,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Write the counter value to the E2E header
 * \details    Write the counter value to the E2E header in big endian format
 * \param[in]  StatePtr  Pointer to the state data structure
 * \param[out] DataPtr   Pointer to the data to be protected
 * \param[in]  Offset    Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteCounter(
    E2E_P44ConstProtectStatePtrType StatePtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteDataID()
 *********************************************************************************************************************/
/*!
 * \brief      Write the DataID to the E2E header
 * \details    Write the DataID to the E2E header in big endian format
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[out] DataPtr    Pointer to the data to be protected
 * \param[in]  Offset     Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteDataID(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Write the computed CRC value to the E2E header
 * \details    Write the computed CRC value to the E2E header in big endian format
 * \param[out] DataPtr      Pointer to the data to be protected
 * \param[in]  Offset       Offset of the E2E header in DataPtr in bytes
 * \param[in]  ComputedCRC  The computed CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteCRC(
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset,
    const E2E_P44CrcType ComputedCRC
);

/**********************************************************************************************************************
 *  CHECK HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Return the CRC value of the E2E header
 * \details    Return the CRC value of the E2E header which was stored in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     ReadCrc  The CRC value of the E2E header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (E2E_P44CrcType, E2E_CODE) E2E_P44CheckReadCRC(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief         Checks the input data of E2E_P44Check
 * \details       All relevant input parameters are checked in order to proceed with E2E_P44Check service
 * \param[in]     ConfigPtr            Pointer to the config data structure
 * \param[in]     StatePtr             Pointer to the state data structure
 * \param[in]     DataPtr              Pointer to the data to be protected
 * \param[in]     Length               Length of the data in bytes
 * \param[in,out] NewDataAvailablePtr  Pointer to variable where new data available indication shall be stored
 * \return        Standard return value, indicating valid/invalid input
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2EP44_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P44CheckVerifyInputs(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ConstCheckStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length,
    P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr
);

/**********************************************************************************************************************
 *  E2E_P44CheckReadLength()
 *********************************************************************************************************************/
/*!
 * \brief      Return the length field of the E2E header
 * \details    Return the length field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr      Pointer to the data to be protected
 * \param[in]  Offset       Offset of the E2E header in DataPtr in bytes
 * \return     ReadLength   The value of the Length field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (E2E_P44LengthType, E2E_CODE) E2E_P44CheckReadLength(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44CheckReadCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Return the Counter field of the E2E header
 * \details    Return the Counter field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr      Pointer to the data to be protected
 * \param[in]  Offset       Offset of the E2E header in DataPtr in bytes
 * \return     CounterValue The value of the Counter field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (E2E_P44CounterType, E2E_CODE) E2E_P44CheckReadCounter(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44CheckReadDataID()
 *********************************************************************************************************************/
/*!
 * \brief      Return the DataID field of the E2E header
 * \details    Return the DataID field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr      Pointer to the data to be protected
 * \param[in]  Offset       Offset of the E2E header in DataPtr in bytes
 * \return     DataIdValue  The value of the DataID field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (E2E_P44IdType, E2E_CODE) E2E_P44CheckReadDataID(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  E2E_P44CheckDoChecksCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Determine the E2E status based on received and expected Counter
 * \details    Determine the E2E status based on received and expected Counter
 * \param[in]  ConfigPtr        Pointer to the config data structure
 * \param[in]  StatePtr         Pointer to the state data structure
 * \param[in]  ReceivedCounter  The value of the Counter field of the E2E header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44CheckDoChecksCounter(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44CheckStatePtrType StatePtr,
    const E2E_P44CounterType ReceivedCounter
);

/**********************************************************************************************************************
 *  SHARED HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44ComputeCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Compute the CRC over payload and header
 * \details    Compute the CRC over payload and header using CRC32 Profile 4 service
 * \param[in]  DataPtr      Pointer to the data to be protected
 * \param[in]  Length       length of the data in bytes
 * \param[in]  Offset       Number of bytes before the E2E header
 * \return     ComputedCRC  The calculated CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2EP44_LOCAL FUNC (E2E_P44CrcType, E2E_CODE) E2E_P44ComputeCRC(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length,
    const E2E_P44OffsetType Offset
);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  PROTECT HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
E2EP44_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P44ProtectVerifyInputs(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ConstProtectStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length
)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    if (  (ConfigPtr != NULL_PTR)
       && (StatePtr != NULL_PTR)
       && (DataPtr != NULL_PTR)
       )
    {
        /* Divide MinDataLength and MaxDataLength by 8. This is done via shifting by 3. (=> MinDataLength / 2^3) */
        if (  (Length >= (ConfigPtr->MinDataLength >> 3u))
           && (Length <= (ConfigPtr->MaxDataLength >> 3u))
           )
        {
            ret = E2E_E_OK;
        }
        else /* Length out of valid range */
        {
          ret = E2E_E_INPUTERR_WRONG;
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteLength(
    E2E_P44DataPtrType DataPtr,
    const E2E_P44LengthType Length,
    const E2E_P44OffsetType Offset
)
{
    DataPtr[Offset] = (uint8) (Length >> 8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 1u] = (uint8) (Length); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteCounter(
    E2E_P44ConstProtectStatePtrType StatePtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    DataPtr[Offset + 2u] = (uint8) (StatePtr->Counter >> 8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 3u] = (uint8) (StatePtr->Counter); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteDataID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteDataID(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    DataPtr[Offset + 4u] = (uint8) (ConfigPtr->DataID >> 24u); /* SBSW_E2E_03 */
    DataPtr[Offset + 5u] = (uint8) (ConfigPtr->DataID >> 16u); /* SBSW_E2E_03 */
    DataPtr[Offset + 6u] = (uint8) (ConfigPtr->DataID >>  8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 7u] = (uint8) (ConfigPtr->DataID); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P44ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44ProtectWriteCRC(
    E2E_P44DataPtrType DataPtr,
    const E2E_P44OffsetType Offset,
    const E2E_P44CrcType ComputedCRC
)
{
    DataPtr[Offset + 8u] =  (uint8) (ComputedCRC  >> 24u); /* SBSW_E2E_03 */
    DataPtr[Offset + 9u] =  (uint8) (ComputedCRC  >> 16u); /* SBSW_E2E_03 */
    DataPtr[Offset + 10u] = (uint8) (ComputedCRC  >>  8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 11u] = (uint8) (ComputedCRC); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  CHECK HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (E2E_P44CrcType, E2E_CODE) E2E_P44CheckReadCRC(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    return (((E2E_P44CrcType) DataPtr[Offset + 8u])  << 24u)
                | (((E2E_P44CrcType) DataPtr[Offset + 9u])  << 16u)
                | (((E2E_P44CrcType) DataPtr[Offset + 10u]) <<  8u)
                | (((E2E_P44CrcType) DataPtr[Offset + 11u]));
}

/**********************************************************************************************************************
 *  E2E_P44CheckVerifyInputs()
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
E2EP44_LOCAL FUNC(Std_ReturnType, E2E_CODE) E2E_P44CheckVerifyInputs(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ConstCheckStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length,
    P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr
)
{
    Std_ReturnType inputState = E2E_E_INPUTERR_WRONG;

    *NewDataAvailablePtr = FALSE; /* SBSW_E2E_04 */

    if((ConfigPtr == NULL_PTR) || (StatePtr == NULL_PTR))
    {
      inputState = E2E_E_INPUTERR_NULL;
    }
    else /* (ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) */
    {
        /* Data is available if pointer is valid and length > 0 */
        if ((DataPtr != NULL_PTR) && (Length != 0u))
        {
            /* Divide MinDataLength and MaxDataLength by 8. This is done via shifting by 3. (=> MinDataLength / 2^3) */
            if (  (Length >= (ConfigPtr->MinDataLength >> 3u))
               && (Length <= (ConfigPtr->MaxDataLength >> 3u)) )
            {
                *NewDataAvailablePtr = TRUE; /* SBSW_E2E_04 */
                inputState = E2E_E_OK;
            }
        }
        /* No data is available if pointer is empty and length is 0 */
        else if ( (DataPtr == NULL_PTR) && (Length == 0u) )
        {
            inputState = E2E_E_OK;
        }
        else /* (DataPtr != NULL_PTR) && (Length == 0) */
        {
            inputState = E2E_E_INPUTERR_WRONG;
        }
    }

    return inputState;
}

/**********************************************************************************************************************
 *  E2E_P44CheckReadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (E2E_P44LengthType, E2E_CODE) E2E_P44CheckReadLength(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    return (E2E_P44LengthType)((((E2E_P44LengthType) DataPtr[Offset]) << 8u)
                | (E2E_P44LengthType) (DataPtr[Offset + 1u]));
}

/**********************************************************************************************************************
 *  E2E_P44CheckReadCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (E2E_P44CounterType, E2E_CODE) E2E_P44CheckReadCounter(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    return (E2E_P44CounterType)((((E2E_P44CounterType) DataPtr[Offset + 2u]) << 8u)
                | (DataPtr[Offset + 3u]));
}

/**********************************************************************************************************************
 *  E2E_P44CheckReadDataID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP44_LOCAL FUNC (E2E_P44IdType, E2E_CODE) E2E_P44CheckReadDataID(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44OffsetType Offset
)
{
    /* #10 Stores DataID bytes in big endian (high byte first) order to local variable ReceivedDataID */
    return (E2E_P44IdType)((((E2E_P44IdType) DataPtr[Offset + 4u]) << 24u)
                   | (((E2E_P44IdType) DataPtr[Offset + 5u]) << 16u)
                   | (((E2E_P44IdType) DataPtr[Offset + 6u]) <<  8u)
                   | (((E2E_P44IdType) DataPtr[Offset + 7u])));
}

/**********************************************************************************************************************
 *  E2E_P44CheckDoChecksCounter()
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
E2EP44_LOCAL FUNC (void, E2E_CODE) E2E_P44CheckDoChecksCounter(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44CheckStatePtrType StatePtr,
    const E2E_P44CounterType ReceivedCounter
)
{
    /*
     * Wrap around case: ReceivedCounter = 0; StatePtr->Counter = 0xFF'FF
     * 0 - 0xFF'FF = 1
     * Wrap around considered, new data will be recognized
     */
    E2E_P44CounterType DeltaCounter = (E2E_P44CounterType)(ReceivedCounter - StatePtr->Counter);

    /*
     * DeltaCounter is ALWAYS greater or equal to 0 due to the nature of unsigned variables.
     * Can be ignored in the condition.
     */
    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
    {
        if (DeltaCounter == 0u)
        {
            StatePtr->Status = E2E_P44STATUS_REPEATED; /* SBSW_E2E_03 */
        }
        else if (DeltaCounter == 1u)
        {
            StatePtr->Status = E2E_P44STATUS_OK; /* SBSW_E2E_03 */
        }
        else /* DeltaCounter >= 2u */
        {
            StatePtr->Status = E2E_P44STATUS_OKSOMELOST; /* SBSW_E2E_03 */
        }
    }
    else /* DeltaCounter > ConfigPtr->MaxDeltaCounter */
    {
        StatePtr->Status = E2E_P44STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
    }

    StatePtr->Counter = ReceivedCounter; /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  SHARED HELPER FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44ComputeCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
E2EP44_LOCAL FUNC (E2E_P44CrcType, E2E_CODE) E2E_P44ComputeCRC(
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length,
    const E2E_P44OffsetType Offset
)
{
    const uint8 bytesBeforeCrc = 8u;
    const uint8 headerLength = (uint8)(bytesBeforeCrc + sizeof(E2E_P44CrcType));

    E2E_P44CrcType ComputedCRC = Crc_CalculateCRC32P4(
                                    &DataPtr[0],
                                    /* Calculate CRC over header of E2E, not including 4 CRC bytes */
                                    (E2E_P44OffsetType)(Offset + bytesBeforeCrc),
                                    0xFFFFFFFFu,
                                    TRUE
                                ); /* SBSW_E2E_03 */

    if (Length > (Offset + headerLength))
    {
        const E2E_P44OffsetType userDataLength = (E2E_P44OffsetType)(Length - Offset - headerLength);
        ComputedCRC = Crc_CalculateCRC32P4(
                        /* Skip the header in the calculation */
                        &(DataPtr[Offset + headerLength]), /* SBSW_E2E_03 */
                        /* Calculate CRC over user data */
                        userDataLength,
                        ComputedCRC,
                        FALSE
                    );
    }
    return ComputedCRC;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P44Protect()
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P44Protect(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ProtectStatePtrType StatePtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44LengthType Length
)
{
    Std_ReturnType ret = E2E_P44ProtectVerifyInputs (ConfigPtr, StatePtr, DataPtr, Length); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        E2E_P44CrcType computedCRC;
        E2E_P44OffsetType offsetInBytes = ConfigPtr->Offset >> 3u;
        E2E_P44ProtectWriteLength (DataPtr, Length, offsetInBytes); /* SBSW_E2E_03 */
        E2E_P44ProtectWriteCounter (StatePtr, DataPtr, offsetInBytes); /* SBSW_E2E_03 */
        E2E_P44ProtectWriteDataID (ConfigPtr, DataPtr, offsetInBytes); /* SBSW_E2E_03 */
        computedCRC = E2E_P44ComputeCRC (DataPtr, Length, offsetInBytes); /* SBSW_E2E_03 */
        E2E_P44ProtectWriteCRC (DataPtr, offsetInBytes, computedCRC); /* SBSW_E2E_03 */
        StatePtr->Counter++; /* SBSW_E2E_03 */
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P44ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44ProtectInit(E2E_P44ProtectStatePtrType StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0u; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P44Check()
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P44Check(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44CheckStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length
)
{
    boolean newDataAvailable = FALSE;
    Std_ReturnType ret = E2E_P44CheckVerifyInputs( 
                            ConfigPtr, /* SBSW_E2E_03 */
                            StatePtr,
                            DataPtr,
                            Length,
                            &newDataAvailable
                        );

    if (ret == E2E_E_OK) 
    {
        if (newDataAvailable == TRUE) 
        {
            const E2E_P44OffsetType offsetInBytes     = ConfigPtr->Offset >> 3u;
            const E2E_P44LengthType receivedLength    = E2E_P44CheckReadLength (DataPtr, offsetInBytes); /* SBSW_E2E_03 */
            const E2E_P44CrcType receivedCRC          = E2E_P44CheckReadCRC (DataPtr, offsetInBytes); /* SBSW_E2E_03 */
            const E2E_P44CrcType computedCRC          = E2E_P44ComputeCRC (DataPtr, Length, offsetInBytes); /* SBSW_E2E_03 */
            const E2E_P44CounterType receivedCounter  = E2E_P44CheckReadCounter (DataPtr, offsetInBytes); /* SBSW_E2E_03 */
            const E2E_P44IdType receivedDataID        = E2E_P44CheckReadDataID (DataPtr, offsetInBytes); /* SBSW_E2E_03 */

            if ((receivedCRC == computedCRC)
                && (receivedDataID == ConfigPtr->DataID)
                && (receivedLength == Length))
            {
                E2E_P44CheckDoChecksCounter(ConfigPtr, StatePtr,  receivedCounter); /* SBSW_E2E_03 */
            }
            else /* receivedLength != Length */
            {
                StatePtr->Status = E2E_P44STATUS_ERROR; /* SBSW_E2E_03 */
            }
        }
        else /* newDataAvailable == FALSE */
        {
            StatePtr->Status = E2E_P44STATUS_NONEWDATA; /* SBSW_E2E_03 */
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P44CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44CheckInit(E2E_P44CheckStatePtrType StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0xFFFFu; /* SBSW_E2E_03 */
        StatePtr->Status = E2E_P44STATUS_ERROR; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P44MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P44MapStatusToSM(
    const Std_ReturnType CheckReturn,
    const E2E_P44CheckStatusType Status
)
{
    E2E_PCheckStatusType ret = E2E_P_ERROR;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
            case E2E_P44STATUS_OK           : ret = E2E_P_OK; break;
            case E2E_P44STATUS_OKSOMELOST   : ret = E2E_P_OK; break;
            case E2E_P44STATUS_REPEATED     : ret = E2E_P_REPEATED; break;
            case E2E_P44STATUS_NONEWDATA    : ret = E2E_P_NONEWDATA; break;
            case E2E_P44STATUS_WRONGSEQUENCE: ret = E2E_P_WRONGSEQUENCE; break;
            default: /* E2E_P44STATUS_ERROR */
              break;
        }
    }

    return ret;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P44.c
 *********************************************************************************************************************/

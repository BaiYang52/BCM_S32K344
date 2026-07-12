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
 *         \file  E2E_P08.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 8
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults 
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for 
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P08.h"

 /*********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined(E2EP08_LOCAL) /* COV_E2E_COMPATIBILITY */
#  define E2EP08_LOCAL static
#endif

#if !defined(E2EP08_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
#  define E2EP08_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P08ConvertBitsToBytes()
 *********************************************************************************************************************/
/*!
 * \brief           Converts bits to bytes.
 * \details         -
 * \param[in]       BitCount      Amount of bits to convert
 * \return          Amount of bytes
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08ConvertBitsToBytes(
    CONST (uint32, AUTOMATIC) BitCount
);

/**********************************************************************************************************************
 *  E2E_P08ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P08Protect
 * \details    All relevant input parameters are checked in order to proceed with E2E_P08Protect service
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \param[in]  Length     Length of the content of DataPtr
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
E2EP08_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P08ProtectVerifyInputs(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
    CONST (uint32, AUTOMATIC) Length
);

/**********************************************************************************************************************
 *  E2E_P08CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P08Check
 * \details    All relevant input parameters are checked in order to proceed with E2E_P05Check service
 * \param[in]  ConfigPtr            Pointer to the config data structure
 * \param[in]  StatePtr             Pointer to the state data structure
 * \param[in]  DataPtr              Pointer to the data to be protected
 * \param[in]  Length               Length of the content of DataPtr
 * \param[out] NewDataAvailablePtr  Pointer to variable where new data available indication shall be stored
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
E2EP08_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P08CheckVerifyInputs(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
    CONST (uint32, AUTOMATIC) Length,
    P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr
);

/**********************************************************************************************************************
 *  E2E_P08DoChecks()
 *********************************************************************************************************************/
/*!
 * \brief           Performs validity checks for the E2E_P08Protect service.
 * \details         Sets the StatePtr->Counter and StatePtr->Status according to the validity checks.
 * \param[in]       ConfigPtr           Pointer to the config data structure
 * \param[in,out]   StatePtr            Pointer to the state data structure
 * \param[in]       NewDataAvailable    Whether new data is available or not
 * \param[in]       ReceivedCRC         Value of the received CRC
 * \param[in]       ComputedCRC         Value of the computed CRC
 * \param[in]       ReceivedDataId      Value of the received DataId
 * \param[in]       Length              Value of the given Length by the user
 * \param[in]       ReceivedLength      Value of the received Length
 * \param[in]       ReceivedCounter     Value of the received Counter
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 */
E2EP08_LOCAL FUNC(void, E2E_CODE) E2E_P08DoChecks(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr, 
    CONST(boolean, AUTOMATIC) NewDataAvailable,
    CONST(uint32, AUTOMATIC) ReceivedCRC,
    CONST(uint32, AUTOMATIC) ComputedCRC,
    CONST(uint32, AUTOMATIC) ReceivedDataId,
    CONST(uint32, AUTOMATIC) Length,
    CONST(uint32, AUTOMATIC) ReceivedLength,
    CONST(uint32, AUTOMATIC) ReceivedCounter
);
                                                                     
/**********************************************************************************************************************
 *  E2E_P08ComputeCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Compute the CRC over payload and header
 * \details    Compute the CRC over payload and header using CRC32P4 routine
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Length   Length of the data in bytes
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \param[in]  DataId   DataId to be added to CRC calculation
 * \return     uint32   The calculated CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08ComputeCRC(
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST (uint32, AUTOMATIC ) Length,
    CONST (uint32, AUTOMATIC ) Offset
);

/**********************************************************************************************************************
 *  E2E_P08GetFourDataBytesBigEndian()
 *********************************************************************************************************************/
/*!
 * \brief      Retrieves 4 data bytes from the DataPtr in big endian order.
 * \details    -
 * \param[in]  DataPtr          Pointer to the data to be read from
 * \param[in]  OffsetInBytes    Offset of the E2E header in DataPtr in bytes
 * \return     uint32           The bytes in big endian order
 * \pre        -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08GetFourDataBytesBigEndian(
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr, 
    CONST (uint32, AUTOMATIC) OffsetInBytes
);

/**********************************************************************************************************************
 *  E2E_P08SetFourDataBytesBigEndian()
 *********************************************************************************************************************/
/*!
 * \brief      Sets 4 data bytes from the DataPtr in big endian order to the given value.
 * \details    -
 * \param[in]  DataPtr          Pointer to the data
 * \param[in]  OffsetInBytes    Offset of the E2E header in DataPtr in bytes
 * \param[in]  ValueToSet       Value to set the 4 bytes
 * \pre        -
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 */
E2EP08_LOCAL FUNC (void, E2E_CODE) E2E_P08SetFourDataBytesBigEndian(
    P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr, 
    CONST (uint32, AUTOMATIC) OffsetInBytes,
    CONST (uint32, AUTOMATIC) ValueToSet
);

/**********************************************************************************************************************
 *  E2E_P08ConvertBitsToBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08ConvertBitsToBytes(
    CONST (uint32, AUTOMATIC) BitCount
)
{
    return (BitCount >> 3); /* Shift by 3 to get a fast division by 8. */
}

/**********************************************************************************************************************
 *  E2E_P08ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP08_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P08ProtectVerifyInputs(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
    CONST (uint32, AUTOMATIC) Length
)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check if input pointer parameters are valid and if length is within valid range */
    if (  (ConfigPtr != NULL_PTR)
       && (StatePtr != NULL_PTR)
       && (DataPtr != NULL_PTR))
    {
        if (  (Length >= (E2E_P08ConvertBitsToBytes(ConfigPtr->MinDataLength)))
           && (Length <= (E2E_P08ConvertBitsToBytes(ConfigPtr->MaxDataLength))) )
        {
            ret = E2E_E_OK;
        }
        else
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08CheckVerifyInputs()
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
E2EP08_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P08CheckVerifyInputs(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
    CONST (uint32, AUTOMATIC) Length,
    P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr
)
{
    Std_ReturnType ret;
    *NewDataAvailablePtr = FALSE; /* SBSW_E2E_04 */

    /* #10 Check if both ConfigPtr and StatePtr are valid */
    if ( (ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        /* #20 Check if both DataPtr and Length are set */
        if ( ((DataPtr != NULL_PTR) && (Length != 0u))
            || ((DataPtr == NULL_PTR) && (Length == 0u)) )
        {
          /* #21 Check if DataPtr is null. This may happen at runtime if queued communication is used and no data 
                 is available */
          if (DataPtr == NULL_PTR)
          {
              ret = E2E_E_OK;
          }
          else
          {
              /* #22 Check if Length is within valid boundaries */
              if ((Length >= (E2E_P08ConvertBitsToBytes(ConfigPtr->MinDataLength)))  
                && (Length <= (E2E_P08ConvertBitsToBytes(ConfigPtr->MaxDataLength))))  
              {
                *NewDataAvailablePtr = TRUE; /* SBSW_E2E_04 */
                ret = E2E_E_OK;
              }
              else
              {
                ret = E2E_E_INPUTERR_WRONG;
              }
          }
        }
        else
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
    }
    else
    {
      ret = E2E_E_INPUTERR_NULL;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08DoChecks()
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
E2EP08_LOCAL FUNC(void, E2E_CODE) E2E_P08DoChecks(
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr, 
    CONST (boolean, AUTOMATIC) NewDataAvailable,
    CONST (uint32, AUTOMATIC) ReceivedCRC,
    CONST (uint32, AUTOMATIC) ComputedCRC,
    CONST (uint32, AUTOMATIC) ReceivedDataId,
    CONST (uint32, AUTOMATIC) Length,
    CONST (uint32, AUTOMATIC) ReceivedLength,
    CONST (uint32, AUTOMATIC) ReceivedCounter
)
{           
    StatePtr->Status = E2E_P08STATUS_NONEWDATA; /* SBSW_E2E_03 */

    if(NewDataAvailable == TRUE)
    {
        if((ReceivedCRC == ComputedCRC) 
            && (ReceivedDataId == ConfigPtr->DataId) 
            && (ReceivedLength == Length))
        {
            /* The complete range of uint32 is used for the counter, no danger through wrap around. */
            uint32 DeltaCounter = ReceivedCounter - StatePtr->Counter;
            /* Check for DeltaCounter >= 0 was left out due to unsigned int constraints (will ALWAYS be >= 0). */
            if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
            {
                if(DeltaCounter > 0u)
                {
                    if(DeltaCounter == 1u)
                    {
                        StatePtr->Status = E2E_P08STATUS_OK; /* SBSW_E2E_03 */
                    }
                    else
                    {
                        StatePtr->Status = E2E_P08STATUS_OKSOMELOST; /* SBSW_E2E_03 */
                    }
                }
                else
                {
                    StatePtr->Status = E2E_P08STATUS_REPEATED; /* SBSW_E2E_03 */
                }
            }
            else
            {
                StatePtr->Status = E2E_P08STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
            }

            StatePtr->Counter = ReceivedCounter; /* SBSW_E2E_03 */
        }
        else
        {
            StatePtr->Status = E2E_P08STATUS_ERROR; /* SBSW_E2E_03 */
        }
    }
} /* PRQA S 6080, 6060 */ /* MD_MSR_STMIF, MD_MSR_STPAR */

/**********************************************************************************************************************
 *  E2E_P08ComputeCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08ComputeCRC(
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST (uint32, AUTOMATIC ) Length,
    CONST (uint32, AUTOMATIC ) Offset
)
{
    uint32 crc;
    const uint32 crcStartValue = 0xFFFFFFFFu;

    /* #10 Check the position of the CRC within the given DataPtr */
    if (Offset > 0u)
    {
        /* #21 If the CRC is somewhere within the data, calculate CRC over data before and behind CRC */
        crc = Crc_CalculateCRC32P4 ( DataPtr, /* SBSW_E2E_03 */
                                    Offset,
                                    crcStartValue,
                                    TRUE
                                 );
        crc = Crc_CalculateCRC32P4 ( &DataPtr[Offset + 4u], /* SBSW_E2E_03 */
                                    (uint32)(Length - (uint32)Offset - 4u),
                                    crc,
                                    FALSE
                                 );
    }
    else
    {
        /* #22 Otherwise the CRC is positioned at the begining of the data, calculate CRC over the whole data */
        /* Offset is always 0 in this case, so left out in the CRC length calculation! */
        crc = Crc_CalculateCRC32P4 ( &DataPtr[Offset + 4u], /* SBSW_E2E_03 */
                                    (uint32)(Length - 4u),
                                    crcStartValue,
                                    TRUE
                                 );
    }

    return crc;
}

/**********************************************************************************************************************
 *  E2E_P08GetFourDataBytesBigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP08_LOCAL FUNC (uint32, E2E_CODE) E2E_P08GetFourDataBytesBigEndian(
    P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr, 
    CONST (uint32, AUTOMATIC) OffsetInBytes
)
{ 
    return ((uint32)(DataPtr[OffsetInBytes]) << 24u)
            | ((uint32)(DataPtr[OffsetInBytes + 1u]) << 16u)
            | ((uint32)(DataPtr[OffsetInBytes + 2u]) << 8u)
            | (DataPtr[OffsetInBytes + 3u]);
}

/**********************************************************************************************************************
 *  E2E_P08SetFourDataBytesBigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2EP08_LOCAL FUNC (void, E2E_CODE) E2E_P08SetFourDataBytesBigEndian(
    P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr, 
    CONST (uint32, AUTOMATIC) OffsetInBytes,
    CONST (uint32, AUTOMATIC) ValueToSet
)
{ 
    DataPtr[OffsetInBytes] = (uint8) (ValueToSet >> 24u);  /* SBSW_E2E_03 */
    DataPtr[OffsetInBytes + 1u] = (uint8) (ValueToSet >> 16u);  /* SBSW_E2E_03 */
    DataPtr[OffsetInBytes + 2u] = (uint8) (ValueToSet >> 8u);   /* SBSW_E2E_03 */
    DataPtr[OffsetInBytes + 3u] = (uint8) (ValueToSet & 0xFFu); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P08Protect()
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
FUNC (Std_ReturnType, E2E_CODE) E2E_P08Protect( 
    P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
    P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
    CONST (uint32, AUTOMATIC) Length
)
{
    /* #10 Verify inputs of the protect function */
    Std_ReturnType ret = E2E_P08ProtectVerifyInputs (ConfigPtr, StatePtr, DataPtr, Length); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        uint32 ComputedCRC;
        /* #31 Compute offset */
        uint32 ByteOffset = E2E_P08ConvertBitsToBytes(ConfigPtr->Offset);

        /* #32 Write Length */
        E2E_P08SetFourDataBytesBigEndian(DataPtr, ByteOffset + 4u, Length); /* SBSW_E2E_03 */

        /* #33 Write Counter */
        E2E_P08SetFourDataBytesBigEndian(DataPtr, ByteOffset + 8u, StatePtr->Counter); /* SBSW_E2E_03 */

        /* #34 Write DataId */
        E2E_P08SetFourDataBytesBigEndian(DataPtr, ByteOffset + 12u, ConfigPtr->DataId); /* SBSW_E2E_03 */

        /* #35 Compute CRC */
        ComputedCRC = E2E_P08ComputeCRC ( DataPtr,   /* SBSW_E2E_03 */
                                          Length,
                                          ByteOffset
                                        );

        /* #36 Write CRC */
        E2E_P08SetFourDataBytesBigEndian(DataPtr, ByteOffset, ComputedCRC); /* SBSW_E2E_03 */

        /* #37 Increment Counter. Implicit wrap-around 0xFFFFFFFF -> 0 */
        StatePtr->Counter++; /* SBSW_E2E_03 */
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08ProtectInit( 
    P2VAR (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Initialize Counter in case StatePtr is valid */
    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08Check()
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
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08Check( P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                              CONST (uint32, AUTOMATIC) Length)
{
    boolean NewDataAvailable = FALSE;
    /* #10 Verify inputs of the check function */
    Std_ReturnType ret = E2E_P08CheckVerifyInputs ( ConfigPtr, /* SBSW_E2E_02 */
                                                    StatePtr,
                                                    DataPtr,
                                                    Length,
                                                    &NewDataAvailable
                                   );

    if (ret == E2E_E_OK)
    {
        uint32 ReceivedLength = 0u;
        uint32 ReceivedCounter = 0u;
        uint32 ReceivedDataId = 0u;
        uint32 ReceivedCRC = 0u;
        uint32 ComputedCRC = 0u;
        /* #30 Check if new data is available */
        if (NewDataAvailable == TRUE) 
        {
            /* #31 Compute offset */
            uint32 ByteOffset = E2E_P08ConvertBitsToBytes(ConfigPtr->Offset);

            /* #32 Read Length */
            ReceivedLength = E2E_P08GetFourDataBytesBigEndian(DataPtr, ByteOffset + 4u); /* SBSW_E2E_02 */

            /* #33 Read Counter */
            ReceivedCounter = E2E_P08GetFourDataBytesBigEndian(DataPtr, ByteOffset + 8u); /* SBSW_E2E_02 */

            /* #34 Read DataId */
            ReceivedDataId = E2E_P08GetFourDataBytesBigEndian(DataPtr, ByteOffset + 12u); /* SBSW_E2E_02 */
            
            /* #35 Read CRC */
            ReceivedCRC = E2E_P08GetFourDataBytesBigEndian(DataPtr, ByteOffset); /* SBSW_E2E_02 */

            /* #36 Compute CRC */
            ComputedCRC = E2E_P08ComputeCRC ( DataPtr,   /* SBSW_E2E_03 */
                                              Length,
                                              ByteOffset
                                            );
        }

        /* #40 Do checks */
        /* In case no new data is available, the other parameters will not be used and may be invalid! */
        E2E_P08DoChecks( /* SBSW_E2E_03 */
            ConfigPtr, 
            StatePtr, 
            NewDataAvailable, 
            ReceivedCRC, 
            ComputedCRC, 
            ReceivedDataId, 
            Length, 
            ReceivedLength,
            ReceivedCounter
        );
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08CheckInit( 
    P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Reset State structure in case StatePtr is valid */
    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0xFFFFFFFFu; /* SBSW_E2E_03 */
        StatePtr->Status  = E2E_P08STATUS_ERROR; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P08MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P08MapStatusToSM(
    CONST (Std_ReturnType, AUTOMATIC) CheckReturn,
    CONST (E2E_P08CheckStatusType, AUTOMATIC) Status
)
{
    E2E_PCheckStatusType ret = E2E_P_ERROR;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
            case E2E_P08STATUS_OK           : ret = E2E_P_OK; break;
            case E2E_P08STATUS_OKSOMELOST   : ret = E2E_P_OK; break;
            case E2E_P08STATUS_REPEATED     : ret = E2E_P_REPEATED; break;
            case E2E_P08STATUS_NONEWDATA    : ret = E2E_P_NONEWDATA; break;
            case E2E_P08STATUS_WRONGSEQUENCE: ret = E2E_P_WRONGSEQUENCE; break;
            default: /* E2E_P08STATUS_ERROR */
              break;
        }
    }

    return ret;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P08.c
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  E2E_P22.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 22
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for
 *                safety-related communication having requirements up to ASIL D. 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P22.h"


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#define E2E_P22_MAX_COUNTER_VALUE 0xFu /*!< Max value the counter for protecting the data can have. */


/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (E2E_P22_LOCAL) /* COV_E2E_COMPATIBILITY */
# define E2E_P22_LOCAL static
#endif

#if !defined (E2E_P22_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
# define E2E_P22_LOCAL_INLINE LOCAL_INLINE
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  E2E_P22ProtectVerifyInputs
 *********************************************************************************************************************/
/*! \brief          Checks parameters of protect functions.
 *  \details        -
 *  \param[in]      ConfigPtr Pointer to static configuration.
 *  \param[in]      StatePtr  Pointer to port/data communication state.
 *  \param[in]      DataPtr   Pointer to Data to be transmitted.
 *  \param[in]      Length    Length of the data in bytes.
 *  \return         E2E_E_OK: Function completed successfully.
 *  \return         E2E_E_INPUTERR_NULL: At least one pointer parameter is a NULL pointer.
 *  \return         E2E_E_INPUTERR_WRONG: Passed length and length in ConfigPtr do not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22ProtectVerifyInputs(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
);

/**********************************************************************************************************************
 *  E2E_P22CheckVerifyInputs
 *********************************************************************************************************************/
/*! \brief          Checks parameters of check functions.
 *  \details        -
 *  \param[in]      ConfigPtr           Pointer to static configuration.
 *  \param[in]      StatePtr            Pointer to port/data communication state.
 *  \param[in]      DataPtr             Pointer to received data.
 *  \param[in]      Length              Length of the data in bytes.
 *  \param[out]     NewDataAvailablePtr Pointer to a variable which stores if new data is available. 
 *  \return         E2E_E_OK: Function completed successfully.
 *  \return         E2E_E_INPUTERR_NULL: At least one pointer parameter is a NULL pointer.
 *  \return         E2E_E_INPUTERR_WRONG: Passed length and length in ConfigPtr do not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22CheckVerifyInputs(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(boolean, AUTOMATIC, E2E_APPL_DATA) NewDataAvailablePtr
);

/**********************************************************************************************************************
 *  E2E_P22ComputeCRC
 *********************************************************************************************************************/
/*! \brief          Calculates the CRC checksum.
 *  \details        -
 *  \param[in]      DataPtr        Pointer to the data to be protected.
 *  \param[in]      Length         Length of the data in bytes.
 *  \param[in]      Offset         Offset of the E2E header in bits.
 *  \param[in]      LocalDataIdPtr Pointer to Data ID for protection against masquerading.
 *  \return         CRC value.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(uint8, E2E_CODE) E2E_P22ComputeCRC(
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
  CONST(uint16, AUTOMATIC) Length,
  CONST(uint16, AUTOMATIC) Offset,
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) LocalDataIdPtr
);

/**********************************************************************************************************************
 *  E2E_P22DoChecks
 *********************************************************************************************************************/
/*! \brief          Checks if the checksums match and if delta counter is acceptable.
 *  \details        -
 *  \param[in]      ReceivedCRC      Received CRC value.
 *  \param[in]      CalculatedCRC    CRC value calculated from the received data.
 *  \param[in,out]  StatePtr         Pointer to the the current state.
 *  \param[in]      ReceivedCounter  Received counter value.
 *  \param[in]      NewDataAvailable Value if new data was received.
 *  \return         Status of the received data.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/

FUNC(E2E_P22CheckStatusType, E2E_CODE) E2E_P22DoChecks(
  CONST(uint8, AUTOMATIC) ReceivedCRC,
  CONST(uint8, AUTOMATIC) ComputedCRC,
  P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
  CONST(uint8, AUTOMATIC) ReceivedCounter,
  CONST(uint8, AUTOMATIC) MaxDeltaCounter,
  CONST(boolean, AUTOMATIC) NewDataAvailable
);


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P22ProtectVerifyInputs
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P22ProtectVerifyInputs(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
)
{
  Std_ReturnType returnValue = E2E_E_INPUTERR_NULL;

  if((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR))
  {
    if(Length == (ConfigPtr->DataLength >> 3u))
    {
      returnValue = E2E_E_OK;
    }   
    else
    {
      returnValue = E2E_E_INPUTERR_WRONG;
    }
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  E2E_P22CheckVerifyInputs
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
 FUNC(Std_ReturnType, E2E_CODE) E2E_P22CheckVerifyInputs(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length,
    P2VAR(boolean, AUTOMATIC, E2E_APPL_DATA) NewDataAvailablePtr
) 
{
  Std_ReturnType returnValue = E2E_E_INPUTERR_NULL;
  (*NewDataAvailablePtr) = FALSE; /* SBSW_E2E_04 */

  if((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
  {
    if(((DataPtr != NULL_PTR) && (Length != 0u))
      || ((DataPtr == NULL_PTR) && (Length == 0u)))
    {
      if(DataPtr != NULL_PTR)
      {
        if(Length == (ConfigPtr->DataLength >> 3u))
        {
          (*NewDataAvailablePtr) = TRUE; /* SBSW_E2E_04 */
          returnValue = E2E_E_OK;
        }
        else
        {
          returnValue = E2E_E_INPUTERR_WRONG;
        }
      }
      else
      {
        returnValue = E2E_E_OK;
      }
    }
    else  
    {
      returnValue = E2E_E_INPUTERR_WRONG;
    }
  }

  return returnValue;
}


/**********************************************************************************************************************
 *  E2E_P22ComputeCRC
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

FUNC(uint8, E2E_CODE) E2E_P22ComputeCRC(
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
  CONST(uint16, AUTOMATIC) Length,
  CONST(uint16, AUTOMATIC) Offset,
  P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) LocalDataIdPtr
)
{
  uint8 crcValue = 0u;
  uint16 offsetInByte = Offset >> 3u;

  if(offsetInByte > 0u)
  {
    crcValue = Crc_CalculateCRC8H2F ( /* SBSW_E2E_03 */
       &DataPtr[0], 
       (uint32)offsetInByte, 
       0xFFu, 
       TRUE
       );

    if(Length > (offsetInByte + 1u))
    {
      crcValue = Crc_CalculateCRC8H2F ( /* SBSW_E2E_03 */
        &DataPtr[offsetInByte + 1u], 
        (uint32)Length - (uint32)offsetInByte - 1u, 
        crcValue, 
        FALSE
        );
    }
  }
  else
  {
    crcValue = Crc_CalculateCRC8H2F ( /* SBSW_E2E_03 */
      &DataPtr[1], 
      (uint32)Length - 1u, 
      0xFFu, 
      TRUE
      );
  }

  crcValue = Crc_CalculateCRC8H2F ( /* SBSW_E2E_03 */
    LocalDataIdPtr, 
    1u, 
    crcValue, 
    FALSE
    );

  return crcValue;
}

/**********************************************************************************************************************
 *  E2E_P22DoChecks
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

FUNC(E2E_P22CheckStatusType, E2E_CODE) E2E_P22DoChecks(
  CONST(uint8, AUTOMATIC) ReceivedCRC,
  CONST(uint8, AUTOMATIC) ComputedCRC,
  P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
  CONST(uint8, AUTOMATIC) ReceivedCounter,
  CONST(uint8, AUTOMATIC) MaxDeltaCounter,
  CONST(boolean, AUTOMATIC) NewDataAvailable
)
{
  E2E_P22CheckStatusType returnValue = E2E_P22STATUS_NONEWDATA;
  uint8 stateCounter = StatePtr->Counter;

  if(NewDataAvailable == TRUE)
  {
    if(ReceivedCRC == ComputedCRC)
    {
      uint8 deltaCounter;
      if(ReceivedCounter >= stateCounter) /* No wrap around occured. */
      {
        deltaCounter = ReceivedCounter - stateCounter;
      }
      else /* Wrap around occured. */
      {
        deltaCounter = ReceivedCounter + (E2E_P22_MAX_COUNTER_VALUE - stateCounter) + 1u;
      }

      if(deltaCounter <= MaxDeltaCounter)
      {
        if(deltaCounter > 0u)
        {
          if(deltaCounter == 1u)
          {
            returnValue = E2E_P22STATUS_OK;
          }
          else
          {
            returnValue = E2E_P22STATUS_OKSOMELOST;
          }
        }
        else
        {
          returnValue = E2E_P22STATUS_REPEATED;      
        }
      }
      else
      {
        returnValue = E2E_P22STATUS_WRONGSEQUENCE;
      }

      StatePtr->Counter = ReceivedCounter; /* SBSW_E2E_03 */
    }
    else
    {
      returnValue = E2E_P22STATUS_ERROR;
    }
  }

  return returnValue;
} /* PRQA S 6080, 6060 */ /* MD_MSR_STMIF, MD_MSR_STPAR */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P22Protect
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P22Protect(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
)
{
  Std_ReturnType returnValue = E2E_P22ProtectVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length); /* SBSW_E2E_02 */

  if(returnValue == E2E_E_OK)
  {
    uint8 crcValue;
    const uint16 counterOffset = (ConfigPtr->Offset >> 3u) + 1u;
    const uint16 crcOffset = ConfigPtr->Offset >> 3u;

    /* Write counter */
    DataPtr[counterOffset] = (DataPtr[counterOffset] & (uint8)0xF0u) | (StatePtr->Counter & ((uint8)0x0Fu)); /* SBSW_E2E_05 */

    /* Compute CRC */
    crcValue = E2E_P22ComputeCRC(DataPtr, Length, ConfigPtr->Offset, &(ConfigPtr->DataIDList[StatePtr->Counter])); /* SBSW_E2E_03 */

    /* Write CRC */
    DataPtr[crcOffset] = crcValue; /* SBSW_E2E_03 */
    
    /* Increment Counter */
    StatePtr->Counter = (StatePtr->Counter + 1u) % (E2E_P22_MAX_COUNTER_VALUE + 1u); /* SBSW_E2E_03 */
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  E2E_P22ProtectInit
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 FUNC(Std_ReturnType, E2E_CODE) E2E_P22ProtectInit(
    P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
  Std_ReturnType returnValue = E2E_E_INPUTERR_NULL;

  if(StatePtr != NULL_PTR)
  {
    StatePtr->Counter = 0u; /* SBSW_E2E_03 */
    returnValue = E2E_E_OK;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  E2E_P22Check
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P22Check(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
)
{
  boolean newDataAvailable = FALSE;

  Std_ReturnType returnValue = E2E_P22CheckVerifyInputs(ConfigPtr, StatePtr, DataPtr, Length, &newDataAvailable); /* SBSW_E2E_02 */

  if(returnValue == E2E_E_OK)
  {
    uint8 crcReceivedValue = 0;
    uint8 crcCalculatedValue = 0;
    uint8 receivedCounter = 0;

    if(newDataAvailable == TRUE)
    {
      const uint16 counterOffset = (ConfigPtr->Offset >> 3u) + 1u;
      const uint16 crcOffset = ConfigPtr->Offset >> 3u;

      /* Read counter */
      receivedCounter = DataPtr[counterOffset] & ((uint8) 0x0Fu);
      
      /* Read CRC */
      crcReceivedValue = DataPtr[crcOffset];
  
      /* Compute CRC */
      crcCalculatedValue = E2E_P22ComputeCRC(DataPtr, Length, ConfigPtr->Offset, &(ConfigPtr->DataIDList[receivedCounter])); /* SBSW_E2E_02 */
    }

    /* Do checks */
    StatePtr->Status = E2E_P22DoChecks(crcReceivedValue, crcCalculatedValue, StatePtr, receivedCounter, ConfigPtr->MaxDeltaCounter, newDataAvailable); /* SBSW_E2E_02 */ /* SBSW_E2E_03 */ 
  }

  return returnValue;
}
      
/**********************************************************************************************************************
 *  E2E_P22CheckInit
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
FUNC(Std_ReturnType, E2E_CODE) E2E_P22CheckInit(
    P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
  Std_ReturnType returnValue = E2E_E_INPUTERR_NULL;

  if(StatePtr != NULL_PTR)
  {
    StatePtr->Counter = 0xFu; /* SBSW_E2E_03 */
    StatePtr->Status = E2E_P22STATUS_ERROR; /* SBSW_E2E_03 */
    returnValue = E2E_E_OK;
  }

  return returnValue;
}


/**********************************************************************************************************************
 *  E2E_P22MapStatusToSM
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P22MapStatusToSM(
    VAR(Std_ReturnType, AUTOMATIC) CheckReturn,
    VAR(E2E_P22CheckStatusType, AUTOMATIC) Status
)
{
  E2E_PCheckStatusType returnValue = E2E_P_ERROR;

  if(CheckReturn == E2E_E_OK)
  {
    switch(Status)
    {
    case E2E_P22STATUS_OK: /* The fall through is intentional. */
    case E2E_P22STATUS_OKSOMELOST:
      returnValue = E2E_P_OK;
      break;
    case E2E_P22STATUS_REPEATED:
      returnValue = E2E_P_REPEATED;
      break;
    case E2E_P22STATUS_NONEWDATA:
      returnValue = E2E_P_NONEWDATA;
      break;
    case E2E_P22STATUS_WRONGSEQUENCE:
      returnValue = E2E_P_WRONGSEQUENCE;
      break;
    default: /* E2E_P22STATUS_ERROR */
      /* returnValue has already E2E_P_ERROR assigned. */
      break;
    }
  }

  return returnValue;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P22.c
 *********************************************************************************************************************/

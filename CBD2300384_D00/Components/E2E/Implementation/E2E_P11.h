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
 *         \file  E2E_P11.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 11
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P11_H)
# define E2E_P11_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*!
 * Configuration of DataId usage for ConfigType.
 */
typedef enum
{
  E2E_P11_DATAID_BOTH   = 0u,
  E2E_P11_DATAID_NIBBLE = 3u
} E2E_P11DataIDMode;

/*!
 * Configuration of transmitted Data (Data Element or I-PDU), for E2E profile 11. For each transmitted Data, there is
 * an instance of this typedef.
 */
typedef struct
{
  uint16            CounterOffset;            /*!< Bit offset of Counter in MSB first order.
                                                   In variants 1A and 1B, CounterOffset is 8.
                                                   Shall be a multiple of 4.*/
  uint16            CRCOffset;                /*!< Bit offset of CRC in MSB first order.
                                                   In variants 1A and 1B, CRCOffset is 0.
                                                   Shall be a multiple of 8. */
  uint16            DataID;                   /*!< Unique identifier for protection against masquerading. */
  uint16            DataIDNibbleOffset;       /*!< Bit offset of the low nibble of the high byte of DataID. */
  E2E_P11DataIDMode DataIDMode;               /*!< Inclusion mode of DataID for E2E_P11Check/E2E_P11Protect. */
  uint16            DataLength;               /*!< Length of data in bits. Shall be a multiple of 8 and <= 240 */
  uint8             MaxDeltaCounter;          /*!< Maximum allowed gap between to counter values of two consecutively
                                                   valid data. */
} E2E_P11ConfigType;

/*!
 * State of the sender for a Data protected with E2E profile 11.
 */
typedef struct
{
  uint8  Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P11ProtectStateType;

/*!
 * Status of the reception on one single Data in one cycle, protected with E2E profile 11.
 */
typedef enum
{
  E2E_P11STATUS_OK            = 0x00u,  /*!< OK:    The checks of the Data in this cycle were successful. */
  E2E_P11STATUS_NONEWDATA     = 0x01u,  /*!< Error: The Check function has been invoked but no new Data is available
                                                    since the last call, according to communication medium. */
  E2E_P11STATUS_ERROR         = 0x07u,  /*!< Error: Error not related to counters occurred. */
  E2E_P11STATUS_REPEATED      = 0x08u,  /*!< Error: The checks of the Data in this cycle were successful, with the
                                                    exception of the repetition. */
  E2E_P11STATUS_OKSOMELOST    = 0x20u,  /*!< OK:    The checks of the Data on this cycle were successful (including
                                                    counter check). */
  E2E_P11STATUS_WRONGSEQUENCE = 0x40u   /*!< Error: The checks of the Data in this cycle were successful, with the
                                                    exception of counter jump, which changed more than the allowed
                                                    delta. */
} E2E_P11CheckStatusType;

/*!
 * State of the reception on one single Data protected with E2E profile 11.
 */
typedef struct
{
  E2E_P11CheckStatusType Status;  /*!< Result of the verification of the Data in this cycle, determined by the Check
                                       function. */
  uint8                  Counter; /*!< Counter of the data in previous cycle. */
} E2E_P11CheckStateType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2E_START_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P11Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protects the array/buffer to be transmitted using the E2E profile 11.
 * \details         Includes checksum calculation and handling of counter.
 * \param[in]       ConfigPtr   Pointer to static configuration.
 * \param[in,out]   StatePtr    Pointer to port/data communication state.
 * \param[in,out]   DataPtr     Pointer to Data to be transmitted.
 * \param[in]       Length      Length of the data in bytes.
 * \return          E2E_E_INPUTERR_NULL:  A parameter is NULL.
 * \return          E2E_E_INPUTERR_WRONG: At least one input parameter is erroneous.
 * \return          E2E_E_OK:             Function completed successfully.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11Protect(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    VAR(uint16, AUTOMATIC) Length);

/**********************************************************************************************************************
 *  E2E_P11ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Initializes the protection state.
 * \details         -
 * \param[in,out]   StatePtr  Pointer to port/data communication state.
 * \return          E2E_E_INPUTERR_NULL: StatePtr is NULL.
 * \return          E2E_E_OK:            Success.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11ProtectInit(P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);

/**********************************************************************************************************************
 *  E2E_P11Check()
 *********************************************************************************************************************/
/*!
 * \brief           Checks the Data received using the E2E profile 11.
 * \details         This includes CRC calculation, handling of Counter.
 *                  The function checks only one single data in one cycle, it does not determine/compute the
 *                  accumulated state of the communication link.
 * \param[in]       ConfigPtr             Pointer to static configuration.
 * \param[in,out]   StatePtr              Pointer to port/data communication state.
 * \param[in]       DataPtr               Pointer to received data.
 * \param[in]       Length                Length of the data in bytes.
 * \return          E2E_E_INPUTERR_NULL:  A parameter is NULL.
 * \return          E2E_E_INPUTERR_WRONG: At least one input parameter is erroneous.
 * \return          E2E_E_OK:             Function completed successfully.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11Check(
    P2CONST(E2E_P11ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    VAR(uint16, AUTOMATIC) Length);

/**********************************************************************************************************************
 *  E2E_P11CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Initializes the check state.
 * \details         -
 * \param[in,out]   StatePtr    Pointer to port/data communication state.
 * \return          E2E_E_INPUTERR_NULL: StatePtr is NULL.
 * \return          E2E_E_OK:            Success.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P11CheckInit(P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);

/**********************************************************************************************************************
 *  E2E_P11MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           The function maps the check status of profile 11 to a generic check status, which can be used by
 *                  E2E state machine check function.
 * \details         The E2E profile 11 delivers a more fine-granular status, but this is not relevant for the E2E
 *                  state machine.
 * \param[in]       CheckReturn   Return value of the E2E_P11Check function.
 * \param[in]       Status        Status determined by E2E_P11Check function.
 * \return          Standard state value to be used in E2Elib state machine.
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P11MapStatusToSM(
    VAR(Std_ReturnType, AUTOMATIC) CheckReturn,
    VAR(E2E_P11CheckStatusType, AUTOMATIC) Status);

# define E2E_STOP_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_P11_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P11.h
 *********************************************************************************************************************/

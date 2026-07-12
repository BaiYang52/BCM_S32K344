/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_P22.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 22
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P22_H)
# define E2E_P22_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct {
    uint16 DataLength; /*!< Length of Data, in bits */
    uint8  DataIDList[16]; /*!< Array of appropriately chosen Data IDs for protection against masquerading */
    uint8  MaxDeltaCounter; /*!< Maximum allowed gap between two counter values of two consecutively received valid Data */
    uint16 Offset; /*!< Offset of the E2E header in the Data[] array, in bits */
} E2E_P22ConfigType;

typedef struct {
    uint8  Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P22ProtectStateType;

typedef enum {
    E2E_P22STATUS_OK = 0x00,
    E2E_P22STATUS_NONEWDATA = 0x01,
    E2E_P22STATUS_ERROR = 0x07,
    E2E_P22STATUS_REPEATED = 0x08,
    E2E_P22STATUS_OKSOMELOST = 0x20,
    E2E_P22STATUS_WRONGSEQUENCE = 0x40
} E2E_P22CheckStatusType;

typedef struct {
    uint8 Counter;                 /*!< Counter of last valid received message */
    E2E_P22CheckStatusType Status; /*!< Result of the verification of the Data */
} E2E_P22CheckStateType;


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2E_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P22Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile 22
 * \details         Protects the array/buffer to be transmitted using the E2E Profile 22. This includes checksum calculation,
 *                  handling of counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P22Protect( P2CONST (E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                VAR (uint16, AUTOMATIC) Length);


/**********************************************************************************************************************
 *  E2E_P22ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile 22
 * \details         Initializes the protection state by resetting the counter
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
/* Set any element of the struct to mark the struct initialized. */
# define E2E_P22ProtectInit(StatePtr) 0; (StatePtr)->Counter = 0


/**********************************************************************************************************************
 *  E2E_P22Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile 22
 * \details         Checks the Data received using the E2E Profile 22. This includes CRC calculation, handling of
 *                  the counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in]       DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P22Check( P2CONST (E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                              VAR (uint16, AUTOMATIC) Length);


/**********************************************************************************************************************
 *  E2E_P22CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile 22
 * \details         Check initialization function of E2Elib Profile 22
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
/* Set any element of the struct to mark the struct initialized. */
# define E2E_P22CheckInit(StatePtr) 0; (StatePtr)->Counter = 0


/**********************************************************************************************************************
 *  E2E_P22MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile 22
 * \details         Maps the check status of Profile 22 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile 22 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_P22Check
 * \param[in]       Status          communication state
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 * \trace           SPEC-2056631
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P22MapStatusToSM (VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR(E2E_P22CheckStatusType, AUTOMATIC) Status);

# define E2E_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_P22_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P22.h
 *********************************************************************************************************************/

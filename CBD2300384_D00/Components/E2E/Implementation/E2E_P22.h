/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  E2E_P22.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 22
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for
 *                safety-related communication having requirements up to ASIL D. 
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P22_H)
# define E2E_P22_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! 
 * Non-modifiable configuration of the data element sent over an RTE port, for E2E profile 22.
 */
typedef struct
{
    uint16 DataLength;                 /*!< Length of Data, in bits. The value shall be a multiple of 8. */
    uint8  DataIDList[16];             /*!< An array of appropriately chosen Data IDs for protection against 
                                            masquerading. */
    uint8  MaxDeltaCounter;            /*!< Initial maximum allowed gap between two counter values of two 
                                            consecutively received valid Data. */
    uint16 Offset;                     /*!< Offset of the E2E header in the Data[] array in bits. */
} E2E_P22ConfigType;

/*! 
 * State of the sender for a Data protected with E2E Profile 22.
 */
typedef struct
{
    uint8 Counter;                     /*!< Counter to be used for protecting the Data. */
} E2E_P22ProtectStateType;

/*! 
 * Result of the verification of the Data in E2E Profile 22, determined by the Checkfunction.
 */
typedef enum
{
    E2E_P22STATUS_OK            = 0x0,      /*!< OK:    The checks of the Data in this cycle were successful. */
    E2E_P22STATUS_NONEWDATA     = 0x01,     /*!< Error: The Check function has been invoked but no new Data is available
                                                        since the last call, according to communication medium. */
    E2E_P22STATUS_ERROR         = 0x07,     /*!< Error: Error not related to counters occurred. */
    E2E_P22STATUS_REPEATED      = 0x08,     /*!< Error: The checks of the Data in this cycle were successful, with the
                                                        exception of the repetition. */
    E2E_P22STATUS_OKSOMELOST    = 0x20,     /*!< OK:    The checks of the Data on this cycle were successful (including
                                                        counter check). */
    E2E_P22STATUS_WRONGSEQUENCE = 0x40      /*!< Error: The checks of the Data in this cycle were successful, with the
                                                        exception of counter jump, which changed more than the allowed
                                                        delta. */
} E2E_P22CheckStatusType;

/*! 
 * State of the sender for a Data protected with E2E Profile 22.
 */
typedef struct
{
    uint8 Counter;                     /*!< Counter of last valid received message. */
    E2E_P22CheckStatusType Status;     /*!< Result of the verification of the Data, determined by the Check function. */
} E2E_P22CheckStateType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define E2E_START_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P22Protect
 *********************************************************************************************************************/
/*! \brief          Protection function of E2Elib Profile 22.
 *  \details        Protects the array/buffer to be transmitted using the E2E profile 22. This includes
 *                  checksum calculation, handling of counter.
 *  \param[in]      ConfigPtr Pointer to static configuration.
 *  \param[in,out]  StatePtr  Pointer to port/data communication state.
 *  \param[in,out]  DataPtr   Pointer to Data to be transmitted.
 *  \param[in]      Length    Length of the data in bytes.
 *  \return         E2E_E_OK: Function completed successfully.
 *  \return         E2E_E_INPUTERR_NULL: At least one pointer parameter is a NULL pointer.
 *  \return         E2E_E_INPUTERR_WRONG: At least one input parameter is erroneous.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-1086
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22Protect(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
);

/**********************************************************************************************************************
 *  E2E_P22ProtectInit
 *********************************************************************************************************************/
/*! \brief          Protection initialization function of E2Elib Profile 22.
 *  \details        Initializes the protection state.
 *  \param[out]     StatePtr Pointer to port/data communication state.
 *  \return         E2E_E_INPUTERR_NULL: Pointer parameter is NULL.
 *  \return         E2E_E_OK: Initialization successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-1087
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22ProtectInit(
    P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**********************************************************************************************************************
 *  E2E_P22Check
 *********************************************************************************************************************/
/*! \brief          Check function of E2Elib Profile 22.
 *  \details        Checks the Data received using the E2E profile 22. This includes CRC calculation,
 *                  handling of Counter.
 *  \param[in]      ConfigPtr Pointer to static configuration.
 *  \param[in,out]  StatePtr  Pointer to port/data communication state.
 *  \param[in]      DataPtr   Pointer to received data.
 *  \param[in]      Length    Length of the data in bytes.
 *  \return         E2E_E_OK: Function completed successfully.
 *  \return         E2E_E_INPUTERR_NULL: At least one pointer parameter is a NULL pointer.
 *  \return         E2E_E_INPUTERR_WRONG: At least one input parameter is erroneous.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-1088
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22Check(
    P2CONST(E2E_P22ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    CONST(uint16, AUTOMATIC) Length
);

/**********************************************************************************************************************
 *  E2E_P22CheckInit
 *********************************************************************************************************************/
/*! \brief          Check initialization function of E2Elib Profile 22.
 *  \details        Initializes the check state.
 *  \param[in,out]  StatePtr Pointer to port/data communication state.
 *  \return         E2E_E_INPUTERR_NULL: Pointer parameter is NULL.
 *  \return         E2E_E_OK: Initialization successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-1089
 *********************************************************************************************************************/
FUNC(Std_ReturnType, E2E_CODE) E2E_P22CheckInit(
    P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**********************************************************************************************************************
 *  E2E_P22MapStatusToSM
 *********************************************************************************************************************/
/*! \brief          State mapping function of E2Elib Profile 22.
 *  \details        The function maps the check status of Profile 22 to a generic check status, which
 *                  can be used by E2E state machine check function. The E2E Profile 22 delivers a
 *                  more fine-granular status, but this is not relevant for the E2E state machine.
 *  \param[in]      CheckReturn Return value of the E2E_P22Check function.
 *  \param[in]      Status      Status determined by E2E_P22Check function.
 *  \return         Standard state value to be used in E2Elib state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-1175
 *********************************************************************************************************************/
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P22MapStatusToSM(
    VAR(Std_ReturnType, AUTOMATIC) CheckReturn,
    VAR(E2E_P22CheckStatusType, AUTOMATIC) Status
);

# define E2E_STOP_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* E2E_P22_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P22.h
 *********************************************************************************************************************/

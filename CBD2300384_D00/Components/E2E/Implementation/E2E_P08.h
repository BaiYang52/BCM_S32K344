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
 *         \file  E2E_P08.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 8
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults 
 *                within the communication link. E2E Library provides mechanisms for E2E protection, adequate for 
 *                safety-related communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P08_H)
# define E2E_P08_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/ 
typedef struct 
{
    uint32          DataId;             /*!< Unique identifier for protection against masquerading */
    uint32          Offset;             /*!< Bit offset of the first bit of the E2E header from the beginning */
    uint32          MinDataLength;      /*!< Minimal length of Data, in bits */
    uint32          MaxDataLength;      /*!< Maximal length of Data, in bits */
    uint32          MaxDeltaCounter;    /*!< Maximum allowed difference between two counter values of consecutively 
                                             received valid messages */
} E2E_P08ConfigType;

typedef struct 
{
    uint32  Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P08ProtectStateType;

typedef enum 
{
    E2E_P08STATUS_OK            = 0x00u,
    E2E_P08STATUS_NONEWDATA     = 0x01u,
    E2E_P08STATUS_ERROR         = 0x07u,
    E2E_P08STATUS_REPEATED      = 0x08u,
    E2E_P08STATUS_OKSOMELOST    = 0x20u,
    E2E_P08STATUS_WRONGSEQUENCE = 0x40u
} E2E_P08CheckStatusType; /*!< Verification type of the data */

typedef struct 
{
    uint32 Counter; /*!< Counter of the data in previous cycle. */
    E2E_P08CheckStatusType Status; /*!< Result of the verification of the Data */
} E2E_P08CheckStateType;



/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2E_START_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P08Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile 8
 * \details         Protects the array/buffer to be transmitted using the E2E Profile 8. This includes checksum 
 *                  calculation, handling of counter and Data ID.
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
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08Protect( P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                CONST (uint32, AUTOMATIC) Length);


/**********************************************************************************************************************
 *  E2E_P08ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile 8
 * \details         Initializes the protection state by resetting the counter
 * \param[out]      StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08ProtectInit( P2VAR (E2E_P08ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);

/**********************************************************************************************************************
 *  E2E_P08Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile 8
 * \details         Checks the Data received using the E2E Profile 8. This includes CRC calculation, handling of
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
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08Check( P2CONST (E2E_P08ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                              CONST (uint32, AUTOMATIC) Length);

/**********************************************************************************************************************
 *  E2E_P08CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile 8
 * \details         -
 * \param[out]      StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P08CheckInit( P2VAR (E2E_P08CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);

/**********************************************************************************************************************
 *  E2E_P08MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile 8
 * \details         Maps the check status of Profile 8 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile 8 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_P08Check
 * \param[in]       Status          communication state
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P08MapStatusToSM (CONST (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            CONST (E2E_P08CheckStatusType, AUTOMATIC) Status);

# define E2E_STOP_SEC_CODE
# include "E2E_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* E2E_P08_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P08.h
 *********************************************************************************************************************/

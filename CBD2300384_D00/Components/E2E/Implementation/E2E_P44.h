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
 *         \file  E2E_P44.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 44
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined(E2E_P44_H)
#  define E2E_P44_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#  include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint16 E2E_P44LengthType; /*!< Length in bytes */
typedef uint16 E2E_P44CounterType;
typedef uint32 E2E_P44OffsetType;
typedef uint32 E2E_P44CrcType;
typedef uint32 E2E_P44IdType;
typedef P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_P44DataPtrType;
typedef P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) E2E_P44ConstDataPtrType;


typedef struct
{
  E2E_P44IdType DataID;     /*!< Unique identifier for protection against masquerading */
  E2E_P44OffsetType Offset; /*!< Bit offset of the first bit of the E2E header from the beginning of the Data */
  uint32 MinDataLength;     /*!< Minimal length of Data, in bits */
  uint32 MaxDataLength;     /*!< Maximal length of Data, in bits */
  uint16 MaxDeltaCounter;   /*!< Maximum allowed difference between two counter values of consecutively received valid messages */
} E2E_P44ConfigType;

typedef struct
{
  uint16 Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P44ProtectStateType;

typedef enum
{
  E2E_P44STATUS_OK            = 0x00,
  E2E_P44STATUS_NONEWDATA     = 0x01,
  E2E_P44STATUS_ERROR         = 0x07,
  E2E_P44STATUS_REPEATED      = 0x08,
  E2E_P44STATUS_OKSOMELOST    = 0x20,
  E2E_P44STATUS_WRONGSEQUENCE = 0x40
} E2E_P44CheckStatusType; /*!< Enum containing all possible statuses */

typedef struct
{
  E2E_P44CheckStatusType Status; /*!< Result of verification of the Data in this cycle */
  uint16 Counter;                /*!< Counter of the Data in previous cycle */
} E2E_P44CheckStateType;

/**********************************************************************************************************************
 *  Typedefs of structures, cannot be used before the declaration of structs
 *********************************************************************************************************************/
typedef P2CONST(E2E_P44ConfigType, AUTOMATIC, E2E_APPL_CONST) E2E_P44ConstConfigPtrType;
typedef P2VAR(E2E_P44ProtectStateType, AUTOMATIC, E2E_APPL_DATA) E2E_P44ProtectStatePtrType;
typedef P2VAR(E2E_P44CheckStateType, AUTOMATIC, E2E_APPL_DATA) E2E_P44CheckStatePtrType;
typedef P2CONST(E2E_P44CheckStateType, AUTOMATIC, E2E_APPL_DATA) E2E_P44ConstCheckStatePtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define E2E_START_SEC_CODE
#  include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_P44Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile 44
 * \details         Protects the array/buffer to be transmitted using the E2E Profile 44. This includes checksum calculation,
 *                  handling of counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 *                  E2E_E_INPUTERR_WRONG: One parameter is erroneous
 *                  E2E_E_INTERR: Internal library error
 *                  E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44Protect(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44ProtectStatePtrType StatePtr,
    E2E_P44DataPtrType DataPtr,
    const E2E_P44LengthType Length
);

/**********************************************************************************************************************
 *  E2E_P44ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile 44
 * \details         Initializes the protection state by resetting the counter
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 *                  E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44ProtectInit(E2E_P44ProtectStatePtrType StatePtr);

/**********************************************************************************************************************
 *  E2E_P44Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile 44
 * \details         Checks the Data received using the E2E Profile 44. This includes CRC calculation, handling of
 *                  the counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in]       DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 *                  E2E_E_INPUTERR_WRONG: One parameter is erroneous
 *                  E2E_E_INTERR: Internal library error
 *                  E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44Check(
    E2E_P44ConstConfigPtrType ConfigPtr,
    E2E_P44CheckStatePtrType StatePtr,
    E2E_P44ConstDataPtrType DataPtr,
    const E2E_P44LengthType Length
);

/**********************************************************************************************************************
 *  E2E_P44CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile 44
 * \details         -
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 *                  E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_P44CheckInit(E2E_P44CheckStatePtrType StatePtr);

/**********************************************************************************************************************
 *  E2E_P44MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile 44
 * \details         Maps the check status of Profile 44 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile 44 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_P44Check
 * \param[in]       Status          communication state
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 * \trace           SPEC-2056631
 */
FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P44MapStatusToSM(
    const Std_ReturnType CheckReturn, 
    const E2E_P44CheckStatusType Status
);

#  define E2E_STOP_SEC_CODE
#  include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* E2E_P44_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P44.h
 *********************************************************************************************************************/

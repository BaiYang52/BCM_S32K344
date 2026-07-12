/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_Lin.c
 *        \brief  vItaHlp functionalities concerning LIN.
 *
 *      \details  -
 *
 *    \attention  Please note:
 *                The demo and example programs only show special aspects of the software. With regard to the fact
 *                that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *                expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define VITAHLP_LIN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if (VITAHLP_LIN == STD_ON)
# include "vItaHlp_CommonUtils.h"
# include "SchM_vItaHlp.h"
# include "vstdlib.h"
# include "vItaHlp_Lin.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Definitions used for the setup test */
# define VITAHLP_LIN_SETUPTEST_PAYLOADLENGTH                (8u)
# define VITAHLP_LIN_SETUPTEST_FRAME_PID                    (0xC4u)  /* ID = 4 */

/* Definitions used for the tests */
# define VITAHLP_LIN_TEST_PAYLOADLENGTH                     (4u)
# define VITAHLP_LIN_MASTER_MASTERRESPONSE_FRAME_PID        (0x80u)  /* ID = 0 */
# define VITAHLP_LIN_MASTER_SLAVERESPONSE_FRAME_PID         (0xC1u)  /* ID = 1 */
# define VITAHLP_LIN_SLAVE_MASTERRESPONSE_FRAME_PID         (0x06u)  /* ID = 6 */
# define VITAHLP_LIN_SLAVE_SLAVERESPONSE_FRAME_PID          (0x85u)  /* ID = 5 */
# define VITAHLP_LIN_SLAVE_IGNORERESPONSE_FRAME_PID         (0x42u)  /* ID = 2 */

/* Definitions used for extracting data from commands and for inserting data in events */
# define VITAHLP_LIN_MAX_NUMBER_DRIVER                      (8u)
# define VITAHLP_LIN_DRIVERINSTANCEID_MASK                  (0xE0u)
# define VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS               (5u)
# define VITAHLP_LIN_MAX_NUMBER_CHANNEL                     (32u)
# define VITAHLP_LIN_CHANNELID_MASK                         (0x1Fu)
# define VITAHLP_LIN_TRANSMISSIONCMD_RESPONSEMODE_MASK      (0x01u)

/* Defintion used for definition of Lin status map */
# define VITAHLP_LIN_NUMBER_OF_LIN_STATUSES                 (11u)

/* Definitions of API function IDs and module ID from AUTOSAR specification of LIN component */
# define VITAHLP_LIN_SID_SENDFRAME_ID                       (4u)
# define VITAHLP_LIN_SID_GOTOSLEEP_ID                       (6u)
# define VITAHLP_LIN_SID_WAKEUP_ID                          (7u)
# define VITAHLP_LIN_SID_GOTOSLEEPINTERNAL_ID               (9u)
# define VITAHLP_LIN_SID_CHECKWAKEUP_ID                     (10u)
# define VITAHLP_LIN_MODULE_ID                              (82u)

/* Definitions of API function IDs from AUTOSAR specification of LinIf component */
# define VITAHLP_LINIF_SID_HEADER_INDICATION_ID             (0x78u)
# define VITAHLP_LINIF_SID_RX_INDICATION_ID                 (0x79u)
# define VITAHLP_LINIF_SID_TX_CONFIRMATION_ID               (0x7Au)
# define VITAHLP_LINIF_SID_LIN_ERROR_INDICATION_ID          (0x7Bu)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if !defined (VITAHLP_LOCAL)
#  define VITAHLP_LOCAL static
# endif

# if !defined (VITAHLP_LOCAL_INLINE)
#  define VITAHLP_LOCAL_INLINE LOCAL_INLINE
# endif

/* Service Identifiers for LIN test module commands */
typedef enum
{
  VITAHLP_LIN_CMD_TRANSMISSION = 0u,
  VITAHLP_LIN_CMD_REQUESTSTATUS = 1u,
  VITAHLP_LIN_CMD_TRIGGER_WAKEUP = 2u,
  VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEP = 3u,
  VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEPINTERNAL = 4u,
  VITAHLP_LIN_CMD_SET_RESPONSEDATA = 5u,
  VITAHLP_LIN_CMD_SERVICEIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_Lin_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for LIN test module events */
typedef enum
{
  VITAHLP_LIN_EVENT_REQUEST_STATUS_RESULT = 0u,
  VITAHLP_LIN_EVENT_WAKEUP_CONFIRMATION = 1u,
  VITAHLP_LIN_EVENT_HEADER_INDICATION = 2u,
  VITAHLP_LIN_EVENT_RX_INDICATION = 3u,
  VITAHLP_LIN_EVENT_TX_CONFIRMATION = 4u,
  VITAHLP_LIN_EVENT_LIN_ERROR_INDICATION = 5u
} vItaHlp_Lin_TestCommunicationEventServiceIdentifierType;

/* Layout Positions for LIN test module commands */
typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID = 0u
} vItaHlp_Lin_TestCommunicationGeneralCommandLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_TRANSMISSION_COMMAND_POS_RESPONSEMODE = 1u,
  VITAHLP_LIN_TESTCOMMUNICATION_TRANSMISSION_COMMAND_POS_PATTERN = 2u
} vItaHlp_Lin_TestCommunicationTransmissionCommandLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_SET_RESPONSE_DATA_COMMAND_POS_RESPONSEDATA = 0u
} vItaHlp_Lin_TestCommunicationSetResponseDataType;

/* Layout Positions for LIN test module events */
typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID = 0u
} vItaHlp_Lin_TestCommunicationGeneralEventLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_REQUEST_STATUS_RESULT_EVENT_POS_STATUS_RESULT = 1u,
  VITAHLP_LIN_TESTCOMMUNICATION_REQUEST_STATUS_RESULT_EVENT_POS_PAYLOAD = 2u
} vItaHlp_Lin_TestCommunicationGetStatusResultEventLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_HEADER_INDICATION_EVENT_POS_FRAME_RESPONSE_TYPE = 1u
} vItaHlp_Lin_TestCommunicationHeaderIndicationEventLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_RX_INDICATION_EVENT_POS_PAYLOAD = 1u
} vItaHlp_Lin_TestCommunicationRxIndicationEventLayoutType;

typedef enum
{
  VITAHLP_LIN_TESTCOMMUNICATION_LIN_ERROR_INDICATION_EVENT_POS_ERROR_TYPE = 1u
} vItaHlp_Lin_TestCommunicationLinErrorIndicationEventLayoutType;

/* Driver independent vItaHlp_Lin_StatusType */
typedef enum
{
  VITAHLP_LIN_NOT_OK = 0x00u,
  VITAHLP_LIN_TX_OK = 0x01u,
  VITAHLP_LIN_TX_BUSY = 0x02u,
  VITAHLP_LIN_TX_HEADER_ERROR = 0x03u,
  VITAHLP_LIN_TX_ERROR = 0x04u,
  VITAHLP_LIN_RX_OK = 0x05u,
  VITAHLP_LIN_RX_BUSY = 0x06u,
  VITAHLP_LIN_RX_ERROR = 0x07u,
  VITAHLP_LIN_RX_NO_RESPONSE = 0x08u,
  VITAHLP_LIN_OPERATIONAL = 0x09u,
  VITAHLP_LIN_CH_SLEEP = 0x0Au
} vItaHlp_Lin_StatusType;

/* Driver and ASR independent definitions of Lin_FrameResponseType */
typedef enum
{
  VITAHLP_LIN_FRAMERESPONSE_TX = 0u,
  VITAHLP_LIN_FRAMERESPONSE_RX = 1u,
  VITAHLP_LIN_FRAMERESPONSE_IGNORE = 2u
} vItaHlp_Lin_FrameResponseType;

/* Driver and ASR independent definitions of Lin_FrameCsModelType */
typedef enum
{
  VITAHLP_LIN_ENHANCED_CS = 0u,
  VITAHLP_LIN_CLASSIC_CS = 1u
} vItaHlp_Lin_FrameCsModelType;

typedef struct svItaHlp_Lin_StatusTypeMapType
{
  Lin_StatusType linDriverStatus;
  vItaHlp_Lin_StatusType linvItaHlpStatus;
} vItaHlp_Lin_StatusTypeMapType;

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* LIN Master nodes: Buffer to store the response data received by another LIN node */
VAR(uint8, VITAHLP_VAR_NOINIT) sduLinRxBuffer[VITAHLP_LIN_TEST_PAYLOADLENGTH];
/* LIN Slave nodes: Buffer to keep the data which is added by the LIN Slave as response to a received frame header */
VAR(uint8, VITAHLP_VAR_NOINIT) sduLinTxResponseBuffer[VITAHLP_LIN_TEST_PAYLOADLENGTH];

/* Variable of LIN Pdu type which is used to provide frame ID, checksum model, response mode, data length
 * and SDU pointer to the LIN driver. Provided in whole LIN Module scope because it must be accessible by different
 * functions.
 */
VAR(Lin_PduType, VITAHLP_VAR_NOINIT) pduTest;

/* Variable which has 2 valid states in order to indicate which of the both patterns for the LIN setup test
 * shall be applied for the next run.
 *           Setup Test LIN messages:
 *           ==>If payload length is even the pattern consists of equal long parts(e.g. for 8 Bytes (max. payload))
 *           Byte0  Byte1  Byte2  Byte3  Byte4  Byte5  Byte6  Byte7
 * FALSE:     0x00   0x00   0x00   0x00   0xFF   0xFF   0xFF   0xFF
 * TRUE:      0xFF   0xFF   0xFF   0xFF   0x00   0x00   0x00   0x00
 *           ==>If payload length is odd the second part consists of one more byte(e.g. for 5 Bytes see below)
 *           Byte0  Byte1  Byte2  Byte3  Byte4
 * FALSE:     0x00   0x00   0xFF   0xFF   0xFF
 * TRUE:      0xFF   0xFF   0x00   0x00   0x00
 */
VAR(boolean, VITAHLP_VAR_NOINIT) vItaHlp_Lin_SetupTestPatternIndicationToggleBit;

# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#define VITAHLP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Array of LIN statuses which is used to map the driver specific LIN status enumeration to the driver independent
 * vItaHlp LIN status enumeration. Accessed via the GetTranslatedLinDriverStatus function.
 */
CONST(vItaHlp_Lin_StatusTypeMapType, VITAHLP_VAR) vItaHlp_Lin_StatusTypeMap[VITAHLP_LIN_NUMBER_OF_LIN_STATUSES] =
{
  /* linDriverStatus */    /* linvItaHlpStatus */
  {LIN_NOT_OK,              VITAHLP_LIN_NOT_OK          },
  {LIN_TX_OK,               VITAHLP_LIN_TX_OK           },
  {LIN_TX_BUSY,             VITAHLP_LIN_TX_BUSY         },
  {LIN_TX_HEADER_ERROR,     VITAHLP_LIN_TX_HEADER_ERROR },
  {LIN_TX_ERROR,            VITAHLP_LIN_TX_ERROR        },
  {LIN_RX_OK,               VITAHLP_LIN_RX_OK           },
  {LIN_RX_BUSY,             VITAHLP_LIN_RX_BUSY         },
  {LIN_RX_ERROR,            VITAHLP_LIN_RX_ERROR        },
  {LIN_RX_NO_RESPONSE,      VITAHLP_LIN_RX_NO_RESPONSE  },
  {LIN_OPERATIONAL,         VITAHLP_LIN_OPERATIONAL     },
  {LIN_CH_SLEEP,            VITAHLP_LIN_CH_SLEEP        }
};

#define VITAHLP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_Lin_InitialWakeup()
 *********************************************************************************************************************/
/*! \brief       Performs an internal sleep and wakeup sequence that sets the referenced channel of the referenced
 *               LIN driver instance to awake/operational.
 *  \details     -
 *  \param[in]   driverInstanceID      Instance ID of the LIN driver.
 *  \param[in]   channelID             ID of the LIN channel for the given LIN driver.
 *  \return      -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_InitialWakeup(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID);

/**********************************************************************************************************************
 *  vItaHlp_Lin_CheckChannelMapEntry()
 *********************************************************************************************************************/
/*! \brief           Check if the given driver channel pairing is in the deposited channel map.
 *  \details         -
 *  \param[in]       driverInstanceID      Instance ID of the LIN driver.
 *  \param[in]       channelID             ID of the LIN channel for the given LIN driver.
 *  \return          TRUE:   channel map entry for given input combination found.
 *                   FALSE:  given input cannot be found in internal config array.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_CheckChannelMapEntry(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID);

/**********************************************************************************************************************
 *  vItaHlp_Lin_FillBufferWithSetupTestPattern()
 *********************************************************************************************************************/
/*! \brief           Fills each byte of the provided buffer with a predefined setup test pattern.
 *  \details         -
 *  \param[out]      sduBufferPtr          Pointer to the Data buffer which gets filled with the pattern.
 *  \param[in]       length                Length of the buffer.
 *  \pre             -
 *  \context         TASK
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_FillBufferWithSetupTestPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduBufferPtr,
  VAR(uint8, AUTOMATIC) length);

/**********************************************************************************************************************
 *  vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry()
 *********************************************************************************************************************/
/*! \brief           Check if the given wakeup source is in the deposited channel map.
 *  \details         -
 *  \param[in]       wakeupSource      Wakeup source which is looked for.
 *  \return          TRUE:   channel map entry for given input found.
 *                   FALSE:  given input cannot be found in internal config array.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry(
  VAR(uint32, AUTOMATIC) wakeupSource);

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetIndexDriverSpecificData()
 *********************************************************************************************************************/
/*! \brief       Verifies if the given LIN driver instance is given in the internal config struct array and
 *               returns the corresponding index.
 *  \details     -
 *  \param[in]   linDriverInstanceId          Instance ID of the LIN driver.
 *  \param[out]  linDriverSpecificDataIndex   Index of the internal config array which contains the entry for the given
 *                                            LIN driver instance ID.
 *  \return      TRUE:   linDriverSpecificDataIndex for given input combination found.
 *               FALSE:  given input cannot be found in internal config array.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_GetIndexDriverSpecificData(
  VAR(vItaHlp_Lin_DriverInstanceIdType, AUTOMATIC) linDriverInstanceId,
  P2VAR(vItaHlp_Lin_DriverInstanceIdType, AUTOMATIC, VITAHLP_APPL_VAR) linDriverSpecificDataIndex);

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource()
 *********************************************************************************************************************/
/*! \brief           Resolves the given wakeup source and returns the associated driver instance ID.
 *  \details         -
 *  \param[in]       wakeupSource      Wakeup source which is linked to the driver instance ID.
 *  \return          Driver instance ID for the given wakeup source.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource(
  VAR(uint32, AUTOMATIC) wakeupSource);

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetChannelIDFromWakeupSource()
 *********************************************************************************************************************/
/*! \brief           Resolves the given wakeup source and returns the associated channel ID.
 *  \details         -
 *  \param[in]       wakeupSource      Wakeup source which is linked to the channel ID.
 *  \return          Channel ID for the given wakeup source.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetChannelIDFromWakeupSource(
  VAR(uint32, AUTOMATIC) wakeupSource);
  
/**********************************************************************************************************************
 *  vItaHlp_Lin_GetTranslatedLinDriverStatus()
 *********************************************************************************************************************/
/*! \brief           Resolves the given LIN driver specific status (enumeration) and returns the associated driver
 *                   independent LIN vItaHlp status (enumeration).
 *  \details         -
 *  \param[in]       linDriverStatus      LIN driver specific status which is linked to the LIN vItaHlp status.
 *  \return          LIN vItaHlp status for the given LIN driver specific status.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(vItaHlp_Lin_StatusType, VITAHLP_CODE) vItaHlp_Lin_GetTranslatedLinDriverStatus(
  VAR(Lin_StatusType, AUTOMATIC) linDriverStatus);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_Wakeup()
 *********************************************************************************************************************/
/*! \brief           Trigger Wakeup for the referenced channel of the referenced LIN driver instance.
 *  \details         -
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_Wakeup(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_GoToSleep()
 *********************************************************************************************************************/
/*! \brief           Trigger GoToSleep for the referenced channel of the referenced LIN driver instance.
 *  \details         -
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_GoToSleep(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_GoToSleepInternal()
 *********************************************************************************************************************/
/*! \brief           Trigger GoToSleepInternal for the referenced channel of the referenced LIN driver instance.
 *  \details         -
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_GoToSleepInternal(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_Transmission()
 *********************************************************************************************************************/
/*! \brief           Sets Pdu data and triggers SendFrame.
 *  \details         -
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Transmission(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_RequestStatus()
 *********************************************************************************************************************/
/*! \brief           Triggers GetStatus and causes RequestStatusResult event.
 *  \details         Equips the event message with the obtained LIN status and the received response.
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_RequestStatus(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_Set_ResponseData()
 *********************************************************************************************************************/
/*! \brief           Stores the data pattern which used for response from LIN Slave on ECU.
 *  \details         The data pattern is stored in a local buffer variable. It is used as response from any LIN Slave
 *                   of the ECU until a new pattern is stored.
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Set_ResponseData(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetDriverInstanceIDFromChannel()
 *********************************************************************************************************************/
/*! \brief           Returns the associated driver instance ID of a given LIN channel.
 *  \details         The LIN channel ID is unique over all configured LIN drivers.
 *  \param[in]       channelID             Channel ID for which the related driver instance ID shall be returned.
 *  \return          Driver instance ID for the given channel.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetDriverInstanceIDFromChannel(
  VAR(uint8, AUTOMATIC) channelID);

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetCombinedDriverChannelInfoData()
 *********************************************************************************************************************/
/*! \brief           Builds the combined driver and channel instance ID.
 *  \details         The value of the combinedDriverChannelId is invalid, if the function returns FALSE.
 *  \param[in]       driverInstanceID      Driver ID for which the combined value shall be build.
 *  \param[in]       channelID             Channel ID for which the combined value shall be build.
 *  \param[out]      driverChannelId       Combined value indicating the driverInstanceID and the channelID.
 *  \return          TRUE if the given driver and channel IDs are within the valid ranges.
 *                   FALSE otherwise.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_GetCombinedDriverChannelInfoData(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) driverChannelId);

/**********************************************************************************************************************
 *  vItaHlp_Lin_IsWakeupConditionFulfilled()
 *********************************************************************************************************************/
/*! \brief           Checks whether the precondition to wakeup a LIN channel is fulfilled.
 *  \details         For a LIN Master node the function returns true, if the given channel is in state LIN_CH_SLEEP
 *                   and can be awaken now.
 *                   For a LIN Slave node the function always returns true, as the internal state cannot be
 *                   determined by an application.
 *                   For driverInstanceID channelID pairing which are not configured as unit under test, the function
 *                   returns always false.
 *  \param[in]       driverInstanceID      Driver ID for which the precondition shall be evaluated.
 *  \param[in]       channelID             Channel ID for which the precondition shall be evaluated.
 *  \return          TRUE if the precondition is fulfilled for a given driver and channel ID.
 *                   FALSE otherwise.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_IsWakeupConditionFulfilled(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID);

/**********************************************************************************************************************
 *  vItaHlp_Lin_IsMasterNode()
 *********************************************************************************************************************/
/*! \brief           Checks whether the given LIN channel is configured as Master node.
 *  \details         -
 *                   Otherwise the functions returns false.
 *  \param[in]       driverInstanceID      Driver ID for which the node type shall be evaluated.
 *  \param[in]       channelID             Channel ID for which the node type shall be evalulated.
 *  \return          TRUE if the given LIN channel is confiugred as Master node.
 *                   FALSE otherwise.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_IsMasterNode(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID);

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Lin_InitialWakeup()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_InitialWakeup(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 linDriverSpecificDataIndex;
  Std_ReturnType internalSleepResult;
  Std_ReturnType alertnessResult;

  /* ----- Implementation ----------------------------------------------- */
  /* If the driverInstanceID could not be determined in case of a configuration error,
   * the function returns immediately - a subsequent test fails and the user is notified at that time
   */
  if (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE)
  {
    internalSleepResult = vItaHlp_Lin_GoToSleepInternal(linDriverSpecificDataIndex, channelID);
    if (internalSleepResult == E_OK)
    {
      alertnessResult = vItaHlp_Lin_Wakeup(linDriverSpecificDataIndex, channelID);
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
      if (alertnessResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_LIN_MODULE_ID,
          driverInstanceID,
          VITAHLP_LIN_SID_WAKEUP_ID,
          alertnessResult);
      }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
    }
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
    else
    {
      vItaHlp_EventModuleUnderTestError(VITAHLP_LIN_MODULE_ID,
        driverInstanceID,
        VITAHLP_LIN_SID_GOTOSLEEPINTERNAL_ID,
        internalSleepResult);
    }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
  }
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_Wakeup()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_Wakeup(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType alertnessResult;
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data */
  driverInstanceID = ( (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_DRIVERINSTANCEID_MASK) >> VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS );
  channelID = (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_CHANNELID_MASK);

  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    /* Check wakeup precondition */
    if (vItaHlp_Lin_IsWakeupConditionFulfilled(driverInstanceID, channelID) == TRUE)
    {
      alertnessResult = vItaHlp_Lin_Wakeup(linDriverSpecificDataIndex, channelID);
      if (alertnessResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_LIN_MODULE_ID,
          driverInstanceID,
          VITAHLP_LIN_SID_WAKEUP_ID,
          alertnessResult);
      }
    }
  }
  else
  {
    /* The referenced driverInstanceID channelID pairing is not in the generated channel map array */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_CMD_TRIGGER_WAKEUP,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_GoToSleep()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_GoToSleep(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Lin_StatusType linStatusResult;
  Std_ReturnType sleepResult;
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data */
  driverInstanceID = ( (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_DRIVERINSTANCEID_MASK) >> VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS );
  channelID = (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_CHANNELID_MASK);

  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    /* Check if the channel is awake */
    linStatusResult = vItaHlp_Lin_GetStatus(linDriverSpecificDataIndex, channelID, &pduTest.SduPtr);
    if (linStatusResult != LIN_CH_SLEEP)
    {
      /* If so perform a go-to-sleep call */
      sleepResult = vItaHlp_Lin_GoToSleep(linDriverSpecificDataIndex, channelID);
      if (sleepResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_LIN_MODULE_ID,
          driverInstanceID,
          VITAHLP_LIN_SID_GOTOSLEEP_ID,
          sleepResult);
      }
    }
  }
  else
  {
    /* The referenced driverInstanceID channelID pairing is not in the generated channel map array */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEP,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_Trigger_GoToSleepInternal()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Trigger_GoToSleepInternal(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType sleepResult;
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data */
  driverInstanceID = ( (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_DRIVERINSTANCEID_MASK) >> VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS );
  channelID = (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_CHANNELID_MASK);

  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    /* Perform a go-to-sleep-internal call */
    sleepResult = vItaHlp_Lin_GoToSleepInternal(linDriverSpecificDataIndex, channelID);
    if (sleepResult != E_OK)
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_LIN_MODULE_ID,
        driverInstanceID,
        VITAHLP_LIN_SID_GOTOSLEEPINTERNAL_ID,
        sleepResult);
    }
  }
  else
  {
    /* The referenced driverInstanceID channelID pairing is not in the generated channel map array */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEPINTERNAL,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  vItaHlp_Lin_CheckChannelMapEntry()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_CheckChannelMapEntry(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  boolean mapEntryExists = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if ( (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId == driverInstanceID) &&
      (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId == channelID) )
    {
      mapEntryExists = TRUE;
      break;
    }
  }
  return mapEntryExists;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetIndexDriverSpecificData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_GetIndexDriverSpecificData(
  VAR(vItaHlp_Lin_DriverInstanceIdType, AUTOMATIC) linDriverInstanceId,
  P2VAR(vItaHlp_Lin_DriverInstanceIdType, AUTOMATIC, VITAHLP_APPL_VAR) linDriverSpecificDataIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 dataIndex;
  boolean result = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (dataIndex = 0u; dataIndex < vItaHlp_Lin_GetNumberOfDrivers(); dataIndex++)
  {
    if (vItaHlp_Lin_DriverSpecificData[dataIndex].linDriverInstanceId == linDriverInstanceId)
    {
      linDriverSpecificDataIndex[0u] = dataIndex;
      result = TRUE;
      break;
    }
  }
  return result;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_FillBufferWithSetupTestPattern()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_FillBufferWithSetupTestPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduBufferPtr,
  VAR(uint8, AUTOMATIC) length)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 halfLength;
  uint8 prolonger = 0u;

  /* ----- Implementation ----------------------------------------------- */
  halfLength = length / 2u;
  prolonger = length % 2u;

  if (vItaHlp_Lin_SetupTestPatternIndicationToggleBit == FALSE)
  {
    VStdLib_MemSet(&sduBufferPtr[0u], 0x00u, halfLength);
    VStdLib_MemSet(&sduBufferPtr[halfLength], 0xFFu, (halfLength + prolonger));
    vItaHlp_Lin_SetupTestPatternIndicationToggleBit = TRUE;
  }
  else
  {
    VStdLib_MemSet(&sduBufferPtr[0u], 0xFFu, halfLength);
    VStdLib_MemSet(&sduBufferPtr[halfLength], 0x00u, (halfLength + prolonger));
    vItaHlp_Lin_SetupTestPatternIndicationToggleBit = FALSE;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  boolean mapEntryExists = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linWakeupSource == wakeupSource)
    {
      mapEntryExists = TRUE;
      break;
    }
  }
  return mapEntryExists;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  uint8 driverInstanceID = 255u;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linWakeupSource == wakeupSource)
    {
      driverInstanceID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId;
      break;
    }
  }
  return driverInstanceID;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetChannelIDFromWakeupSource()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetChannelIDFromWakeupSource(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  uint8 channelID = 255u;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linWakeupSource == wakeupSource)
    {
      channelID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId;
      break;
    }
  }
  return channelID;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetTranslatedLinDriverStatus()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(vItaHlp_Lin_StatusType, VITAHLP_CODE) vItaHlp_Lin_GetTranslatedLinDriverStatus(
  VAR(Lin_StatusType, AUTOMATIC) driverLinStatus)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  vItaHlp_Lin_StatusType vItaHlpLinStatus = VITAHLP_LIN_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0; channelMapIdx < VITAHLP_LIN_NUMBER_OF_LIN_STATUSES; channelMapIdx++)
  {
    if (vItaHlp_Lin_StatusTypeMap[channelMapIdx].linDriverStatus == driverLinStatus)
    {
      vItaHlpLinStatus = vItaHlp_Lin_StatusTypeMap[channelMapIdx].linvItaHlpStatus;
      break;
    }
  }
  return vItaHlpLinStatus;
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Lin_Transmission()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Transmission(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;
  Lin_FrameResponseType responseMode;
  uint8 sduBuffer[VITAHLP_LIN_TEST_PAYLOADLENGTH];
  Std_ReturnType sendResult;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data */
  driverInstanceID = ( (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_DRIVERINSTANCEID_MASK) >> VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS );
  channelID = (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_CHANNELID_MASK);

  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    responseMode =
      (Lin_FrameResponseType)(payloadData[VITAHLP_LIN_TESTCOMMUNICATION_TRANSMISSION_COMMAND_POS_RESPONSEMODE] &
      VITAHLP_LIN_TRANSMISSIONCMD_RESPONSEMODE_MASK);

    /* Prepare pdu data */
    pduTest.Cs = LIN_ENHANCED_CS;
    pduTest.Drc = responseMode;
    pduTest.Dl = VITAHLP_LIN_TEST_PAYLOADLENGTH;

    /* If DUT sends the response fill the Tx Buffer and set the assigned MR frame ID */
    if (responseMode == (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_TX)
    { 
      pduTest.Pid = VITAHLP_LIN_MASTER_MASTERRESPONSE_FRAME_PID;
      pduTest.SduPtr = sduBuffer;
      VStdLib_MemCpy(pduTest.SduPtr,
        &payloadData[VITAHLP_LIN_TESTCOMMUNICATION_TRANSMISSION_COMMAND_POS_PATTERN],
        VITAHLP_LIN_TEST_PAYLOADLENGTH);
    }
    /* If not provide the global LIN Rx Buffer and set the assigned slave response frame ID */
    else
    {
      pduTest.Pid = VITAHLP_LIN_MASTER_SLAVERESPONSE_FRAME_PID;
      pduTest.SduPtr = sduLinRxBuffer;
    }

    /* Transmit the frame */
    sendResult = vItaHlp_Lin_SendFrame(linDriverSpecificDataIndex, channelID, &pduTest);

    if (sendResult != E_OK)
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_LIN_MODULE_ID,
        driverInstanceID,
        VITAHLP_LIN_SID_SENDFRAME_ID,
        sendResult);
    }
  }
  else
  {
    /* The referenced driverInstanceID channelID pairing is not in the generated channel map array */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_CMD_TRANSMISSION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_RequestStatus()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_RequestStatus(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8* sduPosPtr;
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;
  Lin_StatusType driverLinStatusResult;
  vItaHlp_Lin_StatusType vItaHlpLinStatusResult;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data */
  driverInstanceID = ( (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_DRIVERINSTANCEID_MASK) >> VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS );
  channelID = (payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID] &
    VITAHLP_LIN_CHANNELID_MASK);

  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    driverLinStatusResult = vItaHlp_Lin_GetStatus(linDriverSpecificDataIndex, channelID, &pduTest.SduPtr);
    vItaHlpLinStatusResult = vItaHlp_Lin_GetTranslatedLinDriverStatus(driverLinStatusResult);

    /* Equip event message with driver channel pairing and LIN status */
    eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
      payloadData[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_COMMAND_POS_DRIVERINSTANCEID_CHANNELID];
    eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_REQUEST_STATUS_RESULT_EVENT_POS_STATUS_RESULT] =
      (uint8)vItaHlpLinStatusResult;

    /* Set sduPosPtr to payload position in event message */
    sduPosPtr = eventPayloadBuffer + VITAHLP_LIN_TESTCOMMUNICATION_REQUEST_STATUS_RESULT_EVENT_POS_PAYLOAD;

    /* Equip event message with received response (slave response mode only) */
    if (pduTest.Drc == (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_RX)
    {
      VStdLib_MemCpy(sduPosPtr, pduTest.SduPtr, VITAHLP_LIN_TEST_PAYLOADLENGTH);
    }

    /* Forward to EventHandler */
    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_EVENT_REQUEST_STATUS_RESULT,
      (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
  }
  else
  {
    /* The referenced driverInstanceID channelID pairing is not in the generated channel map array */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_CMD_REQUESTSTATUS,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_Set_ResponseData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Set_ResponseData(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* The received payload data is used for each LIN Slave response. There is no need to distinguish between different
   * LIN drivers or LIN channels
   */
  VStdLib_MemCpy(sduLinTxResponseBuffer,
    &payloadData[VITAHLP_LIN_TESTCOMMUNICATION_SET_RESPONSE_DATA_COMMAND_POS_RESPONSEDATA],
    VITAHLP_LIN_TEST_PAYLOADLENGTH);
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetDriverInstanceIDFromChannel()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Lin_GetDriverInstanceIDFromChannel(
  VAR(uint8, AUTOMATIC) channelID)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  uint8 driverInstanceID = 255u;

  /* ----- Implementation ----------------------------------------------- */
  for (channelMapIdx = 0u; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId == channelID)
    {
      driverInstanceID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId;
      break;
    }
  }
  return driverInstanceID;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_GetCombinedDriverChannelInfoData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_GetCombinedDriverChannelInfoData(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) driverChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean driverChannelIdExists;

  /* ----- Implementation ----------------------------------------------- */
  driverChannelIdExists = FALSE;
  if (driverInstanceID < VITAHLP_LIN_MAX_NUMBER_DRIVER && channelID < VITAHLP_LIN_MAX_NUMBER_CHANNEL)
  {
    driverChannelId[0] = (driverInstanceID << VITAHLP_LIN_DRIVERINSTANCEID_BIT_POS) + channelID;
    driverChannelIdExists = TRUE;
  }
  return driverChannelIdExists;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_IsWakeupConditionFulfilled()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_IsWakeupConditionFulfilled(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Lin_StatusType linStatusResult;
  uint8 linDriverSpecificDataIndex;
  boolean isWakeupConditionFulfilled;

  /* ----- Implementation ----------------------------------------------- */
  isWakeupConditionFulfilled = FALSE;
  /* Verify that the driverInstanceID channelID pairing is configured as unit under test */
  if ( (vItaHlp_Lin_CheckChannelMapEntry(driverInstanceID, channelID) == TRUE) &&
    (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) )
  {
    isWakeupConditionFulfilled = TRUE;
    if (vItaHlp_Lin_IsMasterNode(driverInstanceID, channelID))
    {
      /* Check if the channel is asleep */
      linStatusResult = vItaHlp_Lin_GetStatus(linDriverSpecificDataIndex, channelID, &pduTest.SduPtr);
      if (linStatusResult != LIN_CH_SLEEP)
      {
        isWakeupConditionFulfilled = FALSE;
      }
    }
  }
  return isWakeupConditionFulfilled;
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_IsMasterNode()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Lin_IsMasterNode(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint8, AUTOMATIC) channelID)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isMasterNode;
  uint8_least channelMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  isMasterNode = FALSE;
  for (channelMapIdx = 0; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    if ( (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId == driverInstanceID) &&
      (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId == channelID) )
    {
      if (vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linNodeType == VITAHLP_LIN_MASTER)
      {
        isMasterNode = TRUE;
      }
      break;
    }
  }
  return isMasterNode;
}

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Lin_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  uint8 driverInstanceID;
  uint8 channelID;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize test pattern bit for setup test */
  vItaHlp_Lin_SetupTestPatternIndicationToggleBit = FALSE;

  for (channelMapIdx = 0u; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    /* Retrieve driverInstanceID and channelID via channel map */
    driverInstanceID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId;
    channelID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId;

    /* Call the initial wakeup for each configured driver and channel */
    vItaHlp_Lin_InitialWakeup(driverInstanceID, channelID);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Lin_TriggerSetupTest()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_TriggerSetupTest(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least channelMapIdx;
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 sduTxBuffer[VITAHLP_LIN_SETUPTEST_PAYLOADLENGTH];
  uint8 linDriverSpecificDataIndex;
  Lin_PduType pduSetupTest;
  vItaHlp_Lin_NodeType linNodeType;

  /* ----- Implementation ----------------------------------------------- */

  /* Fill sdu data buffer with test pattern */
  vItaHlp_Lin_FillBufferWithSetupTestPattern(sduTxBuffer, VITAHLP_LIN_SETUPTEST_PAYLOADLENGTH);

  /* Transmit a frame for each configured LIN channel*/
  for (channelMapIdx = 0u; channelMapIdx < vItaHlp_Lin_GetNumberOfChannels(); channelMapIdx++)
  {
    /* Retrieve driverInstanceID, channelID and nodeType via channel map */
    driverInstanceID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linDriverInstanceId;
    channelID = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linChannelId;
    linNodeType = vItaHlp_Lin_ChannelSpecificData[channelMapIdx].linNodeType;

    if ( (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE) &&
      (linNodeType == VITAHLP_LIN_MASTER) )
    {
      /* Prepare pdu data */
      pduSetupTest.Pid = VITAHLP_LIN_SETUPTEST_FRAME_PID;
      pduSetupTest.Cs = LIN_ENHANCED_CS;
      pduSetupTest.Drc = (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_TX;
      pduSetupTest.Dl = VITAHLP_LIN_SETUPTEST_PAYLOADLENGTH;
      pduSetupTest.SduPtr = sduTxBuffer;

      /* Transmit the frame */
      (void)vItaHlp_Lin_SendFrame(linDriverSpecificDataIndex, channelID, &pduSetupTest);
    }
  }
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Lin_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  switch (serviceIdentifier)
  {
  case VITAHLP_LIN_CMD_TRANSMISSION:
    vItaHlp_Lin_Transmission(payloadData);
    break;
  case VITAHLP_LIN_CMD_REQUESTSTATUS:
    vItaHlp_Lin_RequestStatus(payloadData);
    break;
  case VITAHLP_LIN_CMD_TRIGGER_WAKEUP:
    vItaHlp_Lin_Trigger_Wakeup(payloadData);
    break;
  case VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEP:
    vItaHlp_Lin_Trigger_GoToSleep(payloadData);
    break;
  case VITAHLP_LIN_CMD_TRIGGER_GOTOSLEEPINTERNAL:
    vItaHlp_Lin_Trigger_GoToSleepInternal(payloadData);
    break;
  case VITAHLP_LIN_CMD_SET_RESPONSEDATA:
    vItaHlp_Lin_Set_ResponseData(payloadData);
    break;
  default:
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      serviceIdentifier,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_UNKNOWN_CMD,
      VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
    break;
  }
}
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  APPLICATION SPECIFIC CALLBACK FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  vItaHlp_EcuM_Lin_CheckWakeup_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_Lin_CheckWakeup_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 linDriverSpecificDataIndex;
  Std_ReturnType checkWakeupResult = E_NOT_OK;
  Std_ReturnType wakeupInternalResult = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry(wakeupSource) == TRUE)
  {
    driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource(wakeupSource);
    channelID = vItaHlp_Lin_GetChannelIDFromWakeupSource(wakeupSource);
    if (vItaHlp_Lin_GetIndexDriverSpecificData(driverInstanceID, &linDriverSpecificDataIndex) == TRUE)
    {
      checkWakeupResult = vItaHlp_Lin_CheckWakeup(linDriverSpecificDataIndex, channelID);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
      if (checkWakeupResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(LIN_MODULE_ID, driverInstanceID, LIN_SID_CHECKWAKEUP_ID, checkWakeupResult);
      }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

      wakeupInternalResult = vItaHlp_Lin_WakeupInternal(linDriverSpecificDataIndex, channelID);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
      if (wakeupInternalResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          LIN_MODULE_ID,
          driverInstanceID,
          LIN_SID_WAKEUPINTERNAL_ID,
          wakeupInternalResult);
      }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
    }
    /* No additional error message required in case the driverInstanceID could not be determined */
  }
}

/**********************************************************************************************************************
 *  vItaHlp_EcuM_Lin_SetWakeupEvent_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_Lin_SetWakeupEvent_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  wakeupSource = wakeupSource;
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_LinIf_WakeupConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_WakeupConfirmation_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 channelID;
  uint8 combinedDriverChannelId;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_Lin_CheckChannelMapForWakeupSourceEntry(wakeupSource) == TRUE)
  {
    driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromWakeupSource(wakeupSource);
    channelID = vItaHlp_Lin_GetChannelIDFromWakeupSource(wakeupSource);

    if (vItaHlp_Lin_GetCombinedDriverChannelInfoData(driverInstanceID, channelID, &combinedDriverChannelId) == TRUE)
    {
      eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
        combinedDriverChannelId;
      vItaHlp_SetEvent(
        VITAHLP_COMPONENTIDENTIFIER_LIN,
        VITAHLP_LIN_EVENT_WAKEUP_CONFIRMATION,
        (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
    }
    else
    {
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_LIN,
        VITAHLP_LIN_EVENT_WAKEUP_CONFIRMATION,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
        VITAHLP_E_INVALID_CONFIGURATION,
        0u);
    }
  }
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_EVENT_WAKEUP_CONFIRMATION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_UNEXPECTED_CALLBACK_RECEIVED,
      0u);
  }
}

#  if (VITAHLP_LINSLAVE == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_LinIf_HeaderIndication_Callout()
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_LinIf_HeaderIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  P2VAR(Lin_PduType, AUTOMATIC, VITAHLP_APPL_VAR) pduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 combinedDriverChannelId;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  boolean knownFrameId;

  /* ----- Implementation ----------------------------------------------- */
  /* Perform only a limited set of parameter checks for vItaHlp */
  if (pduPtr != NULL_PTR)
  {
    knownFrameId = TRUE;
    driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromChannel(channel);
    if (vItaHlp_Lin_GetCombinedDriverChannelInfoData(driverInstanceID, channel, &combinedDriverChannelId) == TRUE)
    {
      switch (pduPtr->Pid)
      {
        case VITAHLP_LIN_SLAVE_SLAVERESPONSE_FRAME_PID:
          /* Add the LIN Slave response data to the received frame header */
          pduPtr->Drc = (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_TX;
          pduPtr->Dl = VITAHLP_LIN_TEST_PAYLOADLENGTH;
          VStdLib_MemCpy(pduPtr->SduPtr, sduLinTxResponseBuffer, VITAHLP_LIN_TEST_PAYLOADLENGTH);
          break;
        case VITAHLP_LIN_SLAVE_MASTERRESPONSE_FRAME_PID:
          pduPtr->Drc = (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_RX;
          pduPtr->Dl = VITAHLP_LIN_TEST_PAYLOADLENGTH;
          break;
        case VITAHLP_LIN_SLAVE_IGNORERESPONSE_FRAME_PID:
          pduPtr->Drc = (Lin_FrameResponseType)VITAHLP_LIN_FRAMERESPONSE_IGNORE;
          pduPtr->Dl = 0u;
          break;
        default:
          knownFrameId = FALSE;
          break;
      }

      if (knownFrameId == TRUE)
      {
        /* Add remaining PDU data */
        pduPtr->Cs = (Lin_FrameCsModelType)VITAHLP_LIN_ENHANCED_CS;

        /* Build the event message */
        eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
          combinedDriverChannelId;
        eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_HEADER_INDICATION_EVENT_POS_FRAME_RESPONSE_TYPE] =
          (uint8)pduPtr->Drc;
        vItaHlp_SetEvent(
          VITAHLP_COMPONENTIDENTIFIER_LIN,
          VITAHLP_LIN_EVENT_HEADER_INDICATION,
          (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
      }
    }
    else
    {
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_LIN,
        VITAHLP_LIN_EVENT_HEADER_INDICATION,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
        VITAHLP_E_INVALID_REQUESTED_PARAM,
        0u);
    }
  }
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LINIF_SID_HEADER_INDICATION_ID,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
  return E_OK;
}

/**********************************************************************************************************************
 *  vItaHlp_LinIf_TxConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_TxConfirmation_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 combinedDriverChannelId;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromChannel(channel);
  if (vItaHlp_Lin_GetCombinedDriverChannelInfoData(driverInstanceID, channel, &combinedDriverChannelId) == TRUE)
  {
    /* Build the event message */
    eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
      combinedDriverChannelId;
    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_EVENT_TX_CONFIRMATION,
      (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
  }
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LINIF_SID_TX_CONFIRMATION_ID,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_LinIf_RxIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_RxIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 combinedDriverChannelId;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Perform only a limited set of parameter checks for vItaHlp */
  if (sduPtr != NULL_PTR)
  {
    driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromChannel(channel);
    if (vItaHlp_Lin_GetCombinedDriverChannelInfoData(driverInstanceID, channel, &combinedDriverChannelId) == TRUE)
    {
      /* Build the event message, including the response data received from LIN Master */
      VStdLib_MemCpy(
        &eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_RX_INDICATION_EVENT_POS_PAYLOAD],
        sduPtr,
        VITAHLP_LIN_TEST_PAYLOADLENGTH);
      eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
        combinedDriverChannelId;
      vItaHlp_SetEvent(
        VITAHLP_COMPONENTIDENTIFIER_LIN,
        VITAHLP_LIN_EVENT_RX_INDICATION,
        (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
    }
    else
    {
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_LIN,
        VITAHLP_LIN_EVENT_RX_INDICATION,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
        VITAHLP_E_INVALID_REQUESTED_PARAM,
        0u);
    }
  }
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LINIF_SID_RX_INDICATION_ID,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_LinIf_LinErrorIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_LinErrorIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  VAR(Lin_SlaveErrorType, AUTOMATIC) errorStatus)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceID;
  uint8 combinedDriverChannelId;
  uint8 eventPayloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  driverInstanceID = vItaHlp_Lin_GetDriverInstanceIDFromChannel(channel);
  if (vItaHlp_Lin_GetCombinedDriverChannelInfoData(driverInstanceID, channel, &combinedDriverChannelId) == TRUE)
  {
    /* Build the event message */
    eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_GENERAL_EVENT_POS_DRIVERINSTANCEID_CHANNELID] =
      combinedDriverChannelId;
    eventPayloadBuffer[VITAHLP_LIN_TESTCOMMUNICATION_LIN_ERROR_INDICATION_EVENT_POS_ERROR_TYPE] = (uint8)errorStatus;
    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LIN_EVENT_LIN_ERROR_INDICATION,
      (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) eventPayloadBuffer);
  }
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_LIN,
      VITAHLP_LINIF_SID_LIN_ERROR_INDICATION_ID,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}
#  endif /* (VITAHLP_LINSLAVE == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_LIN == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Lin.c
 *********************************************************************************************************************/

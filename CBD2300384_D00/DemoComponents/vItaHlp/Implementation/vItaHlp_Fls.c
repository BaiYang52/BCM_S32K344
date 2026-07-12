/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_Fls.c
 *        \brief  vItaHlp functionalities concerning flash.
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

#define VITAHLP_FLS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "vItaHlp.h"

#if (VITAHLP_FLS == STD_ON)
# include "vItaHlp_CommonUtils.h"
# include "SchM_vItaHlp.h"
# include "vstdlib.h"
# include "vItaHlp_Fls.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Constants used as symbolic names for value sent as payload of job error event */
# define VITAHLP_FLS_JOBRESULT_MEMIF_JOB_OK                   (0u)
# define VITAHLP_FLS_JOBRESULT_MEMIF_JOB_FAILED               (1u)
# define VITAHLP_FLS_JOBRESULT_MEMIF_JOB_PENDING              (2u)
# define VITAHLP_FLS_JOBRESULT_MEMIF_JOB_CANCELED             (3u)
# define VITAHLP_FLS_JOBRESULT_MEMIF_BLOCK_INCONSISTENT       (4u)
# define VITAHLP_FLS_JOBRESULT_MEMIF_BLOCK_INVALID            (5u)

/* Constants used for invalid parameter error event */
# define VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS          (1u)

/* Definitions of API function IDs and module ID from AUTOSAR specification of FLS component */
# define VITAHLP_FLS_SID_ERASE_ID                           (0x01u)
# define VITAHLP_FLS_SID_WRITE_ID                           (0x02u)
# define VITAHLP_FLS_SID_READ_ID                            (0x07u)
# define VITAHLP_FLS_SID_COMPARE_ID                         (0x08u)
# define VITAHLP_FLS_SID_BLANKCHECK_ID                      (0x0Au)
# define VITAHLP_FLS_MODULE_ID                              (92u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
# if !defined (VITAHLP_LOCAL)
#  define VITAHLP_LOCAL static
# endif

# if !defined (VITAHLP_LOCAL_INLINE)
#  define VITAHLP_LOCAL_INLINE LOCAL_INLINE
# endif

/* Service Identifiers for Fls test module commands */
typedef enum
{
  VITAHLP_FLS_CMD_FLS_WRITE =       0u,
  VITAHLP_FLS_CMD_FLS_READ =        1u,
  VITAHLP_FLS_CMD_FLS_COMPARE =     2u,
  VITAHLP_FLS_CMD_FLS_ERASE =       3u,
  VITAHLP_FLS_CMD_FLS_BLANKCHECK =  4u,
  VITAHLP_FLS_CMD_RAM2RAM_COMPARE = 5u,
  VITAHLP_FLS_SERVICEIDENTIFIER_UNKNOWN_CMD = 255u
} vItaHlp_Fls_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for Fls test module events */
typedef enum
{
  VITAHLP_FLS_EVENT_FEE_JOBENDNOTIFICATION =   0u,
  VITAHLP_FLS_EVENT_FEE_JOBERRORNOTIFICATION = 1u,
  VITAHLP_FLS_EVENT_RAM2RAM_COMPARE_DONE =     2u,
} vItaHlp_Fls_TestCommunicationEventServiceIdentifierType;

typedef enum
{
  VITAHLP_FLS_IDLE =          0u,
  VITAHLP_FLS_BUSY =          1u,
  VITAHLP_FLS_BUSY_INTERNAL = 2u
} vItaHlp_Fls_StatusType;

typedef enum
{
  VITAHLP_FLS_BUFFERTYPE_COMPARE = 0u,
  VITAHLP_FLS_BUFFERTYPE_FLASHIO = 1u
} vItaHlp_Fls_RAMBufferType;

typedef enum
{
  VITAHLP_FLS_ADDRESSTYPE_PAGE =   0u,
  VITAHLP_FLS_ADDRESSTYPE_SECTOR = 1u
} vItaHlp_Fls_FlsAddressType;

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/* Pattern used to clear the RAM buffer variable content */
CONST(uint8, VITAHLP_VAR_NOINIT_UNSPECIFIED) ZeroPattern[VITAHLP_FLS_PATTERN_LENGTH] = { 0x00, 0x00, 0x00, 0x00 };

/* Buffer in RAM used for all Fls driver jobs
   Variable is not protected by exclusive area.
   Reason:
   - Variable needs to be accessed in the background by a Fls driver job, an exclusive area protecting it
     would make this impossible
   - It is assured that at most one job accessing the variable is active on all Fls drivers at any point
     in time
*/
VAR(uint8, VITAHLP_VAR_NOINIT_UNSPECIFIED) FlashIOBuffer[vItaHlp_Fls_GetRAMBufferSize()];

/* Stores the status of vItaHlp Fls module
   The vItaHlp_FlsJobResult value is not sufficient to control the processing of the vItaHlp Fls module.
   The vItaHlp Fls module may be busy with internal processing (e.g. RAM2RAM compare). Also during this time frame,
   we have to prevent the start of another job. As the Fls driver inhibits starting jobs in parallel only, if the
   DET error reporting mechanism is activated for the Fls driver, we have to track the status of the Fls driver.
   The status value is derived from the processing state of the vItaHlp Fls test module. It is not linked to the
   Fls driver status (Fls_GetStatus() API function not used).
*/
VAR(vItaHlp_Fls_StatusType, VITAHLP_VAR_NOINIT_UNSPECIFIED) vItaHlp_Fls_Status;

# if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
/* Stores the return value of the recent call of Fls_GetJobResult API */
VAR(MemIf_JobResultType, VITAHLP_VAR_NOINIT_UNSPECIFIED) vItaHlp_Fls_JobResult;

/* Stores the current used function pointer of the Fls_GetJobResult API */
VAR(vItaHlp_Fls_GetJobResultFctPtrType, VITAHLP_VAR_NOINIT_UNSPECIFIED) flsGetJobResultFctPtr;
# endif

# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Fls_FillBufferWithPattern()
 *********************************************************************************************************************/
/*! \brief         Function fills the provided buffer with the given pattern.
 *  \details       The pattern will be repeated until the whole buffer is filled.
 *                 Currently a maximum buffer length of 64k Byte is assumed.
 *  \param[in,out] buffer           Data buffer which gets filled with the pattern.
 *  \param[in]     bufferLength     Length of the buffer.
 *  \param[in]     pattern          Buffer which contains the given pattern.
 *  \param[in]     patternLength    Length of the data pattern.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_FillBufferWithPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) buffer,
  VAR(uint16, AUTOMATIC) bufferLength,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) pattern,
  VAR(uint8, AUTOMATIC) patternLength);

/***********************************************************************************************************************
 *  vItaHlp_Fls_GetAddressMapEntry()
 **********************************************************************************************************************/
/*! \brief       Translates index of address table to driver instance index, flash address and length
 *  \details     Reads the desired information from address table. Depending on the addressType selector, the
 *               returned value of the flash address and length are either related to the stored flash page or the
 *               flash sector data.
 *  \param[in]   tableIndex         Selects which entry of the address table shall be used, user must ensure this
 *                                  number is smaller than the number of entries in the table
 *  \param[in]   addressType        Selects which kind of address (page start address, sector start address)
 *                                  shall be read
 *                                  Should some invalid value be passed, defaults to sector start address
 *  \param[out]  address            Address read from the table
 *  \param[out]  length             Length data read from the table
 *  \param[out]  driverInstance     Index of driver instance read from table
 *  \return      TRUE               If data written to address and driver instance is valid (tableIndex is valid)
 *               FALSE              If the table index does not exist
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous FALSE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Fls_GetAddressMapEntry(
  VAR(vItaHlp_Fls_AddressSelection_TableIndexType, AUTOMATIC) tableIndex,
  VAR(vItaHlp_Fls_FlsAddressType, AUTOMATIC) addressType,
  P2VAR(Fls_AddressType, AUTOMATIC, VITAHLP_APPL_VAR) address,
  P2VAR(Fls_LengthType, AUTOMATIC, VITAHLP_APPL_VAR) length,
  P2VAR(vItaHlp_Fls_InstanceIDType, AUTOMATIC, VITAHLP_APPL_VAR) driverInstance);

/***********************************************************************************************************************
 *  vItaHlp_Fls_ErrorMessage_InputInvalid()
 **********************************************************************************************************************/
/*! \brief       Sends error message event that the received input is invalid
 *  \details     Currently only the flash address index (AI) is verified. If the errorInfo is unknown, no error
 *               event is triggered.
 *  \param[in]   serviceID          ID of the service that wants to send the message
 *  \param[in]   errorInfo          VTAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous FALSE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_ErrorMessage_InputInvalid(
  VAR(uint8, AUTOMATIC) serviceID,
  VAR(uint8, AUTOMATIC) errorInfo);

/***********************************************************************************************************************
 *  vItaHlp_Fls_Job_End()
 **********************************************************************************************************************/
/*! \brief       Finalizes a Fls test scenario on ECU
 *  \details     The function calls the SetEvent function indicating that a Fls job is finished. The function is called
 *               either in context of the configured Fls job notifications, or in context of the cyclic call of the
 *               Fls_GetJobResult API
 *               The state of the Fls test module is reset to VITAHLP_FLS_IDLE
 *  \param[in]   payload            Pointer to payload data that is to be sent with this message, length is ensured
 *                                  by caller to be 6
 *  \param[in]   serviceID          Identifier to indicate the type of the finished Fls job
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous FALSE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Job_End(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payload,
  VAR(uint8, AUTOMATIC) serviceID);

/***********************************************************************************************************************
 *  vItaHlp_Fls_Ram2Ram_Compare_End()
 **********************************************************************************************************************/
/*! \brief       Finalizes the RAM to RAM compare on ECU
 *  \details     The function calls the SetEvent function indicating that the RAM to RAM compare is finished. The
 *               detailed outcome of the comparision is added as event parameter
 *               The state of the Fls test module is reset to VITAHLP_FLS_IDLE
 *  \param[in]   result             Result of the RAM to RAM compare
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous FALSE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Ram2Ram_Compare_End(
  VAR(boolean, AUTOMATIC) result);

/***********************************************************************************************************************
 *  vItaHlp_Fls_Write()
 **********************************************************************************************************************/
/*! \brief       Write data to flash
 *  \details     Start job to write to flash memory. The flash page address, the number of flash pages to write and the
 *               data pattern written to the flash memory are derived from the address mapping table based on the
 *               payloadData information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Write(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/***********************************************************************************************************************
 *  vItaHlp_Fls_Read()
 **********************************************************************************************************************/
/*! \brief       Read data from flash
 *  \details     Start job to read data from flash memory to RAM buffer. The flash page address of a first flash page
 *               and the number of bytes to read are derived from the address mapping table based on the payloadData
 *               information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Read(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

#  if (VITAHLP_FLS_COMPARE_API == STD_ON)
/***********************************************************************************************************************
 *  vItaHlp_Fls_Compare()
 **********************************************************************************************************************/
/*! \brief       Compare contents of RAM and flash
 *  \details     Start job to compare the contents of flash memory. The flash page address, the number of bytes to
 *               compare and the pattern to compare to are derived from the address mapping table based on the
 *               payloadData information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Compare(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

#  endif /* (VITAHLP_FLS_COMPARE_API == STD_ON) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_Erase()
 **********************************************************************************************************************/
/*! \brief       Erase flash memory
 *  \details     Start job to erase flash memory. The flash sector address and the number of sectors to be erased
 *               are derived from the address mapping table based on the payloadData information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Erase(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

#  if (VITAHLP_FLS_BLANKCHECK_API == STD_ON)
/***********************************************************************************************************************
*  vItaHlp_Fls_BlankCheck()
**********************************************************************************************************************/
/*! \brief       Performs BlankCheck of a flash memory area
 *  \details     Start job to perform a BlankCheck of a flash memory area. If not supported by the HW derivative,
 *               the expectation is that the triggered API call immediately returns with a negative result and no job
 *               is started.
 *               The flash page address and the number of bytes to be checked are derived from the address mapping
 *               table based on the payloadData information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_BlankCheck(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

#  endif /* (VITAHLP_FLS_BLANKCHECK_API == STD_ON) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_RAM2RAM_Compare()
 **********************************************************************************************************************/
/*! \brief       Compare both RAM buffers
 *  \details     Compare contents of RAM buffer used for Fls_Read and Fls_Write. The number of bytes to compare and the
 *               pattern to compare to are derived from the address mapping table based on the payloadData information
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_RAM2RAM_Compare(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
#   if (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF)
/***********************************************************************************************************************
 *  vItaHlp_Fls_AnalyzeJobResult()
 **********************************************************************************************************************/
/*! \brief       Reads the current Fls driver job status and checks if the job is finished.
 *  \details     The function may be called cyclically to evaluate the current Fls driver job status. If the Fls driver
 *               does not process a Fls job (!VITAHLP_FLS_BUSY), the function returns without any action.
 *               If the job is finished successfully, a JobEndNotification event is created.
 *               If the job is finished with error, a JobErrorNotification event is created.
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_AnalyzeJobResult(void);

#   endif /* (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_ConvertJobResultValue()
 **********************************************************************************************************************/
/*! \brief       Converts the Fls driver job status enumeration value to own vItaHlp data representation.
 *  \details     If the Fls driver job status enumeration value is unknown, the function returns
 *               VITAHLP_FLS_JOBRESULT_MEMIF_JOB_FAILED
 *  \param[in]   jobResult          Fls driver job status value to be converted
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Fls_ConvertJobResultValue(
  VAR(MemIf_JobResultType, AUTOMATIC) jobResult);

#  endif /* (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Fls_FillBufferWithPattern()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_FillBufferWithPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) buffer,
  VAR(uint16, AUTOMATIC) bufferLength,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) pattern,
  VAR(uint8, AUTOMATIC) patternLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16_least index;
  uint8_least patternIndex;

  /* ----- Implementation ----------------------------------------------- */
  patternIndex = 0u;
  for (index = 0u; index < bufferLength; index++)
  {
    if (patternIndex >= patternLength)
    {
      patternIndex = 0u;
    }
    buffer[index] = pattern[patternIndex];
    patternIndex++;
  }
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_GetAddressMapEntry()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Fls_GetAddressMapEntry(
  VAR(vItaHlp_Fls_AddressSelection_TableIndexType, AUTOMATIC) tableIndex,
  VAR(vItaHlp_Fls_FlsAddressType, AUTOMATIC) addressType,
  P2VAR(Fls_AddressType, AUTOMATIC, VITAHLP_APPL_VAR) address,
  P2VAR(Fls_LengthType, AUTOMATIC, VITAHLP_APPL_VAR) length,
  P2VAR(vItaHlp_Fls_InstanceIDType, AUTOMATIC, VITAHLP_APPL_VAR) driverInstance)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean mapEntryExists;

  /* ----- Implementation ----------------------------------------------- */
  mapEntryExists = FALSE;
  if(vItaHlp_Fls_AddressSelection_GetNumberOfAddressTableEntries() > tableIndex)
  {
    /* Table index is valid */
    driverInstance[0u] = vItaHlp_Fls_AddressSelectionData[tableIndex].flsInstanceId;
    if(addressType == VITAHLP_FLS_ADDRESSTYPE_PAGE)
    {
      address[0u] = vItaHlp_Fls_AddressSelectionData[tableIndex].pageStartAddress;
      length[0u] = vItaHlp_Fls_AddressSelectionData[tableIndex].pageBasedLength;
    }
    else
    {
      address[0u] = vItaHlp_Fls_AddressSelectionData[tableIndex].sectorStartAddress;
      length[0u] = vItaHlp_Fls_AddressSelectionData[tableIndex].sectorBasedLength;
    }
    mapEntryExists = TRUE;
  }
  return mapEntryExists;
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_ErrorMessage_InputInvalid()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_ErrorMessage_InputInvalid(
  VAR(uint8, AUTOMATIC) serviceID,
  VAR(uint8, AUTOMATIC) errorInfo)
{
  if(errorInfo == VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS)
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_FLS,
      serviceID,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_REQUESTED_PARAM,
      errorInfo);
  }
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_Job_End()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Job_End(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payload,
  VAR(uint8, AUTOMATIC) serviceID)
{
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_FLS,
    serviceID,
    payload);

  /* Ready for a next job */
  vItaHlp_Fls_Status = VITAHLP_FLS_IDLE;
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_Ram2Ram_Compare_End()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Ram2Ram_Compare_End(
  VAR(boolean, AUTOMATIC) result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  payloadBuffer[0u] = (uint8)result;

  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_FLS,
    VITAHLP_FLS_EVENT_RAM2RAM_COMPARE_DONE,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);

  /* Ready for a next job */
  vItaHlp_Fls_Status = VITAHLP_FLS_IDLE;
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_Write()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Write(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue;
  Fls_AddressType flsAddr;
  Fls_LengthType length;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_WriteFctPtrType flsWriteFctPtr;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_PAGE,
        &flsAddr,
        &length,
        &driverInstance) == TRUE)
  {
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
    /* Set the correct Fls_GetJobResult function pointer for use in vItaHlp_Fls_MainFunction() */
    flsGetJobResultFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsGetJobResultFctPtr;
#  endif

    /* Copy the data pattern */
    vItaHlp_Fls_FillBufferWithPattern(&FlashIOBuffer[0u], (uint16)length, &payloadData[1u], VITAHLP_FLS_PATTERN_LENGTH);

    /* Start to write to flash area */
    flsWriteFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsWriteFctPtr;
    returnValue = flsWriteFctPtr(flsAddr, &FlashIOBuffer[0u], length);
    if (returnValue == E_OK)
    {
      /* Set the BUSY state not before the Fls job is successfully started */
      vItaHlp_Fls_Status = VITAHLP_FLS_BUSY;
    }
    else
    {
      /* Failed to start job */
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_FLS_MODULE_ID,
        driverInstance,
        VITAHLP_FLS_SID_WRITE_ID,
        returnValue);
    }
  }
  else
  {
  /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_FLS_WRITE, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_Read()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Read(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue;
  Fls_AddressType flsAddr;
  Fls_LengthType length;
  vItaHlp_Fls_ReadFctPtrType flsReadFctPtr;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_PAGE,
        &flsAddr,
        &length,
        &driverInstance) == TRUE)
  {
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
    /* Set the correct Fls_GetJobResult function pointer for use in vItaHlp_Fls_MainFunction() */
    flsGetJobResultFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsGetJobResultFctPtr;
#  endif

    /* Clear the content of the FlashIOBuffer (fill with zero pattern) */
    vItaHlp_Fls_FillBufferWithPattern(&FlashIOBuffer[0u], (uint16)length, &ZeroPattern[0u], VITAHLP_FLS_PATTERN_LENGTH);

    /* Start to read from flash area */
    flsReadFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsReadFctPtr;
    returnValue = flsReadFctPtr(flsAddr, &FlashIOBuffer[0u], length);
    if (returnValue == E_OK)
    {
      /* Set the BUSY state not before the Fls job is successfully started */
      vItaHlp_Fls_Status = VITAHLP_FLS_BUSY;
    }
    else
    {
      /* Failed to start job */
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_FLS_MODULE_ID,
        driverInstance,
        VITAHLP_FLS_SID_READ_ID,
        returnValue);
    }
  }
  else
  {
    /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_FLS_READ, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}

#  if (VITAHLP_FLS_COMPARE_API == STD_ON)
/***********************************************************************************************************************
 *  vItaHlp_Fls_Compare()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Compare(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue = E_NOT_OK;
  Fls_AddressType flsAddr;
  Fls_LengthType length;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_CompareFctPtrType flsCompareFctPtr;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_PAGE,
        &flsAddr,
        &length,
        &driverInstance) == TRUE)
  {
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
    /* Set the correct Fls_GetJobResult function pointer for use in vItaHlp_Fls_MainFunction() */
    flsGetJobResultFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsGetJobResultFctPtr;
#  endif

    /* Copy the data pattern */
    vItaHlp_Fls_FillBufferWithPattern(&FlashIOBuffer[0u], (uint16)length, &payloadData[1u], VITAHLP_FLS_PATTERN_LENGTH);

    /* Start comparision of flash memory area */
    flsCompareFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsCompareFctPtr;
    if (flsCompareFctPtr != NULL_PTR) 
    {
      returnValue = flsCompareFctPtr(flsAddr, &FlashIOBuffer[0u], length);
    }
    if (returnValue == E_OK)
    {
      /* Set the BUSY state not before the Fls job is successfully started */
      vItaHlp_Fls_Status = VITAHLP_FLS_BUSY;
    }
    else
    {
      /* Failed to start the job */
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_FLS_MODULE_ID,
        driverInstance,
        VITAHLP_FLS_SID_COMPARE_ID,
        returnValue);
    }
  }
  else
  {
    /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_FLS_COMPARE, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}
#  endif /* (VITAHLP_FLS_COMPARE_API == STD_ON) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_Erase()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Erase(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue;
  Fls_AddressType flsAddr;
  Fls_LengthType length;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_EraseFctPtrType flsEraseFctPtr;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_SECTOR,
        &flsAddr,
        &length,
        &driverInstance) == TRUE)
  {
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
    /* Set the correct Fls_GetJobResult function pointer for use in vItaHlp_Fls_MainFunction() */
    flsGetJobResultFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsGetJobResultFctPtr;
#  endif

    /* Start to erase the flash memory */
    flsEraseFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsEraseFctPtr;
    returnValue = flsEraseFctPtr(flsAddr, length);
    if (returnValue == E_OK)
    {
      /* Set the BUSY state not before the Fls job is successfully started */
      vItaHlp_Fls_Status = VITAHLP_FLS_BUSY;
    }
    else
    {
      /* Failed to start the job */
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_FLS_MODULE_ID,
        driverInstance,
        VITAHLP_FLS_SID_ERASE_ID,
        returnValue);
    }
  }
  else
  {
    /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_FLS_ERASE, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}

#  if (VITAHLP_FLS_BLANKCHECK_API == STD_ON)
/***********************************************************************************************************************
*  vItaHlp_Fls_BlankCheck()
**********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_BlankCheck(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnValue = E_NOT_OK;
  Fls_AddressType flsAddr;
  Fls_LengthType length;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_BlankCheckFctPtrType flsBlankCheckFctPtr;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_PAGE,
        &flsAddr,
        &length,
        &driverInstance) == TRUE)
  {
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
    /* Set the correct Fls_GetJobResult function pointer for use in vItaHlp_Fls_MainFunction() */
    flsGetJobResultFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsGetJobResultFctPtr;
#  endif

    /* Start to perform the BlankCheck of the flash memory area */
    flsBlankCheckFctPtr = vItaHlp_Fls_DriverSpecificData[driverInstance].flsBlankCheckFctPtr;
    if (flsBlankCheckFctPtr != NULL_PTR)
    {
      returnValue = flsBlankCheckFctPtr(flsAddr, length);
    }
    if (returnValue == E_OK)
    {
      /* Set the BUSY state not before the Fls job is successfully started */
      vItaHlp_Fls_Status = VITAHLP_FLS_BUSY;
    }
    else
    {
      /* Failed to start the job */
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_FLS_MODULE_ID,
        driverInstance,
        VITAHLP_FLS_SID_BLANKCHECK_ID,
        returnValue);
    }
  }
  else
  {
    /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_FLS_BLANKCHECK, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}
#  endif /* (VITAHLP_FLS_BLANKCHECK_API == STD_ON) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_RAM2RAM_Compare()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_RAM2RAM_Compare(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Fls_LengthType bufferLength;
  Fls_LengthType index;
  Fls_AddressType flsAddr;
  vItaHlp_Fls_InstanceIDType driverInstance;
  vItaHlp_Fls_AddressSelection_TableIndexType addressTableIndex;
  uint8 patternIndex;
  boolean match;

  /* ----- Implementation ----------------------------------------------- */
  addressTableIndex = payloadData[0u];

  if (vItaHlp_Fls_GetAddressMapEntry(
        addressTableIndex,
        VITAHLP_FLS_ADDRESSTYPE_PAGE,
        &flsAddr,
        &bufferLength,
        &driverInstance) == TRUE)
  {
    /* Internal job processing can be started */
    vItaHlp_Fls_Status = VITAHLP_FLS_BUSY_INTERNAL;

    /* Compare previously retrieved flash memory content (FlashIOBuffer) with expected buffer content.
     * The expected buffer content (4 bytes) is given in payloadData[1u] to paylaodData[4u].
     */
    match = TRUE;
    patternIndex = 1u;
    for (index = 0u; index < bufferLength; index++)
    {
      if (patternIndex > VITAHLP_FLS_PATTERN_LENGTH)
      {
        patternIndex = 1u;
      }
      if (FlashIOBuffer[index] != payloadData[patternIndex])
      {
        match = FALSE;
        break;
      }
      patternIndex++;
    }
    vItaHlp_Fls_Ram2Ram_Compare_End(match);
  }
  else
  {
    /* Should not occur. Invalid generated data */
    vItaHlp_Fls_ErrorMessage_InputInvalid(VITAHLP_FLS_CMD_RAM2RAM_COMPARE, VITAHLP_FLS_INVALID_REQUESTED_PARAM_ADDRESS);
  }
}

#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
#   if (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF)
/***********************************************************************************************************************
 *  vItaHlp_Fls_AnalyzeJobResult()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Fls_AnalyzeJobResult(
  void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_Fls_Status == VITAHLP_FLS_BUSY)
  {
    /* Read the Fls Job status and trigger to send an event message back to the tester if the job is finished now. */
    if (flsGetJobResultFctPtr != NULL_PTR)
    {
      vItaHlp_Fls_JobResult = flsGetJobResultFctPtr();
      if (vItaHlp_Fls_JobResult != MEMIF_JOB_PENDING)
      {
        payloadBuffer[0u] = vItaHlp_Fls_ConvertJobResultValue(vItaHlp_Fls_JobResult);
#   if (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF)
        if (vItaHlp_Fls_JobResult == MEMIF_JOB_OK)
        {
          vItaHlp_Fls_Job_End(payloadBuffer, VITAHLP_FLS_EVENT_FEE_JOBENDNOTIFICATION);
        }
#   endif /* (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF) */
#   if (VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF)
        if (vItaHlp_Fls_JobResult != MEMIF_JOB_OK)
        {
          vItaHlp_Fls_Job_End(payloadBuffer, VITAHLP_FLS_EVENT_FEE_JOBERRORNOTIFICATION);
        }
#   endif /* (VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF) */
      }
    }
  }
}
#   endif /* (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF) */

/***********************************************************************************************************************
 *  vItaHlp_Fls_ConvertJobResultValue()
 **********************************************************************************************************************/
VITAHLP_LOCAL FUNC(uint8, VITAHLP_CODE) vItaHlp_Fls_ConvertJobResultValue(
  VAR(MemIf_JobResultType, AUTOMATIC) jobResult)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 convertedJobResultValue;

  /* ----- Implementation ----------------------------------------------- */
  switch (jobResult)
  {
    case MEMIF_JOB_OK:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_JOB_OK;
      break;
    }
    case MEMIF_JOB_FAILED:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_JOB_FAILED;
      break;
    }
    case MEMIF_JOB_PENDING:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_JOB_PENDING;
      break;
    }
    case MEMIF_JOB_CANCELED:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_JOB_CANCELED;
      break;
    }
    case MEMIF_BLOCK_INCONSISTENT:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_BLOCK_INCONSISTENT;
      break;
    }
    case MEMIF_BLOCK_INVALID:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_BLOCK_INVALID;
      break;
    }
    default:
    {
      convertedJobResultValue = VITAHLP_FLS_JOBRESULT_MEMIF_JOB_FAILED;
    }
  }
  return convertedJobResultValue;
}
#  endif /* (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  vItaHlp_Fls_Init()
 **********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* Init global data structures */
  vItaHlp_Fls_Status = VITAHLP_FLS_IDLE;
# if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
  flsGetJobResultFctPtr = NULL_PTR;
# endif
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Fls_DispatchService()
 **********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_Fls_Status == VITAHLP_FLS_IDLE)
  {
    switch (serviceIdentifier)
    {
      case VITAHLP_FLS_CMD_FLS_WRITE:
      {
        vItaHlp_Fls_Write(payloadData);
        break;
      }
      case VITAHLP_FLS_CMD_FLS_READ:
      {
        vItaHlp_Fls_Read(payloadData);
        break;
      }
#  if (VITAHLP_FLS_COMPARE_API == STD_ON)
      case VITAHLP_FLS_CMD_FLS_COMPARE:
      {
        vItaHlp_Fls_Compare(payloadData);
        break;
      }
#  endif
      case VITAHLP_FLS_CMD_FLS_ERASE:
      {
        vItaHlp_Fls_Erase(payloadData);
        break;
      }
#  if (VITAHLP_FLS_BLANKCHECK_API == STD_ON)
      case VITAHLP_FLS_CMD_FLS_BLANKCHECK:
      {
        vItaHlp_Fls_BlankCheck(payloadData);
        break;
      }
#  endif
      case VITAHLP_FLS_CMD_RAM2RAM_COMPARE:
      {
        vItaHlp_Fls_RAM2RAM_Compare(payloadData);
        break;
      }
      default:
      {
        vItaHlp_EventVItaHlpError(
          VITAHLP_COMPONENTIDENTIFIER_FLS,
          serviceIdentifier,
          VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
          VITAHLP_E_UNKNOWN_CMD,
          VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
        break;
      }
    }
  }
  else
  {
    /* Still busy with another job */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_FLS,
      serviceIdentifier,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_SERVICE_BUSY,
      0u);
  }
}

/***********************************************************************************************************************
 *  vItaHlp_Fls_MainFunction()
 **********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_MainFunction(void)
{
#  if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
#   if (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF)
  vItaHlp_Fls_AnalyzeJobResult();
#   endif /* (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_OFF || VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_OFF) */
#  endif /* (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON) */
}

/**********************************************************************************************************************
 *  APPLICATION SPECIFIC CALLBACK FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  vItaHlp_Fee_JobEndNotification()
 **********************************************************************************************************************/
#  if (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_ON)
FUNC(void, VITAHLP_CODE) vItaHlp_Fee_JobEndNotification(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Terminate the test scenario. Send indication to tester that the job is finished successfully */
  vItaHlp_Fls_Job_End(payloadBuffer, VITAHLP_FLS_EVENT_FEE_JOBENDNOTIFICATION);
}
#  endif /* (VITAHLP_FLS_JOB_END_NOTIFICATION == STD_ON) */

/***********************************************************************************************************************
 *  vItaHlp_Fee_JobErrorNotification()
 **********************************************************************************************************************/
#  if (VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_ON)
FUNC(void, VITAHLP_CODE) vItaHlp_Fee_JobErrorNotification(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Terminate the test scenario. Send indication to tester that the job is finished with error */
#   if (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON)
  if (flsGetJobResultFctPtr != NULL_PTR)
  {
    payloadBuffer[0u] = vItaHlp_Fls_ConvertJobResultValue(flsGetJobResultFctPtr());
    vItaHlp_Fls_Job_End(payloadBuffer, VITAHLP_FLS_EVENT_FEE_JOBERRORNOTIFICATION);
  }
#   else
    VITAHLP_DUMMY_STATEMENT_CONST(payloadBuffer);
#   endif /* (VITAHLP_FLS_GET_JOB_RESULT_API == STD_ON) */
}
#  endif /* (VITAHLP_FLS_JOB_ERROR_NOTIFICATION == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* (VITAHLP_FLS == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Fls.c
 *********************************************************************************************************************/

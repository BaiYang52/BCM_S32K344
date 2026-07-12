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
/**!       \file  NvM_MemIfAbstraction.h
 *        \brief  NvM_MemIfAbstraction header file
 *      \details  Header of MemIf Abstraction unit of the NvM.
 *                Depending on configuration the request is either forwarded to the MemIf on same Core or to a MemIf
 *                on another (memory) Core.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (NVM_MEMIFABSTRACTION)
# define NVM_MEMIFABSTRACTION

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "NvM_Cfg.h"
# include "NvM_PrivateCfg.h"

#if (NVM_JOB_FORWARDING_TO_MEMORY_CORE == STD_ON)
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef enum
{
    NVM_MEMIF_REQUEST_NONE,        /*< No request triggered */
    NVM_MEMIF_REQUEST_READ,        /*< Read request is triggered */
    NVM_MEMIF_REQUEST_WRITE,       /*< Write request is triggered */
    NVM_MEMIF_REQUEST_ERASE,       /*< Erase request is triggered */
    NVM_MEMIF_REQUEST_INVALIDATE  /*< Invalidate request is triggered */
} NvM_MemIfAbstraction_RequestType;

/*! Structure defining requests which shall be forwarded to MemIf placed on another Core */
typedef struct
{
    NvM_MemIfAbstraction_RequestType RequestAsync; /*< Asynchronous Request which shall be performed. */
    boolean RequestCancel; /*< Indicates if a cancel request shall be performed. */
    boolean RequestSetMode; /*< Indicates if a set mode request shall be performed. */
    MemIf_ModeType Mode; /*< Mode Parameter (for Synchronous SetMode request) */
    uint8 * DataBufferPtr; /*< Pointer to a Data Buffer, accessible by both cores (for Read and Write request) */
    uint16 BlockNumber; /*< Target Block Parameter (for Read, Write, Erase, Invalidate request) */
    uint16 BlockOffset; /*< Block Offset Parameter (for Read request) */
    uint16 Length; /*< Length Parameter (for Read request) */
    uint8 DeviceIndexAsync; /*< Device Index Parameter (all asynchronous requests) */
    uint8 DeviceIndexSync; /*< Device Index Parameter (for synchronous Cancel requests) */
} NvM_MemIfAbstraction_PublishedRequestType;

/*! Structure containing state information of the underlying devices placed on another Core */
typedef struct
{
    MemIf_StatusType Status; /*< Current status of the underlying device */
    MemIf_JobResultType JobResult; /* Current JobResult of the underlying device */
    Std_ReturnType LastRequestReturnValue; /* ReturnValue of Last Request,
                                              e.g. to get information if last request was rejected 
                                              from underlying device */
} NvM_MemIfAbstraction_RequestStateType;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define NVM_START_SEC_VAR_SHARED_DATA
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Variable where the current published requests are stored. At a time always only one asynchronous and one synchronous */
extern VAR(NvM_MemIfAbstraction_PublishedRequestType, NVM_PRIVATE_DATA)  NvM_MemIfAbstraction_PublishedRequest;

/*! Array where the current state of the underlying devices are stored. */
 /* The array stores the states and job results of all devices. In this way the satellite always knows the status
   of all devices. Furthermore the information, of which device the status is, must not be synchronized. */
extern VAR(NvM_MemIfAbstraction_RequestStateType, NVM_PRIVATE_DATA)  NvM_MemIfAbstraction_RequestState[MEMIF_NUMBER_OF_DEVICES];

#define NVM_STOP_SEC_VAR_SHARED_DATA
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_Init()
 **********************************************************************************************************************/
/*! \brief       Initialize MemIfAbstraction unit.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_Init(void);

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_Read()
 **********************************************************************************************************************/
/*! \brief       Forward read job to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \param[in]   BlockNumber - Number of the block which shall be read.
 *  \param[in]   BlockOffset - Read offset within block.
 *  \param[out]  DataBufferPtr - Pointer to the data buffer in which the read data shall be copied.
 *  \param[in]   Length - States the length of a read job
 *  \return      E_OK - Job accepted;
 *               E_NOT_OK otherwise.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_Read(
    uint8 DeviceIndex,
    uint16 BlockNumber,
    uint16 BlockOffset,
    uint8 *DataBufferPtr,
    uint16 Length);

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_Write()
 **********************************************************************************************************************/
/*! \brief       Forward write job to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \param[in]   BlockNumber - Number of the block which shall be written.
 *  \param[in]   DataBufferPtr - Pointer to the data which shall be written.
 *  \return      E_OK - Job accepted;
 *               E_NOT_OK otherwise.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_Write(
    uint8 DeviceIndex,
    uint16 BlockNumber,
    uint8 * DataBufferPtr);

# if (NVM_API_CONFIG_CLASS == NVM_API_CONFIG_CLASS_3)
/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_EraseImmediateBlock()
 **********************************************************************************************************************/
/*! \brief       Forward erase job to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \param[in]   BlockNumber - Number of the block which shall be erased.
 *  \return      E_OK - Job accepted;
 *               E_NOT_OK otherwise.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_EraseImmediateBlock(
    uint8 DeviceIndex,
    uint16 BlockNumber);

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_InvalidateBlock()
 **********************************************************************************************************************/
/*! \brief       Forward invalidate job to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \param[in]   BlockNumber - Number of the block which shall be invalidated.
 *  \return      E_OK - Job accepted;
 *               E_NOT_OK otherwise.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, NVM_CODE) NvM_MemIfAbstraction_InvalidateBlock(
     uint8 DeviceIndex,
     uint16 BlockNumber);
# endif

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_Cancel()
 **********************************************************************************************************************/
/*! \brief       Forward cancel to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_Cancel(
    uint8 DeviceIndex);

# if(NVM_DRV_MODE_SWITCH == STD_ON)
/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_SetMode()
 **********************************************************************************************************************/
/*! \brief       Forward SetMode to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   Mode - Mode which shall be forwarded.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, NVM_CODE) NvM_MemIfAbstraction_SetMode(
    MemIf_ModeType Mode);
# endif

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_GetStatus()
 **********************************************************************************************************************/
/*! \brief       Forward GetStatus to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \return      Status
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(MemIf_StatusType, NVM_CODE) NvM_MemIfAbstraction_GetStatus(
    uint8 DeviceIndex);

/***********************************************************************************************************************
 *  NvM_MemIfAbstraction_GetJobResult()
 **********************************************************************************************************************/
/*! \brief       Forward GetJobResult to MemIf.
 *  \details     Depending on configuration MemIf is either on same Core or placed at another (memory Core).
 *  \param[in]   DeviceIndex - Index of the device which shall be used.
 *  \return      JobResult
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(MemIf_JobResultType, NVM_CODE) NvM_MemIfAbstraction_GetJobResult(
    uint8 DeviceIndex);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* NVM_MEMIFABSTRACTION */

/**********************************************************************************************************************
 *  END OF FILE: NvM_MemIfAbstraction.h
 *********************************************************************************************************************/

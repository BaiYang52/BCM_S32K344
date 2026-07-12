/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*!
 *  \defgroup   Dem_Data Data
 *  \{
 *  \file       Dem_Data_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DATA_INTERFACE_H)
#define DEM_DATA_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Data_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Data_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Data_GetPtrToWriteIndexOfDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Returns pointer to write index in the Destination Buffer.
 *
 * \details       Returns pointer to write index in the Destination Buffer.
 *
 * \param[in]     DestinationBuffer
 *                Pointer to destination buffer structure
 *
 * \pre           -
 *
 * \return        Pointer to write index in the Destination Buffer.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );

/* ****************************************************************************
 * Dem_Data_GetRemainingSizeOfDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Returns the amount free space available on the buffer in bytes.
 *
 * \details       Returns the amount free space available on the buffer in bytes.
 *
 * \param[in]     DestinationBuffer
 *                Pointer to destination buffer structure
 * 
 *  \pre           -
 *
 * \return        The amount free space available on the buffer in bytes.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Data_GetRemainingSizeOfDestinationBuffer(
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );

/* ****************************************************************************
 * Dem_Data_PermanentEntryAdd
 *****************************************************************************/
/*!
 * \brief         Stores an event to permanent memory
 *
 * \details       If a free slot is available in the permanent memory, this
 *                function stores a permanent entry for the given event.
 *
 *                This function maps combined events to their master event, so
 *                any event can be passed.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryAdd(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Data_PermanentEntryUpdate
 *****************************************************************************/
/*!
 * \brief         Processes the permanent memory state update.
 *
 * \details       This function transitions the permanent memory states at the
 *                end of a driving cycle if ObdII and permanent memory is supported.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     OldDtcStatus
 *                Dtc status before DCY restart
 * \param[in]     NewDtcStatus
 *                Dtc status after DCY restart
 *
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryUpdate(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus,
  CONST(uint8, AUTOMATIC)  NewDtcStatus
  );

/* ****************************************************************************
 * Dem_Data_PermanentEntryUpdateOnDCyEnd
 *****************************************************************************/
/*!
 * \brief         Processes permanent state changes regarding clearDTC.
 *
 * \details       This function transitions the permanent memory states at the
 *                end of a driving cycle required for entry removal with ClearDTC
 *                if permanent memory for WwhObd is enabled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     OldDtcStatus
 *                Dtc status before DCY restart
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryUpdateOnDCyEnd(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus
  );

/* ****************************************************************************
 * Dem_Data_PermanentEntryClear
 *****************************************************************************/
/*!
 * \brief         Processes clearing a permanent DTC
 *
 * \details       Processes clearing a permanent DTC
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     ProcessClearDTC
 *                TRUE  Function is called from ClearDTC processing context
 *                FALSE Function is not called from ClearDTC processing context
 *
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryClear(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)   ProcessClearDTC
  );

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Data_PermanentEntryRemove
 *****************************************************************************/
/*!
 * \brief         Frees a permanent memory index
 *
 * \details       Frees a permanent memory index
 *
 * \param[in]     PermanentIndex
 *                Index of a permanent entry
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryRemove(
  CONST(uint8, AUTOMATIC)  PermanentIndex
  );
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Data_PermanentEntryFind
 *****************************************************************************/
/*!
 * \brief         Searches an eventId in the permanent memory
 *
 * \details       Searches an eventId in the permanent memory
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Index of the permanent entry used by the event. If no entry
 *                is found, Dem_Cfg_GlobalPermanentMaxCount() is returned.
 *
 * \pre           The passed event must be a master event
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Data_PermanentEntryFind(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif



#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
/* ****************************************************************************
 * Dem_Data_GetObdOdometer
 *****************************************************************************/
/*!
 * \brief         Requests the OBD Odometer from application and composes the
 *                returned data into an uint32 element.
 *
 * \details       Requests the OBD Odometer from application and composes the
 *                returned data into an uint32 element.
 *
 * \param[out]    Odometer
 *                Pointer to receive the value
 *
 * \return        E_OK
 *                An odometer value was successfully retrieved.
 * \return        E_NOT_OK
 *                No odometer could be retrieved. The Odometer parameter is
 *                initialized to 0.
 *
 * \pre           RTE is started (if using RTE)
 * \config        DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_GetObdOdometer(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_VAR_NOINIT)  Odometer
  );
#endif

/* ****************************************************************************
 * Dem_Data_InitDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Initialize passed destination buffer structure
 *
 * \details       Initialize passed destination buffer structure
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure that shall be initialized
 * \param[in]     Buffer
 *                Valid buffer that will be assigned to destination buffer structure
 * \param[in]     Size
 *                Size of passed buffer
 *
 * \pre           DestinationBuffer is a valid pointer
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_InitDestinationBuffer(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_SharedDataPtrType, AUTOMATIC) Buffer,
  CONST(uint16, AUTOMATIC) Size
  );

/* ****************************************************************************
 * Dem_Data_CheckAvailableSizeDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Check if the requested size is available in the DestinationBuffer.
 *
 * \details       Check if the requested size is available in the DestinationBuffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     RequestedSize
 *                Value by which write index shall be incremented
 *
 * \return        E_OK
 *                Requested size is available
 * \return        E_NOT_OK
 *                Requested size is not available
 *
 * \pre           DestinationBuffer is a valid pointer
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_CheckAvailableSizeDestinationBuffer(
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) RequestedSize
  );

/* ****************************************************************************
 * Dem_Data_IncrementWriteIndexDestinationBuffer
 *****************************************************************************/
 /*!
 * \brief         Increment write index of passed Destination buffer
 *
 * \details       Increment write index of passed destination buffer
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Size
 *                Value by which write index shall be incremented
 *
 * \pre           DestinationBuffer is a valid pointer
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_IncrementWriteIndexDestinationBuffer(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Size
);

/* ****************************************************************************
 * Dem_Data_ResetWriteIndexDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Reset write index
 *
 * \details       Reset write index of destination buffer structure
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_ResetWriteIndexDestinationBuffer(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBuffer
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy data from source buffer to destination buffer if available 
 *                buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     SourceBuffer
 *                Pointer to source buffer
 * \param[in]     Size
 *                Number of bytes to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBuffer(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_ConstSharedDataPtrType, AUTOMATIC) SourceBuffer,
  CONST(uint16, AUTOMATIC) Size
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferAtIndex
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint8 data to destination buffer at passed write index
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     WriteIndex
 *                Write index where to copy to
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferAtIndex(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) WriteIndex,
  CONST(uint8, AUTOMATIC) Data
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferUint8
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint8 data to destination buffer 
 *                if available buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint8(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint8, AUTOMATIC) Data
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferUint16
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint16 data in motorola byte order to destination buffer 
 *                if available buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferUint16_Intel
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint16 data in intel byte order to destination buffer 
 *                if available buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint16_Intel(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC) Data
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferUint32
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint32 data in motorola byte order to destination buffer 
 *                if available buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  );

/* ****************************************************************************
 * Dem_Data_WriteDestinationBufferUint32_Intel
 *****************************************************************************/
/*!
 * \brief         Copy data to destination buffer
 *
 * \details       Copy passed uint32 data in intel byte order to destination buffer 
 *                if available buffer size is sufficient
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to destination buffer structure
 * \param[in]     Data
 *                Data to be copied
 *
 * \pre           DestinationBuffer is a valid pointer
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_WriteDestinationBufferUint32_Intel(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint32, AUTOMATIC) Data
  );

/* ****************************************************************************
% Dem_Data_GetDestinationBufferWriteIndex
*****************************************************************************/
/*!
* \brief         Get the current write index of the destination buffer
*
* \details       Get the current write index of the destination buffer
*
* \param[in]     DestinationBuffer
*                Pointer to destination buffer structure
*
* \return        The current write index
*
* \pre           DestinationBuffer is a valid pointer
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Data_GetDestinationBufferWriteIndex(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DATA_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Data_Interface.h
 *********************************************************************************************************************/

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
 *         File:  Dem_Cfg_Declarations.h
 *      Project:  MICROSAR Diagnostic Event Manager (Dem)
 *       Module:  Configuration
 *    Generator:  -
 *
 *  Description:  Inline function declarations of the configuration subcomponent
 *
 *********************************************************************************************************************/
/*!
 *  \addtogroup   Dem_Ungrouped
 *  \{
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CFG_DECLARATIONS_H)
#define DEM_CFG_DECLARATIONS_H


/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "Dem.h"

                                                /* Other subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Satellite_Interface.h"

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ****************************************************************************
 * Dem_Cfg_ComplexIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Return value of Dem_Cfg_ComplexIterGet() is valid
 * \return        FALSE
 *                Return value of Dem_Cfg_ComplexIterGet() is invalid
 *
 * \pre           IterPtr has been initialized
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_ComplexIterExists(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_ComplexIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to a data element descriptor
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_Cfg_ComplexIterGet(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_ComplexIterClear
 *****************************************************************************/
/*!
 * \brief         Clear the iterator
 *
 * \details       Initializes the iterator and the size to zero
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_ComplexIterClear(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_ComplexIterInit
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator
 *
 * \details       Initializes the iterator to enumerate data elements
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     StartIndex
 *                The index of the first element
 * \param[in]     EndIndex
 *                The number of elements to iterate over
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_ComplexIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  CONST(uint16_least, AUTOMATIC)  StartIndex,
  CONST(uint16_least, AUTOMATIC)  EndIndex
  );

/* ****************************************************************************
 * Dem_Cfg_ComplexIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_ComplexIterNext(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_Cfg_CombinedGroupIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_CombinedGroupIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_CombinedGroupIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *                Feature event combination is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_CombinedGroupIterExists(
  CONSTP2CONST(Dem_Cfg_CombinedGroupIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_CombinedGroupIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        An EventId belonging to the combined group.
 *                DEM_EVENT_INVALID if combined groups are not supported.
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_CombinedGroupIterGet(
  CONSTP2CONST(Dem_Cfg_CombinedGroupIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_CombinedGroupIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate all EventIds belonging
 *                to the combined group.
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_CombinedGroupIterInit(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId,
  CONSTP2VAR(Dem_Cfg_CombinedGroupIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_CombinedGroupIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *                Feature event combination is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_CombinedGroupIterNext(
  CONSTP2VAR(Dem_Cfg_CombinedGroupIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );


#if (DEM_CFG_SUPPORT_OPCYCLE_STORAGE == STD_OFF) || (DEM_CFG_SUPPORT_OPCYCLE_AUTOMATIC_END == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_CycleIsAutomaticEnd
 *****************************************************************************/
/*!
 * \brief         Test if the cycle is configured for automatic end
 *
 * \details       Test if the cycle is configured for automatic end
 *
 * \param[in]     CycleId
 *                Unique Id of the cycle
 *
 * \return        TRUE
 *                The cycle is used for automatic end
 * \return        FALSE
 *                The cycle is not used for automatic end.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OPCYCLE_STORAGE == STD_OFF || DEM_CFG_SUPPORT_OPCYCLE_AUTOMATIC_END == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_CycleIsAutomaticEnd(
  CONST(uint8, AUTOMATIC)  CycleId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_CycleIsObdDcy
 *****************************************************************************/
/*!
 * \brief         Test if the cycle is configured to be the OBD driving cycle
 *
 * \details       Test if the cycle is configured to be the OBD driving cycle
 *
 * \param[in]     CycleId
 *                Unique Id of the cycle
 *
 * \return        TRUE
 *                The cycle is the OBD driving cycle
 * \return        FALSE
 *                The cycle is not the OBD driving cycle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_CycleIsObdDcy(
  CONST(uint8, AUTOMATIC)  CycleId
  );


/* ****************************************************************************
 * Dem_Cfg_CycleIsWarmup
 *****************************************************************************/
/*!
 * \brief         Test if the cycle is the warm-up cycle
 *
 * \details       Test if the cycle is the warm-up cycle
 *
 * \param[in]     CycleId
 *                Unique Id of the cycle
 *
 * \return        TRUE
 *                The cycle is the warm-up cycle
 * \return        FALSE
 *                The cycle is not the warm-up cycle
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_OBD == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_CycleIsWarmup(
  CONST(uint8, AUTOMATIC)  CycleId
  );

/* ****************************************************************************
 * Dem_Cfg_DataCallbackType
 *****************************************************************************/
/*!
 * \brief         Get the data element type
 *
 * \details       Get the data element type
 *
 * \param[in]     DataIndex
 *                Handle to a data element descriptor
 *
 * \return        The callback type \ref Dem_Cfg_DataElementTypes
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_DataCallbackType(
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );

#if ( (DEM_CFG_SUPPORT_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) \
   || (DEM_CFG_SUPPORT_WWHOBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
   || (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON) \
   || (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON) \
   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) )
/* ****************************************************************************
 * Dem_Cfg_DataCbkRead
 *****************************************************************************/
/*!
 * \brief         Get the callback function configured for a data element
 *
 * \details       Get the callback function configured for a data element
 *
 * \param[in]     DataIndex
 *                Handle to a data element descriptor
 *
 * \return        The configured callback function.
 *                Data elements that are implemented within the Dem return
 *                NULL_PTR.
 *
 * \pre           -
 * \config        ( (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
 *               || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *               || (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
 *               || (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
 *               || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ReadDataFPtrType, DEM_CODE)
Dem_Cfg_DataCbkRead(
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );

/* ****************************************************************************
 * Dem_Cfg_DataElementDataType
 *****************************************************************************/
/*!
 * \brief         Get the data type configured for a data element
 *
 * \details       Get the data type configured for a data element
 *
 * \param[in]     DataIndex
 *                Handle to a data element descriptor
 *
 * \return        The configured data type.
 *                Data elements that are implemented within the Dem return
 *                NULL_PTR.
 *
 * \pre           -
 * \config        ( (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
 *               || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *               || (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
 *               || (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
 *               || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_DataElementDataType(
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_DataIsStoredInNV
 *****************************************************************************/
/*!
 * \brief         Test if a data element is stored non-volatile
 *
 * \details       Test if a data element is stored non-volatile
 *
 * \param[in]     DataIndex
 *                Handle to a data element descriptor
 *
 * \return        TRUE
 *                The data element is stored non-volatile
 * \return        FALSE
 *                The data element is fetched with each read access
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_DataIsStoredInNV(
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );

/* ****************************************************************************
 * Dem_Cfg_DataSize
 *****************************************************************************/
/*!
 * \brief         Get the data size in bytes of a data element
 *
 * \details       Get the data size in bytes of a data element
 *
 * \param[in]     DataIndex
 *                Handle to a data element descriptor
 *
 * \return        The data size in bytes
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_DataSize(
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  );

/* ****************************************************************************
 * Dem_Cfg_DidBufferIndexFast
 *****************************************************************************/
/*!
 * \brief         Get the buffer offset in the fast collection for the DID
 *
 * \details       Get the buffer offset in the fast collection for the DID
 *
 * \param[in]     DidIndex
 *                Handle to DID descriptor table
 *
 * \return        The buffer offset in bytes
 *
 * \pre           DID must be configured for fast rate collection
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_DidBufferIndexFast(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );

/* ****************************************************************************
 * Dem_Cfg_DidBufferIndexNormal
 *****************************************************************************/
/*!
 * \brief         Get the offset in the normal collection buffer for the DID
 *
 * \details       Get the offset in the normal collection buffer for the DID
 *
 * \param[in]     DidIndex
 *                Handle to DID descriptor table
 *
 * \return        The buffer offset in bytes
 *
 * \pre           DID must be configured for normal rate collection
 * 
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_DidBufferIndexNormal(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );

/* ****************************************************************************
 * Dem_Cfg_DidDataIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_DidDataIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_DidDataIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_DidDataIterExists(
  CONSTP2CONST(Dem_Cfg_DidDataIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_DidDataIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to a data element descriptor
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_Cfg_DidDataIterGet(
  CONSTP2CONST(Dem_Cfg_DidDataIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_DidDataIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate data elements belonging
 *                to a data collection (DID)
 *
 * \param[in]     DidIndex
 *                Handle to DID descriptor table
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DidDataIterInit(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex,
  CONSTP2VAR(Dem_Cfg_DidDataIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_DidDataIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DidDataIterNext(
  CONSTP2VAR(Dem_Cfg_DidDataIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_DidNumber
 *****************************************************************************/
/*!
 * \brief         Get the external ID number configured for a data collection
 *
 * \details       Get the external ID number configured for a data collection.
 *                This can be a UDS DID number, extended data record number,
 *                PID number ...
 *                In case the external ID is only one byte in size, the number
 *                returned must be masked to the low byte. The upper byte is
 *                not guaranteed to be 0, i.e. due to the UDS mapping of PIDs.
 *
 * \param[in]     DidIndex
 *                Handle to DID descriptor table
 *
 * \return        The configured external ID number
 *
 * \pre           Either Dcm, or statistical data, or
 *                OBD is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_DidNumber(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );


/* ****************************************************************************
 * Dem_Cfg_DidSize
 *****************************************************************************/
/*!
 * \brief         Get the total size of the data collection (DID)
 *
 * \details       Get the sum of the size of the DIDs data elements, in bytes.
 *
 * \param[in]     DidIndex
 *                Handle to DID descriptor table
 *
 * \return        The DID size in bytes.
 *
 * \pre           Either Dcm, or statistical data, or
 *                OBD is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_DidSize(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) && (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 * Dem_Cfg_ERecTrigger
 *****************************************************************************/
/*!
 * \brief         Get the storage trigger of the extended data record.
 *
 * \details       Get the storage trigger of the extended data record.
 *
 * \param[in]     ERecIndex
 *                Handle to Extended Data Record descriptor table
 *
 * \return        The storage trigger \ref Dem_Cfg_StorageTriggerType
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON &
 *                DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_Cfg_ERecTrigger(
  CONST(Dem_Cfg_ERecIndexType, AUTOMATIC)  ERecIndex
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_ERecDid
 *****************************************************************************/
/*!
 * \brief         Get the data collection of an extended data record
 *
 * \details       Get the data collection of an extended data record.
 *
 * \param[in]     ERecIndex
 *                Handle to Extended Data Record descriptor table
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_ERecDid(
  CONST(Dem_Cfg_ERecIndexType, AUTOMATIC)  ERecIndex
  );

/* ****************************************************************************
 * Dem_Cfg_ERecType
 *****************************************************************************/
/*!
 * \brief         Get the storage type of the extended data record
 *
 * \details       Get the storage type of the extended data record
 *
 * \param[in]     ERecIndex
 *                Handle to Extended Data Record descriptor table
 *
 * \return        The storage type \ref Dem_Cfg_ERecStorageTypes
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_ERecType(
  CONST(Dem_Cfg_ERecIndexType, AUTOMATIC)  ERecIndex
  );

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_ERecUpdate
 *****************************************************************************/
/*!
 * \brief         Get the update setting of the extended data record
 *
 * \details       Get the update setting of the extended data record
 *
 * \param[in]     ERecIndex
 *                Handle to Extended Data Record descriptor table
 *
 * \return        TRUE
 *                The record is updated on every update trigger
 * \return        FALSE
 *                The record is updated only when not already stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_ERecUpdate(
  CONST(Dem_Cfg_ERecIndexType, AUTOMATIC)  ERecIndex
  );
#endif

#if (DEM_FEATURE_NEED_EVENT_PRECONFIRMED == STD_OFF)
/* ****************************************************************************
 * Dem_Cfg_EventAgingCycle
 *****************************************************************************/
/*!
 * \brief         Get the aging cycle configured for an event
 *
 * \details       Get the aging cycle configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique Id of the events aging cycle
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EVENT_PRECONFIRMED == STD_OFF
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventAgingCycle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventAgingTarget
 *****************************************************************************/
/*!
 * \brief         Get the aging target configured for an event
 *
 * \details       Get the aging target configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Number of cycles required to age the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventAgingTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventAvailableByDefault
 *****************************************************************************/
/*!
 * \brief         Get the default availability state of an event
 *
 * \details       Get the default availability state of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is available after Dem initialization
 * \return        FALSE
 *                The event is not available after Dem initialization
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventAvailableByDefault(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventAvailableByVariant
 *****************************************************************************/
/*!
 * \brief         Get the availability state of an event in the active variant
 *
 * \details       Get the availability state of an event in the active variant
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is available in the active variant
 * \return        FALSE
 *                The event is not available in the active variant
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventAvailableByVariant(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventCbkClearAllowed
 *****************************************************************************/
/*!
 * \brief         Get the clear permission callback for an event
 *
 * \details       Get the clear permission callback for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Function pointer to the clear permission callback.
 *                NULL_PTR if the event does not have a callback configured.
 *
 * \pre           -
 * \config        DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ClearEventAllowedFPtrType, DEM_CODE)
Dem_Cfg_EventCbkClearAllowed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventCbkData
 *****************************************************************************/
/*!
 * \brief         Get the data changed notification callback for an event
 *
 * \details       Get the data changed notification callback for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Function pointer to the data changed notification callback.
 *                NULL_PTR if the event does not have a callback configured.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventDataChangedFPtrType, DEM_CODE)
Dem_Cfg_EventCbkData(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkGetFDC
 *****************************************************************************/
/*!
 * \brief         Get the 'fetch fault detection counter' callback for an event
 *
 * \details       Get the 'fetch fault detection counter' callback for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Function pointer to the configured callback.
 *                NULL_PTR if the event does not have a callback configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DEBOUNCE_MONITORINTERNAL == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_GetFDCFPtrType, DEM_CODE)
Dem_Cfg_EventCbkGetFDC(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitFuncIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventCbkInitFuncIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventCbkInitFuncIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventCbkInitFuncIterExists(
  CONSTP2CONST(Dem_Cfg_EventCbkInitFuncIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitFuncIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Pointer to function initialization callback
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_InitMonitorForFuncFPtrType, DEM_CODE)
Dem_Cfg_EventCbkInitFuncIterGet(
  CONSTP2CONST(Dem_Cfg_EventCbkInitFuncIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitFuncIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate function initialization
 *                callbacks configured for an EventId
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventCbkInitFuncIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_EventCbkInitFuncIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitFuncIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventCbkInitFuncIterNext(
  CONSTP2VAR(Dem_Cfg_EventCbkInitFuncIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitMonitor
 *****************************************************************************/
/*!
 * \brief         Get the monitor initialization callback for an event
 *
 * \details       Get the monitor initialization callback for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Function pointer to the monitor initialization callback.
 *                NULL_PTR if the event does not have a callback configured.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_InitMonitorForEventFPtrType, DEM_CODE)
Dem_Cfg_EventCbkInitMonitor(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkStatusIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventCbkStatusIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventCbkStatusIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventCbkStatusIterExists(
  CONSTP2CONST(Dem_Cfg_EventCbkStatusIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkStatusIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Pointer to event status change notification callback
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventStatusChangedFPtrType, DEM_CODE)
Dem_Cfg_EventCbkStatusIterGet(
  CONSTP2CONST(Dem_Cfg_EventCbkStatusIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkInitFuncIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate status change notification
 *                callbacks configured for an EventId
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventCbkStatusIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_EventCbkStatusIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCbkStatusIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventCbkStatusIterNext(
  CONSTP2VAR(Dem_Cfg_EventCbkStatusIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventCombinedGroup
 *****************************************************************************/
/*!
 * \brief         Get the combined group of an event
 *
 * \details       Get the combined group of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique identifier of the combined event group.
 *                DEM_CFG_COMBINED_GROUP_INVALID if the event does not belong
 *                to a combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_CombinedGroupIndexType, DEM_CODE)
Dem_Cfg_EventCombinedGroup(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventIsCombined
 *****************************************************************************/
/*!
 * \brief         Test if the event belongs to a combined group
 *
 * \details       Test if the event belongs to a combined group
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event does belong to a combined group
 * \return        FALSE
 *                The event does not belong to a combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventIsCombined(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventDebounceContinuous
 *****************************************************************************/
/*!
 * \brief         Test if the event uses debounce continuous
 *
 * \details       Test if the events debounce counter continues while enable
 *                conditions are disabled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event uses debounce continuous
 * \return        FALSE
 *                The event does not use debounce continuous
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventDebounceContinuous(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventDebounceReset
 *****************************************************************************/
/*!
 * \brief         Test if the event uses debounce reset
 *
 * \details       Test if the events debounce counter resets when enable
 *                conditions of the event are re-enabled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event uses debounce reset
 * \return        FALSE
 *                The event does not use debounce reset
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventDebounceReset(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventDebounceType
 *****************************************************************************/
/*!
 * \brief         Get the debounce algorithm of the event
 *
 * \details       Get the debounce algorithm of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The debounce algorithm \ref Dem_Cfg_DebounceType
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventDebounceType(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventDestination
 *****************************************************************************/
/*!
 * \brief         Get the memory an event belongs to
 *
 * \details       Get the memory an event belongs to
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique identifier of the event memory
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventDestination(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventDidIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventDidIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventDidIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventDidIterExists(
  CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventDidIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_EventDidIterGet(
  CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventDidIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for an
 *                event (i.e. Snapshot content)
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventDidIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventDidIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventDidIterNext(
  CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSnapshotDidIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_GlobalSnapshotDidIterExists() will return a valid
 *                element on this iterator
 * \return        FALSE
 *                Dem_Cfg_GlobalSnapshotDidIterExists() is undefined on this
 *                iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_GlobalSnapshotDidIterExists(
  CONSTP2CONST(Dem_Cfg_GlobalSnapshotDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSnapshotDidIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_GlobalSnapshotDidIterGet(
  CONSTP2CONST(Dem_Cfg_GlobalSnapshotDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSnapshotDidIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for a
 *                global snapshot record.
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalSnapshotDidIterInit(
  CONSTP2VAR(Dem_Cfg_GlobalSnapshotDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSnapshotDidIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalSnapshotDidIterNext(
  CONSTP2VAR(Dem_Cfg_GlobalSnapshotDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventFailedTarget
 *****************************************************************************/
/*!
 * \brief         Get the failed threshold for the event
 *
 * \details       Get the failed threshold for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Failed threshold for the event.
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventFailedTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventFunctionalUnit
 *****************************************************************************/
/*!
 * \brief         Get the functional unit configured for the event
 *
 * \details       Get the functional unit configured for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Functional unit of the event
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventFunctionalUnit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventHealingTarget
 *****************************************************************************/
/*!
 * \brief         Get the number of healing cycles of the event
 *
 * \details       Get the number of cycles required to heal the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Number of cycles to heal the event
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventHealingTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventJ1939Dtc
 *****************************************************************************/
/*!
 * \brief         Get the J1939 DTC number of an event
 *
 * \details       Get the J1939 DTC number (SPN+FMI) of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        J1939 DTC number of the event.
 *                DEM_CFG_DTC_J1939_INVALID if the DTC does not have a SPN and
 *                FMI configured
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_EventJ1939Dtc(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventJ1939ExpFFExists
 *****************************************************************************/
/*!
 * \brief         Test if the event stores an expanded J1939 freeze frame
 *
 * \details       Test if the event stores an expanded J1939 freeze frame
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event stores an expanded J1939 freeze frame
 * \return        FALSE
 *                The event does not store an expanded J1939 freeze frame
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventJ1939ExpFFExists(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventJ1939FFExists
 *****************************************************************************/
/*!
 * \brief         Test if the event stores a J1939 freeze frame
 *
 * \details       Test if the event stores a J1939 freeze frame
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event stores a J1939 freeze frame
 * \return        FALSE
 *                The event does not store a J1939 freeze frame
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventJ1939FFExists(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


/* ****************************************************************************
 * Dem_Cfg_EventJ1939NodeIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventJ1939NodeIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventJ1939NodeIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventJ1939NodeIterExists(
  CONSTP2CONST(Dem_Cfg_EventJ1939NodeIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );


/* ****************************************************************************
 * Dem_Cfg_EventJ1939NodeIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        A NodeIndex for which the event is defined
 *
 * \pre           IterPtr has been initialized 
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventJ1939NodeIterGet(
  CONSTP2CONST(Dem_Cfg_EventJ1939NodeIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventJ1939NodeIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate J1939 nodes configured
 *                for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           - 
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventJ1939NodeIterInit(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONSTP2VAR(Dem_Cfg_EventJ1939NodeIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );


/* ****************************************************************************
 * Dem_Cfg_EventJ1939NodeIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventJ1939NodeIterNext(
  CONSTP2VAR(Dem_Cfg_EventJ1939NodeIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventJumpDownValue
 *****************************************************************************/
/*!
 * \brief         Get the jump-down debounce value configured for the event
 *
 * \details       Get the jump-down debounce value configured for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The debounce value used for jump-down
 *
 * \pre           The event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventJumpDownValue(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventJumpUpValue
 *****************************************************************************/
/*!
 * \brief         Get the jump-up debounce value configured for the event
 *
 * \details       Get the jump-up debounce value configured for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The debounce value used for jump-up
 *
 * \pre           The event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventJumpUpValue(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventLatchTestFailed
 *****************************************************************************/
/*!
 * \brief         Test if the event uses latched test failed
 *
 * \details       Test if the event uses latched test failed
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event uses latched test failed
 * \return        FALSE
 *                The event does not use latched test failed
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventLatchTestFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventMilGroupIndex
 *****************************************************************************/
/*!
 * \brief         Get the MIL group of the event
 *
 * \details       Get the MIL group of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique handle of the MIL group
 *                DEM_CFG_MIL_GROUP_INVALID if the event does not support a
 *                MIL group
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_MIL_GROUPS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventMilGroupIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventObdDidIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventObdDidIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventObdDidIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventObdDidIterExists(
  CONSTP2CONST(Dem_Cfg_EventObdDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventObdDidIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_EventObdDidIterGet(
  CONSTP2CONST(Dem_Cfg_EventObdDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventObdDidIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for an
 *                event's WWH-OBD freeze frame
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventObdDidIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_EventObdDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventObdDidIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventObdDidIterNext(
  CONSTP2VAR(Dem_Cfg_EventObdDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_EventObdDtc
 *****************************************************************************/
/*!
 * \brief         Get the OBD DTC number of an event
 *
 * \details       Get the OBD DTC number of en event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        OBD DTC number of the event.
 *                DEM_CFG_DTC_OBD_INVALID if the DTC does not have an OBD DTC
 *                number configured
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventObdDtc(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventOperationCycle
 *****************************************************************************/
/*!
 * \brief         Get the operation cycle configured for an event
 *
 * \details       Get the operation cycle configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique handle of the cycle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventOperationCycle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventPassedTarget
 *****************************************************************************/
/*!
 * \brief         Get the passed threshold for the event
 *
 * \details       Get the passed threshold for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Passed threshold for the event.
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventPassedTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventPreconfirmedTarget
 *****************************************************************************/
/*!
 * \brief         Get the pre-confirmed threshold for the event
 *
 * \details       Get the pre-confirmed threshold for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Pre-confirmed threshold for the event.
 *
 * \pre           Event must be configured for counter based debouncing
 *                Feature preconfirmed storage is supported.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventPreconfirmedTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Cfg_EventPrefailedStep
 *****************************************************************************/
/*!
 * \brief         Get the failed step increment for the event
 *
 * \details       Get the failed step increment for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The failed step increment (value > 0)
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventPrefailedStep(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventPrepassedStep
 *****************************************************************************/
/*!
 * \brief         Get the passed step decrement for the event
 *
 * \details       Get the passed step decrement for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The passed step decrement (value > 0)
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint16, DEM_CODE)
Dem_Cfg_EventPrepassedStep(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventPrestorageIndex
 *****************************************************************************/
/*!
 * \brief         Get the pre-storage index of the event
 *
 * \details       Get the pre-storage index of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The pre-storage index of the event.
 * \return        DEM_PRESTORE_NOT_CONFIGURED_FOR_EVENT
 *                Flag for 'prestorage is not configured for this event'.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventPrestorageIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if ( (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
   || ( ( (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) || (DEM_FEATURE_NEED_OBD == STD_ON) ) && (DEM_CFG_DATA_PRIORITY == STD_ON)) )
/* ****************************************************************************
 * Dem_Cfg_EventPriority
 *****************************************************************************/
/*!
 * \brief         Get the priority of the event
 *
 * \details       Get the priority of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The priority of the event
 *
 * \pre           -
 * \config        ( (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *               || ( ( (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON)
 *                   || (DEM_FEATURE_NEED_OBD == STD_ON) )
 *                 && (DEM_CFG_DATA_PRIORITY == STD_ON) ) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventPriority(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_EventSatelliteEventId
 *****************************************************************************/
/*!
 * \brief         Get the satellite specific event Id of an event
 *
 * \details       Get the satellite specific event Id of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The satellite specific event Id of the given event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_EventSatelliteEventId(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  );

/* ****************************************************************************
 * Dem_Cfg_EventSatelliteId
 *****************************************************************************/
/*!
 * \brief         Get the satellite Id of an event
 *
 * \details       Get the satellite Id of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The satellite Id of the given event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Satellite_IdType, DEM_CODE)
Dem_Cfg_EventSatelliteId(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  );

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSPNIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_EventSPNIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_EventSPNIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSPNIterExists(
  CONSTP2CONST(Dem_Cfg_EventSPNIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSPNIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_EventSPNIterGet(
  CONSTP2CONST(Dem_Cfg_EventSPNIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSPNIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for an
 *                event's J1939 freeze frame
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventSPNIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_EventSPNIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSPNIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_EventSPNIterNext(
  CONSTP2VAR(Dem_Cfg_EventSPNIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif



#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ( (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
     || (DEM_CFG_SUPPORT_SRECS == STD_ON) ) )
/* ****************************************************************************
 * Dem_Cfg_EventSRecUdsSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the events snapshot data
 *
 * \details       This function returns the size of the UDS formatted snapshot
 *                data. This includes the DID number and all DID data, also if
 *                the DID does not contain NV data.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Size in bytes of the events snapshot data. Events without
 *                configured snapshot return 0.
 *
 * \pre           -
 * \config        ( (DEM_CFG_SUPPORT_DCM == STD_ON)
 *               && ( (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                 || (DEM_CFG_SUPPORT_SRECS == STD_ON) ) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventSRecUdsSize(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
);
#endif

/* ****************************************************************************
 * Dem_Cfg_GlobalSRecUdsSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the global snapshot in UDS format.
 *
 * \details       Get the size of the global snapshot in UDS format.
 *
 * \return        Size in bytes of the global snapshot in UDS format.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalSRecUdsSize(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_EventSeverity
 *****************************************************************************/
/*!
 * \brief         Get the severity configured for the event
 *
 * \details       Get the severity configured for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Severity of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCSeverityType, DEM_CODE)
Dem_Cfg_EventSeverity(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventSignificance
 *****************************************************************************/
/*!
 * \brief         Get the significance configured for the event
 *
 * \details       Get the significance configured for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Significance of the event \ref Dem_Cfg_SignificanceTypes
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventSignificance(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventStorageTarget
 *****************************************************************************/
/*!
 * \brief         Get the storage threshold for the event
 *
 * \details       Get the storage threshold for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The storage threshold (value >0)
 *
 * \pre           Event must be configured for counter based debouncing
 *                Feature event storage target is enabled.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventStorageTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventSupportAging
 *****************************************************************************/
/*!
 * \brief         Test if the event may age
 *
 * \details       Test if the event may age
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event may age
 * \return        TRUE
 *                The event may not age
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSupportAging(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSupportAgedCounter
 *****************************************************************************/
/*!
 * \brief         Test if the event has an entry in the aged counter array
 *
 * \details       Test if the event has an entry in the aged counter array
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event has an entry in the aged counter array
 * \return        FALSE
 *                The event has no entry in the aged counter array
 *
 * \pre           -
 * \config        DEM_CFG_DATA_AGED_COUNTER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSupportAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_IsPermanentSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if permanent memory is supported in this (PBS) WWHOBD variant.
 *
 * \details       Test if permanent memory is supported in this (PBS) WWHOBD variant.
 *
 * \return        TRUE
 *                Permanent memory supported.
 * \return        FALSE
 *                Permanent memory not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsPermanentSupportedInVariant(
  void
);

/* ****************************************************************************
 * Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if OBD-II or OBDonUDS is supported in this (PBS) variant.
 *
 * \details       Test if OBD-II or OBDonUDS is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                OBD-II or OBDonUDS supported in this (PBS) variant.
 * \return        FALSE
 *                OBD-II or OBDonUDS not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant(
  void
);


/* ****************************************************************************
 * Dem_Cfg_IsObdIIExclusivelySupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if OBD-II and not OBD on UDS is supported in this (PBS) variant.
 *
 * \details       Test if OBD-II and not OBD on UDS is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                OBD-II and not OBD on UDS supported in this (PBS) variant.
 * \return        FALSE
 *                OBD-II and not OBD on UDS not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdIIExclusivelySupportedInVariant(
  void
);


/* ****************************************************************************
 * Dem_Cfg_IsObdOnUdsSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if OBD on UDS is supported in this (PBS) variant.
 *
 * \details       Test if OBD on UDS is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                OBD on UDS supported in this (PBS) variant.
 * \return        FALSE
 *                OBD on UDS not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdOnUdsSupportedInVariant(
  void
);

/* ****************************************************************************
 * Dem_Cfg_IsObdOnUdsSupported
 *****************************************************************************/
/*!
 * \brief         Test if OBD on UDS is supported.
 *
 * \details       Test if OBD on UDS is supported. If there are multiple
 *                (PBS) variants, test if OBD on UDs is supported in any of
 *                the (PBS) variants.
 *
 * \return        TRUE
 *                OBD on UDS supported.
 * \return        FALSE
 *                OBD on UDS not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdOnUdsSupported(
  void
);

/* ****************************************************************************
 * Dem_Cfg_IsSRecSupportEnabled
 *****************************************************************************/
/*!
 * \brief         Tests whether Snapshot Records Support is enabled.
 *
 * \details       Tests whether Snapshot Records Support is enabled.
 *
 * \return        TRUE
 *                SRec support is enabled.
 * \return        FALSE
 *                SRec support is NOT enabled.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsSRecSupportEnabled(
void
);

/* ****************************************************************************
 * Dem_Cfg_IsGlobalSRecSupportEnabled
 *****************************************************************************/
/*!
 * \brief         Tests whether Global Snapshot Records Support is enabled.
 *
 * \details       Tests whether Global Snapshot Records Support is enabled.
 *
 * \return        TRUE
 *                Global SRec support is enabled.
 * \return        FALSE
 *                Global SRec support is NOT enabled.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsGlobalSRecSupportEnabled(
void
);

/* ****************************************************************************
 * Dem_Cfg_IsWwhObdSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if WWH-OBD is supported in this (PBS) variant.
 *
 * \details       Test if WWH-OBD is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                WWH-OBD supported in this (PBS) variant.
 * \return        FALSE
 *                WWH-OBD not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsWwhObdSupportedInVariant(
  void
);

/* ****************************************************************************
 * Dem_Cfg_IsObdSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test if OBD (OBD-II or WWH-OBD) is supported in this (PBS) variant.
 *
 * \details       Test if OBD (OBD-II or WWH-OBD) is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                OBD supported in this (PBS) variant.
 * \return        FALSE
 *                OBD not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdSupportedInVariant(
  void
);

/* ****************************************************************************
 * Dem_Cfg_IsDtcFormatSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Return if DTC format is supported in this (PBS) variant.
 *
 * \details       Return if DTC format is supported in this (PBS) variant.
 *
 * \return        TRUE
 *                DTC format is supported in this (PBS) variant.
 * \return        FALSE
 *                DTC format is not supported in this (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsDtcFormatSupportedInVariant(
  Dem_DTCFormatType DTCFormat
);

/* ****************************************************************************
 * Dem_Cfg_IsObdIIFreezeFrameInService19Enabled
 *****************************************************************************/
/*!
 * \brief         Return if OBDII freeze frame is supported in service 19.
 * \details       -
 *
 * \return        TRUE
 *                OBD-II freeze frame supported in service 19
 * \return        FALSE
 *                OBD-II freeze frame not supported in service 19
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_IsObdIIFreezeFrameInService19Enabled(
  void
);

/* ****************************************************************************
 * Dem_Cfg_EventSupportImmediateNv
 *****************************************************************************/
/*!
 * \brief         Test if the events data updates are persisted immediately
 *
 * \details       Test if the events data updates are persisted immediately
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The events data updates are persisted immediately
 * \return        TRUE
 *                The events data updates are not persisted immediately
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSupportImmediateNv(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventSupportJumpDown
 *****************************************************************************/
/*!
 * \brief         Test if the event uses jump-down
 *
 * \details       Test if the event uses jump-down
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event uses jump-down
 * \return        TRUE
 *                The events does not use jump-down
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        DEM_CFG_SUPPORT_DEBOUNCE_COUNTER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSupportJumpDown(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventSupportJumpUp
 *****************************************************************************/
/*!
 * \brief         Test if the event uses jump-up
 *
 * \details       Test if the event uses jump-up
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event uses jump-up
 * \return        TRUE
 *                The events does not use jump-up
 *
 * \pre           Event must be configured for counter based debouncing
 * \config        DEM_CFG_SUPPORT_DEBOUNCE_COUNTER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventSupportJumpUp(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventTripTarget
 *****************************************************************************/
/*!
 * \brief         Get the number of trips to confirm the event
 *
 * \details       Get the number of trips to confirm the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The number of trips to confirm the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventTripTarget(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventUdsDtc
 *****************************************************************************/
/*!
 * \brief         Get the UDS DTC of the event
 *
 * \details       Get the UDS DTC of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The UDS DTC of the event
 *                DEM_CFG_DTC_UDS_INVALID if the event does not have a UDS DTC
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_EventUdsDtc(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventWwhObdFFUdsSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the events WWH-OBD snapshot data
 *
 * \details       This function returns the size of the UDS formatted snapshot
 *                data. This includes the DID number and all DID data, also if
 *                the DID does not contain NV data.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Size in bytes of the events WWH-OBD snapshot data. Events
 *                without configured snapshot return 0.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventWwhObdFFUdsSize(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_EventObdOnUdsFFUdsSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the events OBD on UDS snapshot data
 *
 * \details       This function returns the size of the UDS formatted snapshot
 *                data. This includes the DID number and all DID data, also if
 *                the DID does not contain NV data.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Size in bytes of the events OBD on UDS snapshot data. Events
 *                without configured snapshot return 0.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventObdOnUdsFFUdsSize(
  Dem_EventIdType EventId
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCbkControlDtcSetting
 *****************************************************************************/
/*!
 * \brief         Get the ControlDTCSetting notification callback
 *
 * \details       Get the ControlDTCSetting notification callback
 *
 * \return        Function pointer to call
 *                NULL_PTR if no such callback is configured
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ControlDtcSettingChangedFPtrType, DEM_CODE)
Dem_Cfg_GlobalCbkControlDtcSetting(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkData
 *****************************************************************************/
/*!
 * \brief         Get the global data changed notification callback
 *
 * \details       Get the global data changed notification callback
 *
 * \return        Function pointer to call
 *                NULL_PTR if no such callback is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_GlobalDataChangedFPtrType, DEM_CODE)
Dem_Cfg_GlobalCbkData(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkDtcIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_GlobalCbkDtcIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_GlobalCbkDtcIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_GlobalCbkDtcIterExists(
  CONSTP2CONST(Dem_Cfg_GlobalCbkDtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkDtcIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Pointer to a DTC status changed notification callback
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DtcStatusChangedFPtrType, DEM_CODE)
Dem_Cfg_GlobalCbkDtcIterGet(
  CONSTP2CONST(Dem_Cfg_GlobalCbkDtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkDtcIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DTC status changed
 *                notification callbacks
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalCbkDtcIterInit(
  CONSTP2VAR(Dem_Cfg_GlobalCbkDtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkDtcIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalCbkDtcIterNext(
  CONSTP2VAR(Dem_Cfg_GlobalCbkDtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCbkJ1939DtcIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_GlobalCbkJ1939DtcIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_GlobalCbkJ1939DtcIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_GlobalCbkJ1939DtcIterExists(
  CONSTP2CONST(Dem_Cfg_GlobalCbkJ1939DtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCbkJ1939DtcIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Pointer to a J1939 DTC status changed notification callback
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DtcStatusChangedFPtrType, DEM_CODE)
Dem_Cfg_GlobalCbkJ1939DtcIterGet(
  CONSTP2CONST(Dem_Cfg_GlobalCbkJ1939DtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCbkJ1939DtcIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate J1939 DTC status changed
 *                notification callbacks
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalCbkJ1939DtcIterInit(
  CONSTP2VAR(Dem_Cfg_GlobalCbkJ1939DtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCbkJ1939DtcIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalCbkJ1939DtcIterNext(
  CONSTP2VAR(Dem_Cfg_GlobalCbkJ1939DtcIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_GlobalCbkData
 *****************************************************************************/
/*!
 * \brief         Get the global status changed notification callback
 *
 * \details       Get the global status changed notification callback
 *
 * \return        Function pointer to call
 *                NULL_PTR if no such callback is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_GlobalStatusChangedFPtrType, DEM_CODE)
Dem_Cfg_GlobalCbkStatus(
  void
  );

#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON || DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalCombinedGroupCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured combined events
 *
 * \details       Get the number of configured combined events
 *
 * \return        The number of configured combined events
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON OR
 *                DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_CombinedGroupIndexType, DEM_CODE)
Dem_Cfg_GlobalCombinedGroupCount(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_GlobalConfigurationId
 *****************************************************************************/
/*!
 * \brief         Get the configuration ID
 *
 * \details       Get the configuration ID
 *
 * \return        The configuration ID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalConfigurationId(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalCycleCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured cycles
 *
 * \details       Get the number of configured cycles
 *
 * \return        The number of configured cycles
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalCycleCount(
  void
  );

#if ( (DEM_DEV_RUNTIME_CHECKS == STD_ON) \
   && ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) \
     || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
     || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) \
     || (DEM_CFG_SUPPORT_WWHOBD == STD_ON) \
     || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
     || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
     || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) ) )
/* ****************************************************************************
 * Dem_Cfg_GlobalDataElementCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured data elements
 *
 * \details       Get the number of configured data elements
 *
 * \return        The number of configured data elements
 *
 * \pre           -
 * \config        ( (DEM_DEV_RUNTIME_CHECKS == STD_ON)
 *               && ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
 *                 || (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                 || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
 *                 || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
 *                 || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *                 || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                 || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) ) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalDataElementCount(
  void
  );
#endif


/* ****************************************************************************
 * Dem_Cfg_GlobalDebounceStorageEventCount
 *****************************************************************************/
/*!
 * \brief         Get the number of events storing the debounce counter in NV
 *
 * \details       Get the number of events storing the debounce counter in NV
 *
 * \return        The number of events storing the debounce counter in NV
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalDebounceStorageEventCount(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalObdDrivingCycleId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of the OBD driving cycle
 *
 * \details       Get the unique handle of the OBD driving cycle
 *
 * \return        The unique handle of the cycle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalObdDrivingCycleId(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalWarmupCycleId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of the warm-up cycle
 *
 * \details       Get the unique handle of the warm-up cycle
 *
 * \return        The unique handle of the cycle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalWarmupCycleId(
  void
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalDtcFormat
 *****************************************************************************/
/*!
 * \brief         Get the configured DTC format
 *
 * \details       Get the configured DTC format
 *
 * \return        The configured DTC format
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalDtcFormat(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalERecMaxCount
 *****************************************************************************/
/*!
 * \brief         Get the maximum amount of configured extended data records
 *
 * \details       Get the maximum amount of configured extended data records
 *                for a single event. This is the number of records that can
 *                be stored in NV for one event.
 *
 * \return        The maximum amount of configured extended data records.
 *                0 if no extended records are configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON && DEM_DEV_RUNTIME_CHECKS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalERecMaxCount(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalERecMaxRawSize
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of an extended data record
 *
 * \details       Get the maximum size available for a single extended data
 *                record. This only counts the data that needs to be stored
 *                in the user-data area of a memory entry.
 *
 * \return        The maximum raw size of an extended data record
 *                0 if no extended records are configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalERecMaxRawSize(
  void
  );
#endif


/* ****************************************************************************
* Dem_Cfg_EventCountOfStatusBlock
*****************************************************************************/
/*!
* \brief         Get size of arrays storing event specific artifacts in 
*                Dem_Cfg_StatusData.
*
* \details       Get size of arrays storing event specific artifacts in 
*                Dem_Cfg_StatusData.
*
* \return        size of arrays storing event specific artifacts in 
*                Dem_Cfg_StatusData
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventCountOfStatusBlock(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalEventCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured events
 *
 * \details       Get the number of configured events. This includes event 0.
 *
 * \return        The number of configured events
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalEventCount(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_EventCycleStatusCount
 *****************************************************************************/
/*!
 * \brief         Get the size of EventCycleStatus
 *
 * \details       Get the size of EventCycleStatus
 *
 * \return        Size of EventCycleStatus
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventCycleStatusCount(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalSatelliteCount
 *****************************************************************************/
/*!
 * \brief         Get the number of used satellites
 *
 * \details       Get the number of used satellites.
 *
 * \return        The number of used satellites
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalSatelliteCount(
  void
  );

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalJ1939ExpFFRawSize
 *****************************************************************************/
/*!
 * \brief         Get the raw size of the J1939 expanded freeze frame
 *
 * \details       Get the size of the J1939 expanded freeze frame, excluding
 *                data not stored in NV ram.
 *
 * \return        The raw size in bytes of the J1939 expanded freeze frame
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalJ1939ExpFFRawSize(
  void
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalJ1939FFRawSize
 *****************************************************************************/
/*!
 * \brief         Get the raw size of the J1939 freeze frame
 *
 * \details       Get the size of the J1939 freeze frame, excluding data not
 *                stored in NV ram.
 *
 * \return        The raw size in bytes of the J1939 freeze frame
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalJ1939FFRawSize(
  void
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalJ1939FFTotalRawSize
 *****************************************************************************/
/*!
 * \brief         Get the total raw size of the J1939 freeze frame
 *
 * \details       Get the size of the J1939 freeze frame, including expanded
 *                FreezeFrame raw size
 *
 * \return        Total raw size
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalJ1939FFTotalRawSize(
  void
  );
#endif

#if ( (DEM_CFG_SUPPORT_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) \
   || (DEM_CFG_SUPPORT_WWHOBD == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) )
/* ****************************************************************************
 * Dem_Cfg_GlobalMaxDataValueSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the largest DID or extended data record
 *
 * \details       The size of the largest DID or extended data record is the
 *                maximum data size supported for application APIs to read
 *                stored event data.
 *
 * \return        The size in bytes of the largest DID or extended data record
 *
 * \pre           -
 * \config        ( (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *               || (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
 *               || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
 *               || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalMaxDataValueSize(
  void
  );
#endif

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalMilGroupCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured MIL groups
 *
 * \details       Get the number of configured MIL groups
 *
 * \return        The number of configured MIL groups
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_MIL_GROUPS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalMilGroupCount(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_ConfirmedChronologyMaxCount
 *****************************************************************************/
/*!
 * \brief         Get the max size of array 'confirmed chronology'
 *
 * \details       Get the max size of array 'confirmed chronology'
 *
 * \return        The number of entries in 'confirmed chronology'
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_ConfirmedChronologyMaxCount(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalNvBlockCount
 *****************************************************************************/
/*!
 * \brief         Get the number of configured NVRam blocks
 *
 * \details       Get the number of configured NVRam blocks
 *
 * \return        The number of configured NVRam blocks
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_GlobalNvBlockCount(
  void
  );

#if (DEM_CFG_SUPPORT_NVM_WRITELIMIT == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalNvStorageLimit
 *****************************************************************************/
/*!
 * \brief         Get the occurrence limit for immediate NV updates
 *
 * \details       Get the occurrence limit for immediate NV updates
 *
 * \return        The occurrence limit for immediate NV updates
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_NVM_WRITELIMIT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalNvStorageLimit(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_ObdFFSlotCountInVariant
 *****************************************************************************/
/*!
 * \brief         Get the number of stored OBD-II or
 *                OBD on UDS freeze frame slots
 *
 * \details       Get the number of stored OBD-II or
 *                OBD on UDS freeze frame slots in variant.
 *                Depending on which version is supported by the configuration
 *                or variant.
 *
 * \return        The number of OBD-II or OBD on UDS freeze frame slots
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_ObdFFSlotCountInVariant(
  void
  );


/* ****************************************************************************
 * Dem_Cfg_GlobalObdIIFFUdsSize
 *****************************************************************************/
/*!
 * \brief         Get the UDS size of the OBD-II freeze frame
 *
 * \details       Get the data size of the OBD-II freeze frames when formatted
 *                according to the UDS protocol. This includes all data not
 *                stored in NVram, and 2 bytes per PID for the UDS DID number.
 *
 * \return        The UDS size of the OBD-II freeze frame
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalObdIIFFUdsSize(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalObdRequirements
 *****************************************************************************/
/*!
 * \brief         Get the supported OBD requirement byte
 *
 * \details       Get the identifier encoding the supported OBD requirements,
 *                as used in PID 1C and DM5
 *
 * \return        The supported OBD requirement byte
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalObdRequirements(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPermanentDtcRelActivationDist
 *****************************************************************************/
/*!
 * \brief         Get the permanent storage activation distance
 *
 * \details       Get the permanent storage activation distance
 *
 * \return        The permanent storage activation distance
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_GlobalPermanentDtcRelActivationDist(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPermanentMaxCount
 *****************************************************************************/
/*!
 * \brief         Get the maximum number of stored permanent DTCs
 *
 * \details       Get the maximum number of stored permanent DTCs
 *
 * \return        The maximum number of stored permanent DTCs
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalPermanentMaxCount(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPidIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_GlobalPidIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_GlobalPidIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_GlobalPidIterExists(
  CONSTP2CONST(Dem_Cfg_GlobalPidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPidIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_GlobalPidIterGet(
  CONSTP2CONST(Dem_Cfg_GlobalPidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPidIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for
 *                the OBD-II freeze frame (i.e. PIDs)
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalPidIterInit(
  CONSTP2VAR(Dem_Cfg_GlobalPidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPidIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_GlobalPidIterNext(
  CONSTP2VAR(Dem_Cfg_GlobalPidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalPrestorageSize
 *****************************************************************************/
/*!
 * \brief         Get the number of configured pre-storage slots.
 *
 * \details       Get the number of configured pre-storage slots. The array
 *                Dem_Cfg_Prestorage has one additional slot.
 *
 * \return        The number of pre-storage slots
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalPrestorageSize(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_RESTART_CYCLE == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalRestartCycleId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of the cycle started on initialization
 *
 * \details       Get the unique handle of the cycle started on initialization
 *
 * \return        Unique handle of the cycle
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_RESTART_CYCLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalRestartCycleId(
  void
  );
#endif



/* ****************************************************************************
 * Dem_Cfg_GlobalSeverityMask
 *****************************************************************************/
/*!
 * \brief         Get the supported severity mask
 *
 * \details       Get the supported severity mask
 *
 * \return        The supported severity mask
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCSeverityType, DEM_CODE)
Dem_Cfg_GlobalSeverityMask(
  void
  );


#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSRecMaxCount
 *****************************************************************************/
/*!
 * \brief         Get the maximum amount of configured snapshot records
 *
 * \details       Get the maximum amount of configured snapshot records for a
 *                single event. This is the number of records that can be
 *                stored in NV for one event.
 *
 * \return        The maximum amount of configured snapshot records.
 *                0 if no extended records are configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON && DEM_DEV_RUNTIME_CHECKS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalSRecMaxCount(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_EventSRecMaxRawSize
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of a snapshot record
 *
 * \details       Get the maximum size available for a single snapshot record.
 *                This only counts the data that needs to be stored in NVRam.
 *
 * \return        The maximum raw size of an snapshot record
 *                0 if no extended records are configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_EventSRecMaxRawSize(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSRecMaxRawSize
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of a global snapshot record
 *
 * \details       Get the maximum size available for a single global snapshot
 *                record. This only counts the data that needs to be stored in
 *                NVRam.
 *
 * \return        The maximum raw size of an snapshot record
 *                0 if no global snapshot records are configured.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalSRecMaxRawSize(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_GlobalObdFreezeFrameMaxSize
 *****************************************************************************/
/*!
 * \brief         Get the size of largest configured FreezeFrame 0 (OBD II)
 *                record
 *
 * \details       Get the size of largest configured FreezeFrame 0 (OBD II)
 *                record
 *
 * \return        The size of largest configured FreezeFrame 0 (OBD II) record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalObdFreezeFrameMaxSize(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalWwhobdFreezeFrameDataSize
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of the WWH-OBD freeze frame
 *
 * \details       Get the maximum size available for the WWH-OBD freeze frame.
 *                This only counts the data that needs to be stored in NVRam.
 *
 * \return        The maximum raw size of the WWH-OBD freeze frame.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalWwhobdFreezeFrameDataSize(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalPrestoredWwhobdFreezeFrameDataSize
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of the prestored WWH-OBD freeze frame
 *
 * \details       Get the maximum size available for the prestored WWH-OBD
 *                freeze frame.
 *                This only counts the data that needs to be prestored in NVRam.
 *
 * \return        The maximum raw size of the prestored WWH-OBD freeze frame.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalPrestoredWwhobdFreezeFrameDataSize(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_GlobalStatusMask
 *****************************************************************************/
/*!
 * \brief         Get the supported DTC status mask (SAM)
 *
 * \details       Get the supported DTC status mask a.k.a status availability
 *                mask (SAM)
 *
 * \return        The supported DTC status mask
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalStatusMask(
  void
  );

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalTimeSeriesFastBufferSize
 *****************************************************************************/
/*!
 * \brief         Get the buffer size available for fast rate time series
 *
 * \details       Get the buffer size available for fast rate time series
 *
 * \return        The buffer size available for fast rate time series
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalTimeSeriesFastBufferSize(
  void
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalTimeSeriesNormalBufferSize
 *****************************************************************************/
/*!
 * \brief         Get the buffer size available for normal rate time series
 *
 * \details       Get the buffer size available for normal rate time series
 *
 * \return        The buffer size available for fast rate time series
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalTimeSeriesNormalBufferSize(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalEventAvailableDataSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the event available data array
 *
 * \details       Get the size of the event available data array
 *
 * \return        The size of the event available data array
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE_NV == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_GlobalEventAvailableDataSize(
  void
  );
#endif

#if (DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_InitPtrCompatibilityVersion
 *****************************************************************************/
/*!
 * \brief         Get the compatibility version from a Dem configuration set
 *
 * \details       Get the compatibility version from a Dem configuration set.
 *                This number can be used to prevent initializing the Dem with an
 *                incompatible configuration.
 *
 * \param[in]     ConfigPtr
 *                Pointer to a Dem configuration set
 *
 * \return        Compatibility version of the configuration set
 *
 * \pre           -
 * \config        DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_InitPtrCompatibilityVersion(
  CONSTP2CONST(Dem_ConfigType, AUTOMATIC, DEM_INIT_DATA)  ConfigPtr
  );
#endif

#if (DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_InitPtrFinalMagicNumber
 *****************************************************************************/
/*!
 * \brief         Get the final magic number from a Dem configuration set
 *
 * \details       Get the final magic number from a Dem configuration set.
 *                This number can be used to prevent initializing the Dem with an
 *                incompatible configuration.
 *
 * \param[in]     ConfigPtr
 *                Pointer to a Dem configuration set
 *
 * \return        Final magic number of the configuration set
 *
 * \pre           -
 * \config        DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_InitPtrFinalMagicNumber(
  CONSTP2CONST(Dem_ConfigType, AUTOMATIC, DEM_INIT_DATA)  ConfigPtr
  );
#endif

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_J1939NodeIndicatorReadiness
 *****************************************************************************/
/*!
 * \brief         Get the indicator readiness of a J1939 node
 *
 * \details       Get the indicator readiness of a J1939 node
 *
 * \param[in]     NodeIndex
 *                Handle to a Dem J1939 node
 *
 * \return        Indicator readiness of the J1939 node
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_J1939NodeIndicatorReadiness(
  CONST(uint8, AUTOMATIC)  NodeIndex
  );
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_J1939NmNodeId
 *****************************************************************************/
/*!
 * \brief         Get the Nm Node Id for the given node index
 *
 * \details       Get the Nm Node Id for the given node index
 *
 * \param[in]     NodeIndex
 *                Handle to a Dem J1939 node
 *
 * \return        Nm Node Id of the J1939 node
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Cfg_J1939NmNodeId(
  CONST(uint8, AUTOMATIC)  NodeIndex
  );
#endif

#if (DEM_CFG_USE_NVM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_MemoryAdminIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the admin block
 *
 * \details       Get the NV Block handle of the admin block
 *
 * \return        NV Block handle of the admin block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryAdminIndex(
  void
  );
#endif

#if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_SUPPORT_DEBOUNCE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_MemoryDebounceIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the debounce data block
 *
 * \details       Get the NV Block handle of the debounce data block
 *
 * \return        NV Block handle of the debounce data block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON &&
 *                DEM_CFG_SUPPORT_DEBOUNCE_NV == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryDebounceIndex(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_SizeOfReadOutBufferData
 *****************************************************************************/
/*!
 * \brief         Returns size of ReadoutBufferData
 *
 * \details       Returns size of ReadoutBufferData Array in ReadoutBuffer
 *
 * \return        Size of ReadoutBufferData
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_SizeOfReadOutBufferData(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_MemoryDtrIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the Dtr data block
 *
 * \details       Get the NV Block handle of the Dtr data block
 *
 * \return        NV Block handle of the Dtr data block
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryDtrIndex(
  void
  );

#if (DEM_CFG_USE_NVM == STD_ON) && (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_MemoryEventAvailableIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the availability data block
 *
 * \details       Get the NV Block handle of the availability data block
 *
 * \return        NV Block handle of the availability data block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON &&
 *                DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryEventAvailableIndex(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_MemoryFreezeFrameIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the OBD freeze frame data block
 *
 * \details       Get the NV Block handle of the OBD freeze frame data block
 *
 * \return        NV Block handle of the OBD freeze frame data block
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryFreezeFrameIndex(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_MemoryPermanentIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the permanent DTC data block
 *
 * \details       Get the NV Block handle of the permanent DTC data block
 *
 * \return        NV Block handle of the permanent DTC data block
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryPermanentIndex(
  void
  );

#if (DEM_CFG_USE_NVM == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_MemoryStatusIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the event status data block
 *
 * \details       Get the NV Block handle of the event status data block
 *
 * \return        The NV Block handle of the event status data block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryStatusIndex(
  void
  );
#endif

#if ( (DEM_CFG_USE_NVM == STD_ON) \
   && (DEM_FEATURE_NEED_AGING_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_Cfg_MemoryAgingDataIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the aging data memory block
 *
 * \details       Get the NV Block handle of the aging data memory block
 *
 * \return        The NV Block handle of the aging data memory block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON
 *                && DEM_FEATURE_NEED_AGING_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryAgingDataIndex(
  void
  );
#endif

#if ( (DEM_CFG_USE_NVM == STD_ON) \
   && (DEM_FEATURE_NEED_CYCLE_COUNTER_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_Cfg_MemoryCycleCounterDataIndex
 *****************************************************************************/
/*!
 * \brief         Get the NV Block handle of the cycle counter data memory block
 *
 * \details       Get the NV Block handle of the cycle counter data memory block
 *
 * \return        The NV Block handle of the cycle counter data memory block
 *
 * \pre           -
 * \config        DEM_CFG_USE_NVM == STD_ON
 *                && DEM_FEATURE_NEED_CYCLE_COUNTER_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MemoryIndexType, DEM_CODE)
Dem_Cfg_MemoryCycleCounterDataIndex(
  void
);
#endif

#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_ObdDistanceInformation
 *****************************************************************************/
/*!
 * \brief         Get the handle for the OBD distance information data element
 *
 * \details       Get the handle for the OBD distance information data element
 *
 * \return        Handle to a data element descriptor
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_Cfg_ObdDistanceInformation(
  void
  );
#endif

#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_ObdEngineRunTime
 *****************************************************************************/
/*!
 * \brief         Get the handle for the OBD engine run time data element
 *
 * \details       Get the handle for the OBD engine run time data element
 *
 * \return        Handle to a data element descriptor
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_Cfg_ObdEngineRunTime(
  void
  );
#endif


/* ****************************************************************************
 * Dem_Cfg_EventWwhObdFreezeFrameExists
 *****************************************************************************/
/*!
 * \brief         Test if the event stores a WWH-OBD freeze frame
 *
 * \details       Test if the event stores a WWH-OBD freeze frame
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event stores a WWH-OBD freeze frame
 * \return        FALSE
 *                The event does not store a WWH-OBD freeze frame
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_EventWwhObdFreezeFrameExists(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_OemCbkGetCurrentOdometer
 *****************************************************************************/
/*!
 * \brief         Get the global 'get odometer value' callback
 *
 * \details       Get the global 'get odometer value' callback
 *
 * \return        Function pointer to 'get odometer value' callback
 *
 * \pre           -
 * \config        DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON ||
 *                DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_OemCbkGetCurrentOdometerFptrType, DEM_CODE)
Dem_Cfg_OemCbkGetCurrentOdometer(
  void
  );
#endif

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_OemCbkGetExternalTesterStatus
 *****************************************************************************/
/*!
 * \brief         Get the global 'get tester status' callback
 *
 * \details       Get the global 'get tester status' callback
 *
 * \return        Function pointer to 'get tester status' callback
 *
 * \pre           -
 * \config        DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON ||
 *                DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_OemCbkGetExternalTesterStatusFptrType, DEM_CODE)
Dem_Cfg_OemCbkGetExternalTesterStatus(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_SRecId
 *****************************************************************************/
/*!
 * \brief         Get the Record ID of the snapshot
 *
 * \details       Get the Record ID of the snapshot
 *
 * \param[in]     SRecIndex
 *                Handle to Snapshot Record descriptor table
 *
 * \return        The Record ID of the snapshot
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_SRecId(
  CONST(Dem_Cfg_SRecIndexType, AUTOMATIC)  SRecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSRecId
 *****************************************************************************/
/*!
 * \brief         Get the record id of the global snapshot record
 *
 * \details       Get the record id of the global snapshot record
 *
 *
 * \return        The record id of the global snapshot record
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GlobalSRecId(
 void
 );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSRecTrigger
 *****************************************************************************/
/*!
 * \brief         Get the storage trigger of the global snapshot record
 *
 * \details       Get the storage trigger of the global snapshot record
 *
 *
 * \return        The storage trigger of the global snapshot record
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_Cfg_GlobalSRecTrigger(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_GlobalSRecUpdate
 *****************************************************************************/
/*!
 * \brief         Get the update setting of the global snapshot record
 *
 * \details       Get the update setting of the global snapshot record
 *
 *
 * \return        TRUE
 *                The record is updated on every update trigger
 * \return        FALSE
 *                The record is updated only when not already stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_GlobalSRecUpdate(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_SRecTrigger
 *****************************************************************************/
/*!
 * \brief         Get the storage trigger of the snapshot
 *
 * \details       Get the storage trigger of the snapshot
 *
 * \param[in]     SRecIndex
 *                Handle to Snapshot Record descriptor table
 *
 * \return        The storage trigger \ref Dem_Cfg_StorageTriggerType
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_Cfg_SRecTrigger(
  CONST(Dem_Cfg_SRecIndexType, AUTOMATIC)  SRecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_SRecUpdate
 *****************************************************************************/
/*!
 * \brief         Get the update setting of the snapshot
 *
 * \details       Get the update setting of the snapshot
 *
 * \param[in]     SRecIndex
 *                Handle to Snapshot Record descriptor table
 *
 * \return        TRUE
 *                The record is updated on every update trigger
 * \return        FALSE
 *                The record is updated only when not already stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_SRecUpdate(
  CONST(Dem_Cfg_SRecIndexType, AUTOMATIC)  SRecIndex
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_SRecNumStartIdx
 *****************************************************************************/
/*!
 * \brief         Get the start index in the Freeze Frame Number table for the event
 *
 * \details       Get the start index in the Freeze Frame Number table for the event
 *
 * \param[in]     EventId
 *                Unique identifier of the event memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The start index in the Freeze Frame Number table
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_Cfg_SRecNumStartIdx(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Cfg_SRecNumEndIdx
 *****************************************************************************/
/*!
 * \brief         Get the end index in the Freeze Frame Number table for the event
 *
 * \details       Get the end index in the Freeze Frame Number table for the event
 *
 * \param[in]     EventId
 *                Unique identifier of the event memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The end index in the Freeze Frame Number table
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_Cfg_SRecNumEndIdx(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Cfg_SRecNumConfigured
 *****************************************************************************/
/*!
 * \brief         Get the number of snapshot records configured for the event
 *
 * \details       Get the number of snapshot records configured for the event
 *
 * \param[in]     EventId
 *                Unique identifier of the event memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The number of snapshot records configured for the event
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_SRecNumConfigured(
  Dem_EventIdType  EventId
);

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidFastIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_TimeSeriesDidFastIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_TimeSeriesDidFastIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_TimeSeriesDidFastIterExists(
  CONSTP2CONST(Dem_Cfg_TimeSeriesDidFastIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidFastIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_TimeSeriesDidFastIterGet(
  CONSTP2CONST(Dem_Cfg_TimeSeriesDidFastIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidFastIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate all DIDs configured
 *                in fast rate time series snapshots
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_TimeSeriesDidFastIterInit(
  CONSTP2VAR(Dem_Cfg_TimeSeriesDidFastIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidFastIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_TimeSeriesDidFastIterNext(
  CONSTP2VAR(Dem_Cfg_TimeSeriesDidFastIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidNormalIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_TimeSeriesDidNormalIterGet() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_Cfg_TimeSeriesDidNormalIterGet() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_TimeSeriesDidNormalIterExists(
  CONSTP2CONST(Dem_Cfg_TimeSeriesDidNormalIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidNormalIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Cfg_TimeSeriesDidNormalIterGet(
  CONSTP2CONST(Dem_Cfg_TimeSeriesDidNormalIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidFastIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate all DIDs configured
 *                in normal rate time series snapshots
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_TimeSeriesDidNormalIterInit(
  CONSTP2VAR(Dem_Cfg_TimeSeriesDidNormalIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesDidNormalIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_TimeSeriesDidNormalIterNext(
  CONSTP2VAR(Dem_Cfg_TimeSeriesDidNormalIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesFastRate
 *****************************************************************************/
/*!
 * \brief         Get the fast collection rate for time-series
 *
 * \details       Get the fast collection rate for time-series
 *
 * \return        The fast collection rate in main function ticks
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_TimeSeriesFastRate(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesNormalRate
 *****************************************************************************/
/*!
 * \brief         Get the collection rate for time-series
 *
 * \details       Get the collection rate for time-series
 *
 * \return        The collection rate in main function ticks
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_TimeSeriesNormalRate(
  void
  );

#if (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_DemTaskTime
 *****************************************************************************/
/*!
 * \brief         Get the configured DEM Task time in milliseconds
 *
 * \details       Get the configured DEM Task time in milliseconds
 *
 * \return        DEM Task time in milliseconds
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(float32, DEM_CODE)
Dem_Cfg_DemTaskTime(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesPastSamplesFast
 *****************************************************************************/
/*!
 * \brief         Get the number of past samples of fast rate time-series
 *
 * \details       Get the number of past samples of fast rate time-series
 *
 * \return        The number of past samples of fast rate time-series
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesPastSamplesFast(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesFutureSamplesFast
 *****************************************************************************/
/*!
 * \brief         Get the number of future samples of fast rate time-series
 *
 * \details       Get the number of future samples of fast rate time-series
 *
 * \return        The number of future samples of fast rate time-series
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesFutureSamplesFast(
  void
  );

#if (DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesBufferCountFast
 *****************************************************************************/
/*!
 * \brief         Get the number of buffers for fast rate time-series
 *
 * \details       Get the number of buffers for fast rate time-series
 *
 * \return        The number of past sample buffer for fast rate time-series
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES_FAST == STD_ON && DEM_DEV_RUNTIME_CHECKS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesBufferCountFast(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesPastSamplesNormal
 *****************************************************************************/
/*!
 * \brief         Get the number of past samples of time-series
 *
 * \details       Get the number of past samples of time-series
 *
 * \return        The number of past samples of time-series
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesPastSamplesNormal(
  void
  );

/* ****************************************************************************
 * Dem_Cfg_TimeSeriesFutureSamplesNormal
 *****************************************************************************/
/*!
 * \brief         Get the number of future samples of time-series
 *
 * \details       Get the number of future samples of time-series
 *
 * \return        The number of future samples of time-series
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesFutureSamplesNormal(
  void
  );

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_DEV_RUNTIME_CHECKS == STD_ON)
/* ****************************************************************************
 * Dem_Cfg_TimeSeriesBufferCountNormal
 *****************************************************************************/
/*!
 * \brief         Get the number of buffers for normal rate time-series
 *
 * \details       Get the number of buffers for normal rate time-series
 *
 * \return        The number of past sample buffer for normal rate time-series
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON && DEM_DEV_RUNTIME_CHECKS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_TimeSeriesBufferCountNormal(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_TimerEngineRunTimePollRate
 *****************************************************************************/
/*!
 * \brief         Get the poll rate to update engine run time
 *
 * \details       Get the poll rate to update engine run time
 *
 * \return        The poll rate to update engine run time
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Cfg_TimerEngineRunTimePollRate(
  void
  );


/* ****************************************************************************
 * Dem_Cfg_FindJ1939NodeIndexFromNmNodeId
 *****************************************************************************/
/*!
 * \brief         Map the Nm NodeIndex to a Dem Node Handle.
 *
 * \details       Map the Nm NodeIndex to a Dem Node Handle.
 *
 * \param[in]     NmNodeId
 *                J1939Nm node id (symbolic name value)
 *
 * \return        Handle to a J1939 Node descriptor
 *                DEM_CFG_J1939_NODE_INVALID if the NM node is not used in Dem
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_FindJ1939NodeIndexFromNmNodeId(
  CONST(uint16, AUTOMATIC)  NmNodeId
  );

/* ****************************************************************************
 * Dem_Cfg_GetInvalidJ1939NodeIndex
 *****************************************************************************/
/*!
 * \brief         Get invalid J1939 node index
 *
 * \details       Get DEM_CFG_J1939_NODE_INVALID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GetInvalidJ1939NodeIndex(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  );
#if (DEM_CFG_CLEAR_GROUP_DTCS == STD_ON)
/******************************************************************************
 *  Dem_Cfg_EventGroupMask
 ******************************************************************************/
/*!
 * \brief         Map EventId to groupOfDTC's (internal) GROUPMASK.
 *
 * \details       Get the DTC group mask of the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        DEM_CFG_GROUPMASK_NONE
 *                The event has no DTC configured so it is not part of a DTC group.
 * \return        DEM_CFG_GROUPMASK_EMISSION
 *                The event is part of the OBD-II emission related DTC group.
 * \return        DEM_CFG_GROUPMASK_POWERTRAIN
 *                The event is part of the Powertrain DTC group.
 * \return        DEM_CFG_GROUPMASK_CHASSIS
 *                The event is part of the Chassis DTC group.
 * \return        DEM_CFG_GROUPMASK_BODY
 *                The event is part of the Body DTC group.
 * \return        DEM_CFG_GROUPMASK_NETWORK
 *                The event is part of the Network Communication DTC group.
 *
 * \pre           DEM_CFG_CLEAR_GROUP_DTCS == STD_ON
 * \context       ANY
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_EventGroupMask(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Cfg_CombinedGroupGetEvent
 *****************************************************************************/
/*!
 * \brief         Looks up the master event for a combined event
 *
 * \details       This function finds the master event based on any sub-event
 *                of an event combination group.
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 *
 * \return        The EventId representing the combined event group.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_CombinedGroupGetEvent(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId
  );

/* ****************************************************************************
 * Dem_Cfg_GetMasterEvent
 *****************************************************************************/
/*!
 * \brief         Gets the master event of a DTC
 *
 * \details       Selects the representing event (master event) if a DTC belongs
 *                to a combined group.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The EventId representing the combined event group or
 *                the same EventId.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_GetMasterEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Cfg_GetMemoryRepresentative
 *****************************************************************************/
/*!
 * \brief         Gets the event used to represent the DTC in memory
 *
 * \details       Gets the event used to represent the DTC in memory
 *                (master event) if a DTC belongs to combined group type 1.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The EventId used to represent the combined event group 1 or
 *                the same EventId.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_GetMemoryRepresentative(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCFreezeFrameRecordIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_DTCFreezeFrameRecordIterGet() will return a valid
 *                element on this iterator
 * \return        FALSE
 *                Dem_Cfg_DTCFreezeFrameRecordIterGet() is undefined on
 *                this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        ((DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_DTCFreezeFrameRecordIterExists(
  CONSTP2CONST(Dem_Cfg_DTCFreezeFrameIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCFreezeFrameRecordIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Freeze frame Id pointed to by the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_DTCFreezeFrameRecordIterGet(
  CONSTP2CONST(Dem_Cfg_DTCFreezeFrameIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCFreezeFrameRecordIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate over the freeze frame
 *                records of the combined DTC
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     CombinedGroupIdx
 *                Index to the combined DTC group
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DTCFreezeFrameRecordIterInit(
  CONSTP2VAR(Dem_Cfg_DTCFreezeFrameIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCFreezeFrameRecordIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DTCFreezeFrameRecordIterNext(
  CONSTP2VAR(Dem_Cfg_DTCFreezeFrameIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ( (DEM_CFG_SUPPORT_SRECS)\
     && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_GetMaxFreezeFrameIdCombinedEvent
 *****************************************************************************/
/*!
 * \brief         Returns the highest freeze frame record id for combined DTC
 *
 * \details       Returns the highest freeze frame record id for combined DTC
 *
 * \param[in]     CombinedGroupIdx
 *                Index to the combined DTC group
 *
 * \return        Highest freeze frame record id for combined DTC
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_SRECS)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_GetMaxFreezeFrameIdCombinedEvent(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx
  );
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCExtendedDataRecordIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_DTCExtendedDataRecordIterGetRecordNr() will return a valid
 *                element on this iterator
 * \return        FALSE
 *                Dem_Cfg_DTCExtendedDataRecordIterGetRecordNr() is undefined on
 *                this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        ((DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_DTCExtendedDataRecordIterExists(
  CONSTP2CONST(Dem_Cfg_DTCExtendedDataRecordIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCExtendedDataRecordIterGetRecordNr
 *****************************************************************************/
/*!
 * \brief         Get the Record number of the extended data record pointed to
 *                by the iterator
 *
 * \details       Get the Record number of the extended data record pointed to
 *                by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Extended data record number pointed to by the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Cfg_DTCExtendedDataRecordIterGetRecordNr(
  CONSTP2CONST(Dem_Cfg_DTCExtendedDataRecordIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCExtendedDataRecordIterGetERecIndex
 *****************************************************************************/
/*!
 * \brief         Get index to the data collection table of the extended data
 *                record pointed to by the iterator
 *
 * \details       Get index to the data collection table of the extended data
 *                record pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Index to the data collection table of the extended data
 *                record pointed to by the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_ERecIndexType, DEM_CODE)
Dem_Cfg_DTCExtendedDataRecordIterGetERecIndex(
  CONSTP2CONST(Dem_Cfg_DTCExtendedDataRecordIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCExtendedDataRecordIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate over the extended data
 *                records of the combined DTC
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     CombinedGroupIdx
 *                Index to the combined DTC group
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DTCExtendedDataRecordIterInit(
  CONSTP2VAR(Dem_Cfg_DTCExtendedDataRecordIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx
  );
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_DTCExtendedDataRecordIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_DTCExtendedDataRecordIterNext(
  CONSTP2VAR(Dem_Cfg_DTCExtendedDataRecordIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif


#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_ExtendedDataRecordEventIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_ExtendedDataRecordEventIterGet() will return a valid
 *                element on this iterator
 * \return        FALSE
 *                Dem_Cfg_ExtendedDataRecordEventIterGet() is undefined on
 *                this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        ((DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_ExtendedDataRecordEventIterExists(
  CONSTP2CONST(Dem_Cfg_ExtendedDataRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_FreezeFrameRecordEventIterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Cfg_FreezeFrameRecordEventIterGet() will return a valid
 *                element on this iterator
 * \return        FALSE
 *                Dem_Cfg_FreezeFrameRecordEventIterGet() is undefined on
 *                this iterator
 *
 * \pre           IterPtr has been initialized
 * \config        ((DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Cfg_FreezeFrameRecordEventIterExists(
CONSTP2CONST(Dem_Cfg_FreezeFrameRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_ExtendedDataRecordEventIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Event Id pointed to by the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_ExtendedDataRecordEventIterGet(
  CONSTP2CONST(Dem_Cfg_ExtendedDataRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_FreezeFrameRecordEventIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Event Id pointed to by the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Cfg_FreezeFrameRecordEventIterGet(
CONSTP2CONST(Dem_Cfg_FreezeFrameRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_ExtendedDataRecordEventIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate over the list of events
 *                belonging to the combined group containing the selected
 *                extended data record.
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     CombinedGroupIdx
 *                Index to the combined DTC group
 * \param[in]     ERecId
 *                Identifier of the requested extended data record
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Cfg_ExtendedDataRecordEventIterInit(
  CONSTP2VAR(Dem_Cfg_ExtendedDataRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx,
  CONST(uint8, AUTOMATIC) ERecId
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_FreezeFrameRecordEventIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate over the list of events
 *                belonging to the combined group containing the selected
 *                snapshot data record.
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     CombinedGroupIdx
 *                Index to the combined DTC group
 * \param[in]     ERecId
 *                Identifier of the requested extended data record
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                 && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Cfg_FreezeFrameRecordEventIterInit(
CONSTP2VAR(Dem_Cfg_FreezeFrameRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx,
CONST(uint8, AUTOMATIC) SRecId
);
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_ExtendedDataRecordEventIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_ERECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_ExtendedDataRecordEventIterNext(
  CONSTP2VAR(Dem_Cfg_ExtendedDataRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) \
  && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Cfg_FreezeFrameRecordEventIterNext
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 * \config        (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Cfg_FreezeFrameRecordEventIterNext(
CONSTP2VAR(Dem_Cfg_FreezeFrameRecordEventIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);
#endif

#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif  /* DEM_CFG_DECLARATIONS_H */
/*!
 * \}
 */

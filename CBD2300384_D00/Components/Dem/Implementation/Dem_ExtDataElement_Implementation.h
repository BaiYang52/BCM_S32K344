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
/*! \addtogroup Dem_ExtDataElement
 *  \{
 *  \file       Dem_ExtDataElement_Implementation.h
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

#if !defined (DEM_EXTDATAELEMENT_IMPLEMENTATION_H)
#define DEM_EXTDATAELEMENT_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ExtDataElement_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_MemoryEntry_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME "Dem_ExtDataElement_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ExtDataElementIF_Private Private Methods
 * \{
 */

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint8_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S or S/R user data with data type
 *                uint8 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT8_N type is used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint8_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint8_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type sint8 Array from the
 *                application and stores it into the passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT8_N type is used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint8_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint16_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S user data with data type
 *                uint16 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT16_N or 
 *                DEM_CFG_DATA_TYPE_UINT16_N_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint16_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint16_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S user data with data type
 *                sint16 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT16_N or 
 *                DEM_CFG_DATA_TYPE_SINT16_N_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint16_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint32_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S user data with data type
 *                uint32 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT32_N or 
 *                DEM_CFG_DATA_TYPE_UINT32_N_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint32_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint32_N
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S user data with data type
 *                sint32 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT32_N or 
 *                DEM_CFG_DATA_TYPE_SINT32_N_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint32_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Float_N
*****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves C/S user data with data type
 *                float32 Array from the application and stores it into the
 *                passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_FLOAT_N or 
 *                DEM_CFG_DATA_TYPE_FLOAT_N_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Float_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Boolean
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type boolean from the
 *                application and stores it into the passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_BOOLEAN type is used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Boolean(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint8
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S  user data with data type sint8 from the
 *                application and stores it into the passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 * 
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT8 type is used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint8(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint8
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type uint8 from the
 *                application and stores it into the passed buffer.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT32 type is used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint8(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint16
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type sint16
 *                from the application and stores it into the passed buffer with
 *                regards to endianness.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex 
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT16 or 
 *                DEM_CFG_DATA_TYPE_SINT16_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint16(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint16
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type uint16
 *                from the application and stores it into the passed buffer with
 *                regards to endianness.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT16 or 
 *                DEM_CFG_DATA_TYPE_UINT16_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint16(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Sint32
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type sint32
 *                from the application and stores it into the passed buffer with
 *                regards to endianness.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_SINT32 or 
 *                DEM_CFG_DATA_TYPE_SINT32_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint32(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Uint32
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type uint32
 *                from the application and stores it into the passed buffer with
 *                regards to endianness.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_UINT32 or 
 *                DEM_CFG_DATA_TYPE_UINT32_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint32(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectUserData_Float
 *****************************************************************************/
/*!
 * \brief         Requests user data from application
 *
 * \details       Retrieves S/R or C/S user data with data type float32
 *                from the application and stores it into the passed buffer with
 *                regards to endianness.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     DataIndex
 *                Index of requested data element
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           Function can only be called when DEM_CFG_DATA_TYPE_FLOAT or 
 *                DEM_CFG_DATA_TYPE_FLOAT_INTEL types are used.
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE, for different Destination buffers
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Float(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 * Dem_ExtDataElement_CollectRootCauseEventId
 *****************************************************************************/
/*!
 * \brief         Store the event that caused event memory storage
 *
 * \details       Stores the EventId of the event which caused event memory
 *                storage. For combined events this stores the exact sub-event.
 *
 * \param[in,out] DestinationBuffer
 *                Pointer to DestinationBuffer context
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           -
 * \config        ( (DEM_FEATURE_NEED_OBD == STD_ON)
 *               || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
 *               || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectRootCauseEventId(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Boolean
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Boolean(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 1u)
    || (lDemDataElementType != DEM_CFG_DATA_TYPE_BOOLEAN)
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    boolean lDataAppl;
    uint8 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedBooleanFPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lDataAppl);                 /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataBooleanFPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lDataAppl);                                  /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = DEM_EXTDATAELEMENT_INVALID_DATA_1Byte;
    }
    else
    {
      lData = (uint8)lDataAppl;                                                                                                  /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    }

    Dem_Data_WriteDestinationBufferUint8(DestinationBuffer, lData);                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }  
}
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint8
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint8(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 1u)
    || (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT8)
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    sint8 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedSint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataSint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = (sint8)DEM_EXTDATAELEMENT_INVALID_DATA_SIGNED;
    }
    Dem_Data_WriteDestinationBufferUint8(DestinationBuffer, (uint8)lData);                                                       /* PRQA S 2897 */ /* MD_DEM_2897 */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint8
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint8(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 1u)
    || (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT8)
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint8 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedUint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataUint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                        /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = DEM_EXTDATAELEMENT_INVALID_DATA_1Byte;
    }

    Dem_Data_WriteDestinationBufferUint8(DestinationBuffer, lData);                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint16
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint16(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 2u)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_SINT16) && (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT16_INTEL))
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    sint16 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedSint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataSint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = (sint16)DEM_EXTDATAELEMENT_INVALID_DATA_SIGNED;
    }

    if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_SINT16_INTEL)
    {
      Dem_Data_WriteDestinationBufferUint16_Intel(DestinationBuffer, (uint16)lData);                                             /* PRQA S 2897 */ /* MD_DEM_2897 */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferUint16(DestinationBuffer, (uint16)lData);                                                   /* PRQA S 2897 */ /* MD_DEM_2897 */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint16
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint16(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 2u)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_UINT16) && (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT16_INTEL))
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedUint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataUint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = DEM_EXTDATAELEMENT_INVALID_DATA_2Byte;
    }

    if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_UINT16_INTEL)
    {
      Dem_Data_WriteDestinationBufferUint16_Intel(DestinationBuffer, lData);                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferUint16(DestinationBuffer, lData);                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint32
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint32(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 4u)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_SINT32) && (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT32_INTEL))
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    sint32 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedSint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataSint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = (sint32)DEM_EXTDATAELEMENT_INVALID_DATA_SIGNED;
    }


    if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_SINT32_INTEL)
    {
      Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, (uint32)lData);                                             /* PRQA S 2897 */ /* MD_DEM_2897 */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, (uint32)lData);                                                   /* PRQA S 2897 */ /* MD_DEM_2897 */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint32
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint32(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 4u)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_UINT32) && (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT32_INTEL))
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint32 lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedUint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData);                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataUint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData);                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the callback failed, fill with padding bytes */
      lData = DEM_EXTDATAELEMENT_INVALID_DATA_4Byte;
    }

    if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_UINT32_INTEL)
    {
      Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, lData);                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, lData);                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Float
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Float(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataSize(DataIndex) > 4u)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_FLOAT) && (lDemDataElementType != DEM_CFG_DATA_TYPE_FLOAT_INTEL))
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    Dem_ExtDataElement_FloatU32ConverterType lData;

    lRetValue = E_NOT_OK;

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedFloat32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(EventId, &lData.AsFloat32);           /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }
    else
    {
      lRetValue = (*((Dem_ReadDataFloat32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lData.AsFloat32);                            /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALVARIABLE */
    }

    if (lRetValue != E_OK)
    { /* In case the function failed, fill with padding bytes */
      lData.AsUint32 = DEM_EXTDATAELEMENT_INVALID_DATA_4Byte;
    }

    if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_FLOAT_INTEL)
    {
      Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, lData.AsUint32);                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, lData.AsUint32);                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectRootCauseEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectRootCauseEventId(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Data_WriteDestinationBufferUint16(DestinationBuffer, EventId);                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint8_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint8_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT8_N)
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

      if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
      {
        lRetValue = (*((Dem_ReadDataExtendedUint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_DESTINATIONBUFFER */
          EventId,
          lDestinationBufferWritePtr);
      }
      else
      {
        lRetValue = (*((Dem_ReadDataUint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                              /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_DESTINATIONBUFFER */
          lDestinationBufferWritePtr);
      }

      if (lRetValue != E_OK)
      { /* If the callback fails, pad the buffer with 0xff */
        Dem_MemSet(lDestinationBufferWritePtr,                                                                                  /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);
      }

      Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL  */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint8_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint8_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT8_N)
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

      if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
      {
        lRetValue = (*((Dem_ReadDataExtendedSint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_DESTINATIONBUFFER */
          EventId,
          (Dem_SignedNvDataPtrType)lDestinationBufferWritePtr);                                                                  /* PRQA S 0310 */ /* MD_DEM_11.4_sr */
      }
      else
      {
        lRetValue = (*((Dem_ReadDataSint8FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                              /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_DESTINATIONBUFFER */
          (Dem_SignedNvDataPtrType)lDestinationBufferWritePtr);                                                                  /* PRQA S 0310 */ /* MD_DEM_11.4_sr */
      }

      if (lRetValue != E_OK)
      { /* In case the callback failed, fill with padding bytes */
        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);
      }

      Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL  */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint16_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint16_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_UINT16_N) && (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT16_N_INTEL))
    || (Dem_Cfg_DataSize(DataIndex) > ((uint16)DEM_CFG_MAX_16BIT_ARRAY_SIZE * 2U)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;
    uint16 lReadData[DEM_CFG_MAX_16BIT_ARRAY_SIZE];

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedUint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        EventId,
        &lReadData[0]);
    }
    else
    {
      lRetValue = (*((Dem_ReadDataUint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lReadData[0u]);                               /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
    }

    if (lRetValue == E_OK)
    {
      uint16_least lIndex;
      for (lIndex = 0u; lIndex < (uint16_least)(lDataSize / 2u); ++lIndex)
      {
        if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_UINT16_N_INTEL)
        {
          Dem_Data_WriteDestinationBufferUint16_Intel(DestinationBuffer, lReadData[lIndex]);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else
        {
          Dem_Data_WriteDestinationBufferUint16(DestinationBuffer, lReadData[lIndex]);                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
    else
    {   /* In case the callback failed, fill with padding bytes */
      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);

        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF  */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint16_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint16_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_SINT16_N) && (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT16_N_INTEL))
    || (Dem_Cfg_DataSize(DataIndex) > ((uint16)DEM_CFG_MAX_16BIT_ARRAY_SIZE * 2U)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;
    sint16 lReadData[DEM_CFG_MAX_16BIT_ARRAY_SIZE];

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedSint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        EventId,
        &lReadData[0u]);
    }
    else
    {
      lRetValue = (*((Dem_ReadDataSint16FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lReadData[0u]);                               /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
    }

    if (lRetValue == E_OK)
    {
      uint16_least lIndex;
      for (lIndex = 0u; lIndex < (uint16_least)(lDataSize / 2u); ++lIndex)
      {
        if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_SINT16_N_INTEL)
        {
          Dem_Data_WriteDestinationBufferUint16_Intel(DestinationBuffer, (uint16)lReadData[lIndex]);                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else
        {
          Dem_Data_WriteDestinationBufferUint16(DestinationBuffer, (uint16)lReadData[lIndex]);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
    else
    { /* In case the callback failed, fill with padding bytes */
      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);

        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Uint32_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Uint32_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_UINT32_N) && (lDemDataElementType != DEM_CFG_DATA_TYPE_UINT32_N_INTEL))
    || (Dem_Cfg_DataSize(DataIndex) > ((uint16)DEM_CFG_MAX_32BIT_ARRAY_SIZE * 4U)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;
    uint32 lReadData[DEM_CFG_MAX_32BIT_ARRAY_SIZE];

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedUint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        EventId,
        &lReadData[0u]);
    }
    else
    {
      lRetValue = (*((Dem_ReadDataUint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lReadData[0u]);                               /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
    }

    if (lRetValue == E_OK)
    {
      uint16_least lIndex;
      for (lIndex = 0u; lIndex < (uint16_least)(lDataSize / 4u); ++lIndex)
      {
        if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_UINT32_N_INTEL)
        {
          Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, lReadData[lIndex]);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else
        {
          Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, lReadData[lIndex]);                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
    else
    { /* In case the callback failed, fill with padding bytes */
      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);

        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Sint32_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Sint32_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_SINT32_N) && (lDemDataElementType != DEM_CFG_DATA_TYPE_SINT32_N_INTEL))
    || (Dem_Cfg_DataSize(DataIndex) > ((uint16)DEM_CFG_MAX_32BIT_ARRAY_SIZE * 4U)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;
    sint32 lReadData[DEM_CFG_MAX_32BIT_ARRAY_SIZE];

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedSint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                       /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        EventId,
        &lReadData[0u]);
    }
    else
    {
      lRetValue = (*((Dem_ReadDataSint32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(&lReadData[0]);                                /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
    }

    if (lRetValue == E_OK)
    {
      uint16_least lIndex;
      for (lIndex = 0u; lIndex < (uint16_least)(lDataSize / 4u); ++lIndex)
      {
        if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_SINT32_N_INTEL)
        {
          Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, (uint32)lReadData[lIndex]);                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else
        {
          Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, (uint32)lReadData[lIndex]);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
    else
    { /* In case the callback failed, fill with padding bytes */
      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        
        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);

        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                               /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectUserData_Float_N
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectUserData_Float_N(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  uint8 lDemDataElementType = Dem_Cfg_DataElementDataType(DataIndex);

  if ((DataIndex >= Dem_Cfg_GlobalDataElementCount())
    || (Dem_Cfg_DataCbkRead(DataIndex) == NULL_PTR)
    || ((lDemDataElementType != DEM_CFG_DATA_TYPE_FLOAT_N) && (lDemDataElementType != DEM_CFG_DATA_TYPE_FLOAT_N_INTEL))
    || (Dem_Cfg_DataSize(DataIndex) > ((uint16)DEM_CFG_MAX_32BIT_ARRAY_SIZE * 4U)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTDATAELEMENT_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Std_ReturnType lRetValue;
    uint16 lDataSize;
    Dem_ExtDataElement_FloatU32ConverterType lReadData[DEM_CFG_MAX_32BIT_ARRAY_SIZE];

    lRetValue = E_NOT_OK;
    lDataSize = Dem_Cfg_DataSize(DataIndex);

    if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    {
      lRetValue = (*((Dem_ReadDataExtendedFloat32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                      /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        EventId,
        &lReadData[0].AsFloat32);
    }
    else
    {
      lRetValue = (*((Dem_ReadDataFloat32FPtrType)Dem_Cfg_DataCbkRead(DataIndex)))(                                              /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_APPLCALLBACK_WRITE_LOCALBUFFER */
        &lReadData[0].AsFloat32);
    }

    if (lRetValue == E_OK)
    {
      uint16_least lIndex;
      for (lIndex = 0u; lIndex < (uint16_least)(lDataSize / 4u); ++lIndex)
      {
        if (Dem_Cfg_DataElementDataType(DataIndex) == DEM_CFG_DATA_TYPE_FLOAT_N_INTEL)
        {
          Dem_Data_WriteDestinationBufferUint32_Intel(DestinationBuffer, lReadData[lIndex].AsUint32);                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else
        {
          Dem_Data_WriteDestinationBufferUint32(DestinationBuffer, lReadData[lIndex].AsUint32);                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
    else
    { /* In case the callback failed, fill with padding bytes */      
      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDataSize) == E_OK)                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

        Dem_MemSet(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_DESTINATIONBUFFER_INVALID_DATAPATTERN */
          (uint8)DEM_EXTDATAELEMENT_INVALID_DATA_PATTERN,
          lDataSize);

        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDataSize);                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_ExtDataElementIF_Public
 * \{
 */
#if ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_InitCollectDidInfo
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ExtDataElement_InitCollectDidInfo(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  CollectDidInfoPtr->EventId = EventId;                                                                                          /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
  CollectDidInfoPtr->DestinationBuffer = DestinationBuffer;                                                                      /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
}
#endif

#if ( (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) )
/* ****************************************************************************
 % Dem_ExtDataElement_CollectDataElement
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_ExtDataElement_CollectDataElement(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,                                                 /* PRQA S 3673 */ /* MD_DEM_8.13_Design */
  CONST(Dem_Cfg_DataIndexType, AUTOMATIC)  DataIndex
  )
{
  boolean lReturnValue;

  lReturnValue = TRUE;
  if ((Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT)
    || (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID)
    || (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_SR_PORT))
  {
    switch (Dem_Cfg_DataElementDataType(DataIndex))
    {
      case DEM_CFG_DATA_TYPE_UINT8_N:
        Dem_ExtDataElement_CollectUserData_Uint8_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex, 
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_SINT8_N:
        Dem_ExtDataElement_CollectUserData_Sint8_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_UINT16_N:
      case DEM_CFG_DATA_TYPE_UINT16_N_INTEL:
        Dem_ExtDataElement_CollectUserData_Uint16_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_SINT16_N:
      case DEM_CFG_DATA_TYPE_SINT16_N_INTEL:
        Dem_ExtDataElement_CollectUserData_Sint16_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_UINT32_N:
      case DEM_CFG_DATA_TYPE_UINT32_N_INTEL:
        Dem_ExtDataElement_CollectUserData_Uint32_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_SINT32_N:
      case DEM_CFG_DATA_TYPE_SINT32_N_INTEL:
        Dem_ExtDataElement_CollectUserData_Sint32_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_FLOAT_N:
      case DEM_CFG_DATA_TYPE_FLOAT_N_INTEL:
        Dem_ExtDataElement_CollectUserData_Float_N(
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);                                                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        break;

      case DEM_CFG_DATA_TYPE_BOOLEAN:
        Dem_ExtDataElement_CollectUserData_Boolean(                                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer, 
          DataIndex, 
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_SINT8:
        Dem_ExtDataElement_CollectUserData_Sint8(                                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer, 
          DataIndex, 
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_UINT8:
        Dem_ExtDataElement_CollectUserData_Uint8(                                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer, 
          DataIndex, 
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_SINT16:
      case DEM_CFG_DATA_TYPE_SINT16_INTEL:
        Dem_ExtDataElement_CollectUserData_Sint16(                                                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer, 
          DataIndex, 
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_UINT16:
      case DEM_CFG_DATA_TYPE_UINT16_INTEL:
        Dem_ExtDataElement_CollectUserData_Uint16(                                                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_SINT32:
      case DEM_CFG_DATA_TYPE_SINT32_INTEL:
        Dem_ExtDataElement_CollectUserData_Sint32(                                                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);
        break;

      case DEM_CFG_DATA_TYPE_UINT32:
      case DEM_CFG_DATA_TYPE_UINT32_INTEL:
        Dem_ExtDataElement_CollectUserData_Uint32(                                                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex, 
          CollectDidInfoPtr->EventId);
        break;
      case DEM_CFG_DATA_TYPE_FLOAT:
      case DEM_CFG_DATA_TYPE_FLOAT_INTEL:
        Dem_ExtDataElement_CollectUserData_Float(                                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          CollectDidInfoPtr->DestinationBuffer,
          DataIndex,
          CollectDidInfoPtr->EventId);
        break;
      default:
        lReturnValue = FALSE;
        break;

    }
  }
  else if (Dem_Cfg_DataCallbackType(DataIndex) == DEM_CFG_DATA_FROM_ROOTCAUSE_EVENTID)
  {
    Dem_ExtDataElement_CollectRootCauseEventId(CollectDidInfoPtr->DestinationBuffer, CollectDidInfoPtr->EventId);                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    lReturnValue = FALSE;
  }
  
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

#if ( (DEM_CFG_SUPPORT_USER_ERECS == STD_ON) \
   || (DEM_CFG_SUPPORT_SRECS == STD_ON) \
   || (DEM_FEATURE_NEED_OBD == STD_ON) \
   || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) \
   || (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) \
   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_ExtDataElement_CollectStoredDid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ExtDataElement_CollectStoredDid(
  CONST(Dem_ExtDataElement_CollectDidInfoPtrType, AUTOMATIC)  CollectDidInfoPtr,
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  )
{
  Dem_Cfg_DidDataIterType lDataIter;

  /* Each DID consists of 1-* data elements. */
  for (Dem_Cfg_DidDataIterInit(DidIndex, &lDataIter);                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_DidDataIterExists(&lDataIter) == TRUE;                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_DidDataIterNext(&lDataIter))                                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_DataIndexType lDataIndex;

    lDataIndex = Dem_Cfg_DidDataIterGet(&lDataIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if (Dem_Cfg_DataIsStoredInNV(lDataIndex) == TRUE)
    {
      (void) Dem_ExtDataElement_CollectDataElement(CollectDidInfoPtr, lDataIndex);                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
#endif

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 % Dem_ExtDataElement_GetCustomizedCurrentOdometer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedCurrentOdometer(
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId
  )
{
  uint32 lPreciseOdometer;
  Std_ReturnType lReturnValue;
  Dem_OemCbkGetCurrentOdometerFptrType Dem_OemGetCurrentOdometerCbk;

  Dem_OemGetCurrentOdometerCbk = Dem_Cfg_OemCbkGetCurrentOdometer();
  if (Dem_OemGetCurrentOdometerCbk != NULL_PTR)
  {
    /* receives bussed odometer in unit 0.1km per bit */ 
    lReturnValue = (*Dem_OemGetCurrentOdometerCbk)(&lPreciseOdometer);                                                           /* SBSW_DEM_GETCURRENTODOMETER_FPTR */
  }
  else
  {
    /* Set the odometer value as invalid */
    lPreciseOdometer = 0x00FFFFFFuL;
    lReturnValue = E_NOT_OK;
  }

  if ( (lReturnValue != E_OK)
    || (lPreciseOdometer == 0x00FFFFFFuL) )
  {
    /* signal invalid */
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER, 0xFFU);
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER + 1u, 0xFFU);
  }
  else
  if (lPreciseOdometer == 0x00FFFFFEuL)
  {
    /* signal unavailable */
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER, 0xFFU);
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER + 1u, 0xFEU);
  }
  else
  {
    uint16 lScaledOdometer;
    /* Conversion from bussed odometer value to internal odometer is divide by 160 (=0xA0). */
    /* The cast to uint16 is taken directly from the specification */
    lScaledOdometer = (uint16)((lPreciseOdometer / 0xA0uL) & 0x0000FFFFuL);

    if (lScaledOdometer > 0xFFFDu)
    {
      lScaledOdometer = 0xFFFDu;
    }
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER, Dem_GetHiByte(lScaledOdometer));
    Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_CURRENTODOMETER + 1u, Dem_GetLoByte(lScaledOdometer));
  }
}
#endif

#if (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 % Dem_ExtDataElement_GetCustomizedOperatingTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedOperatingTime(
  CONST(Dem_Cfg_MemoryEntryHandleType, AUTOMATIC)  MemoryEntryId
  )
{
  uint32 lOperatingTime;
  uint8 lOperatingTimeStatus;

# if (DEM_CFG_USE_OHDS == STD_ON)
  if (OHDS_Dem_GetOperatingHours(&lOperatingTime, &lOperatingTimeStatus) != E_OK)                                                /* SBSW_DEM_POINTER_FORWARD_STACK */
# endif
  {
    lOperatingTime = 0xffffffffU;
    lOperatingTimeStatus = 0xffu;
  }

  Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_TIME, Dem_GetHiHiByte(lOperatingTime));
  Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_TIME + 1u, Dem_GetHiLoByte(lOperatingTime));
  Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_TIME + 2u, Dem_GetLoHiByte(lOperatingTime));
  Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_TIME + 3u, Dem_GetLoLoByte(lOperatingTime));
  Dem_MemoryEntry_SetStdEnvData(MemoryEntryId, DEM_MEMORYENTRY_STDENVDATA_TIMESTATUS, lOperatingTimeStatus);
} /* (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON) */
#endif

#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
/* ****************************************************************************
 % Dem_ExtDataElement_GetCustomizedExternalTester
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ExtDataElement_GetCustomizedExternalTester(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  Buffer
  )
{
  boolean lTesterPresent;
  Std_ReturnType lReturnValue;
  Dem_OemCbkGetExternalTesterStatusFptrType Dem_OemGetExternalTesterStatusCbk;

  Dem_OemGetExternalTesterStatusCbk = Dem_Cfg_OemCbkGetExternalTesterStatus();
  if (Dem_OemGetExternalTesterStatusCbk != NULL_PTR)
  {
    lReturnValue = (*Dem_OemGetExternalTesterStatusCbk)(&lTesterPresent);                                                        /* SBSW_DEM_OEMEXTERNALTESTERSTATUS_FPTR */
  }
  else
  {
    lTesterPresent = FALSE;
    lReturnValue = E_NOT_OK;
  }

  if (lReturnValue != E_OK)
  { /* assume 'false' was returned. */
    *Buffer = 0x00u;                                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
  {
# if (DEM_DEV_ERROR_DETECT == STD_ON)
    if (((lTesterPresent != TRUE) && (lTesterPresent != FALSE)) == TRUE)
    {
      Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_PARAM_DATA);
      *Buffer = (uint8)0;                                                                                                        /* PRQA S 4404, 4304 */ /* MD_MSR_AutosarBoolean, MD_MSR_AutosarBoolean */ /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
# endif
    {
      *Buffer = (uint8)lTesterPresent; /* Boolean -> Assume 0x00/0x01 */                                                         /* PRQA S 4404, 4304*/ /* MD_MSR_AutosarBoolean, MD_MSR_AutosarBoolean */ /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EXTDATAELEMENT_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtDataElement_Implementation.h
 *********************************************************************************************************************/

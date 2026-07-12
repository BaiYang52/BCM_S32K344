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
 *  \defgroup   Dem_MemObdFreezeFrame Dem_MemObdFreezeFrame
 *  \{
 *  \file       Dem_MemObdFreezeFrame_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Obd Freeze Frame Memory subcomponent which manages storage and erasing of the 
 *              legislative Obd Freeze Frame
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMOBDFREEZEFRAME_INTERFACE_H)
#define DEM_MEMOBDFREEZEFRAME_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_MemObdFreezeFrame_Types.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemObdFreezeFrame_Public Public Methods
 * \{
 */

 /* ****************************************************************************
 * Dem_MemObdFreezeFrame_SelectFFDisplaceIndex
 *****************************************************************************/
/*!
 * \brief         Selects Obd freeze frame index for displacement.
 *                Determines, if freeze frame visibility must be updated
 *
 * \details       Selects lowest prior Obd freeze frame index for displacement.
 *                It also determines, if an invisible freeze frame related to a
 *                misfire or fuel system event must be made visible immediately.
 *                If no less prior entry than Event Id is found,
 *                an invalid index is returned.
 *
 * \param[in]     EventId
 *                EventId of the freeze frame that needs to be stored
 * \param[in]     DisplacePendingFreezeFrame
 *                TRUE: trigger and configuration for displacing a pending 
 *                      freeze frame is given
 *                FALSE: displacing pending freeze frames with confirmed is
 *                       either excluded by configuration or the trigger is not
 *                       confirmed
 * \param[out]    SetHighPrioFFVisibleBeforeDcyQualified
 *                TRUE: invisible freeze frame related to a misfire or fuel
 *                      system event must be set visible before DCY
 *                      qualification
 *                FALSE: no visibility change required
 *
 * \return        Index of the freeze frame to displace if found, otherwise
 *                Dem_Cfg_ObdFFSlotCountInVariant()
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectFFDisplaceIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  DisplacePendingFreezeFrame,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  );

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Selects the freeze frame to be reported in Obd mode 02
 *
 * \details       Selects the oldest freeze frame of a misfire or fuel system 
 *                monitor if available, otherwise selects the oldest visible 
 *                freeze frame
 *
 * \return        Index of the freeze frame to display in mode 02, if no 
 *                candidate is found, return Dem_Cfg_ObdFFSlotCountInVariant()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_MemObdFreezeFrame_SelectHighestPriorFreezeFrame(
  void
  );
#endif

/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameSetVisible
 *****************************************************************************/
/*!
 * \brief         Marks a given freeze frame slot as visible
 *
 * \details       Marks a given freeze frame slot as visible
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameSetVisible(
CONST(uint8, AUTOMATIC)  Index
);  


/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameResetVisible
 *****************************************************************************/
/*!
 * \brief         Marks a given freeze frame slot as not visible
 *
 * \details       Marks a given freeze frame slot as not visible
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameResetVisible(
  CONST(uint8, AUTOMATIC)  Index
  );  

/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameFreeEntry
 *****************************************************************************/
/*!
 * \brief         Marks freeze frame entry as unused
 *
 * \details       Reset eventId of a freeze frame entry
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameFreeEntry(
  CONST(uint8, AUTOMATIC) Index
  );  


#if ((DEM_CFG_SUPPORT_OBDII == STD_ON) \
  && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON))
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_SetFFOfEventVisible
 *****************************************************************************/
/*!
 * \brief         Marks a freeze frame slot related to an event as visible
 *
 * \details       Searches for a freeze frame slot related to the EventId
 *                and marks it as visible
 *
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *                && DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_SetFFOfEventVisible(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_SetOldestHighPrioFreezeFrameVisible
 *****************************************************************************/
/*!
 * \brief         Marks the oldest freeze frame related to a misfire or fuel
 *                system event as visible
 *
 * \details       Searches for the oldest invisible freeze frame slot related
 *                to a misfire and fuel system event and marks it as visible

 * \return        EventId of the event whose slot was marked as visible
 * \return        DEM_EVENT_INVALID if no invisible freeze frame related to a
 *                misfire or fuel system event is stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemObdFreezeFrame_SetOldestHighPrioFreezeFrameVisible(
  void
  );
#endif

/* ****************************************************************************
 * Dem_MemObdFreezeFrame_ClearOBDFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Clears OBD FreezeFrame data
 *
 * \details       Clears OBD FreezeFrame data if stored by passed event and 
 *                if OBDII or OBD on UDS is enabled
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_ClearOBDFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_EventDisplaced
 *****************************************************************************/
/*!
 * \brief         Remove a freeze frame due to event displacement
 *
 * \details       This function clears a freeze frame stored for the passed
 *                Event if OBDII or OBD on UDS is enabled
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Only valid if OBDII is supported in variant.
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON ||
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemObdFreezeFrame_EventDisplaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the data pointer for OBDII FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \details       Get the data pointer for OBDII FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \param[in]     Index
 *                Index of the freeze frame data
 *
 * \return        Pointer to OBD FreezeFrame data
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemObdFreezeFrame_FreezeFrameObdIIGetDataPtr(
  CONST(uint8, AUTOMATIC) Index
  );
#endif

#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
/* ****************************************************************************
 * Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the data pointer for OBD on UDS FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \details       Get the data pointer for OBD on UDS FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \param[in]     Index
 *                Index of the freeze frame data
 * \param[in]     FreezeFrameType
 *                0 OBD FreezeFrame 0x00
 *                1 OBD FreezeFrame 0xF0
 *
 * \return        Pointer to OBD FreezeFrame data
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDONUDS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_FreezeFrameObdOnUdsGetDataPtr(
  uint8 Index,
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
);
#endif


/* ****************************************************************************
* Dem_FaultMemory_MemObdFreezeFrame_UpdateOBDIIFreezeFrame
*****************************************************************************/
/*!
* \brief         Stores or updates an OBDII Freeze Frame.
*
* \details       Stores or updates the OBDII freeze frame data, visibility and
*                time stamp depending on RequiredUpdates.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
* \param[in]     FreezeFrameSlot
*                FreezeFrame slot corresponding to the event.
* \param[in]     RequiredUpdates
*                Required freeze frame updates.
*
* \return        EventId of the event whose freeze frame was updated
*
* \pre           The event must be an OBD relevant event.
*                Can only be called in OBDII configs or variants.
* \config        -
*
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_UpdateOBDIIFreezeFrame(
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot,
  uint8 RequiredUpdates
);

/* ****************************************************************************
* Dem_FaultMemory_MemObdFreezeFrame_UpdateObdOnUdsFreezeFrame
*****************************************************************************/
/*!
* \brief         Stores or updates an OBD on UDS Freeze Frame.
*
* \details       Stores or updates the OBD on UDS freeze frame data.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
* \param[in]     FreezeFrameSlot
*                FreezeFrame slot corresponding to the event.
*
* \pre           The event must be an OBD relevant event.
*                Can only be called in OBD on UDS configs or variants.
* \config        -
*
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_UpdateObdOnUdsFreezeFrame(
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot
);

/* ****************************************************************************
 * Dem_FaultMemory_MemObdFreezeFrame_GetObdOnUdsSRecNumberByFreezeFrameType
 *****************************************************************************/
 /*!
 * \brief         Gets SRec number based on OBD on UDS FF type.
 *
 * \details       Gets SRec number based on OBD on UDS FF type.
 *
 * \param[in]     FreezeFrameType
 *                0 OBD FreezeFrame 0x00
 *                1 OBD FreezeFrame 0xF0
 *
 * \return        SRec Number of provided OBD on UDS FF type.
 * \pre           -
 * \config        -
 *
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_GetObdOnUdsSRecNumberByFreezeFrameType(
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
);

/* ****************************************************************************
 * Dem_FaultMemory_MemObdFreezeFrame_InitRestoreMemoryObd
 *****************************************************************************/
/*!
 * \brief         Initializes OBD II and OBD on UDS Freeze Frame
 *
 * \details       This function verifies the integrity of OBD memory FF entries
 *                and links them up into the runtime control structures.
 *
 * \pre           Feature OBD II or OBDonUDS is enabled. NvM must have restored NV mirrors
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemObdFreezeFrame_InitRestoreMemoryObd(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMOBDFREEZEFRAME_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemObdFreezeFrame_Interface.h
 *********************************************************************************************************************/

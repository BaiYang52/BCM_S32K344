/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM.c
 *         \brief    Source for Autosar 4 module FiM
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *            see FiM.h     
 ***********************************************************************************************************************/

#define FIM_SOURCE

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Dem.h"
#include "FiM.h"

#include "SchM_FiM.h"

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* Include Development Error Tracer */
# include "Det.h"                        
#endif

#if (FIM_CFG_USE_RTE == STD_ON)
# include "Rte_Type.h"
#endif

/* EcuM for ErrorHook */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
# include "EcuM_Error.h"
#endif


/*! Enter critical section of FiM */
#define FiM_EnterCritical_CrossCore()   SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_0()
#define FiM_EnterCritical_LocalCore()   SchM_Enter_FiM_FIM_EXCLUSIVE_AREA_1()
/*! Leave critical section of FiM */
#define FiM_LeaveCritical_CrossCore()   SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_0()
#define FiM_LeaveCritical_LocalCore()   SchM_Exit_FiM_FIM_EXCLUSIVE_AREA_1()


/***********************************************************************************************************************
*  VERSION CHECK
***********************************************************************************************************************/
/* vendor specific version information is decimal coded */
#if (  (FIM_SW_MAJOR_VERSION != (9u)) \
    || (FIM_SW_MINOR_VERSION != (1u)) \
    || (FIM_SW_PATCH_VERSION != (0u)) )      
# error "Vendor specific version numbers of FiM.c and FiM.h are inconsistent"
#endif

/* check whether whether configuration files were generated with compatible generator version */
#if (  (FIM_CFG_GEN_MAJOR_VERSION != (9u)) \
    || (FIM_CFG_GEN_MINOR_VERSION != (1u)) )
# error "Incompatible configuration file (generator) version used!"
#endif

/* versions of all included configuration header files shall be checked */
#if (  (FIM_CFG_SW_MAJOR_VERSION != (9u)) \
    || (FIM_CFG_SW_MINOR_VERSION != (1u)) )
# error "Version numbers of FiM.c and FiM_Cfg.h are inconsistent!"
#endif


/***********************************************************************************************************************
*  LOCAL CONSTANT MACROS
***********************************************************************************************************************/
#if !defined (FIM_LOCAL)                                                                  /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local functions */
# define FIM_LOCAL                 static
#endif

#if !defined (FIM_LOCAL_INLINE)                                                           /* COV_MSR_COMPATIBILITY XF */
/*! Default compiler abstraction for local inline functions */
# define FIM_LOCAL_INLINE          LOCAL_INLINE
#endif


/*! \defgroup  FiM_IncDecCodes  Internal codes for FID increment / decrement
 * Internal codes for FID increment / decrement
 * \{ */
#define FIM_FID_NOCHANGE            (0x00u) /*!< No change */
#define FIM_FID_INC                 (0x01u) /*!< Increment */
#define FIM_FID_DEC                 (0x02u) /*!< Decrement */
/*! \} */


/*! \defgroup  FiM_EventIdTableType  Internal codes which eventId table to use
 * Internal codes which eventId table to use
 * \{ */
#define FIM_EVENTID_TABLE           (0x00u) /*!< Main table for inhibition */
#define FIM_EVENTIDPENDING_TABLE    (0x01u) /*!< Table for pending locks   */
/*! \} */


/* status bits defined by DEM
DEM_MONITOR_STATUS_TF     0x01  bit 0: TestFailed 
DEM_MONITOR_STATUS_TNCTOC 0x02  bit 1: TestNotCompletedThisOperationCycle
DEM_UDS_STATUS_TF         0x01  bit 0: TestFailed 
DEM_UDS_STATUS_TFTOC      0x02  bit 1: TestFailedThisOperationCycle 
DEM_UDS_STATUS_PDTC       0x04  bit 2: PendingDTC 
DEM_UDS_STATUS_CDTC       0x08  bit 3: ConfirmedDTC 
DEM_UDS_STATUS_TNCSLC     0x10  bit 4: TestNotCompletedSinceLastClear 
DEM_UDS_STATUS_TFSLC      0x20  bit 5: TestFailedSinceLastClear 
DEM_UDS_STATUS_TNCTOC     0x40  bit 6: TestNotCompletedThisOperationCycle 
DEM_UDS_STATUS_WIR        0x80  bit 7: WarningIndicatorRequested          */

/*! * \defgroup  DefInhModTable Defines for Inhibition Mode Table
 * Defines for masks, patterns and whether result needs to be inverted that are used in calculation of effects of
 * monitor status changes.
 * \{ */
/*! \defgroup  InhCfgCodeLF Inhibition Config Code "Last Failed"
 * Inhibition configuration inhibits if test failed status bit is set. \n
 * Inhibits if: TF
 * \{ */
#define FIM_LAST_FAILED_MASK                  (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TF */
#define FIM_LAST_FAILED_PATTERN               (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TF set */
#define FIM_LAST_FAILED_INVERTRESULT          (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTst Inhibition Config Code "Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set. \n
 * Inhibits if: /TNCTOC
 * \{ */
#define FIM_TESTED_MASK                       (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC */
#define FIM_TESTED_PATTERN                    (Dem_MonitorStatusType)0x00u  /*!<DEM_MONITOR_STATUS_TNCTOC not set */
#define FIM_TESTED_INVERTRESULT               (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTst Inhibition Config Code "Not Tested"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set. \n
 * Inhibits if: TNCTOC
 * \{ */
#define FIM_NOT_TESTED_MASK                   (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC */
#define FIM_NOT_TESTED_PATTERN                (Dem_MonitorStatusType)0x02u  /*!<DEM_MONITOR_STATUS_TNCTOC set */
#define FIM_NOT_TESTED_INVERTRESULT           (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeTstLF Inhibition Config Code "Tested AND Failed"
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is NOT set AND LastFailed status
 * bit is set. \n
 * Inhibits if: /TNCTOC && TF
 * \{ */
#define FIM_TESTED_AND_FAILED_MASK            (Dem_MonitorStatusType)0x03u  /*!<DEM_MONITOR_STATUS_TNCTOC | DEM_MONITOR_STATUS_TF */
#define FIM_TESTED_AND_FAILED_PATTERN         (Dem_MonitorStatusType)0x01u  /*!<DEM_MONITOR_STATUS_TNCTOC not set, DEM_MONITOR_STATUS_TF set */
#define FIM_TESTED_AND_FAILED_INVERTRESULT    (boolean)FALSE                /*!<do not invert result */
/*! \} */
/*! \defgroup  InhCfgCodeNTstOrLF Inhibition Config Code "Not Tested OR Failed" (Autosar extension)
 * Inhibition configuration inhibits if TestNotCompletedThisOperationCycle status bit is set OR LastFailed status
 * bit is NOT set. \n
 * Inhibits if: TNCTOC || TF -> /(/TNCTOC && /TF)
 * \{ */
#define FIM_NOT_TESTED_OR_FAILED_MASK         (Dem_MonitorStatusType)0x03u  /*!<DEM_MONITOR_STATUS_TNCTOC | DEM_MONITOR_STATUS_TF */
#define FIM_NOT_TESTED_OR_FAILED_PATTERN      (Dem_MonitorStatusType)0x00u  /*!<DEM_MONITOR_STATUS_TNCTOC not set, DEM_MONITOR_STATUS_TF not set */
#define FIM_NOT_TESTED_OR_FAILED_INVERTRESULT (boolean)TRUE                 /*!<invert result */
/*! \} */
/*! \} */


/***********************************************************************************************************************
*  LOCAL FUNCTION MACROS
***********************************************************************************************************************/

/*! Return event id at particular position in event id list (macro is easier to read).                                */
#define FiM_UnitEvHdl_GetEventId_EventIdTable                         FiM_GetEventIdOfEventIdTable
/*! Return event id at particular position in event id pending list (macro is easier to read).                        */
#define FiM_UnitEvHdl_GetEventId_EventIdPendingTable                  FiM_GetEventIdOfEventIdPendingTable
/*! Return index into event to fid table at particular position in event id list (macro is easier to read).           */
#define FiM_UnitEvHdl_GetEventToFidTableIndex_EventIdTable            FiM_GetEventToFidTableIdxOfEventIdTable
/*! Return index into inhibition code table at particular position in event id list (macro is easier to read).        */
#define FiM_UnitEvHdl_GetEventToInhCodeTableIndex_EventIdTable        FiM_GetEventToInhCodeTableIndexOfEventIdTable
/*! Return count of relevant inhibition code table entries at particular position in event id list (easier to read).  */
#define FiM_UnitEvHdl_GetEventToInhCodeTableCount_EventIdTable        FiM_GetInhCodeCountOfEventIdTable
/*! Return satellite id of event at particular position in event id list (macro is easier to read).                   */
#define FiM_UnitEvHdl_GetSatelliteId_EventIdTable        (FiM_SatelliteIdType)FiM_GetSatelliteInfoTableIdxOfEventIdTable

/*! Return pointer to initialization status for given satellite id (macro is easier to read).                         */
#define FiM_UnitGeneral_GetInitStatusPtr_SatelliteInfoTable           FiM_GetInitStatusPtrOfSatelliteInfoTable
/*! Return application id of given satellite id (macro is easier to read).                                            */
#define FiM_UnitGeneral_GetApplicationId_SatelliteInfoTable           FiM_GetApplicationIdOfSatelliteInfoTable

/*! Return pointer to inhibition code table for given satellite id (macro is easier to read).                         */
#define FiM_UnitInhStatHdl_GetInhCodeTableBasePtr_SatelliteInfoTable  FiM_GetInhCfgCodeBasePtrOfSatelliteInfoTablePB
/*! Return pointer to inhibition status table for given satellite id (macro is easier to read).                       */
#define FiM_UnitInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable     FiM_GetInhStatusBasePtrOfSatelliteInfoTable
/*! Return length of inhibition status table for given satellite id (macro is easier to read).                        */
#define FiM_UnitInhStatHdl_GetInhStatusLength_SatelliteInfoTable      FiM_GetInhStatusSizeOfSatelliteInfoTable

/*! Return number of configured FIDs. One less than size of FID table (last element is dummy element).                */
#define FiM_UnitFidHdl_GetNrOfFids()                                  (FiM_GetSizeOfFidTable() - 1U)
/*! Return index into inhibition code table at particular position in fid list (macro is easier to read).             */
#define FiM_UnitFidHdl_GetInhCodeIdx_FidToInhCodeTable                FiM_GetInhCodeTableIdxOfFidToInhCodeTable
/*! Return sat id for inhibition code table at particular position in fid list (macro is easier to read).             */
#define FiM_UnitFidHdl_GetSatId_FidToInhSatIdTable                    FiM_GetSatelliteInfoTableIdxOfFidToInhSatIdTable


/***********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
***********************************************************************************************************************/

#define FIM_START_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
/*! Pointer to configuration data in case of PB loadable. */
P2CONST(FiM_ConfigType, FIM_VAR_NOINIT, FIM_INIT_DATA)              FiM_ConfigDataPtr;
#endif

#define FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
*  LOCAL DATA PROTOTYPES
***********************************************************************************************************************/

typedef uint8   FiM_SatelliteIdType;

/*! Struct that holds info for the calculation of a FID depending on the inhibition configuration. */
typedef struct
{
  Dem_MonitorStatusType       Mask;
  Dem_MonitorStatusType       Pattern;
  boolean                     InvertResult;
} FiM_InhibitionModeType;

/*! Typedef for access to event id table. */
typedef uint16 FiM_EventIdTableIndexType;


#define FIM_START_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM master initialized? Don't use directly. Call FiM_UnitGeneral_GetInitializationStatusMaster() instead. */
FIM_LOCAL VAR(boolean, FIM_VAR_INIT)              FiM_UnitGeneral_MasterInitialized                 = FALSE;

#define FIM_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_INIT_8BIT_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Is the FiM at least pre-initialized? */
FIM_LOCAL VAR(boolean, FIM_VAR_INIT)              FiM_UnitGeneral_PreInitialized                    = FALSE;

/*! Cumulated initialization status of all FiM Satellites. */
FIM_LOCAL VAR(FiM_InitializationStatusType, FIM_VAR_INIT)
                                                  FiM_UnitGeneral_InitializationStatus_SatCumulated = FIM_UNINITIALIZED;

#define FIM_STOP_SEC_VAR_INIT_8BIT_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_NOINIT_16BIT_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Offset into event id table.
    Used if direct table access is used, i.e. event ids are in continuously ascending order.*/
FIM_LOCAL VAR(FiM_EventIdTableIndexType, FIM_VAR_NOINIT)  FiM_UnitEvHdl_SearchEventIdTableDirectOffset;
# if (FIM_FEATURE_OBD == STD_ON)
/*! Offset into event id pending table.
    Used if direct table access is used, i.e. event ids are in continuously ascending order.*/
FIM_LOCAL VAR(FiM_EventIdTableIndexType, FIM_VAR_NOINIT)  FiM_UnitEvHdl_SearchEventIdPendingTableDirectOffset;
#endif

#define FIM_STOP_SEC_VAR_NOINIT_16BIT_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
/*! For cyclic FID calculation where not all FIDs are scheduled per cycle,
    this variable holds the FID between two cycles. */
FIM_LOCAL VAR(FiM_FunctionIdType, FIM_VAR_NOINIT)         FiM_UnitFidHdl_NextFidToSchedule;
#endif

#define FIM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define FIM_START_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer that holds the used function for searching an event id in EventIdTable.         */
FIM_LOCAL FUNC(boolean, FIM_CODE) (*FiM_UnitEvHdl_SearchEventIdTableFct)
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);
# if (FIM_FEATURE_OBD == STD_ON)
/*! Pointer that holds the used function for searching an event id in EventIdPendingTable.  */
FIM_LOCAL FUNC(boolean, FIM_CODE) (*FiM_UnitEvHdl_SearchEventIdPendingTableFct)
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);
#endif

#define FIM_STOP_SEC_VAR_NOINIT_UNSPECIFIED_RESTRICTED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
*  GLOBAL DATA
***********************************************************************************************************************/

#define FIM_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Inhibition Mode Table: used in calculation of effects of event status changes. */
FIM_LOCAL CONST(FiM_InhibitionModeType, FIM_CONST)
  FiM_UnitInhStatHdl_InhibitionModeTable[FIM_INHCODE_NUMBER] =                        /* PRQA S 3218 */ /* MD_FiM_8.9 */
{
  { /* invalid: pattern can never match "anded" mask */
    /* FIM_INHCODE_INVALID */
    (Dem_MonitorStatusType)0x00u,    
    (Dem_MonitorStatusType)0x01u,
    (boolean) FALSE
  },
  { /* FIM_LAST_FAILED */
    FIM_LAST_FAILED_MASK,
    FIM_LAST_FAILED_PATTERN,
    FIM_LAST_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED */
    FIM_NOT_TESTED_MASK,                    
    FIM_NOT_TESTED_PATTERN,
    FIM_NOT_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED */
    FIM_TESTED_MASK,                    
    FIM_TESTED_PATTERN,
    FIM_TESTED_INVERTRESULT
  },
  { /* FIM_TESTED_AND_FAILED */
    FIM_TESTED_AND_FAILED_MASK,            
    FIM_TESTED_AND_FAILED_PATTERN,
    FIM_TESTED_AND_FAILED_INVERTRESULT
  },
  { /* FIM_NOT_TESTED_OR_FAILED */
    FIM_NOT_TESTED_OR_FAILED_MASK,            
    FIM_NOT_TESTED_OR_FAILED_PATTERN,
    FIM_NOT_TESTED_OR_FAILED_INVERTRESULT
  },
  { /* invalid code for inhibition (only pending): pattern can never match "anded" mask */
    /* FIM_INHCODE_PENDING */
    (Dem_MonitorStatusType)0x00u,    
    (Dem_MonitorStatusType)0x01u,
    (boolean) FALSE
  }
};                   


#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  public FiM_UnitFidHdl_Init()
 **********************************************************************************************************************/
 /*! \brief                   Initialize unit FiM_UnitFidHdl.
 *
 *  \details                  See brief.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)  FiM_UnitFidHdl_Init(void);

#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitFidHdl_CalcPermissionStates()
 **********************************************************************************************************************/
/*! \brief                    Calculate permission status of FIDs.
 *
 *  \details                  Calculate permission status of FIDs and store result for later quick retrieval.
 *                            Adhere to the maximum number of FIDs to be calculated per cycle.
 *  \pre                      -
 *  \config                   FIM_CYCLIC_FID_CALCULATION == STD_ON
 *  \context                  TASK
 *  \reentrant                FALSE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)  FiM_UnitFidHdl_CalcPermissionStates(void);
#endif


#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitFidHdl_SetFidEventChanged()
 **********************************************************************************************************************/
/*! \brief                    Set tag that status of event connected to FID has changed.
 *
 *  \details                  See brief.
 *  \pre                      -
 *  \config                   FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitFidHdl_SetFidEventChanged(FiM_FunctionIdType Fid);
#endif


/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_GetInhibitionStatus()
 **********************************************************************************************************************/
/*! \brief                    Returns the inhibition status.
 *
 *  \details                  For one satellite, for one inhibition index.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be returned.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *                            Must be smaller than size of satellite info table!
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitInhStatHdl_GetInhibitionStatus(uint16 inhCodeIdx,
                                                                                FiM_SatelliteIdType satId);

#if (FIM_SUPPORT_USER_CAS == STD_OFF)
/***********************************************************************************************************************
 *  private FiM_UnitCmpSwp_CompareAndSwapDefault
 **********************************************************************************************************************/
/*! \brief                    Internal default version of an atomic compare and swap operation.
 *
 *  \details                  Compares the contents of a memory location (identified by addressPtr) to a given
 *                            value (cmpValue) and, only if they are the same, modifies the contents of that memory
 *                            location to a given new value (newValue).
 *                            This is done as a single atomic operation.
 *                              - Implemented here with a critical section.
 *                              - Can be replaced by more efficient version for multicore systems.
 *  \param[in,out] addressPtr Memory location to be compared and swapped. Must point to a valid memory location.
 *  \param[in]     cmpValue   Value to be compared with.
 *  \param[in]     newValue   Value to be written.
 *  \return                   TRUE  swap has been performed
 *                            FALSE otherwise
 *  \pre                      -
 *  \config                   FIM_SUPPORT_USER_CAS == STD_OFF
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitCmpSwp_CompareAndSwapDefault
                                          (volatile CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) addressPtr /* in/out */,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue             /* in     */,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) newValue             /* in     */);
#endif

/***********************************************************************************************************************
 *  public FiM_UnitCmpSwp_CompareAndSwap
 **********************************************************************************************************************/
/*! \brief                    Atomic compare and swap operation.
 *
 *  \details                  Either uses internal default implementation or custom implementation.
 *  \param[in,out] addressPtr Memory location to be compared and swapped. Must point to a valid memory location.
 *  \param[in]     cmpValue   Value to be compared with.
 *  \param[in]     newValue   Value to be written.
 *  \return                   TRUE  swap has been performed
 *                            FALSE otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitCmpSwp_CompareAndSwap
                                          (volatile CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) addressPtr /* in/out */,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue             /* in     */,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) newValue             /* in     */);


#if (FIM_DEV_ERROR_DETECT == STD_ON)     
/***********************************************************************************************************************
 *  FiM_UnitGeneral_GetNrSatellites
 **********************************************************************************************************************/
/*!
 * \brief                     Return number of configured satellites.
 *
 * \details                   See brief.
 *
 * \param[in]                 None.
 *
 * \return                    Number of satellites
 *
 * \pre                       -
 * \config                    FIM_DEV_ERROR_DETECT == STD_ON
 * \context                   TASK|ISR2
 * \synchronous               TRUE
 * \reentrant                 TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UnitGeneral_GetNrSatellites(void);
#endif


/***********************************************************************************************************************
 * private FiM_UnitBitfieldHdl_GetBitfieldWordIndex
 **********************************************************************************************************************/
/*! \brief                    Calculate and return the word index in a bitfield array.
 *
 *  \details                  A bitfield array is composed of a word array.
 *                            Function calculates the index of the word array where a specified bit position is located.
 *  \param[in]  bitPosition   Specified bit position.
 *  \return                   Index in word array.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint32, FIM_CODE) FiM_UnitBitfieldHdl_GetBitfieldWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition /* in  */);

/***********************************************************************************************************************
 * private FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to determine the search algorithm in an event id table.
 *
 *  \details                  A bitfield array is composed of a word array.
 *                            Function calculates the index of the bit within the word where a specified bit position
 *                            is located.
 *  \param[in]  bitPosition   Specified bit position.
 *  \return                   Bit position within word.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition /* in  */);

/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_TestBitInWord
 **********************************************************************************************************************/
/*! \brief                        Test if the bit at a given position is set within a word.
 *
 *  \details                      see brief
 *  \param[in]  bitfieldWord      Word with bit to test.
 *  \param[in]  bitInWordPosition Specified bit position within word.
 *  \return                       TRUE if bit is set.
 *                                FALSE if bit is not set.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitBitfieldHdl_TestBitInWord
                                          (CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                           CONST(uint8, AUTOMATIC) bitInWordPosition);

/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_TestBitInBitfield
 **********************************************************************************************************************/
/*! \brief                      Test if the bit at a given position is set in a bitfield.
 *
 *  \details                    see brief
 *  \param[in]  bitfieldWordPtr Pointer to bitfield array (actually word array containing bitfield).
 *                              Internally only used for read access.
 *  \param[in]  bitPosition     Specified bit position.
 *  \return                     TRUE if bit is set.
 *                              FALSE if bit is not set.
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE
  FUNC(boolean, FIM_CODE) FiM_UnitBitfieldHdl_TestBitInBitfield
                                    (volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                     CONST(uint32, AUTOMATIC) bitPosition);

/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_SetBitInBitfield
 **********************************************************************************************************************/
/*! \brief                          Set the bit at a given position in a bitfield.
 *
 *  \details                        see brief
 *  \param[in]  bitfieldPtr         Pointer to bitfield array (actually word array containing bitfield).
 *  \param[in]  bitfieldWordArrSize Total size of word array containing bitfield.
 *                                  Must not exceed length of word array allocated in valid memory.
 *  \param[in]  bitPosition         Specified bit position.
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitBitfieldHdl_SetBitInBitfield
                                      (volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                       CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                       CONST(uint32, AUTOMATIC) bitPosition);

/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_ResetBitInBitfield
 **********************************************************************************************************************/
/*! \brief                          Reset the bit at a given position in a bitfield.
 *
 *  \details                        see brief
 *  \param[in]  bitfieldWordPtr     Pointer to bitfield array (actually word array containing bitfield).
 *  \param[in]  bitfieldWordArrSize Total size of word array contianing bitfield.
 *                                  Must not exceed length of word array allocated in valid memory.
 *  \param[in]  bitPosition         Specified bit position.
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitBitfieldHdl_ResetBitInBitfield
                                      (volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                       CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                       CONST(uint32, AUTOMATIC) bitPosition);


/***********************************************************************************************************************
 * private FiM_UnitGeneral_CalcInitSatusSatCumulated
 **********************************************************************************************************************/
/*! \brief                          Calculate the cumulated initialization status of all satellites.
 *
 *  \details                        see brief
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UnitGeneral_CalcInitSatusSatCumulated(void);


/***********************************************************************************************************************
 * FiM_UnitEvHdl_GetCountOfEvents_ByTableType
 **********************************************************************************************************************/
/*! \brief                        Return the number of events (except dummy ones) depending on type of eventId
 *                                table.
 *
 *  \details                      There may be several eventId tables. This function accesses FiM_GetCountOfEvents of
 *                                the table that is specified in the parameter.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       Count of events in the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_EventIdTableIndexType, FIM_CODE)
                                                     FiM_UnitEvHdl_GetCountOfEvents_ByTableType(uint8 eventIdTableType);

/***********************************************************************************************************************
 * private FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType
 **********************************************************************************************************************/
/*! \brief                        Return event id at index position depending on type of eventId table.
 *
 *  \details                      There may be several eventId tables. This function accesses
 *                                FiM_GetEventId_EventId<...>Table of the table type that is specified in the parameter.
 *  \param[in]  index             Index in table to obtain eventId from.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       EventId in the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Dem_EventIdType, FIM_CODE)
             FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(FiM_EventIdTableIndexType index, uint8 eventIdTableType);


/***********************************************************************************************************************
 * private FiM_UnitInhStatHdl_GetInhCode_BySatId
 **********************************************************************************************************************/
/*! \brief                        Return inhibition code at index position for a given satellite id.
 *
 *  \details                      see brief 
 *  \param[in]  index             Index in table to obtain inhibition code from.
 *  \param[in]  satId             Satellite Id for which code is stored.
 *                                Must be smaller than size of satellite info table!
 *  \return                       Inhibition code from the specified table.
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InhCodeTableType, FIM_CODE)
                                         FiM_UnitInhStatHdl_GetInhCode_BySatId(uint16 index, FiM_SatelliteIdType satId);


/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table
 *                                using direct index access.
 *
 *  \details                      There is a direct relation between the event id and it's position in the event id
 *                                table of the kind: event id = position + offset
 *                                Algorithm is determined by FiM_UnitEvHdl_SetSearchEventIdTableFct.
 *                                Offset is stored in FiM_SearchEventId[Pending]TableDirectOffset and was determined in
 *                                FiM_UnitEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          Event Ids in event id table must be in continuously ascending order without gaps.
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table using linear 
 *                                search.
 *
 *  \details                      Just a dumb linear search starting at the beginning of the event id table until either
 *                                end is reached or event id is found.
 *                                Algorithm is determined by FiM_UnitEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          -
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableBinary
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to find a given event id in an event id table using binary 
 *                                search.
 *
 *  \details                      Just a standard half-interval search.
 *                                Algorithm is determined by FiM_UnitEvHdl_SetSearchEventIdTableFct.
 *  \param[in]  eventId           Event id to be searched in event id table.
 *                                May be any value from 0 to max value of Dem_EventIdType.
 *  \param[out] evTableIdx        Pointer to variable where index of found event is stored.
 *                                Must point to a variable of type FiM_EventIdTableIndexType.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \return                       TRUE  event id was found
 *                                FALSE event id was not found
 *  \pre                          Event Ids in event id table must be in ascending order.
 *  \context                      TASK|ISR2
 *  \reentrant                    TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */);

/***********************************************************************************************************************
 *  public FiM_UnitEvHdl_SetSearchEventIdTableFct
 **********************************************************************************************************************/
/*! \brief                        Internal helper function to determine the search algorithm in an event id table.
 *
 *  \details                      This function is used at startup of FIM. Its result depends on the sequence of event
 *                                ids. This is not done at generation time since event id tabel can be calibratable.
 *                                Possible search algorithms: Linear or binary search or direct table access.
 *  \param[in]  eventIdTableType  Which eventId table to use.
 *                                Must be one of FiM_EventIdTableType.
 *  \pre                          -
 *  \context                      TASK
 *  \reentrant                    FALSE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE)    FiM_UnitEvHdl_SetSearchEventIdTableFct(uint8 eventIdTableType);


/***********************************************************************************************************************
 *  FiM_UnitGeneral_GetInitializationStatusMaster()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to get initialization status of master.
 *                        
 *  \details                  This function gets the initialization status of the master depending on the stored
 *                            flags for (pre-)initialization.
 *  \return                   Initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UnitGeneral_GetInitializationStatusMaster(void);

/***********************************************************************************************************************
 *  public FiM_UnitGeneral_GetInitializationStatusSat()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to get initialization status of satellite.
 *
 *  \details                  This function gets the initialization status of a satellite depending on the stored
 *                            initialization status of the satellite and the pre-initialization flag.
 *  \param[in]  satId         Satellite Id for which initialization status is requested.
 *  \return                   Initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
                                                  FiM_UnitGeneral_GetInitializationStatusSat(FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UnitGeneral_GetInitializationStatusSatCumulated()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to access cumulated initialization status of satellites.
 *
 *  \details                  See brief.
 *  \return                   Cumulated initialization status
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE) FiM_UnitGeneral_GetInitializationStatusSatCumulated(void);

/***********************************************************************************************************************
 *  private FiM_UnitGeneral_SetInitializationStatusSat_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to set initialization status of satellite.
 *
 *  \details                  This function sets the initialization status of a satellite.
 *                            If runtime checks are enabled:
 *                              It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  initStatus    Initialization status to be set.
 *  \param[in]  satId         Satellite Id that initialization status should be set for.
 *  \return                   E_OK if initialization status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UnitGeneral_SetInitializationStatusSat_Checked
                                                  (FiM_InitializationStatusType initStatus, FiM_SatelliteIdType satId);


/***********************************************************************************************************************
 *  private FiM_UnitInhStatHdl_ModInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to set or reset inhibition status on satellite.
 *
 *  \details                  This function (re-)sets the inhibition status of the specified inhibition configuration
 *                            on a specified satellite.
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  inhibStat     Inhibition status to be set.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *  \return                   E_OK if inhibition status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UnitInhStatHdl_ModInhibitionStatus_Checked(boolean inhibStat,
                                                                                             uint16 inhCodeIdx,
                                                                                             FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_ClearInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*! \brief                    Sets the inhibition status for one inhibition code to FALSE.
 *
 *  \details                  This function resets the inhibition status of the specified inhibition configuration on a
 *                            specified satellite.
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *  \return                   E_OK if inhibition status could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
**********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE)
                         FiM_UnitInhStatHdl_ClearInhibitionStatus_Checked(uint16 inhCodeIdx, FiM_SatelliteIdType satId);

/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_ClearInhibitionStatusAll_Checked()
 **********************************************************************************************************************/
/*! \brief                    Sets all inhibition states on satellite to FALSE.
 *
 *  \details                  This function sets all inhibition states of a specified satellite to "0" (not inhibited).
 *                            If runtime checks are enabled:
 *                            It is ensured that satellite id is smaller than number of satellite info elements.
 *  \param[in]  satId         Satellite Id that inhibition states should be set on.
 *  \return                   E_OK if inhibition states could be set
 *                            E_NOT_OK otherwise
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE)
                                         FiM_UnitInhStatHdl_ClearInhibitionStatusAll_Checked(FiM_SatelliteIdType satId);


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UnitPendCntHdl_IncPendingCounter_Checked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to increment FID pending counter.
 *
 *  \details                  This function increments the pending counter for the passed FID.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be incremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_IncPendingCounter_Checked(FiM_FunctionIdType fid);

/***********************************************************************************************************************
 *  private FiM_UnitPendCntHdl_DecPendingCounter_Checked()
 **********************************************************************************************************************/
 /*! \brief                   Internal helper function to decrement FID pending counter if counter is not already 0.
 *
 *  \details                  This function decrements the pending counter for the passed FID. It includes a prior
 *                            check for 0.
 *                            If runtime checks are enabled:
 *                              It is ensured that FID is smaller than number of elements in FiM_FidPendingCounter
 *                              before write access is done.
 *  \param[in]  fid           Function Id that pending counter should be decremented for.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 *  \config                   FIM_FEATURE_OBD == STD_ON
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_DecPendingCounter_Checked(FiM_FunctionIdType fid);
#endif


/***********************************************************************************************************************
 *  private FiM_UnitFidHdl_IsFidInhibited()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to calculate whether a specific FID is inhibited.
 *                        
 *  \details                  Checks all inhibitions states connected to FID.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UnitFidHdl_IsFidInhibited(FiM_FunctionIdType FID);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UnitFidHdl_IsFidPendingLocked()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to calculate whether a specific FID is pending locked.
 *
 *  \details                  Checks all pending states connected to FID.
 *  \param[in]  FID           Function identifier.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UnitFidHdl_IsFidPendingLocked(FiM_FunctionIdType FID);
#endif


/***********************************************************************************************************************
 *  public FiM_UnitEvHdl_CalcInhStates()
 **********************************************************************************************************************/
/*! \brief                    Calculates the state of Inhibition Configurations for a given
 *                            satellite.
 *  \details                  Complete re-calculation for given satellite. Request Monitor status from DEM.
 *  \param[in]  satId         Id of satellite.
 *  \pre                      -
 *  \context                  TASK
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE)  FiM_UnitEvHdl_CalcInhStates(FiM_SatelliteIdType satId);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitEvHdl_CalcPendStates()
 **********************************************************************************************************************/
/*! \brief                    Calculates all pending states of the FIDs.
 *  \details                  Complete re-calculation. Request Event status from DEM.
 *  \pre                      -
 *  \config                   FIM_FEATURE_OBD == STD_ON
 *  \context                  TASK
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UnitEvHdl_CalcPendStates(void);
#endif


/***********************************************************************************************************************
 *  private FiM_UnitInhStatHdl_GetInhibitionStatusFromMonitor()
 **********************************************************************************************************************/
/*! \brief                    Internal helper function to obtain the inhibition status
 *                            for one monitor status / inhibition code.
 *
 *  \details                  Obtain the inhibition status for
 *                            - one monitor status
 *                            - one inhibition code
 *  \param[in]  monStatus     monitor status byte of event
 *  \param[in]  inhCode       code of inhibition configuration, i.e. index into FiM_InhibitionModeTable;
 *  \return                   TRUE:  combination inhibits FID.
 *                            FALSE: combination does not inhibit FID.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
                      FiM_UnitInhStatHdl_GetInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus, uint8 inhCode);

/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor()
 **********************************************************************************************************************/
/*! \brief                    Calculates and modifies the inhibition status accordingly for one inhibition code.
 *
 *  \details                  Calculates and modifies the inhibition status accordingly for
 *                            - one monitor status
 *                            - one inhibition code
 *  \param[in]  monStatus     monitor status byte of event
 *  \param[in]  inhCodeIdx    Index in satellite specific array for which inhibition status should be set.
 *  \param[in]  satId         Satellite Id that inhibition status should be set on.
 *                            Must be smaller than size of satellite info table!
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus,
                                                                          uint16 inhCodeIdx,
                                                                          FiM_SatelliteIdType satId);


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_GetPendingCounter()
 **********************************************************************************************************************/
 /*!
 *  \brief                      Returns pending counter.
 *  \details                    See brief.
 *  \param[in]  fid             FID for which pending counter is returned
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 *******************************************************************************/
FIM_LOCAL_INLINE FUNC(FiM_FidCounterType, FIM_CODE) FiM_UnitPendCntHdl_GetPendingCounter(FiM_FunctionIdType fid);
#endif

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_ClearPendingCounterAll()
 **********************************************************************************************************************/
 /*!
 *  \brief                      Clears all pending counters.
 *  \details                    Sets all pending counters to 0.
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 *******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_ClearPendingCounterAll(void);
#endif

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_ModPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 *  \brief                      Modifies pending counter.
 *  \details                    Increment, decrement or leave the counters unchanged depending on passed modification
 *                              code.
 *                              Checks that write access is not done beyond array bounds if FIM_DEV_RUNTIME_CHECKS is on.
 *  \param[in]  fid             FID for which pending counter is modified
 *  \param[in]  modCodePending  code how to modify FID Pending counter
 *                              possible codes:
 *                              FIM_FID_NOCHANGE: FID Pending shall not be changed due to status change
 *                              FIM_FID_INC:      FID Pending shall be incremented due to status change
 *                              FIM_FID_DEC:      FID Pending shall be decremented due to status change
 *  \pre                        -
 *  \config                     FIM_FEATURE_OBD == STD_ON
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
 *******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_ModPendingCounter_Checked(FiM_FunctionIdType fid,
                                                                                   uint8 modCodePending);
#endif


/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_CalcInhibStatusOfEvent()
 **********************************************************************************************************************/
/*!
 *  \brief                      Internal helper function to calculate the inhibition states connected to an event.
 *  \details                    Request Monitor Status from DEM. Set or reset corresponding inhibition status bit.
 *  \param[in]  evTableIdx      Index into event id table for event of which inhibition states are to be calculated.
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitEvHdl_CalcInhibStatusOfEvent(FiM_EventIdTableIndexType evTableIdx);

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_CalcPendStatusOfEvent()
 **********************************************************************************************************************/
/*!
 *  \brief                      Internal helper function to calcuate the FID pending states connected to an event.
 *  \details                    Increment, decrement or leave the counters unchanged depending on passed modification
 *                              codes. Changes all FID pending counters of an event.
 *  \param[in]  evTableIdx      Index into event id table for event of which inhibition states are to be calculated.
 *  \param[in]  modCodePending  Code whether to increment, decrement or leave the counter unchanged.
 *  \pre                        Event Id Pending table must contain at least one element apart from last dummy element.
 *  \context                    TASK|ISR2
 *  \reentrant                  TRUE
*******************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitEvHdl_CalcPendStatusOfEvent(FiM_EventIdTableIndexType evTableIdx,
                                                                          uint8 modCodePending);
#endif


#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#if (FIM_SUPPORT_USER_CAS == STD_OFF)
/***********************************************************************************************************************
 * FiM_UnitCmpSwp_CompareAndSwapDefault
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitCmpSwp_CompareAndSwapDefault
                                          (volatile CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) addressPtr,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) newValue)
{
  boolean retVal  = FALSE;

  /* #10 Enter critical section. */
  FiM_EnterCritical_CrossCore();

  /* #20 Assert that the currently stored value is the same as the passed compare value. */
  if (*addressPtr == cmpValue)
  {
    /* #30 if so: write new value. */
    *addressPtr = newValue;                                                      /* SBSW_FIM_COMPSWAPFUNC_ADDRESS_PTR */
    retVal = TRUE;
  }
  else
  {
    /* #40 otherwise: return FALSE. */
    retVal = FALSE;
  }

  /* #50 Leave critical section. */
  FiM_LeaveCritical_CrossCore();

  return retVal;
}
#endif


/***********************************************************************************************************************
 * public FiM_UnitCmpSwp_CompareAndSwap
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitCmpSwp_CompareAndSwap
                                          (volatile CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) addressPtr,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) cmpValue,
                                           CONST(FiM_ProcessorWordType, AUTOMATIC) newValue)
{
#if (FIM_SUPPORT_USER_CAS == STD_ON)
  return ApplFiM_SyncCompareAndSwap(addressPtr, cmpValue, newValue);/* SBSW_FIM_PTR_FWD_ARG */ /* VCA_FIM_PTR_FWD_ARG */
#else
  return FiM_UnitCmpSwp_CompareAndSwapDefault(addressPtr, cmpValue, newValue);                /* SBSW_FIM_PTR_FWD_ARG */
#endif
}


#if (FIM_DEV_ERROR_DETECT == STD_ON)     
/***********************************************************************************************************************
 * FiM_UnitGeneral_GetNrSatellites
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UnitGeneral_GetNrSatellites(void)
{
  return FiM_GetSizeOfSatelliteInfoTable();
}
#endif


/***********************************************************************************************************************
 * private FiM_UnitBitfieldHdl_GetBitfieldWordIndex
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint32, FIM_CODE) FiM_UnitBitfieldHdl_GetBitfieldWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition)
{
  uint32  retVal = (uint32)(bitPosition / FIM_BITS_PER_PROCESSORWORD);

  return retVal;
}


/***********************************************************************************************************************
 * private FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(uint8, FIM_CODE)  FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex
                                         (CONST(uint32, AUTOMATIC) bitPosition)
{
  uint8 retVal = (uint8)(bitPosition % FIM_BITS_PER_PROCESSORWORD);

  return retVal;
}



/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_TestBitInWord
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitBitfieldHdl_TestBitInWord
                                          (CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                           CONST(uint8, AUTOMATIC) bitInWordPosition)
{
  boolean               retVal  = FALSE;
  
  /* #10 Set up bitmask for the word depending on passed bitPosition. */
  FiM_ProcessorWordType bitMask = 0x01;
                        bitMask = (bitMask << bitInWordPosition);
  /* #20 Mask out all other bits and test whether AND yields something different than 0. */
  if ((bitfieldWord & bitMask) != 0U)
  {
    /* #30 Return TRUE if yes (bit is set). */
    retVal = TRUE;
  }
  else
  {
    /* #40 Return FALSE if no (bit is not set). */
    retVal = FALSE;
  }

  return retVal;
}


/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_TestBitInBitfield
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE
  FUNC(boolean, FIM_CODE) FiM_UnitBitfieldHdl_TestBitInBitfield
                            (volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                             CONST(uint32, AUTOMATIC) bitPosition)
{
  /* ! bitfieldWordPtr may only be used for read access ! */

  uint32  bitfieldWordArrIndex  = FiM_UnitBitfieldHdl_GetBitfieldWordIndex(bitPosition);
  uint8   bitInWordPosition     = FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition);
  
  return FiM_UnitBitfieldHdl_TestBitInWord(bitfieldWordPtr[bitfieldWordArrIndex], bitInWordPosition);
}


/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_SetBitInBitfield
 **********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitBitfieldHdl_SetBitInBitfield
                                       (volatile CONSTP2VAR(FiM_ProcessorWordType,AUTOMATIC,AUTOMATIC) bitfieldWordPtr,
                                        CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                        CONST(uint32, AUTOMATIC) bitPosition)
{
  /* #10 Identify the index into the word array where the bit is located. */
  uint32  bitfieldWordArrIndex = FiM_UnitBitfieldHdl_GetBitfieldWordIndex(bitPosition);

  /* #20 If runtime checks are enabled: Check that write access is not done beyond array bounds. */
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (bitfieldWordArrIndex < bitfieldWordArrSize)
#else
  FIM_DUMMY_STATEMENT_CONST(bitfieldWordArrSize);             /*lint -e{438} */
#endif
  {
    FiM_ProcessorWordType oldValue  = 0;
    FiM_ProcessorWordType newValue  = 0;

    /* #30 Identify the word and the bitmask for the word. */
    volatile P2VAR(FiM_ProcessorWordType, DEM_VAR_UNCACHED, AUTOMATIC) wordPtr = &bitfieldWordPtr[bitfieldWordArrIndex];
    FiM_ProcessorWordType bitMask = 0x01U;
                          bitMask = bitMask << (FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition));

    /* #40 Repeat until CompareAndSwap succeeds. */
    do
    {
      /* #50 Set the correct bit in the bitfield word in a current stack copy. */
      oldValue = *wordPtr;
      newValue = (oldValue | bitMask);
      /* #60 Try to update (compare & swap) the RAM contents. */
    } while(FiM_UnitCmpSwp_CompareAndSwap(wordPtr,oldValue,newValue) == FALSE);/* SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL */
  }
  return;
}


/***********************************************************************************************************************
 * public FiM_UnitBitfieldHdl_ResetBitInBitfield
 **********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitBitfieldHdl_ResetBitInBitfield
                                       (volatile CONSTP2VAR(FiM_ProcessorWordType,AUTOMATIC,AUTOMATIC) bitfieldWordPtr,
                                        CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                        CONST(uint32, AUTOMATIC) bitPosition)
{
  /* #10 Identify the index into the word array where the bit is located. */
  uint32  bitfieldWordArrIndex = FiM_UnitBitfieldHdl_GetBitfieldWordIndex(bitPosition);

  /* #20 If runtime checks are enabled: Check that write access is not done beyond array bounds. */
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (bitfieldWordArrIndex < bitfieldWordArrSize)
#else
  FIM_DUMMY_STATEMENT_CONST(bitfieldWordArrSize);            /*lint -e{438} */
#endif
  {
    FiM_ProcessorWordType oldValue  = 0;
    FiM_ProcessorWordType newValue  = 0;

    /* #30 Identify the word and the bitmask for the word. */
    volatile P2VAR(FiM_ProcessorWordType, DEM_VAR_UNCACHED, AUTOMATIC) wordPtr = &bitfieldWordPtr[bitfieldWordArrIndex];
    FiM_ProcessorWordType bitMask = 0x01U;
                          bitMask = ~(bitMask << (FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex(bitPosition)));

    /* #40 Repeat until CompareAndSwap succeeds. */
    do
    {
      /* #50 Reset the correct bit in the bitfield word in a current stack copy. */
      oldValue = *wordPtr;
      newValue = (oldValue & bitMask);
      /* #60 Try to update (compare & swap) the RAM contents. */
    } while(FiM_UnitCmpSwp_CompareAndSwap(wordPtr,oldValue,newValue) == FALSE);/* SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL */
  }
  return;
}


/***********************************************************************************************************************
 *  private FiM_UnitGeneral_CalcInitSatusSatCumulated
 **********************************************************************************************************************/
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
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UnitGeneral_CalcInitSatusSatCumulated(void)
{
  FiM_SatelliteInfoTableIterType  satInfoIter       = 0;
  FiM_InitializationStatusType    initStatCumulated = FIM_INITIALIZED;

  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    FiM_InitializationStatusType initStatSat
      = FiM_UnitGeneral_GetInitializationStatusSat((FiM_SatelliteIdType)satInfoIter);
    if (initStatSat == FIM_PREINITIALIZED)
    {
      initStatCumulated = FIM_PREINITIALIZED;
    }
    if (initStatSat == FIM_UNINITIALIZED)
    {
      initStatCumulated = FIM_UNINITIALIZED;
      break;
    }
  }
  FiM_UnitGeneral_InitializationStatus_SatCumulated = initStatCumulated;
}


/***********************************************************************************************************************
 * private FiM_UnitEvHdl_GetCountOfEvents_ByTableType
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_EventIdTableIndexType, FIM_CODE)
                                                      FiM_UnitEvHdl_GetCountOfEvents_ByTableType(uint8 eventIdTableType)
{
  FiM_EventIdTableIndexType retVal = 0;
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    retVal = (FiM_EventIdTableIndexType)(FiM_GetSizeOfEventIdPendingTable() - (FiM_EventIdTableIndexType)1U);
  }
  else
  {
    retVal = (FiM_EventIdTableIndexType)(FiM_GetSizeOfEventIdTable() - (FiM_EventIdTableIndexType)1U);
  }
  return retVal;
}


/***********************************************************************************************************************
 * private FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Dem_EventIdType, FIM_CODE)
              FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(FiM_EventIdTableIndexType index, uint8 eventIdTableType)
{
  Dem_EventIdType eventId = FIM_DEM_EVENT_INVALID;
# if (FIM_FEATURE_OBD == STD_OFF)
  FIM_DUMMY_STATEMENT(eventIdTableType);                                 /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#else
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    eventId = FiM_UnitEvHdl_GetEventId_EventIdPendingTable(index);
  }
  else
#endif
  {
    eventId = FiM_UnitEvHdl_GetEventId_EventIdTable(index);
  }
  return eventId;
}


/***********************************************************************************************************************
 * private FiM_UnitInhStatHdl_GetInhCode_BySatId
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_InhCodeTableType, FIM_CODE) FiM_UnitInhStatHdl_GetInhCode_BySatId(uint16 index,
                                                                                            FiM_SatelliteIdType satId)
{
  /* caller has to assert: satId < FiM_GetSizeOfSatelliteInfoTable() */
  const FiM_InhCodeTableType  *inhCodeTableArr  = FiM_UnitInhStatHdl_GetInhCodeTableBasePtr_SatelliteInfoTable(satId);
  FiM_InhCodeTableType        inhCode           = inhCodeTableArr[index];

  return inhCode;
}



/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableDirect
 **********************************************************************************************************************/
/*!
 *  Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableDirect
                                   (Dem_EventIdType eventId /* in */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */)
{
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UnitEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  FiM_EventIdTableIndexType searchEventIdTableDirectOffset = 0;
# if (FIM_FEATURE_OBD == STD_OFF)
  FIM_DUMMY_STATEMENT(eventIdTableType);                                 /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#else
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    /* we're searching in event id pending list                                               */
    searchEventIdTableDirectOffset = FiM_UnitEvHdl_SearchEventIdPendingTableDirectOffset;
  }
  else
#endif
  {
    /* we're searching in event id list                                                       */
    searchEventIdTableDirectOffset = FiM_UnitEvHdl_SearchEventIdTableDirectOffset;
  }
  /* #10 if event id is within range limits */
  if ((eventId >= searchEventIdTableDirectOffset)                             &&
      (eventId <  (countOfEvents + searchEventIdTableDirectOffset)))
  {
    /* #20 event id found: subtract offset from event id, return this as index into event id table */
    (*evTableIdx) =                                                           /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
      (FiM_EventIdTableIndexType)((FiM_EventIdTableIndexType)eventId - searchEventIdTableDirectOffset);
    /* #30 set return value to TRUE */
    retVal = TRUE;
  }
  /* #40 otherwise */
  else
  {
    /* #50 event id not found: set return value to FALSE */
    retVal = FALSE;
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableLinear
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableLinear
                                   (Dem_EventIdType eventId                                            /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                                    uint8 eventIdTableType                                             /* in  */)
{
  /* #10 set return value to FALSE */
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UnitEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  /* #30 loop through all indexes in event id table until end is reached or event id was found */
  for(*evTableIdx = 0;                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
      *evTableIdx < countOfEvents;
      (*evTableIdx)++)                                                        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */
  {
    /* #40 check whether event id at current probe index matches */
    Dem_EventIdType  currEventId  = FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(*evTableIdx, eventIdTableType);
    if (currEventId == eventId)
    {
      /* #50 it matches -> event id found; return this as index into event id table and set return value to TRUE */
      retVal = TRUE;
      break;
    }
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UnitEvHdl_SearchEventIdTableBinary
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(boolean, FIM_CODE) FiM_UnitEvHdl_SearchEventIdTableBinary
                                   (Dem_EventIdType eventId                                           /* in  */,
                                    P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx /* out */,
                                    uint8 eventIdTableType                                            /* in  */)
{
  /* #05 set return value to FALSE */
  boolean                   retVal        = FALSE;
  FiM_EventIdTableIndexType countOfEvents = FiM_UnitEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);

  /* #10 init BOTTOM (loIdx) and TOP (hiIdx) boundaries to first and last element in event id table */
  FiM_EventIdTableIndexType loIdx = 0;
  FiM_EventIdTableIndexType hiIdx = FiM_UnitEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);
  if (hiIdx != 0U)                                                        /* PRQA S 2991, 2995 */ /* MD_FiM_ConfCheck */
  {
    /* indices start at 0 */
    hiIdx--;
  }
  else
  {
    /* only the last dummy element is contained in the array -> there's no valid event id present */
    /* set loIdx to value that immediately ends function */
    loIdx = hiIdx; /* + 1U; avoid GCC warning */                                /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    loIdx++;
  }

  /* #20 loop while bottom index smaller than top index, element not found and search index is still within bounds */
  while (loIdx <= hiIdx)
  {
    boolean         notAvail      = FALSE;
    Dem_EventIdType probeEventId  = FIM_DEM_EVENT_INVALID;

    /* #30 calculate the probe reference (in the middle of loIdx and hiIdx) */
    (*evTableIdx) = (FiM_EventIdTableIndexType)((hiIdx + loIdx) >> 1);        /* SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR */

    /* #40 if event id found */
    probeEventId  = FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(*evTableIdx, eventIdTableType);
    if (probeEventId == eventId)
    {
      /* found a matching element */
      /* #50 set return value to TRUE (found) */
      retVal = TRUE;
    }
    /* #60 otherwise */
    else
    {
      /* not found a matching element */
      /* #70 determine whether lower or higher half shall be taken as interval for next try */
      /* #80 if event id found is higher than the id we're looking for */
      if (probeEventId > eventId)
      {
        /* id we're looking for is lower than the current reference */
        /* #90 Use lower half of interval for next loop, i.e. set new top index to current probe index  */
        /* #100 Make sure new index stays within array bounds, remember if it wants to get beyond       */
        if((*evTableIdx) > 0U)
        {
          hiIdx = (FiM_EventIdTableIndexType)((*evTableIdx) - 1U);
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
      /* #110 otherwise */
      else
      {
        /* id we're looking for is greater than the current reference */
        /* #120 Use upper half of interval for next loop, i.e. set new bottom index to current probe index */
        /* #130 Make sure new index stays within array bounds, remember if it wants to get beyond          */
        if((*evTableIdx) < (countOfEvents - 1U))                          /* PRQA S 2992, 2996 */ /* MD_FiM_ConfCheck */
        {
          loIdx = (FiM_EventIdTableIndexType)((*evTableIdx) + 1U);              /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
        }
        else
        { /* not available */
          notAvail = TRUE; /* found no matching element */
        }
      }
    }
    /* #140 break loop if element was found or index went beyond array bounds */
    /* #150 check whether bottom index is still lower than top index is done in loop condition */
    if ((retVal == TRUE) || (notAvail == TRUE))
    {
      break;
    }
  }

  return retVal;
}


/***********************************************************************************************************************
 *  public FiM_SetSearchEventIdTableFct
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL FUNC(void, FIM_CODE) FiM_UnitEvHdl_SetSearchEventIdTableFct(uint8 eventIdTableType)
{
  Dem_EventIdType           prevEventId  /* previous event id */
    = FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(0, eventIdTableType);  /* previous event id */
  FUNC(boolean, FIM_CODE)   (*searchEventIdTableFct)
                              (Dem_EventIdType eventId                                            /* in  */,
                               P2VAR(FiM_EventIdTableIndexType, AUTOMATIC, AUTOMATIC) evTableIdx  /* out */,
                               uint8 eventIdTableType                                             /* in  */)
    = FiM_UnitEvHdl_SearchEventIdTableDirect;
  FiM_EventIdTableIndexType searchEventIdTableDirectOffset
    = prevEventId;
  FiM_EventIdTableIndexType countOfEvents
    = FiM_UnitEvHdl_GetCountOfEvents_ByTableType(eventIdTableType);

  FiM_EventIdTableIndexType evTableIdx = 1;
  for (evTableIdx = 1; evTableIdx < countOfEvents; evTableIdx++)          /* PRQA S 2994, 2996 */ /* MD_FiM_ConfCheck */
  {
    /* current event id  */
    Dem_EventIdType currEventId = FiM_UnitEvHdl_GetEventId_EventIdTable_ByTableType(evTableIdx, /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
                                                                    eventIdTableType);
    /* are we still hoping for direct access? if so, check whether next id is still continuous */
    if ((searchEventIdTableFct == FiM_UnitEvHdl_SearchEventIdTableDirect) &&
        (currEventId != (prevEventId + 1U)))
    {
      /* not continuous anymore -> switch to binary search if list is not too small */
      /*lint -save -e506 */
      if (countOfEvents >= FIM_BINSEARCH_MINLISTLENGTH) /* PRQA S 2991, 2992, 2995, 2996, 2880 */ /* MD_FiM_ConfCheck */
      /*lint -restore */
      {
        searchEventIdTableFct = FiM_UnitEvHdl_SearchEventIdTableBinary;         /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
      }
      else
      {
        /* use linear algorithm for small lists (if direct access cannot be used) */
        searchEventIdTableFct = FiM_UnitEvHdl_SearchEventIdTableLinear;         /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
      }
    }
    /* are we hoping for binary algorithm, i.e. at least ordered event ids?                                           */
    /* no check if (FiM_UnitEvHdl_SearchEventIdTableFct == FiM_UnitEvHdl_SearchEventIdTableBinary) necessary because: */
    /* - if we're still in direct access mode (current event id <= previous event id)                                 */
    /*   always returns FALSE                                                                                         */
    /* - if we're already in linear search mode it won't do any harm; we may or may not enter the                     */
    /*   block but afterwards it will still be linear mode and we will stop                                           */
    /* Note: the next two decisions should only be true if event ids were re-assigned after                           */
    /*       FIM generation                                                                                           */
    if (currEventId <= prevEventId)
    {
      /* not ordered anymore -> switch to linear search and stop */
      searchEventIdTableFct = FiM_UnitEvHdl_SearchEventIdTableLinear;
    }
    prevEventId = currEventId;
    /* are we down to linear search? */
    if (searchEventIdTableFct == FiM_UnitEvHdl_SearchEventIdTableLinear)
    {
      /* we're down to linear search -> stop the search for the optimal algorithm - it won't get any better */
      break;
    }
  }
# if (FIM_FEATURE_OBD == STD_ON)
  if (eventIdTableType == FIM_EVENTIDPENDING_TABLE)
  {
    /* set search parameters for pending event id list                                            */
    FiM_UnitEvHdl_SearchEventIdPendingTableFct          = searchEventIdTableFct;
    FiM_UnitEvHdl_SearchEventIdPendingTableDirectOffset = searchEventIdTableDirectOffset;
  }
  else
#endif
  {
    /* set search parameters for inhibition event id list                                         */
    FiM_UnitEvHdl_SearchEventIdTableFct           = searchEventIdTableFct;
    FiM_UnitEvHdl_SearchEventIdTableDirectOffset  = searchEventIdTableDirectOffset;
  }
}


/***********************************************************************************************************************
 *  public FiM_UnitGeneral_GetInitializationStatusMaster()
 **********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
                          FiM_UnitGeneral_GetInitializationStatusMaster(void) /* PRQA S 3219 */ /* MD_MSR_Unreachable */
{
  FiM_InitializationStatusType initStatus = FIM_UNINITIALIZED;
  if (FiM_UnitGeneral_PreInitialized == TRUE)
  {
    initStatus = FIM_PREINITIALIZED;
    if (FiM_UnitGeneral_MasterInitialized == TRUE)
    {
      initStatus = FIM_INITIALIZED;
    }
  }
  return initStatus;
}


/***********************************************************************************************************************
 *  public FiM_UnitGeneral_GetInitializationStatusSat()
 **********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
                                                   FiM_UnitGeneral_GetInitializationStatusSat(FiM_SatelliteIdType satId)
{
  FiM_InitializationStatusType initStatus = FIM_UNINITIALIZED;
  if (FiM_UnitGeneral_PreInitialized == TRUE)
  {
    initStatus = FIM_PREINITIALIZED;
    if (*FiM_UnitGeneral_GetInitStatusPtr_SatelliteInfoTable(satId) == FIM_INITIALIZED)
    {
      initStatus = FIM_INITIALIZED;
    }
  }
  return initStatus;
}


/***********************************************************************************************************************
 *  public FiM_UnitGeneral_GetInitializationStatusSatCumulated()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_InitializationStatusType, FIM_CODE)
  FiM_UnitGeneral_GetInitializationStatusSatCumulated(void)                   /* PRQA S 3219 */ /* MD_MSR_Unreachable */
{
  return FiM_UnitGeneral_InitializationStatus_SatCumulated;
}


/***********************************************************************************************************************
 *  private FiM_UnitGeneral_SetInitializationStatusSat_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE)
  FiM_UnitGeneral_SetInitializationStatusSat_Checked(FiM_InitializationStatusType initStatus, FiM_SatelliteIdType satId)
{
  Std_ReturnType retVal = E_NOT_OK;                                                   /* PRQA S 2981 */ /* MD_FiM_2.2 */
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (satId < FiM_GetSizeOfSatelliteInfoTable())
#endif
  {
    /* Set initialization state to passed value. */
    *FiM_UnitGeneral_GetInitStatusPtr_SatelliteInfoTable(satId) = initStatus;            /* SBSW_FIM_ACCESS_INITSTATUSSAT_CHECKED */
    retVal = E_OK;
  }
  return retVal;
}


/***********************************************************************************************************************
 *  private FiM_UnitInhStatHdl_ModInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UnitInhStatHdl_ModInhibitionStatus_Checked(boolean inhibStat,
  uint16 inhCodeIdx,
  FiM_SatelliteIdType satId)
{
  Std_ReturnType retVal = E_NOT_OK;                                                   /* PRQA S 2981 */ /* MD_FiM_2.2 */
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (satId < FiM_GetSizeOfSatelliteInfoTable())
#endif
  {
    FiM_ProcessorWordType *inhStatusBasePtr = FiM_UnitInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);
    uint16                inhStatusLength   = FiM_UnitInhStatHdl_GetInhStatusLength_SatelliteInfoTable(satId);

    /* set bit in bitfield to inhibition status */
    if (inhibStat == TRUE)
    {
      FiM_UnitBitfieldHdl_SetBitInBitfield(inhStatusBasePtr,                   /* SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL */
                                           inhStatusLength, inhCodeIdx);
    }
    else
    {
      FiM_UnitBitfieldHdl_ResetBitInBitfield(inhStatusBasePtr,                 /* SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL */
                                             inhStatusLength, inhCodeIdx);
    }
    retVal = E_OK;
  }
  return retVal;
}


/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_ClearInhibitionStatus_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE) FiM_UnitInhStatHdl_ClearInhibitionStatus_Checked(uint16 inhCodeIdx,
                                                                                              FiM_SatelliteIdType satId)
{
  return FiM_UnitInhStatHdl_ModInhibitionStatus_Checked(FALSE, inhCodeIdx, satId);
}


/***********************************************************************************************************************
 *  public FiM_UnitInhStatHdl_ClearInhibitionStatusAll_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(Std_ReturnType, FIM_CODE)
                                          FiM_UnitInhStatHdl_ClearInhibitionStatusAll_Checked(FiM_SatelliteIdType satId)
{
  Std_ReturnType        retVal              = E_NOT_OK;                               /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (satId < FiM_GetSizeOfSatelliteInfoTable())
#endif
  {
    FiM_ProcessorWordType *inhStatusBasePtr = FiM_UnitInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);
    uint16                inhStatusLength   = FiM_UnitInhStatHdl_GetInhStatusLength_SatelliteInfoTable(satId);
    FiM_ProcessorWordType inhStatIter       = 0;

    FiM_EnterCritical_CrossCore();
    for(inhStatIter = 0; inhStatIter < inhStatusLength; inhStatIter++)
    {
      inhStatusBasePtr[inhStatIter] = 0;                                              /* SBSW_FIM_ACCESS_INHSTATUSSAT */
    }
    FiM_LeaveCritical_CrossCore();
    retVal = E_OK;
  }
  return retVal;
}


# if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
*  private FiM_UnitPendCntHdl_IncPendingCounter_Checked()
**********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_IncPendingCounter_Checked(FiM_FunctionIdType fid)
{
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = fid;
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid16 < FiM_GetSizeOfFidPendingCounter())                           /* PRQA S 2991, 2995 */ /* MD_FiM_ConfCheck */
#endif
  {
    FiM_IncFidPendingCounter(fid16);                                     /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
  }
}


/***********************************************************************************************************************
 *  private FiM_UnitPendCntHdl_DecPendingCounter_Checked()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_DecPendingCounter_Checked(FiM_FunctionIdType fid)
{
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = fid;
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  if (fid16 < FiM_GetSizeOfFidPendingCounter())                           /* PRQA S 2991, 2995 */ /* MD_FiM_ConfCheck */
#endif
  {
    if (FiM_GetFidPendingCounter(fid16) > 0U)
    {
      FiM_DecFidPendingCounter(fid16);                                   /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED */
    }
  }
}
# endif


/***********************************************************************************************************************
 *  private FiM_UnitInhStatHdl_GetInhibitionStatusFromMonitor()
 **********************************************************************************************************************/
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
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
                      FiM_UnitInhStatHdl_GetInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus, uint8 inhCode)
{
  boolean inhibStat = FALSE;
  /* #10 if runtime checks are enabled                                                              */
#if (FIM_DEV_RUNTIME_CHECKS == STD_ON)                                                      /* COV_FIM_RUNTIME_CHECKS */
  /* #20 if inhibition code is not valid                                                            */
  /*     Invalid inhibition codes are ignored                                                       */
  /*     only relevant if there is a problem with the generator                                     */
  /*     would result otherwise in an out of bounds read access and an undefined inhibition status  */
  /*       -> avoid out of bound access and define status to permitted (i.e. ignore code)           */
  if (inhCode >= FIM_INHCODE_NUMBER)
  {
    /* #30 define status to permitted (i.e. ignore code)           */
    inhibStat = FALSE;
  }
  /* #40 otherwise */
  else
#endif
  {
    /* #100 mask the passed monStatus with the mask that's relevant for the passed inhibition code                    */
    /* #110 this yields the bits that are relevant for the passed inhibition code                                     */
    /* #120 if these bits match the bit pattern relevant for the passed inhibition code then combination inhibits FID */
    inhibStat = (boolean)((monStatus & FiM_UnitInhStatHdl_InhibitionModeTable[inhCode].Mask) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
                          == FiM_UnitInhStatHdl_InhibitionModeTable[inhCode].Pattern);
    /* #130 check whether inhibition code requests an inversion of the result                                         */
    if (FiM_UnitInhStatHdl_InhibitionModeTable[inhCode].InvertResult == TRUE)
    {
      inhibStat = (boolean)(!inhibStat);                       /* PRQA S 4116, 4304, 4558 */ /* MD_MSR_AutosarBoolean */
    }
  }

  return inhibStat;
}


/***********************************************************************************************************************
*  public FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void,FIM_CODE) FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor(Dem_MonitorStatusType monStatus,
                                                                                        uint16 inhCodeIdx,
                                                                                        FiM_SatelliteIdType satId)
{
  /* #10 get code for index and satellite                                                                             */
  FiM_InhCodeTableType  inhCode   = FiM_UnitInhStatHdl_GetInhCode_BySatId(inhCodeIdx, satId);
  /* #20 get inhibition status for combination of code / monitor status                                               */
  boolean               inhibStat = FiM_UnitInhStatHdl_GetInhibitionStatusFromMonitor(monStatus, inhCode);
  /* #30 set the calculated status                                                                                    */
  (void)FiM_UnitInhStatHdl_ModInhibitionStatus_Checked(inhibStat, inhCodeIdx, satId);
}


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_GetPendingCounter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(FiM_FidCounterType, FIM_CODE) FiM_UnitPendCntHdl_GetPendingCounter(FiM_FunctionIdType fid)
{
  return FiM_GetFidPendingCounter(fid);
}
#endif


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_ClearPendingCounterAll()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_ClearPendingCounterAll(void)
{
  FiM_FidPendingCounterIterType            fidIter = 0;
  /* reset all pending counters to 0 */
  FiM_EnterCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
  for (fidIter = 0; fidIter < FiM_GetSizeOfFidPendingCounter(); fidIter++)
  {
    FiM_SetFidPendingCounter(fidIter, 0);                    /* SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCINHPENDSTATES */
  }
  FiM_LeaveCritical_LocalCore();                                                   /* PRQA S 3138 */ /* MD_MSR_14.3 */
}
#endif


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  public FiM_UnitPendCntHdl_ModPendingCounter_Checked()
 **********************************************************************************************************************/
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
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitPendCntHdl_ModPendingCounter_Checked(FiM_FunctionIdType fid,
                                                                                   uint8 modCodePending)
{
  {
    /* enter critical section to protect each increment/decrement against interruption; */
    /* only one enter/leave over the whole if/else to reduce number of enter/leaves     */
    FiM_EnterCritical_LocalCore();
    if (modCodePending == FIM_FID_DEC)
    {
      /* decrement FID Pending counter is necessary                              */
      FiM_UnitPendCntHdl_DecPendingCounter_Checked(fid);
    }
    if (modCodePending == FIM_FID_INC)
    {
      /* increment FID Pending counter is necessary                              */
      FiM_UnitPendCntHdl_IncPendingCounter_Checked(fid);
    }
    /* leave critical section */
    FiM_LeaveCritical_LocalCore();
  }
  return;
}
#endif


/**********************************************************************************************************************
 *  private FiM_UnitEvHdl_CalcInhibStatusOfEvent()
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitEvHdl_CalcInhibStatusOfEvent(FiM_EventIdTableIndexType evTableIdx)
{
  /* #10 identify the parts of the complete inhibition config tables that are relevant for this event         */
  /*     do so by identifying start index and stop index (last relevant element plus 1) in the complete array */
  uint16 inhCodeTableIdx_Start = FiM_UnitEvHdl_GetEventToInhCodeTableIndex_EventIdTable(evTableIdx);
  uint16 inhCodeTableIdx_Stop  = (uint16)((uint16)FiM_UnitEvHdl_GetEventToInhCodeTableCount_EventIdTable(evTableIdx) +
                                    inhCodeTableIdx_Start);
  FiM_SatelliteIdType   satId                 = FiM_UnitEvHdl_GetSatelliteId_EventIdTable(evTableIdx);
  const Dem_EventIdType cEvId                 = FiM_UnitEvHdl_GetEventId_EventIdTable(evTableIdx);

  Dem_MonitorStatusType monitorStatusAtBegin  = 0x00;
  Dem_MonitorStatusType monitorStatusAtEnd    = 0x00;
  Std_ReturnType        retCodeAtBegin        = E_NOT_OK;
  /* #20 get monitor status for event from DEM                                                                */
  Std_ReturnType        retCodeAtEnd          =
    Dem_GetMonitorStatus(cEvId, &monitorStatusAtEnd); /* SBSW_FIM_DEMGETMONST_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */

  /* #30 repeat the following until both return code and monitor status didn't change during calculation      */
  /*     (in which case we were interrupted and the basis for our calculation changed -> we have to redo it   */
  do
  {
    uint16  inhCodeIdx = inhCodeTableIdx_Start;

    retCodeAtBegin        = retCodeAtEnd;
    monitorStatusAtBegin  = monitorStatusAtEnd;
    /* #100 loop through all inhibition configurations that are connected to the event */
    for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++)
    {
      /* inhibition configuration table is always generated, no manual calibration here                           */
      /* -> it does not contain invalid inhibition configurations                                                 */
      /* even if it contained an invalid event id, first element of FiM_UnitInhStatHdl_InhibitionModeTable would  */
      /* neutralise it                                                                                            */

      /* #110 if monitor status could be obtained */
      if (retCodeAtBegin == E_OK)
      {
        /* #120 determine the inhibition status of this particular inhibition cfg                                     */
        /* satId is taken from eventId table; as long as event id table is generated correctly, satId is smaller      */
        /* than size of satellite info table -> pre-condition for FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor  */
        /* is fulfilled                                                                                               */
        FiM_UnitInhStatHdl_CalcInhibitionStatusFromMonitor(monitorStatusAtBegin, inhCodeIdx, satId);
      }
      /* #130 otherwise */
      else
      {
        /* #140 event is not available in DEM -> it cannot inhibit the function                           */
        /*      (it may not be available, e.g. because SetEventAvailable was called with parameter FALSE) */
        /* SPEC-63513 */
        (void)FiM_UnitInhStatHdl_ClearInhibitionStatus_Checked(inhCodeIdx, satId);
      }
      
    } /* for(inhCodeIdx = inhCodeTableIdx_Start; inhCodeIdx < inhCodeTableIdx_Stop; inhCodeIdx++) */

    retCodeAtEnd =
      Dem_GetMonitorStatus(cEvId,&monitorStatusAtEnd);/* SBSW_FIM_DEMGETMONST_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */
  } while ((monitorStatusAtEnd != monitorStatusAtBegin) || (retCodeAtEnd != retCodeAtBegin));

  /* #200 if FIM_AFFECTED_FIDS_IDENTIFICATION is enabled */
#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
  /* #210 identify the parts of the complete event to FID config tables that are relevant for this event       */
  /*     do so by identifying start index and stop index (last relevant element plus 1) in the complete array  */
  {
    uint16  evToFidTableIdx_Start = FiM_UnitEvHdl_GetEventToFidTableIndex_EventIdTable(evTableIdx);
    uint16  evToFidTableIdx_Stop  = FiM_UnitEvHdl_GetEventToFidTableIndex_EventIdTable(evTableIdx + 1U);
    uint16  evToFidTableIter;
    for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)
    {
      /* #220 set event changed flag of connected FID */
      FiM_UnitFidHdl_SetFidEventChanged(FiM_GetEventToFidTable(evToFidTableIter));
    }
  }
#endif

  return;
}


#if (FIM_FEATURE_OBD == STD_ON)
/**********************************************************************************************************************
 *  private FiM_UnitEvHdl_CalcPendStatusOfEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitEvHdl_CalcPendStatusOfEvent(FiM_EventIdTableIndexType evTableIdx,
                                                                          uint8  modCodePending)
{
  /* #10 check if a manipulation of FID Pending counters is necessary */
  if (modCodePending != FIM_FID_NOCHANGE)
  {
    /* #20 if yes */
    /* #30 identify the part of the complete FID Pending table that is relevant for this event and inhibition cfg     */
    /*     do so by identifying start index and stop index (last relevant element + 1) in the complete array          */
    /* #40 loop through relevant FIDs and manipulate their Pending counters */
    uint16  evToFidTableIdx_Start =
                FiM_GetEventToFidPendingTableIdxOfEventIdPendingTable(evTableIdx);
    uint16  evToFidTableIdx_Stop  =
                FiM_GetEventToFidPendingTableIdxOfEventIdPendingTable(evTableIdx+1U);/* PRQA S 2841 */ /* MD_FiM_18.1 */
    uint16  evToFidTableIter = evToFidTableIdx_Start;
    for (evToFidTableIter = evToFidTableIdx_Start; evToFidTableIter < evToFidTableIdx_Stop; evToFidTableIter++)
    {
      /* #50 modify FID Pending counters according to given modification code */
      FiM_UnitPendCntHdl_ModPendingCounter_Checked(FiM_GetEventToFidPendingTable(evToFidTableIter), modCodePending);
    }
  }
}
#endif


/**********************************************************************************************************************
 *  public FiM_UnitFidHdl_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)  FiM_UnitFidHdl_Init(void)
{
# if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
  {
    FiM_PermStatusTableIterType         permStatIter  = 0;
    for (permStatIter = 0; permStatIter < FiM_GetSizeOfPermStatusTable(); permStatIter++)
    {
      /* default is "not permitted" until first calculation                       */
      /* reasoning: before completion of initialization it's also "not permitted" */
      FiM_SetPermStatusTable(permStatIter, 0);                                          /* SBSW_FIM_PERMSTATUS_ACCESS */
    }
#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
    {
      FiM_FidEventChangedTableIterType  evChgdIter  = 0;
      for (evChgdIter = 0; evChgdIter < FiM_GetSizeOfFidEventChangedTable(); evChgdIter++)
      {
        /* default is "changed" until first calculation                             */
        /* reasoning: the FIDs need calculation                                     */
        FiM_SetFidEventChangedTable(evChgdIter, ~(FiM_ProcessorWordType)0);              /* SBSW_FIM_EVCHANGED_ACCESS */
      }
    }
#endif
    FiM_UnitFidHdl_NextFidToSchedule = 0;
  }
#endif
}


#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
/***********************************************************************************************************************
 *  private FiM_UnitBitfieldHdl_GetNextSetBitInWord()
 **********************************************************************************************************************/
/*! \brief                          Internal helper function to scan a range within a word for next bit that is set.
 *
 *  \details                        Scans a range (between startInWordIndex and stopInWordIndex - each including)
 *                                  within a word for the next bit that is set. Does not consider wrap around.
 *  
 *  \param[in]      bitfieldWord    Word to scan (part of bitfield).
 *  \param[in,out]  bitIndexPtr     Must point to a valid memory location.
 *                                  In:   Start of bitfield range (including) to scan.
 *                                  Out:  Index of bit that was found.
 *                                        If no bit was found, the index where the scan stopped (not plus 1).
 *  \param[in]      stopInWordIndex Stop of bitfield range (including) to scan. May not be smaller than start index.
 *  \return                         TRUE if bit was found
 *                                  FALSE if bit was not found
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UnitBitfieldHdl_GetNextSetBitInWord(CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                    P2VAR(uint8, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                    CONST(uint8, AUTOMATIC) stopInWordIndex);

/***********************************************************************************************************************
 *  public FiM_UnitBitfieldHdl_GetNextSetBit()
 **********************************************************************************************************************/
/*! \brief                          Scans a bitfield range between for the next bit that is set.
 *
 *  \details                        This function scans from a given start index in the provided bitfield up to a given
 *                                  stop position for the next bit that is set. It also considers wrap around and
 *                                  actual length of bitfield (not necessarily being identical with end of word array).
 *
 *  \param[in]      bitfieldWordPtr Pointer to bitfield array (actually word array containing bitfield).
 *                                  Internally only used for read access.
 *  \param[in]      bitfieldLength  Bitfield length (in bits).
 *                                  Can be smaller than bitfield array length (in words) * FIM_BITS_PER_PROCESSORWORD.
 *  \param[in,out]  bitIndexPtr     Must point to a valid memory location.
 *                                  In:   Start of bitfield range (including) to scan.
 *                                  Out:  Index of bit that was found.
 *                                        If no bit was found, the index where the scan stopped plus 1 (with wrap
 *                                        around if necessary)
 *  \param[in]      stopIndex       Stop of bitfield range (excluding) to scan. Can be smaller than start index due
 *                                  to wrap around at end of bitfield.
 *  \return                         TRUE if bit was found
 *                                  FALSE if bit was not found
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UnitBitfieldHdl_GetNextSetBit(volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                    CONST(uint32, AUTOMATIC) bitfieldLength, /* in bits */
                                    P2VAR(uint32, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                    CONST(uint32, AUTOMATIC) stopBit);
#endif

#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
/**********************************************************************************************************************
 *  private FiM_UnitBitfieldHdl_GetNextSetBitInWord()
 *********************************************************************************************************************/
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
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UnitBitfieldHdl_GetNextSetBitInWord(CONST(FiM_ProcessorWordType, AUTOMATIC) bitfieldWord,
                                          P2VAR(uint8, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                          CONST(uint8, AUTOMATIC) stopInWordIndex)
{
  /* save start index */
  uint8   iterInWordIndex = *bitIndexPtr;
  boolean found           = FALSE;
  /* if no set bit is found, we return the stop position */
          *bitIndexPtr    = stopInWordIndex;                                         /* SBSW_FIM_BITINDEX_ADDRESS_PTR */
  /* #040   check whether this word is 0 (i.e. no bit is set) */
  /*        if it's not 0 we need to take a closer look (set bit could be before startBit or after bitfield end */
  if (bitfieldWord != 0U)
  {
    /* #050   if word is not 0, traverse through bits in word up to calculated local stop */
    for (; iterInWordIndex <= stopInWordIndex; iterInWordIndex++)
    {
      /* #060   as soon as a set bit was found in this area, then break and return found position */
      found = FiM_UnitBitfieldHdl_TestBitInWord(bitfieldWord, iterInWordIndex);
      if (found == TRUE)
      {
        *bitIndexPtr = iterInWordIndex;                                              /* SBSW_FIM_BITINDEX_ADDRESS_PTR */
        break;
      }
    }
  }
  else
  {
    /* #070   if word is 0 there's nothing to do: it's sure that there is no set bit in the local scan area */
  }
  return found;
}


/**********************************************************************************************************************
 *  public FiM_UnitBitfieldHdl_GetNextSetBit()
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_UnitBitfieldHdl_GetNextSetBit(volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                    CONST(uint32, AUTOMATIC) bitfieldLength, /* in bits */
                                    P2VAR(uint32, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                    CONST(uint32, AUTOMATIC) stopBit)
{
  /* do some preparational stuff so we can use indices (including) for the remainder of the function */
  /* last index in bitfield (including)                   */
  uint32  bitfieldEndIndex  = bitfieldLength - 1UL;
  /* overall index from which scan starts (including)     */
  uint32  startIndex        = *bitIndexPtr;
  /* overall index at which scan stops (including)        */
  /*    if (stopBit == 0UL)                               */
  /*      stopIndex = bitfieldEndIndex;                   */
  /*    else                                              */
  /*      stopIndex = stopBit - 1UL;                      */
  uint32  stopIndex = (stopBit == 0UL) ? bitfieldEndIndex : (stopBit - 1UL);
  /* overall index at which we found a set bit (if any)   */
  /* otherwise index where scan stopped (excluding)       */
  uint32  foundIndex        = stopBit;                                 /* PRQA S 2981 */ /* MD_FiM_2981_GetNextSetBit */
  /* have we found a bit that is set?                     */
  boolean found             = FALSE;

  /* #010 set iterator to start of scan */
  uint32 iterIndex = startIndex;
  /* #020 start loop (from startIndex till stopIndex while considering wrap around; stop loop if a set bit was found) */
  for (;;)
  {
    /* #030   get word index in which iterator is located */
    uint32  bitfieldWordArrIndex  = FiM_UnitBitfieldHdl_GetBitfieldWordIndex(iterIndex);
    /* #040   calculate the index at which to stop the next part of the scan; stop at (whichever comes first): */
    /*        - last bit in word (including) */
    uint32 localStopIndex = ((bitfieldWordArrIndex + 1U) * FIM_BITS_PER_PROCESSORWORD) - 1U;
    /*        - or highest bit in bitfield (including) */
    if (localStopIndex > bitfieldEndIndex)
    {
      localStopIndex = bitfieldEndIndex;
    }
    /*        - or stop bit (excluding) - we calculated stopIndex (including) for that                                */
    /*          only if there is no upcoming wrap around (in which case there are upcoming bits at start of bitfield) */
    if ((localStopIndex > stopIndex)  &&
                                          ((stopIndex >= startIndex)    ||  /* wrap around not needed   */
                                           (iterIndex < startIndex)))       /* wrap around already done */
    {
      localStopIndex = stopIndex;
    }

    /* #050   look for a set bit in word (within calculated limits) */
    {
      uint8   foundInWordIndex      = FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex(iterIndex);
      found = FiM_UnitBitfieldHdl_GetNextSetBitInWord(bitfieldWordPtr[bitfieldWordArrIndex],
                                                      &foundInWordIndex,               /* SBSW_FIM_GETNEXTSETBIT_CALL */
                                                      FiM_UnitBitfieldHdl_GetBitfieldBitInWordIndex(localStopIndex));
      /* transform the local index within the word to the found index in the global bitfield  */
      if (found == TRUE)
      {
        foundIndex = foundInWordIndex + (bitfieldWordArrIndex * FIM_BITS_PER_PROCESSORWORD);
      }
    }
    /* #060   advance iterator to last (virtually) checked bit */
    iterIndex = localStopIndex;
    /* #070   stop overall scan if set bit was found or global stop of scan is reached */
    if ((found == TRUE)  ||         /* set bit found */
        (iterIndex == stopIndex))   /* reached requested end of global scan */
    {
      break;
    }
    /* #080     increment iterator to next bit to check (considering potential wrap around)                           */
    iterIndex = (iterIndex == bitfieldEndIndex) ? 0UL : (iterIndex + 1UL);
  }
  /*   #100 end loop and return found index */
  *bitIndexPtr = foundIndex;                                                         /* SBSW_FIM_BITINDEX_ADDRESS_PTR */

  return found;
}
#endif


#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
/**********************************************************************************************************************
 *  public FiM_UnitFidHdl_CalcPermissionStates()
 *********************************************************************************************************************/
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
 *
 *
*/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)  FiM_UnitFidHdl_CalcPermissionStates(void)
{
  /* there is at least one FID */
  /* #010 set starting point of iteration to where we stopped last time */
  const uint32  cFidStart = FiM_UnitFidHdl_NextFidToSchedule;
        uint32  fidIter   = FiM_UnitFidHdl_NextFidToSchedule;
        uint32  fidCount  = 0;
  /* #020 loop maximum through the number of FIDs that are configured per scheduling slice */
  for (fidCount = 0; fidCount < FiM_GetMaxHandledFidsPerCycle(); fidCount++)
  {
#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
    /* #030 if option to identify FIDs affected by an event status change is on, then step ahead to next affected FID */
    /* Scan FidEventChanged bitfield range between current FID (including) and FID that we started from (excluding)   */
    /* for the next FID that is affected by an event status change. Wraps around at end of FID range.                 */
    boolean found =
                 FiM_UnitBitfieldHdl_GetNextSetBit(FiM_GetAddrFidEventChangedTable(0), /* SBSW_FIM_GETNEXTSETBIT_CALL */
                                                   (uint16)FiM_UnitFidHdl_GetNrOfFids(), &fidIter, cFidStart);
    if (found == FALSE)
    {
      /* #040 stop loop if no affected FID was found */
      fidIter = cFidStart;
    }
    else
    {
      /* #050 otherwise reset the affected FID bit                                                                    */
      /*      -> event status changes arriving during our calculation will set it again  and trigger another          */
      /*         calculation in the next cycle                                                                        */
      FiM_UnitBitfieldHdl_ResetBitInBitfield(FiM_GetAddrFidEventChangedTable(0),            /* SBSW_FIM_BITMANIP_CALL */
                                             FiM_GetSizeOfFidEventChangedTable(), fidIter);
#else
    {
#endif
      /* #070 check if FID is inhibited and set bit in bitfield to permission status (which is !inhibition status)    */
      if (FiM_UnitFidHdl_IsFidInhibited((FiM_FunctionIdType)fidIter) == TRUE)
      {
        FiM_UnitBitfieldHdl_ResetBitInBitfield(FiM_GetAddrPermStatusTable(0),               /* SBSW_FIM_BITMANIP_CALL */
                                               FiM_GetSizeOfPermStatusTable(), fidIter);
      }
      else
      {
        FiM_UnitBitfieldHdl_SetBitInBitfield(FiM_GetAddrPermStatusTable(0),                 /* SBSW_FIM_BITMANIP_CALL */
                                             FiM_GetSizeOfPermStatusTable(), fidIter);
      }
      /* #080 select next FID to schedule and roll over if we reached end of FID list */
      fidIter++;
      /* check whether we need to roll over to the start of the FID list */
      if (fidIter >= (uint16)FiM_UnitFidHdl_GetNrOfFids())
      {
        fidIter = 0;
      }
    }
    /* #090 stop if we reached our starting point (each FID is calculated at most once per call to the main function) */
    if (fidIter == cFidStart)
    {
      break;
    }
  }
  /* #100 remember FID to continue with in next call */
  FiM_UnitFidHdl_NextFidToSchedule = (FiM_FunctionIdType)fidIter;
}
#endif


#if (FIM_AFFECTED_FIDS_IDENTIFICATION == STD_ON)
/***********************************************************************************************************************
 * public FiM_UnitFidHdl_SetFidEventChanged
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE) FiM_UnitFidHdl_SetFidEventChanged(FiM_FunctionIdType Fid)
{
  FiM_UnitBitfieldHdl_SetBitInBitfield(FiM_GetAddrFidEventChangedTable(0),                  /* SBSW_FIM_BITMANIP_CALL */
                                       FiM_GetSizeOfFidEventChangedTable(), Fid);
  return;
}
#endif


/**********************************************************************************************************************
 *  private FiM_UnitFidHdl_IsFidInhibited()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UnitFidHdl_IsFidInhibited(FiM_FunctionIdType FID)
{
  boolean       inhibStat = FALSE;

  /* #10 identify the parts of the complete "Fid-to-inhibition-code" table that are relevant for this FID             */
  /* #20 do so by identifying start index and stop index (last relevant element plus 1) in the complete array         */
  uint16_least  fidToInhTableIdx_Start  = (uint16_least)FiM_GetFidTable(FID);
  uint16_least  fidToInhTableIdx_Stop   = (uint16_least)FiM_GetFidTable(FID + 1U);

  /* #30 loop through all inhibition configurations that are connected to the FID                                     */
  uint16_least  fidToInhTableIdx        = fidToInhTableIdx_Start;
  for (fidToInhTableIdx = fidToInhTableIdx_Start; fidToInhTableIdx < fidToInhTableIdx_Stop; fidToInhTableIdx++)
  {
    /* inhibition configuration table is always generated, no manual calibration here                                 */
    /* -> it does not contain invalid inhibition configurations                                                       */
    /* even if it contained an invalid event id, first element of FiM_UnitInhStatHdl_InhibitionModeTable would        */
    /* neutralise it                                                                                                  */

    /* FiM_FidToInhSatIdTable contains only satellite ids smaller than size of satellite info table                   */
    /* satId < FiM_GetSizeOfSatelliteInfoTable() is always true                                                       */
    /* no write access is done later on relying on that fact -> not silence critical -> no runtime check necessary    */
    uint16  inhCodeTableIdx = FiM_UnitFidHdl_GetInhCodeIdx_FidToInhCodeTable(fidToInhTableIdx);
    uint8   satId           = FiM_UnitFidHdl_GetSatId_FidToInhSatIdTable(fidToInhTableIdx);

    /* #40 check if inhibition cfg inhibits FID, break if yes (FID is inhibited if at least one cfg inhibits)         */
    inhibStat = FiM_UnitInhStatHdl_GetInhibitionStatus(inhCodeTableIdx, satId);
    if (inhibStat == TRUE)
    {
      break;
    }
  }
  return inhibStat;
}


#if (FIM_FEATURE_OBD == STD_ON)
/**********************************************************************************************************************
*  private FiM_UnitFidHdl_IsFidPendingLocked()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)  FiM_UnitFidHdl_IsFidPendingLocked(FiM_FunctionIdType FID)
{
  boolean pendingStat = FALSE;

  if (FiM_UnitPendCntHdl_GetPendingCounter(FID) != 0U)
  {
    pendingStat = TRUE;
  }
  else
  {
    pendingStat = FALSE;
  }

  return pendingStat;
}
#endif

 /**********************************************************************************************************************
  *  public FiM_UnitEvHdl_CalcInhStates()
  *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
FIM_LOCAL FUNC(void, FIM_CODE)  FiM_UnitEvHdl_CalcInhStates(FiM_SatelliteIdType satId)
{
  FiM_EventIdTableIndexType     evTableIdx  = 0;

  /* #10 reset all inhibition states of satellite to 0 */
  (void)FiM_UnitInhStatHdl_ClearInhibitionStatusAll_Checked(satId);

  /* #30 loop through all events in FiM_EventIdTable starting at first one until the end of the table is reached */
  for(evTableIdx = 0; evTableIdx < FiM_UnitEvHdl_GetCountOfEvents_ByTableType(FIM_EVENTID_TABLE); evTableIdx++)
  {
    /* event id table is always generated, no manual calibration here -> it does not contain invalid event ids */

    /* #40 calculate the inhibition states of this event if it is mapped to the satellite */
    if (FiM_UnitEvHdl_GetSatelliteId_EventIdTable(evTableIdx) == satId) /* only for events of our own satellite */
    {
      FiM_UnitEvHdl_CalcInhibStatusOfEvent(evTableIdx);
    }
  } /* for(...; evTableIdx < FiM_UnitEvHdl_GetCountOfEvents_ByTableType(); ...) */

  return;
}


#if (FIM_FEATURE_OBD == STD_ON)
/**********************************************************************************************************************
 *  public FiM_UnitEvHdl_CalcPendStates()
 *********************************************************************************************************************/
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
 *
 */
FIM_LOCAL FUNC(void, FIM_CODE)  FiM_UnitEvHdl_CalcPendStates(void)
{
  FiM_EventIdTableIndexType     evTableIdx  = 0;

  /* #20 reset all pending counters to 0 */
  FiM_UnitPendCntHdl_ClearPendingCounterAll();

  /* #30 loop through all events in FiM_EventIdPendingTable starting at the first one until the end of table is reached */
  for(evTableIdx = 0;
      evTableIdx < FiM_UnitEvHdl_GetCountOfEvents_ByTableType(FIM_EVENTIDPENDING_TABLE); /* PRQA S 2994, 2996 */ /* MD_FiM_ConfCheck */
      evTableIdx++)
  {
    /* event id table is always generated, no manual calibration here -> it does not contain invalid event ids */
    {
      Dem_UdsStatusByteType       eventStatus = 0x00;
      /* #50 get event status from DEM for current event id */
      Std_ReturnType  retValue = Dem_GetEventUdsStatus(FiM_UnitEvHdl_GetEventId_EventIdPendingTable(evTableIdx), &eventStatus);/* PRQA S 2880 */ /* MD_FiM_ConfCheck */   /* SBSW_FIM_DEMGETEVENTUDSSTATUS_CALL */ /* VCA_FIM_PASSINGLOCALPTR_CALL */
      /* #60 if event status could be obtained (Dem_GetEventUdsStatus returned E_OK) */
      if(retValue == E_OK)
      {
        uint8   modCode = FIM_FID_NOCHANGE;
        /* #70 determine whether FID pending counter has to be incremented */
        if((eventStatus & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
        {
           modCode = FIM_FID_INC;
        }
        /* #80 calculate the pending status FIDs for this event */
        FiM_UnitEvHdl_CalcPendStatusOfEvent(evTableIdx, modCode);
      } /* if(retValue == E_OK) */
      /* #210 otherwise (Dem_GetEventUdsStatus did not return E_OK) */
      else
      {
        /* #220 Dem_GetEventUdsStatus returned an error: ignore event for calculation */
      } /* else if(returnValue == E_OK) */
    }
  } /* for(...; evTableIdx < FiM_GetCountOfEvents_ByTableType(); ...) */

  return;
}
#endif


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FIM_CODE) FiM_InitMemory(void)
{
  FiM_SatelliteInfoTableIterType  satInfoIter = 0;

  FiM_UnitGeneral_PreInitialized    = FALSE;
  FiM_UnitGeneral_MasterInitialized = FALSE;

  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    (void)FiM_UnitGeneral_SetInitializationStatusSat_Checked(FIM_UNINITIALIZED, (FiM_SatelliteIdType)satInfoIter);
  }
  FiM_UnitGeneral_CalcInitSatusSatCumulated();
}


/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_Init
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)  FiM_Init(P2CONST(FiM_ConfigType, AUTOMATIC, FIM_INIT_DATA) FiMConfigPtr) /*lint -e{438} */ /*lint -e{550} */
{
  /* #10 if PB is enabled: do some checks on passed PB config */
#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
  uint8 errorId = FIM_E_INIT_FAILED;                                              /* PRQA S 2981 */ /* MD_FiM_2.2conf */
  /* #20 if FiMConfigPtr is NULL_PTR */
  if (FiMConfigPtr == NULL_PTR)
  {
    /* #25 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_NULLPTR);
  }
  /* #40 otherwise if there is a mismatch in the magic numbers of the PB config and the code */
  else if (FiMConfigPtr->FinalMagicNumberOfPBConfig != FIM_FINAL_MAGIC_NUMBER)
  {
    /* #45 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  /* #50 otherwise if there is a mismatch in compatibility versions of the generators used for PB config and code */
  else if (FiMConfigPtr->GeneratorCompatibilityVersionOfPBConfig != FIM_CFG_GEN_COMPATIBILITY_VERSION)
  {
    /* #55 call EcuM_BswErrorHook */
    EcuM_BswErrorHook((uint16) FIM_MODULE_ID,
                      (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #60 otherwise PB config is fine */
  else
  {
    /* #70 use PB config pointer */
    FiM_ConfigDataPtr = FiMConfigPtr;
    errorId           = FIM_E_NO_ERROR;                                           /* PRQA S 2983 */ /* MD_FiM_2.2conf */
#else
  {
    FIM_DUMMY_STATEMENT(FiMConfigPtr);                                   /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

    /* #100 determine search algorithm for event id list */
    FiM_UnitEvHdl_SetSearchEventIdTableFct(FIM_EVENTID_TABLE);
# if (FIM_FEATURE_OBD == STD_ON)
    /* #110 in the case of OBD: determine search algorithm for event id pending list */
    FiM_UnitEvHdl_SetSearchEventIdTableFct(FIM_EVENTIDPENDING_TABLE);
#endif

    /* #250 set component state to "pre-initialized" */
    FiM_UnitGeneral_PreInitialized = TRUE;
    FiM_UnitGeneral_CalcInitSatusSatCumulated();
  }

#if (FIM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
# if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #300 if error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_INIT, errorId);
  }
# else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
# endif
#endif
}


/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_DemInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_DemInit(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                     /* PRQA S 2981, 2983 */ /* MD_FiM_2.2, MD_FiM_2.2conf */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 check whether there's actually only one satellite configured                               */
  /*     if there's more than one satellite use FiM_DemInitMaster and FiM_DemInitSatellite instead  */
  if (FiM_UnitGeneral_GetNrSatellites() > 1U)                 /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_FiM_ConfCheck */
  {
    errorId = FIM_E_INIT_FAILED;                          /* PRQA S 2880, 2983 */ /* MD_FiM_ConfCheck, MD_FiM_2.2conf */
  }
  else
#endif
  {
    /* #20 call FiM_DemInit<...> for FIM Master and for the first (and only) satellite */
    FiM_DemInitSatellite(FiM_UnitGeneral_GetApplicationId_SatelliteInfoTable(0));/* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    FiM_DemInitMaster();
    /* #30 there is only one satellite and it's initialized -> we can call FiM_PostInit ourselves */
    FiM_PostInit();
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #40 otherwise if error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)                              /* PRQA S 2991, 2992, 2995, 2996 */ /* MD_FiM_ConfCheck */
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,                   /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
                          FIM_SID_FIM_DEMINIT, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
}


/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_DemInitSatellite
 **********************************************************************************************************************/
/*!
 * Note: May only be called from the respective satellite partition.
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_DemInitSatellite(ApplicationType applicationId)  /*lint -e{529} */
{
  uint8                           errorId     = FIM_E_NO_ERROR;                       /* PRQA S 2981 */ /* MD_FiM_2.2 */
  boolean                         found       = FALSE;
  FiM_SatelliteInfoTableIterType  satInfoIter = 0;

  /* #10 identify FiM satellite id from given OS application id */
  for (satInfoIter = 0; satInfoIter < FiM_GetSizeOfSatelliteInfoTable(); satInfoIter++)
  {
    if (FiM_UnitGeneral_GetApplicationId_SatelliteInfoTable(satInfoIter) == applicationId)
    {
      found = TRUE;
      break;
    }
  }

  /* #15 do compatibility check if DEM still uses 0 as dummy id while FiM now switched to INVALID_OSAPPLICATION */
  if ((found == FALSE)                          &&
      ((uint32)applicationId == 0U)             &&
      (FiM_GetSizeOfSatelliteInfoTable() == 1U) &&                  /* PRQA S 2992, 2995, 2996 */ /* MD_FiM_ConfCheck */
      (FiM_UnitGeneral_GetApplicationId_SatelliteInfoTable(0) == INVALID_OSAPPLICATION))
  {
    satInfoIter = 0;                                                            /* PRQA S 2880 */ /* MD_FiM_ConfCheck */
    found       = TRUE;
  }

  /* #20 check if OS application id is known to FiM (otherwise ignore call), see DSGN-FiM23363    */
  /*     possible reasons if id is not found:                                                     */
  /*        - no FiM satellite is configured on the same OS application as calling DEM satellite  */
  /*        - DEM passed an invalid application id                                                */
  if (found == TRUE)
  {
    /* #30 OS application id is known to FiM */
    FiM_SatelliteIdType satId = (FiM_SatelliteIdType)satInfoIter;

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
    /* #40 check whether FIM is at least pre-initialized */
    if (FiM_UnitGeneral_GetInitializationStatusSat(satId) == FIM_UNINITIALIZED)
    {
      errorId = FIM_E_INIT_FAILED;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
    }
    else
#endif
    {
      /* #50 re-calculate inhibition states of FIDs if it is initialized */
      FiM_UnitEvHdl_CalcInhStates(satId);
      /* #60 set component state to "initialized" */
      (void)FiM_UnitGeneral_SetInitializationStatusSat_Checked(FIM_INITIALIZED, satId);
    }
#if (FIM_DEV_ERROR_REPORT == STD_ON)
    /* #80 otherwise if error was detected report DET */
    if (errorId != FIM_E_NO_ERROR)
    {
      (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_DEMINITSATELLITE, errorId);
    }
#else
    FIM_DUMMY_STATEMENT(errorId);                      /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
  }
}


/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_DemInitMaster
 **********************************************************************************************************************/
/*!
 * Note: May only be called from master partition.
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_DemInitMaster(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 check whether FIM Master is at least pre-initialized */
  if (FiM_UnitGeneral_GetInitializationStatusMaster() == FIM_UNINITIALIZED)
  {
    errorId = FIM_E_INIT_FAILED;                                                  /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  else
#endif
  {
    /* #20 initialize FID handling */
    FiM_UnitFidHdl_Init();                                                           /* PRQA S 2987 */ /* MD_FiM_2987 */

#if (FIM_FEATURE_OBD == STD_ON)
    /* #30 if OBD is enabled: re-calculate pending lock states of FIDs if it is initialized */
    FiM_UnitEvHdl_CalcPendStates();
#endif
    /* #40 set master's state to "initialized" */
    FiM_UnitGeneral_MasterInitialized  = TRUE;
  }
#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #50 otherwise if error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_DEMINITMASTER, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
}


/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_PostInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_PostInit(void)  /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

  /* #10 calculate overall initialization status of all satellites        */
  /*     (should result in "initialized" if function was called correctly) */
  FiM_UnitGeneral_CalcInitSatusSatCumulated();

  /* #20 check whether FIM Master and all satellites are initialized */
  if ((FiM_UnitGeneral_GetInitializationStatusMaster()        != FIM_INITIALIZED) ||
      (FiM_UnitGeneral_GetInitializationStatusSatCumulated()  != FIM_INITIALIZED))
  {
    /* by keeping the calls to ...GetInit... we avoid warnings of unused functions when DET is switched off */
#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
    errorId = FIM_E_INIT_FAILED;                                                  /* PRQA S 2983 */ /* MD_FiM_2.2conf */
#endif
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #30 if error reporting is on and error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_POSTINIT, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
}


#if (FIM_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UnitGeneral::FiM_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE)  FiM_GetVersionInfo
                       (P2VAR(Std_VersionInfoType, AUTOMATIC, FIM_APPL_DATA) versioninfo)

{
  uint8 errorId = FIM_E_NO_ERROR;

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  if (versioninfo == NULL_PTR)
  {
    errorId = FIM_E_PARAM_POINTER;
  }
  else
#endif
  {
    versioninfo->vendorID         = FIM_VENDOR_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->moduleID         = FIM_MODULE_ID;                                        /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_major_version = FIM_SW_MAJOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_minor_version = FIM_SW_MINOR_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
    versioninfo->sw_patch_version = FIM_SW_PATCH_VERSION;                                 /* SBSW_FIM_VERSIONINFO_PTR */
  }
#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_GETVERSIONINFO, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
}
#endif /* (FIM_VERSION_INFO_API == STD_ON) */


/***********************************************************************************************************************
 *  external public FiM_UnitFidHdl::FiM_GetFunctionPermission()
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPermission                             /* PRQA S 0624 */ /* MD_FiM_8.3 */
                                 (FiM_FunctionIdType FID,                             /* PRQA S 1330 */ /* MD_FiM_8.3 */
                                  P2VAR(boolean, AUTOMATIC, FIM_APPL_DATA) Permission
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;                                         /* PRQA S 2981 */ /* MD_FiM_2.2 */
  Std_ReturnType    retCode = E_NOT_OK;                                               /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = FID;

  /* #010 if out parameter Permission is NULL_PTR set corresponding DET code */
  if (Permission == NULL_PTR)
  {
    errorId = FIM_E_PARAM_POINTER;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #020 otherwise if FIM satellites are not initialized set corresponding DET code (SPEC-31915) */
  else if (FiM_UnitGeneral_GetInitializationStatusSatCumulated() != FIM_INITIALIZED)
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  /* #030 otherwise if FID is out of range set corresponding DET code (SPEC-31912) */
  else if (fid16 >= FiM_UnitFidHdl_GetNrOfFids())                         /* PRQA S 2992, 2996 */ /* MD_FiM_ConfCheck */
  {
    errorId = FIM_E_FID_OUT_OF_RANGE;                     /* PRQA S 2880, 2983 */ /* MD_FiM_ConfCheck, MD_FiM_2.2conf */
    *Permission = FALSE;                                                                   /* SBSW_FIM_PERMISSION_PTR */
  }
  else
#endif
  /* #040 otherwise return permission status of FID (SPEC-31914) */
  {
#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
    /* #050 if cyclic FID calculation is configured return pre-calculated (in FiM_MainFunction) permission status */
    boolean permStat = FiM_UnitBitfieldHdl_TestBitInBitfield(FiM_GetAddrPermStatusTable(0), FID); /* SBSW_FIM_BITTEST_CALL */
    *Permission = permStat;                                                                /* SBSW_FIM_PERMISSION_PTR */
#else
    /* #060 otherwise calculate permission status from inhibition states */
    boolean inhibStat = FiM_UnitFidHdl_IsFidInhibited(FID);
    *Permission = (boolean)(!inhibStat); /* PRQA S 4116, 4304, 4558 */ /* MD_MSR_AutosarBoolean */ /* SBSW_FIM_PERMISSION_PTR */
#endif
    retCode = E_OK;
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #070 if error reporting is enabled, report DET code if one was set */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_GETFUNCTIONPERMISSION, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

  return retCode;
}


#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UnitFidHdl::FiM_GetFunctionPendingStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FIM_CODE)  FiM_GetFunctionPendingStatus
                                 (FiM_FunctionIdType FID,
                                  P2VAR(boolean, AUTOMATIC, FIM_DEM_DATA) pendingStatus
                                 )                                                                     /*lint -e{529} */
{
  uint8             errorId = FIM_E_NO_ERROR;                   /* PRQA S 2981, 2983 */ /* MD_FiM_2.2, MD_FiM_2.2conf */
  Std_ReturnType    retCode = E_NOT_OK;                                               /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* avoid warning "comparison is always false due to limited range of data type" if there are exactly 256 FIDs       */
  uint16            fid16   = FID;

  /* #10 if out parameter pendingStatus is NULL_PTR set corresponding DET code */
  if (pendingStatus == NULL_PTR)
  {
    errorId = FIM_E_PARAM_POINTER;                                                /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 otherwise if FIM is not initialized set corresponding DET code */
  else if (FiM_UnitGeneral_GetInitializationStatusMaster() != FIM_INITIALIZED)
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #30 otherwise if FID is out of range set corresponding DET code */
  else if (fid16 >= FiM_GetSizeOfFidPendingCounter())                       /* PRQA S 2992, 2996 */ /* MD_FiM_2.2conf */
  {
    errorId = FIM_E_FID_OUT_OF_RANGE;                                       /* PRQA S 2880, 2983 */ /* MD_FiM_2.2conf */
  }
  else
#endif
  /* #40 otherwise return pending status of FID */
  {
    *pendingStatus = FiM_UnitFidHdl_IsFidPendingLocked(FID);                            /* SBSW_FIM_PENDINGSTATUS_PTR */
    retCode = E_OK;
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #50 if error reporting is enabled, report DET code if one was set */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,
                          FIM_SID_FIM_GETFUNCTIONPENDINGSTATUS, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

  return retCode;
}
#endif

#if (FIM_FEATURE_OBD == STD_ON)
/***********************************************************************************************************************
 *  external public FiM_UnitEvHdl::FiM_DemTriggerOnEventStatus
 **********************************************************************************************************************/
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
 *
 *
 *
 *
*/
FUNC(void, FIM_CODE)  FiM_DemTriggerOnEventStatus
                       (Dem_EventIdType EventId,
                        Dem_UdsStatusByteType EventStatusOld,
                        Dem_UdsStatusByteType EventStatusNew
                       )                                                                               /*lint -e{529} */
{
  uint8 errorId = FIM_E_NO_ERROR;                                                     /* PRQA S 2981 */ /* MD_FiM_2.2 */
  
#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 if FIM is not initialized skip rest of function */
  /* SlientBSW: check is silence relevant */
  if (FiM_UnitGeneral_GetInitializationStatusMaster() != FIM_INITIALIZED)
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 otherwise */
  else
#endif
  {
    boolean                   pendingStatusNew = FALSE;
    boolean                   pendingStatusOld = FALSE;
    uint8                     modCode          = FIM_FID_DEC;
    /* #30 if passed event id is not FIM_DEM_EVENT_INVALID */
    if (EventId != FIM_DEM_EVENT_INVALID)
    {
      if((EventStatusNew & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
      {
        pendingStatusNew = TRUE;
        modCode          = FIM_FID_INC;
      }
      if((EventStatusOld & DEM_UDS_STATUS_PDTC) == DEM_UDS_STATUS_PDTC)
      {
        pendingStatusOld = TRUE;
      }
    }

    /* #40 if the pending status bit changed (we're only interested in changes of this status bit)  */
    /* for FIM_DEM_EVENT_INVALID pendingStatusNew and pendingStatusOld are both FALSE               */
    if (pendingStatusNew != pendingStatusOld)
    {
      FiM_EventIdTableIndexType evTableIdx  = 0;
      /* #50 search for event (i.e. is event known/configured in FIM?) */
      /* Silent:
          SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR:
                    - pointer to local stack variale of type FiM_EventIdTableIndexType
          SBSW_FIM_SEARCHPENDINGIDFUNC_PTR, VCA_FIM_SEARCHPENDINGIDFUNC_PTR:
                    - initialization status is checked above
                    - in FiM_Init function pointer is always set when FIM_FEATURE_OBD == STD_ON 
                    - this function (FiM_DemTriggerOnEventStatus) is only present when FIM_FEATURE_OBD == STD_ON */
      boolean evFound = (*FiM_UnitEvHdl_SearchEventIdPendingTableFct) /* SBSW_FIM_SEARCHPENDINGIDFUNC_PTR */ /* VCA_FIM_SEARCHPENDINGIDFUNC_PTR */
                         (EventId, &evTableIdx, FIM_EVENTIDPENDING_TABLE);
      /* #60 if event was found */
      if (evFound == TRUE)
      {
        /* #80 calculate the pending status FIDs for this event */
        /*     (does not depend on inhibition cfg, just linked to dummy inhibition cfg) */
        FiM_UnitEvHdl_CalcPendStatusOfEvent(evTableIdx, modCode);
      } /* if (evFound == TRUE) */
    } /* if (pendingStatusNew != pendingStatusOld) */
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,
                          FIM_SID_FIM_DEMTRIGGERONEVENTSTATUS, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId); /*lint -e{438} */                        /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

  return;
}
#endif


/***********************************************************************************************************************
 *  external public FiM_UnitEvHdl::FiM_DemTriggerOnMonitorStatus
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
*/
FUNC(void, FIM_CODE)  FiM_DemTriggerOnMonitorStatus
                       (Dem_EventIdType EventId
                       )                                                                               /*lint -e{529} */
{
  uint8           errorId = FIM_E_NO_ERROR;                                           /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #10 assert that FIM is at least pre-initialized otherwise search functions are not yet determined */
  /* SilentBSW: check is silence relevant */
  if (FiM_UnitGeneral_GetInitializationStatusSatCumulated() == FIM_UNINITIALIZED)
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  /* #20 otherwise */
  else
#endif
  {
    boolean                   evFound     = FALSE;
    FiM_EventIdTableIndexType evTableIdx  = 0;
    /* #30 if passed event id is not FIM_DEM_EVENT_INVALID */
    if (EventId != FIM_DEM_EVENT_INVALID)
    {
      /* #40 search for event (i.e. is event known/configured in FIM?) */
      /* Silent:
          SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR:
                    - pointer to local stack variale of type FiM_EventIdTableIndexType
          SBSW_FIM_SEARCHIDFUNC_PTR:
                    - initialization status is checked above
                    - in FiM_Init function pointer is always set */
      evFound = (*FiM_UnitEvHdl_SearchEventIdTableFct)    /* SBSW_FIM_SEARCHIDFUNC_PTR */ /* VCA_FIM_SEARCHIDFUNC_PTR */
                         (EventId, &evTableIdx, FIM_EVENTID_TABLE);
    }
    /* #50 if event was found */
    if (evFound == TRUE) /* not invalid and found */
    {
#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
      /* #60 now we need to check if this FIM satellite is initialized skip rest of function (SPEC-31918) */
      FiM_SatelliteIdType satId = FiM_UnitEvHdl_GetSatelliteId_EventIdTable(evTableIdx);
      if (FiM_UnitGeneral_GetInitializationStatusSat(satId) != FIM_INITIALIZED)
      {
        errorId = FIM_E_UNINIT;                                                   /* PRQA S 2983 */ /* MD_FiM_2.2conf */
      }
      /* #70 otherwise */
      else
#endif
      {
        /* #90 calculate the inhibition states of this event */
        FiM_UnitEvHdl_CalcInhibStatusOfEvent(evTableIdx);
      }
    } /* if (evFound == TRUE) */
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET,
                          FIM_SID_FIM_DEMTRIGGERONMONITORSTATUS, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId); /*lint -e{438} */                        /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

  return;
}


/***********************************************************************************************************************
  *  external public FiM_UnitGeneral::FiM_MainFunction()
  **********************************************************************************************************************/
/*!
 * Note:  May only be called from the master partition.
 *        While accessing it from a different partition may result in a memory protection exception, that's not relevant
 *        for silence because function only modifies its own data. Therefore no check for current application is done.
 * Note:  May only be called from master partition.
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FIM_CODE) FiM_MainFunction(void)
{
#if (FIM_CYCLIC_FID_CALCULATION == STD_ON)
  /* #010 if cyclic FID calculation is configured */
  uint8               errorId = FIM_E_NO_ERROR;                                       /* PRQA S 2981 */ /* MD_FiM_2.2 */

#if (FIM_DEV_ERROR_DETECT == STD_ON)                                                         /* COV_FIM_DET_DETECT TX */
  /* #020 if FIM master or satellites are not initialized set corresponding DET code */
  if ((FiM_UnitGeneral_GetInitializationStatusMaster()        != FIM_INITIALIZED) ||
      (FiM_UnitGeneral_GetInitializationStatusSatCumulated()  != FIM_INITIALIZED))
  {
    errorId = FIM_E_UNINIT;                                                       /* PRQA S 2983 */ /* MD_FiM_2.2conf */
  }
  else
#endif
  /* #030 otherwise calculate permission status of FIDs */
  {
    FiM_UnitFidHdl_CalcPermissionStates();
  }

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* #100 if error reporting is enabled, report DET code if one was set */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, FIM_SID_FIM_MAINFUNCTION, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(errorId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif

  return;
#endif
}


/***********************************************************************************************************************
*  public FiM_UnitInhStatHdl_GetInhibitionStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_UnitInhStatHdl_GetInhibitionStatus(uint16 inhCodeIdx,
                                                                                FiM_SatelliteIdType satId)
{
  /* caller should ensure that satId < FiM_GetSizeOfSatelliteInfoTable()                                              */
  /* no write access is done later on relying on that fact -> not silence critical -> no runtime check necessary      */
  /* - #10 get base pointer to inhibition status table corrseponding to satellite                                     */
  const FiM_ProcessorWordType *inhStatusBasePtr = FiM_UnitInhStatHdl_GetInhStatusBasePtr_SatelliteInfoTable(satId);

  /* #20 return status bit                                                                                            */
  return FiM_UnitBitfieldHdl_TestBitInBitfield(inhStatusBasePtr, inhCodeIdx);                /* SBSW_FIM_BITTEST_CALL */
}



#define FIM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* module specific MISRA deviations:
   MD_FiM_2.2:
     Description: Rule 2.2 (2012)
                  There shall be no dead code.
     Reason:      Necessary for API pattern.
     Risk:        None.
     Prevention:  None.

   MD_FiM_2.2conf:
     Description: Rule 2.2 (2012)
                  This initialization is redundant. The value of this object is never used before being modified.
                  This assignment is redundant. The value of this object is never subsequently used.
                  Static function '%s()' is not used within this translation unit.
     Reason:      Dependent on configuration, code parts that modify or use the value may be switched off.
     Risk:        Untested code that's activated by a different configuration.
     Prevention:  Test runs with different configurations so that the expression is evaluated to all possible results.
                  Guaranteed by test coverage analysis.

   MD_FiM_2981_GetNextSetBit:
     Description: Rule 2.2
                  This initialization is redundant. The value of this object is never used before being modified.
     Reason:      This is a false positive. Depending on the return value of a called function, the initialization value
                  may be returned or not.
     Risk:        None.
     Prevention:  None.

   MD_FiM_2987:
     Description: Rule 2.2
                  This function call produces no side effects and is redundant.
     Reason:      Dependent on configuration, the function contains no code.
     Risk:        None.
     Prevention:  None.

   MD_FiM_8.9:
     Description: Rule 8.9 (2012)
                  An object should be defined at block scope if its identifier only appears in a single function.
     Reason:      Vector style guide prevents usage of static variables in function scope and partly
                  object is used in function parts that are not active in certain configurations.
     Risk:        None.
     Prevention:  None.

   MD_FiM_8.3:
     Description: Rule 8.3 (2012)
                  All declarations of an object or function shall use the same names and type qualifiers.
     Reason:      Conflicting prototype is generated by RTE. Currently there is no way to tell RTE the parameter name.
     Risk:        None.
     Prevention:  None.

   MD_FiM_18.1:
     Description: Rule 18.1 (2012)
                  A pointer resulting from arithmetic on a pointer operand shall address an element of the same array
                  as that pointer operand.
     Reason:      Caller must ensure that array contains at least 2 elements.
     Risk:        Caller does not obey this rule.
     Prevention:  Precondition is documented at function declaration. Review is performed.

   MD_FiM_ConfCheck:
     Description: Check always evaluates to same result. This results in MISRA warnings, e.g.
                  - The value of this 'if' controlling expression is always 'false'.
                  - The result of this logical operation is always 'false'.
                  - This code is unreachable.
                  - Controlling expressions shall not be invariant.
     Reason:      Since the configuration is constant during one test run, the expression always results to the same
                  result.
     Risk:        Untested code that's activated by a different configuration.
     Prevention:  Test runs with different configurations so that the expression is evaluated to all possible results.
                  Guaranteed by test coverage analysis.
*/


/* COV_JUSTIFICATION_BEGIN
\ID COV_FIM_DET_DETECT
  \ACCEPT TX
  \REASON For SafeBSW Error Detection is always switched on.

\ID COV_FIM_RUNTIME_CHECKS
  \ACCEPT TX
  \REASON For SafeBSW Runtime Checks are always switched on.

COV_JUSTIFICATION_END */ 


/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_FIM_COMPSWAPFUNC_ADDRESS_PTR
    \DESCRIPTION    Pointer write access to parameter addressPtr.
    \COUNTERMEASURE \N Compare and Swap function is internal and caller makes sure that a
                       valid pointer 'addressPtr' is passed.

\ID SBSW_FIM_BITINDEX_ADDRESS_PTR
    \DESCRIPTION    Pointer write access to parameter bitIndexPtr.
    \COUNTERMEASURE \N Function is internal and caller makes sure that a valid pointer 'bitIndexPtr' is passed.

\ID SBSW_FIM_BITMANIP_COMPSWAPFUNC_CALL
    \DESCRIPTION    Call of CompareAndSwap with pointer parameter.
    \COUNTERMEASURE \R A Runtime check ensures that the passed pointer is within array bounds.
                       Bit manipulation functions are internal and caller makes sure that a
                       valid pointer to word array is passed and that 'bitfieldWordArrSize'
                       does not exceed length of this word array.

\ID SBSW_FIM_GETNEXTSETBIT_CALL
    \DESCRIPTION Call of FiM_UnitBitfieldHdl_GetNextSetBit/..._GetNextSetBitInWord with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of correct type uint32/uint8 is passed.

\ID SBSW_FIM_PTR_FWD_ARG
    \DESCRIPTION    A pointer parameter is received as argument and forwarded to another function expecting a valid
                    pointer. The called function does not store its pointer parameters.
    \COUNTERMEASURE \N The pointer is used unmodified, as such it is still valid.


\ID SBSW_FIM_EVCHANGED_ACCESS
    \DESCRIPTION    Access of EventChangedTable using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_PERMSTATUS_ACCESS
    \DESCRIPTION    Access of PermStatusTable using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_BITMANIP_CALL
    \DESCRIPTION    Call of bit manipulation function with pointer to array and length of array as parameters.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_BITTEST_CALL
    \DESCRIPTION    Call of bit test function with pointer to array.
    \COUNTERMEASURE \N Bit test function is internal and pointer is only used for read access.

\ID SBSW_FIM_SEARCHIDFUNC_EVTABLEIDX_PTR
    \DESCRIPTION    Pointer write access to out parameter evTableIdx.
    \COUNTERMEASURE \N Search functions are internal and pointer passed by caller always points to a local (stack)
                       variable. See also SBSW_FIM_SEARCHIDFUNC_PTR.

\ID SBSW_FIM_SEARCHIDFUNC_PTR
    \DESCRIPTION Call of function via function pointer.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function.
            
\ID SBSW_FIM_SEARCHPENDINGIDFUNC_PTR
    \DESCRIPTION Call of function via function pointer.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed when FIM_FEATURE_OBD == STD_ON.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function when FIM_FEATURE_OBD == STD_ON.
                       Finally, the function containing the pointer access is only present
                       when FIM_FEATURE_OBD == STD_ON

\ID SBSW_FIM_DEMGETMONST_CALL
    \DESCRIPTION Call of Dem_GetMonitorStatus with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of type Dem_MonitorStatusType is passed.

\ID SBSW_FIM_DEMGETEVENTUDSSTATUS_CALL
    \DESCRIPTION Call of Dem_GetEventUdsStatus with pointer parameter.
    \COUNTERMEASURE \N Pointer to local variable of type Dem_UdsStatusByteType is passed.

\ID SBSW_FIM_ACCESS_INITSTATUSSAT_CHECKED
    \DESCRIPTION    Access to Initialization Status variable via pointer in satellite info table.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer is taken from table.
                    Countermeasure SMI-159210 for ensuring that this pointer points to a valid address.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
                    Countermeasure SMI-438945 together with memory mapping rules in Technical Reference ensures that
                    initialization status bits of satellites are not overwritten by non-trusted parts of software.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159210
                    \S SMI-232888
                    \S SMI-438945

\ID SBSW_FIM_BITMANIP_INHSTATUSSAT_CALL
    \DESCRIPTION    Call of bit manipulation function with pointer to array and length of array as parameters.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer and length are taken
                    from table.
                    Countermeasure SMI-159209 for ensuring that this pointer points to a valid address and
                    the length is the length of this array.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159209
                    \S SMI-232888

\ID SBSW_FIM_ACCESS_INHSTATUSSAT
    \DESCRIPTION    Access to Inhibition Status Array via pointer in satellite info table.
                    Countermeasure CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS for ensuring that pointer and length are taken
                    from table.
                    Countermeasure SMI-159209 for ensuring that this pointer points to the start of an array and
                    the length is the length of this array.
                    Countermeasure SMI-232888 for ensuring that satellite does not overwrite another satellite's data:
                    The information in one line of FiM_SatelliteInfoTable belongs to the same satellite and all lines
                    in the table belong to different satellites.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]
                    \S SMI-159209
                    \S SMI-232888

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CHECKED
    \DESCRIPTION    Access of FimFidPendingCounter using ComStackLib.
    \COUNTERMEASURE \M [CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS]

\ID SBSW_FIM_ACCESS_FIDPENDINGCOUNTER_CALCINHPENDSTATES
    \DESCRIPTION    Access of FimFidPendingCounter using ComStackLib.
    \COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_FIM_VERSIONINFO_PTR
    \DESCRIPTION Pointer write access to versioninfo pointer variable in FiM_GetVersionInfo().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to the parameter
                       versioninfo is passed.

\ID SBSW_FIM_PERMISSION_PTR
    \DESCRIPTION Pointer write access to permission pointer variable in FiM_GetFunctionPermission().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

\ID SBSW_FIM_PENDINGSTATUS_PTR
    \DESCRIPTION Pointer write access to pending status pointer variable in FiM_GetFunctionPendingStatus().
    \COUNTERMEASURE \N The application which uses this API has to make sure that a valid pointer to a variable
                       that can hold a boolean type is passed.

SBSW_JUSTIFICATION_END */


/* VCA_JUSTIFICATION_BEGIN

\ID VCA_FIM_SEARCHIDFUNC_PTR
    \DESCRIPTION Possibly calling null pointer. Failed to determine points-to set.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function.

\ID VCA_FIM_SEARCHPENDINGIDFUNC_PTR
    \DESCRIPTION Possibly calling null pointer. Failed to determine points-to set.
    \COUNTERMEASURE \M Verify that FIM_DEV_ERROR_DETECT == STD_ON (CheckQMDefines).
                       Then function checks whether initialization has been performed when FIM_FEATURE_OBD == STD_ON.
                       Execution of FiM_Init (pre-initialization of FiM) ensures that function pointer
                       is always set to a valid function when FIM_FEATURE_OBD == STD_ON.
                       Finally, the function containing the pointer access is only present
                       when FIM_FEATURE_OBD == STD_ON

\ID VCA_FIM_PASSINGLOCALPTR_CALL
  \DESCRIPTION  Calling undefined function ...
                A function with pointer parameters is directly called, but the function is not
                defined within the analyzed sources. VCA is unable to determine the
                behavior of the function.
  \COUNTERMEASURE \N  Pointer to local variable of appropriate type is passed.
                      The Compiler ensures the correct type of the pointer.
                      The called function uses the pointer only while being called.

\ID VCA_FIM_PTR_FWD_ARG
  \DESCRIPTION  Calling undefined function ...
                A function with pointer parameters is directly called, but the function is not
                defined within the analyzed sources. VCA is unable to determine the
                behavior of the function.
                A pointer parameter is received as argument and forwarded to another function expecting a valid
                pointer. The called function does not store its pointer parameters.
  \COUNTERMEASURE \N  The pointer is used unmodified, as such it is still valid.
                      The Compiler ensures the correct type of the pointer.
                      The called function uses the pointer only while being called.

VCA_JUSTIFICATION_END */


/*

\CM CM_FIM_CSL01_ENABLE_RUNTIME_CHECKS
       Verify that FIM_DEV_RUNTIME_CHECKS == STD_ON (CheckQMDefines).
       Then it's Qualified use case CSL01 of ComStackLib.

*/

/***********************************************************************************************************************
*  END OF FILE: FiM.c
***********************************************************************************************************************/

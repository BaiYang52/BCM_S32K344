/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_MemoryProtection  HAL Memory Protection
 *   \{
 *
 *  \file       Os_Hal_MemoryProtectionInt.h
 *  \brief      Internal functions and data types related to memory protection.
 *  \note       Consider Memory barriers and sync instructions.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_MEMORYPROTECTIONINT_H
# define OS_HAL_MEMORYPROTECTIONINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_Context_Types.h"
# include "Os_Hal_MemoryProtection_Types.h"

/* Os kernel module dependencies */
# include "Os_CommonInt.h"

/* Os Hal dependencies */
# include "Os_Hal_CompilerInt.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! \brief    System wide memory protection configuration.
 *  \details
 *  The kernel uses this data type to initialize access rights for different bus masters (cores, DMAs, etc.).
 *  These are typically MPU, MMU or PPU settings such as region descriptors.
 */
struct Os_Hal_MpSystemConfigType_Tag
{
  void* MpuBase;
  uint32 MpuRegionCount;
  void* MpuRegions;
};


/*! \brief    Memory protection configuration of a core.
 *  \details  The kernel uses this data type to initialize access rights of a core.
 *            These are typically MPU, MMU or PPU settings such as region descriptors.
 */
struct Os_Hal_MpCoreConfigType_Tag
{
  void* MpuBase;
  uint32 MpuRegionCount;
  void* MpuRegions;
};


/*! \brief    Memory protection configuration of an OS application.
 *  \details  The kernel uses this data type to switch access rights, if the active application changes.
 *            These are typically MPU, MMU or PPU settings such as region descriptors or an ASID.
 */
struct Os_Hal_MpAppConfigType_Tag
{
  uint32 Asid;

  void* MpuBase;
  uint32 MpuRegionCount;
  void* MpuRegions;
};


/*! \brief    Memory protection configuration of an OS thread.
 *  \details  The kernel uses this data type to switch access rights, if the active thread changes.
 *            These are typically MPU, MMU or PPU settings such as region descriptors or an ASID.
 */
struct Os_Hal_MpThreadConfigType_Tag
{
  uint32 Asid;

  void* MpuBase;
  uint32 MpuRegionCount;
  void* MpuRegions;
};



/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  Os_Hal_MpSystemInit()
 **********************************************************************************************************************/
/*! \brief          Initialize the system memory protection.
 *  \details        Initializes system's memory protection mechanism and set the passed memory protection settings for
 *                  the system. This function is called once on the master core during OS initialization.
 *                  It typically initializes and enables any system related protection mechanism (system MPU/MMU/PPU).
 *
 *  \param[in]      SystemConfig  Configuration for system's memory protection mechanism.
 *                                Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *  \pre            Executed on master core.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_MpSystemInit,
(
  P2CONST(Os_Hal_MpSystemConfigType, TYPEDEF, OS_CONST) SystemConfig
));


/***********************************************************************************************************************
 *  Os_Hal_MpCoreInit()
 **********************************************************************************************************************/
/*! \brief          Initialize the core memory protection.
 *  \details        Initializes the core's memory protection mechanism and set the passed memory protection settings
 *                  for the core. This function is called once on each core during OS initialization.
 *                  It typically initializes and enables any core related protection mechanism (system MPU/MMU/PPU).
 *
 *  \param[in]      CoreConfig          Core configuration for memory protection mechanism. Parameter must not be NULL.
 *
 *  \param[in]      InitialStackRegion  Stack configuration for memory protection mechanism. Used to set the initially
 *                                      open stack window. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_MpCoreInit,
(
  P2CONST(Os_Hal_MpCoreConfigType, TYPEDEF, OS_CONST) CoreConfig,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
));


/***********************************************************************************************************************
 *  Os_Hal_MpAppSwitch()
 **********************************************************************************************************************/
/*! \brief          Switches application related access rights.
 *  \details        Switches the current memory protection settings to the passed ones.
 *
 *  \param[in]      AppConfig   Configuration for application's memory protection mechanism.
 *                              Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_MpAppSwitch,
(
  P2CONST(Os_Hal_MpAppConfigType, TYPEDEF, OS_CONST) AppConfig
));


/***********************************************************************************************************************
 *  Os_Hal_MpThreadSwitch()
 **********************************************************************************************************************/
/*! \brief          Switches thread related access rights.
 *  \details        Switches the current memory protection settings to the passed ones.
 *
 *  \param[in]      ThreadConfig   Configuration for thread's memory protection mechanism.
 *                                 Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *  \pre            Supervisor mode.
 *  \pre            Application rights are switched (see \ref Os_Hal_MpAppSwitch()).
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_MpThreadSwitch,
(
  P2CONST(Os_Hal_MpThreadConfigType, TYPEDEF, OS_CONST) ThreadConfig
));


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_MEMORYPROTECTIONINT_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtectionInt.h
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup Os_Hal_MemoryProtection
 *  \{
 *
 *  \file
 *  \brief      Memory protection primitives for the Protected Memory System Architecture (PMSA) used by ARMv8-M.
 *  \details
 *  Internal comment removed.
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
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_MEMORYPROTECTION_PMSAV8M_H
# define OS_HAL_MEMORYPROTECTION_PMSAV8M_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Common.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_Hal_MpEnableMPU()
 **********************************************************************************************************************/
/*! \brief        Enables the MPU.
 *  \details      --no details--
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpEnableMPU, (void));


/***********************************************************************************************************************
 *  Os_Hal_MpDisableMPU()
 **********************************************************************************************************************/
/*! \brief        Disables the MPU.
 *  \details      --no details--
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpDisableMPU, (void));


/***********************************************************************************************************************
 *  Os_Hal_MpDisableAllRegions()
 **********************************************************************************************************************/
/*! \brief        Disables all available MPU regions.
 *  \details      --no details--
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpDisableAllRegions, (void));


/***********************************************************************************************************************
 *  Os_Hal_MpWriteRegionDescriptor()
 **********************************************************************************************************************/
/*! \brief        Writes given MPU region configuration into peripheral.
 *  \details      --no details--
 *  \param[in]    Region  MPU region configuration which shall be written. Parameter must not be NULL.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpWriteRegionDescriptor,
(
  P2CONST(Os_Hal_MpRegionType, TYPEDEF, OS_CONST) Region
));


/***********************************************************************************************************************
 *  Os_Hal_MpSetStackRegion()
 **********************************************************************************************************************/
/*! \brief          Set stack region to given values.
 *  \details        -
 *  \param[in]      StackConfig    Pointer to stack window configuration
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Interrupts disabled.
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpSetStackRegion,
(
  P2CONST(Os_Hal_ContextStackConfigType, TYPEDEF, OS_CONST) StackConfig
));


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_MpEnableMPU()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpEnableMPU, (void))
{
  /* #10 Get MPU_CTRL. */
  uint32 mpuControl = OS_HAL_MP_MPU_BASE->Ctrl;                                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  mpuControl |= OS_HAL_MP_MPU_CTRL_ENABLE | OS_HAL_MP_MPU_CTRL_PRIVDEFENA;

  /* #20 Data Sync Barrier. */
  Os_Hal_DataSyncBarrier();

  /* #30 Enable the MPU and activate the background region in the priveleged mode. */
  OS_HAL_MP_MPU_BASE->Ctrl = mpuControl;                                                                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_CTRL */

  /* #40 Instruction Sync Barrier. */
  Os_Hal_InstructionSyncBarrier();
}


/***********************************************************************************************************************
 *  Os_Hal_MpDisableMPU()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpDisableMPU, (void))
{
  /* #10 Get MPU_CTRL. */
  uint32 mpuControl = OS_HAL_MP_MPU_BASE->Ctrl;                                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */
  mpuControl &= ~OS_HAL_MP_MPU_CTRL_ENABLE;

  /* #20 Data Sync Barrier. */
  Os_Hal_DataSyncBarrier();

  /* #30 Clear MPU_CTRL.ENABLE bit. */
  OS_HAL_MP_MPU_BASE->Ctrl = mpuControl;                                                                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_CTRL */

  /* #40 Instruction Sync Barrier. */
  Os_Hal_InstructionSyncBarrier();
}


/***********************************************************************************************************************
 *  Os_Hal_MpDisableAllRegions()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpDisableAllRegions, (void))
{
  uint32 index;
  const uint8_least NumberOfRegions = ((OS_HAL_MP_MPU_BASE->Type) >> 8u) & 0xFFuL;                                      /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  /* #10 For all available MPU regions  */
  for(index = 3; index < NumberOfRegions; index++)                                                                      /* FETA_OS_HAL_MPU */
  {
    /* #20 Disable the region. */
    OS_HAL_MP_MPU_BASE->Rnr = (index & 0xFFuL);                                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RNR */
    OS_HAL_MP_MPU_BASE->Rlar &= ~OS_HAL_MP_REGION_SIZE_ENABLE;                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */
  }
}


/***********************************************************************************************************************
 *  Os_Hal_MpWriteRegionDescriptor()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpWriteRegionDescriptor,
(
  P2CONST(Os_Hal_MpRegionType, TYPEDEF, OS_CONST) Region
))
{
  /* #10 Select the MPU Region for reprogramming. */
  OS_HAL_MP_MPU_BASE->Rnr = (Region->RegionNumber) & 0xFFuL;                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RNR */

  /* #20 Disable the MPU region for the duration of reprogramming. */
  OS_HAL_MP_MPU_BASE->Rlar &= ~OS_HAL_MP_REGION_SIZE_ENABLE;                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */

  /* #30 Write given configuration to the MPU region and enable it again. */
  OS_HAL_MP_MPU_BASE->Rbar = Region->RegionBaseAddress | Region-> RegionAccessControl;                                  /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RBAR */
  OS_HAL_MP_MPU_BASE->Rlar = (OS_HAL_MP_REGION_BASE_ALIGN(Region->RegionEndAddress) | Region->RegionEnableAndFlags);    /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */
}


/***********************************************************************************************************************
 *  Os_Hal_MpSetStackRegion()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpSetStackRegion,
(
  P2CONST(Os_Hal_ContextStackConfigType, TYPEDEF, OS_CONST) StackConfig
))
{
  /* #10 If memory protection is active: */
  if(Os_MpIsEnabled() != 0u)                                                                                            /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    /* #20 Select the MPU Region for reprogramming. */
    OS_HAL_MP_MPU_BASE->Rnr = OS_HAL_MP_STACK_REGION_NUMBER;                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RNR */

    /* #30 Disable the MPU region for the duration of reprogramming. */
    OS_HAL_MP_MPU_BASE->Rlar &= ~OS_HAL_MP_REGION_SIZE_ENABLE;                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */

    /* #40 Write stack base address to the MPU region. */
    OS_HAL_MP_MPU_BASE->Rbar = ((StackConfig->StackRegionStart) |                                                       /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RBAR */
                                OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_RW_U_RW);

    /* #50 Set region attributes, end address and enable it again. */
    OS_HAL_MP_MPU_BASE->Rlar = OS_HAL_MP_REGION_BASE_ALIGN(StackConfig->StackRegionEnd);                                /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */
    OS_HAL_MP_MPU_BASE->Rlar |= OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE;            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */
    OS_HAL_MP_MPU_BASE->Rlar |= OS_HAL_MP_REGION_SIZE_ENABLE;                                                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RASR */

    /* #60 Instruction Sync Barrier. */
    Os_Hal_InstructionSyncBarrier();
  }
}


/***********************************************************************************************************************
 *  Os_Hal_MpGetStackRegion()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_MpGetStackRegion,
(
  P2VAR(Os_Hal_ContextStackConfigType, TYPEDEF, OS_VAR_NOINIT) StackConfig
))
{
  /* #10 If memory protection is active: */
  if(Os_MpIsEnabled() != 0u)                                                                                            /* PRQA S 2991, 2995 */ /* MD_Os_Rule14.3_2991, MD_Os_Rule2.2_2995 */
  {
    /* #20 Select the MPU Region for reading. */
    OS_HAL_MP_MPU_BASE->Rnr = OS_HAL_MP_STACK_REGION_NUMBER;                                                            /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_MP_MPU_RNR */

    /* #30 Read base address and end address of the stack region. */
    StackConfig->StackRegionStart = (OS_HAL_MP_MPU_BASE->Rbar) & (OS_HAL_MP_REGION_SIZE_ALIGN);                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
    StackConfig->StackRegionEnd = (OS_HAL_MP_MPU_BASE->Rlar) & (OS_HAL_MP_REGION_SIZE_ALIGN);                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
  }
}

/***********************************************************************************************************************
 *  Os_Hal_MpSystemInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_MpSystemInit,
(
  P2CONST(Os_Hal_MpSystemConfigType, TYPEDEF, OS_CONST) SystemConfig
))
{
  /* #10 No system MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(SystemConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}


/***********************************************************************************************************************
 *  Os_Hal_MpCoreInit()
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
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpCoreInit,
(
  P2CONST(Os_Hal_MpCoreConfigType , TYPEDEF, OS_CONST) CoreConfig,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
))
{
  uint32 index;
  uint32 interruptLevel;

  /* #10 Disable interrupts and store prior state. */
  interruptLevel = Os_Hal_DisableAndReturnPriorState();

  /* #20 Disable the MPU. */
  Os_Hal_MpDisableMPU();

  /* #30 Initialize memory attribute indirection registers 0 and 1. */
  Os_Hal_InstructionSyncBarrier();
  OS_HAL_MP_MPU_BASE->Mair0 = OS_HAL_MP_MAIR0_INIT_VALUE;                                                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
  OS_HAL_MP_MPU_BASE->Mair1 = OS_HAL_MP_MAIR1_INIT_VALUE;                                                              /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
  Os_Hal_DataSyncBarrier();
  Os_Hal_InstructionSyncBarrier();

  /* #40 Disable all MPU regions. */
  Os_Hal_MpDisableAllRegions();

  /* #50 For each configured MPU region: */
  for(index = 0; index < CoreConfig->MpuRegionCount; index++)                                                           /* FETA_OS_HAL_MPU */
  {
    /* #60 Write configuration to the MPU. */
    Os_Hal_MpWriteRegionDescriptor(&(CoreConfig->MpuRegions[index]));                                                   /* SBSW_OS_HAL_FC_MPUCONFIG */
  }

  /* #70 Set stack region to to the passed values. */
  Os_Hal_MpSetStackRegion(InitialStackRegion);                                                                          /* SBSW_OS_HAL_FC_MPUCONFIG */

  /* #80 Enable the MPU. */
  Os_Hal_MpEnableMPU();

  /* #90 Restore prior interrupt state. */
  Os_Hal_WriteBasepri(interruptLevel);
}


/***********************************************************************************************************************
 *  Os_Hal_MpAppSwitch()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpAppSwitch,
(
  P2CONST(Os_Hal_MpAppConfigType, TYPEDEF, OS_CONST) AppConfig
))
{
  uint32 index;

  /* #10 For each configured MPU region: */
  for(index = 0; index < AppConfig->MpuRegionCount; index++)                                                            /* FETA_OS_HAL_MPU */
  {
    /* #20 Write the region configuration into the MPU. */
    Os_Hal_MpWriteRegionDescriptor(&(AppConfig->MpuRegions[index]));                                                    /* SBSW_OS_HAL_FC_MPUCONFIG */
  }
}


/***********************************************************************************************************************
 *  Os_Hal_MpThreadSwitch()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpThreadSwitch,
(
  P2CONST(Os_Hal_MpThreadConfigType, TYPEDEF, OS_CONST) ThreadConfig
))
{
  uint32 index;

  /* #10 For each configured MPU region: */
  for(index = 0; index < ThreadConfig->MpuRegionCount; index++)                                                         /* FETA_OS_HAL_MPU */
  {
    /* #20 Write the region configuration into the MPU. */
    Os_Hal_MpWriteRegionDescriptor(&(ThreadConfig->MpuRegions[index]));                                                 /* SBSW_OS_HAL_FC_MPUCONFIG */
  }
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* OS_HAL_MEMORYPROTECTION_PMSAV8M_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtection_PMSAv8m.h
 *********************************************************************************************************************/

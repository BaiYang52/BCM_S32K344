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
 *  \brief      Memory protection primitives for the NXP System MPU.
 *  \details
 *  The SMPU module provides macros and functions to access and configure the on chip System Memory Protection Unit.
 *  Therefore it provides interfaces to enable and disable protection and to configure appropriate region descriptors.
 *  The region descriptors consist of region start and end address as well as the regions access rights.
 *
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
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_MEMORYPROTECTION_NXP_H_
# define OS_HAL_MEMORYPROTECTION_NXP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Common.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! \brief Function macro to access the region word 0 of the specified region id
 *  \details
 *  Region Word 0 defines the byte start address of the memory region (16 byte aligned - last 4 bits are zero)
 *  Writes to this register clear the region descriptor valid bit
*/
# define OS_HAL_SMPU_RGD_WORD_0_OFFSET(rgdId)   (OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_0_OFFSET + ((rgdId) * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH))
/*! \brief Function macro to access the region word 1 of the specified region id
 *  \details
 *  Region Word 1 defines the byte end address of the memory region (16 byte aligned - last 4 bits are one)
 *  Writes to this register clear the region descriptor valid bit
*/
# define OS_HAL_SMPU_RGD_WORD_1_OFFSET(rgdId)   (OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_1_OFFSET + ((rgdId) * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH))
/*! Function macro to access the region word 2 of the specified region id */
# define OS_HAL_SMPU_RGD_WORD_2_OFFSET(rgdId)   (OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_2_OFFSET + ((rgdId) * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH))
/*! Function macro to access the region word 3 of the specified region id */
# define OS_HAL_SMPU_RGD_WORD_3_OFFSET(rgdId)   (OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_3_OFFSET + ((rgdId) * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH))

/*! Function macro to access the region alternate access configuration of the specified region id */
# define OS_HAL_SMPU_RGD_AAC_OFFSET(rgdId)      (OS_HAL_SMPU_RGDAAC_DESCRIPTOR_OFFSET + ((rgdId) * 4uL))

/*! Function macro to calculate the value for region word 2
 *  \details
 *  rgt: The access rights.
 *  bm: The bus master.
*/
# define OS_HAL_SMPU_RGD_WORD_2_VALUE(rgt, bm)  ((rgt) << ((bm) * OS_HAL_SMPU_ACCESSRIGHTWIDTH))

/*! Function macro to calculate the value for region word 3 */
# define OS_HAL_SMPU_RGD_WORD_3_VALUE(pid, msk)  (((pid) << 24)|((msk) << 16)|OS_HAL_SMPU_RGD_VALID)

/*! Function macro to access the region word 0 of the specified region id and System MPU base address */
# define OS_HAL_SMPU_RGD_WORD_0(base, rgdId)    ((base) + OS_HAL_SMPU_RGD_WORD_0_OFFSET(rgdId))
/*! Function macro to access the region word 1 of the specified region id and System MPU base address */
# define OS_HAL_SMPU_RGD_WORD_1(base, rgdId)    ((base) + OS_HAL_SMPU_RGD_WORD_1_OFFSET(rgdId))
/*! Function macro to access the region word 2 of the specified region id and System MPU base address */
# define OS_HAL_SMPU_RGD_WORD_2(base, rgdId)    ((base) + OS_HAL_SMPU_RGD_WORD_2_OFFSET(rgdId))
/*! Function macro to access the region word 3 of the specified region id and System MPU base address */
# define OS_HAL_SMPU_RGD_WORD_3(base, rgdId)    ((base) + OS_HAL_SMPU_RGD_WORD_3_OFFSET(rgdId))

/*! Function macro to access the region alternate access configuration of the specified region id and System MPU base address */
# define OS_HAL_SMPU_RGD_AAC(base, rgdId)       ((base) + OS_HAL_SMPU_RGD_AAC_OFFSET(rgdId))

/*! Number of supported regions - calculated with CESR.NRGD */
# define OS_HAL_SMPU_SUPPORTED_REGIONS(base)  ((((*(volatile uint32*)((base) + OS_HAL_SMPU_CESR_OFFSET)) & 0x00000F00uL) >> 6uL) + 8uL)
/*! Function macro to enable the System MPU. */
# define OS_HAL_SMPU_ENABLE(base)             (*(volatile uint32*)((base) + OS_HAL_SMPU_CESR_OFFSET) |= (1uL))
/*! Function macro to enable the System MPU. */
# define OS_HAL_SMPU_DISABLE(base)            (*(volatile uint32*)((base) + OS_HAL_SMPU_CESR_OFFSET) &= (0xFFFFFFFEuL))

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
 *  Os_Hal_MpDisableAllRegions()
 **********************************************************************************************************************/
/*! \brief          Disable all region descriptors of the calling core.
 *  \details        Invalidate all data, instruction and shared regions in the system MPU table
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts disabled.
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpDisableAllRegions,
(
  void
));

/***********************************************************************************************************************
 *  Os_Hal_MpWriteRegionDescriptor()
 **********************************************************************************************************************/
/*! \brief          Write a MPU region descriptor into MPU table
 *  \details        This function uses the system MPU assist registers to write a configured memory protection region
 *                  to the internal system MPU table.
 *
 *  \param[in]      Region  The region information that should be stored in the MPU. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *
 *  \pre            Interrupts locked to TP lock level.
 *  \pre            Supervisor Mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpWriteRegionDescriptor,
(
  P2CONST(Os_Hal_MpRegionType, TYPEDEF, OS_CONST) Region
));

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

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
OS_ALWAYS_INLINE, Os_Hal_MpDisableAllRegions,
(
  void
))
{
  uint8_least index;
  const uint8_least NumberOfRegions = OS_HAL_SMPU_SUPPORTED_REGIONS(OS_HAL_MPU_BASE_ADDRESS);                           /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */

  /* #10 Disable all regions of SMPU */
  for(index = 0; index < NumberOfRegions; index++)                                                                      /* FETA_OS_HAL_MPU */
  {
    /* #20 Set region invalid */
    *(volatile uint32*)(OS_HAL_SMPU_RGD_AAC(OS_HAL_MPU_BASE_ADDRESS, index)) = (OS_HAL_SMPU_RGD_WORD_2_VALUE((OS_HAL_SMPU_UNONE_MSK | OS_HAL_SMPU_SU_MSK), 0u)); /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */
  }
  Os_Hal_DataMemoryBarrier();
}


/***********************************************************************************************************************
 *  Os_Hal_MpWriteRegionDescriptor()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
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
  /* #10 Initialize region descriptor registers. */
  *(volatile uint32*)(OS_HAL_SMPU_RGD_WORD_0(Region->MpuBaseAddress, Region->RegionIndex)) = Region->Word0;             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */
  *(volatile uint32*)(OS_HAL_SMPU_RGD_WORD_1(Region->MpuBaseAddress, Region->RegionIndex)) = Region->Word1;             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */
  *(volatile uint32*)(OS_HAL_SMPU_RGD_WORD_2(Region->MpuBaseAddress, Region->RegionIndex)) = Region->Word2;             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */
  *(volatile uint32*)(OS_HAL_SMPU_RGD_WORD_3(Region->MpuBaseAddress, Region->RegionIndex)) = Region->Word3;             /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */
  /* #20 Ensure that configuration is updated in hardware on return. */
  Os_Hal_DataMemoryBarrier();
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
    /* #20 Read back the current stack region start address from hardware. */
    StackConfig->StackRegionStart = *(volatile uint32*)                                                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
                                    (OS_HAL_SMPU_RGD_WORD_0(OS_HAL_MPU_BASE_ADDRESS, OS_HAL_MP_STACK_REGION_NUMBER));
    /* #30 Read back the current stack region end address from hardware. */
    StackConfig->StackRegionEnd   = *(volatile uint32*)                                                                 /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_PWA_CALLER */
                                    (OS_HAL_SMPU_RGD_WORD_1(OS_HAL_MPU_BASE_ADDRESS, OS_HAL_MP_STACK_REGION_NUMBER));
  }
}


/***********************************************************************************************************************
 *  Os_Hal_MpSystemInit()
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
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,                                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
OS_ALWAYS_INLINE, Os_Hal_MpSystemInit,
(
  P2CONST(Os_Hal_MpSystemConfigType , TYPEDEF, OS_CONST) SystemConfig
))
{
  Os_ObjIdxType index;
  uint32 interruptLevel;

  /* #10 Disable interrupts and store prior state. */
  interruptLevel = Os_Hal_DisableAndReturnPriorState();

  /* #20 Disable the MPU. */
  OS_HAL_SMPU_DISABLE(OS_HAL_MPU_BASE_ADDRESS);                                                                         /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */

  /* #30 Invalidate all region descriptors. */
  Os_Hal_MpDisableAllRegions();

  /* #40 Write configured region descriptors. */
  for(index = 0; index < SystemConfig->MpuRegionCount; index++)                                                         /* FETA_OS_HAL_MPU */
  {
    Os_Hal_MpWriteRegionDescriptor(&(SystemConfig->MpuRegions[index]));                                                 /* SBSW_OS_HAL_FC_MPUCONFIG */
  }

  /* #50 Enable the MPU. */
  OS_HAL_SMPU_ENABLE(OS_HAL_MPU_BASE_ADDRESS);                                                                          /* PRQA S 0303 */ /* MD_Os_Hal_Rule11.4_0303 */ /* SBSW_OS_HAL_SMPU_ADDR */

  /* #60 Restore prior interrupt state. */
  Os_Hal_WriteBasepri(interruptLevel);
}


/***********************************************************************************************************************
 *  Os_Hal_MpCoreInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_MpCoreInit,
(
  P2CONST(Os_Hal_MpCoreConfigType , TYPEDEF, OS_CONST) CoreConfig,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) InitialStackRegion
))
{
  /* #10 No Core MPU supported. Do nothing. */
  OS_IGNORE_UNREF_PARAM(CoreConfig);                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(InitialStackRegion);                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
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
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* OS_HAL_MEMORYPROTECTION_NXP_H_ */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtection_SMPU_1.h
 *********************************************************************************************************************/


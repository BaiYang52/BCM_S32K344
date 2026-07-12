/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
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
 *  \brief      Internal functions and data types related to memory protection.
 *  \details
 *  \{
 *
 *  \file
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_MEMORYPROTECTION_PMSAV8MINT_H
# define OS_HAL_MEMORYPROTECTION_PMSAV8MINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! MPU specific define for stack region configuration. */
# define OS_HAL_MPU_GETSTACKREGIONINFO(stackConfig)   (stackConfig.StackRegionEnd)

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*! Memory Protection Unit  (MPU) base address. */
# define OS_HAL_MP_MPU_BASE    ((volatile Os_Hal_MpuBaseType*)OS_HAL_MPU_BASE_ADDRESS)

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Description of a single region in the ARM PMSAv8m implementation. */
struct Os_Hal_MpRegionType_Tag
{
  uint32 RegionNumber;         /*!< Region number. */
  uint32 RegionBaseAddress;    /*!< Region base address. */
  uint32 RegionEndAddress;     /*!< Region end address. */
  uint32 RegionAccessControl;  /*!< Region base address. */
  uint32 RegionEnableAndFlags; /*!< Region end address. */
};

/*! Description of MPU registers in the ARM PMSAv8m implementation. */
struct Os_Hal_MpuBaseType_Tag
{
  uint32 Type;                 /*!< Offset: 0x00 MPU type register (MPU_TYPE). */
  uint32 Ctrl;                 /*!< Offset: 0x04 MPU control register (MPU_CTRL). */
  uint32 Rnr;                  /*!< Offset: 0x08 MPU region number (MPU_RNR). */
  uint32 Rbar;                 /*!< Offset: 0x0C MPU region base address register (MPU_RBAR). */
  uint32 Rlar;                 /*!< Offset: 0x10 MPU region limit address register (MPU_RLAR). */
  uint32 RbarA;                /*!< Offset: 0x14 MPU region base address alias register (MPU_RBAR_A). */
  uint32 RlarA;                /*!< Offset: 0x18 MPU region limit address register (MPU_RLAR_A). */
  uint32 Reserved[5];          /*!< Offset: 0x1C-0x2C Reserved. */
  uint32 Mair0;                /*!< Offset: 0x30 MPU memory attributeindirection register (MPU_MAIR0). */
  uint32 Mair1;                /*!< Offset: 0x34 MPU memory attributeindirection register (MPU_MAIR1). */
};

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! MPU Control Register, MPU active. */
# define OS_HAL_MP_MPU_CTRL_ENABLE                   (0x01uL)
/*! MPU Control Register, MPU active in HardFault and NMIs. */
# define OS_HAL_MP_MPU_CTRL_HFNMIENA                 (0x01uL << 1u)
/*! MPU Control Register, Enable background region. */
# define OS_HAL_MP_MPU_CTRL_PRIVDEFENA               (0x01uL << 2u)

/*! MPU region limit address register (RLAR): RLAR.En bit. */
# define OS_HAL_MP_REGION_SIZE_ENABLE                (0x01uL)
/*! MPU region base address register (RBAR): RBAR Address bits. */
# define OS_HAL_MP_REGION_SIZE_ALIGN                 (~(uint32)(0x3Fu))
# define OS_HAL_MP_REGION_BASE_ALIGN(x)  ((x) & OS_HAL_MP_REGION_SIZE_ALIGN)

/*! MPU region base address register (RBAR): RBAR.XN bit, allowed value. */
# define OS_HAL_MP_ACCESS_EXECUTE_ALLOWED            (0x0uL)
/*! MPU region base address register (RBAR): RBAR.XN bit, forbidden value. */
# define OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN          (0x01uL)

/*! MPU region base address register (RBAR): RBAR.AP bit.
 *  \details Supervisor: RW- User: ---. */
# define OS_HAL_MP_ACCESS_S_RW_U_NONE                (0x0uL << 1u)
/*! MPU region base address register (RBAR): RBAR.AP bit.
 *  \details Supervisor: RW- User: RW-. */
# define OS_HAL_MP_ACCESS_S_RW_U_RW                  (0x01uL << 1u)
/*! MPU region base address register (RBAR): RBAR.AP bit.
 *  \details Supervisor: R-- User: ---. */
# define OS_HAL_MP_ACCESS_S_R_U_NONE                 (0x02uL << 1u)
/*! MPU region base address register (RBAR): RBAR.AP bit.
 *  \details Supervisor: R-- User: R--. */
# define OS_HAL_MP_ACCESS_S_R_U_R                    (0x03uL << 1u)

/*! MPU region base address register (RBAR): RBAR.SH field. */
# define OS_HAL_MP_ACCESS_TYPE_OUTER_SHAREABLE       (0x02uL << 3u)
/*! MPU region base address register (RBAR): RBAR.SH field. */
# define OS_HAL_MP_ACCESS_TYPE_INNER_SHAREABLE       (0x03uL << 3u)

/* Device memory type: non Gathering, non Re-ordering, non Early Write Acknowledgment. */
# define OS_HAL_MP_ACCESS_TYPE_DEVICE_nGnRnE_MEMORY  (uint8)(0x00u)
/* Device memory type: non Gathering, non Re-ordering, Early Write Acknowledgment. */
# define OS_HAL_MP_ACCESS_TYPE_DEVICE_nGnRE_MEMORY   (uint8)(0x04u)
/* Device memory type: non Gathering, Re-ordering, Early Write Acknowledgment. */
# define OS_HAL_MP_ACCESS_TYPE_DEVICE_nGRE_MEMORY    (uint8)(0x08u)
/* Device memory type: Gathering, Re-ordering, Early Write Acknowledgment. */
# define OS_HAL_MP_ACCESS_TYPE_DEVICE_GRE_MEMORY     (uint8)(0x0Cu)

/* Normal memory. Outer Write-Through transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_TRANSIENT_READALLOCATE  (uint8)(0x02u << 4u)
/* Normal memory. Outer Write-Through transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_TRANSIENT_WRITEALLOCATE  (uint8)(0x01u << 4u)
/* Normal memory. Outer Write-Through transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_TRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x03u << 4u)

/* Normal Memory. Outer Non-Cacheable. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_NON_CACHEABLE   (uint8)(0x04u << 4u)

/* Normal memory. Outer Write-Back transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_TRANSIENT_READALLOCATE  (uint8)(0x06u << 4u)
/* Normal memory. Outer Write-Back transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_TRANSIENT_WRITEALLOCATE  (uint8)(0x05u << 4u)
/* Normal memory. Outer Write-Back transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_TRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x07u << 4u)

/* Normal memory. Outer Write-Through non-transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_NONTRANSIENT_READALLOCATE  (uint8)(0x0Au << 4u)
/* Normal memory. Outer Write-Through non-transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_NONTRANSIENT_WRITEALLOCATE  (uint8)(0x09u << 4u)
/* Normal memory. Outer Write-Through non-transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITETHROUGH_NONTRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x0Bu << 4u)

/* Normal memory. Outer Write-Back non-transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_NONTRANSIENT_READALLOCATE  (uint8)(0x0Eu << 4u)
/* Normal memory. Outer Write-Back non-transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_NONTRANSIENT_WRITEALLOCATE  (uint8)(0x0Du << 4u)
/* Normal memory. Outer Write-Back non-transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_OUTER_WRITEBACK_NONTRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x0Fu << 4u)

/* Normal memory. Inner Write-Through transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_TRANSIENT_READALLOCATE  (uint8)(0x02u)
/* Normal memory. Inner Write-Through transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_TRANSIENT_WRITEALLOCATE  (uint8)(0x01u)
/* Normal memory. Inner Write-Through transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_TRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x03u)

/* Normal Memory. Inner Non-Cacheable. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE   (uint8)(0x04u)

/* Normal memory. Inner Write-Back transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_TRANSIENT_READALLOCATE  (uint8)(0x06u)
/* Normal memory. Inner Write-Back transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_TRANSIENT_WRITEALLOCATE  (uint8)(0x05u)
/* Normal memory. Inner Write-Back transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_TRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x07u)

/* Normal memory. Inner Write-Through non-transient. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_NONTRANSIENT  (uint8)(0x08u)
/* Normal memory. Inner Write-Through non-transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_NONTRANSIENT_READALLOCATE  (uint8)(0x0Au)
/* Normal memory. Inner Write-Through non-transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_NONTRANSIENT_WRITEALLOCATE  (uint8)(0x09u)
/* Normal memory. Inner Write-Through non-transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITETHROUGH_NONTRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x0Bu)

/* Normal memory. Inner Write-Back non-transient. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_NONTRANSIENT  (uint8)(0x0Cu)
/* Normal memory. Inner Write-Back non-transient, read allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_NONTRANSIENT_READALLOCATE  (uint8)(0x0Eu)
/* Normal memory. Inner Write-Back non-transient, write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_NONTRANSIENT_WRITEALLOCATE  (uint8)(0x0Du)
/* Normal memory. Inner Write-Back non-transient, read allocate and write allocate. */
# define OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_WRITEBACK_NONTRANSIENT_READALLOCATE_WRITEALLOCATE  (uint8)(0x0Fu)

/*! Memory Attribute Indirection Registers 0 and 1 (MAIR0 and MAIR1) bits assignement. */
#define OS_HAL_MP_MAIR0_INIT_VALUE  ((uint32)(((uint32)OS_HAL_MP_MAIR0_ATTR3 << 24u) | \
                                              ((uint32)OS_HAL_MP_MAIR0_ATTR2 << 16u) | \
                                              ((uint32)OS_HAL_MP_MAIR0_ATTR1 << 8u)  | \
                                              (uint32)OS_HAL_MP_MAIR0_ATTR0))
#define OS_HAL_MP_MAIR1_INIT_VALUE  ((uint32)(((uint32)OS_HAL_MP_MAIR1_ATTR7 << 24u) | \
                                              ((uint32)OS_HAL_MP_MAIR1_ATTR6 << 16u) | \
                                              ((uint32)OS_HAL_MP_MAIR1_ATTR5 << 8u)  | \
                                              (uint32)OS_HAL_MP_MAIR1_ATTR4))

#endif /* OS_HAL_MEMORYPROTECTION_PMSAV8MINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_MemoryProtection_PMSAv8mInt.h
 **********************************************************************************************************************/

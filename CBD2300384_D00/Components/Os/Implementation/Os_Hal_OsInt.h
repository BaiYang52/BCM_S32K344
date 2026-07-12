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
 *  \defgroup   Os_Hal_Os  HAL OS
 *  \{
 *
 *  \file       Os_Hal_OsInt.h
 *  \brief      HAL interfaces which are visible to the user.
 *  \details
 *  This file is included by Os.h. Therefore all symbols defined here are visible to the user.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_OSINT_H
# define OS_HAL_OSINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_Os_Types.h"

/* Os kernel module dependencies */
# include "Os_Lcfg.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! \brief      Provides function declaration for category 1 ISRs.
 *  \details    -
 *  \param[in]  IsrName   The function name which contains the user code of the category 1 ISR.
 */
# define OS_HAL_ISR1_DECLARE(IsrName)      void IsrName(void)

/*! \brief      Provides function definition for category 1 ISRs.
 *  \details    The kernel uses this macro to provide the platform independent OS_ISR1() interface.
 *  \param[in]  IsrName   The function name which contains the user code of the category 1 ISR.
 */
# define OS_HAL_ISR1_DEFINE(IsrName)       void IsrName(void)

/*! \brief      Provides function declaration for category 0 ISRs.
 *  \param[in]  IsrName   The function name which contains the user code of the category 0 ISR.
 */
# define OS_HAL_ISR0_DECLARE(IsrName)      void IsrName(void)

/*! \brief      Provides function definition for category 0 ISRs.
 *  \details    The kernel uses this macro to provide the platform independent OS_ISR0() interface.
 *  \param[in]  IsrName   The function name which contains the user code of the category 0 ISR.
 */
# define OS_HAL_ISR0_DEFINE(IsrName)       void IsrName(void)


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! \brief     FPU lower registers: S0..S15.
 *  \details   In case floating point context is not supported, registers S0,..,S15 and FPSCR are filled with "DEAD"
 *  -
*/
struct Os_Hal_FpuLowerRegistersType_Tag
{
  /* 0x24 */ uint32 S0;                      /*!< S0. */
  /* 0x28 */ uint32 S1;                      /*!< S1. */
  /* 0x2C */ uint32 S2;                      /*!< S2. */
  /* 0x30 */ uint32 S3;                      /*!< S3. */
  /* 0x34 */ uint32 S4;                      /*!< S4. */
  /* 0x38 */ uint32 S5;                      /*!< S5. */
  /* 0x3C */ uint32 S6;                      /*!< S6. */
  /* 0x40 */ uint32 S7;                      /*!< S7. */
  /* 0x44 */ uint32 S8;                      /*!< S8. */
  /* 0x48 */ uint32 S9;                      /*!< S9. */
  /* 0x4C */ uint32 S10;                     /*!< S10. */
  /* 0x50 */ uint32 S11;                     /*!< S11. */
  /* 0x54 */ uint32 S12;                     /*!< S12. */
  /* 0x58 */ uint32 S13;                     /*!< S13. */
  /* 0x5C */ uint32 S14;                     /*!< S14. */
  /* 0x60 */ uint32 S15;                     /*!< S15. */
};

typedef struct Os_Hal_FpuLowerRegistersType_Tag Os_Hal_FpuLowerRegistersType;

/*! \brief     Frame pushed to exception stack on exception entry.
 *  -
*/
struct Os_Hal_ExceptionStackFrameType_Tag
{
  /* 0x04 */ uint32 R0;                                       /*!< R0. */
  /* 0x08 */ uint32 R1;                                       /*!< R1. */
  /* 0x0C */ uint32 R2;                                       /*!< R2. */
  /* 0x10 */ uint32 R3;                                       /*!< R3. */
  /* 0x14 */ uint32 R12;                                      /*!< R12. */
  /* 0x18 */ uint32 LR;                                       /*!< Link Register (LR) of the routine where the exception occured. */
  /* 0x1C */ uint32 ReturnAddress;                            /*!< Address to which the execution returns after the processor has handled the exception. */
  /* 0x20 */ uint32 XPSR;                                     /*!< Program Status Register (xPSR). */
  /* 0x24 */ Os_Hal_FpuLowerRegistersType FpuLowerRegisters;  /*!< FPU lower registers: S0..S15. */
  /* 0x64 */ uint32 FPSR;                                     /*!< FPSR. */
};

typedef struct Os_Hal_ExceptionStackFrameType_Tag Os_Hal_ExceptionStackFrameType;

/*! \brief     Special purpose registers: PRIMASK, BASEPRI, FAULTMASK, CONTROL.
 *  -
*/
struct Os_Hal_SpecialPurposeRegistersType_Tag
{
  /* 0x6C */ uint32 PRIMASK;      /*!< PRIMASK special purpose register. */
  /* 0x70 */ uint32 BASEPRI;      /*!< BASEPRI special purpose register. */
  /* 0x74 */ uint32 FAULTMASK;    /*!< FAULTMASK special purpose register. */
  /* 0x78 */ uint32 CONTROL;      /*!< CONTROL special purpose register. */
};

typedef struct Os_Hal_SpecialPurposeRegistersType_Tag Os_Hal_SpecialPurposeRegistersType;

/*! \brief     FPU upper registers: S16..S31.
 *  \details   In case floating point context is not supported, registers S16,..,S31 and FPSCR are filled with "DEAD"
 *  -
*/
struct Os_Hal_FpuUpperRegistersType_Tag
{
  /* 0x9C */ uint32 S16;                      /*!< S16. */
  /* 0xA0 */ uint32 S17;                      /*!< S17. */
  /* 0xA4 */ uint32 S18;                      /*!< S18. */
  /* 0xA8 */ uint32 S19;                      /*!< S19. */
  /* 0xAC */ uint32 S20;                      /*!< S20. */
  /* 0xB0 */ uint32 S21;                      /*!< S21. */
  /* 0xB4 */ uint32 S22;                      /*!< S22. */
  /* 0xB8 */ uint32 S23;                      /*!< S23. */
  /* 0xBC */ uint32 S24;                      /*!< S24. */
  /* 0xC0 */ uint32 S25;                      /*!< S25. */
  /* 0xC4 */ uint32 S26;                      /*!< S26. */
  /* 0xC8 */ uint32 S27;                      /*!< S27. */
  /* 0xCC */ uint32 S28;                      /*!< S28. */
  /* 0xD0 */ uint32 S29;                      /*!< S29. */
  /* 0xD4 */ uint32 S30;                      /*!< S30. */
  /* 0xD8 */ uint32 S31;                      /*!< S31. */
};

typedef struct Os_Hal_FpuUpperRegistersType_Tag Os_Hal_FpuUpperRegistersType;

/*! \brief     Current thread Stack region information: start, size, ...
 *  \details
 *   - StackMpuRegionStartInfo : Current thread stack region start info
 *    . Cortex-M : MPU_RBAR register content
 *    . NXP      : RGDx_WORD0 register content which is the stack region start address.
 *
 *   - StackMpuRegionEndInfo   : Current thread stack region end/size info
 *    . Cortex-M : MPU_RASR register content
 *    . NXP      : RGDx_WORD1 register content which is the stack region end address.
 *
 *   -
*/
struct Os_Hal_ThreadStackMpuRegionDetailsType_Tag
{
  /* 0xDC */ uint32 StackMpuRegionStartInfo; /*!< Current thread stack region start info */
  /* 0xE0 */ uint32 StackMpuRegionEndInfo;   /*!< Current thread stack region end/size info */
};

typedef struct Os_Hal_ThreadStackMpuRegionDetailsType_Tag Os_Hal_ThreadStackMpuRegionDetailsType;

/*! \brief     Exception context infomation to be able to resume from an exception or for debugging purposes.
 *  \details
 *   - ValidStackFrame informs if one of the following erros occured when pushing the exception stack frame:
 *      . Failure on a hardware save of context on exception entry, because of an MPU access violation,
 *      . Failure on a hardware save of context on exception entry due to a bus error.
 *     Such errors lead to further exceptions, if the excaption handler tries to read from the stack frame.
 *     1: Valid frame   (no such errors occured),
 *     0: Invalid frame (one of the abovementioned errors occured when pushing the stack frame).
 *   -
*/
struct Os_ExceptionContextType_Tag
{
  /* 0x00 */ uint32 ValidStackFrame;                                                    /*!< 1: Valid frame, 0: Invalid frame */
  /* 0x04 */ Os_Hal_ExceptionStackFrameType ExceptionStackFrame;                        /*!< Exception stack frame content. */
  /* 0x68 */ uint32 Original_SP;                                                        /*!< Original SP of the routine where the exception occured. */
  /* 0x6C */ Os_Hal_SpecialPurposeRegistersType SpecialPurposeRegisters;                /*!< Special purpose registers. */
  /* 0x7C */ uint32 R4;                                                                 /*!< R4. */
  /* 0x80 */ uint32 R5;                                                                 /*!< R5. */
  /* 0x84 */ uint32 R6;                                                                 /*!< R6. */
  /* 0x88 */ uint32 R7;                                                                 /*!< R7. */
  /* 0x8C */ uint32 R8;                                                                 /*!< R8. */
  /* 0x90 */ uint32 R9;                                                                 /*!< R9. */
  /* 0x94 */ uint32 R10;                                                                /*!< R10. */
  /* 0x98 */ uint32 R11;                                                                /*!< R11. */
  /* 0x9C */ Os_Hal_FpuUpperRegistersType FpuUpperRegisters;                            /*!< FPU upper registers: S16..S31. */
  /* 0xDC */ Os_Hal_ThreadStackMpuRegionDetailsType Os_Hal_ThreadStackMpuRegionDetails; /*!< Current thread Stack MPU region information: start, size, ... */
};


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_OS_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_OsInt.h
 **********************************************************************************************************************/

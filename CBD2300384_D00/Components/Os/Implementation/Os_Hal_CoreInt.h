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
 * \ingroup     Os_Hal
 * \defgroup    Os_Hal_Core HAL Core
 * \brief       Core and multi-core related primitives.
 * \details
 * Some of the defined types shall be mapped to the word type. Therefore it is recommended to
 * create a type Os_Hal_WordType which is typedef-ed to an unsigned word.
 *
 * \trace CREQ-1201
 *
 * \{
 * \file
 * \brief   Core and multi-core related primitives.
 *
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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_COREINT_H
# define OS_HAL_COREINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module dependencies */
# include "Os_CommonInt.h"
# include "Os_Hal_Core_Types.h"

/* Os kernel module declarations */
# include "Os_Core_Types.h"

/* Os HAL dependencies */
# include "Os_Hal_InterruptInt.h"
# include "Os_Hal_Context_Types.h"
# include "Os_Core_Cfg.h"


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! \brief    This define is used to mask the MSR bits that are not controlled by the OS (user bits).
 *  \details
 *  The define is needed during core initialization and context switch to avoid destroying the user configuration
 *  of the MSR (Machine State Register).
 *
 *  Internal comment removed.
 *
 *
 */
# define OS_HAL_CORE_MSR_USER_BIT_MSK                  (0x00000000uL)

/*! Defines whether the HAL provides a count leading zeros command (STD_ON) or not (STD_OFF). */
# define OS_HAL_COUNT_LEADING_ZEROS_HARDWARE     STD_ON

/*! The required address width to subscript the bits of a Os_Hal_CoreBitFieldType.
 * The value can be selected from the following table:
 * typeof(Os_Hal_BitFieldType)      | OS_HAL_INDEXBITWIDTH
 * :------------------------------: | :----------------------------
 * uint8                            | 3
 * uint16                           | 4
 * uint32                           | 5
 * uint64                           | 6
 */
# define OS_HAL_INDEXBITWIDTH       (5)

/*! Defines the value which is returned by \ref Os_Hal_SpinTryLock, if locking succeeded. */
# define OS_HAL_SPINLOCKSUCCEEDED    ((Os_Hal_SpinlockResultType)0uL)

/*! Defines the value which is stored by a call of \ref Os_Hal_SpinTryLock, if locking succeeded */
# define OS_HAL_SPINLOCK_LOCKED       ((Os_Hal_SpinlockType)1U)

/*! Program Status Register (PSR): Thumb mode bit mask. */
# define OS_HAL_PSR_THUMB_BIT                   (1uL << 24)

/*! PRIMASK Register: Register value to enable interrupts. */
# define OS_HAL_PRIMASK_INTERRUPTS_NOT_MASKED   (0u)
/*! PRIMASK Register: Register value to disable interrupts. */
# define OS_HAL_PRIMASK_INTERRUPTS_MASKED       (1u)

/*! FAULTMASK Register: Register value to enable exceptions. */
# define OS_HAL_FAULTMASK_INTERRUPTS_NOT_MASKED (0u)
/*! FAULTMASK Register: Register value to disable exceptions. */
# define OS_HAL_FAULTMASK_INTERRUPTS_MASKED     (1u)

/*! CONTROL: Privileged access mask. */
# define OS_HAL_CONTROL_PRIVILEGED_MASK         (1uL)
/*! CONTROL.nPRIV: Privileged access value. */
# define OS_HAL_CONTROL_PRIVILEGED              (0uL << 0)
/*! CONTROL.nPRIV: User access value. */
# define OS_HAL_CONTROL_USER_STATE              (1u << 0)
/*! CONTROL Register: SPSEL bit value: MSP main stack pointer. */
# define OS_HAL_CONTROL_MSP_USED                (0uL << 1)
/*! CONTROL Register: SPSEL bit value: PSP process stack pointer. */
# define OS_HAL_CONTROL_PSP_USED                (1u << 1)

/*! PSR Register: IPSR Mask. */
# define OS_HAL_PSR_IPSR_MASK                   (0x1FFuL)

/*! CONTROL Register: FPCA bit value: floating point extension used. */
# define OS_HAL_CONTROL_FPCA_MASK               (1u << 2)
/*! Index of FPCA bit inside CONTROL register. */
# define OS_HAL_CONTROL_FPCA_BIT_INDEX          (2uL)

/*! The base address of the System Handler Configuration Registers */
# define OS_HAL_SCS_SH_BASE_ADDRESS             (0xE000ED14uL)
/*! The SHCSR.USGFAULTENA bit: UsageFault exception is enabled. */
# define OS_HAL_SCS_SH_SHCSR_USGFAULTENA        (1uL << 18)
/*! The SHCSR.BUSFAULTENA bit: BusFault exception is enabled. */
# define OS_HAL_SCS_SH_SHCSR_BUSFAULTENA        (1uL << 17)
/*! The SHCSR.MEMFAULTENA bit: MemManage exception is enabled. */
# define OS_HAL_SCS_SH_SHCSR_MEMFAULTENA        (1uL << 16)

/*! The CCR.NONBASETHRDENA bit: Return to thread state at any level. */
# define OS_HAL_CCR_NONBASETHRDENA              (1uL << 0)

# if defined (OS_HAL_ARCH_V8M)                                                                                          /* COV_OS_DERIVATIVEBETAMODULE */
#  define Os_Hal_SetsysHandlerCcr()
# else
#  define Os_Hal_SetsysHandlerCcr()  sysHandler->Ccr |= OS_HAL_CCR_NONBASETHRDENA
# endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! Platform specific core data. */
typedef uint32 Os_Hal_CoreType;

/*! \brief    Platform specific core configuration for AUTOSAR and Non-AUTOSAR cores.
 *  \details  Contains information for starting cores and master-slave emulation, if needed.
 */
struct Os_Hal_CoreConfigType_Tag
{
  uint32 CoreId;            /*!< Physical core ID. */
  uint32 CoreStartAddress;  /*!< Core Entry Point. */
};

/*! \brief    Platform specific AUTOSAR core configuration.
 *  \details  Contains the core specific interrupt and exceptions tables.
 */
struct Os_Hal_CoreAsrConfigType_Tag
{
  Os_Hal_ExceptionTableType ExceptionTable;   /*!< Pointer to exception vector table. */
  Os_Hal_VectorTableType VectorTable;         /*!< Pointer to interrupt vector table. */
  uint32 ExceptionPriority[3];                /*!< Exception priorities. */
};

/*! \brief Platform specific dynamic system information.
 */
struct Os_Hal_Core2ThreadType_Tag
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread; /*!< Pointer to current thread. */
};

/*! \brief Platform specific list of system information.
 */
struct Os_Hal_Core2ThreadConfigType_Tag
{
  /*! Pointer to current thread pointer. */
  P2VAR(Os_Hal_Core2ThreadType, AUTOMATIC, OS_VAR_NOINIT_FAST) Core2Thread[OS_CFG_COREPHYSICALID_COUNT];
};

/*! \brief  Platform specific system configuration.
 */
struct Os_Hal_SystemConfigType_Tag
{
  uint32 Dummy; /*!< Data type not used on ARM platform. */
};

/*! System Handler Configuration Registers layout for ARMv7-M Architectures. */
typedef struct
{
  /*! Configuration Control Register (CCR). */
  uint32 Ccr;
  /*! System Handler Priority Register 1 (SHPR1). Used for system handlers 4--7 (MemManage, BusFault, UsageFault, reserved) */
  uint32 Shpr1;
  /*! System Handler Priority Register 2 (SHPR2). Used for system handlers 8--11 (reserved, reserved, reserved, SVCall) */
  uint32 Shpr2;
  /*! System Handler Priority Register 3 (SHPR3). Used for system handlers 12--15 (DebugMonitor, reserved, PendSV, SysTick) */
  uint32 Shpr3;
  /*!< System Handler Control and State Regsiter (SHCSR). */
  uint32 Shcsr;
} Os_Hal_CoreV7MSystemHandlerType;



/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_Hal_CoreGetId()
 **********************************************************************************************************************/
/*! \brief          Returns the physical identifier of the local core
 *  \details
 *
 *  \return         Physical identifier of the local core.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 *
 *  \note           Candidate for OS_NOSIDEEFFECTS or OS_PURE.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_CoreGetId,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_CoreInit()
 **********************************************************************************************************************/
/*! \brief      Hardware initialization of the current core.
 *  \details    The kernel calls this function on each core. It shall not enable memory protection
 *              (this is part of the memory protection module). See also Os_Hal_CoreInitHardware().
 *
 *  \param[in]    Config  Configuration which shall be used to perform hardware initialization on each core.
 *                        Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode
 *  \pre          OS may be not initialized
 *
 *  \trace        CREQ-1201
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_CoreInit
(
  P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config
);


/***********************************************************************************************************************
 *  Os_Hal_CoreInitHardware()
 **********************************************************************************************************************/
/*! \brief      Hardware initialization on the Hardware Init Core.
 *  \details    The kernel calls this function on the Hardware Init Core. In most cases this is the master core too.
 *              It shall not enable memory protection.
 *              See also Os_Hal_CoreInit().
 *
 *  \param[in]    Config  Configuration which shall be used to perform hardware initialization.
 *                        Parameter must not be NULL.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode
 *  \pre          OS may be not initialized
 *
 *  \trace        CREQ-1201
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_CoreInitHardware
(
  P2CONST(Os_Hal_SystemConfigType, AUTOMATIC, OS_CONST) Config
);


/***********************************************************************************************************************
 *  Os_Hal_CoreIsThreadPointerUserReadable()
 **********************************************************************************************************************/
/*! \brief        Returns whether the thread pointer is readable in user mode.
 *  \details      --no details--
 *
 *  \retval       !0 Thread pointer is readable in user mode.
 *  \retval       0  Thread pointer is not readable in user mode.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreIsThreadPointerUserReadable,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_CoreIsCoreIdUserReadable()
 **********************************************************************************************************************/
/*! \brief        Returns whether the core ID is readable in user mode.
 *  \details      --no details--
 *
 *  \retval       !0 CoreId is readable in user mode.
 *  \retval       0  CoreId is not readable in user mode.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreIsCoreIdUserReadable,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_CoreSetThread()
 **********************************************************************************************************************/
/*! \brief        Set the current thread pointer on the local core.
 *  \details      Main intention of holding the current thread pointer in the HAL is to speed up access to it.
 *                The current thread pointer is accessed very often. The thread tells the kernel to which core it
 *                belongs, to which application, which memory access rights are currently active,
 *                which timing protection settings, etc.
 *
 *                There are the following variants to speed this up:
 *                1. OS exclusive special purpose register: On some architectures there are special purpose register for
 *                   OS usage. They can be used to hold information which is frequently used.
 *                2. Fast memory: Map the location for this value to fast accessible memory.
 *
 *                If the architecture does not provide such possibilities, the thread pointer can be an attribute in the
 *                Os_Hal_Core2ThreadConfigType data structure.
 *
 *                Safety measures:
 *                In case of mixed criticality derivatives, the location of the current thread has to be write protected
 *                for other cores. This may be achieved by using either
 *                  - a core local register (write protected in user mode)
 *                  - or by using a constant array which references the location of the current thread.
 *                    This location is write protected for other cores.
 *
 *  \see          \ref Os_Thread
 *  \see          \ref Os_Hal_Core2ThreadType
 *  \see          \ref Os_Hal_Core2ThreadConfigType
 *
 *  \param[out]     Core2Thread    Pointer to a data structure which allows to get the current thread on a specific core.
 *                                 Parameter must not be NULL.
 *  \param[in]    Thread  Pointer to the current thread of the local core. Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreSetThread,
(
  P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread,
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));


/***********************************************************************************************************************
 *  Os_Hal_CoreGetThread()
 **********************************************************************************************************************/
/*! \brief        Returns the current thread pointer of the local core.
 *  \details      See Os_Hal_CoreSetThread().
 *
 *  \param[in]    Core2Thread      Pointer to a data structure which allows to get the current thread on a specific core.
 *                                 Parameter must not be NULL.
 *
 *  \return       Pointer to the current thread of the local core.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          Current thread pointer has been set before.
 *
 *  \note         Candidate for OS_NOSIDEEFFECTS or OS_PURE.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST), OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_CoreGetThread,
(
  P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread
));


/***********************************************************************************************************************
 *  Os_Hal_CoreIsPrivilegedModeUserReadable()
 **********************************************************************************************************************/
/*! \brief        Returns whether the privileged mode is readable in user mode.
 *  \details      User has full access rights to the privileged mode.
 *
 *  \retval       !0 Privileged mode is readable in user mode.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreIsPrivilegedModeUserReadable,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_CoreGetPrivilegedMode()
 **********************************************************************************************************************/
/*! \brief        Returns the current privileged mode.
 *  \details      --no details--
 *
 *  \retval       !0 Handler mode or Thread mode with privileged access.
 *  \retval       0  Thread mode with unprivileged access.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreGetPrivilegedMode,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_CountLeadingZeros()
 **********************************************************************************************************************/
/*! \brief          Returns the number of consecutive zeros counted from the MSb.
 *  \details        If the MSb of BitField is set, zero is returned.
 *                  If the given BitField is zero, the bit width of Os_BitArrayType is returned.
 *
 *  \param[in]      BitField        Field to query.
 *
 *  \return         The number of consecutive zero bits.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_CountLeadingZeros,
(
  Os_Hal_BitFieldType BitField
));


/***********************************************************************************************************************
 *  Os_Hal_StackEndGet()
 **********************************************************************************************************************/
/*! \brief        Returns the stack end of two given addresses.
 *  \details      Stack growing direction is hardware dependent. On some architectures the stack end is at the low
 *                address of the stack, on others at the high address of the stack.
 *
 *  \see          \ref GlossaryStackWording
 *
 *  \param[in,out]    LowAddress  The low address of the stack.
 *  \param[in,out]    HighAddress The high address of the stack.
 *
 *  \return       Stack end pointer.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT),
OS_CODE, OS_ALWAYS_INLINE, Os_Hal_StackEndGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
));


/***********************************************************************************************************************
 *  Os_Hal_StackBottomGet()
 **********************************************************************************************************************/
/*! \brief        Returns the stack bottom of two given addresses.
 *  \details      Stack growing direction is hardware dependent. On some architectures the stack bottom is at the low
 *                address of the stack, on others at the high address of the stack.
 *
 *  \see          \ref GlossaryStackWording
 *
 *  \param[in,out]    LowAddress  The low address of the stack.
 *  \param[in,out]    HighAddress The high address of the stack.
 *
 *  \return       Stack bottom pointer.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT),
OS_CODE, OS_ALWAYS_INLINE, Os_Hal_StackBottomGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
));


/***********************************************************************************************************************
 *  Os_Hal_StackTopGet()
 **********************************************************************************************************************/
/*! \brief        Returns the stack top of the current stack.
 *  \details      Depending on the architecture, the stack pointer points to the last used or the next free entry.
 *                The stack top is defined as the last entry used.
 *
 *  \see          \ref GlossaryStackWording
 *
 *  \return       Stack top of the current stack.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -

 *********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_StackTopGet,
(
  void
));


/***********************************************************************************************************************
 *  Os_Hal_StackPointerMoveDown()
 **********************************************************************************************************************/
/*! \brief        Moves the given pointer towards the stack bottom.
 *  \details      On systems where a pop instruction:
 *                1. Decreases the pointer, this function decreases the given stack address.
 *                2. Increases the pointer, this function increases the given stack address.
 *
 *                Decreasing and increasing is done using 'sizeof(Os_Hal_CoreStackType)'.
 *
 *  \see          \ref GlossaryStackWording
 *
 *  \param[in,out]    StackPointer  The stack pointer to step down.
 *
 *  \return       The result pointer address.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_StackPointerMoveDown,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackPointer
));


/***********************************************************************************************************************
 *  Os_Hal_StackCalculateUsage()
 **********************************************************************************************************************/
/*! \brief        Calculates the number of bytes between the stack bottom and the stack top.
 *  \details      -
 *
 *  \see          \ref GlossaryStackWording
 *
 *  \param[in]    StackBottom   Stack bottom.
 *  \param[in]    StackTop      Stack top.
 *
 *  \return       Number of bytes between StackBottom and StackTop including StackBottom and StackTop.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackCalculateUsage,
(
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackBottom,
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackTop
));


/***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
/*! \brief        Start the given core.
 *  \details      Starting a core is typically done by writing a special purpose register.
 *
 *  \param[in]    Core  The core which shall be started. Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_CoreStart,
(
  P2CONST(Os_Hal_CoreConfigType, AUTOMATIC, OS_CONST) Core
));


/***********************************************************************************************************************
 *  Os_Hal_SpinInit()
 **********************************************************************************************************************/
/*! \brief        Initializes the given spinlock as unlocked.
 *  \details      -
 *
 *  \param[in]    Spinlock  The spinlock to check. Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinInit,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
));


/***********************************************************************************************************************
 *  Os_Hal_SpinIsLocked()
 **********************************************************************************************************************/
/*! \brief        Returns whether the given spinlock is locked or not.
 *  \details      -
 *
 *  \param[in]    Spinlock  The spinlock to check. Parameter must not be NULL.
 *
 *  \retval       0   If the given lock is free.
 *  \retval       !0  If the given lock is in use.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinIsLocked,
(
  P2CONST(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
));


/***********************************************************************************************************************
 *  Os_Hal_SpinTryLock()
 **********************************************************************************************************************/
/*! \brief        Tries to get the lock.
 *  \details      -
 *
 *  \param[in,out]  Spinlock  The spinlock to lock. Parameter must not be NULL.
 *
 *  \retval       OS_HAL_SPINLOCKSUCCEEDED      If getting the lock succeeded.
 *  \retval       not OS_HAL_SPINLOCKSUCCEEDED  If getting the lock failed.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
));


/***********************************************************************************************************************
 *  Os_Hal_SpinUnlock()
 **********************************************************************************************************************/
/*! \brief        Release the given spinlock.
 *  \details      -
 *
 *  \param[in,out]  Spinlock  The spinlock to unlock. Parameter must not be NULL.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinUnlock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
));


/***********************************************************************************************************************
 *  Os_Hal_XSigInit()
 **********************************************************************************************************************/
/*! \brief          Initialize a cross core interrupt using the given configuration.
 *  \details
 *  The kernel uses this function during OS initialization to initialize cross core interrupts.
 *
 *  \param[in,out]  XSigConfig     XSignal hardware configuration. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 *  \pre            Concurrent access is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
));


/***********************************************************************************************************************
 *  Os_Hal_XSigTrigger()
 **********************************************************************************************************************/
/*! \brief          Trigger the given interrupt.
 *  \details
 *  The kernel uses this function to tell a foreign core, that a new cross core message is available.
 *
 *  \param[in,out]  XSigConfig     XSignal hardware configuration. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_XSigTrigger,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
));


/***********************************************************************************************************************
 *  Os_Hal_XSigAcknowledge()
 **********************************************************************************************************************/
/*! \brief          Tell the hardware, that we are handling the interrupt request.
 *  \details
 *  The kernel uses this function to clear pending timer interrupt flags.
 *
 *  \param[in]      XSigConfig      XSignal hardware configuration. Parameter must not be NULL.
 *
 *  \context        ISR2|OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 *  \pre            Concurrent access is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_XSigAcknowledge,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
));


/***********************************************************************************************************************
 *  Os_Hal_CoreIdle()
 **********************************************************************************************************************/
/*! \brief          Behavior during idle loop in kernel.
 *  \details        The kernel calls this function in case of idle state in a loop.
 *
 *  \context        TASK|OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreIdle, (void));


/***********************************************************************************************************************
 *  Os_Hal_CoreNop()
 **********************************************************************************************************************/
/*! \brief          Behavior during loops in kernel.
 *  \details        The kernel calls this function in a loop in case that the loop is expected to take some time.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreNop, (void));


/***********************************************************************************************************************
 *  Os_Hal_CoreFreeze()
 **********************************************************************************************************************/
/*! \brief          Freeze behavior.
 *  \details        The kernel calls this function to enter an endless loop in case of an assertion is not true.
 *                  Typically endless loop.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(void, OS_CODE, OS_NORETURN, Os_Hal_CoreFreeze, (void));


/***********************************************************************************************************************
 *  Os_Hal_Core_InitExceptions()
 **********************************************************************************************************************/
/*! \brief          Initializes exception handling.
 *  \details        This function initializes exception vector table. Depending on the core, this function might do
 *                  nothing. See internal design for more information.
 *  \context        OS_INTERNAL
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_Core_InitExceptions,
(
  P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config
));


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* OS_HAL_COREINT_H */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CoreInt.h
 **********************************************************************************************************************/


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
 *  \addtogroup Os_Hal_Compiler
 *  \{
 *
 *  \file
 *  \brief        Compiler abstraction for Texas Instruments compiler.
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

#ifndef OS_HAL_COMPILER_TIINT_H
# define OS_HAL_COMPILER_TIINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"

# include "Os_Core_Cfg.h"

# include "Os_Hal_Interrupt_Cfg.h"
# include "Os_Hal_Entry_Cfg.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/*! \brief      Declares a function with a compiler specific attribute.
 *  \details    The implementations prototype has to be created with OS_FUNC_ATTRIBUTE_DEFINITION.
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 */
#define OS_FUNC_ATTRIBUTE_DECLARATION(rettype, memclass, attribute, functionName, arguments) \
  attribute FUNC(rettype, memclass) functionName arguments


/*! \brief      Provides a function prototype for function definitions with a compiler specific attribute.
 *  \details    The declaration has to be created with OS_FUNC_ATTRIBUTE_DECLARE.
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 */
#define OS_FUNC_ATTRIBUTE_DEFINITION(rettype, memclass, attribute, functionName, arguments) \
  attribute FUNC(rettype, memclass) functionName arguments



/*! \brief      Provides a constant definition with a compiler specific attribute.
 *  \details    The attribute is only used for compiler without support of defining sections using pragma.
 *  \param[in]  constType     The type of the constant.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {alignment, ...}.
 *  \param[in]  constName     The constant name.
 */
#define OS_HAL_CONST_ATTRIBUTE_DEFINITION(constType, memclass, attribute, constName) \
  CONST(constType, memclass) constName


/*! \brief      Provides a constant declaration with a compiler specific attribute.
 *  \details    The attribute is only used for compiler without support of defining sections using pragma.
 *  \param[in]  constType     The type of the constant.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {alignment, ...}.
 *  \param[in]  constName     The constant name.
 */
#define OS_HAL_CONST_ATTRIBUTE_DECLARATION(constType, memclass, attribute, constName) \
  extern CONST(constType, memclass) constName


/*! \brief    Abstraction for HAL specific compiler attribute to prevent from inlining a function.
 *  \details  This attribute excludes certain functions from inlining within a C++ file, even if they are member
 *            functions defined inside a class,
 */
# define OS_HAL_NOINLINE    __attribute__((noinline))

/*! \brief    Force the compiler to inline a function.
 *  \details  This attribute forces the compiler to inline the specified function,
 *            regardless of the optimization strategy of the compiler itself.
 *            On some compilers you may be able to disable even this kind of inlining, using debugging options.
 */
# define OS_ALWAYS_INLINE   __attribute__((always_inline))


/*! \brief    This attribute tells the compiler that a function never returns.
 *  \details  Therefore any code following the function call may be considered dead code and optimized away.
              For example, this attribute can be used on the standard library functions exit(), abort(), and longjmp().
 */
# define OS_NORETURN


/*! \brief    This attribute forces inlining of all function calls in a function, including nested function calls.
 * \details   Whether the function itself is considered for inlining depends on its size and the current inlining parameters.
 */
# define OS_FLATTEN


/*! \brief    This attribute tells that a function has no side effects and will not access global data.
 *  \details  This can help the compiler to optimize code. Such functions do not examine any values except their arguments,
 *            and have no effects except the return value. This is just slightly more strict class than the OS_PURE attribute,
 *            since function is not allowed to read global memory.
 */
# define OS_NOSIDEEFFECTS


/*! \brief    This attribute tells that a function has no side effects, although it may read global data.
 *  \details  Tells the compiler that this function is pure: the state of the world is the same before and
 *            after a call to this function. A pure function may read memory, throw a C++ exception, or even enter an infinite loop,
 *            but a pure function may not modify global memory (for example, it may not write to a global variable, or write
 *            through a pointer). Marking a function as pure may improve the compiler's ability to perform alias analysis optimizations.
 */
# define OS_PURE


/*! \note     The specified function will not have prologue/epilogue sequences generated by the compiler.
 *            Return instruction is still needed. I'm not sure whether this is usable.
 */
# define OS_NAKED


/*! \note     The attribute to define sections.
 *            This is only required if the compiler does not support section definition using pragmas
 */
# define OS_HAL_SECTION_ATTRIBUTE(x)


/***********************************************************************************************************************
 *  Os_Hal_ReadBasepri()
 **********************************************************************************************************************/
/*! \brief           Returns the base priority mask.
 *  \details         -
 *  \return          Base priority mask.
 *  \context         ANY
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *  \pre             -
 **********************************************************************************************************************/
extern uint32 Os_Hal_ReadBasepri(void);


 /***********************************************************************************************************************
  *  Os_Hal_ReadControl()
  *********************************************************************************************************************/
 /*! \brief           Returns the core's CONTROL register.
  *  \details         -
  *  \return          CONTROL register value.
  *  \context         ANY
  *  \reentrant       TRUE
  *  \synchronous     TRUE
  *  \pre             -
  **********************************************************************************************************************/
extern uint32 Os_Hal_ReadControl(void);


/***********************************************************************************************************************
 *  Os_Hal_ReadPsr()
**********************************************************************************************************************/
/*! \brief            Returns the core's special-purpose program status registers (xPSR).
*  \details          -
*  \return   CurrentPsr  xPSR value.
*  \context          ANY
*  \reentrant        TRUE
*  \synchronous      TRUE
*  \pre              -
**********************************************************************************************************************/
extern uint32 Os_Hal_ReadPsr(void);


/***********************************************************************************************************************
 *  Os_Hal_DisableAndReturnPriorState()
 **********************************************************************************************************************/
/*! \brief           Disable core local interrupts and returns the state of interrupts prior to the operation.
 *  \details         -
 *  \return          PriorState  Previous interrupt state.
 *  \context         ANY
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *  \pre             -
 **********************************************************************************************************************/
extern uint32 Os_Hal_DisableAndReturnPriorState(void);

/***********************************************************************************************************************
 *  Os_Hal_ReadLinkRegister
 **********************************************************************************************************************/
/*! \brief        Returns the link register value.
 *  \details      -
 *  \return       Current Link register value.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
extern uint32 Os_Hal_ReadLinkRegister(void);

/***********************************************************************************************************************
 *  Os_Hal_DataMemoryBarrier()
 **********************************************************************************************************************/
/*! \brief        Executes data memory barrier instruction "dmb".
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_DataMemoryBarrier() asm("  dmb")


/***********************************************************************************************************************
 *  Os_Hal_DataSyncBarrier()
 **********************************************************************************************************************/
/*! \brief        Executes data sync barrier instruction "dsb".
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_DataSyncBarrier() asm("  dsb")


/***********************************************************************************************************************
 *  Os_Hal_InstructionSyncBarrier()
 **********************************************************************************************************************/
/*! \brief        Executes instruction sync barrier instruction "isb".
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_InstructionSyncBarrier() asm("  isb")

/***********************************************************************************************************************
 *  Os_Hal_ReadStackPointer()
 **********************************************************************************************************************/
/*! \brief        Returns the current SP register value.
 *  \details      -
 *  \return       Pointer to current stack value.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
extern uint32 Os_Hal_ReadStackPointer(void);

/***********************************************************************************************************************
 *  Os_Hal_Nop()
 **********************************************************************************************************************/
/*! \brief        Executes "no operation" instruction.
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_Nop()  __nop()


# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Os_Hal_SysCall()
 **********************************************************************************************************************/
/*! \brief        Bypass given pointer and execute SVC instruction.
 *  \details      -
 *  \param[in]    TrapPacket  Value which shall be passed in parameter register. Parameter must not be NULL.
 *                            (The parameter is passed in R0.)
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
#pragma SWI_ALIAS(Os_Hal_SysCall,0)
__attribute__((always_inline))
inline void Os_Hal_SysCall(volatile uint32 TrapPacket);


/***********************************************************************************************************************
 *  Os_Hal_WriteBasepri()
 **********************************************************************************************************************/
/*! \brief        Set the base priority mask.
 *  \details      -
 *  \param[in]    NewBasepri  Value which shall be written.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
__attribute__((noinline))
static void Os_Hal_WriteBasepri(volatile uint32 NewBasepri)
{
  asm("  msr BASEPRI, r0");
}


/***********************************************************************************************************************
 *  Os_Hal_WriteControl()
 **********************************************************************************************************************/
/*! \brief        Set the core's CONTROL register.
 *  \details      -
 *  \param[in]    NewControl  Value which shall be written.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
__attribute__((noinline))
static void Os_Hal_WriteControl(volatile uint32 NewControl)
{
  asm(" msr CONTROL, r0");
}


/***********************************************************************************************************************
 *  Os_Hal_Trigger_Context_SvCall()
 **********************************************************************************************************************/
/*! \brief        Triggers the SVCall with specific parameter for context switch
 *  \details      -
 *  \param[in]    NewSp    Stack Pointer to be used for context switch.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/

#pragma SWI_ALIAS(Os_Hal_Trigger_Context_SvCall,1)
__attribute__((always_inline))
inline void Os_Hal_Trigger_Context_SvCall(volatile uint32 NewSp);


/***********************************************************************************************************************
 *  Os_Hal_Disable()
 **********************************************************************************************************************/
/*! \brief        Disables core local interrupts.
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
__attribute__((noinline))
static void Os_Hal_Disable()
{
  asm(" push {r0} \n"
      " mov r0, #" TOSTRING(OS_CFG_HAL_GLOBAL_DISABLE_LEVEL) "\n"
      " msr basepri, r0 \n"
      " pop {r0} ");
}


/***********************************************************************************************************************
 *  Os_Hal_Enable()
 **********************************************************************************************************************/
/*! \brief        Enables core local interrupts.
 *  \details      -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
__attribute__((noinline))
static void Os_Hal_Enable()
{
  asm(" push {r0} \n"
      " mov r0, #0 \n"
      " msr basepri, r0 \n"
      " pop {r0} ");
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* General function / macros */

/***********************************************************************************************************************
 *  Os_Hal_CLZ()
 **********************************************************************************************************************/
/*! \brief        Takes a 32-bit integer argument and returns the count of leading zeros, which is a number from 0 to
 *                32.
 *  \details      -
 *  \param[in]    X  Integer which shall be inspected.
 *  \return       Count of leading zeros.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_CLZ(X)                      __clz(X)


/***********************************************************************************************************************
 *  Os_Hal_LoadExclusive()
 **********************************************************************************************************************/
/*! \brief        Uses a load exclusive synchronization instruction for the address, which attempts to mark the address
 *                for exclusive access by the current processor, and return the result.
 *  \details      -
 *  \param[in]    Addr  Address value to read from. Parameter must not be NULL.
 *  \return       Value at Addr.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_LoadExclusive(Addr)          __ldrex((void*)Addr)


/***********************************************************************************************************************
 *  Os_Hal_StoreExclusive()
 **********************************************************************************************************************/
/*! \brief        Uses a store exclusive synchronization instruction, which attempts to store the value to the address.
 *                Returns 0 on success or 1 otherwise.
 *  \details      -
 *  \param[out]   Addr    Address to write to. Parameter must not be NULL.
 *  \param[in]    Value   Value which shall be written.
 *  \retval       0       Success.
 *  \retval       1       No success.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
# define Os_Hal_StoreExclusive(Addr, Value)  __strex(Value, (void*)Addr)


#if (OS_CFG_THREAD_FPU_CONTEXT_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  Os_Hal_StoreFpuRegisters()
 **********************************************************************************************************************/
/*! \fn           void Os_Hal_StoreFpuRegisters(uint32 FpuRegs)
 *  \brief        Stores FPU registers (s16-s31) to given address.
 *  \details      -
 *  \param[in]    FpuRegs  Address where to store. Parameter must not be NULL.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
void Os_Hal_StoreFpuRegisters(uint32 FpuRegs);

/***********************************************************************************************************************
 *  Os_Hal_RestoreFpuRegisters()
 **********************************************************************************************************************/
/*! \fn           void Os_Hal_RestoreFpuRegisters(uint32 FpuRegs)
 *  \brief        Restores FPU registers (s16-s31) from given address.
 *  \details      -
 *  \param[in]    FpuRegs  Address where to load from. Parameter must not be NULL.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
void Os_Hal_RestoreFpuRegisters(uint32 FpuRegs);
#endif


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* OS_HAL_COMPILER_TIINT_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Compiler_TiInt.h
 *********************************************************************************************************************/

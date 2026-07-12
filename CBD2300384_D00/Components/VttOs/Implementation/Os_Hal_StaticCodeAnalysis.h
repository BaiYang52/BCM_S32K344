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
 * \ingroup    Os_Hal
 * \addtogroup Os_Hal_Compiler
 * \brief      Compiler abstraction of the ANSI C compiler used for MISRA and ReviewHelper
 *
 * \{
 *
 * \file       Os_Hal_StaticCodeAnalysis.h
 * \brief      Compiler abstraction of the ANSI C compiler used for MISRA and ReviewHelper
 * \details
 *
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_STATICCODEANALYSIS_H
# define OS_HAL_STATICCODEANALYSIS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Os_Hal_Core_Types.h"


/**********************************************************************************************************************
 *  QAC PRAGMAS
 *********************************************************************************************************************/
# if !defined(OS_STATIC_CODE_ANALYSIS_FETA)                                                                             /* COV_OS_STATICCODEANALYSIS */
#  pragma PRQA_NO_RETURN Os_ErrKernelPanic
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! \brief  Abstraction for compiler specific attributes, required by core */
# define OS_ALWAYS_INLINE
# define OS_NORETURN
# define OS_FLATTEN
# define PS_PURE
# define OS_LIKELY(x)                (x)
# define OS_UNLIKELY(x)              (x)

/*! \brief  CANoe specific defines */
# ifndef CANOEAPI_VERSION                                                                                               /* COV_OS_HALVTTCANOEAPIVERSION_003 */
#  define CANOEAPI_VERSION                     1
#  define CANOEAPI_ECUACTION_NOACTION          0
#  define CANOEAPI_ECUACTION_LOAD              1
#  define CANOEAPI_ECUACTION_UNLOAD            2
#  define CANOEAPI_ECUACTION_INITMEASUREMENT   3
#  define CANOEAPI_ECUACTION_STARTMEASUREMENT  4
#  define CANOEAPI_ECUACTION_STOPMEASUREMENT   5
#  define CANOEAPI_ECUACTION_SWITCHON          6
#  define CANOEAPI_ECUACTION_SWITCHOFF         7
#  define CANOEAPI_ECUACTION_GOTOSLEEP         8
#  define CANOEAPI_ECUACTION_WAKEUP            9
#  define CANOEAPI_ECUACTION_RESET            10
# endif


# define CANOEEMUPROCESSOR_ROLLBACK           1
# define CANOEEMUPROCESSOR_IDLE               1
# define CANOEEMUPROCESSOR_INVALIDIRQ         1


# define OS_CANOE_PIT_COUNT                 254
# define OS_CANOE_HRT_COUNT                 254


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
# define OS_FUNC_ATTRIBUTE_DECLARATION(rettype, memclass, attribute, functionName, arguments)                           \
         attribute FUNC(rettype, memclass) functionName arguments                                                       /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */


/*! \brief      Provides a function prototype for function definitions with a compiler specific attribute.
 *  \details    The declaration has to be created with OS_FUNC_ATTRIBUTE_DECLARE.
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 */
# define OS_FUNC_ATTRIBUTE_DEFINITION(rettype, memclass, attribute, functionName, arguments)                            \
         attribute FUNC(rettype, memclass) functionName arguments                                                       /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */

# define CANOEAPI_USERTIMER2_MICROSECONDS(x)   ((sint64)(x) * 1000)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Dummy long jump buffer. */
typedef uint32 jmp_buf[42];                                                                                             /* PRQA S 4605 */ /* MD_Os_Hal_Rule_21.2_SetJmpLib */

# if (CANOEAPI_VERSION == 1)                                                                                            /* COV_OS_HALVTTCANOEAPIVERSION_003 */
typedef sint32 CANoeAPI_UserTimer2_Handle;

typedef void(*CANoeAPI_UserTimer2_Handler)(sint32 InterruptSource, const Os_Hal_CANoeNameType* Name);
# endif


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/*! \brief        Triggers a side effect and returns a non constant value.
 *  \details      Can be used to avoid MISRA findings about functions with no side effects and MISRA detecting constant
 *                expressions in conditions.
 *  \context      OS_INTERNAL
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 */
static inline uint32 Os_Hal_DummySideEffect(void);
static inline uint32 Os_Hal_DummySideEffect(void)                                                                       /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  static uint32 x = 0;
  x++;
  return x;
}

static void assert(uint32 expression);                                                                                  /* PRQA S 4602 */ /* MD_Os_Hal_Rule_21.2_SetJmpLib */
static void assert(uint32 expression)                                                                                   /* PRQA S 3219, 4602 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule_21.2_SetJmpLib */
{
  OS_IGNORE_UNREF_PARAM(expression);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static uint32 setjmp(jmp_buf env);                                                                                      /* PRQA S 4602 */ /* MD_Os_Hal_Rule_21.2_SetJmpLib */
static uint32 setjmp(jmp_buf env)                                                                                       /* PRQA S 3219, 4602 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule_21.2_SetJmpLib */
{
  env[0] = 42u;                                                                                                         /* SBSW_OS_HAL_AWA_TESTSUITE */
  return Os_Hal_DummySideEffect();
}

static void longjmp(jmp_buf env, uint32 val);                                                                           /* PRQA S 4604 */ /* MD_Os_Hal_Rule_21.2_SetJmpLib */
static void longjmp(jmp_buf env, uint32 val)                                                                            /* PRQA S 3219, 4603 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule_21.2_SetJmpLib */
{
  OS_IGNORE_UNREF_PARAM(val);                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  env[0] = 42u;                                                                                                         /* SBSW_OS_HAL_AWA_TESTSUITE */
}

# if (CANOEAPI_VERSION == 1)                                                                                            /* COV_OS_HALVTTCANOEAPIVERSION_003 */

static void CANoeAPI_ConsumeTicks(sint32 ticks);
static void CANoeAPI_ConsumeTicks(sint32 ticks)                                                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(ticks);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  (void) Os_Hal_DummySideEffect();
}

static void CANoeAPI_AtomicBegin(void);
static void CANoeAPI_AtomicBegin(void)                                                                                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
}

static void CANoeAPI_AtomicEnd(void);
static void CANoeAPI_AtomicEnd(void)                                                                                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
}

static CANoeAPI_UserTimer2_Handle CANoeAPI_UserTimer2_Create(const Os_Hal_CANoeNameType* name);
static CANoeAPI_UserTimer2_Handle CANoeAPI_UserTimer2_Create(const Os_Hal_CANoeNameType* name)                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(name);                                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (CANoeAPI_UserTimer2_Handle)0;
}

static void CANoeAPI_UserTimer2_SetCallbackFunction
(
     CANoeAPI_UserTimer2_Handle index,
     CANoeAPI_UserTimer2_Handler f,
     sint32 userparam
);
static void CANoeAPI_UserTimer2_SetCallbackFunction                                                                     /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
(
     CANoeAPI_UserTimer2_Handle index,
     CANoeAPI_UserTimer2_Handler f,
     sint32 userparam
)
{
  OS_IGNORE_UNREF_PARAM(index);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(f);                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(userparam);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static sint32 CANoeAPI_UserTimer2_SetCyclic(CANoeAPI_UserTimer2_Handle index, sint64 nanoSeconds);
static sint32 CANoeAPI_UserTimer2_SetCyclic(CANoeAPI_UserTimer2_Handle index, sint64 nanoSeconds)                       /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(index);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(nanoSeconds);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)0;
}

static void CANoeAPI_UserTimer2_Destroy(CANoeAPI_UserTimer2_Handle index);
static void CANoeAPI_UserTimer2_Destroy(CANoeAPI_UserTimer2_Handle index)                                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(index);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeAPI_SetInterrupt(sint32 irqNumber);
static void CANoeAPI_SetInterrupt(sint32 irqNumber)                                                                     /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeAPI_Printf(const char* format, ...);
static void CANoeAPI_Printf(const char* format, ...)                                                                    /* PRQA S 1337, 3219 */ /* MD_Os_Hal_Rule17.1_1337_StaticCodeAnalysis, MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(format);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static const char* CANoeAPI_FileNameOfDLL(void);
static const char* CANoeAPI_FileNameOfDLL(void)                                                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  static char x = '\0';
  return &x;
}

# endif /* # if (CANOEAPI_VERSION == 1) */


static sint32 CANoeEmuProcessor_SetInterruptLevel(sint32 level);
static sint32 CANoeEmuProcessor_SetInterruptLevel(sint32 level)                                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(level);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  (void) Os_Hal_DummySideEffect();
  return (sint32)0;
}

static sint32 CANoeEmuProcessor_GetInterruptLevel(void);
static sint32 CANoeEmuProcessor_GetInterruptLevel(void)                                                                 /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)Os_Hal_DummySideEffect();
}

static sint32 CANoeEmuProcessor_GetCurrentLevel(void);
static sint32 CANoeEmuProcessor_GetCurrentLevel(void)                                                                   /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static void CANoeEmuProcessor_RequestInterrupt(sint32 irqNumber);
static void CANoeEmuProcessor_RequestInterrupt(sint32 irqNumber)                                                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ClearInterrupt(sint32 irqNumber);
static void CANoeEmuProcessor_ClearInterrupt(sint32 irqNumber)                                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static sint32 CANoeEmuProcessor_IsInterruptRequested(sint32 irqNumber);
static sint32 CANoeEmuProcessor_IsInterruptRequested(sint32 irqNumber)                                                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)Os_Hal_DummySideEffect();
}

static sint32 CANoeEmuProcessor_MaskInterrupt(sint32 irqNumber);
static sint32 CANoeEmuProcessor_MaskInterrupt(sint32 irqNumber)                                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)0;
}

static sint32 CANoeEmuProcessor_UnmaskInterrupt(sint32 irqNumber);
static sint32 CANoeEmuProcessor_UnmaskInterrupt(sint32 irqNumber)                                                       /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)0;
}

static sint32 CANoeEmuProcessor_IsInterruptMasked(sint32 irqNumber);
static sint32 CANoeEmuProcessor_IsInterruptMasked(sint32 irqNumber)                                                     /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)Os_Hal_DummySideEffect();
}

static void CANoeEmuProcessor_ConfigureISR(sint32 irqNumber, void (*f)(void));
static void CANoeEmuProcessor_ConfigureISR(sint32 irqNumber, void (*f)(void))                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(f);                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ConfigureInterruptPriority(sint32 irqNumber, sint32 level);
static void CANoeEmuProcessor_ConfigureInterruptPriority(sint32 irqNumber, sint32 level)                                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(level);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ConfigureInterruptName(sint32 irqNumber, const Os_Hal_CANoeNameType* name);
static void CANoeEmuProcessor_ConfigureInterruptName(sint32 irqNumber, const Os_Hal_CANoeNameType* name)                /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(name);                                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static sint32 CANoeEmuProcessor_GetCurrentInterruptNumber(void);
static sint32 CANoeEmuProcessor_GetCurrentInterruptNumber(void)                                                         /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static void CANoeEmuProcessor_DisableInterruptsAtStartOfISR(uint8 disable);
static void CANoeEmuProcessor_DisableInterruptsAtStartOfISR(uint8 disable)                                              /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(disable);                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static uint8 CANoeEmuProcessor_DisableInterrupts(void);
static uint8 CANoeEmuProcessor_DisableInterrupts(void)                                                                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
  return (uint8)0;
}

static uint8 CANoeEmuProcessor_EnableInterrupts(void);                                                                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
static uint8 CANoeEmuProcessor_EnableInterrupts(void)                                                                   /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
  return (uint8)0;
}

static uint8 CANoeEmuProcessor_InterruptsEnabled(void);
static uint8 CANoeEmuProcessor_InterruptsEnabled(void)                                                                  /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (uint8)0;
}

static sint32 CANoeEmuProcessor_GetCurrentTask(void);
static sint32 CANoeEmuProcessor_GetCurrentTask(void)                                                                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static void CANoeEmuProcessor_SetTaskToRun(sint32 taskNr);
static void CANoeEmuProcessor_SetTaskToRun(sint32 taskNr)                                                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(taskNr);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  (void) Os_Hal_DummySideEffect();
}

static void CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(sint32 taskNr);
static void CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(sint32 taskNr)                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(taskNr);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_RollbackStack(void);
static void CANoeEmuProcessor_RollbackStack(void)                                                                       /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
}

static sint32 CANoeEmuProcessor_RollbackStackOfTask(sint32 taskNr);
static sint32 CANoeEmuProcessor_RollbackStackOfTask(sint32 taskNr)                                                      /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  uint32 dummyReturnVal;
  OS_IGNORE_UNREF_PARAM(taskNr);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /*! Internal comment removed.
 *
 *
 *
 *
 * */
  dummyReturnVal = (Os_Hal_DummySideEffect() > 42uL);                                                                   /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  return (sint32)(dummyReturnVal);
}

static sint32 CANoeEmuProcessor_RollbackStackOfIsr(sint32 level);
static sint32 CANoeEmuProcessor_RollbackStackOfIsr(sint32 level)                                                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(level);                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (sint32)0;
}

static void CANoeEmuProcessor_ConfigureNumberOfTasks(sint32 numberOfTasks);
static void CANoeEmuProcessor_ConfigureNumberOfTasks(sint32 numberOfTasks)                                              /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(numberOfTasks);                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ConfigureTaskFunction(sint32 taskNr, void (*f)(void) );
static void CANoeEmuProcessor_ConfigureTaskFunction(sint32 taskNr, void (*f)(void) )                                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(taskNr);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(f);                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ConfigureTaskName(sint32 taskNr, Os_Hal_CANoeNameType* name);
static void CANoeEmuProcessor_ConfigureTaskName(sint32 taskNr, Os_Hal_CANoeNameType* name)                              /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_Stubs */
{
  OS_IGNORE_UNREF_PARAM(taskNr);                                                                                        /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(name);                                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_ConfigureTaskEndHook( void (*taskEndHook)(void) );
static void CANoeEmuProcessor_ConfigureTaskEndHook( void (*taskEndHook)(void) )                                         /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_Stubs */
{
  OS_IGNORE_UNREF_PARAM(taskEndHook);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_GotoSleep(void);
static void CANoeEmuProcessor_GotoSleep(void)                                                                           /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
 /* Empty */
}

static void CANoeEmuProcessor_PowerOff(void);
static void CANoeEmuProcessor_PowerOff(void)                                                                            /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
 /* Empty */
}

static uint32 CANoeEmuProcessor_InterlockedExchange(volatile uint32* destination, uint32 exchange);
static uint32 CANoeEmuProcessor_InterlockedExchange(volatile uint32* destination, uint32 exchange)                      /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_Stubs */
{
  OS_IGNORE_UNREF_PARAM(destination);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(exchange);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (uint32)0xFF;
}

static uint32 CANoeEmuProcessor_InterlockedCompareExchange(volatile uint32* destination,                                /* PRQA S 3673 */ /* MD_Os_Hal_Rule8.13_3673_Stubs */
                                                                     uint32 exchange, uint32 comperand);
static uint32 CANoeEmuProcessor_InterlockedCompareExchange(volatile uint32* destination,                                /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_Stubs */
                                                                     uint32 exchange, uint32 comperand)
{
  OS_IGNORE_UNREF_PARAM(destination);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(exchange);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(comperand);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (uint32)0;
}

static void CANoeEmuProcessor_SetStartAddr(sint32 core, void (*f)(void));                                               /* PRQA S 3673 */ /* MD_Os_Hal_Rule8.13_3673_Stubs */
static void CANoeEmuProcessor_SetStartAddr(sint32 core, void (*f)(void))                                                /* PRQA S 3219, 3673 */ /* MD_Os_Rule2.1_3219, MD_Os_Hal_Rule8.13_3673_Stubs */
{
  OS_IGNORE_UNREF_PARAM(core);                                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(f);                                                                                             /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_StartCore(sint32 core);
static void CANoeEmuProcessor_StartCore(sint32 core)                                                                    /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(core);                                                                                          /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuProcessor_RequestInterruptOnCore(sint32 coreNumber, sint32 irqNumber);
static void CANoeEmuProcessor_RequestInterruptOnCore(sint32 coreNumber, sint32 irqNumber)                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(coreNumber);                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static sint32 CANoeEmuProcessor_GetCoreNumber(void);
static sint32 CANoeEmuProcessor_GetCoreNumber(void)                                                                      /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  /*! Internal comment removed.
 *
 *
 *
 *
 *
 * */
  uint32 dummyCore = (Os_Hal_DummySideEffect() > 42uL);                                                                 /* PRQA S 4404 */ /* MD_Os_C90BooleanCompatibility */
  return (sint32)(dummyCore);
}

static void CANoeEmuProcessor_InitInterruptRouter(void);
static void CANoeEmuProcessor_InitInterruptRouter(void)                                                                 /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  (void) Os_Hal_DummySideEffect();
}

static void CANoeEmuProcessor_SetInterruptRoutingMask(sint32 irqNumber, uint32 routingMask);
static void CANoeEmuProcessor_SetInterruptRoutingMask(sint32 irqNumber, uint32 routingMask)                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(routingMask);                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static uint32 CANoeEmuProcessor_GetInterruptRoutingMask(sint32 irqNumber);
static uint32 CANoeEmuProcessor_GetInterruptRoutingMask(sint32 irqNumber)                                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (uint32)0;
}

static sint32 CANoeEmuSystemTimer_GetNumberOfTimers(void);
static sint32 CANoeEmuSystemTimer_GetNumberOfTimers(void)                                                               /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static void CANoeEmuSystemTimer_WriteCycleTime(uint32 cycleTime);
static void CANoeEmuSystemTimer_WriteCycleTime(uint32 cycleTime)                                                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(cycleTime);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static uint32 CANoeEmuSystemTimer_ReadCycleTime(void);
static uint32 CANoeEmuSystemTimer_ReadCycleTime(void)                                                                   /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (uint32)0;
}

static void CANoeEmuSystemTimer_WriteIrqNumber(sint32 irqNumber);
static void CANoeEmuSystemTimer_WriteIrqNumber(sint32 irqNumber)                                                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static sint32 CANoeEmuSystemTimer_ReadIrqNumber(void);
static sint32 CANoeEmuSystemTimer_ReadIrqNumber(void)                                                                   /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static sint32 CANoeEmuHRTimer_GetNumberOfTimers(void);
static sint32 CANoeEmuHRTimer_GetNumberOfTimers(void)                                                                   /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  return (sint32)0;
}

static uint32 CANoeEmuHRTimer_GetCounterValue(sint32 hrtIndex);
static uint32 CANoeEmuHRTimer_GetCounterValue(sint32 hrtIndex)                                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (uint32)0x10;
}

static void CANoeEmuHRTimer_SetCompareValue(sint32 hrtIndex, uint32 expirationTime);
static void CANoeEmuHRTimer_SetCompareValue(sint32 hrtIndex, uint32 expirationTime)                                     /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(expirationTime);                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static uint32 CANoeEmuHRTimer_GetCompareValue(sint32 hrtIndex);
static uint32 CANoeEmuHRTimer_GetCompareValue(sint32 hrtIndex)                                                          /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (uint32)0;
}

static void CANoeEmuHRTimer_SetIrqNumber(sint32 hrtIndex, sint32 irqNumber);
static void CANoeEmuHRTimer_SetIrqNumber(sint32 hrtIndex, sint32 irqNumber)                                             /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(irqNumber);                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static void CANoeEmuHRTimer_SetSecondsPerTick(sint32 hrtIndex, float64 duration);
static void CANoeEmuHRTimer_SetSecondsPerTick(sint32 hrtIndex, float64 duration)                                        /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  OS_IGNORE_UNREF_PARAM(duration);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}

static float64 CANoeEmuHRTimer_GetSecondsPerTick(sint32 hrtIndex);
static float64 CANoeEmuHRTimer_GetSecondsPerTick(sint32 hrtIndex)                                                       /* PRQA S 3219 */ /* MD_Os_Rule2.1_3219 */
{
  OS_IGNORE_UNREF_PARAM(hrtIndex);                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (float64)0;
}


#endif /* OS_HAL_STATICCODEANALYSIS_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_StaticCodeAnalysis.h
 *********************************************************************************************************************/

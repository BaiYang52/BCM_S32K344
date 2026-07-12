/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

#ifdef MULTIPLE_IMAGE
	#define RAM_DATA_INIT_ON_ALL_CORES
#endif

/* If this is a secodary core, it shall wait for the MSCM clock to be initialized */
#if defined(CORE1)
	#define NO_MSCM_CLOCK_INIT
#endif

#define MAIN_CORE 0
#define MCME_CTL_KEY    0x402DC000
#define MCME_PRTN1_PUPD 0x402DC304
#define MCME_PRTN1_STAT 0x402DC308
#define MCME_PRTN1_COFB0_CLKEN 0x402DC330
#define MCME_PRTN1_COFB0_STAT 0x402DC310
#define MCME_MSCM_REQ (1 << 24)
#define MCME_KEY 0x5AF0
#define MCME_INV_KEY 0xA50F
#define VTOR_REG  0xE000ED08
#define CM7_ITCMCR 0xE000EF90
#define CM7_DTCMCR 0xE000EF94

    MODULE  ?cstartup

    ;; Forward declaration of sections.
    SECTION CSTACK:DATA:NOROOT(3)

    EXTERN  main
    EXTERN  SystemInit
    EXTERN  init_data_bss
    EXTERN  RESET_CATCH_CORE
    EXTERN  VTABLE
    EXTERN __Stack_start_c0
    EXTERN __Stack_start_c1
    EXTERN __RAM_INIT
    EXTERN __INT_SRAM_START
    EXTERN __INT_SRAM_END
    EXTERN __ITCM_INIT
    EXTERN __INT_ITCM_START
    EXTERN __INT_ITCM_END
    EXTERN __DTCM_INIT
    EXTERN __INT_DTCM_START
    EXTERN __INT_DTCM_END
    EXTERN rwBlock$$Base
    EXTERN rwBlock$$Limit
    EXTERN rwMcalDataBlock$$Base
    EXTERN rwMcalDataBlock$$Limit
    EXTERN ramCodeBlock$$Base
    EXTERN ramCodeBlock$$Limit
    EXTERN dataShBlock$$Base
    EXTERN dataShBlock$$Limit
    EXTERN dataNcBlock$$Base
    EXTERN dataNcBlock$$Limit
    EXTERN constNcBlock$$Base
    EXTERN constNcBlock$$Limit
    EXTERN cacheable_data_init$$Base
    EXTERN cacheable_data_init$$Limit
    EXTERN cacheable_mcal_data_init$$Base
    EXTERN cacheable_mcal_data_init$$Limit
    EXTERN cacheable_code_init$$Base
    EXTERN cacheable_code_init$$Limit
    EXTERN non_cacheable_data_init$$Base
    EXTERN non_cacheable_data_init$$Limit
    EXTERN non_cacheable_const_init$$Base
    EXTERN non_cacheable_const_init$$Limit
    EXTERN intcVectorBlock$$Base
    EXTERN interrupt_vector_init$$Base
    EXTERN interrupt_vector_init$$Limit
    EXTERN shareable_data_init$$Base
    EXTERN shareable_data_init$$Limit
    EXTERN .bss$$Base
    EXTERN .bss$$Limit
    EXTERN .mcal_bss$$Base
    EXTERN .mcal_bss$$Limit
    EXTERN .mcal_bss_no_cacheable$$Base
    EXTERN .mcal_bss_no_cacheable$$Limit
    EXTERN .mcal_shared_bss$$Base
    EXTERN .mcal_shared_bss$$Limit
    EXTERN startup_go_to_user_mode
    EXTERN __CORE0_VTOR
    EXTERN __CORE1_VTOR
    PUBLIC  __INIT_TABLE
    PUBLIC  __ZERO_TABLE
    PUBLIC  _core_loop
    PUBLIC  _start

SBAF_BOOT_MARKER   EQU (0x5AA55AA5)
CM7_0_ENABLE_SHIFT EQU (0)
CM7_1_ENABLE_SHIFT EQU (1)
CM7_0_ENABLE       EQU (1)
#ifndef CM7_1_ENABLE
CM7_1_ENABLE EQU (0)
#endif
CM7_0_VTOR_ADDR    EQU (__CORE0_VTOR)
CM7_1_VTOR_ADDR    EQU (__CORE1_VTOR)
XRDC_CONFIG_ADDR   EQU (0)
LF_CONFIG_ADDR     EQU (0)

/* Table for copying and zeroing */
/* Copy table:
  - Table entries count
    - entry one ram start
    - entry one rom start
    - entry one rom end
    ...
    - entry n ram start
    - entry n rom start
    - entry n rom end
  Zero Table:
    - Table entries count
      - entry one ram start
      - entry one ram end
*/
    SECTION .init_table:CONST(4)
__INIT_TABLE
    DCD 7
    DCD rwBlock$$Base
    DCD cacheable_data_init$$Base
    DCD cacheable_data_init$$Limit
    DCD rwMcalDataBlock$$Base
    DCD cacheable_mcal_data_init$$Base
    DCD cacheable_mcal_data_init$$Limit
    DCD ramCodeBlock$$Base
    DCD cacheable_code_init$$Base
    DCD cacheable_code_init$$Limit
    DCD dataNcBlock$$Base
    DCD non_cacheable_data_init$$Base
    DCD non_cacheable_data_init$$Limit
    DCD constNcBlock$$Base
    DCD non_cacheable_const_init$$Base
    DCD non_cacheable_const_init$$Limit
    DCD intcVectorBlock$$Base
    DCD interrupt_vector_init$$Base
    DCD interrupt_vector_init$$Limit
    DCD dataShBlock$$Base
    DCD shareable_data_init$$Base
    DCD shareable_data_init$$Limit
__INIT_TABLE_END
    SECTION .zero_table:CONST(4)
__ZERO_TABLE
    DCD 4
    DCD .bss$$Base
    DCD .bss$$Limit
    DCD .mcal_bss$$Base
    DCD .mcal_bss$$Limit
    DCD .mcal_bss_no_cacheable$$Base
    DCD .mcal_bss_no_cacheable$$Limit
    DCD .mcal_shared_bss$$Base
    DCD .mcal_shared_bss$$Limit
__ZERO_TABLE_END

    THUMB
    SECTION .core_loop:CODE
_core_loop
    NOP
    NOP
    NOP
    NOP
    B _core_loop

    SECTION .boot_header:CONST(4)
    DCD SBAF_BOOT_MARKER /* IVT marker */
    DCD (CM7_0_ENABLE << CM7_0_ENABLE_SHIFT) | (CM7_1_ENABLE << CM7_1_ENABLE_SHIFT) /* Boot configuration word */
    DCD 0 /* Reserved */
    DCD CM7_0_VTOR_ADDR /* CM7_0 Start address */
    DCD 0 /* Reserved */
    DCD CM7_1_VTOR_ADDR /* CM7_1 Start address */
    DCD 0 /* Reserved */
    DCD 0 /* Reserved */
    DCD XRDC_CONFIG_ADDR /* XRDC configuration pointer */
    DCD LF_CONFIG_ADDR /* Lifecycle configuration pointer */
    DCD 0 /* Reserved */


/************************************************************************/
/* Autosar synopsis of startup code (See MCU Specification):            */
/*                                                                      */
/*   Before the MCU driver can be initialized, a basic initialization   */
/*   of the MCU has to be executed. This MCU specific initialization is */
/*   typically executed in a start-up code. The start-up code of the    */
/*   MCU shall be executed after power up and any kind of micro-        */
/*   controller reset. It shall perform very basic and microcontroller  */
/*   specific start-up initialization and shall be kept short, because  */
/*   the MCU clock and PLL is not yet initialized. The start-up code    */
/*   shall cover MCU specific initialization, which is not part of      */
/*   other MCU services or other MCAL drivers. The following steps      */
/*   summarizes basic functionality which shall be included in the      */
/*   start-up code. They are listed for guidance, because some          */
/*   functionality might not be supported. No code will be found in     */
/*   case.                                                              */
/************************************************************************/

    THUMB
    PUBWEAK Reset_Handler
    SECTION .startup:CODE:REORDER:NOROOT(2)
_start:
Reset_Handler:
/*****************************************************/
/* Skip normal entry point as nothing is initialized */
/*****************************************************/
    CPSID   I               ; Mask interrupts
    ;; Init the rest of the registers
    LDR     R1,=0
    LDR     R2,=0
    LDR     R3,=0
    LDR     R4,=0
    LDR     R5,=0
    LDR     R6,=0
    LDR     R7,=0
    MOV     R8,R7
    MOV     R9,R7
    MOV     R10,R7
    MOV     R11,R7
    MOV     R12,R7

#ifndef NO_MSCM_CLOCK_INIT
InitMSCMClock:
  /* If the MSCM clock is enabled, skip this sequence */
  LDR r0, =MCME_PRTN1_COFB0_STAT
  LDR r1, [r0]
  LDR r2, =MCME_MSCM_REQ
  AND r1, r1, r2
  CMP r1, #0
  BNE SetVTOR

  /* Enable clock in PRTN1 */
  LDR R0, =MCME_PRTN1_COFB0_CLKEN
  LDR R1, [R0]
  LDR R2, =MCME_MSCM_REQ
  ORR R1, R2
  STR R1, [R0]

  /* Set PUPD field */
  LDR R0, =MCME_PRTN1_PUPD
  LDR R1, [R0]
  LDR R2, =1
  ORR R1, R2 
  STR R1, [R0]

  /* Trigger update */
  LDR R0, =MCME_CTL_KEY
  LDR R1, =MCME_KEY
  STR R1, [R0]
  LDR R1, =MCME_INV_KEY
  STR R1, [R0]
#endif
/* Check MSCM clock in PRTN1 */
WaitForClock:
  LDR r0, =MCME_PRTN1_COFB0_STAT
  LDR r1, [r0]
  LDR r2, =MCME_MSCM_REQ
  AND r1, r1, r2
  CMP r1, #0
  BEQ WaitForClock


/*******************************************************************/
/* NXP Guidance 1 - Init registers to avoid lock-step issues */
/* N/A                                                             */
/*******************************************************************/

/*******************************************************************/
/* NXP Guidance 2 - MMU Initialization for CPU               */
/*  TLB0 - PbridgeB                                                */
/*  TLB1 - Internal Flash                                          */
/*  TLB2 - External SRAM                                           */
/*  TLB3 - Internal SRAM                                           */
/*  TLB4 - PbridgeA                                                */
/*******************************************************************/

/******************************************************************/
/* Autosar Guidance 1 - The start-up code shall initialize the    */
/* base addresses for interrupt and trap vector tables. These base*/
/* addresses are provided as configuration parameters or          */
/* linker/locator setting.                                        */
/******************************************************************/

SetVTOR:
/* relocate vector table to RAM */
    LDR R0, =VTOR_REG
    LDR R1, =VTABLE
    STR  r1,[r0]

/******************************************************************/
/* Autosar Guidance 2 - The start-up code shall initialize the    */
/* interrupt stack pointer, if an interrupt stack is              */
/* supported by the MCU. The interrupt stack pointer base address */
/* and the stack size are provided as configuration parameter or  */
/* linker/locator setting.                                        */
/*                                                                */
/******************************************************************/


/******************************************************************/
/* Autosar Guidance 3 - The start-up code shall initialize the    */
/* user stack pointer. The user stack pointer base address and    */
/* the stack size are provided as configuration parameter or      */
/* linker/locator setting.                                        */
/******************************************************************/
    /*GetCoreID*/
    LDR  r0, =0x40260004
    LDR  r1,[r0]

    LDR  r0, =MAIN_CORE
    CMP  r1,r0
    BEQ	 SetCore0Stack
    B SetCore1Stack

SetCore0Stack:
    /* set up stack; r13 SP*/
    LDR  r0, =__Stack_start_c0
    MSR MSP, r0
    B DisableSWT0

SetCore1Stack:
  /* set up stack; r13 SP*/
  LDR  r0, =__Stack_start_c1
  MSR MSP, r0
#ifdef RAM_DATA_INIT_ON_ALL_CORES
  B RamInit
#else
  B DTCM_Init /* SWT1 clock is disabled at startup */
#endif

/******************************************************************/
/* Autosar Guidance 4 - If the MCU supports context save          */
/* operation, the start-up code shall initialize the memory which */
/* is used for context save operation. The maximum amount of      */
/* consecutive context save operations is provided as             */
/* configuration parameter or linker/locator setting.             */
/*                                                                */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 5 - The start-up code shall ensure that the   */
/* MCU internal watchdog shall not be serviced until the watchdog */
/* is initialized from the MCAL watchdog driver. This can be      */
/* done for example by increasing the watchdog service time.      */
/*                                                                */
/******************************************************************/

/* Note from manual: For any operation to be performed on an SWT  */
/* instance, its respective core must be enabled.                 */

DisableSWT0:
  LDR  r0, =0x40270010
  LDR  r1, =0xC520
  STR  r1, [r0]
  LDR  r1, =0xD928
  STR  r1, [r0]
  LDR  r0, =0x40270000
  LDR  r1, =0xFF000040
  STR  r1, [r0]
  B    RamInit
  
DisableSWT1:
  LDR  r0, =0x4046C010
  LDR  r1, =0xC520
  STR  r1, [r0]
  LDR  r1, =0xD928
  STR  r1, [r0]
  LDR  r0, =0x4046C000
  LDR  r1, =0xFF000040
  STR  r1, [r0]
  B    RamInit

/******************************************************************/
/* Autosar Guidance 13 - The start-up code shall initialize a     */
/* minimum amount of RAM in order to allow proper execution of    */
/* the MCU driver services and the caller of these services.      */
/******************************************************************/
RamInit:
    /* Initialize SRAM ECC */
    LDR  R0, =__RAM_INIT
    CMP  R0, #0
    /* Skip if __SRAM_INIT is not set */
    BEQ SRAM_LOOP_END
    LDR R1, =__INT_SRAM_START
    LDR R2, =__INT_SRAM_END
    
    SUBS    R2, R1
    SUBS    R2, #1
    BLE SRAM_LOOP_END

    MOVS    R0, #0
    MOVS    R3, #0
SRAM_LOOP:
    STM R1!, {R0, R3}
    SUBS R2, #8
    BGE SRAM_LOOP
SRAM_LOOP_END:

DTCM_Init:
    /* Initialize DTCM ECC */
    LDR  R0, =__DTCM_INIT
    CMP  R0, #0
    /* Skip if __DTCM_INIT is not set */
    beq DTCM_LOOP_END
    /* Enable TCM */
    LDR r1, =CM7_DTCMCR
    LDR r0, [r1]
    LDR r2, =0x1
    ORR r0, r2
    STR r0, [r1]

    LDR R1, =__INT_DTCM_START
    LDR R2, =__INT_DTCM_END
    
    SUBS    R2, R1
    SUBS    R2, #1
    BLE DTCM_LOOP_END

    MOVS    R0, #0
    MOVS    R3, #0
DTCM_LOOP:
    STM R1!, {R0, R3}
    SUBS R2, #8
    BGE DTCM_LOOP
DTCM_LOOP_END:

ITCM_Init:
    /* Initialize ITCM ECC */
    LDR  R0, =__ITCM_INIT
    CMP  R0, #0
    /* Skip if __TCM_INIT is not set */
    BEQ ITCM_LOOP_END

    /* Enable TCM */
    LDR r1, =CM7_ITCMCR
    LDR r0, [r1]
    LDR r2, =0x1
    ORR r0, r2
    STR r0, [r1]

    LDR R1, =__INT_ITCM_START
    LDR R2, =__INT_ITCM_END
    
    SUBS    R2, R1
    SUBS    R2, #1
    BLE ITCM_LOOP_END

    MOVS    R0, #0
    MOVS    R3, #0
ITCM_LOOP:
    STM R1!, {R0, R3}
    SUBS R2, #8

    BGE ITCM_LOOP
ITCM_LOOP_END:

DebuggerHeldCoreLoop:
  LDR  R0, =RESET_CATCH_CORE
  LDR  R0, [r0]
  LDR  R1, =0x5A5A5A5A
  CMP  R0, R1
  BEQ  DebuggerHeldCoreLoop

/************************/
/* Erase ".bss Section" */
/************************/
_DATA_INIT:
#ifndef RAM_DATA_INIT_ON_ALL_CORES
    /* If this is the primary core, initialize data and bss */
    LDR  R0, =0X40260004
    LDR  R1,[R0]

    LDR  R0, =MAIN_CORE
    CMP  R1,R0
    BEQ	 _INIT_DATA_BSS
    B    __SYSTEM_INIT
#endif

_INIT_DATA_BSS:
  BL init_data_bss



/******************************************************************/
/* Autosar Guidance 6 - If the MCU supports cache memory for data */
/* and/or code, it shall be initialized and enabled in the        */
/* start-up code.                                                 */
/*                                                                */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 7 - The start-up code shall initialize MCU    */
/* specific features of internal memory like memory protection.   */
/*                                                                */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 8 - If external memory is used, the memory    */
/* shall be initialized in the start-up code. The start-up code   */
/* shall be prepared to support different memory configurations   */
/* depending on code location. Different configuration options    */
/* shall be taken into account for code execution from            */
/* external/internal memory.                                      */
/* N/A - external memory is not used                              */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 9 - The settings of the different memories    */
/* shall be provided to the start-up code as configuration        */
/* parameters.                                                    */
/* N/A - all memories are already configured                      */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 10 - In the start-up code a default           */
/* initialization of the MCU clock system shall be performed      */
/* including global clock prescalers.                             */
/******************************************************************/
__SYSTEM_INIT:
  bl SystemInit

/******************************************************************/
/* Autosar Guidance 5 - The start-up code shall ensure that the   */
/* MCU internal watchdog shall not be serviced until the watchdog */
/* is initialized from the MCAL watchdog driver. This can be      */
/* done for example by increasing the watchdog service time.      */
/*                                                                */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 11 - The start-up code shall enable           */
/* protection mechanisms for special function registers(SFR's),   */
/* if supported by the MCU.                                       */
/* N/A - will be handled by Autosar OS                            */
/******************************************************************/

/******************************************************************/
/* Autosar Guidance 12 - The start-up code shall initialize all   */
/* necessary write once registers or registers common to several  */
/* drivers where one write, rather than repeated writes, to the   */
/* register is required or highly desirable.                      */
/******************************************************************/

/*********************************/
/* Set the small ro data pointer */
/*********************************/


/*********************************/
/* Set the small rw data pointer */
/*********************************/

/******************************************************************/
/* Call Main Routine                                              */
/******************************************************************/
_MAIN:
  CPSIE i
  BL startup_go_to_user_mode
  BL main

/******************************************************************/
/* Init runtime check data space                                  */
/******************************************************************/
    PUBLIC MCAL_LTB_TRACE_OFF
MCAL_LTB_TRACE_OFF:
    nop

#ifdef CCOV_ENABLE
    /* code coverage is requested */
    bl ccov_main
#endif

    /*BKPT #1 - removed to avoid debug fault being escalated to hardfault when debugger is not attached or on VDK*/ /* last instruction for the debugger to dump results data */
    PUBLIC _end_of_eunit_test
_end_of_eunit_test:
    b .

#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
    PUBLIC startup_getControlRegisterValue
startup_getControlRegisterValue:
    mrs r0, CONTROL
    bx r14

    PUBLIC startup_getAipsRegisterValue
startup_getAipsRegisterValue:
    mrs r0, IPSR
    bx r14
#endif

    END
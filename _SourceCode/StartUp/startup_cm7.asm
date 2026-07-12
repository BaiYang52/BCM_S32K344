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

#define VTABLE 0x442000
#define __STACK_SP 0x20010000

#define __INT_RETAIN_SRAM_START  0x20400000
#define __INT_RETAIN_SRAM_END 0x20402000

#define __INT_SRAM_START 0x20400000
#define __INT_SRAM_END 0x2044FDFF
#define __INT_ITCM_START 0x0
#define __INT_ITCM_END  0x8000

#define __INT_DTCM_START 0x20000000
#define __INT_DTCM_END  0x20010000

#define MCME_MODE_STATE 0x402DC00C
#define MG_RGM_DES  0x4028C000
 
    ;;EXTERN  OsCfg_Hal_Core_OsCore0_VectorTable
;;#define __vector_table OsCfg_Hal_Core_OsCore0_VectorTable
;;    PUBLIC __vector_table
;;    PUBLIC  _start
    MODULE  ?cstartup

    ;; Forward declaration of sections.
    SECTION CSTACK:DATA:NOROOT(3)

    EXTERN  Brs_PreMainStartup
    EXTERN  BrsMainExceptionStartup


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
    SECTION .brsStartup:CODE:REORDER:NOROOT(2)
;;_start:
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

    B SetCore0Stack

SetCore0Stack:
    /* set up stack; r13 SP*/
    LDR  r0, =__STACK_SP
    MSR MSP, r0
    B DisableSWT0


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


/******************************************************************/
/* Autosar Guidance 13 - The start-up code shall initialize a     */
/* minimum amount of RAM in order to allow proper execution of    */
/* the MCU driver services and the caller of these services.      */
/******************************************************************/
//#define KEEP_STANDBY_RAM_ONLY_BY_WAKEUP
RamInit:
#ifdef KEEP_STANDBY_RAM_ONLY_BY_WAKEUP
    LDR r1, =MCME_MODE_STATE  // if prev_mode == standby
    LDR r0, [r1]
    CMP r0, #0x01
    BEQ StandbyInit
#else
    LDR r1, =MG_RGM_DES      // if reset source == POR
    LDR r0, [r1]
    TST r0, #0x01
    BEQ StandbyInit
#endif
ResetInit:
    LDR R1, =__INT_RETAIN_SRAM_START
    LDR R2, =__INT_SRAM_END
    B StartRamInit
StandbyInit:
    LDR R1, =__INT_SRAM_START
    LDR R2, =__INT_SRAM_END
StartRamInit:
    SUBS    R2, R1
    SUBS    R2, #1

    MOVS    R0, #0
    MOVS    R3, #0
SRAM_LOOP:
    STM R1!, {R0, R3}
    SUBS R2, #8
    BGE SRAM_LOOP
SRAM_LOOP_END:


DTCM_Init:
    /* Enable TCM */
    LDR r1, =CM7_DTCMCR
    LDR r0, [r1]
    LDR r2, =0x1
    ORR r0, r2
    STR r0, [r1]

    ldr r1, =__INT_DTCM_START
    ldr r2, =__INT_DTCM_END

    subs    r2, r1
    subs    r2, #1
    ble DTCM_LOOP_END

    movs    r0, #0
    movs    r3, #0
DTCM_LOOP:
    stm r1!, {r0,r3}
    subs r2, #8
    bge DTCM_LOOP
DTCM_LOOP_END:

ITCM_Init:
    /* Enable TCM */
    LDR r1, =CM7_ITCMCR
    LDR r0, [r1]
    LDR r2, =0x1
    ORR r0, r2
    STR r0, [r1]

    ldr r1, =__INT_ITCM_START
    ldr r2, =__INT_ITCM_END

    subs    r2, r1
    subs    r2, #1
    ble ITCM_LOOP_END

    movs    r0, #0
    movs    r3, #0
ITCM_LOOP:
    stm r1!, {r0,r3}
    subs r2, #8
    bge ITCM_LOOP
ITCM_LOOP_END:
/************************/
/* Erase ".bss Section" */
/************************/
_DATA_INIT:
#ifndef RAM_DATA_INIT_ON_ALL_CORES
    /* If this is the primary core, initialize data and bss */
    LDR  R0, =0X40260004
    LDR  R1,[R0]

    B    _MAIN
#endif

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

  BL Brs_PreMainStartup
  BL BrsMainExceptionStartup


    END

/******************************************************************/
/* End of startup                                                 */
/******************************************************************/

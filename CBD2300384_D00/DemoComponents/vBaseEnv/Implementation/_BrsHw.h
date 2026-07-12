/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  BrsHw.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for platform NXP S32K3xx
 *
 *  \brief Description:  This is the hardware specific header file for Vector Basic Runtime System (BRS).
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Description
 *  --------  ----------  ------  -------------------------------------------------------------------------------------
 *  01.00.00  2020-06-04  vismun  New branch for vBaseEnv 2.0, based on zBrs_S32K3xx BrsHw.c 01.00.00
 *  01.01.00  2020-10-08  visrgm  Review according to Brs_Template@Implementation[1.01.01],
 *                                adapted ports implementation
 *            2020-10-24  vismaa  Removed "AUTHOR IDENTITY"
 *  01.01.01  2020-11-04  visrgm  Reworked BrsHwModeEntryConfig(), BrsHwPllInitPowerOn() & Port init routines for
 *                                S32K3xx. Introduced use of BRSHW_PORT_MUX macro
 *                                Added entries for S32K3xx with S32K3XXCVB board and introduced use of BRSHW_PORT_MUX
 *            2020-11-05  visbwa  Review
 *  01.01.02  2021-02-04  visrgm  Added Muxing Ports for Flexray & Ethernet and introduced BRS_READ_COREID for Dualcore
 *  01.01.03  2021-02-15  visrgm  Corrections for CAN Clock settings
 *  01.02.00  2021-05-14  vismaa  Added GCCNXP
 *  01.02.01  2021-06-24  visbpz  Fixed register offset for SIUL2
 *  01.03.00  2022-03-02  visrgm  HALBE-5961: Removed support for S32K2TV
 *  01.03.02  2022-03-25  visrgm  HALBE-5985: Added IVT Boot Header, implemented brsPreAsmStartupHook for enabling of
 *                                MSCM module clock in startup code & changed PLL Group for S32K311 & S32K312 to C
 *  01.04.00  2022-04-13  visqli  HALBE-6669: Added new DerivativeGroups MWCT201XX and MWCT2D1XX
 *  02.00.00  2022-05-03  visqli  HALBE-6825: Update to Brs_Template 1.03
 *  02.00.02  2022-07-05  visrgm  HALBE-7264: Fix for ESCAN00112174 (usage of wrong startlabel in IVT)
 *  02.00.03  2022-08-11  visbwa  Update to Brs_Template 1.03.05; UsageType=external in ALM (DemoComponents concept)
 *  02.01.00  2022-09-27  visrgm  Added support for S32K3x8/S32K38x/S32K39x derivatives
 *  02.01.01  2022-10-07  visrgm  Several corrections for PLL & Port
 *  02.01.02  2022-10-11  visrgm  Introduced core specific implementation of BrsHwSoftwareResetECU(),
 *                                fixed BrsHwGetResetReasonStartup()
 *  02.01.03  2022-11-04  visrgm  Added disabling of recovery mode for functional resets
 *  02.01.04  2022-11-11  visrgm  Added TIMEBASE_CLOCK setting for 48MHz
 *  02.01.05  2022-12-19  visrgm  Added NOPs to brsPreAsmStartupHook
 *  02.01.06  2023-02-08  visjhr  Added code to set UAA bit to allow user mode access to flash memory (HALBE-7858)
 *  02.01.07  2023-02-14  visrgm  Added BrsHw_UnlockCores()
 *********************************************************************************************************************/

#ifndef _BRSHW_H_
#define _BRSHW_H_

/**********************************************************************************************************************
 *  MODULE VERSION
 *********************************************************************************************************************/
/*
 * Description: This is the BrsHw main and bug fix version. The version numbers are BCD coded.
 *              E.g. a main version of 1.23 is coded with 0x0123, a bug fix version of 9 is coded 0x09.
 */

/* This is the BrsHw template main and bug fix version, the platform BrsHw is reviewed to */
#define BRSHW_SOURCECODE_TEMPLATE_VERSION        0x0103u
#define BRSHW_SOURCECODE_TEMPLATE_BUGFIX_VERSION 0x05u

/* This is the BrsHw main and bug fix version */
#define BRSHW_VERSION        0x0201u
#define BRSHW_BUGFIX_VERSION 0x07u

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*
 * Description: Inclusion of generic ARM parts
 */
#include "ARMBrsHw_CortexM.h"

#if defined (BRS_ENABLE_PORT)
  #include "BrsHw_Ports.h"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
/* The following parameters are necessary for this platform, to be generated by BRS-Cfg5-Generator into BrsCfg.h */
#if !defined (BRS_OSC_CLK)
  #error "BRS CHECK: The parameter BRS_OSC_CLK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_TIMEBASE_CLOCK)
  #error "BRS CHECK: The parameter BRS_TIMEBASE_CLOCK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_PERIPH_CLOCK)
  #error "BRS CHECK: The parameter BRS_PERIPH_CLOCK is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_CPU_MAX_FREQUENCY)
  #error "BRS CHECK: The parameter BRS_CPU_MAX_FREQUENCY is missing! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_CPU_CORE_CORTEX_M7)
  #error "BRS CHECK: The parameter BRS_CPU_CORE_x is missing or not supported! Please check your BRS Cfg5 configuration."
#endif

#if !defined (BRS_CPU_CORE_AMOUNT)
  #error "BRS CHECK: The parameter BRS_CPU_CORE_AMOUNT is missing! Please check your BRS Cfg5 configuration."
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* part of ArmCommon */
/*
 * Description: This corresponds to the return value of BRS_READ_COREID and BrsHw_GetCore().
 *              BRSHW_INIT_CORE_ID is used e.g. in main@BrsMain to detect the valid core for HW initialization (boot core)
 */
#define BRSHW_DERIVATIVE_CORE0_ID     0x00
#define BRSHW_DERIVATIVE_CORE1_ID     0x01

#define BRSHW_INIT_CORE_ID  BRSHW_DERIVATIVE_CORE0_ID

#if defined (BRS_COMP_GHS) 
# if defined (BRS_DERIVATIVE_GROUP_S32K31X) || \
     defined (BRS_DERIVATIVE_GROUP_MWCT201XX) || \
     defined (BRS_DERIVATIVE_GROUP_S32K32X) || \
     defined (BRS_DERIVATIVE_GROUP_MWCT2D1XX) || \
     defined (BRS_DERIVATIVE_GROUP_S32K34X) || \
     defined (BRS_DERIVATIVE_GROUP_S32K3X8) || \
     defined (BRS_DERIVATIVE_GROUP_S32K38X) || \
     defined (BRS_DERIVATIVE_GROUP_S32K39X)
#  define BRS_READ_COREID(c)  BRS_ASM_EQU(mscm_cpuxnum_label_asm, 0x40260004) \
                              __as1(LDR   c, =mscm_cpuxnum_label_asm) \
                              __as1(LDR   c, [c])
# endif /*BRS_DERIVATIVE_GROUP_x*/
#endif /*BRS_COMP_x*/

/**********************************************************************************************************************
 * Compiler abstraction
 *********************************************************************************************************************/
/* part of ArmCommon */

/**********************************************************************************************************************
 *  BrsHW configuration
 *********************************************************************************************************************/
#define BRSHW_PRE_ASM_STARTUP_HOOK_AVAILABLE

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/
/*
 * Description: These constants are used to propagate the Versions over module boundaries.
 *              The version numbers are BCD coded. E.g. a sub version of 23 is coded with 0x23, 
 *              a bug fix version of 9 is coded 0x09.
 */
extern uint8 const kBrsHwMainVersion;
extern uint8 const kBrsHwSubVersion;
extern uint8 const kBrsHwBugfixVersion;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined (BRS_ENABLE_WATCHDOG)
/*****************************************************************************/
/* @brief      This function must be used to initialize the Watchdog.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwWatchdogInitPowerOn(void);
#endif /*BRS_ENABLE_WATCHDOG*/

#if defined (BRS_ENABLE_PLLCLOCKS)
/*****************************************************************************/
/* @brief      This function must be used to initialize the PLL.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPllInitPowerOn(void);
#endif /*BRS_ENABLE_PLLCLOCKS*/

#if defined (BRS_ENABLE_PORT)
/*****************************************************************************/
/* @brief      This function configures a port pin as input pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInput(brsHw_Port_PortType, brsHw_Port_MultiplexedType);

/*****************************************************************************/
/* @brief      This function configures a port pin as output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortOutput(brsHw_Port_PortType, brsHw_Port_ConfType);

/*****************************************************************************/
/* @brief      This function configures a port pin as input and output pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h and
 *             no DrvPort used for port pin initialization.
 * @param[in]  p - brsHw_Port_PortType, to be initialized.
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. driver testsuits).
 *****************************************************************************/
void BrsHwInitPortInputOutput(brsHw_Port_PortType, brsHw_Port_ConfType, brsHw_Port_MultiplexedType);

# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_START_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif
/*****************************************************************************/
/* @brief      This function sets the output level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as output pin with
 *             GPIO functionality.
 * @param[in]  p     - brsHw_Port_PortType, to be set,
 *             Level - level, port pin has to be set to
 *                     (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwPortInitPowerOn() and
 *             provided to external modules (e.g. BrsMainTogglePin()).
 *****************************************************************************/
void BrsHwPort_SetLevel(brsHw_Port_PortType, uint8);

/*****************************************************************************/
/* @brief      This function reads the input level of a port pin.
 * @pre        Port pin configurations available within BrsHw_Ports.h,
 *             no DrvPort used for port pin initialization and
 *             transferred port pin has to be initialized as input pin with
 *             GPIO functionality.
 * @param[in]  p - brsHw_Port_PortType, to be read.
 * @param[out] -
 * @return     Level, read from port pin
 *             (BRSHW_PORT_LOGIC_LOW or BRSHW_PORT_LOGIC_HIGH).
 * @context    Function is provided to external modules.
 *****************************************************************************/
uint8 BrsHwPort_GetLevel(brsHw_Port_PortType);
# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_STOP_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif

/*****************************************************************************/
/* @brief      This function must be used to initialize the used ports.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainInit() at power on initialization
 *****************************************************************************/
void BrsHwPortInitPowerOn(void);
#endif /*BRS_ENABLE_PORT*/

/* BrsHwDisableInterruptAtPowerOn() is part of ArmCommon */

/* BrsHw_ExceptionTable_Init() is part of ArmCommon */

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
/* This code is only needed for the first instance/executable in the system */
#define BRS_START_SEC_STARTUP_CODE
#include "Brs_MemMap.h"
/*****************************************************************************/
/* @brief      Get reset reason
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Reset reason
 * @context    Function is called from BrsMainStartup to determine if reset
 *             was triggered through software call (BrsHwSoftwareResetECU()).
 *             The result is stored by BrsMainStartup in the global variable
 *             brsMain_ResetReason. It should only be called once, during
 *             startup. The old API name BrsHwGetResetReason() is remapped
 *             to BrsMainGetResetReason.
 *****************************************************************************/
brsMain_ResetReasonType BrsHwGetResetReasonStartup(void);
#define BRS_STOP_SEC_STARTUP_CODE
#include "Brs_MemMap.h"

/* Wrapper from old BrsHwGetResetReason() function to new
 * BrsMainGetResetReason(). This is to prevent multiple selection of reset
 * reason registers, as this is not supported on all platforms.
 */
#define BrsHwGetResetReason() BrsMainGetResetReason()
#endif /*BRS_FIRST_EXECUTION_INSTANCE*/

/* BrsHwTime100NOP() is part of ArmCommon */

#if defined (BRS_ENABLE_SAFECTXSUPPORT)
/*****************************************************************************/
/* @brief      This API is used to enable hardware access in untrusted mode
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function must be called after all depending peripheral modules
 *             are supplied by proper clocks AND before the OS is started.
 *****************************************************************************/
void BrsHw_EnableHwAccess(void);
#endif /*BRS_ENABLE_SAFECTXSUPPORT*/

/*****************************************************************************/
/* @brief      This API is used to read the core ID of the actual running core
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     Core ID of the actual running core
 * @context    Function is e.g. called from main@BrsMain, to only call HW-init
 *             code once, on the boot core.
 *             In MultiCore setups, additional BRSHW_INIT_CORE_ID must be
 *             declared inside BrsHw.h, to configure the proper core ID value
 *             of that boot core.
 *****************************************************************************/
uint32 BrsHw_GetCore(void);

#define BRSHW_PLATFORM_SPECIFIC_SOFTWARERESETECU_AVAILABLE
/*****************************************************************************/
/* @brief      Restart ECU (issue a software reset or jump to startup code)
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. ECU state handling.
 *             Core specific implementation.
 *****************************************************************************/
void BrsHwSoftwareResetECU(void);

/* BrsHw_EnableInterrupt() is part of ArmCommon */

/* BrsHw_DisableInterrupt() is part of ArmCommon */

/* BrsHw_TriggerSoftwareInterrupt() is part of ArmCommon */

#if defined (BRS_ENABLE_FBL_SUPPORT)
#define BRSHW_DISABLE_ECC_AVAILABLE
/*****************************************************************************/
/* @brief      This API is used to disable the flash ECC error reporting,
 *             if this is necessary on the platform to grant access to it.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from FBL boot code, before flash is accessed.
 *****************************************************************************/
void BrsHwDisableEccErrorReporting(void);
#endif /*BRS_ENABLE_FBL_SUPPORT*/

#if defined (BRS_ENABLE_OS_MULTICORESUPPORT)
/*****************************************************************************/
/* @brief      This API is used as sync barrier. It is used to unlock cores
 *             that are locked in the Startup code. The implementation is only
 *             needed on platforms, where all cores are started automatically
 *             after a power-on reset. All additional cores are looped, until
 *             the boot core reaches this function.
 * @pre        -
 * @param[in]  uint32 coreId
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsMainStartup on the boot core.
 *****************************************************************************/
void BrsHw_UnlockCores(uint32);
#endif /*BRS_ENABLE_OS_MULTICORESUPPORT*/

#endif /*_BRSHW_H_*/

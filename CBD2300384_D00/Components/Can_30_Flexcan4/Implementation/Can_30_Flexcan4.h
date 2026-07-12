/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */

/*        STARTSINGLE_OF_MULTIPLE_COMMENT      */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*  \file     Can_30_Flexcan4.h
 *  \brief    Interface header file of the CAN-Driver
 *  \details  Declaration of functions, variables, and constants as user API.
 *
 *********************************************************************************************************************/

/* ***************************************************************************
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver       Author  Description
| ---------  ---  ------ -- --------------------------------------------------
| 2019-11-07  1.00.00  Rli   - CAN-790 [Flexcan4] Support of new platform
| 2020-01-28  1.01.00  Rli   - CAN-961 [Flexcan4] MSR4 R24 LL - new HW feature (extended Bittiming)
| 2020-04-30  1.01.01  Rli   - CAN-1447 [Flexcan4] S32K2/3 support
| 2020-12-10  1.01.02  Rli   - CAN-1566 [Flexcan4] Flexcan4: Solve review findings from LL Review for 1.01.01 release
| 2021-06-16  1.02.00  Rli   - CANHAL-188 [Drvcan Flexcan4] Support S32R455
|                            - CANHAL-408 [FlexCan4] MSR4 R26 Implementation
| 2021-08-12  1.03.00  Rli   - CANHAL-463  [DrvCan][Flexcan4] KW45 Implementation
|                            - CANHAL-467  [Drvcan][Flexcan4] KW45 Test Execution
| 2021-11-05  1.04.00  Rli   - CANHAL-704  [DrvCan][Flexcan4] Implementation (ASIL Level)
|                            - CANHAL-704  [Drvcan][Flexcan4] Test Execution (ASIL Level)
|                            - ESCAN00110599 Arm32 Flexcan4: CTRL1 register cannot be written / no CAN communication
|************************************************************************** */

/*****************************************************************************
|-----------------------------------------------------------------------------
|       C O R E    R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------  ----------------------------------------------------
| 2005-09-15 0.01.00 Bir  - Creation
| 2005-11-17 1.00.00 Bir  - Finish CANoe, M32C
|                         - Change include structure and naming
| 2005-11-23 1.01.00 Bir  - some minor changes
| 2005-12-01 1.02.00 Bir  - Global Interrupt handling via OSEK supported
| 2006-01-19 1.03.00 Bir  - Wrong handle found and DET occur in CanHL_30_Flexcan4_FullCanMsgReceived
|                         - C_DISABLE_RESET_CAN_BUSOFF set (baud rate out of CANoe)
| 2006-03-16 1.04.00 Wr   - Added channel compatibility defines
|                         - Removed local parameter canHwChannels
|                         - Changed order of CanLL_TransmitLocalParameter
|                           and CanLL_TxTransmissionLocalParameter in Can_30_Flexcan4_Write()
|                         - Rename CanWakeup() to CanHL_30_Flexcan4_WakeupHandling()
|                         - Added support for NEC V850 aFCAN
|                         - Added CanLL_GeneratedStructureH for platform C_COMP_GHS_V85X  
|                         - Added general return value defines
|                         - Defined APPL_30_FLEXCAN4_CAN_WAKEUP() to CanHL_30_Flexcan4_WakeUpHandling()
|                         - Mapping of Can_30_Flexcan4WakeUp() to Can_30_Flexcan4_SetControllerMode(Controller, CAN_T_WAKEUP)
| 2006-04-19 1.05.00 Wr   - Added V850 specific handling for transition to SLEEP
|                         - Add switches to enable OSEK ISR handling for platform C_COMP_GHS_V85X
| 2006-05-22 2.00.00 Bir  - Support for AutoSar 2.0
|                         - Can.h and can_types.h connected
|                         - Include structure update
|                         - DET - switch change
|                         - API changes Can_30_Flexcan4_Init(), Can_30_Flexcan4_Write()
|                         - add Can_30_Flexcan4_GetVersionInfo()
| 2006-06-06 2.01.00 Bir  - internal naming corrections for version defines
|                         - API for Det_ReportError changed (add parameter)
| 2006-06-12 2.02.00 Bir  - CaninitObject calculation changed
|                         - TriCore update for Interrupt priority defines
| 2006-06-12 2.03.00 Bir  - SetControllerMode return OK for unsupported STOP/START or SLEEP/WAKEUP
|                         - CaninitObject calculation changed
| 2006-06-12 2.04.00 Bir  - kCanNotSupported defined to 0x02 to avoid CAN_NOT_OK for that case
|                         - define for MEMORY_ROM (compatibility)
| 2006-07-05 2.05.00 Bir  - Can_HwHandleType as uint8
|                         - Review rework
| 2006-07-10 2.06.00 Bir  - Can_30_Flexcan4_Write try/send also in case busy
| 2006-08-15 2.07.00 Bir  - mapping of CanSingleGlobalInterruptX changed
| 2006-09-04 2.08.00 Bir  - minor change in Can.h: ControllerName length expand to 20
| 2006-09-04 2.09.00 Bir  - minor change for MISRA compliance
|                         - add canStatus for initialization info
|                         - minor changes for MAC71xx
|                         - No interrupt lock inside "CanHL_30_Flexcan4_BasicCanMsgReceived"
|                         - Add FullCAN TX
|                         - Add TX Polling
|                         - Add Hardware Loop and DEM call
|                         - DET call for initialized call of Can_30_Flexcan4_Init()
|                         - Add Can_30_Flexcan4_InitMemory function
| 2006-11-06 2.10.00 Bir  - review rework
| 2006-11-08 2.11.00 Ces/Bir - Add Platform Mcs12x
|                         - Add local variable Controller in function Can_30_Flexcan4_InitMemory()
|                         - Add define C_30_FLEXCAN4_ENABLE_COPY_TX_DATA in header
| 2006-11-09 2.12.00 Bir  - Prepare for Post-build structure (ORGANI switch "C_30_FLEXCAN4_HL_ENABLE_ASR_POSTBUILD_STRUCT" added)
| 2006-11-11 2.13.00 Pl   - Add Platform TMS470_DCAN
|                    Bir  - Move FullCAN Overrun notification token behind FullCAN Begin token
| 2006-11-15 2.14.00 Bir  - Post-build changes
|                         - Review changes
|                         - change CAN_HL_TX_OFFSET_HW_TO_LOG and logTxObjHandle value
|                         - ESCAN00018785: Send out of confirmation (TX Queue)
|                         - ESCAN00018915: Change multiple-include define
| 2007-01-15 2.15.00 Bir  - ESCAN00018896: wrong call to Dem_SetEventStatus
|                         - deactivate "CanOverrunNotification" due to no more need since ASR2.0
|                         - Support "Can_30_Flexcan4_MainFunction_BusOff"
|                         - call Can_30_Flexcan4_InitController in case of change mode from BUSOFF to START
|                         - Add SchM_Enter_Can_30_Flexcan4_), SchM_Exit_Can_30_Flexcan4_) / interrupt mapping over VStdLib or SchM
|                         - Change Version Scan Information
|                         - Remove V850 specific parts (shift to LL)
|                         - Remove Re-initialize DET report out of Can_30_Flexcan4_InitController()
| 2007-02-20 2.16.00 Bir  - minor improvements
|                         - ESCAN00019720: WAKEUP does not appear after set SLEEP mode
| 2007-03-09 2.20.00 Bir  - ESCAN00019889: Update to ASR2.1
|                         - ESCAN00019889: change calculation of CAN_HL_TX_OFFSET_HW_TO_LOG (because multiplexed TX)
|                         - remove FullCAN switches to secure Post-build functionality without compile with FullCANs
|                         - ESCAN00020129: Compiler error: identifier "i" is undefined
|                         - Reduce BasicCAN logical mailbox to real BasicCAN (DLC_FIFO include size of Basic CAN)
|                         - Reduce Unused mailboxes logical mailbox to amount of sections (DLC_FIFO include size)
|                         - Move all version defines from Proto.h to Can.h (license scanning)
|                         - Add CAN_AR_*_VERSION
|                         - remove PduId out of mailbox (old from CANIf)
|                         - set "C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2" if not defined "C_30_FLEXCAN4_ENABLE_INT_OSCAT1"
|                         - set DLC also for FullCAN TX objects
|                         - generated Tables: CanActiveSendPdu,CanActiveSendState (size matter at link time)
|                         - return not OK for try to set SLEEP mode but failed in HW
|                         - move multichannel API define to Can.h
|                         - set "C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2" if not defined "C_30_FLEXCAN4_ENABLE_INT_OSCAT1"
|                         - do "CanLL_30_Flexcan4_InitPowerOn" before "CanLL_30_Flexcan4_InitPowerOnChannelSpecific"
|                         - move OSEK-switches out of irq.c
|                         - add CanLL_30_Flexcan4_InitMemoryPowerOn token for special ll initialization
| 2007-08-28 2.21.00 Bir  - ESCAN00022112: RX Basic CAN used with wrong stop index
|                         - renaming "CaninitObject" to "Can_Init_CtrlIdx" (no runtime effect)
|                         - ESCAN00022168: Driver does not enter STOP mode for Sleep request (when SLEEP is not sup.)
|                         - ESCAN00022232: Add STOP-mode compatibility for FNOS-RP migration
|                         - ESCAN00022262: CanResetBusOffEnd() used with only one parameter
|                         - Add new Token "CanLL_RxFull_SearchHandle" for special HW-support (XGate)
|                         - ESCAN00022390: HW start-stop-index changed to RAM-table
| 2007-10-15 2.22.00 Bir  - ESCAN00022773: use of #ifndef instead of #if !defined
|                         - ESCAN00022774: improve runtime while change mode (STOP to SLEEP)
|                         - ESCAN00023040: minor improvements (add SPEC-1721, SPEC-1767, SPEC-1784, SPEC-1780, SPEC-1781)
|                         - ESCAN00023040: remove Interrupt locking in Can_30_Flexcan4_Write() due to CanIf do this
|                         - ESCAN00023038: Multiple ECU
|                         - ESCAN00023047: Add version check
|                         - ESCAN00023049: change API of Can_30_Flexcan4_Init() to one parameter, Add constant in Can_30_Flexcan4_InitController() + Add Can_InitStruct API
|                         - ESCAN00023127: compiler warnings due to dummy assignments (used to satisfy other compilers)
|                         - ESCAN00023265: remove brackets around Can_30_Flexcan4_EnterCritical
|                         - QAC / MISRA reworks
|                         - Add memory qualifiers
|                         - add v_def.h include (remove define of V_MEM..)
|                         - Review reworks
| 2007-12-14 2.23.00 Bir  - ESCAN00023442: Add extended Id support
|                         - some small changes for cancellation (cancel only less priority objects - more efficient)
|                         - protect Can_30_Flexcan4_MainFunction_Read against re-entrant (ASR specification)
|                         - ESCAN00024354: Ignore API call with controller deactivated by identity
|                         - review rework
| 2008-02-06 3.00.00 Bir  - ESCAN00024406: ASR3.0
| 2008-03-25 3.01.00 Bir  - Add dummy assignment in "ResetBusOffEnd" avoid compiler warnings
|                         - ESCAN00025672: review reworks
|                         - ESCAN00025664: Message send with wrong ID type
|                         - ESCAN00026087: Case sensitive naming for all File names (and includes)
|                         - ESCAN00026186: Extension for MSR3.0 Link-time checks
|                         - ESCAN00025957: No access on "DEM-events" via DEM created and exported constant values
|                         - Move compatibility section (switches needed earlier)
| 2008-05-14 3.02.00 Bir  - ESCAN00026859: V_MEMRAM0 not allowed for type cast
|                         - ESCAN00026861: Token external declaration added
|                         - ESCAN00026990: Add LL-Token to reject Remote frames
|                         - (see ES_CAN00029839) Change naming: "INTERRUPT" and "POLLING" add prefix "CAN_*"
|                         - (see ES_CAN00029826) Remove include of v_cfg.h and v_def.h (will be generated)
|                         - ESCAN00030521: Linker error because of missing Interrupt service routine
|                         - Move include section to top of file (need before version defines)
| 2008-06-30 3.03.00 Bir  - ESCAN00027986 +(ESCAN00025773): Code optimization (save ROM and runtime)
|                           + Start/StopLoop as macro
|                           + Max Loop counter define move to LL
|                           + Add some FullCAN switches to ORGANI Basic CAN controllers
|                           + Multiple Search for Basic CAN HW object
|                           + CanInterrupt Lock only in nested interrupt case (CAN_NESTED_INTERRUPTS)
|                           + remove some HW-start/stop indexes and there calculation use instead info from mailbox-table
|                         - ESCAN00028069: change controller amount to library capable setting
|                         - ESCAN00026596: Extension for MSR3.0 generator version checks
| 2008-08-19 3.03.01 Bir  - minor review changes
|                         - ESCAN00029385: change naming finishFullCan to finishRxFullCan (CAN-bedded compliance)
| 2008-09-11 3.04.00 Bir  - ESCAN00029942: Generator Version Check failed
| 2008-09-18 3.05.00 Bir  - ESCAN00029830: Add Link-time/Post-build configure CRC check
|                         - ESCAN00029434: rename "INTERRUPT" and "POLLING" switch
| 2008-10-15 3.06.00 Bir  - remove CAN_30_FLEXCAN4_HL_HW_x_STOPINDEX -> not used in HL and does not work for LL
|                         - ESCAN00029707: change transition from uninitialized->START->STOP to uninitialized->STOP
|                         - ESCAN00029129: Source of HW loop timer is not available to application
|                         - ESCAN00030728: move platform typedefs to Can.h
| 2008-10-28 3.06.01 Bir  - ESCAN00030926: lock interrupts while TX confirmation.
|                         - Add include protection for canproto.h
| 2008-10-28 3.07.00 Bir  - ESCAN00032401: CAN driver stays in an endless loop
|                         - ESCAN00032291: BUSOFF handling took some time with locked interrupts
|                         - ESCAN00032612: Support Individual Polling
|                         - ESCAN00032614: Add new memory qualifier V_MEMROM4_CFG for Post-build data access
| 2009-03-16 3.07.01 Bir  - ESCAN00033874: Add LL-token "CanLL_tCan_30_Flexcan4RxInfoStruct" for structure tCan_30_Flexcan4RxInfoStruct
| 2009-04-03 3.07.02 Bir  - ESCAN00034409: Add PRQA comments for MISRA justification
|                         - ESCAN00034551: Add support for reverse RX Basic CAN polling
|                         - ESCAN00034552: Optimize RX Basic CAN polling loop
|                         - ESCAN00034773: Add cancel TX in Hardware within TX polling task
| 2009-04-20 3.08.00 Bir  - ESCAN00034118: Add compiler abstraction
|                         - ESCAN00034119: Add memory abstraction
|                         - ESCAN00035606: Report DEM as DET "CAN_DEV_TIMEOUT_DETECT"
|                         - (see ES_CAN00035490) Report DEM as DET "CAN_DEV_TIMEOUT_DETECT"
|                         - ESCAN00036041: compiler warning label not referenced
| 2009-07-08 3.09.00 Bir  - ESCAN00034780: Confirmation without critical section
| 2009-07-22 3.09.01 Bir  - ESCAN00036593: compile error (variable declaration after assignment)
| 2009-08-03 3.09.02 Bir  - ESCAN00036822: TX data inconsistency
| 2009-08-24 3.09.03 Bir  - ESCAN00037276: Compiler warning "function declared implicitly"
| 2009-09-15 3.10.00 Bir  - ESCAN00037784: Add Generic Confirmation function
|                         - ESCAN00037789: Add feature RX Queue
|                         - change order of RX-DLC and "CanLL_RxBasicGetCANDataPtr" (support for mailbox in RAM)
|                         - ESCAN00038814: Wrong pointer class for TX data used
| 2010-01-19 3.11.00 Bir  - ESCAN00040258: Use Can_HwHandleType instead of uint8
|                         - ESCAN00040432: Code optimization for 1 controller systems
|                         - ESCAN00040433: Support dynamic FullCAN id
|                         - ESCAN00040234: remove v_cfg.h include (already done in can_30_Flexcan4_cfg.h)
| 2010-03-03 3.11.01 Bir  - small review reworks
|                         - ESCAN00041364: Optimize Runtime of Can_30_Flexcan4_SetControllerMode
|                         - ESCAN00041432: Declaration for ISRs missing in case of Full Polling with Individual Polling
| 2010-03-03 3.12.00 Bir  - ESCAN00041791: Suppress unexpected WAKEUP notification
|                         - ESCAN00041984: Support Common CAN
|                         - ESCAN00041996: Split Critical Sections
|                         - ESCAN00042333: Add Hardware assertion notification to DET
|                         - ESCAN00042459: Support WAKEUP-switch-off (no SLEEP mode - STOP mode instead)
|                         - ESCAN00043085: Compiler warnings due to missing cast
|                         - ESCAN00043224: New API Can_30_Flexcan4_GetStatus()
|                         - ESCAN00043250: Add Application Interrupt lock functions
|                         - canWakeUp() call changed to CAN_30_FLEXCAN4_WAKEUP() - used for LL
|                         - ESCAN00047602: compiler warning: "comparison between signed and unsigned"
|                         - ESCAN00044114: Stop sending messages after transition to STOP mode (and back to started)
| 2010-11-23 3.13.00 Bir  - ESCAN00043999: Support usage without v_def.h
|                         - ESCAN00044276: Reject unwanted ID types (software Filter)
|                         - ESCAN00045682: Compiler warning: dead assignment to "X" eliminated
|                         - ESCAN00045883: Can_30_Flexcan4_SetControllerMode doesn't return CAN_NOT_OK when called with invalid Transition parameter
|                         - ESCAN00045884: Can_30_Flexcan4_CheckWakeup doesn't raise the CAN_E_UNINIT development error
|                         - ESCAN00046363: Adapt AUTOSAR version check
|                         - rename "canInterruptCounter" to "canCanInterruptCounter" CBD compatibility
|                         - ESCAN00047875: Add support for partial network
|                         - ESCAN00047951: Compile error, if the feature "Optimize for one controller" is used
| 2011-03-11 4.00.00  Bir - ESCAN00043688: Support AUTOSAR 4.0 R1
|                         - ESCAN00043998: Add GenData checker
|                         - ESCAN00044010: Add Notification for Overrun detection
|                         - ESCAN00047111: Support RAM check
|                         - ESCAN00046778: Support Multiple Identity Configuration
| 2011-04-05 4.00.01  Bir - void cast DET call for MSR40 + review reworks (minors)
|                         - ESCAN00049572: RAM-check: communication is not deactivated
|                         - ESCAN00050862: EcuM_CheckWakeup is not called in case of WAKEUP Polling
|                         - ESCAN00051562: Remove the AUTOSAR Release Version Check
|                         - ESCAN00047929: Can_30_Flexcan4_Init() does not initialize CAN controllers - but Can Interface do this
|                         - ESCAN00049613: AR Version is not correct (2.2.0 instead of 2.2.1)
| 2011-06-21 4.01.00  Bir - ESCAN00050960: Provide API for Generic Pretransmit (AR3-1654 )
|                         - ESCAN00051102: Compiler warning: narrowing or signed-to-unsigned type conversion found: int to unsigned char
|                         - ESCAN00052324: Compile error if the feature "Optimize for one controller" and "RX Queue" is used
|                         - ESCAN00054829: Fault tolerant API: Do not throw DET in case of MultipleEcuConfig inactive controller for 'Can_30_Flexcan4_CheckWakeup'
|                         - ESCAN00053648: Wrong Define name used "CAN_DEACTIVATE_COMMUNICATION"
|                         - ESCAN00053270: AR3-1698: Add MISRA justifications in source code for automatic generation of MISRA reports
|                         - ESCAN00053263: AR3-2069: Remove non-SchM code for critical section handling
|                         - ESCAN00054038: Compiler warning: variable "canApiContext" was set but never used
|                         - ESCAN00054813: Add Mirror functionality
| 2011-11-21 4.01.01  Bir - ESCAN00055001: Support Transition HW-Loops for MSR30
|                         - ESCAN00055076: MISRA compliance
| 2011-12-14 4.01.02  Bir - ESCAN00055566: Include of DET with camel case spelling
| 2011-12-15 4.01.03  Bir - ESCAN00055589: SPEC-1761
|                         - ESCAN00056004: Add Code Trace ability to AutoSar SWS
|                         - Add explicit type casts to satisfy COSMIC compilers (MCS12x)
|                         - ESCAN00056185: Change Version Check
| 2012-01-20 4.02.00  Bir - ESCAN00055076: MISRA compliance
|                         - Use !CAN_USE_VECTOR_IF instead of VGEN_GENERATOR_GENY (also support CFG5 in mixed systems)
|                         - ESCAN00057438: AR3-2383: Implement handling of call to Dem_ReportErrorStatus
|                         - ESCAN00057164: WAKEUP while Re-Initialization ASR3.2.1
|                         - ESCAN00054830: Can_30_Flexcan4_SetControllerMode() return failed
| 2012-05-31 4.02.01  Bir - ESCAN00059203: Compiler error: missing variable canHwChannel in Can_30_Flexcan4_CleanUpSendState()
|                         - ESCAN00059265: Compiler error: hardware loop index is not available as define for API
| 2012-06-12 4.03.00  Bir - ESCAN00057437: AR3-2374: Implement Silent BSW
|                         - ESCAN00059412: AR4-135: support MSR 4 R5
|                         - ESCAN00059488: Compiler error: identifier "Can_ConfigDataPtr" is undefined
|                         - ESCAN00060494: Compiler error: SchM_Enter_Can((area)) causes Compiler Error with external SchM Usage
| 2012-09-03 4.03.01  Bir - Improve MISRA compliance
|                         - ESCAN00061352: Endless Loop
| 2012-09-03 4.04.00  Bir - ESCAN00061042: AR3-2378: Implement AMD time measurement support
|                         - ESCAN00060586: AR3-2446: Update Code analysis and change analysis UUIDs (like described in Silent design)
|                         - ESCAN00060554: AR3-2443: Implement Silent Checks in BSW module code
|                         - Add support for FullCAN TX cancellation only
|                         - ESCAN00062679: Add source defines according to MICROSAR template source file
| 2012-12-10 4.04.01  Bir - Rework Silent BSW
| 2013-01-28 4.04.02  Bir - ESCAN00064474: Modify Hook_On (code can be removed by integration ORGANI)
| 2013-02-02 4.05.00  Bir - optimize token StopReinit usage
|                         - move some global compatibility defines from proto.h to Can.h
|                         - ESCAN00063652: RxQueue extension to 16bit size
|                         - ESCAN00064324: AR4-325: Implement Post-Build Loadable
|                         - ESCAN00066278: Incorrect comment description for function Can_30_Flexcan4_CheckWakeup / Can_Cbk_CheckWakeup
| 2013-06-11 4.05.01  Bir - ESCAN00067936: Can_30_Flexcan4_GetStatus wrongly returns CAN_STATUS_INCONSISTENT
|                         - Add QAC deviations for 6010, 6030, 6050, 6080
| 2013-06-12 4.06.00  Tvi/Bir - ESCAN00069107: AR4-397: Implement support for CAN-FD according to ASR4.1.1
|                         - ESCAN00070197: Compiler warning: variable Can_ConfigDataPtr was declared but never referenced
|                         - ESCAN00068900: Compiler warning
|                         - ESCAN00067711: AR4-328 / RTM measurement R8
|                         - ESCAN00070625: Adapt usage of CanResetBusOffEnd
|                         - ESCAN00071048: Support null_ptr data in Can_30_Flexcan4_Write() for DLC==0
|                         - Add DOXYGEN comments to be used by API scanner
|                         - move some global compatibility defines from proto.h to Can.h
| 2013-10-25 4.06.01  Bir - R9 version / some minor MISRA improvements
|                         - ESCAN00073272: Support Can_30_Flexcan4_InitController() failure in LL
|                         - ESCAN00073676: Its not possible to switch off MIRROR mode for reception when using RxQueue
|                         - ESCAN00073516: MIRROR_MODE add CAN interrupt lock around confirmation
| 2014-06-23 4.06.02  Bir - ESCAN00076374: Add TX Request cancellation in case of HW-loop break
| 2014-06-23 4.07.00  Bir - R10 ComStackLibAbtraction
|                         - ESCAN00074371: AR4-457: Support CAN-FD mode 2
| 2014-07-01 5.00.00  Bir - R11
|                         - ESCAN00076631: Post-build selectable (+loadable)
|                         - ESCAN00076498: CAN FD mode2
|                         - ESCAN00076634: Multiple CAN driver support
|                         - ESCAN00078089: RAM Check also for emulated SLEEP
|                         - ESCAN00079232: rename canProto.h to Can_30_Flexcan4_Local.h
| 2014-11-24 5.00.01  Bir - coverage improvements
|                         - ESCAN00079480: support GENy (ComStackLib abstraction)
| 2014-12-14 5.01.00  Bir - ESCAN00079794: CAN-FD configured padding
|                         - ESCAN00080453: mask out FD-Bit in GenericPrecopy
|                         - ESCAN00080757: Limit DLC to 8 for Classic CAN RX-Frames
|                         - Support TxIndirectionHwToLog and LogToHw (platforms that use hwHandles for TX shadow buffers)
|                         - ESCAN00081930: DET error (CAN_INITCTR_ID, CAN_E_PARAM_CONTROLLER) occur
|                         - ESCAN00082533: BasicCAN message cancelled out of hardware but is not configured
| 2015-01-19 5.02.00  Bir - ESCAN00080306: FEAT-427 SafeBSW
|                           ESCAN00081793: FEAT-1275 SafeBSW step2
|                           + Merge API and Core history and move it to Can.h
|                           + add code design comments / rework comments in general
|                           + rework DET and SilentDet handling (SilentDet is now DET, no more MACRO)
|                           + split large functions in INLINE functions to reduce CYC
|                           ESCAN00073798: disable CAN interrupts while polling
|                           ESCAN00082042: API description Appl_GenericPrecopy
|                           ESCAN00082274: FEAT-1379: Optimized copy operation for large data
|                           ESCAN00082688: DET error (CAN_MAINFCT_*_ID, CAN_E_PARAM_CONTROLLER) occur
| 2015-07-30 5.02.01  Bir - remove goto/labels for HL - add it for LL
|                           shift CanLL_DefinesDataTypesStructsUnions from Can.c to Can_30_Flexcan4_Local.h
|                           typedef Can_ControllerConfigPtrType from Can_Cfg.h in case of postbuild
|                           include det.h only for report level not detection (from 5.02.00)
|                           ESCAN00084263, ESCAN00084264: reject illegal DLC
| 2015-11-09 5.02.02  Bir - ESCAN00086345: BasicCAN RX message not received
| 2015-10-27 5.03.00  Bir - ESCAN00085666: Gateway - multiple BasicCAN Tx
|                           Add CanHL_30_Flexcan4_StopReinit, CanHL_30_Flexcan4_RxBasicCanPolling, CanHL_30_Flexcan4_RxFullCanPolling
|                           remove Copyright info from Core use LL Copyright instead
| 2015-12-21 5.03.01  Bir - ESCAN00087253: preprocessor directives error in CanHL_30_Flexcan4_WakeupTransition() or CanHL_30_Flexcan4_SleepTransition()
| 2016-01-15 5.03.02  Bir - ESCAN00087518: Illegal pointer access when driver is not initialized
|                           test coverage improvements / minor MISRA improvements
|                           remove some _COMMEN_T postfixes for ORGANI (HW-layout may be TOOL dependent)
| 2016-01-29 5.04.00  Bir - test coverage improvements / minor MISRA improvements
|                           ESCAN00088335: FEAT-598: Extended RAM Check / SelfDiag
|                           ESCAN00087810: FEAT-1688: SafeBSW Step 4
|                           ESCAN00089399: Compiler error: 0=0 assignment
| 2016-04-14 5.04.01  Bir - review rework from 5.04.00
|                           rework traceability, rework coverage comments
|                           ESCAN00089754: Compile error overrun notification by DET without DET enabled (MSR401 only)
| 2016-06-21 5.05.00  Bir - ESCAN00089632: FEAT-1908: Access to Interrupt Control Register via OS
|                           ESCAN00089592: FEAT-1513: Release of CAN-FD
|                           FEAT-1602: Release of MultiBasicCAN-Tx (Gateway)
| 2016-08-24 5.05.01  Bir - ESCAN00091600: support MultiBasicCanTx with HW size > 1
| 2016-08-25 5.05.02  Bir - minor design improvement because of SafetyCheck
|                           ESCAN00091764: mode transition will be fully synchronous
| 2016-10-13 5.05.03  Bir - Add infix for local data and code (test without static to stimulate internal interfaces)
|                         - some minor COV comment improvements
| 2017-02-17 5.05.04  Bir - ESCAN00094060: Define CAN_H renamed (linux reuse)
|                           add infix to CAN_DUMMY_STATEMENT
|                           add coverage comment for Flexcan2 usage
|                           split in Can_30_Flexcan4_MainFunction_Write -> add CanHL_30_Flexcan4_TxTaskConfirmationHandling and CanHL_30_Flexcan4_TxTaskCancelationHandling
| 2017-02-23 5.05.05  Bir - Minor Improvements relating COV and MISRA deviations
| 2017-03-29 5.06.00  Bir - FEATC-382: remove saturation
|                           ESCAN00094638: MICROSAR3 only: No RAM Check during wakeup or explicite call of Can_30_Flexcan4_RamCheckExecute()
|                           ESCAN00095298: No transmit cancellation when calling Can_30_Flexcan4_CancelTx() / CanIf_TransmitCancellation()
| 2017-05-29 5.07.00  Bir - STORYC-140: support silent mode
|                           STORYC-645: selective MultiplexedTx
| 2017-08-22 5.07.01  Bir - ESCAN00096086: compiler error: undefined Symbol Can_GetMailbox*IdxOfControllerConfig
|                           ESCAN00096369: Can_30_Flexcan4_Write() return CAN_NOT_OK instead of CAN_BUSY in case of BO
|                           ESCAN00096391: Compiler error: function "CanHL_30_Flexcan4_WakeupProcessed" / "CanHL_30_Flexcan4_SleepProcessed" was referenced but not defined
| 2017-09-04 6.00.00  Bir - STORYC-2176: Remove "Tracing" feature
|                           ESCAN00096367: CAN driver does not recover from BusOff and stop communication.
|                           ESCAN00097096: Compiler error calling VStdMemCpy
|                           FEAT: Compiler error calling VStdMemCpy
|                           STORYC-3057 / ESCAN00098096: GenericPrecopy with FD-bit, GenericConfirmation(API2) with Len (instead of DLC) and FD-bit
|                                         ESCAN00098095: GenericConfirmation(API2) with Len instead of DLC
|                           STORYC-3329: MICROSAR3 Core Adaptation
|                           ESCAN00098892: Compiler warning: transitionRequest
| 2018-04-19 6.00.01  Bir - Minor Improvements relating COV
| 2018-04-23 7.00.00  Bir - Support new functional LL interface / support reduced to MSR403 only
|                           STORYC-4934: TxHwFifo
|                           STORYC-4935: TxHwFifo
|                           ESCAN00095187: support CAN_BUSY enum
| 2018-04-23 7.00.01  Bir - R21 final SafeBSW
| 2018-09-24 8.00.00  Bir - R22
|                           reworks because of QAC metrics
|                           ESCAN00094289: Can_30_Flexcan4_CheckWakeup has to return CAN_OK/CAN_NOT_OK (CAN360)
|                           TASK-91350: Review reworks
|                           ESCAN00101151: internal PIA failed
|                           STORYC-6510: Accept longer DLCs
|                           STORYC-6843: support more than 3 Baudrate configurations 
|                           STORYC-7257: BusMirroring - GetControllerMode/State/ErrorCounter
|                           STORYC-6846: MISRA 2012
| 2019-03-27 8.00.01  Bir - minor platform specific QAC findings fixed
|                           CM syntax fixed
|                           STORYC-8102: reduce STPH in CanHL_TxConfirmation
| 2019-04-29 9.00.00  Bir - R23 (no feature change here)
|                           minor platform specific QAC COV and SBSW findings fixed
|                           RAM_CHECK readback failure for mailboxes not notified
| 2019-09-09 9.00.01  Bir - R23/R24 (no feature change here)
|                           QAC reworks
|                           Infix in comments (Critical Section description)
|                           CanHL_30_Flexcan4_ReInit with return failed in case of DET issue
| 2020-12-11 10.00.00 Hum - CAN-1572: R26 - Support reporting of security events
| 2021-04-28 10.00.01 Bir - Unnecessary DET check for controller (already done in caller)
|                           Single-/Multi-channel API improvements
|                           MISRA comment rework
| 2021-06-30 10.00.02 Bir - mark Can_Write() as reentrant
|                           Add MISRA comment
|                           ESCAN00109136: remove global AREA_3 for Appl_GenericConfirmation() and CanIf_TxConfirmation()
|                                          copy to temp buffer before call Appl_GenericConfirmation()
|                           ESCAN00110429: compiler warning: undefined order of volatile access
| 2021-06-30 10.00.03 Bir - ESCAN00110573: call of Appl_GenericConfirmation() with wrong DLC
|****************************************************************************/

#if !defined(CAN_30_FLEXCAN4_API_H)
#define CAN_30_FLEXCAN4_API_H /* refer to ESCAN00094060 */

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/
/* \trace SPEC-1408 */
#include "Can_30_Flexcan4_Cfg.h"

#if (CAN_30_FLEXCAN4_MIRROR_MODE == STD_ON)
# include "CDDMirror.h"
# define C_30_FLEXCAN4_ENABLE_MIRROR_MODE
#endif

/***************************************************************************/
/* Version                                                                 */
/***************************************************************************/
/* Vector module version */
#define DRVCAN_ARM32FLEXCAN4ASR_VERSION 0x0104u
#define DRVCAN_ARM32FLEXCAN4ASR_RELEASE_VERSION 0x00u

/* internal Core Version */
#define CAN_30_FLEXCAN4_COREVERSION           0x1000u
#define CAN_30_FLEXCAN4_RELEASE_COREVERSION   0x03u

/* AutoSar Version for API */
/* BR:013, BR:014 */
#define CAN_30_FLEXCAN4_AR_RELEASE_MAJOR_VERSION      4u
#define CAN_30_FLEXCAN4_AR_RELEASE_MINOR_VERSION      0u
#define CAN_30_FLEXCAN4_AR_RELEASE_REVISION_VERSION   3u

/* AutoSar module version */
#define CAN_30_FLEXCAN4_SW_MAJOR_VERSION   ((DRVCAN_ARM32FLEXCAN4ASR_VERSION & 0xFF00u) >> 8)
#define CAN_30_FLEXCAN4_SW_MINOR_VERSION   (DRVCAN_ARM32FLEXCAN4ASR_VERSION & 0x00FFu)
#define CAN_30_FLEXCAN4_SW_PATCH_VERSION   (DRVCAN_ARM32FLEXCAN4ASR_RELEASE_VERSION & 0xFFu)

#define CAN_30_FLEXCAN4_VENDOR_ID   30u
#define CAN_30_FLEXCAN4_MODULE_ID   80u

/***************************************************************************/
/* Defines and Macros                                                      */
/***************************************************************************/
/* Module Id's */
#define CAN_30_FLEXCAN4_INIT_ID            0x00u
#define CAN_30_FLEXCAN4_INITCTR_ID         0x02u
#define CAN_30_FLEXCAN4_SETCTR_ID          0x03u
#define CAN_30_FLEXCAN4_DIINT_ID           0x04u
#define CAN_30_FLEXCAN4_ENINT_ID           0x05u
#define CAN_30_FLEXCAN4_WRITE_ID           0x06u
#define CAN_30_FLEXCAN4_VERSION_ID         0x07u
#define CAN_30_FLEXCAN4_MAINFCT_WRITE_ID   0x01u
#define CAN_30_FLEXCAN4_MAINFCT_READ_ID    0x08u
#define CAN_30_FLEXCAN4_MAINFCT_BO_ID      0x09u
#define CAN_30_FLEXCAN4_MAINFCT_WU_ID      0x0Au
#define CAN_30_FLEXCAN4_MAINFCT_MODE_ID    0x0Cu
#define CAN_30_FLEXCAN4_CKWAKEUP_ID        0x0Bu
#define CAN_30_FLEXCAN4_CHANGE_BR_ID       0x0Du
#define CAN_30_FLEXCAN4_CHECK_BR_ID        0x0Eu
#define CAN_30_FLEXCAN4_SET_BR_ID          0x0Fu
#define CAN_30_FLEXCAN4_GETCTR_STATE_ID    0x11u
#define CAN_30_FLEXCAN4_GETCTR_MODE_ID     0x12u
#define CAN_30_FLEXCAN4_GETCTR_RXCNT_ID    0x30u
#define CAN_30_FLEXCAN4_GETCTR_TXCNT_ID    0x31u

/* Internal Id's */
#define CAN_30_FLEXCAN4_RXINDI_ID          0x50u
#define CAN_30_FLEXCAN4_TXCNF_ID           0x51u
#define CAN_30_FLEXCAN4_CTRWAKEUP_ID       0x52u
#define CAN_30_FLEXCAN4_CTRBUSOFF_ID       0x53u
#define CAN_30_FLEXCAN4_HW_ACCESS_ID       0x60u

/* \trace SPEC-1702, SPEC-1401, SPEC-50568, SPEC-1568 */
/* Error Codes */
#define CAN_30_FLEXCAN4_E_NO_ERROR              (uint8)0x00u
#define CAN_30_FLEXCAN4_E_PARAM_POINTER         (uint8)0x01u
#define CAN_30_FLEXCAN4_E_PARAM_HANDLE          (uint8)0x02u
#define CAN_30_FLEXCAN4_E_PARAM_DLC             (uint8)0x03u
#define CAN_30_FLEXCAN4_E_PARAM_CONTROLLER      (uint8)0x04u
#define CAN_30_FLEXCAN4_E_UNINIT                (uint8)0x05u
#define CAN_30_FLEXCAN4_E_TRANSITION            (uint8)0x06u
#define CAN_30_FLEXCAN4_E_DATALOST              (uint8)0x07u   /* for MSR30 extension */
#define CAN_30_FLEXCAN4_E_PARAM_BAUDRATE        (uint8)0x08u   /* MRS403 only */
#define CAN_30_FLEXCAN4_E_RXQUEUE               (uint8)0x10u   /* extension */
#define CAN_30_FLEXCAN4_E_TIMEOUT_DET           (uint8)0x11u   /* extension */
#define CAN_30_FLEXCAN4_E_GENDATA               (uint8)0x12u   /* extension - used by LL to throw standardized issue for inconsistent generated data */
/* Error Codes Platform Specific */
/* error numbers for User Assertions 0x20-0x3f - hardware dependend */

/* error numbers for Generation tool Assertions 0x60-0x7f - hardware dependend */

/* error numbers for Hardware Assertions 0xa0-0xbf - hardware dependend */

/* error numbers for Internal Assertions 0xe0-0xff - hardware dependend */


/* Return values Can_30_Flexcan4_GetStatus() */
#define CAN_30_FLEXCAN4_STATUS_UNINIT           ((uint8)0x00u) /* ASR extension */
#define CAN_30_FLEXCAN4_STATUS_START            ((uint8)0x01u) /* ASR extension (kCanTxOn) */
#define CAN_30_FLEXCAN4_STATUS_STOP             ((uint8)0x02u) /* STOP */
#define CAN_30_FLEXCAN4_STATUS_INIT             ((uint8)0x04u) /* Initialized */
#define CAN_30_FLEXCAN4_STATUS_INCONSISTENT     ((uint8)0x08u) /* Inconsistent      used for common CAN */
#define CAN_30_FLEXCAN4_STATUS_WARNING          ((uint8)0x10u) /* WARNING */
#define CAN_30_FLEXCAN4_STATUS_PASSIVE          ((uint8)0x20u) /* PASSIVE */
#define CAN_30_FLEXCAN4_STATUS_BUSOFF           ((uint8)0x40u) /* BUSOFF */
#define CAN_30_FLEXCAN4_STATUS_SLEEP            ((uint8)0x80u) /* SLEEP */
#define CAN_30_FLEXCAN4_STATUS_MASK_NOMODE      ((uint8)0x0Cu) /* Initialized & Inconsistent/DEACTIVATE */

#if defined(C_DRV_30_FLEXCAN4_INTERNAL) /* just for internal usage */ /* COV_CAN_INTERNAL */
/* mode transition requests */
# define kCan_30_Flexcan4ModeSleep                 ((uint8)0x01u)
/* used by HLL: define kCan_30_Flexcan4ModeWakeupStart          ((uint8)0x02u) */
# define kCan_30_Flexcan4ModeStopReinitFast        ((uint8)0x03u)
# define kCan_30_Flexcan4ModeStopReinit            ((uint8)0x04u)
# define kCan_30_Flexcan4ModeStart                 ((uint8)0x05u)
# define kCan_30_Flexcan4ModeStartReinit           ((uint8)0x06u)
# define kCan_30_Flexcan4ModeWakeupStopReinit      ((uint8)0x07u)
# define kCan_30_Flexcan4ModeSilent                ((uint8)0x08u)
# define kCan_30_Flexcan4ModeResetBusOffStart      ((uint8)0x09u)
# define kCan_30_Flexcan4ModeResetBusOffEnd        ((uint8)0x0Au)
# define kCan_30_Flexcan4ModeNone                  ((uint8)0x10u)
/* precondition for mode transition */
# define kCan_30_Flexcan4ContinueBusOffRecovery    ((uint8)0x00u)
# define kCan_30_Flexcan4FinishBusOffRecovery      ((uint8)0x01u)
/* re-initialization */
# define kCan_30_Flexcan4ExecuteRamCheck           kCan_30_Flexcan4True
# define kCan_30_Flexcan4SuppressRamCheck          kCan_30_Flexcan4False
#endif

/* Macros to use Can_30_Flexcan4_GetStatus() */
/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */
#define CAN_30_FLEXCAN4_HW_IS_OK(state)                     (((state) & (CAN_30_FLEXCAN4_STATUS_WARNING |  \
                                                          CAN_30_FLEXCAN4_STATUS_PASSIVE |  \
                                                          CAN_30_FLEXCAN4_STATUS_BUSOFF) )    == 0u)
#define CAN_30_FLEXCAN4_HW_IS_WARNING(state)                (((state) & CAN_30_FLEXCAN4_STATUS_WARNING)      != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_PASSIVE(state)                (((state) & CAN_30_FLEXCAN4_STATUS_PASSIVE)      != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_BUSOFF(state)                 (((state) & CAN_30_FLEXCAN4_STATUS_BUSOFF)       != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_WAKEUP(state)                 (((state) & CAN_30_FLEXCAN4_STATUS_SLEEP)        == (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_SLEEP(state)                  (((state) & CAN_30_FLEXCAN4_STATUS_SLEEP)        != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_STOP(state)                   (((state) & CAN_30_FLEXCAN4_STATUS_STOP)         != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_START(state)                  (((state) & CAN_30_FLEXCAN4_STATUS_START)        != (uint8)0u)
#define CAN_30_FLEXCAN4_HW_IS_INCONSISTENT(state)           (((state) & CAN_30_FLEXCAN4_STATUS_INCONSISTENT) != (uint8)0u)
/* PRQA L:FctLikeMacroLabel */

/* Hardware Loop defines used by LowLevel for documentation (Core use it for loop check) */
#define kCan_30_Flexcan4LoopMode CAN_30_FLEXCAN4_LOOP_MAX

/* compatibility (LL may need this for check against current status)  */
#define kCan_30_Flexcan4HwIsInit CAN_30_FLEXCAN4_STATUS_INIT
#define kCan_30_Flexcan4HwIsSleep CAN_30_FLEXCAN4_STATUS_SLEEP

/* results of CAN RAM check */
#define CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER               CAN_30_FLEXCAN4_STATUS_INCONSISTENT
#define CAN_30_FLEXCAN4_ACTIVATE_CONTROLLER                 ((uint8)0x00u)

/***************************************************************************/
/* GenTool abstraction                                                     */
/***************************************************************************/

#define C_30_FLEXCAN4_ENABLE_STOP /* COV_CAN_COMPATIBILITY */
#if defined(V_OSTYPE_OSEK) || defined(V_OSTYPE_AUTOSAR) /* COV_CAN_OS_USAGE */
# define C_30_FLEXCAN4_ENABLE_OSEK_OS
#endif
#if (CAN_30_FLEXCAN4_TX_PROCESSING == CAN_30_FLEXCAN4_POLLING) /* COV_CAN_TX_POLLING */
# define C_30_FLEXCAN4_ENABLE_TX_POLLING
#endif
#if (CAN_30_FLEXCAN4_RX_PROCESSING == CAN_30_FLEXCAN4_POLLING) /* COV_CAN_RX_POLLING */
# define C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING
# define C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING
# define C_30_FLEXCAN4_ENABLE_RX_POLLING
#endif
#if (CAN_30_FLEXCAN4_BUSOFF_PROCESSING == CAN_30_FLEXCAN4_POLLING) /* COV_CAN_ERROR_POLLING */
# define C_30_FLEXCAN4_ENABLE_ERROR_POLLING
#endif
#if (CAN_30_FLEXCAN4_WAKEUP_PROCESSING == CAN_30_FLEXCAN4_POLLING) /* COV_CAN_WAKEUP_POLLING */
# define C_30_FLEXCAN4_ENABLE_WAKEUP_POLLING
#endif
#if (CAN_30_FLEXCAN4_INDIVIDUAL_PROCESSING == STD_ON) /* COV_CAN_INDIVIDUAL_POLLING */
# define C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING
#endif
#if (CAN_30_FLEXCAN4_MULTIPLE_BASICCAN == STD_ON) /* COV_CAN_MULTIPLE_BASICCAN */
# define C_30_FLEXCAN4_ENABLE_MULTIPLE_BASICCAN
#endif
#if (CAN_30_FLEXCAN4_MULTIPLE_BASICCAN_TX == STD_ON) /* COV_CAN_MULTIPLE_BASICCAN_TX */
# define C_30_FLEXCAN4_ENABLE_MULTIPLE_BASICCAN_TX
#endif
#if (CAN_30_FLEXCAN4_TX_FULLCAN_OBJECTS == STD_ON) /* COV_CAN_TX_FULLCAN_OBJECTS */
# define C_30_FLEXCAN4_ENABLE_TX_FULLCAN_OBJECTS
#endif
#if (CAN_30_FLEXCAN4_RX_FULLCAN_OBJECTS == STD_ON) /* COV_CAN_RX_FULLCAN_OBJECTS */
# define C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS
#endif
#if (CAN_30_FLEXCAN4_RX_BASICCAN_OBJECTS == STD_ON)
# define C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS
#endif
#if (CAN_30_FLEXCAN4_HW_TRANSMIT_CANCELLATION == STD_ON) /* COV_CAN_HW_TRANSMIT_CANCELLATION */
# define C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW
#endif
/* C_30_FLEXCAN4_ENABLE_CANCEL_FULLCAN_IN_HW may be user defined */
#if defined(C_30_FLEXCAN4_ENABLE_CANCEL_IN_HW) /* COV_CAN_CANCEL_IN_HW */
# define C_30_FLEXCAN4_ENABLE_CANCEL_TX_IN_HW
#endif
#if (CAN_30_FLEXCAN4_CANCEL_SUPPORT_API == STD_ON)
# define C_30_FLEXCAN4_ENABLE_CANCEL_SUPPORT_API
#endif
#if (CAN_30_FLEXCAN4_HARDWARE_CANCELLATION == STD_ON) /* COV_CAN_HARDWARE_CANCELLATION */
#define C_30_FLEXCAN4_ENABLE_HW_LOOP_TIMER
#endif
#if (CAN_30_FLEXCAN4_EXTENDED_ID == STD_ON) /* COV_CAN_EXTENDED_ID */
# define C_30_FLEXCAN4_ENABLE_EXTENDED_ID
#endif
#if (CAN_30_FLEXCAN4_MIXED_ID == STD_ON) /* COV_CAN_MIXED_ID */
# define C_30_FLEXCAN4_ENABLE_MIXED_ID
#endif
#if (CAN_30_FLEXCAN4_SLEEP_SUPPORT == STD_ON) /* COV_CAN_SLEEP_SUPPORT */
# if (CAN_30_FLEXCAN4_WAKEUP_SUPPORT == STD_ON) /* COV_CAN_WAKEUP_SUPPORT */
/* before version 3.12 this switches occur always synchronous both to ON or OFF (C_30_FLEXCAN4_ENABLE_WAKEUP no more used) */
/* if CAN_30_FLEXCAN4_WAKEUP_SUPPORT is switched off, STOP mode will be used instead of SLEEP (see AUTOSAR specification) */
# define C_30_FLEXCAN4_ENABLE_SLEEP_WAKEUP
# endif
#endif
#if (CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL) /* COV_CAN_OVERRUN_NOTIFICATION */
# define C_30_FLEXCAN4_ENABLE_OVERRUN
# define C_30_FLEXCAN4_ENABLE_FULLCAN_OVERRUN
#endif
#if (CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_DET) /* COV_CAN_OVERRUN_NOTIFICATION */
# define C_30_FLEXCAN4_ENABLE_OVERRUN
# define C_30_FLEXCAN4_ENABLE_FULLCAN_OVERRUN
# if (CAN_30_FLEXCAN4_DEV_ERROR_REPORT != STD_ON) /* COV_CAN_COMPATIBILITY */
#  error "CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_DET need CAN_30_FLEXCAN4_DEV_ERROR_REPORT == STD_ON"
# endif
#endif
#if (CAN_30_FLEXCAN4_MULTIPLEXED_TRANSMISSION == STD_ON) /* COV_CAN_MULTIPLEXED_TRANSMISSION */
# define C_30_FLEXCAN4_ENABLE_MULTIPLEXED_TRANSMISSION
#endif
#if (CAN_30_FLEXCAN4_TX_HW_FIFO == STD_ON) /* COV_CAN_TX_HW_FIFO */
# define C_30_FLEXCAN4_ENABLE_TX_HW_FIFO
#endif
#if (CAN_30_FLEXCAN4_RX_QUEUE == STD_ON) /* COV_CAN_RX_QUEUE */
# define C_30_FLEXCAN4_ENABLE_RX_QUEUE
#endif
#if (CAN_30_FLEXCAN4_GENERIC_CONFIRMATION == STD_ON)
# define CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API1
#endif
#if (CAN_30_FLEXCAN4_GENERIC_CONFIRMATION == CAN_30_FLEXCAN4_API2) /* COV_CAN_GENERIC_CONFIRMATION_API2 */
# define CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2
#endif
#if (CAN_30_FLEXCAN4_GENERIC_PRETRANSMIT == STD_ON)
# define C_30_FLEXCAN4_ENABLE_PRETRANSMIT_FCT
#endif
#if (CAN_30_FLEXCAN4_ONE_CONTROLLER_OPTIMIZATION == STD_ON)
# define C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL
# define C_30_FLEXCAN4_DISABLE_MULTICHANNEL_API
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY         void
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL
# define CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY
# define CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY      void
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_LOCAL
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY    void
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST
# define CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT
# define CAN_30_FLEXCAN4_HW_CHANNEL_DUMMY_STATEMENT
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT
#else
# define C_30_FLEXCAN4_MULTIPLE_RECEIVE_CHANNEL
# define C_30_FLEXCAN4_ENABLE_MULTICHANNEL_API
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_ONLY         Can_30_Flexcan4ChannelHandle  Controller
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST        Can_30_Flexcan4ChannelHandle  Controller,
# define CAN_30_FLEXCAN4_CHANNEL_CANTYPE_LOCAL        Can_30_Flexcan4ChannelHandle  Controller;
# define CAN_30_FLEXCAN4_CHANNEL_CANPARA_ONLY         (Can_30_Flexcan4ChannelHandle) Controller
# define CAN_30_FLEXCAN4_CHANNEL_CANPARA_FIRST        (Can_30_Flexcan4ChannelHandle) Controller,
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY      Can_30_Flexcan4ChannelHandle  Controller
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_FIRST     Can_30_Flexcan4ChannelHandle  Controller,
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_LOCAL     Can_30_Flexcan4ChannelHandle  Controller;
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_ONLY      (Can_30_Flexcan4ChannelHandle) Controller
# define CAN_30_FLEXCAN4_HW_CHANNEL_CANPARA_FIRST     (Can_30_Flexcan4ChannelHandle) Controller,
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_ONLY   Can_30_Flexcan4ChannelHandle Controller
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANTYPE_FIRST  Can_30_Flexcan4ChannelHandle Controller,
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_ONLY   Controller
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_CANPARA_FIRST  Controller,
# define CAN_30_FLEXCAN4_CHANNEL_DUMMY_STATEMENT                 CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller)
# define CAN_30_FLEXCAN4_HW_CHANNEL_DUMMY_STATEMENT              CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller)
# define CAN_30_FLEXCAN4_CHANNEL_AND_HW_CHANNEL_DUMMY_STATEMENT  CAN_30_FLEXCAN4_DUMMY_STATEMENT(Controller)
#endif
#if (CAN_30_FLEXCAN4_GET_STATUS == STD_ON)
# define C_30_FLEXCAN4_ENABLE_EXTENDED_STATUS
#endif
#if (CAN_30_FLEXCAN4_RAM_CHECK == CAN_30_FLEXCAN4_NOTIFY_ISSUE) /* COV_CAN_RAM_CHECK */
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_WAKEUP
#endif
#if (CAN_30_FLEXCAN4_RAM_CHECK == CAN_30_FLEXCAN4_NOTIFY_MAILBOX) /* COV_CAN_RAM_CHECK */
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_WAKEUP
# define C_30_FLEXCAN4_ENABLE_NOTIFY_CORRUPT_MAILBOX
#endif
#if (CAN_30_FLEXCAN4_RAM_CHECK == CAN_30_FLEXCAN4_EXTENDED) /* COV_CAN_RAM_CHECK_EXTENDED */
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK
# define C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED
#endif
#if (CAN_30_FLEXCAN4_FD_SUPPORT != CAN_30_FLEXCAN4_NONE) /* COV_CAN_FD_SUPPORT */
# define C_30_FLEXCAN4_ENABLE_CAN_FD_USED
# if (CAN_30_FLEXCAN4_FD_SUPPORT == CAN_30_FLEXCAN4_BRS) /* COV_CAN_FD_SUPPORT */
#  define C_30_FLEXCAN4_ENABLE_CAN_FD_BRS
# endif
# if (CAN_30_FLEXCAN4_FD_SUPPORT == CAN_30_FLEXCAN4_FULL) /* COV_CAN_FD_SUPPORT */
#  define C_30_FLEXCAN4_ENABLE_CAN_FD_FULL
# endif
#endif
#if (CAN_30_FLEXCAN4_USE_INIT_POINTER == STD_ON) /* InitPointer may not be used in variant system for symmetric configuration */
# define CAN_30_FLEXCAN4_ENABLE_USE_INIT_ROOT_POINTER
#endif
#if (CAN_30_FLEXCAN4_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# define CAN_30_FLEXCAN4_ENABLE_USE_ECUM_BSW_ERROR_HOOK
#endif
#if (CAN_30_FLEXCAN4_USE_OS_INTERRUPT_CONTROL == STD_ON) /* COV_CAN_COMPATIBILITY */
# define C_30_FLEXCAN4_ENABLE_USE_OS_INTERRUPT_CONTROL
#endif
#if (CAN_30_FLEXCAN4_SILENT_MODE == STD_ON) /* COV_CAN_COMPATIBILITY */
# define C_30_FLEXCAN4_ENABLE_SILENT_MODE
#endif
#if ((CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_DRIVER) || (CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH)) /* COV_CAN_LOCK_ISR_BY_APPL */
# define C_30_FLEXCAN4_ENABLE_CAN_CAN_INTERRUPT_CONTROL
#endif  
#if ( CAN_30_FLEXCAN4_PROTECTED_MODE == STD_ON ) /* COV_CAN_COMPATIBILITY */
# define C_30_FLEXCAN4_ENABLE_USER_MODE_OS
#endif
#if ( CAN_30_FLEXCAN4_FD_HW_BUFFER_OPTIMIZATION == STD_ON ) /* COV_CAN_COMPATIBILITY */
# define C_30_FLEXCAN4_ENABLE_CAN_FD_HW_BUFFER_OPTIMIZATION
#endif
#if (CAN_30_FLEXCAN4_CHECK_WAKEUP_CAN_RET_TYPE == STD_ON) /* COV_CAN_COMPATIBILITY */
# define C_30_FLEXCAN4_ENABLE_CHECK_WAKEUP_CAN_RET_TYPE
#endif
#if (CAN_30_FLEXCAN4_SECURITY_EVENT_REPORTING == STD_ON)
# define CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING
#endif

/***************************************************************************/
/* Compiler abstraction                                                    */
/***************************************************************************/
#if !defined(V_NULL) /* COV_CAN_COMPATIBILITY */
# define V_NULL          NULL_PTR
#endif
#if !defined(V_NONE) /* COV_CAN_COMPATIBILITY */
# define V_NONE       /* empty used instead of external, STATIC, volatile... */
# define CAN_AUTOMATIC    AUTOMATIC
# define CAN_TYPEDEF      TYPEDEF
#endif
#if !defined(CAN_STATIC) /* COV_CAN_COMPATIBILITY */
# if defined(STATIC) /* COV_CAN_COMPATIBILITY */
#  define CAN_STATIC  STATIC   /* standard by AutoSar 3.x */
# else
#  define CAN_STATIC  static   /* standard AutoSar 4.x define */
# endif
#endif
#if !defined(CAN_LOCAL_INLINE) /* COV_CAN_COMPATIBILITY */
# if defined(LOCAL_INLINE) /* COV_CAN_COMPATIBILITY */
#  define CAN_LOCAL_INLINE  LOCAL_INLINE
# else
#  define CAN_LOCAL_INLINE  
# endif
#endif

/* PRQA S 0342,3453 CompilerAbstractionLabel */ /* MD_Can_CompilerAbstraction,MD_MSR_FctLikeMacro */
#define V_DEF_30_FLEXCAN4_VAR(storage, vartype, memclass)                    storage VAR(vartype, CAN_30_FLEXCAN4_##memclass)
#define V_DEF_30_FLEXCAN4_VAR_FAR(storage, vartype)                          storage VAR(vartype, CAN_30_FLEXCAN4_VAR_FAR)
#define V_DEF_30_FLEXCAN4_VAR_NEAR(storage, vartype)                         storage VAR(vartype, CAN_30_FLEXCAN4_VAR_NEAR)
#define V_DEF_30_FLEXCAN4_VAR_TYPE(storage, vartype)                         typedef storage VAR(vartype, TYPEDEF)
#define V_DEF_30_FLEXCAN4_P2VAR(storage, ptrtype, memclass, ptrclass)        storage P2VAR(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_P2VAR_TYPE(storage, ptrtype, ptrclass)             typedef storage P2VAR(ptrtype, TYPEDEF, CAN_30_FLEXCAN4_##ptrclass )
#define V_DEF_30_FLEXCAN4_P2VAR_PARA(storage, ptrtype, memclass, ptrclass)   storage P2VAR(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_P2SFR_CAN(storage, ptrtype, memclass)              storage P2VAR(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_REG_CANCELL)
#define V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(storage, ptrtype)                   typedef storage P2VAR(ptrtype, TYPEDEF, CAN_30_FLEXCAN4_REG_CANCELL)
#define V_DEF_30_FLEXCAN4_CONSTP2VAR(storage, ptrtype, memclass, ptrclass)   storage CONSTP2VAR(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_CONST(storage, type, memclass)                     storage CONST(type, CAN_30_FLEXCAN4_##memclass)
#define V_DEF_30_FLEXCAN4_CONST_TYPE(storage, type, memclass)                typedef storage CONST(type, TYPEDEF)
#define V_DEF_30_FLEXCAN4_P2CONST(storage, ptrtype, memclass, ptrclass)      storage P2CONST(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_P2CONST_TYPE(storage, ptrtype, ptrclass)           typedef storage P2CONST(ptrtype, TYPEDEF, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_P2CONST_PARA(storage, ptrtype, memclass, ptrclass) storage P2CONST(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_CONSTP2CONST(storage, ptrtype, memclass, ptrclass) storage CONSTP2CONST(ptrtype, CAN_30_FLEXCAN4_##memclass, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_CONSTP2CONST_TYPE(storage, ptrtype, ptrclass)      typedef storage CONSTP2CONST(ptrtype, TYPEDEF, CAN_30_FLEXCAN4_##ptrclass)
#define V_DEF_30_FLEXCAN4_FUNC(storage, rettype, memclass)                   storage FUNC(rettype, CAN_30_FLEXCAN4_##memclass)
#define V_DEF_30_FLEXCAN4_FUNC_API(storage, rettype, memclass)               storage FUNC(rettype, CAN_30_FLEXCAN4_##memclass)
#define V_DEF_30_FLEXCAN4_FUNC_CBK(storage, rettype, memclass)               storage FUNC(rettype, CAN_30_FLEXCAN4_##memclass)
#define V_DEF_30_FLEXCAN4_P2FUNC(storage, rettype, ptrclass, fctname)        storage P2FUNC(rettype, CAN_30_FLEXCAN4_##ptrclass, fctname)
/* PRQA L:CompilerAbstractionLabel */

/* only use internal when not defined before */
#if !defined(vuint8) /* COV_CAN_COMPATIBILITY */
# define CAN_30_FLEXCAN4_HL_NO_V_DEF
# define vuint8 uint8
#endif
#if !defined(vsint8) /* COV_CAN_COMPATIBILITY */
# define vsint8 sint8
#endif
#if !defined(vuint16) /* COV_CAN_COMPATIBILITY */
# define vuint16 uint16
#endif
#if !defined(vsint16) /* COV_CAN_COMPATIBILITY */
# define vsint16 sint16
#endif
#if !defined(vuint32) /* COV_CAN_COMPATIBILITY */
# define vuint32 uint32
#endif
#if !defined(vsint32) /* COV_CAN_COMPATIBILITY */
# define vsint32 sint32
#endif

/***************************************************************************/
/* LL return values                                                        */
/***************************************************************************/

/* return values of Can_30_Flexcan4RxActualIdType */
#define kCan_30_Flexcan4IdTypeStd         (0x00u)
#define kCan_30_Flexcan4IdTypeExt         (0x20u) /* Attention: The SRR bit has to be considered for the transmission of Extended CAN frames, but must be without any effect on the reception side !
                                         ********** In Extended format it must be set to '1' for transmission.
                                                    For the Rx side it will be stored with the value received on the CAN bus, either recessive or dominant. */
#define kCan_30_Flexcan4IdTypeSRR         (0x40u)
#define kCan_30_Flexcan4FdTypeFdByte      (0xC0u)
#define kCan_30_Flexcan4FdTypeFd                            (0x8000u)
#define kCan_30_Flexcan4FdTypeClassic                       (0x0000u) /* usually 0x00, Fd-Flag not set */
#define kCan_30_Flexcan4FdBrsTypeTrue                       (0x4000u)
#define kCan_30_Flexcan4FdBrsTypeFalse                      (0x0000u) /* usually 0x00, Brs-Flag not set */



/***************************************************************************/
/* LL parameter values                                                     */
/***************************************************************************/

/* parameter for Appl_30_Flexcan4CanTimerStart(),-End(), -Loop() */
#define kCan_30_Flexcan4LoopLeaveDisableModeInit  ((vuint8)0x00u)
#define kCan_30_Flexcan4LoopEnterDisableModeInit  ((vuint8)0x01u)
#define kCan_30_Flexcan4LoopMsgReception          ((vuint8)0x02u)
#define kCan_30_Flexcan4LoopEnterFreezeModeInit   ((vuint8)0x03u)
#define kCan_30_Flexcan4LoopResetInit             ((vuint8)0x04u)


/* \trace SPEC-1399 */
/***************************************************************************/
/* data types need for API                                                 */
/***************************************************************************/
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, Std_VersionInfoType, APPL_VAR) Can_30_Flexcan4_VersionInfoPtrType;
V_DEF_30_FLEXCAN4_P2CONST_TYPE(V_NONE, Can_30_Flexcan4_ConfigType, CONST_PBCFG) Can_30_Flexcan4_ConfigPtrType;
V_DEF_30_FLEXCAN4_P2CONST_TYPE(V_NONE, Can_PduType, APPL_VAR)           Can_30_Flexcan4_PduInfoPtrType;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, Can_PduType, APPL_VAR)             Can_30_Flexcan4_PduInfoPtrType_var;
V_DEF_30_FLEXCAN4_P2CONST_TYPE(V_NONE, uint8, RX_TX_DATA)               Can_30_Flexcan4_DataPtrType;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, uint8, APPL_VAR)                   Can_30_Flexcan4_ErrorCounterPtrType;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, Can_ControllerStateType, APPL_VAR) Can_30_Flexcan4_ControllerStatePtrType;
V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, Can_ErrorStateType, APPL_VAR)      Can_30_Flexcan4_ErrorStatePtrType;

/* PRQA S 3453 FctLikeMacroLabel */ /* MD_MSR_FctLikeMacro */


#if defined( C_30_FLEXCAN4_ENABLE_ERROR_POLLING )
# define C_30_FLEXCAN4_DISABLE_CAN_BUSOFF_INTERRUPT
#else
# define C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
#endif

#if (defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) && !defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING )) || \
    (defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS ) && !defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING ))   || \
    (!defined( C_30_FLEXCAN4_ENABLE_TX_POLLING )) || (defined( C_30_FLEXCAN4_ENABLE_INDIVIDUAL_POLLING )) /* COV_CAN_MULTIPLE_BASICCAN */
# define C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
#else
# define C_30_FLEXCAN4_DISABLE_CAN_RXTX_INTERRUPT
#endif


/* data types for driver function parameters */
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint16) Can_30_Flexcan4InitHandle;

/* Can_30_Flexcan4ObjectHandle has to be unsigned! */
# define Can_30_Flexcan4ObjectHandle  Can_HwHandleType

V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint16) Can_30_Flexcan4TransmitHandle;
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint16) Can_30_Flexcan4ReceiveHandle;

/* define data type of local signed variables for message handles */

/* pointer to msgObj & data may point to RAM or CAN cell depend on RX or TX and feature C_30_FLEXCAN4_ENABLE_HWBUFFER */
/* so the pointers have to be the same for all (far or nothing / far also works for RAM) */
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint8)  Can_30_Flexcan4ChipDataPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint8)  Can_30_Flexcan4ChipMsgPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint8)  Can_30_Flexcan4ChipDataPtr8;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint8)  Can_30_Flexcan4ChipMsgPtr8;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint16) Can_30_Flexcan4ChipDataPtr16;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint16) Can_30_Flexcan4ChipMsgPtr16;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint32) Can_30_Flexcan4ChipDataPtr32;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, vuint32) Can_30_Flexcan4ChipMsgPtr32;

/* up to 5 tables can be defined for the transmission ID. The type and how 
   many tables are needed depends on the CPU bus type and on the ID registers 
   of the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCan_30_Flexcan4RxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and
   tCan_30_Flexcan4RxId0 = vuint16, tCan_30_Flexcan4RxId1 = vuint16 and tCan_30_Flexcan4RxId2 = vuint16. */
#if defined( C_30_FLEXCAN4_ENABLE_EXTENDED_ID )
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint32) tCan_30_Flexcan4TxId0;
/* unused by this driver: 
typedef vuint8          tCan_30_Flexcan4TxId1;
typedef vuint8          tCan_30_Flexcan4TxId2;
typedef vuint8          tCan_30_Flexcan4TxId3;
typedef vuint8          tCan_30_Flexcan4TxId4;
*/
#else
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint16) tCan_30_Flexcan4TxId0;
/* unused by this driver: 
typedef vuint8          tCan_30_Flexcan4TxId1;
typedef vuint8          tCan_30_Flexcan4TxId2;
typedef vuint8          tCan_30_Flexcan4TxId3;
typedef vuint8          tCan_30_Flexcan4TxId4;
*/
#endif

/* type of variable for ID type kCan_30_Flexcan4IdTypeExt or kCan_30_Flexcan4IdTypeStd will be assigned to 
   a variable of this type */
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint8) tCan_30_Flexcan4IdType;

/* up to 3 tables can be defined for the reception ID. The type and how many
   tables are needed depends on the CPU bus type and on the ID registers of
   the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCan_30_Flexcan4RxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and 
   tCan_30_Flexcan4RxId0 = vuint16, tCan_30_Flexcan4RxId1 = vuint16 and tCan_30_Flexcan4RxId2 = vuint16. */
# if defined C_30_FLEXCAN4_ENABLE_EXTENDED_ID
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint32) tCan_30_Flexcan4RxId0;
# else
V_DEF_30_FLEXCAN4_VAR_TYPE(V_NONE, vuint16) tCan_30_Flexcan4RxId0;
# endif
/* unused by this driver: 
typedef vuint16          tCan_30_Flexcan4RxId1;
typedef vuint16          tCan_30_Flexcan4RxId2;
typedef vuint16          tCan_30_Flexcan4RxId3;
typedef vuint16          tCan_30_Flexcan4RxId4;
*/


typedef vuint8           tCan_30_Flexcan4Dlc;


/* stucture of the CAN message object used by Can_30_Flexcan4MsgTransmit() and RxQueue */
typedef struct
{
  vuint16 DlcRaw;
  vuint16 TimeStamp;
  vuint32 IdRaw;
#if defined(C_30_FLEXCAN4_ENABLE_CAN_FD_FULL)
  vuint8        DataFld[64];
#else
  vuint8        DataFld[8];
#endif
}tCan_30_Flexcan4MsgTransmitStruct;

#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_USED )
/* types of the variables that hold the FD flag / BRS flag information */
typedef vuint16          tCan_30_Flexcan4FdType;
typedef vuint16          tCan_30_Flexcan4FdBrsType;
#endif


V_DEF_30_FLEXCAN4_P2VAR_TYPE(V_NONE, tCan_30_Flexcan4LLCanIntOld, VAR_NOINIT) tCan_30_Flexcan4LLCanIntOldPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(volatile, tCan_30_Flexcan4RxMsgBuffer)  tCan_30_Flexcan4RxMsgBufferPtr;

typedef volatile struct
{
#if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  vuint16  timestamp; /*!< timestamp: reading timestamp unlocks the current locked mailbox */
  vuint16  control;   /*!< control, status and DLC */
#endif
  vuint32  msgID;     /*!< message ID */
#if defined( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
  vuint32  data[16];  /*!< message data */
#else
  vuint32  data[2];   /*!< message data */
#endif
}tCan_30_Flexcan4MsgObj;

typedef volatile struct
{
#if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  vuint16  canmaxmb;       /*!< 00 Maximum Message Buffer */
  vuint16  canmcr;         /*!< 02 Module Configuration Register */
#endif  
  vuint32  control1;       /*!< 04 CAN Control 1: PRESDIV, RJW, PSEG1, PSEG2, BOFF_MSK, ERR_MSK, CLK_SRC, LPB, TWRN_MSK, RWRN_MSK, SMP, BOFF_REC, T_SYNC, LBUF, LOM and PROPSEG */
  vuint32  timer;          /*!< 08 Timer */
  vuint32  rsv1;           /*!< 0C Reserved */
  vuint32  rxmgmask;       /*!< 10 Rx Global Mask */
  vuint32  rx14mask;       /*!< 14 Rx 14 Mask */
  vuint32  rx15mask;       /*!< 18 Rx 15 Mask */
  vuint32  ecr;            /*!< 1C Error Counter */
  vuint32  estat;          /*!< 20 Error / Status */
  vuint32  imask2;         /*!< 24 Interrupt Mask 2 (MB 32 - 63) Register */
  vuint32  imask1;         /*!< 28 Interrupt Mask 1 (MB  0 - 31) Register */
  vuint32  iflag2;         /*!< 2C Interrupt Flag 2 (MB 32 - 63) Register */
  vuint32  iflag1;         /*!< 30 Interrupt Flag 1 (MB  0 - 31) Register */
  vuint32  ctrl2;          /*!< 34 CAN Control 2: WRMFRZ, RFEN, TASD, MRP, RRS, EACEN */
  vuint32  esr2;           /*!< 38 IMB, VPS, LPTM */
  vuint32  rsv4[2];        /*!< 3C - 44 Reserved */
  vuint32  crcr;           /*!< 44 TXCRC, MBCRC */
  vuint32  rxfgmask;       /*!< 48 Rx FiFo global mask */
  vuint32  rxfir;          /*!< 4C Rx FiFo information Register */
  vuint32  cbt;            /*!< 50 CAN Bit Timing Register */
  vuint32  rsv5[5];        /*!< 54 - 68 Reserved */
  vuint32  imask4;         /*!< 68 Interrupt Mask 4 (MB 96 - 127) Register */
  vuint32  imask3;         /*!< 6C Interrupt Mask 3 (MB 64 -  95) Register */
  vuint32  iflag4;         /*!< 70 Interrupt Flag 4 (MB 96 - 127) Register */
  vuint32  iflag3;         /*!< 74 Interrupt Flag 3 (MB 64 -  95) Register */
  vuint32  rsv6[2];        /*!< 78 - 80 Reserved */
#if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  tCan_30_Flexcan4MsgObj canObject[128];  /*!< Buffer for 128 messages */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  tCan_30_Flexcan4MsgObj canObject[96];  /*!< Buffer for 96 messages */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  tCan_30_Flexcan4MsgObj canObject[64];  /*!< Buffer for 64 messages */
#else
  tCan_30_Flexcan4MsgObj canObject[32];  /*!< Buffer for 32 messages */
#endif
}tFlex_30_Flexcan4CAN;

V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tFlex_30_Flexcan4CAN) tFlex_30_Flexcan4CANPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4MsgObj) tCan_30_Flexcan4MsgObjPtr;

typedef volatile struct
{
#if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  vuint32 indivRxMask[128]; /*!< Individual Receive Masks */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  vuint32 indivRxMask[96]; /*!< Individual Receive Masks */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  vuint32 indivRxMask[64]; /*!< Individual Receive Masks */
#else
  vuint32 indivRxMask[32]; /*!< Individual Receive Masks */
#endif
}tCan_30_Flexcan4RxMask;

typedef volatile struct
{
  vuint32  gfwR;          /*!< Glitch filter */
}tCan_30_Flexcan4GlitchFilter;

typedef volatile struct
{
  vuint32  mecr;           /*!< MECR register */
  vuint32  errIaR;         /*!< Error Injection Address Register */
  vuint32  errIdpR;        /*!< Error Injection Data Pattern Register */
  vuint32  errIppR;        /*!< Error Injection Parity Pattern Register */
  vuint32  rErrAddressR;   /*!< Error Report Address Register */
  vuint32  rErrDataR;      /*!< Error Report AData Register */
  vuint32  rErrSyndromR;   /*!< Error Report Syndrome Register */
  vuint32  errStatusR;     /*!< Error Status Register */
}tCan_30_Flexcan4ParityCheck;

typedef volatile struct
{
  vuint32 eprs;            /*!< Enhanced CAN Bit Timing Prescalers Register */
  vuint32 encbt;           /*!< Enhanced Nominal CAN Bit Timing Register */
  vuint32 edcbt;           /*!< Enhanced Data Phase CAN bit Timing Register */
  vuint32 etdc;            /*!<  Enhanced Transceiver Delay Compensation Register */
}tCan_30_Flexcan4EBTRegister;

typedef volatile struct
{
  vuint32  fdctrl;         /*!< FD Control Register */
  vuint32  fdcbt;          /*!< FD CBT Register */
  vuint32  fdcrc;          /*!< FD CRC Register */
}tCan_30_Flexcan4FDRegister;

typedef volatile struct
{
  vuint32 erfcr;           /*!<  Enhanced Rx FIFO Control Register */
  vuint32 erfier;          /*!<  Enhanced Rx FIFO Interrup Enable Register */
  vuint32 erfsr;           /*!<  Enhanced Rx FIFO Status Register */
}tCan_30_Flexcan4ERxFifoRegister;

typedef volatile struct
{
#if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  volatile vuint32 hrTimestamp[128]; /*!< High Resolution Timestamps */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  volatile vuint32 hrTimestamp[96]; /*!< IHigh Resolution Timestamps */
#elif defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
  volatile vuint32 hrTimestamp[64]; /*!< High Resolution Timestamps */
#else
  volatile vuint32 hrTimestamp[32]; /*!< High Resolution Timestamps */
#endif
}tCan_30_Flexcan4HRTimestamp;

typedef volatile struct
{
#if defined( C_CPUTYPE_LITTLEENDIAN ) /* COV_CAN_FLEXCAN4_DERIVATIVE_LITTLE_ENDIAN */
  vuint16  timestamp; /*!< timestamp: reading timestamp unlocks the current locked mailbox */
  vuint16  control;   /*!< control, status and DLC */
#endif
  vuint32  msgID;     /*!< message ID */
  volatile vuint32  data[16];  /*!< message data */
}tCan_30_Flexcan4ERxFifoObj;

typedef volatile struct
{
  volatile vuint32 filterElement[128];
}tCan_30_Flexcan4ERxFifoFilter;

V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4RxMask) tCan_30_Flexcan4RxMaskPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4ParityCheck) tCan_30_Flexcan4ParityCheckPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4EBTRegister) tCan_30_Flexcan4EBTRegisterPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4FDRegister) tCan_30_Flexcan4FDRegisterPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4ERxFifoRegister) tCan_30_Flexcan4ERxFifoRegisterPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4HRTimestamp) tCan_30_Flexcan4HRTimestampPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4ERxFifoObj) tCan_30_Flexcan4ERxFifoObjPtr;
V_DEF_30_FLEXCAN4_P2SFR_CAN_TYPE(V_NONE, tCan_30_Flexcan4ERxFifoFilter) tCan_30_Flexcan4ERxFifoFilterPtr;

#if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
# define pFlex_30_Flexcan4CAN(x)             ((tFlex_30_Flexcan4CANPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[x]))
# define pCan_30_Flexcan4RxMask(x)           ((tCan_30_Flexcan4RxMaskPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x0880u))
# define pCan_30_Flexcan4GlitchFilter(x)     ((tCan_30_Flexcan4GlitchFilterPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x09E0u))
# define pCan_30_Flexcan4ParityCheck(x)      ((tCan_30_Flexcan4ParityCheckPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x0AE0u))
# define pCan_30_Flexcan4EBTRegister(x)      ((tCan_30_Flexcan4EBTRegisterPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0xBF0u))
# define pCan_30_Flexcan4FDRegister(x)       ((tCan_30_Flexcan4FDRegisterPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x0C00u))
# define pCan_30_Flexcan4ERxFifoRegister(x)  ((tCan_30_Flexcan4ERxFifoRegisterPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x0C0Cu))
# define pCan_30_Flexcan4HRTimestamp(x)      ((tCan_30_Flexcan4HRTimestampPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x0C30u))
# define pCan_30_Flexcan4ERxFifoObj(x)       ((tCan_30_Flexcan4ERxFifoObjPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x2000u))
# define pCan_30_Flexcan4ERxFifoFilter(x)    ((tCan_30_Flexcan4ERxFifoFilterPtr)((vuint32)canLL_30_Flexcan4_VirtualPtrField[(x)] + (vuint32)0x3000u))
#else
#  define pFlex_30_Flexcan4CAN(x)             ((tFlex_30_Flexcan4CANPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x)))
#  define pCan_30_Flexcan4RxMask(x)           ((tCan_30_Flexcan4RxMaskPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x0880u))
#  define pCan_30_Flexcan4GlitchFilter(x)     ((tCan_30_Flexcan4GlitchFilterPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x09E0u))
#  define pCan_30_Flexcan4ParityCheck(x)      ((tCan_30_Flexcan4ParityCheckPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x0AE0u))
#  define pCan_30_Flexcan4EBTRegister(x)      ((tCan_30_Flexcan4EBTRegisterPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0xBF0u))
#  define pCan_30_Flexcan4FDRegister(x)       ((tCan_30_Flexcan4FDRegisterPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x0C00u))
#  define pCan_30_Flexcan4ERxFifoRegister(x)  ((tCan_30_Flexcan4ERxFifoRegisterPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x0C0Cu))
#  define pCan_30_Flexcan4HRTimestamp(x)      ((tCan_30_Flexcan4HRTimestampPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x0C30u))
#  define pCan_30_Flexcan4ERxFifoObj(x)       ((tCan_30_Flexcan4ERxFifoObjPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x2000u))
#  define pCan_30_Flexcan4ERxFifoFilter(x)    ((tCan_30_Flexcan4ERxFifoFilterPtr)((vuint32)Can_30_Flexcan4_GetBaseAddressOfControllerConfig(x) + (vuint32)0x3000u))
#endif

#if defined ( C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL )
#  define CAN_30_FLEXCAN4_INT_MASK1(x) Can_30_Flexcan4_GetInterruptMask1OfControllerConfig(0)
#  if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_INT_MASK2(x) Can_30_Flexcan4_GetInterruptMask2OfControllerConfig(0)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_INT_MASK3(x) Can_30_Flexcan4_GetInterruptMask3OfControllerConfig(0)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_INT_MASK4(x) Can_30_Flexcan4_GetInterruptMask4OfControllerConfig(0)
#  endif
#  if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) && defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING ) /* COV_CAN_MULTIPLE_BASICCAN */
#   define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK1(x) Can_30_Flexcan4_GetRxBasicPollingMask1OfControllerConfig(0)
#   if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK2(x) Can_30_Flexcan4_GetRxBasicPollingMask2OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK3(x) Can_30_Flexcan4_GetRxBasicPollingMask3OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK4(x) Can_30_Flexcan4_GetRxBasicPollingMask4OfControllerConfig(0)
#   endif
#  endif
#  if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS ) && defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING )
#   define CAN_30_FLEXCAN4_RXFULL_POLL_MASK1(x) Can_30_Flexcan4_GetRxFullPollingMask1OfControllerConfig(0)
#   if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXFULL_POLL_MASK2(x) Can_30_Flexcan4_GetRxFullPollingMask2OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXFULL_POLL_MASK3(x) Can_30_Flexcan4_GetRxFullPollingMask3OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_RXFULL_POLL_MASK4(x) Can_30_Flexcan4_GetRxFullPollingMask4OfControllerConfig(0)
#   endif
#  endif
#  if defined( C_30_FLEXCAN4_ENABLE_TX_POLLING )
#   define CAN_30_FLEXCAN4_TX_POLL_MASK1(x) Can_30_Flexcan4_GetTxPollingMask1OfControllerConfig(0)
#   if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_TX_POLL_MASK2(x) Can_30_Flexcan4_GetTxPollingMask2OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_TX_POLL_MASK3(x) Can_30_Flexcan4_GetTxPollingMask3OfControllerConfig(0)
#   endif
#   if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#    define CAN_30_FLEXCAN4_TX_POLL_MASK4(x) Can_30_Flexcan4_GetTxPollingMask4OfControllerConfig(0)
#   endif
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
#   define CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(x) Can_30_Flexcan4_GetFDMailboxMaxWordLengthOfControllerConfig(0)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
#   define CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(x) Can_30_Flexcan4_GetNumberOfMaxMailboxesOfControllerConfig(x)
#  endif
#else /* C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL */
# define CAN_30_FLEXCAN4_INT_MASK1(x) Can_30_Flexcan4_GetInterruptMask1OfControllerConfig(x)
# if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define CAN_30_FLEXCAN4_INT_MASK2(x) Can_30_Flexcan4_GetInterruptMask2OfControllerConfig(x)
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define CAN_30_FLEXCAN4_INT_MASK3(x) Can_30_Flexcan4_GetInterruptMask3OfControllerConfig(x)
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define CAN_30_FLEXCAN4_INT_MASK4(x) Can_30_Flexcan4_GetInterruptMask4OfControllerConfig(x)
# endif
# if defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_OBJECTS ) && defined( C_30_FLEXCAN4_ENABLE_RX_BASICCAN_POLLING ) /* COV_CAN_MULTIPLE_BASICCAN */
#  define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK1(x) Can_30_Flexcan4_GetRxBasicPollingMask1OfControllerConfig(x)
#  if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK2(x) Can_30_Flexcan4_GetRxBasicPollingMask2OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK3(x) Can_30_Flexcan4_GetRxBasicPollingMask3OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXBASIC_POLL_MASK4(x) Can_30_Flexcan4_GetRxBasicPollingMask4OfControllerConfig(x)
#  endif
# endif
# if defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_OBJECTS ) && defined( C_30_FLEXCAN4_ENABLE_RX_FULLCAN_POLLING )
#  define CAN_30_FLEXCAN4_RXFULL_POLL_MASK1(x) Can_30_Flexcan4_GetRxFullPollingMask1OfControllerConfig(x)
#  if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXFULL_POLL_MASK2(x) Can_30_Flexcan4_GetRxFullPollingMask2OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXFULL_POLL_MASK3(x) Can_30_Flexcan4_GetRxFullPollingMask3OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_RXFULL_POLL_MASK4(x) Can_30_Flexcan4_GetRxFullPollingMask4OfControllerConfig(x)
#  endif
# endif
# if defined( C_30_FLEXCAN4_ENABLE_TX_POLLING )
#  define CAN_30_FLEXCAN4_TX_POLL_MASK1(x) Can_30_Flexcan4_GetTxPollingMask1OfControllerConfig(x)
#  if defined ( C_30_FLEXCAN4_ENABLE_MB32TO63 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#  define CAN_30_FLEXCAN4_TX_POLL_MASK2(x) Can_30_Flexcan4_GetTxPollingMask2OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB64TO95 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_TX_POLL_MASK3(x) Can_30_Flexcan4_GetTxPollingMask3OfControllerConfig(x)
#  endif
#  if defined ( C_30_FLEXCAN4_ENABLE_MB96TO127 ) /* COV_CAN_FLEXCAN4_DERIVATIVE_NUMBER_OF_HW_MB */
#   define CAN_30_FLEXCAN4_TX_POLL_MASK4(x) Can_30_Flexcan4_GetTxPollingMask4OfControllerConfig(x)
#  endif
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
#  define CAN_30_FLEXCAN4FD_MAILBOX_MAX_WORD_LENGTH(x) Can_30_Flexcan4_GetFDMailboxMaxWordLengthOfControllerConfig(x)
# endif
# if defined ( C_30_FLEXCAN4_ENABLE_ASYM_MAILBOXES ) /* COV_CAN_FLEXCAN4_DERIVATIVE_ASYM_MAILBOXES */
#  define CAN_30_FLEXCAN4_NUMBER_OF_MAX_MAILBOXES(x) Can_30_Flexcan4_GetNumberOfMaxMailboxesOfControllerConfig(x)
# endif
#endif /* C_30_FLEXCAN4_SINGLE_RECEIVE_CHANNEL */

#if defined ( C_30_FLEXCAN4_ENABLE_CAN_FD_FULL )
# if defined( C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS )
#  define CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)     ((tCan_30_Flexcan4MsgObjPtr)(((vuint32)(Can_30_Flexcan4_GetFDMailboxMapping((vuint16)(Can_30_Flexcan4_GetFDHwChannelIndirection((hwCh)))+(vuint16)(hwObjHandle))) - (vuint32)(Can_30_Flexcan4_GetBaseAddressOfControllerConfig(hwCh))) + (vuint32)(canLL_30_Flexcan4_VirtualPtrField[(hwCh)])))->structMember
#  define CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)    ((tCan_30_Flexcan4MsgObjPtr)(((vuint32)(Can_30_Flexcan4_GetFDMailboxMapping((vuint16)(Can_30_Flexcan4_GetFDHwChannelIndirection((hwCh)))+(vuint16)(hwObjHandle))) - (vuint32)(Can_30_Flexcan4_GetBaseAddressOfControllerConfig(hwCh))) + (vuint32)(canLL_30_Flexcan4_VirtualPtrField[(hwCh)])))->structMember
# else
#  define CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)     ((tCan_30_Flexcan4MsgObjPtr)(Can_30_Flexcan4_GetFDMailboxMapping((vuint16)(Can_30_Flexcan4_GetFDHwChannelIndirection((hwCh)))+(vuint16)(hwObjHandle))))->structMember
#  define CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)    ((tCan_30_Flexcan4MsgObjPtr)(Can_30_Flexcan4_GetFDMailboxMapping((vuint16)(Can_30_Flexcan4_GetFDHwChannelIndirection((hwCh)))+(vuint16)(hwObjHandle))))->structMember
# endif
#else
# define CAN_30_FLEXCAN4_LOCAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)      pFlex_30_Flexcan4CANLocal->canObject[(hwObjHandle)].structMember
# define CAN_30_FLEXCAN4_GLOBAL_MAILBOX_ACCESS(hwCh, hwObjHandle, structMember)     pFlex_30_Flexcan4CAN((hwCh))->canObject[(hwObjHandle)].structMember
#endif

#define CANSFR_TYPE        vuint32

/* PRQA L:FctLikeMacroLabel */

/* \trace SPEC-1400 */
/***************************************************************************/
/* function prototypes / API                                               */
/***************************************************************************/
#define CAN_30_FLEXCAN4_START_SEC_CODE  /*---------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/************************************************************************************************************
 *  Can_30_Flexcan4_InitMemory()
 ***********************************************************************************************************/
/*! \brief         Power-up memory initialization
 *  \details       Initializes component variables in *_INIT_* sections at power up. \n
 *                 Use this to re-run the system without performing a new start from power on. \n
 *                 (E.g.: used to support an ongoing debug session without a complete re-initialization.) \n
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Use this function in case these variables are not initialized by the 
 *                 start up code. \n
 *                 Must be followed by a call to "Can_30_Flexcan4_Init()".
 *  \trace         CREQ-970
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_InitMemory( void );

/************************************************************************************************************
 *  Can_30_Flexcan4_Init()
 ***********************************************************************************************************/
/*! \brief         Initializes component
 *  \details       Initializes all component variables and sets the component state to initialized.
 *  \param[in]     ConfigPtr          Component configuration structure
 *  \pre           Interrupts are disabled.
 *  \pre           Module is uninitialized.
 *  \pre           Can_30_Flexcan4_InitMemory() has been called unless CAN Module is initialized by start up code.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Parameter "ConfigPtr" will be taken into account only for "Multiple ECU Configuration" and in Post-Build variant. \n
 *  \trace         CREQ-969
************************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_Init( Can_30_Flexcan4_ConfigPtrType ConfigPtr );

#if ((CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON) || (CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_OFF))
/************************************************************************************************************
 *  Can_30_Flexcan4_ChangeBaudrate()
 ***********************************************************************************************************/
/*! \brief         Change baud rate
 *  \details       This service shall change the baud rate and reinitialize the CAN controller.
 *  \param[in]     Controller    CAN controller to be changed
 *  \param[in]     Baudrate      Baud rate to be set
 *  \return        E_NOT_OK      Baud rate is not set
 *  \return        E_OK          Baud rate is set
 *  \pre           The CAN controller must be in "STOP Mode".
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        (CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON) || (CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_OFF)
 *  \notes         Has to be called during the start up sequence before CAN communication takes place but after calling "Can_30_Flexcan4_Init()".
 *  \trace         CREQ-995
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_ChangeBaudrate( uint8 Controller, V_DEF_30_FLEXCAN4_CONST(V_NONE, uint16, APPL_CONST) Baudrate );
#endif

#if (CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON)
/************************************************************************************************************
 *  Can_30_Flexcan4_CheckBaudrate()
 ***********************************************************************************************************/
/*! \brief         Checks baud rate
 *  \details       This service shall check if the given baud rate is supported of the CAN controller.
 *  \param[in]     Controller    CAN controller to be checked
 *  \param[in]     Baudrate      Baud rate to be checked
 *  \return        E_NOT_OK      Baud rate is not available
 *  \return        E_OK          Baud rate is available
 *  \pre           The CAN controller must be initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_CHANGE_BAUDRATE_API == STD_ON
 *  \trace         CREQ-995
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_CheckBaudrate( uint8 Controller, V_DEF_30_FLEXCAN4_CONST(V_NONE, uint16, APPL_CONST) Baudrate );
#endif

#if (CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_ON)
/************************************************************************************************************
 *  Can_30_Flexcan4_SetBaudrate()
 ***********************************************************************************************************/
/*! \brief         Set baud rate
 *  \details       This service shall change the baud rate and reinitialize the CAN controller. \n
 *                 (Similar to Can_30_Flexcan4_ChangeBaudrate() but used when identical baud rates are used for different CAN FD settings).
 *  \param[in]     Controller        CAN controller to be set
 *  \param[in]     BaudRateConfigID  Identity of the configured baud rate (available as Symbolic Name)
 *  \return        E_NOT_OK      Baud rate is not set
 *  \return        E_OK          Baud rate is set
 *  \pre           The CAN controller must be in "STOP Mode".
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_SET_BAUDRATE_API == STD_ON
 *  \trace         CREQ-995
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );
#endif


#if (CAN_30_FLEXCAN4_VERSION_INFO_API == STD_ON) /* BR:007 */
/************************************************************************************************************
 *  Can_30_Flexcan4_GetVersionInfo()
 ***********************************************************************************************************/
/*! \brief         Returns the version information
 *  \details       Returns version information (MICROSAR3: BCD coded), vendor ID and AUTOSAR module ID.
 *  \param[out]    VersionInfo    Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_VERSION_INFO_API == STD_ON
 *  \trace         CREQ-991
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_GetVersionInfo( Can_30_Flexcan4_VersionInfoPtrType VersionInfo );
#endif

#if (CAN_30_FLEXCAN4_GET_STATUS == STD_ON)
/************************************************************************************************************
 *  Can_30_Flexcan4_GetStatus()
 ***********************************************************************************************************/
/*! \brief         Get status/mode of the given controller
 *  \details       Delivers the status of the hardware, as a bit coded value where multiple bits may be set.
 *  \param[in]     Controller   CAN controller requested for status information
 *  \return        CAN_30_FLEXCAN4_STATUS_START            START mode
 *  \return        CAN_30_FLEXCAN4_STATUS_STOP             STOP mode
 *  \return        CAN_30_FLEXCAN4_STATUS_INIT             Initialized controller
 *  \return        CAN_30_FLEXCAN4_STATUS_INCONSISTENT     STOP or SLEEP are inconsistent over common CAN controllers
 *  \return        CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER   RAM check failed CAN controller is deactivated
 *  \return        CAN_30_FLEXCAN4_STATUS_WARNING          WARNING state
 *  \return        CAN_30_FLEXCAN4_STATUS_PASSIVE          PASSIVE state
 *  \return        CAN_30_FLEXCAN4_STATUS_BUSOFF           BUSOFF mode
 *  \return        CAN_30_FLEXCAN4_STATUS_SLEEP            SLEEP mode
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GET_STATUS == STD_ON
 *  \notes         the return value can be analysed using the provided API macros: \n
                   CAN_30_FLEXCAN4_HW_IS_OK(), CAN_30_FLEXCAN4_HW_IS_WARNING(), CAN_30_FLEXCAN4_HW_IS_PASSIVE() \n
                   CAN_30_FLEXCAN4_HW_IS_BUSOFF(), CAN_30_FLEXCAN4_HW_IS_WAKEUP(), CAN_30_FLEXCAN4_HW_IS_SLEEP()\n
                   CAN_30_FLEXCAN4_HW_IS_STOP(), CAN_30_FLEXCAN4_HW_IS_START(), CAN_30_FLEXCAN4_HW_IS_INCONSISTENT()\n
                   None AUTOSAR API
 *  \trace         CREQ-978
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, uint8, CODE) Can_30_Flexcan4_GetStatus( uint8 Controller );

/************************************************************************************************************
 *  Can_30_Flexcan4_GetControllerMode()
 ***********************************************************************************************************/
/*! \brief         Get mode of the given controller
 *  \details       Delivers the controllers mode.
 *  \param[in]     Controller         CAN controller requested for status information
 *  \param[out]    ControllerModePtr  pointer to variable to store CAN controllers mode. Parameter must not be NULL.
 *  \return        E_NOT_OK           Controller mode request has not been accepted
 *  \return        E_OK               Controller mode request has been accepted
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GET_STATUS == STD_ON
 *  \trace         CREQ-178460
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerMode( uint8 Controller, Can_30_Flexcan4_ControllerStatePtrType ControllerModePtr );

/************************************************************************************************************
 *  Can_30_Flexcan4_GetControllerErrorState()
 ***********************************************************************************************************/
/*! \brief         Get status of the given controller
 *  \details       Delivers the controllers error state.
 *  \param[in]     Controller         CAN controller requested for status information
 *  \param[out]    ErrorStatePtr      pointer to variable to store AN controllers error state. Parameter must not be NULL.
 *  \return        E_NOT_OK           Controller state request has not been accepted
 *  \return        E_OK               Controller state request has been accepted
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GET_STATUS == STD_ON
 *  \trace         CREQ-178459
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerErrorState( uint8 Controller, Can_30_Flexcan4_ErrorStatePtrType ErrorStatePtr );

/************************************************************************************************************
 *  Can_30_Flexcan4_GetControllerTxErrorCounter()
 ***********************************************************************************************************/
/*! \brief         Get status of the given controller
 *  \details       Delivers the controllers TX error counter.
 *  \param[in]     Controller         CAN controller requested for TX error counter.
 *  \param[out]    TxErrorCounterPtr  pointer to variable to store CAN controllers TX error counter. Parameter must not be NULL.
 *  \return        E_NOT_OK           Controller TX error counter not available
 *  \return        E_OK               Controller TX error counter available
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GET_STATUS == STD_ON
 *  \trace         CREQ-178459
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerTxErrorCounter( uint8 Controller, Can_30_Flexcan4_ErrorCounterPtrType TxErrorCounterPtr );

/************************************************************************************************************
 *  Can_30_Flexcan4_GetControllerRxErrorCounter()
 ***********************************************************************************************************/
/*! \brief         Get status of the given controller
 *  \details       Delivers the controllers RX error counter.
 *  \param[in]     Controller         CAN controller requested for RX error counter.
 *  \param[out]    RxErrorCounterPtr  pointer to variable to store CAN controllers RX error counter. Parameter must not be NULL.
 *  \return        E_NOT_OK           Controller RX error counter not available
 *  \return        E_OK               Controller RX error counter available
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GET_STATUS == STD_ON
 *  \trace         CREQ-178459
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_GetControllerRxErrorCounter( uint8 Controller, Can_30_Flexcan4_ErrorCounterPtrType RxErrorCounterPtr );

#endif

/************************************************************************************************************
 *  Can_30_Flexcan4_SetControllerMode()
 ***********************************************************************************************************/
/*! \brief         Change the controller mode
 *  \details       Request a mode transition that will be processed a short time synchronous, \n
 *                 (may be finished ansynconous in Can_30_Flexcan4_MainFunction_Mode()) and call a notification when successful. \n
 *                 BUSOFF, WAKEUP and RAM check will be handled as well.
 *  \param[in]     Controller    CAN controller to be set
 *  \param[in]     Transition    Requested transition to destination mode CAN_T_START, CAN_T_STOP, CAN_T_SLEEP, CAN_T_WAKEUP
 *  \return        CAN_NOT_OK    transition request rejected
 *  \return        CAN_OK        transition request accepted
 *  \pre           Interrupts locked
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *  \notes         Must not be called within CAN driver context like RX, TX or BUSOFF call out.
 *  \trace         CREQ-978
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_SetControllerMode( uint8 Controller, Can_StateTransitionType Transition );

/************************************************************************************************************
 *  Can_30_Flexcan4_Write()
 ***********************************************************************************************************/
/*! \brief         Send a CAN message
 *  \details       copy data, DLC and ID to the send mailbox and request a transmission.
 *  \param[in]     Hth           Handle of the mailbox intended to send the message
 *  \param[in]     PduInfo       Information about the outgoing message (ID, dataLength, data)
 *  \return        CAN_NOT_OK    transmit request rejected
 *  \return        CAN_OK        transmit request successful
 *  \return        CAN_BUSY      transmit request could not be accomplished due to the controller is busy.
 *  \pre           disabled CAN interrupts / interrupts locked (Refer to critical section).
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-971
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_Write( Can_HwHandleType Hth, Can_30_Flexcan4_PduInfoPtrType PduInfo );

/************************************************************************************************************
 *  Can_30_Flexcan4_CancelTx()
 ***********************************************************************************************************/
/*! \brief         Cancel TX message
 *  \details       Cancel the TX message in the hardware buffer (if possible) or mark the message as not to be confirmed \n
 *                 in case of the cancellation is unsuccessful.
 *  \param[in]     Hth     Handle of the mailbox intended to be cancelled.
 *  \param[in]     PduId   PDU identifier
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Do not call this API within TX confirmation context. Otherwise confirmation will occur for already
 *                 cancelled messages. Only one message will be cancelled, if there are multiple requests also multiple cancellations expected.\n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_CancelTx( Can_HwHandleType Hth, PduIdType PduId );

#if defined(C_30_FLEXCAN4_ENABLE_MIRROR_MODE)
/************************************************************************************************************
 *  Can_30_Flexcan4_SetMirrorMode()
 ***********************************************************************************************************/
/*! \brief         Activate mirror mode 
 *  \details       Switch the Appl_GenericPreCopy/Confirmation function ON or OFF.
 *  \param[in]     Controller   CAN controller
 *  \param[in]     mirrorMode   Activate or deactivate the mirror mode.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_MIRROR_MODE (user configuration file)
 *  \notes         Called by "Mirror Mode" CDD.\n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_SetMirrorMode( uint8 Controller, CddMirror_MirrorModeType mirrorMode );
#endif

#if defined(C_30_FLEXCAN4_ENABLE_SILENT_MODE)
/************************************************************************************************************
 *  Can_30_Flexcan4_SetSilentMode()
 ***********************************************************************************************************/
/*! \brief         Activate and deactivate the silent mode
 *  \details       Switch to silent mode, as a listen only mode without ACK. Also deactivate this mode again.
 *  \param[in]     Controller   CAN controller
 *  \param[in]     silentMode   Activate or deactivate the silent mode CAN_SILENT_INACTIVE, CAN_SILENT_ACTIVE (Enumaration).
 *  \return        E_OK         mode change successful
 *  \return        E_NOT_OK     mode change failed
 *  \pre           The CAN controller must be in "STOP Mode".
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_SILENT_MODE == STD_ON
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_SetSilentMode( uint8 Controller, Can_30_Flexcan4_SilentModeType silentMode );
#endif

/************************************************************************************************************
 *  Can_30_Flexcan4_CheckWakeup()
 ***********************************************************************************************************/
/*! \brief         Check WAKEUP occur
 *  \details       Check the occurrence of WAKEUP events for the given controller \n
 *                 (used as WAKEUP callback for higher layers).
 *  \param[in]     Controller           CAN controller to be checked for WAKEUP events.
 *  \return        E_OK/CAN_OK          the given controller caused a WAKEUP before.
 *  \return        E_NOT_OK/CAN_NOT_OK  the given controller caused no WAKEUP before.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-983
 *  \config        return type depend on CAN_30_FLEXCAN4_CHECK_WAKEUP_CAN_RET_TYPE
************************************************************************************************************/
#if defined(C_30_FLEXCAN4_ENABLE_CHECK_WAKEUP_CAN_RET_TYPE) /* COV_CAN_COMPATIBILITY */
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, CODE) Can_30_Flexcan4_CheckWakeup( uint8 Controller );
#else
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Std_ReturnType, CODE) Can_30_Flexcan4_CheckWakeup( uint8 Controller );
#endif

/************************************************************************************************************
 *  Can_30_Flexcan4_DisableControllerInterrupts()
 ***********************************************************************************************************/
/*! \brief         Disable CAN interrupts
 *  \details       Disable the CAN interrupt for the given controller (e.g. due to data consistency reasons).
 *  \param[in]     Controller    CAN controller to disable interrupts for.
 *  \pre           Must not be called while CAN controller is in SLEEP mode.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Called within Critical Area handling or out of Application code.
 *  \trace         CREQ-988
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_DisableControllerInterrupts( uint8 Controller );

/************************************************************************************************************
 *  Can_30_Flexcan4_EnableControllerInterrupts()
 ***********************************************************************************************************/
/*! \brief         Enable CAN interrupts
 *  \details       Re-enable the CAN interrupt for the given controller (e.g. due to data consistency reasons).
 *  \param[in]     Controller    CAN controller to enable interrupts for.
 *  \pre           Must not be called while CAN controller is in SLEEP mode.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Called within Critical Area handling or out of Application code.
 *  \trace         CREQ-988
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_EnableControllerInterrupts( uint8 Controller );

/* polling functions (always available for non polled systems also) */
/************************************************************************************************************
 *  Can_30_Flexcan4_MainFunction_Write()
 ***********************************************************************************************************/
/*! \brief         TX message observation
 *  \details       Polling TX events (confirmation, cancellation) for all controllers and all TX mailboxes \n
 *                 to accomplish the TX confirmation handling (like CanInterface notification).
 *  \pre           Must not interrupt the call of Can_30_Flexcan4_Write(). CAN driver has to be initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Empty function in case no polling is active.
 *  \trace         CREQ-979
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, CODE) Can_30_Flexcan4_MainFunction_Write( void ); /* PRQA S 3449, 3451 */ /* MD_Can_HL_ExternalDeclaration */

/************************************************************************************************************
 *  Can_30_Flexcan4_MainFunction_Read()
 ***********************************************************************************************************/
/*! \brief         RX message observation
 *  \details       Polling RX events for all controllers and all RX mailboxes to accomplish the \n
 *                 RX indication handling (like CanInterface notification). \n
 *                 Also used for a delayed read (from task level) of the RX Queue messages which were queued from interrupt context.
 *  \pre           CAN driver has to be initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Empty function in case no polling is active.
 *  \trace         CREQ-979
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, CODE) Can_30_Flexcan4_MainFunction_Read( void ); /* PRQA S 3449, 3451 */ /* MD_Can_HL_ExternalDeclaration */

/************************************************************************************************************
 *  Can_30_Flexcan4_MainFunction_BusOff()
 ***********************************************************************************************************/
/*! \brief         BUSOFF observation
 *  \details       Polling of BUSOFF events to accomplish the BUSOFF handling.
 *                 Service function to poll BUSOFF events for all controllers to accomplish the BUSOFF handling \n
 *                 (like calling of CanIf_30_Flexcan4_ControllerBusOff() in case of BUSOFF occurrence).
 *  \pre           CAN driver has to be initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Empty function in case no polling is active.
 *  \trace         CREQ-979
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, CODE) Can_30_Flexcan4_MainFunction_BusOff( void ); /* PRQA S 3449, 3451 */ /* MD_Can_HL_ExternalDeclaration */

/************************************************************************************************************
 *  Can_30_Flexcan4_MainFunction_Wakeup()
 ***********************************************************************************************************/
/*! \brief         WAKEUP observation
 *  \details       Polling WAKEUP events for all controllers to accomplish the WAKEUP handling \n
 *                 (like calling of "CanIf_30_Flexcan4_SetWakeupEvent()" in case of WAKEUP occurrence).
 *  \pre           CAN driver has to be initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         Empty function in case no polling is active.
 *  \trace         CREQ-983
 *  \trace         CREQ-979
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, CODE) Can_30_Flexcan4_MainFunction_Wakeup( void ); /* PRQA S 3449, 3451 */ /* MD_Can_HL_ExternalDeclaration */

/************************************************************************************************************
 *  Can_30_Flexcan4_MainFunction_Mode()
 ***********************************************************************************************************/
/*! \brief         Mode transition observation
 *  \details       Polling Mode changes over all controllers. \n
                   (This is handled asynchronous if not accomplished in "Can_30_Flexcan4_SetControllerMode()")
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ-978
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, CODE) Can_30_Flexcan4_MainFunction_Mode( void ); /* PRQA S 3449, 3451 */ /* MD_Can_HL_ExternalDeclaration */

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED)
/************************************************************************************************************
 *  Can_30_Flexcan4_RamCheckExecute()
 ***********************************************************************************************************/
/*! \brief         Start checking the CAN cells RAM
 *  \details       Check all controller specific and mailbox specific registers by write patterns and read back.
 *                 Issue notification will appear in this context.
 *  \param[in]     Controller    CAN controller to be checked.
 *  \pre           Has to be called within STOP mode.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_RAM_CHECK == CAN_EXTENDED
 *  \trace         CREQ-106641
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckExecute( uint8 Controller );

/************************************************************************************************************
 *  Can_30_Flexcan4_RamCheckEnableMailbox()
 ***********************************************************************************************************/
/*! \brief         Reactivate a mailbox after RamCheck failed.
 *  \details       Mailbox will be reactivated by clear deactivation flag.
 *  \param[in]     htrh    CAN mailbox to be reactivated.
 *  \pre           Has to be called within STOP mode after RamCheck failed (controller is deactivated).
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \notes         must be followed by Can_30_Flexcan4_RamCheckEnableController() to activate mailbox and controller.
 *  \config        CAN_RAM_CHECK == CAN_EXTENDED
 *  \trace         CREQ-106641
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckEnableMailbox( Can_HwHandleType htrh );

/************************************************************************************************************
 *  Can_30_Flexcan4_RamCheckEnableController()
 ***********************************************************************************************************/
/*! \brief         Reactivate CAN cells after RamCheck failed.
 *  \details       CAN cell will be reactivated by execute reinitialization.
 *  \param[in]     Controller    CAN controller to be reactivated.
 *  \pre           Has to be called within STOP mode after RamCheck failed (controller is deactivated).
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_RAM_CHECK == CAN_EXTENDED
 *  \trace         CREQ-106641
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4_RamCheckEnableController( uint8 Controller );
#endif

/* Platform specific API */

/* ISR prototypes (see can_irq.c) */

/* Possibility to disable the prototypes of interrupt service routines in the driver configuration header file.  */
# if !defined( C_30_FLEXCAN4_ENABLE_ISR_PROTOTYPE ) && !defined( C_30_FLEXCAN4_DISABLE_ISR_PROTOTYPE ) /* COV_CAN_LL_ISR_PROTOTYPE TF tf xf */
#  define C_30_FLEXCAN4_ENABLE_ISR_PROTOTYPE
# endif

#if defined (C_30_FLEXCAN4_ENABLE_ISR_PROTOTYPE)
/* if following #if is false: ISR prototypes must be provided by OSEK header file */
# if !defined(C_30_FLEXCAN4_ENABLE_OSEK_OS) || !defined(C_30_FLEXCAN4_ENABLE_OSEK_OS_INTCAT2) /* COV_CAN_OS_USAGE */
#  if defined( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_SINGLE */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_0()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_0.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_0( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_0 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_1()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_1.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_1( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_1 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_2()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_2.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_2( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_2 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_3()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_3.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_3( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_3 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_4()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_4.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_4( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_4 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_5()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_5.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_5( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_5 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_6()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_6.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_6( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_6 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
        defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Isr_7()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for events on CAN_30_FLEXCAN4_7.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4Isr_7( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_7 */
#  endif /* C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE */

#  if defined ( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_0 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_0()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_0.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_0( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_0()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_0.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_0( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_0()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_0.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_0( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_0 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_1 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_1()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_1.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_1( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_1()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_1.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_1( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_1()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_1.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_1( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_1 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_2 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_2()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_2.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_2( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_2()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_2.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_2( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_2()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_2.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_2( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_2 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_3 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_3()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_3.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_3( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_3()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_3.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_3( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_3()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_3.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_3( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_3 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_4 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_4()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_4.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_4( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_4()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_4.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_4( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_4()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_4.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_4( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_4 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_5 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_5()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_5.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_5( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_5()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_5.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_5( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_5()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_5.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_5( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_5 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_6 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_6()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_6.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_6( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_6()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_6.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_6( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_6()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_6.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_6( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_6 */
#   if defined( kCan_30_Flexcan4PhysToLogChannelIndex_7 ) /* COV_CAN_EQUAL_CHANNEL_LAYOUT */
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffIsr_7()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_7.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4BusOffIsr_7( void );
/* CODE CATEGORY 1 END */
#      if defined (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE) && defined (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING) /* COV_CAN_ERROR_INTERRUPT_AVAILABLE */
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4ErrorIsr_7()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for BUSOFF event on CAN_30_FLEXCAN4_7.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT && (C_30_FLEXCAN4_ENABLE_FLEXCAN_ERROR_INTERRUPT_AVAILABLE)
 *                 && (CAN_30_FLEXCAN4_ENABLE_SECURITY_EVENT_REPORTING)
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE)  Can_30_Flexcan4ErrorIsr_7( void );
/* CODE CATEGORY 1 END */
#      endif
#    endif
#    if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxIsr_7()
 ***********************************************************************************************************/
/*! \brief         Interrupt service function for transmission and reception events on CAN_30_FLEXCAN4_7.
 *  \details       Calls the interrupt handler.
 *  \context       ISR1|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 *  \notes         None AUTOSAR API
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC_API(V_NONE, void, CODE) Can_30_Flexcan4MailboxIsr_7( void );
/* CODE CATEGORY 1 END */
#    endif
#   endif /* kCan_30_Flexcan4PhysToLogChannelIndex_7 */
#  endif /* C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE */

# endif
#endif /* C_30_FLEXCAN4_ENABLE_ISR_PROTOTYPE */

# if defined( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_SINGLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_SINGLE */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )   || \
      defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT ) /* COV_CAN_INTERRUPT_SUPPORT */ 
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4Interrupt()
 ***********************************************************************************************************/
/*! \brief         Handles all interrupt events.
 *  \details       Checks interrupt events and calls the indication or confirmation function.
 *  \param[in]     Controller          CAN controller (configuration dependent)
 *  \context       ISR1|ISR2
 *  \reentrant     TRUE for different handles
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT || C_30_FLEXCAN4_ENABLE_CAN_WAKEUP_INTERRUPT
 *  \pre           -
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4Interrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END */
#  endif
# endif

# if defined ( C_30_FLEXCAN4_ENABLE_INTERRUPT_SOURCE_MULTIPLE ) /* COV_CAN_FLEXCAN4_DERIVATIVE_INTERRUPT_SOURCE_MULTIPLE */
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4BusOffInterrupt()
 ***********************************************************************************************************/
/*! \brief         Handles BUSOFF interrupt events.
 *  \details       Calls error handling.
 *  \param[in]     Controller          CAN controller (configuration dependent)
 *  \context       ISR1|ISR2
 *  \reentrant     TRUE for different handles
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_BUSOFF_INTERRUPT
 *  \pre           -
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4BusOffInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END */
#  endif
#  if defined( C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT )
/* CODE CATEGORY 1 START */
/************************************************************************************************************
 *  Can_30_Flexcan4MailboxInterrupt()
 ***********************************************************************************************************/
/*! \brief         Handles transmission and reception interrupt events.
 *  \details       Checks interrupt events and calls the indication or confirmation function.
 *  \param[in]     Controller          CAN controller (configuration dependent)
 *  \context       ISR1|ISR2
 *  \reentrant     TRUE for different handles
 *  \synchronous   TRUE
 *  \config        C_30_FLEXCAN4_ENABLE_CAN_RXTX_INTERRUPT
 *  \pre           -
 ***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, CODE) Can_30_Flexcan4MailboxInterrupt( CAN_30_FLEXCAN4_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END */
#  endif
# endif

# if !defined( C_30_FLEXCAN4_ENABLE_PROTECTED_RX_PROCESS ) && !defined( C_30_FLEXCAN4_DISABLE_PROTECTED_RX_PROCESS )
#  define C_30_FLEXCAN4_ENABLE_PROTECTED_RX_PROCESS
# endif



#define CAN_30_FLEXCAN4_STOP_SEC_CODE  /*------------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CAN_30_FLEXCAN4_START_SEC_CODE_APPL  /*----------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_ON)
/************************************************************************************************************
 *  Appl_30_Flexcan4_GenericPrecopy()
 ***********************************************************************************************************/
/*! \brief         Common RX indication callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about all incoming RX messages including the contained data. \n
 *                 It can be used to block notification to upper layer. E.g. to filter incoming messages or route it for special handling.
 *  \param[in]     Controller   CAN controller which received the message.
 *  \param[in]     ID           ID of the received message (include IDE,FD). \n
 *                              In case of extended or mixed ID systems the highest bit (bit 31) is set to mark an extended ID. \n
 *                              FD-bit (bit 30) can be masked out with a define CAN_30_FLEXCAN4_ID_MASK_IN_GENERIC_CALLOUT in the user config file.
 *  \param[in]     DataLength   Data length of the received message.
 *  \param[in]     pData        Pointer to the data of the received message (read only).
 *  \return        CAN_OK       Higher layer indication will be called afterwards (CanIf_RxIndication()).
 *  \return        CAN_NOT_OK   Higher layer indication will not be called afterwards.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GENERIC_PRECOPY == STD_ON
 *  \notes         "pData" is read only and must not be accessed for further write operations. \n
 *                 The parameter DataLength refers to the received data length by the CAN controller hardware.\n
 *                 Note, that the CAN protocol allows the usage of data length values greater than eight (CAN-FD).\n
 *                 Depending on the implementation of this callback it may be necessary to consider this special case (e.g. if the data length is used as index value in a buffer write access).
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, Can_ReturnType, APPL_CODE) Appl_30_Flexcan4_GenericPrecopy( uint8 Controller, Can_IdType ID, uint8 DataLength, Can_30_Flexcan4_DataPtrType DataPtr );
#endif

#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API1)
/************************************************************************************************************
 *  Appl_30_Flexcan4_GenericConfirmation()
 ***********************************************************************************************************/
/*! \brief         Common TX notification callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about TX messages being sent to the CAN bus.
 *                 It can be used to block confirmation or route the information to other layer as well.
 *  \param[in]     PduId       Handle of the PDU specifying the message.
 *  \return        CAN_OK      Higher layer confirmation will be called afterwards (CanIf_TxConfirmation()).
 *  \return        CAN_NOT_OK  Higher layer confirmation will not be called afterwards.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GENERIC_CONFIRMATION == STD_ON
 *  \notes         "PduId" is read only and must not be accessed for further write operations. \n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, Can_ReturnType, APPL_CODE) Appl_30_Flexcan4_GenericConfirmation( PduIdType PduId );
#endif

#if defined(CAN_30_FLEXCAN4_ENABLE_GENERIC_CONFIRMATION_API2)
/************************************************************************************************************
 *  Appl_30_Flexcan4_GenericConfirmation()
 ***********************************************************************************************************/
/*! \brief         Common TX notification callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about TX messages being sent to the CAN bus.
 *                 It can be used to block confirmation or route the information to other layer as well.
 *  \param[in]     Controller  CAN controller which send the message.
 *  \param[in]     DataPtr     Pointer to a Can_PduType structure including ID (contain IDE,FD bit), DataLength, PDU and data pointer.
 *  \return        CAN_OK      Higher layer (CanInterface) confirmation will be called.
 *  \return        CAN_NOT_OK  No further higher layer (CanInterface) confirmation will be called.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GENERIC_CONFIRMATION == CAN_30_FLEXCAN4_API2
 *  \notes         A new transmission within this call out will corrupt the DataPtr context, if the CanHardwareObject is a FIFO object and the FIFO is full. \n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, Can_ReturnType, APPL_CODE) Appl_30_Flexcan4_GenericConfirmation( uint8 Controller, Can_30_Flexcan4_PduInfoPtrType DataPtr );
#endif

#if defined(C_30_FLEXCAN4_ENABLE_PRETRANSMIT_FCT)
/************************************************************************************************************
 *  Appl_30_Flexcan4_GenericPreTransmit()
 ***********************************************************************************************************/
/*! \brief         Common transmit callback.
 *  \details       Application callback function allowing the modification of the data to be transmitted (e.g.: add CRC).
 *  \param[in]     Controller  CAN controller on which the message will be send.
 *  \param[in]     DataPtr     Pointer to a Can_PduType structure including ID (contain IDE,FD bit), DataLength, PDU and data pointer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_GENERIC_PRETRANSMIT == STD_ON
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4_GenericPreTransmit( uint8 Controller, Can_30_Flexcan4_PduInfoPtrType_var DataPtr );
#endif

# if (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON)
/************************************************************************************************************
 *  Appl_30_Flexcan4CanTimerStart()
 ***********************************************************************************************************/
/*! \brief         Start time out monitoring
 *  \details       Service function to start an observation timer.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *                             (only if not using "Optimize for one controller")
 *  \param[in]     source      Source for the hardware observation.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON) && (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON)
 *  \notes         Please refer to chapter "Hardware Loop Check". \n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, APPL_CODE) Appl_30_Flexcan4CanTimerStart( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 source );

/************************************************************************************************************
 *  Appl_30_Flexcan4CanTimerLoop()
 ***********************************************************************************************************/
/*! \brief         Time out monitoring
 *  \details       Service function to check (against generated maximum loop value) whether a hardware loop shall be continued or broken.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *                             (only if not using "Optimize for one controller")
 *  \param[in]     source      Source for the hardware observation.
 *  \return        CAN_NOT_OK  when loop shall be broken (observation stops) \n
 *                             CAN_NOT_OK should only be used in case of a time out occurs due to a hardware issue. \n
 *                             After this an appropriate error handling is needed (see chapter Hardware Loop Check / Time out Monitoring).
 *  \return        CAN_OK      when loop shall be continued (observation continues)
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON) && (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON)
 *  \notes         Please refer to chapter "Hardware Loop Check". \n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, Can_ReturnType, APPL_CODE) Appl_30_Flexcan4CanTimerLoop( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 source );

/************************************************************************************************************
 *  Appl_30_Flexcan4CanTimerEnd()
 ***********************************************************************************************************/
/*! \brief         Stop time out monitoring
 *  \details       Service function to to end an observation timer.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *                             (only if not using "Optimize for one controller")
 *  \param[in]     source      Source for the hardware observation.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON) && (CAN_30_FLEXCAN4_HW_LOOP_SUPPORT_API == STD_ON)
 *  \notes         Please refer to chapter "Hardware Loop Check". \n
 *                 None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(V_NONE, void, APPL_CODE) Appl_30_Flexcan4CanTimerEnd( CAN_30_FLEXCAN4_CHANNEL_CANTYPE_FIRST uint8 source );
# endif

#if (CAN_30_FLEXCAN4_INTLOCK != CAN_30_FLEXCAN4_DRIVER)
/************************************************************************************************************
 *  Appl_30_Flexcan4CanInterruptDisable()
 ***********************************************************************************************************/
/*! \brief         CAN interrupt disabling by application
 *  \details       Disabling of CAN Interrupts by the application. \n
 *                 E.g.: the CAN driver itself should not access the common Interrupt Controller due to application \n
 *                 specific restrictions (like security level). Or the application like to be informed because of \n
 *                 an CAN interrupt lock. called by Can_30_Flexcan4_DisableControllerInterrupts().
 *  \param[in]     Controller   CAN controller for the CAN interrupt lock.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_APPL || CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4CanInterruptDisable( uint8 Controller );

/************************************************************************************************************
 *  Appl_30_Flexcan4CanInterruptRestore()
 ***********************************************************************************************************/
/*! \brief         CAN interrupt restore by application
 *  \details       Re-enabling of CAN Interrupts by the application. \n
                   E.g.: the CAN driver itself should not access the common Interrupt Controller due to application \n
                   specific restrictions (like security level). Or the application like to be informed because of \n
                   an CAN interrupt lock. called by Can_30_Flexcan4_EnableControllerInterrupts().
 *  \param[in]     Controller   CAN controller for the CAN interrupt unlock.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_APPL || CAN_30_FLEXCAN4_INTLOCK == CAN_30_FLEXCAN4_BOTH
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4CanInterruptRestore( uint8 Controller );
#endif

#if (CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL)
/************************************************************************************************************
 *  Appl_30_Flexcan4_CanOverrun()
 ***********************************************************************************************************/
/*! \brief         Overrun detection
 *  \details       Called when an overrun is detected for a BasicCAN mailbox. \n
 *                 Alternatively a DET call can be selected instead of ("CanOverrunNotification" is set to "DET").
 *  \param[in]     Controller   CAN controller for which the overrun was detected.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4_CanOverrun ( uint8 Controller );

/************************************************************************************************************
 *  Appl_30_Flexcan4_CanFullCanOverrun()
 ***********************************************************************************************************/
/*! \brief         Overrun detection
 *  \details       Called when an overrun is detected for a FullCAN mailbox. \n
 *                 Alternatively a DET call can be selected instead of ("CanOverrunNotification" is set to "DET").
 *  \param[in]     Controller   CAN controller for which the overrun was detected.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_OVERRUN_NOTIFICATION == CAN_30_FLEXCAN4_APPL
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4_CanFullCanOverrun ( uint8 Controller );
#endif

#if defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK)
# if !defined(C_30_FLEXCAN4_ENABLE_CAN_RAM_CHECK_EXTENDED) /* standard RAM_CHECK */
#  if defined(C_30_FLEXCAN4_ENABLE_NOTIFY_CORRUPT_MAILBOX)
/************************************************************************************************************
 *  Appl_30_Flexcan4_CanCorruptMailbox()
 ***********************************************************************************************************/
/*! \brief         Mailbox notification in case of RAM check failure.
 *  \details       Will notify the application (during re-initialization) about a defect mailbox within the CAN cell.
 *  \param[in]     Controller   CAN controller for which the check failed.
 *  \param[in]     hwObjHandle  Hardware handle of the defect mailbox.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_RAM_CHECK == CAN_30_FLEXCAN4_NOTIFY_MAILBOX
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4_CanCorruptMailbox(uint8 Controller, Can_HwHandleType hwObjHandle);
#  endif

/************************************************************************************************************
 *  Appl_30_Flexcan4_CanRamCheckFailed()
 ***********************************************************************************************************/
/*! \brief         CAN controller notification in case of RAM check failure
 *  \details       will notify the application (during re-initialization) about a defect CAN controller \n
 *                 due to a previous failed mailbox check. The return value decide how to proceed with the initialization.
 *  \param[in]     Controller   CAN controller for which the check failed.
 *  \return        CAN_30_FLEXCAN4_DEACTIVATE_CONTROLLER   deactivate the controller
 *  \return        CAN_30_FLEXCAN4_ACTIVATE_CONTROLLER     activate the controller
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        CAN_30_FLEXCAN4_RAM_CHECK == CAN_30_FLEXCAN4_NOTIFY_MAILBOX / CAN_30_FLEXCAN4_NOTIFY_ISSUE
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, uint8, APPL_CODE) Appl_30_Flexcan4_CanRamCheckFailed(uint8 Controller);
# endif /* standard RAM_CHECK */
#endif

# if defined ( C_30_FLEXCAN4_ENABLE_USER_MODE_APPL )
/************************************************************************************************************
 *  Appl_30_Flexcan4CanWriteProtectedRegister()
 ***********************************************************************************************************/
/*! \brief         Callback to write protected registers.
 *  \details       Application function which is called by the CAN driver to write special protected registers,
 *                 when CAN driver runs in user mode. 
 *  \param[in]     regPtr   Pointer to the register which has to be written.
 *  \param[in]     mask     The mask specifies the bits of the register to be written.
 *  \param[in]     value    The value which has to be written to the register.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        define C_30_FLEXCAN4_ENABLE_USER_MODE_APPL via user configuration file
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4CanWriteProtectedRegister8(Can_30_Flexcan4ChipMsgPtr8 regPtr, uint8 mask, uint8 value);
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4CanWriteProtectedRegister16(Can_30_Flexcan4ChipMsgPtr16 regPtr, uint16 mask, uint16 value);
V_DEF_30_FLEXCAN4_FUNC(extern, void, APPL_CODE) Appl_30_Flexcan4CanWriteProtectedRegister32(Can_30_Flexcan4ChipMsgPtr32 regPtr, uint32 mask, uint32 value);
/************************************************************************************************************
 *  Appl_30_Flexcan4CanReadProtectedRegister()
 ***********************************************************************************************************/
/*! \brief         Callback to read protected registers.
 *  \details       Application function which is called by the CAN driver to read special protected registers,
 *                 when CAN driver runs in user mode.
 *  \return        value    Returns the value of the register that was read
 *  \param[in]     regPtr   Pointer to the register which has to be written.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        define C_30_FLEXCAN4_ENABLE_USER_MODE_APPL via user configuration file
 *  \notes         None AUTOSAR API
***********************************************************************************************************/
V_DEF_30_FLEXCAN4_FUNC(extern, uint8, APPL_CODE) Appl_30_Flexcan4CanReadProtectedRegister8(Can_30_Flexcan4ChipMsgPtr8 regPtr);
V_DEF_30_FLEXCAN4_FUNC(extern, uint16, APPL_CODE) Appl_30_Flexcan4CanReadProtectedRegister16(Can_30_Flexcan4ChipMsgPtr16 regPtr);
V_DEF_30_FLEXCAN4_FUNC(extern, uint32, APPL_CODE) Appl_30_Flexcan4CanReadProtectedRegister32(Can_30_Flexcan4ChipMsgPtr32 regPtr);
# endif


/* \trace SPEC-1588 */
#if defined(C_30_FLEXCAN4_ENABLE_UPDATE_BASE_ADDRESS)
/* CODE CATEGORY 4 START */
V_DEF_30_FLEXCAN4_FUNC_CBK(V_NONE, void*, APPL_CODE) Appl_30_Flexcan4CanPowerOnGetBaseAddress(vuint32 physAddr, vuint16 size);
/* CODE CATEGORY 4 END */
#endif



#define CAN_30_FLEXCAN4_STOP_SEC_CODE_APPL  /*-----------------------------------------*/
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if !defined(C_DRV_30_FLEXCAN4_INTERNAL) /* COV_CAN_INTERNAL */
# if defined(CAN_30_FLEXCAN4_HL_NO_V_DEF) /* COV_CAN_INTERNAL */
/* only use internal when not defined before - undef to get no conflict in CBD - MSR mixed systems */
#  undef vuint8   /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
#  undef vsint8   /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
#  undef vuint16  /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
#  undef vsint16  /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
#  undef vuint32  /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
#  undef vsint32  /* PRQA S 0841 */ /* MD_Can_HL_UndefTypes */
# endif
#endif

/*!
 * \exclusivearea CAN_EXCLUSIVE_AREA_1
 * Ensures consistency while modifying the interrupt counter and CAN interrupt lock registers.
 * \protects canCanInterruptCounter, CAN interrupt registers
 * \usedin Can_30_Flexcan4_EnableControllerInterrupts, Can_30_Flexcan4_DisableControllerInterrupts
 * \exclude All functions provided by Can.
 * \length SHORT The interrupt registers and a counter variable are modified.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_2
 * Ensures consistency while modifying the TX mailbox states and lock registers.
 * \protects Can_30_Flexcan4_ActiveSendState, CAN mailbox registers
 * \usedin Can_30_Flexcan4_Write
 * \exclude Can_30_Flexcan4_Write, Can_30_Flexcan4_MainFunction_Write, Can TX Interrupt
 * \length MEDIUM Multiplexed TX and cancellation handling. Cancellation handling by CAN Interface triggered. CAN TX mailbox registers and semaphore for locked mailbox modified.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_3
 * Ensures send order (PIA) while CanIf TX buffer reload.
 * \protects Canif_CancelTxConfirmation processing against Can_30_Flexcan4_Write call inbetween to secure the send order.
 * \usedin Can_30_Flexcan4_MainFunction_Write, Can TX Interrupt
 * \exclude Can_30_Flexcan4_Write, Can_30_Flexcan4_MainFunction_Write, Can TX Interrupt
 * \length MEDIUM CanIf_CancelTxConfirmation. Cancellation handling and follow up CanIf TX buffer reload by CAN Interface triggered.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_4
 * Ensures consistency of RX Queue.
 * \protects RX Queue counter and data
 * \usedin Can_30_Flexcan4_MainFunction_Read, Can RX Interrupt
 * \exclude Can_30_Flexcan4_MainFunction_Read, Can RX Interrupt
 * \length SHORT Modify queue counter and copy data to queue.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_5
 * Ensures atomic wakeup mode transition. Do not use CAN interrupt locks because CAN interrupt status may be changed within.
 * \protects wakeup transition
 * \usedin Can_30_Flexcan4_MainFunction_Wakeup
 * \exclude Can_30_Flexcan4_SetControllerMode, Can BusOff Interrupt, Can Wakeup, Can_30_Flexcan4_MainFunction_Wakeup.
 * \length MEDIUM wakeup mode transition will be performed.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_6
 * Ensures atomic mode transition.
 * \protects mode transition
 * \usedin Can_30_Flexcan4_MainFunction_Mode, Can_30_Flexcan4_SetControllerMode, Can_30_Flexcan4_MainFunction_BusOff, Can BusOff Interrupt
 * \exclude Can_30_Flexcan4_MainFunction_Mode, Can_30_Flexcan4_SetControllerMode, Can_30_Flexcan4_MainFunction_BusOff, Can BusOff Interrupt, Can_30_Flexcan4_MainFunction_Wakeup.
 * \length MEDIUM mode transition will be performed.
 * \endexclusivearea
 *
 * \exclusivearea CAN_EXCLUSIVE_AREA_7
 * Ensures consistency of TX HW FIFO.
 * \protects TX FIFO counter and data
 * \usedin Can_30_Flexcan4_MainFunction_Write, Can TX Interrupt
 * \exclude Can_30_Flexcan4_MainFunction_Write, Can TX Interrupt
 * \length SHORT Modify queue counter and copy data to queue.
 * \endexclusivearea
 */

#endif /* end file */
/*  STOPSINGLE_OF_MULTIPLE_COMMENT  */

/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */


/* Kernbauer Version: 1.14 Konfiguration: DrvCan_Arm32Flexcan3Asr Erzeugungsgangnummer: 576 */



/************   Organi, Version 3.9.2 Vector-Informatik GmbH  ************/

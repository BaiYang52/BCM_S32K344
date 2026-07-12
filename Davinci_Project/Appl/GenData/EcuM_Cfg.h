/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: EcuM
 *           Program: VAG AR4 (MSR_Vag_SLP5)
 *          Customer: Faurecia Clarion Electronics US
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: S32K314NHT1VPBST
 *    License Scope : The usage is restricted to CBD2300384_D00
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: EcuM_Cfg.h
 *   Generation Time: 2026-07-12 12:56:23
 *           Project: BCM_S32K344 - Version 1.0
 *          Delivery: CBD2300384_D00
 *      Tool Version: DaVinci Configurator Classic (beta) 5.26.40 SP3
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! This version of DaVinci Configurator Classic and/or the related Basic Software Package is BETA software.               !
 ! BETA Software is basically operable, but not sufficiently tested, verified and/or qualified for use in series      !
 ! production and/or in vehicles operating on public or non-public roads.                                             !
 ! In particular, without limitation, BETA Software may cause unpredictable ECU behavior, may not provide all         !
 ! functions necessary for use in series production and/or may not comply with quality requirements which are         !
 ! necessary according to the state of the art. BETA Software must not be used in series production.                  !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/


#if !defined(ECUM_CFG_H)
#define ECUM_CFG_H

/**********************************************************************************************************************
 *  PRE-COMPILE Switches
 *********************************************************************************************************************/
/* pre-compile switches for EcuM configuration and services */
#ifndef ECUM_DEV_ERROR_DETECT
#define ECUM_DEV_ERROR_DETECT STD_ON
#endif
#ifndef ECUM_DEV_ERROR_REPORT
#define ECUM_DEV_ERROR_REPORT STD_ON
#endif

# define ECUM_SCHM_START_INITIALIZATION                               (STD_ON)
# define ECUM_DEFERRED_BSWM_NOTIFICATION                              (STD_OFF)
# define ECUM_NONCHANNEL_WAKEUP_RUN                                   (STD_OFF)
# define ECUM_SLAVE_CORE_HANDLING                                     (STD_OFF)
# define ECUM_BSW_ERROR_HOOK                                          (STD_ON)


/* pre-compile switches for module API */
# define ECUM_VERSION_INFO_API                                        (STD_ON)

/* pre-compile switches for the EcuMFlex Behavior */ 
# define ECUM_ENABLE_DEF_BEHAVIOR                                     (STD_OFF)
# define ECUM_RESET_LOOP_DETECTION                                    (STD_OFF)
# define ECUM_ALARM_CLOCK_PRESENT                                     (STD_OFF)
# define ECUM_MODE_HANDLING                                           (STD_OFF)



/* pre-compile switches for the EcuMFixed Behavior */
# define ECUM_FIXED_BEHAVIOR                                          (STD_OFF)
# define ECUM_SUPPORT_RTE                                             (STD_OFF)
# define ECUM_SUPPORT_RTE_MODE_SWITCH                                 (STD_OFF)
# define ECUM_SUPPORT_RTE_MODE_SWITCH_ACK                             (STD_OFF)
# define ECUM_INCLUDE_NVRAM_MGR                                       (STD_OFF)
# define ECUM_SUPPORT_DEM                                             (STD_OFF)
# define ECUM_SUPPORT_COMM                                            (STD_OFF)

/* current configuration variant ECUM_VARIANT_PRECOMPILE  || ECUM_VARIANT_POSTBUILD */

/* General Defines */
#ifndef ECUM_USE_DUMMY_STATEMENT
#define ECUM_USE_DUMMY_STATEMENT STD_ON /* /MICROSAR/vSet/vSetGeneral/vSetDummyStatementKind */
#endif
#ifndef ECUM_DUMMY_STATEMENT
#define ECUM_DUMMY_STATEMENT(v) (v)=(v) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */  /* /MICROSAR/vSet/vSetGeneral/vSetDummyStatementKind */
#endif
#ifndef ECUM_DUMMY_STATEMENT_CONST
#define ECUM_DUMMY_STATEMENT_CONST(v) (void)(v) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */  /* /MICROSAR/vSet/vSetGeneral/vSetDummyStatementKind */
#endif
#ifndef ECUM_ATOMIC_BIT_ACCESS_IN_BITFIELD
#define ECUM_ATOMIC_BIT_ACCESS_IN_BITFIELD STD_OFF /* /MICROSAR/EcuC/EcucGeneral/AtomicBitAccessInBitfield */
#endif
#ifndef ECUM_ATOMIC_VARIABLE_ACCESS
#define ECUM_ATOMIC_VARIABLE_ACCESS 32u /* /MICROSAR/EcuC/EcucGeneral/AtomicVariableAccess */
#endif
#ifndef ECUM_PROCESSOR_S32K314
#define ECUM_PROCESSOR_S32K314
#endif
#ifndef ECUM_COMP_IAR
#define ECUM_COMP_IAR
#endif
#ifndef ECUM_GEN_GENERATOR_MSR
#define ECUM_GEN_GENERATOR_MSR
#endif
#ifndef ECUM_CPUTYPE_BITORDER_LSB2MSB
#define ECUM_CPUTYPE_BITORDER_LSB2MSB /* /MICROSAR/vSet/vSetPlatform/vSetBitOrder */
#endif
#ifndef ECUM_CONFIGURATION_VARIANT_PRECOMPILE
#define ECUM_CONFIGURATION_VARIANT_PRECOMPILE 1
#endif
#ifndef ECUM_CONFIGURATION_VARIANT_LINKTIME
#define ECUM_CONFIGURATION_VARIANT_LINKTIME 2
#endif
#ifndef ECUM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE
#define ECUM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE 3
#endif
#ifndef ECUM_CONFIGURATION_VARIANT
#define ECUM_CONFIGURATION_VARIANT ECUM_CONFIGURATION_VARIANT_PRECOMPILE
#endif
#ifndef ECUM_POSTBUILD_VARIANT_SUPPORT
#define ECUM_POSTBUILD_VARIANT_SUPPORT STD_OFF
#endif


/**********************************************************************************************************************
 *  PRE-COMPILE Parameters
 *********************************************************************************************************************/
/* Variables that are used to determine the arraysizes or the number of some modes */
#define ECUM_RESETMODECOUNT                                           (4u)
#define ECUM_SHUTDOWNCAUSECOUNT                                       (4u)
#define ECUM_NUMBER_OF_CORES                                          (1u)
#define ECUM_NUMBER_OF_OS_APPLICATIONS                                (1u)

#define ECUM_CORE_ID_STARTUP                                          OS_CORE_ID_MASTER
#define ECUM_CORE_ID_BSW                                              OS_CORE_ID_MASTER
#define ECUM_OS_RESOURCE                                              OsResource /*  The selected OsResource  */ 


#define ECUM_BSWM_CONFIG_POINTER                                      BswM_Config_Ptr
#define ECUM_SCHM_CONFIG_POINTER                                      


# if (STD_ON == ECUM_DEV_ERROR_DETECT)
#  define EcuM_Det_ReportError(EcuM_FunctionId, EcuM_ErrorCode)       ((void)Det_ReportError((ECUM_MODULE_ID), (ECUM_INSTANCE_ID), (EcuM_FunctionId), (EcuM_ErrorCode))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define EcuM_Det_ReportError(EcuM_FunctionId, EcuM_ErrorCode)  
# endif

/*---- configurable DEM Errors ---------------------------------------------*/



/**********************************************************************************************************************
 *  POSTBUILD Parameters
 *********************************************************************************************************************/
 
 /**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/* Postbuild */
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** 
  \defgroup  EcuMPBDataSwitches  EcuM Data Switches  (POST_BUILD)
  \brief  These defines are used to deactivate data and their processing.
  \{
*/ 
#define ECUM_PBCONFIG                                                                               STD_OFF  /**< Deactivateable: 'EcuM_PBConfig' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
#define ECUM_LTCONFIGIDXOFPBCONFIG                                                                  STD_OFF  /**< Deactivateable: 'EcuM_PBConfig.LTConfigIdx' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
#define ECUM_PCCONFIGIDXOFPBCONFIG                                                                  STD_OFF  /**< Deactivateable: 'EcuM_PBConfig.PCConfigIdx' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
/** 
  \}
*/ 


/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** 
  \defgroup  EcuMPCDataSwitches  EcuM Data Switches  (PRE_COMPILE)
  \brief  These defines are used to deactivate data and their processing.
  \{
*/ 
#define ECUM_ALARMCLOCKTIMEOUT                                                                      STD_OFF  /**< Deactivateable: 'EcuM_AlarmClockTimeOut' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_COMMCHANNELREFS                                                                        STD_OFF  /**< Deactivateable: 'EcuM_ComMChannelRefs' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_CHANNELOFCOMMCHANNELREFS                                                               STD_OFF  /**< Deactivateable: 'EcuM_ComMChannelRefs.Channel' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_COMMPNCREFS                                                                            STD_OFF  /**< Deactivateable: 'EcuM_ComMPNCRefs' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_PNCOFCOMMPNCREFS                                                                       STD_OFF  /**< Deactivateable: 'EcuM_ComMPNCRefs.PNC' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_DEFAULTAPPMODE                                                                         STD_ON
#define ECUM_DEFAULTSHUTDOWNMODE                                                                    STD_ON
#define ECUM_DEFAULTSHUTDOWNTARGET                                                                  STD_ON
#define ECUM_DRIVERINITONE                                                                          STD_ON
#define ECUM_FUNCTIONOFDRIVERINITONE                                                                STD_ON
#define ECUM_DRIVERRESTARTLIST                                                                      STD_OFF  /**< Deactivateable: 'EcuM_DriverRestartList' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_FUNCTIONOFDRIVERRESTARTLIST                                                            STD_OFF  /**< Deactivateable: 'EcuM_DriverRestartList.Function' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_ECUM_CRCHASH_LOWER                                                                     STD_ON
#define ECUM_ECUM_CRCHASH_UPPER                                                                     STD_ON
#define ECUM_FINALMAGICNUMBER                                                                       STD_OFF  /**< Deactivateable: 'EcuM_FinalMagicNumber' Reason: 'the module configuration does not support flashing of data.' */
#define ECUM_FLEXUSER                                                                               STD_OFF  /**< Deactivateable: 'EcuM_FlexUser' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_ALARMCLOCKIDOFFLEXUSER                                                                 STD_OFF  /**< Deactivateable: 'EcuM_FlexUser.AlarmClockId' Reason: 'Alarm Clock Feature is not active.' */
#define ECUM_GODOWNALLOWEDOFFLEXUSER                                                                STD_OFF  /**< Deactivateable: 'EcuM_FlexUser.GoDownAllowed' Reason: 'Defensive Behavior is not active!' */
#define ECUM_MODULEIDOFFLEXUSER                                                                     STD_OFF  /**< Deactivateable: 'EcuM_FlexUser.ModuleId' Reason: 'Defensive Behavior is not active.' */
#define ECUM_SETCLOCKALLOWEDOFFLEXUSER                                                              STD_OFF  /**< Deactivateable: 'EcuM_FlexUser.SetClockAllowed' Reason: 'Alarm Clock Feature is not active.' */
#define ECUM_GENERATORCOMPATIBILITYVERSION                                                          STD_ON
#define ECUM_INITDATAHASHCODE                                                                       STD_OFF  /**< Deactivateable: 'EcuM_InitDataHashCode' Reason: 'the module configuration does not support flashing of data.' */
#define ECUM_MAXWAKEUPSOURCECOUNT                                                                   STD_ON
#define ECUM_MODULESTATE                                                                            STD_ON
#define ECUM_NORMALMCUMODE                                                                          STD_ON
#define ECUM_PARTITIONDATA                                                                          STD_OFF  /**< Deactivateable: 'EcuM_PartitionData' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_APPLICATIONOFPARTITIONDATA                                                             STD_OFF  /**< Deactivateable: 'EcuM_PartitionData.Application' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_BSWMPARTITIONOFPARTITIONDATA                                                           STD_OFF  /**< Deactivateable: 'EcuM_PartitionData.BswMPartition' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_ECUMPARTITIONOFPARTITIONDATA                                                           STD_OFF  /**< Deactivateable: 'EcuM_PartitionData.EcuMPartition' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_RESOURCEOFPARTITIONDATA                                                                STD_OFF  /**< Deactivateable: 'EcuM_PartitionData.Resource' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_SIZEOFDRIVERINITONE                                                                    STD_ON
#define ECUM_SLEEPMODELIST                                                                          STD_OFF  /**< Deactivateable: 'EcuM_SleepModeList' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_MCUMODEOFSLEEPMODELIST                                                                 STD_OFF  /**< Deactivateable: 'EcuM_SleepModeList.McuMode' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_POLLINGOFSLEEPMODELIST                                                                 STD_OFF  /**< Deactivateable: 'EcuM_SleepModeList.Polling' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_VALIDOFSLEEPMODELIST                                                                   STD_OFF  /**< Deactivateable: 'EcuM_SleepModeList.Valid' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_WAKEUPSOURCEOFSLEEPMODELIST                                                            STD_OFF  /**< Deactivateable: 'EcuM_SleepModeList.WakeupSource' Reason: 'the array is deactivated because the size is 0 and the piece of data is in the configuration class: PRE_COMPILE' */
#define ECUM_VALIDATIONTIMEOUTTABLE                                                                 STD_OFF  /**< Deactivateable: 'EcuM_ValidationTimeoutTable' Reason: 'No validation timeouts configured.' */
#define ECUM_WAKEUPSOURCELIST                                                                       STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define ECUM_CHECKWAKEUPTIMEOFWAKEUPSOURCELIST                                                      STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.CheckWakeupTime' Reason: 'the value of EcuM_CheckWakeupTimeOfWakeupSourceList is always '0' due to this, the array is deactivated.' */
#define ECUM_COMMCHANNELREFSENDIDXOFWAKEUPSOURCELIST                                                STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMChannelRefsEndIdx' Reason: 'the optional indirection is deactivated because ComMChannelRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_COMMCHANNELREFSSTARTIDXOFWAKEUPSOURCELIST                                              STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMChannelRefsStartIdx' Reason: 'the optional indirection is deactivated because ComMChannelRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_COMMCHANNELREFSUSEDOFWAKEUPSOURCELIST                                                  STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMChannelRefsUsed' Reason: 'the optional indirection is deactivated because ComMChannelRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_COMMPNCREFSENDIDXOFWAKEUPSOURCELIST                                                    STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMPNCRefsEndIdx' Reason: 'the optional indirection is deactivated because ComMPNCRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_COMMPNCREFSSTARTIDXOFWAKEUPSOURCELIST                                                  STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMPNCRefsStartIdx' Reason: 'the optional indirection is deactivated because ComMPNCRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_COMMPNCREFSUSEDOFWAKEUPSOURCELIST                                                      STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ComMPNCRefsUsed' Reason: 'the optional indirection is deactivated because ComMPNCRefsUsedOfWakeupSourceList is always 'FALSE' and the target of the indirection is of the Configuration Class 'PRE_COMPILE'.' */
#define ECUM_POLLINGOFWAKEUPSOURCELIST                                                              STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.Polling' Reason: 'the value of EcuM_PollingOfWakeupSourceList is always 'false' due to this, the array is deactivated.' */
#define ECUM_REASONOFWAKEUPSOURCELIST                                                               STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.Reason' Reason: 'No Mcu Reset Reason configured.' */
#define ECUM_VALIDOFWAKEUPSOURCELIST                                                                STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.Valid' Reason: 'the value of EcuM_ValidOfWakeupSourceList is always 'true' due to this, the array is deactivated.' */
#define ECUM_VALIDATIONTIMEOFWAKEUPSOURCELIST                                                       STD_OFF  /**< Deactivateable: 'EcuM_WakeupSourceList.ValidationTime' Reason: 'the value of EcuM_ValidationTimeOfWakeupSourceList is always '0' due to this, the array is deactivated.' */
#define ECUM_PCCONFIG                                                                               STD_ON
#define ECUM_DEFAULTAPPMODEOFPCCONFIG                                                               STD_ON
#define ECUM_DEFAULTSHUTDOWNMODEOFPCCONFIG                                                          STD_ON
#define ECUM_DEFAULTSHUTDOWNTARGETOFPCCONFIG                                                        STD_ON
#define ECUM_DRIVERINITONEOFPCCONFIG                                                                STD_ON
#define ECUM_ECUM_CRCHASH_LOWEROFPCCONFIG                                                           STD_ON
#define ECUM_ECUM_CRCHASH_UPPEROFPCCONFIG                                                           STD_ON
#define ECUM_FINALMAGICNUMBEROFPCCONFIG                                                             STD_OFF  /**< Deactivateable: 'EcuM_PCConfig.FinalMagicNumber' Reason: 'the module configuration does not support flashing of data.' */
#define ECUM_GENERATORCOMPATIBILITYVERSIONOFPCCONFIG                                                STD_ON
#define ECUM_INITDATAHASHCODEOFPCCONFIG                                                             STD_OFF  /**< Deactivateable: 'EcuM_PCConfig.InitDataHashCode' Reason: 'the module configuration does not support flashing of data.' */
#define ECUM_MAXWAKEUPSOURCECOUNTOFPCCONFIG                                                         STD_ON
#define ECUM_MODULESTATEOFPCCONFIG                                                                  STD_ON
#define ECUM_NORMALMCUMODEOFPCCONFIG                                                                STD_ON
#define ECUM_SIZEOFDRIVERINITONEOFPCCONFIG                                                          STD_ON
/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCIsReducedToDefineDefines  EcuM Is Reduced To Define Defines (PRE_COMPILE)
  \brief  If all values in a CONST array or an element in a CONST array of structs are equal, the define is STD_ON else STD_OFF.
  \{
*/ 
#define ECUM_ISDEF_FUNCTIONOFDRIVERINITONE                                                          STD_OFF
#define ECUM_ISDEF_DRIVERINITONEOFPCCONFIG                                                          STD_ON
#define ECUM_ISDEF_MODULESTATEOFPCCONFIG                                                            STD_ON
/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCEqualsAlwaysToDefines  EcuM Equals Always To Defines (PRE_COMPILE)
  \brief  If all values in a CONST array or an element in a CONST array of structs are equal, the define contains the always equals value.
  \{
*/ 
#define ECUM_EQ2_FUNCTIONOFDRIVERINITONE                                                            
#define ECUM_EQ2_DRIVERINITONEOFPCCONFIG                                                            EcuM_DriverInitOne
#define ECUM_EQ2_MODULESTATEOFPCCONFIG                                                              (&(EcuM_ModuleState))
/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCSymbolicInitializationPointers  EcuM Symbolic Initialization Pointers (PRE_COMPILE)
  \brief  Symbolic initialization pointers to be used in the call of a preinit or init function.
  \{
*/ 
#define EcuM_Config_Ptr                                                                             NULL_PTR  /**< symbolic identifier which shall be used to initialize 'EcuM' */
/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCInitializationSymbols  EcuM Initialization Symbols (PRE_COMPILE)
  \brief  Symbolic initialization pointers which may be used in the call of a preinit or init function. Please note, that the defined value can be a 'NULL_PTR' and the address operator is not usable.
  \{
*/ 
#define EcuM_Config                                                                                 NULL_PTR  /**< symbolic identifier which could be used to initialize 'EcuM */
/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCGeneral  EcuM General (PRE_COMPILE)
  \brief  General constant defines not associated with a group of defines.
  \{
*/ 
#define ECUM_CHECK_INIT_POINTER                                                                     STD_OFF  /**< STD_ON if the init pointer shall not be used as NULL_PTR and a check shall validate this. */
#define ECUM_FINAL_MAGIC_NUMBER                                                                     0x0A1Eu  /**< the precompile constant to validate the size of the initialization structure at initialization time of EcuM */
#define ECUM_INDIVIDUAL_POSTBUILD                                                                   STD_OFF  /**< the precompile constant to check, that the module is individual postbuildable. The module 'EcuM' is not configured to be postbuild capable. */
#define ECUM_INIT_DATA                                                                              ECUM_CONST  /**< CompilerMemClassDefine for the initialization data. */
#define ECUM_INIT_DATA_HASH_CODE                                                                    1797910197  /**< the precompile constant to validate the initialization structure at initialization time of EcuM with a hashcode. The seed value is '0x0A1Eu' */
#define ECUM_USE_ECUM_BSW_ERROR_HOOK                                                                STD_OFF  /**< STD_ON if the EcuM_BswErrorHook shall be called in the ConfigPtr check. */
#define ECUM_USE_INIT_POINTER                                                                       STD_OFF  /**< STD_ON if the init pointer EcuM shall be used. */
/** 
  \}
*/ 



/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
typedef P2FUNC ( void, ECUM_CODE, EcuM_DriverFuncType)( void );

/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: SIZEOF DATA TYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL SIMPLE DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL COMPLEX DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: SIZEOF DATA TYPES
**********************************************************************************************************************/
/** 
  \defgroup  EcuMPCSizeOfTypes  EcuM SizeOf Types (PRE_COMPILE)
  \brief  These type definitions are used for the SizeOf information.
  \{
*/ 
/**   \brief  value based type definition for EcuM_SizeOfDriverInitOne */
typedef uint8 EcuM_SizeOfDriverInitOneType;

/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL SIMPLE DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/** 
  \defgroup  EcuMPCIterableTypes  EcuM Iterable Types (PRE_COMPILE)
  \brief  These type definitions are used to iterate over an array with least processor cycles for variable access as possible.
  \{
*/ 
/**   \brief  type used to iterate EcuM_DriverInitOne */
typedef uint8_least EcuM_DriverInitOneIterType;

/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCValueTypes  EcuM Value Types (PRE_COMPILE)
  \brief  These type definitions are used for value based data representations.
  \{
*/ 
/**   \brief  value based type definition for EcuM_DefaultAppMode */
typedef uint32 EcuM_DefaultAppModeType;

/**   \brief  value based type definition for EcuM_DefaultShutdownMode */
typedef uint8 EcuM_DefaultShutdownModeType;

/**   \brief  value based type definition for EcuM_EcuM_CRCHash_Lower */
typedef uint32 EcuM_EcuM_CRCHash_LowerType;

/**   \brief  value based type definition for EcuM_EcuM_CRCHash_Upper */
typedef uint32 EcuM_EcuM_CRCHash_UpperType;

/**   \brief  value based type definition for EcuM_GeneratorCompatibilityVersion */
typedef uint16 EcuM_GeneratorCompatibilityVersionType;

/**   \brief  value based type definition for EcuM_MaxWakeupSourceCount */
typedef uint8 EcuM_MaxWakeupSourceCountType;

/**   \brief  value based type definition for EcuM_NormalMcuMode */
typedef uint32 EcuM_NormalMcuModeType;

/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL COMPLEX DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/** 
  \defgroup  EcuMPCStructTypes  EcuM Struct Types (PRE_COMPILE)
  \brief  These type definitions are used for structured data representations.
  \{
*/ 
/**   \brief  type used in EcuM_DriverInitOne */
typedef struct sEcuM_DriverInitOneType
{
  EcuM_DriverFuncType FunctionOfDriverInitOne;
} EcuM_DriverInitOneType;

/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCRootPointerTypes  EcuM Root Pointer Types (PRE_COMPILE)
  \brief  These type definitions are used to point from the config root to symbol instances.
  \{
*/ 
/**   \brief  type used to point to EcuM_DriverInitOne */
typedef P2CONST(EcuM_DriverInitOneType, TYPEDEF, ECUM_CONST) EcuM_DriverInitOnePtrType;

/**   \brief  type used to point to EcuM_ModuleState */
typedef P2VAR(EcuM_StateType, TYPEDEF, ECUM_VAR_NO_INIT) EcuM_ModuleStatePtrType;

/**   \brief  type used to point to EcuM_DefaultAppModeOfPCConfig */
typedef P2CONST(EcuM_DefaultAppModeType, TYPEDEF, ECUM_CONST) EcuM_DefaultAppModeOfPCConfigPtrType;

/**   \brief  type used to point to EcuM_DefaultShutdownTargetOfPCConfig */
typedef P2CONST(EcuM_StateType, TYPEDEF, ECUM_CONST) EcuM_DefaultShutdownTargetOfPCConfigPtrType;

/**   \brief  type used to point to EcuM_NormalMcuModeOfPCConfig */
typedef P2CONST(EcuM_NormalMcuModeType, TYPEDEF, ECUM_CONST) EcuM_NormalMcuModeOfPCConfigPtrType;

/** 
  \}
*/ 

/** 
  \defgroup  EcuMPCRootValueTypes  EcuM Root Value Types (PRE_COMPILE)
  \brief  These type definitions are used for value representations in root arrays.
  \{
*/ 
/**   \brief  type used in EcuM_PCConfig */
typedef struct sEcuM_PCConfigType
{
  uint8 EcuM_PCConfigNeverUsed;  /**< dummy entry for the structure in the configuration variant precompile which is not used by the code. */
} EcuM_PCConfigType;

typedef EcuM_PCConfigType EcuM_PbConfigType;  /**< A structure type is present for data in each configuration class. This typedef redefines the probably different name to the specified one. */

/** 
  \}
*/ 




/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/





#endif /* ECUM_CFG_H */
/**********************************************************************************************************************
 *  END OF FILE: ECUM_CFG.H
 *********************************************************************************************************************/



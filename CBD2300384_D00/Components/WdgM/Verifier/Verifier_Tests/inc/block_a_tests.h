/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  black_a_tests.h
 *        \brief  black_a_tests header file
 *
 *      \details  This is the header file of test block 'a'
 *
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the Verifier's header file.
 *********************************************************************************************************************/


#ifndef VERIFIER_BLOCK_A_TESTS_H
#define VERIFIER_BLOCK_A_TESTS_H

#include <stdbool.h>
#include "WdgM_Types.h"
#include "wdgm_verifier_types.h"

/**
 * test_001
 *
 *  The number of checkpoints according to the EDF and the number of
 *  checkpoints referenced by supervised entities must match.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_001
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_002
 *
 *  The number of checkpoints according to the EDF and the number of
 *  checkpoints stored in the NrOfAllCheckpoints member of the main
 *  structure must match.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_002
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_003
 *
 *  The number of local transitions according to the EDF shall match the
 *  number of local transitions referenced by checkpoints according to the
 *  corresponding NrOfLocalTransitions members.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_003
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_004
 *
 *  The number of global transitions according to the EDF shall match the
 *  number of global transitions referenced by checkpoints according to the
 *  corresponding NrOfGlobalTransitions members.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_004
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_005
 *
 *  The number of Supervised Entities according to the EDF shall match the
 *  value of the main structure's NrOfSupervisedEntities member.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_005
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_017
 *
 *  A checkpoint's NrOfStartedGlobalTransitions value shall match the number
 *  of global transitions having that checkpoint as starting point according
 *  to the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_017
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_019
 *
 *  If an alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMExpectedAliveIndications member shall match
 *  the alive supervision's number of expected alive indications
 *  as specified in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_019
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_020
 *
 *  If an alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMMinMargin member shall match the correspoinding
 *  attribute (..../WdgMMinMargin) in the alive supervision as
 *  specified in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_020
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_021
 *
 *  If an alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMMaxMargin member shall match the correspoinding
 *  attribute (..../WdgMMaxMargin) in the alive supervision as
 *  specified in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_021
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_022
 *
 *  If an alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMSupervisionReferenceCycle member shall match the
 *  correspoinding attribute (..../WdgMSupervisionReferenceCycle)
 *  in the alive supervision as specified in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_022
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_027 A
 *
 *  checkpoint's NrOfLocalTransitions value shall be set to the number
 *  of local transitions having that checkpoint as destination point
 *  according to the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_027
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_028 A
 *
 *  checkpoint's NrOfGlobalTransitions value shall be set to the number
 *  of global transitions having that checkpoint as destination point
 *  according to the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_028
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_032
 *
 *  If no alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMExpectedAliveIndications shall be zero.
 *  (See Test 19)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_032
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_033
 *
 *  If no alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMMinMargin member shall be zero. (See Test 20)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_033
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_034
 *
 *  If no alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMMaxMargin member shall be zero. (See Test 21)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_034
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_035
 *
 *  If no alive supervision is defined for a checkpoint then that
 *  checkpoint's WdgMSupervisionReferenceCycle member shall be zero.
 *  (See Test 22)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_035
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_037
 *
 *  WdgM_TransitionType->WdgMDeadlineMin shall match the corresponding
 *  value in the ECU description file.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_037
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_038
 *
 *  WdgM_TransitionType->WdgMDeadlineMax shall match the corresponding
 *  value in the ECU description file.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_038
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_039
 *
 *  WdgM_GlobalTransitionType->WdgMDeadlineMin shall match the
 *  corresponding value in the ECU description file.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_039
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_040
 *
 *  WdgM_GlobalTransitionType->WdgMDeadlineMax shall match the
 *  corresponding value in the ECU description file.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_040
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_041
 *
 *  Every supervised entity's WdgMInitialStatus value shall match
 *  the value entered as WdgMSupervisedEntityInitialMode for the
 *  WdgMLocalStatusParams element assigned to the supervised entity.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_041
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_042
 *
 *  For every entity:
 *  X shall match Y where X is the WdgMFailedSupervisionRefCycleTol
 *  member of a that supervised entity in the generated configuration and
 *  Y is the element WdgMFailedSupervisionRefCycleTol in the
 *  WdgMLocalStatusParams defined for the same entity in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_042
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_043
 *
 *  For every entity:
 *  X shall match Y where X is the WdgMFailedDeadlineRefCycleTol
 *  member of a that supervised entity in the generated configuration and
 *  Y is the element WdgMFailedDeadlineRefCycleTol in the
 *  WdgMLocalStatusParams defined for the same entity in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_043
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_044
 *
 *  For every entity:
 *  X shall match Y where X is the WdgMDeadlineReferenceCycle
 *  member of a that supervised entity in the generated configuration and
 *  Y is the element WdgMDeadlineReferenceCycle in the
 *  WdgMLocalStatusParams defined for the same entity in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_044
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_045
 *
 *  For every entity:
 *  X shall match Y where X is the WdgMFailedProgramFlowRefCycleTol
 *  member of a that supervised entity in the generated configuration and
 *  Y is the element WdgMFailedProgramFlowRefCycleTol in the
 *  WdgMLocalStatusParams defined for the same entity in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_045
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_046
 *
 *  For every entity:
 *  X shall match Y where X is the WdgMProgramFlowReferenceCycle
 *  member of a that supervised entity in the generated configuration and
 *  Y is the element WdgMProgramFlowReferenceCycle in the
 *  WdgMLocalStatusParams defined for the same entity in the EDF.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_046
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_047
 *
 *  If an OS application is defined for a supervised entity then that entity's
 *  OSApplication member shall match the corresponding element
 *  (...WdgMSupervisedEntity/OSApplication) in the EDF; otherwise the member
 *  shall be set to the INVALID_OSAPPLICATION constant.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_047
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_085
 *
 *  The set of relations between alive supervisions and checkpoints in the
 *  EDF is the same as in GCF; i.e. each CP has on both sides either the
 *  same alive supervision associated or no alive supervision associated.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_085
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_086
 *
 *  In GCF, for each SE: All CPs that are referenced in SE are defined
 *  (in array WdgMCheckPoint). (Note: This includes check for references
 *  by CP-ID and references by address to CP-list item - related to Error 1093)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_086
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_089
 *
 * Check if the WDGM_VERSION_INFO_API in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_089
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_090
 *
 * Check if the WDGM_DEV_ERROR_DETECT in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_090
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_091
 *
 * Check if the WDGM_DEM_REPORT in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_091
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_092
 *
 * Check if the WDGM_DEFENSIVE_BEHAVIOR in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_092
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_093
 *
 * Check if the WDGM_IMMEDIATE_RESET in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_093
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_094
 *
 * Check if the WDGM_OFF_MODE_ENABLED in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_094
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_095
 *
 * Check if the WDGM_USE_OS_SUSPEND_INTERRUPT in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_095
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_096
 *
 * Check if the WDGM_TIMEBASE_SOURCE in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_096
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_097
 *
 * Check if the WDGM_SECOND_RESET_PATH in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_097
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_098
 *
 * Check if the WDGM_TICK_OVERRUN_CORRECTION in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_098
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_099
 *
 * Check if the WDGM_ENTITY_DEACTIVATION_ENABLED in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_099
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_100
 *
 * Check if the WDGM_STATUS_REPORTING_MECHANISM in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_100
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_101
 *
 * Check if the WDGM_USE_RTE in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_101
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_102
 *
 * Check if the WDGM_DEM_SUPERVISION_REPORT in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_102
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_103
 *
 * Check if the WDGM_FIRSTCYCLE_ALIVECOUNTER_RESET in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_103
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_104
 *
 * Check if the WDGM_GLOBAL_TRANSITIONS in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_104
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_105
 *
 * Check if the WDGM_AUTOSAR_3_1_X_COMPATIBILITY in wdgm_cfg_features has the same value
 * as in wdgm_verifier_info.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_105
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

#endif /* VERIFIER_BLOCK_A_TESTS_H */

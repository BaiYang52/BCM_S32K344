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
/*!        \file  black_b_tests.h
 *        \brief  black_b_tests header file
 *
 *      \details  This is the header file of test block 'b'
 *
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the Verifier's header file.
 *********************************************************************************************************************/

#ifndef VERIFIER_BLOCK_B_TESTS_H
#define VERIFIER_BLOCK_B_TESTS_H

#include <stdbool.h>
#include "WdgM_Types.h"
#include "wdgm_verifier_types.h"

/**
 * test_018
 *
 *  A checkpoint's WdgMIsEndCheckpointGlobal shall be TRUE if
 *  the checkpoint is not the source of any global transition.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_018
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_023
 *
 *  A checkpoint's WdgMAliveLRef shall only be NULL_PTR if and only if
 *  there is no alive supervision defined for that checkpoint.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_023
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_024
 *
 *  A checkpoint's WdgMAliveGRef shall only be NULL_PTR if and only if
 *  there is no alive supervision defined for that checkpoint.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_024
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_025
 *
 *  A checkpoint's WdgMDeadlineMonitoring shall be set to TRUE if that
 *  checkpoint is source or destination of at least one transition with
 *  associated deadline monitoring; otherwise it will be set to FALSE.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_025
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_026
 *
 *  A checkpoint's WdgMOutgoingDeadlineMax shall be set to the maximum
 *  deadline associated to any of the transitions having that checkpoint
 *  as starting point (Challenge: seconds-to-ticks conversion?)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_026
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_029
 *
 *  A checkpoint's WdgMLocalTransitionRef member shall be set to NULL_PTR
 *  if and only if there are no local transitions having that checkpoint
 *  as destination point.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_029
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_030
 *
 *  A checkpoint's WdgMGlobalTransitionsRef member shall be set to NULL_PTR
 *  if and only if there are no global transitions having that checkpoint
 *  as destination point.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_030
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_031
 *
 *  A checkpoint's WdgMStartsAGlobalTransition shall be set to TRUE if that
 *  checkpoint is the starting point of a global transition; otherwise it
 *  shall be set to FALSE.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_031
    ( const WdgM_ConfigType        * main_struct
    );


/**
 * test_048
 *
 *  For every supervised entity the following condition shall be fulfilled:
 *  either WdgMFailedProgramFlowRefCycleTol is greater than zero or
 *  WdgMProgramFlowRefCycle is zero (see Error1053)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_048
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );


/**
 * test_049
 *
 *  For every supervised entity the following condition shall be fulfilled:
 *  either WdgMFailedDeadlineRefCycleTol is zero or WdgMDeadlineReferenceCycle
 *  is greater than zero (see Error1054).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_049
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_050
 *
 *  For every supervised entity the following condition shall be fulfilled:
 *  either WdgMFailedDeadlineRefCycleTol is greater than zero or
 *  WdgMDeadlineReferenceCycle is zero (see Error1055)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_050
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_051
 *
 *  For systems with internal timebase source: The shortest WdgMDeadlineMax
 *  value among all WdgMDeadlineSupervision elements shall be greater or equal
 *  to the WdgMSupervisionCycle (see Error1059).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_051
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_052
 *
 *  The following condition shall be fulfilled:
 *  (1 / WdgMTicksPerSecond) <= WdgMSupervisionCycle (see Error1060)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_052
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_053
 *
 *  The following condition shall be fulfilled:
 *  (0 <= WdgMTriggerWindowStart <= WdgMSupervisionCycle <=
 *  WdgMTriggerConditionValue)
 *  (see Error1061)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_053
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_054
 *
 *  The following condition shall be fulfilled:
 *  0 < ticks_per_second <= rti_hz / 2 (see Error1074)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_054
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_055
 *
 *  The targeted precision shall fulfill the following condition:
 *  int (round (ticks_per_second *  window_start * 0.001)) <= 65535
 *  (Note: 65535 is the maximum 16-bit integer - see Error1075)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_055
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_056
 *
 *  The targeted precision shall fulfill the following condition:
 *  int (round (ticks_per_second * condition_value * 0.001)) <= 65535
 *  (Note: 65535 is the maximum 16-bit integer - see Error1075)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_056
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_057
 *
 *  Every WdgMWatchdog element shall have a WdgMTrigger associated to it
 *  (see Error1076)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_057
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_058
 *
 *  In every supervised entity, there shall be a maximum of one checkpoint
 *  having an alive counter (see Error1086).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_058
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_059
 *
 *  It shall be ensured that transitions reference existing checkpoints
 *  (see Error1091)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_059
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_060
 *
 *  It shall be ensured that global transitions reference only existing
 *  supervised entities as source (see Error1094).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_060
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_061
 *
 *  It shall be ensured that global transitions reference only existing
 *  supervised entities as destination (see Error1095).
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_061
    ( const verifier_main_struct_t * verifier_info
    );

/**
 * test_062
 *
 *  The minimum deadline of every WdgMDeadlineSupervision element shall
 *  be less or equal to the maximum deadline (see Error1096).
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_062
    ( const verifier_main_struct_t * verifier_info
    );

/**
 * test_063
 *
 *  No deadline value shall be greater than (1 / tps) * MAX_16_BIT_VALUE
 *  (see Error1097)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_063
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_064
 *
 *  For configurations with internal tick counter source the following
 *  condition must be fulfilled:
 *  (1 / WdgMTicksPerSecond[Hz]) = WdgMSupervisionCycle[s] (see Error1098)
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_064
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_065
 *
 *  The trigger modes belonging to each trigger shall build a zero-based
 *  list of increasing integers without a gap (see Error1109)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_065
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_066
 *
 *  Every transition shall have no more than one WdgMDeadlineSupervision
 *  element assigned to it (see Error1114).
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_066
    ( const verifier_main_struct_t * verifier_info
    );

/**
 * test_067
 *
 *  A supervised entity's WdgMProgramFlowMonitoring boolean value shall
 *  be true if and only if there are local or global transitions starting
 *  or ending in any of the entity's checkpoints.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_067
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_087
 *
 *  All defined Watchdog devices in the EDF must have the same number of
 *  WdgMTrigger elements
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true in case the test passed
 *                  false in case the test failed
 *
 **/

bool test_087
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    );
/**
 * test_088
 *
 *  For every supervised entity the following condition shall be fulfilled:
 *  (WdgMFailedProgramFlowRefCycleTol = 0) OR (WdgMProgramFlowRefCycle > 0)
 *  (Note: related to Error1052)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_088
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_107
 *
 *  The WdgMTriggerTimeout field in every element in the WdgMTriggerMode
 *  array (of type WdgM_TriggerModeType) shall have a value greater than
 *  zero (Error1124).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_107
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_108
 *
 *  The source Supervised Entity and the destination Supervised Entity
 *  of each global/external transition shall be part of OS applications
 *  running on the same CPU core.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_108 (const verifier_main_struct_t * verifier_info);

/**
 * test_109
 *
 *  Each Supervised Entity's `WdgMEntityCoreId` field in the
    generated configuration shall match the CPU core ID which
    executes the OS Application related to the Supervised Entity
    according to the ECU Description File.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_109
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_110
 *
 *  The source checkpoint and the destination checkpoint
 *  of each local/internal transition shall be part of the same
 *  Supervised Entity
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_110 (const verifier_main_struct_t * verifier_info);

/**
 * test_112
 *
 *  For every element in the array `deadline_supervisions`: There shall
 *  be either a local/internal or a global/external transition having the
 *  same source and destination checkpoints as the deadline.
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_112 (const verifier_main_struct_t * verifier_info);

#endif /* VERIFIER_BLOCK_B_TESTS_H */

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
/*!        \file  black_c_tests.h
 *        \brief  black_c_tests header file
 *
 *      \details  This is the header file of test block 'c'
 *
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the Verifier's header file.
 *********************************************************************************************************************/

#ifndef VERIFIER_BLOCK_C_TESTS_H
#define VERIFIER_BLOCK_C_TESTS_H

#include <stdbool.h>
#include "WdgM_Types.h"
#include "wdgm_verifier_types.h"


/**
 * test_006
 *
 *  The main structure's WdgMSupervisedEntityRef shall be a NULL pointer
 *  if and only if the number of Supervised Entities according to the EDF
 *  is zero.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_006
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_007
 *
 *  The EntityStatusLRef member of every SE must not be a NULL pointer.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_007
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_008
 *
 *  The EntityStatusGRef member of every SE must not be a NULL pointer.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_008
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_009
 *
 *  The WdgMAliveLRef member of each checkpoint shall be NULL_PTR
 *  if and only if the member WdgMAliveGRef in the same SE is NULL_PTR.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_009
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_010
 *
 *  The main WdgM_ConfigType structure shall have its DataGSRef member set
 *  to a non-NULL pointer.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_010
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_011
 *
 *  The main WdgM_ConfigType structure shall have its DataGRef member set
 *  to a non-NULL pointer.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_011
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_012
 *
 *  The main WdgM_ConfigType structure shall have its EntityGSRef member
 *  set to a non-NULL pointer.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_012
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_013
 *

 *  The main WdgM_ConfigType structure shall have its
 *  GlobalTransitionFlagsGS member set to NULL if and only if there are
 *  no global transitions.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_013
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_014
 *
 *  The value of WdgM_GlobalTransitionType->GlobalTransitionFlagId must
 *  match the current element's position in the WdgM_GlobalTransitionType
 *  array.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_014
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_015
 *
 *  Each supervised entity's EntityStatusLRef member shall point to a
 *  unique variable.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_015
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_016
 *
 *  Each supervised entity's EntityStatusGRef member shall point to a
 *  unique variable.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_016
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_036
 *
 *  For every supervised entity there shall be among the checkpoints
 *  belonging to that entity at least one having its attribute
 *  WdgMIsEndCheckpoint set to TRUE.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_036
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_068
 *
 *  The checkpoints belonging to each supervised entity shall have IDs
 *  building a zero-based list of increasing integers without a gap
 *  (see Error1001)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_068
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_069
 *
 *  Each supervised entity shall have at least one checkpoint
 *  (see Error1016)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_069
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_070
 *
 *  There shall be either a global transition or a local transition
 *  for every WdgMDeadlineSupervision element (see Error1031)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_070
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_071
 *
 *  Each supervised entity's ID shall be unique (see Error1062).
 *  Note: actually superseded by handling of Error1071 - see below).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_071
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_072
 *
 *  Every supervised entity shall have an initial checkpoint
 *  (see Error1064).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_072
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_073
 *
 *  There shall be at least one callback function defined for the
 *  supervised entities or for the main structure if the flag
 *  WDGM_STATUS_REPORTING_MECHANISM is set to WDGM_USE_NOTIFICATIONS (see Error1066).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_073
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_074
 *
 *  The number of supervised entities shall not be zero (see Error1090).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_074
    ( const WdgM_ConfigType        * main_struct
    );

/**
 * test_075
 *
 *  Each supervised entity's WdgM_LocalStatusChangeNotification member shall point
 *  to the callback function defined for that entity; otherwise it shall
 *  be NULL_PTR (see Error1066).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_075
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_076
 *
 *  The main structure's WdgM_GlobalStatusChangeNotification member shall be
 *  NULL_PTR if no callback function has been configured for signaling
 *  a global state change (see Error1066)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_076
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_077
 *
 *  The callback functions assigned to supervised entities shall have a
 *  unique name (see Error1068)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_077
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_078
 *
 *  Checkpoints defined as local end checkpoints shall have no outgoing
 *  local transitions (see Erro1069).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_078
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_079
 *
 *  Checkpoints defined as local initial checkpoints shall have no incoming
 *  local transitions (see Error1070).
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_079
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_080
 *
 *  The supervised entity IDs shall build a zero-based list of increasing
 *  integers without a gap (see Error1071)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_080
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_081
 *
 *  If a supervised entity's WdgMFailedSupervisionRefCycleTol is set to
 *  greater than zero then there shall be an alive supervision counter
 *  associated to one of the entity's checkpoints (see Error1057).
 *  (Challenge: entity-to-checkpoint-to-alive-supervision navigation)
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_081
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_083
 *
 *  The STD_OFF and STD_ON constants shall be defined as zero (0)
 *  and one (1), resp.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_083
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );

/**
 * test_084
 *
 *  The value for WdgMTicksPerSecond shall be greater than zero.
 *
 *  @param  [IN]    main_struct     pointer to configuration to be checked
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/
bool test_084
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    );


/**
 * test_111
 *
 * The OS applications stored in the ECUC Description File shall have
 * unique names
 *
 *  @param  [IN]    verifier_info   pointer to reference configuration
 *
 *  @return bool    true    in case the test passed
 *                  false   in case the test failed
 **/

bool test_111 (verifier_main_struct_t const * verifier_info);

#endif /* VERIFIER_BLOCK_C_TESTS_H */

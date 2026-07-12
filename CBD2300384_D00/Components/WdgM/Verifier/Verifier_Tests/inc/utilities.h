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
/*!        \file  utilities.h.h
 *        \brief  utilities.h header file
 *
 *      \details  This is the header file of WdgM verifier utilities.
 *
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the Verifier's header file.
 *********************************************************************************************************************/
 
#ifndef WDGM_VERIFIER_UTILITIES_H
#define WDGM_VERIFIER_UTILITIES_H

#include "wdgm_verifier_types.h"

#define MAX_INTERNAL_ERROR 1002
#define BAD_ID             -1

#define CHECK_NULL_PTR(p, i) \
    if (((void *) p) == NULL) \
    { \
        printf ("Test failed due to unexpected NULL pointer (%d)\n", i); \
        return false; \
    }

#define ASSERT_ENTITIES() \
    if (chk_cnt_or_ptr \
            ( main_struct->WdgMSupervisedEntityRef \
            , main_struct->NrOfSupervisedEntities \
            , "entities") == false \
       ) \
    { \
        return false; \
    }

#define ASSERT_CHECKPOINTS(ENTITY) \
    if (chk_cnt_or_ptr \
            ( (ENTITY)->WdgMCheckpointRef \
            , (ENTITY)->NrOfCheckpoints \
            , "checkpoints") == false \
       ) \
    { \
        return false; \
    }

#define ASSERT_G_TRANSITIONS(CHECKPOINT) \
    if (chk_cnt_and_ptr \
            ( (CHECKPOINT)->WdgMGlobalTransitionsRef \
            , (CHECKPOINT)->NrOfGlobalTransitions \
            , "global transitions") == false \
       ) \
    { \
        return false; \
    }

#define ASSERT_L_TRANSITIONS(CHECKPOINT) \
    if (chk_cnt_and_ptr \
            ( (CHECKPOINT)->WdgMLocalTransitionRef \
            , (CHECKPOINT)->NrOfLocalTransitions \
            , "local transitions") == false \
       ) \
    { \
        return false; \
    }

#define CONTINUE_IF_IRRELEVANT_SE(se) \
    if (main_struct->WdgMModeCoreAssignment != se->WdgMEntityCoreId) continue

#define EPS 0.0001
#define CORE_ID_NOT_FOUND -1

/**
 * equals
 *
 *  just a wrapper for the idiom `strcmp (x, y) == 0`
 *
 *  @param  [IN]    s1 pointer to 1st string
 *  @param  [IN]    s2 pointer to 2nd string
 *
 *  @return bool    true    if both strings are equal
 *                  false   otherwise
 **/

bool equals (char const * s1, char const * s2);

int get_core_id_for_os_app
    ( char const * app_name
    , const verifier_main_struct_t * verifier_info
    );

entity_struct_t const * get_vi_entity_for_id
    ( WdgM_SupervisedEntityIdType id
    , const verifier_main_struct_t * verifier_info
    , bool verbose
    );

entity_struct_t const * get_vi_entity_for_name
    ( char const * name
    , const verifier_main_struct_t * verifier_info
    );

WdgM_CheckPointType const * get_first_cp (const WdgM_ConfigType *main_struct);

WdgM_GlobalTransitionType const * getFirstGlobalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    );

WdgM_SupervisedEntityType const * get_first_se
    (const WdgM_ConfigType *main_struct);


WdgM_SupervisedEntityType const * get_wdgm_se
    (const WdgM_ConfigType * main_struct, int se_id);

WdgM_CheckPointType   const * get_wdgm_cp
    ( WdgM_ConfigType const * main_struct
    , int se_id
    , int cp_id
    );

WdgM_TransitionType const * getFirstLocalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    );

boolean const  cp_exists
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    , int se_id
    , int cp_id
    );

int countAllCPInSE        (const WdgM_ConfigType * main_struct, bool verbose);
int total_local_trans_cnt
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    );

bool sanity_test
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info);

void load_config_info (const char *header_file);
void newline ();
void print_all_checkpoint_IDs (WdgM_CheckPointType const * first_cp, int cp_count);
void print_result_header (void);
bool print_comparison (int x, int y);
void print_result (bool result);
bool has_test_data (void const * p, int test_number, char const * item_name);
bool chk_cnt_and_ptr (void const * p, unsigned int const c, char const * n);
bool chk_cnt_or_ptr (void const * p, unsigned int const c, char const * n);
int get_se_id_for_cp_pos (const WdgM_ConfigType * main_struct, int cp_pos);
int count_global_transitions_starting_at
    ( int se_id
    , int cp_id
    , const verifier_main_struct_t * verifier_info
    );
int total_global_trans_cnt
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    );


/**
 * apply_to_edf_cp_list
 *
 * Loop over all checkpoints in the verifier_info (= ECU Description File),
 * find the corresponding checkpoints in the generated configuration and
 * apply a boolean function to them.
 *
 *
 * @param [IN]  main_struct     pointer to main struct in generated config
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  f               a boolean function
 *
 * @return Return true if that function evaluates
 * to true for all checkpoints; otherwise return false.
 *
 **/
 bool apply_to_edf_cp_list
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    , bool (* f) (const verifier_main_struct_t * vi, WdgM_SupervisedEntityType const  * _se, WdgM_CheckPointType const  * _cp)
    );

/**
 * apply_to_edf_se_list
 *
 * Loop over all supervised entitys in the verifier_info (= ECU Description File),
 * find the corresponding supervised entitys in the generated configuration and
 * apply a boolean function to them.
 *
 *
 * @param [IN]  main_struct     pointer to main struct in generated config
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  f               a boolean function
 *
 * @return Return true if that function evaluates
 * to true for all supervised entitys; otherwise return false.
 *
 **/
 bool apply_to_edf_se_list
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    , bool (* f) (const verifier_main_struct_t * vi, WdgM_SupervisedEntityType const  * _se, int counter)
    );

unsigned int number_of_se_in_mode (const WdgM_ConfigType * main_struct);
#endif

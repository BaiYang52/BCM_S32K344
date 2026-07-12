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
/**        \file  black_b_tests.c
 *        \brief  black_b_tests source file
 *
 *      \details  This is the implementation of all test cases of the WdgM verifier of test block 'b'
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>

#include "WdgM.h"
#include "utilities.h"
#include "edf_utilities.h"

#define NR_OF_CHECKPOINTS        xslt_checkpoint_cnt(verifier_info)
#define NR_OF_ENTITIES           xslt_entity_cnt(verifier_info)

bool test_018
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    const WdgM_GlobalTransitionType * g_transition;
    WdgM_SupervisedEntityIdType se_id;
    WdgM_CheckpointIdType cp_id;
    int checkpoints;
    int i, j, x, y, z;
    unsigned int transition_count;

    newline ();
    printf ("Test 18\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMIsEndCheckpointGlobal shall be TRUE if\n"
      "the checkpoint is not the source of any global transition.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1);
        se_id = ms_entity->WdgMSupervisedEntityId;
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("E(%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2);
            cp_id = check_point->WdgMCheckpointId;
            if ( check_point->WdgMIsEndCheckpointGlobal == false)
            {
                printf (" C[%u] id = %u WdgMIsEndCheckpointGlobal == false\n"
                       , j, cp_id
                       );
                continue;
            }
            printf (" C[%u] id = %u WdgMIsEndCheckpointGlobal == true\n"
                   , j, cp_id
                   );

            /* loop over all global transitions */
            transition_count = 0;
            for (x = 0; x < entity_cnt; x++)
            {
                ms_entity = &main_struct->WdgMSupervisedEntityRef[x];
                CHECK_NULL_PTR(ms_entity, 3);
                for (y = 0; y < ms_entity->NrOfCheckpoints; y++)
                {
                    check_point = &ms_entity->WdgMCheckpointRef[y];
                    CHECK_NULL_PTR(check_point,4)
                    ASSERT_CHECKPOINTS(ms_entity)

                    ASSERT_G_TRANSITIONS(check_point)
                    for (z = 0; z < check_point->NrOfGlobalTransitions; z++)
                    {
                        g_transition =
                            &check_point->WdgMGlobalTransitionsRef[z];
                        CHECK_NULL_PTR(g_transition,5)
                        if ( (g_transition->EntitySourceId     == se_id)
                          && (g_transition->CheckpointSourceId == cp_id)
                           )
                        {
                            transition_count++;
                            printf ("  [%u,%u,%u] se(%u),cp(%u) is source!\n"
                                   , se_id, cp_id, x, y, z);
                        }
                    }
                }
            }

            ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
            CHECK_NULL_PTR (ms_entity, 6);
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR (check_point, 7);
            if ( check_point->WdgMIsEndCheckpointGlobal == true)
            {
                is_ok = (transition_count == 0);
            }
            else
            {
                is_ok = true;
            }

            printf ("[%u,%u]: WdgMIsEndCheckpointGlobal=%u, source transitions=%u\n"
                   , i, j
                   , check_point->WdgMIsEndCheckpointGlobal
                   , transition_count
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_023
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    int checkpoints;
    int i, j;

    printf ("Test 23\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMAliveLRef shall only be NULL_PTR if and only if\n"
      "there is no alive supervision defined for that checkpoint.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2);
            if (check_point->WdgMAliveLRef == NULL)
            {
                is_ok = (check_point->WdgMExpectedAliveIndications == 0);
            }
            else
            {
                is_ok = (check_point->WdgMExpectedAliveIndications != 0);
            }
            printf ("[%u,%u]: WdgMAliveLRef=%s, alive supervisions=%u\n"
                   , i, j
                   , (check_point->WdgMAliveLRef == NULL)
                     ? "NULL" : "NOT NULL"
                   , (unsigned int)check_point->WdgMExpectedAliveIndications
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_024
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    int checkpoints;
    int i, j;

    printf ("Test 24\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMAliveGRef shall only be NULL_PTR if and only if\n"
      "there is no alive supervision defined for that checkpoint.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            if (check_point->WdgMAliveGRef == NULL)
            {
                is_ok = (check_point->WdgMExpectedAliveIndications == 0);
            }
            else
            {
                is_ok = (check_point->WdgMExpectedAliveIndications != 0);
            }
            printf ("[%u,%u]: WdgMAliveGRef=%s, alive supervisions=%u\n"
                   , i, j
                   , (check_point->WdgMAliveLRef == NULL)
                     ? "NULL" : "NOT NULL"
                   , (unsigned int)check_point->WdgMExpectedAliveIndications
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_025
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    WdgM_SupervisedEntityIdType se_id;
    WdgM_CheckpointIdType cp_id;
    int checkpoints;
    int i, j, x, y, z;
    unsigned int dl_transition_count;

    printf ("Test 25\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMDeadlineMonitoring shall be set to TRUE if that\n"
      "checkpoint is source or destination of at least one transition with\n"
      "associated deadline monitoring; otherwise it will be set to FALSE.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
        ; i < entity_cnt
        ; i++, ms_entity++
        )
    {
        CHECK_NULL_PTR (ms_entity, 1);
        se_id = ms_entity->WdgMSupervisedEntityId;
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            CHECK_NULL_PTR (check_point, 2);
            cp_id               = check_point->WdgMCheckpointId;
            dl_transition_count = 0;

            /* loop over all incoming transitions */
            ASSERT_L_TRANSITIONS(check_point)
            for (z = 0; z < check_point->NrOfLocalTransitions; z++)
            {
                if (check_point->WdgMLocalTransitionRef[z].
                   WdgMDeadlineMax != 0)
                {
                    dl_transition_count++;
                    printf ("[%u,%u,%u] cp %u is destination (local)\n"
                           , i, j, z, cp_id);
                }
            }
            ASSERT_G_TRANSITIONS(check_point)
            for (z = 0; z < check_point->NrOfGlobalTransitions; z++)
            {
                if (check_point->WdgMGlobalTransitionsRef[z].
                   WdgMDeadlineMax != 0)
                {
                    dl_transition_count++;
                    printf ("[%u,%u,%u] cp %u is destination (global)\n"
                           , i, j, z, cp_id);
                }
            }

            /* loop over all outgoing transitions */
            for (x = 0; x < entity_cnt; x++)
            {
                ms_entity = &main_struct->WdgMSupervisedEntityRef[x];
                for (y = 0; y < ms_entity->NrOfCheckpoints; y++)
                {
                    check_point = &ms_entity->WdgMCheckpointRef[y];
                    ASSERT_CHECKPOINTS(ms_entity)

                    ASSERT_L_TRANSITIONS(check_point)
                    for (z = 0; z < check_point->NrOfLocalTransitions; z++)
                    {
                        if ( (check_point->WdgMLocalTransitionRef[z].
                                EntitySourceId == se_id)
                          && (check_point->WdgMLocalTransitionRef[z].
                                CheckpointSourceId == cp_id)
                           )
                        {
                            if (check_point->WdgMLocalTransitionRef[z].
                               WdgMDeadlineMax != 0)
                            {
                                dl_transition_count++;
                                printf ("[%u,%u,%u] cp %u se %u is source (local)\n"
                                       , x, y, z, cp_id, se_id);
                            }
                        }
                    }
                    ASSERT_G_TRANSITIONS(check_point)
                    for (z = 0; z < check_point->NrOfGlobalTransitions; z++)
                    {
                        if ( (check_point->WdgMGlobalTransitionsRef[z].
                                EntitySourceId == se_id)
                          && (check_point->WdgMGlobalTransitionsRef[z].
                                CheckpointSourceId == cp_id)
                           )
                        {
                            if (check_point->WdgMGlobalTransitionsRef[z].
                               WdgMDeadlineMax != 0)
                            {
                                dl_transition_count++;
                                printf ("[%u,%u,%u] cp %u se %u is source (global)\n"
                                       , x, y, z, cp_id, se_id);
                            }
                        }
                    }
                }
            }

            ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
            check_point = &ms_entity->WdgMCheckpointRef[j];
            if (check_point->WdgMDeadlineMonitoring == false)
            {
                is_ok = (dl_transition_count == 0);
            }
            else
            {
                is_ok = (dl_transition_count != 0);
            }

            printf ("[%u,%u]: WdgMDeadlineMonitoring=%u, deadline transitions=%u\n"
                   , i, j
                   , check_point->WdgMDeadlineMonitoring
                   , dl_transition_count
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_026
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    WdgM_TransitionType       const * local_trans;
    WdgM_GlobalTransitionType const * global_trans;
    WdgM_SupervisedEntityIdType se_id;
    WdgM_CheckpointIdType cp_id;
    int checkpoints;
    int i, j, x, y, z;
    WdgM_TimeBaseTickType deadline_max;

    printf ("Test 26\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMOutgoingDeadlineMax shall be set to the maximum\n"
      "deadline associated to any of the transitions having that checkpoint\n"
      "as starting point (Challenge: seconds-to-ticks conversion?)\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        se_id = ms_entity->WdgMSupervisedEntityId;
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            cp_id = check_point->WdgMCheckpointId;
            deadline_max = 0;

            /* loop over all outgoing transitions */
            for (x = 0; x < entity_cnt; x++)
            {
                ms_entity = &main_struct->WdgMSupervisedEntityRef[x];
                CHECK_NULL_PTR(ms_entity, 3)
                for (y = 0; y < ms_entity->NrOfCheckpoints; y++)
                {
                    check_point = &ms_entity->WdgMCheckpointRef[y];
                    CHECK_NULL_PTR(check_point, 4)
                    ASSERT_CHECKPOINTS(ms_entity)

                    ASSERT_L_TRANSITIONS(check_point)
                    for ( z = 0, local_trans = check_point->WdgMLocalTransitionRef
                        ; z < check_point->NrOfLocalTransitions
                        ; z++, local_trans++
                        )
                    {
                        CHECK_NULL_PTR (local_trans, 5);
                        if ( (local_trans->EntitySourceId == se_id)
                          && (local_trans->CheckpointSourceId == cp_id)
                           )
                        {
                            printf ("[%u,%u,%u] cp %u se %u dl = %u\n"
                                   , x, y, z, cp_id, se_id
                                   , (int) local_trans->WdgMDeadlineMax);
                            if (local_trans->WdgMDeadlineMax > deadline_max)
                            {
                                deadline_max = local_trans->WdgMDeadlineMax;
                            }
                        }
                    }
                    ASSERT_G_TRANSITIONS(check_point)
                    for ( z = 0, global_trans = check_point->WdgMGlobalTransitionsRef
                        ; z < check_point->NrOfGlobalTransitions
                        ; z++, global_trans++
                        )
                    {
                        CHECK_NULL_PTR (global_trans, 6);
                        if (  (global_trans->EntitySourceId     == se_id)
                           && (global_trans->CheckpointSourceId == cp_id)
                           )
                        {
                            printf ("[%u,%u,%u] cp %u se %u dl = %u\n"
                                   , x, y, z, cp_id, se_id
                                   , (int) global_trans->WdgMDeadlineMax);
                            if (global_trans->WdgMDeadlineMax > deadline_max)
                            {
                                deadline_max = global_trans->WdgMDeadlineMax;
                            }
                        }
                    }
                }
            }

            ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
            CHECK_NULL_PTR (ms_entity, 7);
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR (check_point, 8);
            is_ok = (check_point->WdgMOutgoingDeadlineMax == deadline_max);
            printf ("[%u,%u]: WdgMOutgoingDeadlineMax=%u, deadline max=%u\n"
                   , i, j
                   , (int)check_point->WdgMOutgoingDeadlineMax
                   , (int)deadline_max
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_029
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    int checkpoints;
    int i, j;

    printf ("Test 29\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMLocalTransitionRef member shall be set to NULL_PTR\n"
      "if and only if there are no local transitions having that checkpoint\n"
      "as destination point.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            if (check_point->WdgMLocalTransitionRef == NULL)
            {
                is_ok = (check_point->NrOfLocalTransitions == 0);
            }
            else
            {
                is_ok = (check_point->NrOfLocalTransitions != 0);
            }
            printf ("[%u,%u]: "
                    "WdgMLocalTransitionRef=%s alive NrOfLocalTransitions=%u\n"
                   , i, j
                   , (check_point->WdgMLocalTransitionRef == NULL)
                     ? "NULL" : "NOT NULL"
                   , check_point->NrOfLocalTransitions
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_030
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    int checkpoints;
    int i, j;

    printf ("Test 30\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMGlobalTransitionsRef member shall be set to\n"
      "NULL_PTR if and only if there are no global transitions having\n"
      "that checkpoint as destination point.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            if (check_point->WdgMGlobalTransitionsRef == NULL)
            {
                is_ok = (check_point->NrOfGlobalTransitions == 0);
            }
            else
            {
                is_ok = (check_point->NrOfGlobalTransitions != 0);
            }
            printf ("[%u,%u]: WdgMGlobalTransitionsRef=%s alive NrOfGlobalTransitions=%u\n"
                   , i, j
                   , (check_point->WdgMGlobalTransitionsRef == NULL)
                     ? "NULL" : "NOT NULL"
                   , check_point->NrOfGlobalTransitions
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_031
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    WdgM_GlobalTransitionType const * global_trans;
    WdgM_SupervisedEntityIdType se_id;
    WdgM_CheckpointIdType cp_id;
    int checkpoints;
    int i, j, x, y, z;
    unsigned int gbl_transitions_started;

    printf ("Test 31\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A checkpoint's WdgMStartsAGlobalTransition shall be set to TRUE\n"
      "if that checkpoint is the starting point of a global transition;\n"
      "otherwise it shall be set to FALSE.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        se_id = ms_entity->WdgMSupervisedEntityId;
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            cp_id = check_point->WdgMCheckpointId;
            gbl_transitions_started = 0;

            printf ("checking checkpoint id = %u\n", cp_id);
            /* loop over all outgoing transitions */
            for (x = 0; x < entity_cnt; x++)
            {
                ms_entity = &main_struct->WdgMSupervisedEntityRef[x];
                CHECK_NULL_PTR(ms_entity, 3)
                for (y = 0; y < ms_entity->NrOfCheckpoints; y++)
                {
                    check_point = &ms_entity->WdgMCheckpointRef[y];
                    CHECK_NULL_PTR(check_point, 4)
                    ASSERT_CHECKPOINTS(ms_entity)

                    ASSERT_G_TRANSITIONS(check_point)
                    for ( z = 0, global_trans = check_point->WdgMGlobalTransitionsRef
                        ; z < check_point->NrOfGlobalTransitions
                        ; z++, global_trans++
                        )
                    {
                        CHECK_NULL_PTR (global_trans, 5);
                        if (  (global_trans->EntitySourceId     == se_id)
                           && (global_trans->CheckpointSourceId == cp_id)
                           )
                        {
                            gbl_transitions_started++;
                            printf ("[%u,%u,%u] cp %u se %u is source\n"
                                   , x, y, z, cp_id, se_id);
                        }
                    }
                }
            }

            ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
            check_point = &ms_entity->WdgMCheckpointRef[j];

            if (check_point->WdgMStartsAGlobalTransition == false)
            {
                is_ok = (gbl_transitions_started == 0);
            }
            else
            {
                is_ok = (gbl_transitions_started != 0);
            }
            printf ("[%u,%u]: WdgMStartsAGlobalTransition=%u, global transitions started=%u\n"
                   , i, j
                   , (int)check_point->WdgMStartsAGlobalTransition
                   , (int)gbl_transitions_started
                   );
            printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

static bool _condition_048 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    bool ok = true;

    if ( !( (wdgm_se->WdgMFailedProgramFlowRefCycleTol > 0)
         || (wdgm_se->WdgMProgramFlowReferenceCycle == 0)
          )
       )
    {
         ok = false;
    }

    if (!ok)
    {
        printf("Problem at supervised entity: >> %d << !\n", wdgm_se->WdgMSupervisedEntityId );
    }

    return ok;
}

bool test_048
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 48\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Each supervised entity's WdgMFailedProgramFlowRefCycleTol shall be\n"
      "greater than zero or each supervised entity's\n"
      "WdgMProgramFlowRefCycle shall be zero.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_048);

    printf
        ( "%ssupervised entity's WdgMFailedProgramFlowRefCycleTol or\n"
          "WdgMProgramFlowRefCycle is greater than zero or zero: %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

static bool _condition_049 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    bool ok = true;

    if ( !( (wdgm_se->WdgMFailedDeadlineRefCycleTol == 0)
         || (wdgm_se->WdgMDeadlineReferenceCycle > 0)
          )
       )
    {
        ok = false;
    }

    if (!ok)
    {
        printf("Problem at supervised entity: >> %d << !\n", wdgm_se->WdgMSupervisedEntityId );
    }

    return ok;
}

bool test_049
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 49\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "For every supervised entity the following condition shall\n"
      "be fulfilled: either `WdgMFailedDeadlineRefCycleTol` is zero\n"
      "or `WdgMDeadlineReferenceCycle` is greater than zero\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_049);

    printf
        ( "%ssupervised entity's `WdgMFailedDeadlineRefCycleTol`\n"
          "is 0 or  'WdgMDeadlineReferenceCycle' is\n"
          "greater 0 : %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

static bool _condition_050 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    bool ok = true;

    if ( !( (wdgm_se->WdgMFailedDeadlineRefCycleTol > 0)
         || (wdgm_se->WdgMDeadlineReferenceCycle == 0)
          )
       )
    {
        ok = false;
    }

    if (!ok)
    {
        printf("Problem at supervised entity: >> %d << !\n", wdgm_se->WdgMSupervisedEntityId );
    }

    return ok;
}

bool test_050
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 50\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Each supervised entity's WdgMFailedDeadlineRefCycleTol  shall be\n"
      "greater than zero or each supervised entity's\n"
      "WdgMDeadlineReferenceCycle  shall be zero.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_050);

    printf
        ( "%ssupervised entity's WdgMFailedDeadlineRefCycleTol  or\n"
          "WdgMDeadlineReferenceCycle  is greater than zero or zero: %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_051
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    wdgm_mode_t               const * mode = NULL;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    WdgM_TransitionType       const * local_trans;
    WdgM_GlobalTransitionType const * global_trans;
    int se_cnt = (int) main_struct->NrOfSupervisedEntities;
    int cp_cnt;
    int trans_cnt;
    int i, j, k;
    unsigned int dl_max, supervision_cycle;


    mode = get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 51\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "For systems with internal timebase source: The shortest\n"
      "WdgMDeadlineMax value among all WdgMDeadlineSupervision\n"
      "elements shall be greater or equal to the WdgMSupervisionCycle\n"
      "(see Error1059).\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);

    if (verifier_info->WdgMTimebaseSource != WDGM_INTERNAL_SOFTWARE_TICK) {
        printf ("System has no internal software timebase source - this test cannot fail!\n");
        newline ();
        print_result (true);
        newline ();
        return true;
    }

    supervision_cycle = (unsigned int)
        ((double) (1.0 * mode->supervision_cycle * mode->ticks_per_second));

    ASSERT_ENTITIES()

    for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
        ; i < se_cnt
        ; i++, ms_entity++
        )
    {
        CHECK_NULL_PTR (ms_entity, 2);
        cp_cnt = (int) ms_entity->NrOfCheckpoints;
        ASSERT_CHECKPOINTS(ms_entity)

        for ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < cp_cnt
            ; j++, check_point++
            )
        {
            CHECK_NULL_PTR (check_point, 3);
            trans_cnt = (int) check_point->NrOfLocalTransitions;
            ASSERT_L_TRANSITIONS(check_point)
            for ( k = 0, local_trans = check_point->WdgMLocalTransitionRef
                ; k < trans_cnt
                ; k++, local_trans++
                )
            {
                CHECK_NULL_PTR (local_trans, 4);
                dl_max = (unsigned int) local_trans->WdgMDeadlineMax;

                is_ok = dl_max > 0 ? (dl_max >= supervision_cycle) : true;
                if (!is_ok)
                {
                    printf
                        ( "Supervised Entity ID %u, checkpoint ID %u "
                          "has invalid local transitions where \n"
                          "`WdgMDeadlineMax` = %u < %u = supervision cycle: "
                          "NOT OK\n\n"
                        , ms_entity->WdgMSupervisedEntityId
                        , j
                        , dl_max
                        , supervision_cycle
                        );
                }
                result = result && is_ok;
            }
            trans_cnt = (int)check_point->NrOfGlobalTransitions;
            ASSERT_G_TRANSITIONS(check_point)
            for ( k = 0, global_trans = check_point->WdgMGlobalTransitionsRef
                ; k < trans_cnt
                ; k++, global_trans++
                )
            {
                CHECK_NULL_PTR (global_trans, 5);
                dl_max = (unsigned int) global_trans->WdgMDeadlineMax;

                is_ok = dl_max > 0 ? (dl_max >= supervision_cycle) : true;
                if (!is_ok)
                {
                    printf
                        ( "Supervised Entity ID %u, checkpoint ID %u "
                          "has invalid global transitions where \n"
                          "`WdgMDeadlineMax` = %u < %u = supervision cycle: "
                          "NOT OK\n\n"

                        , ms_entity->WdgMSupervisedEntityId
                        , j
                        , dl_max
                        , supervision_cycle
                        );
                }
                result = result && is_ok;
            }
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_052
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 52\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The following condition shall be fulfilled:\n"
      "(1 / WdgMTicksPerSecond) <= WdgMSupervisionCycle\n"
      "(see Error1060)\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);

    printf ("ticks_per_second     = %f\n" , mode->ticks_per_second);
    printf ("WdgMSupervisionCycle = %f\n" , mode->supervision_cycle);

    result = (1 / mode->ticks_per_second <= mode->supervision_cycle);

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_053
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result              = true;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);
    double supervision_cycle = 0.0;

    printf ("Test 53\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The ``WdgMSupervisionCycle`` stored in the EDF shall be "
          "greater than zero (see Error1123)\n"
        );

    CHECK_NULL_PTR (mode, 1);
    newline ();
    supervision_cycle = (double) (mode->supervision_cycle * 1000.0);
    printf
        ( "Supervision cycle = %5.2f [ms]\n\n"
        , supervision_cycle
        );
    result = supervision_cycle > 0.0;
    print_result (result);
    newline ();
    return result;
}


bool test_054
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 54\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The following condition shall be fulfilled:\n"
      "0 < ticks_per_second <= rti_hz / 2 (see Error1074)\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);

    printf ("Since rti_hz can not be read from any of the configurations\n"
            "ticks_per_second <= rti_hz/2 will not be executed.\n\n");

    printf ("ticks_per_second     = %f\n" , mode->ticks_per_second);
    result = (0 < mode->ticks_per_second);

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_055
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);
    int i;
    double X,Y,Z;

    printf ("Test 55\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The targeted precision shall fulfill the following condition:\n"
      "int (round (ticks_per_second *  window_start * 0.001)) <= 65535\n"
      "(Note: 65535 is the maximum 16-bit integer - see Error1075)\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);

    printf ("Found %u triggers.\n",mode->trigger_cnt);
    for (i = 0; i < mode->trigger_cnt; i++)
    {
        wdgm_trigger_t const * trigger = &mode->triggers[i];

        X = (double)trigger->WdgMTriggerWindowStart;
        Y = (double)mode->ticks_per_second;
        Z = X * Y * 0.001;

        printf ( "Trigger[%u]:\n"
                 " X = WdgMTriggerWindowStart    = %f\n"
                 " Y = ticks_per_second          = %f\n"
                 " Z = X * Y * 0.001             = %f\n"
               , i,X,Y,Z);

        newline ();

        is_ok = ( (unsigned int)(Z) <= 65535);
        printf ("result : %s\n", is_ok ? "OK" : "NOT OK");

        result = result && is_ok;
        newline ();
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_056
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);
    int i;
    double X,Y,Z;

    printf ("Test 56\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The targeted precision shall fulfill the following condition:\n"
      "int (round (ticks_per_second * condition_value * 0.001)) <= 65535\n"
      "(Note: 65535 is the maximum 16-bit integer - see Error1075)\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);

    printf ("Found %u triggers.\n",mode->trigger_cnt);
    for (i = 0; i < mode->trigger_cnt; i++)
    {
        wdgm_trigger_t const * trigger = &mode->triggers[i];

        X = (double)trigger->WdgMTriggerConditionValue;
        Y = (double)mode->ticks_per_second;
        Z = X * Y * 0.001;

        printf ( "Trigger[%u]:\n"
                 " X = WdgMTriggerConditionValue = %f\n"
                 " Y = ticks_per_second          = %f\n"
                 " Z = X * Y * 0.001             = %f\n"
               , i,X,Y,Z);

        newline ();

        is_ok = ( (unsigned int)(Z) <= 65535);
        printf ("result : %s\n", is_ok ? "OK" : "NOT OK");

        result = result && is_ok;
        newline ();
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_057
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i;
    bool result    = true;
    bool subresult = true;
    bool tmode_is_null = true;
    WdgM_WatchdogDeviceType   const  * WdgMWatchdogList = main_struct->WdgMWatchdogDeviceRef;
    WdgM_WatchdogDeviceType   const  * wd               = NULL;
    WdgM_TriggerModeType      const  * tmode            = NULL;

    if (WdgMWatchdogList == NULL)
    {
        printf ("Test 57\n");
        printf ("======\n");
        printf ("Result\n");
        printf ("------\n");
        printf ("No watchdog element found: NOT OK\n");
        newline ();
        return false;
    }

    for
        ( i = 0, wd = WdgMWatchdogList
        ; i < main_struct->NrOfWatchdogDevices
        ; i++, wd++
        )
    {
        CHECK_NULL_PTR (wd, 1);
        tmode = wd->WdgMTriggerModeRef;
        tmode_is_null = tmode == NULL_PTR;
        newline ();
        printf ("Test 57.%d\n", i + 1);
        printf ("=========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "Every WdgMWatchdog element shall have a WdgMTrigger associated to it\n\n"
            );
        printf
            ( "WdgMTrigger at array offset %d = %sNULL\n\n"
            , i
            , tmode_is_null ? "" : "NOT "
            );
        printf ("Result\n");
        printf ("------\n");
        subresult = !tmode_is_null;
        result    = result && subresult;
        printf
            ( "WdgMWatchdog element has %s WdgMTrigger associated\n"
              "to it: %sOK.\n"
            , subresult ? "a" : "no"
            , subresult ? ""   : "Not "
        );
    }
    newline ();
    return result;
}

bool test_058
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    WdgM_CheckpointIdType cp_id;
    int checkpoints;
    int i, j;
    unsigned int cp_alive_counter;

    printf ("Test 58\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "In every supervised entity, there shall be a maximum of one\n"
      "checkpoint having an alive counter (see Error1086).\n"
      "\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        cp_alive_counter = 0;
        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            cp_id = check_point->WdgMCheckpointId;

            if (check_point->WdgMExpectedAliveIndications != 0)
            {
                cp_alive_counter++;
                printf ("[%u,%u] cp %u has alive counter\n"
                       , i, j, cp_id);
            }
        }

        is_ok = (cp_alive_counter <= 1);
        printf ("[%u]: checkpoints with alive counter=%u\n"
               , i, cp_alive_counter);
        printf ("result = %s\n", is_ok ? "OK" : "NOT OK");
        result = result && is_ok;
        newline ();
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_059
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i,j,k;
    bool ok     = true;
    bool result = true;
    char const * cp_name;

    WdgM_SupervisedEntityType const  * first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * se       = NULL;
    WdgM_CheckPointType       const  * cp       = NULL;
    WdgM_TransitionType       const  * tr       = NULL;

    printf ("Test 59\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "It shall be ensured that transitions reference existing checkpoints\n\n"
    );
    printf ("Result\n");
    printf ("------\n");
    if (first_se == NULL)
    {
        printf ("No supervised entities found: NOT OK\n");
        newline ();
        return false;
    }

    printf ("Entities: %u\n",xslt_entity_cnt (verifier_info));
    for ( i = 0, se = first_se
        ; i < xslt_entity_cnt (verifier_info)
        ; i++, se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (se);

        CHECK_NULL_PTR(se, 1)

        printf
            ( "\nSupervised Entity `%s` has %d checkpoints...\n"
            , get_vi_se_name (verifier_info, se->WdgMSupervisedEntityId)
            , se->NrOfCheckpoints
            );

        if (se->NrOfCheckpoints != 0 && se->WdgMCheckpointRef == NULL)
        {
            printf ("No checkpoint found: NOT OK\n");
            newline ();
            return false;
        }

        for ( j = 0, cp = se->WdgMCheckpointRef
            ; j < se->NrOfCheckpoints
            ; j++, cp++
            )
        {
            CHECK_NULL_PTR(cp, 2)
            ASSERT_L_TRANSITIONS(cp)
            cp_name = get_vi_cp_name
                ( verifier_info
                , se->WdgMSupervisedEntityId
                , cp->WdgMCheckpointId
                );
            printf
                ( "Checkpoint `%s` has %d local transitions\n"
                , cp_name
                , cp->NrOfLocalTransitions
                );

            for ( k = 0, tr = cp->WdgMLocalTransitionRef
                ; k < cp->NrOfLocalTransitions
                ; k++, tr++
                )
            {
                CHECK_NULL_PTR(tr, 3)
                ok = cp_exists
                    ( main_struct
                    , verifier_info
                    , tr->EntitySourceId
                    , tr->CheckpointSourceId
                    );
                printf
                    ( "%d/%d -> %d/%d: %sOK\n"
                    , tr->EntitySourceId
                    , tr->CheckpointSourceId
                    , se->WdgMSupervisedEntityId
                    , cp->WdgMCheckpointId
                    , ok ? "" : "NOT "
                    );
                result = result && ok;
            }
        }
    }

    printf
        ( "%stransition references existing checkpoints\n\n"
        , result ? "Every " : "Not every "
        );
    return result;
}

bool test_060
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j;
    bool ok = true;
    bool flag = false;

    WdgM_SupervisedEntityType const  * first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * se       = NULL;
    WdgM_GlobalTransitionType const  * first_gt = getFirstGlobalTransition (main_struct, verifier_info);
    WdgM_GlobalTransitionType const  * gt       = NULL;

    printf ("Test 60\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "It shall be ensured that global transitions reference\n"
      "only existing supervised entities as source\n\n"
    );
    printf ("Result\n");
    printf ("------\n");
    if (first_se == NULL)
    {
        printf ("No supervised entities found: NOT OK\n");
        newline ();
        return false;
    }

    if (first_gt == NULL)
    {
        printf ("No global transitions found: this test does not apply\n");
        newline ();
        return true;
    }

    for ( i = 0, gt = first_gt
        ; i < xslt_global_trans_cnt(verifier_info)
        ; i++, gt++
        )
    {
        CHECK_NULL_PTR(gt, 1)
        flag = false;
        for ( j = 0, se = first_se
            ; j < xslt_entity_cnt (verifier_info)
            ; j++, se++
            )
        {
            CHECK_NULL_PTR(se, 2)
            if (se->WdgMSupervisedEntityId == gt->EntitySourceId)
            {
                flag = true;
            }
        }
        if (flag == false)
        {
            printf
                ("Problem with global transition in GCF at array offset << %d >> \n\n"
                , i
                );
            ok = false;
        }
    }
    printf
        ( "%sglobal transition references existing \n"
          "supervised entities as source. %sOK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_061
    ( const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int transitions = (int)verifier_info->global_trans_cnt;
    int entities  = (int)verifier_info->entity_cnt;
    int i, j;

    printf ("Test 61\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "It shall be ensured that global transitions reference only\n"
      "existing supervised entities as destination (see Error1095).\n"
      "\n"
    );

    printf ("Since the destination for the configuration is defined by:\n"
            "WdgM_ConfigType->->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMGlobalTransitionsRef[transition]\n"
            "the check is done for verifier_main_struct_t only.\n"
           );

    printf ("transitions = %u\n",transitions);
    printf ("entities    = %u\n",entities);
    for (i = 0; i < transitions; i++)
    {
        const global_transition_t * global_transition =
            &verifier_info->global_transitions[i];

        is_ok = false;
        for (j = 0; j < entities; j++)
        {
            entity_struct_t const * entity =
                &verifier_info->entity_list[j];

            if (equals(global_transition->dest_se,entity->name))
            {
                is_ok = true;
                printf ("[%u,%u]: ref %s as destination\n"
                       , i, j, entity->name
                       );
            }
        }

        printf ("[%u]     result %s\n",i,is_ok ? "OK" : "NOT OK");
        result = result && is_ok;
        newline ();
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}


bool test_062
    ( const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int supervisions  = (int)verifier_info->dl_supervision_cnt;
    int i;

    printf ("Test 62\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The minimum deadline of every WdgMDeadlineSupervision \n"
      "element shall be less or equal to the maximum deadline \n\n"
    );

    printf ("Since Test 39 and 40 check that the configuration deadlines\n"
            "match the corresponding value in the ECU description file\n"
            "the check is done for verifier_main_struct_t only.\n\n"
           );

    printf ("deadline supervisions = %u\n",supervisions);
    for (i = 0; i < supervisions; i++)
    {
        dl_supervision_t const * supervision =
            &verifier_info->dl_supervision_list[i];

        is_ok = (supervision->dl_min <= supervision->dl_max);
        printf ("[%u] dl_min = %f dl_max = %f : result %s\n"
               , i, supervision->dl_min, supervision->dl_max
               , is_ok ? "OK" : "NOT OK"
               );
        result = result && is_ok;
        newline ();
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}


bool test_063
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    WdgM_TransitionType       const * local_trans;
    WdgM_GlobalTransitionType const * global_trans;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);
    int se_cnt = (int)main_struct->NrOfSupervisedEntities;
    int cp_cnt;
    int trans_cnt;
    int i, j, k;
    double dl, limit;
    int tps = 0;

    printf ("Test 63\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "No deadline value shall be greater than (1 / tps) * MAX_16_BIT_VALUE\n"
      "(see Error1097)\n"
      "\n"
    );
    CHECK_NULL_PTR (mode, 1);
    tps = mode->ticks_per_second;

    limit = (double)((1.0 / tps)) * 0xFFFF;

    printf ("NrOfSupervisedEntities = %u\n",se_cnt);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMLocalTransitionRef(WdgMGlobalTransitionsRef)[transition].\n"
            "WdgMDeadlineMax [ticks]\n");
    printf ("Note: it is already checked that WdgMDeadlineMin <= WdgMDeadlineMax.\n");
    printf ("limit = 1 / verifier_info->mode_info->ticks_per_second) * 0xFFFF"
            " = %f\n", limit
           );
    newline ();

    ASSERT_ENTITIES()

    for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
        ; i < se_cnt
        ; i++, ms_entity++
        )
    {
        CHECK_NULL_PTR(ms_entity, 2)
        cp_cnt = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, cp_cnt);

        ASSERT_CHECKPOINTS(ms_entity)

        for ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < cp_cnt
            ; j++, check_point++
            )
        {
            CHECK_NULL_PTR(check_point, 3)
            trans_cnt = (int)check_point->NrOfLocalTransitions;
            printf ("checkpoint[%u].NrOfLocalTransitions = %u\n"
                   , j, trans_cnt);
            ASSERT_L_TRANSITIONS(check_point)
            for ( k = 0, local_trans = check_point->WdgMLocalTransitionRef
                ; k < trans_cnt
                ; k++, local_trans++
                )
            {
                CHECK_NULL_PTR(local_trans, 4)
                dl = (double) (1.0 * local_trans->WdgMDeadlineMax / tps);

                is_ok = (dl <= limit);
                printf ("[%u,%u]: dl = %f, limit = %f, result = %s\n"
                       , ms_entity->WdgMSupervisedEntityId, j, dl, limit
                       , is_ok ? "OK" : "NOT OK"
                       );
                result = result && is_ok;
            }

            trans_cnt = (int)check_point->NrOfGlobalTransitions;
            printf ("checkpoint[%u].NrOfGlobalTransitions = %u\n"
                   , j, trans_cnt);
            ASSERT_G_TRANSITIONS(check_point)
            for ( k = 0, global_trans = check_point->WdgMGlobalTransitionsRef
                ; k < trans_cnt
                ; k++, global_trans++
                )
            {
                CHECK_NULL_PTR(global_trans, 5)
                dl = (double) (1.0 * global_trans->WdgMDeadlineMax / tps);

                is_ok = (dl <= limit);
                printf ("[%u,%u]: dl = %f, limit = %f, result = %s\n"
                       , ms_entity->WdgMSupervisedEntityId, j, dl, limit
                       , is_ok ? "OK" : "NOT OK"
                       );
                result = result && is_ok;
            }
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_064
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool   result;
    int    tps;
    double left, right, diff;
    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 64\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "For configurations with internal tick counter source\n"
      "the following condition must be fulfilled:\n"
      "(1 / WdgMTicksPerSecond[Hz]) = WdgMSupervisionCycle[s]\n"
      "(see Error1098)\n"
      "\n"
    );

    CHECK_NULL_PTR (mode, 1);
    if (verifier_info->WdgMTimebaseSource != WDGM_INTERNAL_SOFTWARE_TICK) {
        printf ("System has no internal timebase source!\n");
        newline ();
        print_result (true);
        newline ();
        return true;
    }
    tps = mode->ticks_per_second;
    if (tps == 0)
    {
        printf ("System's ticks per second is zero - invalid configuration\n");
        return false;
    }

    printf ("X = 1 / ticks_per_second = %f\n", (double) (1.0 / tps));
    printf ("Y = WdgMSupervisionCycle = %f\n", (double) mode->supervision_cycle);

    left   = (double) (1.0 / tps);
    right  = (double) (mode->supervision_cycle);
    diff   = left - right;
    result = (-EPS < diff) && (diff < EPS);

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_065
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i,j;
    bool result    = true;
    bool subresult = true;
    bool wdgm_triggerm_is_null;
    WdgM_WatchdogDeviceType   const  * WdgMWatchdogList = main_struct->WdgMWatchdogDeviceRef;
    WdgM_WatchdogDeviceType   const  * wdgm_wd          = NULL;
    WdgM_TriggerModeType      const  * wdgm_tr          = NULL;

    if (WdgMWatchdogList == NULL)
    {
        printf ("Test 65\n");
        printf ("======\n");
        printf ("Result\n");
        printf ("------\n");
        printf ("No watchdog element found: NOT OK\n");
        newline ();
        return false;
    }

    for
        ( i = 0, wdgm_wd = WdgMWatchdogList
        ; i < main_struct->NrOfWatchdogDevices
        ; i++, wdgm_wd++
        )
    {
       CHECK_NULL_PTR(wdgm_wd, 1)
       for
           ( j = 0, wdgm_tr = wdgm_wd->WdgMTriggerModeRef
           ; j < main_struct->NrOfTriggerModes
           ; j++, wdgm_tr++
           )
       {
           CHECK_NULL_PTR(wdgm_tr, 2)
           wdgm_triggerm_is_null = wdgm_wd->WdgMTriggerModeRef == NULL_PTR;
           newline ();
           printf ("Test 65.%d.%d\n", i + 1, j + 1);
           printf ("=========\n");
           printf ("Description\n");
           printf ("-----------\n");
           printf
               ( "The trigger modes belonging to each trigger shall build\n"
                 " a zero-based list of increasing integers without a gap\n\n"
               );
           printf
               ( "Test for trigger at array offset %d\n"
                 "and trigger mode at array offset %d: %sOK\n\n"
               , i
               , j
               , !wdgm_triggerm_is_null ? "" : "NOT "
               );
               printf ("Result\n");
               printf ("------\n");
               subresult = !wdgm_triggerm_is_null;
               result    = result && subresult;

               printf
                   ( "The trigger modes belonging to each trigger %s build\n"
                     "a zero based list of increasing integers without a gap: %sOK.\n"
                     , result ? ""  : "do not"
                     , result ? ""  : "Not "
                   );
       }
    }
    newline ();
    return result;
}

bool test_066
    ( const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int global_trans_cnt                 = verifier_info->global_trans_cnt;
    int local_trans_cnt                  = verifier_info->local_trans_cnt;
    global_transition_t const * gtr      = NULL;
    local_transition_t const  * ltr      = NULL;
    dl_supervision_t const * supervision = NULL;
    int dl_supervisions_cnt;
    int i, j;

    printf ("Test 66\n");
    printf ("=======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Every transition shall have no more than one\n"
      "WdgMDeadlineSupervision element assigned to it\n"
      "(see Error1114).\n"
      "\n"
    );

    printf ("Found %d global/external transitions\n", global_trans_cnt);
    for ( i = 0, gtr = verifier_info->global_transitions
        ; i < global_trans_cnt
        ; i++, gtr++
        )
    {

        dl_supervisions_cnt = 0;
        for ( j = 0, supervision = verifier_info->dl_supervision_list
            ; j < verifier_info->dl_supervision_cnt
            ; j++, supervision++
            )
        {
            if (  equals (gtr->source_se, supervision->source_se)
               && equals (gtr->source_cp, supervision->source_cp)
               && equals (gtr->dest_se,   supervision->dest_se)
               && equals (gtr->dest_cp,   supervision->dest_cp)
               )
            {
                dl_supervisions_cnt++;
            }
        }
        is_ok = (dl_supervisions_cnt <= 1);
        printf ("Global Transition %s/%s -> %s/%s has %d deadline(s): %s\n"
               , gtr->source_se
               , gtr->source_cp
               , gtr->dest_se
               , gtr->dest_cp
               , dl_supervisions_cnt
               , is_ok ? "OK" : "NOT OK"
               );
        result = result && is_ok;
    }
    newline ();

    printf ("Found %d local/internal transitions\n",local_trans_cnt);
    for ( i = 0, ltr = verifier_info->local_transitions
        ; i < local_trans_cnt
        ; i++, ltr++
        )
    {

        dl_supervisions_cnt = 0;
        for ( j = 0, supervision = verifier_info->dl_supervision_list
            ; j < verifier_info->dl_supervision_cnt
            ; j++, supervision++
            )
        {
            if (  equals (ltr->source_se, supervision->source_se)
               && equals (ltr->source_cp, supervision->source_cp)
               && equals (ltr->dest_se  , supervision->dest_se)
               && equals (ltr->dest_cp  , supervision->dest_cp)
               )
            {
                dl_supervisions_cnt++;
            }
        }
        is_ok = (dl_supervisions_cnt <= 1);

        printf ("Local Transition %s/%s -> %s/%s has %d deadline(s): %s\n"
               , ltr->source_se
               , ltr->source_cp
               , ltr->dest_se
               , ltr->dest_cp
               , dl_supervisions_cnt
               , is_ok ? "OK" : "NOT OK"
               );

        result = result && is_ok;
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_067
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool result = true, is_ok;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    const WdgM_TransitionType * l_transition;
    const WdgM_GlobalTransitionType * g_transition;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    int checkpoints;
    int transitions;
    int i, j, k, l, m;
    WdgM_CheckpointIdType       cp_id;
    WdgM_SupervisedEntityIdType se_id;

    printf ("Test 67\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "A supervised entity's 'WdgMProgramFlowMonitoring' boolean value\n"
      "shall be true if and only if there are local or global transitions\n"
      "starting or ending in any of the entity's checkpoints.\n\n"
    );

    printf ("NrOfSupervisedEntities = %u\n", entity_cnt);
    newline ();

    ASSERT_ENTITIES()

    for (i = 0; i < entity_cnt; i++)
    {
        int transition_cnt = 0;

        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        CHECK_NULL_PTR(ms_entity, 1)
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity)

        for (j = 0; j < checkpoints; j++)
        {
            check_point = &ms_entity->WdgMCheckpointRef[j];
            CHECK_NULL_PTR(check_point, 2)
            transition_cnt += (int)check_point->NrOfLocalTransitions;
            printf ("checkpoint[%u].NrOfLocalTransitions = %u\n"
                   , j, transition_cnt);

            transition_cnt += (int)check_point->NrOfGlobalTransitions;
            printf ("checkpoint[%u].NrOfGlobalTransitions = %u\n"
                   , j, transition_cnt);

            se_id = ms_entity->WdgMSupervisedEntityId;
            cp_id = check_point->WdgMCheckpointId;

            for (k = 0; k < entity_cnt; k++)
            {
                ms_entity = &main_struct->WdgMSupervisedEntityRef[k];
                CHECK_NULL_PTR(ms_entity, 3)
                for (l = 0; l < ms_entity->NrOfCheckpoints; l++)
                {
                    check_point = &ms_entity->WdgMCheckpointRef[l];
                    CHECK_NULL_PTR(check_point, 4)
                    transitions = (int)check_point->NrOfLocalTransitions;
                    ASSERT_CHECKPOINTS(ms_entity)

                    ASSERT_L_TRANSITIONS(check_point)
                    for (m = 0; m < transitions; m++)
                    {
                        l_transition =
                            &check_point->WdgMLocalTransitionRef[m];
                        CHECK_NULL_PTR(l_transition, 5)
                        if ( (l_transition->EntitySourceId == se_id)
                          && (l_transition->CheckpointSourceId == cp_id)
                           )
                        {
                            transition_cnt++;
                            printf ("for [%u,%u,%u] is SE(%u) "
                                    "local transition source\n"
                                   , k, l, m, se_id
                                   );
                        }
                    }
                    transitions = (int)check_point->NrOfGlobalTransitions;
                    ASSERT_G_TRANSITIONS(check_point)
                    for (m = 0; m < transitions; m++)
                    {
                        g_transition =
                            &check_point->WdgMGlobalTransitionsRef[m];
                        CHECK_NULL_PTR(g_transition, 6)
                        if ( (g_transition->EntitySourceId == se_id)
                          && (g_transition->CheckpointSourceId == cp_id)
                           )
                        {
                            transition_cnt++;
                            printf ("for [%u,%u,%u] is SE(%u) "
                                    "global transition source\n"
                                   , k, l, m, se_id
                                   );
                        }
                    }
                }
            }
            /* restore entity pointer */
            ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        }

        if (ms_entity->WdgMProgramFlowMonitoring == false)
        {
            is_ok = (transition_cnt == 0);
        }
        else
        {
            is_ok = (transition_cnt != 0);
        }

        printf
            ( "[%u]: WdgMProgramFlowMonitoring = %u, transitions = %u, "
              "result = %s\n"
            , ms_entity->WdgMSupervisedEntityId
            , ms_entity->WdgMProgramFlowMonitoring
            , transition_cnt
            , is_ok ? "OK" : "NOT OK"
            );
        result = result && is_ok;
        newline ();
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_087
    ( WdgM_ConfigType const        * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result;
    int core_id = main_struct->WdgMModeCoreAssignment;
    int watchdogs, trigger_modes;
    wdgm_mode_t const * mode = get_vi_wdgm_mode (verifier_info, core_id);

    printf ("Test 87\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "All defined Watchdog devices in the EDF must have the same "
      "number of WdgMTrigger elements.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");
    if (mode == NULL)
    {
        printf
            ( "Error: WdgMMode data not found in `wdgm_verifier_info.c`"
              "for core ID %d\n"
            , core_id
            );
        result = false;
    }
    else
    {
        watchdogs     = main_struct->NrOfWatchdogDevices;
        trigger_modes = main_struct->NrOfTriggerModes;
        result        = mode->trigger_cnt == watchdogs * trigger_modes;
        if (result)
        {
            printf
                ( "%d WdgMTrigger element(s), %d "
                  "for each one of %d WdgMWatchdog(s): OK\n"
                , watchdogs * trigger_modes
                , trigger_modes
                , watchdogs
                );
        }
        else
        {
            printf
                ( "WdgMWatchdogs have different number of WdgMTriggers: "
                  "NOT OK\n"
                );
        }
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

static bool _condition_088
    ( const verifier_main_struct_t * verifier_info
    , WdgM_SupervisedEntityType const  * wdgm_se, int counter
    )
{
    bool ok = true;

    if ( !( (wdgm_se->WdgMProgramFlowReferenceCycle > 0)
         || (wdgm_se->WdgMFailedProgramFlowRefCycleTol == 0)
          )
       )
    {
         ok = false;
    }

    if (!ok)
    {
        printf
            ( "Problem at supervised entity: >> %d << !\n"
            , wdgm_se->WdgMSupervisedEntityId
            );
    }

    return ok;
}

bool test_088
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 88\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "At each supervised entity the following condition shall be fulfilled:\n"
      "(WdgMFailedProgramFlowRefCycleTol = 0) OR (WdgMProgramFlowRefCycle > 0)"
      "\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_088);

    printf
        ( "%ssupervised entity's WdgMFailedProgramFlowRefCycleTol or\n"
          "WdgMProgramFlowRefCycle is greater than zero or zero: %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_107
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    int  i      = 0;

    WdgM_WatchdogDeviceType const * wdg_list     =
        main_struct->WdgMWatchdogDeviceRef;
    WdgM_TriggerModeType    const * trigger_mode = NULL;

    printf ("Test 107\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The ``WdgMTriggerTimeout`` field in every element in the\n"
          "``WdgMTriggerMode`` array (of type ``WdgM_TriggerModeType``)\n"
          "where trigger mode is not WDGIF_OFF_MODE shall have\n"
          "a value greater than zero\n"
        );

    CHECK_NULL_PTR (wdg_list, 1);

    for ( i = 0, trigger_mode = wdg_list->WdgMTriggerModeRef
        ; i < main_struct->NrOfWatchdogDevices * main_struct->NrOfTriggerModes
        ; i++, trigger_mode++
        )
    {
        CHECK_NULL_PTR (trigger_mode, 2);
        if (  (trigger_mode->WdgMWatchdogMode   != WDGIF_OFF_MODE)
           && (trigger_mode->WdgMTriggerTimeout == 0)
           )
        {
            printf
                ( "Found `WdgM_TriggerModeType` elements with a\n"
                  "condition value == 0 (see field `WdgMTriggerTimeout`)\n"
                );
            result = false;
            break;
        }
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_108 (const verifier_main_struct_t * verifier_info)
{
    bool result                        = true;
    int  i                             = 0;
    int core_id_source                 = 0;
    int core_id_dest                   = 0;
    global_transition_t const * gt     = NULL;
    entity_struct_t const *     source = NULL;
    entity_struct_t const *     dest   = NULL;

    printf ("Test 108\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The source Supervised Entity and the destination Supervised\n"
          "Entity of each global/external transition shall be part of\n"
          "OS applications running on the same CPU core.\n\n"
          "If no OS Application is referenced, the core id must be zero!\n\n"
        );
    if (verifier_info->global_transitions != NULL)
    {
      for
          ( i = 0, gt = verifier_info->global_transitions
          ; i < verifier_info->global_trans_cnt
          ; i++, gt++
          )
          {
              source = get_vi_entity_for_name (gt->source_se, verifier_info);
              dest   = get_vi_entity_for_name (gt->dest_se, verifier_info);
              if ((source == NULL) || (dest == NULL))
              {
                  result = false;
                  break;
              }

              if (verifier_info->os_application_array == NULL)
              {
                printf("No OS Applications are configured!\n\n");
              }
              else
              {
                core_id_source =
                    get_core_id_for_os_app (source->app_task_ref, verifier_info);
                core_id_dest   =
                    get_core_id_for_os_app (dest->app_task_ref, verifier_info);

                printf
                    ( "Verifying %s/%s (core %d) -> %s/%s (core %d)...\n"
                    , gt->source_se
                    , gt->source_cp
                    , core_id_source
                    , gt->dest_se
                    , gt->dest_cp
                    , core_id_dest
                    );
                if (core_id_source != core_id_dest)
                {
                    printf ("NOT OK\n\n");
                    result = false;
                }
              }
          }
    }
    else
    {
      printf("No global/external transitions configured!\n\n");
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_109
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    bool found  = false;
    const WdgM_SupervisedEntityType * ms_entity;
    int i;
    int cfg_core_id, vi_core_id;
    int entity_cnt = (int)main_struct->NrOfSupervisedEntities;
    entity_struct_t const * vi_se = NULL;

    printf ("Test 109\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "Each Supervised Entity's `WdgMEntityCoreId` field in the\n"
          "generated configuration shall match the CPU core ID which\n"
          "executes the OS Application related to the Supervised Entity\n"
          "according to the ECU Description File.\n\n"
          "If no OS Application is referenced, the core id must be zero!\n\n"
        );
    if (verifier_info->os_application_array == NULL)
    {
      printf("No OS Applications are configured!\n\n");
    }
    else
    {
      for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
          ; i < entity_cnt
          ; i++, ms_entity++
          )
      {
          found       = false;
          cfg_core_id = ms_entity->WdgMEntityCoreId;
          vi_se       = get_vi_se (verifier_info, ms_entity->WdgMSupervisedEntityId);
          if (vi_se != NULL)
          {
            vi_core_id = get_core_id_for_os_app(vi_se->app_task_ref, verifier_info);
            if (cfg_core_id == vi_core_id)
            {
                found = true;
            }
          }
          printf
              ( "Supervised Entity `%s`: Core ID %d is %scorrect.\n"
              , vi_se->name
              , cfg_core_id
              , found ? "" : "not "
              );
          result = result && found;
      }
    }

    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_110 (const verifier_main_struct_t * verifier_info)
{
    bool result                       = true;
    int  i                            = 0;
    local_transition_t const * lt     = NULL;

    printf ("Test 110\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The source checkpoint and the destination checkpoint\n"
          "of each local/internal transition shall be part of the same\n"
          "Supervised Entity.\n\n"
        );
    for
        ( i = 0, lt = verifier_info->local_transitions
        ; i < verifier_info->local_trans_cnt
        ; i++, lt++
        )
        {
            printf
                ( "Verifying %s/%s -> %s/%s...\n"
                , lt->source_se
                , lt->source_cp
                , lt->dest_se
                , lt->dest_cp
                );
            if (!equals (lt->source_se, lt->dest_se))
            {
                printf ("NOT OK\n\n");
                result = false;
            }
        }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_112
    ( const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    bool found  = true;
    int i, j;
    int global_trans_cnt   = verifier_info->global_trans_cnt;
    int local_trans_cnt    = verifier_info->local_trans_cnt;
    int dl_supervision_cnt = verifier_info->dl_supervision_cnt;

    printf ("Test 112\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "Every Deadline Supervision' source and destination shall match\n"
          " the source and destination of one existing global or local\n"
          "transition.\n"
          "\n"
        );
    dl_supervision_t const * supervision = NULL;
    global_transition_t const * gtr      = NULL;
    local_transition_t const  * ltr      = NULL;

    printf ("Found %d Deadline Supervisions\n", dl_supervision_cnt);
    for ( i = 0, supervision = verifier_info->dl_supervision_list
        ; i < dl_supervision_cnt
        ; i++, supervision++
        )
    {
        found = false;
        for ( j = 0, gtr = verifier_info->global_transitions
            ; (j < global_trans_cnt) && !found
            ; j++, gtr++
            )
        {
            if (  equals (gtr->source_se, supervision->source_se)
               && equals (gtr->source_cp, supervision->source_cp)
               && equals (gtr->dest_se,   supervision->dest_se)
               && equals (gtr->dest_cp,   supervision->dest_cp)
               )
            {
                found = true;
            }
        }
        if (!found)
        {
            for ( j = 0, ltr = verifier_info->local_transitions
                ; (j < local_trans_cnt) && !found
                ; j++, ltr++
                )
            {
                if (  equals (ltr->source_se, supervision->source_se)
                   && equals (ltr->source_cp, supervision->source_cp)
                   && equals (ltr->dest_se,   supervision->dest_se)
                   && equals (ltr->dest_cp,   supervision->dest_cp)
                   )
                {
                    found = true;
                }
            }
        }
        printf
            ( "Verifying Deadline Supervision %s/%s -> %s/%s... %s\n"
            , supervision->source_se
            , supervision->source_cp
            , supervision->dest_se
            , supervision->dest_cp
            , found ? "OK" : "NOT OK"
            );
        result = result && found;
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

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
/**        \file  black_a_tests.c
 *        \brief  black_a_tests source file
 *
 *      \details  This is the implementation of all test cases of the WdgM verifier of test block 'a'
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

#define FSEC_2_TICKS(vi_mode, sec)  \
    (int)((double)(((sec) * vi_mode->ticks_per_second) + 0.1))

/* --------------------------------------------------------------------------*/

bool test_001
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int cp_count           = countAllCPInSE (main_struct, false);
    int edf_cp_count       = xslt_checkpoint_cnt (verifier_info);

    printf ("Test 1\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The number of checkpoints according to the EDF and the number of\n"
          "checkpoints referenced by supervised entities must match.\n\n"
        );
    printf
        ( "X = Number of Checkpoints according to the ECU desc file = %d\n"
        , edf_cp_count
        );
    printf
        ( "Y = Total number of checkpoints referenced by Supervised Entities: "
          "%d\n"
        , cp_count
        );
    return print_comparison (edf_cp_count, cp_count);
}

bool test_002
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int nrOfAllCheckpoints = countAllCPInSE (main_struct, false);
    int edf_cp_count       = xslt_checkpoint_cnt (verifier_info);

    printf ("Test 2\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The number of checkpoints according to the EDF and the number of\n"
          "checkpoints stored in the NrOfAllCheckpoints member of the main\n"
          "structure must match.\n\n"
        );
    printf
        ( "X = Number of Checkpoints according to the ECU desc file = %d\n"
        , edf_cp_count
        );
    printf
        ( "Y = Number of Checkpoints according to main structure = %d\n\n"
        , nrOfAllCheckpoints
        );
    return print_comparison (edf_cp_count, nrOfAllCheckpoints);
}

bool test_003
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    int local_trans_count = total_local_trans_cnt
        (main_struct, verifier_info);
    int edf_local_trans_cnt = xslt_local_trans_cnt (verifier_info);
    newline ();
    printf ("Test 3\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("The number of local transitions according to the EDF shall\n"
            "match the number of local transitions referenced by checkpoints\n"
            "according to the corresponding NrOfLocalTransitions members.\n\n");
    printf
        ( "X = number of local transitions according to ECU Description File"
          " = %d\n"
        , edf_local_trans_cnt
        );
    printf
        ( "Y = number of local transitions referenced by checkpoints "
          " = %d\n\n"
        , local_trans_count
        );
    return print_comparison (edf_local_trans_cnt, local_trans_count);
}

bool test_004
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    int global_trans_count   =
        total_global_trans_cnt (main_struct, verifier_info);
    int edf_global_trans_cnt = xslt_global_trans_cnt (verifier_info);

    newline ();
    printf ("Test 4\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("The number of global transitions according to the EDF shall\n"
            "match the number of global transitions referenced by\n"
            "checkpoints according to the corresponding NrOfGlobalTransitions\n"
            "members.\n\n");

    printf
        ( "X = number of global transitions according to the ECU Description"
          "File = %d\n"
        , edf_global_trans_cnt
        );
    printf
        ( "Y = number of global transitions referenced by checkpoints "
          "= %d\n\n"
        , global_trans_count
        );

    return print_comparison (edf_global_trans_cnt, global_trans_count);
}

bool test_005
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    int edf_entity_cnt = xslt_entity_cnt (verifier_info);
    int entity_cnt     = main_struct->NrOfSupervisedEntities;
    newline ();
    printf ("Test 5\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("The number of supervised entities according to the EDF shall\n"
            "match the value of the main structure's NrOfSupervisedEntities\n"
            "member.\n\n");

    printf
        ( "X = number of supervised entities according to the ECU Description"
          "File = %d\n"
        , edf_entity_cnt
        );
    printf
        ( "Y = number of supervised entities according to the main structure "
          "= %d\n\n"
        , entity_cnt
        );
    return print_comparison (edf_entity_cnt, entity_cnt);
}

bool test_017
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result    = true;
    bool subresult = false;
    WdgM_CheckPointType const * first_cp = get_first_cp (main_struct);
    WdgM_CheckPointType const * cp;
    int cp_list_length = countAllCPInSE (main_struct, false);
    int i;
    int se_id;
    int t_cnt;

    newline ();

    if (!has_test_data (first_cp, 17, "checkpoints"))
    {
        return false;
    }

    for
        ( i = 0, cp = first_cp
        ; i < cp_list_length
        ; i++, cp++
        )
    {
        newline ();
        printf ("Test 17.%d\n", i);
        printf ("==========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "A checkpoint's NrOfStartedGlobalTransitions value shall match\n"
              "the number of global transitions having that checkpoint as\n"
              "starting point according to the EDF.\n"
            );
        newline ();
        printf
            ( "X = `NrOfStartedGlobalTransitions` of checkpoint at array\n"
              "offset %d = %d\n"
            , i
            , cp->NrOfStartedGlobalTransitions
            );
        se_id = get_se_id_for_cp_pos (main_struct, i);
        if (se_id == BAD_ID)    /* TODO: improve the error handling! */
        {
            return false;
        }
        t_cnt = count_global_transitions_starting_at
            (se_id, cp->WdgMCheckpointId, verifier_info);
        if (t_cnt == BAD_ID)   /* TODO: improve the error handling! */
        {
            return false;
        }
        printf
            ( "Y = number of global transitions started by checkpoint at\n"
              "array offset %d according to the EDF = %d\n\n"
            , i
            , t_cnt
            );
        subresult = print_comparison (cp->NrOfStartedGlobalTransitions, t_cnt);
        result    = result && subresult;
    }
    return result;
}

bool test_019
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    entity_struct_t const * vi_entity;
    int checkpoints;
    int i, j, k;
    unsigned int X, Y;

    newline ();
    printf ("Test 19\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If an alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMExpectedAliveIndications member shall match\n"
            "the alive supervision's number of expected alive indications\n"
            "as specified in the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMExpectedAliveIndications\n");
    printf ("Y = verifier_info->alive_supervision_list[index].\n"
            "WdgMExpectedAliveIndications\n");
    newline ();

    ASSERT_ENTITIES();

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for
                ( j = 0, check_point = ms_entity->WdgMCheckpointRef
                ; j < checkpoints
                ; j++, check_point++
                )
            {
                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    printf ("[%u,%u]: No alive supervision defined.\n",i,j);
                }
                else
                {
                    char const * se_name = get_vi_se_name
                        (verifier_info, ms_entity->WdgMSupervisedEntityId);
                    char const * cp_name = get_vi_cp_name
                        ( verifier_info
                        , ms_entity->WdgMSupervisedEntityId
                        , check_point->WdgMCheckpointId
                        );
                    int supervisions = (int)
                        verifier_info->alive_supervision_cnt;
                    bool found = false;

                    X = (unsigned int)
                        check_point->WdgMExpectedAliveIndications;
                    Y = (unsigned int) 0;

                    for (k = 0; k < supervisions; k++)
                    {
                            alive_supervision_t * alive_supervision =
                            &verifier_info->alive_supervision_list[k];

                        if ( (equals(se_name,alive_supervision->se))
                          && (equals(cp_name,alive_supervision->cp))
                           )
                        {
                            found = true;
                            Y = (unsigned int)
                                verifier_info->alive_supervision_list[k].
                                    WdgMExpectedAliveIndications;
                            break;
                        }
                    }

                    if (found)
                    {
                        is_ok = (X == Y);
                        printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                               , i, j, X, Y
                               , is_ok ? "OK" : "NOT OK"
                               );
                    }
                    else
                    {
                        is_ok = false;
                        printf ("[%u,%u]: alive_supervision not found.\n",i,j);
                    }
                    result = result && is_ok;
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_020
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok = true;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity         = NULL;
    WdgM_CheckPointType       const * check_point       = NULL;
    entity_struct_t           const * vi_entity         = NULL;
    alive_supervision_t       const * alive_supervision = NULL;
    int checkpoints;
    int i, j, k;
    unsigned int cp_min_margin, vi_min_margin;
    unsigned int cp_max_margin, vi_max_margin;
    unsigned int cp_indications, vi_indications;
    unsigned int cp_ref_cycle, vi_ref_cycle;

    newline ();
    printf ("Test 20\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If an alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMMinMargin member shall match the corresponding\n"
            "attribute (.../WdgMMinMargin) in the alive supervision as\n"
            "specified in the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMMinMargin\n");
    printf ("Y = verifier_info->alive_supervision_list[index].\n"
            "WdgMMinMargin\n");
    newline ();

    ASSERT_ENTITIES();

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for
                ( j = 0, check_point = ms_entity->WdgMCheckpointRef
                ; j < checkpoints
                ; j++, check_point++
                )
            {

                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    printf
                        ( "SE/CP %u/%u: No alive supervision defined.\n"
                        , ms_entity->WdgMSupervisedEntityId
                        , check_point->WdgMCheckpointId
                        );
                }
                else
                {
                    char const * se_name = get_vi_se_name
                        (verifier_info, ms_entity->WdgMSupervisedEntityId);
                    char const * cp_name = get_vi_cp_name
                        ( verifier_info
                        , ms_entity->WdgMSupervisedEntityId
                        , check_point->WdgMCheckpointId
                        );
                    int supervisions = (int)
                        verifier_info->alive_supervision_cnt;
                    bool found = false;

                    cp_min_margin  = (unsigned int) check_point->WdgMMinMargin;
                    cp_max_margin  = (unsigned int) check_point->WdgMMaxMargin;
                    cp_indications = (unsigned int) check_point->WdgMExpectedAliveIndications;
                    cp_ref_cycle   = (unsigned int) check_point->WdgMSupervisionReferenceCycle;
                    vi_min_margin  = (unsigned int) 0;
                    vi_max_margin  = (unsigned int) 0;
                    vi_indications = (unsigned int) 0;
                    vi_ref_cycle   = (unsigned int) 0;

                    for
                        (   k                 = 0
                          , alive_supervision =
                            verifier_info->alive_supervision_list
                        ; k < supervisions
                        ; k++, alive_supervision++
                        )
                    {
                        if ( (equals(se_name,alive_supervision->se))
                          && (equals(cp_name,alive_supervision->cp))
                           )
                        {
                            found         = true;
                            vi_min_margin =
                                (unsigned int) alive_supervision->WdgMMinMargin;
                            vi_max_margin =
                                (unsigned int) alive_supervision->WdgMMaxMargin;
                            vi_indications =
                                (unsigned int) alive_supervision->WdgMExpectedAliveIndications;
                            vi_ref_cycle   =
                                (unsigned int) alive_supervision->WdgMSupervisionReferenceCycle;
                            break;
                        }
                    }

                    if (found)
                    {
                        is_ok =
                            (  (cp_min_margin  == vi_min_margin)
                            && (cp_max_margin  == vi_max_margin)
                            && (cp_indications == vi_indications)
                            && (cp_ref_cycle   == vi_ref_cycle)
                            );
                        printf ("%s/%s - result = %s\n"
                               , se_name
                               , cp_name
                               , is_ok ? "OK" : "NOT OK"
                               );
                        if (!is_ok)
                        {
                            printf
                                ( "Minimum margin: %d, %d; maximum margin: %d, %d, "
                                  "expected alive indications: %d, %d, "
                                  "reference cycle: %d, %d.\n"
                                , cp_min_margin, vi_min_margin
                                , cp_max_margin, vi_max_margin
                                , cp_indications, vi_indications
                                , cp_ref_cycle, vi_ref_cycle
                                );
                        }
                    }
                    else
                    {
                        is_ok = false;
                        printf
                            ( "%s/%s: alive_supervision not found.\n"
                            , se_name
                            , cp_name
                            );
                    }
                    result = result && is_ok;
                }
            }
        }
        newline ();
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_021
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    entity_struct_t           const * vi_entity;
    alive_supervision_t       const * alive_supervision = NULL;
    int checkpoints;
    int i, j, k;
    unsigned int X, Y;

    newline ();
    printf ("Test 21\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If an alive supervision is defined for a checkpoint then\n"
            "that checkpoint's WdgMMaxMargin member shall match the\n"
            "correspoinding attribute (..../WdgMMaxMargin) in the alive\n"
            "supervision asspecified in the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMMaxMargin\n");
    printf ("Y = verifier_info->alive_supervision_list[index].\n"
            "WdgMMaxMargin\n");
    newline ();

    ASSERT_ENTITIES();

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for
                ( j = 0, check_point = ms_entity->WdgMCheckpointRef
                ; j < checkpoints
                ; j++, check_point++
                )
            {
                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    printf ("[%u,%u]: No alive supervision defined.\n",i,j);
                }
                else
                {
                    char const * se_name = get_vi_se_name
                        (verifier_info, ms_entity->WdgMSupervisedEntityId);
                    char const * cp_name = get_vi_cp_name
                        ( verifier_info
                        , ms_entity->WdgMSupervisedEntityId
                        , check_point->WdgMCheckpointId
                        );
                    int supervisions = (int)
                        verifier_info->alive_supervision_cnt;
                    bool found = false;

                    X = (unsigned int)
                        check_point->WdgMMaxMargin;
                    Y = (unsigned int) 0;

                    for
                        (   k                 = 0
                          , alive_supervision =
                            verifier_info->alive_supervision_list
                        ; k < supervisions
                        ; k++, alive_supervision++
                        )
                    {
                        if ( (equals(se_name,alive_supervision->se))
                          && (equals(cp_name,alive_supervision->cp))
                           )
                        {
                            found = true;
                            Y     =
                                (unsigned int) alive_supervision->WdgMMaxMargin;
                            break;
                        }
                    }

                    if (found)
                    {
                        is_ok = (X == Y);
                        printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                               , i, j, X, Y
                               , is_ok ? "OK" : "NOT OK"
                               );
                    }
                    else
                    {
                        is_ok = false;
                        printf ("[%u,%u]: alive_supervision not found.\n",i,j);
                    }
                    result = result && is_ok;
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_022
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity;
    WdgM_CheckPointType       const * check_point;
    alive_supervision_t       const * alive_supervision = NULL;
    int checkpoints;
    int i, j, k;
    unsigned int X, Y;

    newline ();
    printf ("Test 22\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If an alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMSupervisionReferenceCycle member shall match\n"
            "the correspoinding attribute (../WdgMSupervisionReferenceCycle)\n"
            "in the alive supervision as specified in the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMSupervisionReferenceCycle\n");
    printf ("Y = verifier_info->alive_supervision_list[index].\n"
            "WdgMSupervisionReferenceCycle\n");
    newline ();

    ASSERT_ENTITIES();

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            if ( (check_point->WdgMAliveLRef == NULL)
              || (check_point->WdgMAliveGRef == NULL)
               )
            {
                printf ("[%u,%u]: No alive supervision defined.\n",i,j);
            }
            else
            {
                char const * se_name = get_vi_se_name
                    (verifier_info, ms_entity->WdgMSupervisedEntityId);
                char const * cp_name = get_vi_cp_name
                    ( verifier_info
                    , ms_entity->WdgMSupervisedEntityId
                    , check_point->WdgMCheckpointId
                    );
                int supervisions = (int)
                    verifier_info->alive_supervision_cnt;
                bool found = false;

                X = (unsigned int)
                    check_point->WdgMSupervisionReferenceCycle;
                Y = (unsigned int) 0;

                for
                    (   k                 = 0
                      , alive_supervision =
                        verifier_info->alive_supervision_list
                    ; k < supervisions
                    ; k++, alive_supervision++
                    )
                {
                    if ( (equals(se_name,alive_supervision->se))
                      && (equals(cp_name,alive_supervision->cp))
                       )
                    {
                        found = true;
                        Y = (unsigned int)
                            verifier_info->alive_supervision_list[k].
                                WdgMSupervisionReferenceCycle;
                        break;
                    }
                }

                if (found)
                {
                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                }
                else
                {
                    is_ok = false;
                    printf ("[%u,%u]: alive_supervision not found.\n",i,j);
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

bool test_027
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity        = NULL;
    WdgM_CheckPointType       const * check_point      = NULL;
    local_transition_t        const * local_transition = NULL;
    char                      const * se_name  = NULL;
    char                      const * cp_name  = NULL;
    int transitions = 0;


    int checkpoints;
    int i, j, k;
    unsigned int X, Y;

    newline ();
    printf ("Test 27\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("checkpoint's NrOfLocalTransitions value shall be set to the\n"
            "number of local transitions having that checkpoint as\n"
            "destination point according to the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].NrOfLocalTransitions\n");
    printf ("Y = COUNT(verifier_info->local_transitions[index])\n"
            "    with se_name == local_transitions[index].dest_se\n"
            "    and  cp_name == local_transitions[index].dest_cp\n");
    newline ();

    ASSERT_ENTITIES();

    for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CHECK_NULL_PTR (ms_entity, 1);
        checkpoints = (int) ms_entity->NrOfCheckpoints;
        printf ( "Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId
               , checkpoints
               );

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            CHECK_NULL_PTR (check_point, 2);
            se_name = get_vi_se_name
                (verifier_info, ms_entity->WdgMSupervisedEntityId);
            cp_name = get_vi_cp_name
                ( verifier_info
                , ms_entity->WdgMSupervisedEntityId
                , check_point->WdgMCheckpointId
                );
            transitions = (int) verifier_info->local_trans_cnt;


            X = (unsigned int) check_point->NrOfLocalTransitions;
            Y = (unsigned int) 0;

            for ( k = 0, local_transition = verifier_info->local_transitions
                ; k < transitions
                ; k++, local_transition++
                )
            {
                CHECK_NULL_PTR (local_transition, 3);
                if
                    (  (equals (se_name, local_transition->dest_se))
                    && (equals (cp_name, local_transition->dest_cp))
                    )
                {
                    Y++;
                }
            }

            is_ok = (X == Y);
            printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                   , i, j, X, Y
                   , is_ok ? "OK" : "NOT OK"
                   );
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_028
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity         = NULL;
    WdgM_CheckPointType       const * check_point       = NULL;
    global_transition_t       const * global_transition = NULL;
    char                      const * se_name           = NULL;
    char                      const * cp_name           = NULL;
    int transitions = 0;
    int checkpoints;
    int i, j, k;
    unsigned int X, Y;

    newline ();
    printf ("Test 28\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("checkpoint's NrOfGlobalTransitions value shall be set to the\n"
            "number of global transitions having that checkpoint as\n"
            "destination point according to the EDF.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].NrOfGlobalTransitions\n");
    printf ("Y = COUNT(verifier_info->global_transitions[index])\n"
            "    with se_name == global_transitions[index].dest_se\n"
            "    and  cp_name == global_transitions[index].dest_cp\n");
    newline ();

    ASSERT_ENTITIES();

    for ( i = 0, ms_entity = main_struct->WdgMSupervisedEntityRef
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CHECK_NULL_PTR (ms_entity, 1);
        checkpoints = (int) ms_entity->NrOfCheckpoints;
        printf ( "Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId
               , checkpoints
               );

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            CHECK_NULL_PTR (check_point, 2);
            se_name = get_vi_se_name
                (verifier_info, ms_entity->WdgMSupervisedEntityId);
            cp_name = get_vi_cp_name
                ( verifier_info
                , ms_entity->WdgMSupervisedEntityId
                , check_point->WdgMCheckpointId
                );
            transitions = (int)
                verifier_info->global_trans_cnt;

            CHECK_NULL_PTR (check_point, 3);

            X = (unsigned int)
                check_point->NrOfGlobalTransitions;
            Y = (unsigned int) 0;

            for ( k = 0, global_transition = verifier_info->global_transitions
                ; k < transitions
                ; k++, global_transition++
                )
            {
                CHECK_NULL_PTR (global_transition, 4);
                if
                    (  (equals (se_name, global_transition->dest_se))
                    && (equals (cp_name, global_transition->dest_cp))
                    )
                {
                    Y++;
                }
            }

            is_ok = (X == Y);
            printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                   , i, j, X, Y
                   , is_ok ? "OK" : "NOT OK"
                   );
            result = result && is_ok;
            newline ();
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_032
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    entity_struct_t const * vi_entity;
    int checkpoints;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 32\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If no alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMExpectedAliveIndications shall be zero.\n"
            "(See Test 19)\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMExpectedAliveIndications\n");
    printf ("Y = 0\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for (j = 0; j < checkpoints; j++)
            {
                check_point = &ms_entity->WdgMCheckpointRef[j];

                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    X = (unsigned int)
                        check_point->WdgMExpectedAliveIndications;
                    Y = (unsigned int) 0;

                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                    result = result && is_ok;
                }
                else
                {
                    printf ("[%u,%u]: Alive supervision defined.\n",i,j);
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_033
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    entity_struct_t const * vi_entity;
    int checkpoints;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 33\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If no alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMMinMargin member shall be zero. (See Test 20)\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMMinMargin\n");
    printf ("Y = 0\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for (j = 0; j < checkpoints; j++)
            {
                check_point = &ms_entity->WdgMCheckpointRef[j];

                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    X = (unsigned int)
                        check_point->WdgMMinMargin;
                    Y = (unsigned int) 0;

                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                    result = result && is_ok;
                }
                else
                {
                    printf ("[%u,%u]: Alive supervision defined.\n",i,j);
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_034
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    entity_struct_t const * vi_entity;
    int checkpoints;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 34\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If no alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMMaxMargin member shall be zero. (See Test 21)\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMMaxMargin\n");
    printf ("Y = 0\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for (j = 0; j < checkpoints; j++)
            {
                check_point = &ms_entity->WdgMCheckpointRef[j];

                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    X = (unsigned int)
                        check_point->WdgMMaxMargin;
                    Y = (unsigned int) 0;

                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                    result = result && is_ok;
                }
                else
                {
                    printf ("[%u,%u]: Alive supervision defined.\n",i,j);
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_035
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    const WdgM_CheckPointType * check_point;
    entity_struct_t const * vi_entity;
    int checkpoints;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 35\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("If no alive supervision is defined for a checkpoint then that\n"
            "checkpoint's WdgMSupervisionReferenceCycle member shall be\n"
            "zero. (See Test 22)\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].WdgMSupervisionReferenceCycle\n");
    printf ("Y = 0\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for (j = 0; j < checkpoints; j++)
            {
                check_point = &ms_entity->WdgMCheckpointRef[j];

                if ( (check_point->WdgMAliveLRef == NULL)
                  || (check_point->WdgMAliveGRef == NULL)
                   )
                {
                    X = (unsigned int)
                        check_point->WdgMSupervisionReferenceCycle;
                    Y = (unsigned int) 0;

                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                    result = result && is_ok;
                }
                else
                {
                    printf ("[%u,%u]: Alive supervision defined.\n",i,j);
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_037
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity      = NULL;
    WdgM_CheckPointType const       * check_point    = NULL;
    WdgM_TransitionType const       * p_transition   = NULL;
    dl_supervision_t const          * dl_supervision = NULL;
    entity_struct_t const           * vi_entity      = NULL;
    int checkpoints;
    int transitions;
    int i, j, k, l;
    int core_id = main_struct->WdgMModeCoreAssignment;
    unsigned int X, Y;

    wdgm_mode_t const * vi_mode = get_vi_wdgm_mode (verifier_info, core_id);

    newline ();
    printf ("Test 37\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("WdgM_TransitionType->WdgMDeadlineMin shall match the\n"
            "corresponding value in the ECU description file.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMLocalTransitionRef[transition].\n"
            "WdgMDeadlineMin\n");
    printf ("Y = verifier_info->dl_supervision_list[index].dl_min\n");
    newline ();

    ASSERT_ENTITIES();

    if (vi_mode == NULL)
    {
        result = false;
        print_result (result);
        newline ();
        return result;
    }

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);

        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            ASSERT_CHECKPOINTS(ms_entity);

            for
                ( j = 0, check_point = ms_entity->WdgMCheckpointRef
                ; j < checkpoints
                ; j++, check_point++
                )
            {
                transitions = (int)check_point->NrOfLocalTransitions;

                ASSERT_L_TRANSITIONS(check_point)

                printf ("Checkpoint[%u].NrOfLocalTransitions = %u\n"
                       , j, transitions);

                for
                    ( k = 0, p_transition = check_point->WdgMLocalTransitionRef
                    ; k < transitions
                    ; k++, p_transition++
                    )
                {
                    char const * se_name_dst = get_vi_se_name
                        (verifier_info, ms_entity->WdgMSupervisedEntityId);
                    char const * cp_name_dst = get_vi_cp_name
                        ( verifier_info
                        , ms_entity->WdgMSupervisedEntityId
                        , check_point->WdgMCheckpointId
                        );
                    char * se_name_src = (char*)
                        get_vi_se_name
                            ( verifier_info
                            , p_transition->EntitySourceId
                            );
                    char * cp_name_src = (char*)
                        get_vi_cp_name
                            ( verifier_info
                            , p_transition->EntitySourceId
                            , p_transition->CheckpointSourceId
                            );
                    int supervisions = (int)
                        verifier_info->dl_supervision_cnt;
                    bool found = false;



                    if (p_transition->WdgMDeadlineMax == 0)
                    {
                        continue;
                    }

                    X = (unsigned int) p_transition->WdgMDeadlineMin;
                    Y = (unsigned int) 0;

                    for
                        ( l = 0
                          , dl_supervision = verifier_info->dl_supervision_list
                        ; l < supervisions
                        ; l++, dl_supervision++
                        )
                    {
                        if ( (equals(se_name_dst,dl_supervision->dest_se))
                          && (equals(cp_name_dst,dl_supervision->dest_cp))
                          && (equals(se_name_src,dl_supervision->source_se))
                          && (equals(cp_name_src,dl_supervision->source_cp))
                           )
                        {
                            double dl_val = (double) dl_supervision->dl_min;

                            found = true;
                            printf ("dl_min = %f\n",dl_val);
                            Y = (unsigned int)FSEC_2_TICKS(vi_mode, dl_val);
                            break;
                        }
                    }

                    if (found)
                    {
                        is_ok = (X == Y);
                        printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                               , i, j, X, Y
                               , is_ok ? "OK" : "NOT OK"
                               );
                    }
                    else
                    {
                        is_ok = false;
                        printf ("[%u,%u]: dl_supervision not found."
                                " result = NOT OK\n"
                               , i, j
                               );
                    }
                    result = result && is_ok;
                }
                newline ();
            }
        }
    }
    newline ();
    print_result (result);
    newline ();
    return result;
}

bool test_038
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity      = NULL;
    WdgM_CheckPointType       const * check_point    = NULL;
    WdgM_TransitionType       const * p_transition   = NULL;
    dl_supervision_t          const * dl_supervision = NULL;
    int checkpoints;
    int transitions;
    int i, j, k, l;
    int core_id = main_struct->WdgMModeCoreAssignment;
    unsigned int X, Y;

    wdgm_mode_t const * vi_mode = get_vi_wdgm_mode (verifier_info, core_id);

    newline ();
    printf ("Test 38\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("WdgM_TransitionType->WdgMDeadlineMax shall match the\n"
            "corresponding value in the ECU description file.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMLocalTransitionRef[transition].WdgMDeadlineMax\n");
    printf ("Y = verifier_info->dl_supervision_list[index].dl_max\n");
    newline ();

    ASSERT_ENTITIES();

    if (vi_mode == NULL)
    {
        result = false;
        print_result (result);
        newline ();
        return result;
    }

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            transitions = (int)check_point->NrOfLocalTransitions;

            ASSERT_L_TRANSITIONS(check_point)

            printf ("Checkpoint[%u].NrOfLocalTransitions = %u\n"
                   , j, transitions);

            for
                ( k = 0, p_transition = check_point->WdgMLocalTransitionRef
                ; k < transitions
                ; k++, p_transition++
                )
            {
                char const * se_name_dst = get_vi_se_name
                    (verifier_info, ms_entity->WdgMSupervisedEntityId);
                char const * cp_name_dst = get_vi_cp_name
                    ( verifier_info
                    , ms_entity->WdgMSupervisedEntityId
                    , check_point->WdgMCheckpointId
                    );
                char * se_name_src = (char*)
                    get_vi_se_name
                        ( verifier_info
                        , p_transition->EntitySourceId
                        );
                char * cp_name_src = (char*)
                    get_vi_cp_name
                        ( verifier_info
                        , p_transition->EntitySourceId
                        , p_transition->CheckpointSourceId
                        );
                int supervisions = (int)
                    verifier_info->dl_supervision_cnt;
                bool found = false;

                if (p_transition->WdgMDeadlineMax == 0)
                {
                    continue;
                }

                X = (unsigned int) p_transition->WdgMDeadlineMax;
                Y = (unsigned int) 0;

                for
                    ( l = 0
                      , dl_supervision = verifier_info->dl_supervision_list
                    ; l < supervisions
                    ; l++, dl_supervision++
                    )
                {
                    if ( (equals(se_name_dst,dl_supervision->dest_se))
                      && (equals(cp_name_dst,dl_supervision->dest_cp))
                      && (equals(se_name_src,dl_supervision->source_se))
                      && (equals(cp_name_src,dl_supervision->source_cp))
                       )
                    {
                        double dl_val = (double) dl_supervision->dl_max;

                        found = true;
                        printf ("dl_max = %f\n",dl_val);
                        Y = (unsigned int)FSEC_2_TICKS(vi_mode, dl_val);
                        break;
                    }
                }

                if (found)
                {
                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                }
                else
                {
                    is_ok = false;
                    printf ("[%u,%u]: dl_supervision not found."
                            " result = NOT OK\n"
                           , i, j
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

bool test_039
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity      = NULL;
    WdgM_CheckPointType       const * check_point    = NULL;
    WdgM_GlobalTransitionType const * p_transition   = NULL;
    dl_supervision_t          const * dl_supervision = NULL;
    int checkpoints;
    int transitions;
    int i, j, k, l;
    int core_id = main_struct->WdgMModeCoreAssignment;
    unsigned int X, Y;

    wdgm_mode_t const * vi_mode = get_vi_wdgm_mode (verifier_info, core_id);

    newline ();
    printf ("Test 39\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("WdgM_GlobalTransitionType->WdgMDeadlineMin shall match the\n"
            "corresponding value in the ECU description file.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMGlobalTransitionsRef[transition].WdgMDeadlineMin\n");
    printf ("Y = verifier_info->dl_supervision_list[index].dl_min\n");
    newline ();

    ASSERT_ENTITIES();

    if (vi_mode == NULL)
    {
        result = false;
        print_result (result);
        newline ();
        return result;
    }

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            transitions = (int)check_point->NrOfGlobalTransitions;

            ASSERT_G_TRANSITIONS(check_point)

            printf ("Checkpoint[%u].NrOfGlobalTransitions = %u\n"
                   , j, transitions);

            for
                ( k = 0, p_transition = check_point->WdgMGlobalTransitionsRef
                ; k < transitions
                ; k++, p_transition++
                )
            {
                char const * se_name_dst = get_vi_se_name
                    (verifier_info, ms_entity->WdgMSupervisedEntityId);
                char const * cp_name_dst = get_vi_cp_name
                    ( verifier_info
                    , ms_entity->WdgMSupervisedEntityId
                    , check_point->WdgMCheckpointId
                    );
                char * se_name_src = (char*)
                    get_vi_se_name (verifier_info, p_transition->EntitySourceId);
                char * cp_name_src = (char*)
                    get_vi_cp_name
                        ( verifier_info
                        , p_transition->EntitySourceId
                        , p_transition->CheckpointSourceId
                        );
                int supervisions = (int) verifier_info->dl_supervision_cnt;
                bool found = false;

                if (p_transition->WdgMDeadlineMin == 0)
                {
                    continue;
                }

                X = (unsigned int) p_transition->WdgMDeadlineMin;
                Y = (unsigned int) 0;

                for
                    ( l = 0
                      , dl_supervision = verifier_info->dl_supervision_list
                    ; l < supervisions
                    ; l++, dl_supervision++
                    )
                {

                    if ( (equals(se_name_dst,dl_supervision->dest_se))
                      && (equals(cp_name_dst,dl_supervision->dest_cp))
                      && (equals(se_name_src,dl_supervision->source_se))
                      && (equals(cp_name_src,dl_supervision->source_cp))
                       )
                    {
                        double dl_val = (double) dl_supervision->dl_min;

                        found = true;
                        printf ("dl_min = %f\n",dl_val);
                        Y = (unsigned int)FSEC_2_TICKS(vi_mode, dl_val);
                        break;
                    }
                }

                if (found)
                {
                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                }
                else
                {
                    is_ok = false;
                    printf ("[%u,%u]: dl_supervision not found."
                            " result = NOT OK\n"
                           , i, j
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

bool test_040
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    WdgM_SupervisedEntityType const * ms_entity      = NULL;
    WdgM_CheckPointType       const * check_point    = NULL;
    WdgM_GlobalTransitionType const * p_transition   = NULL;
    dl_supervision_t          const * dl_supervision = NULL;
    int checkpoints;
    int transitions;
    int i, j, k, l;
    int core_id = main_struct->WdgMModeCoreAssignment;
    unsigned int X, Y;

    wdgm_mode_t const * vi_mode = get_vi_wdgm_mode (verifier_info, core_id);

    newline ();
    printf ("Test 40\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("WdgM_GlobalTransitionType->WdgMDeadlineMax shall match the\n"
            "corresponding value in the ECU description file.\n"
            "\n");

    printf ("NrOfSupervisedEntities = %u\n", number_of_se_in_mode (main_struct));
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMCheckpointRef[checkpoint].\n"
            "WdgMGlobalTransitionsRef[transition].WdgMDeadlineMin\n");
    printf ("Y = verifier_info->dl_supervision_list[index].dl_max\n");
    newline ();

    ASSERT_ENTITIES();

    if (vi_mode == NULL)
    {
        result = false;
        print_result (result);
        newline ();
        return result;
    }

    for
        ( i = 0, ms_entity = get_first_se (main_struct)
        ; i < ms_entities
        ; i++, ms_entity++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (ms_entity);
        checkpoints = (int)ms_entity->NrOfCheckpoints;
        printf ("Entity(id=%u).NrOfCheckpoints = %u\n"
               , ms_entity->WdgMSupervisedEntityId, checkpoints);

        ASSERT_CHECKPOINTS(ms_entity);

        for
            ( j = 0, check_point = ms_entity->WdgMCheckpointRef
            ; j < checkpoints
            ; j++, check_point++
            )
        {
            transitions = (int)check_point->NrOfGlobalTransitions;

            ASSERT_G_TRANSITIONS(check_point)

            printf ("Checkpoint[%u].NrOfGlobalTransitions = %u\n"
                   , j, transitions);

            for
                ( k = 0, p_transition = check_point->WdgMGlobalTransitionsRef
                ; k < transitions
                ; k++, p_transition++
                )
            {
                char const * se_name_dst = get_vi_se_name
                    (verifier_info, ms_entity->WdgMSupervisedEntityId);
                char const * cp_name_dst = get_vi_cp_name
                    ( verifier_info
                    , ms_entity->WdgMSupervisedEntityId
                    , check_point->WdgMCheckpointId
                    );
                char * se_name_src = (char*)
                    get_vi_se_name
                        ( verifier_info
                        , p_transition->EntitySourceId
                        );
                char * cp_name_src = (char*)
                    get_vi_cp_name
                        ( verifier_info
                        , p_transition->EntitySourceId
                        , p_transition->CheckpointSourceId
                        );
                int supervisions = (int)
                    verifier_info->dl_supervision_cnt;
                bool found = false;

                if (p_transition->WdgMDeadlineMax == 0)
                {
                    continue;
                }

                X = (unsigned int) p_transition->WdgMDeadlineMax;
                Y = (unsigned int) 0;

                for
                    ( l = 0
                      , dl_supervision = verifier_info->dl_supervision_list
                    ; l < supervisions
                    ; l++, dl_supervision++
                    )
                {

                    if ( (equals(se_name_dst,dl_supervision->dest_se))
                      && (equals(cp_name_dst,dl_supervision->dest_cp))
                      && (equals(se_name_src,dl_supervision->source_se))
                      && (equals(cp_name_src,dl_supervision->source_cp))
                       )
                    {
                        double dl_val = (double) dl_supervision->dl_max;

                        found = true;
                        printf ("dl_max = %f\n",dl_val);
                        Y = (unsigned int)FSEC_2_TICKS(vi_mode, dl_val);
                        break;
                    }
                }

                if (found)
                {
                    is_ok = (X == Y);
                    printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                           , i, j, X, Y
                           , is_ok ? "OK" : "NOT OK"
                           );
                }
                else
                {
                    is_ok = false;
                    printf ("[%u,%u]: dl_supervision not found."
                            " result = NOT OK\n"
                           , i, j
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

bool test_041
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;


    newline ();
    printf ("Test 41\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("Every supervised entity's WdgMInitialStatus value shall match\n"
            "the value entered as WdgMSupervisedEntityInitialMode for the\n"
            "WdgMLocalStatusParams element assigned to the supervised entity."
            "\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMInitialStatus\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMSupervisedEntityInitialMode\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int) ms_entity->WdgMInitialStatus;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMSupervisedEntityInitialMode;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

bool test_042
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 42\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("For every entity: X shall match Y where X is the\n"
            "WdgMFailedSupervisionRefCycleTol member of a that supervised\n"
            "entity in the generated configuration and Y is the element\n"
            "WdgMFailedSupervisionRefCycleTol in the WdgMLocalStatusParams\n"
            "defined for the same entity in the EDF.\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMFailedSupervisionRefCycleTol\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMFailedSupervisionRefCycleTol\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int)
                ms_entity->WdgMFailedSupervisionRefCycleTol;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMFailedSupervisionRefCycleTol;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

bool test_043
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 43\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("For every entity: X shall match Y where X is the\n"
            "WdgMFailedDeadlineRefCycleTol member of a that supervised\n"
            "entity in the generated configuration and Y is the element\n"
            "WdgMFailedDeadlineRefCycleTol in the WdgMLocalStatusParams\n"
            "defined for the same entity in the EDF.\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMFailedDeadlineRefCycleTol\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMFailedDeadlineRefCycleTol\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int)
                ms_entity->WdgMFailedDeadlineRefCycleTol;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMFailedDeadlineRefCycleTol;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

bool test_044
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 44\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("For every entity: X shall match Y where X is the\n"
            "WdgMDeadlineReferenceCycle member of a that supervised\n"
            "entity in the generated configuration and Y is the element\n"
            "WdgMDeadlineReferenceCycle in the WdgMLocalStatusParams\n"
            "defined for the same entity in the EDF.\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMDeadlineReferenceCycle\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMDeadlineReferenceCycle\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int)
                ms_entity->WdgMDeadlineReferenceCycle;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMDeadlineReferenceCycle;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

bool test_045
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 45\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("For every entity: X shall match Y where X is the\n"
            "WdgMFailedProgramFlowRefCycleTol member of a that supervised\n"
            "entity in the generated configuration and Y is the element\n"
            "WdgMFailedProgramFlowRefCycleTol in the WdgMLocalStatusParams\n"
            "defined for the same entity in the EDF.\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMFailedProgramFlowRefCycleTol\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMFailedProgramFlowRefCycleTol\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int)
                ms_entity->WdgMFailedProgramFlowRefCycleTol;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMFailedProgramFlowRefCycleTol;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

bool test_046
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    bool result = true, is_ok;
    int ms_entities = (int)main_struct->NrOfSupervisedEntities;
    const WdgM_SupervisedEntityType * ms_entity;
    entity_struct_t const * vi_entity;
    int i, j;
    unsigned int X, Y;

    newline ();
    printf ("Test 46\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("For every entity: X shall match Y where X is the\n"
            "WdgMProgramFlowReferenceCycle member of a that supervised\n"
            "entity in the generated configuration and Y is the element\n"
            "WdgMProgramFlowReferenceCycle in the WdgMLocalStatusParams\n"
            "defined for the same entity in the EDF.\n\n");

    printf ("NrOfSupervisedEntities = %u\n",ms_entities);
    printf ("X = main_struct->WdgMSupervisedEntityRef[entity].\n"
            "WdgMProgramFlowReferenceCycle\n");
    printf ("Y = verifier_info->lsp_list[index].\n"
            "WdgMProgramFlowReferenceCycle\n");
    newline ();

    ASSERT_ENTITIES();

    for (i = 0; i < ms_entities; i++)
    {
        ms_entity = &main_struct->WdgMSupervisedEntityRef[i];
        vi_entity =
            get_vi_entity_for_id (ms_entity->WdgMSupervisedEntityId, verifier_info, true);

        if (vi_entity == NULL)
        {
            result = false;
            printf ("vi_entity id=%u not found.\n"
                    , ms_entity->WdgMSupervisedEntityId);
        }
        else
        {
            char * se_name = (char*)vi_entity->name;
            int local_status_params = (int)verifier_info->lsp_cnt;
            bool found = false;

            X = (unsigned int)
                ms_entity->WdgMProgramFlowReferenceCycle;
            Y = (unsigned int) 0;

            for (j = 0; j < local_status_params; j++)
            {
                const local_status_param_t * local_status_param =
                    &verifier_info->lsp_list[j];

                if (equals(se_name,local_status_param->se))
                {
                    found = true;
                    Y = (unsigned int)
                        local_status_param->WdgMProgramFlowReferenceCycle;
                    break;
                }
            }

            if (found)
            {
                is_ok = (X == Y);
                printf ("[%u,%u]: X = %u, Y = %u, result = %s\n"
                       , i, j, X, Y
                       , is_ok ? "OK" : "NOT OK"
                       );
            }
            else
            {
                is_ok = false;
                printf ("[%u,%u]: dl_supervision not found.\n",i,j);
            }
            result = result && is_ok;
        }
    }
    newline ();
    return result;
}

static bool _condition_047 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    bool ok = true;

    if (wdgm_se->OSApplication != WDGM_INVALID_OSAPPLICATION)
    {
        ok = false;
        printf("The Supervised Entity with ID: >> %d << has OSApplication  \n"
                "not set to WDGM_INVALID_OSAPPLICATION \n"
                , counter);
    }
    return ok;
}

bool test_047
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 47\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Every supervised entity in the generated configuration shall have \n"
      "its OSApplication set to WDGM_INVALID_OSAPPLICATION.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_047);

    printf
        ( "%ssupervised entity in GCF has its OSApplication \n"
           "set to WDGM_INVALID_OSAPPLICATION: %sOK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

static bool test85_all_vi_as_in_cfg
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i;
    bool result    = true;
    bool subresult = true;
    WdgM_CheckPointType       const  * wdgm_cp      = NULL;
    int                       const  * cp_id        = NULL;
    int                       const  * se_id        = NULL;
    WdgM_SupervisedEntityType const  * wdgm_se      = NULL;
    alive_supervision_t       const  * xsl_alsuperv = NULL;

    printf
        ( "Verifying Alive Supervision elements found in "
          "'verifier info' file\n"
        );
    printf
        ( ".............................................."
          "....................\n"
        );
    for
        ( i = 0, xsl_alsuperv = verifier_info->alive_supervision_list
        ; i < verifier_info->alive_supervision_cnt
        ; i++, xsl_alsuperv++
        )
    {
        CHECK_NULL_PTR (xsl_alsuperv, 1);
        subresult = true;

        cp_id = get_vi_cp_id(verifier_info, xsl_alsuperv->se, xsl_alsuperv->cp);
        se_id = get_vi_se_id(verifier_info, xsl_alsuperv->se);

        if (cp_id == NULL_PTR && se_id == NULL_PTR )
        {
            subresult = false;
        }
        else
        {
            wdgm_se = get_wdgm_se (main_struct, *se_id);
            wdgm_cp = get_wdgm_cp (main_struct, *se_id, *cp_id);
            if ((wdgm_se == NULL) || (wdgm_cp == NULL))
            {
                subresult = false;
            }
            else
            {
                CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
                if (  wdgm_cp->WdgMAliveGRef == NULL_PTR
                   || wdgm_cp->WdgMAliveLRef == NULL_PTR
                   )
                {
                    subresult = false;
                }
            }
        }
        printf
           ( "Alive Supervision for CP `%s/%s`: %sOK\n"
           , xsl_alsuperv->se
           , xsl_alsuperv->cp
           , subresult ? "" : "NOT "
           );
       result = result && subresult;
    }
    return result;
}

static bool test85_all_cfg_as_in_vi
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result    = true;
    bool subresult = true;

    WdgM_SupervisedEntityType const * wdgm_first_se  = get_first_se (main_struct);
    WdgM_SupervisedEntityType const * wdgm_se        = NULL;
    WdgM_CheckPointType       const * wdgm_cp        = NULL;
    alive_supervision_t       const * vi_as          = NULL;
    int i, j;

    printf
        ( "Verifying Alive Supervision elements found in generated "
          "configuration\n"
        );
    printf
        ( "........................................................"
          ".............\n"
        );
    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < main_struct->NrOfSupervisedEntities
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        for ( j = 0, wdgm_cp = wdgm_se->WdgMCheckpointRef
            ; j < wdgm_se->NrOfCheckpoints
            ; j++, wdgm_cp++
            )
        {
            subresult = true;
            if (  wdgm_cp->WdgMAliveGRef != NULL_PTR
               && wdgm_cp->WdgMAliveLRef != NULL_PTR
               )
            {
                vi_as     = get_vi_as
                    ( verifier_info
                    , wdgm_se->WdgMSupervisedEntityId
                    , wdgm_cp->WdgMCheckpointId
                    );
                if (vi_as == NULL)
                {
                    subresult = false;
                }
                printf
                   ( "Alive Supervision for CP `%d/%d`: %sOK\n"
                   , wdgm_se->WdgMSupervisedEntityId
                   , wdgm_cp->WdgMCheckpointId
                   , subresult ? "" : "NOT "
                   );
            }
            result = result && subresult;
        }
    }
    return true;
}

bool test_085
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;

    printf ("Test 85\n");
    printf ("=======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The set of relations between alive supervisions  \n"
      "and checkpoints in the EDF is the same as in GCF; i.e. each \n"
      "CP has on both sides either the same alive supervision associated \n"
      "or no alive supervision associated.\n\n"
    );

    ASSERT_ENTITIES();

    result =
        (  test85_all_vi_as_in_cfg (main_struct, verifier_info)
        && test85_all_cfg_as_in_vi (main_struct, verifier_info)
        );


    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The set of relations between alive supervisions and checkpoints \n"
          "in the EDF is %s the same as in GCF: %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_086
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j, wdgm_se_id;
    bool result    = true;
    bool subresult = false;

    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;
    WdgM_CheckPointType       const  * wdgm_cp       = NULL;
    entity_struct_t           const  * xsl_se        = NULL;
    cp_id_data_t              const  * xsl_cp        = NULL;

    printf ("Test 86\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "In GCF, for each SE: All CPs that are referenced in SE are defined \n"
      "(in array WdgMCheckPoint). (Note: This includes check for references \n"
      "by CP-ID and references by address to CP-list item - related to Error "
      "1093) \n\n"
    );

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < main_struct->NrOfSupervisedEntities
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        wdgm_se_id = wdgm_se->WdgMSupervisedEntityId;

        xsl_se    = get_vi_se (verifier_info, wdgm_se_id);
        if (xsl_se == NULL)
        {
            printf
                ( "Supervised Entity with ID %d not found in EDF\n"
                , wdgm_se_id
                );
            result = false;
            break;
        }
        else
        {
            if (wdgm_se-> NrOfCheckpoints != xsl_se->cp_cnt)
            {
                printf
                    ( "Supervised Entity with ID %d: Number of checkpoints "
                      "in the generated configuration (%d) and in the EDF "
                      "(%d) do not match\n"
                    , wdgm_se_id
                    , wdgm_se-> NrOfCheckpoints
                    , xsl_se->cp_cnt
                    );
                result = false;
                break;
            }
            else
            {
                for ( j = 0, wdgm_cp = wdgm_se->WdgMCheckpointRef
                    ; j < wdgm_se->NrOfCheckpoints
                    ; j++, wdgm_cp++
                    )
                {

                    xsl_cp = get_vi_cp
                        ( verifier_info->entity_list
                        , verifier_info->entity_cnt
                        , wdgm_se_id
                        , wdgm_cp->WdgMCheckpointId
                        );
                    if (xsl_cp == NULL)
                    {
                        printf
                            ( "Checkpoint with SE-ID/CP-ID %d/%d not found\n"
                            , wdgm_se_id
                            , wdgm_cp->WdgMCheckpointId
                            );
                        subresult = false;
                    }
                    else
                    {
                        subresult = true;
                    }
                    result = result && subresult;
                }
            }
        }
    }

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "%sEvery checkpoint in EDF has its equivalent in GCF: %sOK.\n\n"
        , result ? "" : "Not "
        , result ? "" : "Not "
        );
    return result;
}

bool test_089
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 89\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_VERSION_INFO_API in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->version_info_api == edf->version_info_api;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_VERSION_INFO_API in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_090
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 90\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_DEV_ERROR_DETECT in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->dev_error_detect == edf->dev_error_detect;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_DEV_ERROR_DETECT in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_091
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 91\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_DEM_REPORT in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->dem_report == edf->dem_report;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_DEM_REPORT in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_092
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 92\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_DEFENSIVE_BEHAVIOR in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->defensive_behavior == edf->defensive_behavior;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_DEFENSIVE_BEHAVIOR in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_093
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 93\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_IMMEDIATE_RESET in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->immediate_reset == edf->immediate_reset;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_IMMEDIATE_RESET in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_094
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 94\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_OFF_MODE_ENABLED in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->off_mode_enabled == edf->off_mode_enabled;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_OFF_MODE_ENABLED in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_095
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 95\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_USE_OS_SUSPEND_INTERRUPT in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->use_os_suspend_interrupt == edf->use_os_suspend_interrupt;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_USE_OS_SUSPEND_INTERRUPT in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_096
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 96\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_TIMEBASE_SOURCE in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->timebase_source == edf->timebase_source;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_TIMEBASE_SOURCE in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_097
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 97\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_SECOND_RESET_PATH in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->second_reset_path == edf->second_reset_path;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_SECOND_RESET_PATH in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_098
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 98\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_TICK_OVERRUN_CORRECTION in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->tick_overrun_correction == edf->tick_overrun_correction;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_TICK_OVERRUN_CORRECTION in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_099
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 99\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_ENTITY_DEACTIVATION_ENABLED in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->entity_deactivation_enabled == edf->entity_deactivation_enabled;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_ENTITY_DEACTIVATION_ENABLED in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_100
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 100\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_STATUS_REPORTING_MECHANISM in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->status_reporting_mechanism == edf->status_reporting_mechanism;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_STATUS_REPORTING_MECHANISM in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_101
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 101\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_USE_RTE in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.\n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->use_rte == edf->use_rte;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_USE_RTE in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_102
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 102\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_DEM_SUPERVISION_REPORT in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->dem_supervision_report == edf->dem_supervision_report;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_DEM_SUPERVISION_REPORT in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_103
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    wdgm_features_t const * cfg = verifier_info->cfg_features;
    wdgm_features_t const * edf = verifier_info->edf_features;

    printf ("Test 103\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_FIRSTCYCLE_ALIVECOUNTER_RESET in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    CHECK_NULL_PTR (cfg, 1);
    CHECK_NULL_PTR (edf, 2);

    result = cfg->firstcycle_alive_counter_reset == edf->firstcycle_alive_counter_reset;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_FIRSTCYCLE_ALIVECOUNTER_RESET in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_104
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = true;
    bool subresult = false;

    printf ("Test 104\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The WDGM_GLOBAL_TRANSITIONS in wdgm_cfg_features shall have the same \n"
      "value as in wdgm_verifier_info.  \n\n"
    );

    if (verifier_info->global_trans_cnt > 0)
    {
        subresult = true;
    }

    CHECK_NULL_PTR (verifier_info->cfg_features, 1);
    CHECK_NULL_PTR (verifier_info->cfg_features->computed_features, 2);
    result = verifier_info->cfg_features->computed_features->global_transitions == subresult;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_GLOBAL_TRANSITIONS in wdgm_cfg_features has %s the same value \n"
          "as in wdgm_verifier_info!  %sOK.\n\n"
        , result ? "" : "not"
        , result ? "" : "Not "
        );
    return result;
}

bool test_105
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{

    int j;
    bool result  = false;
    bool is_3_1  = false;
    bool any_3_1 = false;
    entity_struct_t const * xsl_se = NULL;
    wdgm_features_t const * cfg_features = verifier_info->cfg_features;

    printf ("Test 105\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The value WDGM_AUTOSAR_3_1_X_COMPATIBILITY in WdgM_Cfg_Features.h \n"
      "shall be STD_ON if there is at least one supervised entity with \n"
      "its attribute WdgMSupportedAutosarAPI set to the enumeration value \n"
      "API_3_1; otherwise that value shall be STD_OFF.\n\n"
    );

    CHECK_NULL_PTR (cfg_features, 1);
    CHECK_NULL_PTR (cfg_features->computed_features, 2);
    is_3_1 = cfg_features->computed_features->autosar_3_1_compatibility;

    for
        ( j = 0, xsl_se = verifier_info->entity_list
        ; j < verifier_info->entity_cnt
        ; j++, xsl_se++
        )
    {
        CHECK_NULL_PTR (xsl_se, 3);

        any_3_1 = any_3_1 || xsl_se->autosar_3_1_compatibility;
    }

    result = is_3_1 == any_3_1;

    printf ("\nResult\n");
    printf ("------\n");
    printf
        ( "The WDGM_AUTOSAR_3_1_X_COMPATIBILITY in wdgm_cfg_features is STD_ON \n"
          "only if there is at least one supervised entity configured for API 3.1: "
          "%sOK.\n\n"
        , result ? "" : "Not "
        );
    return result;
}


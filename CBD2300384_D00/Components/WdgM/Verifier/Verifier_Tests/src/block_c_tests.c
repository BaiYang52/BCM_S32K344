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
/**        \file  black_c_tests.c
 *        \brief  black_c_tests source file
 *
 *      \details  This is the implementation of all test cases of the WdgM verifier of test block 'c'
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
#include "WdgM_Types.h"
#include "utilities.h"
#include "edf_utilities.h"
#include "wdgm_verifier_types.h"

#define NR_OF_CHECKPOINTS        xslt_checkpoint_cnt(verifier_info)
#define NR_OF_ENTITIES           xslt_entity_cnt(verifier_info)

typedef enum {forTest7, forTest8} se_field_key;

static void const * field_pointer
    ( WdgM_SupervisedEntityType const * wdgm_se
    , se_field_key field_key
    )
{
    switch (field_key)
    {
        case forTest7 :
            return (void *) wdgm_se->EntityStatusLRef;
        case forTest8 :
            return (void *) wdgm_se->EntityStatusGRef;
        default :
            exit (42);
    }
}

static bool any_se_with_null_pointer
    ( WdgM_SupervisedEntityType const * wdgm_first_se
    , int se_list_length
    , char const * title
    , char const * field_name
    , se_field_key field_key
    )
{
    int  i                               = 0;
    bool is_ok                           = true;
    bool result                          = true;
    WdgM_SupervisedEntityType const * wdgm_se = wdgm_first_se;

    if (wdgm_first_se == NULL)
    {
        return false;
    }
    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < se_list_length
        ; i++, wdgm_se++
        )
    {
        CHECK_NULL_PTR (wdgm_se, 1);
        is_ok = field_pointer (wdgm_se, field_key) != NULL;
        result  = result && is_ok;
        newline ();
        printf ("%s.%d\n", title, i + 1);
        printf ("===========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "The `%s` member of every wdgm_se must not be a \n"
              "`NULL` pointer.\n\n"
            , field_name
            );

        printf
            ( "X = `%s` of entity at array offset %d = "
              "%sNULL\n\n"
            , field_name
            , i
            , is_ok ? "NOT " : ""
            );
        printf ("Result\n");
        printf ("------\n");
        printf
            ( "`%s` of entity at array offset %d is not NULL: "
              "%sOK\n"
            , field_name
            , i
            , is_ok ? "" : "NOT "
            );
    }
    newline ();
    return result;
}

bool test_006
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    int edf_entity_cnt = xslt_entity_cnt (verifier_info);
    bool is_null       = main_struct->WdgMSupervisedEntityRef == NULL;
    bool no_entities   = edf_entity_cnt == 0;
    bool result        = is_null == no_entities;
    newline ();
    printf ("Test 6\n");
    printf ("======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("The main structure's WdgMSupervisedEntityRef shall be a NULL \n"
            "pointer if and only if the number of Supervised Entities \n"
            "according to the EDF is zero.\n\n");

    printf
        ( "X = `main_struct->WdgMSupervisedEntityRef` is NULL = %s\n"
        , is_null ? "TRUE" : "FALSE"
        );
    printf
        ( "Y = Number of Supervised Entities (%d) according to EDF is zero = "
          "%s\n\n"
        , edf_entity_cnt
        , no_entities ? "TRUE" : "FALSE"
        );
    print_result_header ();
    printf
        ( "X if and only if Y : %s OK\n\n"
        , result ? "" : "NOT"
        );
    return result;
}

bool test_007 (const WdgM_ConfigType * main_struct)
{
    return any_se_with_null_pointer
        ( main_struct->WdgMSupervisedEntityRef
        , main_struct->NrOfSupervisedEntities
        , "Test 7"
        , "EntityStatusLRef"
        , forTest7
        );
}


bool test_008 (const WdgM_ConfigType * main_struct)
{

    return any_se_with_null_pointer
        ( main_struct->WdgMSupervisedEntityRef
        , main_struct->NrOfSupervisedEntities
        , "Test 8"
        , "EntityStatusGRef"
        , forTest8
        );
}

bool test_009 (const WdgM_ConfigType * main_struct)
{
    WdgM_CheckPointType const * first_cp = get_first_cp (main_struct);
    WdgM_CheckPointType const * wdgm_cp;
    int cp_list_length = countAllCPInSE (main_struct, false);
    int i;
    bool is_x_null;
    bool is_y_null;
    bool result    = true;
    bool subresult = true;

    if (first_cp == NULL)
    {
        printf ("Test 9\n");
        printf ("======\n");
        printf ("Result\n");
        printf ("------\n");
        printf ("Cannot execute test #9 (no checkpoints found)\n\n");
        return false;
    }

    for
        ( i = 0, wdgm_cp = first_cp
        ; i < cp_list_length
        ; i++, wdgm_cp++
        )
    {
        CHECK_NULL_PTR (wdgm_cp, 1);
        is_x_null = wdgm_cp->WdgMAliveLRef == NULL;
        is_y_null = wdgm_cp->WdgMAliveGRef == NULL;
        newline ();
        printf ("Test 9.%d\n", i + 1);
        printf ("=========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "The `WdgMAliveLRef` member of each Checkpoint shall be NULL if\n"
              "and only if the member `WdgMAliveGRef` in the same checkpoint\n"
              "is NULL\n\n"
            );
        printf
            ( "X = `WdgMAliveLRef` of checkpoint at array offset %d = %sNULL\n"
            , i
            , is_x_null ? "" : "NOT "
            );
        printf
            ( "Y = `WdgMAliveGRef` of checkpoint at array offset "
              "%d = %sNULL\n\n"
            , i
            , is_y_null ? "" : "NOT "
            );
        printf ("Result\n");
        printf ("------\n");
        subresult = is_x_null == is_y_null;
        result    = result && subresult;
        printf
            ( "X is NULL if and only if Y is NULL: %sOK\n"
            , subresult ? "" : "NOT "
            );
    }
    newline ();
    return result;
}

bool test_010 (const WdgM_ConfigType * main_struct)
{
    bool is_null = main_struct->DataGSRef == NULL;
    newline ();
    printf ("Test 10\n");
    printf ("=======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The main WdgM_ConfigType structure shall have its `DataGSRef`\n"
          "member set to a non-NULL pointer.\n\n"
        );
    printf ( "X = `DataGSRef` member in the main structure = %sNULL\n\n"
           , is_null ? "" : "NOT "
           );
    printf ("Result\n");
    printf ("------\n");
    printf ("X is not NULL: %s OK\n\n", is_null ? "NOT " : "");
    return !is_null;
}

bool test_011 (const WdgM_ConfigType * main_struct)
{
    bool is_null = main_struct->DataGRef == NULL;
    newline ();
    printf ("Test 11\n");
    printf ("=======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The main WdgM_ConfigType structure shall have its `DataGRef`\n"
          "member set to a non-NULL pointer.\n\n"
        );
    printf ( "X = `DataGRef` member in the main structure = %sNULL\n\n"
           , is_null ? "" : "NOT "
           );
    printf ("Result\n");
    printf ("------\n");
    printf ("X is not NULL: %s OK\n\n", is_null ? "NOT " : "");
    return !is_null;
}

bool test_012 (const WdgM_ConfigType * main_struct)
{
    bool is_null = main_struct->EntityGSRef == NULL;
    newline ();
    printf ("Test 12\n");
    printf ("=======\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The main WdgM_ConfigType structure shall have its `EntityGSRef`\n"
          "member set to a non-NULL pointer.\n\n"
        );
    printf ( "X = `EntityGSRef` member in the main structure = %sNULL\n\n"
           , is_null ? "" : "NOT "
           );
    print_result_header ();
    printf ("X is not NULL: %s OK\n\n", is_null ? "NOT " : "");
    return !is_null;
}

bool test_013
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int edf_global_trans_cnt                =
            xslt_global_trans_cnt (verifier_info);
    bool no_glob_trans                      = edf_global_trans_cnt == 0;
    bool is_null                            =
        main_struct->GlobalTransitionFlagsGSRef == NULL;
    bool result                             = is_null == no_glob_trans;
    newline ();
    printf ("Test 13\n");
    printf ("-------\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ( "The main WdgM_ConfigType structure shall have its \n"
          "`GlobalTransitionFlagsGSRef` member set to NULL if and only if \n"
          "there are no global transitions.\n\n"
        );
    printf
        ( "X = main structure's `GlobalTransitionFlagsGSRef` is NULL = %s\n"
        , is_null ? "TRUE" : "FALSE"
        );
    printf
        ( "Y = Number of global transitions (%d) according to EDF is zero = "
          "%s\n\n"
        , edf_global_trans_cnt
        , no_glob_trans ? "TRUE" : "FALSE"
        );
    print_result_header ();
    printf
        ( "X if and only if Y : %s OK\n\n"
        , result ? "" : "NOT"
        );
    return result;
}

bool test_014
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    WdgM_GlobalTransitionType const * first_global_trans =
        getFirstGlobalTransition (main_struct, verifier_info);
    WdgM_GlobalTransitionType const * trans              = NULL;

    bool result    = true;
    bool subresult = true;
    int i          = 0;
    int x          = 0;
    int y          = 0;
    int edf_global_trans_cnt = xslt_global_trans_cnt (verifier_info);

    newline ();
    if (first_global_trans == NULL)
    {
        printf ("Test 14\n");
        printf ("=======\n");
        printf ("Result\n");
        printf ("------\n");
        printf
            ("No global transitions found - this test does not apply\n");
        newline ();
        return true;
    }
    for
        ( i = 0, trans = first_global_trans
        ; i < edf_global_trans_cnt
        ; i++, trans++
        )
    {
        CHECK_NULL_PTR (trans, 1);
        x = i;
        y = trans->GlobalTransitionFlagId;
        printf ("Test 14.%d\n", i + 1);
        printf ("==========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "The value of WdgM_GlobalTransitionType->GlobalTransitionFlagId\n"
              "must match the current element's position in the\n"
              "WdgM_GlobalTransitionType array.\n\n"
            );
        printf ("X = current array element's index = %d\n", x);
        printf
            ( "Y = current array element's GlobalTransitionFlagId value = "
              "%d\n\n"
            , y
            );

        printf ("Result\n");
        printf ("------\n");
        subresult = x == y;
        result    = result && subresult;
        printf ("X = Y : %sOK\n", subresult ? "" : "NOT ");
        newline ();
    }
    return result;
}

bool test_015
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j;
    bool ok = true;

    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;
    WdgM_SupervisedEntityType const  * wdgm_c_se     = NULL;


    printf ("Test 015\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Each supervised entity's EntityStatusLRef member shall point to \n"
      "a unique variable.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < NR_OF_ENTITIES
        ; i++, wdgm_se++
        )
        {
            CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
            CHECK_NULL_PTR (wdgm_se, 1);
            if (wdgm_se->EntityStatusLRef != NULL_PTR)
            {
                for
                    ( j = 0, wdgm_c_se = wdgm_first_se
                    ; j < i
                    ; j++, wdgm_c_se++
                    )
                {
                    CHECK_NULL_PTR (wdgm_c_se, 2);
                    if (wdgm_c_se->EntityStatusLRef == wdgm_se->EntityStatusLRef)
                    {
                        ok = false;
                        printf ("The supervised entity IDs: >> %d << and >> %d << points to the same EntityStatusLRef!\n",
                                wdgm_c_se->WdgMSupervisedEntityId, wdgm_se->WdgMSupervisedEntityId);
                    }
                }
            }
        }

    printf
        ( "%ssupervised entity has its `EntityStatusLRef` member pointing\n"
          "to a unique variable: %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_016
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j;
    bool ok = true;

    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;
    WdgM_SupervisedEntityType const  * wdgm_c_se     = NULL;

    printf ("Test 016\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Each supervised entity's EntityStatusGRef member shall point to \n"
      "a unique variable.\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < NR_OF_ENTITIES
        ; i++, wdgm_se++
        )
        {
            CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
            CHECK_NULL_PTR (wdgm_se, 1);
            if (wdgm_se->EntityStatusGRef != NULL_PTR)
            {
                for
                    ( j = 0, wdgm_c_se = wdgm_first_se
                    ; j < i
                    ; j++, wdgm_c_se++
                    )
                {
                    CHECK_NULL_PTR (wdgm_c_se, 2);
                    if (wdgm_c_se->EntityStatusGRef == wdgm_se->EntityStatusGRef)
                    {
                        ok = false;
                        printf ("The supervised entity IDs: >> %d << and >> %d << points to the same EntityStatusGRef!\n",
                                wdgm_c_se->WdgMSupervisedEntityId, wdgm_se->WdgMSupervisedEntityId);
                    }
                }
            }
        }

    printf
        ( "%ssupervised entity has its `EntityStatusGRef` member pointing\n"
          "to a unique variable: %sOK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}


bool test_068
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j;
    bool result    = true;
    bool subresult = true;
    bool wdgm_checkpoint_is_null;

    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;
    WdgM_CheckPointType       const  * WdgM_first_cp = NULL;
    WdgM_CheckPointType       const  * wdgm_cp       = NULL;

    printf ("Result\n");
    printf ("------\n");
    if (wdgm_first_se == NULL)
    {
        printf ("Test 68\n");
        printf ("======\n");
        printf ("Result\n");
        printf ("------\n");
        printf ("No supervised entities found: NOT OK\n");
        newline ();
        return false;
    }
    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < xslt_entity_cnt (verifier_info)
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        CHECK_NULL_PTR (wdgm_se, 1);
        WdgM_first_cp =  wdgm_se->WdgMCheckpointRef;
        for
           ( j = 0, wdgm_cp = WdgM_first_cp
           ; j < wdgm_se->NrOfCheckpoints
           ; j++, wdgm_cp++
           )
        {
           CHECK_NULL_PTR (wdgm_cp, 2);
           wdgm_checkpoint_is_null = wdgm_cp->WdgMCheckpointId != j;
           newline ();
           printf ("Test 68.%d.%d\n", i + 1, j + 1);
           printf ("=========\n");
           printf ("Description\n");
           printf ("-----------\n");
           printf
           ( "The checkpoints belonging to each supervised entity \n"
             "shall have IDs building a zero-based list of increasing \n"
             "integers without a gap \n\n"
           );
           printf
           ( "Test for supervised entity at array offset %d \n"
             "and checkpoint at array offset %d: %sOK \n\n"
           , i
           , j
           , !wdgm_checkpoint_is_null ? "" : "NOT "
           );
           printf ("Result\n");
           printf ("------\n");
           subresult = !wdgm_checkpoint_is_null;
           result    = result && subresult;
           printf
           ( "The checkpoints belonging to the supervised entity \n"
             "have IDs building a zero-based list of increasing \n"
             "integers without a gap: %sOK.\n\n"
           , subresult ? ""  : "Not "
           );
        }
    }
    newline ();
    return result;
}

bool test_069
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i;
    bool result    = true;
    bool subresult = true;
    bool wdgm_no_checkpoint;
    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;

    if (wdgm_first_se == NULL)
    {
        printf ("Test 69\n");
        printf ("======\n");
        printf ("Result\n");
        printf ("------\n");
        printf ("No supervised entities found: NOT OK\n");
        newline ();
        return false;
    }

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < NR_OF_ENTITIES
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        CHECK_NULL_PTR (wdgm_se, 1);
        wdgm_no_checkpoint = (wdgm_se->NrOfCheckpoints <= 0);
        printf ("Test 69.%d\n", i + 1);
        printf ("=========\n");
        printf ("Description\n");
        printf ("-----------\n");
        printf
            ( "Each supervised entity shall have at least one checkpoint \n\n"
            );
        printf
            ( "Supervised entity at array offset %d has at \n"
                "least one checkpoint %sOK. \n\n"
            , i
            , !wdgm_no_checkpoint ? "" : "NOT "
            );
        printf ("Result\n");
        printf ("------\n");
        subresult = !wdgm_no_checkpoint;
        result    = result && subresult;
        printf
            ( "Supervised entity has at least one \n"
              "checkpoint : %sOK.\n\n"
            , subresult ? ""  : "Not "
        );
    }
    newline ();
    return result;
}

static void _test_70_print_info
    ( bool local_found
    , bool global_found
    , char const * src_se
    , char const * src_cp
    , char const * dst_se
    , char const * dst_cp
    )
{
    if (local_found)
    {
        printf
            ( "Local transition found for DL Supervision %s/%s -> %s/%s\n"
            , src_se, src_cp, dst_se, dst_cp
            );
    }
    else
    {
        if (global_found)
        {
            printf
                ( "Global transition found for DL Supervision %s/%s -> %s/%s\n"
                , src_se, src_cp, dst_se, dst_cp);
        }
        else
        {
            printf
                ( "No transition found for DL Supervision %s/%s -> %s/%s\n"
                , src_se, src_cp, dst_se, dst_cp
                );
        }
    }
}

static bool _test_070_find_local_transition
    ( WdgM_CheckPointType  const  * wdgm_cp
    , int vi_dl_src_se_id
    , int vi_dl_src_cp_id
    )
{
    bool result = false;
    int WdgM_DeadlineMin;
    int WdgM_DeadlineMax;
    int r;
    WdgM_TransitionType const * wdgm_lt = NULL;

    if (wdgm_cp->WdgMLocalTransitionRef != NULL_PTR)
    {
        for
            ( r = 0 , wdgm_lt = wdgm_cp->WdgMLocalTransitionRef
            ; r < wdgm_cp->NrOfLocalTransitions
            ; r++, wdgm_lt++
            )
        {
            CHECK_NULL_PTR (wdgm_lt, 2);

            if (  (wdgm_lt->EntitySourceId     == vi_dl_src_se_id)
               && (wdgm_lt->CheckpointSourceId == vi_dl_src_cp_id)
               )
            {
                WdgM_DeadlineMin = wdgm_lt->WdgMDeadlineMin;
                WdgM_DeadlineMax = wdgm_lt->WdgMDeadlineMax;

                if ( !(  (WdgM_DeadlineMin == 0)
                      && (WdgM_DeadlineMax == 0)
                      )
                   )
                {
                    result = true;
                }
            }
        }
    }
    return result;
}

static bool _test_070_find_global_transition
    ( WdgM_CheckPointType  const  * wdgm_cp
    , int vi_dl_src_se_id
    , int vi_dl_src_cp_id
    )
{
    bool result = false;
    int m;
    int WdgM_DeadlineMin;
    int WdgM_DeadlineMax;
    WdgM_GlobalTransitionType const  * wdgm_gt = NULL;

    if (wdgm_cp->WdgMGlobalTransitionsRef != NULL_PTR)
    {
        for
            ( m = 0, wdgm_gt = wdgm_cp->WdgMGlobalTransitionsRef
            ; m < wdgm_cp->NrOfGlobalTransitions
            ; m++, wdgm_gt++
            )
        {
            CHECK_NULL_PTR (wdgm_gt, 3);

            if (  (wdgm_gt->EntitySourceId     == vi_dl_src_se_id)
               && (wdgm_gt->CheckpointSourceId == vi_dl_src_cp_id)
               )
            {

                WdgM_DeadlineMin = wdgm_gt->WdgMDeadlineMin;
                WdgM_DeadlineMax = wdgm_gt->WdgMDeadlineMax;

                if ( !(  (WdgM_DeadlineMin == 0)
                      && (WdgM_DeadlineMax == 0)
                      )
                   )
                {
                    result = true;
                }
            }
        }
    }
    return result;
}

bool test_070
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j, k;
    bool ok           = true;
    bool local_found  = false;
    bool global_found = false;
    bool found_se     = false;

    WdgM_SupervisedEntityType const  * wdgm_first_se      = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se            = NULL;
    WdgM_CheckPointType       const  * wdgm_cp            = NULL;
    dl_supervision_t          const  * vi_dl              = NULL;
    int                       const  * vi_dl_src_se_id    = NULL;
    int                       const  * vi_dl_src_cp_id    = NULL;
    int                       const  * vi_dl_dst_se_id    = NULL;
    int                       const  * vi_dl_dst_cp_id    = NULL;

    printf ("Test 70\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "There shall be either a global transition or a local \n"
      "transition for every WdgMDeadlineSupervision element  \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    for
        ( i = 0, vi_dl = verifier_info->dl_supervision_list
        ; i < verifier_info->dl_supervision_cnt
        ; i++, vi_dl++
        )
    {
        local_found  = false;
        global_found = false;
        found_se     = false;

        // get the DL Supervision's source & destination data
        vi_dl_src_se_id = get_vi_se_id (verifier_info, vi_dl->source_se);
        vi_dl_src_cp_id = get_vi_cp_id (verifier_info, vi_dl->source_se, vi_dl->source_cp);
        vi_dl_dst_se_id = get_vi_se_id (verifier_info, vi_dl->dest_se);
        vi_dl_dst_cp_id = get_vi_cp_id (verifier_info, vi_dl->dest_se, vi_dl->dest_cp);

        if ( (vi_dl_src_se_id == NULL) || (vi_dl_src_cp_id == NULL)
             || (vi_dl_dst_se_id == NULL) || (vi_dl_dst_cp_id == NULL) )
        {
            return false;
        }

        for
            ( k = 0, wdgm_se = wdgm_first_se
            ; k < main_struct->NrOfSupervisedEntities
            ; k++, wdgm_se++
            )
        {
            CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
            CHECK_NULL_PTR (wdgm_se, 1);

            if (wdgm_se->WdgMSupervisedEntityId == *vi_dl_dst_se_id)
            {
                found_se = true;

                for
                ( j = 0, wdgm_cp = wdgm_se->WdgMCheckpointRef
                ; j < wdgm_se->NrOfCheckpoints
                ; j++, wdgm_cp++
                )
                {
                    if (wdgm_cp->WdgMCheckpointId != *vi_dl_dst_cp_id)
                    {
                        continue;
                    }
                    local_found = _test_070_find_local_transition
                        (wdgm_cp, *vi_dl_src_se_id, *vi_dl_src_cp_id);

                    if (!local_found)
                    {
                        global_found = _test_070_find_global_transition
                            (wdgm_cp, *vi_dl_src_se_id, *vi_dl_src_cp_id);
                    }
                }
            }
        }
        if (found_se)
        {
            _test_70_print_info
                ( local_found
                , global_found
                , vi_dl->source_se
                , vi_dl->source_cp
                , vi_dl->dest_se
                , vi_dl->dest_cp
                );
        }
        ok = ok && (local_found || global_found || !found_se);
    }
    printf
        ( "%sdeadline Supervision element has either a global \n"
          "transition or a local transition! %sOK\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_071
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i, j;
    bool flag = false;
    bool ok   = true;

    WdgM_SupervisedEntityType   const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType   const  * wdgm_se       = NULL;
    WdgM_SupervisedEntityIdType        * xsl_p         = NULL;

    printf ("Test 71\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf ("Each supervised entity's ID shall be unique. \n\n");
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    for
        ( i = 0,  xsl_p = verifier_info->entity_id_array
        ; i < xslt_entity_cnt (verifier_info)
        ; i++, xsl_p++
        )
    {
        CHECK_NULL_PTR (xsl_p, 1);
        *xsl_p = -1;
    }

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < xslt_entity_cnt (verifier_info)
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        CHECK_NULL_PTR (wdgm_se, 2);
        if (flag)
        {
            break;
        }

        for
            ( j = 0, xsl_p = verifier_info->entity_id_array
            ; j < i
            ; j++, xsl_p++
            )
        {
            CHECK_NULL_PTR (xsl_p, 3);
            if ((*xsl_p) == wdgm_se->WdgMSupervisedEntityId)
            {
                ok = false;
                flag = true;
                break;
            }
        }

        *(xsl_p) = wdgm_se->WdgMSupervisedEntityId;
    }

    printf
        ( "%ssupervised entity has a unique ID:\n"
          "%sOK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

static bool _condition_072 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    bool ok = true;

    if (wdgm_se->WdgMCheckpointRef == NULL_PTR)
    {
        ok = false;
        printf("The Supervised Entity with ID: >> %d << has no initial checkpoint!\n", counter);
    }
    return ok;
}

bool test_072
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 72\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Every supervised entity shall have an initial checkpoint \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_072);

    printf
        ( "%ssupervised entity has an intial checkpoint %s OK.\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_073
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = false;
    static bool overAllConfigsNotificationsUsed = false;
    int i;
    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;

    printf ("Test 73\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "There shall be at least one callback function \n"
      "defined for the supervised entities or for the \n"
      "main structure if the flag WdgmStatusReportingMechanism \n"
      "is set to WDGM_USE_NOTIFICATIONS or WDGM_USE_MODE_SWITCH_PORTS\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    if(verifier_info->edf_features->status_reporting_mechanism != WDGM_USE_NO_STATUS_REPORTING)
    {

      if (overAllConfigsNotificationsUsed == false)
      {
        for
            ( i = 0, wdgm_se = wdgm_first_se
            ; i < xslt_entity_cnt (verifier_info)
            ; i++, wdgm_se++
            )
        {
            CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
            CHECK_NULL_PTR (wdgm_se, 1);

            if ( wdgm_se->WdgM_LocalStatusChangeNotification != NULL_PTR
               || main_struct->WdgM_GlobalStatusChangeNotification != NULL_PTR )
            {
                ok   = true;
                overAllConfigsNotificationsUsed = true;
                break;
            }
        }
      }
      else
      {
        ok   = true;
      }
        printf
           ( "There is %s callback function defined and \n"
             "WDGM_STATUS_REPORTING_MECHANISM is set\n"
             "to WDGM_USE_NOTIFICATIONS: %sOK \n\n"
             , ok ? "a"  : "no"
             , ok ? ""   : "Not "
              );
    }
    else
    {
      ok = true;
      printf
          ( "WDGM_STATUS_REPORTING_MECHANISM is set to WDGM_USE_NO_STATUS_REPORTING: %sOK \n\n"
             , ok ? ""       : "Not "
          );
    }


    return ok;
}


bool test_074
    ( const WdgM_ConfigType        * main_struct
    )
{
    bool ok = true;

    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);

    printf ("Test 74\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The number of supervised entities shall not be zero \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    CHECK_NULL_PTR (wdgm_first_se, 1);

    if (wdgm_first_se == NULL || main_struct->NrOfSupervisedEntities <= 0)
    {
        ok = false;
    }

    printf
        ( "There is %s supervised entity %s OK.\n\n"
        , ok ? "a"  : "no"
        , ok ? ""   : "Not "
        );
    return ok;
}

static bool _condition_075 (const verifier_main_struct_t * verifier_info, WdgM_SupervisedEntityType const  * wdgm_se, int counter)
{
    void  (*f1) (WdgM_LocalStatusType local_status);

    entity_struct_t const * xsl_entity;
    bool ok = true;

    xsl_entity = get_vi_se (verifier_info,wdgm_se->WdgMSupervisedEntityId);
    CHECK_NULL_PTR (xsl_entity, 1);

    if(verifier_info->edf_features->status_reporting_mechanism != WDGM_USE_NO_STATUS_REPORTING)
    {
      f1 = xsl_entity->WdgM_LocalStatusChangeNotification;

      if (wdgm_se->WdgM_LocalStatusChangeNotification != f1 )
      {
          ok = false;
      }
    }
    else
    {
      ok = true;
    }


    if (!ok)
    {
        printf("Problem at supervised entity: >> %d << !\n", wdgm_se->WdgMSupervisedEntityId );
    }

    return ok;
}

bool test_075
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    printf ("Test 75\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
        ("Each supervised entity's 'WdgM_LocalStatusChangeNotification'\n"
         "member shall point to the callback/Rte function defined for \n"
         "that entity; otherwise it shall be NULL_PTR\n\n"
        );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_se_list (main_struct, verifier_info, _condition_075);

    printf
        ( "%ssupervised entity's 'WdgM_LocalStatusChangeNotification' \n"
          "member points to the callback function defined for \n"
          "that entity or it is NULL_PTR :\n"
          "%sOK \n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_076
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    wdgm_mode_t const * mode_info =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 76\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The main structure's WdgM_GlobalStatusChangeNotification member shall\n"
      "be NULL_PTR only if no callback function has been configured for \n"
      "signaling a global state change!\n\n"
    );

    if (mode_info == NULL)
    {
        printf
            ( "Error: WdgMMode data not found in `wdgm_verifier_info.c`\n"
            );
        ok = false;
    }
    else
    {
      if(verifier_info->edf_features->status_reporting_mechanism != WDGM_USE_NO_STATUS_REPORTING)
      {
        if (mode_info->WdgM_GlobalStatusChangeNotification == NULL)
        {
            ok = main_struct->WdgM_GlobalStatusChangeNotification == NULL;
            if (!ok)
            {
                printf
                    ( "Global State Change Callback function for WdgMMode "
                      "for core %d should be NULL but it is not!\n"
                    , mode_info->core_id
                    );
            }
        }
        else
        {
            ok = main_struct->WdgM_GlobalStatusChangeNotification != NULL;
        }
      }
      else
      {
        ok = true;
      }
    }
    print_result (ok);
    return ok;
}

bool test_077
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;
    int i,j;
    WdgM_SupervisedEntityType const  * wdgm_first_se   = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se         = NULL;
    WdgM_SupervisedEntityType const  * wdgm_c_se       = NULL;

    printf ("Test 77\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The callback functions assigned to supervised  \n"
      "entities shall have a unique name \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();
    if(verifier_info->edf_features->status_reporting_mechanism == WDGM_USE_NOTIFICATIONS)
    {
      for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < NR_OF_ENTITIES
        ; i++, wdgm_se++
        )
      {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        CHECK_NULL_PTR (wdgm_se, 1);

        if (wdgm_se->WdgM_LocalStatusChangeNotification != NULL_PTR)
        {
          for
              ( j = 0, wdgm_c_se = wdgm_first_se
              ; j < i
              ; j++, wdgm_c_se++
              )
          {
            CHECK_NULL_PTR (wdgm_c_se, 2);

            if ( wdgm_se->WdgM_LocalStatusChangeNotification == wdgm_c_se->WdgM_LocalStatusChangeNotification)
            {
               ok = false;
               printf("Problem at supervised entity: >> %d << !\n", wdgm_se->WdgMSupervisedEntityId );
               break;
             }
          }

          if (!ok)
          {
            break;
          }
        }
      }

      printf
          ( "The callback function names assigned \n"
            "to supervised entities are %s: %sOK.\n\n"
          , ok ? "unique" : "not unique"
          , ok ? ""       : "Not "
          );
    }
    else
    {
      printf( "WDGM_STATUS_REPORTING_MECHANISM is configured with WDGM_USE_NO_STATUS_REPORTING!\n\n");
    }

    return ok;
}

static bool _condition_078
    ( verifier_main_struct_t    const * verifier_info
    , WdgM_SupervisedEntityType const * wdgm_se
    , WdgM_CheckPointType       const * wdgm_cp
    )
{
    int k;
    bool ok = true;

    local_transition_t const * t = NULL;
    char const   * wdgm_se_name  = NULL;
    char const   * wdgm_cp_name  = NULL;
    char const   * xsl_se_name   = NULL;
    char const   * xsl_cp_name   = NULL;

    if (wdgm_cp->WdgMIsEndCheckpoint)
    {
        for
            ( k = 0, t = verifier_info->local_transitions
            ; k < verifier_info->local_trans_cnt
            ; k++, t++
            )
        {
            CHECK_NULL_PTR (t, 1);
            wdgm_se_name = get_vi_se_name
                (verifier_info, wdgm_se->WdgMSupervisedEntityId);
            wdgm_cp_name = get_vi_cp_name
                ( verifier_info
                , wdgm_se->WdgMSupervisedEntityId
                , wdgm_cp->WdgMCheckpointId
                );
            xsl_se_name = t->source_se;
            xsl_cp_name = t->source_cp;

            if (  equals (wdgm_se_name, xsl_se_name)
               && equals (wdgm_cp_name, xsl_cp_name)
               )
            {
                ok = false;
            }
        }
        if (!ok)
        {
            printf
            ( "Checkpoint %s/%s has outgoing local transition(s)!\n"
            , wdgm_se_name, wdgm_cp_name
            );
        }
    }

    return ok;
}

bool test_078
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = false;
    printf ("Test 78\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Checkpoints defined as local end checkpoints shall have no \n"
    "outgoing local transitions!\n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_cp_list (main_struct, verifier_info, _condition_078);

    if (ok)
    {
        printf
            ( "OK: No checkpoints defined as local checkpoints have outgoing "
              "local transitions\n\n"
            );
    }
    else
    {
        printf
            ( "NOT OK: Some checkpoint(s) defined as local end checkpoints "
              "have outgoing local transitions\n\n"
            );
    }
    return ok;
}

static bool _condition_079
      (const verifier_main_struct_t      * verifier_info
      , WdgM_SupervisedEntityType const  * wdgm_se
      , WdgM_CheckPointType const        * wdgm_cp
      )
{
    bool ok = true;
    if ((wdgm_se->WdgMCheckpointLocInitialId) == (wdgm_cp->WdgMCheckpointId))
    {
        if (wdgm_cp->NrOfLocalTransitions > 0)
        {
            ok = false;
        }
    }

    if (!ok)
    {
        printf("The checkpoint with the ID: >> SE:%d CP:%d << has an incoming local transition!\n", wdgm_se->WdgMSupervisedEntityId, wdgm_cp->WdgMCheckpointId );
    }
    return ok;
}

bool test_079
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{

    bool ok = true;

    printf ("Test 79\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "Checkpoints defined as local initial checkpoints shall \n"
      "have no incoming local transitions \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ok = apply_to_edf_cp_list(main_struct,verifier_info,_condition_079);

    printf
        ( "%sCheckpoint which is defined as local initial \n"
            "checkpoint has no incoming local transition : %sOK\n\n"
        , ok ? "Every " : "Not every "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_080
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int  i  = 0;
    bool ok = true;
    WdgM_SupervisedEntityType const  * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se       = NULL;

    printf ("Test 80\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The supervised entity IDs shall build a zero-based list \n"
      "of increasing integers without a gap! \n\n"
    );
    printf ("Result\n");
    printf ("------\n");

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < NR_OF_ENTITIES
        ; i++, wdgm_se++
        )
        {
            ok = ok && (wdgm_se->WdgMSupervisedEntityId == i);
        }

    printf
        ( "The supervised entity IDs build %s a zero-based list without a gap! %sOK.\n\n"
        , ok ? ""       : "not "
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_081
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i,j;
    bool result = true;
    bool subresult;

    WdgM_SupervisedEntityType const  * wdgm_first_se   = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * wdgm_se         = NULL;
    WdgM_CheckPointType       const  * wdgm_cp         = NULL;

    printf ("Test 81\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ("If a supervised entity's WdgMFailedSupervisionRefCycleTol is set  \n"
     "to greater than zero then there shall be an alive supervision counter \n"
     "associated to one of the entity's checkpoints. \n\n"
    );

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < xslt_entity_cnt(verifier_info)
        ; i++, wdgm_se++
        )
    {
        CONTINUE_IF_IRRELEVANT_SE (wdgm_se);
        subresult = false;
        CHECK_NULL_PTR(wdgm_se, 1);
        ASSERT_CHECKPOINTS(wdgm_se);

        if (wdgm_se->WdgMFailedSupervisionRefCycleTol > 0 )
        {
            for
                ( j = 0, wdgm_cp = wdgm_se->WdgMCheckpointRef
                ; j < wdgm_se->NrOfCheckpoints
                ; j++, wdgm_cp++
                )
            {
                CHECK_NULL_PTR(wdgm_cp, 2);
                if ( wdgm_cp != NULL_PTR)
                {
                    if (wdgm_cp->WdgMAliveGRef    != NULL_PTR
                       && wdgm_cp->WdgMAliveLRef  != NULL_PTR)
                    {
                       subresult = true;
                    }
                }
                else
                {
                    subresult = true;
                }
            }

            result = result && subresult;
            printf
               ("Test for supervised entity with ID << %d >> : %sOK \n"
               , wdgm_se->WdgMSupervisedEntityId
               , subresult ? "" : "Not "
               );
        }
        else
        {
            printf
               ("Test for supervised entity with ID << %d >> : OK \n"
               , wdgm_se->WdgMSupervisedEntityId
               );
        }
    }

    newline();
    printf ("Result\n");
    printf ("------\n");
    printf
        ( "If a supervised entity's WdgMFailedSupervisionRefCycleTol is set \n"
          "to greater than zero than there are is %s alive supervision counter\n"
          "associated to one of the entity's checkpoints: %sOK \n\n"
        , result ? "" : "no"
        , result ? "" : "Not "
        );

    return result;
}

bool test_083
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool result = false;

    printf ("Test 83\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The STD_OFF and STD_ON constants shall be defined as \n"
      "zero (0) and one (1), resp.  \n\n"
    );

    result = (STD_ON == 1) && (STD_OFF == 0);
    printf ("Result\n");
    printf ("------\n");
    printf
        ( "STD_ON = %d and STD_OFF = %d : %sOK\n\n"
        , STD_ON
        , STD_OFF
        , result ? ""       : "Not "
        );
    return result;
}

bool test_084
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    bool ok = true;

    wdgm_mode_t const * mode =
        get_vi_wdgm_mode (verifier_info, main_struct->WdgMModeCoreAssignment);

    printf ("Test 84\n");
    printf ("========\n");
    printf ("Description\n");
    printf ("-----------\n");
    printf
    ( "The value for WdgMTicksPerSecond shall be greater than zero. \n\n"
    );
    printf ("Result\n");
    printf ("------\n");
    CHECK_NULL_PTR (mode, 1);

    if ( !(mode->ticks_per_second > 0))
    {
        ok = false;
    }

    printf
        ( "The value for WdgMTicksPerSecond is %s greater than zero. %s OK.\n\n"
        , ok ? ""       : "not"
        , ok ? ""       : "Not "
        );
    return ok;
}

bool test_111 (verifier_main_struct_t const * verifier_info)
{
    bool ok = true;
    os_application_t const * p;
    os_application_t const * q;
    int i, j;
    int unconventional_loop_limit = verifier_info->os_application_cnt - 1;


    printf ("Test 111\n");
    printf ("========\n");
    printf ("Description\n");
    printf
        ( "The OS applications stored in the ECU Description File shall have "
          "unique names.\n\n"
        );


    for
        ( i = 0, p = verifier_info->os_application_array
        ; i < unconventional_loop_limit
        ; i++, p++
        )
        {
            q = p + 1;
            for
                ( j = i + 1
                ; j < verifier_info->os_application_cnt
                ; j++, q++
                )
            {
                if (equals (p->name, q->name))
                {
                    ok = false;
                }
            }
        }

    printf ("Result\n");
    printf ("------\n");
    printf ("The application names are %sunique\n", ok ? "" : "not ");


    return ok;
}

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
/**        \file  utilities.c
 *        \brief  utilities.c source file
 *
 *      \details  This is the implementation of all utilities of the WdgM verifier
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
#include <limits.h>
#include <string.h>

#include "utilities.h"
#include "edf_utilities.h"

/* couldn't find `LINE_MAX` in my MS-Windows environment */
#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

void newline ()
{
    printf ("\n");
}

/* return a pointer to the first supervised entity or NULL */
WdgM_SupervisedEntityType const * get_first_se
    (const WdgM_ConfigType * main_struct)
{
    return main_struct->WdgMSupervisedEntityRef;
}

/* return a pointer to the first checkpoint or NULL */
WdgM_CheckPointType const * get_first_cp
    (const WdgM_ConfigType * main_struct)
{
    WdgM_CheckPointType       const * result   = NULL;
    WdgM_SupervisedEntityType const * first_se =
        get_first_se (main_struct);

    if (first_se != NULL)
    {
        result = first_se->WdgMCheckpointRef;
    }
    return result;
}

void print_result_header (void)
{
    printf ("Result\n");
    printf ("------\n");
}

/*
 * return the core ID where the OS Application with the given `app_name` is
 * running.
 *
 * CAUTION: You should NOT call this function if there are no OS Applications
 * at all!
 */
int get_core_id_for_os_app
    ( char const * app_name
    , const verifier_main_struct_t * verifier_info
    )
{
    int i                      = 0;
    os_application_t const * p = NULL;

    for
        ( i = 0, p = verifier_info->os_application_array
        ; i < verifier_info->os_application_cnt
        ; i++, p++
        )
    {
        if (equals (p->name, app_name))
        {
            return p->OsApplicationCoreAssignment;
        }
    }

    return -1;
}
/*
 * return a pointer to the Supervised Entity in the verifier info file having
 * the given ID
 * */
entity_struct_t const * get_vi_entity_for_id
    ( WdgM_SupervisedEntityIdType id
    , const verifier_main_struct_t * verifier_info
    , bool verbose
    )
{
    int i;
    for (i = 0; i < verifier_info->entity_cnt; i++)
    {
        if (verifier_info->entity_list[i].id == id)
        {
            if (verbose)
            {
                printf ("Found %s (id=%u)\n"
                       , verifier_info->entity_list[i].name
                       , verifier_info->entity_list[i].id
                       );
            }

            return &verifier_info->entity_list[i];
        }
    }
    return NULL;
}

/*
 * return a pointer to the Supervised Entity in the verifier info file having
 * the given name
 * */
entity_struct_t const * get_vi_entity_for_name
    ( char const * name
    , const verifier_main_struct_t * verifier_info
    )
{
    int i;
    for (i = 0; i < verifier_info->entity_cnt; i++)
    {
        if (equals (verifier_info->entity_list[i].name, name))
        {
            return &verifier_info->entity_list[i];
        }
    }
    return NULL;
}

/* return true if the SE has a correct reference to its chekpoints */
static bool st1
    (  verifier_main_struct_t    const  * verifier_info
     , WdgM_SupervisedEntityType const  * wdgm_se
     , int counter
     )
{
    return (  (wdgm_se->NrOfCheckpoints > 0)
           && (wdgm_se->WdgMCheckpointRef != NULL)
           );
}

/* return true if the checkpoint reference to its global transitions
 * is consistent; e.g., if it has no global transitions then the
 * corresponding pointer is NULL
 */
static bool st2
    ( verifier_main_struct_t    const * verifier_info
    , WdgM_SupervisedEntityType const * wdgm_se
    , WdgM_CheckPointType       const * wdgm_cp
    )
{
    if (  (wdgm_cp->NrOfGlobalTransitions > 0)
       && (wdgm_cp->WdgMGlobalTransitionsRef != NULL)
       )
    {
        return true;
    }
    else if (  (wdgm_cp->NrOfGlobalTransitions == 0)
            && (wdgm_cp->WdgMGlobalTransitionsRef == NULL)
            )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* return true if the checkpoint reference to its local transitions
 * is consistent; e.g., if it has no local transitions then the
 * corresponding pointer is NULL
 */
static bool st3
    ( verifier_main_struct_t    const * verifier_info
    , WdgM_SupervisedEntityType const * wdgm_se
    , WdgM_CheckPointType       const * wdgm_cp
    )
{
    if (  (wdgm_cp->NrOfLocalTransitions > 0)
       && (wdgm_cp->WdgMLocalTransitionRef != NULL)
       )
    {
        return true;
    }
    else if (  (wdgm_cp->NrOfLocalTransitions == 0)
            && (wdgm_cp->WdgMLocalTransitionRef == NULL)
            )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool sanity_test
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info)
{
    bool entity_flag        = false;

    if (  (main_struct->WdgMSupervisedEntityRef != NULL)
       && (main_struct->NrOfSupervisedEntities > 0)
       )
    {
        entity_flag = true;
    }

    if ( !entity_flag )
    {
        printf ( "--------------------------------------------------\n");
        printf ( "Sanity_test: \n");
        printf ( "ENTITY CHECK: \n"
                 "main_struct->WdgMSupervisedEntityRef is NULL_PTR or\n"
                 "main_struct->NrOfSupervisedEntities is <= 0,\n\n"
                );
        return false;
    }

    if ( !(apply_to_edf_se_list (main_struct, verifier_info, st1)) )
    {
        printf ( "--------------------------------------------------\n");
        printf ( "Sanity_test: \n");
        printf ( "CHECKPOINT CHECK: \n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef is NULL_PTR or\n"
                 "main_struct->WdgMSupervisedEntityRef->NrOfCheckpoints is <= 0,\n\n"
                );
        return false;
    }

    if ( !(apply_to_edf_cp_list (main_struct, verifier_info, st2)) )
    {
        printf ( "--------------------------------------------------\n");
        printf ( "Sanity_test: \n");
        printf ( "GLOBAL TRANSITION CHECK: \n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->WdgMGlobalTransitionsRef is NULL or\n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->NrOfGlobalTransitions <= 0,\n"
                 "OR \n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->WdgMGlobalTransitionsRef is not NULL or\n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->NrOfGlobalTransitions != 0,\n\n"
                );
        return false;

    }

    if ( !(apply_to_edf_cp_list (main_struct, verifier_info, st3)) )
    {
        printf ( "--------------------------------------------------\n");
        printf ( "Sanity_test: \n");
        printf ( "LOCAL TRANSITION CHECK: \n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->WdgMLocalTransitionRef is NULL or\n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->NrOfLocalTransitions <= 0,\n"
                 "OR \n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->WdgMLocalTransitionRef is not NULL or\n"
                 "main_struct->WdgMSupervisedEntityRef->WdgMCheckpointRef->NrOfLocalTransitions != 0,\n\n"
                );
        return false;
    }

    return true;
}

bool print_comparison (int x, int y)
{
    bool result = x == y;
    print_result_header ();
    printf ("X = Y : %sOK\n", result ? "" : "NOT ");
    newline ();
    return result;
}

void print_result (bool result)
{
    print_result_header ();
    printf ("result : %sOK\n", result ? "" : "NOT ");
    newline ();
}

/* return true if the given checkpoint has at least one local transition */
static bool cpHasLocalTransition (WdgM_CheckPointType const * cp)
{
    return (cp != NULL) && (cp->WdgMLocalTransitionRef != NULL);
}


/* return true if the given checkpoint has at least one global transition */
static bool cpHasGlobalTransition (WdgM_CheckPointType const * cp)
{
    return (cp != NULL) && (cp->WdgMGlobalTransitionsRef != NULL);
}


/* Return a pointer to the first Checkpoint fullfilling the given
 * condition or NULL.
 *
 * (Manages looping over all checkpoints)
 *
 * */
static WdgM_CheckPointType const * getFirstCpFullfillingCondition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    , bool (* condition) (WdgM_CheckPointType const * cp)
    )
{
    WdgM_CheckPointType const * result   = NULL;
    WdgM_CheckPointType const * first_cp = get_first_cp (main_struct);
    WdgM_CheckPointType const * cp       = NULL;
    int i                                = 0;

    for
        ( i = 0, cp = first_cp
        ; i < xslt_checkpoint_cnt (verifier_info)
        ; i++, cp++
        )
    {
        if (cp == NULL)
        {
            return NULL;
        }
        if (condition (cp))
        {
            result = cp;
            break;
        }
    }
    return result;
}


/* return a pointer to the first Checkpoint having at least a
 * local transition or NULL
 * */
static WdgM_CheckPointType const * getFirstCpWithLocalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    return getFirstCpFullfillingCondition
        ( main_struct
        , verifier_info
        , cpHasLocalTransition);
}


/* return a pointer to the first Checkpoint having at least a
 * global transition or NULL
 * */
static WdgM_CheckPointType const * getFirstCpWithGlobalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    return getFirstCpFullfillingCondition
        ( main_struct
        , verifier_info
        , cpHasGlobalTransition
        );
}


/* return a pointer to the first local transition or NULL */
WdgM_TransitionType const * getFirstLocalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    WdgM_CheckPointType const * cp =
        getFirstCpWithLocalTransition (main_struct, verifier_info);
    return cp == NULL ? NULL : cp->WdgMLocalTransitionRef;
}

/* return a pointer to the first global transition or NULL */
WdgM_GlobalTransitionType const * getFirstGlobalTransition
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    WdgM_CheckPointType const * cp
        = getFirstCpWithGlobalTransition (main_struct, verifier_info);
    return cp == NULL ? NULL : cp->WdgMGlobalTransitionsRef;
}


int countAllCPInSE (const WdgM_ConfigType * main_struct, bool verbose)
{
    int i                                      = 0;
    int cp_count                               = 0;
    int n                                      = 0;
    WdgM_SupervisedEntityType const * se       = NULL;
    WdgM_SupervisedEntityType const * first_se = get_first_se (main_struct);

    for
        ( i = 0, se = first_se
        ; i < (int) main_struct->NrOfSupervisedEntities
        ; i++, se++
        )
    {
        n         = (se == NULL) ? 0 : (int) se->NrOfCheckpoints;
        cp_count += n;
        if (verbose)
        {
            printf ("SE [%d] has %d checkpoints\n", i, n);
        }
    }
    return cp_count;
}


int sum_over_cp_list
    ( WdgM_ConfigType const * main_struct
    , verifier_main_struct_t const * verifier_info
    , int (* getter) (WdgM_CheckPointType const * cp)
    )
{
    WdgM_CheckPointType const * first_cp = get_first_cp (main_struct);
    WdgM_CheckPointType const * cp       = NULL;
    int i                                = 0;
    int result                           = 0;

    if (first_cp != NULL)
    {
        for
            ( i = 0, cp = first_cp
            ; i < xslt_checkpoint_cnt (verifier_info)
            ; i++, cp++)
        {
            result += getter (cp);
        }
    }

    return result;
}

// to be passed as parameter
static int NrOfGlobalTransitions (WdgM_CheckPointType const *cp)
{
    return cp == NULL ? 0 : cp->NrOfGlobalTransitions;
}


// to be passed as parameter
static int NrOfLocalTransitions (WdgM_CheckPointType const *cp)
{
    return cp == NULL ? 0 : cp->NrOfLocalTransitions;
}


int total_global_trans_cnt
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    return sum_over_cp_list
        (main_struct, verifier_info, NrOfGlobalTransitions);
}


int total_local_trans_cnt
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    )
{
    return sum_over_cp_list
        (main_struct, verifier_info, NrOfLocalTransitions);
}


void print_all_checkpoint_IDs
    (WdgM_CheckPointType const * first_cp, int cp_count)
{
    int i = 0;
    WdgM_CheckPointType const * cp;

    printf ("Checkpoint IDs: ");
    for ( i = 0, cp = first_cp; i < cp_count; i++, cp++)
    {
        if ((bool) i)
            printf (", ");
        printf (cp == NULL ? "??" : "%d", (int) cp->WdgMCheckpointId);
    }
}

bool has_test_data (void const * p, int test_number, char const * item_name)
{
    if (p == NULL)
    {
        newline ();
        printf ("Result\n");
        printf ("------\n");
        printf
            ( "Cannot execute test #%d (no %s found).\n\n"
            , test_number
            , item_name
            );
        return false;
    }
    return true;
}

bool chk_cnt_and_ptr (void const * p, unsigned int const c, char const * n)
{
    if (c != 0 && p == NULL)
    {
        newline ();
        printf ("Result\n");
        printf ("------\n");
        printf ( "For \"%s\" count = %u, ref = NULL\n",n,c);
        printf ("------\n");
        newline ();
        return false;
    }
    return true;
}

bool chk_cnt_or_ptr (void const * p, unsigned int const c, char const * n)
{
    if (c == 0 || p == NULL)
    {
        newline ();
        printf ("Result\n");
        printf ("------\n");
        printf ( "For \"%s\" count = %u, ref = NULL\n",n,c);
        printf ("------\n");
        newline ();
        return false;
    }
    return true;
}


/***
 * get the ID of the Supervised Entity containing the checkpoint at the
 * given position.
 *
 **/
int get_se_id_for_cp_pos (const WdgM_ConfigType * main_struct, int cp_pos)
{
    WdgM_SupervisedEntityType const * p = NULL;

    int ceiling      = 0;
    int last_ceiling = 0;
    int i            = 0;

    for
        ( i = 0, p = main_struct->WdgMSupervisedEntityRef
        ; i < main_struct->NrOfSupervisedEntities
        ; i++, p++
        )
        {
            if (p == NULL)
            {
                return BAD_ID;
            }
            ceiling += p->NrOfCheckpoints;
            if ((last_ceiling <= cp_pos) && (cp_pos < ceiling))
            {
                return p->WdgMSupervisedEntityId;
            }
            last_ceiling += p->NrOfCheckpoints;
        }
    return BAD_ID;
}


boolean const  cp_exists
    ( WdgM_ConfigType        const * main_struct
    , verifier_main_struct_t const * verifier_info
    , int se_id
    , int cp_id
    )
{
    int i;
    WdgM_SupervisedEntityType const  * first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const  * se       = NULL;
    WdgM_CheckPointType       const  * cp       = NULL;

    for
        ( i = 0, se = first_se
        ; i < xslt_entity_cnt (verifier_info)
        ; i++, se++
        )
    {
        CHECK_NULL_PTR (se, 1001);
        if (se_id == se->WdgMSupervisedEntityId)
        {
            for
                (i = 0, cp = se->WdgMCheckpointRef
                ; i < se->NrOfCheckpoints
                ; i++, cp++
                )
            {
                CHECK_NULL_PTR (cp, 1002);
                if (cp_id == cp->WdgMCheckpointId)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool equals (char const * s1, char const * s2)
{
    if ( s1 == NULL || s2 == NULL)
    {
        return false;
    }

    return (strcmp (s1, s2) == 0);
}

int count_global_transitions_starting_at
    ( int se_id
    , int cp_id
    , const verifier_main_struct_t * verifier_info
    )
{
    int result = 0;
    int i;
    char se_name [MAX_IDENTIFIER_LENGTH];
    char cp_name [MAX_IDENTIFIER_LENGTH];
    int se_cnt                        = verifier_info->entity_cnt;
    global_transition_t const * trans = verifier_info->global_transitions;
    cp_id_data_t const * p;

    entity_struct_t const * se_array = verifier_info->entity_list;
    p                                = get_vi_cp
        (se_array, se_cnt, se_id, cp_id);
    if (p != NULL)
    {
        strcpy (se_name, p->se_name);
        strcpy (cp_name, p->cp_name);
        printf ("Found: %s/%s\n", se_name, cp_name);
        for
            ( i = 0, trans = verifier_info->global_transitions
            ; i < verifier_info->global_trans_cnt
            ; i++, trans++
            )
        {
            if (trans == NULL)      /* TODO: improve error handling */
            {
                return BAD_ID;
            }
            if
                (  (strcmp (p->se_name, trans->source_se) == 0)
                && (strcmp (p->cp_name, trans->source_cp) == 0)
                )
            {
                result++;
            }
        }
    }
    else
    {
        result = BAD_ID;
    }

    return result;
}


WdgM_SupervisedEntityType const * get_wdgm_se ( const WdgM_ConfigType * main_struct, int se_id)
{
    int i;
    WdgM_SupervisedEntityType const * result        = NULL;
    WdgM_SupervisedEntityType const * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const * se            = NULL;

    ASSERT_ENTITIES();

    for
        ( i = 0, se = wdgm_first_se
        ; i < (int) main_struct->NrOfSupervisedEntities
        ; i++, se++
        )
        {
            if (se->WdgMSupervisedEntityId == se_id)
            {
                result = se;
            }
        }

    return result;
}



WdgM_CheckPointType   const * get_wdgm_cp
    ( WdgM_ConfigType const * main_struct
    , int se_id
    , int cp_id
    )
{
    int i, j;
    WdgM_CheckPointType       const * result        = NULL;
    WdgM_SupervisedEntityType const * wdgm_first_se = get_first_se (main_struct);
    WdgM_SupervisedEntityType const * wdgm_se       = NULL;
    WdgM_CheckPointType       const * cp            = NULL;

    ASSERT_ENTITIES();

    for
        ( i = 0, wdgm_se = wdgm_first_se
        ; i < (int) main_struct->NrOfSupervisedEntities
        ; i++, wdgm_se++
        )
    {
        if (wdgm_se->WdgMSupervisedEntityId == se_id)
        {
            CHECK_NULL_PTR (wdgm_se, 2);
            ASSERT_CHECKPOINTS(wdgm_se);

            for ( j = 0, cp = wdgm_se->WdgMCheckpointRef
                ; j < wdgm_se->NrOfCheckpoints
                ; j++, cp++
                )
            {
                if (cp->WdgMCheckpointId == cp_id)
                {
                    result = cp;
                }
            }
        }
    }

    return result;
}

bool apply_to_edf_se_list
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    , bool (* f) (const verifier_main_struct_t * vi, WdgM_SupervisedEntityType const  * _se, int counter)
    )
{
    int i, xsl_se_id;
    bool result    = true;
    bool subresult = true;

    entity_struct_t           const * se             = NULL;
    WdgM_SupervisedEntityType const * target_wdgm_se = NULL;

    for
        ( i = 0, se = verifier_info->entity_list
        ; i < verifier_info->entity_cnt
        ; i++, se++
        )
        {
            xsl_se_id =  se->id;
            target_wdgm_se = get_wdgm_se (main_struct, xsl_se_id);

            if (target_wdgm_se == NULL)
            {
                return false;
            }
            else
            {
                subresult = f (verifier_info, target_wdgm_se, i);
                result    = result && subresult;
            }
        }
    return result;
}

bool apply_to_edf_cp_list
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    , bool (* f) (const verifier_main_struct_t * vi, WdgM_SupervisedEntityType const  * _se, WdgM_CheckPointType const  * _cp)
    )
{
    int i, j, xsl_se_id, xsl_cp_id;
    bool result    = true;
    bool subresult = true;

    entity_struct_t const * se = NULL;
    cp_id_data_t          * cp = NULL;

    WdgM_SupervisedEntityType const * target_wdgm_se = NULL;
    WdgM_CheckPointType       const * target_wdgm_cp = NULL;

    for
        ( i = 0, se = verifier_info->entity_list
        ; i < verifier_info->entity_cnt
        ; i++, se++
        )
        {
            xsl_se_id =  se->id;
            target_wdgm_se = get_wdgm_se (main_struct, xsl_se_id);

            if (target_wdgm_se == NULL)
            {
                return false;
            }

            for
                ( j = 0, cp = se->checkpoint_list
                ; j < se->cp_cnt
                ; j++, cp++
                )
            {
                xsl_cp_id = cp->cp_id;
                target_wdgm_cp = get_wdgm_cp (main_struct, xsl_se_id, xsl_cp_id);

                if (target_wdgm_cp != NULL)
                {
                    subresult = f (verifier_info, target_wdgm_se, target_wdgm_cp);
                    result    = result && subresult;
                }
            }

        }
    return result;
}

unsigned int number_of_se_in_mode (const WdgM_ConfigType * main_struct)
{
    unsigned int result = 0;
    unsigned int i      = 0;

    WdgM_SupervisedEntityType const * se       = NULL;
    WdgM_SupervisedEntityType const * first_se = get_first_se (main_struct);

    for
        ( i = 0, se = first_se
        ; i < (int) main_struct->NrOfSupervisedEntities
        ; i++, se++
        )
    {
        if (main_struct->WdgMModeCoreAssignment == se->WdgMEntityCoreId)
        {
            result++;
        }
    }
    return result;
}

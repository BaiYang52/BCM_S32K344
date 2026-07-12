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
/**        \file  edf_utilities.c
 *        \brief  edf_utilities.c source file
 *
 *      \details  This is the implementation of all edf utilities of the WdgM verifier
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

#include <stdbool.h>
#include "edf_utilities.h"
#include "utilities.h"


int xslt_local_trans_cnt (verifier_main_struct_t const * verifier_info)
{
    return verifier_info->local_trans_cnt;
}

int xslt_global_trans_cnt (verifier_main_struct_t const * verifier_info)
{
    return verifier_info->global_trans_cnt;
}

int xslt_checkpoint_cnt (verifier_main_struct_t const * verifier_info)
{
    int result                 = 0;
    int i                      = 0;
    entity_struct_t const * se = verifier_info->entity_list;

    if (se != NULL)
    {
        for (i = 0; i < verifier_info->entity_cnt; i++, se++)
        {
            result += se->cp_cnt;
        }
    }
    return result;
}

int xslt_entity_cnt (verifier_main_struct_t const * verifier_info)
{
    return verifier_info->entity_cnt;
}

alive_supervision_t const * get_vi_as
    ( verifier_main_struct_t const * verifier_info
    , int se_id
    , int cp_id
    )
{
    int i;
    alive_supervision_t const * result = NULL;
    alive_supervision_t const * p      = NULL;

    cp_id_data_t const * vi_cp = get_vi_cp
        ( verifier_info->entity_list
        , verifier_info->entity_cnt
        , se_id
        , cp_id
        );
    for
        ( i = 0, p = verifier_info->alive_supervision_list
        ; i < verifier_info->alive_supervision_cnt
        ; i++, p++
        )
    {
        if (  equals (p->se, vi_cp->se_name)
           && equals (p->cp, vi_cp->cp_name)
           )
        {
            result = p;
        }
    }
    return result;
}

entity_struct_t const * get_vi_se (verifier_main_struct_t const * verifier_info, int se_id)
{
  int i;

  for
        ( i = 0
        ; i < verifier_info->entity_cnt
        ; i++
        )
    {
    if (se_id == verifier_info->entity_list[i].id){

      return &(verifier_info->entity_list[i]);
    }
  }
  return NULL;
}

cp_id_data_t const * get_vi_cp
    ( entity_struct_t const * se_array
    , int se_cnt
    , int se_id
    , int cp_id
    )
{
    int i, j;
    entity_struct_t const * se = se_array;
    cp_id_data_t    const * cp = NULL;

    for
        ( i = 0, se = se_array
        ; i < se_cnt
        ; i++, se++
        )
    {
        if (se == NULL)
        {
            return NULL;
        }
        if (se_id == se->id)
        {
            for
                ( j = 0, cp = se->checkpoint_list
                ; j < se->cp_cnt
                ; j++, cp++
                )
            {
                if (cp == NULL)
                {
                    return NULL;
                }
                if (cp->cp_id == cp_id)
                {
                    return cp;
                }
            }
        }
    }
    return NULL;
}

char const * get_vi_se_name (verifier_main_struct_t const * verifier_info, int se_id)
{
  int i;

  for
        ( i = 0
        ; i < verifier_info->entity_cnt
        ; i++
        )
    {
    if (se_id == verifier_info->entity_list[i].id){

      return verifier_info->entity_list[i].name;
    }
  }
  return NULL;
}

char const * get_vi_cp_name
    (verifier_main_struct_t const * verifier_info, int se_id, int cp_id)
{
  int i, j;
  entity_struct_t const * vi_se = NULL;
  cp_id_data_t const * vi_cp    = NULL;

    for
        ( i = 0, vi_se = verifier_info->entity_list
        ; i < verifier_info->entity_cnt
        ; i++, vi_se++
        )
    {
    if (se_id == verifier_info->entity_list[i].id)
    {

        for
            ( j = 0, vi_cp = vi_se->checkpoint_list
            ; j < vi_se->cp_cnt
            ; j++, vi_cp++
            )
        {
            if (cp_id == vi_cp->cp_id)
            {
                return vi_cp->cp_name;
            }
        }
    }
  }
  return NULL;
}

int const * get_vi_se_id
    (verifier_main_struct_t const * verifier_info, char const * se_name)
{
    int i = 0;
    entity_struct_t const * vi_se = NULL;

    for
        ( i = 0, vi_se = verifier_info->entity_list
        ; i < verifier_info->entity_cnt
        ; i++, vi_se++
        )
    {
        if (equals (se_name, vi_se->name))
        {
          return &(vi_se->id);
        }
    }
    return NULL;
}

int const * get_vi_cp_id
    ( verifier_main_struct_t const * verifier_info
    , char const * se_name
    , char const * cp_name
    )
{
    int i = 0, j = 0;
    entity_struct_t const * vi_se = NULL;
    cp_id_data_t    const * vi_cp = NULL;

    for
        ( i = 0, vi_se = verifier_info->entity_list
        ; i < verifier_info->entity_cnt
        ; i++, vi_se++
        )
    {
        if (equals(se_name, vi_se->name))
        {
            for
                ( j = 0, vi_cp = vi_se->checkpoint_list
                ; j < vi_se->cp_cnt
                ; j++, vi_cp++
                )
            {
                if (equals(cp_name, vi_cp->cp_name))
                {
                    return &(vi_cp->cp_id);
                }
            }
        }
  }
  return NULL;
}

wdgm_mode_t const * get_vi_wdgm_mode
    ( verifier_main_struct_t const * verifier_info
    , int core_id
    )
{
    int i;
    wdgm_mode_t const * result = verifier_info->mode_info;

    if (result != NULL)
    {
        for
            ( i = 0
            ; i < verifier_info->wdgm_mode_cnt
            ; i++, result++
            )
        {
            if (result->core_id == core_id)
            {
                return result;
            }
        }

    }
    return result;
}





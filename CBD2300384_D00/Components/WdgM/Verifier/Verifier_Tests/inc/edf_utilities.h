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
/*!        \file  edf_utilities.h.h
 *        \brief  edf_utilities.h header file
 *
 *      \details  This is the header file of WdgM verifier edf utilities.
 *
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the Verifier's header file.
 *********************************************************************************************************************/
 
#ifndef WDGM_VERIFIER_EDF_UTILITIES_H
#define WDGM_VERIFIER_EDF_UTILITIES_H

#include <stdlib.h>
#include "wdgm_verifier_types.h"
int xslt_local_trans_cnt  (verifier_main_struct_t const * verifier_info);
int xslt_checkpoint_cnt   (const verifier_main_struct_t * verifier_info);
int xslt_entity_cnt       (verifier_main_struct_t const * verifier_info);
int xslt_global_trans_cnt (verifier_main_struct_t const * verifier_info);

/**
 * get_vi_as
 *
 * Return a pointer to the Alive Supervision related to the checkpoint
 * identified by the given Supervised Entity ID <se_id> and Checkpoint ID
 * <cp_id>
 **/
alive_supervision_t const * get_vi_as
    ( verifier_main_struct_t const * verifier_info
    , int se_id
    , int cp_id
    );

/**
 * get_vi_cp
 *
 * Return a pointer to the checkpoint with the given Supervised Entity Id
 * and Checkpoint ID (or NULL if no checkpoint can be found)
 *
 * @param [IN]  se_array        array containing the Supervised Entities
 * @param [IN]  se_cnt          length of <se_array>
 * @param [IN]  se_id           ID of the SE containing the looked-for CP
 * @param [IN]  cp_id           ID of the looked-for checkpoint
 **/
cp_id_data_t const * get_vi_cp
    ( entity_struct_t const * se_array
    , int se_cnt
    , int se_id
    , int cp_id
    );

/**
 * get_vi_se
 *
 * Return a pointer to the supervised entity with the given ID
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  se_id           ID of the SE we are looking for
 *
 * @return entity_struct_t const * a pointer to the SE or NULL
 *
 **/
entity_struct_t const * get_vi_se
    (verifier_main_struct_t const * verifier_info, int se_id);

/**
 * get_vi_se_name
 *
 * Return a pointer with the name of the supervised entity with the given ID
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  se_id           ID of the SE we are looking for
 *
 * @return entity_struct_t const * a pointer to the name of the SE or NULL
 *
 **/
char const * get_vi_se_name
    (verifier_main_struct_t const * verifier_info, int se_id);

/**
 * get_vi_cp_name
 *
 * Return a pointer with the name of the supervised entitys checkpoint
 * with the given supervised entity and checkpoint ID
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  se_id           ID of the SE we are looking for
 * @param [IN]  cp_id           ID of the CP we are looking for
 *
 * @return entity_struct_t const * a pointer to the CP or NULL
 *
 **/
char const * get_vi_cp_name
    (verifier_main_struct_t const * verifier_info, int se_id, int cp_id);

/**
 * get_vi_se_id
 *
 * Return a pointer with the id of the supervised entity with the given name
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  se_name         name of the SE we are looking for
 *
 * @return entity_struct_t const * a pointer to the id of the SE or NULL
 *
 **/
int const * get_vi_se_id
    (verifier_main_struct_t const * verifier_info, char const * se_name);

/**
 * get_vi_cp_id
 *
 * Return a pointer with the id of the supervised entitys checkpoint
 * with the given supervised entity and checkpoint name
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  se_name         name of the SE we are looking for
 * @param [IN]  cp_name         name of the CP we are looking for
 *
 * @return entity_struct_t const * a pointer to the CP or NULL
 *
 **/
int const * get_vi_cp_id
    ( verifier_main_struct_t const * verifier_info
    , char const * se_name
    , char const * cp_name
    );



/**
 * get_vi_wdgm_mode
 *
 * Return a pointer with to the struct representing the WdgMMode element
 * having the given core ID
 *
 * @param [IN]  verifier_info   pointer to reference configuration
 * @param [IN]  core_id         the core ID of the WdgMMode we are looking for
 * @param [IN]  cp_name         name of the CP we are looking for
 *
 * @return wdgm_mode_t const * a pointer to the struct with mode data or NULL
 *
 **/
wdgm_mode_t const * get_vi_wdgm_mode
    ( verifier_main_struct_t const * verifier_info
    , int wdgm_core_id
    );
#endif

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actECTools.h
 *        \brief  Interface EC domain parameter initialization.
 *
 *      \details This file is part of the embedded systems library cvActLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ACTECTOOLS_H
# define ACTECTOOLS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* These flags are used to determined for which algorithm/protocol the workspace is going to be used. The distinction
 * is necessary, since the different algorithms require different workspace sizes. Before storing curve parsed curve
 * parameters in the workspace, it is checked whether the workspace size is sufficient. */
# define actEC_ALGO_FLAG_KG                                           (1u) /* workspace shall be used for key generation */
# define actEC_ALGO_FLAG_SP                                           (2u) /* workspace shall be used for signature generation */
# define actEC_ALGO_FLAG_VP                                           (3u) /* workspace shall be used for signature verification */
# define actEC_ALGO_FLAG_DH_PRIM                                      (4u) /* workspace shall be used for Diffie Hellman primitive */
# define actEC_ALGO_FLAG_DH                                           (5u) /* workspace shall be used for Diffie Hellman protocol */
# define actEC_ALGO_FLAG_BDKA                                         (6u) /* workspace shall be used for Burmester Desmedt protocol */
# define actEC_ALGO_FLAG_SPAKE2P                                      (7u) /* workspace shall be used for SPAKE2+ protocol */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C" {
# endif

/**********************************************************************************************************************
 *  actECParseDomain()
 *********************************************************************************************************************/
/*! \brief          Function to parse domain TLV object.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in,out]  p_bytes      (in)  buffer to return extracted length value
 *                               (out) length of p in bytes
 *  \param[in,out]  p_addr       (in)  buffer to return extracted address
 *                               (out) address of p
 *  \param[in,out]  a_addr       (in)  buffer to return extracted address
 *                               (out) address of a
 *  \param[in,out]  n_bytes      (in)  buffer to return extracted length value
 *                               (out) length of n in bytes
 *  \param[in,out]  n_addr       (in)  buffer to return extracted address
 *                               (out) address of n
 *  \return         actEXCEPTION_DOMAIN       domain decoding error
 *                  actOK                     else
 *  \pre            provided pointers must be valid
 *                  domain must be a valid ASN1 structure for EC domain parameters.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( actRETURNCODE ) actECParseDomain(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
   VSECPRIM_P2VAR_PARA( actLengthType ) p_bytes,
   VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) p_addr,
   VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) a_addr,
   VSECPRIM_P2VAR_PARA( actLengthType ) n_bytes,
  VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)) n_addr
);

/**********************************************************************************************************************
 *  actECInit()
 *********************************************************************************************************************/
/*! \brief          This function initializes the ECC workspace.
 *  \details        -
 *  \param[in]      domain       domain parameter
 *  \param[in]      domain_ext   domain parameter extensions (Montgomery constants etc.)
 *  \param[in]      speedup_ext  (optional) precomputations (for ECDSA-Sign, -GenKey)
 *  \param[in]      algo_flag    actEC_ALGO_FLAG_* (see macros above), indicates how long the minimal workspace size
 *                               must be
 *  \param[in,out]  wksp         (in) workspace
 *                               (out) initialized workspace
 *  \param[in]      wksp_len     length of workspace in bytes
 *  \return         actEXCEPTION_DOMAIN       domain decoding error
 *                  actEXCEPTION_DOMAIN_EXT   domain_ext decoding error
 *                  actEXCEPTION_SPEEDUP_EXT  speedup_ext decoding error
 *                  actEXCEPTION_MEMORY       wksp_len to small
 *                  actEXCEPTION_ALGOID       algo_flag invalid
 *                  actOK                     else
 *  \pre            domain, domain_ext and wksp must be valid pointers
 *                  domain, domain_ext and speedup_ext need to be valid ASN1 structures for EC domain parameters and
 *                  precomputed points.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECInit(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  actU16 algo_flag,
  VSECPRIM_P2VAR_PARA(actECPBasicStruct) wksp,
  actLengthType wksp_len);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif
# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECTOOLS_H */

/**********************************************************************************************************************
 *  END OF FILE: actECTools.h
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2018 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actIECBD.h
 *        \brief  ECBD implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

/****************************************************************************
 **
 ** EC-B/D key agreement (internal) programming interface
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   actECBDinit
 **   actECBDgenKeyPair
 **   actECBDcalcXi
 **   actECBDinitK
 **   actECBDupdateK
 **   actECBDretrieveK
 **
 ***************************************************************************/

#ifndef ACTIECBD_H
# define ACTIECBD_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actECPoint.h"

 /* PRQA S 5209 EOF */ /* MD_VSECPRIM_USE_OF_BASIC_TYPES */

/****************************************************************************
 ** constants and types
 ***************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/****************************************************************************
 **
 **  actECBDinit
 **
 **   Initialize the EC-B/D key agreement:
 **   - validate scratch memory size,
 **   - "allocate" scratch memory,
 **   - initialize curve structure.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - wsSize:       work space size (includes scratch memory)
 ** - nECUs:        number of ECUs partaking in the key agreement
 ** - ecuID:        ID of this ECU (1..nECUs)
 ** - domain:       domain parameters           (p, a, b, G, n, h)
 ** - domainExt:    domain parameter extensions (Montgomery data)
 ** - speedupExt:   speed-up extensions         (base point)
 **
 ** output:
 ** - returns:      actEXCEPTION_NULL         when an input parameter is NULL
 **                 actEXCEPTION_MEMORY       wsSize too small
 **                 actEXCEPTION_DOMAIN       domain decoding error
 **                 actEXCEPTION_DOMAIN_EXT   domainExt decoding error
 **                 actEXCEPTION_SPEEDUP_EXT  speedupExt decoding error
 **                 actOK                     otherwise
 **
 ** assumes:
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC( actRETURNCODE ) actECBDinit(VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                            const actLengthType  wsSize,
                            const actU32            nECUs,
                            const actU32            ecuID,
                            VSECPRIM_P2ROMCONST_PARA(actU8) domain,
                            VSECPRIM_P2ROMCONST_PARA(actU8) domainExt,
                            VSECPRIM_P2ROMCONST_PARA(actU8) speedupExt);

/****************************************************************************
 **
 **  actECBDgenKeyPair
 **
 **   Generate the ephemeral key pair (ai, Zi):
 **   - ai is not returned, but stored in the scratch memory,
 **   - Zi   IS   returned  AND stored in the scratch memory.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - privateKey:   pointer to memory for private key (if NULL, key is not returned)
 ** - pubKey_x:     pointer to memory for public key (Zi - X coordinate)
 ** - pubKey_y:     pointer to memory for public key (Zi - Y coordinate)
 ** - watchdog:     Watchdog trigger function pointer
 **
 ** output:
 ** - privateKey:   private key (if pointer not NULL)
 ** - pubKey_x:     public key (Zi, X coordinate)
 ** - pubKey_y:     public key (Zi, Y coordinate)
 ** - returns:      actEXCEPTION_NULL       when an input parameter is NULL
 **                 actEXCEPTION_RANDOM     when the RNG doesn't work
 **                 actEXCEPTION_POINT      when the resulting public key is invalid
 **                                         (point not on curve or infinite)
 **                 actOK                   otherwise
 **
 ** assumes:
 ** - work space has been set up using actECBDinit()
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC( actRETURNCODE ) actECBDgenKeyPair (VSECPRIM_P2VAR_PARA( actECBDstruct )ws,
                                        VSECPRIM_P2VAR_PARA( actU8 ) privKey,
                                        VSECPRIM_P2VAR_PARA( actU8 ) pubKey_x,
                                        VSECPRIM_P2VAR_PARA( actU8 ) pubKey_y,
                                        VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 **
 **  actECBDcalcXi
 **
 **   Calculate the intermediate value Xi from neighbors' public keys:
 **   - ai is taken from scratch memory,
 **   - Xi is returned.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - ZiLeft_x:     pointer to left  neighbor's public key (Zi-1 - X coordinate)
 ** - ZiLeft_y:     pointer to left  neighbor's public key (Zi-1 - Y coordinate)
 ** - ZiRight_x:    pointer to right neighbor's public key (Zi+1 - X coordinate)
 ** - ZiRight_y:    pointer to right neighbor's public key (Zi+1 - Y coordinate)
 ** - Xi_x:         pointer to memory for intermediate value (Xi - X coordinate)
 ** - Xi_y:         pointer to memory for intermediate value (Xi - Y coordinate)
 ** - watchdog:     Watchdog trigger function pointer
 **
 ** output:
 ** - Xi_x:         intermediate value (Xi - X coordinate)
 ** - Xi_y:         intermediate value (Xi - Y coordinate)
 ** - returns:      actEXCEPTION_NULL       when an input parameter is NULL
 **                 actEXCEPTION_POINT      when the resulting Xi is invalid
 **                                         (point not on curve or infinite)
 **                 actOK                   otherwise
 **
 ** assumes:
 ** - work space has been set up using actECBDinit()
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC( actRETURNCODE ) actECBDcalcXi (VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                    VSECPRIM_P2CONST_PARA( actU8 ) ZiLeft_x,
                                    VSECPRIM_P2CONST_PARA( actU8 ) ZiLeft_y,
                                    VSECPRIM_P2CONST_PARA( actU8 ) ZiRight_x,
                                    VSECPRIM_P2CONST_PARA( actU8 ) ZiRight_y,
                                    VSECPRIM_P2VAR_PARA( actU8 ) Xi_x,
                                    VSECPRIM_P2VAR_PARA( actU8 ) Xi_y,
                                    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 **
 **  actECBDinitK
 **
 **   Initalize the common secret (K) calculation:
 **   - ai, Zi, Xi-1 and nNodes are taken from scratch memory.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - watchdog:     Watchdog trigger function pointer
 **
 ** output:
 ** - returns:      actEXCEPTION_NULL       when an input parameter is NULL
 **                 actOK                   otherwise
 **
 ** assumes:
 ** - work space has been set up using actECBDinit() and all neccessary
 **   data have been stored by actECBDgenKeyPair() and actECBDcalcXi()
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC( actRETURNCODE ) actECBDinitK (VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                                    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 **
 **  actECBDupdateK
 **
 **   Update the common secret (K) with data received:
 **   - current K, nNodes and the own ecuID are taken from scratch memory,
 **   - K is updated with the data received.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - ecuID:        ID of the ECU the data (Xi) were received from
 ** - Xi_x:         pointer to intermediate value (Xi - X coordinate)
 ** - Xi_y:         pointer to intermediate value (Xi - Y coordinate)
 ** - watchdog:     Watchdog trigger function pointer
 **
 ** output:
 ** - returns:      actEXCEPTION_NULL       when an input parameter is NULL
 **                 actOK                   otherwise
 **
 ** assumes:
 ** - work space has been set up using actECBDinit() and common secret
 **   calculation has been set up by< actECBDinitK()
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC( actRETURNCODE ) actECBDupdateK (VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                                      const actU32          ecuID,
                                                      VSECPRIM_P2CONST_PARA( actU8 ) Xi_x,
                                                      VSECPRIM_P2CONST_PARA( actU8 ) Xi_y,
                                                      VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 **
 **  actECBDupdateK
 **
 **   Retrieve the common secret (K):
 **   - K (a point) is converted from Montgomery representation,
 **   - the (affine) coordinates are returned from scratch memory,
 **   - the scratch memory is erased.
 **
 ** input:
 ** - ws:           work space      (includes scratch memory)
 ** - K_x:          pointer to memory for the common secret (K - X coordinate)
 ** - K_y:          pointer to memory for the common secret (K - Y coordinate)
 ** - watchdog:     Watchdog trigger function pointer
 **
 ** output:
 ** - returns:      actEXCEPTION_NULL       when an input parameter is NULL
 **                 actEXCEPTION_POINT      when the resulting K is invalid
 **                                         (point not on curve or infinite)
 **                 actOK                   otherwise
 **
 ** assumes:
 ** - work space has been set up using actECBDinit() and common secret
 **   calculation has been set up by< actECBDinitK()
 **
 ** uses:
 **
 ***************************************************************************/
extern VSECPRIM_FUNC(actRETURNCODE) actECBDretrieveK(VSECPRIM_P2VAR_PARA(actECBDstruct) ws,
  VSECPRIM_P2VAR_PARA(actU8) K_x,
  VSECPRIM_P2VAR_PARA(actU8) K_y,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTIECBD_H  */

/**********************************************************************************************************************
 *  END OF FILE: actIECBD.h
 *********************************************************************************************************************/

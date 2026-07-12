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
/*!        \file  actIECBD.c
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
 **   actECBDscratch        scratch pad addressing
 **
 ** macros:
 **   thisECU               this ECUs ID
 **   leftECU               the "left neighbor" ECUs ID
 **   ecoOff                offset of the given ECU to this ECU (circular left)
 **
 ** functions:
 **   actECBDinit           initialize EC-N/D protocol
 **   actECBDgenKeyPair     generate the ephemeral key pair
 **   actECBDcalcXi         calculate the intermediate value
 **   actECBDinitK          initialize calculation of common secret
 **   actECBDupdateK        update common secret with another ECUs intermediate
 **   actECBDretrieveK      retrieve the common secret
 **
 ***************************************************************************/

#define ACTIECBD_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIECBD.h"
#include "actECTools.h"
#include "actECPoint.h"
#include "actECLengthInfo.h"

#if (VSECPRIM_ACTECBD_GENERIC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  DATA PROTOTYPES
 *********************************************************************************************************************/

typedef struct
{
    VSECPRIM_P2VAR_PARA(actBNDIGIT) ai;   /*  private key */
    actECPOINT   Zi;   /*  public  key  (own / left) */
    actECPOINT   Xi;   /*  intermediate (own) */
    actECPOINT   TP;   /*  temporary point */
} actECBDscratch;

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define thisECU(ws)                                                  (     (ws)->bd.ecuID)
# define leftECU(ws)                                                  ((1u == (ws)->bd.ecuID) ? (ws)->bd.nECUs : ((ws)->bd.ecuID -1u))
# define ecuOff(ecu, ws)                                              (((ws)->bd.nECUs + (ecu) - (ws)->bd.ecuID) % (ws)->bd.nECUs)

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*  initialize scratch-pad memory */
VSECPRIM_LOCAL_FUNC( void )           initScratch  (VSECPRIM_P2VAR_PARA( actECBDscratch ) scratch, VSECPRIM_P2VAR_PARA( actECBDstruct ) ws);

VSECPRIM_LOCAL_FUNC( void )          actECPImport (VSECPRIM_P2VAR_PARA( actECPOINT ) P,
                                                    VSECPRIM_P2CONST_PARA( actU8 ) x,
                                                    VSECPRIM_P2CONST_PARA( actU8 ) y,
                                                    VSECPRIM_P2CONST_PARA( actECCURVE ) curve,
                                                    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

VSECPRIM_LOCAL_FUNC( actRETURNCODE ) actECPExport (VSECPRIM_P2VAR_PARA( actU8 ) x,
                                                    VSECPRIM_P2VAR_PARA( actU8 ) y,
                                                    VSECPRIM_P2VAR_PARA( actECPOINT ) P,
                                                    VSECPRIM_P2VAR_PARA( actECPOINT )TP,
                                                    VSECPRIM_P2VAR_PARA( actECCURVE ) curve,
                                                    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
VSECPRIM_FUNC( actRETURNCODE ) actECBDinit (VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                     const actLengthType    wsSize,
                     const actU32              nECUs,
                     const actU32              ecuID,
                     VSECPRIM_P2ROMCONST_PARA(actU8) domain,
                     VSECPRIM_P2ROMCONST_PARA(actU8) domainExt,
                     VSECPRIM_P2ROMCONST_PARA(actU8) speedupExt) {

    VSECPRIM_P2VAR_PARA(actECPBasicStruct) ecWS     = &ws->baseData;       /*  actECInit() expects:         actECCURVE | scratch */
    actLengthType ecWSSize = wsSize - sizeof (ws->bd);   /*  actECInit() expects: sizeof (actECCURVE | scratch) */

   /*  parameters already validated */
    ws->bd.nECUs     = nECUs;
    ws->bd.ecuID     = ecuID;

    return actECInit (domain, domainExt, speedupExt, actEC_ALGO_FLAG_BDKA, ecWS, ecWSSize);
}

VSECPRIM_FUNC( actRETURNCODE ) actECBDgenKeyPair(VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                                 VSECPRIM_P2VAR_PARA( actU8 ) privKey,
                                                 VSECPRIM_P2VAR_PARA( actU8 ) pubKey_x,
                                                 VSECPRIM_P2VAR_PARA( actU8 ) pubKey_y,
                                                 VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{

    actECBDscratch  scratch;
    actRETURNCODE   rc;

   /*  parameters already validated */
    if (NULL_PTR == ws)
    {
      return actEXCEPTION_NULL;
    }

   /*  initialize working variables */
    initScratch ((VSECPRIM_P2VAR_PARA( actECBDscratch ))&scratch, ws);

   /*  generate ai at random */
    rc = actBNModRandomize(scratch.ai, &ws->baseData.Curve.n_field, ws->baseData.Curve.t, watchdog);
    if (rc != actOK)
    {
       /*  clear ai */
        actBNSetZero (scratch.ai, ws->baseData.Curve.n_field.m_length);
        return rc;
    }

   /*  calculate Zi = ai * G */
    actECPMultG (scratch.ai, &scratch.TP, &scratch.Zi, &ws->baseData.Curve, watchdog);

   /*  return public key */
    rc = actECPExport (pubKey_x, pubKey_y, &scratch.Zi, NULL_PTR, &ws->baseData.Curve, watchdog);

    if (actOK != rc)
    {
       /*  clear ai and Zi */
        actBNSetZero (scratch.ai,      ws->baseData.Curve.n_field.m_length);
        actBNSetZero (scratch.Zi.x, (3u*ws->baseData.Curve.p_field.m_length));
    }
    else
    {
      if (NULL_PTR != privKey)
      {
        /*  return private key */
        actBNOctetString(privKey, ws->baseData.Curve.n_field.m_byte_length, scratch.ai, ws->baseData.Curve.n_field.m_length);
      }
    }

    return rc;
}

VSECPRIM_FUNC( actRETURNCODE ) actECBDcalcXi(VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                             VSECPRIM_P2CONST_PARA( actU8 ) ZiLeft_x,
                                             VSECPRIM_P2CONST_PARA( actU8 ) ZiLeft_y,
                                             VSECPRIM_P2CONST_PARA( actU8 ) ZiRight_x,
                                             VSECPRIM_P2CONST_PARA( actU8 ) ZiRight_y,
                                             VSECPRIM_P2VAR_PARA( actU8 ) Xi_x,
                                             VSECPRIM_P2VAR_PARA( actU8 ) Xi_y,
                                             VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{

    actECBDscratch  scratch;
    actRETURNCODE   rc;

   /*  parameters already validated */
    if (NULL_PTR == ws)
    {
      return actEXCEPTION_NULL;
    }

   /*  initialize working variables */
    initScratch (&scratch, ws);

   /*  import Zi(l) -> Zi (remains stored) */
    actECPImport (&scratch.Zi, ZiLeft_x,  ZiLeft_y,  &ws->baseData.Curve, watchdog);

   /*  import Zi(r) -> TP (just temporarily) */
    actECPImport (&scratch.TP, ZiRight_x, ZiRight_y, &ws->baseData.Curve, watchdog);

   /*  Xi = ai * (Zi(r) - Zi(l)) */
    actECPSub    (&scratch.TP, &scratch.Zi,              &ws->baseData.Curve, watchdog);      /*  TP = (Zi(r) - Zi(l)) */
    actECPMult   (&scratch.TP,  scratch.ai, &scratch.Xi, &ws->baseData.Curve, watchdog);      /*  Xi = ai * (Zi(r) - Zi(l)) */

   /*  export Xi */

    rc = actECPExport (Xi_x, Xi_y, &scratch.Xi, &scratch.TP, &ws->baseData.Curve, watchdog);
    if (actOK != rc)
    {
       /*  clear ai, Zi and Xi */
        actBNSetZero (scratch.ai,      ws->baseData.Curve.n_field.m_length);
        actBNSetZero (scratch.Zi.x, (6u* ws->baseData.Curve.p_field.m_length));
    }
   /*  clear TP */
    actBNSetZero     (scratch.TP.x, (3u* ws->baseData.Curve.p_field.m_length));

    return rc;
}

VSECPRIM_FUNC( actRETURNCODE ) actECBDinitK(VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                            VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) {

    actECBDscratch  scratch;

   /*  NO parameters to validate */
    if (NULL_PTR == ws)
    {
      return actEXCEPTION_NULL;
    }

   /*  initialize working variables */
    initScratch (&scratch, ws);

   /*  K   = (nECUs * ai) * Zi(l) + (nECUs -1) * Xi */
    actECPMult      (&scratch.Zi, scratch.ai,      &scratch.TP, &ws->baseData.Curve, watchdog);   /*  TP      = ai    * Zi(l) */
    actECPMultShort (&scratch.TP, ws->bd.nECUs,    &scratch.Zi, &ws->baseData.Curve, watchdog);   /*  K (Zi)  = nECUs * TP */
    actECPMultShort (&scratch.Xi, (ws->bd.nECUs - 1u), &scratch.TP, &ws->baseData.Curve, watchdog);   /*  TP      = (nECUs -1) * Xi */
    actECPAdd       (&scratch.Zi, &scratch.TP,                  &ws->baseData.Curve, watchdog);   /*  K (Zi) += TP */

   /*  clear ai, Xi and TP */
    actBNSetZero (scratch.ai,      ws->baseData.Curve.n_field.m_length);
    actBNSetZero (scratch.Xi.x, (6u* ws->baseData.Curve.p_field.m_length));

   /*  Zi has been overwritten with K */

    return actOK;
}

VSECPRIM_FUNC( actRETURNCODE ) actECBDupdateK(VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                        const actU32         ecuID,
                        VSECPRIM_P2CONST_PARA(actU8) Xi_x,
                        VSECPRIM_P2CONST_PARA(actU8) Xi_y,
                        VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) {

    actECBDscratch  scratch;
    actU32          tmp;

   /*  parameters already validated */
    if (NULL_PTR == ws)
    {
      return actEXCEPTION_NULL;
    }

   /*  initialize working variables */
    initScratch (&scratch, ws);

   /*  check ecuID */
    if ((ecuID == thisECU(ws))
      || (ecuID == leftECU(ws)))
      /*  done! */
      /*  these have been handled in actECBDinitK() */
    {
      return actOK;
    }

   /*  import Xi */
    actECPImport (&scratch.Xi, Xi_x,  Xi_y,  &ws->baseData.Curve, watchdog);

   /*  K  += ((nECUs -1) - ((ecuID - ownID) mod nECUs)) * Xi */
    tmp = (ws->bd.nECUs -1u) - ecuOff (ecuID, ws);                  /*  tmp     = (nECUs -1) - ((ecuID - ownID) mod nECUs) */
    actECPMultShort (&scratch.Xi, tmp, &scratch.TP, &ws->baseData.Curve, watchdog);   /*  TP      = tmp * Xi */
    actECPAdd       (&scratch.Zi,      &scratch.TP, &ws->baseData.Curve, watchdog);   /*  K (Zi) += TP */

   /*  clear Xi, TP */
    actBNSetZero (scratch.Xi.x, (6u* ws->baseData.Curve.p_field.m_length));

    return actOK;
}

VSECPRIM_FUNC( actRETURNCODE ) actECBDretrieveK (VSECPRIM_P2VAR_PARA( actECBDstruct ) ws,
                                VSECPRIM_P2VAR_PARA(actU8) K_x,
                                VSECPRIM_P2VAR_PARA(actU8) K_y,
                                VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) {

    actECBDscratch  scratch;
    actRETURNCODE   rc;

   /*  parameters already validated */
    if (NULL_PTR == ws)
    {
      return actEXCEPTION_NULL;
    }

   /*  initialize working variables */
    initScratch (&scratch, ws);

   /*  export K (Zi) */
    rc = actECPExport (K_x, K_y, &scratch.Zi, NULL_PTR, &ws->baseData.Curve, watchdog);

   /*  clear K (Zi) */
    actBNSetZero (scratch.Zi.x, (3u* ws->baseData.Curve.p_field.m_length));

    return rc;
}

/* --------------------------------------------------------------------------
 | local (private) functions
 ------------------------------------------------------------------------- */

VSECPRIM_LOCAL_FUNC( void ) initScratch(VSECPRIM_P2VAR_PARA( actECBDscratch )scratch, VSECPRIM_P2VAR_PARA( actECBDstruct ) ws) {

    scratch->ai = ws->ai;
    scratch->Zi.x = ws->Zi_x;
    scratch->Zi.y = ws->Zi_y;
    scratch->Zi.z = ws->Zi_z;
    scratch->Xi.x = ws->Xi_x;
    scratch->Xi.y = ws->Xi_y;
    scratch->Xi.z = ws->Xi_z;
    scratch->TP.x = ws->TP_x;
    scratch->TP.y = ws->TP_y;
    scratch->TP.z = ws->TP_z;

    /*  ASSUME Zi and Xi are storing points in projective (Montgomery) representation!
     *
     *  This is a valid assumption, as those points have been calculated before:
     *  * keyGeneration:    Zi   = ai * G
     *  * intermediate:     Zi   = Zi(l) (imported)
     *                      Xi   = Zi(r) - Zi(l)
     *  * init K calc.:  K (Zi)  = (n * ai) * Zi(l) + (n -1) * Xi
     *  * update K:      K (Zi) += (x) * Xi (imported)
     *
     *  TP is never used to store points between function calls.
     */
    scratch->Zi.is_affine = FALSE;
    scratch->Zi.is_infinity = FALSE;
    scratch->Xi.is_affine = FALSE;
    scratch->Xi.is_infinity = FALSE;
}

/*  import a point (x, y) and convert it to */
/*  projective Montgomery representation */
VSECPRIM_LOCAL_FUNC( void ) actECPImport (VSECPRIM_P2VAR_PARA( actECPOINT ) P, VSECPRIM_P2CONST_PARA( actU8 ) x, VSECPRIM_P2CONST_PARA( actU8 ) y, VSECPRIM_P2CONST_PARA( actECCURVE ) curve, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void)) {
    actLengthType pDigits = curve->p_field.m_length;
    actLengthType pBytes  = curve->p_field.m_byte_length;

    actBNSetOctetString (P->x, pDigits, x, pBytes);
    actBNSetOctetString (P->y, pDigits, y, pBytes);
    actBNSetOne         (P->z, pDigits);
    P->is_affine   = FALSE;
    P->is_infinity = FALSE;

    actECPToMont (P, curve, watchdog);
}

/*  convert a point to affine coordinates (x, y) and export it */
/*  TP is an optional temporary work space, if the point P has to be left unchanged! */
VSECPRIM_LOCAL_FUNC( actRETURNCODE ) actECPExport (VSECPRIM_P2VAR_PARA( actU8 ) x, VSECPRIM_P2VAR_PARA( actU8 ) y, VSECPRIM_P2VAR_PARA( actECPOINT ) P, VSECPRIM_P2VAR_PARA( actECPOINT ) TP, VSECPRIM_P2VAR_PARA( actECCURVE ) curve, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actECPOINT) tempPoint;
  actLengthType pDigits = curve->p_field.m_length;
  actLengthType pBytes  = curve->p_field.m_byte_length;

  if (NULL_PTR != TP)
  {
    tempPoint = TP;
    actECPAssign(TP, P, curve, watchdog);
  }
  else
  {
    tempPoint = P;
  }

  if (actECPToAffineFromMont(tempPoint, curve, 0, watchdog) != actOK)
  {
    return actEXCEPTION_POINT;
  }

  if (NULL_PTR != x)
  {
    actBNOctetString(x, pBytes, tempPoint->x, pDigits);
  }
  if (NULL_PTR != y)
  {
    actBNOctetString(y, pBytes, tempPoint->y, pDigits);
  }

  return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECBD_GENERIC_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIECBD.c
 *********************************************************************************************************************/

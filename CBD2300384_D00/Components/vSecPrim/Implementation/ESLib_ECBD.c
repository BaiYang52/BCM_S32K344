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
/*!        \file  ESLib_ECBD.c
 *        \brief  EC Burmester-Desmedt implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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
 ** EC-B/D key agreement programming interface
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **   esl_initECBD
 **   esl_generateEphemeralKeyPairECBD
 **   esl_calculateIntermediateECBD
 **   esl_initSharedSecretECBD
 **   esl_updateSharedSecretECBD
 **   esl_retrieveSharedSecretECBD
 **
 ***************************************************************************/
#define ESLIB_ECBD_SOURCE
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actIECBD.h"

#if (VSECPRIM_ECBD_GENERIC_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
/* PRQA S 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 *  LOCAL functions
 ***************************************************************************/
VSECPRIM_LOCAL_FUNC( eslt_ErrorCode ) esl_validateWS(VSECPRIM_P2CONST_PARA( eslt_WorkSpaceECBD ) workSpace);

VSECPRIM_LOCAL_FUNC( eslt_ErrorCode ) get_ESL_ERC_from_actEXCEPTION (actRETURNCODE rc);

/**********************************************************************************************************************
 *  esl_initECBD()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace,
                                              const eslt_Size32                nDevices,
                                              const eslt_Size32                deviceID,
                                              VSECPRIM_P2CONST_PARA( eslt_EccDomain ) domain,
                                              VSECPRIM_P2CONST_PARA( eslt_EccDomainExt ) domainExt,
                                              VSECPRIM_P2CONST_PARA( eslt_EccSpeedUpExt ) speedUpExt)
{

    actRETURNCODE    rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate work space */
    if (NULL == workSpace)
    {
      return ESL_ERC_PARAMETER_INVALID;
    }
    if (ESL_MAXSIZEOF_WS_ECBD > workSpace->header.size)
    {
      return ESL_ERC_WS_TOO_SMALL;
    }
    ws = &workSpace->wsECBD;

   /*  validate parameters */
    if ((NULL == domain)
      || (NULL == domainExt)
      || (NULL == speedUpExt))
    {
      return ESL_ERC_PARAMETER_INVALID;
    }

    if ((deviceID <= 0u)
      || (deviceID > nDevices)
      || (3u > nDevices))
    {
      return ESL_ERC_INPUT_INVALID;
    }

   /* initialize actCLib EC-B/D */
    rc = actECBDinit(ws, ESL_MAXSIZEOF_WS_ECBD, nDevices, deviceID, domain, domainExt, speedUpExt);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

   /* set workSpace state */
    workSpace->header.status = (ESL_WST_ALGO_ECBD_KA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);

    return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_generateEphemeralKeyPairECBD()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_generateEphemeralKeyPairECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace,
                                                                  VSECPRIM_P2VAR_PARA( eslt_Byte ) privateKey,
                                                                  VSECPRIM_P2VAR_PARA( eslt_Byte ) publicKey_x,
                                                                  VSECPRIM_P2VAR_PARA( eslt_Byte ) publicKey_y)
{
    eslt_ErrorCode  ec;
    actRETURNCODE   rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate workSpace */
    ec = esl_validateWS(workSpace);
    if (ec != ESL_ERC_NO_ERROR)
    {
      return ec;
    }
    ws = &workSpace->wsECBD;

   /*  validate parameters */
    if ((NULL == publicKey_x)
      || (NULL == publicKey_y))
    {
      return ESL_ERC_PARAMETER_INVALID;
    }

   /* call actCLib EC-B/D key pair generation */
    rc = actECBDgenKeyPair(ws, privateKey, publicKey_x, publicKey_y, workSpace->header.watchdog);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

    return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_calculateIntermediateECBD()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_calculateIntermediateECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace,
                                                               VSECPRIM_P2CONST_PARA( eslt_Byte ) pubKeyLeft_x,
                                                               VSECPRIM_P2CONST_PARA( eslt_Byte ) pubKeyLeft_y,
                                                               VSECPRIM_P2CONST_PARA( eslt_Byte ) pubKeyRight_x,
                                                               VSECPRIM_P2CONST_PARA( eslt_Byte ) pubKeyRight_y,
                                                               VSECPRIM_P2VAR_PARA( eslt_Byte ) intermediate_x,
                                                               VSECPRIM_P2VAR_PARA( eslt_Byte ) intermediate_y) {
    eslt_ErrorCode  ec;
    actRETURNCODE   rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate workSpace */
    ec = esl_validateWS(workSpace);
    if (ec != ESL_ERC_NO_ERROR)
    {
      return ec;
    }
    ws = &workSpace->wsECBD;

   /*  validate parameters */
    if ((NULL == pubKeyLeft_x)
      || (NULL == pubKeyLeft_y)
      || (NULL == pubKeyRight_x)
      || (NULL == pubKeyRight_y)
      || (NULL == intermediate_x)
      || (NULL == intermediate_y))
    {
      return ESL_ERC_PARAMETER_INVALID;
    }

   /* call actCLib EC-B/D intermediate calculation */
    rc = actECBDcalcXi(ws, pubKeyLeft_x, pubKeyLeft_y, pubKeyRight_x, pubKeyRight_y, intermediate_x, intermediate_y, workSpace->header.watchdog);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

    return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_initSharedSecretECBD()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_initSharedSecretECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace) {

    eslt_ErrorCode  ec;
    actRETURNCODE   rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate workSpace */
    ec = esl_validateWS(workSpace);
    if (ec != ESL_ERC_NO_ERROR)
    {
      return ec;
    }
    ws = &workSpace->wsECBD;

   /* call actCLib EC-B/D K initialization */
    rc = actECBDinitK(ws, workSpace->header.watchdog);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

    return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_updateSharedSecretECBD()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_updateSharedSecretECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace,
                                                            const eslt_Size32          deviceID,
                                                            VSECPRIM_P2CONST_PARA( eslt_Byte ) intermediate_x,
                                                            VSECPRIM_P2CONST_PARA( eslt_Byte ) intermediate_y)
{

    eslt_ErrorCode  ec;
    actRETURNCODE   rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate workSpace */
    ec = esl_validateWS(workSpace);
    if (ec != ESL_ERC_NO_ERROR)
    {
      return ec;
    }
    ws = &workSpace->wsECBD;

   /*  validate parameters */
    if ((NULL == intermediate_x)
      || (NULL == intermediate_y))
    {
      return ESL_ERC_PARAMETER_INVALID;
    }

    if ((deviceID <= 0u)
      || (deviceID > ws->bd.nECUs))
    {
      return ESL_ERC_INPUT_INVALID;
    }

   /* call actCLib EC-B/D K update */
    rc = actECBDupdateK(ws, deviceID, intermediate_x, intermediate_y, workSpace->header.watchdog);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

    return ESL_ERC_NO_ERROR;
}

/**********************************************************************************************************************
 *  esl_retrieveSharedSecretECBD()
 *********************************************************************************************************************/
 /*!
  *
  * Internal comment removed.
 *
 *
 *
 *
  */
VSECPRIM_FUNC( eslt_ErrorCode ) esl_retrieveSharedSecretECBD (VSECPRIM_P2VAR_PARA( eslt_WorkSpaceECBD ) workSpace,
                                                              VSECPRIM_P2VAR_PARA( eslt_Byte ) sharedSecret_x,
                                                              VSECPRIM_P2VAR_PARA( eslt_Byte ) sharedSecret_y)
{
    eslt_ErrorCode  ec;
    actRETURNCODE   rc;
    VSECPRIM_P2VAR_PARA( actECBDstruct ) ws;

   /*  validate workSpace */
    ec = esl_validateWS(workSpace);
    if (ec != ESL_ERC_NO_ERROR)
    {
      return ec;
    }
    ws = &workSpace->wsECBD;

   /*  validate parameters */
    if (NULL == sharedSecret_x)
      /*  ||  (NULL == intermediate_y))     */     /*  optional! */
    {
      return ESL_ERC_PARAMETER_INVALID;
    }

   /* call actCLib EC-B/D K retrieval */
    rc = actECBDretrieveK(ws, sharedSecret_x, sharedSecret_y, workSpace->header.watchdog);
    if (rc != actOK)
    {
      return get_ESL_ERC_from_actEXCEPTION(rc);
    }

    return ESL_ERC_NO_ERROR;
}

/****************************************************************************
 *  LOCAL functions
 ***************************************************************************/

VSECPRIM_LOCAL_FUNC( eslt_ErrorCode ) esl_validateWS (VSECPRIM_P2CONST_PARA( eslt_WorkSpaceECBD ) workSpace)
{
  if (NULL == workSpace)
  {
    return ESL_ERC_PARAMETER_INVALID;
  }

    if ((ESL_WST_ALGO_ECBD_KA != (workSpace->header.status & ESL_WST_M_ALGO))
    ||  (ESL_WST_M_RUNNING    != (workSpace->header.status & ESL_WST_M_RUNNING)))
    {
        return ESL_ERC_WS_STATE_INVALID;
    }

    if (ESL_MAXSIZEOF_WS_ECBD > workSpace->header.size)
    {
        return ESL_ERC_WS_TOO_SMALL;
    }

    return ESL_ERC_NO_ERROR;
}

VSECPRIM_LOCAL_FUNC( eslt_ErrorCode ) get_ESL_ERC_from_actEXCEPTION (actRETURNCODE rc)
{
  eslt_ErrorCode retVal;

    switch (rc)
    {
    case actEXCEPTION_UNKNOWN:
      retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      break;
    case actEXCEPTION_NULL:
      retVal = ESL_ERC_PARAMETER_INVALID;
      break;
    case actEXCEPTION_MEMORY:
      retVal = ESL_ERC_WS_TOO_SMALL;
      break;
    case actEXCEPTION_DOMAIN:
      retVal = ESL_ERC_ECC_DOMAIN_INVALID;
      break;
    case actEXCEPTION_DOMAIN_EXT:
      retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
      break;
    case actEXCEPTION_SPEEDUP_EXT:
      retVal = ESL_ERC_ECC_SPEEDUPEXT_INVALID;
      break;
    case actEXCEPTION_POINT:
      retVal = ESL_ERC_ECC_INTERNAL_ERROR;
      break;
    case actEXCEPTION_RANDOM:
      retVal = ESL_ERC_RNG_FAILED;
      break;
    default:
      retVal = ESL_ERC_ERROR;
      break;
    }

    return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ECBD_GENERIC_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_ECBD.c
 *********************************************************************************************************************/

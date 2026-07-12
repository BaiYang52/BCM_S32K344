/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actSPAKE2P.c
 *        \brief  Implementation file for actISPAKE2P.h
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
#define ACTISPAKE2P_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actISPAKE2P.h"
#include "actECTools.h"
#include "actECPoint.h"
#include "actECLengthInfo.h"
#include "actISHA2_32.h"
#include "actIHKDF_HMAC.h"
#include "actICMACAES.h"
#include "actUtilities.h"
#include "actConfig.h"

#if (VSECPRIM_ACTSPAKE2P_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define actSPAKE2P_CURVEP256_P_BYTE_LENGTH                           (32u)
# define actSPAKE2P_32BIT_BYTE_LENGTH                                 (4u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* If the field prime is p_bytes many bytes long, the length of an encoded point is given
 * by (2*p_bytes + 1), since the encoded point contains two coordinates and one encoding byte */
# define actSSPAKE2PEncodedPointLength(fieldPrimeLength)              ((2u*fieldPrimeLength) + 1u)

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) confirmationKeyString[16] = {
  0x43, 0x6f, 0x6e, 0x66, 0x69, 0x72, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x4b, 0x65, 0x79, 0x73
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actSPAKE2PCalcCommonValues()
 **********************************************************************************************************************/
/*! \brief       Computes both parties' common values Z and V.
 *  \details     For party A, Z = h*x*(Y-w0*N) and V = h*w1*(Y-w0*N). For party B, Z = h*y*(X-w0*M) and V = h*y*L.
 *  \param[in,out] wsSpake          (in)  Pointer to SPAKE2P context structure
 *                                  (out) Pointer to SPAKE2P context structure, member Z and V updated
 *  \param[in]     watchdog         Watchdog trigger function pointer
 *  \return        actEXCEPTION_POINT computations failed
 *                 actOK              computation successful
 *  \pre         wsSpake must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSPAKE2PCalcCommonValues(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actSPAKE2PHashPoint()
 **********************************************************************************************************************/
/*! \brief       Auxiliary function to feed a curve point in uncompressed representation to the hash workspace.
 *  \details     The SPAKE2+ protocol also requires the length value of the point to be in 8-byte LE representation
 *               before the length value and the coordinates are hashed. This function also sets up such a length
 *               representation.
 *  \param[in,out] wsSpake          (in)  Pointer to SPAKE2P context structure
 *                                  (out) Pointer to SPAKE2P context structure
 *  \param[in,out] wsHash           (in)  Pointer to SHA256 context structure
 *                                  (out) Pointer to SHA256 context structure
 *  \param[in]     point            Pointer to the point whose coordinates shall be hashed
 *  \param[in]     watchdog         Pointer to watchdog trigger function
 *  \pre         All provided pointers must be valid.
 *               The hash workspace wsHash must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PHashPoint(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2CONST_PARA(actECPprojectivePointBuffer) point,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actSPAKE2PHashTT()
 **********************************************************************************************************************/
/*! \brief       Computes Hash(TT), where TT = infoA || infoB || X || Y || Z || V || w0.
 *  \details     infoA and infoB, if given, are already hashed during the call to actSPAKE2PSetAdditionalInformation.
 *               X, Y, Z, V and w0 are members of the workspace wsSpake.
 *  \param[in,out] wsSpake          (in)  Pointer to SPAKE2P context structure
 *                                  (out) Pointer to SPAKE2P context structure
 *  \param[in,out] wsHash           (in)  Pointer to SHA256 context structure
 *                                  (out) Pointer to SHA256 context structure
 *  \param[in,out] hashTT           (in)  Pointer to buffer to store hash value TT
 *                                  (out) Pointer to buffer, storing the hash value TT
 *  \param[in]     watchdog         Pointer to watchdog trigger function
 *  \pre         Pointers must be valid.
 *               The buffer referenced by hashTT must provide at least actHASH_SIZE_SHA256 bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PHashTT(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2VAR_PARA(actU8) hashTT,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSPAKE2PCalcCmac()
 *********************************************************************************************************************/
/*! \brief       Computes CMACAES128([0x04 || pointXCoord || pointYCoord]) based on the given macKey.
 *  \details     The point is the other party's public value.
 *  \param[in]     wsSpake          Pointer to SPAKE2P context structure
 *  \param[in]     macKey           key for CMAC computation
 *  \param[in]     pointXCoord      Pointer to the x-coordinate of the point in actBNDIIGIT representation
 *  \param[in]     pointYCoord      Pointer to the y-coordinate of the point in actBNDIIGIT representation
 *  \param[in,out] pointBuffer      (in)  Pointer to buffer to store a coordinate of the point in byte-wise
 *                                        representation
 *                                  (out) Pointer to buffer, storing the y-coordinate of the point in byte-wise
 *                                        representation
 *  \param[in,out] wsCMAC           (in)  CMAC context structure
                                    (out) CMAC context structure
 *  \param[in,out] mac              (in)  Pointer to buffer to store the resulting CMAC
 *                                  (out) Pointer to buffer, storing the resulting CMAC
 *  \param[in]     watchdog         Pointer to watchdog trigger function
 *  \pre         Pointers must be valid.
 *               The buffer referenced by pointBuffer must provide at least
 *               wsSpake->baseData.Curve.p_field.m_byte_length many bytes.
 *               The buffer referenced by mac must provide at least actAES_BLOCK_SIZE bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PCalcCmac(
  VSECPRIM_P2CONST_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2CONST_PARA(actU8) macKey,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) pointXCoord,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) pointYCoord,
  VSECPRIM_P2VAR_PARA(actU8) pointBuffer,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2VAR_PARA(actU8) mac,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actSPAKE2PCalcCommonValues()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSPAKE2PCalcCommonValues(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actECPOINT publicValueOtherParty, curvePoint, result, Z, V, L;
  actLengthType p_length = wsSpake->baseData.Curve.p_field.m_length;

  /* auxiliary buffer to copy the public value of the other party,
   * because the public value itself will also be used during the key confirmation phase and
   * shall not be modified before */
  publicValueOtherParty.x = wsSpake->tmpVariables.tmp2.xcoord;
  publicValueOtherParty.y = wsSpake->tmpVariables.tmp2.ycoord;
  publicValueOtherParty.z = wsSpake->tmpVariables.tmp2.zcoord;
  publicValueOtherParty.is_affine = FALSE;
  publicValueOtherParty.is_infinity = FALSE;

  Z.x = wsSpake->Z.xcoord;
  Z.y = wsSpake->Z.ycoord;
  Z.z = wsSpake->Z.zcoord;
  Z.is_affine = FALSE;
  Z.is_infinity = FALSE;

  V.x = wsSpake->V.xcoord;
  V.y = wsSpake->V.ycoord;
  V.z = wsSpake->V.zcoord;
  V.is_affine = FALSE;
  V.is_infinity = FALSE;

  L.x = wsSpake->L.xcoord;
  L.y = wsSpake->L.ycoord;
  L.is_affine = TRUE;
  L.is_infinity = FALSE;

  result.x = wsSpake->tmp1.xcoord;
  result.y = wsSpake->tmp1.ycoord;
  result.z = wsSpake->tmp1.zcoord;
  result.is_affine = FALSE;
  result.is_infinity = FALSE;

  /* compute common values Z and V */
  if (wsSpake->isPartyA == TRUE)
  {
    /* Copy public value because the public value will also be used during key confirmation phase */
    actBNCopy(publicValueOtherParty.x, wsSpake->pB.xcoord, p_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNCopy(publicValueOtherParty.y, wsSpake->pB.ycoord, p_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOne(publicValueOtherParty.z, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    curvePoint.x = wsSpake->N.xcoord;
    curvePoint.y = wsSpake->N.ycoord;
    curvePoint.is_affine = TRUE;
    curvePoint.is_infinity = FALSE;

    actECPToMont(&publicValueOtherParty, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPToMont(&curvePoint, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* cofactor for P256 is 1, so multiplying by the cofactor is obsolete */
    /* Compute Z = h * x * (Y - w_0 * N) */
    actECPMult(&curvePoint, wsSpake->w0, &result, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPSub(&publicValueOtherParty, &result, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPMult(&publicValueOtherParty, wsSpake->randomNumber, &Z, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    retVal = actECPToAffineFromMont(&Z, &(wsSpake->baseData.Curve), FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {
      /* Compute V = h*w1*(Y - w_0*N); (Y - w_0*N) is still stored in publicValueOtherParty */
      actECPMult(&publicValueOtherParty, wsSpake->w1, &V, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      retVal = actECPToAffineFromMont(&V, &(wsSpake->baseData.Curve), FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }
  else
  {
    /* Copy public value because the public value will also be used during key confirmation phase */
    actBNCopy(publicValueOtherParty.x, wsSpake->pA.xcoord, p_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNCopy(publicValueOtherParty.y, wsSpake->pA.ycoord, p_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOne(publicValueOtherParty.z, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    curvePoint.x = wsSpake->M.xcoord;
    curvePoint.y = wsSpake->M.ycoord;
    curvePoint.is_affine = TRUE;
    curvePoint.is_infinity = FALSE;

    /* cofactor for P256 is 1, so multiplying by cofactor is obsolete */
    /* Compute Z = h * y * (X - w_0 * M) */
    actECPToMont(&publicValueOtherParty, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPToMont(&curvePoint, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    actECPMult(&curvePoint, wsSpake->w0, &result, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPSub(&publicValueOtherParty, &result, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actECPMult(&publicValueOtherParty, wsSpake->randomNumber, &Z, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    retVal = actECPToAffineFromMont(&Z, &(wsSpake->baseData.Curve), FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {
      /* Compute V = h*y*L */
      actECPToMont(&L, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actECPMult(&L, wsSpake->randomNumber, &V, &(wsSpake->baseData.Curve), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      retVal = actECPToAffineFromMont(&V, &(wsSpake->baseData.Curve), FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actSPAKE2PHashPoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PHashPoint(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2CONST_PARA(actECPprojectivePointBuffer) point,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 lengthBuffer[4];
  actU8 allZeroBuffer[4];
  actU8 encodingByte = 0x04u;

  actLengthType p_length = wsSpake->baseData.Curve.p_field.m_length;
  actLengthType p_bytes = wsSpake->baseData.Curve.p_field.m_byte_length;

  actU8 * tmpBuffer = wsSpake->pointToByteString;

  actMemClear(allZeroBuffer, 4); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  /* Hash length of X in 8-byte LE representation: len(X) must be the length of the uncompressed representation of X, i.e.,
  * 2*(length of one coordinate) + 1 byte for encoding */
  /* hash len(X) || X */
  actU32toLE(lengthBuffer, ((2u * p_bytes) + 1u)); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
  (void)actSHA256Update(wsHash, lengthBuffer, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  (void)actSHA256Update(wsHash, allZeroBuffer, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  (void)actSHA256Update(wsHash, &encodingByte, 1, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actBNOctetString(tmpBuffer, p_bytes, point->xcoord, p_length);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actSHA256Update(wsHash, tmpBuffer, p_bytes, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actBNOctetString(tmpBuffer, p_bytes, point->ycoord, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actSHA256Update(wsHash, tmpBuffer, p_bytes, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actSPAKE2PHashTT()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PHashTT(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2VAR_PARA(actU8) hashTT,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Note: The string TT requires length information to be of size 8 byte. */

  actU8 lengthBuffer[actSPAKE2P_32BIT_BYTE_LENGTH];
  actU8 allZeroBuffer[actSPAKE2P_32BIT_BYTE_LENGTH];

  actLengthType n_length = wsSpake->baseData.Curve.n_field.m_length;
  actLengthType n_bytes = wsSpake->baseData.Curve.n_field.m_byte_length;

  actMemClear(allZeroBuffer, actSPAKE2P_32BIT_BYTE_LENGTH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  /* Note: wsHash is initialized in actSPAKE2PSetAdditionalInformation, if the function is called.
   * Also, A's and B's names are hashed there (if provided).
   * If the function was not called, the SHA workspace needs to be initialized here. */
  if (wsSpake->setAdditionalInformationWasCalled == FALSE)
  {
    (void)actSHA256Init(wsHash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  actSPAKE2PHashPoint(wsSpake, wsHash, &wsSpake->pA, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  actSPAKE2PHashPoint(wsSpake, wsHash, &wsSpake->pB, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  actSPAKE2PHashPoint(wsSpake, wsHash, &wsSpake->Z, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  actSPAKE2PHashPoint(wsSpake, wsHash, &wsSpake->V, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actU32toLE(lengthBuffer, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
  (void)actSHA256Update(wsHash, lengthBuffer, actSPAKE2P_32BIT_BYTE_LENGTH, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  (void)actSHA256Update(wsHash, allZeroBuffer, actSPAKE2P_32BIT_BYTE_LENGTH, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actBNOctetString(wsSpake->pointToByteString, n_bytes, wsSpake->w0, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actSHA256Update(wsHash, wsSpake->pointToByteString, n_bytes, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  (void)actSHA256Finalize(wsHash, hashTT, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actSPAKE2PCalcCmac()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSPAKE2PCalcCmac(
  VSECPRIM_P2CONST_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2CONST_PARA(actU8) macKey,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) pointXCoord,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) pointYCoord,
  VSECPRIM_P2VAR_PARA(actU8) pointBuffer,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2VAR_PARA(actU8) mac,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 encodingByte = 0x04u;
  actLengthType p_length = wsSpake->baseData.Curve.p_field.m_length;
  actLengthType p_bytes = wsSpake->baseData.Curve.p_field.m_byte_length;

  /* actCMACAESInit returns actOK, since macKey is a valid pointer and actAES128_KEY_SIZE is a valid key size */
  (void)actCMACAESInit(wsCMAC, macKey, actAES128_KEY_SIZE, watchdog);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  /* actCMACAESUpdate always returns actOK */
  (void)actCMACAESUpdate(wsCMAC, &encodingByte, 1u, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actBNOctetString(pointBuffer, p_bytes, pointXCoord, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actCMACAESUpdate(wsCMAC, pointBuffer, p_bytes, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  actBNOctetString(pointBuffer, p_bytes, pointYCoord, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  (void)actCMACAESUpdate(wsCMAC, pointBuffer, p_bytes, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* actCMACAESFinalize always returns actOK */
  (void)actCMACAESFinalize(wsCMAC, mac, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actSPAKE2PPreambleInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PPreambleInit(
  VSECPRIM_P2VAR_PARA(actSPAKE2PPreambleStruct) wksp,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  actLengthType wksp_len)
{
  actRETURNCODE retVal;
  actLengthType p_byte_length;

  retVal = actECInit(domain, domain_ext, (VSECPRIM_P2CONST_PARA(actU8)) NULL_PTR,
                     actEC_ALGO_FLAG_SPAKE2P, &(wksp->baseData), wksp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (retVal == actOK)
  {
    p_byte_length = wksp->baseData.Curve.p_field.m_byte_length;
    if (p_byte_length != actSPAKE2P_CURVEP256_P_BYTE_LENGTH)
    {
      retVal = actEXCEPTION_LENGTH;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actSPAKE2PPreambleCalc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PPreambleCalc(
  VSECPRIM_P2VAR_PARA(actSPAKE2PPreambleStruct) wksp,
  VSECPRIM_P2CONST_PARA(actU8) pbkdfOutput,
  const actLengthType pbkdfOutputLength,
  VSECPRIM_P2VAR_PARA(actU8) w0OutputBuffer,
  VSECPRIM_P2VAR_PARA(actLengthType) w0BufferLength,
  VSECPRIM_P2VAR_PARA(actU8) w1OutputBuffer,
  VSECPRIM_P2VAR_PARA(actLengthType) w1BufferLength,
  VSECPRIM_P2VAR_PARA(actU8) pointL,
  VSECPRIM_P2VAR_PARA(actLengthType) pointLBufferLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actLengthType wDigitLength;

  actLengthType p_ByteLength = wksp->baseData.Curve.p_field.m_byte_length;
  actLengthType p_DigitLength = wksp->baseData.Curve.p_field.m_length;
  actLengthType n_ByteLength = wksp->baseData.Curve.n_field.m_byte_length;
  actLengthType n_DigitLength = wksp->baseData.Curve.n_field.m_length;

  actECPOINT P, L;

  if ((pbkdfOutputLength == 0u) || (pbkdfOutputLength > actSPAKE2P_PREAMBLE_OKM_LENGTH))
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else if ((*w0BufferLength < n_ByteLength) ||
           (*w1BufferLength < n_ByteLength) ||
           (*pointLBufferLength < actSSPAKE2PEncodedPointLength(p_ByteLength)))
  {
    retVal = actEXCEPTION_MEMORY;
  }
  else
  {
    /* pbkdfOutoutLength is even, because this was checked in esl_reduceSPAKE2PPreamble */
    wDigitLength = ((pbkdfOutputLength / 2u) + actBN_BYTES_PER_DIGIT - 1u) / actBN_BYTES_PER_DIGIT;

    actBNSetOctetString(wksp->digitsW0, wDigitLength, pbkdfOutput, (pbkdfOutputLength / 2u)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOctetString(wksp->digitsW1, wDigitLength, &pbkdfOutput[(pbkdfOutputLength / 2u)], (pbkdfOutputLength / 2u)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    retVal = actBNReduce(wksp->digitsW0, wDigitLength, wksp->baseData.n_field_modulus,
                         n_DigitLength, wksp->result, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    if (retVal == actOK)
    {
      actBNOctetString(w0OutputBuffer, n_ByteLength, wksp->result, n_DigitLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      *w0BufferLength = n_ByteLength; /* SBSW_VSECPRIM_WRITE_POINTER */

      retVal = actBNReduce(wksp->digitsW1, wDigitLength, wksp->baseData.n_field_modulus,
                           n_DigitLength, wksp->result, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      if (retVal == actOK)
      {
        actBNOctetString(w1OutputBuffer, n_ByteLength, wksp->result, n_DigitLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *w1BufferLength = n_ByteLength; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

    }
  }

  if (retVal == actOK)
  {
    P.x = wksp->basePoint.xcoord;
    P.y = wksp->basePoint.ycoord;

    actBNSetOctetStringROM(P.x, p_DigitLength, wksp->baseData.Curve.G_R, p_ByteLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOctetStringROM(P.y, p_DigitLength, &(wksp->baseData.Curve.G_R[p_ByteLength]), p_ByteLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    P.is_affine = TRUE;
    P.is_infinity = FALSE;

    L.x = wksp->L.xcoord;
    L.y = wksp->L.ycoord;
    L.z = wksp->L.zcoord;
    L.is_affine = FALSE;
    L.is_infinity = FALSE;

    /* compute random number * base point */
    actECPMult(&P, wksp->result, &L, &wksp->baseData.Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    retVal = actECPToAffineFromMont(&L, &wksp->baseData.Curve, FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {
      pointL[0] = 0x04u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
      actBNOctetString(&pointL[1], p_ByteLength, L.x, p_DigitLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNOctetString(&pointL[p_ByteLength + 1u], p_ByteLength, L.y, p_DigitLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      *pointLBufferLength = actSSPAKE2PEncodedPointLength(p_ByteLength); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actSPAKE2PInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PInit(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wksp,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2CONST_PARA(actU8) pointM,
  const actLengthType mlength,
  VSECPRIM_P2CONST_PARA(actU8) pointN,
  const actLengthType nlength,
  actLengthType wksp_len)
{
  actRETURNCODE retVal;
  actLengthType p_bytes;

  retVal = actECInit(domain, domain_ext, (VSECPRIM_P2CONST_PARA(actU8)) NULL_PTR,
                     actEC_ALGO_FLAG_SPAKE2P, &(wksp->baseData), wksp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  if (retVal == actOK)
  {
    p_bytes = wksp->baseData.Curve.p_field.m_byte_length;

    if (p_bytes != actSPAKE2P_CURVEP256_P_BYTE_LENGTH)
    {
      retVal = actEXCEPTION_LENGTH;
    }
    /* mlength > 0, mlength odd checked in esl_initSPAKE2P */
    else if ((mlength != actSSPAKE2PEncodedPointLength(p_bytes)) ||
             (nlength != actSSPAKE2PEncodedPointLength(p_bytes)))
    {
      retVal = actEXCEPTION_INPUT_LENGTH;
    }
    else
    {
      /* store points M and N in workspace */

      actBNSetOctetString(wksp->M.xcoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                          &pointM[1], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetString(wksp->M.ycoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                          &pointM[p_bytes + 1u], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOne(wksp->M.zcoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes))); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      actBNSetOctetString(wksp->N.xcoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                          &pointN[1], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetString(wksp->N.ycoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                          &pointN[p_bytes + 1u], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOne(wksp->N.zcoord, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes))); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actSPAKE2PSetPreambleData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PSetPreambleData(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2CONST_PARA(actU8) w0,
  const actLengthType w0length,
  VSECPRIM_P2CONST_PARA(actU8) w1,
  const actLengthType w1length,
  VSECPRIM_P2CONST_PARA(actU8) pointL,
  const actLengthType pointLLength)
{
  actRETURNCODE retVal = actOK;
  actLengthType p_length, p_bytes, n_bytes;

  actECPOINT P, L;

  n_bytes = wsSpake->baseData.Curve.n_field.m_byte_length;

  p_length = wsSpake->baseData.Curve.p_field.m_length;
  p_bytes = wsSpake->baseData.Curve.p_field.m_byte_length;

  if (w0length != n_bytes)
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else
  {
    actBNSetOctetString(wsSpake->w0, actBNGetDigitLengthFromBit(actBytesToBits(w0length)), w0, w0length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* store base point in workspace; the length of P is checked during actECInit
     * Note: The base point is already in Montgomery representation! */
    P.x = wsSpake->basePoint.xcoord;
    P.y = wsSpake->basePoint.ycoord;

    actBNSetOctetStringROM(P.x, p_length, wsSpake->baseData.Curve.G_R, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOctetStringROM(P.y, p_length, &(wsSpake->baseData.Curve.G_R[p_bytes]), p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* if w1 is given and L is not, compute L as w1*P */
    if (w1 != NULL_PTR)
    {
      if (w1length != n_bytes)
      {
        retVal = actEXCEPTION_INPUT_LENGTH;
      }
      else
      {
        wsSpake->isPartyA = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
        actBNSetOctetString(wsSpake->w1, actBNGetDigitLengthFromBit(actBytesToBits(w1length)), w1, w1length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      }
    }
    else if (pointL != NULL_PTR)
    {
      if (pointLLength != actSSPAKE2PEncodedPointLength(p_bytes))
      {
        retVal = actEXCEPTION_INPUT_LENGTH;
      }
      else
      {
        wsSpake->isPartyA = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */

        L.x = wsSpake->L.xcoord;
        L.y = wsSpake->L.ycoord;

        /* store pointL in workspace */
        actBNSetOctetString(L.x, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                            &pointL[1], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actBNSetOctetString(L.y, actBNGetDigitLengthFromBit(actBytesToBits(p_bytes)),
                            &pointL[p_bytes + 1u], p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      }
    }
    else
    {
      /* Either w1 or L must be given */
      retVal = actEXCEPTION_NULL;
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actSPAKE2PCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PCalcPubVal(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actU8) pubValXCoordinate,
  VSECPRIM_P2VAR_PARA(actU8) pubValYCoordinate,
  VSECPRIM_P2VAR_PARA(actLengthType) pubValCoordinateLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actECCURVE * Curve = &(wsSpake->baseData.Curve);

  /* curvePoint will be either M or N, depending on the flag */
  actECPOINT P, curvePoint, pubVal, multBasePoint;

  actLengthType p_length = wsSpake->baseData.Curve.p_field.m_length;
  actLengthType p_bytes = wsSpake->baseData.Curve.p_field.m_byte_length;

  if (*pubValCoordinateLength < p_bytes)
  {
    retVal = actEXCEPTION_OUTPUT_BUFFER;
  }
  else
  {
    /* reference base point p_RR by P */
    P.x = wsSpake->basePoint.xcoord;
    P.y = wsSpake->basePoint.ycoord;
    P.is_affine = TRUE;
    P.is_infinity = FALSE;

    /* auxiliary point to store randomNumber * basePoint */
    multBasePoint.x = wsSpake->kG.xcoord;
    multBasePoint.y = wsSpake->kG.ycoord;
    multBasePoint.z = wsSpake->kG.zcoord;
    multBasePoint.is_affine = FALSE;
    multBasePoint.is_infinity = FALSE;

    /* Choose random number; store random number (x or y) in workspace because it will be used again for the computation of
     * of the common values Z and V */
    retVal = actBNModRandomize(wsSpake->randomNumber, &(Curve->n_field), Curve->t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {
      if (wsSpake->isPartyA == TRUE)
      {
        pubVal.x = &(wsSpake->pA.xcoord[0]);
        pubVal.y = &(wsSpake->pA.ycoord[0]);
        pubVal.z = &(wsSpake->pA.zcoord[0]);

        curvePoint.x = &(wsSpake->M.xcoord[0]);
        curvePoint.y = &(wsSpake->M.ycoord[0]);
      }
      else
      {
        pubVal.x = &(wsSpake->pB.xcoord[0]);
        pubVal.y = &(wsSpake->pB.ycoord[0]);
        pubVal.z = &(wsSpake->pB.zcoord[0]);

        curvePoint.x = &(wsSpake->N.xcoord[0]);
        curvePoint.y = &(wsSpake->N.ycoord[0]);
      }

      pubVal.is_affine = FALSE;
      pubVal.is_infinity = FALSE;

      curvePoint.is_affine = TRUE;
      curvePoint.is_infinity = FALSE;

      actECPToMont(&curvePoint, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* compute random number * base point */
      actECPMult(&P, wsSpake->randomNumber, &multBasePoint, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      /* compute w0 * M / w0 * N */
      actECPMult(&curvePoint, wsSpake->w0, &pubVal, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      actECPAdd(&pubVal, &multBasePoint, Curve, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      retVal = actECPToAffineFromMont(&pubVal, Curve, FALSE, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      if (retVal == actOK)
      {
        /* write public value to output buffer; the public value is also stored in the workspace */
        actBNOctetString(pubValXCoordinate, p_bytes, pubVal.x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actBNOctetString(pubValYCoordinate, p_bytes, pubVal.y, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *pubValCoordinateLength = p_bytes; /* SBSW_VSECPRIM_WRITE_POINTER */
      }
    }
  }

  wsSpake->setAdditionalInformationWasCalled = FALSE; /* SBSW_VSECPRIM_WRITE_POINTER */
  return retVal;
}

/**********************************************************************************************************************
 *  actSPAKE2PSetAdditionalInformation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PSetAdditionalInformation(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2CONST_PARA(actU8) infoA,
  const actLengthType infoALength,
  VSECPRIM_P2CONST_PARA(actU8) infoB,
  const actLengthType infoBLength,
  VSECPRIM_P2CONST_PARA(actU8) aad,
  const actLengthType aadLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;
  actU8 lengthBuffer[4];
  actU8 allZeros[4];

  actMemClear(allZeros, 4); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */
  (void)actSHA256Init(wsHash); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  /* If A and/or B is given, hash A and/or B */
  if ((infoA != NULL_PTR) && (infoALength > 0u))
  {
    /* Hash length of A in 8-byte LE representation */
    actU32toLE(lengthBuffer, infoALength); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    (void)actSHA256Update(wsHash, lengthBuffer, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA256Update(wsHash, allZeros, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    (void)actSHA256Update(wsHash, infoA, infoALength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if ((infoB != NULL_PTR) && (infoBLength > 0u))
  {
    /* Hash length of B in 8-byte LE representation */
    actU32toLE(lengthBuffer, infoBLength); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER */
    (void)actSHA256Update(wsHash, lengthBuffer, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA256Update(wsHash, allZeros, 4, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    (void)actSHA256Update(wsHash, infoB, infoBLength, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  if (aadLength > actSPAKE2P_MAX_AAD_SIZE)
  {
    wsSpake->aadLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else
  {
    /* Copy AAD to workspace */
    actMemCpyByteArray(wsSpake->addInHKDF, confirmationKeyString, 16); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    if ((aad != NULL_PTR) && (aadLength > 0u))
    {
      actMemCpyByteArray(&(wsSpake->addInHKDF[16]), aad, aadLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      wsSpake->aadLength = aadLength; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      wsSpake->aadLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  wsSpake->setAdditionalInformationWasCalled = TRUE; /* SBSW_VSECPRIM_WRITE_POINTER */
  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actSPAKE2PCalcSharedSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PCalcSharedSecret(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actSHA256STRUCT) wsHash,
  VSECPRIM_P2VAR_PARA(actHKDFSTRUCT) wsHKDF,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2CONST_PARA(actU8) pubValXCoordinate,
  VSECPRIM_P2CONST_PARA(actU8) pubValYCoordinate,
  const actLengthType pubValCoordinateLength,
  VSECPRIM_P2VAR_PARA(actU8) sharedSecret,
  VSECPRIM_P2VAR_PARA(actU8) confirmationMAC,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;
  actU8 * hashTT = wsSpake->tmpVariables.hashTT;
  actLengthType p_length = wsSpake->baseData.Curve.p_field.m_length;
  actLengthType p_bytes = wsSpake->baseData.Curve.p_field.m_byte_length;

  if (pubValCoordinateLength != p_bytes)
  {
    retVal = actEXCEPTION_INPUT_LENGTH;
  }
  else
  {
    if (wsSpake->isPartyA == TRUE)
    {
      actBNSetOctetString(wsSpake->pB.xcoord, p_length, pubValXCoordinate, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetString(wsSpake->pB.ycoord, p_length, pubValYCoordinate, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
    else
    {
      actBNSetOctetString(wsSpake->pA.xcoord, p_length, pubValXCoordinate, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetString(wsSpake->pA.ycoord, p_length, pubValYCoordinate, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }

    /* 1: Calculate common values Z and V */
    retVal = actSPAKE2PCalcCommonValues(wsSpake, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (retVal == actOK)
    {

      /* 2: Compute shared secret and MAC keys */
      actSPAKE2PHashTT(wsSpake, wsHash, hashTT, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* 3: derive keys: Ke is the second half of hashTT, write Ke to output buffer sharedSecret */
      actMemCpyByteArray(sharedSecret, &hashTT[actHASH_SIZE_SHA256 / 2u], actHASH_SIZE_SHA256 / 2u); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* 4: Derive keys KcA and KcB for CMAC; those keys will be used during the key confirmation phase
       * For HKDF: secret = Ka, salt = NULL, additionalInput = "ConfirmationKeys" + AAD
       * (Ka is the first half of hashTT) */
      if (wsSpake->setAdditionalInformationWasCalled == FALSE)
      {
        actMemCpyByteArray(wsSpake->addInHKDF, confirmationKeyString, 16); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        wsSpake->aadLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      actHKDF_HMAC_Extract(wsHKDF, hashTT, actHASH_SIZE_SHA256 / 2u, NULL_PTR, 0u, actHASHMACSHA256_ALGO_ID, watchdog); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      /* actHKDF_HMAC_SHA256_Expand returns actEXCEPTION_LENGTH, if the desired key length exceeds 255*32,
       * 2u * actAES128_KEY_SIZE = 32 < 255*32; Thus, the function can be voided. */
      (void)actHKDF_HMAC_Expand(wsHKDF, wsSpake->addInHKDF, (16u + wsSpake->aadLength),
                                       wsSpake->macKeys, 2u * actAES128_KEY_SIZE, actHASHMACSHA256_ALGO_ID, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM */

      /* compute CMAC(own key, other party's public value) */
      if (wsSpake->isPartyA == TRUE)
      {
        if ((wsSpake->ciphersuite & actSPAKE2P_VERSION_MASK) == actSPAKE2P_VERSION_KEY_A_FIRST)
        {
          actSPAKE2PCalcCmac(wsSpake, wsSpake->macKeys, wsSpake->pB.xcoord, wsSpake->pB.ycoord,
                             wsSpake->pointToByteString, wsCMAC, confirmationMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
        else
        {
          actSPAKE2PCalcCmac(wsSpake, &wsSpake->macKeys[actAES128_KEY_SIZE], wsSpake->pB.xcoord, wsSpake->pB.ycoord,
                             wsSpake->pointToByteString, wsCMAC, confirmationMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
      }
      else
      {
        if ((wsSpake->ciphersuite & actSPAKE2P_VERSION_MASK) == actSPAKE2P_VERSION_KEY_A_FIRST)
        {
          actSPAKE2PCalcCmac(wsSpake, &wsSpake->macKeys[actAES128_KEY_SIZE], wsSpake->pA.xcoord, wsSpake->pA.ycoord,
                             wsSpake->pointToByteString, wsCMAC, confirmationMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
        else
        {
          actSPAKE2PCalcCmac(wsSpake, wsSpake->macKeys, wsSpake->pA.xcoord, wsSpake->pA.ycoord,
                             wsSpake->pointToByteString, wsCMAC, confirmationMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
      }

    }
  }

  return retVal;
}  /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actSPAKE2PConfirmKey()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSPAKE2PConfirmKey(
  VSECPRIM_P2VAR_PARA(actSPAKE2PSTRUCT) wsSpake,
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) wsCMAC,
  VSECPRIM_P2CONST_PARA(actU8) mac,
  VSECPRIM_P2VAR_PARA(actU8) recomputedMAC,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal;

  /* Recompute CMAC(other party's key, own public value) */
  if (wsSpake->isPartyA == TRUE)
  {
    if ((wsSpake->ciphersuite & actSPAKE2P_VERSION_MASK) == actSPAKE2P_VERSION_KEY_A_FIRST)
    {
      actSPAKE2PCalcCmac(wsSpake, &wsSpake->macKeys[actAES128_KEY_SIZE], wsSpake->pA.xcoord, wsSpake->pA.ycoord,
                          wsSpake->pointToByteString, wsCMAC, recomputedMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      actSPAKE2PCalcCmac(wsSpake, wsSpake->macKeys, wsSpake->pA.xcoord, wsSpake->pA.ycoord,
                          wsSpake->pointToByteString, wsCMAC, recomputedMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }
  else
  {
    if ((wsSpake->ciphersuite & actSPAKE2P_VERSION_MASK) == actSPAKE2P_VERSION_KEY_A_FIRST)
    {
      actSPAKE2PCalcCmac(wsSpake, wsSpake->macKeys, wsSpake->pB.xcoord, wsSpake->pB.ycoord,
                          wsSpake->pointToByteString, wsCMAC, recomputedMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      actSPAKE2PCalcCmac(wsSpake, &wsSpake->macKeys[actAES128_KEY_SIZE], wsSpake->pB.xcoord, wsSpake->pB.ycoord,
                          wsSpake->pointToByteString, wsCMAC, recomputedMAC, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
  }

  /* compare recomputed MAC and given MAC */
  if (actMemcmp(recomputedMAC, mac, actAES_BLOCK_SIZE) != TRUE) /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  {
    retVal = actVERIFICATION_FAILED;
  }
  else
  {
    retVal = actOK;
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTSPAKE2P_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actSPAKE2P.c
 *********************************************************************************************************************/

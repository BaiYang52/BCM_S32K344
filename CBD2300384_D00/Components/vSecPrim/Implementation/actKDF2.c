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
/*!        \file  actKDF2.c
 *        \brief  KDF2 key derivation function according to PKCS#5 using SHA-1 or SHA-256 as underlying hash function.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
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
#define ACTKDF2_SOURCE

#include "actIKDF2.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTKDF2_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

# if (VSECPRIM_ACTKDF2HMACSHA256_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define VSECPRIM_WRAPPER_HMAC_SHA256
# endif

# if (VSECPRIM_ACTKDF2HMACSHA1_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  define VSECPRIM_WRAPPER_HMAC_SHA1
# endif

typedef actHASHMACUNION_KDF2 actHASHMACUNION;
# include "actHashMACWrappers_int.h" /* PRQA S 5087 */ /* MD_VSECPRIM_19.1 */

/****************************************************************************
 ** Global Prototypes
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actKDF2Init
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actKDF2Init(
   VSECPRIM_P2VAR_PARA(actKDF2STRUCT) info, actLengthType iteration_count)
{
    actRETURNCODE retVal;

   if (iteration_count <= 0u)
   {
      retVal = actEXCEPTION_LENGTH;
   }
   else
   {
     info->iteration_count = iteration_count; /* SBSW_VSECPRIM_WRITE_POINTER */
     retVal = actOK;
   }
   return retVal;
}

/**********************************************************************************************************************
 *  actKDF2Derive
 **********************************************************************************************************************/
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
VSECPRIM_FUNC(actRETURNCODE) actKDF2Derive(
   VSECPRIM_P2VAR_PARA(actKDF2STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) passwd, actLengthType passwd_length,
   VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length,
   VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length,
   actU8 hashMACidx,
   actLengthType sizeSHA)
{
    actRETURNCODE retVal = actOK;

   VSECPRIM_P2VAR_PARA(actHASHMACUNION_KDF2) hmac = &info->hmac;
   VSECPRIM_P2VAR_PARA(actU8) U_buf = info->U_buf;
   VSECPRIM_P2VAR_PARA(actU8) F_buf = info->F_buf;
   actU8 INT[4];

   actU32 c, l, r;

   if (key_length >= (0xFFFFFFFFuL - sizeSHA))
   {
     retVal = actEXCEPTION_INPUT_LENGTH;
   }
   else
   {
     actU32 i, j;

     c = info->iteration_count;
     l = (key_length + sizeSHA - 1u) / sizeSHA;
     r = key_length - ((l - 1u) * sizeSHA);

     /* derive key */
     for (i = 1u; i <= l; ++i)
     {
       /* clear F_buf */
       actMemset(F_buf, 0, sizeSHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

       /* U_buf = U(1) = HMAC(passwd, salt||INT(i)) */
       actU32toBE(INT, (actU32)i); /* SBSW_VSECPRIM_CALL_FUNCTION */

       (void)actHashMACInitWrapper(hmac, passwd, passwd_length, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */

       if (salt != NULL_PTR)
       {
         (void)actHashMACUpdateWrapper(hmac, salt, salt_length, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */
       }
       (void)actHashMACUpdateWrapper(hmac, INT, sizeof(INT), VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */
       (void)actHashMACFinalizeWrapper(hmac, U_buf, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */

       for (j = 2u; j <= c; ++j)
       {
         /* F_buf = F_buf ^ U(j-1) */
         actXOR(F_buf, U_buf, sizeSHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

         /* U_buf = U(j) = HMAC(passwd, U(j-1)) */
         (void)actHashMACInitWrapper(hmac, passwd, passwd_length, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */
         (void)actHashMACUpdateWrapper(hmac, U_buf, sizeSHA, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */
         (void)actHashMACFinalizeWrapper(hmac, U_buf, VSECPRIM_FUNC_NULL_PTR, hashMACidx); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */ /* SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER */
       }

       /* F_buf = F_buf ^ U(c) */
       actXOR(F_buf, U_buf, sizeSHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

       /* copy block T_i in F_buf to output key */
       if (i != l)
       {
         actMemCpyByteArray(&(key[(i - 1u)*sizeSHA]), F_buf, sizeSHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
       }
       else
       {
         actMemCpyByteArray(&(key[(i - 1u)*sizeSHA]), F_buf, r); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */
       }
     }
   }

   return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTKDF2_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_LibCv_Curve.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for curve parameter prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_CURVE_H)
# define CRYPTO_30_LIBCV_CURVE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Empty curve vector START *********************************************************************************************/
/* This vector is used to initialize pointers which shall point to the EC domain, the EC domain extension and
 * the EC SpeedUp extension. If one of the regular curve parameters were used for this purpose, the corresponding
 * arrays will always be used and therefore never be removed by the linker.
 * Since this array does not start with the ASN1 sequence tag '0x30', the parser of the underlying crypto
 * library will recognize these domains as invalid. */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveEmptyDomain[2];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* Empty curve vector END ***********************************************************************************************/

/* SEC_p160r1 START *****************************************************************************************************/
/* SECp160r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1Domain[155];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[147];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[149];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[153];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1SpeedUpExt[355];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* SEC_p160r1 END *********************************************************************************************/

/* NIST_SEC_p224r1 START *********************************************************************************************/
/* NISTp224r1 = SECp224r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1Domain[203];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# if (actBN_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[195];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (actBN_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[197];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (actBN_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[201];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1SpeedUpExt[483];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* NIST_SEC_p224r1 END *********************************************************************************************/

/* NIST_ANSI_SEC_p256r1 START *********************************************************************************************/
/* NISTp256r1 = ANSIp256r1 = SECp256r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain[227];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[219];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[221];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[225];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt[547];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* NIST_ANSI_SEC_p256r1 END *********************************************************************************************/

/* NIST_SEC_p384r1 START *********************************************************************************************/
/* NISTp384r1 = SECp384r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1Domain[324];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[316];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[318];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[323];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt[803];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* NIST_SEC_p384r1 END *********************************************************************************************/

/* NIST_SEC_p521r1 START *********************************************************************************************/
/* NISTp521r1 = SECp521r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1Domain[432];  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[427];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[429];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[433];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1SpeedUpExt[1099];   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
/* NIST_SEC_p521r1 END *********************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CRYPTO_30_LIBCV_CURVE_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_LIBCV_CURVE.H
 *********************************************************************************************************************/

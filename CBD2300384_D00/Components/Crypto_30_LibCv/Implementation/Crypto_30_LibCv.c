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
/*      \file  Crypto_30_LibCv.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
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

#define CRYPTO_30_LIBCV_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_InternalApi.h"
#include "CryIf_Cbk.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CRYPTO header file */
#if ((CRYPTO_30_LIBCV_MAJOR_VERSION != (12u)) \
    || (CRYPTO_30_LIBCV_MINOR_VERSION != (2u)) \
    || (CRYPTO_30_LIBCV_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of Crypto_30_LibCv.c and Crypto_30_LibCv.h are inconsistent"
#endif

/* Check Autosar defines for array access */
#if (CRYPTO_HASH != 0u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_MACGENERATE != 1u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_MACVERIFY != 2u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ENCRYPT != 3u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_DECRYPT != 4u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_AEADENCRYPT != 5u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_AEADDECRYPT != 6u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_SIGNATUREGENERATE != 7u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_SIGNATUREVERIFY != 8u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_RANDOMGENERATE != 11u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_RANDOMSEED != 12u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_KEYGENERATE != 13u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_KEYDERIVE != 14u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_KEYEXCHANGECALCPUBVAL != 15u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_KEYEXCHANGECALCSECRET != 16u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_KEYSETVALID != 19u)
# error "Define is not Autosar conform."
#endif

#if (CRYPTO_ALGOFAM_NOT_SET != 0u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA1 != 1u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_224 != 2u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_256 != 3u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_384 != 4u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_512 != 5u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_512_224 != 6u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA2_512_256 != 7u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA3_224 != 8u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA3_256 != 9u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA3_384 != 10u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHA3_512 != 11u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHAKE128 != 12u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_SHAKE256 != 13u)
# error "Define is not Autosar conform."
#endif
#if (CRYPTO_ALGOFAM_RIPEMD160 != 14u)
# error "Define is not Autosar conform."
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_LOCAL) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL                                        static
#endif

#if !defined (CRYPTO_30_LIBCV_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL_INLINE                                 LOCAL_INLINE
#endif

#define CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_IDLE                        (0x00u)
#define CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_PROCESSING                  (0x01u)

/* Redirection */
#define CRYPTO_30_LIBCV_REDIRECTION_MASK_NOT_USED                     (0x00u)
#define Crypto_30_LibCv_IsRedirection(redir, mask)                    (((redir) & (mask)) == (mask))

/* IO Check */
#define Crypto_30_LibCv_IsIOLengthInvalid(length)                     ((length) > CRYPTO_30_LIBCV_UINT16_MAX)
#define Crypto_30_LibCv_IsIOMask(value, mask)                         (((value) & (mask)) == (mask))

#define CRYPTO_30_LIBCV_IOMASK_EMPTY                                  CRYPTO_30_LIBCV_REDIRECTION_MASK_NOT_USED

#ifdef CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT    /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
# define CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT                         CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT
#else
# define CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT                         (0x01u)
#endif
#define CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT_INV                      (0xFEu)

#ifdef CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT  /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
# define CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT                       CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT
#else
# define CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT                       (0x02u)
#endif

#ifdef CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT   /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
# define CRYPTO_30_LIBCV_IOMASK_TERTIARY_INPUT                        CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT
#else
# define CRYPTO_30_LIBCV_IOMASK_TERTIARY_INPUT                        (0x04u)
#endif

#ifdef CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT   /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
# define CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT                        CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT
#else
# define CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT                        (0x10u)
#endif

#ifdef CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT   /* COV_CRYPTO_30_LIBCV_CSM_DEFINES */
# define CRYPTO_30_LIBCV_IOMASK_SECONDARY_OUTPUT                      CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT
#else
# define CRYPTO_30_LIBCV_IOMASK_SECONDARY_OUTPUT                      (0x20u)
#endif

#define CRYPTO_30_LIBCV_IOMASK_VERIFY_OUTPUT                          (0x40u)

#define CRYPTO_30_LIBCV_OBJECTID_LENGTH                               (0x04u)
#define CRYPTO_30_LIBCV_ADDITIONAL_INFO_LENGTH                        (0x04u)
#define CRYPTO_30_LIBCV_WORKSPACE_SETTINGS_LENGTH                     (CRYPTO_30_LIBCV_OBJECTID_LENGTH + CRYPTO_30_LIBCV_ADDITIONAL_INFO_LENGTH)

/* for each driver object --------------------------- */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_VAR_ZERO_INIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
VAR(uint8, CRYPTO_30_LIBCV_VAR_ZERO_INIT) Crypto_30_LibCv_ModuleInitialized = CRYPTO_30_LIBCV_UNINIT;
#endif

#define CRYPTO_30_LIBCV_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 *
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
 *
 *
 *
 */

#define CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) || (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
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
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskUpdate[CRYPTO_KEYSETVALID + 1u] = {   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                  */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_MACGENERATE           */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_MACVERIFY             */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_ENCRYPT               */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_DECRYPT               */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_AEADENCRYPT           */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_AEADDECRYPT           */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_SIGNATUREGENERATE     */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_SIGNATUREVERIFY       */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_SECCOUNTERINCREMENT   */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD        */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE        */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMSEED            */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYGENERATE           */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEPARSE      */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYSETVALID           */    CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskUpdateOptional[CRYPTO_KEYSETVALID + 1u] = {   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                 */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACGENERATE          */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACVERIFY            */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_ENCRYPT              */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_DECRYPT              */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADENCRYPT          */    CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT,
  /* CRYPTO_AEADDECRYPT          */    CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT,
  /* CRYPTO_SIGNATUREGENERATE    */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SIGNATUREVERIFY      */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERINCREMENT  */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD       */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE       */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskUpdateOptionalKeepFromFinish[CRYPTO_KEYSETVALID + 1u] = {  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                  */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACVERIFY             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_ENCRYPT               */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_DECRYPT               */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADENCRYPT           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADDECRYPT           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SIGNATUREGENERATE     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SIGNATUREVERIFY       */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERINCREMENT   */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD        */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE        */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskUpdateZeroLength[CRYPTO_KEYSETVALID + 1u] = {  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                  */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_MACGENERATE           */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_MACVERIFY             */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_ENCRYPT               */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_DECRYPT               */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADENCRYPT           */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_AEADDECRYPT           */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_SIGNATUREGENERATE     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SIGNATUREVERIFY       */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERINCREMENT   */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD        */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE        */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) || (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskFinish[CRYPTO_KEYSETVALID + 1u] = {   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                 */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_MACGENERATE          */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_MACVERIFY            */    CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT | CRYPTO_30_LIBCV_IOMASK_VERIFY_OUTPUT,
  /* CRYPTO_ENCRYPT              */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_DECRYPT              */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_AEADENCRYPT          */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT | CRYPTO_30_LIBCV_IOMASK_SECONDARY_OUTPUT,
  /* CRYPTO_AEADDECRYPT          */    CRYPTO_30_LIBCV_IOMASK_TERTIARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT | CRYPTO_30_LIBCV_IOMASK_VERIFY_OUTPUT,
  /* CRYPTO_SIGNATUREGENERATE    */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_SIGNATUREVERIFY      */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT | CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT | CRYPTO_30_LIBCV_IOMASK_VERIFY_OUTPUT,
  /* CRYPTO_SECCOUNTERINCREMENT  */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD       */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE       */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_IOMASK_TERTIARY_INPUT | CRYPTO_30_LIBCV_IOMASK_SECONDARY_OUTPUT,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_IOServiceMaskFinishOptional[CRYPTO_KEYSETVALID + 1u] = {  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                 */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACGENERATE          */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_MACVERIFY            */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_ENCRYPT              */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_DECRYPT              */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADENCRYPT          */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_AEADDECRYPT          */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SIGNATUREGENERATE    */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_SIGNATUREVERIFY      */    CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT,
  /* CRYPTO_SECCOUNTERINCREMENT  */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_SECCOUNTERREAD       */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMGENERATE       */    CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_IOMASK_EMPTY,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_IOMASK_EMPTY
};
#endif

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_RedirectReadAccessRight[CRYPTO_KEYSETVALID + 1u] = {  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
  /* CRYPTO_HASH                 */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_MACGENERATE          */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_MACVERIFY            */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_ENCRYPT              */    CRYPTO_30_LIBCV_RA_ALLOWED,
  /* CRYPTO_DECRYPT              */    CRYPTO_30_LIBCV_RA_ALLOWED,
  /* CRYPTO_AEADENCRYPT          */    CRYPTO_30_LIBCV_RA_ALLOWED,
  /* CRYPTO_AEADDECRYPT          */    CRYPTO_30_LIBCV_RA_ALLOWED,
  /* CRYPTO_SIGNATUREGENERATE    */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_SIGNATUREVERIFY      */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_SECCOUNTERINCREMENT  */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_SECCOUNTERREAD       */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_RANDOMGENERATE       */    CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_RANDOMSEED            */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_KEYGENERATE           */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_KEYDERIVE             */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_KEYEXCHANGECALCPUBVAL */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_KEYEXCHANGECALCSECRET */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_CERTIFICATEPARSE      */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_CERTIFICATEVERIFY     */   CRYPTO_30_LIBCV_RA_INTERNAL_COPY,
  /* CRYPTO_KEYSETVALID           */   CRYPTO_30_LIBCV_RA_DENIED
};
#endif
#define CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VAR(boolean, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_ProcessJob_Trigger_Write[Crypto_30_LibCv_GetSizeOfDriverObjectState()]; /* PRQA S 1514 */ /* MD_CRYPTO_30_LIBCV_8.7 */

#define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_16BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
VAR(uint16, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_RandomSourceGenerateCount; /* PRQA S 1514 */ /* MD_CRYPTO_30_LIBCV_8.7 */
#endif

#define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_16BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

CRYPTO_30_LIBCV_LOCAL VAR(Crypto_30_LibCv_SizeOfPrimitiveInfoType, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_Dispatch_QueuePrimitiveInfoIdx[Crypto_30_LibCv_GetSizeOfDriverObjectState()]; /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
CRYPTO_30_LIBCV_LOCAL VAR(Crypto_30_LibCv_Redirect_Type, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_Redirect_Buffer[Crypto_30_LibCv_GetSizeOfDriverObjectState()]; /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
#endif

#define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockGetNotProtected()
 **********************************************************************************************************************/
/*! \brief          Try to obtain long term workspace lock.
 *  \details        This function tries to lock the long term workspace and determines the workspace idx.
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[out]     longWsIdxPtr      Provided long term workspace idx.
 *  \return         E_OK              Lock is obtained or was already obtained.
 *                  E_NOT_OK          Request Failed
 *  \pre            longWsIdxPtr has to be a valid pointer.
 *                  Has to be called inside an exclusive area which protects the lock.
 *  \note           Is a lock is already occupied for the key id the service will return this Idx.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockGetNotProtected(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsIsLockNotProtected()
 **********************************************************************************************************************/
/*! \brief          Check if long term workspace is locked.
 *  \details        This function checks if the long term workspace is locked and proviedes the workspace idx.
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[out]     longWsIdxPtr      Provided long term workspace idx.
 *  \return         E_OK              Lock is obtained or was already obtained.
 *                  E_NOT_OK          Request Failed
 *  \pre            longWsIdxPtr has to be a valid pointer.
 *                  Has to be called inside an exclusive area which protects the lock.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsIsLockNotProtected(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockReleaseNotProtected()
 **********************************************************************************************************************/
/*! \brief          Release long term workspace lock.
 *  \details        -
 *  \param[in]      cryptoKeyId       Holds the key id which will lock the workspace
 *  \param[in]      longWsIdx         Holds the long term workspace idx.
 *  \pre            longWsIdx has to be a valid index.
 *                  Has to be called inside an exclusive area which protects the lock.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockReleaseNotProtected(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx);
#endif /* (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_TryObtainingLock()
 *********************************************************************************************************************/
/*! \brief          Try to obtain lock workspace.
 *  \details        This function tries to lock the workspace with the job.
 *  \param[in]      objectId          Holds the identifier of the lock.
 *  \param[in]      job               Pointer to the job
 *  \param[in]      keyList           Pointer to the key lock list
 *  \return         E_OK              Lock is obtained or was already obtained.
 *                  E_NOT_OK          Request Failed
 *                  CRYPTO_E_BUSY     Lock is occupied by other job.
 *  \pre            objectId has to be a valid object id.
 *                  job has to be a valid job object.
 *                  Has to be called inside an exclusive area which protects the lock.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_TryObtainingLock(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_UpdateJobState()
 *********************************************************************************************************************/
/*! \brief          Updates the state of the job according to retVal of dispatcher.
 *  \details        This function updates the state of job according to retVal of dispatcher as long as dispatcher is not busy.
 *  \param[in]      retVal             Return value of the dispatcher
 *  \param[in,out]  job                Pointer to the job whose state shall be updated
 *  \pre            job has to be a valid job object.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_UpdateJobState(
  Std_ReturnType retVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SearchService()
 *********************************************************************************************************************/
/*! \brief         Search Service in List
 *  \details       This function searches the provided job to the service as specified in the job configuration.
 *  \param[in]     objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \param[out]    primitiveInfoIdx   Provide the primitive info index.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *  \pre           objectId has to be a valid driver object handle.
 *                 job has to be a valid job object.
 *                 primitiveInfoIdx has to be a valid Id.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SearchService(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_SizeOfPrimitiveInfoType, AUTOMATIC, AUTOMATIC)  primitiveInfoIdx);

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Verify()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the verify parameters.
 *  \details       -
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in,out] dataPtr            data pointer
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \note          The verification pointer is set to CRYPTO_E_VER_NOT_OK by default,
 *                 as it is required  to hold the ECDSA safety requirement.
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Verify(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Input()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the input parameters.
 *  \details       -
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in]     dataPtr            data pointer
 *  \param[in]     dataLength         data length
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Input(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the input parameters.
 *  \details       DataLength 0 is a valid value. If dataLength is 0 the dataPtr can be a NULL_PTR.
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in]     dataPtr            data pointer
 *  \param[in]     dataLength         data length
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_InputZeroLength()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the input parameters.
 *  \details       DataLength 0 is a valid value.
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in]     dataPtr            data pointer
 *  \param[in]     dataLength         data length
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_InputZeroLength(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Output()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the output parameters.
 *  \details       -
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in]     dataPtr            data pointer
 *  \param[in]     dataLengthPtr      data length pointer
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Output(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataLengthPtr);

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_OutputOptional()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the output parameters.
 *  \details       DataLength 0 is a valid value.
 *  \param[out]    errorId            Output pointer to the detected Error-Type-Id
 *  \param[in]     dataPtr            data pointer
 *  \param[in]     dataLengthPtr      data length pointer
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_OutputOptional(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataLengthPtr);
# endif

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryInput()
 **********************************************************************************************************************/
/*! \brief         Dispatches Det checks for primary input.
 *  \details       -
 *  \param[out]    errorId        Output pointer to the detected Error-Type-Id
 *  \param[in]     job            Pointer to the job which shall be dispatched.
 *  \param[in]     optionalMask   Mask which optional parameter shall be checked.
 *  \param[in]     zeroLengthMask Mask which zero-length-enabled parameter shall be checked.
 *  \pre           job has to be a valid job object. errorId pointer has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryInput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask,
  uint8 zeroLengthMask);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_SecondaryInput()
 **********************************************************************************************************************/
/*! \brief         Dispatches Det checks for secondary input.
 *  \details       -
 *  \param[out]    errorId        Output pointer to the detected Error-Type-Id
 *  \param[in]     job            Pointer to the job which shall be dispatched.
 *  \param[in]     optionalMask   Mask which optional parameter shall be checked.
 *  \pre           job has to be a valid job object. errorId pointer has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_SecondaryInput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryOutput()
 **********************************************************************************************************************/
/*! \brief         Dispatches Det checks for primary output.
 *  \details       -
 *  \param[out]    errorId        Output pointer to the detected Error-Type-Id
 *  \param[in]     job            Pointer to the job which shall be dispatched.
 *  \param[in]     optionalMask   Mask which optional parameter shall be checked.
 *  \pre           job has to be a valid job object. errorId pointer has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryOutput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_All()
 **********************************************************************************************************************/
/*! \brief         Det Checks for the Service Parameters
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job            Pointer to the job which shall be dispatched.
 *  \param[in]     mask           Mask which parameter shall be checked.
 *  \param[in]     optionalMask   Mask which optional parameter shall be checked.
 *  \param[in]     zeroLengthMask Mask which zero-length-enabled parameter shall be checked.
 *  \return        CRYPTO_E_NO_ERROR        No Det error detected.
 *                 CRYPTO_E_PARAM_POINTER   Det error detected, invalid pointer.
 *                 CRYPTO_E_PARAM_VALUE     Det error detected, invalid value.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(uint8, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_All(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 mask,
  uint8 optionalMask,
  uint8 zeroLengthMask);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_DetChecksServiceValues()
 *********************************************************************************************************************/
/*! \brief         Det Checks for the Service Parameters
 *  \details       This function checks the Det-Checks for all required in- and output-values.
 *  \param[in]     job                Pointer to the job which shall be dispatched.
 *  \param[out]    errorId            Output Pointer to the detected Error-Type-Id
 *  \return        E_OK               No Det error detected.
 *                 E_NOT_OK           Det error detected.
 *  \pre           objectId has to be a valid driver object handle.
 *                 errorId has to be a valid pointer.
 *                 job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) errorId);
#endif /* (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Process()
 *********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter. Takes care of jobs operation mode.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \param[in,out] keyList                 Pointer to the key lock list
 *  \param[in]     primitiveInfoIdx        Holds the identifier of the primitive.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *                 CRYPTO_E_QUEUE_FULL          Request failed, the queue is full.
 *                 CRYPTO_E_SMALL_BUFFER        Request failed, the provided buffer is too small to store the result.
 *                 CRYPTO_E_ENTROPY_EXHAUSTION  Request failed, the entropy is exhausted
 *                 CRYPTO_E_JOB_CANCELED        The service request failed because the synchronous Job has been canceled.
 *  \pre           Lock has to be reserved for the job object.
 *                 objectId has to be a valid driver object handle.
 *                 job has to be a job object handle.
 *                 primitiveInfoIdx has to be a valid Id.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Process(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList,
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx);

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_UpdateObjectWorkspace()
 *********************************************************************************************************************/
/*! \brief          Save the last driver object wokspace type.
 *  \details        -
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[in]      job                Pointer to the job which shall be dispatched.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *                  job has to be a valid job object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_UpdateObjectWorkspace(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);
#endif /* (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON) */

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_ClearObjectWorkspace()
 *********************************************************************************************************************/
/*! \brief          Clear the last driver object wokspace type.
 *  \details        -
 *  \param[in]      objectId           Holds the identifier of the Crypto Driver Object.
 *  \return         E_OK               Dispatching was successful.
 *                  E_NOT_OK           Dispatching failed.
 *  \pre            objectId has to be a valid driver object handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearObjectWorkspace(
  uint32 objectId);
#endif /* (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON) */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_IsRedirectUsed()
 **********************************************************************************************************************/
/*! \brief         Is redirection used.
 *  \details       Check if redirection is used in the current Job.
 *  \param[in]     job       Pointer to the job which shall checked.
 *  \return        TRUE      Job is redirected.
 *                 FALSE     Job is not redirected.
 *  \pre           job has to be a valid job object.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_IsRedirectUsed(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPreSetBufferForKey()
 **********************************************************************************************************************/
/*! \brief         Check access rights for given key elment.
 *  \details       Check access rights according to their read/write access.
 *  \param[in]     cryptoKeyId      Holds the crypto key id.
 *  \param[in]     keyElementId     Holds the key element id.
 *  \param[out]    elementPtr       Pointer to the key element storage.
 *  \param[out]    resultLengthPtr  Pointer to the key element length.
 *  \param[in]     readWrite        Read or write access.
 *  \param[in]     cryptoService    Holds the crypto service.
 *  \return        E_OK                         Request success.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_READ_FAIL   Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL  Request failed, write access was denied.
 *                 CRYPTO_E_KEY_EMPTY       Request failed, key is empty.
 *  \pre           cryptoKeyId need to be valid.
 *                 cryptoService need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) elementPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  uint8 readWrite,
  Crypto_ServiceInfoType cryptoService);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPostSaveKeyResult()
 **********************************************************************************************************************/
/*! \brief         Save result for output key elements.
 *  \details       -
 *  \param[in]     cryptoKeyId      Holds the crypto key id.
 *  \param[in]     keyElementId     Holds the key element id.
 *  \param[in]     resultLength     Holds the result length. (current update length)
 *  \param[in,out] writtenLengthPtr Holds the a pointer to the already written length. (last update length)
 *  \param[in]     result           Current update result
 *  \pre           cryptoKeyId need to be valid.
 *                 cryptoService need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPostSaveKeyResult(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 resultLength,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) writtenLengthPtr,
  Std_ReturnType result);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyListRedirection()
 **********************************************************************************************************************/
/*! \brief         Create list of keys for redirection.
 *  \details       A key is only referenced once in the list with the highest access.
 *  \param[in]     job       Pointer to the job which shall checked.
 *  \param[in,out] keyList   Pointer to the keyList.
 *  \pre           job has to be a valid job object.
 *                 keyList has to be a valid keyList object with at least 6 elements if redirection is used, otherwise 1.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyListRedirection(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPreRedirKeys()
 **********************************************************************************************************************/
/*! \brief         Pre redirect keys and in- and out-buffer.
 *  \details       Set the in- and out-buffer to the redirected keys.
 *  \param[in,out] redirectBuffer          Pointer to the redirection buffer.
 *  \param[in,out] job                     Pointer to the configuration of the job.
 *  \return        E_OK                         Request success.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE   Request failed, the key is not available.
 *                 CRYPTO_E_KEY_READ_FAIL   Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL  Request failed, write access was denied.
 *                 CRYPTO_E_KEY_EMPTY       Request failed, key is empty.
 *  \pre           Lock has to be reserved for the job object.
 *                 redirectBuffer has to be a valid pointer.
 *                 job has to be a job object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPreRedirKeys(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPostRestoreBuffer()
 **********************************************************************************************************************/
/*! \brief         Post redirect keys and in- and out-buffer.
 *  \details       Save result and restore in- and out-buffer.
 *  \param[in,out] redirectBuffer   Pointer to the redirection buffer.
 *  \param[in,out] job              Pointer to the configuration of the job.
 *  \param[in]     result           Current update result
 *  \pre           Lock has to be reserved for the job object.
 *                 redirectBuffer has to be a valid pointer.
 *                 job has to be a job object handle.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPostRestoreBuffer(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Std_ReturnType result);
#endif /* (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) */

#if (CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SaveContextJob()
 **********************************************************************************************************************/
/*! \brief         Save the context.
 *  \details       Stores the current workspace context in the output buffer.
 *                 If the output length pointer is set to 0, the expected buffer length is written to the output length pointer.
 *  \param[in]     objectId         Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job              Pointer to the configuration of the job.
 *  \param[in]     primitiveInfoIdx Holds the identifier of the primitive.
 *  \pre           objectId has to be a valid driver object handle.
 *                 job has to be a job object handle.
 *                 primitiveInfoIdx has to be a valid Id.
 *                 length of the output buffer must be greater than or equal to the context length.
 *                 Also refer to SMI-722498
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SaveContextJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx);

/**********************************************************************************************************************
 * Crypto_30_LibCv_RestoreContextJob()
 **********************************************************************************************************************/
/*! \brief         Restore the context.
 *  \details       Restores the workspace context with the information from the job input pointer.
 *  \param[in]     objectId         Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job              Pointer to the configuration of the job.
 *  \param[in]     primitiveInfoIdx Holds the identifier of the primitive.
 *  \pre           objectId has to be a valid driver object handle.
 *                 job has to be a job object handle.
 *                 primitiveInfoIdx has to be a valid Id.
 *                 length of the input buffer must be greater than or equal to the context length.
 *                 Also refer to SMI-722498
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RestoreContextJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx);
#endif /* (CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchJob()
 **********************************************************************************************************************/
/*! \brief         Dispatch the job.
 *  \details       Forward the job to the correct primitive.
 *  \param[in]     objectId           Holds the identifier of the Crypto Driver Object.
 *  \param[out]    unlockingNecessary Is set when unlocking is required
 *  \param[out]    contextMode        Is set when context mode is enabled
 *  \param[in,out] job                Pointer to the configuration of the job.
 *  \param[in]     primitiveInfoIdx   Holds the identifier of the primitive.
 *  \pre           objectId has to be a valid driver object handle.
 *                 job has to be a job object handle.
 *                 primitiveInfoIdx has to be a valid Id.
 *                 length of the input buffer must be greater than or equal to the context length.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchJob(
  uint32 objectId,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) unlockingNecessary,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) contextMode,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockGetNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockGetNotProtected(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_LibCv_LongTermWsLockIterType id;

  /* ----- Implementation ------------------------------------------------- */
  /* Lock already occupied */
  retVal = Crypto_30_LibCv_Local_LongWsIsLockNotProtected(cryptoKeyId, longWsIdxPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */

  /* Check if a lock is free and can be occupied. */
  if (retVal != E_OK)
  {
    for (id = 0u; id < Crypto_30_LibCv_GetSizeOfLongTermWsLock(); id++)
    {
      if (Crypto_30_LibCv_GetLongTermWsLock(id) == CRYPTO_30_LIBCV_LONG_TERM_WS_LOCK_FREE)
      {
        Crypto_30_LibCv_SetLongTermWsLock(id, cryptoKeyId);/* SBSW_CRYPTO_30_LIBCV_CSL01 */
        *longWsIdxPtr = (Crypto_30_LibCv_SizeOfLongTermWsLockType)id; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        retVal = E_OK;
        break;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsIsLockNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsIsLockNotProtected(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_LongTermWsLockIterType id;

  /* ----- Implementation ------------------------------------------------- */
  for (id = 0u; id < Crypto_30_LibCv_GetSizeOfLongTermWsLock(); id++)
  {
    if (Crypto_30_LibCv_IsLongTermWsLock(id, cryptoKeyId))
    {
      *longWsIdxPtr = (Crypto_30_LibCv_SizeOfLongTermWsLockType)id; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      retVal = E_OK;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockReleaseNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockReleaseNotProtected(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx)
{
  /* ----- Local Variables ------------------------------------------------ */

  /* ----- Implementation ------------------------------------------------- */
  if (Crypto_30_LibCv_IsLongTermWsLock(longWsIdx, cryptoKeyId)) /* COV_CRYPTO_30_LIBCV_KEY_LOCKING_RELEASE */
  {
    Crypto_30_LibCv_SetLongTermWsLock(longWsIdx, CRYPTO_30_LIBCV_LONG_TERM_WS_LOCK_FREE);/* SBSW_CRYPTO_30_LIBCV_CSL01 */
  }

}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockGet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockGet(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  retVal = Crypto_30_LibCv_Local_LongWsLockGetNotProtected(cryptoKeyId, longWsIdxPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsIsLock()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsIsLock(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfLongTermWsLockType, AUTOMATIC, AUTOMATIC) longWsIdxPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  retVal = Crypto_30_LibCv_Local_LongWsIsLockNotProtected(cryptoKeyId, longWsIdxPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_LongWsLockRelease()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_LongWsLockRelease(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfLongTermWsLockType longWsIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  Crypto_30_LibCv_Local_LongWsLockReleaseNotProtected(cryptoKeyId, longWsIdx);
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

}
#endif /* (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_TryObtainingLock()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_TryObtainingLock(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Std_ReturnType retVal, localRetVal;
  /* #5 Lock driver object for the given job if it is not already locked. */

  if (Crypto_30_LibCv_IsLockOccupiedByJob(objectId, job->jobInfo->jobId))
  {
    retVal = E_OK;
  }
#if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  else if ((Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_START)) || (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_RESTORE_CONTEXT)))
#else
  else if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_START))
#endif
  {
    retVal = CRYPTO_E_BUSY;
    if (Crypto_30_LibCv_IsLockFree(objectId))
    {
      /* #10 Lock key if operation mode is start or restore */
      localRetVal = Crypto_30_LibCv_Local_KeyListPreLockKeys(keyList);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */

      if (localRetVal == E_OK)
      {
        Crypto_30_LibCv_SetLock(objectId, job->jobInfo->jobId); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        retVal = E_OK;
      }
    }
  }
  else
  {
    /* [SWS_Crypto_00118] No Start flag while in idle mode */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_UpdateJobState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_UpdateJobState(
  Std_ReturnType retVal,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* #5 Set job state based on parameter return value. */
  if (retVal == E_OK)
  {
    job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER = CRYPTO_JOBSTATE_ACTIVE; /* SBSW_CRYPTO_30_LIBCV_UPDATE_OF_JOB_STATE */
  }
  else
  {
    /* [SWS_Crypto_00025] [SWS_Crypto_00119] Set job to IDLE if error occurred */
    job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_LIBCV_UPDATE_OF_JOB_STATE */
  }
}

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_IsObjectWorkspaceUnchanged()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_IsObjectWorkspaceUnchanged(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  /* #1 check if the object workspace type is not changed */
  if (job->cryptoKeyId == Crypto_30_LibCv_GetKeyIdOfLastJob(objectId))
  {
    if ((job->jobPrimitiveInfo->primitiveInfo->algorithm.family == Crypto_30_LibCv_GetFamilyOfLastJob(objectId))&&
        (job->jobPrimitiveInfo->primitiveInfo->algorithm.mode == Crypto_30_LibCv_GetModeOfLastJob(objectId)))
    {
      retVal = E_OK;
    }
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ClearObjectWorkspaceForChangedKey()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearObjectWorkspaceForChangedKey(
  uint32 cryptoKeyId)
{
  uint32 objectId;
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  /* #1 check if key is used in any object workspace type */
  for (objectId = 0; objectId < Crypto_30_LibCv_GetSizeOfObjectInfo(); objectId++)
  {
    if (Crypto_30_LibCv_GetKeyIdOfLastJob(objectId) == cryptoKeyId)
    {
      /* #2 clear the last object workspace type */
      Crypto_30_LibCv_SetKeyIdOfLastJob(objectId, CRYPTO_30_LIBCV_UINT32_MAX); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetFamilyOfLastJob(objectId, CRYPTO_ALGOFAM_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetModeOfLastJob(objectId, CRYPTO_ALGOMODE_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    }
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_UpdateObjectWorkspace()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_UpdateObjectWorkspace(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* #1 update the last object workspace type */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  Crypto_30_LibCv_SetKeyIdOfLastJob(objectId, job->cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetFamilyOfLastJob(objectId, job->jobPrimitiveInfo->primitiveInfo->algorithm.family); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetModeOfLastJob(objectId, job->jobPrimitiveInfo->primitiveInfo->algorithm.mode); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ClearObjectWorkspace()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ClearObjectWorkspace(
  uint32 objectId)
{
  /* #1 clear the last object workspace type */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  Crypto_30_LibCv_SetKeyIdOfLastJob(objectId, CRYPTO_30_LIBCV_UINT32_MAX); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetFamilyOfLastJob(objectId, CRYPTO_ALGOFAM_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  Crypto_30_LibCv_SetModeOfLastJob(objectId, CRYPTO_ALGOMODE_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}
#endif /* (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON) */

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Verify()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Verify(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr)
{

  /* Check Pointer */
  if (dataPtr == NULL_PTR)
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* In all primitives in which the verification pointer is usage, it is set to CRYPTO_E_VER_NOT_OK by default.
       The verification pointer should be set to CRYPTO_E_VER_OK only in successful run. */
    *dataPtr = CRYPTO_E_VER_NOT_OK; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }

}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Input()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Input(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength)
{

  /* Check Pointer */
  if (dataPtr == NULL_PTR)
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  /* Check Length */
# if(CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_ON)
  else if (dataLength == 0u)
# else
  else if ((dataLength == 0u) ||
           Crypto_30_LibCv_IsIOLengthInvalid(dataLength))
# endif
  {
    *errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }

}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength)
{
  /* Check Pointer */
  if ((dataPtr == NULL_PTR) &&
      (dataLength != 0u))
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
# if(CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_OFF)
  /* Check Length */
  else if (Crypto_30_LibCv_IsIOLengthInvalid(dataLength))
  {
    *errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
# endif
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_InputZeroLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_InputZeroLength(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 dataLength)
{
  /* Check Pointer */
  if (dataPtr == NULL_PTR)
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  /* Check Length */
# if(CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_OFF)
  else if Crypto_30_LibCv_IsIOLengthInvalid(dataLength)
  {
    *errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
# endif
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }

# if(CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_ON)
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_Output()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_Output(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataLengthPtr)
{

  /* Check Pointer */
  if ((dataPtr == NULL_PTR) ||
      (dataLengthPtr == NULL_PTR))
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  /* Check Length */
# if (CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_ON)
  else if (*dataLengthPtr == 0u)
# else
  else if ((*dataLengthPtr == 0u) ||
           Crypto_30_LibCv_IsIOLengthInvalid(*dataLengthPtr))
# endif
  {
    *errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }

}

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_OutputOptional()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_OutputOptional(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  P2CONST(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataLengthPtr)
{
  /* Check Pointer */
  if ((dataPtr == NULL_PTR) || (dataLengthPtr == NULL_PTR))
  {
    *errorId = CRYPTO_E_PARAM_POINTER; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
#  if(CRYPTO_30_LIBCV_VSEC_PRIM_ESLT_LENGTH_32 == STD_OFF)
  /* Check Length */
  else if (Crypto_30_LibCv_IsIOLengthInvalid(*dataLengthPtr))
  {
    *errorId = CRYPTO_E_PARAM_VALUE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
#  endif
  else
  {
    /* Det Checks passed. Nothing has to be done. */
  }
}
# endif

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryInput()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryInput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask,
  uint8 zeroLengthMask)
{
  if (Crypto_30_LibCv_IsIOMask(optionalMask, CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
  else if (Crypto_30_LibCv_IsIOMask(zeroLengthMask, CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_InputZeroLength(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
  else
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Input(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_SecondaryInput()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_SecondaryInput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask)
{
  if (Crypto_30_LibCv_IsIOMask(optionalMask, CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_InputOptional(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
  else
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Input(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryInputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryOutput()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryOutput(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 optionalMask)
{
# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  if (Crypto_30_LibCv_IsIOMask(optionalMask, CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_OutputOptional(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }
  else
# endif
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Output(errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_OFF)
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(optionalMask); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_DetChecksServiceValues_All()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010 4 */ /* MD_MSR_STPTH */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(uint8, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues_All(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  uint8 mask,
  uint8 optionalMask,
  uint8 zeroLengthMask
  )
{
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* Dispatch Det checks. */
  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryInput(&errorId, job, optionalMask, zeroLengthMask); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_SECONDARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_SecondaryInput(&errorId, job, optionalMask); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_TERTIARY_INPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Input(&errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.tertiaryInputLength); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_PrimaryOutput(&errorId, job, optionalMask); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_SECONDARY_OUTPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Output(&errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputPtr, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.secondaryOutputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  if (Crypto_30_LibCv_IsIOMask(mask, CRYPTO_30_LIBCV_IOMASK_VERIFY_OUTPUT))
  {
    Crypto_30_LibCv_Local_DetChecksServiceValues_Verify(&errorId, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.verifyPtr); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  }

  return errorId;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_DetChecksServiceValues()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6050 3 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_DetChecksServiceValues(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) errorId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_OK;
  Crypto_ServiceInfoType cryptoService = job->jobPrimitiveInfo->primitiveInfo->service;
  uint8 mask = CRYPTO_30_LIBCV_IOMASK_EMPTY;
  uint8 optional = CRYPTO_30_LIBCV_IOMASK_EMPTY;
  uint8 zeroLength = CRYPTO_30_LIBCV_IOMASK_EMPTY;

  /* ----- Development Error Checks --------------------------------------- */
  /* #0 Check plausibility of parameters */

  if (job->jobPrimitiveInfo->primitiveInfo->service > CRYPTO_KEYSETVALID)
  {
    /* #11 Service is not supported by Crypto Driver Object */
    /* [SWS_Crypto_00064] check if job->jobPrimitiveInfo->primitiveInfo->service is supported by Crypto Driver Object */
    *errorId = CRYPTO_E_PARAM_HANDLE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */
  }
  else
  {
    /* Check first FINISH then UPDATE this is required for optional special handling. */

    if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH))
    {
      mask = Crypto_30_LibCv_IOServiceMaskFinish[cryptoService];
      optional = Crypto_30_LibCv_IOServiceMaskFinishOptional[cryptoService];
    }

    if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_UPDATE))
    {
      mask |= Crypto_30_LibCv_IOServiceMaskUpdate[cryptoService];
      optional &= Crypto_30_LibCv_IOServiceMaskUpdateOptionalKeepFromFinish[cryptoService];
      optional |= Crypto_30_LibCv_IOServiceMaskUpdateOptional[cryptoService];
      zeroLength = Crypto_30_LibCv_IOServiceMaskUpdateZeroLength[cryptoService];
    }

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
    if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SAVE_CONTEXT))
    {
      mask |= CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT;
      optional |= CRYPTO_30_LIBCV_IOMASK_PRIMARY_OUTPUT;
    }

    if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_RESTORE_CONTEXT))
    {
      mask |= CRYPTO_30_LIBCV_IOMASK_PRIMARY_INPUT;
    }
# endif
# if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
    if (Crypto_30_LibCv_Local_IsRedirectUsed(job) == TRUE)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    {
      mask &= (uint8)~(job->jobRedirectionInfoRef->redirectionConfig);
      optional &= (uint8)~(job->jobRedirectionInfoRef->redirectionConfig);
    }
# endif

    if (mask != CRYPTO_30_LIBCV_IOMASK_EMPTY)
    {
      *errorId = Crypto_30_LibCv_Local_DetChecksServiceValues_All(job, mask, optional, zeroLength); /* SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    }
  }

  /* #12 Set return Value to E_NOT_OK if a failure was detected. */
  if ((*errorId == CRYPTO_E_PARAM_POINTER) ||
      (*errorId == CRYPTO_E_PARAM_VALUE) || (*errorId == CRYPTO_E_PARAM_HANDLE))
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListAddKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListAddKey(
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList,
  uint32 cryptoKeyId,
  uint8 keyAccess)
{
  boolean found = FALSE;
  uint8 i;
  /* Check if key is already in key list. */
  for (i = 0; i < keyList->numKeys; i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[i].keyId == cryptoKeyId)
    {
      found = TRUE;
      /* Add key access to found key. */
      /* 0 if read lock / >0 if write lock */
      keyList->keyLockList[i].keyAccess = (uint8)(keyList->keyLockList[i].keyAccess + keyAccess); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
      break;
    }
  }

  /* Add new key to list. */
  if (found == FALSE)
  {
    keyList->keyLockList[keyList->numKeys].keyId = cryptoKeyId; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    keyList->keyLockList[keyList->numKeys].keyAccess = keyAccess; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    keyList->numKeys++; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }
}

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyListRedirection()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyListRedirection(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{

  /* # Check if redirection is used. */
  if (Crypto_30_LibCv_Local_IsRedirectUsed(job) == TRUE)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  {
    P2CONST(Crypto_JobRedirectionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirInfoRef = job->jobRedirectionInfoRef;
    Crypto_InputOutputRedirectionConfigType redirConfig = redirInfoRef->redirectionConfig;

    /* # Add redirected keys to key list.  */
    if (Crypto_30_LibCv_IsRedirection(redirConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(keyList, redirInfoRef->inputKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    }

    if (Crypto_30_LibCv_IsRedirection(redirConfig, CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(keyList, redirInfoRef->secondaryInputKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    }

    if (Crypto_30_LibCv_IsRedirection(redirConfig, CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(keyList, redirInfoRef->tertiaryInputKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    }

    if (Crypto_30_LibCv_IsRedirection(redirConfig, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(keyList, redirInfoRef->outputKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    }

    if (Crypto_30_LibCv_IsRedirection(redirConfig, CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(keyList, redirInfoRef->secondaryOutputKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    }
  }
}
#endif

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyList()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyList(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  keyList->numKeys = 0u; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
  Crypto_30_LibCv_Local_GetKeyListRedirection(job, keyList);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
#endif

  /* # Add Job key to key list. */
  switch (job->jobPrimitiveInfo->primitiveInfo->service) /* PRQA S 3315 */ /* MD_CRYPTO_30_LIBCV_3315 */
  {
#if (CRYPTO_30_LIBCV_SERVICE_RANDOM == STD_ON)
  case CRYPTO_RANDOMGENERATE:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_KEY_SET_VALID == STD_ON)
  case CRYPTO_KEYSETVALID:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_RANDOM_SEED == STD_ON)
  case CRYPTO_RANDOMSEED:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_KEY_DERIVE == STD_ON)
  case CRYPTO_KEYDERIVE:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->targetCryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_KEY_EXCHANGE_CALC_PUBVAL == STD_ON)
  case CRYPTO_KEYEXCHANGECALCPUBVAL:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_KEY_EXCHANGE_CALC_SECRET == STD_ON)
  case CRYPTO_KEYEXCHANGECALCSECRET:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
#if (CRYPTO_30_LIBCV_SERVICE_KEY_GENERATE == STD_ON)
  case CRYPTO_KEYGENERATE:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_WRITE); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
#endif
  default:
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, job->cryptoKeyId, CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    break;
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetKeyListAndDet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetKeyListAndDet(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Std_ReturnType retVal = E_OK;
  uint8 i;

  /* Get key list */
  Crypto_30_LibCv_Local_GetKeyList(job, keyList);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */

  /* Check if the redirected keys have valid keyIds. */
  for (i = 0; i < keyList->numKeys; i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[i].keyId >= Crypto_30_LibCv_GetSizeOfKey())
    {
      retVal = E_NOT_OK;
      break;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListPreLockKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListPreLockKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8_least i;
  uint8_least j;

  /* Try to obtain key locks for key list. */

  for (i = 0; i < keyList->numKeys; i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[i].keyAccess == CRYPTO_30_LIBCV_KEY_ACCESS_READ) /* 0 if read lock / >0 if write lock */
    {
      retVal = Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(keyList->keyLockList[i].keyId);
    }
    else
    {
      retVal = Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(keyList->keyLockList[i].keyId);
    }

    if (retVal != E_OK)
    {
      break;
    }
  }

  /* Locking failed, release already locked keys. */
  if (retVal != E_OK)
  {
    for (j = 0; j < i; j++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
    {
      if (keyList->keyLockList[j].keyAccess == CRYPTO_30_LIBCV_KEY_ACCESS_READ) /* 0 if read lock / >0 if write lock */
      {
        Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(keyList->keyLockList[j].keyId);
      }
      else
      {
        Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(keyList->keyLockList[j].keyId);
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyListPostFreeKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyListPostFreeKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  uint8 i;
  /* Release already locked keys from key list. */
  for (i = 0; i < keyList->numKeys; i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[i].keyAccess == CRYPTO_30_LIBCV_KEY_ACCESS_READ) /* 0 if read lock / >0 if write lock */
    {
      Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(keyList->keyLockList[i].keyId);
    }
    else
    {
      Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(keyList->keyLockList[i].keyId);
    }
  }
}

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_IsRedirectUsed()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_IsRedirectUsed(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  boolean retVal = FALSE;

# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  /* Check if save or restore mode is used and ignore redirection for these modes */
  if(Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SAVE_CONTEXT) || Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_RESTORE_CONTEXT))
  {
    retVal = FALSE;
  }
  else
# endif
  /* # Check if redirection is used in the current Job. */
  if (job->jobRedirectionInfoRef != NULL_PTR)
  {
    retVal = TRUE;
  }
  else
  {
    /* retVal already set to FALSE */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPreSetBufferForKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) elementPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  uint8 readWrite,
  Crypto_ServiceInfoType cryptoService)
{
  Std_ReturnType retVal = E_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #20 Search Key Element */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  else
  {
    /* #40 Check if read access is allowed */
    if (readWrite == CRYPTO_30_LIBCV_KEY_ACCESS_READ)
    {
      if (Crypto_30_LibCv_RedirectReadAccessRight[cryptoService] < (Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
      {
        retVal = CRYPTO_E_KEY_READ_FAIL;
      }
      else if (!Crypto_30_LibCv_IsKeyElementValid(elementIndex))
      {
        retVal = CRYPTO_E_KEY_NOT_VALID;
      }
      else
      {
        /* retVal is already set */
      }
    }
    else
    {
      if ((CRYPTO_30_LIBCV_WA_DENIED == (Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))))||
          (!Crypto_30_LibCv_IsKeyElementPartial(elementIndex)) || Crypto_30_LibCv_IsKeyElementWriteOnce(elementIndex))
      {
        retVal = CRYPTO_E_KEY_WRITE_FAIL;
      }
    }

    if (retVal == E_OK)
    {
      retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic( elementIndex, elementPtr, resultLengthPtr, CRYPTO_30_LIBCV_LENGTH_CHECK_NONE, CRYPTO_30_LIBCV_SHE_SERVICE_OTHER); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */

      if (readWrite == CRYPTO_30_LIBCV_KEY_ACCESS_WRITE)
      {
        *resultLengthPtr = Crypto_30_LibCv_GetKeyElementLength(elementIndex); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      }
      else
      {
        if (*resultLengthPtr == 0u)
        {
          retVal = CRYPTO_E_KEY_EMPTY;
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPreRedirKeys()
 **********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6010, 6030 3 */ /* MD_MSR_STPTH , MD_MSR_STCYC */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPreRedirKeys(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  Std_ReturnType retVal = E_OK;
  uint32 redirKeyLength = 0u;
  Crypto_30_LibCv_SizeOfKeyStorageType elementStorageIndex = 0u;
  P2CONST(Crypto_JobRedirectionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirInfoRef = job->jobRedirectionInfoRef;
  P2VAR(Crypto_JobPrimitiveInputOutputType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) inOutRef = &job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER;
  Crypto_ServiceInfoType cryptoService = job->jobPrimitiveInfo->primitiveInfo->service;
  Crypto_InputOutputRedirectionConfigType redir;

  /* # Reset buffer length in start mode. */
  if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_START))
  {
    redirectBuffer->firstOutputLengthWritten = 0u;  /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    redirectBuffer->secondaryOutputLengthWritten = 0u;  /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }

  /* # If redirection is used. */
  if (Crypto_30_LibCv_Local_IsRedirectUsed(job) == TRUE)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  {
    redir = job->jobRedirectionInfoRef->redirectionConfig;
    redir &= (Crypto_30_LibCv_IOServiceMaskUpdate[cryptoService] | Crypto_30_LibCv_IOServiceMaskFinish[cryptoService]);

    redirectBuffer->jobPrimitiveInputOutput_Restore = job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER;  /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

    /* # Set redirection for primary input. */
    if (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT))
    {
      retVal = Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(redirInfoRef->inputKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        redirInfoRef->inputKeyElementId,
        &elementStorageIndex,
        &redirKeyLength,
        CRYPTO_30_LIBCV_KEY_ACCESS_READ,
        cryptoService);
      inOutRef->inputPtr = Crypto_30_LibCv_GetAddrKeyStorage(elementStorageIndex); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      inOutRef->inputLength = redirKeyLength;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Set redirection for secondary input. */
    if ((retVal == E_OK) &&
        (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)))
    {
      retVal = Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(redirInfoRef->secondaryInputKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        redirInfoRef->secondaryInputKeyElementId,
        &elementStorageIndex,
        &redirKeyLength,
        CRYPTO_30_LIBCV_KEY_ACCESS_READ,
        cryptoService);
      inOutRef->secondaryInputPtr = Crypto_30_LibCv_GetAddrKeyStorage(elementStorageIndex); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      inOutRef->secondaryInputLength = redirKeyLength;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Set redirection for tertiary input. */
    if ((retVal == E_OK) &&
        (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)))
    {
      retVal = Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(redirInfoRef->tertiaryInputKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        redirInfoRef->tertiaryInputKeyElementId,
        &elementStorageIndex,
        &redirKeyLength,
        CRYPTO_30_LIBCV_KEY_ACCESS_READ,
        cryptoService);
      inOutRef->tertiaryInputPtr = Crypto_30_LibCv_GetAddrKeyStorage(elementStorageIndex); /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      inOutRef->tertiaryInputLength = redirKeyLength;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Set redirection for primary output. */
    if ((retVal == E_OK) &&
        (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)))
    {
      retVal = Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(redirInfoRef->outputKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        redirInfoRef->outputKeyElementId,
        &elementStorageIndex,
        &redirKeyLength,
        CRYPTO_30_LIBCV_KEY_ACCESS_WRITE,
        cryptoService);
      inOutRef->outputPtr = &Crypto_30_LibCv_GetAddrKeyStorage(elementStorageIndex)[redirectBuffer->firstOutputLengthWritten]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      redirectBuffer->firstOutputLength = redirKeyLength - redirectBuffer->firstOutputLengthWritten;  /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      inOutRef->outputLengthPtr = &redirectBuffer->firstOutputLength;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Set redirection for secondary output. */
    if ((retVal == E_OK) &&
        (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)))
    {
      retVal = Crypto_30_LibCv_Local_RedirectPreSetBufferForKey(redirInfoRef->secondaryOutputKeyId, /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        redirInfoRef->secondaryOutputKeyElementId,
        &elementStorageIndex,
        &redirKeyLength,
        CRYPTO_30_LIBCV_KEY_ACCESS_WRITE,
        cryptoService);
      inOutRef->secondaryOutputPtr = &Crypto_30_LibCv_GetAddrKeyStorage(elementStorageIndex)[redirectBuffer->secondaryOutputLengthWritten]; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
      redirectBuffer->secondaryOutputLength = redirKeyLength - redirectBuffer->secondaryOutputLengthWritten;  /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      inOutRef->secondaryOutputLengthPtr = &redirectBuffer->secondaryOutputLength;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }

    /* # Restore buffer if there was a failure.  */
    if (retVal != E_OK)
    {
      job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER = redirectBuffer->jobPrimitiveInputOutput_Restore;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPostSaveKeyResult()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPostSaveKeyResult(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 resultLength,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) writtenLengthPtr,
  Std_ReturnType result)
{
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex;

  /* This can only work if the key was already redirected. */
  (void)Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_NORMAL_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */

  if (result == E_OK)
  {
    *writtenLengthPtr += resultLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }

  Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(cryptoKeyId, elementIndex, *writtenLengthPtr);
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_RedirectPostRestoreBuffer()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_RedirectPostRestoreBuffer(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Std_ReturnType result)
{
  Crypto_InputOutputRedirectionConfigType redir;
  Crypto_InputOutputRedirectionConfigType redirMask = 0;
  P2CONST(Crypto_JobRedirectionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirInfoRef = job->jobRedirectionInfoRef;

  if (Crypto_30_LibCv_Local_IsRedirectUsed(job) == TRUE)   /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  {
    redir = job->jobRedirectionInfoRef->redirectionConfig;

    if(Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_UPDATE))
    {
      redirMask = Crypto_30_LibCv_IOServiceMaskUpdate[job->jobPrimitiveInfo->primitiveInfo->service];
    }
    if(Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH))
    {
      redirMask |= Crypto_30_LibCv_IOServiceMaskFinish[job->jobPrimitiveInfo->primitiveInfo->service];
    }

    redir &= redirMask;

    /* Restore IO buffer */
    if (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT))
    {
      Crypto_30_LibCv_Local_RedirectPostSaveKeyResult(redirInfoRef->outputKeyId, redirInfoRef->outputKeyElementId,
        redirectBuffer->firstOutputLength, &(redirectBuffer->firstOutputLengthWritten), result); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    }

    if (Crypto_30_LibCv_IsIOMask(redir, CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT))
    {
      Crypto_30_LibCv_Local_RedirectPostSaveKeyResult(redirInfoRef->secondaryOutputKeyId, redirInfoRef->secondaryOutputKeyElementId,
        redirectBuffer->secondaryOutputLength, &(redirectBuffer->secondaryOutputLengthWritten), result); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    }

    job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER = redirectBuffer->jobPrimitiveInputOutput_Restore;  /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
  }
}
#endif /* (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Process()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6050 5 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Process(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx)
{
  Std_ReturnType retVal;
  boolean unlockingNecessary = FALSE;
  boolean contextMode = FALSE;
  Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = FALSE; /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
  retVal = Crypto_30_LibCv_Local_RedirectPreRedirKeys(&Crypto_30_LibCv_Redirect_Buffer[objectId], job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_AND_GLOBAL_ARRAY_PTR */
  if (retVal == E_OK)
#endif
  {
    retVal = Crypto_30_LibCv_DispatchJob(objectId, &unlockingNecessary, &contextMode, job, primitiveInfoIdx); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
    Crypto_30_LibCv_Local_RedirectPostRestoreBuffer(&Crypto_30_LibCv_Redirect_Buffer[objectId], job, retVal); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_AND_GLOBAL_ARRAY_PTR */
#endif
  }

  /* #15 Free workspace in case of an error or if FINISH mode completed. */
#if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  if (((retVal != E_OK) && (contextMode == FALSE)) || (unlockingNecessary == TRUE))
#else
  if ((retVal != E_OK) || (unlockingNecessary == TRUE))
#endif
  {
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    Crypto_30_LibCv_FreeLock(objectId); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
    Crypto_30_LibCv_Local_KeyListPostFreeKeys(keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    if (Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId]) /* COV_CRYPTO_30_LIBCV_SERVICE_CONFIGURATION_2 */
    {
      Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(job->cryptoKeyId));
    }
#endif
  }

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
  if (retVal == E_OK)
  {
    Crypto_30_LibCv_UpdateObjectWorkspace(objectId, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  }
# if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  else if (contextMode == TRUE)
  {
    /* Workspace of the object should remain untouched for operation mode save or restore */
  }
# endif
  else
  {
    Crypto_30_LibCv_ClearObjectWorkspace(objectId);
  }
#endif

  /* Remove job from queue and release processing */
  Crypto_30_LibCv_SetDriverObjectState(objectId, CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_IDLE); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

  /* #20 Notify CryIf about the result of the job if it was processed asynchronous. */
  if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
  {
    Crypto_30_LibCv_SetQueue(Crypto_30_LibCv_GetQueueIdxOfObjectInfo(objectId), NULL_PTR); /* SBSW_CRYPTO_30_LIBCV_CSL02_QUEUE_VIA_DRIVEROBJECT */
    /* Call Callback */
    CryIf_CallbackNotification(job, retVal); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SearchService()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF  */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SearchService(
  uint32 objectId,
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  P2VAR(Crypto_30_LibCv_SizeOfPrimitiveInfoType, AUTOMATIC, AUTOMATIC)  primitiveInfoIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  Crypto_30_LibCv_PrimitiveInfoIterType primitiveInfoStartIdx, primitiveInfoEndIdx, primitiveInfoIteratorIdx;

  Crypto_30_LibCv_ObjectInfoIndIterType objectInfoIndStartIdx, objectInfoIndEndIdx, objectInfoIndIdx;

  Crypto_30_LibCv_PrimitiveFctIterType functionIdx;

  P2CONST(Crypto_PrimitiveInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) primitive = job->jobPrimitiveInfo->primitiveInfo;
  P2CONST(Crypto_AlgorithmInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) algo = &primitive->algorithm;

  uint32 combi;

  /* ----- Implementation ----------------------------------------------- */
  /* # Check if primitive info is used */
  if (Crypto_30_LibCv_IsPrimitiveInfoUsedOfPrimitiveServiceInfo(primitive->service) == TRUE) /* COV_CRYPTO_30_LIBCV_SERVICE_CONFIGURATION */
  {
    /* Get scope for iteration */
    primitiveInfoStartIdx = Crypto_30_LibCv_GetPrimitiveInfoStartIdxOfPrimitiveServiceInfo(primitive->service);
    primitiveInfoEndIdx = Crypto_30_LibCv_GetPrimitiveInfoEndIdxOfPrimitiveServiceInfo(primitive->service);

    /* Generate combined value */
    combi = (((uint32)primitive->service << 24) | ((uint32)algo->family << 16) | ((uint32)algo->mode << 8) | ((uint32)algo->secondaryFamily));

    /* # Iterate over primitive info elements */
    for (primitiveInfoIteratorIdx = primitiveInfoStartIdx; primitiveInfoIteratorIdx < primitiveInfoEndIdx; primitiveInfoIteratorIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
    {
      /* # Check for match with combined value */
      if (Crypto_30_LibCv_GetCombinedOfPrimitiveInfo(primitiveInfoIteratorIdx) == combi)
      {
        /* Get primitive function index */
        functionIdx = Crypto_30_LibCv_GetPrimitiveFctIdxOfPrimitiveInfo(primitiveInfoIteratorIdx);

        /* Get scope for iteration */
        objectInfoIndStartIdx = Crypto_30_LibCv_GetObjectInfoIndStartIdxOfPrimitiveFct(functionIdx);
        objectInfoIndEndIdx = Crypto_30_LibCv_GetObjectInfoIndEndIdxOfPrimitiveFct(functionIdx);

        /* # Iterate over object info elements */
        for (objectInfoIndIdx = objectInfoIndStartIdx; objectInfoIndIdx < objectInfoIndEndIdx; objectInfoIndIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
        {
          /* # Check for match with object ID */
          if (Crypto_30_LibCv_GetObjectInfoInd(objectInfoIndIdx) == objectId)
          {
            /* Reference primitive function pointer */
            *primitiveInfoIdx = (Crypto_30_LibCv_SizeOfPrimitiveInfoType)primitiveInfoIteratorIdx; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
            retVal = E_OK;
            break;
          }
        }
      }

      /* If found break loop */
      if (retVal == E_OK)
      {
        break;
      }
    }
  }

  return retVal;
}

#if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SaveContextJob()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SaveContextJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 outputLength;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  outputLength = CRYPTO_30_LIBCV_WORKSPACE_SETTINGS_LENGTH + Crypto_30_LibCv_GetLengthOfSaveAndRestoreWorkspace(objectId);

  /* Check primitive context and output length */
  if ((job->jobPrimitiveInfo->primitiveInfo->service <= CRYPTO_SIGNATUREVERIFY) && (Crypto_30_LibCv_IsContextOfPrimitiveInfo(primitiveInfoIdx)))
  {
# if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
    /* Check if output redirection was used in the previous calls of the current job */
    if(Crypto_30_LibCv_Redirect_Buffer[objectId].firstOutputLengthWritten != 0u)
    {
      /* No saving of output redirected job allowed. retVal already set to E_NOT_OK */
    }
    else
# endif
    if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr == 0u)
    {
      /* If the output buffer does not have the sufficient length, the expected length is written into the output buffer */
      *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = outputLength; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
    }
    else if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr >= outputLength)
    {
      /* Copy context to output [ 4 Byte ObjectId | 4 Byte Additional Info | x byte Workspace data ] */
      Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(Crypto_30_LibCv_GetWorkspaceAddrOfObjectInfo(objectId), objectId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */
      retVal = Appl_Crypto_30_LibCv_SaveContextCallout(objectId,
                                                       job->jobInfo->jobId,
                                                       (P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))Crypto_30_LibCv_GetWorkspaceAddrOfObjectInfo(objectId),
                                                       outputLength,
                                                       job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr,
                                                       job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */
    }
    else
    {
      /* retval already set */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_RestoreContextJob()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RestoreContextJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 inputLength;
  uint32 inputObjectId;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if primitive context is set */
  if ((job->jobPrimitiveInfo->primitiveInfo->service <= CRYPTO_SIGNATUREVERIFY) && (Crypto_30_LibCv_IsContextOfPrimitiveInfo(primitiveInfoIdx)))
  {
    /* Get workspace address and length [ 4 byte ObjectId | 4 Byte Additional Info | x byte Workspace Data ] */
    (void) Crypto_30_LibCv_GetDispatchOfPrimitiveFct(Crypto_30_LibCv_GetPrimitiveFctIdxOfPrimitiveInfo(primitiveInfoIdx))(objectId, job, CRYPTO_OPERATIONMODE_RESTORE_CONTEXT); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    inputLength = CRYPTO_30_LIBCV_WORKSPACE_SETTINGS_LENGTH + Crypto_30_LibCv_GetLengthOfSaveAndRestoreWorkspace(objectId);

    /* Check if the input length is correct */
    if (job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength >= inputLength)
    {
      /* Write Workspace data to the Workspace address */
      retVal = Appl_Crypto_30_LibCv_RestoreContextCallout(objectId,
                                                          job->jobInfo->jobId,
                                                          (P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))Crypto_30_LibCv_GetWorkspaceAddrOfObjectInfo(objectId),
                                                          inputLength,
                                                          job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr,
                                                          job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */

      if (retVal == E_OK)
      {
        /* Get objectId */
        Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&inputObjectId, Crypto_30_LibCv_GetWorkspaceAddrOfObjectInfo(objectId)); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

        if (inputObjectId == objectId)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 * Crypto_30_LibCv_DispatchJob()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_DispatchJob(
  uint32 objectId,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) unlockingNecessary,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) contextMode,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 Dispatch job according to job mode in START, UPDATE, FINISH, SAVE or Restore. */
  /* [SWS_Crypto_00017] [SWS_Crypto_00020] START while IDLE or ACTIVE */
  if (((job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER == CRYPTO_JOBSTATE_IDLE) ||
    (job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER == CRYPTO_JOBSTATE_ACTIVE)) &&
    Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_START))
  {
    retVal = Crypto_30_LibCv_GetDispatchOfPrimitiveFct(Crypto_30_LibCv_GetPrimitiveFctIdxOfPrimitiveInfo(primitiveInfoIdx))(objectId, job, CRYPTO_OPERATIONMODE_START);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    Crypto_30_LibCv_UpdateJobState(retVal, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  }

  /* UPDATE while ACTIVE */
  if ((job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER == CRYPTO_JOBSTATE_ACTIVE) &&
    Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_UPDATE)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_LibCv_GetDispatchOfPrimitiveFct(Crypto_30_LibCv_GetPrimitiveFctIdxOfPrimitiveInfo(primitiveInfoIdx))(objectId, job, CRYPTO_OPERATIONMODE_UPDATE);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    Crypto_30_LibCv_UpdateJobState(retVal, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
  }

  /* [SWS_Crypto_00023] FINISH while ACTIVE */
  if ((job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER == CRYPTO_JOBSTATE_ACTIVE) &&
    Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_FINISH)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_LibCv_GetDispatchOfPrimitiveFct(Crypto_30_LibCv_GetPrimitiveFctIdxOfPrimitiveInfo(primitiveInfoIdx))(objectId, job, CRYPTO_OPERATIONMODE_FINISH);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_LIBCV_DIRECT_ACCESS_TO_JOB_STATE */
    *unlockingNecessary = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }
#if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
  /* SAVE while ACTIVE */
  if ((job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER == CRYPTO_JOBSTATE_ACTIVE) &&
    Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SAVE_CONTEXT)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_LibCv_SaveContextJob(objectId, job, primitiveInfoIdx); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER = CRYPTO_JOBSTATE_ACTIVE; /* SBSW_CRYPTO_30_LIBCV_DIRECT_ACCESS_TO_JOB_STATE */
    *contextMode = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }
  /* RESTORE the workspace */
  if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_RESTORE_CONTEXT)) /* [SWS_Crypto_00024] */
  {
    retVal = Crypto_30_LibCv_RestoreContextJob(objectId, job, primitiveInfoIdx); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    Crypto_30_LibCv_UpdateJobState(retVal, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
    *contextMode = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(*contextMode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */ /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Crypto_30_LibCv_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #5 Uninitialize module. */
  Crypto_30_LibCv_ModuleInitialized = (uint8)CRYPTO_30_LIBCV_UNINIT;
#endif
} /* Crypto_30_LibCv_InitMemory() */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_KeySimple()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_KeySimple(
  Crypto_30_LibCv_KeyElementsIterType elementIndex)
{
  Crypto_30_LibCv_SizeOfKeyStorageType keyStorageIdx;

  /* clear extension */
  Crypto_30_LibCv_SetKeyElementExtension(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_CLEAR_ALL_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
  keyStorageIdx = Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(elementIndex);
  Crypto_30_LibCv_SetKeyStorage(keyStorageIdx, 0u); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
  Crypto_30_LibCv_SetKeyStorage(keyStorageIdx + 1u, 0u); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
  Crypto_30_LibCv_SetKeyElementState(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_ALL_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */

}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_Key()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_Key(
  Crypto_30_LibCv_KeyElementsIterType elementIndex,
  boolean initAllBytes)
{
#if (CRYPTO_30_LIBCV_INITVALUE == STD_ON)
  Crypto_30_LibCv_SizeOfInitValueType initValueIdx;
  uint16 initValueLength;
#endif
  Crypto_30_LibCv_SizeOfKeyStorageType keyStorageIdx;
  uint32 keyElementLength;

  keyStorageIdx = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  keyElementLength = (uint32)Crypto_30_LibCv_GetLengthOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex));

#if (CRYPTO_30_LIBCV_INITVALUE == STD_ON)
  /* Fill elements with init values */
  if (Crypto_30_LibCv_HasKeyElementInitValue(elementIndex))
  {
    initValueIdx = Crypto_30_LibCv_GetInitValueStartIdxOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex));
    initValueLength = (uint16)Crypto_30_LibCv_GetKeyElementInitValueLength(elementIndex);

    /* Store the init value t the key element */
    Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIdx), Crypto_30_LibCv_GetAddrInitValue(initValueIdx), (uint32)initValueLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

    if (initAllBytes)
    {
      /* Initialize remaning bytes with zeros. */
      Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIdx + initValueLength), (uint32)(keyElementLength- initValueLength)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    }
    /* clear extension */
    Crypto_30_LibCv_SetKeyElementExtension(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_CLEAR_ALL_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */

    /* Set the size of the init value written to the element */
    keyStorageIdx = Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(elementIndex);
    Crypto_30_LibCv_SetKeyStorage(keyStorageIdx, (uint8)(Crypto_30_LibCv_GetKeyElementInitValueLength(elementIndex) >> 8)); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
    Crypto_30_LibCv_SetKeyStorage(keyStorageIdx + 1u, (uint8)(Crypto_30_LibCv_GetKeyElementInitValueLength(elementIndex))); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
    Crypto_30_LibCv_SetKeyElementState(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
  }
  else
#endif
  {
    Crypto_30_LibCv_Init_KeySimple(elementIndex);
    if (initAllBytes)
    {
      /* Initialize data bytes with zeros. */
      Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIdx), keyElementLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
    }
  }
}

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init_KeySheAdditional()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init_KeySheAdditional(
  Crypto_30_LibCv_KeyElementsIterType elementIndex)
{
  Crypto_30_LibCv_SetKeyElementExtension(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_KEY_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
  /* SHE keys are invalid also if they have an init value. */
  Crypto_30_LibCv_SetKeyElementState(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_ALL_MASK); /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT */
}
#endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Init()
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
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #5 Check initialization state of the component. */
  if (Crypto_30_LibCv_IsModuleInitialized())
  {
    errorId = CRYPTO_E_INIT_FAILED;
  }
  else
#endif
  {

    Crypto_30_LibCv_KeyElementsIterType elementIndex;
    Crypto_30_LibCv_KeyIterType keyIndex;
#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
    Crypto_30_LibCv_SheKeysIterType sheKeyIndex;
#endif
    Crypto_30_LibCv_QueueIterType queueIdx;
    Crypto_30_LibCv_LockIterType lockIdx;
#if (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON)
    Crypto_30_LibCv_LongTermWsLockIterType longWsLockIdx;
#endif
    Crypto_30_LibCv_ObjectInfoIterType objectId;
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    Crypto_30_LibCv_SizeOfNvBlockType blockIdx;
#endif

    /* #10 Initialize key elements which have a configured initial value. */
    for (elementIndex = 0u; elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements(); elementIndex++)
    {
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
      if (Crypto_30_LibCv_IsKeyElementPersist(elementIndex))
      {
        Crypto_30_LibCv_Init_KeySimple(elementIndex);
      }
      else
#endif
      {
        Crypto_30_LibCv_Init_Key(elementIndex, FALSE);
      }
    }

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
    for (sheKeyIndex = 0u; sheKeyIndex < Crypto_30_LibCv_GetSizeOfSheKeys(); sheKeyIndex++)
    {

      elementIndex = Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(sheKeyIndex);
      Crypto_30_LibCv_Init_KeySheAdditional(elementIndex);
    }
# if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
    /* It is only allowed to access this data if the SheInfoKey is locked */
    Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag = FALSE;
# endif
#endif
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    for (blockIdx = 0u; blockIdx < Crypto_30_LibCv_GetSizeOfNvBlock(); blockIdx++)
    {
      Crypto_30_LibCv_NvBlock_State_Init(blockIdx);
    }
#endif
#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
    Crypto_30_LibCv_RandomSourceGenerateCount = 0u;
#endif

    /* #15 Free all locks of workspaces. */
    for (lockIdx = 0u; lockIdx < Crypto_30_LibCv_GetSizeOfLock(); lockIdx++)
    {
      Crypto_30_LibCv_FreeLock(lockIdx); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    }

#if (CRYPTO_30_LIBCV_LONGTERMWS == STD_ON)
    /* #17 Free all locks of long term workspaces. */
    for (longWsLockIdx = 0u; longWsLockIdx < Crypto_30_LibCv_GetSizeOfLongTermWsLock(); longWsLockIdx++)
    {
      Crypto_30_LibCv_SetLongTermWsLock(longWsLockIdx, CRYPTO_30_LIBCV_LONG_TERM_WS_LOCK_FREE); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    }
#endif

    /* #20 Clear job queue. */
    for (queueIdx = 0u; queueIdx < Crypto_30_LibCv_GetSizeOfQueue(); queueIdx++)
    {
      Crypto_30_LibCv_SetQueue(queueIdx, NULL_PTR); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    }

    /* #25 Init Driver Object State. */
    for (objectId = 0u; objectId < Crypto_30_LibCv_GetSizeOfObjectInfo(); objectId++)
    {
      Crypto_30_LibCv_SetDriverObjectState(objectId, CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_IDLE); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
      /* clear the last object workspace type */
      Crypto_30_LibCv_SetKeyIdOfLastJob(objectId, CRYPTO_30_LIBCV_UINT32_MAX); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetFamilyOfLastJob(objectId, CRYPTO_ALGOFAM_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
      Crypto_30_LibCv_SetModeOfLastJob(objectId, CRYPTO_ALGOMODE_NOT_SET); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
#endif

#if (CRYPTO_30_LIBCV_FIPS186 == STD_ON)
      if (Crypto_30_LibCv_GetRandomSeedBufferStartIdxOfObjectInfo(objectId) != CRYPTO_30_LIBCV_NO_RANDOMSEEDBUFFERENDIDXOFOBJECTINFO)
      {
        Crypto_30_LibCv_SetRandomSeedBuffer(Crypto_30_LibCv_GetRandomSeedBufferStartIdxOfObjectInfo(objectId), CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_ALL_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
      }
#endif

#if(CRYPTO_30_LIBCV_SAVEANDRESTOREWORKSPACE == STD_ON)
      Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetWorkspaceAddrOfObjectInfo(objectId), Crypto_30_LibCv_GetWorkspaceLengthOfObjectInfo(objectId)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE */
#endif
    }

    for (keyIndex = 0u; keyIndex < Crypto_30_LibCv_GetSizeOfKeyLock(); keyIndex++)
    {
      Crypto_30_LibCv_SetKeyLock(keyIndex, CRYPTO_30_LIBCV_KEY_LOCK_FREE); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    }

    /* Set module state to initialized */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
    Crypto_30_LibCv_ModuleInitialized = CRYPTO_30_LIBCV_INITIALIZED;
#endif

  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_INIT, errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
}

#if (CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #5 Check plausibility of parameters. */
  if (versioninfo == NULL_PTR)
  {
    /* [SWS_Crypto_00042] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #10 Write versionInfo pointer with modules versions. */
    versioninfo->vendorID = (uint16)CRYPTO_30_LIBCV_VENDOR_ID; /* SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO */
    versioninfo->moduleID = (uint8)CRYPTO_30_LIBCV_MODULE_ID; /* SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO */
    versioninfo->sw_major_version = (uint8)CRYPTO_30_LIBCV_MAJOR_VERSION; /* SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO */
    versioninfo->sw_minor_version = (uint8)CRYPTO_30_LIBCV_MINOR_VERSION; /* SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO */
    versioninfo->sw_patch_version = (uint8)CRYPTO_30_LIBCV_PATCH_VERSION; /* SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          (uint8)CRYPTO_30_LIBCV_SID_GET_VERSION_INFO,
                          errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif
}
#endif /* (CRYPTO_30_LIBCV_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Crypto_30_LibCv_ObjectInfoIterType objectId;
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx = 0u; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */

#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #1 Check initialization state of the component. */
  if (Crypto_30_LibCv_IsModuleInitialized())
#endif
  {
    /* #2 Process NvM handling  */
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    Crypto_30_LibCv_NvBlock_MainFunction();
#endif

    /* #5 Process next enqueued job for all available queues. */
    for (objectId = 0u; objectId < Crypto_30_LibCv_GetSizeOfObjectInfo(); objectId++)
    {
      boolean processJob = FALSE;
      P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job;
      Crypto_30_LibCv_KeyLockListType keyList;

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      job = Crypto_30_LibCv_GetQueue(Crypto_30_LibCv_GetQueueIdxOfObjectInfo(objectId));

      if (job != NULL_PTR)
      {
        Crypto_30_LibCv_Local_GetKeyList(job, &keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
        primitiveInfoIdx = Crypto_30_LibCv_Dispatch_QueuePrimitiveInfoIdx[objectId]; /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */

#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
        if (Crypto_30_LibCv_IsDefaultRandomSourceOfPrimitiveInfo(primitiveInfoIdx))
        {
          Crypto_30_LibCv_Local_KeyListAddKey(&keyList, Crypto_30_LibCv_GetDefaultRandomKey(), CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        }
#endif
        if (Crypto_30_LibCv_TryObtainingLock((uint32)objectId, job, &keyList) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
        {
          Crypto_30_LibCv_SetDriverObjectState(objectId, CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_PROCESSING); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
          processJob = TRUE;
        }
      }
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      if (processJob == TRUE)
      {
        /* #10 Remove job from queue if it was processed or an error occurred. */
        (void)Crypto_30_LibCv_Process((uint32)objectId, job, &keyList, primitiveInfoIdx); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */ /* PRQA S 2973 */ /* MD_CRYPTO_30_LIBCV_2973 */
      }
    }
  }
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_ProcessJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6030,6050,6080 3 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_ProcessJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Crypto_30_LibCv_KeyLockListType keyList;
  Crypto_30_LibCv_SizeOfPrimitiveInfoType primitiveInfoIdx = 0u;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_Crypto_00057] */
  /* #5 Check initialization state of the component. */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  /* [SWS_Crypto_00058] */
  /* #10 Check plausibility of parameters. */

  /* [SWS_Crypto_00059] Check if job is a null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (Crypto_30_LibCv_Local_DetChecksServiceValues(job, &errorId) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  {
    /* Error Id is set by sub-function */
  }
  else
#endif
  if (objectId >= Crypto_30_LibCv_GetSizeOfObjectInfo())
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (Crypto_30_LibCv_Local_GetKeyListAndDet(job, &keyList) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (Crypto_30_LibCv_SearchService(objectId, job, &primitiveInfoIdx) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR */
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
#if (CRYPTO_30_LIBCV_DEFAULT_RANDOM_SOURCE == STD_ON)
    if (Crypto_30_LibCv_IsDefaultRandomSourceOfPrimitiveInfo(primitiveInfoIdx))
    {
      Crypto_30_LibCv_Local_KeyListAddKey(&keyList, Crypto_30_LibCv_GetDefaultRandomKey(), CRYPTO_30_LIBCV_KEY_ACCESS_READ); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    }
#endif

    /* #15 Directly process job if it is synchronous. */
    if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_SYNC)
    {
      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      retVal = Crypto_30_LibCv_TryObtainingLock(objectId, job, &keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      if (retVal == E_OK)
      {
        Crypto_30_LibCv_SetDriverObjectState(objectId, CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_PROCESSING); /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
        retVal = Crypto_30_LibCv_Process(objectId, job, &keyList, primitiveInfoIdx); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
    }
    else
    {
      /* #20 Enqueue job if it is asynchronous and the queue is not full and the lock is not occupied by another job. */
      Crypto_30_LibCv_QueueIdxOfObjectInfoType queueIdx = Crypto_30_LibCv_GetQueueIdxOfObjectInfo(objectId);
      retVal = CRYPTO_E_QUEUE_FULL;

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
      /* If job is asynchronous and job queue is not full, queue job. Otherwise return CRYPTO_E_QUEUE_FULL. */
      if (Crypto_30_LibCv_GetQueue(queueIdx) == NULL_PTR)
      {
        if ((Crypto_30_LibCv_IsLockFree(objectId)) ||
            ((Crypto_30_LibCv_IsLockOccupiedByJob(objectId, job->jobInfo->jobId)) == TRUE))
        {
          Crypto_30_LibCv_SetQueue(queueIdx, job); /* SBSW_CRYPTO_30_LIBCV_CSL02_QUEUE_VIA_DRIVEROBJECT */
          Crypto_30_LibCv_Dispatch_QueuePrimitiveInfoIdx[objectId] = (Crypto_30_LibCv_SizeOfPrimitiveInfoType)primitiveInfoIdx; /* SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID */
          retVal = E_OK;
        }
      }
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_PROCESS_JOB,
                          errorId);
  }
# if (CRYPTO_30_LIBCV_SERVICE_RANDOM == STD_ON)
  if (retVal == CRYPTO_E_ENTROPY_EXHAUSTION)
  {
    (void)Det_ReportRuntimeError(CRYPTO_30_LIBCV_MODULE_ID,
                                 CRYPTO_30_LIBCV_INSTANCE_ID,
                                 CRYPTO_30_LIBCV_SID_PROCESS_JOB,
                                 CRYPTO_E_RE_ENTROPY_EXHAUSTED);
  }
# endif
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Crypto_30_LibCv_CancelJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 3 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_Crypto_00123] */
  /* #5 Check initialization state of the component. */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  /* [SWS_Crypto_00124] */
  /* #10 Check plausibility of parameters. */
  /* [SWS_Crypto_00125] Check if job is a null pointer */
  else if (job == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
#endif
  if (objectId >= Crypto_30_LibCv_GetSizeOfObjectInfo())
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {

    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    /* Driver object is not processing anything at the moment */
    if (Crypto_30_LibCv_GetDriverObjectState(objectId) == CRYPTO_30_LIBCV_DRIVEROBJECTSTATE_IDLE)
    {
      Crypto_30_LibCv_QueueIdxOfObjectInfoType queueIdx = Crypto_30_LibCv_GetQueueIdxOfObjectInfo(objectId);
      P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) queuedJob = Crypto_30_LibCv_GetQueue(queueIdx);

      if ((queuedJob != NULL_PTR) &&
          (queuedJob->jobInfo->jobId == job->jobInfo->jobId))
      {
        Crypto_30_LibCv_SetQueue(queueIdx, NULL_PTR); /* SBSW_CRYPTO_30_LIBCV_CSL02_QUEUE_VIA_DRIVEROBJECT */
        retVal = E_OK;
      }

      if (Crypto_30_LibCv_IsLockOccupiedByJob(objectId, job->jobInfo->jobId))
      {
        Crypto_30_LibCv_KeyLockListType keyList;
        Crypto_30_LibCv_Local_GetKeyList(job, &keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */

        Crypto_30_LibCv_FreeLock(objectId); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
        job->CRYPTO_30_LIBCV_JOB_STATE_MEMBER = CRYPTO_JOBSTATE_IDLE; /* SBSW_CRYPTO_30_LIBCV_ACCESS_OF_JOB_MEMBER_CANCEL_JOB */
        Crypto_30_LibCv_Local_KeyListPostFreeKeys(&keyList);  /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        retVal = E_OK;
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (retVal == E_OK)
    {
#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
      Crypto_30_LibCv_ClearObjectWorkspace(objectId);
#endif
      if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
      {
        CryIf_CallbackNotification(job, CRYPTO_E_JOB_CANCELED); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError(CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_CANCEL_JOB, errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

#if (CRYPTO_30_LIBCV_REDIRECTION == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_GetKeyListRedirection()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_GetKeyListRedirection(
  P2CONST(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Crypto_30_LibCv_Local_GetKeyListRedirection(job, keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_RedirectPreRedirKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_RedirectPreRedirKeys(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyLockListType keyList;

  if (Crypto_30_LibCv_Local_GetKeyListAndDet(job, &keyList) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = Crypto_30_LibCv_Local_RedirectPreRedirKeys(redirectBuffer, job); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_RedirectPostRestoreBuffer()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_RedirectPostRestoreBuffer(
  P2VAR(Crypto_30_LibCv_Redirect_Type, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) redirectBuffer,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job,
  Std_ReturnType result)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyLockListType keyList;

  if (Crypto_30_LibCv_Local_GetKeyListAndDet(job, &keyList) == E_NOT_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  {
    retVal = E_NOT_OK;
  }
  else
  {
    Crypto_30_LibCv_Local_RedirectPostRestoreBuffer(redirectBuffer, job, result); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    retVal = E_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_REDIRECTION == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListAddKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListAddKey(
  P2VAR(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList,
  uint32 cryptoKeyId,
  uint8 keyAccess)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    Crypto_30_LibCv_Local_KeyListAddKey(keyList, cryptoKeyId, keyAccess); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListPreLockKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListPreLockKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean keysValid = TRUE;
  uint8 currentKeyIdx = 0;

  /* Check if the keyList keys have valid keyIds. */
  for (currentKeyIdx = 0; currentKeyIdx < keyList->numKeys; currentKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[currentKeyIdx].keyId >= Crypto_30_LibCv_GetSizeOfKey())
    {
      keysValid = FALSE;
      break;
    }
  }

  if (keysValid == TRUE)
  {
    retVal = Crypto_30_LibCv_Local_KeyListPreLockKeys(keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyListPostFreeKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyListPostFreeKeys(
  P2CONST(Crypto_30_LibCv_KeyLockListType, AUTOMATIC, AUTOMATIC) keyList)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean keysValid = TRUE;
  uint8 currentKeyIdx = 0;

  /* Check if the keyList keys have valid keyIds. */
  for (currentKeyIdx = 0; currentKeyIdx < keyList->numKeys; currentKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR */
  {
    if (keyList->keyLockList[currentKeyIdx].keyId >= Crypto_30_LibCv_GetSizeOfKey())
    {
      keysValid = FALSE;
      break;
    }
  }

  if (keysValid == TRUE)
  {
    Crypto_30_LibCv_Local_KeyListPostFreeKeys(keyList); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
    retVal = E_OK;
  }

  return retVal;
}
#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations:

MD_CRYPTO_30_LIBCV_STPAR_ASR:
Reason:     Internal forwarding of parameters of an AUTOSAR API.
Risk:       Code is difficult to maintain. Poor readability of the code due to too many input parameters.
Prevention: Design and code review. Clearly structured and commented code.

MD_CRYPTO_30_LIBCV_STPAR_VSECPRIM:
Reason:     Parameters are required by vSecPrim, due to code design and metrics code need to be split in subfunctions.
Risk:       Code is difficult to maintain. Poor readability of the code due to too many input parameters.
Prevention: Design and code review. Clearly structured and commented code.

MD_CRYPTO_30_LIBCV_STPAR:
Reason:     Parameters are required by code design and to hold other code metrics.
Risk:       Code is difficult to maintain. Poor readability of the code due to too many input parameters.
Prevention: Design and code review. Clearly structured and commented code.

MD_CRYPTO_30_LIBCV_8.5:
Description: Rule 8.5. There shall be no definitions of objects or functions in a header file.
Reason:      Subcomponent structure is implemented in header files. The implementation files can be identified by their name ending in _Int.h
Risk:        None.
Prevention:  None.

MD_CRYPTO_30_LIBCV_8.7:
Reason:     The deceleration is on block. However the static source is only required by one function.
Risk:       None
Prevention: Not necessary

MD_CRYPTO_30_LIBCV_3.1:
Reason:     Cast is needed since lower layer crypto library expects -1 as parameter value.
Risk:       Cast from signed to unsigned results in an invalid value.
Prevention: Covered by code review.

MD_CRYPTO_30_LIBCV_0310:
Reason:     Cast is needed since lower layer crypto library expects super type as parameter. Internally it is casted back to concrete workspace type.
Risk:       Wrong workspace type is casted or size does not match.
Prevention: Covered by code review.

MD_CRYPTO_30_LIBCV_0311:
Reason:     Parameter has to be casted to var since lower layer crypto library expects a var parameter even if it does not write to it.
Risk:       Lower layer API could modify the given parameter.
Prevention: Covered by code review and component test.

MD_CRYPTO_30_LIBCV_0316:
Reason:     Cast is needed since upper layer NvM expects super type as parameter.
Risk:       Wrong type is casted or size does not match.
Prevention: Covered by code review.

MD_CRYPTO_30_LIBCV_2963:
Reason:     The value does not need to be initialized because it was initialized in the previous steps or the value is not used.
Risk:       None
Prevention: N/A

MD_CRYPTO_30_LIBCV_2985:
Reason:     The redundant operation is a side effect of a better readability of the code.
Risk:       None
Prevention: N/A

MD_CRYPTO_30_LIBCV_3219:
Reason:     This function is inlined and therefore it has to be implemented here. The function is not used by all configurations of the implementation.
Risk:       None.
Prevention: None.

MD_CRYPTO_30_LIBCV_3219_1:
Reason:     The function is not used by all configurations of the implementation.
Risk:       None.
Prevention: None.

MD_CRYPTO_30_LIBCV_2973:
Reason:     Parameter is set in future code execution. The code section is only reachable if the value is already set before.
Risk:       This function argument points to data that is possibly unset.
Prevention: Covered by code review and component test.

MD_CRYPTO_30_LIBCV_3395:
Reason:     Define is provided by header of lower layer crypto library.
Risk:       Compiler wrongly calculates the value.
Prevention: Covered by component test.

MD_CRYPTO_30_LIBCV_3315:
Reason:     This 'switch' statement contains only a single path - it is redundant.
Risk:       None.
Prevention: None.

MD_CRYPTO_30_LIBCV_3673_1:
Reason:     Parameter cannot be set to const since it is passed as var parameter to lower layer crypto library.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_3673_2:
Reason:     Pointer can only be const when method is empty and only Dummy statements are used.
Risk:       None
Prevention: Covered by code review.

MD_CRYPTO_30_LIBCV_3673_3:
Reason:     Parameter cannot be set to const since it is used as var parameter.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_3673_API_REQUIREMENT
Reason:     Pointer cannot be const because of the Autosar API requirements.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_3678:
Reason:     Parameter cannot be set to const since it is passed as var parameter to lower layer crypto library.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_4394:
Reason:     Signed integer type is given as parameter to the lower layer crypto library, the cast has to be applied.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_5013:
Reason:     Signed integer type is given as return type by lower layer crypto library. Size does not matter since only -1 and 0 is returned.
Risk:       None
Prevention: None

MD_CRYPTO_30_LIBCV_0310_CRYPTOLIBRARY_CAST:
Reason:     It is necessary to access inner members of the structure and therefore, the cast has to be applied.
Risk:       None
Prevention: Covered by code review.

MD_CRYPTO_30_LIBCV_3305_CRYPTOLIBRARY_CAST:
Reason:     It is necessary to access inner members of the structure and therefore, the cast has to be applied.
Risk:       None
Prevention: Covered by code review.

 */

/*
\CM CM_CRYPTO_30_LIBCV_STACK_ARRAY_WITH_SIZE The stack arrays are given with its size to the called API.
\CM CM_CRYPTO_30_LIBCV_STACK_ARRAY_WITHOUT_SIZE The stack array has the same length as expected by the called API.
\CM CM_CRYPTO_30_LIBCV_WORKSPACE_STACK The workspace is located on the stack and the pointer is valid throughout the call.
\CM CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME The workspace is received via ComStackLib access macros. As it is an 0:1 relation, a runtime check for availability is performed in the Dispatching function.
\CM CM_CRYPTO_30_LIBCV_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME All job concerning in- and output buffer were checked in Crypto_30_LibCv_DetChecksServiceValues before based on service and mode and therefore are valid in this context.
 */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR
\DESCRIPTION    The caller forwards as parameter one or more pointer.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE
\DESCRIPTION    The caller passes the address of the workspace of the respective object as a parameter. It is used to save and restore context.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH
\DESCRIPTION    The caller forwards the length of the workspace of the respective object as a parameter.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_ACCESS_TO_WORKSPACE_RESTORE
\DESCRIPTION    Write access to a pointer of the workspace object which is determined by primitive dispatcher.
\COUNTERMEASURE \R The pointer is forwarded and assumed to be valid and the length values which are set by dispatcher are correct.
                According to SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_OF_WORKSPACE, SBSW_CRYPTO_30_LIBCV_FORWARDING_SIZEOFWORKSPACELENGTH and [SMI-722498].

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_AND_MEMBER_OF_JOB_PTR
\DESCRIPTION    The caller forwards as parameters a pointer and a member of a job pointer.
\COUNTERMEASURE \N The pointers are forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR_WITH_ECC_CURVE_DATA
\DESCRIPTION    The caller forwards pointers from parameters  and  pointer from global Ecc curve data.
\COUNTERMEASURE \N The pointers are forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER
\DESCRIPTION The function is called with pointers pointing to stack buffers.
\COUNTERMEASURE \N Either the sizes of the buffers are given in the call or the sizes are according to the callee requirements.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER_AND_PARAMETER_BUFFER
\DESCRIPTION The function is called with pointers pointing to stack buffers and pointer given as parameter to the caller.
\COUNTERMEASURE \N Either the sizes of the buffers are given in the call or the sizes are according to the callee requirements. It is assumed that the pointer given as parameter to the caller are valid.

\ID SBSW_CRYPTO_30_LIBCV_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK
\DESCRIPTION Crypto_30_LibCv_SheKeyUpdateDecrypt is called and a pointer to a keybuffer and to an output buffer is passed as parameter. In addition to that, a pointer to a local key buffer located on the stack is passed as parameter.
\COUNTERMEASURE \N The output buffer and the keybuffer are passed as argument to the caller and are assumed to be valid and of the correct size as requested by the callee.
The size of the local key buffer is according to the requirements of the callee.

\ID SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to a key buffer is provided with a size which is smaller then the provided buffer.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]
The key pointer is checked in function Crypto_30_LibCv_Local_KeyElementSet for size.

\ID SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to a data buffer is given with a size which is smaller then or equal to the provided buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]
The data buffer is passed as argument to the caller and is assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_OUTPUT_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to an output buffer is provided without a reference to the size of the buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]
The output buffer is passed as argument to the caller and is assumed to be valid and of the correct size as requested by the callee.

\ID SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. A pointer to an output buffer is given without a reference to the size of the buffer.
In addition to that, a pointer to a data buffer is given with a size which is smaller or equal to the provided buffer.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]
The output buffer is passed as argument to the caller and is assumed to be valid and of the correct size as requested by the callee.
The data buffer is passed as argument to the caller and is assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]

\ID SBSW_CRYPTO_30_LIBCV_ESL_CONST_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the const ram.
\COUNTERMEASURE \N The workspace is located on the const ram and the pointer is valid throughout the call.

\ID SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded which is located on the stack. Other parameters are located on the stack and have the same size as expected from the called API.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER
\DESCRIPTION  The esl library is called and a pointer to a workspace is forwarded which is located on the stack.
Other parameters are located on the stack and have the same size as expected from the called API.
In addition, there is one pointer which is already given as parameter to the calling function.
\COUNTERMEASURE \N  [CM_CRYPTO_30_LIBCV_WORKSPACE_STACK]
[CM_CRYPTO_30_LIBCV_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_LIBCV_PTR_ACCESS
\DESCRIPTION Write access to a pointer which is passed to the function.
\COUNTERMEASURE \N The pointer is assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK
\DESCRIPTION Write access to a pointer which is passed to the inlined function. The memory pointed to by the pointer is located in the stack of the calling function.
\COUNTERMEASURE \N The pointer points to memory located on the stack. The call context is synchronous and therefore, the memory stays valid throughout the call.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR
\DESCRIPTION A pointer to a key buffer is forwarded to the callee without a size.
\COUNTERMEASURE \R The key pointer is checked in function Crypto_30_LibCv_Local_KeyElementSet for size and matches the requirements of the callee.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT
\DESCRIPTION A pointer to a key buffer is provided with its size and a pointer to another key array located on the stack is provided without size. In addition to that, an output buffer is forwarded to the callee.
\COUNTERMEASURE \R The key pointer is checked in function Crypto_30_LibCv_Local_KeyElementSet for size.
The array located on the stack is sufficiently large and the call context is synchronous. The output buffer is only forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY
\DESCRIPTION The KDFbuffer is forwarded to the callee with its size. The key ptr is pointing to a stack array which corresponds to the API requirements.
\COUNTERMEASURE \N KDFbuffer is only forwarded and assumed to be valid. The buffer is located on the stack and the call is synchronous.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER
\DESCRIPTION The KDFbuffer is located on the stack and given with its size to the callee. The outputbuffer is also located on the stack and matches the callees requirements in terms of size. The pointer to the key is given as parameter to the caller and only forwarded to the callee.
\COUNTERMEASURE \N The call is synchronous and the stack arrays stay valid throughout the call. The key pointer is assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS
\DESCRIPTION Write access to array given by pointer through a parameter. The API requires the array to have a certain size and it is assumed that the size is available in the given pointer.
\COUNTERMEASURE \R The caller has to assure that the API requirements are matched and the size of the buffer is according to these requirements.

\ID SBSW_CRYPTO_30_LIBCV_CSL01
\DESCRIPTION Access of Queue, KeyElementWrittenLength, KeyElement, RandomSeedBuffer and Lock using ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_CSL01_OBJECTID
\DESCRIPTION Access of WrittenLength, BufferLength, DriverObjectState and Lock using ComStackLib. The GetSizeOf-check is performed in one of the global APIs Crypto_30_LibCv_CancelJob or Crypto_30_LibCv_ProcessJob.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_CSL02_QUEUE_VIA_DRIVEROBJECT
\DESCRIPTION Access of Queue (via indirection over DriverObject) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT
\DESCRIPTION Access of KeyStorage (via indirection over keyElement) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY
\DESCRIPTION Access of KeyElement (via indirection over Key) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK
\DESCRIPTION Access of NvBlock (via indirection NvBlockId) with same index using ComStackLib.
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE
\DESCRIPTION Access of array located on stack.
\COUNTERMEASURE \N The static size of the array is used as upper bound and therefore no illegal write access is done.

\ID SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE
\DESCRIPTION Access of global array located in data segment.
\COUNTERMEASURE \N The static size of the array is used as upper bound and therefore no illegal write access is done.

\ID SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR
\DESCRIPTION One or more variables located on the stack are given as parameter pointer to a function.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR_AND_MEMBER_OF_JOB_PTR
\DESCRIPTION One or more variables located on the stack and one or more members of job pointer are given as parameter pointer to a function.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR
\DESCRIPTION A pointer to an array located on the stack is given as parameter to a function including its size.
\COUNTERMEASURE \N The function call is synchronous and the size of the array is given, therefore the stack array is valid for the function call.

\ID SBSW_CRYPTO_30_LIBCV_ACCESS_TO_ERRORID_PTR
\DESCRIPTION Access of errorId pointer to set the errorId. The context is an inlined function which is always called synchronously from Crypto_30_LibCv_Process,
where the variable is located on the stack.
\COUNTERMEASURE \N The function call is synchronous and the variable errorId has to be located on the stack.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_PTR
\DESCRIPTION The call is only forwarded and has as parameter a job pointer.
\COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_UPDATE_OF_JOB_STATE
\DESCRIPTION Direct write access to state member of job ptr. The context is the inlined function Crypto_30_LibCv_UpdateJobState,
which is always called synchronously from Crypto_30_LibCv_Process.
\COUNTERMEASURE \R The ptr is checked in the global API Crypto_ProcessJob or Crypto_MainFunction.

\ID SBSW_CRYPTO_30_LIBCV_DIRECT_ACCESS_TO_JOB_STATE
\DESCRIPTION Direct write access to state member of job ptr. The context is the inlined function Crypto_30_LibCv_Process,
which is always called synchronously from Crypto_30_LibCv_ProcessJob or Crypto_30_LibCv_MainFunction.
\COUNTERMEASURE \R The job object is already checked in global API.

\ID SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR
\DESCRIPTION Write access to a pointer of the jobs primitive input output object.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]

\ID SBSW_CRYPTO_30_LIBCV_ACCESS_OF_JOB_MEMBER_CANCEL_JOB
\DESCRIPTION Write access to the state member of job pointer.
\COUNTERMEASURE \N The pointer is checked for validity in the beginning of the Crypto_30_LibCv_CancelJob function.

\ID SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_INIT
\DESCRIPTION Direct array access to key storage array generated by ComStackLib.
The access is in bounds since it is iterated over all key elements and the access is either direct or via indirection (KeyElement to KeyStorage).
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_NV
\DESCRIPTION Direct array access to key storage array generated by ComStackLib.
The access is in bounds since it is iterated over all key elements and the access is either direct or via indirection (KeyElement to KeyStorage).
\COUNTERMEASURE \N Qualified use case CSL02 of ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_INIT_ESL_WORKSPACE
\DESCRIPTION The function esl_initWorkSpaceHeader is called with a pointer to the workspace header and a size.
The workspace pointer was received by calling Crypto_30_LibCv_GetWorkspaceOf<Name of Primitive> which is provided by the ComStackLib.
\COUNTERMEASURE \R CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME

\ID SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE
\DESCRIPTION The function esl_initWorkSpaceHeader is called with a pointer to the workspace header and a size. The workspace is located on the stack.
\COUNTERMEASURE \N The call to esl_initWorkSpaceHeader is synchronous and therefore, the workspace stays valid during the lifetime of the call.

\ID SBSW_CRYPTO_30_LIBCV_ACT_RANDOM_WORKSPACE_ARRAY_ACCESS
\DESCRIPTION Array access to X_KEY member of random workspace pointer of type ActFIPS186STRUCT.
\COUNTERMEASURE \N The validity of the pointer to the random workspace is given since the workspace is provided by the ComStackLib and the access is in bounds.
Since the accessing index is limited by the static size of the X_KEY array, it is ensured that is in range.

\ID SBSW_CRYPTO_30_LIBCV_WORKSPACE_MEMBER_ACCESS
\DESCRIPTION Access to member of workspace pointer.
\COUNTERMEASURE \N The validity of the pointer to the workspace is given since the workspace is provided by the ComStackLib.

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE
\DESCRIPTION The esl library is called and a pointer to a workspace is forwarded. The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS
\DESCRIPTION The esl library is called and a pointer to a workspace as well as in - and output buffers are forwarded.
The pointer to the workspace is already initialized before and therefore valid. All other buffers are checked before in local function Crypto_30_LibCv_Local_DetChecksServiceValues,
which is called in the context of the global API Crypto_30_LibCv_Process.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
\R [CM_CRYPTO_30_LIBCV_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS_AND_NULL_PTR
\DESCRIPTION The esl library is called and a pointer to a workspace as well as in - and output buffers are forwarded.
The pointer to the workspace is already initialized before and therefore valid. All other buffers are checked before in local function Crypto_30_LibCv_Local_DetChecksServiceValues,
which is called in the context of the global API Crypto_30_LibCv_Process.
There is a optional pointer which is allowed to be a NULL_PTR.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
\R [CM_CRYPTO_30_LIBCV_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]
\R The called function allows that the param pointer is a NULL_PTR.

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER
\DESCRIPTION The esl library is called and a pointer to a workspace as well as buffer and lengths of stack variables are given as parameter to the function call.
The pointer to the workspace is already initialized before and therefore valid. All other buffers are located on the stack and the function call is synchronous. Therefore, the memory
stays valid throughout the call.
\COUNTERMEASURE \N All pointers point to stack variables and the length is also provided to the called function.

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFER_AND_STACK_BUFFER
\DESCRIPTION The esl library is called with a pointer to a workspace,
a pointer located at the job object (pointing to a buffer) with an length denoting the length of the buffer
and a pointer to an buffer which is located on the stack and without size.
\COUNTERMEASURE \R  [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
[CM_CRYPTO_30_LIBCV_JOB_INPUT_OUTPUT_BUFFERS_RUNTIME]
[CM_CRYPTO_30_LIBCV_STACK_ARRAY_WITHOUT_SIZE]

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_NULL_PTR
\DESCRIPTION The esl library is called with a pointer to a workspace and a null pointer is given to the library as parameter.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
The null pointer is a valid input to the calling function.

\ID SBSW_CRYPTO_30_LIBCV_CALL_WITH_NULL_PTR
\DESCRIPTION The NvM is called with a null pointer is given as parameter.
\COUNTERMEASURE \N The null pointer is a valid input to the calling function.

\ID SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_STACK_BUFFER_NOLENGTHGIVEN
\DESCRIPTION The esl library is called and a pointer to a workspace as well as pointer to stack arrays are given without an information how large the arrays are.
This is due to the fact that these esl library function do not provide an parameter for the length of the buffer.
\COUNTERMEASURE \N The stack arrays have the same length as the esl library expects.

\ID SBSW_CRYPTO_30_LIBCV_CALL_WITH_WORKSPACE
\DESCRIPTION The service is called and a pointer to a workspace is forwarded. The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \R [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_JOB_PTR
\DESCRIPTION The call is only forwarded and has as parameter a pointer to a workspace a job pointer.
The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
The job pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_WORKSPACE_AND_GLOBAL_DATA_PTR
\DESCRIPTION The call is only forwarded and has as parameter a pointer to a workspace a job pointer.
The workspace pointer points to a workspace provided by ComStackLibrary via Indirection over ObjectId.
\COUNTERMEASURE \N [CM_CRYPTO_30_LIBCV_WORKSPACE_VIA_COMSTACKLIB_RUNTIME]
The job pointer is forwarded and assumed to be valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_JOB_AND_GLOBAL_ARRAY_PTR
\DESCRIPTION The call is only forwarded and has as parameter a pointer to a job and a pointer to a global
array located in data segment.
\COUNTERMEASURE \N The job pointer is forwarded and assumed to be valid. For the pointer to a global array
the static size of the array is used as upper bound and therefore no illegal access is done.

\ID SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK
\DESCRIPTION The value of a pointer to a variable located on the stack is written.
\COUNTERMEASURE \N The function call is synchronous and there is no array access.

\ID SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK
\DESCRIPTION The array is written with an index, the size of the array is known in this context.
\COUNTERMEASURE \R It is checked that there is no out of bounds via the provided length.

\ID SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK
\DESCRIPTION Write access to a pointer.
\COUNTERMEASURE \R The pointer is checked for validity before.

\ID SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK
\DESCRIPTION One or more pointer are forwarded as parameter to the caller.
\COUNTERMEASURE \R The pointer are checked for validity before.

\ID SBSW_CRYPTO_30_LIBCV_RNG_SEED
\DESCRIPTION Access to KeyElementState via indirection over Key.
\COUNTERMEASURE \N The index is not explicitly checked. Since the code is only executed if the Crypto_30_LibCv_KeyElementSet has successfully written the same KeyElement, it is guaranteed that the key element exist and therefore the index is also valid.

\ID SBSW_CRYPTO_30_LIBCV_GETVERSIONINFO
\DESCRIPTION The function Crypto_30_LibCv_GetVersionInfo writes to the object referenced by parameter VersionInfo
\COUNTERMEASURE \N The caller ensures that the pointers passed to the parameter VersionInfo is valid.

\ID SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER
\DESCRIPTION The NvBuffer is forwarded and accessed without a length given to the service.
\COUNTERMEASURE \R  Qualified use case CSL02 of ComStackLib.

SBSW_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

 \ID COV_CRYPTO_30_LIBCV_LOCAL_DEFINE
 \ACCEPT XF
 \ACCEPT TX
 \REASON CRYPTO_30_LIBCV_LOCAL is always defined externally.

 \ID COV_CRYPTO_30_LIBCV_COMPATIBILITY_DEFINE
 \ACCEPT TX
 \REASON Define is always defined internally but cane be overwritten for compatibility.

 \ID COV_CRYPTO_30_LIBCV_CSM_DEFINES
 \ACCEPT XF
 \ACCEPT TX
 \REASON This defines could be defined in other crypto drivers as well. In order to increase flexibility, defines are defined in crypto driver if not provided yet.

 \ID COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT
 \ACCEPT X
 \REASON This else/default statements are required by misra but cannot be reached in code. The statement is captured by upper Api.

 \ID COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE
 \ACCEPT XF
 \ACCEPT TX
 \REASON This encapsulation is for different platform types. Not all platform types can be covered by development test. Covered by code review and integration test.

 \ID COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ELEMENTS
 \ACCEPT TX
 \REASON The Crypto needs custom key elements, which could be defined in other modules as well.

 \ID COV_CRYPTO_30_LIBCV_CUSTOM_ALGORITHM
 \ACCEPT TX
 \REASON The Crypto needs custom algorithm and algorithm mode defines, which could be defined in other modules as well.

 \ID COV_CRYPTO_30_LIBCV_CUSTOM_KEY_ALGORITHM
 \ACCEPT TX
 \REASON The Crypto needs custom key algorithm, which could be defined in other modules as well.

 \ID COV_CRYPTO_30_LIBCV_KEY_LOCKING_RELEASE
 \ACCEPT TX
 \REASON This check can only be false if there is a implementation mistake.

 \ID COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT
 \ACCEPT TX
 \REASON This check can only be false if the parameter is defined externally.

 \ID COV_CRYPTO_30_LIBCV_SHE_ECU_MASTER_KEY
 \ACCEPT TX
 \ACCEPT XF
 \REASON The result of the check depends of the SHE configuration. If the ECU_MASTER_KEY is configured the result can be TX otherwise XF.

 \ID COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION
 \ACCEPT TF
 \ACCEPT XF
 \REASON The result of the  check depends of the SHE configuration. If the required key is not configured the result can be XF.

 \ID COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2
 \ACCEPT TF
 \ACCEPT TX
 \REASON The result of the  check depends of the SHE configuration. If the required key is not configured the result can be TX.

 \ID COV_CRYPTO_30_LIBCV_SERVICE_CONFIGURATION
 \ACCEPT TX
 \ACCEPT TF
 \REASON The result of the check depends of the service configuration. If all services are enabled the result can be TX.

 \ID COV_CRYPTO_30_LIBCV_SERVICE_CONFIGURATION_2
 \ACCEPT XF
 \ACCEPT TF
 \REASON The result of the check depends of the service configuration. If no service with key write access is enabled the result can be XX.

 \ID COV_CRYPTO_30_LIBCV_ECDSA_CONFIGURATION
 \ACCEPT TF
 \ACCEPT TX
 \ACCEPT XF
 \REASON The result of the check depends of the ECDSA configuration. If pre hash length is longer than the curve length the result is always TX. If pre hash length is shorter than the curve length the result is always TX.

 \ID COV_CRYPTO_30_LIBCV_ECDSA_PREHASHING_ENABLED
 \ACCEPT TF
 \ACCEPT TX
 \ACCEPT XF
 \REASON The result of the check depends of the ECDSA configuration. In a configuration where only curve SECp521r1 is supported, the ECDSA is always used together with SHA512 prehashing and thus,
         the algorithm secondary family is always set.

 \ID COV_CRYPTO_30_LIBCV_ASR_COMPATIBILITY
 \ACCEPT XF
 \ACCEPT TX
 \REASON The Crypto_JobType differs between the different ASR version to support ASR 4.3.0 the defines ca be overwritten. Standard is ASR 4.3.1.

 \ID COV_CRYPTO_30_LIBCV_BINARYSEARCH
 \ACCEPT TX
 \REASON This check can never be false, as the searched list is generated as an ordered list without gaps.
         Consequently, the used binary search algorithm will always find the searched entry if the list was generated correctly,
         and the check will never be false.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  FETA JUSTIFICATIONS
 *********************************************************************************************************************/

 /* FETA_JUSTIFICATION_BEGIN

 \ID FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA
 \DESCRIPTION Loop bounds are determined by static GenData which is accessed via ComStackLib.
 \COUNTERMEASURE \N The here applied loop uses variables as lower and upper bound which are set via a ComStackLib macro
                    and remain unchanged during loop execution. The lower bound variable is guaranteed to be less 
                    than or equal to the upper bound variable. The loop's type of the counter variable is appropriate 
                    for the loop's type of lower and upper bound determining variables as it is an iteration type suitable for the 
                    used CSL indirection. The counter variable is increased once within each single loop iteration (strictly monotonically increasing)
                    until it reaches a value that makes the loop stop without any occurrences of overflows. This
                    is done by a comparison used as loop termination condition that considers the amount of the
                    increment that is applied within each single loop.
                    
 \ID FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_UPPER_BOUND_STATIC_GENDATA
 \DESCRIPTION Loop upper bound is determined by static GenData which is accessed via ComStackLib.
 \COUNTERMEASURE \N The here applied loop uses a variables as upper bound which is set via a ComStackLib macro
                    and remains unchanged during loop execution. The loop's type of the counter variable is appropriate
                    for the loop's type of lower and upper bound determining variables as it is an iteration type suitable for the
                    used CSL indirection. The counter variable is increased once within each single loop iteration (strictly monotonically increasing)
                    until it reaches a value that makes the loop stop without any occurrences of overflows. This
                    is done by a comparison used as loop termination condition that considers the amount of the
                    increment that is applied within each single loop.

 \ID FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_WITH_CHECK
 \DESCRIPTION Loop upper bound is determined by a constant parameter which is passed to the function.
 \COUNTERMEASURE \R The caller ensures correctness of the parameter by checking it for boundary conditions 
                    described in the function doxygen comment. 
                    [CM_CRYPTO_LIBCV_FOR_LOOP_WITH_NON_CHANGING_UPPER_BOUND]

 \ID FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND_PTR
 \DESCRIPTION Loop upper bound is determined by a pointer parameter which is passed to the function.
 \COUNTERMEASURE \N [CM_CRYPTO_LIBCV_FOR_LOOP_WITH_NON_CHANGING_UPPER_BOUND]

 \ID FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND
 \DESCRIPTION Loop upper bound is determined by a constant parameter which is passed to the function.
 \COUNTERMEASURE \N [CM_CRYPTO_LIBCV_FOR_LOOP_WITH_NON_CHANGING_UPPER_BOUND]

 \ID FETA_CRYPTO_30_LIBCV_BINARY_SEARCH
 \DESCRIPTION While loop which is used for a binary search of a cryptoKeyId via its elementIndex.
 \COUNTERMEASURE \N The here applied while loop is controlled by a comparison of the variables minKey and maxKey,
                    which are of the same type.
                    Within each single loop execution:
                       - the value of variable middleKey, which is of an approriate type, is caluclated and then it is: minKey <= middleKey < maxKey,
                       - Then
                         - either maxKey becomes middleKey, so right decreases
                         - or minKey becomes (middleKey + 1), so left increases
                         - or keyFound becomes TRUE which makes the loop stop by a call of break
                         So with each single loop the difference (maxKey - minKey) becomes smaller (strictly monotonically decreasing) and 
                         the loop will finally stop after a maximum of (maxKey - minKey) loop executions.


 \CM CM_CRYPTO_LIBCV_FOR_LOOP_WITH_NON_CHANGING_UPPER_BOUND 
    The loop's type of the counter variable is appropriate for the loop's type of upper bound
    variable as it is the same type as the bound determining variables. The upper bound remains unchanged during loop execution.
    The counter variable is increased once within each single loop iteration (strictly monotonically increasing)
    until it reaches a value that makes the loop stop without any occurrences of overflows. This
    is done by a comparison used as loop termination condition that considers the amount of the
    increment that is applied within each single loop.

 FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv.c
 *********************************************************************************************************************/

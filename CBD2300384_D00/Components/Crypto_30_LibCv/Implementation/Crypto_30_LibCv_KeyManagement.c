/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_LibCv_KeyManagement.c
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_KEYMANAGEMENT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_LibCv.h"
#include "Crypto_30_LibCv_Services.h"
#include "Crypto_30_LibCv_Curve.h"
#include "Crypto_30_LibCv_RandomSeed.h"
#include "Crypto_30_LibCv_KeyExchange.h"
#include "Crypto_30_LibCv_KeyGenerate.h"
#include "Crypto_30_LibCv_KeyDerive.h"
#include "Crypto_30_LibCv_KeySetValid.h"
#include "Crypto_30_LibCv_InternalApi.h"

#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
# include "NvM.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if !defined (CRYPTO_30_LIBCV_LOCAL) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL                                        static
#endif

#if !defined (CRYPTO_30_LIBCV_LOCAL_INLINE) /* COV_CRYPTO_30_LIBCV_LOCAL_DEFINE */
# define CRYPTO_30_LIBCV_LOCAL_INLINE                                 LOCAL_INLINE
#endif

#define Crypto_30_LibCv_GetKeyElementWrittenLength(Index)             ((uint32)(((uint32)Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))) << 8) | (uint32)Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements((Index))+1u)))
#define CRYPTO_30_LIBCV_EMPTY_KEY_LENGTH                              (0u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_KEY                                (16u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT                    (16u)
#define CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER                             (CRYPTO_30_LIBCV_SIZEOF_SHE_KEY+ CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT)
#define CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER                             (CRYPTO_30_LIBCV_SIZEOF_SHE_KEY+ CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT)

#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_UID                         (0u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID                             (15u)
#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS                         (CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M1_IDS                             (1u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M1                                 (CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID + CRYPTO_30_LIBCV_SIZEOF_SHE_M1_IDS)

#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M2                             (CRYPTO_30_LIBCV_SIZEOF_SHE_M1)
#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M2_COUNTER                     (CRYPTO_30_LIBCV_STARTINDEX_SHE_M2)
#define CRYPTO_30_LIBCV_MASK_SHE_M2_COUNTER                           (0xFFFFFFF0uL)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M2_COUNTER_BIT                     (28u)
#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M2_KEYFLAG                     (CRYPTO_30_LIBCV_STARTINDEX_SHE_M2)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M2                                 (32u)

#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M3                             (CRYPTO_30_LIBCV_STARTINDEX_SHE_M2 + CRYPTO_30_LIBCV_SIZEOF_SHE_M2)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M3                                 (16u)

#define CRYPTO_30_LIBCV_STARTINDEX_SHE_M4                             (CRYPTO_30_LIBCV_STARTINDEX_SHE_M3 + CRYPTO_30_LIBCV_SIZEOF_SHE_M3)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M4                                 (32u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES              (4u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M4_ENC                             (16u)

#define CRYPTO_30_LIBCV_SIZEOF_SHE_M5                                 (16u)

#define CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3                              (CRYPTO_30_LIBCV_SIZEOF_SHE_M1 + CRYPTO_30_LIBCV_SIZEOF_SHE_M2 + CRYPTO_30_LIBCV_SIZEOF_SHE_M3)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5                              (CRYPTO_30_LIBCV_SIZEOF_SHE_M4 + CRYPTO_30_LIBCV_SIZEOF_SHE_M5)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M5                              (CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3 + CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5)

#define CRYPTO_30_LIBCV_SHE_M1_SECRET_KEY_ID                          (0u)
#define CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID                      (1u)
#define CRYPTO_30_LIBCV_SHE_M1_MAC_KEY_ID                             (2u)
#define CRYPTO_30_LIBCV_SHE_M1_MAC_ID                                 (3u)
#define CRYPTO_30_LIBCV_SHE_M1_KEY_N_START_ID                         (4u)
#define CRYPTO_30_LIBCV_SHE_M1_KEY_N_END_ID                           (13u)
#define CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID                             (14u)
#define CRYPTO_30_LIBCV_SHE_NUM_KEYS                                  (25u)

#define CRYPTO_30_LIBCV_SHE_TYPE_SECRET_KEY                           (0u)
#define CRYPTO_30_LIBCV_SHE_TYPE_MASTER_KEY                           (1u)
#define CRYPTO_30_LIBCV_SHE_TYPE_MAC_KEY                              (2u)
#define CRYPTO_30_LIBCV_SHE_TYPE_MAC                                  (3u)
#define CRYPTO_30_LIBCV_SHE_TYPE_KEY_N                                (4u)
#define CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY                              (5u)
#define CRYPTO_30_LIBCV_SHE_NUM_KEY_TYPES                             (6u)

#define CRYPTO_30_LIBCV_SHE_UID_WILDCARD_VALUE                        (0x00u)

#define CRYPTO_30_LIBCV_SHE_PAGE0                                     (0u)
#define CRYPTO_30_LIBCV_SHE_PAGE1                                     (1u)

#define CRYPTO_30_LIBCV_SIZEOF_SHE_CHALLENGE                          (16u)
#define CRYPTO_30_LIBCV_SIZEOF_CMAC                                   (16u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_STATUS                             (1u)
#define CRYPTO_30_LIBCV_SIZEOF_SHE_CMD_GET_ID_OUTPUT                  (CRYPTO_30_LIBCV_SIZEOF_SHE_STATUS + CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID + CRYPTO_30_LIBCV_SIZEOF_CMAC)

#define CRYPTO_30_LIBCV_SHE_CLEARED_STATUS                            (0x00u) /* Cleared status */
#define CRYPTO_30_LIBCV_SHE_STATUS_BUSY                               (0x01u) /* Whenever SHE is processing a command. */
#define CRYPTO_30_LIBCV_SHE_STATUS_SECURE_BOOT                        (0x02u) /* If the secure booting is activated. */
#define CRYPTO_30_LIBCV_SHE_STATUS_BOOT_INIT                          (0x04u) /* If the secure booting has been personalized during the boot sequence. */
#define CRYPTO_30_LIBCV_SHE_STATUS_BOOT_FINISHED                      (0x08u) /* When the secure booting has been finished by calling either CMD_BOOT_FAILURE or CMD_BOOT_OK or if CMD_SECURE_BOOT failed in verifying BOOT_MAC. */
#define CRYPTO_30_LIBCV_SHE_STATUS_BOOT_OK                            (0x10u) /* If the secure booting (CMD_SECURE_BOOT) succeeded. If CMD_BOOT_FAILURE is called the bit is erased. */
#define CRYPTO_30_LIBCV_SHE_STATUS_RND_INIT                           (0x20u) /* If the random number generator has been initialized. */
#define CRYPTO_30_LIBCV_SHE_STATUS_EXT_DEBUGGER                       (0x40u) /* If an external debugger is connected to the chip, i.e. it reflects the input for debugger activation. */
#define CRYPTO_30_LIBCV_SHE_STATUS_INT_DEBUGGER                       (0x80u) /* If the internal debugging mechanisms of SHE are activated */

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION               (0x80u)
/* The FID for boot protection is only checked if the feature is enabled.
 * Otherwise use 0x00 Mask to ignore boot protection. */
#  if (CRYPTO_30_LIBCV_KEYELEMENTSBOOTPROTECTIONIDXOFSHEPAGE == STD_ON)
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION               (0x40u)
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION_INV           (0xBFu)
#  else
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION               (0x00u)
#  endif
/* The FID for debugger protection is only checked if the feature is enabled.
 * Otherwise use 0x00 Mask to ignore debugger protection. */
#  if (CRYPTO_30_LIBCV_KEYELEMENTSDEBUGGERPROTECTIONIDXOFSHEPAGE == STD_ON)
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION           (0x20u)
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION_INV       (0xDFu)
#  else
#   define CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION           (0x00u)
#  endif
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE                      (0x10u)
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD                       (0x08u)
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_CMAC_USAGE                     (0x04u)
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED                         (0x02u)
#  define CRYPTO_30_LIBCV_SHE_FID_MASK_DEFAULT                        (0x00u)
# endif
#endif

/* NvM Block Handling */
/* State:
 * 1Byte State: [ Bit7-3 not used | DataCopied | WriteRequested| DataChanged ]
 */
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE                (0x01u)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_WRITE_REQUESTED            (0x02u)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_WRITE_COPIED               (0x04u)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_CLEAR_WRITE_COPIED         (0xFBu)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_CLEAR_WRITE_REQUESTED      (0xFDu)

#define CRYPTO_30_LIBCV_NVBLOCK_STATE_IDLE                            (0x00u)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_WRITE_REQ_PENDING               (CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_WRITE_REQ                       (CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE|CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_WRITE_REQUESTED)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_COPY_DATA                       (CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_WRITE_COPIED)
#define CRYPTO_30_LIBCV_NVBLOCK_STATE_REQ_WHILE_PENDING               (CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_WRITE_COPIED|CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE)

/* Nv Header Length and Position */
#define CRYPTO_30_LIBCV_POS_NVBLOCK_VERSION                           (0u)
#define CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_VERSION                        (1u)
#define CRYPTO_30_LIBCV_POS_NVBLOCK_FLAGS                             (CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_VERSION)
#define CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_FLAGS                          (1u)
#define CRYPTO_30_LIBCV_POS_NVBLOCK_CRC                               (CRYPTO_30_LIBCV_POS_NVBLOCK_FLAGS+CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_FLAGS)
#define CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_CRC                            (4u)
#define CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_HEADER_CRC                     (CRYPTO_30_LIBCV_POS_NVBLOCK_CRC + CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_CRC)

/* Nv Version and Flags */
#define CRYPTO_30_LIBCV_NVBLOCK_VERSION_0                             (0u)
#define CRYPTO_30_LIBCV_NVBLOCK_FLAGS_0                               (0u)

/* Nvm Service Id for NvM_WriteBlock */
#ifndef CRYPTO_30_LIBCV_NVM_WRITE_BLOCK /* COV_CRYPTO_30_LIBCV_COMPATIBILITY_DEFINE */
# define CRYPTO_30_LIBCV_NVM_WRITE_BLOCK                              (7u) /* Service ID NvM_WriteBlock() */
#endif
/* Nvm Service Id for NvM_WriteBlock */
#ifndef CRYPTO_30_LIBCV_NVM_WRITE_ALL /* COV_CRYPTO_30_LIBCV_COMPATIBILITY_DEFINE */
# define CRYPTO_30_LIBCV_NVM_WRITE_ALL                                (13u) /* Service ID NvM_WriteAll() */
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* SHE Key Update */
#define Crypto_30_LibCv_She_M1_GetAuthId(value)                       ((uint8)((value) & 0x0Fu)) /* Mask authentication id */
#define Crypto_30_LibCv_She_M1_GetId(value)                           ((uint8)((value) >> 4)) /* Mask update SHE id */
#define Crypto_30_LibCv_She_M1_BuildIds(Id, AuthId)                   (((uint8)(((Id) << 4) & 0xF0u)) | ((uint8)((AuthId) & 0x0Fu)))/* Mask update SHE id */
#define Crypto_30_LibCv_She_IsDebugCmd(cryptoKeyId, elementId)        ((cryptoKeyId == Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate()) && (elementId == CRYPTO_KE_CUSTOM_SHE_DEBUG_CMD))

/* Convert Counter from M1 to Store Value */
#define Crypto_30_LibCv_She_ConvertCounter(value)                     (((value) & CRYPTO_30_LIBCV_MASK_SHE_M2_COUNTER) >> 4)

/* Get SHE Constant */
#define Crypto_30_LibCv_She_GetPtrMacConst(indexOfSheKey)             (Crypto_30_LibCv_GetAddrSheConstants(Crypto_30_LibCv_GetSheConstantsMacStartIdxOfShePage(Crypto_30_LibCv_GetShePageIdxOfSheKeys((indexOfSheKey)))))
#define Crypto_30_LibCv_She_GetPtrEncConst(indexOfSheKey)             (Crypto_30_LibCv_GetAddrSheConstants(Crypto_30_LibCv_GetSheConstantsEncStartIdxOfShePage(Crypto_30_LibCv_GetShePageIdxOfSheKeys((indexOfSheKey)))))

/* NvM Block Handling */
#define Crypto_30_LibCv_GetNvBlockState(blockIdx)                     (Crypto_30_LibCv_NvBlock_State[blockIdx])
#define Crypto_30_LibCv_SetNvBlockState(blockIdx, state)              ((Crypto_30_LibCv_NvBlock_State[blockIdx]) = (state))
#define Crypto_30_LibCv_IsNvBlockState(blockIdx, state)               ((Crypto_30_LibCv_NvBlock_State[blockIdx]) == (state))
#define Crypto_30_LibCv_IsNvBlockStateMask(blockIdx, Mask)            (((Crypto_30_LibCv_NvBlock_State[blockIdx]) & (Mask)) == (Mask))
#define Crypto_30_LibCv_SetNvBlockStateMask(blockIdx, Mask)           ((Crypto_30_LibCv_NvBlock_State[blockIdx]) = (Crypto_30_LibCv_NvBlock_State_Type)((Crypto_30_LibCv_NvBlock_State[blockIdx]) | (Mask)))
#define Crypto_30_LibCv_ClearNvBlockStateMask(blockIdx, Mask)         ((Crypto_30_LibCv_NvBlock_State[blockIdx]) = (Crypto_30_LibCv_NvBlock_State_Type)((Crypto_30_LibCv_NvBlock_State[blockIdx]) & (Mask)))

#ifndef Crypto_30_LibCv_NvM_WriteBlock /* COV_CRYPTO_30_LIBCV_COMPATIBILITY_DEFINE */
# define Crypto_30_LibCv_NvM_WriteBlock                               Crypto_30_LibCv_GetNvWriteBlockFctNameOfNvStorage()
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Typedef for she key type */
#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
typedef uint8 Crypto_30_LibCv_SheKeyTypeType;
#endif

/* NvM Block Handling */
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
typedef uint8 Crypto_30_LibCv_NvBlock_State_Type;
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_SheKeyCheckFlags[CRYPTO_30_LIBCV_SHE_NUM_KEY_TYPES - 1u] = /* Ram Key is not checked. */   /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
{
  /* SECRET_KEY     */  CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION,
  /* ECU_MASTER_KEY */  CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD,
  /* BOOT_MAC_KEY   */  CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD,
  /* BOOT_MAC       */  CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD,
  /* KEY_N          */  CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE | CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD | CRYPTO_30_LIBCV_SHE_FID_MASK_CMAC_USAGE,
};

CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_SheKeySetAccessFlags[CRYPTO_30_LIBCV_SHE_NUM_KEY_TYPES - 1u] = /* Ram Key is not checked. */  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
{
  /* SECRET_KEY     */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED,
  /* ECU_MASTER_KEY */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED,
  /* BOOT_MAC_KEY   */  CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE | CRYPTO_30_LIBCV_SHE_FID_MASK_CMAC_USAGE,
  /* BOOT_MAC       */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED,
  /* KEY_N          */  0u,
};

CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_SheKeyServiceFlags[CRYPTO_30_LIBCV_SHE_NUM_SERVICES] =  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
{
  /* Enc/Decrypt     */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED | CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE,
  /* Mac Generate    */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED | CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE | CRYPTO_30_LIBCV_SHE_FID_MASK_CMAC_USAGE,
  /* Mac Verify      */  CRYPTO_30_LIBCV_SHE_FID_MASK_DENIED | CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE,
  /* SHE Update Read */  CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION | CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION
};

CRYPTO_30_LIBCV_LOCAL CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_SheKeyServiceFlagsResult[CRYPTO_30_LIBCV_SHE_NUM_SERVICES] =  /* PRQA S 3218 */ /* MD_CRYPTO_30_LIBCV_8.7 */
{
  /* Enc/Decrypt     */  0u,
  /* Mac Generate    */  CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE, /* Use for Mac */
  /* Mac Verify      */  CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE, /* Use for Mac */
  /* SHE Update Read */  0u
};
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */
#endif /* (CRYPTO_30_LIBCV_SHEKEYS == STD_ON) */

#define CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_LIBCV_START_SEC_VAR_NOINIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
/* It is only allowed to access this data if the SheInfoKey is locked */
CRYPTO_30_LIBCV_LOCAL VAR(uint8, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_She_Debug_Cmd_Challenge[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
# endif
#endif

/* NvM Block Handling */
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
CRYPTO_30_LIBCV_LOCAL VAR(Crypto_30_LibCv_NvBlock_State_Type, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_NvBlock_State[Crypto_30_LibCv_GetSizeOfNvBlock()];
#endif

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
/* It is only allowed to access this data if the SheInfoKey is locked */
VAR(boolean, CRYPTO_30_LIBCV_VAR_NOINIT) Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag;
# endif
#endif

#define CRYPTO_30_LIBCV_STOP_SEC_VAR_NOINIT_8BIT
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CRYPTO_30_LIBCV_START_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck()
 **********************************************************************************************************************/
/*! \brief         Check access rights for copy key element
 *  \details       The key element can only be copied, if the destination key element write access right is less than
 *                 WA_INTERNAL_COPY.
 *                 Additional the read access right of the source must be less than RA_INTERNAL_COPY and the destination
 *                 read access right must be higher or equal than the source read access right.
 *  \param[in]     elementIndexSrc              Holds the element index of the source key element. Has to be in bounds.
 *  \param[in]     elementIndexDst              Holds the element index of the destination key element. Has to be in bounds.
 *  \return        E_OK                         Request successful.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *  \pre           elementIndexSrc has to be a valid element storage index.
 *                 elementIndexDst has to be a valid element storage index.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementCopy()
 *********************************************************************************************************************/
/*! \brief         Copy key element
 *  \details       Copies a key element to another key element in the same crypto driver.
 *  \param[in]     elementIndexSrc              Holds the element index of the source key element. Has to be in bounds.
 *  \param[in]     elementIndexDst              Holds the element index of the destination key element. Has to be in bounds.
 *  \param[in]     dstCryptoKeyId               Holds the identifier of the destination key whose key element shall be set.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *  \pre           elementIndexSrc has to be a valid element storage index.
 *                 elementIndexDst has to be a valid element storage index.
 *                 dstCryptoKeyId has to be a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst,
  uint32 dstCryptoKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementCopyPartial()
 **********************************************************************************************************************/
/*! \brief         Copy key element partial
 *  \details       Copies a key element to another key element in the same crypto driver. The keyElementSourceOffset
 *                 and keyElementCopyLength allows to copy just a part of the source key element into the destination.
 *                 The offset of the target key is also specified with this function.
 *                 The key element can only be copied, if the destination key element write access right is less than
 *                 WA_INTERNAL_COPY.
 *                 Additional the read access right of the source must be less than RA_INTERNAL_COPY and the destination
 *                 read access right must be higher or equal than the source read access right.
 *  \param[in]     elementIndexSrc              Holds the element index of the source key element. Has to be in bounds.
 *  \param[in]     elementIndexDst              Holds the element index of the destination key element. Has to be in bounds.
 *  \param[in]     dstCryptoKeyId               Holds the identifier of the destination key whose key element shall be set.
 *  \param[in]     keyElementSourceOffset  Holds the offset of the of the source key element indicating the start index
 *                                         of the copy operation.
 *  \param[in]     keyElementTargetOffset  Holds the offset of the of the target key element indicating the start index
 *                                         of the copy operation.
 *  \param[in]     keyElementCopyLength    Holds the number of bytes that shall be copied.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_KEY_READ_FAIL       Request failed, read access was denied.
 *                 CRYPTO_E_KEY_WRITE_FAIL      Request failed, write access was denied.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, the key element sizes are not compatible.
 *                 CRYPTO_E_KEY_EMPTY           Request failed, uninitialized source key element.
 *  \pre           elementIndexSrc and elementIndexDst must be valid.
 *                 dstCryptoKeyId has to be a valid key id.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopyPartial(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst,
  uint32 dstCryptoKeyId,
  uint32 keyElementSourceOffset,
  uint32 keyElementTargetOffset,
  uint32 keyElementCopyLength);

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetLengthCheck()
 **********************************************************************************************************************/
/*!
 * \brief         Check the length.
 * \details       Check the length according to lengthCheck (NONE, MAX, EQUAL).
 *                NONE  Provide key always / key length is written to resultLengthPtr.
 *                MAX   Provide key if  the stored key length is smaller or equal to resultLengthPtr / key length is written to resultLengthPtr.
 *                EQUAL Provide key if  the stored key length is equal to resultLengthPtr / key length is written to resultLengthPtr.
 * \param[in]     resultLengthptr  Pointer which contains the size of the available buffer.
 * \param[in]     elementIndex     Index of the Key element.
 * \param[in]     lengthCheck      Length check mode.
 * \return        TRUE       Length check passed.
 *                FALSE      Length check failed.
 * \pre           elementIndex is a valid parameter
 *                resultIndexPtr need to be valid ptr
 *                resultLengthPtr need to be valid ptr
 * \context       TASK
 * \reentrant     TRUE
 * \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetLengthCheck(
  P2CONST(uint32, AUTOMATIC, AUTOMATIC) resultLengthptr,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGet_Standard()
 *********************************************************************************************************************/
/*!
 * \brief         Gets content of the non-virtual key element.
 * \details       Gets content of the non-virtual key element from the key storage.
 * \param[in,out] resultIndexPtr   Holds the identifier of the key element storage index.
 * \param[in,out] resultLengthPtr  Pointer to a variable which contains the size of the available buffer.
 *                                 After calling this function, the parameter contains the number of bytes written to the buffer.
 * \param[in]     elementIndex     Index of the Key element.
 * \param[in]     lengthCheck      Length check mode.
 * \return        E_OK                    Request successful.
 *                CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result. / Size mismatch for length check equal.
 * \pre           elementIndex is a valid parameter
 *                resultIndexPtr need to be valid ptr
 *                resultLengthPtr need to be valid ptr
 * \context       TASK
 * \reentrant     TRUE
 * \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGet_Standard(
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_LengthCheckType lengthCheck);

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetSheId()
 **********************************************************************************************************************/
/*! \brief         Get SHE Id for element.
 *  \details       -
 *  \param[in]     elementIndex            Holds the identifier of the key element
 *  \param[out]    sheIdPtr                Holds the pointer to the SHE id
 *  \pre           elementIndex must identify a valid key element for a SHE key
 *                 sheIdPtr has to be a valid ptr
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetSheId(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sheIdPtr);
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetSheIndex()
 **********************************************************************************************************************/
/*! \brief         Get index of SHE key in SheKeys.
 *  \details       -
 *  \param[in]     elementIndex            Holds the identifier of the key element
 *  \param[out]    indexSheKeyPtr          Holds the pointer to the sheKeys index
 *  \return        E_OK                    Request successful, SHE key found.
 *                 E_NOT_OK                Request failed.
 *  \pre           elementIndex must identify a valid key element for a SHE key
 *                 indexSheKeyPtr has to be a valid ptr
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetSheIndex(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, AUTOMATIC) indexSheKeyPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetKeyType()
 **********************************************************************************************************************/
/*! \brief         Get SHE key type for SHE Id.
 *  \details       -
 *  \param[in]     sheId                Holds the identifier to the SheKeyId.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           sheId must identify a valid key element
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(uint8, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetKeyType(
  uint8 sheId);

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetElementIndex()
 **********************************************************************************************************************/
/*! \brief         Check if the element with the given SheKeyId is present.
 *  \details       -
 *  \param[in]     sheKeyId                Holds the identifier to the SheKeyId.
 *  \param[out]    elementIndex            Pointer to the element index.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           sheId must identify a valid key element
 *                 All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetElementIndex(
  uint8 sheKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) elementIndex);
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetSheCheckFid()
 **********************************************************************************************************************/
/*! \brief         Check FID access rights for key usage.
 *  \details       -
 *  \param[in]     elementIndex            Holds the identifier of the key element
 *  \param[in]     serviceType             Holds the usage service type
 *  \return        TRUE                    Request successful / Usage allowed.
 *                 FALSE                   Request failed.
 *  \pre           elementIndex must identify a valid key element
 *                 serviceType must to be a valid service type
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetSheCheckFid(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_ServiceType serviceType);
# endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel()
 *********************************************************************************************************************/
/*! \brief         The function derives a key from given input data.
 *  \details       The function derives a key from given input data. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to input data from which the key is derived
 *  \param[in]     length                  Length of input data.
 *  \param[out]    outputPtr               Pointer to derived key
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes.
 *                 Length of the buffer provided by dataPtr has to be a multiple of 16.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateCmac()
 *********************************************************************************************************************/
/*! \brief         Generates Cmac based on given data and given key.
 *  \details       The function calculates the cmac based on the given data and the key. This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data from which the mac is calculated
 *  \param[in]     length                  Length of input data
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to derived key
 *  \param[out]    workspace               Pointer to workspace
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputPtr has to be 16 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the provided dataPtr has to be a multiple of 16 Bytes.
 *                 workspace has to be a valid pointer.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCmac(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) dataPtr,
  uint32 length,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) outputPtr,
  P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateVerifyAndExtract()
 *********************************************************************************************************************/
/*! \brief         Verify and extract key
 *  \details       Verify M3, extract key and generate M4 and M5.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the identifier of the key element which shall be set
 *  \param[in]     m1m2m3                  Holds the pointer to the key buffer which shall be used to update the key element with M1M2M3.
 *  \param[in]     indexOfSheKey           Index of the update key element in SheKeys
 *  \param[in]     indexOfAuthSheKey       Index of the authentication key element in SheKeys
 *  \param[in]     outputElement           Index of the destination key element
 *  \param[in]     proofAvailable          Holds the value if the proof element is available.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId and outputElement as well as elementIndex must identify a valid key - key element pair
 *                 keyPtr has to be a valid ptr with the length of CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3
 *                 indexOfSheKey and indexOfAuthSheKey need to be valid index for SheKeys.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyAndExtract(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  Crypto_30_LibCv_SizeOfKeyElementsType outputElement,
  Std_ReturnType proofAvailable);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdate()
 *********************************************************************************************************************/
/*! \brief         Updates key element based on SHE key update mechanism.
 *  \details       Interprets the given key buffer as M1M2M3 of the SHE key update mechanism and extracts relevant
 *                 information for setting the key element. This is used during the SHE key updated protocol.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the identifier of the key element which shall be set
 *  \param[in]     indexOfSheKey           Hold the index of the update key in SheKeys.
 *  \param[in]     m1m2m3                  Holds the pointer to the key buffer which shall be used to update the key element which holds M1M2M3.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *  \pre           cryptoKeyId, elementIndex as well as indexOfSheKey must identify a valid key - key element pair
 *                 keyPtr has to be a valid ptr with the length of CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementSetShe()
 *********************************************************************************************************************/
/*! \brief         Sets a she key element.
 *  \details       Sets the given key element using she behavior. She key updated for she key and plantext update for ram key.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndex            Holds the index of the key element which shall be set.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \param[in]     oldRetVal               Contains the old return value.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_WRITE_FAIL Request failed, write access was denied.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           cryptoKeyId and elementIndex have to be a valid identifiers to a key in the keyStorage.
 *                 The crypto key has to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSetShe(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength,
  Std_ReturnType oldRetVal);

# if (CRYPTO_30_LIBCV_RAM_KEY_EXPORT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetShe()
 *********************************************************************************************************************/
/*! \brief         Get a she key element.
 *  \details       Get the given key element using she behavior. Only the She ram key can be provided for key export.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be provided.
 *  \param[in]     elementIndex            Holds the index of the key element which shall be provided.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId and elementIndex have to be a valid identifiers to a key in the keyStorage.
 *                 The crypto key has to be locked for read access.
 *  \context       TASK
 *  \reentrant     TRUE, for different crypto keys
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetShe(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyExport_M1M2M3()
 *********************************************************************************************************************/
/*! \brief         Calculate M1M2M3
 *  \details       Calculate M1M2M3 for She ram key export.
 *  \param[out]    m1m2m3                  Pointer to key buffer with M1M2M3 (Sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3)
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER.
 *                                         The first 16 bytes contain to be the secret key.
 *  \param[in,out] encBufferPtr            Pointer to the working buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER.
 *  \param[in]     uid                     Pointer to the She uid. Size need to be CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID.
 *  \param[in]     ramKey                  Holds the pointer to the She ram key with sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_KEY.
 *  \param[in]     indexOfSheKey           Hold the index of the update key in SheKeys.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           indexOfSheKey have to be a valid identifiers to a key in the keyStorage.
 *                 All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyExport_M1M2M3(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) encBufferPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ramKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyExport()
 *********************************************************************************************************************/
/*! \brief         She key export calculate M1-M3 or M1-M5
 *  \details       Calculate M1-M3 or M1-M5 She ram key export.
 *                 (64 Bytes for M1-M3 and 112Bytes for M1-M5)
 *  \param[in]     indexOfSheKey           Hold the index of the update key in SheKeys.
 *  \param[in]     indexOfAuthSheKey       Hold the index of the authentication key in SheKeys.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           indexOfSheKey and indexOfAuthSheKey have to be a valid identifiers to a key in the keyStorage.
 *                 All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyExport(
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr);
# endif /* (CRYPTO_30_LIBCV_RAM_KEY_EXPORT == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateDecrypt()
 *********************************************************************************************************************/
/*! \brief         Decrypts data based on given key.
 *  \details       The function decrypts the given data using the provided key. AES128 is used as underlying algorithm.
 *                 This is used during the SHE key updated protocol.
 *  \param[in]     m2m3                    Pointer to data which shall be decrypted which hold M2M3.
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the decrypted data will be copied to
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be 32 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the buffer provided by outputPtr has to be 32 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateDecrypt(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m2m3,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateEncrypt()
 *********************************************************************************************************************/
/*! \brief         Encrypts data based on given key.
 *  \details       The function encrypts the given data using the provided key. AES128 is used as underlying algorithm.
 *                 This is used during the SHE key updated protocol.
 *  \param[in]     dataPtr                 Pointer to data which shall be encrypted
 *  \param[in]     keyPtr                  Pointer to key buffer
 *  \param[out]    outputPtr               Pointer to buffer where the encrypted data will be copied to
 *  \param[in]     numberOfBlocks          Specifies the number of blocks.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by dataPtr has to be numberOfBlocks * 16 Bytes
 *                 Length of the key provided by keyPtr has to be 16 Bytes
 *                 Length of the buffer provided by outputPtr has to be numberOfBlocks * 16 Bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateEncrypt(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr,
  uint8 numberOfBlocks);

/**********************************************************************************************************************
 * Crypto_30_LibCv_GetSheKey()
 **********************************************************************************************************************/
/*! \brief         Get SHE index in SheKeys for the given SHE Id and Page.
 *  \details       -
 *  \param[in]     sheKeyId                Holds the SHE key id.
 *  \param[in]     shePage                 Holds the SHE key page.
 *  \param[out]    indexOfSheKeyPtr        Pointer to the Index of the key element in SheKeys.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetSheKey(
  uint8 sheKeyId,
  Crypto_30_LibCv_SizeOfShePageType shePage,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) indexOfSheKeyPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateCheckM1Ids()
 *********************************************************************************************************************/
/*! \brief         Determines the authentication key element based on M1.
 *  \details       The function checks if the authentication key element should be the element itself
 *                 or the SHE master key. This is done by processing ID byte of M1.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3
 *  \param[in]     indexOfSheKey           Index of the key element in SheKeys
 *  \param[out]    indexOfAuthSheKeyPtr    Pointer to the Index of the authentication key element in SheKeys
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the key provided by keyPtr has to be 16 Bytes
 *                 indexOfSheKey has to be a valid SheKeysIndex.
 *                 All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckM1Ids(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) indexOfAuthSheKeyPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCheckUid()
 **********************************************************************************************************************/
/*! \brief         Check M1 UID is valid.
 *  \details       Check if the M1 UID is the wildcard or the ECU UID.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3.
 *  \param[in]     elementIndex            Index of the key element.
 *  \param[in]     sheKeyType              Type of the SHE key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the key provided by keyPtr has to be 15 Bytes
 *                 elementIndex has to be a valid index of a key element.
 *                 All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckUid(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateVerifyM3()
 *********************************************************************************************************************/
/*! \brief         Verifies M3.
 *  \details       The function verifies the M3.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3
 *  \param[in]     indexOfAuthSheKey       Index of the authenticcation key element in SheKeys.
 *  \param[in]     indexOfSheKey           Index of the update key element in SheKeys.
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *                 Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *                 indexOfSheKey and indexOfAuthSheKey need to be valid sheKeys Index.
 *                 page need to be a valid SHE key page.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr);

# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCheckCounter()
 **********************************************************************************************************************/
/*! \brief         Check if the used counter from M1 is valid.
 *  \details       -
 *  \param[in]     elementIndexCounter     Index of the counter key element.
 *  \param[in]     decryptedKeyPtr         Pointer to key buffer
 *  \param[in]     sheKeyType              Holds the type of the SHE key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId must identify a valid key.
 *                 Length of the buffer provided by decryptedKeyPtr has to be 32 Bytes.
 *                 All pointers need to be valid.
 *                 elementIndexCounter need to be a valid element index of a SHE counter.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckCounter(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexCounter,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType);
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON) */

# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCopyCounter()
 **********************************************************************************************************************/
/*! \brief         Save the new counter value.
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     elementIndexCounter     Index of the counter key element.
 *  \param[in]     decryptedKeyPtr         Pointer to key buffer
 *  \pre           Length of the buffer provided by decryptedKeyPtr has to be 32 Bytes.
 *                 All pointers need to be valid.
 *                 elementIndexCounter need to be a valid element index of a SHE counter.
 *                 cryptoKeyId must identify a valid key.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCopyCounter(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexCounter,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr);
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON) */

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCopyFid()
 **********************************************************************************************************************/
/*! \brief         Convert and save FID from M1.
 *  \details       -
 *  \param[in]     elementIndex            Index of the update key element.
 *  \param[in]     decryptedKeyPtr         Pointer to key buffer
 *  \param[in]     sheKeyType              Holds the type of the SHE key.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           Length of the buffer provided by decryptedKeyPtr has to be 32 Bytes.
 *                 All pointers need to be valid.
 *                 elementIndex need to be a valid element index of a SHE key.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCopyFid(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType);
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateExtractKey()
 *********************************************************************************************************************/
/*! \brief         Extracts new Key.
 *  \details       The function provides the new key.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3
 *  \param[in]     elementIndex            Index of the key element.
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer
 *  \param[in]     KDFbufferLength         Length of the KDF buffer
 *  \param[in,out] outputBufferPtr         pointer to the working buffer.
 *  \param[in]     indexOfSheKey           Index of the update key element in SheKeys.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId must identify a valid key.
 *                 Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *                 Length of the buffer provided by KDFbufferPtr has to be 32 Bytes.
 *                 indexOfSheKey need to be valid sheKeys Index.
 *                 sheKeyType need to be a valid SHE key type.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateExtractKey(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateProofM4M5()
 *********************************************************************************************************************/
/*! \brief         Calculates M4 and M5
 *  \details       The function calculate the proof elements M4 and M5.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3
 *  \param[in]     outputElement           Index of the destination key element
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER.
 *                                         The first 16 bytes contain to be the ram key.
 *  \param[in,out] outputBufferPtr         pointer to the working buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER.
 *                                         The first 16 bytes contain to be the counter.
 *  \param[in]     indexOfSheKey           Hold the index of the update key in SheKeys.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId must identify a valid key.
 *                 Length of the buffer provided by outputBufferPtr has to be 32 Bytes.
 *                 indexOfSheKey need to be a valid index of sheKeys.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateProofM4M5(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_SheKeyUpdateGenM4M5()
 *********************************************************************************************************************/
/*! \brief         Calculates M4 and M5
 *  \details       The function calculate the proof elements M4 and M5.
 *  \param[in]     m1m2m3                  Pointer to key buffer with M1M2M3 (Sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3)
 *  \param[out]    m4m5                    Pointer to key buffer for M4M5. (Sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5)
 *  \param[in]     uid                     Pointer to the She uid. Size need to be CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID.
 *  \param[in,out] KDFbufferPtr            Pointer to the key derive function buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER.
 *                                         The first 16 bytes contain to be the ram key.
 *  \param[in,out] encBufferPtr            Pointer to the working buffer. Size need to be CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER.
 *                                         The first 16 bytes contain to be the counter.
 *  \param[in]     indexOfSheKey           Hold the index of the update key in SheKeys.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           All pointer need to be valid.
 *                 indexOfSheKey need to be a valid index of sheKeys.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateGenM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m4m5,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) encBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey);

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateFindProof()
 **********************************************************************************************************************/
/*! \brief         Check if the Proof element is available.
 *  \details       Allowed is CRYPTO_KE_MAC_PROOF and CRYPTO_KE_CIPHER_PROOF.
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[out]    outputElementIndexPtr   Ptr to the identifier of the proof key element which shall be set.
 *  \return        E_OK                        Request successful.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request successful no proof element found.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed.
 *  \pre           cryptoKeyId must identify a valid key.
 *                 outputElementIndexPtr need to be a valid ptr.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateFindProof(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementsIterType, AUTOMATIC, AUTOMATIC) outputElementIndexPtr);

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateLockKeys()
 **********************************************************************************************************************/
/*! \brief         Lock the info and authentication key for reading.
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     authKeyId               Holds the identifier of the key which is used for authentication.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as authKeyId must identify a valid key.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateLockKeys(
  uint32 cryptoKeyId,
  uint32 authKeyId);

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateUnlockKeys()
 **********************************************************************************************************************/
/*! \brief         Release the lock for the info and authentication key.
 *  \details       -
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     authKeyId               Holds the identifier of the key which is used for authentication.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *  \pre           cryptoKeyId as well as authKeyId must identify a valid key.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateUnlockKeys(
  uint32 cryptoKeyId,
  uint32 authKeyId);

# if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_GetChallenge()
 *********************************************************************************************************************/
/*! \brief         Provide challenge for She DEBUG_CMD
 *  \details       Provide the challenge required for She DEBUG_CMD and save request.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the provided buffer is too small to store the result.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_GetChallenge(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_SetAuthorizationAndLock()
 *********************************************************************************************************************/
/*! \brief         Execute She DEBUG_CMD
 *  \details       Set Authorization and verify access. Delete deletion keys on success.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element.
 *  \param[in]     keyLength               Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_SetAuthorizationAndLock(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_SetAuthorization()
 *********************************************************************************************************************/
/*! \brief         Execute She DEBUG_CMD
 *  \details       Set Authorization and verify access. Delete deletion keys on success.
 *  \param[in]     keyPtr                  Holds the pointer to the key data which shall be set as key element
 *                                         with length of CRYPTO_30_LIBCV_SIZEOF_SHE_KEY.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, the key element size does not match size of provided
 *                                             data.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_SetAuthorization(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_DeleteKeys()
 *********************************************************************************************************************/
/*! \brief         Delete deletion keys.
 *  \details       Overwrite deletion keys with zero and restore inital values.
 *  \pre           All deletion keys need to be locked for write access.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_DeleteKeys(void);

#  if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_IsWriteProtected()
 *********************************************************************************************************************/
/*! \brief         Is a deletion key write protected?
 *  \details       -
 *  \return        TRUE            At least one key is write protected.
 *                 FALSE           No key is write protected.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_IsWriteProtected(void);
#  endif

/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_DebugCmd_Authorization()
 *********************************************************************************************************************/
/*! \brief         Verify She DEBUG_CMD Authorization
 *  \details       -
 *  \param[in]     masterKey        Holds the pointer to the She master key with sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_KEY.
 *  \param[in]     uid              Holds the pointer to the She uid with sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID.
 *  \param[in]     challenge        Holds the pointer to the She challenge with sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_KEY.
 *  \param[in]     authorization    Holds the pointer to the She authorization with sizeof CRYPTO_30_LIBCV_SIZEOF_SHE_KEY.
 *  \return        E_OK             Request successful.
 *                 E_NOT_OK         Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_Authorization(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) masterKey,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) challenge,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) authorization);
# endif /* (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON) */
#endif /* (CRYPTO_30_LIBCV_SHEKEYS == STD_ON) */

#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Trigger_Write_Req()
 **********************************************************************************************************************/
/*! \brief         Trigger write request.
 *  \details       Call NvM_SetRamBlockStatus and NvM_WriteBlock dependend on the block processing
 *  \param[in]     blockIdx             Holds the identifier of the nv block.
 *  \pre           blockIdx has to be a valid identifier.
 *  \context       TASK
 *  \reentrant     TRUE, for different blocks.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Trigger_Write_Req(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_State_CallbackWrittenToBlock()
 **********************************************************************************************************************/
/*!\brief       Write Block request finished Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to notify the Crypto driver that an
 *              asynchronous single block request has been finished.
 * \param[in]   blockIdx               The internal identifier of the NvM block.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_CallbackWrittenToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_State_WrittenToBlock()
 **********************************************************************************************************************/
/*!\brief       Update State for WrittenToBlock
 * \details     -
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_WrittenToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_WriteToBlock_Copy()
 **********************************************************************************************************************/
/*!\brief       Copy data to NvMBufferPtr
 * \details     Copy header and data.
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \param[out]  NvMBufferPtr    RAM mirror where Ram block data shall be written to.
 * \pre         blockIdx need to be valid.
 *              NvMBufferPtr need to be a valid pointer.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock_Copy(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_ReadFromBlock_Copy()
 **********************************************************************************************************************/
/*!\brief       Read from Block callback routine.
 * \details     Block specific callback routine which is called by NvM in order to let the Crypto driver copy data from
 *              NvM RAM mirror to Crypto RAM block.
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \param[in]   NvMBuffer       RAM mirror where Ram block data can be read from.
 * \return      E_OK      Data was either copied from buffer or restored.
 * \pre         blockIdx need to be valid.
 *              NvMBufferPtr need to be a valid pointer.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_Copy(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_ReadFromBlock_HeaderCrc()
 **********************************************************************************************************************/
/*!\brief       Check Header for CRC
 * \details     -
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \param[in]   NvMBuffer       RAM mirror where Ram block data can be read from.
 * \return      TRUE      Header valid.
 * \return      FALSE     Header not valid.
 * \pre         blockIdx need to be valid.
 *              NvMBufferPtr need to be a valid pointer.
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_HeaderCrc(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBufferPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_WriteToBlock_HeaderCrc()
 **********************************************************************************************************************/
/*!\brief       Copy Header for CRC
 * \details     -
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \param[out]  NvMBufferPtr    RAM mirror where Ram block data shall be written to.
 * \param[out]  writtenLengthPtr    Pointer where written Length shall be written to.
 * \pre         blockIdx need to be valid.
 *              All pointers need to be a valid.
 * \context     TASK
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock_HeaderCrc(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) writtenLengthPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_NvBlock_ReadFromBlock_Restore()
 **********************************************************************************************************************/
/*!\brief       Restore data for the given block.
 * \details     -
 * \param[in]   blockIdx        The internal identifier of the NvM block.
 * \pre         blockIdx need to be valid.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_Restore(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx);
#endif /* (CRYPTO_30_LIBCV_NVBLOCK == STD_ON) */

#if(CRYPTO_30_LIBCV_SHECMDGETID == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_LibCv_She_Cmd_Get_Status()
 *********************************************************************************************************************/
/*! \brief         Get the current she status
 *  \details       -
 *  \param[out]    statusPtr          Holds the pointer to the current she status.
 *  \return        E_OK               Request successful.
 *  \return        E_NOT_OK           Request failed.
 *  \pre           All pointers need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_Cmd_Get_Status(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) statusPtr);
#endif /* (CRYPTO_30_LIBCV_SHECMDGETID == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_BinarySearchCryptoKeyId()
 *********************************************************************************************************************/
/*! \brief         Binary search for the key index.
 *  \details       Binary search for the key Index of the given elementIndex in the global key list.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[out]    cryptoKeyId             Will hold the identifier of the key after it has been found.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key not found.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *                 cryptoKeyId pointer need to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_BinarySearchCryptoKeyId(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) cryptoKeyId);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGet()
 *********************************************************************************************************************/
/*! \brief         This interface shall be used to get a key element of the key identified by the cryptoKeyId and store
 *                 the key element in the memory location pointed to by the result pointer.
 *  \details       The key element can only be provided, if the read access right is RA_ALLOWED. If the key element
 *                 is configured to allow partial access, resultLengthPtr contains the amount of data which should
 *                 be read from the key element.
 *                 The behavior for partial access can be switched on or off in the configuration.
 *                 If the read access right is RA_ENCRYPTED the She ram key can be exported.
 *                 (64 Bytes for M1-M3 and 112Bytes for M1-M5)
 *  \param[in]     cryptoKeyId             Holds the identifier of the key whose key element shall be set.
 *  \param[in]     keyElementId            Holds the identifier of the key element which shall be set.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \param[out]    errorIdPtr              Holds the pointer to the error id.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_KEY_NOT_AVAILABLE  Request failed, the key is not available.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           cryptoKeyId needs to be a valid key id.
 *                 keyElementId needs to be a valid key element id.
 *                 resultPtr pointer needs to be valid.
 *                 resultLengthPtr pointer needs to be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) errorIdPtr);

/**********************************************************************************************************************
 *  Crypto_30_LibCv_Local_KeyElementGetNoShe()
 *********************************************************************************************************************/
/*! \brief         Stores the key element identified by the provided elementIndex in the in the memory location
 *                 pointed to by the result pointer.
 *  \details       If the key element has partial access enabled, also only parts of the key element can be stored.
 *                 This behavior depends on the available buffer and on the configuration.
 *  \param[in]     elementIndex            Holds the identifier of the keyElement in the keyStorage.
 *  \param[out]    resultPtr               Holds the pointer to the key data which shall be set as key element.
 *  \param[in,out] resultLengthPtr         Contains the length of the key element in bytes.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed, key not found.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_READ_FAIL  Request failed, read access was denied.
 *                 CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           elementIndex has to be a valid identifier to a keyElement in the keyStorage.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetNoShe(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Crypto_30_LibCv_SetKeyElementWrittenLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLength(
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength)
{
  /* #10 Set high- and low-byte for keyElement written length */
  Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementIndex), (uint8)((keyElementLength >> 8) & 0xFFu)); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageWrittenLengthStartIdxOfKeyElements(keyElementIndex) + 1u, (uint8)(keyElementLength & 0xFFu)); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyIdSearch()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyIdSearch(
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint32 cryptoKeyId = 0u;

  /* #10 Search the Crypto Key Id if the functionality is enabled and trigger that the key value is changed */
  retVal = Crypto_30_LibCv_Local_BinarySearchCryptoKeyId(keyElementIndex, &cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  if (retVal == E_OK) 
  {
    Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(cryptoKeyId, keyElementIndex, keyElementLength);
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType keyElementIndex,
  uint32 keyElementLength)
{

  /* #10 Set high- and low-byte for keyElement written length */
  Crypto_30_LibCv_SetKeyElementWrittenLength(keyElementIndex, keyElementLength);

#if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
  /* #20 Call the callout if the functionality is enabled and trigger that the key value is changed */
  Crypto_30_LibCv_GetKeyValueChangedCalloutFctNameOfConfigurableCallouts()(cryptoKeyId, Crypto_30_LibCv_GetIdOfKeyElements(keyElementIndex));
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_SetKeyElementStateWritten()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_SetKeyElementStateWritten(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_NORMAL_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  if (Crypto_30_LibCv_IsKeyElementWriteOnce(elementIndex))
  {
    Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SetKeyState()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SetKeyState(
  uint32 cryptoKeyId,
  uint8 mask)
{
  Crypto_30_LibCv_KeyElementsIterType elementIndex;
  boolean changed = FALSE;

  /* #10 Set key state for all key elements */

  /* Loop over all elements of the source key */
  for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
  {
#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    if (Crypto_30_LibCv_IsKeyElementPersist(elementIndex) && !Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, mask))
    {
      changed = TRUE;
    }
#endif
    Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, mask); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }

  /* #20 Call the callout if the functionality is enabled and trigger that the key is set to valid or not */
#if(CRYPTO_30_LIBCV_KEYVALIDITYSETCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
  if (mask == CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK)
  {
    Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(cryptoKeyId, TRUE);
  }
  else
  {
    Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(cryptoKeyId, FALSE);
  }
#endif

  return changed;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst)
{
  Std_ReturnType retVal = E_OK;
  /* #10 Check if the access flags are matching the conditions to copy a key element */
  /* Read != DENIED and Destination Protection >= Source Protection */
  if (((Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndexSrc))) == CRYPTO_30_LIBCV_RA_DENIED) ||
      ((Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndexSrc))) > (Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndexDst)))))
  {
    retVal = CRYPTO_E_KEY_READ_FAIL;
  }
  /* WRITE <= Internal Copy */
  else if ((Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndexDst))) == CRYPTO_30_LIBCV_WA_DENIED)
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
  }
  /* #20 Check if it is a write once key which is written already. */
  else if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIndexDst, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
  }
  else
  {
    /* #30 Check She key rights */
#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
    if (Crypto_30_LibCv_IsSheKey(elementIndexSrc))
    {
      retVal = CRYPTO_E_KEY_READ_FAIL;
    }
    else if (Crypto_30_LibCv_IsSheKey(elementIndexDst))
    {
      retVal = CRYPTO_E_KEY_WRITE_FAIL;
    }
    else
    {
      /* Success */
    }
#endif
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementCopy()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 3 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopy(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst,
  uint32 dstCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndexSrc;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndexDst;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Check if the access flags are matching the conditions to copy a key element */
  retVal = Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck(elementIndexSrc, elementIndexDst);
  if (retVal == E_OK)
  {
    retVal = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    /* Are both key elements non-virtual */
    /* #30 Handle not virtual key elements */
    {
      /* Check if the available data is not bigger than the maximum size of the destination element */
      /* #35 Copy key element data if the size is matching */
      if ((Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) == Crypto_30_LibCv_GetKeyElementLength(elementIndexDst)) ||
          ((Crypto_30_LibCv_IsKeyElementPartial(elementIndexDst) == TRUE) &&
          (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc) <= Crypto_30_LibCv_GetKeyElementLength(elementIndexDst))))
      {
        keyStorageIndexSrc = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc);
        keyStorageIndexDst = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);
        /* Copy the source element to the destination element */
        Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndexDst), Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndexSrc), Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

        /* store the written length in the destination element */
        Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(dstCryptoKeyId, elementIndexDst, Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc));
        Crypto_30_LibCv_Local_SetKeyElementStateWritten(elementIndexDst);

        retVal = E_OK;
      }
      else
      {
        /* The destination element is to short to store the available data of the source element */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementCopyPartial()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6030, 6060, 6080 6 */ /* MD_MSR_STCYC, MD_CRYPTO_30_LIBCV_STPAR_ASR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementCopyPartial(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst,
  uint32 dstCryptoKeyId,
  uint32 keyElementSourceOffset,
  uint32 keyElementTargetOffset,
  uint32 keyElementCopyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndexSrc, keyStorageIndexDst;
  uint32 writtenLengthSrc, writtenLengthDst;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Check if the access flags are matching the conditions to copy a key element */
  retVal = Crypto_30_LibCv_Local_KeyElementCopy_RightsCheck(elementIndexSrc, elementIndexDst);
  if (retVal == E_OK)
  {
    retVal = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    {
      writtenLengthSrc = Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexSrc);
      writtenLengthDst = Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndexDst);

      if (writtenLengthSrc == CRYPTO_30_LIBCV_EMPTY_KEY_LENGTH)
      {
        retVal = CRYPTO_E_KEY_EMPTY;
      }
      else if (
        (Crypto_30_LibCv_IsKeyElementPartial(elementIndexDst) == TRUE) &&
        /* [SWS_Crypto_00207] readLength */
        ((keyElementSourceOffset + keyElementCopyLength) <= writtenLengthSrc) &&
        ((Crypto_30_LibCv_IsUint32Overflow(keyElementSourceOffset, keyElementCopyLength)) == FALSE) &&
        /* [SWS_Crypto_00208] writeLength */
        ((keyElementTargetOffset + keyElementCopyLength) <= Crypto_30_LibCv_GetKeyElementLength(elementIndexDst)) &&
        ((Crypto_30_LibCv_IsUint32Overflow(keyElementTargetOffset, keyElementCopyLength)) == FALSE))
      {
        keyStorageIndexSrc = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexSrc);
        keyStorageIndexDst = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndexDst);

        /* If there is a gap betwen offset and written length, clear it. */
        if (writtenLengthDst < keyElementTargetOffset)
        {
          Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndexDst + writtenLengthDst), (uint32)(keyElementTargetOffset - writtenLengthDst)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
        }

        /* Copy the source element to the destination element */
        Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndexDst + keyElementTargetOffset), Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndexSrc + keyElementSourceOffset), keyElementCopyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

        /* store the written length in the destination element */
        /* [SWS_Crypto_00210], [SWS_Crypto_00211] */
        if (writtenLengthDst < (keyElementTargetOffset + keyElementCopyLength))
        {
          Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(dstCryptoKeyId, elementIndexDst, (keyElementTargetOffset + keyElementCopyLength));
        }
        Crypto_30_LibCv_Local_SetKeyElementStateWritten(elementIndexDst);

        retVal = E_OK;
      }
      else
      {
        /* The destination element is too short to store the available data of the source element */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetLengthCheck()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetLengthCheck(
  P2CONST(uint32, AUTOMATIC, AUTOMATIC) resultLengthptr,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_LengthCheckType lengthCheck)
{
  boolean retVal = FALSE;

  switch (lengthCheck)
  {
    case CRYPTO_30_LIBCV_LENGTH_CHECK_NONE:
    retVal = TRUE;
    break;

    case CRYPTO_30_LIBCV_LENGTH_CHECK_MAX:
    if (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) <= *resultLengthptr)
    {
      retVal = TRUE;
    }
    break;

    case CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL:
    if (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) == *resultLengthptr)
    {
      retVal = TRUE;
    }
    break;

    case CRYPTO_30_LIBCV_LENGTH_CHECK_MIN:
    if (Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex) >= *resultLengthptr)
    {
      retVal = TRUE;
    }
    break;

    default:  /* COV_CRYPTO_30_LIBCV_MISRA_ADDITIONAL_STATEMENT */
    break;
  }

  return retVal;
}

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetSheId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetSheId(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) sheIdPtr)
{
  Crypto_30_LibCv_SheKeysIterType i;

  /* Get key SHE id of SHE key list. */
  for (i = 0u; i < Crypto_30_LibCv_GetSizeOfSheKeys(); i++)
  {
    if (Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(i) == elementIndex)
    {
      *sheIdPtr = Crypto_30_LibCv_GetSheIdOfSheKeys(i);  /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      break;
    }
  }
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetSheIndex()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetSheIndex(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, AUTOMATIC) indexSheKeyPtr)
{
  Crypto_30_LibCv_SheKeysIterType i;
  Std_ReturnType retVal = E_NOT_OK;

  /* Get index of SHE key in list. */
  for (i = 0u; i < Crypto_30_LibCv_GetSizeOfSheKeys(); i++)
  {
    if (Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(i) == elementIndex)
    {
      *indexSheKeyPtr = (Crypto_30_LibCv_SizeOfSheKeysType)i; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      retVal = E_OK;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetKeyType()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(uint8, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetKeyType(
  uint8 sheId)
{
  uint8 keyType;
  /* Get key type for SHE key. */
  if (sheId == CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID)
  {
    keyType = CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY;
  }
  else if (sheId >= CRYPTO_30_LIBCV_SHE_M1_KEY_N_START_ID)
  {
    keyType = CRYPTO_30_LIBCV_SHE_TYPE_KEY_N;
  }
  else
  {
    keyType = sheId;
  }
  return keyType;
}

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyGetElementIndex()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyGetElementIndex(
  uint8 sheKeyId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) elementIndex)
{
  Std_ReturnType found = E_NOT_OK;

  Crypto_30_LibCv_SheKeysIterType i;

  /* Check if the element with the given SheKeyId is present. */
  for (i = 0u; i < Crypto_30_LibCv_GetSizeOfSheKeys(); i++)
  {
    if (Crypto_30_LibCv_GetSheIdOfSheKeys(i) == sheKeyId)
    {
      *elementIndex = Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(i); /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
      found = E_OK;
      break;
    }
  }

  return found;
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetSheCheckFid()
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
 *
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetSheCheckFid(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_ServiceType serviceType)
{
  boolean retVal = FALSE;
  uint8 fid, keyType, sheId = 0u;
  Crypto_30_LibCv_SizeOfKeyElementsType localElementIndex = 0u;

  /* Get SHE id for element */
  Crypto_30_LibCv_SheKeyGetSheId(elementIndex, &sheId); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  keyType = Crypto_30_LibCv_SheKeyGetKeyType(sheId);

  /* Ram key usage is always allowed. */
  if (keyType != CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY)
  {

    if (keyType == CRYPTO_30_LIBCV_SHE_TYPE_SECRET_KEY)
    {
      /* If it is the secret key load FID from master key. */
      if (Crypto_30_LibCv_SheKeyGetElementIndex(CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID, &localElementIndex) == E_OK) /* COV_CRYPTO_30_LIBCV_SHE_ECU_MASTER_KEY */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      {
        fid = Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageExtensionIdxOfKeyElements(localElementIndex));
      }
      else
      {
        /* Use default values if there is no master key. */
        fid = CRYPTO_30_LIBCV_SHE_FID_MASK_DEFAULT;
      }
    }
    else
    {
      /* load FID */
      fid = Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageExtensionIdxOfKeyElements(elementIndex));
    }

    fid &= Crypto_30_LibCv_SheKeyCheckFlags[keyType]; /* clear flags which not need to be checked according to key type */
    fid |= Crypto_30_LibCv_SheKeySetAccessFlags[keyType]; /* set additional flags for access protection according to key type */
    fid &= Crypto_30_LibCv_SheKeyServiceFlags[serviceType]; /* clear flags which not need to be checked according to service type */

    /* Check Boot Protection */
#  if (CRYPTO_30_LIBCV_KEYELEMENTSBOOTPROTECTIONIDXOFSHEPAGE == STD_ON)
    localElementIndex = Crypto_30_LibCv_GetKeyElementsBootProtectionIdxOfShePage(CRYPTO_30_LIBCV_SHE_PAGE0);
    if ((Crypto_30_LibCv_IsModuleBootProtected(Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(localElementIndex)))) &&
        (Crypto_30_LibCv_GetKeyElementWrittenLength(localElementIndex) == 1u))
    {
      /* If Boot is passed clear bit to allow access. */
      fid &= CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION_INV;
    }
#  endif

    /* Check Debugger Protection */
#  if (CRYPTO_30_LIBCV_KEYELEMENTSDEBUGGERPROTECTIONIDXOFSHEPAGE == STD_ON)
    localElementIndex = Crypto_30_LibCv_GetKeyElementsDebuggerProtectionIdxOfShePage(CRYPTO_30_LIBCV_SHE_PAGE0);
    if ((Crypto_30_LibCv_IsModuleDebuggerProtected(Crypto_30_LibCv_GetKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(localElementIndex)))) &&
        (Crypto_30_LibCv_GetKeyElementWrittenLength(localElementIndex) == 1u))
    {
      /* If no Debugger is attached clear bit to allow access. */
      fid &= CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION_INV;
    }
#  endif

    /* Check Result */
    if (fid == Crypto_30_LibCv_SheKeyServiceFlagsResult[serviceType])
    {
      retVal = TRUE;
    }
  }
  else
  {
    retVal = TRUE;
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */
#endif /* (CRYPTO_30_LIBCV_SHEKEYS == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGet_Standard()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGet_Standard(
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_LengthCheckType lengthCheck)
{
  Std_ReturnType retVal;

  /* #20 Handle non-virtual key element. */
  /* Check if the buffer for the result is large enough.
  Depending on the actual length of the element the data is stored or an error is generated if the buffer is to small */

  /* #25 Set storage index and length if size is adequate */
  if (Crypto_30_LibCv_Local_KeyElementGetLengthCheck(resultLengthPtr, elementIndex, lengthCheck) == TRUE) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  {
    *resultIndexPtr = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    /* [SWS_Crypto_00092] */
    *resultLengthPtr = Crypto_30_LibCv_GetKeyElementWrittenLength(elementIndex); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
    retVal = E_OK;
  }
  else
  {
    /* [SWS_Crypto_00093] */
    retVal = CRYPTO_E_SMALL_BUFFER;
  }

  return retVal;
}

#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel()
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
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  uint32 length,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  /*
    AES-MP(x i): OUT_i  = ENC_(ECB, OUT_i-1) (x i)  XOR x_i  XOR OUT_i-1;  i > 0;   OUT_0  = 0;
    */
  eslt_WorkSpaceAES128Block workSpaceAes128Block;
  Std_ReturnType retVal = E_NOT_OK;
  uint32 currentOffset = 0u;
  uint8_least i;
  uint8 Out[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 Out_last[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 tempCalcByte;
  eslt_ErrorCode eslRetVal = ESL_ERC_ERROR;

  /* #10 initialize all parameters */
  /* Init Out_i-1 (Out_last) */
  Crypto_30_LibCv_ClearData(Out, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  Crypto_30_LibCv_ClearData(Out_last, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

  /* #15 initialize workspace */
  if (esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128Block.header, /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    ESL_MAXSIZEOF_WS_AES128,
    CRYPTO_30_LIBCV_WATCHDOG_PTR) == ESL_ERC_NO_ERROR)
  {
    /* #20 Calculate Block-Hash until data size is reached */
    for(currentOffset = 0u; currentOffset < length; currentOffset += CRYPTO_30_LIBCV_SIZEOF_SHE_KEY) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
    {
      /* Set Key */
      /* #25 Set old Hash as round Key */
      eslRetVal = esl_initEncryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128Block,
                                             (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Out_last); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS */

      /* Encrypt */
      /* #30 Calculate new Hash */
      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        eslRetVal = esl_encryptAES128Block((P2VAR(eslt_WorkSpaceAES128Block, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128Block,
                                           &dataPtr[currentOffset],
                                           (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))Out); /* SBSW_CRYPTO_30_LIBCV_WORKSPACE_STACK_WITH_STACK_BUFFERS_AND_PTR_AS_PARAMETER */
      }

      if (eslRetVal == ESL_ERC_NO_ERROR)
      {
        /* #35 Exor new and old hash */
        for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
        {
          /* Calculate Out and Out_last */
          tempCalcByte = (uint8)(Out_last[i] ^ Out[i] ^ dataPtr[currentOffset + i]);
          Out_last[i] = tempCalcByte; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          Out[i] = tempCalcByte; /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
        }
      }
      else
      {
        break;
      }
    }
  }

  /* #40 Save Result if compression was successful */
  if (eslRetVal == ESL_ERC_NO_ERROR)
  {
    retVal = E_OK;

    Crypto_30_LibCv_CopyData(outputPtr, Out, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCmac()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC( Std_ReturnType, CRYPTO_30_LIBCV_CODE ) Crypto_30_LibCv_SheKeyUpdateCmac(
  P2CONST( uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR ) dataPtr,
  uint32 length,
  P2CONST( uint8, AUTOMATIC, AUTOMATIC ) keyPtr,
  P2VAR( uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR ) outputPtr,
  P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 Init work space */
  if (esl_initWorkSpaceHeader(&workspace->header, /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
    ESL_MAXSIZEOF_WS_CMACAES,
    CRYPTO_30_LIBCV_WATCHDOG_PTR ) == ESL_ERC_NO_ERROR)
  {
    /* #15 Init CMAC */
    if (esl_initCMACAES128( workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
      CRYPTO_30_LIBCV_CMACAES_MAX_KEY_SIZE,
      (P2CONST( eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR ))keyPtr ) == ESL_ERC_NO_ERROR)
    {

      /* #25 Update Calculation */
      if (esl_updateCMACAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_PTR */
        (eslt_Length)length,
        (P2CONST( eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR ))dataPtr ) == ESL_ERC_NO_ERROR)
      {
        /* Finalize */
        if (esl_finalizeCMACAES128(workspace, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_OUTPUT_PTR */
          outputPtr ) == ESL_ERC_NO_ERROR)
        {
          retVal = E_OK;
        }
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateDecrypt()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateDecrypt(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m2m3,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_WorkSpaceAES128 workSpaceAes128;
  eslt_ErrorCode eslRet;
  eslt_Length outLength = 2u * CRYPTO_30_LIBCV_AES_BLOCK_SIZE;
  eslt_Length written;

  /* #10 Init work space */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128.header,
                                   ESL_MAXSIZEOF_WS_AES128,
                                   CRYPTO_30_LIBCV_WATCHDOG_PTR); /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */

  /* #15 Init AES decryption */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initDecryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))keyPtr,
                                   ESL_BM_CBC,
                                   ESL_PM_OFF,
                                   NULL_PTR);
  }

  /* #20 Decrypt data */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_decryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128,
                               (eslt_Length)2u * CRYPTO_30_LIBCV_AES_BLOCK_SIZE,
                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))m2m3,
                               (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outLength,
                               (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))outputPtr); /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
  }

  written = outLength;
  outLength = (eslt_Length)(((eslt_Length)(2u * CRYPTO_30_LIBCV_AES_BLOCK_SIZE)) - written);

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_finalizeDecryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                       (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outLength,
                                       (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputPtr[written]);
  }

  written = (eslt_Length)(written + outLength);

  if ((eslRet == ESL_ERC_NO_ERROR) &&
      (written == (2u * CRYPTO_30_LIBCV_AES_BLOCK_SIZE)))
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateEncrypt()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateEncrypt(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) dataPtr,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) keyPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputPtr,
  uint8 numberOfBlocks)
{
  Std_ReturnType retVal = E_NOT_OK;
  eslt_ErrorCode eslRet;
  eslt_WorkSpaceAES128 workSpaceAes128;
  eslt_Length outLength = (eslt_Length)((eslt_Length)(numberOfBlocks) * CRYPTO_30_LIBCV_AES_BLOCK_SIZE);
  eslt_Length written;

  /* #10 Init work space */
  eslRet = esl_initWorkSpaceHeader((P2VAR(eslt_WorkSpaceHeader, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&(workSpaceAes128.header), /* SBSW_CRYPTO_30_LIBCV_INIT_ESL_STACK_WORKSPACE */
                                   ESL_MAXSIZEOF_WS_AES128,
                                   CRYPTO_30_LIBCV_WATCHDOG_PTR);

  /* #15 Init AES encryption */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_initEncryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_KEY_PTR */
                                   (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))keyPtr,
                                   ESL_BM_CBC,
                                   ESL_PM_OFF,
                                   NULL_PTR);
  }

  /* #20 encrypt data */
  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_encryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128, /* SBSW_CRYPTO_30_LIBCV_ESL_STACK_WORKSPACE_WITH_DATA_OUTPUT */
                               (eslt_Length)((eslt_Length)(numberOfBlocks) * CRYPTO_30_LIBCV_AES_BLOCK_SIZE),
                               (P2CONST(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))dataPtr,
                               (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))&outLength,
                               (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))outputPtr);
  }

  written = outLength;
  outLength = (eslt_Length)(((eslt_Length)numberOfBlocks * CRYPTO_30_LIBCV_AES_BLOCK_SIZE) - written);

  if (eslRet == ESL_ERC_NO_ERROR)
  {
    eslRet = esl_finalizeEncryptAES128((P2VAR(eslt_WorkSpaceAES128, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceAes128, /* SBSW_CRYPTO_30_LIBCV_ESL_CALL_WITH_WORKSPACE_AND_BUFFERS */
                                       (P2VAR(eslt_Length, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outLength,
                                       (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&outputPtr[written]);
  }

  written = (eslt_Length)(written + outLength);

  if ((eslRet == ESL_ERC_NO_ERROR) &&
      (written == ((eslt_Length)numberOfBlocks * CRYPTO_30_LIBCV_AES_BLOCK_SIZE)))
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_GetSheKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_GetSheKey(
  uint8 sheKeyId,
  Crypto_30_LibCv_SizeOfShePageType shePage,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) indexOfSheKeyPtr)
{
  Std_ReturnType found = E_NOT_OK;
  Crypto_30_LibCv_SheKeysIterType i;

  /* Get SHE index in SheKeys for the given SHE Id and Page. */
  for (i = 0u; i < Crypto_30_LibCv_GetSizeOfSheKeys(); i++)
  {
    if ((Crypto_30_LibCv_GetSheIdOfSheKeys(i) == sheKeyId) &&
        (Crypto_30_LibCv_GetShePageIdxOfSheKeys(i) == shePage))
    {
      *indexOfSheKeyPtr = (Crypto_30_LibCv_SizeOfSheKeysType)i; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      found = E_OK;
      break;
    }
  }

  return found;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCheckM1Ids()
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
 *
 *
 */
/* PRQA S 6030, 6080 4 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckM1Ids(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2VAR(Crypto_30_LibCv_SizeOfSheKeysType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) indexOfAuthSheKeyPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 m1AuthId, m1SheId;
  Crypto_30_LibCv_SizeOfShePageType authPage;
  boolean loadAuthKey = FALSE;

  m1AuthId = Crypto_30_LibCv_She_M1_GetAuthId(m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS]);
  m1SheId = Crypto_30_LibCv_She_M1_GetId(m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS]);

  /* Secret key */
  if (m1SheId == CRYPTO_30_LIBCV_SHE_M1_SECRET_KEY_ID)
  {
    /* not possible */
  }
  /* check if keyId matches configured key elementId */
  else if (m1SheId == Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey))
  {
    /* set default page to 0 */
    authPage = CRYPTO_30_LIBCV_SHE_PAGE0;

    /* master key, mac key and key_<n> */
    if ((m1SheId != CRYPTO_30_LIBCV_SHE_M1_MAC_ID) &&
             (m1SheId <= CRYPTO_30_LIBCV_SHE_M1_KEY_N_END_ID))
    {
      /* Master key authentication */
      if (m1AuthId == CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID) /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION */
      {
        loadAuthKey = TRUE;
      }
      /* Self update */
      else if (m1AuthId == m1SheId)
      {
        /* Use index from update key to refer to all required information e.g. SHE page */
        *indexOfAuthSheKeyPtr = indexOfSheKey; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
        retVal = E_OK;
      }
      else
      {
        /* not valid */
      }
    }
    /* mac */
    else if (m1SheId == CRYPTO_30_LIBCV_SHE_M1_MAC_ID)
    {
      /* Master key authentication */
      if (m1AuthId == CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID) /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION */
      {
        loadAuthKey = TRUE;
      }
      /* Mac key authentication */
      else if (m1AuthId == CRYPTO_30_LIBCV_SHE_M1_MAC_KEY_ID) /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION */
      {
        loadAuthKey = TRUE;
      }
      else
      {
        /* not valid */
      }
    }
    /* ram key */
    else
    {
      /* Secret key authentication */
      if (m1AuthId == CRYPTO_30_LIBCV_SHE_M1_SECRET_KEY_ID)
      {
        loadAuthKey = TRUE;
      }
      /* KEY_N authentication on the same page */
      else if ((m1AuthId >= CRYPTO_30_LIBCV_SHE_M1_KEY_N_START_ID) &&
               (m1AuthId <= CRYPTO_30_LIBCV_SHE_M1_KEY_N_END_ID))
      {
        loadAuthKey = TRUE;
        authPage = Crypto_30_LibCv_GetShePageIdxOfSheKeys(indexOfSheKey);
      }
      else
      {
        /* retVal is already set */
      }
    }

    if (loadAuthKey == TRUE)
    {
      retVal = Crypto_30_LibCv_GetSheKey(m1AuthId, authPage, indexOfAuthSheKeyPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
  }
  else
  {
    /* not possible */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCheckUid()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckUid(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType)
{
  uint8 i;
  Std_ReturnType retVal = E_NOT_OK;
  boolean verified;
  uint32 uidLength = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID;
  Crypto_30_LibCv_SizeOfKeyStorageType uIdStorageIndex;

  verified = TRUE;
  /* Check if the value is wildcard */
  for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID; i++)
  {
    if (m1m2m3[i] != CRYPTO_30_LIBCV_SHE_UID_WILDCARD_VALUE)
    {
      verified = FALSE;
      break;
    }
  }

  /* check UID if the wildcard is not matching. */
  if (verified == FALSE)
  {
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_UID, &uIdStorageIndex, &uidLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MIN) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    {
      verified = TRUE;
      for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID; i++)
      {
        if (m1m2m3[i] != Crypto_30_LibCv_GetKeyStorage(uIdStorageIndex + i))
        {
          verified = FALSE;
          break;
        }
      }
    }
  }
  else
  {
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
    /* If the wildcard is valid, check if it is valid to use the wildcard. */
    if ((Crypto_30_LibCv_IsKeyElementExtensionByMask(elementIndex, CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD)) &&
        (sheKeyType != CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY))
    {
      verified = FALSE;
    }
# else
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(sheKeyType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(elementIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif
  }

  if (verified == TRUE)
  {
    retVal = E_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateVerifyM3()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060, 6080 8 */ /* MD_CRYPTO_30_LIBCV_STPAR, MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyM3(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY]; /* K2 */
  uint32 authKeyLength = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
  Crypto_30_LibCv_SizeOfKeyStorageType keyStorageIndex;
  eslt_WorkSpaceCMACAES workSpaceCmacAes;

  if (Crypto_30_LibCv_IsKeyElementValid(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfAuthSheKey)))
  {
    if (Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic( Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfAuthSheKey), &keyStorageIndex, &authKeyLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE) == E_OK)  /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    {
      /* Set constant for SHE page handling */
      /* Set Key_Update_Mac_C constant */
      Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrMacConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

      /* #10 Generate K2 */
      /* KDF AuthKey|Key_Update_Mac_C */
      Crypto_30_LibCv_CopyData(KDFbufferPtr, Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

      if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
      {

        /* #15 Calculate M3 / CMAC and verify */
        /* M3 proof:  M3* = outputBufferPtr */
        if (Crypto_30_LibCv_SheKeyUpdateCmac(m1m2m3, CRYPTO_30_LIBCV_STARTINDEX_SHE_M3, key, outputBufferPtr, (P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR))&workSpaceCmacAes) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
        {
          retVal = E_OK;
          /* Check (M3 = M3*) */
          for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_M3; i++)
          {
            if (m1m2m3[i + CRYPTO_30_LIBCV_STARTINDEX_SHE_M3] != outputBufferPtr[i])
            {
              retVal = E_NOT_OK;
            }
          }
        }
      }
    }
  }

  return retVal;
}

# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCheckCounter()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCheckCounter(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexCounter,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType)
{
  uint32 oldCounter, newCounter;
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyStorageType keyStorage;
  uint32 counterLength = CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES;

  if (sheKeyType == CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY)
  {
    /* no counter check for ram key */
    retVal = E_OK;
  }
  else
  {
    /* There is a validation that there need to be a counter */
    (void)Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic( elementIndexCounter, &keyStorage, &counterLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE);/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&oldCounter, Crypto_30_LibCv_GetAddrKeyStorage(keyStorage));/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&newCounter, decryptedKeyPtr);/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    newCounter = Crypto_30_LibCv_She_ConvertCounter(newCounter);

    /* check if new counter is bigger */
    if ((newCounter) > (oldCounter))
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON) */

# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCopyCounter()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCopyCounter(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexCounter,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr)
{
  uint32 newCounter;
  uint8 counterValue[CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES];

  /*  Save the new counter value. */
  Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&newCounter, decryptedKeyPtr); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  newCounter = Crypto_30_LibCv_She_ConvertCounter(newCounter);
  Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(counterValue, newCounter);/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId(cryptoKeyId, elementIndexCounter, counterValue, CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES);/* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON) */

# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateCopyFid()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateCopyFid(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) decryptedKeyPtr,
  Crypto_30_LibCv_SheKeyTypeType sheKeyType)
{
  uint8 fid;

  /* Convert FID */
  fid = (uint8)(((uint8)(decryptedKeyPtr[3] << 4)) | ((uint8)(decryptedKeyPtr[4] >> 4) & 0x0Cu));

  /* Add flag for SHE key identification. */
  fid |= CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_KEY_MASK;
  Crypto_30_LibCv_SetKeyStorage(Crypto_30_LibCv_GetKeyStorageExtensionIdxOfKeyElements(elementIndex), fid); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

  /* Add write once flag in key element state. */
  if ((Crypto_30_LibCv_Uint8CheckMask(fid, CRYPTO_30_LIBCV_SHE_FID_MASK_WRITE_PROTECTION)) &&
      (sheKeyType != CRYPTO_30_LIBCV_SHE_TYPE_RAM_KEY))
  {
    Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }
  else
  {
    Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_INV_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
  }
}
# endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateExtractKey()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateExtractKey(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  uint8 KDFbufferLength,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Generate K1 */
  /* Set Key_Update_Enc_C constant */
  Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrEncConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

  if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, KDFbufferLength, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    Crypto_30_LibCv_ClearData(outputBufferPtr, CRYPTO_30_LIBCV_SIZEOF_SHE_M2); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    /* #15 Decrypt C,K,F */
    if (Crypto_30_LibCv_SheKeyUpdateDecrypt(&m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M2], key, outputBufferPtr) == E_OK) /* SBSW_CRYPTO_30_LIBCV_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK */
    {
      /* check counter */
# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
      Crypto_30_LibCv_SheKeyTypeType sheKeyType;
      sheKeyType = Crypto_30_LibCv_SheKeyGetKeyType(Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey));
      if (Crypto_30_LibCv_SheKeyUpdateCheckCounter(Crypto_30_LibCv_GetKeyElementsCounterIdxOfSheKeys(indexOfSheKey), outputBufferPtr, sheKeyType) == E_OK) /* SBSW_CRYPTO_30_LIBCV_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK */
# endif
      {
        /* #20 Store new Key and key length. */
        for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
        {
          Crypto_30_LibCv_SetKeyStorage(keyStorageIndex + i, outputBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + i]); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

          /*  KDF Key|Key_Update_Enc_C */
          KDFbufferPtr[i] = outputBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + i]; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
        }

        Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(cryptoKeyId, elementIndex, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY);
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateGenM4M5()
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
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateGenM4M5(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m4m5,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) encBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey)
{
  Std_ReturnType retVal = E_NOT_OK;
  /* Buffer */
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY]; /* Key3 and Key4 */
  eslt_WorkSpaceCMACAES workSpaceCmacAes;

  /* Set Key_Update_Enc_C constant */
  Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrEncConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

  /* # Generate K3 */
  if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {
    /* # Generate M4* */
    /* counter(28bits) | "1" | "0" ... "0"x99 (=128 bit) */
    encBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES - 1u] |= 0x08u; /* Add leading one for padding ENC(ECB,K3) */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    encBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES - 1u] &= 0xF8u; /* Add leading zero for padding ENC(ECB,K3) */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    /* Add padding */
    Crypto_30_LibCv_ClearData(&(encBufferPtr[4]), CRYPTO_30_LIBCV_SIZEOF_SHE_M4_ENC - CRYPTO_30_LIBCV_SIZEOF_SHE_M4_COUNTER_FULL_BYTES); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    /* # Copy UID of M4 */
    Crypto_30_LibCv_CopyData(m4m5, uid, CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

    /* # Copy Ids of M4 */
    Crypto_30_LibCv_CopyData(&m4m5[CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS], &m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS], CRYPTO_30_LIBCV_SIZEOF_SHE_M1_IDS); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

    /* # Generate and store second block of M4 */
    if (Crypto_30_LibCv_SheKeyUpdateEncrypt(encBufferPtr, key, &m4m5[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], 1u) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
    {
      /* Set Key_Update_Mac_C constant */
      Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrMacConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

      /* # Generate K4 */
      if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
      {
        /* Generate M5 */
        retVal = Crypto_30_LibCv_SheKeyUpdateCmac(m4m5, 32u, key, &m4m5[32], &workSpaceCmacAes); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateProofM4M5()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 8 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateProofM4M5(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfKeyElementsType outputElement,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) outputBufferPtr,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint32 uidLength = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID;
  Crypto_30_LibCv_SizeOfKeyStorageType uIdStorageIndex;
  /* Buffer */
  uint8 m4m5[CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5];

  /* # Copy UID of M4 */
  if (Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_UID, &uIdStorageIndex, &uidLength, CRYPTO_30_LIBCV_LENGTH_CHECK_MIN) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {

    /* # Build M4 and M5 */
    retVal = Crypto_30_LibCv_SheKeyUpdateGenM4M5(m1m2m3, m4m5, Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), KDFbufferPtr, outputBufferPtr, indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
    if (retVal == E_OK)
    {
      /* # Copy M4 & M5 */
      Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(outputElement), m4m5, CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

      /* # Store written length */
      Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(cryptoKeyId, outputElement, CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateFindProof()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateFindProof(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementsIterType, AUTOMATIC, AUTOMATIC) outputElementIndexPtr)
{
  Std_ReturnType retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  Crypto_30_LibCv_KeyElementsIterType outputElement;

  /* Determine proof output slot */
  for (outputElement = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
       outputElement < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId);
       outputElement++)
  {
    if ((Crypto_30_LibCv_GetIdOfKeyElements(outputElement) == CRYPTO_KE_MAC_PROOF)||
        (Crypto_30_LibCv_GetIdOfKeyElements(outputElement) == CRYPTO_KE_CIPHER_PROOF))
    {
      if ((Crypto_30_LibCv_GetKeyStorageEndIdxOfKeyElements(outputElement) - Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(outputElement)) >= (uint16)CRYPTO_30_LIBCV_SIZEOF_SHE_M4_M5)
      {
        *outputElementIndexPtr = outputElement;  /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        retVal = E_OK;
      }
      else
      {
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
      }
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateLockKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateLockKeys(
  uint32 cryptoKeyId,
  uint32 authKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint32 sheInfoKeyId;

  sheInfoKeyId = Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate();
  /* check if SHE info need to be locked */
  if (Crypto_30_LibCv_Local_KeyReadLockGet(sheInfoKeyId) == E_OK)
  {
    /* check if SHE info need to be locked */
    if (authKeyId != cryptoKeyId)
    {
      if (Crypto_30_LibCv_Local_KeyReadLockGet(authKeyId) == E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        Crypto_30_LibCv_Local_KeyReadLockRelease(sheInfoKeyId);
      }
    }
    else
    {
      retVal = E_OK;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateUnlockKeys()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateUnlockKeys(
  uint32 cryptoKeyId,
  uint32 authKeyId)
{
  uint32 sheInfoKeyId;

  sheInfoKeyId = Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate();

  Crypto_30_LibCv_Local_KeyReadLockRelease(sheInfoKeyId);

  if (authKeyId != cryptoKeyId)
  {
    Crypto_30_LibCv_Local_KeyReadLockRelease(authKeyId);
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdateVerifyAndExtract()
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
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdateVerifyAndExtract(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  Crypto_30_LibCv_SizeOfKeyElementsType outputElement,
  Std_ReturnType proofAvailable)
{
  /* # Init KDF Parameters */
  /* KDF Buffer */
  uint8 KDFbuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER];

  /* # Init Workspace */
  /* Working Buffers */
  uint8 encBuffer[CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER];

  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SheKeyTypeType sheKeyType = Crypto_30_LibCv_SheKeyGetKeyType(Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey));

  /* # Verify M3 */
  if (Crypto_30_LibCv_SheKeyUpdateVerifyM3(m1m2m3, indexOfAuthSheKey, indexOfSheKey, KDFbuffer, CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER, encBuffer) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
  {
    /* # check UID */
    if (Crypto_30_LibCv_SheKeyUpdateCheckUid(m1m2m3, elementIndex, sheKeyType) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
    {
      /* # Extract Key */
      retVal = Crypto_30_LibCv_SheKeyUpdateExtractKey(cryptoKeyId, m1m2m3, elementIndex, KDFbuffer, CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER, encBuffer, indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */

      if (retVal == E_OK)
      {
        /* store FID and Counter */
# if(CRYPTO_30_LIBCV_SHE_ENABLE_COUNTER == STD_ON)
        /* RAM key has no counter */
        if (Crypto_30_LibCv_IsKeyElementsCounterUsedOfSheKeys(indexOfSheKey))
        {
          Crypto_30_LibCv_SheKeyUpdateCopyCounter(cryptoKeyId, Crypto_30_LibCv_GetKeyElementsCounterIdxOfSheKeys(indexOfSheKey), encBuffer); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        }
# endif
# if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
        Crypto_30_LibCv_SheKeyUpdateCopyFid(elementIndex, encBuffer, sheKeyType); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
# else
        Crypto_30_LibCv_ClearKeyElementExtensionByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_CLEAR_PLAIN_KEY_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
# endif

        /* # Generate M4 & M5, if proof slot is available */
        if (proofAvailable == E_OK)
        {
          retVal = Crypto_30_LibCv_SheKeyUpdateProofM4M5(cryptoKeyId, m1m2m3, (Crypto_30_LibCv_SizeOfKeyElementsType)outputElement, KDFbuffer, encBuffer, indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyUpdate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyUpdate(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3)
{
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType proofAvailable;
  Crypto_30_LibCv_KeyElementsIterType outputElement = 0u;
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey = 0u;

  /* # Determine proof output slot */
  proofAvailable = Crypto_30_LibCv_SheKeyUpdateFindProof(cryptoKeyId, &outputElement); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if ((proofAvailable == E_OK) ||
    (proofAvailable == CRYPTO_E_KEY_NOT_AVAILABLE))
  {
    /*# Check key ID */
    if (Crypto_30_LibCv_SheKeyUpdateCheckM1Ids(m1m2m3, indexOfSheKey, &indexOfAuthSheKey) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      if (Crypto_30_LibCv_SheKeyUpdateLockKeys(cryptoKeyId, Crypto_30_LibCv_GetKeyIdxOfSheKeys(indexOfAuthSheKey)) == E_OK)
      {
        /* # Verify M3, Extract Key and Generate M4 & M5, if proof slot is available  */
        retVal = Crypto_30_LibCv_SheKeyUpdateVerifyAndExtract(cryptoKeyId, elementIndex, m1m2m3, indexOfSheKey, indexOfAuthSheKey, (Crypto_30_LibCv_SizeOfKeyElementsType)outputElement, proofAvailable); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */

        Crypto_30_LibCv_SheKeyUpdateUnlockKeys(cryptoKeyId, Crypto_30_LibCv_GetKeyIdxOfSheKeys(indexOfAuthSheKey));
      }
      else
      {
        retVal = CRYPTO_E_BUSY;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSetShe()
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
 */
/* PRQA S 6080 6 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSetShe(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength,
  Std_ReturnType oldRetVal)
{
  Std_ReturnType retVal = oldRetVal;
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey;

  /* Check if it is a write once key which is written already. */
  if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
  }
    /* Key update using SHE key update protocol */
  else if ((keyLength == CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3) &&
    (Crypto_30_LibCv_GetKeyElementLength(elementIndex) == CRYPTO_30_LIBCV_SIZEOF_SHE_KEY))
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
    if (Crypto_30_LibCv_SheKeyGetSheIndex(elementIndex, &indexOfSheKey) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      if ((CRYPTO_30_LIBCV_WA_ENCRYPTED == Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))) ||
        ((CRYPTO_30_LIBCV_WA_ALLOWED == Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))) &&
          (Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey) == CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID)))
      {
        /* She Key Protocol */
        retVal = Crypto_30_LibCv_SheKeyUpdate(cryptoKeyId, elementIndex, indexOfSheKey, keyPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
      }
    }
  }
  /* Set plaintext ram key */
  else if (keyLength == CRYPTO_30_LIBCV_SIZEOF_SHE_KEY)
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
    if (CRYPTO_30_LIBCV_WA_ALLOWED == Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex)))
    {
      /* Is the given key a she key and is it a ram key. */
      if (Crypto_30_LibCv_SheKeyGetSheIndex(elementIndex, &indexOfSheKey) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        if (Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey) == CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID)
        {
          /* Set ram key as plaintext. */
          retVal = Crypto_30_LibCv_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, keyPtr, keyLength, CRYPTO_30_LIBCV_WA_ENCRYPTED); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */

          if (retVal == E_OK)
          {
            Crypto_30_LibCv_SetKeyElementExtensionByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_PLAIN_KEY_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
          }
        }
      }
    }
  }
  else
  {
    /* nothing to do */
  }
  return retVal;
}

# if (CRYPTO_30_LIBCV_RAM_KEY_EXPORT == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetShe()
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
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetShe(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey, indexOfAuthSheKey = 0u;
  /* Is it a she key */
  if (Crypto_30_LibCv_SheKeyGetSheIndex(elementIndex, &indexOfSheKey) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    /* Is it the ram key */
    if (Crypto_30_LibCv_GetSheIdOfSheKeys(indexOfSheKey) == CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID)
    {
      if (*resultLengthPtr >= CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3)
      {
        /* Get Secret Key, find key can be voided because secret key must be available for ram key */
        (void)Crypto_30_LibCv_GetSheKey(CRYPTO_30_LIBCV_SHE_M1_SECRET_KEY_ID, CRYPTO_30_LIBCV_SHE_PAGE0, &indexOfAuthSheKey); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        /* Lock Keys */
        if (Crypto_30_LibCv_SheKeyUpdateLockKeys(cryptoKeyId, (uint32)Crypto_30_LibCv_GetKeyIdxOfSheKeys(indexOfAuthSheKey)) == E_OK)
        {
          /* export key */
          retVal = Crypto_30_LibCv_SheKeyExport(indexOfSheKey, indexOfAuthSheKey, resultPtr, resultLengthPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          /* Unlock Keys */
          Crypto_30_LibCv_SheKeyUpdateUnlockKeys(cryptoKeyId, Crypto_30_LibCv_GetKeyIdxOfSheKeys(indexOfAuthSheKey));
        }
        else
        {
          retVal = CRYPTO_E_BUSY;
        }
      }
      else
      {
        retVal = CRYPTO_E_SMALL_BUFFER;
      }
    }
    else
    {
      retVal = CRYPTO_E_KEY_READ_FAIL;
    }
  }
  else
  {
    retVal = CRYPTO_E_KEY_READ_FAIL;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyExport_M1M2M3()
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
 */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyExport_M1M2M3(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) m1m2m3,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) KDFbufferPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) encBufferPtr,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) ramKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  eslt_WorkSpaceCMACAES workSpaceCmacAes;

  /* M1 */
  Crypto_30_LibCv_CopyData(m1m2m3, uid, CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M1_IDS] = Crypto_30_LibCv_She_M1_BuildIds(CRYPTO_30_LIBCV_SHE_M1_RAM_KEY_ID, CRYPTO_30_LIBCV_SHE_M1_SECRET_KEY_ID);  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

  /* M2 */
  /* Generate K1 */
  /* Set AuthKey|Key_Update_Enc_C constant */
  Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrEncConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

  if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
  {

    /* Encrypt C,K,F */
    Crypto_30_LibCv_ClearData(encBufferPtr, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
    Crypto_30_LibCv_CopyData(&encBufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], ramKey, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */

    if (Crypto_30_LibCv_SheKeyUpdateEncrypt(encBufferPtr, key, &m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M2], 2u) == E_OK) /* SBSW_CRYPTO_30_LIBCV_KEY_OUTPUT_BUFFER_PARAMETER_KEY_STACK */
    {
      /* M3 */
      /* Generate K2 */
      /* KDF AuthKey|Key_Update_Mac_C */
      Crypto_30_LibCv_CopyData(&KDFbufferPtr[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], Crypto_30_LibCv_She_GetPtrMacConst(indexOfSheKey), CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
      if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(KDFbufferPtr, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + CRYPTO_30_LIBCV_SIZEOF_SHE_UPDATE_CONSTANT, key) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_AND_KEY */
      {

        /* Calculate M3 CMAC */
        if (Crypto_30_LibCv_SheKeyUpdateCmac(m1m2m3, CRYPTO_30_LIBCV_STARTINDEX_SHE_M3, key, &m1m2m3[CRYPTO_30_LIBCV_STARTINDEX_SHE_M3], &workSpaceCmacAes) == E_OK) /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR_KEY_OUTPUT */
        {
          retVal = E_OK;
        }
      }
    }

  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_SheKeyExport()
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
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_SheKeyExport(
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey,
  Crypto_30_LibCv_SizeOfSheKeysType indexOfAuthSheKey,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  Std_ReturnType retVal = E_NOT_OK, localRet;
  uint8 m1m5[CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M5];
  uint8 kdfBuffer[CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER];
  uint8 encBuffer[CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER];
  uint32 writtenLength = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M3;

  Crypto_30_LibCv_SizeOfKeyStorageType uIdStorageIndex, keyStorageIndex, authKeyStorageIndex;
  uint32 length;

  /* Get UID */
  length = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID;
  localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_UID, &uIdStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_MIN); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* Get SecretKey */
  if (localRet == E_OK)
  {
    length = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
    localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic( Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfAuthSheKey), &authKeyStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE);  /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    if (!Crypto_30_LibCv_IsKeyElementValid(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfAuthSheKey)))
    {
      localRet = E_NOT_OK;
    }

    /* Get RamKey */
    if (localRet == E_OK)
    {
      length = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
      localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic( Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfSheKey), &keyStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE);  /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

      /* Check if Ram key is written as plaintext */
      if (localRet == E_OK)
      {
        if (!Crypto_30_LibCv_IsKeyElementExtensionByMask(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfSheKey), CRYPTO_30_LIBCV_KEYELEMENTSEXTENSION_SHE_PLAIN_KEY_MASK))
        {
          retVal = CRYPTO_E_KEY_READ_FAIL;
          localRet = E_NOT_OK;
        }

        if (localRet == E_OK)
        {
          /* Calculate m1-m3 */
          Crypto_30_LibCv_CopyData(kdfBuffer, Crypto_30_LibCv_GetAddrKeyStorage(authKeyStorageIndex), CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
          retVal = Crypto_30_LibCv_SheKeyExport_M1M2M3(m1m5, kdfBuffer, encBuffer, Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */

          /* Calculate m4-m5 if required */
          if ((retVal == E_OK) &&
              (*resultLengthPtr >= CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M5))
          {
            Crypto_30_LibCv_CopyData(kdfBuffer, Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
            retVal = Crypto_30_LibCv_SheKeyUpdateGenM4M5(m1m5, &m1m5[CRYPTO_30_LIBCV_STARTINDEX_SHE_M4], Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), kdfBuffer, encBuffer, indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KDFBUFFER_KEY_OUTPUTBUFFER */
            writtenLength = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_M5;
          }

          /* Copy key Export to the given Buffer. */
          if (retVal == E_OK)
          {
            Crypto_30_LibCv_CopyData(resultPtr, m1m5, writtenLength);  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
            *resultLengthPtr = writtenLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
          }
        }
      }
    }
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_RAM_KEY_EXPORT == STD_ON) */

# if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_Authorization()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_Authorization(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) masterKey,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) uid,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) challenge,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) authorization)
{
  Std_ReturnType retVal = E_NOT_OK;
  const uint8 debugKeyC[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY] = { 0x01U, 0x03U, 0x53U, 0x48U, 0x45U, 0x00U, 0x80U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0xB0U };

  uint8 key[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 result[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY];
  uint8 buffer[CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER];
  uint8_least i;
  eslt_WorkSpaceCMACAES workSpaceCmacAes;

  /* K = KDF(KEYMASTER_ECU_KEY, DEBUG_KEY_C) */
  Crypto_30_LibCv_CopyData(buffer, masterKey, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], debugKeyC, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
  if (Crypto_30_LibCv_SheKeyUpdateMiyaguchiPreneel(buffer, CRYPTO_30_LIBCV_SIZEOF_KDF_BUFFER, key) == E_OK)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  {
    /* AUTHORIZATION = CMAC(CHALLENGE | UID) with K */
    Crypto_30_LibCv_CopyData(buffer, challenge, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_KEY], uid, CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
    if (Crypto_30_LibCv_SheKeyUpdateCmac(buffer, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY + CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID, key, result, &workSpaceCmacAes) == E_OK)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    {
      /* Check (authorization = authorization*) */
      retVal = E_OK;
      for (i = 0u; i < CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; i++)
      {
        if (authorization[i] != result[i])
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }

  return retVal;
}

#  if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_IsWriteProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_IsWriteProtected(void)
{
  boolean writeProtection = FALSE;
  Crypto_30_LibCv_KeyIterType keyIdx;
  Crypto_30_LibCv_KeyElementsIterType elementIdx;

  /* Check if a key is write protected. */
  for (keyIdx = 0u; keyIdx < Crypto_30_LibCv_GetSizeOfKey(); keyIdx++)
  {
    if (Crypto_30_LibCv_IsDebugDeletionOfKey(keyIdx))
    {
      /* Iterate over all elements in the key */
      for (elementIdx = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(keyIdx); elementIdx < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(keyIdx); elementIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
      {
        if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIdx, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
        {
          writeProtection = TRUE;
          break;
        }
      }
    }
  }

  return writeProtection;
}
#  endif /* (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_DeleteKeys()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_DeleteKeys(void)
{
  Crypto_30_LibCv_KeyIterType keyIdx;
  Crypto_30_LibCv_SheKeysIterType sheKeyIndex;
  Crypto_30_LibCv_KeyElementsIterType elementIdx;

  /* Set keys to initial state. */
  for (keyIdx = 0u; keyIdx < Crypto_30_LibCv_GetSizeOfKey(); keyIdx++)
  {
    if (Crypto_30_LibCv_IsDebugDeletionOfKey(keyIdx))
    {
      for (elementIdx = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(keyIdx); elementIdx < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(keyIdx); elementIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
      {
        /* Delete old value */
        Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorageOfKeyElements(elementIdx), Crypto_30_LibCv_GetKeyElementLength(elementIdx)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */

        /* init key again */
        Crypto_30_LibCv_Init_Key(elementIdx, FALSE);

#  if(CRYPTO_30_LIBCV_KEYVALUECHANGEDCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
        /* Call the callout if the functionality is enabled and trigger that the key value is changed. */
        Crypto_30_LibCv_GetKeyValueChangedCalloutFctNameOfConfigurableCallouts()(keyIdx, Crypto_30_LibCv_GetIdOfKeyElements(elementIdx));
#  endif
      }
    }
  }

  for (sheKeyIndex = 0u; sheKeyIndex < Crypto_30_LibCv_GetSizeOfSheKeys(); sheKeyIndex++)
  {
    elementIdx = Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(sheKeyIndex);
    Crypto_30_LibCv_Init_KeySheAdditional(elementIdx);
  }

#  if(CRYPTO_30_LIBCV_KEYVALIDITYSETCALLOUTFCTNAMEOFCONFIGURABLECALLOUTS == STD_ON)
  /* Call the callout if the functionality is enabled and trigger that the key is set to valid */
  {
    boolean isValid;

    for (keyIdx = 0u; keyIdx < Crypto_30_LibCv_GetSizeOfKey(); keyIdx++)
    {
      isValid = TRUE;

      if (Crypto_30_LibCv_IsDebugDeletionOfKey(keyIdx))
      {
        for (elementIdx = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(keyIdx); elementIdx < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(keyIdx); elementIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
        {
          if (!Crypto_30_LibCv_IsKeyElementValid(elementIdx))
          {
            isValid = FALSE;
            break;
          }
        }

        if (isValid == TRUE)
        {
          Crypto_30_LibCv_GetKeyValiditySetCalloutFctNameOfConfigurableCallouts()(keyIdx, TRUE);
        }
      }
    }
  }
#  endif
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_GetChallenge()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_GetChallenge(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  eslt_ErrorCode localRet;
  /* Check length */
  if (*resultLengthPtr >= CRYPTO_30_LIBCV_SIZEOF_SHE_KEY)
  {
    /* Check write protection */
#  if (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON)
    if (!Crypto_30_LibCv_She_DebugCmd_IsWriteProtected())
#  endif
    {
      /* Provide RNG as challenge */
      localRet = esl_getBytesRNG( CRYPTO_30_LIBCV_SIZEOF_SHE_KEY, (P2VAR(eslt_Byte, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))Crypto_30_LibCv_She_Debug_Cmd_Challenge ); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
      if (localRet == E_OK)
      {
        Crypto_30_LibCv_CopyData(resultPtr, Crypto_30_LibCv_She_Debug_Cmd_Challenge, CRYPTO_30_LIBCV_SIZEOF_SHE_KEY); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
        *resultLengthPtr = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag = TRUE;
        retVal = E_OK;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else
  {
    retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_SetAuthorization()
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
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_SetAuthorization(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr)
{
  Std_ReturnType retVal = E_NOT_OK, localRet;
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheKey = 0u;
  Crypto_30_LibCv_SizeOfKeyStorageType uIdStorageIndex, keyStorageIndex = 0u;
  uint32 length;

  Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag = FALSE;
  /* Get Master Key She Idx */
  localRet = Crypto_30_LibCv_GetSheKey(CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID, CRYPTO_30_LIBCV_SHE_PAGE0, &indexOfSheKey); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  /* Get UID */
  length = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID;
  localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_UID, &uIdStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_MIN); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* Get Master Key */
  if (localRet == E_OK)
  {
    length = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
    localRet = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfSheKey), &keyStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE);  /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (localRet == E_OK)
    {
      /* Check Authorization */
      retVal = Crypto_30_LibCv_She_DebugCmd_Authorization(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), Crypto_30_LibCv_She_Debug_Cmd_Challenge, keyPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
      if (retVal == E_OK)
      {
        /* Delete keys */
        Crypto_30_LibCv_She_DebugCmd_DeleteKeys();
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_She_DebugCmd_SetAuthorizationAndLock()
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
 */
/* PRQA S 6010, 6030, 6080 3 */ /* MD_MSR_STPTH , MD_MSR_STCYC , MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_DebugCmd_SetAuthorizationAndLock(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  Crypto_30_LibCv_KeyIterType lockedKeyIdx, releaseKeyIdx;
#  if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
  Crypto_30_LibCv_KeyElementsIterType elementIdx;
  boolean persist;
#  endif
  Std_ReturnType retVal = E_NOT_OK;

  /* Is it a valid request */
  if ((keyLength == CRYPTO_30_LIBCV_SIZEOF_SHE_KEY) &&
      Crypto_30_LibCv_She_Debug_Cmd_ChallengeFlag)
  {
    /* Lock keys for write access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (lockedKeyIdx = 0u; lockedKeyIdx < Crypto_30_LibCv_GetSizeOfKey(); lockedKeyIdx++)
    {
      if (Crypto_30_LibCv_IsDebugDeletionOfKey(lockedKeyIdx))
      {
        if (Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected((uint32)lockedKeyIdx) == E_NOT_OK)
        {
          retVal = CRYPTO_E_BUSY;
        }
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (retVal != CRYPTO_E_BUSY)
    {
      retVal = Crypto_30_LibCv_She_DebugCmd_SetAuthorization(keyPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_OF_KEYPTR */
    }

    /* Release keys for write access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (releaseKeyIdx = 0; releaseKeyIdx < lockedKeyIdx; releaseKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_UPPER_BOUND_STATIC_GENDATA */
    {
      if (Crypto_30_LibCv_IsDebugDeletionOfKey(releaseKeyIdx))
      {
        Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected((uint32)releaseKeyIdx);
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

#  if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
    if (retVal == E_OK)
    {
    /* Trigger write block. */
      for (releaseKeyIdx = 0; releaseKeyIdx < lockedKeyIdx; releaseKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_UPPER_BOUND_STATIC_GENDATA */
      {
        persist = FALSE;

        if (Crypto_30_LibCv_IsDebugDeletionOfKey(releaseKeyIdx))
        {
          for (elementIdx = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(releaseKeyIdx); elementIdx < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(releaseKeyIdx); elementIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
          {
            if (Crypto_30_LibCv_IsKeyElementPersist(elementIdx))
            {
              persist = TRUE;
            }
          }
          if (persist == TRUE)
          {
            Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(releaseKeyIdx));
          }
        }
      }
    }
#  endif
  }
  else
  {
    retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }

  return retVal;
}
# endif /* (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON) */
#endif /* CRYPTO_30_LIBCV_SHEKEYS == STD_ON */

#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Trigger_Write_Req()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Trigger_Write_Req(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK, localRet = E_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* ----- Implementation ------------------------------------------------- */

  /* Set Ram Block Status */
# if (CRYPTO_30_LIBCV_NV_MENABLE_SET_RAM_BLOCK_STATUS == STD_ON)
  localRet = NvM_SetRamBlockStatus((NvM_BlockIdType)Crypto_30_LibCv_GetDescriptorOfNvBlock(blockIdx), TRUE);
  if (localRet == E_OK)
# endif
  {
    /* Trigger write request if it is a IMMEDIATE block.  */
    if ((Crypto_30_LibCv_GetProcessingOfNvBlock(blockIdx) == CRYPTO_30_LIBCV_NV_PROCESSING_IMMEDIATE))
    {
      localRet = Crypto_30_LibCv_NvM_WriteBlock((NvM_BlockIdType)Crypto_30_LibCv_GetDescriptorOfNvBlock(blockIdx), NULL_PTR); /* SBSW_CRYPTO_30_LIBCV_CALL_WITH_NULL_PTR */
    }
  }

  /* If an error occurred clear write requested bit mask in state. */
  if (localRet == E_OK)
  {
    retVal = E_OK;
  }
  else
  {
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    Crypto_30_LibCv_ClearNvBlockStateMask(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_CLEAR_WRITE_REQUESTED); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_MainFunction()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_MainFunction(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  Crypto_30_LibCv_NvBlockIterType blockIdx;

  /* ----- Implementation ------------------------------------------------- */

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  /* Check if write request need to be triggered for any block. */
  for (blockIdx = 0u; blockIdx < Crypto_30_LibCv_GetSizeOfNvBlock(); blockIdx++)
  {
    if (Crypto_30_LibCv_GetNvBlockState(blockIdx) == CRYPTO_30_LIBCV_NVBLOCK_STATE_WRITE_REQ_PENDING)
    {
      Crypto_30_LibCv_SetNvBlockState(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_WRITE_REQ); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
      SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

      (void)Crypto_30_LibCv_NvBlock_Trigger_Write_Req((Crypto_30_LibCv_SizeOfNvBlockType)blockIdx);

      SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    }
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Write_Req()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Write_Req(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */

  boolean setBlockStatus = FALSE;
  /* ----- Implementation ------------------------------------------------- */

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  /* If idle trigger new request */
  if (Crypto_30_LibCv_IsNvBlockState(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_IDLE))
  {
    Crypto_30_LibCv_SetNvBlockState(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_WRITE_REQ); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
    setBlockStatus = TRUE;
  }
  /* Otherwise set only flag for changed data .*/
  else
  {
    Crypto_30_LibCv_SetNvBlockStateMask(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  if (setBlockStatus)
  {
    (void)Crypto_30_LibCv_NvBlock_Trigger_Write_Req(blockIdx);
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_State_CallbackWrittenToBlock()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_CallbackWrittenToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  /* Update state  */
  Crypto_30_LibCv_ClearNvBlockStateMask(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_CLEAR_WRITE_COPIED); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_State_WrittenToBlock()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_WrittenToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  /* Update state if data is marked as changed. */
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  if (Crypto_30_LibCv_IsNvBlockStateMask(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_MASK_DATA_CHANGE))
  {
    Crypto_30_LibCv_SetNvBlockState(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_COPY_DATA); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
  }
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_WriteToBlock_Copy()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock_Copy(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Crypto_30_LibCv_SizeOfKeyStorageType length, writtenLength = 0u;

  /* ----- Development Error Checks ------------------------------------- */

  /* ----- Implementation ------------------------------------------------- */

  /* Update State */
  Crypto_30_LibCv_NvBlock_State_WrittenToBlock(blockIdx);

  /* Set header */
  if (Crypto_30_LibCv_GetConsistencyLevelOfNvBlock(blockIdx) == CRYPTO_30_LIBCV_NV_CONSISTENCY_LEVEL_DETECT)
  {
    Crypto_30_LibCv_NvBlock_WriteToBlock_HeaderCrc(blockIdx, NvMBufferPtr, &writtenLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
  }
  else
  {
    /* CRYPTO_30_LIBCV_NV_STORAGE_KIND_NONE */
  }

  /* Data */
  length = (Crypto_30_LibCv_SizeOfKeyStorageType)(Crypto_30_LibCv_GetKeyStorageEndIdxOfNvBlock(blockIdx) - Crypto_30_LibCv_GetKeyStorageStartIdxOfNvBlock(blockIdx));
  /* Copy Key Storage to Buffer */
  Crypto_30_LibCv_CopyData(&NvMBufferPtr[writtenLength], Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfNvBlock(blockIdx)), length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
  writtenLength = (Crypto_30_LibCv_SizeOfKeyStorageType)(writtenLength + length);
  /* Clear remaning Bytes of Buffer to have a specified value */
  Crypto_30_LibCv_ClearData(&NvMBufferPtr[writtenLength], (Crypto_30_LibCv_SizeOfKeyStorageType)(Crypto_30_LibCv_GetLengthOfNvBlock(blockIdx) - (writtenLength))); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_ReadFromBlock_Copy()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_Copy(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBufferPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean validHeader = TRUE;
  Crypto_30_LibCv_SizeOfKeyStorageType length, dataStartPos = 0u;

  /* ----- Development Error Checks ------------------------------------- */

  /* ----- Implementation ------------------------------------------------- */

  /* Set header */
  Crypto_30_LibCv_NvBlock_State_Init(blockIdx);

  /* Check header */
  if ((Crypto_30_LibCv_GetConsistencyLevelOfNvBlock(blockIdx) == CRYPTO_30_LIBCV_NV_CONSISTENCY_LEVEL_DETECT))
  {
    validHeader = Crypto_30_LibCv_NvBlock_ReadFromBlock_HeaderCrc(blockIdx, NvMBufferPtr);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
    dataStartPos = (Crypto_30_LibCv_SizeOfKeyStorageType)CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_HEADER_CRC;
  }

  /* Copy Buffer to Key Storage */
  if (validHeader)
  {
    length = (Crypto_30_LibCv_SizeOfKeyStorageType)(Crypto_30_LibCv_GetKeyStorageEndIdxOfNvBlock(blockIdx) - Crypto_30_LibCv_GetKeyStorageStartIdxOfNvBlock(blockIdx));
    Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(Crypto_30_LibCv_GetKeyStorageStartIdxOfNvBlock(blockIdx)), &NvMBufferPtr[dataStartPos], length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_KEY_STORAGE_ACCESS_NV */
  }
  /* Restore data in Key Storage */
  else
  {
    Crypto_30_LibCv_NvBlock_ReadFromBlock_Restore(blockIdx);
  }

  return E_OK;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_ReadFromBlock_HeaderCrc()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(boolean, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_HeaderCrc(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBufferPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean validHeader = TRUE;
  uint32 crc;

  /* ----- Implementation ------------------------------------------------- */
  /* Header */
  /* Version */
  if (NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_VERSION] != CRYPTO_30_LIBCV_NVBLOCK_VERSION_0)
  {
    validHeader = FALSE;
  }
  /* Flags */
  if (NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_FLAGS] != CRYPTO_30_LIBCV_NVBLOCK_FLAGS_0)
  {
    validHeader = FALSE;
  }
  /* Check Structure Crc */
  Crypto_30_LibCv_Local_Uint8ArrayToUint32BigEndian(&crc, &NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_CRC]);  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
  if (crc != Crypto_30_LibCv_GetCrcOfNvBlock(blockIdx))
  {
    validHeader = FALSE;
  }

  return validHeader;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_WriteToBlock_HeaderCrc()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock_HeaderCrc(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) writtenLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  /* Header */
  /* Version */
  NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_VERSION] = CRYPTO_30_LIBCV_NVBLOCK_VERSION_0;  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */

  /* Flags */
  NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_FLAGS] = CRYPTO_30_LIBCV_NVBLOCK_FLAGS_0;  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */

  /* Copy Structure Crc */
  Crypto_30_LibCv_Local_Uint32ToUint8ArrayBigEndian(&NvMBufferPtr[CRYPTO_30_LIBCV_POS_NVBLOCK_CRC], Crypto_30_LibCv_GetCrcOfNvBlock(blockIdx));  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
  *writtenLengthPtr = (Crypto_30_LibCv_SizeOfKeyStorageType)(*writtenLengthPtr + CRYPTO_30_LIBCV_SIZEOF_NVBLOCK_HEADER_CRC); /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_ReadFromBlock_Restore()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock_Restore(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  /* Restore new init values for the given block. */
  (void)Crypto_30_LibCv_NvBlock_Init(blockIdx);
}
#endif /* (CRYPTO_30_LIBCV_NVBLOCK == STD_ON) */

#if(CRYPTO_30_LIBCV_SHECMDGETID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_She_Cmd_Get_Status()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_She_Cmd_Get_Status(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) statusPtr)
{
  Std_ReturnType retVal;
  Crypto_30_LibCv_SizeOfKeyStorageType bootProtectionElementIndexPtr = 0u;
  Crypto_30_LibCv_SizeOfKeyStorageType debuggerProtectionElementIndexPtr = 0u;
  uint32 elementLength = 1u;

  /* Init status */
  *statusPtr = CRYPTO_30_LIBCV_SHE_CLEARED_STATUS; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

  /* Get BOOT_PROTECTION and DEBUGGER_PROTECTION element id */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_BOOT_PROTECTION, &bootProtectionElementIndexPtr, &elementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  retVal |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_DEBUGGER_PROTECTION, &debuggerProtectionElementIndexPtr, &elementLength, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

  if (retVal == E_OK)
  {
    /* Check boot protection */
    if ((*(Crypto_30_LibCv_GetAddrKeyStorage(bootProtectionElementIndexPtr)) == 1u))
    {
      *statusPtr |= CRYPTO_30_LIBCV_SHE_STATUS_BOOT_FINISHED; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      *statusPtr |= CRYPTO_30_LIBCV_SHE_STATUS_BOOT_OK; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }

    /* Check debugger protection */
    if ((*(Crypto_30_LibCv_GetAddrKeyStorage(debuggerProtectionElementIndexPtr)) == 0u))
    {
      *statusPtr |= CRYPTO_30_LIBCV_SHE_STATUS_EXT_DEBUGGER; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_SHECMDGETID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGet()
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
 *
 *
 */
/* [SWS_Crypto_91006] */
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) errorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex = 0u;

  /* Try to get read lock */
  if (Crypto_30_LibCv_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
  {
    retVal = CRYPTO_E_BUSY;
  }
  else
  {
#if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
    /* Check if it is She DEBUG_CMD */
    if (Crypto_30_LibCv_She_IsDebugCmd(cryptoKeyId, keyElementId))
    {
      retVal = Crypto_30_LibCv_She_DebugCmd_GetChallenge(resultPtr, resultLengthPtr); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
    }
    else
#endif
      /* Search Key Element */
      if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        /* [SWS_Crypto_00087] Check that keyElementId is in valid range */
        *errorIdPtr = CRYPTO_E_PARAM_HANDLE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_INLINE_STACK */
        /* Info: This return value is according to the Spec although CRYPTO_E_KEY_NOT_AVAILABLE would make more sense */
        retVal = E_NOT_OK;
      }
      else
      {
        /* Check if the Key element is valid */
        if (FALSE == Crypto_30_LibCv_IsKeyElementValid(elementIndex))
        {
          /* [SWS_Crypto_00039] */
          /* Info: This return value is according to the Spec although CRYPTO_E_KEY_NOT_VALID would make more sense */
          retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
        }
        else
        {
          /* Check if read access is allowed */
          if (CRYPTO_30_LIBCV_RA_ALLOWED == (Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
          {
            retVal = Crypto_30_LibCv_Local_KeyElementGetNoShe(elementIndex, resultPtr, resultLengthPtr); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
          }
#if (CRYPTO_30_LIBCV_RAM_KEY_EXPORT == STD_ON)
          /* She key */
          else if (CRYPTO_30_LIBCV_RA_ENCRYPTED == (Crypto_30_LibCv_GetReadOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
          {
            retVal = Crypto_30_LibCv_Local_KeyElementGetShe(cryptoKeyId, elementIndex, resultPtr, resultLengthPtr); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
          }
#endif
          else
          {
            retVal = CRYPTO_E_KEY_READ_FAIL;
          }
        }
      }
  }
  Crypto_30_LibCv_Local_KeyReadLockRelease(cryptoKeyId);

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetNoShe()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91006] */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetNoShe(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_30_LibCv_SizeOfKeyStorageType resultIndex;
  Crypto_30_LibCv_LengthCheckType lengthCheck;
  uint32 writtenKeyLength = 0u;

#if (CRYPTO_30_LIBCV_STRICT_LENGTH_CHECK_FOR_KEY_ELEMENT_GET == STD_OFF)
  /* If the Key Element has partial access allowed, length check should not be strict */
  if (Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == TRUE)
  {
    lengthCheck = CRYPTO_30_LIBCV_LENGTH_CHECK_NONE;
  }
  else
#endif
  {
    lengthCheck = CRYPTO_30_LIBCV_LENGTH_CHECK_MAX;
  }

  writtenKeyLength = *resultLengthPtr;

  /* Get SHE key always if ra is allowed */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(elementIndex, &resultIndex, &writtenKeyLength, lengthCheck, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */

#if (CRYPTO_30_LIBCV_STRICT_LENGTH_CHECK_FOR_KEY_ELEMENT_GET == STD_OFF)
  if (Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == TRUE)
  {
    /* Check if written length is less than requested length */
    if (writtenKeyLength < *resultLengthPtr)
    {
      /* Use written length of key */
      *resultLengthPtr = writtenKeyLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
    }
    else
    {
      /* Use requested length of the key (already set). This will result in a partial copy. */
    }
  }
  else
#endif
  {
    /* Use written length of key */
    *resultLengthPtr = writtenKeyLength; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
  }

  /* Copy key element if access is allowed */
  if (retVal == E_OK)
  {
    Crypto_30_LibCv_CopyData(resultPtr, Crypto_30_LibCv_GetAddrKeyStorage(resultIndex), *resultLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_OFF)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_CopyData_Implementation()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_CopyData_Implementation(
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) sourceData,
  uint32 dataLength)
{
  uint32_least byteIdx;
  /* #10 Copy data bytewise to the target buffer */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
  {
    targetData[byteIdx] = sourceData[byteIdx]; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_SetData_Implementation()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_SetData_Implementation(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataBuf,
  uint8 pattern,
  uint32 dataLength)
{
  uint32_least byteIdx;
  /* #10 Set data bytewise */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
  {
    dataBuf[byteIdx] = pattern; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_ClearData_Implementation()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ClearData_Implementation(
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) dataBuf,
  uint32 dataLength)
{
  /* #10 Clear data bytewise */
  Crypto_30_LibCv_Local_SetData_Implementation(dataBuf, 0x00u, dataLength); /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
}
#endif /* (CRYPTO_30_LIBCV_USE_VSTD_LIB == STD_OFF) */

#if (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_CompareData_IsSmaller()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_CompareData_IsSmaller(
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) targetData,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) referenceData,
  uint32 dataLength)
{
  uint32_least byteIdx;
  Std_ReturnType retVal = E_NOT_OK;
  /* #10 Compare data bytewise. */
  for (byteIdx = 0u; byteIdx < dataLength; byteIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
  {
    if (targetData[byteIdx] < referenceData[byteIdx])
    {
      retVal = E_OK;
      break;
    }
  }
  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_KDF_ALGO_ISO_15118_CERTIFICATE_HANDLING_ENABLED == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyReadLockGetNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if ((Crypto_30_LibCv_GetKeyLock(cryptoKeyId) >= CRYPTO_30_LIBCV_KEY_LOCK_FREE) && (Crypto_30_LibCv_GetKeyLock(cryptoKeyId) < CRYPTO_30_LIBCV_KEY_LOCK_READ_MAX))
  {
    Crypto_30_LibCv_IncKeyLock(cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId)
{
  if (Crypto_30_LibCv_GetKeyLock(cryptoKeyId) > CRYPTO_30_LIBCV_KEY_LOCK_FREE) /* COV_CRYPTO_30_LIBCV_KEY_LOCKING_RELEASE */
  {
    Crypto_30_LibCv_DecKeyLock(cryptoKeyId); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
  }
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (Crypto_30_LibCv_GetKeyLock(cryptoKeyId) == CRYPTO_30_LIBCV_KEY_LOCK_FREE)
  {
    Crypto_30_LibCv_SetKeyLock(cryptoKeyId, CRYPTO_30_LIBCV_KEY_LOCK_WRITE);  /* SBSW_CRYPTO_30_LIBCV_CSL01 */
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(
  uint32 cryptoKeyId)
{

  if (Crypto_30_LibCv_GetKeyLock(cryptoKeyId) == CRYPTO_30_LIBCV_KEY_LOCK_WRITE) /* COV_CRYPTO_30_LIBCV_KEY_LOCKING_RELEASE */
  {
    Crypto_30_LibCv_SetKeyLock(cryptoKeyId, CRYPTO_30_LIBCV_KEY_LOCK_FREE); /* SBSW_CRYPTO_30_LIBCV_CSL01 */
  }

}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyReadLockGet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockGet(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal;

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  retVal = Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(cryptoKeyId);
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyReadLockRelease()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyReadLockRelease(
  uint32 cryptoKeyId)
{
  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(cryptoKeyId);
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyWriteLockGet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockGet(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal;

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  retVal = Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(cryptoKeyId);
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyWriteLockRelease()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyWriteLockRelease(
  uint32 cryptoKeyId)
{

  SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(cryptoKeyId);
  SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSearch()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyElementsIterType elementIdx;

  /* #10 Iterate over all elements in the key to find a matching key element */
  for (elementIdx = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIdx < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
  {
    if (keyElementId == Crypto_30_LibCv_GetIdOfKeyElements(elementIdx))
    {
      retVal = E_OK;
      *elementIndex = (Crypto_30_LibCv_SizeOfKeyElementsType)elementIdx; /* SBSW_CRYPTO_30_LIBCV_VARIABLE_ACCESS_PTR_FORM_STACK */
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_BinarySearchCryptoKeyId()
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
 */
CRYPTO_30_LIBCV_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_BinarySearchCryptoKeyId(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* Check if the found crypto key ID is smaller than the key element end index. */
  if (elementIndex < Crypto_30_LibCv_GetSizeOfKeyElements())
  {
    Crypto_30_LibCv_SizeOfKeyType minKey = 0u;
    Crypto_30_LibCv_SizeOfKeyType maxKey = (Crypto_30_LibCv_SizeOfKeyType)(Crypto_30_LibCv_GetSizeOfKey());
    Crypto_30_LibCv_SizeOfKeyType middleKey = 0u;

    /* Iterate through the list to find a key whose value is less than the Key Element End Index and
       greater than or equal to Key Element Start Index. The element index is less than the maximum element
       index and there is no gap between them, so we find the CryptoKeyId and exit the loop. */
    while (minKey < maxKey) /* COV_CRYPTO_30_LIBCV_BINARYSEARCH */ /* FETA_CRYPTO_30_LIBCV_BINARY_SEARCH */
    {
      /* Start at the middle and check if the elementIndex is smaller or larger than the beginning of the middle key index.
         If it is smaller, set the new middle key value to the middle between 0 and the old middle key.
         If it is larger, set the new middle key value to the middle between the old middle key value and the maximum key size. */
      middleKey = (Crypto_30_LibCv_SizeOfKeyType)Crypto_30_LibCv_Math_CalcMiddle(minKey, maxKey);

      if (Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(middleKey) > elementIndex)
      {
        /* cryptoKeyId must be lower than the middle */
        maxKey = (Crypto_30_LibCv_SizeOfKeyType)(middleKey);
      }
      else if (Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(middleKey) <= elementIndex)
      {
        /* cryptoKeyId must be higher than the middle */
        minKey = (Crypto_30_LibCv_SizeOfKeyType)(middleKey + 1u);
      }
      else
      {
        *cryptoKeyId = middleKey; /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        retVal = E_OK;
        /* Key has been found. Leave loop. */
        break;
      }
    }

    /* If key couldn't be found, retVal is already set to E_NOT_OK */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementIdsGet()
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
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementIdsGet(
  uint32 cryptoKeyId,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyElementIdsLengthPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00161] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (keyElementIdsPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (keyElementIdsLengthPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00162] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    Crypto_30_LibCv_KeyElementsIterType elementIndex;
    uint32 writeIdx = 0u;
    retVal = E_OK;

    /* #20 Iterate over all Key Element Ids */
    for(elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndex++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
    {
      /* #25 Copy Key Element Id, if the buffer is to small return with error */
      if (writeIdx < *(keyElementIdsLengthPtr))
      {
        keyElementIdsPtr[writeIdx] = Crypto_30_LibCv_GetIdOfKeyElements(elementIndex); /* SBSW_CRYPTO_30_LIBCV_ARRAY_ACCESS_WITH_SIZE_CHECK */
      }
      else
      {
        /* provided array is too small to store all element ids */
        retVal = CRYPTO_E_SMALL_BUFFER;
        /* [SWS_Crypto_00164] Check that provided buffer is big enough */
        errorId = CRYPTO_E_SMALL_BUFFER;
        break;
      }

      writeIdx++;
    }

    /* #30 save written data length */
    *keyElementIdsLengthPtr = writeIdx; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS_WITH_DET_CHECK */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_ELEMENT_IDS_GET,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyCopy()
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
 */
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyCopy(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Std_ReturnType retValBuf = E_NOT_OK;
  Crypto_30_LibCv_KeyElementsIterType elementIndexSrc;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst = 0u;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00156] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00157] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00158] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
    /* ----- Implementation ------------------------------------------------- */
  {
    /* #16 Lock write access for the target CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #17 Lock read access for the source CryptoKey */
      if (Crypto_30_LibCv_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
      {
        retVal = CRYPTO_E_BUSY;
      }
      else
      {
        retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
        /* #20 Loop over all elements of the source key */
        for (elementIndexSrc = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(cryptoKeyId); elementIndexSrc < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(cryptoKeyId); elementIndexSrc++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
        {
          /* #25 Loop over all elements of the destination key to find a matching element for the current source element */
          if (Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, Crypto_30_LibCv_GetIdOfKeyElements(elementIndexSrc), &elementIndexDst) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
          {
            /* #30 copy key element with Crypto_30_LibCv_Local_KeyElementCopy */
            retValBuf = Crypto_30_LibCv_Local_KeyElementCopy((Crypto_30_LibCv_SizeOfKeyElementsType)elementIndexSrc, elementIndexDst, targetCryptoKeyId);
            if (retVal != E_OK)
            {
              retVal = retValBuf;
            }
          }
        }
        Crypto_30_LibCv_Local_KeyReadLockRelease(cryptoKeyId);
      }
      Crypto_30_LibCv_Local_KeyWriteLockRelease(targetCryptoKeyId);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_COPY,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementCopy()
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
 */
  /* PRQA S 6080 5 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementCopy(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc = 0u;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst = 0u;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00149] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00150] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00151] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* #20 Find source element. Otherwise return with Error. */
    if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndexSrc) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* ----- Implementation ------------------------------------------------- */
      /* [SWS_Crypto_00152] if keyElementId is out of valid range return E_NOT_OK */
      /* [SWS_Crypto_00153] if targetKeyElementId is out of valid range return E_NOT_OK */
      retVal = E_NOT_OK;
    }
    else
    {
      /* ----- Implementation ------------------------------------------------- */
      /* #25 Find destination element. Otherwise return with Error. */
      if (Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, targetKeyElementId, &elementIndexDst) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        /* [SWS_Crypto_00152] Check that keyElementId is in valid range */
        /* [SWS_Crypto_00153] Check that targetKeyElementId is in valid range */
        errorId = CRYPTO_E_NO_ERROR;

        /* #30 Lock write access for the target CryptoKey */
        if (Crypto_30_LibCv_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
        {
          retVal = CRYPTO_E_BUSY;
        }
        else
        {
          /* #35 Lock read access for the source CryptoKey if necessary */
          /* #40 copy key element with Crypto_30_LibCv_Local_KeyElementCopy */
          if (cryptoKeyId == targetCryptoKeyId)
          {
            retVal = Crypto_30_LibCv_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
          }
          else if (Crypto_30_LibCv_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
          {
            retVal = CRYPTO_E_BUSY;
          }
          else
          {
            retVal = Crypto_30_LibCv_Local_KeyElementCopy(elementIndexSrc, elementIndexDst, targetCryptoKeyId);
            Crypto_30_LibCv_Local_KeyReadLockRelease(cryptoKeyId);
          }
          Crypto_30_LibCv_Local_KeyWriteLockRelease(targetCryptoKeyId);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementCopyPartial()
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
 */
/* PRQA S 6060, 6080 8 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementCopyPartial(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  uint32 keyElementSourceOffset,
  uint32 keyElementTargetOffset,
  uint32 keyElementCopyLength,
  uint32 targetCryptoKeyId,
  uint32 targetKeyElementId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexSrc = 0u;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndexDst = 0u;
  uint8 errorId = CRYPTO_E_PARAM_HANDLE;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00205] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00206] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00206] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* #20 Find source element. Otherwise return with Error. */
    if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndexSrc) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
    {
      /* ----- Implementation ------------------------------------------------- */
      /* [SWS_Crypto_00206] if keyElementId is out of valid range return E_NOT_OK */
      /* [SWS_Crypto_00206] if targetKeyElementId is out of valid range return E_NOT_OK */
    }
    else
    {
      /* ----- Implementation ------------------------------------------------- */
      /* #25 Find destination element. Otherwise return with Error. */
      if (Crypto_30_LibCv_Local_KeyElementSearch(targetCryptoKeyId, targetKeyElementId, &elementIndexDst) == E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      {
        /* [SWS_Crypto_00206] Check that keyElementId is in valid range */
        /* [SWS_Crypto_00206] Check that targetKeyElementId is in valid range */
        errorId = CRYPTO_E_NO_ERROR;

        /* #30 Lock write access for the target CryptoKey */
        if (Crypto_30_LibCv_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
        {
          retVal = CRYPTO_E_BUSY;
        }
        else
        {
          /* #35 Lock read access for the source CryptoKey if necessary */
          /* #40 copy key element with Crypto_30_LibCv_Local_KeyElementCopyPartial */
          if (cryptoKeyId == targetCryptoKeyId)
          {
            retVal = Crypto_30_LibCv_Local_KeyElementCopyPartial(elementIndexSrc, elementIndexDst, targetCryptoKeyId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength); /* PRQA S 2963 */ /* MD_CRYPTO_30_LIBCV_2963 */
          }
          else if (Crypto_30_LibCv_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
          {
            retVal = CRYPTO_E_BUSY;
          }
          else
          {
            retVal = Crypto_30_LibCv_Local_KeyElementCopyPartial(elementIndexSrc, elementIndexDst, targetCryptoKeyId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength);
            Crypto_30_LibCv_Local_KeyReadLockRelease(cryptoKeyId);
          }
          Crypto_30_LibCv_Local_KeyWriteLockRelease(targetCryptoKeyId);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_ELEMENT_COPY_PARTIAL,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementSetInternalStandard()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternalStandard(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex;
  uint32 maxLength = Crypto_30_LibCv_GetKeyElementLength(elementIndex);

  keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Copy keyPtr to key storage. */

  Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), keyPtr, keyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  /* #15 If the new written length is shorter than the old, delete the rest of the old key. */
  if (maxLength > keyLength)
  {
    Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex + keyLength), (uint32)(maxLength - keyLength)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  }

  retVal = Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyIdSearch(elementIndex, keyLength);

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  Crypto_30_LibCv_KeyStorageIterType keyStorageIndex;
  uint32 maxLength = Crypto_30_LibCv_GetKeyElementLength(elementIndex);

  keyStorageIndex = Crypto_30_LibCv_GetKeyStorageStartIdxOfKeyElements(elementIndex);
  /* #10 Copy keyPtr to key storage. */

  Crypto_30_LibCv_CopyData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex), keyPtr, keyLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  /* #15 If the new written length is shorter than the old, delete the rest of the old key. */
  if (maxLength > keyLength)
  {
    Crypto_30_LibCv_ClearData(Crypto_30_LibCv_GetAddrKeyStorage(keyStorageIndex + keyLength), (uint32)(maxLength - keyLength)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_STORAGE_VIA_KEY_ELEMENT */
  }

  Crypto_30_LibCv_SetKeyElementWrittenLengthWithCryptoKeyId(cryptoKeyId, elementIndex, keyLength);
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSetInternal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSetInternal(
  uint32 cryptoKeyId,
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength,
  Crypto_30_LibCv_WriteOfKeyElementInfoType writeAccess)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */
#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
  if ((writeAccess != CRYPTO_30_LIBCV_WA_ENCRYPTED) &&
      Crypto_30_LibCv_IsSheKey(elementIndex))
  {
    retVal = CRYPTO_E_KEY_WRITE_FAIL;
  }
  else
#endif
  {
    {
      /* #10 Check if it is a write once key which is written already. */
      if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
      {
        retVal = CRYPTO_E_KEY_WRITE_FAIL;
      }
      /* #20 Copy data if there is enough free space */
      /* Check if the available data is not bigger than the maximum size of the destination element */
      else if ((keyLength == Crypto_30_LibCv_GetKeyElementLength(elementIndex)) ||
        ((Crypto_30_LibCv_IsKeyElementPartial(elementIndex) == TRUE) &&
               (keyLength <= Crypto_30_LibCv_GetKeyElementLength(elementIndex))))
      {
        if (writeAccess >= (Crypto_30_LibCv_GetWriteOfKeyElementInfo(Crypto_30_LibCv_GetKeyElementInfoIdxOfKeyElements(elementIndex))))
        {
          Crypto_30_LibCv_KeyElementSetInternalStandardWithCryptoKeyId(cryptoKeyId, elementIndex, keyPtr, keyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
          retVal = E_OK;
        }
        else
        {
          retVal = CRYPTO_E_KEY_WRITE_FAIL;
        }
      }
      else
      {
        /* Key length is to large */
        retVal = CRYPTO_E_KEY_SIZE_MISMATCH; /* [SWS_Crypto_00146] */
      }
    }
  }
  return retVal;
}

#if ((CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON))
/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementSet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* [SWS_Crypto_91004] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex = 0u;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Find key element, otherwise return with Error */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  else
  {
    /* #15 Handle non-virtual and virtual Key */
    retVal = Crypto_30_LibCv_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, keyPtr, keyLength, CRYPTO_30_LIBCV_WA_INTERNAL_COPY); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

    if (retVal == E_OK)
    {
      Crypto_30_LibCv_Local_SetKeyElementStateWritten(elementIndex);
    }
  }

  return retVal;
}
#endif /* ((CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON) || (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementSet()
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
 */
/* [SWS_Crypto_91004] */
/* PRQA S 6030, 6050, 6080 5 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementSet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00075] */
  }
  /* #15 Check plausibility of parameters */
  else if (keyPtr == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER; /* [SWS_Crypto_00078] */
  }
  else
# if (CRYPTO_30_LIBCV_KEY_ELEMENT_DELETE == STD_OFF)
  if (keyLength == 0u)
  {
    errorId = CRYPTO_E_PARAM_VALUE; /* [SWS_Crypto_00079] */
  }
  else
# endif
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00076] */
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    /* ----- Local Variables ------------------------------------------------ */
    Crypto_30_LibCv_SizeOfKeyElementsType elementIndex = 0u;

    /* ----- Implementation ------------------------------------------------- */

    /* #20 Lock write access for the CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
#if (CRYPTO_30_LIBCV_SHE_DEBUG_CMD == STD_ON)
      /* Check if it is She DEBUG_CMD */
      if (Crypto_30_LibCv_She_IsDebugCmd(cryptoKeyId, keyElementId))
      {
        retVal = Crypto_30_LibCv_She_DebugCmd_SetAuthorizationAndLock(keyPtr, keyLength); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
      }
      else
#endif
        /* #23 Find key element, otherwise return with Error */
        if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        {
          /* [SWS_Crypto_00077] check if keyElementId is in valid range */
          errorId = CRYPTO_E_PARAM_HANDLE;
          retVal = E_NOT_OK;
        }
        else
        {
          /* #25 Handle non-virtual and virtual key elements */
          retVal = Crypto_30_LibCv_Local_KeyElementSetInternal(cryptoKeyId, elementIndex, keyPtr, keyLength, CRYPTO_30_LIBCV_WA_ALLOWED); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
#if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
          if ((retVal == CRYPTO_E_KEY_SIZE_MISMATCH) ||
              (retVal == CRYPTO_E_KEY_WRITE_FAIL))
          {
            /* [SWS_Crypto_00146] */
            /* #30 Else: if key size matching update using SHE key update protocol */
            retVal = Crypto_30_LibCv_Local_KeyElementSetShe(cryptoKeyId, elementIndex, keyPtr, keyLength, retVal); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
          }
#endif

          if (retVal == E_OK)
          {
            /* Keep write once flag this can be set with M1M2M3. */
            Crypto_30_LibCv_ClearKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_CLEAR_NORMAL_MASK | CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
            if (Crypto_30_LibCv_IsKeyElementWriteOnce(elementIndex))
            {
              Crypto_30_LibCv_SetKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK); /* SBSW_CRYPTO_30_LIBCV_CSL02_KEY_ELEMENT_VIA_KEY */
            }
          }
        }
      Crypto_30_LibCv_Local_KeyWriteLockRelease(cryptoKeyId);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_ELEMENT_SET,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyValidSet()
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
 */
/* [SWS_Crypto_91005] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyValidSet(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  boolean writeBlock;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT; /* [SWS_Crypto_00082] */
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    errorId = CRYPTO_E_PARAM_HANDLE; /* [SWS_Crypto_00083] */
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    /* #17 Lock write access for the CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGet(cryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #20 Set the given KeyId valid */
      writeBlock = Crypto_30_LibCv_SetKeyState(cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK);

#if (CRYPTO_30_LIBCV_CMAC_AES_ROUNDKEY_REUSE == STD_ON)
      Crypto_30_LibCv_ClearObjectWorkspaceForChangedKey(cryptoKeyId);
#endif
      Crypto_30_LibCv_Local_KeyWriteLockRelease(cryptoKeyId);

#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
      if (writeBlock)
      {
        Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(cryptoKeyId));
      }
#else
      CRYPTO_30_LIBCV_DUMMY_STATEMENT(writeBlock); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_VALID_SET,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */
#if ((CRYPTO_30_LIBCV_SHEKEYS == STD_ON) && (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON))
  boolean access = FALSE;
  if (!Crypto_30_LibCv_IsSheKey(elementIndex))
  {
    access = TRUE;
  }
  else
  {
    if (serviceType != CRYPTO_30_LIBCV_SHE_SERVICE_OTHER)
    {
      access = Crypto_30_LibCv_Local_KeyElementGetSheCheckFid(elementIndex, serviceType);
    }
  }

  if (access == FALSE)
  {
    retVal = CRYPTO_E_KEY_READ_FAIL;
  }
  else
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(serviceType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  {
    {
      /* #40 Handle non-virtual key element */
      retVal = Crypto_30_LibCv_Local_KeyElementGet_Standard(resultIndexPtr, (P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))resultLengthPtr, elementIndex, lengthCheck); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
/* [SWS_Crypto_91006] */
/* PRQA S 6060 7 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Search Key Element */
  if (Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex) != E_OK) /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
  {
    retVal = CRYPTO_E_KEY_NOT_AVAILABLE;
  }
  /* #15 Check if the Key element is valid */
  else if (FALSE == Crypto_30_LibCv_IsKeyElementValid(elementIndex))
  {
    /* [SWS_Crypto_00039] */
    retVal = CRYPTO_E_KEY_NOT_VALID;
  }
  else
  {
    /* #20 Get Key Element */
    retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(elementIndex, resultIndexPtr, resultLengthPtr, lengthCheck, serviceType); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetStorageIndex()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndex(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* # Get key element storage index. */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(cryptoKeyId, keyElementId, resultIndexPtr, resultLengthPtr, lengthCheck, CRYPTO_30_LIBCV_SHE_SERVICE_OTHER); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob()
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
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* # Get key element storage index. */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(cryptoKeyId, keyElementId, resultIndexPtr, resultLengthPtr, lengthCheck, CRYPTO_30_LIBCV_SHE_SERVICE_OTHER); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* # Parse allowed return values for job processing. */
  if (retVal == E_OK)
  {

  }
  else if (retVal == CRYPTO_E_KEY_NOT_VALID)
  {

  }
  else  if (retVal == CRYPTO_E_SMALL_BUFFER)
  {
    retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional()
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
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_KeyElementGetStorageIndexJobOptional(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, AUTOMATIC) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, AUTOMATIC) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* # Get key element storage index. */
  retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(cryptoKeyId, keyElementId, resultIndexPtr, resultLengthPtr, lengthCheck, CRYPTO_30_LIBCV_SHE_SERVICE_OTHER); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

  /* # Parse allowed return values for job processing. */
  if (retVal == E_OK)
  {

  }
  else if (retVal == CRYPTO_E_KEY_NOT_VALID)
  {

  }
  else if (retVal == CRYPTO_E_KEY_NOT_AVAILABLE)
  {

  }
  else
  {
    retVal = CRYPTO_E_KEY_SIZE_MISMATCH;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_GetElementsIndexJob()
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
 */
/* [SWS_Crypto_91006] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_GetElementsIndexJob(
  uint32 cryptoKeyId,
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType numberOfElements,
  Crypto_30_LibCv_LengthCheckType lengthCheck
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_KeyElementGetSizeType i;
  for (i = 0; i < numberOfElements; i++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_PARAMETER_DEPENDENT_UPPER_BOUND */
  {
    retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexJob(cryptoKeyId, keyElements[i].keyElementId, &keyElements[i].keyElementIndex, &keyElements[i].keyElementLength, lengthCheck); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
    if (retVal != E_OK)
    {
      break;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_ElementGetterSetId()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ElementGetterSetId(
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType elementNumber,
  uint32 keyElementId)
{
  keyElements[elementNumber].keyElementId = keyElementId; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Local_ElementGetterSetIdAndLength()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91006] */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Local_ElementGetterSetIdAndLength(
  P2VAR(Crypto_30_LibCv_KeyElementGetType, AUTOMATIC, AUTOMATIC) keyElements,
  Crypto_30_LibCv_KeyElementGetSizeType elementNumber,
  uint32 keyElementId,
  uint32 keyLength)
{
  keyElements[elementNumber].keyElementId = keyElementId; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
  keyElements[elementNumber].keyElementLength = keyLength; /* SBSW_CRYPTO_30_LIBCV_API_CHECKED_SIZE_ACCESS */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyElementGet()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91006] */
/* PRQA S 6080 5 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyElementGet(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00085] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (resultPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00088] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (resultLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00089] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*resultLengthPtr == 0u)
  {
    /* [SWS_Crypto_00090] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00086] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    retVal = Crypto_30_LibCv_Local_KeyElementGet(cryptoKeyId, keyElementId, resultPtr, resultLengthPtr, &errorId); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #90 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_ELEMENT_GET,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_RandomSeed()
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
 */
/* [SWS_Crypto_91013] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_RandomSeed(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) entropyPtr,
  uint32 entropyLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00128] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (entropyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00130] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (entropyLength == 0u)
  {
    /* [SWS_Crypto_00131] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00129] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
#if ((CRYPTO_30_LIBCV_FIPS186 == STD_ON) || (CRYPTO_30_LIBCV_DRBGAES == STD_ON) || (CRYPTO_30_LIBCV_DRBGHASHSHA512 == STD_ON))
    retVal = Crypto_30_LibCv_Local_RandomSeed(cryptoKeyId, entropyPtr, entropyLength); /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
#else
    retVal = E_NOT_OK;
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(entropyPtr);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(entropyLength);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #25 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_RANDOM_SEED,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyGenerate()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

/* [SWS_Crypto_91007] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyGenerate(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00094] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00095] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
#if (CRYPTO_30_LIBCV_KEY_GENERATE_ALGORITHM == STD_ON)
    retVal = Crypto_30_LibCv_Local_KeyGenerate(cryptoKeyId);
#else
    retVal = E_NOT_OK;
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_GENERATE,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyDerive()
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
/* [SWS_Crypto_91008] */
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyDerive(
  uint32 cryptoKeyId,
  uint32 targetCryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00097] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00098] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (targetCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_?????] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
#if (CRYPTO_30_LIBCV_KEY_DERIVE_ALGORITHM == STD_ON)
    boolean writeBlock = FALSE;
    /* #30 Lock write access for the target CryptoKey */
    if (Crypto_30_LibCv_Local_KeyWriteLockGet(targetCryptoKeyId) != E_OK)
    {
      retVal = CRYPTO_E_BUSY;
    }
    else
    {
      /* #35 Lock read access for the source CryptoKey if necessary */
      /* #40 derive key element with Crypto_30_LibCv_Local_KeyDerive */
      if (cryptoKeyId == targetCryptoKeyId)
      {
        retVal = Crypto_30_LibCv_Local_KeyDerive(cryptoKeyId, targetCryptoKeyId, &writeBlock); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
      }
      else if (Crypto_30_LibCv_Local_KeyReadLockGet(cryptoKeyId) != E_OK)
      {
        retVal = CRYPTO_E_BUSY;
      }
      else
      {
        retVal = Crypto_30_LibCv_Local_KeyDerive(cryptoKeyId, targetCryptoKeyId, &writeBlock); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */
        Crypto_30_LibCv_Local_KeyReadLockRelease(cryptoKeyId);
      }

      Crypto_30_LibCv_Local_KeyWriteLockRelease(targetCryptoKeyId);

# if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
      if (writeBlock)
      {
        Crypto_30_LibCv_NvBlock_Write_Req(Crypto_30_LibCv_GetNvBlockIdxOfKey(targetCryptoKeyId));
      }
# endif
    }
#else
    retVal = E_NOT_OK;
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(targetCryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID, CRYPTO_30_LIBCV_INSTANCE_ID, CRYPTO_30_LIBCV_SID_KEY_DERIVE, errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyExchangeCalcPubVal()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91009] */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcPubVal(
  uint32 cryptoKeyId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValuePtr, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_API_REQUIREMENT */
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) publicValueLengthPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_API_REQUIREMENT */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = CRYPTO_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00103] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (publicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00105] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (publicValueLengthPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00106] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (*publicValueLengthPtr == 0u)
  {
    /* [SWS_Crypto_00107] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00104] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  /* The retVal is already captured in the methods body. */
  /* [SWS_Crypto_00110] */
  else
  {
    /* ----- Implementation ------------------------------------------------- */

#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)
    /* #20 Calculate Public Value */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcPubVal(cryptoKeyId, publicValuePtr, publicValueLengthPtr);  /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */

# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
    if (retVal == CRYPTO_E_SMALL_BUFFER)
    {
      /* [SWS_Crypto_00110] */
      errorId = CRYPTO_E_PARAM_VALUE;
    }
# endif
#else
    retVal = E_NOT_OK;
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(publicValuePtr);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(publicValueLengthPtr);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_PUB_VAL,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_KeyExchangeCalcSecret()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* [SWS_Crypto_91010] */
/* PRQA S 6080 4 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_KeyExchangeCalcSecret(
  uint32 cryptoKeyId,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00111] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (partnerPublicValuePtr == NULL_PTR)
  {
    /* [SWS_Crypto_00113] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else if (partnerPublicValueLength == 0u)
  {
    /* [SWS_Crypto_00115] */
    errorId = CRYPTO_E_PARAM_VALUE;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00112] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Runtime Error Check ------------------------------------------------- */
#if (CRYPTO_30_LIBCV_KEY_EXCHANGE_ALGORITHM == STD_ON)
    /* #20 Calculate Secret Value */
    retVal = Crypto_30_LibCv_Local_KeyExchangeCalcSecret(cryptoKeyId, partnerPublicValuePtr, partnerPublicValueLength);  /* SBSW_CRYPTO_30_LIBCV_PTR_FORWARDING_WITH_DET_CHECK */
#else
    retVal = E_NOT_OK;
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(partnerPublicValuePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(partnerPublicValueLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #45 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_KEY_EXCHANGE_CALC_SECRET,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_CertificateParse()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateParse(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00168] */
    errorId = CRYPTO_E_UNINIT;
  }
  else
#endif
  /* #15 Check plausibility of parameters */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00169] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */

    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_CERTIFICATE_PARSE,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_CertificateVerify()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */

FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_CertificateVerify(
  uint32 cryptoKeyId,
  uint32 verifyCryptoKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_2 */
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    /* [SWS_Crypto_00172] */
    errorId = CRYPTO_E_UNINIT;
  }
  /* #15 Check plausibility of parameters */
  else if (verifyPtr == NULL_PTR)
  {
    /* [SWS_Crypto_00175] */
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
#endif
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00173] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else if (verifyCryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    /* [SWS_Crypto_00174] */
    errorId = CRYPTO_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------- */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(cryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(verifyCryptoKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    CRYPTO_30_LIBCV_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_CERTIFICATE_VERIFY,
                          errorId);
  }
#else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif

  return retVal;
}

#if (CRYPTO_30_LIBCV_NVBLOCK == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_State_Init()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_State_Init(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* ----- Implementation ------------------------------------------------- */
  Crypto_30_LibCv_SetNvBlockState(blockIdx, CRYPTO_30_LIBCV_NVBLOCK_STATE_IDLE); /* SBSW_CRYPTO_30_LIBCV_CSL02_NV_BLOCK */
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_ReadFromBlock()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_ReadFromBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2CONST(void, AUTOMATIC, CRYPTO_30_LIBCV_APPL_DATA) NvMBuffer)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  boolean locked = FALSE;
  Crypto_30_LibCv_KeyIterType lockedKeyIdx, releaseKeyIdx;
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr = (P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))NvMBuffer; /* PRQA S 0316 */ /* MD_CRYPTO_30_LIBCV_0316 */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  else if (NvMBuffer == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------- */

    /* Lock keys for write access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (lockedKeyIdx = 0u; lockedKeyIdx < Crypto_30_LibCv_GetSizeOfKey(); lockedKeyIdx++)
    {
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(lockedKeyIdx) == blockIdx)
      {
        locked = TRUE;
        if (Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected((uint32)lockedKeyIdx) == E_NOT_OK)
        {
          locked = FALSE;
          break;
        }
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (locked == TRUE)
    {
      /* Copy Buffer to  Key Storage */
      retVal = Crypto_30_LibCv_NvBlock_ReadFromBlock_Copy(blockIdx, NvMBufferPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
    }

    /* Release keys for write access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (releaseKeyIdx = 0; releaseKeyIdx < lockedKeyIdx; releaseKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_UPPER_BOUND_STATIC_GENDATA */
    {
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(releaseKeyIdx) == blockIdx)
      {
        Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected((uint32)releaseKeyIdx);
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

  }
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_NVBLOCK_READFROMBLOCK,
                          errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_WriteToBlock()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 3 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_WriteToBlock(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  P2VAR(void, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBuffer)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  boolean locked = FALSE;
  Crypto_30_LibCv_KeyIterType lockedKeyIdx, releaseKeyIdx;
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) NvMBufferPtr = (P2VAR(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR))NvMBuffer; /* PRQA S 0316 */ /* MD_CRYPTO_30_LIBCV_0316 */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  else if (NvMBuffer == NULL_PTR)
  {
    errorId = CRYPTO_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------- */
    /* Lock keys for read access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (lockedKeyIdx = 0u; lockedKeyIdx < Crypto_30_LibCv_GetSizeOfKey(); lockedKeyIdx++)
    {
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(lockedKeyIdx) == blockIdx)
      {
        locked = TRUE;
        if (Crypto_30_LibCv_Local_KeyReadLockGetNotProtected((uint32)lockedKeyIdx) == E_NOT_OK)
        {
          locked = FALSE;
          break;
        }
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();

    if (locked == TRUE)
    {
      /* Copy Key Storage to Buffer. */
      Crypto_30_LibCv_NvBlock_WriteToBlock_Copy(blockIdx, NvMBufferPtr); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_AND_ACCESS_TO_NVBUFFER */
      retVal = E_OK;
    }

    /* Release keys for read access. */
    SchM_Enter_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
    for (releaseKeyIdx = 0; releaseKeyIdx < lockedKeyIdx; releaseKeyIdx++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_UPPER_BOUND_STATIC_GENDATA */
    {
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(releaseKeyIdx) == blockIdx)
      {
        Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected((uint32)releaseKeyIdx);
      }
    }
    SchM_Exit_Crypto_30_LibCv_CRYPTO_30_LIBCV_EXCLUSIVE_AREA_0();
  }
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_NVBLOCK_WRITETOBLOCK,
                          errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Init()
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
 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Init(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Crypto_30_LibCv_KeyIterType keyIdx;
  Crypto_30_LibCv_KeyElementsIterType elementIndex;
# if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
  Crypto_30_LibCv_SheKeysIterType sheKeyIndex;
# endif
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------- */
    retVal = E_OK;
    Crypto_30_LibCv_NvBlock_State_Init(blockIdx);

    /* Initialize keys */
    for (keyIdx = 0u; keyIdx < Crypto_30_LibCv_GetSizeOfKey(); keyIdx++)
    {
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(keyIdx) == blockIdx)
      {
        for (elementIndex = Crypto_30_LibCv_GetKeyElementsStartIdxOfKey(keyIdx); elementIndex < Crypto_30_LibCv_GetKeyElementsEndIdxOfKey(keyIdx); elementIndex++) /* FETA_CRYPTO_30_LIBCV_MONOTONIC_LOOP_UP_WITH_UNCHANGED_BOUNDS_STATIC_GENDATA */
        {
          if (Crypto_30_LibCv_IsKeyElementPersist(elementIndex))
          {
            Crypto_30_LibCv_Init_Key(elementIndex, TRUE);
          }
        }
      }
    }

    /* Initialize she keys */
# if (CRYPTO_30_LIBCV_SHEKEYS == STD_ON)
    for (sheKeyIndex = 0u; sheKeyIndex < Crypto_30_LibCv_GetSizeOfSheKeys(); sheKeyIndex++)
    {
      keyIdx = Crypto_30_LibCv_GetKeyIdxOfSheKeys(sheKeyIndex);
      if (Crypto_30_LibCv_GetNvBlockIdxOfKey(keyIdx) == blockIdx)
      {
        elementIndex = Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(sheKeyIndex);
        if (Crypto_30_LibCv_IsKeyElementPersist(elementIndex))
        {
          Crypto_30_LibCv_Init_KeySheAdditional(elementIndex);
        }
      }
    }
# endif
  }
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_NVBLOCK_INIT,
                          errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_NvBlock_Callback()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_NvBlock_Callback(
  Crypto_30_LibCv_SizeOfNvBlockType blockIdx,
  uint8 ServiceId,
  NvM_RequestResultType JobResult)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CRYPTO_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CRYPTO_30_LIBCV_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (Crypto_30_LibCv_IsModuleNotInitialized())
  {
    errorId = CRYPTO_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------- */
    retVal = E_OK;
    switch (ServiceId)
    {
      case CRYPTO_30_LIBCV_NVM_WRITE_BLOCK:
      case CRYPTO_30_LIBCV_NVM_WRITE_ALL:
      {
        Crypto_30_LibCv_NvBlock_State_CallbackWrittenToBlock(blockIdx);
        break;
      }

      default:
      /* Service not handled. */
      break;
    }
    /* ----- Development Error Report --------------------------------------- */
  }
# if (CRYPTO_30_LIBCV_DEV_ERROR_REPORT == STD_ON)
  /* Report error if applicable */
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CRYPTO_30_LIBCV_MODULE_ID,
                          CRYPTO_30_LIBCV_INSTANCE_ID,
                          CRYPTO_30_LIBCV_SID_NVBLOCK_CALLBACK,
                          errorId);
  }
# else
  CRYPTO_30_LIBCV_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  CRYPTO_30_LIBCV_DUMMY_STATEMENT(JobResult); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_NVBLOCK == STD_ON) */

#if(CRYPTO_30_LIBCV_KEYSETVALID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_KeySetValid()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
/* [SWS_Crypto_91014] */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_KeySetValid(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  if (mode == CRYPTO_OPERATIONMODE_FINISH)
  {
    /* #10 Set the given KeyId valid */
    Crypto_30_LibCv_ProcessJob_Trigger_Write[objectId] = Crypto_30_LibCv_SetKeyState(job->cryptoKeyId, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK); /* SBSW_CRYPTO_30_LIBCV_GLOBAL_ARRAY_ACCESS_WITH_SIZE */

  }
  return E_OK;
}
#endif /* Crypto_30_LibCv_Dispatch_KeySetValid */

#if(CRYPTO_30_LIBCV_SHECMDGETID == STD_ON)
/**********************************************************************************************************************
 * Crypto_30_LibCv_Dispatch_SheCmdGetId()
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
/* PRQA S 6050,6080 4 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Dispatch_SheCmdGetId(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) job, /* PRQA S 3673 */ /* MD_CRYPTO_30_LIBCV_3673_1 */
  Crypto_OperationModeType mode)
{
  Std_ReturnType retVal = E_NOT_OK, localRet;
  Crypto_30_LibCv_SizeOfSheKeysType indexOfSheMasterKey = 0u;
  Crypto_30_LibCv_SizeOfKeyStorageType uIdStorageIndex, masterKeyStorageIndex;
  uint32 length;
  uint8 result[CRYPTO_30_LIBCV_SIZEOF_CMAC];
  uint8 buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_CMD_GET_ID_OUTPUT];
  uint8 status;

  if (Crypto_30_LibCv_IsJobMode(job, CRYPTO_OPERATIONMODE_SINGLECALL))
  {
    if (mode == CRYPTO_OPERATIONMODE_FINISH)
    {
      P2VAR(eslt_WorkSpaceCMACAES, AUTOMATIC, CRYPTO_30_LIBCV_CRYPTOCV_APPL_VAR) workspace = Crypto_30_LibCv_GetWorkspaceOfSheCmdGetId(Crypto_30_LibCv_GetSheCmdGetIdIdxOfObjectInfo(objectId));

      /* #10 Get Master Key She Idx */
      /* Find key can be voided because master key must be available for cmd get id */
      (void)Crypto_30_LibCv_GetSheKey(CRYPTO_30_LIBCV_SHE_M1_ECU_MASTER_KEY_ID, CRYPTO_30_LIBCV_SHE_PAGE0, &indexOfSheMasterKey); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

      /* #20 The input must have a length of 16 bytes, the crypto key must be set to the master key and must also be valid */
      if ((job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputLength == CRYPTO_30_LIBCV_SIZEOF_SHE_CHALLENGE) &&
        (Crypto_30_LibCv_GetKeyIdxOfSheKeys(indexOfSheMasterKey) == job->cryptoKeyId) &&
        (Crypto_30_LibCv_IsKeyElementValid(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfSheMasterKey))))
      {
        /* #30 Lock key for reading */
        if (Crypto_30_LibCv_Local_KeyReadLockGet(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate()) == E_OK)
        {
          /* #40 Get she status */
          localRet = Crypto_30_LibCv_She_Cmd_Get_Status(&status); /* SBSW_CRYPTO_30_LIBCV_STACK_VARIABLE_AS_PTR */

          /* #50 Get UID */
          length = CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID;
          localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndex(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate(), CRYPTO_KE_CUSTOM_SHE_UID, &uIdStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_MIN); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

          /* #60 Get Master Key */
          length = CRYPTO_30_LIBCV_SIZEOF_SHE_KEY;
          localRet |= Crypto_30_LibCv_Local_KeyElementGetStorageIndexBasic(Crypto_30_LibCv_GetKeyElementsKeyIdxOfSheKeys(indexOfSheMasterKey), &masterKeyStorageIndex, &length, CRYPTO_30_LIBCV_LENGTH_CHECK_EQUAL, CRYPTO_30_LIBCV_SHE_SERVICE_KEY_SERVICE);  /* COV_CRYPTO_30_LIBCV_SHE_KEY_CONFIGURATION_2 */ /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */

          if (localRet == E_OK)
          {
            /* MAC = CMAC[ 16 bytes CHALLENGE | 15 bytes UID | 1 byte STATUS ] with Master K */
            Crypto_30_LibCv_CopyData(buffer, job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.inputPtr, CRYPTO_30_LIBCV_SIZEOF_SHE_CHALLENGE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
            Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_CHALLENGE], Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
            Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_CHALLENGE + CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID], &status, CRYPTO_30_LIBCV_SIZEOF_SHE_STATUS); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

            /* #70 Calculate Mac */
            if (Crypto_30_LibCv_SheKeyUpdateCmac(buffer, CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER, Crypto_30_LibCv_GetAddrKeyStorage(masterKeyStorageIndex), result, workspace) == E_OK)  /* SBSW_CRYPTO_30_LIBCV_FORWARDING_STACK_BUFFER */
            {
              /* Output = [ 15 bytes UID | 1 byte STATUS | 16 bytes CMAC ] */
              Crypto_30_LibCv_CopyData(buffer, Crypto_30_LibCv_GetAddrKeyStorage(uIdStorageIndex), CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
              Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID], &status, CRYPTO_30_LIBCV_SIZEOF_SHE_STATUS); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */
              Crypto_30_LibCv_CopyData(&buffer[CRYPTO_30_LIBCV_SIZEOF_SHE_M1_UID + CRYPTO_30_LIBCV_SIZEOF_SHE_STATUS], result, CRYPTO_30_LIBCV_SIZEOF_CMAC); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

              /* Adapt output size */
              if (*job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr > CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER)
              {
                *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr = CRYPTO_30_LIBCV_SIZEOF_ENC_BUFFER; /* SBSW_CRYPTO_30_LIBCV_WRITE_ACCESS_TO_JOBPRIMITIVE_INPUTOUTPUT_PTR */
              }

              /* #80 Copy result to output buffer */
              Crypto_30_LibCv_CopyData(job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputPtr, buffer, *job->CRYPTO_30_LIBCV_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.outputLengthPtr); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_ACCESS_WITH_SIZE */

              retVal = E_OK;
            }
          }
          /* #90 Unlock Key */
          Crypto_30_LibCv_Local_KeyReadLockRelease(Crypto_30_LibCv_GetSheInfoKeyRefOfSheKeyUpdate());
        }
      }
    }
    else
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif /* (CRYPTO_30_LIBCV_SHECMDGETID == STD_ON) */

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_GetKeyFlags()
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030 3 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_GetKeyFlags(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_KeyFlagType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyFlagsPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex = 0u;

  /* ----- Implementation ------------------------------------------------- */
  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* Get elementIndex, keyElementId is checked implicitly */
    retVal = Crypto_30_LibCv_Local_KeyElementSearch(cryptoKeyId, keyElementId, &elementIndex); /* SBSW_CRYPTO_30_LIBCV_STACK_ARRAY_VARIABLE_AS_PTR */
  }

  if (retVal == E_OK)
  {
    /* Set correct Flags */
    /* TRUE: Key is valid
       FALSE: Key is invalid */
    if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_VALID_MASK))
    {
      keyFlagsPtr->isKeyValid = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }
    else
    {
      keyFlagsPtr->isKeyValid = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }

    /* TRUE:  Rewriting and erasing disabled
       FALSE: Rewriting and erasing enabled */
    if (Crypto_30_LibCv_IsKeyElementStateByMask(elementIndex, CRYPTO_30_LIBCV_KEYELEMENTSTATE_WRITTEN_ONCE_MASK))
    {
      keyFlagsPtr->write_protection = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }
    else
    {
      keyFlagsPtr->write_protection = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
    }
    {
#if ((CRYPTO_30_LIBCV_SHEKEYS == STD_ON) && (CRYPTO_30_LIBCV_SHE_ENABLE_FID == STD_ON))
      uint8 sheExtensions;
      sheExtensions = Crypto_30_LibCv_GetKeyElementExtension(elementIndex);

      /* TRUE: Key is an SHE Key
         FALSE: Key is not a SHE Key */
      if (Crypto_30_LibCv_IsSheKey(elementIndex))
      {
        keyFlagsPtr->isSheKey = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */

# if (CRYPTO_30_LIBCV_KEYELEMENTSBOOTPROTECTIONIDXOFSHEPAGE == STD_ON)
      /* TRUE: usage is limited
         FALSE: Key usage is not limited according to result of secure boot */
        if (Crypto_30_LibCv_Uint8CheckMask(sheExtensions, CRYPTO_30_LIBCV_SHE_FID_MASK_BOOT_PROTECTION))
        {
          keyFlagsPtr->boot_protection = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
        else
# endif
        {
          keyFlagsPtr->boot_protection = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }

# if (CRYPTO_30_LIBCV_KEYELEMENTSDEBUGGERPROTECTIONIDXOFSHEPAGE == STD_ON)
      /* TRUE: key can not be used if debugger is attached
         FALSE: Use of a debug interface does not impose a limitation on key usage */
        if (Crypto_30_LibCv_Uint8CheckMask(sheExtensions, CRYPTO_30_LIBCV_SHE_FID_MASK_DEBUGGER_PROTECTION))
        {
          keyFlagsPtr->debugger_protection = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
        else
# endif
        {
          keyFlagsPtr->debugger_protection = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }

      /* TRUE: key can be used for CMAC generate/verify
         FALSE: Key can be used for AES ECB+CBC encrypt/decrypt */
        if (Crypto_30_LibCv_Uint8CheckMask(sheExtensions, CRYPTO_30_LIBCV_SHE_FID_MASK_KEY_USAGE))
        {
          keyFlagsPtr->key_usage = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
        else
        {
          keyFlagsPtr->key_usage = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }

        /* TRUE: Key update by wildcard is disabled
           FALSE: Key update by wildcard UID is enabled */
        if (Crypto_30_LibCv_Uint8CheckMask(sheExtensions, CRYPTO_30_LIBCV_SHE_FID_MASK_WILDCARD))
        {
          keyFlagsPtr->disable_wildcard = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
        else
        {
          keyFlagsPtr->disable_wildcard = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }

        /* TRUE: Key can not be used to execute CMAC generate but only for CMAC verify
           FALSE: Key can be used to execute CMAC generate */
        if (Crypto_30_LibCv_Uint8CheckMask(sheExtensions, CRYPTO_30_LIBCV_SHE_FID_MASK_CMAC_USAGE))
        {
          keyFlagsPtr->cmac_usage = TRUE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
        else
        {
          keyFlagsPtr->cmac_usage = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        }
      }
      else
#endif
      {
        keyFlagsPtr->isSheKey = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        keyFlagsPtr->boot_protection = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        keyFlagsPtr->debugger_protection = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        keyFlagsPtr->key_usage = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        keyFlagsPtr->disable_wildcard = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
        keyFlagsPtr->cmac_usage = FALSE; /* SBSW_CRYPTO_30_LIBCV_PTR_ACCESS */
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyElementGetStorageIndex()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 6 */ /* MD_CRYPTO_30_LIBCV_STPAR */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementGetStorageIndex(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyStorageType, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultIndexPtr,
  P2VAR(uint32, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) resultLengthPtr,
  Crypto_30_LibCv_LengthCheckType lengthCheck,
  Crypto_30_LibCv_ServiceType serviceType)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* Call Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended */
    retVal = Crypto_30_LibCv_Local_KeyElementGetStorageIndexExtended(
      cryptoKeyId,
      keyElementId,
      resultIndexPtr,
      resultLengthPtr,
      lengthCheck,
      serviceType); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyReadLockGetNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyReadLockGetNotProtected(
  uint32 cryptoKeyId)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* Call Crypto_30_LibCv_Local_KeyReadLockGetNotProtected */
    retVal = Crypto_30_LibCv_Local_KeyReadLockGetNotProtected(cryptoKeyId);
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyReadLockReleaseNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyReadLockReleaseNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* Call Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected */
    Crypto_30_LibCv_Local_KeyReadLockReleaseNotProtected(cryptoKeyId);
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyElementSearch()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementSearch(
  uint32 cryptoKeyId,
  uint32 keyElementId,
  P2VAR(Crypto_30_LibCv_SizeOfKeyElementsType, AUTOMATIC, AUTOMATIC) elementIndex)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = Crypto_30_LibCv_Local_KeyElementSearch( /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
      cryptoKeyId,
      keyElementId,
      elementIndex);
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyWriteLockGetNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyWriteLockGetNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = Crypto_30_LibCv_Local_KeyWriteLockGetNotProtected(cryptoKeyId);
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyWriteLockReleaseNotProtected()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyWriteLockReleaseNotProtected(
  uint32 cryptoKeyId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (cryptoKeyId >= Crypto_30_LibCv_GetSizeOfKey())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    Crypto_30_LibCv_Local_KeyWriteLockReleaseNotProtected(cryptoKeyId);
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Crypto_30_LibCv_Api_KeyElementSetInternalStandard()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_LIBCV_CODE) Crypto_30_LibCv_Api_KeyElementSetInternalStandard(
  Crypto_30_LibCv_SizeOfKeyElementsType elementIndex,
  P2CONST(uint8, AUTOMATIC, CRYPTO_30_LIBCV_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (elementIndex >= Crypto_30_LibCv_GetSizeOfKeyElements())
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = Crypto_30_LibCv_KeyElementSetInternalStandard(elementIndex, keyPtr, keyLength); /* SBSW_CRYPTO_30_LIBCV_FORWARDING_PTR */
  }

  return retVal;
}

#define CRYPTO_30_LIBCV_STOP_SEC_CODE
#include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_KeyManagement.c
 *********************************************************************************************************************/

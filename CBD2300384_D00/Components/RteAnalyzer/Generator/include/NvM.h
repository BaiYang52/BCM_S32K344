/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  
 *        \brief  NvM Header for RTE Analyzer
 *
 *      \details  This header provides the static defines, prototypes and datatypes of the NVM module
 *                that are required for the static analysis with RTE Analyzer.
 *
 *********************************************************************************************************************/
 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-07-31  visso                 Initial creation
 *  01.00.01  2018-10-26  visdes                STORYC-6726: [RTE] Improved handling of NVM_E_BLOCK_PENDING for NvBlock SWCs RTE Analyzer
 *  01.02.00  2021-02-18  visso   RTE-4007      Rework TechRef according to templates and remove full name from sources
 *  01.03.00  2022-02-01  visso   RTE-5303      Add NvM_GetDataIndex and NvM_SetDataIndex to RTE Analyzer NVM stubs
 *********************************************************************************************************************/

#ifndef NVM_H
# define NVM_H 1


#include "RteAnalyzer/Source/NvM_Cfg.h"

/**********************************************************************************************************************
 * API TYPE DEFINITIONS
 *********************************************************************************************************************/

 /* result values of asynchronous requests (stored in the RAM Mngmnt),
    * They are also defined by the RTE, since these are the important values for an SW-C */

#ifndef NVM_REQ_OK
# define NVM_REQ_OK               (0u)  /* The last asynchronous request has been finished successfully */
#endif
#ifndef NVM_REQ_NOT_OK
# define NVM_REQ_NOT_OK           (1u)  /* The last asynchronous request has been finished unsuccessfully */
#endif
#ifndef NVM_REQ_PENDING
# define NVM_REQ_PENDING          (2u)  /* An asynchronous request is currently being processed */
#endif
#ifndef NVM_REQ_INTEGRITY_FAILED
# define NVM_REQ_INTEGRITY_FAILED (3u)  /* Result of the last NvM_ReadBlock or NvM_ReadAll is an integrity failure */
#endif
#ifndef NVM_REQ_BLOCK_SKIPPED
# define NVM_REQ_BLOCK_SKIPPED    (4u)  /* The referenced block was skipped during a multi block request */
#endif
#ifndef NVM_REQ_NV_INVALIDATED
# define NVM_REQ_NV_INVALIDATED   (5u)  /* The NV block is invalidated. */
#endif
#ifndef NVM_REQ_CANCELED
# define NVM_REQ_CANCELED        (6u)  /* A WriteAll was cancelled */
#endif
#ifndef NVM_REQ_REDUNDANCY_FAILED
# define NVM_REQ_REDUNDANCY_FAILED  (7u) /* A redundant block lost its redundancy */
#endif
#ifndef NVM_REQ_RESTORED_FROM_ROM
# define NVM_REQ_RESTORED_FROM_ROM  (8u) /* Default data from ROM are restored */
#endif

#define NVM_START_SEC_CODE
#include "MemMap.h"


FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_GetErrorStatus(NvM_BlockIdType BlockId, P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) RequestResultPtr);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_ReadBlock(NvM_BlockIdType BlockId, P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_WriteBlock(NvM_BlockIdType BlockId, P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_InvalidateNvBlock(NvM_BlockIdType BlockId);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_EraseNvBlock(NvM_BlockIdType BlockId);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvM_DstPtr);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_GetDataIndex(NvM_BlockIdType BlockId, P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) DataIndexPtr);

FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex);

#define NVM_STOP_SEC_CODE
#include "MemMap.h"

#endif

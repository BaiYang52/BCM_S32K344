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
/*!         \file  ESLib_AES_common.h
 *         \brief  AES common includes, types and functions (inline!).
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
#ifndef ESLIB_AES_COMMON_H
# define ESLIB_AES_COMMON_H

# include "ESLib.h"
# include "ESLib_Helper.h"

/* actCLib includes */
# include "actIAES.h"

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * esl_AES_setBlockAndPaddingModes()
 *********************************************************************************************************************/
/*! \brief          Sets block mode and padding mode.
 *  \details         Checks if block mode and padding mode are valid. If it is valid, the function sets them.
 *  \param[out]      target          Pointer to option byte where block mode and padding mode is stored.
 *  \param[in]       extBlockMode    Block mode which shall be set.
 *  \param[in]       extPaddingMode  Padding mode which shall be set
 *  \return          ESL_ERC_MODE_INVALID, if padding mode or block mode is invalid
 *                   ESL_ERC_NO_ERROR, otherwise
 *  \pre             target has to be a valid pointer.
 *  \context         TASK
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_setBlockAndPaddingModes(VSECPRIM_P2VAR_PARA(eslt_Byte) target, eslt_BlockMode extBlockMode, eslt_PaddingMode extPaddingMode);

/**********************************************************************************************************************
 * esl_AES_CommonInitChecks()
 *********************************************************************************************************************/
/*! \brief          Checks workspace and key buffer.
 *  \details         Checks workspace for validity as well as key buffer.
 *  \param[in]       workSpace  pointer to workspace which shall be checked.
 *  \param[in]       wsSize  size of the workspace
 *  \param[in]       key  pointer to buffer which shall be checked for validity.
 *  \return          ESL_ERC_NO_ERROR  Operation successful
 *                   ESL_ERC_PARAMETER_INVALID  key parameter is NULL
 *                   ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre             -
 *  \context         TASK
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonInitChecks(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) key);

/**********************************************************************************************************************
 * esl_AES_CommonInit()
 *********************************************************************************************************************/
/*! \brief          Checks workspace and key buffer and sets block- and padding mode.
 *  \details         Initializes AES by checking the workspace and key buffer and setting block- and padding mode.
 *  \param[out]      targetMode    Pointer to option byte where block mode and padding mode is stored.
 *  \param[in]       workSpace     pointer to workspace which shall be checked.
 *  \param[in]       wsSize        size of the workspace.
 *  \param[in]       key           pointer to buffer which shall be checked for validity.
 *  \param[in]       blockMode     block mode which shall be set.
 *  \param[in]       paddingMode   padding mode which shall be set.
 *  \return          ESL_ERC_NO_ERROR, if operation was successful.
 *                   ESL_ERC_MODE_INVALID, if padding mode or block mode is invalid
 *                   ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                   ESL_ERC_PARAMETER_INVALID  key parameter is NULL
 *  \pre             -
 *  \context         TASK
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonInit(VSECPRIM_P2VAR_PARA(eslt_Byte) targetMode, VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) key,
  eslt_BlockMode blockMode, eslt_PaddingMode paddingMode);

/**********************************************************************************************************************
 * esl_AES_CommonProcChecks()
 *********************************************************************************************************************/
/*! \brief          Checks current state of the workspace as well as the buffers.
 *  \details         Checks workspace and buffers for validity and the current state of the workspace.
 *  \param[in]       workSpace   Pointer to the workspace which shall be checked.
 *  \param[in]       algo        Algorithm value
 *  \param[in]       wsSize      Size of the workspace
 *  \param[in]       inBuffer    Input Buffer which shall be checked for validity.
 *  \param[in]       outBuffer   Output Buffer which shall be checked for validity.
 *  \return          ESL_ERC_NO_ERROR            Operation successful
 *                   ESL_ERC_PARAMETER_INVALID   Input parameter or output parameter are NULL
 *                   ESL_ERC_WS_STATE_INVALID    Work space state invalid
 *  \pre             -
 *  \context         TASK
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_AES_CommonProcChecks(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceAES) workSpace, eslt_WSStatus algo, eslt_Length wsSize, VSECPRIM_P2CONST_PARA(eslt_Byte) inBuffer, VSECPRIM_P2CONST_PARA(eslt_Byte) outBuffer);

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ESLIB_AES_COMMON_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_AES_common.h
 *********************************************************************************************************************/

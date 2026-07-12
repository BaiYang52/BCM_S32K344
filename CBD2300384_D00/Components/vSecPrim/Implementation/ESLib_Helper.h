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
/*!        \file  ESLib_Helper.h
 *        \brief  ES Library internal header file
 *
 *      \details  Contains type definitions and helper function declarations, which are internally used
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

#ifndef ESLIB_HELPER_H
# define ESLIB_HELPER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ESLib.h"
# include "ESLib_types.h"

# include "actUtilities.h"

 /**********************************************************************************************************************
  *  GLOBAL FUNCTION MACROS
  *********************************************************************************************************************/
# define Esl_MemSet(dest, fill, count)                                (actMemset(dest, fill, count))
# define Esl_MemClear(dest, count)                                    (actMemClear(dest, count))
# define Esl_MemCpyByteArray(dest, src, count)                        (actMemCpyByteArray(dest, src, count))
# define Esl_MemCpyWordArray(dest, src, count)                        (actMemCpyWordArray(dest, src, count))

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef VSECPRIM_P2VAR_PARA(eslt_WorkSpace) eslt_WorkSpaceRefType;
typedef VSECPRIM_P2CONST_PARA(eslt_WorkSpace) eslt_WorkSpaceConstRefType;

typedef VSECPRIM_P2CONST_PARA(eslt_Length) eslt_LengthConstRef;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  esl_ResetAndClearWorkspace
 **********************************************************************************************************************/
/*! \brief         Resets and clears the workspace.
 *  \details       This function nullifies the workspace and resets the workspace state.
 *  \param[in,out] workSpaceHeader  Pointer to workspace header
 *  \param[in,out] workSpaceBuffer  Pointer to workspace buffer
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspace objects
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(void) esl_ResetAndClearWorkspace(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  VSECPRIM_P2VAR_PARA(void) workSpaceBuffer);

/***********************************************************************************************************************
 *  esl_SetWorkspaceStatus
 **********************************************************************************************************************/
/*! \brief         Sets the workspace status.
 *  \details       This function sets the workspace status and the algorithm.
 *  \param[in,out] workSpaceHeader  Pointer to workspace header
 *  \param[in]     algo  Algorithm value
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspace objects
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(void) esl_SetWorkspaceStatus(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_WSStatus algo);

/***********************************************************************************************************************
 *  esl_CheckWorkSpaceHeader
 **********************************************************************************************************************/
/*! \brief         Check workspace integrity.
 *  \details       This function checks the workspace pointer for validity as well as evaluates the size of the work-
 *                 space.
 *  \param[in]     workSpaceHeader  Pointer to workspace header
 *  \param[in]     minSize  Mininum workspace size required
 *  \return        ESL_ERC_WS_TOO_SMALL value stored in workSpaceHeader->size is smaller than minSize
 *                 ESL_ERC_NO_ERROR  else
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceHeader(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_Length minSize);

/***********************************************************************************************************************
 *  esl_CheckWorkSpaceState
 **********************************************************************************************************************/
/*! \brief         Check workspace header state.
 *  \details       -
 *  \param[in]     workSpaceHeader  Pointer to workspace header
 *  \param[in]     algo             Algorithm value
 *  \return        ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_NO_ERROR          else
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspace objects
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceState(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_WSStatus algo);

/***********************************************************************************************************************
 *  esl_CheckWorkSpaceHeaderAndState
 **********************************************************************************************************************/
/*! \brief         Check workspace header and state.
 *  \details       This function checks  the size of the workspace and the internal state.
 *  \param[in]     workSpaceHeader  Pointer to workspace header
 *  \param[in]     minSize          Mininum workspace size required
 *  \param[in]     algo             Algorithm value
 *  \return        ESL_ERC_WS_TOO_SMALL value stored in workSpaceHeader->size is smaller than minSize
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *                 ESL_ERC_NO_ERROR  else
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspace objects
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_CheckWorkSpaceHeaderAndState(VSECPRIM_P2CONST_PARA(eslt_WorkSpaceHeader) workSpaceHeader,
  eslt_Length minSize,
  eslt_WSStatus algo);

/***********************************************************************************************************************
 *  esl_checkLengthRef
 **********************************************************************************************************************/
/*! \brief         Check length reference
 *  \details       This function checks the pointer validity and its size.
 *  \param[in]     lenRef  Pointer to the length value
 *  \param[in]     lengthValue  Needed length
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(eslt_ErrorCode) esl_checkLengthRef(eslt_LengthConstRef lenRef,
  eslt_Length lengthValue);

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ESLIB_HELPER_H */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_Helper.h
 *********************************************************************************************************************/

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
/**        \file  Dcm_MemMgr.h
 *         \unit  MemMgr
 *        \brief  Contains public inline function definitions of Memory Manager unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_MEMMGR_H)
# define DCM_MEMMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_MemMgrTypes.h"
# if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)
#  include "Dcm_Uti.h"
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Memory manager operations function prototype descriptors */
#  define DCM_MEMMGR_OP_READ                                         ((Dcm_MemMgrMemoryOpType)0u)
#  if (DCM_MEMMGR_MEMOP_READ_ENABLED == STD_ON)
#   define DCM_MEMMGR_OP_WRITE                                       ((Dcm_MemMgrMemoryOpType)(DCM_MEMMGR_OP_READ + 1u))
#  else
#   define DCM_MEMMGR_OP_WRITE                                       ((Dcm_MemMgrMemoryOpType)(DCM_MEMMGR_OP_READ + 0u))
#  endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_MemMgrMemoryOpType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_MemMgrValidateAndGetAlfid()
 *********************************************************************************************************************/
/*! \brief          Validates the request memory ALFID and extracts it from request.
 *  \details        -
 *  \param[in,out]  pMsgContext        Requested memory data starting with the ALFID byte.
 *  \param[out]     pAlfid             ALFID byte
 *  \param[out]     pBlockLength       Returns the left part of the stream (i.e. without protocol header)
 *  \param[out]     ErrorCode          The NRC
 *  \return         DCM_E_OK           No error found
 *  \return         DCM_E_NOT_OK       Invalid ALFID (NRC is set to ErrorCode)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            The request data within the pMsgContext must contain an ALFID byte.
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrValidateAndGetAlfid(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8VarDataPtrType pAlfid,
  Dcm_Uint8VarDataPtrType pBlockLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
);

/**********************************************************************************************************************
 *  Dcm_MemMgrParseMemStream()
 *********************************************************************************************************************/
/*! \brief          Validates the request memory ALFID.
 *  \details        -
 *  \param[in]      alfid         Requested ALFID byte
 *  \param[in,out]  pMsgContext   Current message context with the memory address and size byte stream
 *  \param[out]     pMemBlock     Interpreted memory block information
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            It shall be assured that the ALFID value matches the length of the data stream to be parsed.
 *********************************************************************************************************************/
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrParseMemStream(
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_MemMgrMemBlockPtrType pMemBlock
);
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_MemMgrCheckMemBlock()
 *********************************************************************************************************************/
/*! \brief         Checks a specific memory block.
 *  \details        -
 *  \param[in]      pMemBlock       The memory block descriptor for validation
 *  \param[in]      memOp           The memory block operation type
 *  \param[out]     ErrorCode       The NRC
 *  \return         DCM_E_OK        Check is positive
 *  \return         DCM_E_NOT_OK    Check is negative
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemBlock(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_NegativeResponseCodePtrType ErrorCode
);

#  if (DCM_MEMMGR_MEMOP_READ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MemMgrReadMemory()
 *********************************************************************************************************************/
/*! \brief          Executes a read memory operation.
 *  \details        -
 *  \param[in]      opStatus           Current operation status
 *  \param[in]      pMemBlock          The memory block descriptor for validation
 *  \param[in]      pDataContext       Pointer to the data context
 *  \param[out]     ErrorCode          The NRC
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP  Forces a RCR-RP response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrReadMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
);
#  endif

#  if (DCM_MEMMGR_MEMOP_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MemMgrWriteMemory()
 *********************************************************************************************************************/
/*! \brief          Executes a write memory operation.
 *  \details        -
 *  \param[in]      opStatus      Current operation status
 *  \param[in]      pMemBlock     The memory block descriptor for validation
 *  \param[in]      data          data buffer
 *  \param[out]     ErrorCode     The NRC
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP  Forces a RCR-RP response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrWriteMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
);
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  if !defined(DCM_NOUNIT_MEMMGR)                                                                                                                    /* COV_DCM_NOUNIT TX */
/**********************************************************************************************************************
 *  Dcm_MemMgrValidateAndGetAlfid()
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
DCM_GLOBAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrValidateAndGetAlfid(                                                                      /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8VarDataPtrType pAlfid,
  Dcm_Uint8VarDataPtrType pBlockLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_UtiValidateAndGetAlfid(pMsgContext, Dcm_CfgMemMgrAlfidLookUpTable, pAlfid, pBlockLength, ErrorCode);                              /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_POINTER_FORWARD_STACK */

  if (lStdResult == DCM_E_OK)
  {
    if (*pBlockLength > pMsgContext->reqDataLen)
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_MemMgrParseMemStream()
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
DCM_GLOBAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrParseMemStream(                                                                                     /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_MemMgrMemBlockPtrType pMemBlock
  )
{
  uint8_least lAddressLength;

  lAddressLength = Dcm_UtiGetMemBlockAddrSize(alfid);

  if (Dcm_UtiLookUpUint8IsEmpty(Dcm_CfgMemMgrMidLookUpTable))                                                                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  {
    pMemBlock->Mid = 0;                                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    --lAddressLength; /* skip mid */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &(pMemBlock->Mid));                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* extract address */
  pMemBlock->Address = (Dcm_CfgMemMgrReqAddrType)Dcm_UtiDwordExtract(pMsgContext, lAddressLength);                                                   /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* extract size  */
  pMemBlock->Length = (Dcm_CfgMemMgrReqSizeType)Dcm_UtiDwordExtract(pMsgContext, Dcm_UtiGetMemBlockLenSize(alfid));                                  /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}
#  endif /* !defined(DCM_NOUNIT_MEMMGR) */
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_MEMMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_MemMgr.h
 *********************************************************************************************************************/

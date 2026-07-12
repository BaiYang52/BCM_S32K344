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
/**        \file  Dcm_MemMgr.c
 *         \unit  MemMgr
 *        \brief  Contains the implementation of Memory Manager unit.
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
#define DCM_MEMMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_MemMgr.h"
#if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Debug.h"
# include "Dcm_State.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
*  Dcm_MemMgrCheckMemoryRange()
*********************************************************************************************************************/
/*! \brief          Validates memory range within a particular memory map.
*  \details        -
*  \param[in]      pMemBlock          The memory block descriptor for validation
*  \param[in]      memOp              The memory block operation type
*  \param[in]      pMemMapDescriptor  The the corresponding memory map to be searched in
*  \param[out]     ErrorCode          The NRC
*  \return         DCM_E_OK     - memory range is valid
*  \return         DCM_E_NOT_OK - memory range validation failed (ErrorCode contains NRC)
*  \context        TASK
*  \reentrant      TRUE
*  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemoryRange(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor,
  Dcm_NegativeResponseCodePtrType ErrorCode
);
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
*  Dcm_MemMgrCheckMemoryRange()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemoryRange(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Std_ReturnType lStdResult;
  Dcm_CfgMemMgrReqAddrType reqEndAddress;
  Dcm_CfgMemMgrMemMapInfoPtrType pUsedMemMap;

  pUsedMemMap = pMemMapDescriptor->MemMapTable;
  reqEndAddress = pMemBlock->Address + (Dcm_CfgMemMgrReqAddrType)(pMemBlock->Length) - 1u; /* possible overflows does not take any effect for now */

                                                                                           /* Check for address overflow or no data required (length == 0) */
  if ((pMemBlock->Length != 0u)
    && (reqEndAddress >= pMemBlock->Address))
  {
    Dcm_CfgMemMgrMemMapIdxOptType  memMapIter = 0;
    Dcm_CfgMemMgrMemAddrType       lastEndAddr = pUsedMemMap->EndAddr;
    boolean                        isBlockBeginFound = FALSE;
    boolean                        isMemMapAreaFound = FALSE;

    lStdResult = DCM_E_OK;

    for (; memMapIter < pMemMapDescriptor->Size; ++memMapIter)                                                                                       /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
    {
      if (pUsedMemMap[memMapIter].EndAddr >= pMemBlock->Address) /* first block that may contain the requested range */
      {
        if (isBlockBeginFound == FALSE)
        {
          isBlockBeginFound = TRUE;
          if (pUsedMemMap[memMapIter].StartAddr > pMemBlock->Address) /* the start address must be within the configured block  */
          {
            /* the requested start address is not within the memory block */
            break;
          }
          /* else - OK, let's analyze the other properties of the configuration block */
        }
        else /* scanning subsequent blocks */
        {
          ++lastEndAddr; /* prepare for the potential next concatenated range startAddr */
          if (pUsedMemMap[memMapIter].StartAddr != lastEndAddr) /* no concatenated ranges */
          {
            /* Stop scanning due to an error */
            break;
          }/* else - the end address block was not found yet, check this range borders and conditions */
        }

        if (pUsedMemMap[memMapIter].ExecCondRefs[memOp] == 0u) /* memory operation supported for this block in currently active variant(s) ? */
        {
          /* Stop scanning due to an error */
          break;
        }
        else/* else - range supports operation - go on with state checks */
        {
# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
          if (Dcm_DiagIsEnabledInActiveVariants((Dcm_CfgStateRefOptType)(pUsedMemMap[memMapIter].ExecCondRefs[memOp]) - (Dcm_CfgStateRefOptType)1u) == FALSE) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
          {
            /* Stop scanning due to an error */
            break;
          }
# endif
        }

        if (lStdResult == DCM_E_OK) /* still no error found */
        {
          lStdResult = Dcm_StateCheck(((Dcm_CfgStateRefOptType)(pUsedMemMap[memMapIter].ExecCondRefs[memOp]) - (Dcm_CfgStateRefOptType)1u),
            DCM_DIAG_CHK_LVL_PARAMETER,
            ErrorCode);                                                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        }/* else - ErrorCode already set, let see if the range is at least OK */

        if (((Dcm_CfgMemMgrReqAddrType)pUsedMemMap[memMapIter].EndAddr) >= reqEndAddress)
        {
          isMemMapAreaFound = TRUE;
          /* stop scanning: send either POS_RES or first condition error: depends on the last lStdResult value
          * (ErrorCode already set in case of E_NOT_OK)
          */
          break;
        }/* else - try next range (if any still available) */
        lastEndAddr = pUsedMemMap[memMapIter].EndAddr;/* update for next iteration */
      }/* else - not reached a range from the bottom */
    }/* no valid range found */

     /* If requested start address is too high or end address was not met:
     * (e.g. due to wrong operation in any sub-area or end address too high, resp. outside of started area)
     */
    if (isMemMapAreaFound == FALSE)
    {
      /* Send negative response NRC 0x31 */
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }/* Otherwise - matching area found: either POS_RES for valid states or NEG_RES due to invalid states */
  }
  else/* else - length == 0 or req_addr > blockEnd */
  {
    /* Send negative response NRC 0x31 */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
*  Dcm_MemMgrCheckMemBlock()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemBlock(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Std_ReturnType lStdResult;
  sint16_least   lMidIdx = 0;

  if (!Dcm_UtiLookUpUint8IsEmpty(Dcm_CfgMemMgrMidLookUpTable))                                                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  {
    lMidIdx = Dcm_UtiLookUpUint8(Dcm_CfgMemMgrMidLookUpTable, pMemBlock->Mid);                                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }

  if (lMidIdx < 0)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    lStdResult = Dcm_MemMgrCheckMemoryRange(pMemBlock, memOp, Dcm_CfgWrapMemMgrMidInfo(lMidIdx), ErrorCode);                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  return lStdResult;
}

# if (DCM_MEMMGR_MEMOP_READ_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_MemMgrReadMemory()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrReadMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Std_ReturnType                lResult;
  Dcm_ReturnReadWriteMemoryType memOpResult;
  boolean                       lUnknown = FALSE;

  *ErrorCode = DCM_E_GENERALREJECT;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  if (Dcm_DebugDetectRuntimeError(pDataContext->AvailLen < pMemBlock->Length))                                                                       /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK XF */
  {
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else /* The data to be read fits into the available buffer */
  {
    memOpResult = Dcm_ReadMemory(opStatus
      , pMemBlock->Mid
      , pMemBlock->Address
      , pMemBlock->Length
      , Dcm_UtiGetDataContextBuffer(pDataContext)                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
      , ErrorCode
#  endif
    );                                                                                                                                               /* SBSW_DCM_CALL_READMEMORY */

    if (memOpResult == DCM_E_OK)
    {
      Dcm_UtiCommitData(pDataContext, pMemBlock->Length);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    }

    lResult = Dcm_UtiConvMemoryOpResult(memOpResult, opStatus, &lUnknown, ErrorCode);  /* RFC 57196 - return NRC 0x10 */                             /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (lUnknown == TRUE)
    {
      *ErrorCode = DCM_E_GENERALREJECT;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_READMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
    }

    if ((lResult != DCM_E_PENDING)
      && (lResult != DCM_E_FORCE_RCRRP))
    {
      /* Release resource */
      Dcm_RsrcMgrReleaseMemLock();
    }
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
    Dcm_UtiHandleApplNrc(lResult, ErrorCode, DCM_E_GENERALREJECT);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif
  }

  return lResult;
}                                                                                                                                                    /* PRQA S 6050 */ /*  MD_MSR_STCAL */
# endif

# if (DCM_MEMMGR_MEMOP_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_MemMgrWriteMemory()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrWriteMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Dcm_ReturnReadWriteMemoryType memOpResult;
  Std_ReturnType                lResult;
  boolean                       lUnknown = FALSE;

  *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

  memOpResult = Dcm_WriteMemory(opStatus
    , pMemBlock->Mid
    , pMemBlock->Address
    , pMemBlock->Length
    , data
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
    , ErrorCode
#  endif
  );                                                                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  lResult = Dcm_UtiConvMemoryOpResult(memOpResult, opStatus, &lUnknown, ErrorCode);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lUnknown == TRUE)
  {
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_WRITEMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
  }

  if ((lResult != DCM_E_PENDING)
    && (lResult != DCM_E_FORCE_RCRRP))
  {
    /* Release resource */
    Dcm_RsrcMgrReleaseMemLock();
  }
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
  Dcm_UtiHandleApplNrc(lResult, ErrorCode, DCM_E_GENERALPROGRAMMINGFAILURE);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif
  return lResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_MemMgr.c
 *********************************************************************************************************************/

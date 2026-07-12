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
/*!        \file  Com_Notifications.h
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_NOTIFICATIONS_H)
# define COM_NOTIFICATIONS_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com.h"
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */


# if ((COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)||(COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_TxNHdlr_InitTxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a tx I-PDU in Com_TxNHdlr.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_InitTxIpdu(Com_TxPduInfoIterType ComPduId);
# endif

# if (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxNHdlr_MainFunctionTx
**********************************************************************************************************************/
/*! \brief        This function calls all deferred Tx notification functions.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId);
# endif

# if (COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxNHdlr_StopTxIpdu
**********************************************************************************************************************/
/*! \brief        This function calls all error notifications of the passed ComIPdu if a transmit of the PDU was not confirmed yet.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \trace        SPEC-2736843
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_StopTxIpdu(Com_TxPduInfoIterType ComPduId);
# endif

# if ((COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)|| (COM_CBKTXACKIMFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)|| (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_TxNHdlr_Confirmation
**********************************************************************************************************************/
/*! \brief        This method sets the internal confirmation flag or calls the confirmation callback function.
    \details      -
    \pre          -
    \param[in]    ComPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_Confirmation(Com_TxPduInfoIterType ComPduId);
# endif

# if (COM_RXCBKFUNCPTR == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxCbkFctPtr
**********************************************************************************************************************/
/*! \brief            This caches or calls the function pointer in immediate or deferred cache.
    \details          -
    \pre              -
    \param[in]        cbkFuncPtrIdx       Index of function pointer that should be cached or called.
    \param[in]        comRxPduInfoIdx     Pdu Id
    \param[in,out]    fctPtrCacheStrctPtr contains all information of function pointer cache (location, size, index)
    \context          TASK|ISR2
    \synchronous      TRUE
    \reentrant        FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CacheOrCallRxCbkFctPtr(Com_RxCbkFuncPtrIterType cbkFuncPtrIdx, Com_RxPduInfoIterType comRxPduInfoIdx, P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr);
# endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_NOTIFICATIONS_H */
/**********************************************************************************************************************
  END OF FILE: Com_Notifications.h
**********************************************************************************************************************/

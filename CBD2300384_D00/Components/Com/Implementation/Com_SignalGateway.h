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
/*!        \file  Com_SignalGateway.h
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_SIGNALGATEWAY_H)
# define COM_SIGNALGATEWAY_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

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

# if (COM_GWINFOENDIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_InitRxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a rx I-PDU in Com_SignalGw.
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_InitRxIpdu(Com_RxPduInfoIterType rxPduId);
# endif

# if (COM_GWINFOUSEDOFRXSIGINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SigEvent
**********************************************************************************************************************/
/*! \brief        This function sets a signal routing event flag for the passed ComSignal.
    \details      -
    \pre          -
    \param[in]    idxRxSigInfo Index of RxSigInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_SigEvent(Com_RxSigInfoIterType idxRxSigInfo);
# endif

# if (COM_GWINFOUSEDOFRXSIGGRPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SigGrpEvent
**********************************************************************************************************************/
/*! \brief        This function sets a signal routing event flag for the passed ComSignalGroup.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo Index of RxSigGrpInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);
# endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_MainFunction
**********************************************************************************************************************/
/*! \brief        This function processes signal gateway events.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling route signals main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId);
# endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_SIGNALGATEWAY_H */

/**********************************************************************************************************************
  END OF FILE: Com_SignalGateway.h
**********************************************************************************************************************/

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
/*!        \file  Com_DescriptionGateway.h
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_DESCRIPTIONGATEWAY_H)
# define COM_DESCRIPTIONGATEWAY_H

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

# if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_MainFunction
**********************************************************************************************************************/
/*! \brief        This function processes deferred gateway description events that are queued in the gateway Event Cache
 *                in reverse order.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling route signals main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DescriptionGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId);
# endif

# if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_GwDescriptionEvent
**********************************************************************************************************************/
/*! \brief        This function handles the gateway description events.
    \details      -
    \pre          -
    \param[in]    rxComPduId          Handle of the rx I-PDU.
    \param[in]    PduInfoPtr          contains the SDU buffer and the SDU length
    \param[in]    isCalledFromMainFunction  Info if the function is called from the main function or not. Set to true in the main function, set to false in RxIndication
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwDescriptionEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, boolean isCalledFromMainFunction);
# endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_DESCRIPTIONGATEWAY_H */

/**********************************************************************************************************************
  END OF FILE: Com_DescriptionGateway.h
**********************************************************************************************************************/

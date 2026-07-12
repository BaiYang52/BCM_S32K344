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
/*!        \file  Com_CprQueueSigGw.h
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
 #if !defined (COM_CPRQUEUESIGGW_H)
# define COM_CPRQUEUESIGGW_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
# define COM_CPRQUEUE_HEADERVALUE_SIGNAL        0x01u
# define COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP   0x03u

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

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_Signal
 *********************************************************************************************************************/
/*! \brief       Get the content of the next signal gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \param[out]  idxGwSigMapping          Index of the gateway signal mapping
 *  \param[out]  data                     Pointer to the data
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigMapping, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_SignalGroup
 *********************************************************************************************************************/
/*! \brief       Get the content of the next signalGroup gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \param[out]  idxGwSigGrpMapping       Index of the gateway signalGroup mapping
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx, P2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, COM_APPL_DATA) idxGwSigGrpMapping);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_GroupSignal
 *********************************************************************************************************************/
/*! \brief       Get the content of the next group signal gateway routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  idxGwGrpSigMapping       Index of the gateway group signal mapping
 *  \param[out]  data                     Pointer to the data
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingEndIdxOfGwSigGrpMappingType idxGwGrpSigMapping, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) &&  (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Put_Signal
 *********************************************************************************************************************/
/*! \brief       Write the content of a signal or groupSignal gateway routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   idxGwSigMapping          Index of the gateway signal mapping
 *  \param[in]   data                     Pointer to the data to be written
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_Signal(Com_CCprQueueIterType routingQueueIndex, Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Put_SignalGroup
 *********************************************************************************************************************/
/*! \brief       Write the index of signalGroup gateway routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   idxGwSigGrpMapping       Index of the gateway signalGroup mapping
 *  \param[in]   data                     Pointer to the data to be writte
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping, CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
#endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CprQueueSigGw_H */

/**********************************************************************************************************************
  END OF FILE: Com_CprQueueSigGw.h
**********************************************************************************************************************/

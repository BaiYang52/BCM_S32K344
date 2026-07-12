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
/*!        \file  Com_CprQueueDescrGw.h
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_CPRQUEUEDESCRGW_H)
# define COM_CPRQUEUEDESCRGW_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
# define COM_CPRQUEUE_HEADERVALUE_DESCRIPTION   0x02u

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


# if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ReadData
 *********************************************************************************************************************/
/*! \brief       Get the content of the next description routing from the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[out]  gwTxPduIdx               Index of TxPduDescriptionInfo
 *  \param[out]  data                     Pointer to the data
 *  \param[in]   readIdx                  Index to the position in the queue where the data shall be read
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ReadData(Com_CCprQueueIterType routingQueueIndex, P2VAR(Com_GwTxPduDescriptionInfoIterType, AUTOMATIC, COM_APPL_DATA) gwTxPduIdx,
                                                  P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) data, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
# endif

# if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_WriteData
 *********************************************************************************************************************/
/*! \brief       Write the content of a description routing into the CrossPartitionRoutingQueue.
 *  \details     -
 *  \param[in]   routingQueueIndex        Id of the queue to be used
 *  \param[in]   gwTxPduIdx               Index of TxPduDescriptionInfo
 *  \param[in]   data                     Pointer to the data to be written
 *  \return      E_OK                     Write operation was successful.
 *               E_NOT_OK                 Write operation was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_WriteData(Com_CCprQueueIterType routingQueueIndex, Com_GwTxPduDescriptionInfoIterType gwTxPduIdx,
                                                             CONSTP2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) data);
# endif


# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_CprQueueDescrGw_H */

/**********************************************************************************************************************
  END OF FILE: Com_CprQueueDescrGw.h
**********************************************************************************************************************/

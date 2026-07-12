/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_Os.h
 *        \brief  Service API declaration for OS test module.
 *
 *      \details  -
 *
 *    \attention  Please note:
 *                The demo and example programs only show special aspects of the software. With regard to the fact
 *                that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *                expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (VITAHLP_OS_H)
# define VITAHLP_OS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# if ((VITAHLP_OS == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
#  include "vItaHlp_Os_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define VITAHLP_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_Os_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the OS test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Os_Core_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the requested service.
 *  \details     The detailed service parameters are read from the shared memory and are processed on the
 *               corresponding core of the device under test.
 *  \param[in]   logicalCoreId  The core identifier used to retrieve the command parameters from shared memory.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_Core_DispatchService(VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId);

/**********************************************************************************************************************
 *  vItaHlp_Os_EventTaskActivation()
 *********************************************************************************************************************/
/*! \brief       Function creates event content during task activation procssing.
 *  \details     -
 *  \param[in]   taskID  The taskID of the task that was activated.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_EventTaskActivation(VAR(vItaHlp_Os_TaskType, AUTOMATIC) taskID);

#  define VITAHLP_STOP_SEC_CODE
#  include "MemMap.h"

# endif /* ((VITAHLP_OS == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)) */

#endif /* VITAHLP_OS_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Os.h
 *********************************************************************************************************************/

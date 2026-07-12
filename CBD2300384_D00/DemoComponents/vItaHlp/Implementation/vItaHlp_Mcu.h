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
/**        \file  vItaHlp_Mcu.h
 *        \brief  vItaHlp functionalities concerning MCU.
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

#if !defined (VITAHLP_MCU_H)
# define VITAHLP_MCU_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# if ((VITAHLP_MCU == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
#  include "vItaHlp_Mcu_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Mcu_IncrementTaskActivationCounter()
 *********************************************************************************************************************/
/*! \brief       Increment the task activation counter.
 *  \details     A test task is configured for the MCU Clock Timing test. This counter is incremented, each time this
 *               test task is scheduled.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_IncrementTaskActivationCounter(void);

 /**********************************************************************************************************************
 *  vItaHlp_Mcu_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the MCU test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_DispatchService(VAR(uint8, AUTOMATIC) serviceIdentifier);

# endif /* ((VITAHLP_MCU == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)) */
    
#endif /* VITAHLP_MCU_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Mcu.h
 *********************************************************************************************************************/
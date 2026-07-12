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
/**        \file  vItaHlp_Fls.h
*        \brief  Service API declaration for Fls test module.
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

#if !defined (VITAHLP_FLS_H)
# define VITAHLP_FLS_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# include "vItaHlp_Fls_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define VITAHLP_FLS_PATTERN_LENGTH    (4u)

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
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

/***********************************************************************************************************************
   vItaHlp_Fls_Init()
**********************************************************************************************************************/
/*! \brief       Initialize Fls test module.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_Init
 **********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_Init(void);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)

/**********************************************************************************************************************
  vItaHlp_Fls_DispatchService
**********************************************************************************************************************/
/*! \brief       Dispatchs the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the Fls test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \config      Test communication is enabled.
 **********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
*  vItaHlp_Fls_MainFunction()
*********************************************************************************************************************/
/*! \brief       Main Function of vItaHlp_Fls.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_MainFunction
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Fls_MainFunction(void);

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_FLS_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Fls.h
 *********************************************************************************************************************/

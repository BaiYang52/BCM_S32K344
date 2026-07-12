/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actICMACAES.h
 *        \brief  Implementation of CMAC-AES
 *
 *      \details  RFC 4493 AES-CMAC.
 *                This file is part of the embedded systems library cvActLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ACTICMACAES_H
# define ACTICMACAES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

# include "actITypes.h"
# include "actAES.h"

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  actCMACAESInit
 **********************************************************************************************************************/
/*! \brief         Initialize CMAC calculation.
 *  \details       This function initializes the CMAC calculation and stores the user key.
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     key  AES key to be used
 *  \param[in]     key_length  Size of AES key to be used
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *                 actEXCEPTION_NULL  Key is NULL
 *                 actEXCEPTION_LENGTH  Invalid key length
 *  \pre           info must be a valid workspace pointer
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces.
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actCMACAESInit(
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actCMACAESUpdate
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation.
 *  \details       This function feeds the CMAC calculation with input data.
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *  \pre           info must be a valid workspace pointer
 *                 workspace must be initialized
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actCMACAESUpdate(
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actCMACAESFinalize
 **********************************************************************************************************************/
/*! \brief         Finalize CMAC calculation.
 *  \details       This function finalizes the CMAC calculation and provide the calculated MAC.
 *  \param[in]     info  Pointer to context structure
 *  \param[out]    mac  Pointer to computed CMAC, buffer size msut be 16 bytes
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *                 actEXCEPTION_LENGTH  Invalid input length
 *  \pre           info must be a valid workspace pointer
 *                 workspace must be initialized
 *                 mac must point to valid memory of at least 16 bytes
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actCMACAESFinalize(
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) mac,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/***********************************************************************************************************************
 *  END OF FILE: ACTICMACAES.H
 **********************************************************************************************************************/

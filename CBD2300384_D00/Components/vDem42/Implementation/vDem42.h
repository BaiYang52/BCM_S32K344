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
/**        \file  vDem42.h
 *        \brief  vDem42 SWC
 *
 *      \details  Header file of the vDem42 SWC
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-09-07  visygr  -             Initial version
 *  01.01.00  2017-10-19  visstk  STORYC-2285   Wrap configured callbacks (General)CallbackEventStatusChange
 *  01.02.00  2018-03-05  visstk  STORYC-4205   Wrap APIs Dem_GetEventFreezeFrameData, Dem_GetDTCOfEvent
 *                        visstk  ESCAN00098741 RTE wrapper functions called directly
 *  01.03.01  2021-11-30  visygr  ESCAN00105734 Compiler error: Cannot open include file 'Rte_VDem42_Type.h': No such file or directory
 *                        visygr  ESCAN00110715 Compiler warning: Incompatible redefinition of macro "UINT<8/16/24/32>_MAX"
 *  02.00.00  2022-02-18  visepp  DCM-4812      Usage of <Ma>_MemMap.h in vDem42
 *********************************************************************************************************************/

#if !defined (VDEM42_H)
# define VDEM42_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* Communication types */
# include "vDem42_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define VDEM42_VENDOR_ID                        (30u)
# define VDEM42_MODULE_ID                        (0xFFu)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VDEM42_SW_MAJOR_VERSION                 (2u)
# define VDEM42_SW_MINOR_VERSION                 (0u)
# define VDEM42_SW_PATCH_VERSION                 (0u)

# define VDEM42_UINT8_MAX                        0xFFu
# define VDEM42_UINT32_MAX                       0xFFFFFFFFu

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define VDEM42_IGNORE_UNREF_PARAM(param)                            ((void)(param))                                                                 /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VDEM42_START_SEC_CODE
# include "vDem42_MemMap.h"                                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  VDem42_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Memory initialization
 *  \details     Initializes the memory of the vDem42.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-139583
 *********************************************************************************************************************/
FUNC(void, VDEM42_CODE) VDem42_InitMemory(void);

/**********************************************************************************************************************
 *  VDem42_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the vDem42
 *  \details     Initializes the vDem42.
 *  \param[in]   configPtr               Pointer to a concrete configuration root - not used for now.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-139583
 *********************************************************************************************************************/
FUNC(void, VDEM42_CODE) VDem42_Init(void* configPtr);

# define VDEM42_STOP_SEC_CODE
# include "vDem42_MemMap.h"                                                                                                                          /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* VDEM42_H */

/**********************************************************************************************************************
 *  END OF FILE: vDem42.h
 *********************************************************************************************************************/

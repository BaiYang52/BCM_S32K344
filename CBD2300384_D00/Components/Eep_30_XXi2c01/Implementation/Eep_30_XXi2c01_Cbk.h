/*********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       \file     Eep_30_XXi2c01.h
 *       \brief    Eep Spi Callback header file
 *
 *       \details  Eep provides API services to upper layer (Ea). Eep performs jobs on underlying EEPROM device via
 *                 I2C bus communication.
 *
 *********************************************************************************************************************/

#if !defined (EEP_30_XXI2C01_CBK_H)
# define EEP_30_XXI2C01_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "Eep_30_XXi2c01_Cfg.h"

/**********************************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EEP_30_XXI2C01_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Eep_30_XXi2c01_ComEnd()
**********************************************************************************************************************/
/*!
 * \brief      Called by underlying i2c driver upon communication finish
 * \details    -
 * \pre        -
 * \context    ANY
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Eep22906
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_ComEnd(void);

# if (EEP_30_XXI2C01_MULTI_CHANNEL == STD_ON)
/**********************************************************************************************************************
  Eep_30_XXi2c01_Inst2_ComEnd()
**********************************************************************************************************************/
/*!
 * \brief      Called by underlying i2c driver upon communication finish
 * \details    -
 * \pre        Second EEP instance configured
 * \context    ANY
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Eep22906
 */
FUNC(void, EEP_30_XXI2C01_CODE) Eep_30_XXi2c01_Inst2_ComEnd(void);
# endif


# define EEP_30_XXI2C01_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * END OF FILE: Eep_30_XXi2c01_Cbk.h
 *********************************************************************************************************************/

#endif /* EEP_30_XXI2C01_CBK_H */

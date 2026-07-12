/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \ingroup      Os_Hal
 * \defgroup     Os_Hal_Derivative HAL Derivative
 *
 * \{
 * \file         Os_Hal_DerivativeInt.h
 * \brief        Selects a derivative specific header and includes it.
 * \details
 *  This modules decides which HAL specific sub components are needed to implement HAL functionality on a specific
 *  derivative.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/


#ifndef OS_HAL_DERIVATIVEINT_H
# define OS_HAL_DERIVATIVEINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */

/* Os module declarations */
# include "Os_Cfg.h"

/* Os kernel module dependencies */

/* Os hal dependencies */
# if defined(OS_CFG_DERIVATIVEGROUP_ATSAMV7X)
#  include "Os_Hal_Derivative_ATSAMV7xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K14X)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32K14xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K148)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32K148Int.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_GENERIC_CORTEXM)                                                                  /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_Generic_CortexMInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_GENERIC_CORTEXM_MP)                                                               /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_Generic_CortexM_MpInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_TDA2X)                                                                            /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_TDA2xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT2BX)                                                                           /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_CYT2BxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT2CX)                                                                           /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_CYT2CxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8X)                                                                            /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_IMX8xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8XDUALCORE)                                                                    /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_IMX8xDualCoreInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8DXL)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_IMX8DXLInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32X)                                                                             /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32G3X)                                                                           /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32G3xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4BFX)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_CYT4BFxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4BBX)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_CYT4BBxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4DNX)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_CYT4DNxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K3XX)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32K3xxInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K2TV)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_S32K2TVInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMXRT117X)                                                                        /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_IMXRT117xInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_GENERIC_CORTEXM33)                                                                /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_Generic_CortexM33Int.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_GENERIC_CORTEXM33_MP)                                                             /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_Generic_CortexM33_MpInt.h"
# elif defined(OS_CFG_DERIVATIVEGROUP_SAF85XX)                                                                          /* COV_OS_DERIVATIVEGROUP */
#  include "Os_Hal_Derivative_SAF85xxInt.h"
# else
#  error "The selected derivative is not supported!"
# endif /* OS_CFG_DERIVATIVE_<> */

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_DERIVATIVEINT_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_DerivativeInt.h
 **********************************************************************************************************************/

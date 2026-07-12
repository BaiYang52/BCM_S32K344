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
 * \ingroup     Os_Hal
 * \defgroup    Os_Hal_Feature HAL Features
 * \brief       Primitive checks for feature switching
 * \details
 * This file defines macros and functions which are used to switch Os features on and off for internal testing purposes.
 *
 * \{
 * \file    Os_Hal_FeatureInt.h
 * \brief   Primitive checks for feature switching
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
#ifndef OS_HAL_FEATUREINT_H
# define OS_HAL_FEATUREINT_H

# include "Std_Types.h"

#ifdef OS_TST_NOLEVELSUPPORT                                                                                            /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_LEVELSUPPORT STD_OFF
#else
#  define OS_HAL_TST_LEVELSUPPORT STD_ON
#endif

#ifdef OS_TST_NOCLEARPENDINGINTERRUPTSUPPORT                                                                            /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_CLEARPENDINGINTERRUPTSUPPORT   STD_OFF
#else
#  define OS_HAL_TST_CLEARPENDINGINTERRUPTSUPPORT   STD_ON
#endif

#ifdef OS_TST_GLOBAL2LEVELSWITCHSUPPORT                                                                                 /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_GLOBAL2LEVELSWITCHSUPPORT STD_ON
#else
#  define OS_HAL_TST_GLOBAL2LEVELSWITCHSUPPORT STD_OFF
#endif

#ifdef OS_TST_NOINTERNALEXCEPTIONDETECTIONSUPPORT                                                                       /* COV_OS_HALVTTFEATURESUPPORT */
#  include "Os_Error_Cfg.h"
#  undef OS_CFG_INTERNAL_EXCEPTION_DETECTION
#  define OS_CFG_INTERNAL_EXCEPTION_DETECTION STD_OFF
#endif

#ifdef OS_TST_SETEXCEPTIONCONTEXTSUPPORT                                                                                /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_SETEXCEPTIONCONTEXTSUPPORT STD_ON
#else
#  define OS_HAL_TST_SETEXCEPTIONCONTEXTSUPPORT STD_OFF
#endif

#ifdef OS_TST_GETEXCEPTIONCONTEXTSUPPORT                                                                                /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_GETEXCEPTIONCONTEXTSUPPORT STD_ON
#else
#  define OS_HAL_TST_GETEXCEPTIONCONTEXTSUPPORT STD_OFF
#endif

#ifdef OS_TST_NOPERIPHERALINTERRUPTTRIGGERSUPPORT                                                                       /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_PERIPHERALINTERRUPTTRIGGERSUPPORT STD_OFF
#else
#  define OS_HAL_TST_PERIPHERALINTERRUPTTRIGGERSUPPORT STD_ON
#endif

#ifdef OS_TST_CLZINHWSUPPORT                                                                                            /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_CLZINHWSUPPORT STD_ON
#else
#  define OS_HAL_TST_CLZINHWSUPPORT STD_OFF
#endif

#ifdef OS_TST_NOPRIVILEGEDMODEUSERREADABLE                                                                              /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_PRIVILEGEDMODEUSERREADABLE STD_OFF
#else
#  define OS_HAL_TST_PRIVILEGEDMODEUSERREADABLE STD_ON
#endif

#ifdef OS_TST_NOTHREADPOINTERUSERREADABLE                                                                               /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_THREADPOINTERUSERREADABLE STD_OFF
#else
#  define OS_HAL_TST_THREADPOINTERUSERREADABLE STD_ON
#endif

#ifdef OS_TST_NOCOREIDUSERREADABLE                                                                                      /* COV_OS_HALVTTFEATURESUPPORT */
#  define OS_HAL_TST_COREIDUSERREADABLE STD_OFF
#else
#  define OS_HAL_TST_COREIDUSERREADABLE STD_ON
#endif


#define Os_Hal_IsFeatureActive(feature) (feature == STD_ON)

#define Os_Hal_IsFeatureInactive(feature) (feature == STD_OFF)



#endif /* OS_HAL_FEATUREINT_H */

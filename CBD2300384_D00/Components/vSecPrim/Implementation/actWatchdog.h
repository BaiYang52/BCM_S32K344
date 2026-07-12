/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actWatchdog.h
 *        \brief  Watchdog functionality.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#ifndef ACTWATCHDOG_H
# define ACTWATCHDOG_H

# include "actConfig.h"
# include "actUtilities.h"

/****************************************************************************
 ** Macros
 ***************************************************************************/
# if (actWD_LEVEL > 0)
#  define actTriggerWD(watchdog)                                      actWatchdogTriggerFunction(watchdog)
#  define actL1trigger(watchdog)                                      actTriggerWD(watchdog)
# else
#  define actTriggerWD(watchdog)                                      VSECPRIM_DUMMY_STATEMENT(watchdog)
#  define actL1trigger(watchdog)                                      VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

# if (actWD_LEVEL > 1)
#  define actL2trigger(watchdog)                                      actTriggerWD(watchdog)
# else
#  define actL2trigger(watchdog)                                      VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

# if (actWD_LEVEL > 2)
#  define actL3trigger(watchdog)                                      actTriggerWD(watchdog)
# else
#  define actL3trigger(watchdog)                                      VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

/* additional WD macros */
# if (actWD_LEVEL >= 1)
#  define ifWDisL1(condition, watchdog)                               if ((watchdog != NULL_PTR) && (condition)) (*watchdog)()
# else
#  define ifWDisL1(condition, watchdog)                               VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

# if (actWD_LEVEL >= 2)
#  define ifWDisL2(condition, watchdog)                               if ((watchdog != NULL_PTR) && (condition)) (*watchdog)()
# else
#  define ifWDisL2(condition, watchdog)                               VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

# if (actWD_LEVEL >= 3)
#  define ifWDisL3(condition, watchdog)                               if ((watchdog != NULL_PTR) && (condition)) (*watchdog)()
# else
#  define ifWDisL3(condition, watchdog)                               VSECPRIM_DUMMY_STATEMENT(watchdog)
# endif

#endif /* ACTWATCHDOG_H */

/**********************************************************************************************************************
 *  END OF FILE: actWatchdog.h
 *********************************************************************************************************************/

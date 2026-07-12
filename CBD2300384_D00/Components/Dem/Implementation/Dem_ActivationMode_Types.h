/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_ActivationMode
 *  \{
 *  \file       Dem_ActivationMode_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ACTIVATIONMODE_TYPES_H)
#define DEM_ACTIVATIONMODE_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*!
  * \defgroup  Dem_ActivationMode_WwhObdClass  Macros encoding WWHOBD DTC Classes
  * Macros encoding WWHOBD DTC Classes
  * \{
  */
# define DEM_ACTIVATIONMODE_NUMBER_DTCCLASS          5U  /*!< Number of WWH-OBD DTC classes */

# define DEM_ACTIVATIONMODE_DTCCLASS_NO              0U  /*!< No WWH-OBD DTC class */
# define DEM_ACTIVATIONMODE_DTCCLASS_C               1U  /*!< WWH-OBD DTC class C */
# define DEM_ACTIVATIONMODE_DTCCLASS_B2              2U  /*!< WWH-OBD DTC class B2 */
# define DEM_ACTIVATIONMODE_DTCCLASS_B1              3U  /*!< WWH-OBD DTC class B1 */
# define DEM_ACTIVATIONMODE_DTCCLASS_A               4U  /*!< WWH-OBD DTC class A */
  /*!
   * \}
   */

/*!
 * \defgroup  Dem_ActivationMode_B1Counter  Macros encoding B1 counter values
 * Macros encoding B1 counter values
 * \{
 */
#define DEM_ACTIVATIONMODE_B1_HEALING_COUNTER_THRESHOLD    (0x03U)    /*!< B1 healing counter threshold */
#define DEM_ACTIVATIONMODE_B1COUNTER_190H                  (0x2c88U)  /*!< B1 counter lower threshold after B1 deactivation */
#define DEM_ACTIVATIONMODE_B1COUNTER_200H                  (0x2ee0U)  /*!< B1 counter upper threshold for escalation */
#define DEM_ACTIVATIONMODE_B1COUNTER_LATCHED               (0x3bc4U)  /*!< B1 latched value */
/*!
 * \}
 */

/*!
 * \defgroup  Dem_ActivationMode_ContinuousMICounter  Macros encoding continuous MI counter values
 * Macros encoding continuous MI counter values
 * \{
 */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_200H              (0x2ee0U)  /*!< continuous MI counter threshold */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_LATCHED           (0x3bc4U)  /*!< continuous MI counter latched value */
/*!
 * \}
 */

/*!
 * \defgroup  Dem_ActivationMode_ContinuousMIHealing  Macros encoding special values for healing of continuous MI
 * Macros encoding special values for healing of continuous MI
 * \{
 */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_THRESHOLD  (0x03U)  /*!< continuous MI healing counter threshold */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_INVALID    (0xffU)  /*!< continuous MI healing counter invalid value (not healing) */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_LOCKED     (0xfeU)  /*!< continuous MI healing counter locked value */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_HEALING_COUNTER_HEALED     (0xfcU)  /*!< continuous MI healing counter healing completed value */
/*!
 * \}
 */

/*!
 * \defgroup  Dem_ActivationMode_ContinuousMIAging  Macros encoding special values for aging counter of continuous MI
 * Macros encoding special values for aging counter of continuous MI
 * \{
 */

#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_THRESHOLD  (0x28U)  /*!< continuous MI aging counter threshold */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_INVALID    (0xffU)  /*!< continuous MI aging counter invalid value (not aging) */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_COUNTER_AGED       (0xfcU)  /*!< continuous MI aging counter aging completed value */
/*!
 * \}
 */

/*!
 * \defgroup  Dem_ActivationMode_ContinuousMITimer  Macros encoding special values for aging timer of continuous MI
 * Macros encoding special values for aging timer of continuous MI
 * \{
 */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_INVALID     (0xffffU)  /*!< continuous MI aging timer invalid value (not aging) */
#define DEM_ACTIVATIONMODE_CONTINUOUS_MI_AGING_TIMER_AGED        (0xffccU)  /*!< continuous MI aging timer aging completed value */
/*!
 * \}
 */
#endif /* DEM_ACTIVATIONMODE_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ActivationMode_Types.h
 *********************************************************************************************************************/

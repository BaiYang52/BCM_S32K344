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
/*! \addtogroup Dem_Esm
 *  \{
 *  \file       Dem_Esm_Types.h
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

#if !defined (DEM_ESM_TYPES_H)
#define DEM_ESM_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*!
 * \defgroup  Dem_Esm_PermanentDtcStatus  Macros encoding status values for permanent DTCs
 * Macros encoding status values for permanent DTCs. Low byte is used for the state and the high byte is used for the visibility and the clear conditions
 * \{
 */
# define DEM_ESM_PERMANENT_STATES                (0x03U)  /*!< range of valid permanent states */
# define DEM_ESM_PERMANENT_STATEMASK             (0xfcU)  /*!< inverted DEM_ESM_PERMANENT_STATES */

# define DEM_ESM_PERMANENT_NONE                  (0x00U)  /*!< Permanent entry not used */
# define DEM_ESM_PERMANENT_ACTIVE                (0x01U)  /*!< Permanent entry active */
# define DEM_ESM_PERMANENT_PASSIVE               (0x02U)  /*!< Permanent entry not active */

# define DEM_ESM_PERMANENT_VISIBLE               (0x80U)  /*!< Permanent entry visible in mode 0xa */
# define DEM_ESM_PERMANENT_VISIBLEMASK           (0x7fU)  /*!< Mask to reset DEM_ESM_PERMANENT_VISIBLE */
# define DEM_ESM_PERMANENT_CONDITION_TESTED      (0x40U)  /*!< Permanent entry DTC tested this DCY */
# define DEM_ESM_PERMANENT_CONDITION_TESTEDMASK  (0xbfU)  /*!< Mask to reset DEM_ESM_PERMANENT_CONDITION_TESTED */
# define DEM_ESM_PERMANENT_CONDITION_PFC         (0x20U)  /*!< Permanent entry PFC condition fulfilled */
# define DEM_ESM_PERMANENT_CONDITION_PFCMASK     (0xdfU)  /*!< Mask to reset DEM_ESM_PERMANENT_CONDITION_PFC */
# define DEM_ESM_PERMANENT_CONDITION_DCY         (0x10U)  /*!< Permanent entry DCY qualified */
# define DEM_ESM_PERMANENT_CONDITION_DCYMASK     (0xefU)  /*!< Mask to reset DEM_ESM_PERMANENT_CONDITION_DCY */
/*!
 * \}
 */



/*! Aging timer 200 hour threshold value */
#define DEM_ESM_AGINGTIME_200h                   (0x00002ee0U)

#if ( (DEM_CFG_PROCESS_CDTC_STOREDONLY == STD_ON)   \
   || (DEM_CFG_PROCESS_PDTC_STOREDONLY == STD_ON)   \
   || (DEM_CFG_PROCESS_TFSLC_STOREDONLY == STD_ON) )
/*! Option setting if status bits depend on creation of an event memory entry */
# define DEM_FEATURE_NEED_STOREDONLY_BITS        STD_ON
#else
/*! Option setting if status bits depend on creation of an event memory entry */
# define DEM_FEATURE_NEED_STOREDONLY_BITS        STD_OFF
#endif

#if (DEM_CFG_PROCESS_PDTC_STOREDONLY == STD_ON)
/*! Mask for stored only Pending DTC trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_PENDING        DEM_CFG_TRIGGER_PENDING
#else
/*! Mask for stored only Pending DTC trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_PENDING        0x00U
#endif
#if (DEM_CFG_PROCESS_CDTC_STOREDONLY == STD_ON)
/*! Mask for stored only Confirmed DTC trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_CONFIRMED      DEM_CFG_TRIGGER_CONFIRMED
#else
/*! Mask for stored only Confirmed DTC trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_CONFIRMED      0x00U
#endif
#if (DEM_CFG_PROCESS_WIR_STOREDONLY == STD_ON)
/*! Mask for stored only Warning Indicator Requested trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_INDICATOR      DEM_CFG_TRIGGER_INDICATOR
#else
/*! Mask for stored only Warning Indicator Requested trigger */
# define DEM_ESM_EVENT_NO_TRIGGER_INDICATOR      0x00U
#endif


#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/*! Trigger Mask for event occurrence */
#define DEM_ESM_EVENT_OCCURRENCE_TRIGGER            (DEM_CFG_TRIGGER_TESTFAILED|DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE)
#else
/*! Trigger Mask for event updates */
#define DEM_ESM_EVENT_OCCURRENCE_TRIGGER             DEM_CFG_TRIGGER_TESTFAILED
#endif

/*! Trigger Mask for environmental data that could not be stored previously
    (due to missing event entry) and have a configured trigger included in TF transition*/
#define DEM_ESM_EVENT_CATCHUP_TRIGGER             (DEM_CFG_TRIGGER_TESTFAILED|DEM_CFG_TRIGGER_FDC|DEM_CFG_TRIGGER_FIRSTTHISCYCLE|DEM_CFG_TRIGGER_PENDING)

/*! Reset Mask for stored only triggers */
#define DEM_ESM_EVENT_RESET_TRIGGER_MASK         ((uint8)(0xffU ^ (DEM_ESM_EVENT_NO_TRIGGER_PENDING   \
                                                                 | DEM_ESM_EVENT_NO_TRIGGER_CONFIRMED \
                                                                 | DEM_ESM_EVENT_NO_TRIGGER_INDICATOR)))


/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* Permanent status bit 'event visible in permanent memory' ---------------- */
/*! Test permanent entry visible in permanent entry state */
# define DEM_ESM_TEST_PERMANENT_VISIBLE(Var)        (((Var) & DEM_ESM_PERMANENT_VISIBLE) != 0u)
/*! Set permanent entry visible in permanent entry state */
# define DEM_ESM_SET_PERMANENT_VISIBLE(Status)      ((uint8)((Status) | DEM_ESM_PERMANENT_VISIBLE))
/*! Reset permanent entry visible in permanent entry state */
# define DEM_ESM_RESET_PERMANENT_VISIBLE(Status)    ((uint8)((Status) & DEM_ESM_PERMANENT_VISIBLEMASK))

/* Permanent status bit 'tested condition' ---------------- */
/*! Test permanent condition tested in permanent entry state */
# define DEM_ESM_TEST_PERMANENT_CONDITION_TESTED(Var)        (((Var) & DEM_ESM_PERMANENT_CONDITION_TESTED) != 0u)
/*! Set permanent condition tested in permanent entry state */
# define DEM_ESM_SET_PERMANENT_CONDITION_TESTED(Status)      ((uint8)((Status) | DEM_ESM_PERMANENT_CONDITION_TESTED))
/*! Reset permanent condition tested in permanent entry state */
# define DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(Status)    ((uint8)((Status) & DEM_ESM_PERMANENT_CONDITION_TESTEDMASK))

/* Permanent status bit 'PFC condition' ---------------- */
/*! Test permanent condition PFC in permanent entry state */
# define DEM_ESM_TEST_PERMANENT_CONDITION_PFC(Var)           (((Var) & DEM_ESM_PERMANENT_CONDITION_PFC) != 0u)
/*! Set permanent condition PFC in permanent entry state */
# define DEM_ESM_SET_PERMANENT_CONDITION_PFC(Status)         ((uint8)((Status) | DEM_ESM_PERMANENT_CONDITION_PFC))
/*! Reset permanent condition PFC in permanent entry state */
# define DEM_ESM_RESET_PERMANENT_CONDITION_PFC(Status)       ((uint8)((Status) & DEM_ESM_PERMANENT_CONDITION_PFCMASK))

/* Permanent status bit 'DCY condition' ---------------- */
/*! Test permanent condition DCY in permanent entry state */
# define DEM_ESM_TEST_PERMANENT_CONDITION_DCY(Var)           (((Var) & DEM_ESM_PERMANENT_CONDITION_DCY) != 0u)
/*! Set permanent condition DCY in permanent entry state */
# define DEM_ESM_SET_PERMANENT_CONDITION_DCY(Status)         ((uint8)((Status) | DEM_ESM_PERMANENT_CONDITION_DCY))
/*! Reset permanent condition DCY in permanent entry state */
# define DEM_ESM_RESET_PERMANENT_CONDITION_DCY(Status)       ((uint8)((Status) & DEM_ESM_PERMANENT_CONDITION_DCYMASK))

/* Permanent entry current FSM status -------------------------------------- */
/*! Get permanent state part of permanent entry condition byte */
# define DEM_ESM_GET_PERMANENT_STATE(Status)        ((uint8)((Status) & DEM_ESM_PERMANENT_STATES))
/*! Set permanent state part of permanent entry condition byte */
# define DEM_ESM_SET_PERMANENT_STATE(Var, Status)   ((uint8)(((uint8)((Var) & DEM_ESM_PERMANENT_STATEMASK)) | ((uint8)((Status) & DEM_ESM_PERMANENT_STATES))))





/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* DEM_ESM_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Esm_Types.h
 *********************************************************************************************************************/

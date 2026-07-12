/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file   I2c_ConfigMgr.h
 *        \brief  I2c Configuration Manager
 *
 *      \details  This sub-module is responsible for providing access to the module's configuration structure.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to revision history in I2c.h.
 *********************************************************************************************************************/

#if !defined COMPONENTS_I2C_I2C_CONFIGMGR_H
# define COMPONENTS_I2C_I2C_CONFIGMGR_H

/*************************************************************************************************
 * INCLUDES
 *************************************************************************************************/
# include "I2c_Cfg.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (I2C_LOCAL) /* COV_I2C_LOCAL */
# define I2C_LOCAL static
#endif

#if !defined (I2C_LOCAL_INLINE) /* COV_I2C_LOCAL */
# define I2C_LOCAL_INLINE LOCAL_INLINE
#endif

/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetChannelDesc()
 *********************************************************************************************************************/
/*! \brief          This service returns a reference to the channel object within the
 *                  channelDescList at the channelId position.
 *  \details        none
 *  \param[in]      Channel ID
 *  \return         Reference to the channel descriptor object.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(I2c_ConstChannelDescPtrType, I2C_CODE) I2c_ConfigMgr_GetChannelDesc
(
  I2c_ChannelType channelId
);

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetHwUnitDesc()
 *********************************************************************************************************************/
/*! \brief          This service returns a reference to the hwUnit object within
 *                  the hwUnitDescList at the hwUnitId position.
 *  \details        none
 *  \param[in]      HwUnit ID
 *  \return         Reference to the HwUnit descriptor object.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(I2c_ConstHwUnitDescPtrType, I2C_CODE) I2c_ConfigMgr_GetHwUnitDesc
(
  I2c_HwUnitType hwUnitId
);

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetHwUnitDescViaSeq()
 *********************************************************************************************************************/
/*! \brief          This service returns a reference to the requested sequence's HwUnit descriptor object.
 *  \details        none
 *  \param[in]      Sequence ID
 *  \return         Reference to the HwUnit descriptor object.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(I2c_ConstHwUnitDescPtrType, I2C_CODE) I2c_ConfigMgr_GetHwUnitDescViaSeq
(
  I2c_SequenceType sequenceId
);

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetSeqDesc()
 *********************************************************************************************************************/
/*! \brief          This service returns a reference to the requested sequence descriptor object.
 *  \details        none
 *  \param[in]      Sequence ID
 *  \return         Reference to the sequence descriptor object.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(I2c_ConstSeqDescPtrType, I2C_CODE) I2c_ConfigMgr_GetSeqDesc
(
  I2c_SequenceType sequenceId
);

/*************************************************************************************************
 * GLOBAL FUNCTIONS
 *************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetChannelDesc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL_INLINE FUNC(I2c_ConstChannelDescPtrType, I2C_CODE) I2c_ConfigMgr_GetChannelDesc /* PRQA S 3219 */ /* MD_I2C_3219 */
(
  I2c_ChannelType channelId
)
{
  I2c_ConstChannelDescPtrType retVal = NULL_PTR;

  /* #10 Check if the requested channel ID is valid and return a reference to the requested channel descriptor object. */
  if(channelId < I2c_ConfigPtr->NumOfCh)
  {
    retVal = &I2c_ConfigPtr->ChannelCfgPtr[channelId];
  }

  return retVal;
} /* I2c_ConfigMgr_GetChannelDesc */

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetHwUnitDesc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL_INLINE FUNC(I2c_ConstHwUnitDescPtrType, I2C_CODE) I2c_ConfigMgr_GetHwUnitDesc /* PRQA S 3219 */ /* MD_I2C_3219 */
(
  I2c_HwUnitType hwUnitId
)
{
  I2c_ConstHwUnitDescPtrType retVal = NULL_PTR;

  /* #10 Check if the requested HwUnit ID is valid and return a reference to the requested hwUnit descriptor object. */
  if(hwUnitId < I2c_ConfigPtr->NumOfHwUnits)
  {
    retVal = &I2c_ConfigPtr->HwCfgPtr[hwUnitId];
  }

  return retVal;
} /* I2c_ConfigMgr_GetHwUnitDesc */

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetHwUnitDescViaSeq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL_INLINE FUNC(I2c_ConstHwUnitDescPtrType, I2C_CODE) I2c_ConfigMgr_GetHwUnitDescViaSeq /* PRQA S 3219 */ /* MD_I2C_3219 */
(
  I2c_SequenceType sequenceId
)
{
  I2c_ConstHwUnitDescPtrType retVal = NULL_PTR;

  /*
   * #10 Check if the requested sequence ID is valid and return a reference to the
   * requested sequence's corresponding hwUnit descriptor element.
   */
  if(sequenceId < I2c_ConfigPtr->NumOfSeq)
  {
    retVal = I2c_ConfigPtr->SequenceCfgPtr[sequenceId].BusCfgPtr->HwCfgPtr;
  }

  return retVal;
} /* I2c_ConfigMgr_GetHwUnitDescViaSeq */

/**********************************************************************************************************************
 *  I2c_ConfigMgr_GetSeqDesc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL_INLINE FUNC(I2c_ConstSeqDescPtrType, I2C_CODE) I2c_ConfigMgr_GetSeqDesc
(
  I2c_SequenceType sequenceId
)
{
  I2c_ConstSeqDescPtrType retVal = NULL_PTR;

  /* #10 Check if the requested sequence ID is valid and return a reference to the requested sequence descriptor element. */
  if(sequenceId < I2c_ConfigPtr->NumOfSeq)
  {
    retVal = &I2c_ConfigPtr->SequenceCfgPtr[sequenceId];
  }

  return retVal;
} /* I2c_ConfigMgr_GetSeqDesc */

#endif /* COMPONENTS_I2C_I2C_CONFIGMGR_H_ */

/*********************************************************************************************************************
 *  END OF FILE: I2c_ConfigMgr.h
 *********************************************************************************************************************/

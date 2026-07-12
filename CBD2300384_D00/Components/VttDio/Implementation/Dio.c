/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dio.h
 *      Project:  DrvDio_VttCanoe01Asr
 *       Module:  MICROSAR Dio
 *    Generator:  -
 *
 *  Description:  The DIO Driver provides services for reading and writing to/from
 *                - DIO Channels (Pins)
 *                - DIO Ports
 *                - DIO Channel Groups
 *                In the emulated MCAL, the channels, channel groups and ports are simulated by CANoe environment
 *                variables.
 *
 **********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#define DIO_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Dio.h"
#if (DIO_DEV_ERROR_REPORT == STD_ON)
#include "Det.h"
#endif
/**********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
#if (  (DIO_SW_MAJOR_VERSION != (0x02u)) \
    || (DIO_SW_MINOR_VERSION != (0x05u)) \
    || (DIO_AR_RELEASE_MAJOR_VERSION != (4u)) \
    || (DIO_AR_RELEASE_MINOR_VERSION != (0u)) \
    || (DIO_AR_RELEASE_PATCH_VERSION != (3u)) )
  #error "Version numbers of Dio.c and Dio.h are inconsistent!"
#endif

#if (  (DIO_CFG_MAJOR_VERSION != (0x04u)) \
    || (DIO_CFG_MINOR_VERSION != (0x03u)) )
  #error "Version numbers of Dio.c and Dio_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
#if !defined (DIO_LOCAL) /* COV_VMEMACCM_COMPATIBILITY */
# define DIO_LOCAL static
#endif
 
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
/* CurrentConfigPtr keeps the reference to the post-build configuration set, which is passed to function Dio_Init() */
typedef P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) CurrentConfigConstPtrType;
DIO_LOCAL VAR(CurrentConfigConstPtrType, DIO_VAR) CurrentConfigPtr;

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  Dio_InitMemory
 **********************************************************************************************************************/
/*! \brief      This service initializes the global variables in case the startup code does not work
 *  \param[in]  void
 *  \return     void
 *  \pre        Module must not be initialized
 *  \context    Function is called during startup
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_InitMemory (void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Reset configuration pointer. All other variables are reset in Dio_Init() */
  CurrentConfigPtr = NULL_PTR;
  
  return;
}

/**********************************************************************************************************************
 *  Dio_Init
 **********************************************************************************************************************/
/*! \brief      This service initialize the DIO component.
 *  \param[in]  ConfigPtr               Pointer to the used configuration struct
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_Init_Vtt (P2CONST(Dio_ConfigType, AUTOMATIC, DIO_APPL_CONST) ConfigPtr)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* Check the ChannelId    */
  if (ConfigPtr == NULL_PTR)
  {
    errorId = DIO_E_PARAM_CONFIG; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    CurrentConfigPtr = ConfigPtr;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_INIT, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* Dio_Init_Vtt() */


/**********************************************************************************************************************
 *  Dio_ReadChannel
 **********************************************************************************************************************/
/*! \brief      This service reads and returns the level of a single DIO channel.
 *  \param[in]  ChannelId               ID of DIO channel
 *  \return     Dio_LevelType           The physical level of the corresponding Pin
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_LevelType, DIO_CODE) Dio_ReadChannel (Dio_ChannelType ChannelId)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */
  Dio_LevelType retVal;

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  retVal = 0u;
  /* Check the ChannelId    */
  if (DIO_MAX_CHANNEL_COUNT <= ChannelId)
  {
    errorId = DIO_E_PARAM_INVALID_CHANNEL_ID; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = (Dio_LevelType)VttDio_ReadChannel(ChannelId);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_READ_CHANNEL, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* Dio_ReadChannel() */



/**********************************************************************************************************************
 *  Dio_WriteChannel
 **********************************************************************************************************************/
/*! \brief      This service sets the level of a single DIO channel to STD_HIGH or STD_LOW.
 *  \param[in]  ChannelId               ID of DIO channel
 *  \param[in]  Level                   Level to be set
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level )
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* Check the ChannelId    */
  if (DIO_MAX_CHANNEL_COUNT <= ChannelId)
  {
    errorId = DIO_E_PARAM_INVALID_CHANNEL_ID; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Set the variable in CANoe */
    VttDio_WriteChannel(ChannelId, (uint32)(Level) & 0x1u);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_WRITE_CHANNEL, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* Dio_WriteChannel() */


/**********************************************************************************************************************
 *  Dio_ReadPort
 **********************************************************************************************************************/
/*! \brief      This servics returns the level of all channels of a port.
 *  \param[in]  PortId                  ID of DIO Port
 *  \return     Dio_PortLevelType       Returns the level of all channels
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadPort (Dio_PortType PortId)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */
  Dio_PortLevelType retVal;

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  retVal = 0u;
  /* Check the PortId */
  if (DIO_MAX_PORT_COUNT <= PortId)
  {
    errorId = DIO_E_PARAM_INVALID_PORT_ID; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Read the variable in CANoe */
    retVal = VttDio_ReadPort(PortId);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_READ_PORT, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* Dio_ReadPort() */



/**********************************************************************************************************************
 *  Dio_WritePort
 **********************************************************************************************************************/
/*! \brief      This service sets the level of a DIO port to STD_HIGH or STD_LOW.
 *  \param[in]  PortId                  ID of DIO Port
 *  \param[in]  Level                   Value to be written
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* Check the PortId */
  if (DIO_MAX_PORT_COUNT <= PortId)
  {
    errorId = DIO_E_PARAM_INVALID_PORT_ID; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Write the content of Level into the CANoe value  */
    VttDio_WritePort(PortId, Level);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_WRITE_PORT, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* Dio_WritePort() */


/**********************************************************************************************************************
 *  Dio_ReadChannelGroup
 **********************************************************************************************************************/
/*! \brief      This servics returns the level of all channels of a ChannelGroup.
 *  \param[in]  ChannelGroupId          Input parameter
 *  \return     Dio_PortLevelType       Returns the LSB aligned port value.
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_PortLevelType, DIO_CODE) Dio_ReadChannelGroup
(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId /* PRQA S 3206 */ /* MD_Dio_ParamNotUsed */
)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */
  Dio_PortLevelType retVal;
#if (DIO_MAX_CHANNELGROUP_COUNT != 0)
  uint32 Index = 0u;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  retVal = 0u;
  /* Check the ChannelId    */
  if (NULL_PTR == ChannelGroupId)
  {
    errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
#if (DIO_MAX_CHANNELGROUP_COUNT != 0)
    /* Search for the correct Index of ChannelGroupRefs */
    while ((Index < DIO_MAX_CHANNELGROUP_COUNT) && (Dio_ChannelGroupArray[Index] != ChannelGroupId))
    {
      Index++;
    }

    /* Check the Index of the ChannelGroupRefs */
    if (Index >= DIO_MAX_CHANNELGROUP_COUNT)
    {
      retVal = 0u;
      errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
    }
    else
    {
      retVal = (Dio_PortLevelType)VttDio_ReadChannelGroup((sint32)Index);

      retVal &= (Dio_ChannelGroupArray[Index]->mask);
      /* Shift the result to the LSB by offset */
      retVal >>= (Dio_ChannelGroupArray[Index]->offset);
    }
#else
    /* Set return value to a defined state */
    retVal = 0u;

    errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR) /* PRQA S 2991, 2995 */ /* MD_Dio_APIPattern */
  {
    Dio_ReportDet(DIO_SID_READ_CHL_GROUP, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* Dio_ReadChannelGroup() */


/**********************************************************************************************************************
 *  Dio_WriteChannelGroup
 **********************************************************************************************************************/
/*! \brief      This servics sets the level of a ChannelGroup.
 *  \param[in]  ChannelGroupId          Pointer to a ChannelGroup
 *  \param[in]  Level                   Value to be written
 *  \return     void
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_WriteChannelGroup
(
  P2CONST(Dio_ChannelGroupType, AUTOMATIC, DIO_APPL_CONST) ChannelGroupId, /* PRQA S 3206 */ /* MD_Dio_ParamNotUsed */
  Dio_PortLevelType Level /* PRQA S 3206 */ /* MD_Dio_ParamNotUsed */
)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */
#if (DIO_MAX_CHANNELGROUP_COUNT != 0)
  Dio_PortLevelType tempVal;
  uint32 Index = 0u;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* Check the ChannelId    */
  if (NULL_PTR == ChannelGroupId)
  {
    errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
#if (DIO_MAX_CHANNELGROUP_COUNT != 0)
    /* ----- Implementation ----------------------------------------------- */
    /* Search for the correct Index of ChannelGroupRefs */
    while ((Index < DIO_MAX_CHANNELGROUP_COUNT) && (Dio_ChannelGroupArray[Index] != ChannelGroupId))
    {
      Index++;
    }

    /* Check the Index of the ChannelGroupRefs */
    if (Index >= DIO_MAX_CHANNELGROUP_COUNT)
    {
      errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
    }
    else
    {
      /* Move the "Level" to the postition of the channelgroup */
      tempVal = (Level << Dio_ChannelGroupArray[Index]->offset);

      /* Mask out all bits which are not configured */
      tempVal &= Dio_ChannelGroupArray[Index]->mask;

      VttDio_WriteChannelGroup((sint32)Index, tempVal);
    }
#else
    /* Parameter Level is not used if no channel group is configured */
    DIO_DUMMY_STATEMENT(Level);
    errorId = DIO_E_PARAM_INVALID_GROUP; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR) /* PRQA S 2991, 2995 */ /* MD_Dio_APIPattern */
  {
    Dio_ReportDet(DIO_SID_WRITE_CHL_GROUP, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* Dio_WriteChannelGroup() */

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**********************************************************************************************************************
 *  Dio_FlipChannel
 **********************************************************************************************************************/
/*! \brief      This servics inverts the status of a channel
 *  \param[in]  ChannelGroupId          Pointer to a ChannelGroup
 *  \return     Dio_LevelType            The level to which the channel was set
 *  \context    Function could be called from interrupt or task level.
 *  \note
 **********************************************************************************************************************/
FUNC(Dio_LevelType, DIO_CODE) Dio_FlipChannel (Dio_ChannelType ChannelId)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */
  Dio_LevelType retVal;

  /* ----- Development Error Checks ------------------------------------- */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  retVal = STD_LOW;
  /* Check the ChannelId    */
  if (DIO_MAX_CHANNEL_COUNT <= ChannelId)
  {
    errorId = DIO_E_PARAM_INVALID_CHANNEL_ID; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
#endif  /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = Dio_ReadChannel(ChannelId);
    if (retVal == STD_HIGH)
    {
      retVal = STD_LOW;
    }
    else
    {
      retVal = STD_HIGH;
    }
    Dio_WriteChannel(ChannelId, retVal);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_FLIP_CHANNEL, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
}
#endif /* DIO_FLIP_CHANNEL_API */


#if (DIO_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dio_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      This service returns the version information of this module.
 *  \param[out] versioninfo           Pointer to where to store the version information of this module.
 *  \return     void
 *  \context    Function is called from task level
 *  \note
 **********************************************************************************************************************/
FUNC(void, DIO_CODE) Dio_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DIO_APPL_DATA) versioninfo)
{
  /* ----- Function Local Variables ------------------------------------- */
  uint8 errorId = DIO_E_NO_ERROR; /* PRQA S 2981, 3205 */ /* MD_Dio_APIPattern */

  /* ----- Development Error Checks ------------------------------------- */
# if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* Check the versioninfo pointer for being a NULL_PTR */
  if (versioninfo == NULL_PTR)
  {
    errorId = DIO_E_PARAM_VINFO; /* PRQA S 2983 */ /* MD_Dio_APIPattern */
  }
  else
# endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (DIO_VENDOR_ID);
    versioninfo->moduleID = (DIO_MODULE_ID);
    versioninfo->sw_major_version = (DIO_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (DIO_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (DIO_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DIO_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DIO_E_NO_ERROR)
  {
    Dio_ReportDet(DIO_SID_GET_VERSION_INFO, errorId);
  }
# else
  DIO_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return;
} /* Dio_GetVersionInfo() */
#endif  /* (DIO_GET_VERSION_INFO_API == STD_ON) */


/* Justification for module-specific MISRA deviations:

  MD_Dio_APIPattern: rule 2.2, 2.3, 14.3
    Reason:     Coding rules requires the "API pattern" implementation, which leads to this MISRA issue: redundant
                assignment and initialization. Unused errorId will be solved by DUMMY_STATEMENT. The API pattern also
                requires single point of return - with disabled parameter checks the return value variable will never
                be used before modified.
    Risk:       None
    Prevention: None

  MD_Dio_ParamNotUsed: rule 2.7
    Reason:     If no channel group is configured the parameter Level is not used. No channel is set with the given
                value.
    Risk:       None
    Prevention: None

*/

/**********************************************************************************************************************
 *  END OF FILE: Dio.h
 **********************************************************************************************************************/

/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file    I2c.h
 *        \brief    I2c header file
 *
 *      \details    The I2c driver is responsible for managing the communication within a bus network. It is capable
 *                  of managing the communication within a multi-master network and can handle communication issues
 *                  like de-synchronisation.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                        Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Lukas Zirngibl              virzlu        Vector Informatik GmbH
 *  Matthias Scheid             virmid        Vector Informatik GmbH
 *  Daniel Richter              virrdl        Vector Informatik GmbH
 *  Annkathrin Bauer            virnba        Vector Informatik GmbH
 *  Christina Oberpriller       coberpriller  Vector Informatik GmbH
 *  Wolfram Schiessl            virwsl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date              Author        Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2018-01-30        virzlu        ESCAN00097546   Development according to safeBSW process.
 *                              virmid
 *  04.00.01  2018-06-29        virmid        STORYC-5453     Use CORE_VERSION macros for Version check.
 *                                            STORYC-5631     Updated MemMap sections.
 *            2018-07-12                      ESCAN00100005   I2c remains stuck in endless "BUSY loop".
 *  04.00.02  2019-02-28        virrdl        ESCAN00102243   Potential race condition during asynchronous data transmission.
 *  04.00.03  2019-10-24        virzlu        CTM-792         Trivial change: Add missing comment.
 *  04.00.04  2019-11-18        virmid        CTM-600         Reach QM state 'ASIL D' for DrvI2c__coreAsr.
 *  04.00.05  2020-03-09        virnba        ESCAN00104634   Sequences with status PENDING can be queued.
 *  04.00.06  2020-03-18        virnba        ESCAN00105885   Sequence with status PENDING can be queued but the state does not say it.
 *  04.01.00  2020-06-23        coberpriller  CTM-1727        Improved structure of generated files.
 *  04.01.01  2020-07-07        virwsl        CTM-1798        MISRA upgrade to Helix QAC.
 *********************************************************************************************************************/
#if !defined I2C_H
# define I2C_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "I2c_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define I2C_VENDOR_ID                          (30u)
# define I2C_MODULE_ID                          (84u)

/* AUTOSAR Software specification version information */
# define I2C_AR_RELEASE_MAJOR_VERSION           (4u)
# define I2C_AR_RELEASE_MINOR_VERSION           (0u)
# define I2C_AR_RELEASE_REVISION_VERSION        (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define I2C_CORE_MAJOR_VERSION                 (4u)
# define I2C_CORE_MINOR_VERSION                 (1u)
# define I2C_CORE_PATCH_VERSION                 (1u)

# define I2C_INSTANCE_ID_DET                    (0x00u)
# define I2C_10_BIT_ADDRESS                     (0x3FFu)

/* ----- API service IDs ----- */
# define I2C_SID_INIT                           (0x00u) /* Service ID: I2c_Init() */
# define I2C_SID_DEINIT                         (0x01u) /* Service ID: I2c_DeInit() */
# define I2C_SID_GET_VERSION_INFO               (0x02u) /* Service ID: I2c_GetVersionInfo() */
# define I2C_SID_SETUP_EB                       (0x03u) /* Service ID: I2c_SetupEB() */
# define I2C_SID_SETUP_EB_DYNAMIC               (0x04u) /* Service ID: I2c_SetupDynamicEB() */
# define I2C_SID_ASYNC_TRANSMIT                 (0x05u) /* Service ID: I2c_AsyncTransmit() */
# define I2C_SID_CANCEL                         (0x06u) /* Service ID: I2c_Cancel() */
# define I2C_SID_GET_RESULT                     (0x07u) /* Service ID: I2c_GetResult() */
# define I2C_SID_GET_SEQUENCE_RESULT            (0x08u) /* Service ID: I2c_GetSequenceResult() */
# define I2C_SID_MAIN_FUNCTION                  (0x09u) /* Service ID: I2c_MainFunction() */

/* ----- Error codes ----- */
# define I2C_E_NO_ERROR                         (0x00u)
# define I2C_E_UNINIT                           (0x01u)
# define I2C_E_ALREADY_INITIALIZED              (0x02u)
# define I2C_E_BUSY                             (0x03u)
# define I2C_E_PARAM_CHANNEL                    (0x04u)
# define I2C_E_PARAM_SEQUENCE                   (0x05u)
# define I2C_E_PARAM_LENGTH                     (0x06u)
# define I2C_E_PARAM_VINFO_PTR                  (0x07u)
# define I2C_E_PARAM_CONFIG                     (0x08u)
# define I2C_E_PARAM_TRANS_BUFFER               (0x09u)
# define I2C_E_PARAM_ADDRESS                    (0x0Au)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! critical section macros */
# define I2c_EnterCritical(Section)              SchM_Enter_I2c_##Section() /* PRQA S 0342 */ /* MD_I2C_342 */
# define I2c_ExitCritical(Section)               SchM_Exit_I2c_##Section() /* PRQA S 0342 */ /* MD_I2C_342 */


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * I2c_Init()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        This function initializes the module I2c according to the configuration parameters defined within
 *                  the ConfigPtr. It initializes all variables and sets the module state to initialized.
 *                  This service can also be used for the purpose of re-initialization, but only after the module's De-Init()
 *                  has been called.
 *  \param[in]      Config Pointer
 *  \pre            Module is uninitialized.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_Init
(
  I2c_ConfigPtrType configPtr
);

/**********************************************************************************************************************
 *  I2c_DeInit()
 *********************************************************************************************************************/
/*! \brief          De-initialization function.
 *  \details        This function de-initializes the module I2c. During de-initialization the module is shut down.
 *                  The module is put into its power-on reset state.
 *  \param[in]      void
 *  \return         E_OK: De-initialization was successful.
 *  \return         E_NOT_OK: De-initialization was not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_DeInit(void);

# if (I2C_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief          Returns the version information
 *  \details        I2c_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]     versioninfo            - Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-115822
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_GetVersionInfo
(
  I2c_VersionInfoPtrType versionInfoPtr
);
# endif

/**********************************************************************************************************************
 *  I2c_SetupEB()
 *********************************************************************************************************************/
/*! \brief          Setup external buffer.
 *  \details        The service takes over the given parameters, and prepares a channel for transmission (transmit buffer)
 *                  and reception (receive buffer).
 *  \param[in]      Channel ID
 *  \param[in]      Transmit data buffer pointer        - Pointer to a buffer containing data for transmission.
 *  \param[in]      Length                              - Number of bytes which shall be transmit/received.
 *  \param[out]     Receive data buffer pointer         - Pointer to a buffer provided for receiving data.
 *  \return         E_OK                                - Setup has been successful.
 *  \return         E_NOT_OK                            - Setup has been unsuccessful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_SetupEB
(
  I2c_ChannelType channelId,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
);

/**********************************************************************************************************************
 *  I2c_SetupEBDynamic()
 *********************************************************************************************************************/
/*! \brief          Dynamically setup external buffer.
 *  \details        The service takes over the given parameters, and prepares a channel for transmission (transmit buffer)
 *                  and reception (receive buffer). This service does not use the channel's configured address for
 *                  transmission/reception, but the given address parameter.
 *  \param[in]      Channel ID
 *  \param[in]      Address                             - Address given by the service's caller,
 *                                                        used for the transmission/reception.
 *  \param[in]      Transmit data buffer pointer        - Pointer to a buffer containing data for transmission.
 *  \param[in]      Length                              - Number of bytes which shall be transmit/received.
 *  \param[out]     Receive data buffer pointer         - Pointer to a buffer provided for receiving data.
 *  \return         E_OK                                - Setup has been successful.
 *  \return         E_NOT_OK                            - Setup has been unsuccessful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_SetupEBDynamic
(
  I2c_ChannelType channelId,
  I2c_AddressType address,
  I2c_DataConstPtrType txDataBufferPtr,
  I2c_DataPtrType rxDataBufferPtr,
  I2c_DataLengthType length
);

/**********************************************************************************************************************
 *  I2c_AsyncTransmit()
 *********************************************************************************************************************/
/*! \brief          Asynchronous data transmission.
 *  \details        This service executes a data exchange with the prepared channels. Only channels of the
 *                  given sequence are affected of the data transmission.
 *  \param[in]      Sequence ID
 *  \return         E_OK:         - The request to transmit the sequence has been accepted.
 *  \return         E_NOT_OK:     - The request to transmit the sequence has been declined.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_AsyncTransmit
(
  I2c_SequenceType sequenceId
);

#if (I2C_CANCEL_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_Cancel()
 *********************************************************************************************************************/
/*! \brief          Cancel a transmission.
 *  \details        A request that is already in transmission progress, is cancelled by finishing the current sequence.
 *                  This is done to achieve a stable state with no undefined data.
 *                  A queued transmission request is cancelled at once.
 *  \param[in]      Sequence ID
 *  \return         E_OK          - The request to cancel the sequence transmission has been accepted.
 *  \return         E_NOT_OK      - The request to cancel the sequence transmission has been declined.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_Cancel
(
  I2c_SequenceType sequenceId
);
#endif

/**********************************************************************************************************************
 *  I2c_SetHandling()
 *********************************************************************************************************************/
/*! \brief          Set the timeout handling.
 *  \details        This service sets the I2c module's timeout handling.
 *  \param[in]      handling      - BURST_MODE: In case of an timeout the I2c module does not perform any further actions,
 *                                  but detaches from the bus.
 *                                - RECURRENT_MODE: In case of an timeout the I2c module sends nine clock pulses to the
 *                                  slave in order to synchronize the bus again.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_SetHandling
(
  I2c_HandlingType handling
);

/**********************************************************************************************************************
 *  I2c_GetResult()
 *********************************************************************************************************************/
/*! \brief          Returns the requested channel's result.
 *  \details        -
 *  \param[in]      Channel ID
 *  \return         Channel result in 2 Bytes.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(I2c_ChannelResultType, I2C_CODE) I2c_GetResult
(
  I2c_ChannelType channelId
);

/**********************************************************************************************************************
 *  I2c_GetSequenceResult()
 *********************************************************************************************************************/
/*! \brief          Returns the requested sequence's result.
 *  \details        -
 *  \param[in]      Sequence ID
 *  \return         Sequence result
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(I2c_SeqResultType, I2C_CODE) I2c_GetSequenceResult
(
  I2c_SequenceType sequenceId
);

#if (I2C_GET_STATUS_API == STD_ON)
/**********************************************************************************************************************
 *  I2c_GetStatus()
 *********************************************************************************************************************/
/*! \brief          Returns the module's status.
 *  \details        -
 *  \param[in]      void
 *  \return         Module status
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-115819
 *********************************************************************************************************************/
FUNC(I2c_ComponentStatusType, I2C_CODE) I2c_GetStatus(void);
#endif

/**********************************************************************************************************************
 *  I2c_MainFunction()
 *********************************************************************************************************************/
/*! \brief          The components MainFunction is responsible for processing the sequence queue and for
 *                  updating the components status.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_MainFunction(void);

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */


/*!
*\exclusivearea I2C_EXCLUSIVE_AREA_0
* Ensures consistency during asynchronous transmission.
* \protects I2c_TransProcessor_ProcessHwUnit
* \usedin I2c_AsyncTransmit
* \exclude I2c_MainFunction
* \length LONG This exclusive area covers calls to several sub-functions.
*\endexclusivearea
*/

#endif /* I2C_H */

/**********************************************************************************************************************
 *  END OF FILE: I2c.h
 *********************************************************************************************************************/

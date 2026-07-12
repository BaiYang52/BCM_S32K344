/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_Vtt.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Implementation of the MICROSAR Crypto Driver (Crypto)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials       Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Markus Schneider              vismss         Vector Informatik GmbH
 *  Christian Leder               virchl         Vector Informatik GmbH
 *  Tobias Finke                  vistof         Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-10-31  virchl                Initial creation for Vtt (cloned from Crypto_30_Vtt)
 *  01.00.01  2018-01-04  vistof  ESCAN00097872 Fixed Compiler error: Inclue file SchM_Crypto_30_Vtt.h is missing
 *  01.00.02  2018-03-05  virchl  ESCAN00097872 Fixed Compiler error: Inclue file SchM_Crypto_30_Vtt.h is missing
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_VTT_H)
# define CRYPTO_30_VTT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Csm_Types.h"
# include "Crypto_30_Vtt_Cfg.h"
# include "Crypto_30_Vtt_KeyManagement.h"
# include "SchM_VttCrypto_30_Vtt.h"

# include "Crypto_30_Vtt_Custom.h"
# include "VttCntrl_Crypto.h"

# if (CRYPTO_30_VTT_USE_VSTD_LIB == STD_ON)
#  include "vstdlib.h"
# endif
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CRYPTO_30_VTT_VENDOR_ID                      (30u)
# define CRYPTO_30_VTT_MODULE_ID                      (114u)
# define CRYPTO_30_VTT_INSTANCE_ID                    (0u)

/* AUTOSAR Software specification version information */
# define CRYPTO_30_VTT_AR_RELEASE_MAJOR_VERSION       (0x04u)
# define CRYPTO_30_VTT_AR_RELEASE_MINOR_VERSION       (0x03u)
# define CRYPTO_30_VTT_AR_RELEASE_REVISION_VERSION    (0x00u)

/* Component version information (decimal version of ALM implementation package) */
# define CRYPTO_30_VTT_MAJOR_VERSION                  (1u)
# define CRYPTO_30_VTT_MINOR_VERSION                  (0u)
# define CRYPTO_30_VTT_PATCH_VERSION                  (2u)

/* API service IDs */
# define CRYPTO_30_VTT_SID_INIT                       (0x00u) /*!< Service ID: Crypto_30_Vtt_Init() */
# define CRYPTO_30_VTT_SID_GET_VERSION_INFO           (0x01u) /*!< Service ID: Crypto_30_Vtt_GetVersionInfo() */
# define CRYPTO_30_VTT_SID_PROCESS_JOB                (0x03u) /*!< Service ID: Crypto_30_Vtt_ProcessJob() */
# define CRYPTO_30_VTT_SID_CANCEL_JOB                 (0x0Eu) /*!< Service ID: Crypto_30_Vtt_CancelJob() */
# define CRYPTO_30_VTT_SID_KEY_ELEMENT_SET            (0x04u) /*!< Service ID: Crypto_30_Vtt_KeyElementSet() */
# define CRYPTO_30_VTT_SID_KEY_VALID_SET              (0x05u) /*!< Service ID: Crypto_30_Vtt_KeyValidSet() */
# define CRYPTO_30_VTT_SID_KEY_ELEMENT_GET            (0x06u) /*!< Service ID: Crypto_30_Vtt_KeyElementGet() */
# define CRYPTO_30_VTT_SID_KEY_ELEMENT_COPY           (0x0Fu) /*!< Service ID: Crypto_30_Vtt_KeyElementCopy() */
# define CRYPTO_30_VTT_SID_KEY_COPY                   (0x10u) /*!< Service ID: Crypto_30_Vtt_KeyCopy() */
# define CRYPTO_30_VTT_SID_KEY_ELEMENT_IDS_GET        (0x11u) /*!< Service ID: Crypto_30_Vtt_KeyElementIdsGet() */
# define CRYPTO_30_VTT_SID_RANDOM_SEED                (0x0Du) /*!< Service ID: Crypto_30_Vtt_RandomSeed() */
# define CRYPTO_30_VTT_SID_KEY_GENERATE               (0x07u) /*!< Service ID: Crypto_30_Vtt_KeyGenerate() */
# define CRYPTO_30_VTT_SID_KEY_DERIVE                 (0x08u) /*!< Service ID: Crypto_30_Vtt_KeyDerive() */
# define CRYPTO_30_VTT_SID_KEY_EXCHANGE_CALC_PUB_VAL  (0x09u) /*!< Service ID: Crypto_30_Vtt_KeyExchangeCalcPubVal() */
# define CRYPTO_30_VTT_SID_KEY_EXCHANGE_CALC_SECRET   (0x0Au) /*!< Service ID: Crypto_30_Vtt_KeyExchangeCalcSecret() */
# define CRYPTO_30_VTT_SID_CERTIFICATE_PARSE          (0x0Bu) /*!< Service ID: Crypto_30_Vtt_CertificateParse() */
# define CRYPTO_30_VTT_SID_CERTIFICATE_VERIFY         (0x12u) /*!< Service ID: Crypto_30_Vtt_CertificateVerify() */
# define CRYPTO_30_VTT_SID_MAIN_FUNCTION              (0x0Cu) /*!< Service ID: Crypto_30_Vtt_MainFunction() */

/* Modes */
/* State: Uninitialized */
# define CRYPTO_30_VTT_UNINIT                         (0x00u)
/* State: Initialized */
# define CRYPTO_30_VTT_INITIALIZED                    (0x01u)

/* Development Error Types [SWS_Crypto_00040] */
# ifndef CRYPTO_E_NO_ERROR /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_NO_ERROR       (255u)
# endif
# ifndef CRYPTO_E_UNINIT /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_UNINIT         (0u)
# endif
# ifndef CRYPTO_E_INIT_FAILED /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_INIT_FAILED    (1u)
# endif
# ifndef CRYPTO_E_PARAM_POINTER /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_PARAM_POINTER  (2u)
# endif
# ifndef CRYPTO_E_PARAM_HANDLE /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_PARAM_HANDLE   (4u)
# endif
# ifndef CRYPTO_E_PARAM_VALUE /* COV_CRYPTO_30_VTT_CSM_DEFINES */
#  define CRYPTO_E_PARAM_VALUE    (5u)
# endif

/* Data Services */
# if (CRYPTO_30_VTT_USE_VSTD_LIB == STD_ON)
/* use VStdLib */
#  define Crypto_30_Vtt_CopyData(destinationPtr,sourcePtr,length)   (VStdLib_MemCpy((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(destinationPtr), (P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(sourcePtr), length)) /* PRQA S 3453 */ /* MD_CRYPTO_30_VTT_19.7 */
#  define Crypto_30_Vtt_ClearData(dataPtr,length)                   (VStdLib_MemClr((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(dataPtr), length)) /* PRQA S 3453 */ /* MD_CRYPTO_30_VTT_19.7 */
# else
/* use internal implementation */
#  define Crypto_30_Vtt_CopyData(destinationPtr,sourcePtr,length)   (Crypto_30_Vtt_Local_CopyData_Implementation((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(destinationPtr), (P2CONST(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(sourcePtr), length)) /* PRQA S 3453 */ /* MD_CRYPTO_30_VTT_19.7 */
#  define Crypto_30_Vtt_ClearData(dataPtr,length)                   (Crypto_30_Vtt_Local_ClearData_Implementation((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR))(dataPtr), length)) /* PRQA S 3453 */ /* MD_CRYPTO_30_VTT_19.7 */
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_VTT_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
*  Crypto_30_Vtt_Init()
*********************************************************************************************************************/
/*! \brief      Initializes the Crypto Driver
*  \details     This function initializes the module Crypto_30_Vtt. It initializes all variables and sets the
*               module state to initialized.
*  \pre         Interrupts are disabled.
*               Module is uninitialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*  \note        Specification of module initialization
*  \trace       CREQ-131331
*********************************************************************************************************************/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_Init(void);

/**********************************************************************************************************************
*  Crypto_30_Vtt_InitMemory()
*********************************************************************************************************************/
/*! \brief      The function initializes variables, which cannot be initialized with the startup code.
*  \details     Initialize component variables at power up.
*  \pre         Module is uninitialized.
*  \context     TASK
*  \reentrant   FALSE
*  \synchronous TRUE
*********************************************************************************************************************/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_InitMemory(void);

# if (CRYPTO_30_VTT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_Vtt_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Crypto_30_Vtt_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CRYPTO_30_VTT_VERSION_INFO_API == STD_ON
 *  \trace       CREQ-131329
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) versioninfo);
# endif

/**********************************************************************************************************************
 *  Crypto_30_Vtt_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \return        E_OK                    Request successful.
 *  \return        E_NOT_OK                Request failed.
 *  \return        CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy.
 *  \return        CRYPTO_E_KEY_NOT_VALID  Request failed, the key is not valid.
 *  \return        CRYPTO_E_QUEUE_FULL     Request failed, the queue is full.
 *  \return        CRYPTO_E_SMALL_BUFFER   Request failed, the provided buffer is too small to store the result.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_ProcessJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_Vtt_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the received job
 *  \details       This interface removes the provided job from the queue and cancels the processing of the job if possible.
 *  \param[in]     objectId                Holds the identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful, job has been removed.
 *  \return        E_NOT_OK                Request failed, job could not be removed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VTT_CODE) Crypto_30_Vtt_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VTT_APPL_VAR) job);

# define CRYPTO_30_VTT_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*!
* \exclusivearea CRYPTO_30_VTT_EXCLUSIVE_AREA_0
* Ensures consistency of global RAM variables.
* \protects DriverObjectState, Queue and Lock variables.
* \usedin Crypto_30_Vtt_MainFunction, Crypto_30_Vtt_ProcessJob, Crypto_30_Vtt_CancelJob
* \exclude -
* \length SHORT in Crypto_30_Vtt_MainFunction, Crypto_30_Vtt_ProcessJob, Crypto_30_Vtt_CancelJob, Crypto_30_Vtt_KeyExchangeCalcPubVal, Crypto_30_Vtt_KeyExchangeCalcSecret, Crypto_30_Vtt_KeyCopy, Crypto_30_Vtt_KeyElementCopy, Crypto_30_Vtt_KeyElementSet, Crypto_30_Vtt_KeyValidSet, Crypto_30_Vtt_KeyElementGet, Crypto_30_Vtt_RandomSeed and Crypto_30_Vtt_KeyDerive.
* \endexclusivearea

* \exclusivearea CRYPTO_30_VTT_EXCLUSIVE_AREA_1
* Ensures consistency of global RAM variables. Only necessary if 32-bit access of variables is non-atomic.
* \protects Lock variables.
* \usedin Crypto_30_Vtt_Process
* \exclude -
* \length SHORT as only one operation is secured.
* \endexclusivearea

* \exclusivearea CRYPTO_30_VTT_EXCLUSIVE_AREA_2
* Ensures consistency of reading key elements against a parallel key manipulation.
* \protects Key reading.
* \usedin Crypto_30_Vtt_Process (Random Job)
* \exclude Crypto_30_Vtt_KeyElementSet, Crypto_30_Vtt_KeyCopy, Crypto_30_Vtt_KeyElementCopy, Crypto_30_Vtt_KeyDerive, Crypto_30_Vtt_KeyGenerate, Crypto_30_Vtt_KeyExchangeCalcPubVal, Crypto_30_Vtt_KeyExchangeCalcSecret
* \length LONG in all functions since key is copied and time depends on the length of the key.
* \endexclusivearea
*/

#endif /* CRYPTO_30_VTT_H */
/**********************************************************************************************************************
 *  END OF FILE: CRYPTO_30_VTT.H
 *********************************************************************************************************************/

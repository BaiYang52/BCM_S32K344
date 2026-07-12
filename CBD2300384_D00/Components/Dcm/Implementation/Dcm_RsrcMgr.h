/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_RsrcMgr.h
 *         \unit  RsrcMgr
 *        \brief  Contains public inline function definitions of Resource Manager unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_RSRCMGR_H)
# define DCM_RSRCMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_RsrcMgrTypes.h"
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Memory access types, used for locking the memory manager to protect from concurrent access */
# define DCM_RSRCMGR_MEM_ACCESS_NONE                                 ((Dcm_RsrcMgrMemLockOwnerType)0u)  /*!< The access to the memory is not locked */
# define DCM_RSRCMGR_MEM_ACCESS_REQ_EXT                              ((Dcm_RsrcMgrMemLockOwnerType)1u)  /*!< The access to the memory is locked by SVC 0x23/0x3D */
# define DCM_RSRCMGR_MEM_ACCESS_REQ_INT                              ((Dcm_RsrcMgrMemLockOwnerType)2u)  /*!< The access to the memory is locked by SVC 0x2C */

/*! DID/VID access types, used for locking the DID/VID to protect from concurrent access */
# define DCM_RSRCMGR_DIDLOCK_OWNER_NONE                              ((Dcm_RsrcMgrDidLockOwnerType)0)  /*!< The access to the DID is not locked */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID09                             ((Dcm_RsrcMgrDidLockOwnerType)1)  /*!< The access to the DID/VID is locked by SVC 0x09 */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID22                             ((Dcm_RsrcMgrDidLockOwnerType)2)  /*!< The access to the DID is locked by SVC 0x22 */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID2A                             ((Dcm_RsrcMgrDidLockOwnerType)3)  /*!< The access to the DID is locked by SVC 0x2A */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID2C                             ((Dcm_RsrcMgrDidLockOwnerType)4)  /*!< The access to the DID is locked by SVC 0x2C */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID2E                             ((Dcm_RsrcMgrDidLockOwnerType)5)  /*!< The access to the DID is locked by SVC 0x2E */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID2F                             ((Dcm_RsrcMgrDidLockOwnerType)6)  /*!< The access to the DID is locked by SVC 0x2F */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03                          ((Dcm_RsrcMgrDidLockOwnerType)7)  /*!< The access to the DID is locked by SVC 0x86 subfunction 0x03 */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07                          ((Dcm_RsrcMgrDidLockOwnerType)8)  /*!< The access to the DID is locked by SVC 0x86 subfunction 0x07 */
# define DCM_RSRCMGR_DIDLOCK_OWNER_SECURE_CODING                     ((Dcm_RsrcMgrDidLockOwnerType)9)  /*!< The access to the DID is locked by RidMgr for Secure Coding */
# define DCM_RSRCMGR_RIDLOCK_OWNER_NONE                              ((Dcm_RsrcMgrRidLockOwnerType)DCM_NUM_THREADS)  /*!< The access to the RID is not locked */

# define DCM_RSRCMGR_SERVICELOCK_OWNER_NONE                          ((Dcm_RsrcMgrServiceLockOwnerType)DCM_NUM_THREADS)  /*!< The access to the Service is not locked */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RsrcMgrInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the resource manager sub-component.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RsrcMgrInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasMemLock()
 *********************************************************************************************************************/
/*! \brief          Checks whether the given owner has locked the memory resource.
 *  \details        -
 *  \param[in]      owner    Service specific owner
 *  \return         TRUE     Memory resource is already locked by the given owner
 *  \return         FALSE    Memory resource is not locked by the given owner
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasMemLock(
  Dcm_RsrcMgrMemLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetMemLock()
 *********************************************************************************************************************/
/*! \brief          Tries to lock the memory resource.
 *  \details        -
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      owner            Service specific owner
 *  \return         DCM_E_OK         Memory resource was successfully locked
 *  \return         DCM_E_PENDING    Memory resource is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetMemLock(
  Dcm_OpStatusType opStatus,
  Dcm_RsrcMgrMemLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseMemLock()
 *********************************************************************************************************************/
/*! \brief          Releases the lock of the memory resource.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseMemLock(
  void
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasDidLock()
 *********************************************************************************************************************/
/*! \brief          Checks whether the given owner has locked any Did.
 *  \details        -
 *  \param[in]      owner    Service specific owner of the to be locked Did
 *  \return         TRUE     Did is already locked by the given owner
 *  \return         FALSE    Did is not locked by the given owner
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasDidLock(
  Dcm_RsrcMgrDidLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetDidLock()
 *********************************************************************************************************************/
/*! \brief          Tries to lock a given Did.
 *  \details        -
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      did              Did to be locked
 *  \param[in]      owner            Service specific owner of the to be locked Did
 *  \return         DCM_E_OK         Did was successfully locked or owner has already a lock for that Did
 *  \return         DCM_E_PENDING    Did is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetDidLock(
  Dcm_OpStatusType opStatus,
  uint16 did,
  Dcm_RsrcMgrDidLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseDidLock()
 *********************************************************************************************************************/
/*! \brief          Releases a Did locked by the given owner.
 *  \details        -
 *  \param[in]      owner    Service specific owner of the to be released Did
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseDidLock(
  Dcm_RsrcMgrDidLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetRidLock()
 *********************************************************************************************************************/
/*! \brief          Tries to lock a given Rid.
 *  \details        -
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      rid              Rid to be locked
 *  \param[in]      owner            Service specific owner of the to be locked Rid
 *  \return         DCM_E_OK         Rid was successfully locked or owner has already a lock for that Rid
 *  \return         DCM_E_PENDING    Rid is locked
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetRidLock(
  Dcm_OpStatusType opStatus,
  uint16 rid,
  Dcm_RsrcMgrRidLockOwnerType owner
  );

/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseRidLock()
 *********************************************************************************************************************/
/*! \brief          Releases a Rid locked by the given owner.
 *  \details        -
 *  \param[in]      owner    Service specific owner of the to be released Rid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseRidLock(
  Dcm_RsrcMgrRidLockOwnerType owner
  );

# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrHasServiceLock()
 *********************************************************************************************************************/
/*! \brief          Checks whether the given owner has locked any service resource.
 *  \details        -
 *  \param[in]      owner    Owner of the service
 *  \return         TRUE     Service resource is already locked by the given owner
 *  \return         FALSE    Service resource is not locked by the given owner
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_RsrcMgrHasServiceLock(
  Dcm_RsrcMgrServiceLockOwnerType owner
  );
# endif

# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrGetServiceLock()
 *********************************************************************************************************************/
/*! \brief          Tries to lock a given Service.
 *  \details        -
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      service          Service to be locked
 *  \param[in]      owner            Owner of the to be locked Service
 *  \return         DCM_E_OK         Service was successfully locked
 *  \return         DCM_E_NOT_OK     In case of a cancellation
 *  \return         DCM_E_PENDING    Service is locked by another thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RsrcMgrGetServiceLock(
  Dcm_OpStatusType opStatus,
  uint8 service,
  Dcm_RsrcMgrServiceLockOwnerType owner
  );
# endif

# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RsrcMgrReleaseServiceLock()
 *********************************************************************************************************************/
/*! \brief          Releases a Service locked by the given owner.
 *  \details        -
 *  \param[in]      owner    Owner of the to be released Service
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RsrcMgrReleaseServiceLock(
  Dcm_RsrcMgrServiceLockOwnerType owner
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_RSRCMGR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_RsrcMgr.h
 *********************************************************************************************************************/

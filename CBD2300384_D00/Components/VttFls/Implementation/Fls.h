/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Fls.h
 *      Project:  DrvFls_VttCanoe01Asr
 *       Module:  MICROSAR Fls
 *    Generator:  -
 *
 *  Description:  MICROSAR Module Fls implements a Flash driver for the CANoe Emulation. Instead of a real Flash
 *                peripheral a text file is used as non-volatile memory.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-09-13  virlg   ESCAN00068488 Initial Development
 *  01.00.01  2014-03-14  virchl                No modifications
 *  01.00.02  2014-05-15  virchl                Version number incremented
 *  02.00.00  2015-01-21  virchl   FEAT-254     Global Renaming of VIP to VTT
 *  02.00.01  2015-04-07  virchl  ESCAN00082265 Remove of VttFls_Init in Fls_Init
 *  02.01.00  2015-07-08  virchl   FEAT-1479    Support of automatic registration of Fls_Init (if VTTOnly-SIP)
 *                                              Fls_Init renamed to Fls_Init_Vtt and a define in Fls_Cfg.h reroutes it
 *  02.02.00  2015-08-27  virchl   FEAT-1372    Update of SwVersions due to increase of generator version
 *  02.02.00  2015-10-05  virchl  ESCAN00084658 Include of SchM_Fls.h added
 *  02.02.00  2015-12-10  virchl  ESCAN00086867 Fls_ReadSync fixed. Index counter added in VttFls_Read function
 *  03.00.00  2016-04-21  virbmz  ESCAN00089564 the local struct Fls_Job_Struct, local enum Fls_JobType and local
 *                                              variables Fls_CurrJob_t, Fls_Status_t and PointToConfig_pt are changed
 *                                              into globals, to offer an access in Wrapper Functionality
 *                                              Fls_JobType was extended with FLS_CANCEL_JOB
 *  03.00.01  2016-07-01  virbmz  OIL-Entry     sharepoint-Entry:Several compiler warning in different modules
 *                                              removed local variables which are not used
 *  03.00.02  2016-08-04  virbmz  ESCAN00090667 Fls_17_Pmu.h was modified
 *                                FEAT-1842     Support of external driver
 *  03.01.00  2017-01-00  virbmz  FEAT-1824     Support a component-specific SafeBSW configuration switch - version update 
 *                                              instead of MemIf_AddressType, MemIf_LengthType use Fls_30_Stub_AddressType, 
 *                                              Fls_30_Stub_LengthType, the MemIf types were valid, only for older ASR versions
 *  03.02.00  2017-05-18  virbmz  TASK-33340    Version number incremented
 *  03.03.00  2017-06-12  virbmz  STORYC-1212   Version number incremented
 *  03.04.00  2017-12-01  virbmz  STORY-3133    Version number incremented
 *  03.04.01  2018-01-11  virbmz  OI-6435       Version number incremented
 *  03.05.00  2018-09-06  virbmz  STORYC-6285   Implement the VttFls Infineon Part generic (Infix independent)
 *  03.05.01  2018-10-30  virbmz  TASK-96823    VttFls: Generate Infixed Defines (job types, service IDs)
 *  03.05.02  2019-04-03  virbmz  ESCAN00101506 API Fls_BlankCheck should be available for each controller
 *  03.05.03  2020-04-21  vireno  ESCAN00105952 Reads too small units of data
 *  03.05.04  2020-09-03  virbmz  ESCAN00107112 Vtt Fls_ReadSync invokes the Fee_JobEndNotification
 *  03.06.00  2021-03-04  virbmz  MWDG-3996     VttFls: Implement Blank Check and Overwrite Detection
 *********************************************************************************************************************/

#if !defined FLS_H
# define FLS_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define FLS_VENDOR_ID                           (30u)
# define FLS_MODULE_ID                           (92u)
# define FLS_INSTANCE_ID                         (0u)

/* AUTOSAR Software Specification Version Information */
# define FLS_AR_RELEASE_MAJOR_VERSION            (4u)
# define FLS_AR_RELEASE_MINOR_VERSION            (0u)
# define FLS_AR_RELEASE_PATCH_VERSION            (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define FLS_SW_MAJOR_VERSION                    (3u)
# define FLS_SW_MINOR_VERSION                    (6u)
# define FLS_SW_PATCH_VERSION                    (0u)

/* ----- API service IDs ----- */
# define FLS_SID_INIT                            (0x00u)  /* Fls_Init()             */
# define FLS_SID_ERASE                           (0x01u)  /* Fls_Erase()            */
# define FLS_SID_WRITE                           (0x02u)  /* Fls_Write()            */
# define FLS_SID_CANCEL                          (0x03u)  /* Fls_Cancel()           */
# define FLS_SID_GETSTATUS                       (0x04u)  /* Fls_GetStatus()        */
# define FLS_SID_GETJOBRESULT                    (0x05u)  /* Fls_GetJobResult()     */
# define FLS_SID_MAINFUNCTION                    (0x06u)  /* Fls_MainFunction()     */
# define FLS_SID_READ                            (0x07u)  /* Fls_Read()             */
# define FLS_SID_COMPARE                         (0x08u)  /* Fls_Compare()          */
# define FLS_SID_SETMODE                         (0x09u)  /* Fls_SetMode()          */
# define FLS_SID_GETVERSIONINFO                  (0x10u)  /* Fls_GetVersionInfo()   */
# define FLS_SID_BLANK_CHECK                     (0x0Au)  /* Fls_BlankCheck()       */
# define FLS_SID_COPY                            (0x20u)  /* Fls_Copy()             */
# define FLS_SID_READ_SYNC                       (0x21u)  /* Fls_ReadSync()         */


/* ----- Error codes ----- */
# define FLS_E_NO_ERROR                          (0x00u)  /* Used to check if no error occurred */                         
# define FLS_E_PARAM_CONFIG                      (0x01u)  /* Pointer to the configurations was 0 */
# define FLS_E_PARAM_ADDRESS                     (0x02u)  /* Address not aligned to a segment address or out of range */
# define FLS_E_PARAM_LENGTH                      (0x03u)  /* Length not greater than 0 */
# define FLS_E_PARAM_DATA                        (0x04u)  /* wrong pointer parameter */
# define FLS_E_UNINIT                            (0x05u)  /* Error code for not initialized module */
# define FLS_E_BUSY                              (0x06u)  /* Error code if the flash current busy */
# define FLS_E_VERIFY_ERASE_FAILED               (0x07u)  /* Error code if write API service is called on already written (not erased) memory */ 

# define FLS_E_PARAM_VINFO                       (0x15u)  /* VersionInfo-Ptr references NULL_PTR */
# define FLS_E_ADDRESS_OVERLAP                   (0x20u)  /* Source- and TargetAddress overlap at copying process */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES AFTER GLOBAL TYPES AND STRUCTURES
 *********************************************************************************************************************/
# include "Fls_Cfg.h"
# include "VttCntrl_Fls.h"
/* include of SchM_Fls.h moves into Fls_Cfg.h because the file name is API-Infix dependent */


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* Job type enumeration */
typedef enum
{
  FLS_NO_JOB = 0,
  FLS_READ_JOB,
  FLS_WRITE_JOB,
  FLS_ERASE_JOB,
  FLS_COMPARE_JOB,
  FLS_COPY_JOB,
  FLS_CANCEL_JOB, 
  FLS_READSYNC_JOB, /* extended to support AURIX Fee. Not intepreted by main function */
  FLS_BLANKCHECK_JOB
} Fls_JobType;

/* Possible settings of a job */
typedef struct
{
  Fls_JobType     JobType;
  Fls_AddressType StartAddress;
  Fls_AddressType CurrAddress;
  Fls_LengthType  JobLength;
  uint32          CurrByte;
  const uint8*    WriteFlsPointer;
  uint8*          ReadFlsPointer;
} Fls_Job_Struct;

# if (FLS_30_INFINEON_EXTENTION == STD_ON)
typedef P2CONST(uint32, AUTOMATIC, FLS_APPL_CONST) Fls_ConstWordPtrType;
# endif

extern VAR(Fls_Job_Struct, FLS_VAR) Fls_CurrJob_t;                           /* status of the current job */
extern VAR(MemIf_StatusType, FLS_VAR) Fls_Status_t;                          /* status of the Fls driver */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/* Declarations of API functions */
FUNC(void, FLS_CODE) Fls_InitMemory(void);

FUNC(void, FLS_CODE) Fls_Init_Vtt(P2CONST(Fls_ConfigType, AUTOMATIC, FLS_APPL_CONST) ConfigPtr);

FUNC(Std_ReturnType, FLS_CODE) Fls_Erase(Fls_AddressType  FlsAddress, Fls_LengthType Length);
# if (FLS_30_INFINEON_EXTENTION == STD_ON)
#  if (FLS_USE_ERASESUSPEND == STD_ON)
FUNC(Std_ReturnType, FLS_CODE) Fls_SuspendErase(uint32 SectorNumber);
FUNC(Std_ReturnType, FLS_CODE) Fls_ResumeErase(uint32 SectorNumber);
#  else
#   define Fls_ResumeErase(x)  E_OK
#   define Fls_SuspendErase(x) E_OK
#  endif
# endif

FUNC(Std_ReturnType, FLS_CODE) Fls_Write
(
Fls_AddressType FlsAddress,
Fls_ConstBytePtrType DataBufferPtr,
Fls_LengthType Length
);

# if (STD_ON == FLS_CANCEL_API)
FUNC(void, FLS_CODE) Fls_Cancel(void);
# endif

# if (FLS_30_INFINEON_EXTENTION == STD_ON)
FUNC(void, FLS_CODE) Fls_CancelNonEraseJobs(void);
# endif

# if (STD_ON == FLS_GET_STATUS_API)
FUNC(MemIf_StatusType, FLS_CODE) Fls_GetStatus(void);
# endif

# if (STD_ON == FLS_GET_JOB_RESULT_API)
FUNC(MemIf_JobResultType, FLS_CODE) Fls_GetJobResult(void);
# endif

FUNC(Std_ReturnType, FLS_CODE) Fls_Read
(
  Fls_AddressType FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType Length
);

# if (FLS_READSYNC_API == STD_ON)
FUNC(MemIf_JobResultType, FLS_CODE) Fls_ReadSync
(
  Fls_AddressType FlsAddress,
  Fls_VarBytePtrType DataBufferPtr,
  Fls_LengthType Length
);
# endif

# if (FLS_30_INFINEON_EXTENTION == STD_ON)
FUNC(Std_ReturnType, FLS_CODE) Fls_ReadWordsSync
(
  Fls_AddressType FlsAddress, 
  uint32* DataBufferPtr, 
  Fls_LengthType Length
 );

FUNC(Fls_JobType, FLS_CODE) Fls_GetNotifCaller(void);

FUNC(Std_ReturnType, FLS_CODE) Fls_VerifyErase
(
  uint32 Offset, 
  uint32* UnerasedWordlineAddressPtr, 
  uint8* const UnerasedWordlineCountPtr
);

FUNC(Std_ReturnType, FLS_CODE) Fls_VerifySectorErase
(
  Fls_AddressType TargetAddress, 
  uint32 * UnerasedWordlineAddressPtr, 
  uint8 * const UnerasedWordlineCountPtr, uint8 Sector
);
# endif

FUNC(Std_ReturnType, FLS_CODE) Fls_BlankCheck(
  Fls_AddressType FlsAddress, 
  Fls_LengthType Length
);

# if (STD_ON == FLS_COMPARE_API)
FUNC(Std_ReturnType, FLS_CODE) Fls_Compare
(
  Fls_AddressType FlsAddress,
  Fls_ConstBytePtrType DataBufferPtr,
  Fls_LengthType Length
);
# endif

# if (FLS_30_INFINEON_EXTENTION == STD_ON)
FUNC(Std_ReturnType, FLS_CODE) Fls_CompareWordsSync
(
  Fls_AddressType FlsAddress, 
  Fls_ConstWordPtrType DataBufferPtr, 
  Fls_LengthType Length
);
# endif

# if (STD_ON == FLS_SET_MODE_API)
FUNC(void, FLS_CODE) Fls_SetMode(MemIf_ModeType Mode);
# endif

# if (STD_ON == FLS_VERSION_INFO_API)
FUNC(void, FLS_CODE) Fls_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FLS_APPL_DATA) versioninfo);
# endif

# if (STD_ON == FLS_COPY_API)
FUNC(Std_ReturnType, FLS_CODE) Fls_Copy
(
  Fls_AddressType TargetAddress,
  Fls_AddressType SourceAddress,
  Fls_LengthType Length
);
# endif

FUNC(Std_ReturnType, FLS_CODE) Fls_SimulateError(void);



/*Main-Function declaration is provided by the SCHM*/

# if (FLS_30_INFINEON_EXTENTION == STD_ON)
/* ----- APIs which are not supported by VTT  ----- */
#  define CRC32(a,b)                (0x0u)   /* CRC32 algorithm */
#  define Fls_GetOperStatus()       E_OK     /* provides the OPER error status - E_OK: no OPER error */
#  define Fls_ControlTimeoutDet(x)  UNUSED_PARAMETER(x) /* disable and re-enable detection and reporting of time-out DET, nothing is done */
# endif
  
#endif /* FLS_H */

/**********************************************************************************************************************
 *  END OF FILE: Fls.h
 *********************************************************************************************************************/

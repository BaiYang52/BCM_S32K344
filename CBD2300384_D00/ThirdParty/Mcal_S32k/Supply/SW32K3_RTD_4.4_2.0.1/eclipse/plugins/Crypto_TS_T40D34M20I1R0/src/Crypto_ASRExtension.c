/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : HSE
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file
*
*   @addtogroup CRYPTO
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Crypto_ASRExtension.h"
#include "Crypto_Private.h"
#include "Crypto_Util.h"
#include "Det.h"
#include "Crypto.h"
#include "Crypto_Ipw.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_ASREXTENSION_C                       43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C        4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C        4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C     0
#define CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C                2
#define CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C                0
#define CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if Crypto ASRExtensions source file and Crypto ASRExtensions header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_C != CRYPTO_VENDOR_ID_ASREXTENSION_H)
    #error "Crypto_ASRExtension.c and Crypto_ASRExtension.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions source file and Crypto ASRExtensions header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_H) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Crypto_ASRExtension.h are different"
#endif

/* Check if Crypto ASRExtensions source file and Crypto ASRExtensions header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MINOR_VERSION_ASREXTENSION_H) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C != CRYPTO_SW_PATCH_VERSION_ASREXTENSION_H)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.c and Crypto_ASRExtension.h are different"
#endif


/* Check if Crypto ASRExtensions source file and Crypto_Private header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_C != CRYPTO_VENDOR_ID_PRIVATE)
    #error "Crypto_ASRExtension.c and Crypto_Private.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions source file and Crypto_Private header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Crypto_Private.h are different"
#endif

/* Check if Crypto ASRExtensions source file and Crypto_Private header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C != CRYPTO_SW_PATCH_VERSION_PRIVATE)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.c and Crypto_Private.h are different"
#endif


/* Check if Crypto ASRExtensions source file and Crypto_Util header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_C != CRYPTO_UTIL_VENDOR_ID)
    #error "Crypto_ASRExtension.c and Crypto_Util.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions source file and Crypto_Util header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C    != CRYPTO_UTIL_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C    != CRYPTO_UTIL_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C != CRYPTO_UTIL_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Crypto_Util.h are different"
#endif

/* Check if Crypto ASRExtensions source file and Crypto_Util header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C != CRYPTO_UTIL_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C != CRYPTO_UTIL_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C != CRYPTO_UTIL_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.c and Crypto_Util.h are different"
#endif


/* Check if Crypto ASRExtensions source file and Crypto header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_C != CRYPTO_VENDOR_ID)
    #error "Crypto_ASRExtension.c and Crypto.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions source file and Crypto header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Crypto.h are different"
#endif

/* Check if Crypto ASRExtensions source file and Crypto header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C != CRYPTO_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.c and Crypto.h are different"
#endif


/* Check if Crypto ASRExtensions source file and Crypto Ipw header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_ASREXTENSION_C != CRYPTO_VENDOR_ID_IPW_H)
    #error "Crypto_ASRExtension.c and Crypto_Ipw.h have different vendor ids"
#endif

/* Check if Crypto ASRExtensions source file and Crypto Ipw header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_IPW_H) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_IPW_H) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_ASREXTENSION_C != CRYPTO_AR_RELEASE_REVISION_VERSION_IPW_H) \
    )
    #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Crypto_Ipw.h are different"
#endif

/* Check if Crypto ASRExtensions source file and Crypto Ipw header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MAJOR_VERSION_IPW_H) || \
     (CRYPTO_SW_MINOR_VERSION_ASREXTENSION_C != CRYPTO_SW_MINOR_VERSION_IPW_H) || \
     (CRYPTO_SW_PATCH_VERSION_ASREXTENSION_C != CRYPTO_SW_PATCH_VERSION_IPW_H)    \
    )
    #error "Software Version Numbers of Crypto_ASRExtension.c and Crypto_Ipw.h are different"
#endif


#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if Crypto_ASRExtension.c file and Det header file are of the same AutoSar version */
    #if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_ASREXTENSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (CRYPTO_AR_RELEASE_MINOR_VERSION_ASREXTENSION_C != DET_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Crypto_ASRExtension.c and Det.h are different"
    #endif
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define CRYPTO_START_SEC_CODE
#include "Crypto_MemMap.h"

Std_ReturnType Crypto_Exts_FormatKeyCatalogs(void)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_FORMATKEYCATALOGS_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_FormatKeyCatalogs(u32PartitionId);
        if (CRYPTO_RET_OPERATION_TIMEOUT <= RetVal)
        {
            Crypto_ReportErrorToDet((uint8)CRYPTO_EXTS_FORMATKEYCATALOGS_ID, (uint8)RetVal);
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SetSynchronousRequestsTimeout
(
    uint32 u32Timeout
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SETSYNCREQUESTSTIMEOUT_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        Crypto_Ipw_SetSynchronousRequestsTimeout(u32PartitionId, u32Timeout);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SHE_BootFailure(void)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_BOOTFAILURE_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheSetBootStatus(u32PartitionId, FALSE);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SHE_BootOk(void)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_BOOTOK_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheSetBootStatus(u32PartitionId, TRUE);
    }

    return RetVal;
}


Std_ReturnType Crypto_Exts_SHE_GetStatus
(
    uint8*  pStatus
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_GETSTATUS_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else if (NULL_PTR == pStatus)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_GETSTATUS_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheGetStatus(u32PartitionId, pStatus);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SHE_GetId
(
    const uint8* pChallenge,
    const uint8* pId,
    const uint8* pSreg,
    const uint8* pMac
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_GETID_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else if ((NULL_PTR == pChallenge) || (NULL_PTR == pId) || (NULL_PTR == pSreg) || (NULL_PTR == pMac))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_GETID_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheGetId(u32PartitionId, pChallenge, pId, pSreg, pMac);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SHE_DebugChal
(
    const uint8* pChallenge
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_DEBUGCHAL_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else if (NULL_PTR == pChallenge)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_DEBUGCHAL_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheDebugChal(u32PartitionId, pChallenge);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_SHE_DebugAuth
(
    const uint8* pAuthorization
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_DEBUGAUTH_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else if (NULL_PTR == pAuthorization)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_DEBUGAUTH_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheDebugAuth(u32PartitionId, pAuthorization);
    }

    return RetVal;
}

Std_ReturnType Crypto_Exts_MPCompression
(
    const uint8*  pInput,
          uint32  u32InputLen,
    const uint8*  pResult,
    const uint32* pResultLen
)
{
    const uint32   u32PartitionId = Crypto_Util_GetPartitionId();
    Std_ReturnType RetVal         = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == CRYPTO_DEV_ERROR_DETECT)
     /* Check whether the call is coming from an authorized core and the driver is initialized */
    if ((Std_ReturnType)E_NOT_OK == Crypto_CheckValidCoreAndDrvInitForNonCdoApis(u32PartitionId, CRYPTO_EXTS_SHE_MPCOMPRESSION_ID))
    {
        /* Nothing to do. The reporting to DET was done from inside the Crypto_CheckValidCoreAndDrvInitForNonCdoApis() function */
    }
    else if ((NULL_PTR == pInput) || (NULL_PTR == pResult) || (NULL_PTR == pResultLen))
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_MPCOMPRESSION_ID, (uint8)CRYPTO_E_PARAM_POINTER);
    }
    else if (0U == u32InputLen)
    {
        (void)Det_ReportError((uint16)CRYPTO_MODULE_ID, (uint8)CRYPTO_INSTANCE_ID, (uint8)CRYPTO_EXTS_SHE_MPCOMPRESSION_ID, (uint8)CRYPTO_E_PARAM_VALUE);
    }
    else
#endif /* (STD_ON == CRYPTO_DEV_ERROR_DETECT) */
    {
        RetVal = Crypto_Ipw_SheMPCompression(u32PartitionId, pInput, u32InputLen, pResult, pResultLen);
    }

    return RetVal;
}

#define CRYPTO_STOP_SEC_CODE
#include "Crypto_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */



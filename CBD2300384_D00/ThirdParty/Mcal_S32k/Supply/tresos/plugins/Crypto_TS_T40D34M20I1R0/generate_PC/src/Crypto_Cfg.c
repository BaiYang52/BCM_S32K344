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
#include "Crypto_Cfg.h"
#include "Crypto_Private.h"
#include "Crypto.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRYPTO_VENDOR_ID_CFG_C                      43
#define CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG_C       4
#define CRYPTO_AR_RELEASE_MINOR_VERSION_CFG_C       4
#define CRYPTO_AR_RELEASE_REVISION_VERSION_CFG_C    0
#define CRYPTO_SW_MAJOR_VERSION_CFG_C               2
#define CRYPTO_SW_MINOR_VERSION_CFG_C               0
#define CRYPTO_SW_PATCH_VERSION_CFG_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!][!//
[!INCLUDE "Crypto_VersionCheck_Src.m"!][!//
[!ENDNOCODE!][!//
/* Check if Crypto configuration source file and Crypto configuration header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_CFG_C != CRYPTO_VENDOR_ID_CFG)
    #error "Crypto_Cfg.c and Crypto_Cfg.h have different vendor ids"
#endif

/* Check if Crypto configuration source file and Crypto configuration header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_CFG) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_CFG_C != CRYPTO_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Crypto_Cfg.c and Crypto_Cfg.h are different"
#endif

/* Check if Crypto configuration source file and Crypto configuration header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_CFG_C != CRYPTO_SW_MAJOR_VERSION_CFG) || \
     (CRYPTO_SW_MINOR_VERSION_CFG_C != CRYPTO_SW_MINOR_VERSION_CFG) || \
     (CRYPTO_SW_PATCH_VERSION_CFG_C != CRYPTO_SW_PATCH_VERSION_CFG)    \
    )
    #error "Software Version Numbers of Crypto_Cfg.c and Crypto_Cfg.h are different"
#endif

/* Check if Crypto configuration source file and Crypto private header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_CFG_C != CRYPTO_VENDOR_ID_PRIVATE)
    #error "Crypto_Cfg.c and Crypto_Private.h have different vendor ids"
#endif

/* Check if Crypto configuration source file and Crypto private header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_CFG_C != CRYPTO_AR_RELEASE_REVISION_VERSION_PRIVATE) \
    )
    #error "AutoSar Version Numbers of Crypto_Cfg.c and Crypto_Private.h are different"
#endif

/* Check if Crypto configuration source file and Crypto private header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_CFG_C != CRYPTO_SW_MAJOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_MINOR_VERSION_CFG_C != CRYPTO_SW_MINOR_VERSION_PRIVATE) || \
     (CRYPTO_SW_PATCH_VERSION_CFG_C != CRYPTO_SW_PATCH_VERSION_PRIVATE)    \
    )
    #error "Software Version Numbers of Crypto_Cfg.c and Crypto_Private.h are different"
#endif

/* Check if Crypto configuration source file and Crypto header file are of the same vendor */
#if (CRYPTO_VENDOR_ID_CFG_C != CRYPTO_VENDOR_ID)
    #error "Crypto_Cfg.c and Crypto.h have different vendor ids"
#endif

/* Check if Crypto configuration source file and Crypto header file are of the same Autosar version */
#if ((CRYPTO_AR_RELEASE_MAJOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MAJOR_VERSION) || \
     (CRYPTO_AR_RELEASE_MINOR_VERSION_CFG_C    != CRYPTO_AR_RELEASE_MINOR_VERSION) || \
     (CRYPTO_AR_RELEASE_REVISION_VERSION_CFG_C != CRYPTO_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crypto_Cfg.c and Crypto.h are different"
#endif

/* Check if Crypto configuration source file and Crypto header file are of the same Software version */
#if ((CRYPTO_SW_MAJOR_VERSION_CFG_C != CRYPTO_SW_MAJOR_VERSION) || \
     (CRYPTO_SW_MINOR_VERSION_CFG_C != CRYPTO_SW_MINOR_VERSION) || \
     (CRYPTO_SW_PATCH_VERSION_CFG_C != CRYPTO_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crypto_Cfg.c and Crypto.h are different"
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
[!AUTOSPACING!][!//

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_8
#include "Crypto_MemMap.h"

[!IF "num:i(count(CryptoGeneral/CryptoEcucPartitionRef/*)) > 0"!][!//
    [!VAR "MaxNumPartitions" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
    [!FOR "x" = "0" TO "num:i($MaxNumPartitions - 1)"!][!//
        [!VAR "NumMappedCdos" = "0"!][!//
        [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
            [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
                [!VAR "CurCdoPartition" = "node:value(.)"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                        [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                            [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                                [!IF "$x = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!][!//
                                    [!VAR "NumMappedCdos" = "$NumMappedCdos + 1"!][!//
                                [!ENDIF!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
        [!IF "$NumMappedCdos > 0"!][!//
            [!INDENT "0"!][!//
                /* Array storing the indexes of the Crypto Driver Objects mapped on partition [!"as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*[num:i($x) + 1]/@name"!] */
                static const uint8 Crypto_au8CDOs_[!"as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*[num:i($x) + 1]/@name"!][] =
                {
            [!ENDINDENT!][!//
            [!INDENT "4"!][!//
            [!VAR "NumFoundCdos" = "0"!][!//
            [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
                [!VAR "PartitionMatched" = "'false'"!][!//
                [!VAR "IdxCdo" = "@index"!][!//
                [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
                    [!VAR "CurCdoPartition" = "node:value(.)"!][!//
                    [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                        [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                            [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                                [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                                    [!IF "$x = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!][!//[!ENDIF!][!//
                                        [!VAR "PartitionMatched" = "'true'"!][!//
                                        [!VAR "NumFoundCdos" = "$NumFoundCdos + 1"!][!//
                                    [!ENDIF!][!//
                                [!ENDIF!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDLOOP!][!//
                [!ENDLOOP!][!//
                [!IF "$PartitionMatched = 'true'"!][!//
                    [!"$IdxCdo"!]U[!IF "$NumFoundCdos < $NumMappedCdos"!],[!ENDIF!][!CR!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!ENDINDENT!][!//
            [!INDENT "0"!][!//
                };
            [!ENDINDENT!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!//
[!ELSE!][!//
    [!INDENT "0"!][!//
        /* Array storing the indexes of the Crypto Driver Objects */
        static const uint8 Crypto_au8CDOs[] =
        {
    [!VAR "CurCdo" = "0"!][!//
    [!VAR "NumCdos" = "num:i(count(CryptoDriverObjects/CryptoDriverObject/*))"!][!//
    [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
        [!VAR "CurCdo" = "$CurCdo + 1"!][!//
        [!INDENT "4"!][!//
        [!"@index"!]U[!IF "$CurCdo < $NumCdos"!],[!ENDIF!][!CR!][!//
        [!ENDINDENT!][!//
    [!ENDLOOP!][!//
        };
    [!ENDINDENT!][!//
[!ENDIF!][!//

[!NOCODE!][!//
[!IF "node:exists(CryptoKeyElements)"!][!//
    [!LOOP "node:CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "'true'  "!][!//
            [!// Check first the Encrypted Import and Export properties
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./EncryptedKeyImport) and node:value(./EncryptedKeyImport) = 'true' and node:name(./CipherScheme) = 'SymmetricCipher' and num:i(string-length(./CipherScheme/InitializationVector)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when encrypted with a symmetric cipher */
                    static const uint8 Crypto_au8SymCipherIV_[!"node:name(.)"!][[!"num:i(string-length(./CipherScheme/InitializationVector) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CipherScheme/InitializationVector)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./CipherScheme/InitializationVector))) "!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CipherScheme/InitializationVector), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CipherScheme/InitializationVector), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) = 'ExportSymmetricCipher' and num:i(string-length(./ExportCipherScheme/ExportInitializationVector)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when exported encrypted with a symmetric cipher */
                    static const uint8 Crypto_au8ExportSymCipherIV_[!"node:name(.)"!][[!"num:i(string-length(./ExportCipherScheme/ExportInitializationVector) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportCipherScheme/ExportInitializationVector)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportCipherScheme/ExportInitializationVector))) "!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportCipherScheme/ExportInitializationVector), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportCipherScheme/ExportInitializationVector), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };
                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./EncryptedKeyImport) and node:value(./EncryptedKeyImport) = 'true' and node:name(./CipherScheme) = 'AeadCipherScheme'"!][!//
                [!IF "num:i(string-length(./CipherScheme/Tag)) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                        /* Array storing the Tag Pointer used by the Crypto Key Element [!"node:name(.)"!] when encrypted with a Authenticated cipher */
                        static const uint8 Crypto_au8AeadCipherTag_[!"node:name(.)"!][[!"num:i(string-length(./CipherScheme/Tag) div 2)"!]U] =
                        {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CipherScheme/Tag)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./CipherScheme/Tag))) "!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/Tag), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/Tag), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                        };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
                [!IF "num:i(string-length(./CipherScheme/AeadInitializationVector)) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                        /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when encrypted with a Authenticated cipher */
                        static const uint8 Crypto_au8AeadCipherIV_[!"node:name(.)"!][[!"num:i(string-length(./CipherScheme/AeadInitializationVector) div 2)"!]U] =
                        {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CipherScheme/AeadInitializationVector)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./CipherScheme/AeadInitializationVector))) "!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/AeadInitializationVector), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/AeadInitializationVector), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                        };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
                [!IF "num:i(string-length(./CipherScheme/AadHeaderData)) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                        /* Array storing the Additional Data (AAD) used by the Crypto Key Element [!"node:name(.)"!] when encrypted with a Authenticated cipher */
                        static const uint8 Crypto_au8AeadCipherAAD_[!"node:name(.)"!][[!"num:i(string-length(./CipherScheme/AadHeaderData) div 2)"!]U] =
                        {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CipherScheme/AadHeaderData)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./CipherScheme/AadHeaderData))) "!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/AadHeaderData), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./CipherScheme/AadHeaderData), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                        };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme'"!][!//
                [!IF "num:i(string-length(./ExportCipherScheme/ExportAeadInitializationVector)) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                        /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when exported encrypted with a Authenticated cipher */
                        static const uint8 Crypto_au8ExportAeadCipherIV_[!"node:name(.)"!][[!"num:i(string-length(./ExportCipherScheme/ExportAeadInitializationVector) div 2)"!]U] =
                        {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportCipherScheme/ExportAeadInitializationVector)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportCipherScheme/ExportAeadInitializationVector))) "!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./ExportCipherScheme/ExportAeadInitializationVector), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./ExportCipherScheme/ExportAeadInitializationVector), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                        };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./EncryptedKeyImport) and node:value(./EncryptedKeyImport) = 'true' and node:name(./CipherScheme) = 'RSACipherScheme' and node:name(./CipherScheme/RsaScheme) = 'RSAES_OAEP' and num:i(string-length(./CipherScheme/RsaScheme/Label)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Label used by the Crypto Key Element [!"node:name(.)"!] when encrypted with a RSA OAEP cipher */
                    static const uint8 Crypto_au8RsaOaepCipherLabel_[!"node:name(.)"!][[!"num:i(string-length(./CipherScheme/RsaScheme/Label) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CipherScheme/RsaScheme/Label)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./CipherScheme/RsaScheme/Label)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CipherScheme/RsaScheme/Label), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CipherScheme/RsaScheme/Label), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!][!//
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) = 'ExportRSACipherScheme' and node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesOaep' and num:i(string-length(./ExportCipherScheme/ExportRsaScheme/ExportLabel)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Label used by the Crypto Key Element [!"node:name(.)"!] when exported encrypted with a RSA OAEP cipher */
                    static const uint8 Crypto_au8ExportRsaOaepCipherLabel_[!"node:name(.)"!][[!"num:i(string-length(./ExportCipherScheme/ExportRsaScheme/ExportLabel) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportCipherScheme/ExportRsaScheme/ExportLabel)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportCipherScheme/ExportRsaScheme/ExportLabel)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportCipherScheme/ExportRsaScheme/ExportLabel), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportCipherScheme/ExportRsaScheme/ExportLabel), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!][!//
            [!ENDIF!][!//
            [!// Check now the Authenticated Import and Export properties
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./AuthenticatedKeyImport) and node:value(./AuthenticatedKeyImport) = 'true' and node:name(./AuthenticationScheme) = 'MacScheme' and node:name(./AuthenticationScheme/Scheme) = 'GMAC' and num:i(string-length(./AuthenticationScheme/Scheme/InitializationVector)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when authenticated with GMAC mac scheme */
                    static const uint8 Crypto_au8GmacAuthIV_[!"node:name(.)"!][[!"num:i(string-length(./AuthenticationScheme/Scheme/InitializationVector) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./AuthenticationScheme/Scheme/InitializationVector)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./AuthenticationScheme/Scheme/InitializationVector)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./AuthenticationScheme/Scheme/InitializationVector), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./AuthenticationScheme/Scheme/InitializationVector), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportAuthenticationScheme) = 'ExportMacScheme' and node:name(./ExportAuthenticationScheme/ExportScheme) = 'GMAC' and num:i(string-length(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Initialization Vector used by the Crypto Key Element [!"node:name(.)"!] when authenticated export with GMAC mac scheme */
                    static const uint8 Crypto_au8ExportGmacAuthIV_[!"node:name(.)"!][[!"num:i(string-length(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./AuthenticatedKeyImport) and node:value(./AuthenticatedKeyImport) = 'true' and node:name(./AuthenticationScheme) = 'SignatureScheme' and node:name(./AuthenticationScheme/SchemeInfo) = 'EDDSA' and num:i(string-length(./AuthenticationScheme/SchemeInfo/Context)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Context used by the Crypto Key Element [!"node:name(.)"!] when authenticated with a EDDSA signature scheme */
                    static const uint8 Crypto_au8EddsaAuthContext_[!"node:name(.)"!][[!"num:i(string-length(./AuthenticationScheme/SchemeInfo/Context) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./AuthenticationScheme/SchemeInfo/Context)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./AuthenticationScheme/SchemeInfo/Context)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./AuthenticationScheme/SchemeInfo/Context), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./AuthenticationScheme/SchemeInfo/Context), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
            [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportAuthenticationScheme) = 'ExportSignatureScheme' and node:name(./ExportAuthenticationScheme/ExportSchemeInfo) = 'EDDSA' and num:i(string-length(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext)) != 0"!][!//
                [!INDENT "0"!]
                [!CODE!][!//
                    /* Array storing the Context used by the Crypto Key Element [!"node:name(.)"!] when authenticated with a EDDSA signature scheme */
                    static const uint8 Crypto_au8ExportEddsaAuthContext_[!"node:name(.)"!][[!"num:i(string-length(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext) div 2)"!]U] =
                    {
                [!ENDCODE!][!//
                [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext)))" STEP "num:i(2)"!][!//
                    [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext)))"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext), $Index, 2)"!]
                        [!ENDCODE!][!//
                    [!ELSE!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext), $Index, 2)"!],
                        [!ENDCODE!][!//
                    [!ENDIF!][!//
                [!ENDFOR!][!//
                [!ENDINDENT!][!//
                [!INDENT "0"!][!//
                [!CODE!][!//
                    };

                [!ENDCODE!][!//
                [!ENDINDENT!]
            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
#define CRYPTO_STOP_SEC_CONST_8
#include "Crypto_MemMap.h"

[!NOCODE!][!//
[!IF "node:exists(CryptoKeyElements)"!][!//
    [!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
        [!INDENT "0"!]
        [!CODE!][!//
        #define CRYPTO_START_SEC_CONFIG_DATA_8_NO_CACHEABLE
        #include "Crypto_MemMap.h"

        [!ENDCODE!][!//
        [!ENDINDENT!]
        [!LOOP "node:CryptoKeyElements/CryptoKeyElement/*"!][!//
            [!IF "'true'  "!][!//
                [!IF "node:value(./UseHseKey) = 'true' and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme'"!][!//
                    [!IF "num:i(string-length(./ExportCipherScheme/ExportAadHeaderData)) != 0"!][!//
                        [!INDENT "0"!]
                        [!CODE!][!//
                            /* Array storing the Additional Data (AAD) used by the Crypto Key Element [!"node:name(.)"!] when exported encrypted with a Authenticated cipher */
                            static const uint8 Crypto_au8ExportAeadCipherAAD_[!"node:name(.)"!][[!"num:i(string-length(./ExportCipherScheme/ExportAadHeaderData) div 2)"!]U] =
                            {
                        [!ENDCODE!][!//
                        [!ENDINDENT!][!//
                        [!INDENT "4"!][!//
                        [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./ExportCipherScheme/ExportAadHeaderData)))" STEP "num:i(2)"!][!//
                            [!IF "num:i($Index + 1) = num:i(string-length(node:value(./ExportCipherScheme/ExportAadHeaderData))) "!][!//
                                [!CODE!][!//
                                    0x[!"substring(node:value(./ExportCipherScheme/ExportAadHeaderData), $Index, 2)"!]
                                [!ENDCODE!][!//
                            [!ELSE!][!//
                                [!CODE!][!//
                                    0x[!"substring(node:value(./ExportCipherScheme/ExportAadHeaderData), $Index, 2)"!],
                                [!ENDCODE!][!//
                            [!ENDIF!][!//
                        [!ENDFOR!][!//
                        [!ENDINDENT!][!//
                        [!INDENT "0"!][!//
                        [!CODE!][!//
                            };

                        [!ENDCODE!][!//
                        [!ENDINDENT!]
                    [!ENDIF!][!//
                [!ENDIF!][!//
                [!IF "node:value(./UseHseKey) = 'true' and node:exists(./AuthenticatedKeyImport) and node:value(./AuthenticatedKeyImport) = 'true' and num:i(string-length(node:value(./KeyContainer/AuthenticationTag0))) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                    /* Array storing the Tag (MAC, RSA signature, first part of the signature (r,s) for ECDSA or EDDSA) used by the Crypto Key Element [!"node:name(.)"!] when authenticated */
                    static const uint8 Crypto_au8AuthTag0_[!"node:name(.)"!][[!"num:i(string-length(./KeyContainer/AuthenticationTag0) div 2)"!]U] =
                    {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./KeyContainer/AuthenticationTag0)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./KeyContainer/AuthenticationTag0)))"!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./KeyContainer/AuthenticationTag0), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./KeyContainer/AuthenticationTag0), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                    };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
                [!IF "node:value(./UseHseKey) = 'true' and node:exists(./AuthenticatedKeyImport) and node:value(./AuthenticatedKeyImport) = 'true' and num:i(string-length(node:value(./KeyContainer/AuthenticationTag1))) != 0"!][!//
                    [!INDENT "0"!]
                    [!CODE!][!//
                    /* Array storing the second Tag (second part of the signature (r,s) for ECDSA or EDDSA) used by the Crypto Key Element [!"node:name(.)"!] when authenticated */
                    static const uint8 Crypto_au8AuthTag1_[!"node:name(.)"!][[!"num:i(string-length(./KeyContainer/AuthenticationTag1) div 2)"!]U] =
                    {
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "4"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./KeyContainer/AuthenticationTag1)))" STEP "num:i(2)"!][!//
                        [!IF "num:i($Index + 1) = num:i(string-length(node:value(./KeyContainer/AuthenticationTag1)))"!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./KeyContainer/AuthenticationTag1), $Index, 2)"!]
                            [!ENDCODE!][!//
                        [!ELSE!][!//
                            [!CODE!][!//
                                0x[!"substring(node:value(./KeyContainer/AuthenticationTag1), $Index, 2)"!],
                            [!ENDCODE!][!//
                        [!ENDIF!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                    [!INDENT "0"!][!//
                    [!CODE!][!//
                    };

                    [!ENDCODE!][!//
                    [!ENDINDENT!]
                [!ENDIF!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!INDENT "0"!][!//
        [!CODE!][!//
        #define CRYPTO_STOP_SEC_CONFIG_DATA_8_NO_CACHEABLE
        #include "Crypto_MemMap.h"

        [!ENDCODE!][!//
        [!ENDINDENT!]
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!IF "node:exists(CryptoKeyElements)"!][!//
#define CRYPTO_START_SEC_CONST_32
#include "Crypto_MemMap.h"

[!IF "node:exists(CryptoKeys)"!][!//
    [!LOOP "node:order(CryptoKeys/CryptoKey/*,'node:value(./CryptoKeyId)')"!][!//
        [!CODE!][!//
        [!INDENT "0"!]
        /* Array of indexes for each Crypto Key Element referred by Crypto Key [!"node:name(.)"!] */
        static const uint32 Crypto_au32KeyElementList_[!"node:name(.)"!][[!"num:i(count(node:ref(node:value(./CryptoKeyTypeRef))/CryptoKeyElementRef/*))"!]U] =
        {
        [!ENDINDENT!]
        [!ENDCODE!][!//
        [!VAR "CryptoLoopIt" = "num:i(count(node:ref(node:value(./CryptoKeyTypeRef))/CryptoKeyElementRef/*))"!][!//
        [!LOOP "node:ref(node:value(./CryptoKeyTypeRef))/CryptoKeyElementRef/*"!]
            [!VAR "CryptoLoopIt" = "$CryptoLoopIt - 1"!][!//
            [!IF "$CryptoLoopIt != 0"!][!//
                [!CODE!][!//
                [!INDENT "4"!]
                [!"node:ref(.)/@index"!]U,
                [!ENDINDENT!][!//
                [!ENDCODE!][!//
            [!ELSE!][!//
                [!CODE!][!//
                [!INDENT "4"!]
                [!"node:ref(.)/@index"!]U
                [!ENDINDENT!][!//
                [!ENDCODE!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!CODE!][!//
        [!INDENT "0"!]
        };
        [!ENDINDENT!]
        [!CR!]
        [!ENDCODE!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
#define CRYPTO_STOP_SEC_CONST_32
#include "Crypto_MemMap.h"
[!ENDIF!][!//

#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(./CryptoDriverObjectId)')"!][!//
    [!INDENT "0"!]
    /* Array storing the Crypto primitives in the Crypto Driver Object [!"node:name(.)"!] */
    static const Crypto_PrimitiveType Crypto_aPrimitives_[!"node:name(.)"!][[!"num:i(count(CryptoPrimitiveRef/*))"!]U] =
    {
    [!ENDINDENT!][!//
    [!VAR "CryptoLoopIt" = "num:i(count(CryptoPrimitiveRef/*))"!][!//
    [!LOOP "node:order(CryptoPrimitiveRef/*)"!][!//
        [!INDENT "4"!]
        {
        [!ENDINDENT!][!//
        [!INDENT "8"!]
        [!"node:value(as:ref(node:value(.))/CryptoPrimitiveService)"!],
        (uint8)[!"node:value(as:ref(node:value(.))/CryptoPrimitiveAlgorithmFamily)"!],
        (uint8)[!"node:value(as:ref(node:value(.))/CryptoPrimitiveAlgorithmMode)"!],
        (uint8)[!"node:value(as:ref(node:value(.))/CryptoPrimitiveAlgorithmSecondaryFamily)"!]
        [!ENDINDENT!]
        [!INDENT "4"!]
        [!IF "$CryptoLoopIt != 1"!][!//
            },
        [!ELSE!][!//
            }
        [!ENDIF!][!//
        [!ENDINDENT!][!//
        [!VAR "CryptoLoopIt" = "$CryptoLoopIt - 1"!][!//
    [!ENDLOOP!][!//
    [!INDENT "0"!]
    };

    [!ENDINDENT!][!//
[!ENDLOOP!]
[!IF "node:exists(CryptoKeyElements)"!][!//
    [!LOOP "node:CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./UseHseKey) = 'true'  and node:exists(./EncryptedKeyImport) and node:value(./EncryptedKeyImport) = 'true'"!][!//
            [!INDENT "0"!]
            /* Structure needed for encrypted import of the HSE key associated with the Key Element [!"node:name(.)"!] */
            static const Crypto_CipherSchemeType Crypto_CipherScheme_[!"node:name(.)"!] =
            {
            [!ENDINDENT!][!//
            [!INDENT "4"!]
                /* Cipher algorithm */
                [!IF "node:name(./CipherScheme) = 'SymmetricCipher'"!][!//
                    HSE_CIPHER_[!"node:value(./CipherScheme/HseCipherAlgo)"!],
                [!ELSEIF "node:name(./CipherScheme) = 'RSACipherScheme'"!][!//
                    [!IF "node:name(./CipherScheme/RsaScheme) = 'RSAES_OAEP'"!][!//
                        HSE_RSA_[!"node:value(./CipherScheme/RsaScheme/HseCipherAlgo)"!],
                    [!ELSEIF "node:name(./CipherScheme/RsaScheme) = 'RSAES_PKCS1_V15'"!][!//
                        HSE_RSA_[!"node:value(./CipherScheme/RsaScheme/HseCipherAlgo)"!],
                    [!ENDIF!][!//
                [!ELSE!][!//
                    HSE_CIPHER_ALGO_NULL,
                [!ENDIF!][!//
                /* Cipher block mode in case a symmetric cipher is used */
                [!IF "node:name(./CipherScheme) = 'SymmetricCipher'"!][!//
                    HSE_CIPHER_[!"node:value(./CipherScheme/HseCipherBlockMode)"!],
                [!ELSE!][!//
                    HSE_CIPHER_BLOCK_MODE_NULL,
                [!ENDIF!][!//
                /* Initialization Vector length in case a symmetric cipher is used */
                [!IF "node:name(./CipherScheme) = 'SymmetricCipher' and num:i(string-length(./CipherScheme/InitializationVector)) != 0"!][!//
                    [!"num:i(string-length(./CipherScheme/InitializationVector) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Initialization Vector in case a symmetric cipher is used */
                [!IF "node:name(./CipherScheme) = 'SymmetricCipher' and num:i(string-length(./CipherScheme/InitializationVector)) != 0"!][!//
                    Crypto_au8SymCipherIV_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Authenticated cipher mode */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and ((node:value(node:refs(./EncryptedImportHseKeyCatalogGroupRef)/KeyType)) = 'AES')"!][!//
                    HSE_AUTH_[!"node:value(./CipherScheme/HseAeadCipherMode)"!],
                [!ELSE!][!//
                    (hseAuthCipherMode_t)0U,
                [!ENDIF!][!//
                /* Tag in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/Tag)) != 0"!][!//
                    [!"num:i(string-length(./CipherScheme/Tag) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Tag pointer in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/Tag)) != 0"!][!//
                    Crypto_au8AeadCipherTag_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Initialization Vector length in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/AeadInitializationVector)) != 0"!][!//
                    [!"num:i(string-length(./CipherScheme/AeadInitializationVector) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Initialization Vector in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/AeadInitializationVector)) != 0"!][!//
                    Crypto_au8AeadCipherIV_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Additional Data length in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/AadHeaderData)) != 0"!][!//
                    [!"num:i(string-length(./CipherScheme/AadHeaderData) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Additional Data in case a Authenticated cipher is used */
                [!IF "node:name(./CipherScheme) = 'AeadCipherScheme' and num:i(string-length(./CipherScheme/AadHeaderData)) != 0"!][!//
                    Crypto_au8AeadCipherAAD_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Hash algorithm in case a RSA OAEP cipher is used */
                [!IF "node:name(./CipherScheme) = 'RSACipherScheme' and node:name(./CipherScheme/RsaScheme) = 'RSAES_OAEP'"!][!//
                    HSE_HASH_[!"node:value(./CipherScheme/RsaScheme/HseHashAlgo)"!],
                [!ELSE!][!//
                    HSE_HASH_ALGO_NULL,
                [!ENDIF!][!//
                /* Label length in case a RSA OAEP cipher is used */
                [!IF "node:name(./CipherScheme) = 'RSACipherScheme' and node:name(./CipherScheme/RsaScheme) = 'RSAES_OAEP' and num:i(string-length(./CipherScheme/RsaScheme/Label)) != 0"!][!//
                    [!"num:i(string-length(./CipherScheme/RsaScheme/Label) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Label in case a RSA OAEP cipher is used */
                [!IF "node:name(./CipherScheme) = 'RSACipherScheme' and node:name(./CipherScheme/RsaScheme) = 'RSAES_OAEP' and num:i(string-length(./CipherScheme/RsaScheme/Label)) != 0"!][!//
                    Crypto_au8RsaOaepCipherLabel_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Encrypted key len in bits */
                [!"node:value(./EncryptedHseKeyBitLen)"!]U
            [!ENDINDENT!][!//
            [!INDENT "0"!][!//
            };
            [!ENDINDENT!][!//

        [!ENDIF!][!//
        [!IF "node:value(./UseHseKey) = 'true'  and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES')"!][!//
            [!INDENT "0"!]
            /* Structure needed for encrypted symmetric key export of the HSE key associated with the Key Element [!"node:name(.)"!] */
            static const Crypto_ExportCipherSchemeType Crypto_ExportCipherScheme_[!"node:name(.)"!] =
            {
            [!ENDINDENT!][!//
            [!INDENT "4"!]
                /* Cipher algorithm */
                [!IF "node:name(./ExportCipherScheme) = 'ExportSymmetricCipher'"!][!//
                    HSE_CIPHER_[!"node:value(./ExportCipherScheme/ExportHseCipherAlgo)"!],
                [!ELSEIF "node:name(./ExportCipherScheme) = 'ExportRSACipherScheme'"!][!//
                    [!IF "node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesOaep'"!][!//
                        HSE_RSA_[!"node:value(./ExportCipherScheme/ExportRsaScheme/ExportHseCipherAlgo)"!],
                    [!ELSEIF "node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesPkcs1V15'"!][!//
                        HSE_RSA_[!"node:value(./ExportCipherScheme/ExportRsaScheme/ExportHseCipherAlgo)"!],
                    [!ENDIF!][!//
                [!ELSE!][!//
                    HSE_CIPHER_ALGO_NULL,
                [!ENDIF!][!//
                /* Cipher block mode in case a symmetric cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportSymmetricCipher'"!][!//
                    HSE_CIPHER_[!"node:value(./ExportCipherScheme/ExportHseCipherBlockMode)"!],
                [!ELSE!][!//
                    HSE_CIPHER_BLOCK_MODE_NULL,
                [!ENDIF!][!//
                /* Initialization Vector length in case a symmetric cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportSymmetricCipher' and num:i(string-length(./ExportCipherScheme/ExportInitializationVector)) != 0"!][!//
                    [!"num:i(string-length(./ExportCipherScheme/ExportInitializationVector) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Initialization Vector in case a symmetric cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportSymmetricCipher' and num:i(string-length(./ExportCipherScheme/ExportInitializationVector)) != 0"!][!//
                    Crypto_au8ExportSymCipherIV_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Authenticated cipher mode in case a Authenticated cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme' and ((node:value(node:refs(./EncryptedExportHseKeyCatalogGroupRef)/KeyType)) = 'AES')"!][!//
                    HSE_AUTH_[!"node:value(./ExportCipherScheme/ExportHseAeadCipherMode)"!],
                [!ELSE!][!//
                    (hseAuthCipherMode_t)0U,
                [!ENDIF!][!//
                /* Initialization Vector length in case a Authenticated cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme' and num:i(string-length(./ExportCipherScheme/ExportAeadInitializationVector)) != 0"!][!//
                    [!"num:i(string-length(./ExportCipherScheme/ExportAeadInitializationVector) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Initialization Vector in case a Authenticated cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme' and num:i(string-length(./ExportCipherScheme/ExportAeadInitializationVector)) != 0"!][!//
                    Crypto_au8ExportAeadCipherIV_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Additional Data length in case a Authenticated cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme' and num:i(string-length(./ExportCipherScheme/ExportAadHeaderData)) != 0"!][!//
                    [!"num:i(string-length(./ExportCipherScheme/ExportAadHeaderData) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Additional Data in case a Authenticated cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportAeadCipherScheme' and num:i(string-length(./ExportCipherScheme/ExportAadHeaderData)) != 0"!][!//
                    Crypto_au8ExportAeadCipherAAD_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Hash algorithm in case a RSA OAEP cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportRSACipherScheme' and node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesOaep'"!][!//
                    HSE_HASH_[!"node:value(./ExportCipherScheme/ExportRsaScheme/ExportHseHashAlgo)"!],
                [!ELSE!][!//
                    HSE_HASH_ALGO_NULL,
                [!ENDIF!][!//
                /* Label length in case a RSA OAEP cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportRSACipherScheme' and node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesOaep' and num:i(string-length(./ExportCipherScheme/ExportRsaScheme/ExportLabel)) != 0"!][!//
                    [!"num:i(string-length(./ExportCipherScheme/ExportRsaScheme/ExportLabel) div 2)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* Pointer to array storing the Label in case a RSA OAEP cipher is used */
                [!IF "node:name(./ExportCipherScheme) = 'ExportRSACipherScheme' and node:name(./ExportCipherScheme/ExportRsaScheme) = 'ExportRsaesOaep' and num:i(string-length(./ExportCipherScheme/ExportRsaScheme/ExportLabel)) != 0"!][!//
                    Crypto_au8ExportRsaOaepCipherLabel_[!"node:name(.)"!]
                [!ELSE!][!//
                    NULL_PTR
                [!ENDIF!][!//
            [!ENDINDENT!][!//
            [!INDENT "0"!][!//
            };
            [!ENDINDENT!][!//

        [!ENDIF!][!//
        [!IF "node:value(./UseHseKey) = 'true'  and node:exists(./AuthenticatedKeyImport) and node:value(./AuthenticatedKeyImport) = 'true'"!][!//
            [!IF "node:name(./AuthenticationScheme)='MacScheme'"!][!//
                [!INDENT "0"!]
                /* Structure needed for authenticated import using MAC scheme of the HSE key associated with the Key Element [!"node:name(.)"!] */
                static const Crypto_AuthMacSchemeType Crypto_AuthMacScheme_[!"node:name(.)"!] =
                {
                [!ENDINDENT!][!//
                    [!INDENT "4"!]
                    /* MAC algorithm in case a MAC authentication scheme is used */
                    HSE_MAC_ALGO_[!"node:name(./AuthenticationScheme/Scheme)"!],
                    /* Algorithm in case a CMAC authentication scheme is used */
                    [!IF "node:name(./AuthenticationScheme/Scheme)='CMAC'"!]
                        HSE_CIPHER_[!"node:value(./AuthenticationScheme/Scheme/CipherAlgorithm)"!],
                    [!ELSE!]
                        HSE_CIPHER_ALGO_NULL,
                    [!ENDIF!]
                    /* Hash algorithm in case a HMAC authentication scheme is used */
                    [!IF "node:name(./AuthenticationScheme/Scheme)='HMAC'"!]
                        HSE_HASH_[!"node:value(./AuthenticationScheme/Scheme/HashAlgo)"!],
                    [!ELSE!]
                        HSE_HASH_ALGO_NULL,
                    [!ENDIF!]
                    [!IF "node:name(./AuthenticationScheme/Scheme)='GMAC' and num:i(string-length(./AuthenticationScheme/Scheme/InitializationVector)) != 0"!]
                        /* Length of the Initialization Vector in case a GMAC authentication scheme is used */
                        [!"num:i(string-length(./AuthenticationScheme/Scheme/InitializationVector) div 2)"!]U,
                        /* Pointer to array storing the Initialization Vector in case a GMAC authentication scheme is used */
                        Crypto_au8GmacAuthIV_[!"node:name(.)"!]
                    [!ELSE!]
                        /* Length of the Initialization Vector in case a GMAC authentication scheme is used */
                        0U,
                        /* Pointer to array storing the Initialization Vector in case a GMAC authentication scheme is used */
                        NULL_PTR
                    [!ENDIF!]
                [!ENDINDENT!][!//
                [!INDENT "0"!]
                };

                [!ENDINDENT!][!//
            [!ELSEIF "node:name(./AuthenticationScheme)='SignatureScheme'"!][!//
                [!INDENT "0"!]
                /* Structure needed for authenticated import using signature scheme of the HSE key associated with the Key Element [!"node:name(.)"!] */
                static const Crypto_AuthSignSchemeType Crypto_AuthSignScheme_[!"node:name(.)"!] =
                {
                [!ENDINDENT!][!//
                    [!INDENT "4"!]
                    /* Signature scheme in case the authentication is done using a signature */
                    HSE_SIGN_[!"node:name(./AuthenticationScheme/SchemeInfo)"!],
                    [!IF "node:name(./AuthenticationScheme/SchemeInfo) = 'EDDSA' and num:i(string-length(./AuthenticationScheme/SchemeInfo/Context)) != 0"!]
                        /* Length of the Context in case authentication with a EDDSA signature scheme is used */
                        [!"num:i(string-length(./AuthenticationScheme/SchemeInfo/Context) div 2)"!]U,
                        /* Pointer to array storing the Context in case authentication with a EDDSA signature scheme is used */
                        Crypto_au8EddsaAuthContext_[!"node:name(.)"!],
                    [!ELSE!]
                        /* Length of the Context in case authentication with a EDDSA signature scheme is used */
                        0U,
                        /* Pointer to array storing the Context in case authentication with a EDDSA signature scheme is used */
                        NULL_PTR,
                    [!ENDIF!]
                    /* Boolean flag stating if HSE should pre-hash the input and perform a HashEddsa signature, in case EDDSA signature scheme is used */
                    [!IF "node:name(./AuthenticationScheme/SchemeInfo) = 'EDDSA'"!]
                        [!IF "node:value(./AuthenticationScheme/SchemeInfo/HashEddsa)"!][!//
                            (boolean)TRUE,
                        [!ELSE!][!//
                            (boolean)FALSE,
                        [!ENDIF!][!//
                    [!ELSE!]
                        (boolean)FALSE,
                    [!ENDIF!]
                    /* Salt Length in case authentication with a RSASSA_PSS signature scheme is used */
                    [!IF "node:name(./AuthenticationScheme/SchemeInfo) = 'RSASSA_PSS'"!]
                        [!"node:value(./AuthenticationScheme/SchemeInfo/SaltLength)"!]U,
                    [!ELSE!]
                        0U,
                    [!ENDIF!]
                    /* Hash algorithm in case authentication with signature scheme is used */
                    [!IF "node:name(./AuthenticationScheme/SchemeInfo) != 'EDDSA'"!]
                        HSE_HASH_[!"node:value(./AuthenticationScheme/SchemeInfo/HashAlgo)"!]
                    [!ELSE!]
                        HSE_HASH_ALGO_NULL
                    [!ENDIF!]
                [!ENDINDENT!][!//
                [!INDENT "0"!]
                };

                [!ENDINDENT!][!//
            [!ENDIF!][!//
            [!INDENT "0"!]
            /* Structure storing the Container information for authenticating the Crypto key element: [!"node:name(.)"!] */
            static const Crypto_AuthContainerType Crypto_AuthContainer_[!"node:name(.)"!] =
            {
            [!ENDINDENT!][!//
                [!INDENT "4"!][!//
                /* Container length in bytes */
                [!"node:value(./KeyContainer/KeyContainerLength)"!]U,
                /* Container address */
                [!"num:inttohex(node:value(./KeyContainer/pKeyContainer),8)"!]U,
                /* Authentication tag length */
                [!"num:i(string-length(./KeyContainer/AuthenticationTag0) div 2)"!]U,
                /* Pointer to array storing the authentication tag */
                [!IF "num:i(string-length(./KeyContainer/AuthenticationTag0)) != 0"!][!//
                    Crypto_au8AuthTag0_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Second authentication tag length */
                [!"num:i(string-length(./KeyContainer/AuthenticationTag1) div 2)"!]U,
                /* Pointer to array storing the second authentication tag */
                [!IF "num:i(string-length(./KeyContainer/AuthenticationTag1)) != 0"!][!//
                    Crypto_au8AuthTag1_[!"node:name(.)"!]
                [!ELSE!][!//
                    NULL_PTR
                [!ENDIF!][!//
                [!ENDINDENT!][!//
            [!INDENT "0"!][!//
            };
            [!ENDINDENT!][!//

        [!ENDIF!][!//
        [!IF "node:value(./UseHseKey) = 'true'  and node:exists(./KeyExport) and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) != 'ExportAeadCipherScheme' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES' or ./AuthenticatedExportPublicKey = 'true')"!][!//
            [!IF "node:name(./ExportAuthenticationScheme)='ExportMacScheme'"!][!//
                [!INDENT "0"!]
                /* Structure needed for authenticated export using MAC scheme of the HSE key associated with the Key Element [!"node:name(.)"!] */
                static const Crypto_AuthMacSchemeType Crypto_ExportAuthMacScheme_[!"node:name(.)"!] =
                {
                [!ENDINDENT!][!//
                    [!INDENT "4"!]
                    /* MAC algorithm in case a MAC authentication scheme is used */
                    HSE_MAC_ALGO_[!"node:name(./ExportAuthenticationScheme/ExportScheme)"!],
                    /* Algorithm in case a CMAC authentication scheme is used */
                    [!IF "node:name(./ExportAuthenticationScheme/ExportScheme)='CMAC'"!]
                        HSE_CIPHER_[!"node:value(./ExportAuthenticationScheme/ExportScheme/ExportCipherAlgorithm)"!],
                    [!ELSE!]
                        HSE_CIPHER_ALGO_NULL,
                    [!ENDIF!]
                    /* Hash algorithm in case a HMAC authentication scheme is used */
                    [!IF "node:name(./ExportAuthenticationScheme/ExportScheme)='HMAC'"!]
                        HSE_HASH_[!"node:value(./ExportAuthenticationScheme/ExportScheme/ExportHashAlgo)"!],
                    [!ELSE!]
                        HSE_HASH_ALGO_NULL,
                    [!ENDIF!]
                    [!IF "node:name(./ExportAuthenticationScheme/ExportScheme)='GMAC' and num:i(string-length(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector)) != 0"!]
                        /* Length of the Initialization Vector in case a GMAC authentication scheme is used */
                        [!"num:i(string-length(./ExportAuthenticationScheme/ExportScheme/ExportInitializationVector) div 2)"!],
                        /* Pointer to array storing the Initialization Vector in case a GMAC authentication scheme is used */
                        Crypto_au8ExportGmacAuthIV_[!"node:name(.)"!]
                    [!ELSE!]
                        /* Length of the Initialization Vector in case a GMAC authentication scheme is used */
                        0U,
                        /* Pointer to array storing the Initialization Vector in case a GMAC authentication scheme is used */
                        NULL_PTR
                    [!ENDIF!]
                    [!ENDINDENT!][!//
                [!INDENT "0"!]
                };
                [!ENDINDENT!][!//
            [!ELSEIF "node:name(./ExportAuthenticationScheme)='ExportSignatureScheme'"!][!//
                [!INDENT "0"!]
                /* Structure needed for authenticated export using signature scheme of the HSE key associated with the Key Element [!"node:name(.)"!] */
                static const Crypto_AuthSignSchemeType Crypto_ExportAuthSignScheme_[!"node:name(.)"!] =
                {
                [!ENDINDENT!][!//
                    [!INDENT "4"!]
                    /* Signature scheme in case the authentication is done using a signature */
                    HSE_SIGN_[!"node:name(./ExportAuthenticationScheme/ExportSchemeInfo)"!],
                    [!IF "node:name(./ExportAuthenticationScheme/ExportSchemeInfo) = 'EDDSA' and num:i(string-length(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext)) != 0"!]
                        /* Length of the Context in case authentication with a EDDSA signature scheme is used */
                        [!"num:i(string-length(./ExportAuthenticationScheme/ExportSchemeInfo/ExportContext) div 2)"!]U,
                        /* Pointer to array storing the Context in case authentication with a EDDSA signature scheme is used */
                        Crypto_au8ExportEddsaAuthContext_[!"node:name(.)"!],
                    [!ELSE!]
                        /* Length of the Context in case authentication with a EDDSA signature scheme is used */
                        0U,
                        /* Pointer to array storing the Context in case authentication with a EDDSA signature scheme is used */
                        NULL_PTR,
                    [!ENDIF!]
                    /* Boolean flag stating if HSE should pre-hash the input and perform a HashEddsa signature, in case EDDSA signature scheme is used */
                    [!IF "node:name(./ExportAuthenticationScheme/ExportSchemeInfo) = 'EDDSA'"!]
                         [!IF "node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportHashEddsa)"!][!//
                            (boolean)TRUE,
                        [!ELSE!][!//
                            (boolean)FALSE,
                        [!ENDIF!][!//
                    [!ELSE!]
                        (boolean)FALSE,
                    [!ENDIF!]
                    /* Salt Length in case authentication with a RSASSA_PSS signature scheme is used */
                    [!IF "node:name(./ExportAuthenticationScheme/ExportSchemeInfo) = 'RSASSA_PSS'"!]
                        [!"node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportSaltLength)"!]U,
                    [!ELSE!]
                        0U,
                    [!ENDIF!]
                    /* Hash algorithm in case authentication with signature scheme is used */
                    [!IF "node:name(./ExportAuthenticationScheme/ExportSchemeInfo) != 'EDDSA'"!]
                        HSE_HASH_[!"node:value(./ExportAuthenticationScheme/ExportSchemeInfo/ExportHashAlgo)"!]
                    [!ELSE!]
                        HSE_HASH_ALGO_NULL
                    [!ENDIF!]
                    [!ENDINDENT!][!//
                [!INDENT "0"!]
                };
                [!ENDINDENT!][!//

            [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
[!ENDIF!][!//
#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
[!NOCODE!][!//
[!IF "node:exists(CryptoKeyElements)"!][!//
    /* Create the uint8 array with the information that needs to be stored for all non-persistent Key Elements (buffers + actual sizes) */
    /* Count first the max size of the information about the non-persistent (volatile) key elements. Need to be known when to append commas after each value */
    [!VAR "SizeOfKeyElemVolatileValues"="0"!][!//
    [!VAR "KeyElemVolatileValuesHaveAllBytesZero"="'true'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'false' "!][!//
            /* If the key element does not use a HSE key, add the max size of the key element buffer + 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemVolatileValues" = "$SizeOfKeyElemVolatileValues + node:value(./CryptoKeyElementSize) + num:i(4)"!][!//
            [!IF "num:i(string-length(node:value(./CryptoKeyElementInitValue)))!=0"!][!//
                [!VAR "KeyElemVolatileValuesHaveAllBytesZero"="'false'"!][!//
            [!ENDIF!][!//
        [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog' "!][!//
            /* If the key element uses a HSE key and it is a Ram key, add the 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemVolatileValues" = "$SizeOfKeyElemVolatileValues + num:i(4)"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$SizeOfKeyElemVolatileValues > 0"!][!//
        [!IF "$KeyElemVolatileValuesHaveAllBytesZero = 'true'"!][!//
            [!INDENT "0"!]
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing the key element information that is volatile (no need to be persistent across resets) */
                static uint8 Crypto_au8VolatileKeyElemValues[[!"num:i($SizeOfKeyElemVolatileValues)"!]U];

                #define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
            [!ENDINDENT!][!//
        [!ELSE!][!// IF "$KeyElemVolatileValuesHaveAllBytesZero = 'true'"
            [!INDENT "0"!]
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing the key element information that is volatile (no need to be persistent across resets) */
                static uint8 Crypto_au8VolatileKeyElemValues[[!"num:i($SizeOfKeyElemVolatileValues)"!]U] =
                {
            [!ENDCODE!][!//
            [!ENDINDENT!][!//
            [!VAR "CurIdx"="0"!][!//
            [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
                [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'false' "!][!//
                    /* If the key element does not use a HSE key, add the 4 bytes storing the 32bit value of the actual size + the max size buffer of the key element */
                    [!INDENT "4"!]
                    [!CODE!][!//
                        /* Offset [!"num:i($CurIdx)"!] - The following 4 bytes store the actual length of the key element [!"node:name(.)"!] */
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),0)),255),2)"!]U,
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),8)),255),2)"!]U,
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),16)),255),2)"!]U,
                        [!"num:inttohex(bit:and((bit:shr(num:i(string-length(node:value(./CryptoKeyElementInitValue)) div 2),24)),255),2)"!]U,
                    [!ENDCODE!][!//
                    [!VAR "CurIdx" = "$CurIdx + 4"!][!//
                    [!CODE!][!//
                        /* Offset [!"num:i($CurIdx)"!] - The following [!"num:i(node:value(./CryptoKeyElementSize))"!] bytes store the buffer of the key element [!"node:name(.)"!] */
                    [!ENDCODE!][!//
                    [!VAR "Index" = "num:i(-1)"!][!//
                    [!FOR "Index" = "num:i(1)" TO "num:i(string-length(node:value(./CryptoKeyElementInitValue)))" STEP "num:i(2)"!][!//
                        [!VAR "CurIdx" = "$CurIdx + 1"!][!//
                        [!CODE!][!//
                            0x[!"substring(node:value(./CryptoKeyElementInitValue), $Index, 2)"!]U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemVolatileValues)"!],[!ENDIF!][!CR!]
                        [!ENDCODE!][!//
                    [!ENDFOR!][!//
                    [!VAR "CountEl" = "$Index"!][!//
                    [!FOR "Index" = "num:i($CountEl+2)" TO "num:i(2 * node:value(./CryptoKeyElementSize))" STEP "num:i(2)"!][!//
                        [!VAR "CurIdx" = "$CurIdx + 1"!][!//
                        [!CODE!][!//
                            0x00U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemVolatileValues)"!],[!ENDIF!][!CR!]
                        [!ENDCODE!][!//
                    [!ENDFOR!][!//
                    [!ENDINDENT!][!//
                [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog' "!][!//
                    /* If the key element uses a HSE key and it is a Ram key, add the 4 bytes storing the 32bit value of the actual size */
                    [!"keyElemActualSize = num:i(string-length(node:value(./CryptoKeyElementInitValue)))"!][!//
                    [!INDENT "4"!]
                    [!CODE!][!//
                        /* Offset [!"num:i($CurIdx)"!] - The following 4 bytes store the actual length of the key element [!"node:name(.)"!] */
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U,
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U,
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U,
                        [!VAR "CurIdx" = "$CurIdx + 1"!]0x00U[!IF "num:i($CurIdx) < num:i($SizeOfKeyElemVolatileValues)"!],[!ENDIF!][!CR!]
                    [!ENDCODE!][!//
                    [!ENDINDENT!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!INDENT "0"!]
            [!CODE!][!//
                };

                #define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
            [!ENDINDENT!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!NOCODE!][!//
[!// Loop first through all Crypto Driver Objects checking if at least one has the jobs queue enabled
[!VAR "MemMapSectionVarNoInitUnspecifiedNeeded"="'false'"!][!//
[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(./CryptoDriverObjectId)')"!][!//
    [!IF "node:value(./CryptoQueueSize) != 0"!][!//
        [!VAR "MemMapSectionVarNoInitUnspecifiedNeeded"="'true'"!][!//
    [!ENDIF!][!//
[!ENDLOOP!]
[!IF "$MemMapSectionVarNoInitUnspecifiedNeeded = 'true'"!][!//
    [!INDENT "0"!][!//
    [!CODE!][!//
    #define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Crypto_MemMap.h"

    [!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(./CryptoDriverObjectId)')"!][!//
        [!IF "node:value(./CryptoQueueSize) != 0"!][!//
            /* Array storing the queued jobs for the Crypto Driver Object [!"node:name(.)"!] */
            static Crypto_QueueElementType Crypto_aQueuedJobs_[!"node:name(.)"!][[!"node:value(./CryptoQueueSize)"!]U];
        [!ENDIF!][!//
    [!ENDLOOP!]

    #define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Crypto_MemMap.h"
    [!ENDCODE!][!//
    [!ENDINDENT!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_8
#include "Crypto_MemMap.h"

[!INDENT "0"!][!//
[!VAR "MaxNumPartitions" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
    /* Array storing the mapping of the MU instance per partition */
    const Crypto_PartitionToMuMappingType Crypto_aPartitionToMuMapping[CRYPTO_MAX_NUMBER_PARTITIONS_U8] =
    {
[!INDENT "4"!][!//
[!IF "num:i(count(CryptoGeneral/CryptoEcucPartitionRef/*)) > 0"!][!//
    [!VAR "IdxUsedMuInstance" = "-1"!][!//
    [!FOR "x" = "0" TO "num:i($MaxNumPartitions - 1)"!][!//
        [!VAR "PartitionMatched" = "'false'"!][!//
        [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
            [!VAR "UsedMu" = "concat('CRYPTO_', MuInstance, '_U8')"!][!//
            [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
                [!VAR "CurCdoPartition" = "node:value(.)"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                        [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                            [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                                [!IF "$x = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!][!//
                                    [!VAR "PartitionMatched" = "'true'"!][!//
                                    [!VAR "IdxUsedMuInstance" = "$IdxUsedMuInstance + 1"!][!//
                                [!ENDIF!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDLOOP!][!//
            [!IF "$PartitionMatched = 'true'"!][!//
                [!BREAK!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$PartitionMatched = 'true'"!][!//
            {[!"$UsedMu"!], [!"num:inttohex($IdxUsedMuInstance,2)"!]U}[!IF "$x < ($MaxNumPartitions -1)"!],[!ENDIF!][!CR!][!//
        [!ELSE!][!//
            {0xFFU, 0xFFU}[!IF "$x < ($MaxNumPartitions -1)"!],[!ENDIF!][!CR!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!//
[!ELSE!][!//
    {CRYPTO_[!"CryptoDriverObjects/CryptoDriverObject/*[1]/MuInstance"!]_U8, 0x00U}
[!ENDIF!][!//
[!ENDINDENT!][!//
    };
[!ENDINDENT!][!//

#define CRYPTO_STOP_SEC_CONST_8
#include "Crypto_MemMap.h"


#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

[!INDENT "0"!][!//
/* Array of structures storing the information of the Crypto Driver Objects mapped on partitions */
const Crypto_PartitionToCdoMappingType Crypto_aPartitionToCdoMapping[CRYPTO_MAX_NUMBER_PARTITIONS_U8] =
{
[!IF "num:i(count(CryptoGeneral/CryptoEcucPartitionRef/*)) > 0"!][!//
    [!VAR "MaxNumPartitions" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
    [!FOR "x" = "0" TO "num:i($MaxNumPartitions - 1)"!][!//
        [!VAR "NumMappedCdos" = "0"!][!//
        [!LOOP "CryptoDriverObjects/CryptoDriverObject/*"!][!//
            [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
                [!VAR "CurCdoPartition" = "node:value(.)"!][!//
                [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                    [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                        [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                            [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                                [!IF "$x = node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!][!//
                                    [!VAR "NumMappedCdos" = "$NumMappedCdos + 1"!][!//
                                [!ENDIF!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDLOOP!][!//
        [!ENDLOOP!][!//
        [!INDENT "4"!][!//
            /* Structure storing the information of the Crypto Driver Objects mapped on partition [!"as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*[num:i($x) + 1]/@name"!] */
            {
        [!INDENT "8"!][!//
        [!IF "$NumMappedCdos > 0"!][!//
            /* Number of Crypto Driver Objects */
            [!"num:i($NumMappedCdos)"!]U,
            /* Reference to array storing the indexes of the Crypto Driver Objects */
            Crypto_au8CDOs_[!"as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*[num:i($x) + 1]/@name"!]
        [!ELSE!][!//
            /* Number of Crypto Driver Objects */
            0U,
            /* Reference to array storing the indexes of the Crypto Driver Objects */
            NULL_PTR
        [!ENDIF!][!//
        [!ENDINDENT!][!//
            }[!IF "$x < ($MaxNumPartitions -1)"!],[!ENDIF!][!CR!][!//
        [!ENDINDENT!][!//
    [!ENDFOR!][!//
[!ELSE!][!//
    [!INDENT "4"!][!//
    /* Structure storing the information of the Crypto Driver Objects */
    {
    [!INDENT "8"!][!//
        /* Number of Crypto Driver Objects */
        [!"num:i(count(CryptoDriverObjects/CryptoDriverObject/*))"!]U,
        /* Reference to array storing the indexes of the Crypto Driver Objects */
        Crypto_au8CDOs
    [!ENDINDENT!][!//
    }
    [!ENDINDENT!][!//
[!ENDIF!][!//
};
[!ENDINDENT!][!//

[!INDENT "0"!]
    /* Array of structures storing the information about the Crypto Driver Objects */
    const Crypto_ObjectType Crypto_aDriverObjectList[CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32] =
    {
[!ENDINDENT!][!//
[!VAR "CryptoLoopIt" = "count(CryptoDriverObjects/CryptoDriverObject/*)"!][!//
[!LOOP "node:order(CryptoDriverObjects/CryptoDriverObject/*,'node:value(./CryptoDriverObjectId)')"!][!//
    [!VAR "CurCdoName" = "node:name(.)"!][!//
    [!VAR "CryptoLoopIt" = "$CryptoLoopIt - 1"!][!//
    [!INDENT "4"!]
    /* Structure storing the information about Crypto Driver Object [!"node:name(.)"!] */
    {
    [!ENDINDENT!]
    [!INDENT "8"!]
        [!LOOP "CryptoDriverObjectEcucPartitionRef/*"!][!//
            [!VAR "PartitionMatched" = "'false'"!][!//
            [!VAR "CurCdoPartition" = "node:value(.)"!][!//
            [!LOOP "as:modconf('Os')[1]/OsApplication/*"!][!//
                [!IF "node:refvalid(./OsAppEcucPartitionRef)"!][!//
                    [!IF "$CurCdoPartition = node:value(./OsAppEcucPartitionRef)"!][!//
                        [!IF "node:refvalid(./OsApplicationCoreRef)"!][!//
                            [!VAR "PartitionMatched" = "'true'"!][!//
                            /* Id of the partition that is valid for the Crypto Driver Object */
                            [!"node:value(node:ref(./OsApplicationCoreRef)/EcucCoreId)"!]U,
                            [!BREAK!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDIF!][!//
            [!ENDLOOP!][!//
            [!IF "$PartitionMatched = 'false'"!][!//
                [!ERROR!][!//
                    The referenced ECUC partition '[!"$CurCdoPartition"!]' in the CryptoEcucPartitionRef list of the Crypto Driver Object '[!"$CurCdoName"!]' is not associated with any OsApplication or the OsApplication does not have a valid OsApplicationCoreRef!
                [!ENDERROR!][!//
            [!ENDIF!][!//
        [!ENDLOOP!][!//
        /* Reference to the jobs queue */
        [!IF "node:value(./CryptoQueueSize)!= 0"!][!//
            Crypto_aQueuedJobs_[!"node:name(.)"!],
        [!ELSE!][!//
            NULL_PTR,
        [!ENDIF!][!//
        /* Jobs queue size */
        [!"node:value(./CryptoQueueSize)"!]U,
        /* Reference to the Crypto primitives list */
        Crypto_aPrimitives_[!"node:name(.)"!],
        /* Number of crypto primitives */
        [!"num:i(count(CryptoPrimitiveRef/*))"!]U
        [!INDENT "4"!]
            [!IF "$CryptoLoopIt != 0"!][!//
                },
            [!ELSE!][!//
                }
            [!ENDIF!][!//
        [!ENDINDENT!][!//
    [!ENDINDENT!][!//
[!ENDLOOP!][!//
[!INDENT "0"!]
};
[!ENDINDENT!][!//

[!IF "node:exists(CryptoKeys)"!][!//
    [!INDENT "0"!]
        /* Array of structures storing the information about the Crypto Keys */
        const Crypto_KeyType Crypto_aKeyList[CRYPTO_NUMBER_OF_KEYS_U32] =
        {
    [!ENDINDENT!][!//
    [!VAR "CryptoLoopIt" = "count(CryptoKeys/CryptoKey/*)"!][!//
    [!LOOP "node:order(CryptoKeys/CryptoKey/*,'node:value(./CryptoKeyId)')"!][!//
        [!VAR "CryptoLoopIt" = "$CryptoLoopIt - 1"!][!//
        [!INDENT "4"!]
        {
        [!ENDINDENT!]
        [!INDENT "8"!]
            /* Number of key elements in the key [!"node:name(.)"!] */
            [!"num:i(count(node:ref(node:value(./CryptoKeyTypeRef))/CryptoKeyElementRef/*))"!]U,
            /* Reference to the list of key elements in the key [!"node:name(.)"!] */
            Crypto_au32KeyElementList_[!"node:name(.)"!]
        [!ENDINDENT!][!//
        [!INDENT "4"!]
        [!IF "$CryptoLoopIt != 0"!][!//
            },
        [!ELSE!][!//
            }
        [!ENDIF!][!//
        [!ENDINDENT!][!//
    [!ENDLOOP!][!//
[!INDENT "0"!]
};
[!ENDINDENT!][!//
[!ENDIF!][!//

[!IF "node:exists(CryptoKeyElements)"!][!//
    [!INDENT "0"!]
        /* Array of structures storing the information about the Crypto Key Elements */
        const Crypto_KeyElementType Crypto_aKeyElementList[CRYPTO_NUMBER_OF_KEY_ELEMENTS_U32] =
        {
    [!ENDINDENT!][!//
    [!VAR "CryptoLoopIt" = "count(CryptoKeyElements/CryptoKeyElement/*)"!][!//
    [!VAR "CurIdxInPersistentArray"="0"!][!//
    [!VAR "CurIdxInVolatileArray"="0"!][!//
    [!LOOP "node:CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!VAR "CryptoLoopIt" = "$CryptoLoopIt - 1"!][!//
        [!INDENT "4"!]
        /* Structure containing information for Key Element [!"node:name(.)"!] */
        {
        [!ENDINDENT!]
        [!INDENT "8"!]
            /* KeyElementId */
            [!"node:value(./CryptoKeyElementId)"!]U,
            /* Allow partial access */
            [!IF "node:value(./CryptoKeyElementAllowPartialAccess)"!][!//
                (boolean)TRUE,
            [!ELSE!][!//
                (boolean)FALSE,
            [!ENDIF!][!//
            /* Key element format */
            [!"text:replaceAll(node:value(./CryptoKeyElementFormat), 'CRYPTO_', concat('CRYPTO', '', '_'))"!],
            /* Key element persistent */
            [!IF "node:value(./UseHseKey)='true'"!][!//
                [!IF "node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog'"!][!//
                    (boolean)TRUE,
                [!ELSE!][!//
                    (boolean)FALSE,
                [!ENDIF!][!//
            [!ELSE!][!//
                [!IF "node:value(./CryptoKeyElementPersist)"!][!//
                    (boolean)TRUE,
                [!ELSE!][!//
                    (boolean)FALSE,
                [!ENDIF!][!//
            [!ENDIF!][!//
            /* Read access type */
            [!"text:replaceAll(node:value(./CryptoKeyElementReadAccess), 'CRYPTO_', concat('CRYPTO', '', '_'))"!],
            /* Key element max size */
            [!"node:value(./CryptoKeyElementSize)"!]U,
            /* Write access type */
            [!"text:replaceAll(node:value(./CryptoKeyElementWriteAccess), 'CRYPTO_', concat('CRYPTO', '', '_'))"!],
            [!IF "node:value(./UseHseKey)='false'"!][!//
                [!IF "node:value(./CryptoKeyElementPersist) = 'true' "!][!//
                    /* Pointer to location storing the Key Element actual size */
                    (uint32*)&Crypto_au8NvramBlob1[[!"num:i($CurIdxInPersistentArray)"!]U],
                    [!VAR "CurIdxInPersistentArray" = "$CurIdxInPersistentArray + 4"!][!//
                    /* Pointer to location storing the Key Element value */
                    &Crypto_au8NvramBlob1[[!"num:i($CurIdxInPersistentArray)"!]U],
                    [!VAR "CurIdxInPersistentArray" = "$CurIdxInPersistentArray + node:value(./CryptoKeyElementSize)"!][!//
                [!ELSE!][!//
                    /* Pointer to location storing the Key Element actual size */
                    (uint32*)&Crypto_au8VolatileKeyElemValues[[!"num:i($CurIdxInVolatileArray)"!]U],
                    [!VAR "CurIdxInVolatileArray" = "$CurIdxInVolatileArray + 4"!][!//
                    /* Pointer to location storing the Key Element value */
                    &Crypto_au8VolatileKeyElemValues[[!"num:i($CurIdxInVolatileArray)"!]U],
                    [!VAR "CurIdxInVolatileArray" = "$CurIdxInVolatileArray + node:value(./CryptoKeyElementSize)"!][!//
                [!ENDIF!][!//
            [!ELSE!][!//
                [!IF "(node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') "!][!//
                    /* Pointer to location storing the Key Element actual size */
                    (uint32*)&Crypto_au8NvramBlob1[[!"num:i($CurIdxInPersistentArray)"!]U],
                    [!VAR "CurIdxInPersistentArray" = "$CurIdxInPersistentArray + 4"!][!//
                    /* Pointer to location storing the Key Element value */
                    NULL_PTR,
                [!ELSEIF "(node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') "!][!//
                    /* Pointer to location storing the Key Element actual size */
                    (uint32*)&Crypto_au8VolatileKeyElemValues[[!"num:i($CurIdxInVolatileArray)"!]U],
                    [!VAR "CurIdxInVolatileArray" = "$CurIdxInVolatileArray + 4"!][!//
                    /* Pointer to location storing the Key Element value */
                    NULL_PTR,
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyImport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
                /* Identifier of the cipher key in the key catalogs */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./EncryptedKeyImport) = 'true'"!][!//
                    [!"num:inttohex(num:i(node:name(concat(noderef:ref(EncryptedImportHseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') * (bit:shl(1,16) + bit:shl(num:i(node:ref(EncryptedImportHseKeyCatalogGroupRef)/@index), 8) + EncryptedImportHseKeySlot) + num:i(node:name(concat(noderef:ref(EncryptedImportHseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') * (bit:shl(1,17) + bit:shl(num:i(node:ref(EncryptedImportHseKeyCatalogGroupRef)/@index), 8) + EncryptedImportHseKeySlot), 8)"!]U,
                [!ELSE!][!//
                    (uint32)HSE_INVALID_KEY_HANDLE,
                [!ENDIF!][!//
                /* Reference to the cipher scheme for key import */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./EncryptedKeyImport) = 'true'"!][!//
                    &Crypto_CipherScheme_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Identifier of the authentication key in the key catalogs */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./AuthenticatedKeyImport) = 'true'"!][!//
                    [!"num:inttohex(num:i(node:name(concat(noderef:ref(AuthenticatedImportHseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') * (bit:shl(1,16) + bit:shl(num:i(node:ref(AuthenticatedImportHseKeyCatalogGroupRef)/@index), 8) + AuthenticatedImportHseKeySlot) + num:i(node:name(concat(noderef:ref(AuthenticatedImportHseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') * (bit:shl(1,17) + bit:shl(num:i(node:ref(AuthenticatedImportHseKeyCatalogGroupRef)/@index), 8) + AuthenticatedImportHseKeySlot), 8)"!]U,
                [!ELSE!][!//
                    (uint32)HSE_INVALID_KEY_HANDLE,
                [!ENDIF!][!//
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./AuthenticatedKeyImport) = 'true'"!][!//
                    /* Reference to the authentication MAC scheme for key import */
                    [!IF "node:name(./AuthenticationScheme)='MacScheme'"!][!//
                        &Crypto_AuthMacScheme_[!"node:name(.)"!],
                    [!ELSE!][!//
                        NULL_PTR,
                    [!ENDIF!][!//
                    /* Reference to the authentication signature scheme for key import */
                    [!IF "node:name(./AuthenticationScheme)='SignatureScheme'"!][!//
                        &Crypto_AuthSignScheme_[!"node:name(.)"!],
                    [!ELSE!][!//
                        NULL_PTR,
                    [!ENDIF!][!//
                [!ELSE!][!//
                    /* Reference to the authentication MAC scheme for key import */
                    NULL_PTR,
                    /* Reference to the authentication signature scheme for key import */
                    NULL_PTR,
                [!ENDIF!][!//
                /* Reference to the authentication container for key import */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./AuthenticatedKeyImport) = 'true'"!][!//
                    &Crypto_AuthContainer_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* HSE key counter */
                [!IF "node:value(./UseHseKey) = 'true'"!][!//
                    [!"node:value(./HseKeyCounter)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "ecu:get(concat('Crypto.HseSptEncAuthKeyExport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
                /* Identifier of the export cipher key in the key catalogs */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./KeyExport) = 'true' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES')"!][!//
                    [!"num:inttohex(num:i(node:name(concat(noderef:ref(EncryptedExportHseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') * (bit:shl(1,16) + bit:shl(num:i(node:ref(EncryptedExportHseKeyCatalogGroupRef)/@index), 8) + EncryptedExportHseKeySlot) + num:i(node:name(concat(noderef:ref(EncryptedExportHseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') * (bit:shl(1,17) + bit:shl(num:i(node:ref(EncryptedExportHseKeyCatalogGroupRef)/@index), 8) + EncryptedExportHseKeySlot), 8)"!]U,
                [!ELSE!][!//
                    (uint32)HSE_INVALID_KEY_HANDLE,
                [!ENDIF!][!//
                /* Reference to the cipher scheme for key export, only for symmetric key export */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./KeyExport) = 'true' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES')"!][!//
                    &Crypto_ExportCipherScheme_[!"node:name(.)"!],
                [!ELSE!][!//
                    NULL_PTR,
                [!ENDIF!][!//
                /* Identifier of the export authentication key in the key catalogs */
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme) != 'ExportAeadCipherScheme' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES' or ./AuthenticatedExportPublicKey = 'true')"!][!//
                    [!"num:inttohex(num:i(node:name(concat(noderef:ref(AuthenticatedExportHseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') * (bit:shl(1,16) + bit:shl(num:i(node:ref(AuthenticatedExportHseKeyCatalogGroupRef)/@index), 8) + AuthenticatedExportHseKeySlot) + num:i(node:name(concat(noderef:ref(AuthenticatedExportHseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') * (bit:shl(1,17) + bit:shl(num:i(node:ref(AuthenticatedExportHseKeyCatalogGroupRef)/@index), 8) + AuthenticatedExportHseKeySlot), 8)"!]U,
                [!ELSE!][!//
                    (uint32)HSE_INVALID_KEY_HANDLE,
                [!ENDIF!][!//
                [!IF "node:value(./UseHseKey) = 'true'  and node:value(./KeyExport) = 'true' and node:name(./ExportCipherScheme)!='ExportAeadCipherScheme' and (node:ref(./HseKeyCatalogGroupRef)/KeyType = 'AES' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'HMAC' or node:ref(./HseKeyCatalogGroupRef)/KeyType = 'TDES' or ./AuthenticatedExportPublicKey = 'true')"!][!//
                    /* Reference to the MAC authentication scheme for key export */
                    [!IF "node:name(./ExportAuthenticationScheme)='ExportMacScheme'"!][!//
                        &Crypto_ExportAuthMacScheme_[!"node:name(.)"!],
                    [!ELSE!][!//
                        NULL_PTR,
                    [!ENDIF!][!//
                    /* Reference to the signature authentication scheme for key export */
                    [!IF "node:name(./ExportAuthenticationScheme)='ExportSignatureScheme'"!][!//
                        &Crypto_ExportAuthSignScheme_[!"node:name(.)"!],
                    [!ELSE!][!//
                        NULL_PTR,
                    [!ENDIF!][!//
                [!ELSE!][!//
                    /* Reference to the MAC authentication scheme for key export */
                    NULL_PTR,
                    /* Reference to the signature authentication scheme for key export */
                    NULL_PTR,
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "ecu:get(concat('Crypto.HseSptEccKeyImport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
                /* ECC curve id */
                [!IF "node:value(./UseHseKey) = 'true' "!][!//
                    HSE_EC_[!"substring(node:value(./HseEccCurveId),5)"!],
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "ecu:get(concat('Crypto.HseSptSymKeyImport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                  ecu:get(concat('Crypto.HseSptRsaKeyImport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                  ecu:get(concat('Crypto.HseSptEccKeyImport.', string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                  ecu:get(concat('Crypto.HseSptKeyDerive.',    string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON'"!][!//
                /* Flags of the HSE key */
                [!IF "node:value(./UseHseKey) = 'true' "!][!//
                    ((uint16)([!LOOP "HseKeyFlags/*"!]HSE_KF_[!"node:value(HseKeyFlag)"!] | [!ENDLOOP!]0U)),
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
                /* HSE key SMR flags */
                [!IF "node:value(./UseHseKey) = 'true' "!][!//
                    [!"num:inttohex(node:value(./HseSMRFlags),8)"!]U,
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
            [!ENDIF!][!//
            [!IF "(ecu:get(concat('Crypto.HseSptKeyImport.',         string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                   ecu:get(concat('Crypto.HseSptKeyExport.',         string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                   ecu:get(concat('Crypto.HseSptKeyDerive.',         string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON' or
                   ecu:get(concat('Crypto.HseSptKeyGenerate.',       string(node:fallback(./../../../CryptoGeneral/HseFwType,'CUSTOM')))) = 'STD_ON')"!][!//
                /* HSE key type */
                [!IF "node:value(./UseHseKey) = 'true' "!][!//
                    (uint8)HSE_KEY_TYPE_[!"node:value(node:ref(HseKeyCatalogGroupRef)/KeyType)"!],
                [!ELSE!][!//
                    0U,
                [!ENDIF!][!//
            [!ELSE!][!//
                0U,
            [!ENDIF!][!//
            /* Identifier of the HSE key in the key catalogs */
            [!IF "node:value(./UseHseKey) = 'true'"!][!//
                [!IF "'true' "!][!//
                    [!"num:inttohex(num:i(node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog') * (bit:shl(1,16) + bit:shl(num:i(node:ref(HseKeyCatalogGroupRef)/@index), 8) + HseKeySlot) + num:i(node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'RamKeyCatalog') * (bit:shl(1,17) + bit:shl(num:i(node:ref(HseKeyCatalogGroupRef)/@index), 8) + HseKeySlot), 8)"!]U
                [!ELSE!][!//
                [!ENDIF!][!//
            [!ELSE!][!//
                (uint32)HSE_INVALID_KEY_HANDLE
            [!ENDIF!][!//
        [!ENDINDENT!][!//
        [!INDENT "4"!]
        [!IF "$CryptoLoopIt != 0"!][!//
            },
        [!ELSE!][!//
            }
        [!ENDIF!][!//
        [!ENDINDENT!][!//
    [!ENDLOOP!][!//
[!INDENT "0"!]
};
[!ENDINDENT!][!//

    [!IF "node:exists(CryptoGeneral/CryptoUpdateNvramBlobHandler)"!][!//
    [!INDENT "0"!][!//
        /* Pointer to the upper layer handler that will update in Nvram blobs of persistent information */
        const Crypto_UpdateNvramBlobHandlerType pfCryptoUpdateNvramBlobHandler = &[!"CryptoGeneral/CryptoUpdateNvramBlobHandler"!];

    [!ENDINDENT!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!//
[!//
[!IF "ecu:get(concat('Crypto.HseSptFormatKeyCatalogs.', string(node:fallback(CryptoGeneral/HseFwType,'STANDARD')))) = 'STD_ON'"!][!//
/* Table containing NVM key catalog entries */
const hseKeyGroupCfgEntry_t aHseNvmKeyCatalog[] =
{
[!NOCODE!][!//
    [!LOOP "NvmKeyCatalog/*"!][!//
        [!VAR "IdxItem"="0"!][!//
        [!VAR "NumOfListItems"= "num:i(count(MuMask/*))-1"!][!//
        [!VAR "StrMuMask"="''"!][!//
        [!LOOP "MuMask/*"!][!//
            [!VAR "StrMuValue"="concat('HSE_MU', substring(./MU,4,1), '_MASK')"!][!//
            [!IF "$IdxItem<$NumOfListItems"!][!//
                [!VAR "StrMuMask"="concat($StrMuMask, $StrMuValue, ' | ')"!][!//
            [!ELSE!][!//
                [!VAR "StrMuMask"="concat($StrMuMask, $StrMuValue)"!][!//
            [!ENDIF!][!//
            [!VAR "IdxItem"= "$IdxItem+1"!][!//
        [!ENDLOOP!][!//
        [!CODE!][!WS"4"!]/* [!"node:name(.)"!] */[!ENDCODE!][!CR!][!//
        [!CODE!][!WS"4"!]{([!"$StrMuMask"!]), HSE_KEY_[!"node:value(KeyOwner)"!], HSE_KEY_TYPE_[!"node:value(KeyType)"!], [!"num:i(node:value(NumOfKeySlots))"!]U, [!"num:i(node:value(MaxKeyBitLen))"!]U, {0U, 0U}},[!ENDCODE!][!CR!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
    /* Marker to end the key catalog */
    {0U, 0U, 0U, 0U, 0U, {0U, 0U}}
};

/* Table containing RAM key catalog entries */
const hseKeyGroupCfgEntry_t aHseRamKeyCatalog[] =
{
[!NOCODE!][!//
    [!LOOP "RamKeyCatalog/*"!][!//
        [!VAR "IdxItem"="0"!][!//
        [!VAR "NumOfListItems"= "num:i(count(MuMask/*))-1"!][!//
        [!VAR "StrMuMask"="''"!][!//
        [!LOOP "MuMask/*"!][!//
            [!VAR "StrMuValue"="concat('HSE_MU', substring(./MU,4,1), '_MASK')"!][!//
            [!IF "$IdxItem<$NumOfListItems"!][!//
                [!VAR "StrMuMask"="concat($StrMuMask, $StrMuValue, ' | ')"!][!//
            [!ELSE!][!//
                [!VAR "StrMuMask"="concat($StrMuMask, $StrMuValue)"!][!//
            [!ENDIF!][!//
            [!VAR "IdxItem"= "$IdxItem+1"!][!//
        [!ENDLOOP!][!//
        [!CODE!][!WS"4"!]/* [!"node:name(.)"!] */[!ENDCODE!][!CR!][!//
        [!CODE!][!WS"4"!]{([!"$StrMuMask"!]), HSE_KEY_[!"node:value(KeyOwner)"!], HSE_KEY_TYPE_[!"node:value(KeyType)"!], [!"num:i(node:value(NumOfKeySlots))"!]U, [!"num:i(node:value(MaxKeyBitLen))"!]U, {0U, 0U}},[!ENDCODE!][!CR!][!//
    [!ENDLOOP!][!//
[!ENDNOCODE!][!//
    /* Marker to end the key catalog */
    {0U, 0U, 0U, 0U, 0U, {0U, 0U}}
};

[!ENDIF!][!//
#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
[!NOCODE!][!//
/* When key exists but user does not enable the NVM storage, Crypto driver will keep internally in RAM the 2 blobs */
[!IF "node:exists(CryptoKeyElements) and not(node:exists(CryptoGeneral/CryptoUpdateNvramBlobHandler))"!][!//
    [!INDENT "0"!][!//
    [!CODE!][!//
    #define CRYPTO_START_SEC_VAR_CLEARED_8
    #include "Crypto_MemMap.h"

    /* Array of 8 bit blocks storing a bit map of valid flags for each Crypto key */
    uint8 Crypto_au8NvramBlob0[CRYPTO_SIZEOF_NVRAM_BLOB_0];

    #define CRYPTO_STOP_SEC_VAR_CLEARED_8
    #include "Crypto_MemMap.h"

    [!ENDCODE!][!//
    /* Create the uint8 array with the information that needs to be stored in Nvram for all persistent Key Elements (buffers + actual sizes) */
    /* Count first the max size of the blob storing information about the persistent key elements. Need to be known when to append commas after each value */
    [!VAR "SizeOfKeyElemPersistValuesBlob"="0"!][!//
    [!VAR "KeyElemPersistBlobHasAllBytesZero"="'true'"!][!//
    [!LOOP "CryptoKeyElements/CryptoKeyElement/*"!][!//
        [!IF "node:value(./UseHseKey)='false' and node:value(./CryptoKeyElementPersist) = 'true' "!][!//
            /* If the key element does not use a HSE key, add the max size of the key element buffer + 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + node:value(./CryptoKeyElementSize) + num:i(4)"!][!//
            [!IF "num:i(string-length(node:value(./CryptoKeyElementInitValue)))!=0"!][!//
                [!VAR "KeyElemPersistBlobHasAllBytesZero"="'false'"!][!//
            [!ENDIF!][!//
        [!ELSEIF "node:value(./UseHseKey)='true' and node:name(concat(noderef:ref(HseKeyCatalogGroupRef), '/../.')) = 'NvmKeyCatalog' "!][!//
            /* If the key element uses a HSE key and it is a Nvm key, add the 4 bytes storing the 32bit value of the actual size */
            [!VAR "SizeOfKeyElemPersistValuesBlob" = "$SizeOfKeyElemPersistValuesBlob + num:i(4)"!][!//
        [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$SizeOfKeyElemPersistValuesBlob > 0"!][!//
        [!IF "$KeyElemPersistBlobHasAllBytesZero = 'true'"!][!//
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing blob 1 containing information that should be persistent between resets */
                uint8 Crypto_au8NvramBlob1[CRYPTO_SIZEOF_NVRAM_BLOB_1];

                #define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
        [!ELSE!][!// IF "$KeyElemPersistBlobHasAllBytesZero = 'true'"
            [!CODE!][!//
                #define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

                /* Array storing blob 1 containing information that should be persistent between resets */
                uint8 Crypto_au8NvramBlob1[CRYPTO_SIZEOF_NVRAM_BLOB_1] = CRYPTO_NVRAM_BLOB_1_INITIALIZER;

                #define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
                #include "Crypto_MemMap.h"

            [!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
    [!ENDINDENT!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//
[!//
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

Crypto_ObjectQueueType Crypto_aObjectQueueList[CRYPTO_NUMBER_OF_DRIVER_OBJECTS_U32];

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */


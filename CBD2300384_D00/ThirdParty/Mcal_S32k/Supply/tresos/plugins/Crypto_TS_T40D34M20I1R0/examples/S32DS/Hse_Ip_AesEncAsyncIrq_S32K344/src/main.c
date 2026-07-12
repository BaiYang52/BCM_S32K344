/*
*   (c) Copyright 2021 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Hse_Ip.h"
#include "OsIf.h"
#include "IntCtrl_Ip.h"
#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/* Enumeration storing the type of encryption requests performed in this example */
typedef enum
{
    ENCRYPT_SYNC = 0UL,
    ENCRYPT_ASYNC_IRQ,
}App_EncryptionType;

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
/* Function-like macros to set and get the status of the example run */
#define App_SetSuccessStatus(value)         (u32NumFailedOperations += ((value) ? 0U : 1U))
#define App_GetSuccessStatus()              (0U == u32NumFailedOperations)

/* Identifier for the only MU instance used in this example (MU0) */
#define MU0_INSTANCE_U8                     ((uint8)0U)

/* Identifier of the MU channel used by HSE to listen for administrative requests (eg. read/write attributes) */
#define MU_ADMIN_CHANNEL_U8                 ((uint8)0U)

/* Timeout for the Hse_Ip layer while waiting for a response from HSE for a synchronous request.
   As the Hse component is configured in this example to have the type of Timeout Counter set to 'OSIF_COUNTER_DUMMY',
the timeout in the variable below will be expressed in ticks */
#define TIMEOUT_TICKS_U32                   ((uint32)10000000U)

/* Size in bytes of the AES128 Encrypt/Decrypt key */
#define APP_SHE_KEY_SIZE                    (16U)

/* Size in bytes of the plain text that will be used to test encryption */
#define APP_AES128_ECB_PLAIN_TEXT_SIZE      (16U)

/* Size in bytes of the cipher text that will result after encryption */
#define APP_AES128_ECB_CIPHER_TEXT_SIZE     (16U)

/* Identifier of the HSE key in the key catalog: RAM catalog, group 0, slot 0 */
#define SHE_RAM_KEY_HSE_KEY_HANDLE          (0x00020000U)

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#define CRYPTO_START_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Table containing NVM key catalog entries */
const hseKeyGroupCfgEntry_t Hse_aNvmKeyCatalog[] =
{
    /* NvmKeyGroup_MASTER_ECU_KEY__BOOT_MAC_KEY__Key1_To_Key10 */
    {(HSE_MU0_MASK), HSE_KEY_OWNER_ANY, HSE_KEY_TYPE_SHE, 12U, 128U, {0U, 0U}},
    /* Marker to end the key catalog */
    {0U, 0U, 0U, 0U, 0U, {0U, 0U}}
};

/* Table containing RAM key catalog entries */
const hseKeyGroupCfgEntry_t Hse_aRamKeyCatalog[] =
{
    /* RamKeyGroup_RamKey */
    {(HSE_MU0_MASK), HSE_KEY_OWNER_ANY, HSE_KEY_TYPE_SHE, 1U, 128U, {0U, 0U}},
    /* Marker to end the key catalog */
    {0U, 0U, 0U, 0U, 0U, {0U, 0U}}
};

#define CRYPTO_STOP_SEC_CONST_UNSPECIFIED
#include "Crypto_MemMap.h"

/* Array storing the expected cipherText resulted when encrypting the plainText stored in App_au8Aes128EcbPlaintext[] array with
the key stored in App_au8SheRamKey[] array */
static const uint8 App_au8Aes128ExpectedEcbCiphertext[APP_AES128_ECB_CIPHER_TEXT_SIZE] =
{
    0x9c, 0x54, 0xd5, 0x71, 0x70, 0x2c, 0xfa, 0x0f, 0x03, 0xf3, 0x62, 0x15, 0x67, 0x6b, 0xab, 0x78
};

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/* Variable that will store the internal state machine of the Hse Ip layer. As this application uses only MU0 to communicate
   with HSE, there is no need for an array of such variables.
   The address of the variable storing the internal state machine should be provided as parameter when the Hse Ip layer
is initialized on one MU instance. */
static Hse_Ip_MuStateType HseIp_MuState;

/* Array of variables used to store the application requests to Hse Ip layer. This example uses both synchronous and asynchronous
calls to Hse Ip, made only on MU0 instance. For this reason, we will need to have a request variable for each of the channels of the MU. */
static Hse_Ip_ReqType HseIp_aRequest[HSE_NUM_OF_CHANNELS_PER_MU];

/* Variable storing the number of successful responses received from HSE for asynchronous requests using interrupts. A response is successful if:
    - HseResponse parameter of the callback is set to HSE_SRV_RSP_OK and
    - the buffer containing the ciper for the channel that triggered the callback contains the expected cipherText */
static uint8 u8NumSuccessfulHseResponses = 0;

/* The variables in the section below need to be put in non-cacheable memory area in order to be correctly seen by the HSE */
#define CRYPTO_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Variable where HSE will write (return) the supported capabilities */
static hseAttrCapabilities_t Hse_AttrCapabilities;

/* Variable where application will store the request of configuring (enabling/disabling) the MU instances */
static hseAttrMUConfig_t     Hse_MuConfig;

#define CRYPTO_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

#define CRYPTO_START_SEC_VAR_SHARED_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Array of variables used to store the descriptors containing the service request for HSE firmware. This example uses both synchronous and
asynchronous calls to Hse Ip, made only on MU0 instance. For this reason, we need to have a descriptor variable available for each of the channels in the MU0.
   The descriptors should be placed in the shared memory, in a non-cacheable area */
static hseSrvDescriptor_t   Hse_aSrvDescriptor[HSE_NUM_OF_CHANNELS_PER_MU];

#define CRYPTO_STOP_SEC_VAR_SHARED_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE */
#define CRYPTO_START_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Array storing the value of the key */
static uint8 App_au8SheRamKey[APP_SHE_KEY_SIZE] __attribute__((aligned)) =
{
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

/* Array storing the value of the plainText to encrypt */
static uint8 App_au8Aes128EcbPlaintext[APP_AES128_ECB_PLAIN_TEXT_SIZE] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

#define CRYPTO_STOP_SEC_VAR_INIT_8_NO_CACHEABLE
#include "Crypto_MemMap.h"


/* The following RAM variables need to be put in non-cacheable memory in order to be correctly seen by HSE */
#define CRYPTO_START_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/* Array storing the value of the cipherText resulted after a synchronous encryption request */
static uint8  App_au8SyncAes128EcbCipherText[APP_AES128_ECB_CIPHER_TEXT_SIZE];

/* Array storing the value of the cipherText resulted after asynchronous encryption. Because there is a test that will send asynchronous encryption
requests on all channels of MU0, we need to keep a result buffer for each MU channel, in order to test that the encryption
operation produced the expected result on each channel */
static uint8  App_au8AsyncAes128EcbCipherText[HSE_NUM_OF_CHANNELS_PER_MU][APP_AES128_ECB_CIPHER_TEXT_SIZE];

#define CRYPTO_STOP_SEC_VAR_CLEARED_8_NO_CACHEABLE
#include "Crypto_MemMap.h"

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void Hse_Ip_RxCallback
(
    uint8            u8MuInstance,
    uint8            u8MuChannel,
    hseSrvResponse_t HseResponse,
    void*            pCallbackParam
);

static hseSrvResponse_t App_GetHseAttrCapabilities
(
    const uint8            u8MuInstance,
    hseAttrCapabilities_t* pHseAttrCapabilities
);

static hseSrvResponse_t App_ActivateAllMuInstances(void);

static hseSrvResponse_t App_FormatHseKeyCatalogs(void);

static hseSrvResponse_t App_SheLoadPlainRamKey(void);

static hseSrvResponse_t App_Aes128EncryptEcb
(
    App_EncryptionType encryptionType
);

/* Utility functions */
static boolean Util_Memcmp
(
    uint8* pSource,
    uint8* pDest,
    uint32 u32Size
);

static void Util_Memset
(
    void*  pDest,
    uint8  u8Value,
    uint32 u32Size
);

static void Util_DelayTicks
(
    uint32 numTicks
);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/* Callback for asynchronous requests sent to Hse_Ip layer */
static void Hse_Ip_RxCallback
(
    uint8            u8MuInstance,
    uint8            u8MuChannel,
    hseSrvResponse_t HseResponse,
    void*            pCallbackParam
)
{
    if((HSE_SRV_RSP_OK == HseResponse) &&
       (Util_Memcmp((uint8*)App_au8AsyncAes128EcbCipherText[u8MuChannel], (uint8*)App_au8Aes128ExpectedEcbCiphertext, APP_AES128_ECB_CIPHER_TEXT_SIZE)))
    {
        u8NumSuccessfulHseResponses++;
    }

    /* Clear the buffer where the cipherText will be updated by HSE, in case this channel will be re-used */
    Util_Memset(App_au8AsyncAes128EcbCipherText[u8MuChannel], 0U, APP_AES128_ECB_CIPHER_TEXT_SIZE);

    /* Ignore compiler warning for not used parameters */
    (void)u8MuInstance;
    (void)pCallbackParam;
}

static hseSrvResponse_t App_GetHseAttrCapabilities
(
    const uint8            u8MuInstance,
    hseAttrCapabilities_t* pHseAttrCapabilities
)
{
    hseSrvDescriptor_t* pHseSrvDescriptor;

    /* Fill the descriptor for the HSE request. Because the request to get an attribute is an administrative one,
    it should be sent over channel 0 */
    pHseSrvDescriptor = &Hse_aSrvDescriptor[MU_ADMIN_CHANNEL_U8];

    pHseSrvDescriptor->srvId                     = HSE_SRV_ID_GET_ATTR;
    pHseSrvDescriptor->hseSrv.getAttrReq.attrId  = HSE_CAPABILITIES_ATTR_ID;
    pHseSrvDescriptor->hseSrv.getAttrReq.attrLen = sizeof(hseAttrCapabilities_t);
    pHseSrvDescriptor->hseSrv.getAttrReq.pAttr   = HSE_PTR_TO_HOST_ADDR(pHseAttrCapabilities);

    /* Build the request to be sent to Hse Ip layer */
    HseIp_aRequest[MU_ADMIN_CHANNEL_U8].eReqType   = HSE_IP_REQTYPE_SYNC;
    HseIp_aRequest[MU_ADMIN_CHANNEL_U8].u32Timeout = TIMEOUT_TICKS_U32;

    /* Send the request to Hse Ip layer */
    return Hse_Ip_ServiceRequest(u8MuInstance, MU_ADMIN_CHANNEL_U8, &HseIp_aRequest[MU_ADMIN_CHANNEL_U8], pHseSrvDescriptor);
}

static hseSrvResponse_t App_ActivateAllMuInstances(void)
{
    uint8               u8Index;
    hseSrvDescriptor_t* pHseSrvDescriptor;

    /* MU 0 is by default activated and should stay like this */
    Hse_MuConfig.muInstances[0U].muConfig = HSE_MU_ACTIVATED;

    /* Activate the other MU instances */
    for(u8Index = 1U; u8Index < HSE_NUM_OF_MU_INSTANCES; u8Index++)
    {
        Hse_MuConfig.muInstances[u8Index].muConfig = HSE_MU_ACTIVATED;
    }

    /* Fill the descriptor for the HSE request. Because the request to set an attribute is an administrative one,
    it should be sent over channel 0. */
    pHseSrvDescriptor = &Hse_aSrvDescriptor[MU_ADMIN_CHANNEL_U8];

    pHseSrvDescriptor->srvId                     = HSE_SRV_ID_SET_ATTR;
    pHseSrvDescriptor->hseSrv.setAttrReq.attrId  = HSE_MU_CONFIG_ATTR_ID;
    pHseSrvDescriptor->hseSrv.setAttrReq.attrLen = sizeof(hseAttrMUConfig_t);
    pHseSrvDescriptor->hseSrv.setAttrReq.pAttr   = HSE_PTR_TO_HOST_ADDR(&Hse_MuConfig);

    /* Build the request to be sent to Hse Ip layer */
    HseIp_aRequest[MU_ADMIN_CHANNEL_U8].eReqType   = HSE_IP_REQTYPE_SYNC;
    HseIp_aRequest[MU_ADMIN_CHANNEL_U8].u32Timeout = TIMEOUT_TICKS_U32;

    /* Send the request to Hse Ip layer. As the MU0 is the only one used for now by HSE to listen for requests,
    the request will be sent on MU0 */
    return Hse_Ip_ServiceRequest(MU0_INSTANCE_U8, MU_ADMIN_CHANNEL_U8, &HseIp_aRequest[MU_ADMIN_CHANNEL_U8], pHseSrvDescriptor);
}

static hseSrvResponse_t App_FormatHseKeyCatalogs(void)
{
    hseSrvDescriptor_t* pHseSrvDescriptor;
    hseSrvResponse_t    RetVal      = HSE_SRV_RSP_GENERAL_ERROR;
    uint8               u8MuChannel = Hse_Ip_GetFreeChannel(MU0_INSTANCE_U8);

    if(HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        /* Optimize a bit the code by storing the address of the channel's descriptor in a pointer */
        pHseSrvDescriptor = &Hse_aSrvDescriptor[u8MuChannel];

        /* Create the service request for HSE by setting the descriptor's members */
        pHseSrvDescriptor->srvId                                         = HSE_SRV_ID_FORMAT_KEY_CATALOGS;
        pHseSrvDescriptor->hseSrv.formatKeyCatalogsReq.pNvmKeyCatalogCfg = HSE_PTR_TO_HOST_ADDR(Hse_aNvmKeyCatalog);
        pHseSrvDescriptor->hseSrv.formatKeyCatalogsReq.pRamKeyCatalogCfg = HSE_PTR_TO_HOST_ADDR(Hse_aRamKeyCatalog);

        /* Build the request to be sent to Hse Ip layer */
        HseIp_aRequest[u8MuChannel].eReqType   = HSE_IP_REQTYPE_SYNC;
        HseIp_aRequest[u8MuChannel].u32Timeout = TIMEOUT_TICKS_U32;

        /* Send the request to Hse Ip layer */
        RetVal = Hse_Ip_ServiceRequest(MU0_INSTANCE_U8, u8MuChannel, &HseIp_aRequest[u8MuChannel], pHseSrvDescriptor);
    }

    return RetVal;
}

static hseSrvResponse_t App_SheLoadPlainRamKey(void)
{
    hseSrvDescriptor_t*      pHseSrvDescriptor;
    hseSheLoadPlainKeySrv_t* pSheLoadPlainKeyReq;
    hseSrvResponse_t         RetVal      = HSE_SRV_RSP_GENERAL_ERROR;
    uint8                    u8MuChannel = Hse_Ip_GetFreeChannel(MU0_INSTANCE_U8);

    if(HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        /* Optimize a bit the code */
        pHseSrvDescriptor   = &Hse_aSrvDescriptor[u8MuChannel];
        pSheLoadPlainKeyReq =  &(pHseSrvDescriptor->hseSrv.sheLoadPlainKeyReq);

        /* Create the service request for HSE by setting the descriptor's members */
        pHseSrvDescriptor->srvId  = HSE_SRV_ID_SHE_LOAD_PLAIN_KEY;
        pSheLoadPlainKeyReq->pKey = HSE_PTR_TO_HOST_ADDR(App_au8SheRamKey);

        /* Build the request to be sent to Hse Ip layer */
        HseIp_aRequest[u8MuChannel].eReqType   = HSE_IP_REQTYPE_SYNC;
        HseIp_aRequest[u8MuChannel].u32Timeout = TIMEOUT_TICKS_U32;

        /* Send the request to Hse Ip layer */
        RetVal = Hse_Ip_ServiceRequest(MU0_INSTANCE_U8, u8MuChannel, &HseIp_aRequest[u8MuChannel], pHseSrvDescriptor);
    }

    return RetVal;
}


static hseSrvResponse_t App_Aes128EncryptEcb
(
    App_EncryptionType encryptionType
)
{
    hseSrvDescriptor_t* pHseSrvDescriptor;
    hseSymCipherSrv_t*  pHseEncrypt;
    hseSrvResponse_t    RetVal      = HSE_SRV_RSP_GENERAL_ERROR;
    uint8               u8MuChannel = Hse_Ip_GetFreeChannel(MU0_INSTANCE_U8);

    if(HSE_IP_INVALID_MU_CHANNEL_U8 != u8MuChannel)
    {
        /* Optimize a bit the code */
        pHseSrvDescriptor = &Hse_aSrvDescriptor[u8MuChannel];
        pHseEncrypt       = &(pHseSrvDescriptor->hseSrv.symCipherReq);

        /* Create the service request for HSE by setting the descriptor's members */
        pHseSrvDescriptor->srvId     = HSE_SRV_ID_SYM_CIPHER;
        pHseEncrypt->accessMode      = HSE_ACCESS_MODE_ONE_PASS;
        pHseEncrypt->sgtOption       = HSE_SGT_OPTION_NONE;
        pHseEncrypt->cipherAlgo      = HSE_CIPHER_ALGO_AES;
        pHseEncrypt->cipherDir       = HSE_CIPHER_DIR_ENCRYPT;
        pHseEncrypt->cipherBlockMode = HSE_CIPHER_BLOCK_MODE_ECB;
        pHseEncrypt->inputLength     = (uint32)APP_AES128_ECB_PLAIN_TEXT_SIZE;
        pHseEncrypt->pInput          = HSE_PTR_TO_HOST_ADDR(App_au8Aes128EcbPlaintext);
        pHseEncrypt->keyHandle       = SHE_RAM_KEY_HSE_KEY_HANDLE;

        switch(encryptionType)
        {
            case ENCRYPT_SYNC:
                pHseEncrypt->pOutput = HSE_PTR_TO_HOST_ADDR(App_au8SyncAes128EcbCipherText);

                /* Build the request to be sent to Hse Ip layer */
                HseIp_aRequest[u8MuChannel].eReqType   = HSE_IP_REQTYPE_SYNC;
                HseIp_aRequest[u8MuChannel].u32Timeout = TIMEOUT_TICKS_U32;
                break;

            case ENCRYPT_ASYNC_IRQ:
                pHseEncrypt->pOutput = HSE_PTR_TO_HOST_ADDR(App_au8AsyncAes128EcbCipherText[u8MuChannel]);

                /* Build the request to be sent to Hse Ip layer */
                HseIp_aRequest[u8MuChannel].eReqType   = HSE_IP_REQTYPE_ASYNC_IRQ;
                HseIp_aRequest[u8MuChannel].pfCallback = Hse_Ip_RxCallback;
                break;

            default:
                break;
        }

        /* Send the request to Hse Ip layer */
        RetVal = Hse_Ip_ServiceRequest(MU0_INSTANCE_U8, u8MuChannel, &HseIp_aRequest[u8MuChannel], pHseSrvDescriptor);
    }

    return RetVal;
}

/* Utility functions */
static boolean Util_Memcmp
(
    uint8* pSource,
    uint8* pDest,
    uint32 u32Size
)
{
    boolean bResult      = (boolean)TRUE;
    uint32  u32InputSize = u32Size;

    while (0U < u32InputSize--)
    {
        if (*pSource == *pDest)
        {
            pSource++;
            pDest++;
        }
        else
        {
            bResult = (boolean)FALSE;
            break;
        }
    }
    return bResult;
}

static void Util_Memset
(
    void*  pDest,
    uint8  u8Value,
    uint32 u32Size
)
{
    uint32 u32Counter;
    uint8* ptr = (uint8*)pDest;

    for (u32Counter = 0U; u32Counter < u32Size; u32Counter++)
    {
        *ptr = u8Value;
        ptr++;
    }
}

static void Util_DelayTicks
(
    uint32 numTicks
)
{
    static volatile uint32 u32NumTicks;
    uint32                 u32Counter;

    u32NumTicks = numTicks;

    for (u32Counter = 0; u32Counter < u32NumTicks; u32Counter++);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        MU0 Ored Rx Interrupt handler
* @details      Defined in Mu_Ip_Irq.c and used by Hse_Ip layer to process the interrupts triggered when
                responses are received from HSE for requests made on MU0 instance
*/
extern void Mu_Ip_Mu0_OredRx_Isr(void);

/**
* @brief          Checks that HSE FW is up and running, formats the key catalogs, imports a SHE RAM key and sends synchronous and asynchronous encryption requests to HSE on one MU instance
*
* @details        The code in the main function of the application performs the following steps:
*                  - Initializes the OsIf component. It is used by Hse_Ip layer to determine the timeout while waiting for responses from HSE for synchronous requests
*                  - Installs the handler for treating OredRx interrupt for MU0 instance
*                  - Checks the status of the HSE FW by calling the Hse_Ip_GetHseStatus() API and checking the returned value
*                  - Initializes the Hse Ip layer to work with MU0 instance
*                  - Sends a synchronous request over MU0 to read the HSE FW capabilities and checks that the RANDOM and SHE capabilities are enabled
*                  - Sends a synchronous request to enable the other MU instances, such that HSE will listen to requests sent over all MU instances
*                  - Formats the HSE Nvm and Ram key catalogs in order for each to contain the SHE keys (12 Nvm + 1 Ram). Only one key (the SHE Ram Key) will be used by the example
*                  - Imports the material of the SHE key in the SHE Ram Key slot
*                  - Initiates a synchronous AES128 encryption request and checks that the cipherText is the expected one
*                  - Initiates a bulk of asynchronous using interrupts AES128 encryption requests and checks that the cipherTexts are the expected ones
*
*                 The configuration of the drivers used by this example are done like below:
*                  - Hse component
*                     - Timeout Counter Type set to OSIF_COUNTER_SYSTEM, such that the time passed from the moment a request is made from Hse Ip layer to HSE FW till
*                       the response is received is measured in microseconds
*                  - Osif component
*                     - Use System Time option enabled
*/
int main(void)
{
    hseStatus_t            HseStatus;
    Hse_Ip_StatusType      HseIpStatus;
    hseSrvResponse_t       HseResponse;
    uint8                  u8Counter;
    static volatile uint8  u8NumSuccessfulHseRequests = 0;
    static volatile uint32 u32NumFailedOperations     = 0U;

    /* =============================================================================================================================== */
    /*    Initialize OsIf component. It will be used to measure timeouts while waiting for HSE to respond to synchronous requests      */
    /* =============================================================================================================================== */
    OsIf_Init(NULL_PTR);

    /* =============================================================================================================================== */
    /*    Install and enable the needed interrupt handlers                                                                             */
    /* =============================================================================================================================== */
    /* Install ORed RX interrupt for MU-0 */
    IntCtrl_Ip_InstallHandler(HSE_MU0_RX_IRQn, &Mu_Ip_Mu0_OredRx_Isr, NULL_PTR);
    /* Enable ORed RX interrupt for MU-0 */
    IntCtrl_Ip_EnableIrq(HSE_MU0_RX_IRQn);

    /* =============================================================================================================================== */
    /*    Check that HSE is initialized (along with RNG module) by reading the status bits in FSR                                      */
    /* =============================================================================================================================== */
    HseStatus = Hse_Ip_GetHseStatus(MU0_INSTANCE_U8);
    App_SetSuccessStatus(0U != (HseStatus & HSE_STATUS_INIT_OK));
    App_SetSuccessStatus(0U != (HseStatus & HSE_STATUS_RNG_INIT_OK));

    /* =============================================================================================================================== */
    /*    Initialize Hse Ip layer for MU0 instance                                                                                     */
    /* =============================================================================================================================== */
    HseIpStatus = Hse_Ip_Init(MU0_INSTANCE_U8, &HseIp_MuState);
    App_SetSuccessStatus(HSE_IP_STATUS_SUCCESS == HseIpStatus);

    /* =============================================================================================================================== */
    /*    Read HSE FW Capabilities, by sending a request over MU0                                                                      */
    /* =============================================================================================================================== */
    HseResponse = App_GetHseAttrCapabilities(MU0_INSTANCE_U8, &Hse_AttrCapabilities);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);
    App_SetSuccessStatus(0U != (Hse_AttrCapabilities & (HSE_ALGO_CAP_MASK(HSE_CAP_IDX_RANDOM) | HSE_ALGO_CAP_MASK(HSE_CAP_IDX_SHE))));

    /* =============================================================================================================================== */
    /*    Activate all MU instances. By default, HSE only listens for requests coming on MU0 instance                                  */
    /* =============================================================================================================================== */
    HseResponse = App_ActivateAllMuInstances();
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* =============================================================================================================================== */
    /*    Format HSE Nvm and Ram key catalogs                                                                                          */
    /* =============================================================================================================================== */
    HseResponse = App_FormatHseKeyCatalogs();

    /* =============================================================================================================================== */
    /*    Import a key in the SHE RAM key slot                                                                                         */
    /* =============================================================================================================================== */
    HseResponse = App_SheLoadPlainRamKey();
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);

    /* =============================================================================================================================== */
    /*    Perform a synchronous AES128 encryption                                                                                      */
    /* =============================================================================================================================== */
    /* Clear the buffer where the cipherText will be updated by HSE */
    Util_Memset(App_au8SyncAes128EcbCipherText, 0U, APP_AES128_ECB_CIPHER_TEXT_SIZE);

    HseResponse = App_Aes128EncryptEcb(ENCRYPT_SYNC);
    App_SetSuccessStatus(HSE_SRV_RSP_OK == HseResponse);
    App_SetSuccessStatus(Util_Memcmp((uint8*)App_au8SyncAes128EcbCipherText, (uint8*)App_au8Aes128ExpectedEcbCiphertext, APP_AES128_ECB_CIPHER_TEXT_SIZE));

    /* =============================================================================================================================== */
    /*    Perform a set of asynchronous AES128 encryption requests, on all channels that are available on the MU0 instance             */
    /* =============================================================================================================================== */
    /* Clear the buffer where the cipherText will be updated by HSE, for each MU channel */
    for (u8Counter = 0; u8Counter < HSE_NUM_OF_CHANNELS_PER_MU; u8Counter++)
    {
        Util_Memset(App_au8AsyncAes128EcbCipherText[u8Counter], 0U, APP_AES128_ECB_CIPHER_TEXT_SIZE);
    }
    /* Try to initiate 20 asynchronous requests. Only some of them will find a free channel. Count the ones that do */
    for (u8Counter = 0; u8Counter < 20U; u8Counter++)
    {
        HseResponse = App_Aes128EncryptEcb(ENCRYPT_ASYNC_IRQ);
        if(HSE_SRV_RSP_OK == HseResponse)
        {
            u8NumSuccessfulHseRequests++;
        }
    }
    /* Allow some time (counted in ticks) for the requests to be processed */
    Util_DelayTicks(1000000);
    /* Check that the number of successfully sent requests have been received successful responses */
    App_SetSuccessStatus(u8NumSuccessfulHseRequests == u8NumSuccessfulHseResponses);

    /* =============================================================================================================================== */
    /*    Finish application execution, signaling the status                                                                           */
    /* =============================================================================================================================== */
    Exit_Example(App_GetSuccessStatus());
    return (0U);
}


#ifdef __cplusplus
}
#endif

/** @} */

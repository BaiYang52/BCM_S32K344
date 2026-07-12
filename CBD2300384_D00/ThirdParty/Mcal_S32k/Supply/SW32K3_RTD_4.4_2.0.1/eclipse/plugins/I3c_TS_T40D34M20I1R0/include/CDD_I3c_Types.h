/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
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

#ifndef I3C_TYPES_H
#define I3C_TYPES_H

/**
*   @file
*
*   @addtogroup i3c
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
#include "CDD_I3c_CfgDefines.h"
#include "I3c_Ip_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_VENDOR_ID_TYPES                    43
#define I3C_MODULE_ID_TYPES                    255
#define I3C_AR_RELEASE_MAJOR_VERSION_TYPES     4
#define I3C_AR_RELEASE_MINOR_VERSION_TYPES     4
#define I3C_AR_RELEASE_REVISION_VERSION_TYPES  0
#define I3C_SW_MAJOR_VERSION_TYPES             2
#define I3C_SW_MINOR_VERSION_TYPES             0
#define I3C_SW_PATCH_VERSION_TYPES             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and CDD_I3c_CfgDefines header file are of the same vendor */
#if (I3C_VENDOR_ID_TYPES != I3C_VENDOR_ID_CFGDEFINES)
    #error "CDD_I3c_Types.h and CDD_I3c_CfgDefines.h have different vendor ids"
#endif

/* Check if current file and CDD_I3c_CfgDefines header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_TYPES     != I3C_AR_RELEASE_MAJOR_VERSION_CFGDEFINES) || \
     (I3C_AR_RELEASE_MINOR_VERSION_TYPES     != I3C_AR_RELEASE_MINOR_VERSION_CFGDEFINES) || \
     (I3C_AR_RELEASE_REVISION_VERSION_TYPES  != I3C_AR_RELEASE_REVISION_VERSION_CFGDEFINES))
    #error "AutoSar Version Numbers of CDD_I3c_Types.h and CDD_I3c_CfgDefines.h are different"
#endif

/* Check if current file and CDD_I3c_CfgDefines header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_TYPES != I3C_SW_MAJOR_VERSION_CFGDEFINES) || \
     (I3C_SW_MINOR_VERSION_TYPES != I3C_SW_MINOR_VERSION_CFGDEFINES) || \
     (I3C_SW_PATCH_VERSION_TYPES != I3C_SW_PATCH_VERSION_CFGDEFINES))
    #error "Software Version Numbers of CDD_I3c_Types.h and CDD_I3c_CfgDefines.h are different"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same vendor */
#if (I3C_VENDOR_ID_TYPES != I3C_IP_VENDOR_ID_TYPES)
    #error "CDD_I3c_Types.h and I3c_Ip_Types.h have different vendor ids"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same Autosar version */
#if ((I3C_AR_RELEASE_MAJOR_VERSION_TYPES     != I3C_IP_AR_RELEASE_MAJOR_VERSION_TYPES) || \
     (I3C_AR_RELEASE_MINOR_VERSION_TYPES     != I3C_IP_AR_RELEASE_MINOR_VERSION_TYPES) || \
     (I3C_AR_RELEASE_REVISION_VERSION_TYPES  != I3C_IP_AR_RELEASE_REVISION_VERSION_TYPES))
    #error "AutoSar Version Numbers of CDD_I3c_Types.h and I3c_Ip_Types.h are different"
#endif

/* Check if current file and I3c_Ip_Types header file are of the same software version */
#if ((I3C_SW_MAJOR_VERSION_TYPES != I3C_IP_SW_MAJOR_VERSION_TYPES) || \
     (I3C_SW_MINOR_VERSION_TYPES != I3C_IP_SW_MINOR_VERSION_TYPES) || \
     (I3C_SW_PATCH_VERSION_TYPES != I3C_IP_SW_PATCH_VERSION_TYPES))
    #error "Software Version Numbers of CDD_I3c_Types.h and I3c_Ip_Types.h are different"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/**
* @brief I3c broadcast address.
*
* @details All I3c slave devices shall match this address. In addition, no I2c slave device shall match this address
*          because it is reserved and unused in I2c.
*
*/
#define I3C_BROADCAST_ADDRESS                (I3C_IP_BROADCAST_ADDRESS)

/**
* @brief I3C master error flags.
*
* @details These can be used with I3c_GetError() function to filter the active errors.
*          Use these masks to check which specific errors/warnings occurred.
*
*/
#define I3C_MASTER_ERROR_NACK                (I3C_IP_MASTER_ERROR_NACK)             /**< Slave(s) NACKed the last address */
#define I3C_MASTER_ERROR_WRITE_ABORT         (I3C_IP_MASTER_ERROR_WRITE_ABORT)      /**< Slave NACKed the write data */
#define I3C_MASTER_ERROR_TERM                (I3C_IP_MASTER_ERROR_TERM)             /**< Master terminated a slave read */
#define I3C_MASTER_ERROR_PARITY              (I3C_IP_MASTER_ERROR_PARITY)           /**< Parity error from a DDR read */
#define I3C_MASTER_ERROR_CRC                 (I3C_IP_MASTER_ERROR_CRC)              /**< CRC error from a DDR read */
#define I3C_MASTER_ERROR_OREAD               (I3C_IP_MASTER_ERROR_OREAD)            /**< Read from MRDATAB register when FIFO empty */
#define I3C_MASTER_ERROR_OWRITE              (I3C_IP_MASTER_ERROR_OWRITE)           /**< Write to MWDATAB register when FIFO full */
#define I3C_MASTER_ERROR_MSG                 (I3C_IP_MASTER_ERROR_MSG)              /**< Read/write SDR/DDR mismatch */
#define I3C_MASTER_ERROR_INVALID_REQUEST     (I3C_IP_MASTER_ERROR_INVALID_REQUEST)  /**< Invalid use of a request */
#define I3C_MASTER_ERROR_TIMEOUT             (I3C_IP_MASTER_ERROR_TIMEOUT)          /**< The module has stalled too long in a frame */

/**
* @brief I3C slave error flags.
*
* @details These can be used with I3c_GetError() function to filter the active errors.
*          Use these masks to check which specific errors/warnings occurred.
*
*/
#define I3C_SLAVE_ERROR_RX_OVERRUN           (I3C_IP_SLAVE_ERROR_RX_OVERRUN)        /**< Slave internal RX buffer/FIFO overrun */
#define I3C_SLAVE_ERROR_TX_UNDERRUN          (I3C_IP_SLAVE_ERROR_TX_UNDERRUN)       /**< Slave internal TX buffer/FIFO underrun */
#define I3C_SLAVE_ERROR_TX_UNDERRUN_NACK     (I3C_IP_SLAVE_ERROR_TX_UNDERRUN_NACK)  /**< The internal Tx buffer/FIFO was underrun in the read header and so the module NACKed the header*/
#define I3C_SLAVE_ERROR_TERM                 (I3C_IP_SLAVE_ERROR_TERM)              /**< Master terminated a read from slave */
#define I3C_SLAVE_ERROR_INVALID_START        (I3C_IP_SLAVE_ERROR_INVALID_START)     /**< Invalid start with SCL falling while SDA = 1 is in a STOP condition */
#define I3C_SLAVE_ERROR_SDR_PARITY           (I3C_IP_SLAVE_ERROR_SDR_PARITY)        /**< SDR parity error on message from master */
#define I3C_SLAVE_ERROR_HDR_PARITY_ERROR     (I3C_IP_SLAVE_ERROR_HDR_PARITY_ERROR)  /**< HDR parity error on message from master */
#define I3C_SLAVE_ERROR_HDR_CRC              (I3C_IP_SLAVE_ERROR_HDR_CRC)           /**< HDR-DDR CRC error on a message from master */
#define I3C_SLAVE_ERROR_S0S1                 (I3C_IP_SLAVE_ERROR_S0S1)              /**< S0 or S1 error */
#define I3C_SLAVE_ERROR_OREAD                (I3C_IP_SLAVE_ERROR_OREAD)             /**< The SRDATAB register was read for more bytes than were available */
#define I3C_SLAVE_ERROR_OWRITE               (I3C_IP_SLAVE_ERROR_OWRITE)            /**< The SWDATA register was written when FULL */

/**
* @brief I3C master status flags.
*
* @details These can be used with I3c_GetStatus() function to filter the active status flags.
*          Use these masks to check which specific status is set.
*
*/
#define I3C_MASTER_STATUS_IBIADDR            (I3C_IP_MASTER_STATUS_IBIADDR)         /**< The address of the Slave Request (IBI, MR, HJ) */
#define I3C_MASTER_STATUS_NOWMASTER          (I3C_IP_MASTER_STATUS_NOWMASTER)       /**< Now this module is a master, but was previously a slave and mastership was handed */
#define I3C_MASTER_STATUS_ERROR_WARNING      (I3C_IP_MASTER_STATUS_ERROR_WARNING)   /**< An error or warning occurred */
#define I3C_MASTER_STATUS_IBIWON             (I3C_IP_MASTER_STATUS_IBIWON)          /**< A slave request won the arbitration on a header address, regardless of whether it was ACKed/NACKed */
#define I3C_MASTER_STATUS_TXNOTFULL          (I3C_IP_MASTER_STATUS_TXNOTFULL)       /**< Tx FIFO not yet full, according to its watermark */
#define I3C_MASTER_STATUS_RXPENDING          (I3C_IP_MASTER_STATUS_RXPENDING)       /**< Rx FIFO has receiving data bytes, according to its watermark */
#define I3C_MASTER_STATUS_COMPLETE           (I3C_IP_MASTER_STATUS_COMPLETE)        /**< A message has completed. For more details, please check the Reference Manual */
#define I3C_MASTER_STATUS_CTRLDONE           (I3C_IP_MASTER_STATUS_CTRLDONE)        /**< A Master Control request was completed. For more details, please check the Reference Manual */
#define I3C_MASTER_STATUS_SLVSTART           (I3C_IP_MASTER_STATUS_SLVSTART)        /**< A slave is/was requesting a START by pulling the SDA low */
#define I3C_MASTER_STATUS_IBITYPE            (I3C_IP_MASTER_STATUS_IBITYPE)         /**< Slave request type (IBI, MR, HJ) */
#define I3C_MASTER_STATUS_NACKED             (I3C_IP_MASTER_STATUS_NACKED)          /**< The last address header was NACKed by the addressed slave */
#define I3C_MASTER_STATUS_BETWEEN            (I3C_IP_MASTER_STATUS_BETWEEN)         /**< Between messages or Dynamic Address Assignment. For more details, please check the Reference Manual */
#define I3C_MASTER_STATUS_STATE              (I3C_IP_MASTER_STATUS_STATE)           /**< Indicates the current master state */

/* These values can be used together with the above defines to get the actual value of the bit fields
   which have more than 1 bit length. */
#define I3C_MASTER_STATUS_IBIADDR_SHIFT      (I3C_IP_MASTER_STATUS_IBIADDR_SHIFT)
#define I3C_MASTER_STATUS_IBITYPE_SHIFT      (I3C_IP_MASTER_STATUS_IBITYPE_SHIFT)
#define I3C_MASTER_STATUS_STATE_SHIFT        (I3C_IP_MASTER_STATUS_STATE_SHIFT)

/**
* @brief I3C slave status flags.
*
* @details These can be used with I3c_GetStatus() function to filter the active status flags.
*          Use these masks to check which specific status is set.
*
*/
#define I3C_SLAVE_STATUS_TIMECTRL            (I3C_IP_SLAVE_STATUS_TIMECTRL)      /**< Indicates if time control is currently enabled */
#define I3C_SLAVE_STATUS_ACTSTATE            (I3C_IP_SLAVE_STATUS_ACTSTATE)      /**< Activity state from Common Command Codes (CCC) */
#define I3C_SLAVE_STATUS_HJDIS               (I3C_IP_SLAVE_STATUS_HJDIS)         /**< Hot-Join is disabled at this time. While Hot-Join is disabled, MCTRL requests will be held off */
#define I3C_SLAVE_STATUS_MRDIS               (I3C_IP_SLAVE_STATUS_MRDIS)         /**< Master Requests are disabled at this time. While Master Requests are disabled, MCTRL requests will be held off */
#define I3C_SLAVE_STATUS_IBIDIS              (I3C_IP_SLAVE_STATUS_IBIDIS)        /**< In-Band interrupts are disabled at this time. While Master Requests are disabled, MCTRL requests will be held off */
#define I3C_SLAVE_STATUS_EVDET               (I3C_IP_SLAVE_STATUS_EVDET)         /**< Slave request details: if it was NACKed, ACKed, or sill waiting for START, Bus Available, or Bus-Idle (HJ) conditions */
#define I3C_SLAVE_STATUS_EVENT               (I3C_IP_SLAVE_STATUS_EVENT)         /**< Slave request was sent and ACKed. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_CHANDLED            (I3C_IP_SLAVE_STATUS_CHANDLED)      /**< A Common Command Code (CCC) is being handled by the module */
#define I3C_SLAVE_STATUS_HDRMATCHED          (I3C_IP_SLAVE_STATUS_HDRMATCHED)    /**< An HDR command matched this device’s I3C Dynamic Address */
#define I3C_SLAVE_STATUS_ERROR_WARNING       (I3C_IP_SLAVE_STATUS_ERROR_WARNING) /**< An error or warning has occurred */
#define I3C_SLAVE_STATUS_CCC                 (I3C_IP_SLAVE_STATUS_CCC)           /**< A Common Command Code (CCC) has been received, and is not handled by the I3C module */
#define I3C_SLAVE_STATUS_DACHG               (I3C_IP_SLAVE_STATUS_DACHG)         /**< The Slave Dynamic Address (DA) has been assigned, re-assigned, or reset (lost) */
#define I3C_SLAVE_STATUS_TXNOTFULL           (I3C_IP_SLAVE_STATUS_TXNOTFULL)     /**< Tx FIFO not yet full, according to its watermark */
#define I3C_SLAVE_STATUS_RXPENDING           (I3C_IP_SLAVE_STATUS_RXPENDING)     /**< Rx FIFO has receiving data bytes, according to its watermark */
#define I3C_SLAVE_STATUS_STOP                (I3C_IP_SLAVE_STATUS_STOP)          /**< STOP signal was detected on the bus */
#define I3C_SLAVE_STATU_MATCHED              (I3C_IP_SLAVE_STATU_MATCHED)        /**< An incoming header matched this device’s I3C Dynamic or I2C Static address (if any) */
#define I3C_SLAVE_STATUS_START               (I3C_IP_SLAVE_STATUS_START)         /**< START or repeated START was seen after the START bit was last cleared */
#define I3C_SLAVE_STATUS_STHDR               (I3C_IP_SLAVE_STATUS_STHDR)         /**< The I3C bus is in HDR-DDR mode. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_STDAA               (I3C_IP_SLAVE_STATUS_STDAA)         /**< The I3C bus is in Enter Dynamic Address Assignment (ENTDAA) mode. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_STREQWR             (I3C_IP_SLAVE_STATUS_STREQWR)       /**< The request in process is SDR write data from the master to this slave. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_STREQRD             (I3C_IP_SLAVE_STATUS_STREQRD)       /**< The request in process is SDR read from this slave, or an IBI is pushed out. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_STCCCH              (I3C_IP_SLAVE_STATUS_STCCCH)        /**< A Common Command Code (CCC) message is being handled automatically */
#define I3C_SLAVE_STATUS_STMSG               (I3C_IP_SLAVE_STATUS_STMSG)         /**< This slave is listening to the bus traffic or responding. For more details, please check the Reference Manual */
#define I3C_SLAVE_STATUS_STNOTSTOP           (I3C_IP_SLAVE_STATUS_STNOTSTOP)     /**< The I3c bus is busy (STOP has not been detected yet) */

/* These values can be used together with the above defines to get the actual value of the bit fields
   which have more than 1 bit length. */
#define I3C_SLAVE_STATUS_TIMECTRL_SHIFT      (I3C_IP_SLAVE_STATUS_TIMECTRL_SHIFT)
#define I3C_SLAVE_STATUS_ACTSTATE_SHIFT      (I3C_IP_SLAVE_STATUS_ACTSTATE_SHIFT)
#define I3C_SLAVE_STATUS_EVDET_SHIFT         (I3C_IP_SLAVE_STATUS_EVDET_SHIFT)

/**
* @brief Common Command Codes.
*        These defines should be used with I3c_SendCommonCommandCode() and I3c_ReadCommonCommandCode().
*        For more details about each command, please see Mipi Basic Specifications v1.0 document.
*
*/
#define I3C_CCC_ENEC_BROADCAST      (I3C_IP_CCC_ENEC_BROADCAST)          /**< Enable events */
#define I3C_CCC_DISEC_BROADCAST     (I3C_IP_CCC_DISEC_BROADCAST)         /**< Disable events */
#define I3C_CCC_ENTAS0_BROADCAST    (I3C_IP_CCC_ENTAS0_BROADCAST)        /**< Enter activity state 0 */
#define I3C_CCC_ENTAS1_BROADCAST    (I3C_IP_CCC_ENTAS1_BROADCAST)        /**< Enter activity state 1 */
#define I3C_CCC_ENTAS2_BROADCAST    (I3C_IP_CCC_ENTAS2_BROADCAST)        /**< Enter activity state 2 */
#define I3C_CCC_ENTAS3_BROADCAST    (I3C_IP_CCC_ENTAS3_BROADCAST)        /**< Enter activity state 3 */
#define I3C_CCC_RSTDAA_BROADCAST    (I3C_IP_CCC_RSTDAA_BROADCAST)        /**< Reset Dynamic Address Assignment */
#define I3C_CCC_ENTDAA_BROADCAST    (I3C_IP_CCC_ENTDAA_BROADCAST)        /**< Enter Dynamic Address Assignment */
#define I3C_CCC_DEFSLVS_BROADCAST   (I3C_IP_CCC_DEFSLVS_BROADCAST)       /**< Define list of slaves */
#define I3C_CCC_SETMWL_BROADCAST    (I3C_IP_CCC_SETMWL_BROADCAST)        /**< Set maximum write length */
#define I3C_CCC_SETMRL_BROADCAST    (I3C_IP_CCC_SETMRL_BROADCAST)        /**< Set maximum read length */
#define I3C_CCC_ENTTM_BROADCAST     (I3C_IP_CCC_ENTTM_BROADCAST)         /**< Enter test mode */
#define I3C_CCC_ENDXFER_BROADCAST   (I3C_IP_CCC_ENDXFER_BROADCAST)       /**< Provides the framework for Controllers and Targets to exchange set-up parameters
                                                for ending data transfers in supported HDR Modes. For HDR-DDR, it controls Data Transfer
                                                Ending Procedure. For more details, please check MIPI I3c Basic Specification document. */
#define I3C_CCC_ENTHDR0_BROADCAST   (I3C_IP_CCC_ENTHDR0_BROADCAST)       /**< Enter HDR-DDR mode */
#define I3C_CCC_ENEC_DIRECT         (I3C_IP_CCC_ENEC_DIRECT)             /**< Enable events */
#define I3C_CCC_DISEC_DIRECT        (I3C_IP_CCC_DISEC_DIRECT)            /**< Disable events */
#define I3C_CCC_ENTAS0_DIRECT       (I3C_IP_CCC_ENTAS0_DIRECT)           /**< Enter activity state 0 */
#define I3C_CCC_ENTAS1_DIRECT       (I3C_IP_CCC_ENTAS1_DIRECT)           /**< Enter activity state 1 */
#define I3C_CCC_ENTAS2_DIRECT       (I3C_IP_CCC_ENTAS2_DIRECT)           /**< Enter activity state 2 */
#define I3C_CCC_ENTAS3_DIRECT       (I3C_IP_CCC_ENTAS3_DIRECT)           /**< Enter activity state 3 */
#define I3C_CCC_RSTDAA_DIRECT       (I3C_IP_CCC_RSTDAA_DIRECT)           /**< Reset Dynamic Address Assignment */
#define I3C_CCC_SETDASA_DIRECT      (I3C_IP_CCC_SETDASA_DIRECT)          /**< Set Dynamic Addres from Static Address */
#define I3C_CCC_SETNEWDA_DIRECT     (I3C_IP_CCC_SETNEWDA_DIRECT)         /**< Set New Dynamic Addres */
#define I3C_CCC_SETMWL_DIRECT       (I3C_IP_CCC_SETMWL_DIRECT)           /**< Set maximum write length */
#define I3C_CCC_SETMRL_DIRECT       (I3C_IP_CCC_SETMRL_DIRECT)           /**< Set maximum read length */
#define I3C_CCC_GETMWL_DIRECT       (I3C_IP_CCC_GETMWL_DIRECT)           /**< Get maximum write length */
#define I3C_CCC_GETMRL_DIRECT       (I3C_IP_CCC_GETMRL_DIRECT)           /**< Get maximum read length */
#define I3C_CCC_GETPID_DIRECT       (I3C_IP_CCC_GETPID_DIRECT)           /**< Get Provisional ID */
#define I3C_CCC_GETBCR_DIRECT       (I3C_IP_CCC_GETBCR_DIRECT)           /**< Get Bus Characteristic Register */
#define I3C_CCC_GETDCR_DIRECT       (I3C_IP_CCC_GETDCR_DIRECT)           /**< Get Device Characteristic Register */
#define I3C_CCC_GETSTATUS_DIRECT    (I3C_IP_CCC_GETSTATUS_DIRECT)        /**< Get device status */
#define I3C_CCC_GETACCMST_DIRECT    (I3C_IP_CCC_GETACCMST_DIRECT)        /**< Get accept mastership */
#define I3C_CCC_ENDXFER_DIRECT      (I3C_IP_CCC_ENDXFER_DIRECT)          /**< Provides the framework for Controllers and Targets to exchange set-up parameters
                                                for ending data transfers in supported HDR Modes. For HDR-DDR, it controls Data Transfer
                                                Ending Procedure. For more details, please check MIPI I3c Basic Specification document. */
#define I3C_CCC_SETBRGTGT_DIRECT    (I3C_IP_CCC_SETBRGTGT_DIRECT)        /**< Set bridge targets */
#define I3C_CCC_GETMXDS_DIRECT      (I3C_IP_CCC_GETMXDS_DIRECT)          /**< Get maximum data speed */

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/**
* @brief Definition of the I3C Hw Unit mode.
*
*/
typedef enum
{
    I3C_MASTER_MODE         = 0x0U, /**< The module is an I3C Master */
    I3C_SLAVE_MODE          = 0x1U, /**< The module is an I3C Slave */
    I3C_MASTER_CAPABLE_MODE = 0x2U  /**< The I3C module is master-capable; however the module is operating as a slave now.
                                         When used from the start, the I3C module will start as a slave, but will be prepared to switch to master mode using specific procedures. */
} I3c_MasterSlaveModeType;


/**
* @brief Direction of the transfer: send/receive.
*
*/
typedef enum
{
    I3C_SEND    = I3C_IP_WRITE, /**< @brief Used to send data to a slave */
    I3C_RECEIVE = I3C_IP_READ   /**< @brief Used to receive data from a slave */

} I3c_DirectionType;

/**
* @brief Bus type with EmitStartAddr.
*
*/
typedef enum
{
    I3C_BUS_TYPE_I3C_SDR    = I3C_IP_BUS_TYPE_I3C_SDR,  /**< I3C SDR mode */
    I3C_BUS_TYPE_I2C        = I3C_IP_BUS_TYPE_I2C,      /**< I2C mode */
    I3C_BUS_TYPE_I3C_DDR    = I3C_IP_BUS_TYPE_I3C_DDR   /**< I3C HDR-DDR mode */
} I3c_BusType;

/**
* @brief Indicates size of the transfer message.
*
*/
typedef enum
{
    I3C_TRANSFER_BYTES      = I3C_IP_TRANSFER_BYTES,        /**< Send/read in bytes */
    I3C_TRANSFER_HALF_WORDS = I3C_IP_TRANSFER_HALF_WORDS    /**< Send/read in half-words */
} I3c_TransferSizeType;

/**
* @brief I3C specific error codes.
*
*/
typedef enum
{
    I3C_STATUS_SUCCESS              = I3C_IP_STATUS_SUCCESS,
    I3C_STATUS_ERROR                = I3C_IP_STATUS_ERROR,
    I3C_STATUS_BUSY                 = I3C_IP_STATUS_BUSY,
    I3C_STATUS_TIMEOUT              = I3C_IP_STATUS_TIMEOUT,            /**< The module has stalled too long in a frame */
    I3C_STATUS_MAX_SLAVES_EXCEED    = I3C_IP_STATUS_MAX_SLAVES_EXCEED,  /**< Maximum number of devices on the bus has been exceeded */
    I3C_STATUS_IBI_WON              = I3C_IP_STATUS_IBI_WON,            /**< IBI/MR/HJ won the arbitration on a header address */
    I3C_STATUS_SLAVE_START          = I3C_IP_STATUS_SLAVE_START,
    I3C_STATUS_CONTROL_DONE         = I3C_IP_STATUS_CONTROL_DONE
} I3c_StatusType;

#if (STD_ON == I3C_ENABLE_I3C_FEATURES)
/**
* @brief Causes of slave Dynamic Address change.
*
*/
typedef enum
{
    I3C_DA_NOT_CONFIGURED  = I3C_IP_SLAVE_DA_NOT_CONFIGURED,    /**< No info. DA was not configure. */
    I3C_DA_CAUSE_ENTDAA    = I3C_IP_SLAVE_DA_CAUSE_ENTDAA,      /**< DA assigned using ENTDAA command */
    I3C_DA_CAUSE_SETD      = I3C_IP_SLAVE_DA_CAUSE_SET,         /**< DA assigned using SETDASA, SETAASA, or SETNEWDA command */
    I3C_DA_CAUSE_RSTDAA    = I3C_IP_SLAVE_DA_CAUSE_RSTDAA,      /**< DA cleared using RSTDAA command */
    I3C_DA_CAUSE_AUTOMAP   = I3C_IP_SLAVE_DA_CAUSE_AUTOMAP      /**< Auto MAP change happened last. This may have changed this DA as well */
} I3c_DynamicAddressChangeType;
#endif /* (STD_ON == I3C_ENABLE_I3C_FEATURES)*/

/**
* @brief Indicates the current slave operation mode.
*
*/
typedef enum
{
    I3C_SLAVE_NORMAL_MODE        = I3C_IP_SLAVE_NORMAL_MODE,     /**< Normal mode */
    I3C_SLAVE_IBI_REQUEST        = I3C_IP_SLAVE_IBI_REQUEST,     /**< Start an IBI event */
    I3C_SLAVE_MASTER_REQUEST     = I3C_IP_SLAVE_MASTER_REQUEST,  /**< Start a MR or P2P event */
    I3C_SLAVE_HOT_JOIN_REQUEST   = I3C_IP_SLAVE_HOT_JOIN_REQUEST /**< Start a HJ event */
} I3c_SlaveRequestType;

/**
* @brief In-Band Interrupt (IBI) response.
*
*/

#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
typedef enum
{
    I3C_IBI_RESP_ACK            = I3C_IP_IBI_RESP_ACK,              /**< ACK with no mandatory byte */
    I3C_IBI_RESP_NACK           = I3C_IP_IBI_RESP_NACK,             /**< NACK */
    I3C_IBI_RESP_ACK_MANDATORY  = I3C_IP_IBI_RESP_ACK_MANDATORY,    /**< ACK with mandatory byte */
    I3C_IBI_RESP_MANUAL         = I3C_IP_IBI_RESP_MANUAL            /**< Reserved */
} I3c_IbiResponseType;
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief This gives the numeric ID (paritition number) of an I3C Hw Unit.
*
*/
typedef uint8 I3c_PartCoreType;

/**
* @brief Data Type to be sent or received
*
*/
typedef uint8 I3c_DataType;

/**
* @brief Slave device characteristics structure
*
* This structure is used to store the characteristics of the I3C slave devices on the bus.
*
*/
typedef I3c_Ip_SlaveDeviceType I3c_SlaveDeviceType;

/**
* @brief This structure contains the Hw Channel Configuration of the I3C module.
*
*/
typedef struct
{
    const I3c_Ip_MasterConfigType * MasterConfig;
    const I3c_Ip_SlaveConfigType * SlaveConfig;
} I3c_HwChannelConfigType;

/**
* @brief This structure contains the Hw Unit Configuration of the I3C module.
*
*/
typedef struct
{
    const uint8 I3c_HwUnit; /**< I3c hardware instance */
    const I3c_MasterSlaveModeType MasterSlaveModeConfig;
    const I3c_HwChannelConfigType * HwChannelConfig;
} I3c_HwConfigType;

/**
* @brief This structure is passed to I3c_SyncTransmit or I3c_AsyncTransmit functions.
*        It holds the necessary information required for a transfer on the I3C Bus.
*
*/
typedef struct 
{
    uint8 SlaveAddress;                     /**< Slave Device Address */ 
    boolean SendStop;                       /**< Specifies if either a STOP or repeated START signal shall be emitted */
    I3c_DirectionType Direction;            /**< Specifies the direction of the transfer, send or receive */
    I3c_TransferSizeType TransferSize;      /**< Specifies if the transfer is in half words or in bytes */  
    I3c_BusType BusType;                    /**< Bus Type: SDR, DDR, I2C */ 
}I3c_RequestType;

/**
* @brief This structure contains initialization data for the I3C driver.
*
*/
typedef struct
{
    const I3c_PartCoreType I3c_CoreId;              /**< Numeric Partition Id */
    const I3c_HwConfigType * const * I3c_HwConfig;  /**< Pointer to the I3C Hw Unit configuration */
} I3c_ConfigType;

/**
* @brief Contains the baud rates options.
*        This structure should be set by the user at runtime.
*
*/
typedef I3c_Ip_MasterBaudRateType I3c_MasterBaudRateType;

#if (STD_ON == I3C_ENABLE_SLAVE_REQUESTS)
/**
* @brief Contains the In-Band interrupt options for the following Slave Requests.
*        This structure should be set by the user at runtime.
*
*/
typedef I3c_Ip_MasterIbiOptionType I3c_MasterIbiOptionType;
#endif /* (STD_ON == I3C_ENABLE_SLAVE_REQUESTS )*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* I3C_TYPES_H */

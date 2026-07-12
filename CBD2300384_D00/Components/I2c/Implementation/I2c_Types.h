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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  I2c_Types.h
 *        \brief  I2c types header file
 *
 *      \details  Contains type definitions for I2c component.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to revision history in I2c.h.
 *********************************************************************************************************************/

#if !defined I2C_TYPES_H
# define I2C_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef CONST(uint8, I2C_CONST) I2c_HwUnitType; /*<! Specifies the identification (ID) of a hardware unit. */
typedef uint8 I2c_HwUnitIndexType;
typedef CONST(uint8, I2C_CONST) I2c_BusType; /*<! Specifies the identification (ID) of a bus. */

typedef uint8 I2c_ChannelType; /*<! Specifies the identification (ID) of a channel. */
typedef P2CONST(I2c_ChannelType, AUTOMATIC, I2C_CONST) I2c_ChannelConstPtrType;
typedef uint8 I2c_ChannelIndexType;

typedef uint16 I2c_AddressType; /*<! Address type for bus clients. */
typedef uint16 I2c_DataLengthType; /*<! Length type for buffer handling. */

typedef uint8 I2c_SequenceType; /*<! Specifies the identification (ID) of a sequence. */
typedef P2VAR(I2c_SequenceType, AUTOMATIC, I2C_VAR) I2c_PrioQueueType; /*<! Type for the priority queues containing sequence IDs. */
typedef I2c_SequenceType I2c_PrioQueueLengthType; /*<! Length type for priority queues. */
typedef uint8 I2c_SeqPrioType; /*<! Type for sequence priority */

typedef uint8 I2c_DataType; /*<! Type of data to be transmitted */
typedef P2VAR(I2c_DataType, AUTOMATIC, I2C_VAR) I2c_DataPtrType; /*<! Pointer to a buffer element */
typedef P2CONST(I2c_DataType, AUTOMATIC, I2C_CONST) I2c_DataConstPtrType; /*<! Pointer to const. buffer element */

typedef uint8 I2c_TimeoutType;

/*! Sequence Notifications. */
typedef P2FUNC(void, I2C_CODE, SequenceEndNotificationType) (void);
typedef P2FUNC(void, I2C_CODE, SequenceErrorNotificationType) (void);

typedef P2VAR(Std_VersionInfoType, AUTOMATIC, I2C_VAR) I2c_VersionInfoPtrType;

/*! Channel result type definition. */
typedef enum
{
  I2C_CH_RESULT_OK = 0u, /*<! Channel transmission successful. */
  I2C_CH_RESULT_BUSFAIL, /*<! Channel transmission failed due to a bus error (e.g. SCL stuck low). */
  I2C_CH_RESULT_ARBFAIL, /*<! Channel transmission failed due to an arbitration error. */
  I2C_CH_RESULT_NACKFAIL /*<! Channel transmission failed due to unexpected NACK. */
} I2c_ChannelResultType;
typedef P2VAR(I2c_ChannelResultType, AUTOMATIC, I2C_VAR_NO_INIT) I2c_ChannelResultPtrType;

/*! Channel status type definition. */
typedef enum
{
  I2C_CH_STATUS_IDLE = 0u, /*< Channel is not in use currently.  */
  I2C_CH_STATUS_PENDING, /*< Channel is scheduled for transmission. */
  I2C_CH_STATUS_ACTIVE  /*< Channel is transmitted currently. */
} I2c_ChannelStatusType;

/*! Sequence result type definition. */
typedef enum
{
  I2C_SEQ_OK = 0u, /*<! Sequence transmission was successful. */
  I2C_SEQ_PENDING, /*<! Sequence is processed. */
  I2C_SEQ_QUEUED, /*<! Sequence is queued in priority queue. */
  I2C_SEQ_NACK, /*<! Sequence could not address a device. */
  I2C_SEQ_FAILED, /*<! Sequence transmission was not successful. */
  I2C_SEQ_CANCELED /*<! Sequence was canceled by user request. */
} I2c_SeqResultType;
typedef P2VAR(I2c_SeqResultType, AUTOMATIC, I2C_VAR_NO_INIT) I2c_SeqResultPtrType;

/*! Component status type definition. */
typedef enum
{
  I2C_UNINIT = 0u, /*<! Component is uninitialized. */
  I2C_IDLE, /*<! Component idles. */
  I2C_BUSY /*<! Component is busy. */
} I2c_ComponentStatusType;

/*! Error handling type definition. */
typedef enum
{
  I2C_BURST = 0u, /*<! Try to free the bus in case of an arbitration failure.  */
  I2C_RECURRENT /*<! Detach from bus in case of an arbitration failure.  */
} I2c_HandlingType;

/*! Transmission direction type definition. */
typedef enum
{
  I2C_WRITE = 0u, /*<! Write to slave.  */
  I2C_READ = 1u /*<! Read from slave. */
} I2c_TransDirectionType;

/*! Transmission buffer type definition. */
typedef struct
{
  I2c_DataPtrType BufPtr; /*<! Pointer to first element of external buffer. */
  I2c_DataLengthType DataLength; /*<! Length of external buffer. */
  boolean SingleValue; /*<! Transmit single buffer value repeatedly. */
} I2c_TransBufferDescType;
typedef P2VAR(I2c_TransBufferDescType, AUTOMATIC, I2C_VAR_NO_INIT) I2c_TransBufferDescPtrType;
typedef P2CONST(I2c_TransBufferDescType, AUTOMATIC, I2C_VAR_NO_INIT) I2c_ConstTransBufferDescPtrType;

/*! Hardware processing type definition. (Used by I2c_TransProc to store hardware unit specific data) */
typedef struct
{
  I2c_SequenceType CurrentSeq; /*<! Currently processed sequence. */
  boolean SequenceInProcess; /*<! Determines if the currentSequence is in process. */
  boolean CancelProcess; /*<! Determines if the ongoing transmission process shall be aborted. */
} I2c_HardwareProcessorType;
typedef P2VAR(I2c_HardwareProcessorType, AUTOMATIC, I2C_VAR) I2c_HardwareProcessorPtrType;
typedef P2CONST(I2c_HardwareProcessorType, AUTOMATIC, I2C_CONST) I2c_ConstHardwareProcessorPtrType;

/**********************************************************************************************************************
 *  END OF FILE: I2c_Types.h
 *********************************************************************************************************************/

#endif /* I2C_TYPES_H */


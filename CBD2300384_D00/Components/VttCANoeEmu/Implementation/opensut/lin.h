/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file lin.h
 *  Contains declarations related to the Local Interconnect Network (LIN).
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

/*! The checksum model of a LIN frame.
 *  This type is used to specify the checksum model to be used for the LIN frame.
 */
typedef enum
{
  OPENSUT_LIN_CHECKSUM_MODEL_UNDEFINED = 0x00U,
  OPENSUT_LIN_CHECKSUM_MODEL_ENHANCED  = 0x01U,
  OPENSUT_LIN_CHECKSUM_MODEL_CLASSIC   = 0x02U
} opensut_lin_checksum_model_t;

/*! Value is passed to the opensut_lin_send_handler() callback.
 *  Determines whether the master (the SUT) also provides a frame response
 *  or if the frame response is expected to be provided from a slave.
 */
typedef enum
{
  OPENSUT_LIN_FRAME_RESPONSE_TYPE_MASTER_RESPONSE = 0x00U, //<! Response is generated from this (master) node
  OPENSUT_LIN_FRAME_RESPONSE_TYPE_SLAVE_RESPONSE  = 0x01U, //<! Response is generated from a slave node
  OPENSUT_LIN_FRAME_RESPONSE_TYPE_SLAVE_TO_SLAVE  = 0x02U  //<! Response is generated from one slave to another slave
} opensut_lin_frame_response_type_t;

/*! The state of a LIN transmission.
 *  Used to indicate the success or failure of a LIN transmission to the SUT
 *  via opensut_lin_receive().
 */
typedef enum
{
  OPENSUT_LIN_FRAME_STATUS_NOT_OK              = 0x00U, //!< currently not in use
  OPENSUT_LIN_FRAME_STATUS_LIN_TX_OK           = 0x01U, //!< Successful transmission of a Master or Slave2Slave frame
  OPENSUT_LIN_FRAME_STATUS_LIN_TX_BUSY         = 0x02U, //!< Ongoing transmission (Header or Response)
  OPENSUT_LIN_FRAME_STATUS_LIN_TX_HEADER_ERROR = 0x03U, //!< Erroneous header transmission
  OPENSUT_LIN_FRAME_STATUS_LIN_TX_ERROR        = 0x04U, //!< Erroneous response transmission
  OPENSUT_LIN_FRAME_STATUS_LIN_RX_OK           = 0x05U, //!< Reception of correct response
  OPENSUT_LIN_FRAME_STATUS_LIN_RX_BUSY         = 0x06U, //!< Ongoing reception
  OPENSUT_LIN_FRAME_STATUS_LIN_RX_ERROR        = 0x07U, //!< Erroneous response reception
  OPENSUT_LIN_FRAME_STATUS_LIN_RX_NO_RESPONSE  = 0x08U, //!< No response byte has been received so far
} opensut_lin_frame_status_t;

/*! A LIN frame.
 *  This type is used to provide LIN id, checksum model, data length and data.
 */
typedef struct
{
  uint8_t data[8];                             //!< the actual payload
  opensut_lin_checksum_model_t checksum_model; //!< checksum model
  uint8_t id;                                  //!< LIN identifier
  uint8_t data_length;                         //!< data length
} opensut_lin_frame_t;

/*! A LIN message.
 *  Contains a LIN frame and metadata.
 */
typedef struct
{
  size_t channel_id;             //!< identifies the transmitting network
  opensut_lin_frame_t lin_frame; //!< the LIN frame
} opensut_lin_message_t;

/*! Indicates reception of a LIN message to the SUT.
 *  @param in_message LIN message data to receive. The pointer needs to be valid during the call's lifetime.
 *  @param in_frame_status Status of the LIN transmission. See \ref opensut_lin_frame_status_t
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_message is nullptr or contains invalid data
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 *    or if the SUT (LIN master) is sleeping
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_receive(
  const opensut_lin_message_t* in_message,
  opensut_lin_frame_status_t in_frame_status
);

/*! This callback is invoked by the SUT (the master) for transmitting a LIN message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not.
 *  The transmission must always be confirmed by calling opensut_lin_receive().
 *  @param in_context used-defined context parameter
 *  @param in_lin_message message data to be transmitted. Pointer is valid during the call's lifetime only.
 *  If an implementation needs to preserve message data for asynchronous transmission, it needs to copy that
 *  to privately managed memory.
 *  @param in_response_type
 *  - MasterResponse: the frame is sent from this controller to all connected slaves
 *  - SlaveResponse: the frame response must be provided by a connected slave and is received by this controller
 *  - SlaveToSlave: the frame response must be provided by a connected slave and is ignored by this controller
 */
typedef void(*opensut_lin_send_handler)(
  void* in_context,
  const opensut_lin_message_t* in_lin_message,
  opensut_lin_frame_response_type_t in_response_type
);

/*! Registers the implementation for handling LIN message transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_send_handler(
  opensut_lin_send_handler in_handler, 
  void* in_context
);

/*! Indicates reception of a go-to-sleep-message to the SUT.
 *  @param channel The affected channel
 *  @return
 *  - OPENSUT_OK if command was processed
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_go_to_sleep(
  size_t channel
);

/*! Indicates reception of a wakeup-message to the SUT.
 *  @param channel The affected channel
 *  @return
 *  - OPENSUT_OK if command was processed
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_wakeup(
  size_t channel
);

/*! This callback is invoked by the SUT (the master) for transmitting a LIN Goto-Sleep-Message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not.
 *  @param in_context used-defined context parameter
 *  @param channel The channel to go to sleep
 */
typedef void(*opensut_lin_go_to_sleep_handler)(
  void* in_context,
  size_t channel
);

/*! This callback is invoked by the SUT (the master) for transmitting a LIN Wakeup-Message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not.
 *  @param in_context used-defined context parameter
 *  @param channel The channel to wakeup
 */
typedef void(*opensut_lin_wakeup_handler)(
  void* in_context,
  size_t channel
);

/*! This callback is invoked by the SUT (the master) for transmitting a LIN Goto-Sleep-Internal-Message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not.
 *  @param in_context used-defined context parameter
 *  @param channel The channel to go to sleep
 */
typedef void(*opensut_lin_go_to_sleep_internal_handler)(
  void* in_context,
  size_t channel
);

/*! This callback is invoked by the SUT (the master) for transmitting a LIN Wakeup-Internal-Message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not.
 *  @param in_context used-defined context parameter
 *  @param channel The channel to wakeup
 */
typedef void(*opensut_lin_wakeup_internal_handler)(
  void* in_context,
  size_t channel
);

/*! Registers the implementation for handling LIN sleep transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_go_to_sleep_handler(
  opensut_lin_go_to_sleep_handler in_handler,
  void* in_context
);

/*! Registers the implementation for handling LIN wakeup transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_wakeup_handler(
  opensut_lin_wakeup_handler in_handler, 
  void* in_context
);

/*! Registers the implementation for handling LIN sleep internal transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_go_to_sleep_internal_handler(
  opensut_lin_go_to_sleep_internal_handler in_handler,
  void* in_context
);

/*! Registers the implementation for handling LIN wakeup internal transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_wakeup_internal_handler(
  opensut_lin_wakeup_internal_handler in_handler,
  void* in_context
);

#pragma pack(pop)

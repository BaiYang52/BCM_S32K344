/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file eth.h
 * Contains declarations related to Ethernet (Eth).
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

/*! Contains a Ethernet frame and metadata. The frame is given as an array of bytes, that contains the byte stream
 *  of the Ethernet frame. The array begins with the first byte of the receiver MAC address and ends with the last byte
 *  of the payload field / padding field of the frame.
 *  It contains neither the preamble nor the start frame delimiter (SFD) of the Ethernet packet.
 */
typedef struct
{
  size_t channel_id;     //!< identifies the transmitting network
  uint32_t frame_length; //!< Length of Ethernet frame in bytes (without preamble, SFD and CRC) and this is also the
                         // length of the byte array addressed by the pointer element 'frame'.
  uint8_t* frame;        //!< pointer to an array of bytes that contains the Ethernet frame
} opensut_eth_message_t;

/*! This callback is invoked by the SUT for transmitting a Ethernet message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not. The transmission must always be
 *  confirmed by calling opensut_eth_acknowledge().
 *  @param in_context used-defined context parameter
 *  @param in_eth_message message data to be transmitted. Pointer is valid during the call's lifetime only.
 *  If an implementation needs to preserve message data for asynchronous transmission, it needs to copy that
 *  to privately managed memory.
 *  @param in_send_job_id Caller-supplied job identifier which needs to be passed to
 *  opensut_eth_acknowledge() later on.
 */
typedef void (
  *opensut_eth_send_handler)(void* in_context, const opensut_eth_message_t* in_eth_message, size_t in_send_job_id);

/*! Indicates reception of a Ethernet message to the SUT.
 *  @param in_message Ethernet message data to receive. The pointer needs to be valid during the call's lifetime.
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_message is nullptr or contains invalid data
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_eth_receive(const opensut_eth_message_t* in_message);

/*! Acknowledges the transmission of a Ethernet message to the SUT.
 *  @param in_send_job_id id obtained from opensut_eth_send_handler()
 *  @return
 *  - OPENSUT_OK if acknowledgment was successfully processed.
 *    Job id is invalid now and may be reused for future transmissions.
 *  - OPENSUT_PARAMETER_INVALID if job id is invalid
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_eth_acknowledge(size_t in_send_job_id);

/*! Registers the implementation for handling Ethernet message transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_eth_set_send_handler(opensut_eth_send_handler in_handler, void* in_context);

#pragma pack(pop)

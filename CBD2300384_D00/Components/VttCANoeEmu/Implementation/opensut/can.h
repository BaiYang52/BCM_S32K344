/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file can.h
 * Contains declarations related to the Controller Area Network (CAN).
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

/*! Contains a CAN frame and metadata.
 */
typedef struct
{
  size_t channel_id;             //!< identifies the transmitting network
  opensut_can_frame_t can_frame; //!< the CAN frame
} opensut_can_message_t;

/*! This callback is invoked by the SUT for transmitting a CAN message.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already transmitted or not. The transmission must always be
 *  confirmed by calling opensut_can_acknowledge().
 *  @param in_context used-defined context parameter
 *  @param in_can_message message data to be transmitted. Pointer is valid during the call's lifetime only.
 *  If an implementation needs to preserve message data for asynchronous transmission, it needs to copy that
 *  to privately managed memory.
 *  @param in_send_job_id Caller-supplied job identifier which needs to be passed to
 *  opensut_can_acknowledge() later on.
 */
typedef void (
  *opensut_can_send_handler)(void* in_context, const opensut_can_message_t* in_can_message, size_t in_send_job_id);

/*! Indicates reception of a CAN message to the SUT.
 *  @param in_message CAN message data to receive. The pointer needs to be valid during the call's lifetime.
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_message is nullptr or contains invalid data
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_can_receive(const opensut_can_message_t* in_message);

/*! Acknowledges the transmission of a CAN message to the SUT.
 *  @param in_send_job_id id obtained from opensut_can_send_handler()
 *  @return
 *  - OPENSUT_OK if acknowledgement was successfully processed.
 *    Job id is invalid now and may be reused for future transmissions.
 *  - OPENSUT_PARAMETER_INVALID if job id is invalid
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_can_acknowledge(size_t in_send_job_id);

/*! Registers the implementation for handling CAN message transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
 *  in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_can_set_send_handler(opensut_can_send_handler in_handler, void* in_context);

#pragma pack(pop)

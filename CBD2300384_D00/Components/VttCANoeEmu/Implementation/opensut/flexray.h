/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file flexray.h
 * Contains declarations related to FlexRay.
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

//! FlexRay channels none, A, B, or AB
typedef enum
{
  OPENSUT_FR_CHANNEL_NONE = 0x00U, //!< Neither channel A nor B
  OPENSUT_FR_CHANNEL_A    = 0x01U, //!< Send/receive FlexRay frame over channel A
  OPENSUT_FR_CHANNEL_B    = 0x02U, //!< Send/receive FlexRay frame over channel B
  OPENSUT_FR_CHANNEL_AB   = 0x03U  //!< Send/receive FlexRay frame over channel A and B
} opensut_fr_channel_t;

//! Indicators for FlexRay frame header flags
typedef enum
{
  OPENSUT_FR_HEADER_FLAG_SUF_INDICATOR = 0x01U, //!< Startup frame indicator
  OPENSUT_FR_HEADER_FLAG_SYF_INDICATOR = 0x02U, //!< Sync frame indicator
  OPENSUT_FR_HEADER_FLAG_NF_INDICATOR  = 0x04U, //!< Null frame indicator
  OPENSUT_FR_HEADER_FLAG_PP_INDICATOR  = 0x08U, //!< Payload preamble indicator
  OPENSUT_FR_HEADER_FLAG_RESERVED_BIT  = 0x10U  //!< Reserved bit, shall not be used by the application
} opensut_fr_header_flags_t;

//! Specifies a FlexRay frame header
typedef struct
{
  /*!
   * \brief FlexRay message header flags bit map. Description:
   *  - [7-5]: unused
   *  - [4]: Reserved bit
   *  - [3]: Payload preamble indicator: 0, Regular payload; 1, NM vector or message ID
   *  - [2]: Null frame indicator: 0, No valid payload data and payload preamble indicator = 0; 1, valid payload data
   *  - [1]: Sync frame indicator: 0, Frame not used for synchronization; 1, frame shall be used for sync
   *  - [0]: Startup frame indicator: 0, Not a startup frame; 1, a startup frame
   */
  opensut_fr_header_flags_t flags;
  uint16_t slot_id;       //!< Slot ID in which the frame was sent: 1 - 2047, 11 bits
  uint8_t payload_length; //!< Payload length: 0-127, 7 bits (the number of payload data bytes divided by two)
  uint16_t header_crc;    //!< Header CRC, 11 bits
  uint8_t cycle_count;    //!< Cycle in which the frame was sent: 0 - 63, 6 bits
} opensut_fr_header_t;

//! Specifies a FlexRay frame
typedef struct
{
  opensut_fr_header_t header;  //!< Header of the FlexRay frame
  opensut_byte_vector payload; //!< Contains a pointer to a byte array providing the frame's payload and it's length
} opensut_fr_frame_t;

//! Specifies a FlexRay segment
typedef enum
{
  OPENSUT_FR_SEGMENT_STATIC  = 0x00U, //!< Static segment of the FlexRay cycle
  OPENSUT_FR_SEGMENT_DYNAMIC = 0x01U  //!< Dynamic segment of the FlexRay cycle
} opensut_fr_segment_t;

//! Specifies a FlexRay message
typedef struct
{
  size_t channel_id;            //!< Identifies the transmitting network
  opensut_time_t timestamp;     //!< Time at end of frame transmission
  opensut_fr_channel_t channel; //!< FlexRay channel A, B or AB
  uint8_t unused;               //!< This member is currently not used and will be ignored
  opensut_fr_frame_t* frame;    //!< Received FlexRay frame
} opensut_fr_message_t;

//! Protocol Operation Control (POC) state of the FlexRay communication controller (CC)
typedef enum
{
  OPENSUT_FR_POC_STATE_DEFAULT_CONFIG = 0x00U, //!< CC expects configuration, initial state after reset
  OPENSUT_FR_POC_STATE_CONFIG         = 0x01U, //!< CC is in configuration mode for setting communication parameters
  OPENSUT_FR_POC_STATE_READY          = 0x02U, //!< Intermediate state for initialization process (after Config)
  OPENSUT_FR_POC_STATE_STARTUP        = 0x03U, //!< FlexRay startup phase
  OPENSUT_FR_POC_STATE_WAKEUP         = 0x04U, //!< FlexRay wakeup phase
  OPENSUT_FR_POC_STATE_NORMAL_ACTIVE  = 0x05U, //!< Normal operating mode
  OPENSUT_FR_POC_STATE_NORMAL_PASSIVE = 0x06U, //!< Operating mode with transient or tolerable errors
  OPENSUT_FR_POC_STATE_HALT           = 0x07U  //!< CC is halted (caused by the application or by a fatal error)
} opensut_fr_poc_state_t;

//! Configuration of Tx-Buffer, used in opensut_fr_frame_registration_handler()
typedef struct
{
  size_t channel_id;               //!< Identifies the transmitting network
  opensut_fr_channel_t channels;   //!< FlexRay channel A, B or AB
  uint16_t slot_id;                //!< Frame slot number within the cycle, the frame shall be send in
  uint8_t base_cycle;              //!< Cycle number (0-63) in which the FlexRay controller starts sending the frame
  uint8_t repetition;              //!< Repetition factor (1,2,4,8,16,32,64) for periodic transmission
  opensut_fr_header_flags_t flags; //!< FlexRay message header flags
  uint8_t payload_length;          //!< Payload length: 0-127, 7 bits (the number of payload data bytes divided by two)
} opensut_fr_tx_buffer_config_t;

//! Configuration of the wakeup pattern, used in opensut_fr_wakeup_handler()
typedef struct
{
  size_t channel_id;             //!< Identifies the transmitting network
  opensut_fr_channel_t channels; //!< FlexRay channel A, B or AB
  uint16_t wu_count;             //!< Number of repetitions (2-63) of the wakeup symbol in a wakeup pattern
  uint16_t wu_tx_idle;           //!< Number of idle bits in a wakeup symbol
  uint16_t wu_tx_low;            //!< Number of low bits in a wakeup symbol
} opensut_fr_wakeup_pattern_t;

/*! This callback is invoked by the SUT for registering a FlexRay message.
 *  Since message registration happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the message was already registered or not.
 *  @param in_context used-defined context parameter.
 *  @param in_tx_buffer_config configuration data for the message to be registered. Pointer is valid during the call's
 *  lifetime only. If an implementation needs to preserve message data for asynchronous transmission, it needs to copy
 *  that to privately managed memory.
 */
typedef void (
  *opensut_fr_frame_registration_handler)(void* in_context, const opensut_fr_tx_buffer_config_t* in_tx_buffer_config);

/*! This callback is invoked by the SUT for updating a FlexRay frame buffer.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the frame buffer was already updated or not.
 *  @param in_context used-defined context parameter.
 *  @param in_tx_buffer_config message data to be transmitted. Pointer is valid during the call's lifetime only.
 *  If an implementation needs to preserve message data for asynchronous transmission, it needs to copy that
 *  to privately managed memory.
 *  @param in_payload the payload to be updated. Pointer is valid during the call's lifetime only.
 *  If an implementation needs to preserve message data for asynchronous transmission, it needs to copy that
 *  to privately managed memory.
 */
typedef void (*opensut_fr_update_buffer_handler)(void* in_context,
  const opensut_fr_tx_buffer_config_t* in_tx_buffer_config,
  const opensut_byte_vector* in_payload);

/*! This callback is invoked by the SUT for resetting the FlexRay communication controller.
 *  Since reset is asynchronous, an implementation shall return immediately, regardless of whether the FlexRay
 *  communication controller was already reset or not.
 *  @param in_context used-defined context parameter.
 *  @param in_channel_id identifies the transmitting network.
 */
typedef void (*opensut_fr_reset_cc_handler)(void* in_context, size_t in_channel_id);

/*! This callback is invoked by the SUT for transmitting a FlexRay wakeup pattern.
 *  Since message transmission happens asynchronously, an implementation shall return immediately,
 *  regardless of whether the wakeup pattern was already transmitted or not.
 *  @param in_context used-defined context parameter.
 *  @param in_wakeup_pattern configuration for the wakeup pattern to be transmitted. Pointer is valid during the call's
 *  lifetime only. If an implementation needs to preserve message data for asynchronous transmission, it needs to copy
 *  that to privately managed memory.
 */
typedef void (*opensut_fr_wakeup_handler)(void* in_context, const opensut_fr_wakeup_pattern_t* in_wakeup_pattern);

/*! Indicates reception of a FlexRay message to the SUT.
 *  @param in_message FlexRay message data to receive. The pointer needs to be valid during the call's lifetime.
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_message is nullptr or contains invalid data
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_fr_receive(const opensut_fr_message_t* in_message);

/*! Indicates a start cycle event to the SUT.
 *  @param in_channel_id identifies the transmitting network.
 *  @param in_cycle current value of the cycle counter.
 *  @param in_nm_vector length of the network management vector and a pointer to a byte array providing it's data.
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_channel_id or \p in_cycle is out of range
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_fr_start_cycle(size_t in_channel_id, uint8_t in_cycle, const opensut_byte_vector* in_nm_vector);

/*! Indicates a protocol operation control (POC) state change event to the SUT.
 *  @param in_channel_id identifies the transmitting network.
 *  @param in_poc_state next POC state.
 *  @return
 *  - OPENSUT_OK if reception was processed
 *  - OPENSUT_PARAMETER_INVALID if \p in_channel_id is out of range or if \p in_poc_state has an invalid value
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_poc_state(size_t in_channel_id, opensut_fr_poc_state_t in_poc_state);

/*! Registers the implementation for handling FlexRay frame registration requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler.
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
    in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_fr_set_frame_registration_handler(opensut_fr_frame_registration_handler in_handler, void* in_context);

/*! Registers the implementation for handling FlexRay buffer update requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler.
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
    in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_fr_set_update_buffer_handler(opensut_fr_update_buffer_handler in_handler, void* in_context);

/*! Registers the implementation for handling requests to reset the FlexRay communication controller.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler.
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
    in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_fr_set_reset_cc_handler(opensut_fr_reset_cc_handler in_handler, void* in_context);

/*! Registers the implementation for handling FlexRay wakeup pattern transmission requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler.
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
    in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_fr_set_wakeup_handler(opensut_fr_wakeup_handler in_handler, void* in_context);

/*! Registers the implementation for handling POC state requests.
 *  Only one handler may be registered during the whole SUT lifetime.
 *  @param in_handler points to the handler.
 *  @param in_context user-defined data. The OpenSUT implementation will forward this data to each invocation of
    in_handler. It does not attempt to interpret or dereference it.
 *  @return
 *  - OPENSUT_OK if handler was set successfully
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */

#pragma pack(pop)

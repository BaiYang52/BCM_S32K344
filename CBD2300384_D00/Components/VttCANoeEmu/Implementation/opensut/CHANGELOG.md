# Changelog
All notable changes to the OpenSUT programming interface will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning 2.0.0](https://semver.org/lang/de/).

## [3.0.0] - 2022-10-06
### Changed

* FlexRay: The member 'segment' of the opensut_fr_message_t struct was not used and can be ignored from now on.
  * flexray.h
    * old
      <pre>
      typedef struct
      {
        size_t channel_id;            //!< Identifies the transmitting network
        opensut_time_t timestamp;     //!< Time at end of frame transmission
        opensut_fr_channel_t channel; //!< FlexRay channel A, B or AB
        <b>opensut_fr_segment_t segment; //!< Static or dynamic segment of the cycle</b>
        opensut_fr_frame_t* frame;    //!< Received FlexRay frame
      } opensut_fr_message_t;
      </pre>
    * new
      <pre>
      typedef struct
      {
        size_t channel_id;            //!< Identifies the transmitting network
        opensut_time_t timestamp;     //!< Time at end of frame transmission
        opensut_fr_channel_t channel; //!< FlexRay channel A, B or AB
        <b>uint8_t unused;               //!< This member is currently not used and will be ignored</b>
        opensut_fr_frame_t* frame;    //!< Received FlexRay frame
      } opensut_fr_message_t;
      </pre>
* LIN: Adjust enum constants to SilKit constants
  * lin.h
    <pre>
    opensut_lin_checksum_model_t
    opensut_lin_frame_response_type_t
    opensut_lin_frame_status_t
    </pre>

## [2.0.0] - 2022-04-29
### Added
* RPC: Methods to support remote procedure calls. 
  * com_item.h
    <pre>
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_get(opensut_handle_t in_id, opensut_rpc_object* out_rpc_object);
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_arg_get_type(opensut_rpc_object in_rpc_object, opensut_rpc_object_arg_direction_t in_arg_direction, const opensut_type_t** out_type);
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_client_register_observer(opensut_rpc_object in_rpc_object, opensut_rpc_object_call_handler in_call_observer, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_server_begin_call(opensut_rpc_object in_rpc_object, opensut_rpc_object_call_context* out_call_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_server_invoke_call(opensut_rpc_object_call_context in_call_context, opensut_rpc_object_call_handler in_call_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_call_context_arg_read(opensut_rpc_object_call_context in_call_context, opensut_rpc_object_arg_direction_t in_arg_direction, void* out_buffer, size_t buffer_bytes);
    OPENSUT_FUNCTION(opensut_result_t) opensut_call_context_arg_write(opensut_rpc_object_call_context in_call_context, opensut_rpc_object_arg_direction_t in_arg_direction, const void* in_buffer, size_t buffer_bytes);
    OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_client_return(opensut_rpc_object_call_context* in_out_call_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_call_context_release(opensut_rpc_object_call_context* in_out_call_context);
    </pre>
  * typedefs.h: New opensut_result_t return value.
    <pre>
    typedef enum
    {
      ...
      <b>OPENSUT_CALL_CONTEXT_ARGS_NOT_SET = 5 //!< The corresponding arguments are not yet set</b>
    } opensut_result_t;
    </pre>
* FlexRay: Methods to support FlexRay communication.
  * flexray.h
    <pre>
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_receive(const opensut_fr_message_t* in_message);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_start_cycle(size_t in_channel_id, uint8_t in_cycle, const opensut_byte_vector* in_nm_vector);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_poc_state(size_t in_channel_id, opensut_fr_poc_state_t in_poc_state);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_frame_registration_handler(opensut_fr_frame_registration_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_update_buffer_handler(opensut_fr_update_buffer_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_reset_cc_handler(opensut_fr_reset_cc_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_fr_set_wakeup_handler(opensut_fr_wakeup_handler in_handler, void* in_context);
    </pre>
* LIN: Methods to support Local Interconnect Network (LIN) communication.
  * lin.h
    <pre>
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_receive(const opensut_lin_message_t* in_message, opensut_lin_frame_status_t in_frame_status);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_send_handler(opensut_lin_send_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_go_to_sleep(size_t channel);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_wakeup(size_t channel);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_go_to_sleep_handler(opensut_lin_go_to_sleep_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_wakeup_handler(opensut_lin_wakeup_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_go_to_sleep_internal_handler(opensut_lin_go_to_sleep_internal_handler in_handler, void* in_context);
    OPENSUT_FUNCTION(opensut_result_t) opensut_lin_set_wakeup_internal_handler(opensut_lin_wakeup_internal_handler in_handler, void* in_context);
    </pre>
* Logging: Method to set the loglevel for the SUT.
  * log.h
    <pre>
    OPENSUT_FUNCTION(opensut_result_t) opensut_set_log_level(opensut_log_level_t log_level);
    </pre>
* Versioning: Method and makros to get the version of the OpenSUT programming interface.
  * typedefs.h
    <pre>
    #define GET_OPENSUT_API_VERSION_MAJOR(version) ((version) / 100000)
    #define GET_OPENSUT_API_VERSION_MINOR(version) (((version) % 100000) / 100)
    #define GET_OPENSUT_API_VERSION_PATCH(version) ((version) % 100) 
    ...
    OPENSUT_FUNCTION(uint32_t) opensut_api_get_version();
    </pre>

* Types
  * typedefs.h: New init flag and bus tags.
    <pre>
    typedef enum
    {
      ...
      <b>OPENSUT_INIT_CONTROLLER_2 = 1</b>
    } opensut_init_flags;
    ...
    typedef enum
    {
      ...
      <b>OPENSUT_BUS_TAG_LIN      = 2, //!< Local Interconnect Network (LIN)</b>
      <b>OPENSUT_BUS_TAG_FLEXRAY  = 3  //!< FlexRay</b>
    } opensut_bus_tag_t;
    </pre>

### Changed
* Communication item: Communication items of RPC kind have been renamed.
  * com_item.h
    * old
      <pre>
      typedef enum
      {
        OPENSUT_DATA_ITEM_IN = 1,     //!< Data sink (data flows from environment to SUT)
        OPENSUT_DATA_ITEM_OUT = 2,    //!< Data source (data flows from SUT to environment)
        OPENSUT_DATA_ITEM_IN_OUT = 3, //!< Bidirectional object
        OPENSUT_RPC_<b>ITEM</b>_CLIENT = 4,  //!< Outbound function object (SUT calls environment)
        OPENSUT_RPC_<b>ITEM</b>_SERVER = 5   //!< Inbound function object (environment calls SUT)
      } opensut_communication_item_kind_t;
      </pre>
    * new
      <pre>
      typedef enum
      {
        <b>OPENSUT_COM_ITEM_UNDEFINED = 0, //!< only used for internal initialization purposes</b>
        OPENSUT_DATA_ITEM_IN       = 1, //!< Data sink (data flows from environment to SUT)
        OPENSUT_DATA_ITEM_OUT      = 2, //!< Data source (data flows from SUT to environment)
        OPENSUT_DATA_ITEM_IN_OUT   = 3, //!< Bidirectional object
        OPENSUT_RPC_<b>OBJECT</b>_CLIENT  = 4, //!< Outbound function object (SUT calls environment)
        OPENSUT_RPC_<b>OBJECT</b>_SERVER  = 5  //!< Inbound function object (environment calls SUT)
      } opensut_communication_item_kind_t;
      </pre>

* Versioning
  * typedefs.h
    * old
      <pre>
      /*! \brief OpenSUT version: MAJOR * <b>1000</b> + MINOR * 100 + <b>REVISION</b>.
      *  OpenSUT complies with semantic versioning 2.0.0 (https://semver.org/lang/de/).
      */
      #define OPENSUT_API_VERSION <b>1000</b>
      </pre>
    * new
      <pre>
      /*! \brief OpenSUT version: MAJOR * <b>100000</b> + MINOR * 100 + <b>PATCH</b>.
      *  OpenSUT complies with semantic versioning 2.0.0 (https://semver.org/lang/de/).
      */
      #define OPENSUT_API_VERSION <b>200000</b>
      </pre>

* Changed return values: The following methods will now return `OPENSUT_FAILED` if they are invoked before the simulation is running, i.e. `opensut_run_async_for()` has not yet been called.
  <pre>
  opensut_can_receive()
  opensut_eth_receive()
  </pre>

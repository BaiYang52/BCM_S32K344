/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file com_item.h
 * Contains declarations related to application-level communication with virtual ECUs.
 */

#pragma once

#include "opensut/type_system.h"
#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

//! Enumerates communication item categories.
typedef enum
{
  OPENSUT_COM_ITEM_UNDEFINED = 0, //!< only used for internal initialization purposes
  OPENSUT_DATA_ITEM_IN       = 1, //!< Data sink (data flows from environment to SUT)
  OPENSUT_DATA_ITEM_OUT      = 2, //!< Data source (data flows from SUT to environment)
  OPENSUT_DATA_ITEM_IN_OUT   = 3, //!< Bidirectional object
  OPENSUT_RPC_OBJECT_CLIENT  = 4, //!< Outbound function object (SUT calls environment)
  OPENSUT_RPC_OBJECT_SERVER  = 5  //!< Inbound function object (environment calls SUT)
} opensut_communication_item_kind_t;

/*! Callback handler for data update notifications.
 *  @param context User - defined content.The OpenSUT implementation does not attempt to dereference or interpret
 *         this parameter.
 */
typedef void (*opensut_data_update_handler)(void* context);

/*! Describes a communication item.
 *  A communication item is a generic communication channel between a SUT and its environment.
 *  It is intended for application-level exchange of data which are not mapped to network communication.
 *  Typical uses: Representing analog or digital I/O, ECU control signals (power, reset),
 *  AUTOSAR VFB-level interfacing, generic model I/O like with Functional Mockup Interface (FMI).
 *  There are two fundamental kinds of communication items: data items and function objects.
 *  Data items provide signal-oriented data exchange. The dataflow can be from environment to SUT,
 *  from SUT to environment, or bidirectional. Data items support exchanging any serializable data.
 *  A description of an item's data type (which includes its binary serialization) can be obtained from
 *  opensut_data_item_get_type().
 *  RPC objects support asynchronous "remote" procedure calls. In this context, the notion "remote" does not
 *  necessarily refer to process boundaries. However, the context from which we initiate calls to the SUT or accept
 *  calls from the SUT, respectively, resides conceptually "outside" the SUT. We still need to serialize call
 *  parameters forth and back and need to cope with asynchrony, since the SUT's threading model is isolated from ours.
 *  Any RPC object is either inbound (SUT is server) or outbound (SUT is client). A description of the RPC object
 *  function parameters type (which includes its binary serialization) can be obtained from
 *  opensut_rpc_object_arg_get_type().
 */
typedef struct
{
  opensut_handle_t id;                    //!< unique instance handle
  const char* name;                       //!< item name. Memory must not be explicitly freed and remains
                                          //   valid as long as SUT is in state 'initialized'.
  opensut_communication_item_kind_t kind; //!< classifies the item
} opensut_communication_item_t;

/*! Retrieves the total count of communication items in this SUT.
 *  @param out_count receives the retrieved count.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if out_count is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_communication_item_count(size_t* out_count);

/*! Retrieves a communication item description given by the index
 *  @param in_com_item_index zero-based index of the communication item to retrieve
 *  @param out_communication_item receives the desired descriptor
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_com_item_index is out of range or \p out_communication_item is nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_communication_item_get(size_t in_com_item_index, opensut_communication_item_t* out_communication_item);

//! Opaque handle type for data items
typedef void* opensut_data_item;

/*! Accesses a communication item as data item. The retrieved opaque handle may be used for further interaction.
 *  The handle does not need to be explicitly closed and remains valid as long as the SUT is in 'initialized' state.
 *  @param in_id id obtained from an item descriptor
 *  @param out_data_item receives an opaque handle which can be passed to opensut_data_item_get_type(),
 *    opensut_data_item_set_data_update_handler(), opensut_data_item_read(), and opensut_data_item_write().
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_id is invalid or \p out_data_item is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_data_item_get(opensut_handle_t in_id, opensut_data_item* out_data_item);

/*! Retrieves the description of a given data item's type and binary serialization.
 *  @param in_data_item item handle (obtained from opensut_data_item_get() )
 *  @param out_type receives a pointer to the item's type description.
 *         The pointer must not be explicitly freed. It will remain valid as long as the SUT is in
           'initialized' state.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_data_item is invalid or \p out_type is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_data_item_get_type(opensut_data_item in_data_item, const opensut_type_t** out_type);

/*! Registers a callback handler which will be invoked whenever the data item's value was updated.
 *  This function is applicable to OPENSUT_DATA_ITEM_IN and OPENSUT_DATA_ITEM_IN_OUT items.
 *  The change of an item's value always triggers an update, but the inverse is not true: Update events may also occur
 *  when the item's value remains the same. Think of a sensor which periodically transmits measurements: A update
 *  indicates that a new measurement result is available, which might be identical to the previous measurement result.
 *  The callback invocation always happens on opensut_do_callbacks().
 *  Only one callback handler per data item is supported. If the function is called more than once for a single data
 *  item, then a previously registered callback function is unregistered. Use a nullptr to explicitly unregister a
 *  callback handler.
 *  @param in_data_item item handle (obtained from opensut_data_item_get() ).
 *  @param in_handler Function pointer to a callback handler or a nullptr.
 *  @param in_context User-defined content which will be passed to the callback handler upon any invocation.
 *         The OpenSUT implementation does not attempt to dereference or interpret this parameter.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_data_item is invalid
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_data_item_set_data_update_handler(opensut_data_item in_data_item,
  opensut_data_update_handler in_handler,
  void* in_context);

/*! Reads a data item's current value. This function is applicable to OPENSUT_DATA_ITEM_OUT and
 *  OPENSUT_DATA_ITEM_IN_OUT items.
 *  The supplied buffer must provide exactly the amount of space which is required to hold the entire item value.
 *  The required buffer size can be computed from evaluating the item's data type, obtained from
 *  opensut_data_item_get_type(). The OpenSUT implementation ensures data consistency, even if SUT is in 'running'
 *  state.
 *  @param in_data_item item handle (obtained from opensut_data_item_get() )
 *  @param out_buffer points to a caller-provided buffer, into which the implementation will copy the item value.
 *  @param buffer_bytes depicts the buffer size (in bytes)
 *  @return
 *  - OPENSUT_OK if item's value was successfully copied to out_buffer
 *  - OPENSUT_PARAMETER_INVALID if \p in_data_item is invalid, \p out_buffer is nullptr, or the provided
 *    buffer space is insufficient
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_data_item_read(opensut_data_item in_data_item, void* out_buffer, size_t buffer_bytes);

/*! Updates a data item's value. This function is applicable to OPENSUT_DATA_ITEM_IN and
 *  OPENSUT_DATA_ITEM_IN_OUT items. The supplied buffer must provide exactly the amount of space which is required to
 *  hold the entire item value.
 *  @param in_data_item item handle (obtained from opensut_data_item_get() )
 *  @param in_buffer points to a caller-provided buffer, from which the implementation will copy the item value.
 *  @param buffer_bytes depicts the buffer size (in bytes)
 *  @return
 *  - OPENSUT_OK if item's value was successfully updated
 *  - OPENSUT_PARAMETER_INVALID if \p in_data_item is invalid, \p in_buffer is nullptr, or the buffer
 *    size does not match the size expected from the item's data type
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_data_item_write(opensut_data_item in_data_item, const void* in_buffer, size_t buffer_bytes);

//! Possible function argument directions of a RPC object.
typedef enum
{
  OPENSUT_RPC_OBJECT_ARG_DIRECTION_UNDEFINED = 0, //!< only used for internal initialization purposes
  OPENSUT_RPC_OBJECT_ARG_DIRECTION_IN        = 1, //!< used for IN and INOUT arguments
  OPENSUT_RPC_OBJECT_ARG_DIRECTION_OUT       = 2, //!< used for OUT and INOUT arguments
} opensut_rpc_object_arg_direction_t;

//! Opaque handle type for RPC objects
typedef void* opensut_rpc_object;

/*! Accesses a communication item as RPC object. The retrieved opaque handle may be used for further interaction.
 *  The handle does not need to be explicitly closed and remains valid as long as the SUT is in 'initialized' state.
 *  @param in_id Id obtained from an item descriptor
 *  @param out_rpc_object Receives an opaque handle which can be passed to opensut_rpc_object_arg_get_type().
 *         Furthermore, client RPC objects can be passed to opensut_rpc_object_client_register_observer()
 *         and server RPC objects to opensut_rpc_object_server_begin_call().
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_id is invalid or \p out_rpc_object is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_rpc_object_get(opensut_handle_t in_id, opensut_rpc_object* out_rpc_object);

/*! Retrieves the description of a given RPC object's argument type and binary serialization for the specified
 *  direction. If the function returns something other than void, the OUT argument type will also contain the return
 *  value.
 *  @param in_rpc_object RPC object handle (obtained from opensut_rpc_object_get() ).
 *  @param in_arg_direction Has to be either OPENSUT_RPC_OBJECT_ARG_DIRECTION_IN or
 *         OPENSUT_RPC_OBJECT_ARG_DIRECTION_OUT to get the type description for IN and INOUT
 *         respectively OUT and INOUT arguments.
 *  @param out_type Receives a pointer to the RPC object's type description. The type definition will always be
 *         of type opensut_struct_type_t. The pointer must not be explicitly freed. It will remain valid as long as
 *         the SUT is in 'initialized' state.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_rpc_object is invalid, \p in_arg_direction is undefined
 *    or \p out_type is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_rpc_object_arg_get_type(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_arg_direction_t in_arg_direction,
  const opensut_type_t** out_type);

//! Opaque handle type for call contexts
typedef void* opensut_rpc_object_call_context;

/*! Callback handler for function call notifications. Used as call observer for client RPC objects
 *  and as callback function for server RPC objects.
 *  @param in_context User-defined content. The OpenSUT implementation does not attempt to dereference or interpret
 *         this parameter.
 *  @param in_call_context Context of the call.
 */
typedef void (*opensut_rpc_object_call_handler)(void* in_context, opensut_rpc_object_call_context in_call_context);

/*! Whenever the SUT as a client invokes a call on the server side this registered observer is called.
 *  This function is applicable to OPENSUT_RPC_OBJECT_CLIENT objects (the SUT is the client).
 *  Only one observer per RPC object is supported. If the function is called more than once for a single RPC object,
 *  then a previously registered observer is unregistered. Use a nullptr to to explicit unregister a callback handler.
 *  If no server is registered for a particular object, any call will be internally answered with RTE_E_UNCONNECTED.
 *  The observer invocation always happens on opensut_do_callbacks().
 *  @param in_rpc_object Client RPC object handle (obtained from opensut_rpc_object_get() ).
 *  @param in_call_observer Function pointer to a call observer or a nullptr.
 *  @param in_context User-defined content which will be passed to the observer upon any invocation.
 *         The OpenSUT implementation does not attempt to dereference or interpret this parameter.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_rpc_object is invalid
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_rpc_object_client_register_observer(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_call_handler in_call_observer,
  void* in_context);

/*! Initiate a function call on the SUT. The received call context can be passed to opensut_call_context_arg_write()
 *  to set all IN and INOUT arguments and to opensut_rpc_object_server_invoke_call() to invoke the call on the SUT.
 *  The received call context can be used for exactly one function call. For issuing a second function call,
 *  opensut_rpc_object_server_begin_call() must be invoked again to obtain a new call context. To prevent a resource
 *  leak, any call context obtained by this routine must be released by opensut_call_context_release().
 *  This function is applicable to OPENSUT_RPC_OBJECT_SERVER objects (the SUT is the server).
 *  @param in_rpc_object Server RPC object handle (obtained from opensut_rpc_object_get() ).
 *  @param out_call_context Pointer to the context of the call.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_rpc_object is invalid or \p out_call_context is a nullptr
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 *    or if no observer on the SUT-side has been registered
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_rpc_object_server_begin_call(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_call_context* out_call_context);

/*! Trigger a function call that was previously initiated by opensut_rpc_object_server_begin_call() and after all IN or
 *  INOUT arguments (if any) were set by opensut_call_context_arg_write().
 *  This function is applicable to OPENSUT_RPC_OBJECT_SERVER objects. It may be called from any thread context.
 *  @param in_call_context Context of the call (obtained from opensut_rpc_object_server_begin_call() ).
 *  @param in_call_handler Function pointer to a callback handler or a nullptr. The callback will be invoked once the
 *         call returns.
 *  @param in_context User-defined content which will be passed to the observer upon any invocation.
 *         The OpenSUT implementation does not attempt to dereference or interpret this parameter.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p in_call_context is nullptr, belongs to a client object or is already in use
 *    by a previous invocation
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_CALL_CONTEXT_ARGS_NOT_SET if the function to be invoked takes any IN or INOUT arguments but they are not
 *    yet set
 *  - OPENSUT_FAILED if simulation is not yet running (see opensut_run_async_for() )
 *    or if no observer on the SUT-side has been registered
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_rpc_object_server_invoke_call(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_call_handler in_call_handler,
  void* in_context);

/*! Reads the value of all arguments for a given call context and argument direction. On the server side (within an
 *  opensut_rpc_object_call_handler() function) IN and INOUT arguments can be read. On the client side (context of the
 *  call obtained from opensut_rpc_object_server_begin_call() ) OUT and INOUT arguments can be read. If the function
 *  returns something other than void, the OUT argument buffer will also contain the return value.
 *  The supplied buffer must provide exactly the amount of space which is required to hold all arguments of the
 *  specified direction. The required buffer size can be computed from evaluating the argument's type, obtained from
 *  opensut_rpc_object_arg_get_type().
 *  @param in_call_context Context of the call (obtained from opensut_rpc_object_server_begin_call() or as parameter
 *         within an opensut_rpc_object_call_handler() function).
 *  @param in_arg_direction Has to be either OPENSUT_RPC_OBJECT_ARG_DIRECTION_IN or
 *         OPENSUT_RPC_OBJECT_ARG_DIRECTION_OUT to read IN and INOUT respectively OUT and INOUT arguments.
 *  @param out_buffer Points to a caller-provided buffer, into which the implementation will copy the argument values.
 *  @param buffer_bytes Depicts the buffer size (in bytes).
 *  @return
 *  - OPENSUT_OK if argument values were successfully copied to out_buffer
 *  - OPENSUT_PARAMETER_INVALID if \p in_call_context is nullptr, \p in_arg_direction is undefined,
 *    \p out_buffer is nullptr, or the provided buffer space is insufficient
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_CALL_CONTEXT_ARGS_NOT_SET if the requested argument is not yet available
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_call_context_arg_read(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_arg_direction_t in_arg_direction,
  void* out_buffer,
  size_t buffer_bytes);

/*! Sets the value of all arguments for a given call context and argument direction. On the server side (within an
 *  opensut_rpc_object_call_handler() function) OUT and INOUT arguments can be written. On the client side (context of
 *  the call obtained from opensut_rpc_object_server_begin_call() ) IN and INOUT arguments can be written. If the
 *  function returns something other than void, the OUT argument buffer also has to contain the return value.
 *  The supplied buffer must provide exactly the amount of space which is required to hold all arguments of the
 *  specified direction.
 *  @param in_call_context Context of the call (obtained from opensut_rpc_object_server_begin_call() or as parameter
 *         within an opensut_rpc_object_call_handler() function).
 *  @param in_arg_direction Has to be either OPENSUT_RPC_OBJECT_ARG_DIRECTION_IN or
 *         OPENSUT_RPC_OBJECT_ARG_DIRECTION_OUT to set IN and INOUT respectively OUT and INOUT arguments.
 *  @param in_buffer Points to a caller-provided buffer, from which the implementation will copy the argument values.
 *  @param buffer_bytes Depicts the buffer size (in bytes).
 *  @return
 *  - OPENSUT_OK if argument values were successfully set.
 *  - OPENSUT_PARAMETER_INVALID if \p in_call_context is nullptr, \p in_arg_direction is undefined,
 *    \p in_buffer is nullptr, the buffer size does not match the size expected from the arguments type,
 *    or if the requested argument was already submitted
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_call_context_arg_write(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_arg_direction_t in_arg_direction,
  const void* in_buffer,
  size_t buffer_bytes);

/*! Notify a SUT-side client object that the function call has been processed and the function returns. This function
 *  may be called from within the user's opensut_rpc_object_call_handler() function, but it may also be called
 *  from an arbitrary thread context. Returning the function call automatically releases the call context. Do not call
 *  opensut_call_context_release(), since this would result in undefined behaviour.
 *  @param in_out_call_context Pointer to the context of the call (obtained as parameter within a client's
 *         opensut_rpc_object_call_handler() function). *in_out_call_context will be nullptr after invocation.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p *in_out_call_context is nullptr or belongs to a server object
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 *  - OPENSUT_CALL_CONTEXT_ARGS_NOT_SET if the function returns any OUT or INOUT arguments but they are not yet set
 */
OPENSUT_FUNCTION(opensut_result_t)
opensut_rpc_object_client_return(opensut_rpc_object_call_context* in_out_call_context);

/*! Release the memory resources associated with a call context obtained from opensut_rpc_object_server_begin_call().
 *  This function must be called exactly once per call context obtained from opensut_rpc_object_server_begin_call().
 *  Omitting it leads to a memory leak. Calling it twice results in undefined behaviour. Do not call this function
 *  for call contexts belonging to client objects. These are released implicitly by opensut_rpc_object_client_return().
 *  @param in_out_call_context Pointer to the call context to be released. *in_out_call_context will be nullptr after
 *         invocation.
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if \p *in_out_call_context is nullptr or belongs to a client object
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_call_context_release(opensut_rpc_object_call_context* in_out_call_context);

#pragma pack(pop)

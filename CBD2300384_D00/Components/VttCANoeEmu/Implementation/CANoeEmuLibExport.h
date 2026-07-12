/*----------------------------------------------------------------------------------------------------------------------
Module: CANoeEmu - AUTOSAR platform emulator for CANoe and VTT
Interfaces: all
------------------------------------------------------------------------------------------------------------------------
This header file provides required APIs for loading a DLL / shared Library and contains the export functions for a 
CANoe NodeLayer module and for the OpenSUT API.
------------------------------------------------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
----------------------------------------------------------------------------------------------------------------------*/
#pragma once

#if !defined(__cplusplus)
#  error "This is a header file for the C++ programming language. Do not include it from pure C code."
#endif

#include <cstdint>

extern "C"
{
#include "opensut/log.h"
#include "opensut/sut.h"
#include "opensut/bus.h"
#include "opensut/can.h"
#include "opensut/eth.h"
#include "opensut/lin.h"
#include "opensut/flexray.h"
#include "opensut/com_item.h"
}

class VIAService;
class VIAModuleApi;
struct CAPL_DLL_INFO;

namespace NCANoeEmu
{
  // ===================================================================================================================
  // Library Management
  // ===================================================================================================================

  // Initialize emulator immediately after loading of the library
  void OnLibraryLoad();

  // Finalize emulator just before unloading of the library
  void OnLibraryUnLoad();

  // Windows Only: Configure the CANoeEmu-Library with the instance handle of the DLL.
  // The function CANoeAPI_SetDllInstanceHandle should be called by the VTT framework, when the DLL is loaded into a
  // windows process. The call is normally done in the call of function 'DllMain' of the Visual Studio C Runtime.
  void SetDllInstanceHandle(void* dllInstanceHandle);

  // ===================================================================================================================
  // Required functionality for a CANoe NodeLayer library (VIA)
  // ===================================================================================================================
  
  void VIARequiredVersion(int32_t* majorversion, int32_t* minorversion);
  void VIADesiredVersion(int32_t* majorversion, int32_t* minorversion);
  void VIASetService(VIAService* service);
  VIAModuleApi* VIAGetModuleApi(int32_t argc, char* argv[]);
  void VIAReleaseModuleApi(VIAModuleApi* api);
  int32_t NLGetModuleOptions(int32_t option);

  // ===================================================================================================================
  // Required functionality for a CANalyzer/CANoe CAPL DLL
  // ===================================================================================================================

  CAPL_DLL_INFO* GetCaplDllTable();

  // ===================================================================================================================
  // Functionality for creating a list of defines system variables
  // ===================================================================================================================
 
  int32_t DumpSut(const wchar_t* path);

  // ===================================================================================================================
  // Functionality for VTT-SIL-API (OpenSUT)
  // ===================================================================================================================

  namespace OpenSUT
  {
    uint32_t opensut_api_get_version();
    opensut_result_t opensut_set_log_level(opensut_log_level_t log_level);
    opensut_result_t opensut_init(opensut_init_flags in_init_flags);
    opensut_result_t opensut_shutdown();

    namespace SUT
    {
      opensut_result_t opensut_run_async_for(opensut_time_duration_t in_diff_time);
      opensut_result_t opensut_get_time_of_next_event(opensut_time_t* out_time);
      opensut_result_t opensut_get_poll_handle(portable_handle_t* out_handle);
      opensut_result_t opensut_do_callbacks(opensut_time_duration_t in_timeout);
    }

    namespace Bus
    {
      opensut_result_t opensut_bus_get_config_count(size_t* out_count);
      opensut_result_t opensut_bus_get_config(size_t in_config_idx, opensut_bus_cfg_t* out_config);
    }

    namespace CAN
    {
      opensut_result_t opensut_can_receive(const opensut_can_message_t* in_message);
      opensut_result_t opensut_can_acknowledge(size_t in_send_job_id);
      opensut_result_t opensut_can_set_send_handler(opensut_can_send_handler in_handler, void* in_context);
    }

    namespace Eth
    {
      opensut_result_t opensut_eth_receive(const opensut_eth_message_t* in_message);
      opensut_result_t opensut_eth_acknowledge(size_t in_send_job_id);
      opensut_result_t opensut_eth_set_send_handler(opensut_eth_send_handler in_handler, void* in_context);
    }

    namespace LIN
    {
      opensut_result_t opensut_lin_receive(const opensut_lin_message_t* in_message, opensut_lin_frame_status_t in_frame_status);
      opensut_result_t opensut_lin_set_send_handler(opensut_lin_send_handler in_handler, void* in_context);
      opensut_result_t opensut_lin_wakeup(size_t channel);
      opensut_result_t opensut_lin_go_to_sleep(size_t channel);
      opensut_result_t opensut_lin_set_go_to_sleep_handler(opensut_lin_go_to_sleep_handler in_handler, void* in_context);
      opensut_result_t opensut_lin_set_wakeup_handler(opensut_lin_wakeup_handler in_handler, void* in_context);
      opensut_result_t opensut_lin_set_go_to_sleep_internal_handler(opensut_lin_go_to_sleep_internal_handler in_handler, void* in_context);
      opensut_result_t opensut_lin_set_wakeup_internal_handler(opensut_lin_wakeup_internal_handler in_handler, void* in_context);
    }

    namespace FlexRay
    {
      opensut_result_t opensut_fr_receive(const opensut_fr_message_t* in_message);
      opensut_result_t opensut_fr_start_cycle(size_t in_channel_id, uint8_t in_cycle, const opensut_byte_vector* in_nm_vector);
      opensut_result_t opensut_fr_set_poc_state(size_t in_channel_id, opensut_fr_poc_state_t in_poc_state);
      opensut_result_t opensut_fr_set_frame_registration_handler(
        opensut_fr_frame_registration_handler in_handler, void* in_context);
      opensut_result_t opensut_fr_set_update_buffer_handler(opensut_fr_update_buffer_handler in_handler, void* in_context);
      opensut_result_t opensut_fr_set_reset_cc_handler(opensut_fr_reset_cc_handler in_handler, void* in_context);
      opensut_result_t opensut_fr_set_wakeup_handler(opensut_fr_wakeup_handler in_handler, void* in_context);
    }
	
    namespace ComItem
    {
      opensut_result_t opensut_communication_item_count(size_t* out_count);
      opensut_result_t opensut_communication_item_get(size_t in_com_item_index,
        opensut_communication_item_t* out_communication_item);
      opensut_result_t opensut_data_item_get(opensut_handle_t in_handle, opensut_data_item* out_data_item);
      opensut_result_t opensut_data_item_get_type(opensut_data_item in_data_item, const opensut_type_t** out_type_ptr);
      opensut_result_t opensut_data_item_set_data_update_handler(opensut_data_item in_data_item,
        opensut_data_update_handler in_handler, void* in_context);
      opensut_result_t opensut_data_item_read(opensut_data_item in_data_item, void* out_buffer, size_t buffer_bytes);
      opensut_result_t opensut_data_item_write(opensut_data_item in_data_item, const void* in_buffer,
        size_t buffer_bytes);
      opensut_result_t opensut_rpc_object_get(opensut_handle_t in_handle, opensut_rpc_object* out_rpc_object);
      opensut_result_t opensut_rpc_object_arg_get_type(opensut_rpc_object in_rpc_object,
        opensut_rpc_object_arg_direction_t in_arg_direction, const opensut_type_t** out_type_ptr);
      opensut_result_t opensut_rpc_object_client_register_observer(opensut_rpc_object in_rpc_object,
        opensut_rpc_object_call_handler in_call_observer, void* in_call_context);
      opensut_result_t opensut_rpc_object_server_begin_call(opensut_rpc_object in_rpc_object,
        opensut_rpc_object_call_context* out_call_context);
      opensut_result_t opensut_rpc_object_server_invoke_call(opensut_rpc_object_call_context in_call_context,
        opensut_rpc_object_call_handler in_call_handler, void* in_context);
      opensut_result_t opensut_call_context_arg_read(opensut_rpc_object_call_context in_call_context,
        opensut_rpc_object_arg_direction_t in_arg_direction, void* out_buffer, size_t buffer_bytes);
      opensut_result_t opensut_call_context_arg_write(opensut_rpc_object_call_context in_call_context,
        opensut_rpc_object_arg_direction_t in_arg_direction, const void* in_buffer, size_t buffer_bytes);
      opensut_result_t opensut_rpc_object_client_return(opensut_rpc_object_call_context* in_out_call_context);
      opensut_result_t opensut_call_context_release(opensut_rpc_object_call_context* in_out_call_context);
    }
  }
} // namespace NCANoeEmu
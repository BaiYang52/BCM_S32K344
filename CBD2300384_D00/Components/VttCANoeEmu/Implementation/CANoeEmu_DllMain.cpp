/*----------------------------------------------------------------------------------------------------------------------
Module: CANoeEmu - AUTOSAR platform emulator for CANoe and VTT
------------------------------------------------------------------------------------------------------------------------
This source file provides the functions, that must be exported from the VTT SUT Library. The Library is a  dynamic link
library (DLL) for window or a shared object (SO) for Linux.

Also the DllMain function for a Windows DLL is implemented here. The main purpose of DLLMain is to initialize the
CANoeEmu-Library with the DLL instance handle and the activation of the memory leak detection feature of the
C Runtime from Visual Studio.
------------------------------------------------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
----------------------------------------------------------------------------------------------------------------------*/


#if defined(_MSC_VER)
#  pragma warning( disable : 4820 )
#  pragma warning( disable : 4514 )
#  pragma warning( disable : 4668 )
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1910)
#  pragma warning( disable : 5039 )
#endif

#if defined(__linux__) && (defined (__clang__) || defined (__GNUC__))
// Linux-x64 platform (64-Bit) with GCC or Clang compiler.
// The CANoeEmu-Library together with BSW-Emulation-Runtime requires the C++ 11 ABI of the GNU standard C++ library.
#  if !defined(_GLIBCXX_USE_CXX11_ABI)
#    error "Invalid compiler setting, Macro _GLIBCXX_USE_CXX11_ABI is undefined. CANoeEmu-Library requires the C++ 11 ABI of the GNU standard C++ library!"
#  elif (_GLIBCXX_USE_CXX11_ABI == 0)
#    error "Invalid compiler setting, Macro _GLIBCXX_USE_CXX11_ABI is zero. CANoeEmu-Library requires the C++ 11 ABI of the GNU standard C++ library!"
#  endif
#elif defined(_WIN32) && defined(__MINGW32__)
// Windows platform (64-Bit or 32-Bit) with MINGW32 compiler
// The CANoeEmu-Library together with BSW-Emulation-Runtime requires the C++ 11 ABI of the GNU standard C++ library.
#  if defined(_GLIBCXX_USE_CXX11_ABI) && (_GLIBCXX_USE_CXX11_ABI != 1)
#    error "Invalid compiler setting, CANoeEmu-Library requires the C++ 11 ABI of the GNU standard C++ library!"
#  endif
#endif

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#  include <crtdbg.h> 
#endif

#include <cstdint>

#if defined(_WIN64) && (defined(_MSC_VER) || defined(__MINGW32__))
  // Windows-x64 platform (64-Bit) with Visual Studio or MINGW32 compiler
#define CANOEEMU_SUT_EXPORT __declspec(dllexport)
#define OPENSUT_API __declspec(dllexport)
#define CANOEEMU_CAPLDECL
#define CANOEEMU_VIADECL __stdcall
#elif (defined(_WIN32) && !defined(_WIN64)) && (defined(_MSC_VER) || defined(__MINGW32__))
  // Windows-x86 platform (32-Bit) with Visual Studio or MINGW32 compiler
#define CANOEEMU_SUT_EXPORT __declspec(dllexport)
#define OPENSUT_API __declspec(dllexport)
#define CANOEEMU_CAPLDECL __cdecl
#define CANOEEMU_VIADECL __stdcall
#elif defined(__linux__) && (defined (__clang__) || defined (__GNUC__))
  // Linux-x64 platform (64-Bit) with GCC or Clang compiler
#define CANOEEMU_SUT_EXPORT __attribute__((visibility("default")))
#define OPENSUT_API __attribute__((visibility("default")))
#define CANOEEMU_CAPLDECL
#define CANOEEMU_VIADECL
#else
#error "Unknown Compiler"
#endif


#include "CANoeApi.h"
#include "CANoeEmuLibExport.h"
extern "C"
{
#include "opensut/log.h"
#include "opensut/sut.h"
#include "opensut/bus.h"
#include "opensut/can.h"
#include "opensut/com_item.h"
}

// =====================================================================================================================
// The DllMain function for a Windows DLL.
// This function is called by the windows system (DLL-Loader) when loading/unloading a DLL.
// =====================================================================================================================

#if defined(_WIN32)
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
  if (dwReason == DLL_PROCESS_ATTACH)
  {
    // Example for setting a breakpoint when reaching the given allocation number
    //
    //   _CrtSetBreakAlloc(161);
    //

    // Perform automatic leak checking when DLL is unloaded.
    // _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    // Example for a memory leak. Uncomment the next line to see memory leak checking in action.
    //
    //   int* a = new int;
    // 

    // Initialize the CANoe Emulation Library
    NCANoeEmu::SetDllInstanceHandle(hInstance);
    NCANoeEmu::OnLibraryLoad();
  }

  if (dwReason == DLL_PROCESS_DETACH)
  {
    // Finalize the CANoe Emulation Library
    NCANoeEmu::OnLibraryUnLoad();

    // We can do an explicit dump for memory leaks here for easier debugging, but normally it is better doing it later, 
    // because the destructor functions of global C++ variables are not call yet.
    // For an explicit dump do the following:  _CrtDumpMemoryLeaks();
  }

  return TRUE;
}
#endif

// =====================================================================================================================
// Initialize and finalize functions for a shared library on Linux.
// This function are called by the Linux system (ELF loader) when loading/unloading a shared library.
// =====================================================================================================================

#if defined(__linux__)
__attribute__((constructor))
static void initialize_library()
{
  // Initialize the CANoe Emulation Library
  NCANoeEmu::OnLibraryLoad();
}


__attribute__((destructor))
static void finalize_library()
{
  // Finalize the CANoe Emulation Library
  NCANoeEmu::OnLibraryUnLoad();
}
#endif

// =====================================================================================================================
// Required functions for a CANoe NodeLayer library (VIA)
// =====================================================================================================================

extern "C" CANOEEMU_SUT_EXPORT
void CANOEEMU_VIADECL VIARequiredVersion(int32_t* majorversion, int32_t* minorversion)
{
  NCANoeEmu::VIARequiredVersion(majorversion, minorversion);
}

extern "C" CANOEEMU_SUT_EXPORT
void CANOEEMU_VIADECL VIADesiredVersion(int32_t* majorversion, int32_t* minorversion)
{
  NCANoeEmu::VIADesiredVersion(majorversion, minorversion);
}

extern "C" CANOEEMU_SUT_EXPORT
void CANOEEMU_VIADECL VIASetService(VIAService* service)
{
  NCANoeEmu::VIASetService(service);
}

extern "C" CANOEEMU_SUT_EXPORT
VIAModuleApi* CANOEEMU_VIADECL VIAGetModuleApi(int32_t argc, char* argv[])
{
  return NCANoeEmu::VIAGetModuleApi(argc, argv);
}

extern "C" CANOEEMU_SUT_EXPORT
void CANOEEMU_VIADECL VIAReleaseModuleApi(VIAModuleApi* api)
{
  NCANoeEmu::VIAReleaseModuleApi(api);
}

extern "C" CANOEEMU_SUT_EXPORT
int32_t CANOEEMU_VIADECL NLGetModuleOptions(int32_t option)
{
  return NCANoeEmu::NLGetModuleOptions(option);
}

// =====================================================================================================================
// Required functions for tool VttDumpSut.exe
// =====================================================================================================================

extern "C" CANOEEMU_SUT_EXPORT
int32_t CANOEEMU_VIADECL CANoeEmuVTT_DumpSut(const wchar_t* path)
{
  return NCANoeEmu::DumpSut(path);
}

// =====================================================================================================================
// Required functions for a for a CANalyzer/CANoe CAPL DLL
// =====================================================================================================================

extern "C" CANOEEMU_SUT_EXPORT
CAPL_DLL_INFO* CANOEEMU_CAPLDECL caplDllGetTable(void)
{
  return NCANoeEmu::GetCaplDllTable();
}

// =====================================================================================================================
// Required functions for VTT-SIL-API (OpenSUT)
// =====================================================================================================================

extern "C" OPENSUT_API
uint32_t opensut_api_get_version()
{
  return NCANoeEmu::OpenSUT::opensut_api_get_version();
}

extern "C" OPENSUT_API
opensut_result_t opensut_set_log_level(opensut_log_level_t log_level)
{
  return NCANoeEmu::OpenSUT::opensut_set_log_level(log_level);
}

extern "C" OPENSUT_API
opensut_result_t opensut_init(opensut_init_flags in_init_flags)
{
  return NCANoeEmu::OpenSUT::opensut_init(in_init_flags);
}

extern "C" OPENSUT_API
opensut_result_t opensut_shutdown()
{
  return NCANoeEmu::OpenSUT::opensut_shutdown();
}

extern "C" OPENSUT_API
opensut_result_t opensut_run_async_for(opensut_time_duration_t in_diff_time)
{
  return NCANoeEmu::OpenSUT::SUT::opensut_run_async_for(in_diff_time);
}

extern "C" OPENSUT_API
opensut_result_t opensut_get_time_of_next_event(opensut_time_t* out_time)
{
  return NCANoeEmu::OpenSUT::SUT::opensut_get_time_of_next_event(out_time);
}

extern "C" OPENSUT_API
opensut_result_t opensut_get_poll_handle(portable_handle_t* out_handle)
{
  return NCANoeEmu::OpenSUT::SUT::opensut_get_poll_handle(out_handle);
}

extern "C" OPENSUT_API
opensut_result_t opensut_do_callbacks(opensut_time_duration_t in_timeout)
{
  return NCANoeEmu::OpenSUT::SUT::opensut_do_callbacks(in_timeout);
}

extern "C" OPENSUT_API
opensut_result_t opensut_bus_get_config_count(size_t* out_count)
{
  return NCANoeEmu::OpenSUT::Bus::opensut_bus_get_config_count(out_count);
}

extern "C" OPENSUT_API
opensut_result_t opensut_bus_get_config(size_t in_config_idx, opensut_bus_cfg_t* out_config)
{
  return NCANoeEmu::OpenSUT::Bus::opensut_bus_get_config(in_config_idx, out_config);
}

extern "C" OPENSUT_API
opensut_result_t opensut_can_receive(const opensut_can_message_t* in_message)
{
  return NCANoeEmu::OpenSUT::CAN::opensut_can_receive(in_message);
}

extern "C" OPENSUT_API
opensut_result_t opensut_can_acknowledge(size_t in_send_job_id)
{
  return NCANoeEmu::OpenSUT::CAN::opensut_can_acknowledge(in_send_job_id);
}

extern "C" OPENSUT_API
opensut_result_t opensut_can_set_send_handler(opensut_can_send_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::CAN::opensut_can_set_send_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_communication_item_count(size_t* out_count)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_communication_item_count(out_count);
}

extern "C" OPENSUT_API
opensut_result_t opensut_communication_item_get(size_t in_com_item_index,
  opensut_communication_item_t* out_communication_item)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_communication_item_get(in_com_item_index, out_communication_item);
}

extern "C" OPENSUT_API
opensut_result_t opensut_data_item_get(opensut_handle_t in_handle, opensut_data_item* out_data_item)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_data_item_get(in_handle, out_data_item);
}

extern "C" OPENSUT_API
opensut_result_t opensut_data_item_get_type(opensut_data_item in_data_item, const opensut_type_t** out_type_ptr)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_data_item_get_type(in_data_item, out_type_ptr);
}

extern "C" OPENSUT_API
opensut_result_t opensut_data_item_set_data_update_handler(opensut_data_item in_data_item,
  opensut_data_update_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_data_item_set_data_update_handler(in_data_item, in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_data_item_read(opensut_data_item in_data_item, void* out_buffer, size_t buffer_bytes)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_data_item_read(in_data_item, out_buffer, buffer_bytes);
}

extern "C" OPENSUT_API
opensut_result_t opensut_data_item_write(opensut_data_item in_data_item, const void* in_buffer, size_t buffer_bytes)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_data_item_write(in_data_item, in_buffer, buffer_bytes);
}

extern "C" OPENSUT_API
opensut_result_t opensut_eth_receive(const opensut_eth_message_t* in_message)
{
  return NCANoeEmu::OpenSUT::Eth::opensut_eth_receive(in_message);
}

extern "C" OPENSUT_API
opensut_result_t opensut_eth_acknowledge(size_t in_send_job_id)
{
  return NCANoeEmu::OpenSUT::Eth::opensut_eth_acknowledge(in_send_job_id);
}

extern "C" OPENSUT_API
opensut_result_t opensut_eth_set_send_handler(opensut_eth_send_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::Eth::opensut_eth_set_send_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_receive(const opensut_lin_message_t * in_message, opensut_lin_frame_status_t frame_status)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_receive(in_message, frame_status);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_set_send_handler(opensut_lin_send_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_set_send_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_wakeup(size_t channel)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_wakeup(channel);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_go_to_sleep(size_t channel)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_go_to_sleep(channel);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_set_go_to_sleep_handler(opensut_lin_go_to_sleep_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_set_go_to_sleep_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_set_wakeup_handler(opensut_lin_wakeup_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_set_wakeup_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_set_go_to_sleep_internal_handler(opensut_lin_go_to_sleep_internal_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_set_go_to_sleep_internal_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_lin_set_wakeup_internal_handler(opensut_lin_wakeup_internal_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::LIN::opensut_lin_set_wakeup_internal_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_receive(const opensut_fr_message_t* in_message)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_receive(in_message);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_start_cycle(size_t in_channel_id, uint8_t in_cycle, const opensut_byte_vector* in_nm_vector)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_start_cycle(in_channel_id, in_cycle, in_nm_vector);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_set_poc_state(size_t in_channel_id, opensut_fr_poc_state_t in_poc_state)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_set_poc_state(in_channel_id, in_poc_state);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_set_frame_registration_handler(
  opensut_fr_frame_registration_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_set_frame_registration_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_set_update_buffer_handler(opensut_fr_update_buffer_handler in_handler,
  void* in_context)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_set_update_buffer_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_set_reset_cc_handler(opensut_fr_reset_cc_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_set_reset_cc_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_fr_set_wakeup_handler(opensut_fr_wakeup_handler in_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::FlexRay::opensut_fr_set_wakeup_handler(in_handler, in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_get(opensut_handle_t in_handle,
  opensut_rpc_object* out_rpc_object)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_get(in_handle, out_rpc_object);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_arg_get_type(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_arg_direction_t in_arg_direction, const opensut_type_t** out_type)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_arg_get_type(in_rpc_object,
    in_arg_direction, out_type);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_client_register_observer(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_call_handler in_call_observer, void* in_call_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_client_register_observer(in_rpc_object,
    in_call_observer, in_call_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_server_begin_call(opensut_rpc_object in_rpc_object,
  opensut_rpc_object_call_context* out_call_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_server_begin_call(in_rpc_object, out_call_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_server_invoke_call(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_call_handler in_call_handler, void* in_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_server_invoke_call(in_call_context, in_call_handler,
    in_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_call_context_arg_read(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_arg_direction_t in_arg_direction, void* out_buffer, size_t buffer_bytes)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_call_context_arg_read(in_call_context, in_arg_direction, out_buffer,
    buffer_bytes);
}

extern "C" OPENSUT_API
opensut_result_t opensut_call_context_arg_write(opensut_rpc_object_call_context in_call_context,
  opensut_rpc_object_arg_direction_t in_arg_direction, const void* in_buffer, size_t buffer_bytes)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_call_context_arg_write(in_call_context, in_arg_direction, in_buffer,
    buffer_bytes);
}

extern "C" OPENSUT_API
opensut_result_t opensut_rpc_object_client_return(opensut_rpc_object_call_context* in_out_call_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_rpc_object_client_return(in_out_call_context);
}

extern "C" OPENSUT_API
opensut_result_t opensut_call_context_release(opensut_rpc_object_call_context* in_out_call_context)
{
  return NCANoeEmu::OpenSUT::ComItem::opensut_call_context_release(in_out_call_context);
}

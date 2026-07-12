/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file typedefs.h
 * Contains common basic definitions. It is usually not necessary to include this file directly.
 */

#pragma once

#pragma pack(push)
#pragma pack(8)

/*! \brief OpenSUT version: MAJOR * 100000 + MINOR * 100 + PATCH.
 *  OpenSUT complies with semantic versioning 2.0.0 (https://semver.org/lang/de/).
 */
#define OPENSUT_API_VERSION 300000
#define GET_OPENSUT_API_VERSION_MAJOR(version) ((version) / 100000)
#define GET_OPENSUT_API_VERSION_MINOR(version) (((version) % 100000) / 100)
#define GET_OPENSUT_API_VERSION_PATCH(version) ((version) % 100)

#include <stddef.h>
#include <stdint.h>

//! The macro OPENSUT_API specifies function attributes used to import or export symbols from a shared library.
#ifndef OPENSUT_API
#define OPENSUT_API
#endif

//! Helper macro for OpenSUT function prototype declarations
#define OPENSUT_FUNCTION(returnType) OPENSUT_API returnType

/*! Get the OpenSUT API version as combined single integer.
 *  @return OpenSUT API version = MAJOR * 100000 + MINOR * 100 + PATCH
 */
OPENSUT_FUNCTION(uint32_t) opensut_api_get_version();

//! absolute time in nanoseconds [ns]
typedef uint64_t opensut_time_t;
//! time difference in nanoseconds [ns]
typedef int64_t opensut_time_duration_t;
//! generic handle type
typedef uint64_t opensut_handle_t;

//! Special time constants
#define OPENSUT_TIME_INFINITE ((opensut_time_duration_t)0x7fffffffffffffffLL) //!< Represents an infinite time interval

//! Possible results of OpenSUT functions
typedef enum
{
  OPENSUT_OK                        = 0, //!< Success
  OPENSUT_FAILED                    = 1, //!< Failure. See documentation of particular function for possible reasons
  OPENSUT_PARAMETER_INVALID         = 2, //!< An invalid parameter was passed
  OPENSUT_NOTINITIALIZED            = 3, //!< SUT is uninitialized
  OPENSUT_TIMEOUT                   = 4, //!< The function call returned due to a timeout condition
  OPENSUT_CALL_CONTEXT_ARGS_NOT_SET = 5  //!< The corresponding arguments are not yet set
} opensut_result_t;

//! Initialization options
typedef enum
{
  OPENSUT_INIT_DEFAULT      = 0, //!< Original default initialization of OpenSUT.
  OPENSUT_INIT_CONTROLLER_2 = 1  //!< Use new controller interface which is recommended from now on. This interface
                                 // provides two data items PowerRequest and PowerState instead of the PowerSupply
                                 // data item to set and retrieve the power state of the ECU. In addition, two RPC
                                 // objects SwitchOn and SwitchOff are provided to switch the ECU on and off, which may
                                 // be used as alternatives. This option also leads to different communication item
                                 // names, i.e. different namespaces and different delimiters.
} opensut_init_flags;

//! Networking categories, see also \ref bus.h
typedef enum
{
  OPENSUT_BUS_TAG_CAN      = 0, //!< Controller Area Network (CAN)
  OPENSUT_BUS_TAG_ETHERNET = 1, //!< Ethernet (Eth)
  OPENSUT_BUS_TAG_LIN      = 2, //!< Local Interconnect Network (LIN)
  OPENSUT_BUS_TAG_FLEXRAY  = 3  //!< FlexRay
} opensut_bus_tag_t;

//! Specifies flags for CAN and CAN-FD messages sent/received using \ref opensut_can_frame_t
typedef enum
{
  OPENSUT_CAN_FLAGS_RTR = 0x10,   //!< Remote transmission request.
                                  // If set, then 4 bit dlc are set, but there is no payload
  OPENSUT_CAN_FLAGS_IDE = 0x0200, //!< Identifier Extension, SRR (substitute remote request)
                                  // bit for ExtId Can messages, SRR is sent dominant if set
                                  // If bit is zero, them CAN message uses 11-Bit standard identifiers
                                  // If bit is one, then CAN message uses 29-Bit extended identifiers
  OPENSUT_CAN_FLAGS_FDF = 0x1000, //!< FDF, FD Format Indicator bit for CAN FD, (0 CAN, 1 CAN-FD)
  OPENSUT_CAN_FLAGS_BRS = 0x2000, //!< BRS (bit rate switch) bit for CAN FD
                                  // If bit is zero, then use arbitration bit rate for data segment
                                  // If bit is one, then use data bit rate for data segment
} opensut_can_flags_t;

//! Special CAN data length code
typedef enum
{
  OPENSUT_CAN_DLC_AUTO = 0xff //!< Automated DLC computation
} opensut_can_dlc_t;

//! Specifies a CAN frame.
typedef struct
{
  uint32_t id;               //!< CAN identifier
  opensut_can_flags_t flags; //!< frame flags
  uint8_t dlc;               //!< data length code, use OPENSUT_CAN_DLC_AUTO when dlc should be computed from
                             // data_length and the FDF protocol bit.
  uint32_t data_length;      //!< payload byte count
  uint8_t data[64];          //!< data field
} opensut_can_frame_t;

typedef struct
{
  const uint8_t* data;
  size_t size;
} opensut_byte_vector;

#if defined(_WIN32) || defined(_WINDOWS)
//! Type alias for a Windows HANDLE for synchronization API usage, e.g. WaitForMultipleObjects
typedef void* portable_handle_t;
#elif defined(__linux__) || defined(_LINUX)
//! Type alias for a Linux file descriptor, to be used with epoll_wait
typedef int portable_handle_t;
#endif

#pragma pack(pop)

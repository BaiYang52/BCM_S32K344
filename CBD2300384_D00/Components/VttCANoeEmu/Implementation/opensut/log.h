/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file log.h
 * Contains declarations related to logging of the SUT.
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

//! Log levels, default is INFO under Windows and OFF under Linux
typedef enum
{
  OPENSUT_LOG_INFO  = 0, //!< Informational content including DET errors
  OPENSUT_LOG_WARN  = 1, //!< Warnings
  OPENSUT_LOG_ERROR = 2, //!< Errors
  OPENSUT_LOG_OFF   = 3, //!< Logging is disabled
} opensut_log_level_t;

/*! Sets the log level for the SUT. Can be invoked at any time. DET errors are only logged with
    log level OPENSUT_LOG_INFO.
    Note that on Linux the logging may cause a crash if dlopen is used with RTLD_DEEPBIND. A workaround
    for this crash may be to compile the program that loads the SUT with position-independent code (-fPIC)
    or to link statically against the standard library libstdc++ (-static-libstdc++).
 *  @param log_level the log level that shall be applied
 *  @return
 *  - OPENSUT_OK if the log level was set successfully
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_set_log_level(opensut_log_level_t log_level);

#pragma pack(pop)

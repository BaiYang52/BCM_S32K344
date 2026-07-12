/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file bus.h
 * Contains generic networking-related declarations.
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

/*! Describes a SUT network connector.
 */
typedef struct
{
  size_t channel_id;         /*!< unique index amongst all connectors with the the same bus_tag. */
  const char* name;          /*!< network name */
  opensut_bus_tag_t bus_tag; /*!< network category */
} opensut_bus_cfg_t;

/*! Get the count of bus connectors that the SUT provides.
 *  @param out_count out parameter to receive the count
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID out_count is a null pointer
 *  - OPENSUT_NOTINITIALIZED SUT is not initialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_bus_get_config_count(size_t* out_count);

/*! Retrieves a particular connector description.
 *  @param in_config_idx zero-based index of desired configuration descriptor
 *  @param out_config receives the requested configuration descriptor
 *  @return
 *  - OPENSUT_OK descriptor was copied successfully to \p out_config
 *  - OPENSUT_PARAMETER_INVALID config_index out of range or out_config is null pointer
 *  - OPENSUT_NOTINITIALIZED SUT is not initialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_bus_get_config(size_t in_config_idx, opensut_bus_cfg_t* out_config);

#pragma pack(pop)

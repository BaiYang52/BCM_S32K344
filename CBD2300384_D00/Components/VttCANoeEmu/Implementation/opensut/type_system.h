/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file type_system.h
 * Contains definitions for describing primitive and composed data types.
 */

#pragma once

#include <stdint.h>

#pragma pack(push)
#pragma pack(8)

//! Primitive data type enumeration
typedef enum
{
  // 8-bit data types
  OPENSUT_PRIMITIVE_TYPE_BOOL  = 0, //!< 8 bit bool
  OPENSUT_PRIMITIVE_TYPE_SINT8 = 1, //!< 8 bit signed integer
  OPENSUT_PRIMITIVE_TYPE_UINT8 = 2, //!< 8 bit unsigned integer

  // 16-bit data types
  OPENSUT_PRIMITIVE_TYPE_SINT16 = 3, //!< 16 bit signed integer
  OPENSUT_PRIMITIVE_TYPE_UINT16 = 4, //!< 16 bit unsigned integer

  // 32-bit data types
  OPENSUT_PRIMITIVE_TYPE_SINT32  = 5, //!< 32 bit signed integer
  OPENSUT_PRIMITIVE_TYPE_UINT32  = 6, //!< 32 bit unsigned integer
  OPENSUT_PRIMITIVE_TYPE_FLOAT32 = 7, //!< 32 bit float

  // 64-bit data types
  OPENSUT_PRIMITIVE_TYPE_SINT64  = 8, //!< 64 bit signed integer
  OPENSUT_PRIMITIVE_TYPE_UINT64  = 9, //!< 64 bit unsigned integer
  OPENSUT_PRIMITIVE_TYPE_FLOAT64 = 10 //!< 64 bit float

  // further primitive data types
} opensut_primitive_type_tag_t;

//! Enumerates type categories.
typedef enum
{
  OPENSUT_TYPE_CATEGORY_PRIMITIVE = 0, //!< Primitive type, such as int32
  OPENSUT_TYPE_CATEGORY_STRUCT    = 1, //!< A structure (aka record)
  OPENSUT_TYPE_CATEGORY_ARRAY     = 2, //!< fixed-size array
} opensut_type_category_t;

// forward declaration
struct opensut_type_t;

//! Describes a struct member.
typedef struct
{
  const char* name;                  //!< member name
  const opensut_type_t* member_type; //!< member type
} opensut_struct_member_t;

/*! Describes a struct.
 *  Struct members are serialized member after member, without any padding (as if you would declare the corresponding
 *  C struct with preceding \verbatim #pragma pack(1) \endverbatim ). The reason is that we need a stable "canonical"
 *  serialization scheme, which does not depend on any platform/compiler settings.
 */
typedef struct
{
  const opensut_struct_member_t* members; //!< an array of struct members
  size_t member_count;                    //!< member count
} opensut_struct_type_t;

//! Describes an array.
typedef struct
{
  const opensut_type_t* type_ptr; //!< element type
  size_t array_length;            //!< number of array elements
} opensut_array_type_t;

/*! Union of possible type definitions.
 * The discriminator is given by \p opensut_type_ptr_t::category.
 */
typedef union
{
  opensut_primitive_type_tag_t primitive_tag;     //!< Primitive type tag
  const opensut_struct_type_t* struct_definition; //!< Points to a struct definition
  const opensut_array_type_t* array_definition;   //!< Points to an array definition
} opensut_type_union_t;

//! Describes a data type.
typedef struct opensut_type_t
{
  const char* name;                 //!< the name of the data type (optional)
  size_t byte_size;                 //!< the 1-byte-packed byte size of the type
  opensut_type_category_t category; //!< tells, which of definition's members is value
  opensut_type_union_t definition;  //!< the actual type definition
} opensut_type_t;

#pragma pack(pop)

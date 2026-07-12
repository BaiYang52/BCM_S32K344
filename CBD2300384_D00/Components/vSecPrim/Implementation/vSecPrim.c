 /**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  vSecPrim.c
 *        \brief  Provides static informations. No functionality provided.
 *
 *      \details  Main file of vSecPrim providing Misra and SBSW justifications.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/
#define VSECPRIM_SOURCE
#include "vSecPrim.h"
#include "vSecPrim_Cfg.h"
#include "actConfig.h"
#include "actPlatformTypes.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of vSecPrim header file */
#if ((VSECPRIM_SW_MAJOR_VERSION != (7u)) \
    || (VSECPRIM_SW_MINOR_VERSION != (0u)) \
    || (VSECPRIM_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of vSecPrim.c and vSecPrim.h are inconsistent"
#endif


/**********************************************************************************************************************
 *  Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations:

  MD_VSECPRIM_STPAR:
    Reason:     Artificially restricting the number of parameters to a function would lead to less readable code.
                The function parameters are chosen in accordance with specifications and are complemented by
                other necessary parameters, such as length indications for buffers. 
    Risk:       Usage of function is not easily comprehensible. 
    Prevention: Code review. 

  MD_VSECPRIM_8.7:
    Reason:     Following this MISRA rule would lead to unreadable code, as the constant fields are huge.
                Vector style guide prevents usage of static variables/constant objects in function scope.
    Risk:       None
    Prevention: Not necessary

  MD_VSECPRIM_5.6:
    Reason:     Naming of the buffer and struct members are similar to provided parameters to make the code more
                readable.
    Risk:       One could read wrong and adapt the code falsely.
    Prevention: Covered by code review and tests.

  MD_VSECPRIM_11.4:
    Reason:     Casting between different pointer is necessary due to the nature of this library.
    Risk:       None - the basic types remain the same.
    Prevention: Covered by code review and tests.

  MD_VSECPRIM_13.1:
    Reason:     An assignment operator is being used in a boolean expression this decreases the
                readability of the code.
    Risk:       Code could be mistaken.
    Prevention: Additional comment is stated.

  MD_VSECPRIM_21.1:
    Reason:     There could be no wraparound because of the previous check.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_16.7:
    Reason:     Pointer can only be const when method is empty and only Dummy statements are used.
    Risk:       None
    Prevention: Covered by code review.

  MD_VSECPRIM_14.6:
    Reason:     Two break statements are implemented to provide simple yet efficient code.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_19.1:
    Reason:     The included file contains wrapper functions that can be used in different contexts. Before including
                it, it is necessary to define the data type the wrappers shall be defined for.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_WATCHDOG:
    Reason:     Calling the watchdog trigger function depends basically on two factors:
                  - If the watchdog trigger function is a valid pointer
                  - Watchdog trigger level
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_P2VOID_CAST: MISRA Dir 1.1
    Reason:     The implicit conversion from a pointer to object type to a pointer to void is used calling functions
                that allow pointers to different object types as parameter.
    Risk:       There is an invalid conversion within the function from a pointer to void to a pointer to object type
                and thus invalid data are read.
    Prevention: Code inspection.

  MD_VSECPRIM_11.4_P2UINT8_CAST: MISRA Rule 11.4
    Reason:     The conversion from a pointer to void to a pointer to object type is used to access the object's memory bytewise.
                The size of the object's memory is known.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_REDUNDANT_OP:
    Reason:     The redundant operation is a side effect of a better readability of the code.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_CONFIGURATION_DEPENDENT_REDUNDANT_OP:
    Reason:     The redundant operation is a side effect of the configurability of the code and required to avoid
                compile errors in certain configurations.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_ASSIGNMENT_RESULT_IN_OP: Misra Rule 13.4
    Reason:     The result of an assignment is being used in an arithmetic operation or another assigning operation in
                order to keep the processing flow of the algorithm's complex arithmetic operations as they are 
                described in the algorithm's specification.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_USE_OF_BASIC_TYPES: Misra Dir 4.6
    Reason:     Only non safe files are affected.
    Risk:       None
    Prevention: Review.

  MD_VSECPRIM_CONST_BOOL_OPERAND: Misra Rule 14.3
    Reason:     Boolean operand might be constant in definition depend on configuration.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_REDUNDANT_ASSIGNMENT: Misra Rule 2.2
    Reason:     Redundant assignment in last cycle of for loop.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_ALGORITHM_REQUIRED_ASSIGNMENT
    Reason:     Redundant assignment due to algorithm specification
    Risk:       None
    Prevention: N/A


  MD_VSECPRIM_2.2
    Reason:     Redundant assignment to avoid compiler warning of use of potentially uninitialized variable.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_CONST_POINTER: Misra Rule 8.13
    Reason:     The function parameter is a pointer to a pointer. The object referenced by the inner pointer 
                (and therefore also by the pointer to the pointer) is modified or the outer pointer is passed to another function, 
                but this is not detected by QAC. Thus, the parameter can not be passed as pointer-to-const.
    Risk:       The use of a pointer referencing another pointer makes the code more complicated to understand.
    Prevention: Code review.

  MD_VSECPRIM_8.9
    Reason:     An array lies in static ROM, but is only accessed in one function.
    Risk:       Additional Rom usage.
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_METRIC
    Reason:     A function in the QM part of the component violates a metric threshold. The function is
                proven in use.
    Risk:       Code is difficult to comprehend
    Prevention: Code review.

  MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION
    Reason:     A function in the QM part of the component is not used in some configurations. This affects
                placeholder functions that are used when a certain algorithm is turned off. The function is
                proven in use.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_RETURN_PATHS
    Reason:     A function in the QM part of the component has more than one return path. The function is
                proven in use.
    Risk:       Code is difficult to comprehend
    Prevention: Code review.

  MD_VSECPRIM_QM_ONLY_REDUNDANT_ASSIGNMENT_OR_OPERATION
    Reason:     A redundant assignment or operation is made in the QM part of the component. The function is
                proven in use.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_CASTING_POINTER_TYPE
    Reason:     A pointer is cast to a pointer referencing an object of different type in the QM part 
                of the component. The function is proven in use.
    Risk:       None
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_POINTER_ARITHMETIC
    Reason:     There is an arithmetic operation on a pointer in the QM part of the component. The function is
                proven in use.
    Risk:       Code is difficult to comprehend.
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_VARIABLE_IN_LOOP
    Reason:     In a for-loop, two variables instead of one are compared to a stopping criterion and are incremented
                in the QM part of the component. The function is proven in use.
    Risk:       Code is difficult to comprehend.
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_SHORT_DECREMENT_INCREMENT_OPERATION
    Reason:     A decrement or increment operation is used in combination with another operation in the QM part of the component.
                The function is proven in use.
    Risk:       Code is difficult to comprehend.
    Prevention: N/A

  MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE
    Reason:     A pointer is cast to a type with a stricter alignment requirement in the QM part of the component.
                The function is proven in use.
    Risk:       None.
    Prevention: N/A

  MD_VSECPRIM_WRAPAROUND
    Reason:     There is a wraparound in an unsigned arithmetic operation. This is intended due to the specification of the algorithm.
    Risk:       None
    Prevention: Code review.


 */

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK
    \DESCRIPTION This function has writing operation on an array without any boundary checks.
    \COUNTERMEASURE \N The specific function must not be called by the user but only by the vSecPrim module itself.
                       The calling operations where a possible boundary violation could occur have been validated during the code review.

  \ID SBSW_VSECPRIM_WRITE_ARRAY
    \DESCRIPTION This function has writing operation on an array with boundary check.
    \COUNTERMEASURE \N The specific function must not be called by the user but only by the vSecPrim module itself.
                       Possible boundary violations are evaluated during the code review.

  \ID SBSW_VSECPRIM_WRITE_POINTER
    \DESCRIPTION    The function does a write access on an pointer.
    \COUNTERMEASURE \R The validity of the pointer was checked before.

  \ID SBSW_VSECPRIM_CALL_FUNCTION
    \DESCRIPTION    The function passes a pointer to another function.
    \COUNTERMEASURE \R The validity of the pointer was checked before either by the calling function or a function in the upper layer.

  \ID SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG
    \DESCRIPTION    The function passes a function pointer referencing a watchdog trigger fucntion to another function.
                    That passed function pointer in turn has been passed by the caller. The API requires that the referenced fucntion is 
                    a watchdog trigger fucntion.
    \COUNTERMEASURE \N The caller ensures that the function pointer passed as parameter is valid and references a watchdog trigger function.

  \ID SBSW_VSECPRIM_FUNCTION_POINTER_CALL_WATCHDOG
    \DESCRIPTION    A function pointer to a watchdog trigger function to be called inside the called function is
                    provided by the caller. The API requires that watchdog is a pointer to a watchdog trigger 
                    function.
    \COUNTERMEASURE \N The specific function must not be called by the user but only by the vSecPrim module itself.
                       The validity of the function pointer is guaranted by the caller of this function.

  \ID SBSW_VSECPRIM_FUNCTION_POINTER_CALL_TRANSFORM
    \DESCRIPTION    A function pointer to a transformation function to be called inside the called function is
                    provided by the caller. The API requires that transfom is a pointer to a  transformation
                    function.
    \COUNTERMEASURE \N The specific function must not be called by the user but only by the vSecPrim module itself.
                       The validity of the function pointer is guaranted by the caller of this function.

  \ID SBSW_VSECPRIM_FUNCTION_CALL_POINTER_CHECKED_WITHIN
    \DESCRIPTION    Pointer(s) are passed to the function without previous validity checks.
    \COUNTERMEASURE \N The function pointer is checked for validity within this function.

  \ID SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER
    \DESCRIPTION    The function uses the pointer to write to the referenced object. The pointer is passed as
                    parameter. The API requires that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX
    \DESCRIPTION    The function writes array elememts using fixed indices. The array is defined locally and of known
                    size.
    \COUNTERMEASURE \N [CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_REVIEW]

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX
    \DESCRIPTION    The function writes array elememts using indices that are masked resp. size-restricted by a modulo
                    operation before being applied. The array is defined locally and of known size.
    \COUNTERMEASURE \R [CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_RUNTIME_MEASURE]

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_VARIABLE_INDEX_CHECK
    \DESCRIPTION    The function writes array elememts using indices that are checked against the array size before 
                    being applied. The array is defined locally and of known size.
    \COUNTERMEASURE \N [CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_REVIEW]

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX
    \DESCRIPTION    The function writes array elememts using fixed indices. A pointer to the array is passed as
                    parameter. The API requires that the referenced array is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_REVIEW]

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK
    \DESCRIPTION    The function writes array elememts using variable indices that are checked against the array size
                    before being applied. A pointer to the array is passed as parameter. The API requires that the
                    referenced array is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_REVIEW]

  \ID SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER
    \DESCRIPTION    The function passes a pointer referencing a locally defined buffer of known size to another function.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLEE_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE
    \DESCRIPTION    The function passes a pointer referencing a locally defined buffer of known size and its size to
                    another function. The passed buffer size is fixed.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer in
                    turn has been passed by the caller. The API requires that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer in
                    turn has been passed by the caller. The API requires that the referenced object is of valid size.
                    The function passes a pointer referencing a locally defined buffer of known size and its size to
                    another function.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_P2CONSTPARAM_AND_LOCAL_BUFFER
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function, where this parameter is 
                    qualified as const. The passed pointer in turn has been passed by the caller. The API requires 
                    that the referenced object is of valid size. The function passes a pointer referencing a locally 
                    defined buffer of known size to the same function.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_P2CONSTPARAM
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer in
                    turn has been passed by the caller. The API requires that the referenced object is of valid size. 
                    The function passes another pointer referencing a buffer to the same function. 
                    The parameter for passing the pointer is qualified as "pointer to const".
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE
    \DESCRIPTION    The function passes a pointer referencing a buffer and its size to another function. The passed
                    buffer size is fixed. That passed pointer in turn has been passed by the caller. The API requires 
                    that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE
    \DESCRIPTION    The function passes a pointer referencing a buffer and its size to another function. The passed
                    buffer size is variable. That passed pointer in turn has been passed by the caller. The API requires 
                    that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE]
                       and:
                       The buffer to be written is at least of the size of one block,
                       The size of the written bytes is always equal or smaller than the block size, see [CM_VSECPRIM_BUFFER_USED_SMALLER_BLOCK_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE_CHECK
    \DESCRIPTION    The function passes a pointer referencing a buffer and its size to another function. The passed
                    buffer size is variable and it is checked that the size does not exceed the size of the passed buffer.
                    That passed pointer in  turn has been passed by the caller. The API requires 
                    that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer is a
                    fixed sub-address within the buffer passed by the caller. The API requires that the referenced object is
                    of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_VALID_SUBADDRESS]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS_FIXED_SIZE
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer is a
                    fixed sub-address within the buffer passed by the caller. The passed buffer size is fixed. The API
                    requires that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_VALID_SUBADDRESS]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer is a
                    variable sub-address within the buffer passed by the caller. The API requires that the referenced object is
                    of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_VALID_SUBADDRESS]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE
    \DESCRIPTION    The function passes a pointer referencing a buffer to another function. That passed pointer is a
                    variable sub-address within the buffer passed by the caller. The passed buffer size is variable.
                    The API requires that the referenced object is of valid size.
    \COUNTERMEASURE \N [CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE] and [CM_VSECPRIM_CALLEE_ENSURES_VALID_SUBADDRESS]
                    and:
                    info->buffer is the size of one block  (e.g. actSIPHASH_BLOCK_SIZE, actGHASH_BLOCK_SIZE, actGCM_BLOCK_SIZE)
                    &info->buffer[info->buffer_used] is within info->buffer since info->buffer_used is always equal or smaller than the block size, see [CM_VSECPRIM_BUFFER_USED_SMALLER_BLOCK_SIZE]
                    the size of the buffer passed by &info->buffer[info->buffer_used] is (block size - info->buffer_used),
                    the size to be written (e.g. diff) is equal or smaller than (block size - info->buffer_used), see [CM_VSECPRIM_BUFFER_USED_SMALLER_BLOCK_SIZE]

  \ID SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM
    \DESCRIPTION    A pointer referencing a buffer is passed to another function. The corresponding parameter of the called function is qualified as "pointer to const".
    \COUNTERMEASURE \N Pointers qualified as "pointer to const" cannot be used for write access.

  \ID SBSW_VSECPRIM_FUNCTION_CALL_WITH_FUNCTION_POINTER
    \DESCRIPTION    An array of function pointers is accessed via a fixed index.
    \COUNTERMEASURE \N Code review verifies that only valied indices are used.

  \ID SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX_SIPHASH_FINALIZE
    \DESCRIPTION    The function esl_finalizeSipHash writes to the obejct referenced by parameter 'messageMAC'.
                    It writes array elememts using fixed indices.
    \COUNTERMEASURE \S The caller of function esl_finalizeSipHash must ensure that the pointer passed to the parameter 'messageMAC'
                       references a valid memory location and that the size of the array referenced by the parameter 'messageMAC'
                       is greater or equal to 8 bytes. [SMI-160485]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_UPDATE
    \DESCRIPTION    The functions esl_updatePlaintextEncryptGCM and esl_updateCiphertextDecryptGCM write to the object referenced by parameter 'output'.
                    They pass the pointer given by 'output' to another function.
    \COUNTERMEASURE \S The caller of functions esl_updatePlaintextEncryptGCM and esl_updateCiphertextDecryptGCM must ensure that the pointer passed to 
                       the parameter 'output' references a valid memory location and that the size of the array referenced by the 
                       parameter 'output' is a multiple of the GCM block size (16 bytes) and greater than or equal to the size given by parameter 'inputLength'. [SMI-160486]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GCM_ENCRYPT_DECRYPT_FINALIZE
    \DESCRIPTION    The functions esl_finalizeEncryptGCM and esl_finalizeDecryptGCM write to the object referenced by parameter 'output'.
                    They pass the pointer given by 'output' to another function.
    \COUNTERMEASURE \S The caller of functions esl_finalizeEncryptGCM and esl_finalizeDecryptGCM must ensure that the pointer passed to 
                       the parameter 'output' references a valid memory location and that the size of the array referenced by the 
                       parameter 'output' is greater or equal than the GCM block size (16 bytes). [SMI-160487]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_CMACAES128_FINALIZE
    \DESCRIPTION    The function esl_finalizeCMACAES128 writes to the object referenced by parameter 'messageCMAC'.
                    It passes the pointer given by 'messageCMAC' to another function.
    \COUNTERMEASURE \S The caller of functions esl_finalizeCMACAES128 must ensure that the pointer passed to 
                       the parameter 'messageCMAC' references a valid memory location and that the size of the array referenced by the 
                       parameter 'messageCMAC' is greater than or equal to the AES block size (16 bytes). [SMI-163627]

    \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_HMACSHA256_FINALIZE
    \DESCRIPTION    The function esl_finalizeHashMACSHA256 writes to the object referenced by parameter 'messageHashMAC'.
                    It passes the pointer given by 'messageHashMAC' to another function.
    \COUNTERMEASURE \S The caller of functions esl_finalizeHashMACSHA256 must ensure that the pointer passed to 
                       the parameter 'messageHashMAC' references a valid memory location and that the size of the array referenced by the 
                       parameter 'messageHashMAC' is greater than or equal to the SHA2-256 digest size (32 bytes). [SMI-552783]

  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_SHA_FINALIZE
    \DESCRIPTION    The functions 
                      esl_finalizeSHA1
                      esl_finalizeSHA224
                      esl_finalizeSHA256
                      esl_finalizeSHA384
                      esl_finalizeSHA512
                      esl_finalizeSHA512_224
                      esl_finalizeSHA512_256
                    write to the object referenced by parameter 'messageDigest'.
                    They pass the pointer given by 'messageDigest' to another function.
    \COUNTERMEASURE \S The caller of functions 
                        esl_finalizeSHA1
                        esl_finalizeSHA224
                        esl_finalizeSHA256
                        esl_finalizeSHA384
                        esl_finalizeSHA512
                        esl_finalizeSHA512_224
                        esl_finalizeSHA512_256
                       must ensure that the pointer passed to the parameter 'messageDigest' references a valid memory
                       location and that the size of the array referenced by the parameter 'messageDigest' is greater 
                       than or equal to 20 (28/32/48/28/32/64) bytes for esl_finaleSHA1 (esl_finalizeSHA224/esl_finalizeSHA256/
                       esl_finalizeSHA384/esl_finalizeSHA512/esl_finalizeSHA512_224/esl_finalizeSHA512_256). [SMI-163628]


  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_FINALIZE_HASH
    \DESCRIPTION    The functions
                     esl_finalizeHashSignRSASHA256_PSS
                     esl_finalizeHashVerifyRSASHA256_PSS
                       write to the object referenced by parameter 'messageDigest'.
                       They pass the pointer given by 'messageDigest' to another function.
    \COUNTERMEASURE \S The caller of functions
                        esl_finalizeHashSignRSASHA256_PSS
                        esl_finalizeHashVerifyRSASHA256_PSS
                       must ensure that the pointer passed to the parameter 'messageDigest' references a valid memory
                       location and that the size of the array referenced by the parameter 'messageDigest' is greater
                       than or equal to 32 bytes. [SMI-423463]

                     
  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_SALTEDHASH
    \DESCRIPTION    The function
                     esl_calcSaltedHashRSASHA256_PSS
                    writes to the object referenced by parameter 'saltedHash'.
                    It passes the pointer given by 'saltedHash' to another function.
    \COUNTERMEASURE \S The caller of function
                        esl_calcSaltedHashRSASHA256_PSS
                       must ensure that the pointer passed to the parameter 'saltedHash' references a valid memory
                       location and that the size of the array referenced by the parameter 'saltedHash' is greater
                       than or equal to 32 bytes. [SMI-423463]                  


  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_RSAPSSSHA256_MASK
    \DESCRIPTION    The function
                     esl_generateMaskMGF1RSASHA256_PSS
                    writes to the object referenced by parameter 'tempHash'.
                    It passes the pointer given by 'tempHash' to another function.
    \COUNTERMEASURE \S The caller of function
                        esl_generateMaskMGF1RSASHA256_PSS
                       must ensure that the pointer passed to the parameter 'tempHash' references a valid memory
                       location and that the size of the array referenced by the parameter 'tempHash' is greater
                       than or equal to 32 bytes. [SMI-423463]       


  \ID SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_GENERATEKEY_ECP
    \DESCRIPTION    The function
                     esl_generateKeyEcP_prim
                    writes to the objects referenced by the parameters 'privateKey', 'publicKey_x' and 'publicKey_y'.
                    It passes the pointers given by 'privateKey', 'publicKey_x' and 'publicKey_y' to another function.
    \COUNTERMEASURE \S The user of MICROSAR Safe shall verify that the pointers privateKey, publicKey_x and publicKey_y
                       passed to the function
                        esl_generateKeyEcP_prim
                       reference valid memory locations and that the sizes of the arrays referenced by the parameters 
                       privateKey, publicKey_x and publicKey_y are greater than or equal to the return values of
                        esl_getLengthOfEcPprivateKey(domain) for privateKey and 
                        esl_getLengthOfEcPpublicKey_comp(domain) for publicKey_x and publicKey_y
                       in bytes, where domain is the ASN1 structure describing the  domain parameters of the underlying curve. 
                       The maximally required buffer size is 32 byte, if ECP_MAX_KEY_SIZE = ECP_MAX_KEY_SIZE_256, and 48 byte, 
                       if ECP_MAX_KEY_SIZE = ECP_MAX_KEY_SIZE_384. [SMI-455434]


SBSW_JUSTIFICATION_END */

/*

  \CM CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_REVIEW The write operations where a possible boundary violation could occur have been validated during the code review.

  \CM CM_VSECPRIM_EXCLUDE_BOUNDARY_VIOLATION_BY_RUNTIME_MEASURE Write operations where a possible boundary violation could occur are prevented by run-time measure.

  \CM CM_VSECPRIM_CALLER_ENSURES_BUFFER_SIZE The caller ensures that the pointer passed as parameter is valid and references an object of valid size.

  \CM CM_VSECPRIM_CALLEE_ENSURES_BUFFER_SIZE The callee ensures that the passed buffer is of valid size, i.e. that the size is as required by the called function.

  \CM CM_VSECPRIM_CALLEE_ENSURES_PASSED_BUFFER_SIZE The callee ensures that the passed buffer size does not exceed the size of the passed buffer.

  \CM CM_VSECPRIM_CALLEE_ENSURES_VALID_SUBADDRESS The callee ensures that the passed buffer referenced by the sub-address is of valid size, i.e. that it is fully within the buffer passed by the caller.

  \CM CM_VSECPRIM_BUFFER_USED_SMALLER_BLOCK_SIZE  Evaulation of all write accesses to info->buffer_used, proofing that info->buffer_used < BLOCK_SIZE
                                                  1.) In init and finalize functions:
                                                      info->buffer_used = 0u;
                                                  2.) In update functions:
                                                      I) diff = minimum(length, BLOCK_SIZE - info->buffer_used)
                                                          info->buffer_used += diff
                                                          --> info->buffer_used <= BLOCK_SIZE
                                                          A) If info->buffer_used < BLOCK_SIZE:
                                                            Update is finished 
                                                            --> info->buffer_used < BLOCK_SIZE
                                                          B) If info->buffer_used == BLOCK_SIZE:
                                                            blocks = (length - diff) / BLOCK_SIZE
                                                            diff is as calculated previously
                                                            info->buffer_used = (length - (diff + (BLOCK_SIZE * blocks)))
                                                            info->buffer_used = length - (diff + (BLOCK_SIZE * blocks))
                                                                              = (length - diff) - ((length - diff) / BLOCK_SIZE ) * BLOCK_SIZE
                                                                              = (length - diff) % BLOCK_SIZE
                                                            --> info->buffer_used < BLOCK_SIZE

 */


/* COV_JUSTIFICATION_BEGIN

  \ID COV_VSECPRIM_GHASH_SPEED_UP
    \REASON This check always has the same result since the switch actGHASH_SPEED_UP is constantly defined to the value 1.
    \PREVENTION Covered by code review.

  \ID COV_VSECPRIM_CPLUSPLUS
    \REASON This check always results in FALSE since the switch __cplusplus is always undefined.
    \PREVENTION Covered by code review.

  \ID COV_VSECPRIM_NO_SAFE_CONFIG
    \REASON This algorithm or option must not be configured for building a safe library.
    \PREVENTION ELISA plug-in

  \ID COV_VSECPRIM_STD_VALUES
    \ACCEPT TX
    \ACCEPT XF
    \REASON This check can only be true if the parameter is not defined externally.
            This check can only be false if the parameter is defined externally.

  \ID COV_VSECPRIM_NOT_CONFIGURABLE
    \REASON This check always results in the same boolean value since the considered parameter is constantly defined internally.
    \PREVENTION Covered by code review.

COV_JUSTIFICATION_END */


/**********************************************************************************************************************
 *  END OF FILE: vSecPrim.c
 *********************************************************************************************************************/

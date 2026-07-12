/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_CommonUtils.h
 *        \brief  Service API declaration with util functions for vItaHlp.
 *
 *      \details  -
 *
 *    \attention  Please note:
 *                The demo and example programs only show special aspects of the software. With regard to the fact
 *                that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *                expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (VITAHLP_COMMONUTILS_H)
# define VITAHLP_COMMONUTILS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define vItaHlp_CommonUtils_GetSafeBooleanValue(inputValue) \
( ( (inputValue) == FALSE ) ? (boolean)FALSE : (boolean)TRUE )

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_CommonUtils_Uint32ToByteArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts an uint32 input value to a given byte array interpreted in Big-Endian format.
 *  \details     Byte 0 gets the MSB and Byte 3 the LSB of the output value.
 *  \param[out]  outputValue output value which gets the converted input value
 *  \param[in]   inputValue input value which is converted as described to the output value
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CommonUtils_Uint32ToByteArrayBigEndian(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) outputValue,
  VAR(uint32, AUTOMATIC) inputValue);

/**********************************************************************************************************************
 *  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts an uint16 input value to a given byte array interpreted in Big-Endian format.
 *  \details     Byte 0 gets the MSB and Byte 1 the LSB of the output value.
 *  \param[out]  outputValue output value which gets the converted input value
 *  \param[in]   inputValue input value which is converted as described to the output value
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) outputValue,
  VAR(uint16, AUTOMATIC) inputValue);

/**********************************************************************************************************************
 *  vItaHlp_CommonUtils_ByteArrayToUint16BigEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts an input byte array to an uint16 value interpreted in Big-Endian format.
 *  \details     Byte 0gets the MSB and Byte 1 the LSB of the input value.
 *  \param[in]   inputValue input value which is converted as described to the output value
 *  \return      outputValue output value which gets the converted input value
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(uint16, VITAHLP_CODE) vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) inputValue);

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_COMMONUTILS_H */

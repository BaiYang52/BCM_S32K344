/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  E2EXf.c
 *        \brief  Vector AUTOSAR E2EXf code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module E2EXf.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0857 EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2, MD_MSR_Dir1.1 */
#define E2EXF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "E2EXf.h"

#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of E2EXf header file */
#if (  (E2EXF_SW_MAJOR_VERSION != (4U)) \
    || (E2EXF_SW_MINOR_VERSION != (32U)) \
    || (E2EXF_SW_PATCH_VERSION != (0U)) )
# error "Vendor specific version numbers of E2EXf.c and E2EXf.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (E2EXF_CFG_MAJOR_VERSION != (1U)) \
    || (E2EXF_CFG_MINOR_VERSION != (15U)) )
# error "Version numbers of E2EXf.c and E2EXf_LCfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Whether any of the profiles is enabled */
#if (E2EXF_P01_ENABLED == STD_ON) || (E2EXF_P02_ENABLED == STD_ON) || (E2EXF_P04_ENABLED == STD_ON) || (E2EXF_P05_ENABLED == STD_ON) \
    || (E2EXF_P06_ENABLED == STD_ON) || (E2EXF_P07_ENABLED == STD_ON) || (E2EXF_P08_ENABLED == STD_ON) || (E2EXF_P11_ENABLED == STD_ON) \
    || (E2EXF_P22_ENABLED == STD_ON) || (E2EXF_P44_ENABLED == STD_ON)
# define E2EXF_ANYPROFILE_ENABLED STD_ON
#else 
# define E2EXF_ANYPROFILE_ENABLED STD_OFF
#endif

/* Let RteAnalyzer check silence of an access explicitly */
#ifdef RTE_ANALYZER /* COV_E2EXF_STATIC_ANALYSIS */
# define E2EXF_ASSERT_SILENCE(access)    access = access;
#else
# define E2EXF_ASSERT_SILENCE(access)
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (E2EXF_LOCAL) /* COV_E2EXF_MSR_COMPATIBILITY */
# define E2EXF_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define E2EXF_START_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#include "E2EXf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
E2EXF_LOCAL VAR(uint8, E2EXF_VAR_ZERO_INIT_NOCACHE) E2EXf_ModuleInitialized = E2EXF_UNINIT;

#define E2EXF_STOP_SEC_VAR_NOCACHE_ZERO_INIT_8BIT
#include "E2EXf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define E2EXF_START_SEC_CODE
#include "E2EXf_MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_MemMap */

#if (E2EXF_ANYPROFILE_ENABLED == STD_ON)

/**********************************************************************************************************************
*  E2EXf_Get_Source_Buffer_Offset()
*********************************************************************************************************************/
/*! \brief     Get pointer to the source data buffer start
*  \details    This function decides whether the input is read from buffer or inputBufffer and at what offset based on the passed options.
*  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
*  \param[in]  inputBuffer             Input data for the E2E transformer.
*  \param[in]  options                 E2E transformer options.
*  \pre        -
*  \return     Pointer to the input data
*********************************************************************************************************************/
E2EXF_LOCAL FUNC_P2CONST(uint8, E2EXF_APPL_DATA, E2EXF_CODE) E2EXf_Get_Source_Buffer_Offset(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);

/**********************************************************************************************************************
*  E2EXf_Get_Source_Buffer_Offset()
*********************************************************************************************************************/
/*! \brief     Get pointer to the source data buffer
*  \details    This function decides whether the input is read from buffer or inputBufffer without any offset based on the passed options.
*  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
*  \param[in]  inputBuffer             Input data for the E2E transformer.
*  \param[in]  options                 E2E transformer options.
*  \pre        -
*  \return     Pointer to the input data
*********************************************************************************************************************/
E2EXF_LOCAL FUNC_P2CONST(uint8, E2EXF_APPL_DATA, E2EXF_CODE) E2EXf_Get_Source_Buffer(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);

/**********************************************************************************************************************
*  E2EXf_Get_Source_Buffer_Offset()
*********************************************************************************************************************/
/*! \brief     Get pointer to the target data buffer start
*  \details    This function calculates the start of the target buffer based on the passed options.
*  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
*  \param[in]  options                 E2E transformer options.
*  \pre        -
*  \return     Pointer to the target data start
*********************************************************************************************************************/
E2EXF_LOCAL FUNC_P2VAR(uint8, E2EXF_APPL_VAR, E2EXF_CODE) E2EXf_Get_Dest_Buffer_Offset(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);

/**********************************************************************************************************************
 *  E2EXf_Check_Parameters()
 *********************************************************************************************************************/
/*! \brief      Check input parameters
 *  \details    This function checks the input parameters for plausibility.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \param[in]  minInputBufferLength    Minimum length of the input data.
 *  \param[in]  max16Bit                Check that inputBufferLength does not exceed 16 bit (for profiles != 7)
 *  \pre        -
 *  \return     E_OK - input parameters plausible
 *  \return     E_SAFETY_HARD_RUNTIMEERROR - input parameters not plausible
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_Check_Parameters(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint32, AUTOMATIC, E2EXF_APPL_DATA) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  uint32 minInputBufferLength,
  boolean max16Bit);

/**********************************************************************************************************************
 *  E2EXf_Move_Header()
 *********************************************************************************************************************/
/*! \brief      Move header
 *  \details    This function moves the header.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \pre        -
 *  \spec
 *    requires  buffer != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);

/**********************************************************************************************************************
 *  E2EXf_Inv_Move_Header()
 *********************************************************************************************************************/
/*! \brief      Inverse move header
 *  \details    This function moves the header inverse.
 *  \param[in]  buffer                  Buffer where the E2E transformer places its output data.
 *  \param[in]  bufferLength            Used length of the buffer.
 *  \param[in]  inputBuffer             Input data for the E2E transformer.
 *  \param[in]  inputBufferLength       Length of the input data.
 *  \param[in]  options                 E2E transformer options.
 *  \pre        -
 *********************************************************************************************************************/
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Inv_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options);
#endif /* E2EXF_ANYPROFILE_ENABLED == STD_ON */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (E2EXF_ANYPROFILE_ENABLED == STD_ON)
/**********************************************************************************************************************
*  E2EXf_Get_Source_Buffer_Offset()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
E2EXF_LOCAL FUNC_P2CONST(uint8, E2EXF_APPL_DATA, E2EXF_CODE) E2EXf_Get_Source_Buffer_Offset(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer, /* PRQA S 3206 */ /* MD_E2EXf_3206 */
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) src;
# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  if (options->inPlace == TRUE)
  {
    src = &buffer[options->headerLength >> 3U];
  }
  else
  {
    src = inputBuffer;
  }
# else
  src = &buffer[options->headerLength >> 3U];
  E2EXF_DUMMY_STATEMENT(inputBuffer); /* PRQA S 1338, 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif
  return src;
}

/**********************************************************************************************************************
*  E2EXf_Get_Source_Buffer()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
E2EXF_LOCAL FUNC_P2CONST(uint8, E2EXF_APPL_DATA, E2EXF_CODE) E2EXf_Get_Source_Buffer(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer, /* PRQA S 3206 */ /* MD_E2EXf_3206 */
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options) /* PRQA S 3206 */ /* MD_E2EXf_3206 */
{
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) src;
# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  if (options->inPlace == TRUE)
  {
    src = buffer;
  }
  else
  {
    src = inputBuffer;
  }
# else
  src = buffer;
  E2EXF_DUMMY_STATEMENT(inputBuffer); /* PRQA S 1338, 3112, 2983 */ /* MD_MSR_DummyStmt */
  E2EXF_DUMMY_STATEMENT(options); /* PRQA S 1338, 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif
  return src;
}

/**********************************************************************************************************************
*  E2EXf_Get_Dest_Buffer_Offset()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
E2EXF_LOCAL FUNC_P2VAR(uint8, E2EXF_APPL_VAR, E2EXF_CODE) E2EXf_Get_Dest_Buffer_Offset(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_DATA) dst;
# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  if (options->inPlace == TRUE)
  {
    dst = &buffer[options->headerLength >> 3U];
  }
  else 
  {
    dst = buffer;
  }
# else
  dst = &buffer[options->headerLength >> 3U];
# endif
  return dst;
}

/**********************************************************************************************************************
 *  E2EXf_Check_Parameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
E2EXF_LOCAL FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_Check_Parameters(
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) buffer,
  P2CONST(uint32, AUTOMATIC, E2EXF_APPL_DATA) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  uint32 minInputBufferLength,
  boolean max16Bit)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) src = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);

  /* ----- Implementation ----------------------------------------------- */
  /* [SWS_E2EXf_00102] 1. / [SWS_E2EXf_00105] 1. / [SWS_E2EXf_00106] 1. / [SWS_E2EXf_00103] 1. */
  if ((src == NULL_PTR) && (inputBufferLength != 0U))
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else if ((src != NULL_PTR) && (inputBufferLength < minInputBufferLength))
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  /* [SWS_E2EXf_00102] 2. / [SWS_E2EXf_00105] 2. / [SWS_E2EXf_00106] 2. / [SWS_E2EXf_00103] 2. */
  else if (bufferLength == NULL_PTR)
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  /* [SWS_E2EXf_00106] 3. / [SWS_E2EXf_00103] 3. */
  else if ((options->inPlace == FALSE) && (buffer == NULL_PTR))
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
# endif
  /* execution of E2EXf is explicitly not wanted/expected in this case */
  /* Note: buffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00102/SWS_E2EXf_00105 */
  /* Note: inputBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00103/SWS_E2EXf_00106 */
  else if ((options->executeDespiteDataUnavailability == FALSE) && (src == NULL_PTR))
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  /* E2E APIs for profiles != 7 only have 16 bit length parameters */
  else if ((max16Bit == TRUE) && (inputBufferLength > 0xFFFFU))
  {
    retVal = E_SAFETY_HARD_RUNTIMEERROR;
  }
  else
  {
    /* nothing to do */
  }

  /*@ assert buffer != NULL_PTR; */ /* VCA_E2EXF_CHECKED_PARAMETER_VALID */
  /*@ assert inputBuffer != NULL_PTR; */ /* VCA_E2EXF_CHECKED_PARAMETER_VALID */
  /*@ assert bufferLength != NULL_PTR; */ /* VCA_E2EXF_CHECKED_PARAMETER_VALID */
  return retVal;

} /* E2EXf_Check_Parameters() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  E2EXf_Move_Header()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) psrc;
  uint32 i;
  uint32 headerShiftBytes;
  uint32 headerBytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  headerShiftBytes = options->headerLength >> 3U;
  headerBytesToCopy = options->upperHeaderBitsToShift >> 3U;

  if ((inputBufferLength > 0U) && (options->upperHeaderBitsToShift > 0U))
  {
    psrc = E2EXf_Get_Source_Buffer_Offset(buffer, inputBuffer, options);

    /* [SWS_E2EXf_00108] [SWS_E2EXf_00109] */
    /* copy header bytes */
    for (i = 0U; i < headerBytesToCopy; i++) /* FETA_E2EXf_UpperHeaderBitsToShift */
    {
      buffer[i] = psrc[i];
    }
  }

# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  if (options->inPlace == FALSE)
  {
    /* copy remaining bytes */
    for (i = headerBytesToCopy; i < inputBufferLength; i++) /* FETA_E2EXf_UpperHeaderBitsToShift */
    {
      buffer[i + headerShiftBytes] = inputBuffer[i];
    }
    *bufferLength = inputBufferLength + headerShiftBytes;
  }
  else
# endif
  if (options->upperHeaderBitsToShift == 0U)
  {
    *bufferLength = inputBufferLength;
  }
  else
  {
    /* [SWS_E2EXf_00111] */
    *bufferLength = inputBufferLength + headerShiftBytes;
  }

} /* E2EXf_Move_Header() */

/**********************************************************************************************************************
 *  E2EXf_Inv_Move_Header()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2EXF_LOCAL FUNC (void, E2EXF_CODE) E2EXf_Inv_Move_Header(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) psrc;
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) pdest;
  uint32 i;
  uint32 headerShiftBytes;
  uint32 headerBytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  headerShiftBytes = options->headerLength >> 3U;
  headerBytesToCopy = options->upperHeaderBitsToShift >> 3U;
  pdest = E2EXf_Get_Dest_Buffer_Offset(buffer, options);

  if ((inputBufferLength > 0U) && (options->upperHeaderBitsToShift > 0U))
  {
    psrc = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);

    /* [SWS_E2EXf_00112] [SWS_E2EXf_00113] */
    /* copy header bytes */
    for (i = 0U; i < headerBytesToCopy; i++) /* FETA_E2EXf_UpperHeaderBitsToShift */
    {
      pdest[i] = psrc[i];
    }

  }

# if E2EXF_OUTOFPLACE_ENABLED == STD_ON
  if (options->inPlace == FALSE)
  {
    /* copy remaining bytes */
    for (i = headerBytesToCopy + headerShiftBytes; i < inputBufferLength; i++) /* FETA_E2EXf_UpperHeaderBitsToShift */
    {
      pdest[i - headerShiftBytes] = inputBuffer[i];
    }
    *bufferLength = inputBufferLength - headerShiftBytes;
  }
  else
# endif
  if (options->upperHeaderBitsToShift == 0U)
  {
    *bufferLength = inputBufferLength;
  }
  else
  {
    /* [SWS_E2EXf_00114] */
    *bufferLength = inputBufferLength - headerShiftBytes;
  }

} /* E2EXf_Inv_Move_Header() */
#endif /* (E2EXF_ANYPROFILE_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2EXf_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  E2EXf_ModuleInitialized = E2EXF_UNINIT;

} /* E2EXf_InitMemory() */

/***********************************************************************************************************************
 *  E2EXf_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_Init(P2CONST(E2EXf_ConfigType, AUTOMATIC, E2EXF_PBCFG) config) /* PRQA S 3206 */ /* MD_E2EXf_3206 */ 
{
  /* ----- Implementation ----------------------------------------------- */
  E2EXf_Init_Calls();

  E2EXf_ModuleInitialized = (uint8)E2EXF_INIT;

  E2EXF_DUMMY_STATEMENT(config); /* PRQA S 1338, 3112, 2983 */ /* MD_MSR_DummyStmt */

} /* E2EXf_Init() */

/***********************************************************************************************************************
 *  E2EXf_DeInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (void, E2EXF_CODE) E2EXf_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  uint8 errorId = E2EXF_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  /* Check initialization state of the component */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* [SWS_E2EXf_00148] */
    E2EXf_ModuleInitialized = (uint8)E2EXF_UNINIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  /* [SWS_E2EXF_00146] */
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_DE_INIT, errorId);
  }
#endif

} /* E2EXf_DeInit() */

#if (E2EXF_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  E2EXf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, E2EXF_CODE) E2EXf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, E2EXF_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = E2EXF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = E2EXF_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (E2EXF_VENDOR_ID);
    versioninfo->moduleID = (E2EXF_MODULE_ID);
    versioninfo->sw_major_version = (E2EXF_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (E2EXF_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (E2EXF_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  /* [SWS_E2EXF_00149] */
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_GET_VERSION_INFO, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

} /* E2EXf_GetVersionInfo() */
#endif /* (E2EXF_VERSION_INFO_API == STD_ON) */

#if (E2EXF_P01_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P01()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P01(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00139] */
      if (*bufferLength != ((uint32)config->DataLength >> 3U))
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00155] */
        /* if DataIDMode equals E2E_P01_DATAID_NIBBLE the low nibble of high byte of DataID is transmitted explicitly, otherwise the unused nibble is set to 0xF */
        if ((config->DataIDMode != E2E_P01_DATAID_NIBBLE) && (options->headerLength == 16U))
        {
          if ((config->CounterOffset & 0x07U) == 0U)
          {
            buffer[(config->CounterOffset >> 3U)] |= 0xF0U;
          }
          else
          {
            buffer[(config->CounterOffset >> 3U)] |= 0x0FU;
          }
        }

        /* [SWS_E2EXf_00107] */
        E2EXF_ASSERT_SILENCE(buffer[(config->CounterOffset >> 3U)])
        E2EXF_ASSERT_SILENCE(buffer[(config->CRCOffset >> 3U)])
        retVal = E2E_P01Protect(config, state, buffer);

        /* [SWS_E2EXf_00018] */
        if (retVal == E2E_E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P01() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P01_ENABLED == STD_ON) */

#if (E2EXF_P11_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P11()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P11(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P11ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P11ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00139] */
      if (*bufferLength != ((uint32)config->DataLength >> 3U))
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00155] */
        /* if DataIDMode equals E2E_P11_DATAID_NIBBLE the low nibble of high byte of DataID is transmitted explicitly, otherwise the unused nibble is set to 0xF */
        if ((config->DataIDMode != E2E_P11_DATAID_NIBBLE) && (options->headerLength == 16U))
        {
          if ((config->CounterOffset & 0x07U) == 0U)
          {
            buffer[(config->CounterOffset >> 3U)] |= 0xF0U;
          }
          else
          {
            buffer[(config->CounterOffset >> 3U)] |= 0x0FU;
          }
        }

        /* [SWS_E2EXf_00107] */
        E2EXF_ASSERT_SILENCE(buffer[(config->CounterOffset >> 3U)])
        E2EXF_ASSERT_SILENCE(buffer[(config->CRCOffset >> 3U)])
        retVal = E2E_P11Protect(config, state, buffer, (uint16)*bufferLength);

        /* [SWS_E2EXf_00018] */
        if (retVal == E2E_E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P11() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P11_ENABLED == STD_ON) */

#if (E2EXF_P02_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P02()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P02(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00139] */
      if (*bufferLength != ((uint32)config->DataLength >> 3U))
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR;
      }
      else
      {
        /* [SWS_E2EXf_00155] */
        if (options->headerLength == 16U)
        {
          buffer[(config->Offset >> 3U) + 1U] |= 0xF0U;
        }

        /* [SWS_E2EXf_00107] */
        E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 1U])
        retVal = E2E_P02Protect(config, state, buffer);

        /* [SWS_E2EXf_00018] */
        if (retVal == E2E_E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P02() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P02_ENABLED == STD_ON) */

#if (E2EXF_P22_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P22()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P22(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P22ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P22ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */

  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00155] */
      if (options->headerLength == 16U)
      {
        buffer[(config->Offset >> 3U) + 1U] |= 0xF0U;
      }

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 1U])
      retVal = E2E_P22Protect(config, state, buffer, (uint16)*bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P22() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P22_ENABLED == STD_ON) */

#if (E2EXF_P04_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P04()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P04(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 11U])
      retVal = E2E_P04Protect(config, state, buffer, (uint16)*bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P04() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P04_ENABLED == STD_ON) */

#if (E2EXF_P44_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P44()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P44(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P44ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P44ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR;
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 11U])
      retVal = E2E_P44Protect(config, state, buffer, (uint16)*bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P44() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P44_ENABLED == STD_ON) */

#if (E2EXF_P05_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P05()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P05(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 2U])
      retVal = E2E_P05Protect(config, state, buffer, (uint16)*bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P05() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P05_ENABLED == STD_ON) */

#if (E2EXF_P06_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P06()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P06(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 5U])
      retVal = E2E_P06Protect(config, state, buffer, (uint16)*bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P06() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P06_ENABLED == STD_ON) */

#if (E2EXF_P07_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P07()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P07(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, FALSE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 19U])
      retVal = E2E_P07Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P07() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P07_ENABLED == STD_ON) */

#if (E2EXF_P08_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P08()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P08(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P08ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P08ProtectStateType, AUTOMATIC, E2EXF_APPL_VAR) state)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00151] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00150] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = options->upperHeaderBitsToShift >> 3U;

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, FALSE);

    if (retVal == E_OK)
    {
      E2EXf_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);

      /* [SWS_E2EXf_00107] */
      E2EXF_ASSERT_SILENCE(buffer[(config->Offset >> 3U) + 15U])
      retVal = E2E_P08Protect(config, state, buffer, *bufferLength);

      /* [SWS_E2EXf_00018] */
      if (retVal == E2E_E_OK)
      {
        retVal = E_OK;
      }
      else
      {
        retVal = E_SAFETY_HARD_RUNTIMEERROR; /* E2E protection could not be applied, output not valid -> HARD error */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P08() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P08_ENABLED == STD_ON) */

#if (E2EXF_P01_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P01_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P01_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P01ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P01CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  uint8 dummyByte = 0U;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);

        /* [SWS_E2EXf_00140] / [SWS_E2EXf_00141] */
        /* Note: pCheckBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00105/SWS_E2EXf_00106 */
        if (pCheckBuffer == NULL_PTR)
        {
          state->NewDataAvailable = FALSE;
          pCheckBuffer = &dummyByte; /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] pointer to a 1-byte static variable */
        }
        else if (inputBufferLength == ((uint32)config->DataLength >> 3U))
        {
          state->NewDataAvailable = TRUE;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR;
        }

        if (retVal == E_OK)
        {
          /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] */
          retVal = E2E_P01Check(config, state, pCheckBuffer);

          /* [SWS_E2EXf_00142] */
          if (options->profileBehavior == FALSE)
          {
            state->MaxDeltaCounter = config->MaxDeltaCounterInit;
          }

          /* [SWS_E2EXf_00029] */
          profileStatus = E2E_P01MapStatusToSM(retVal, state->Status, options->profileBehavior);

          /* [SWS_E2EXf_00028] */
          retVal = E2E_SMCheck(profileStatus, smConfig, smState);

          /* [SWS_E2EXf_00027] */
          if (retVal == E2E_E_OK)
          {
            retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
          }
          else
          {
            retVal = E_SAFETY_SOFT_RUNTIMEERROR;
          }
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P01_Inv() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P01_ENABLED == STD_ON) */

#if (E2EXF_P11_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P11_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P11_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P11ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P11CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        
        retVal = E2E_P11Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P11MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P11_Inv() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P11_ENABLED == STD_ON) */

#if (E2EXF_P02_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P02_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6060, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P02_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P02ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  uint8 dummyByte = 0U;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);

        /* [SWS_E2EXf_00140] / [SWS_E2EXf_00141] */
        /* Note: pCheckBuffer == NULL_PTR && inputBufferLength != 0 already handled by SWS_E2EXf_00105/SWS_E2EXf_00106 */
        if (pCheckBuffer == NULL_PTR)
        {
          state->NewDataAvailable = FALSE;
          pCheckBuffer = &dummyByte; /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] pointer to a 1-byte static variable */
        }
        else if (inputBufferLength == ((uint32)config->DataLength >> 3U))
        {
          state->NewDataAvailable = TRUE;
        }
        else
        {
          retVal = E_SAFETY_HARD_RUNTIMEERROR;
        }

        if (retVal == E_OK)
        {
          /* [SWS_E2EXf_00123] / [SWS_E2EXf_00124] */
          retVal = E2E_P02Check(config, state, pCheckBuffer);

          /* [SWS_E2EXf_00142] */
          if (options->profileBehavior == FALSE)
          {
            state->MaxDeltaCounter = config->MaxDeltaCounterInit;
          }

          /* [SWS_E2EXf_00029] */
          profileStatus = E2E_P02MapStatusToSM(retVal, state->Status, options->profileBehavior);

          /* [SWS_E2EXf_00028] */
          retVal = E2E_SMCheck(profileStatus, smConfig, smState);

          /* [SWS_E2EXf_00027] */
          if (retVal == E2E_E_OK)
          {
            retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
          }
          else
          {
            retVal = E_SAFETY_SOFT_RUNTIMEERROR;
          }
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P02_Inv() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P02_ENABLED == STD_ON) */

#if (E2EXF_P22_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P22_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P22_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P22ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P22CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  E2E_PCheckStatusType profileStatus;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);

        retVal = E2E_P22Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P22MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P22_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P22_ENABLED == STD_ON) */

#if (E2EXF_P04_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P04_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P04_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P04ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        
        retVal = E2E_P04Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P04MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P04_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P04_ENABLED == STD_ON) */

#if (E2EXF_P44_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P44_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P44_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P44ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P44CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        
        retVal = E2E_P44Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P44MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P44_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P44_ENABLED == STD_ON) */

#if (E2EXF_P05_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P05_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P05_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P05ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P05CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        retVal = E2E_P05Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P05MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P05_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P05_ENABLED == STD_ON) */

#if (E2EXF_P06_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P06_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P06_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P06ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, TRUE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        retVal = E2E_P06Check(config, state, pCheckBuffer, (uint16)inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P06MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P06_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P06_ENABLED == STD_ON) */

#if (E2EXF_P07_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P07_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P07_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P07ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, FALSE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        retVal = E2E_P07Check(config, state, pCheckBuffer, inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P07MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P07_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P07_ENABLED == STD_ON) */

#if (E2EXF_P08_ENABLED == STD_ON)
/***********************************************************************************************************************
 *  E2EXf_P08_Inv()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6060, 6080 1 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
FUNC (Std_ReturnType, E2EXF_CODE) E2EXf_P08_Inv(
  P2VAR(uint8, AUTOMATIC, E2EXF_APPL_VAR) buffer,
  P2VAR(uint32, AUTOMATIC, E2EXF_APPL_VAR) bufferLength,
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) inputBuffer,
  uint32 inputBufferLength,
  P2CONST(E2EXf_OptionsType, AUTOMATIC, E2EXF_APPL_DATA) options,
  P2CONST(E2E_P08ConfigType, AUTOMATIC, E2EXF_APPL_DATA) config,
  P2VAR(E2E_P08CheckStateType, AUTOMATIC, E2EXF_APPL_VAR) state,
  P2CONST(E2E_SMConfigType, AUTOMATIC, E2EXF_APPL_DATA) smConfig,
  P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2EXF_APPL_VAR) smState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_SAFETY_HARD_RUNTIMEERROR;
  uint8 errorId = E2EXF_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint32 minInputBufferLength;
  P2CONST(uint8, AUTOMATIC, E2EXF_APPL_DATA) pCheckBuffer;
  E2E_PCheckStatusType profileStatus;

  /* ----- Development Error Checks ------------------------------------- */
  /* [SWS_E2EXf_00153] [SWS_E2EXf_00133] */
  if (E2EXf_ModuleInitialized == (uint8)E2EXF_UNINIT)
  {
    errorId = E2EXF_E_UNINIT; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# if (E2EXF_DEV_ERROR_DETECT == STD_ON)
  /* [SWS_E2EXf_00152] */
  else if ((options == NULL_PTR) || (config == NULL_PTR) || (state == NULL_PTR) || (smConfig == NULL_PTR) || (smState == NULL_PTR))
  {
    errorId = E2EXF_E_PARAM_POINTER; /* PRQA S 2983 */ /* MD_E2EXf_2983 */
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* calculate minimum inputBuffer length */
    minInputBufferLength = (options->upperHeaderBitsToShift >> 3U) + (options->headerLength >> 3U);

    /* check input parameters */
    retVal = E2EXf_Check_Parameters(buffer, bufferLength, inputBuffer, inputBufferLength, options, minInputBufferLength, FALSE);

    if (retVal == E_OK)
    {
      if (options->disableEndToEndCheck == FALSE)
      {
        /* [SWS_E2EXf_00104] */
        pCheckBuffer = E2EXf_Get_Source_Buffer(buffer, inputBuffer, options);
        retVal = E2E_P08Check(config, state, pCheckBuffer, inputBufferLength);

        /* [SWS_E2EXf_00029] */
        profileStatus = E2E_P08MapStatusToSM(retVal, state->Status);

        /* [SWS_E2EXf_00028] */
        retVal = E2E_SMCheck(profileStatus, smConfig, smState);

        /* [SWS_E2EXf_00027] */
        if (retVal == E2E_E_OK)
        {
          retVal = ((uint8)smState->SMState << 4U) | ((uint8)profileStatus & 0xFU);
        }
        else
        {
          retVal = E_SAFETY_SOFT_RUNTIMEERROR;
        }
      }
      E2EXf_Inv_Move_Header(buffer, bufferLength, inputBuffer, inputBufferLength, options);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (E2EXF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != E2EXF_E_NO_ERROR)
  {
    (void)Det_ReportError(E2EXF_MODULE_ID, E2EXF_INSTANCE_ID_DET, E2EXF_SID_INV_TRANSFORMER_ID, errorId);
  }
# else
  E2EXF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;

} /* E2EXf_P08_Inv() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* (E2EXF_P08_ENABLED == STD_ON) */

#define E2EXF_STOP_SEC_CODE
#include "E2EXf_MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 MISRA 2012 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_E2EXf_3206:  MISRA rule: Rule2.7
     Reason:       The parameter's are not used by the code in all possible code variants.
     Risk:         No functional risk.
     Prevention:   Not required.

   MD_E2EXf_2983:  MISRA rule: Rule2.2
     Reason:       The assignment is not used by the code in all possible code variants.
     Risk:         No functional risk.
     Prevention:   Not required.
*/

/**********************************************************************************************************************
 VCA justifications
 *********************************************************************************************************************/

/* VCA_JUSTIFICATION_BEGIN

\ID VCA_E2EXF_CHECKED_PARAMETER_VALID
\DESCRIPTION The different valid compinations of the input parameters are checked in E2EXf_Check_Parameters. 
             The assertion after the check is necessary to help the VCA and avoids false positives. After the
			 runtime checks all parameters are valid or E2EXf_Check_Parameters returns with E_SAFETY_HARD_RUNTIMEERROR.
\COUNTERMEASURE \N The assertion avoids false positives of the VCA and was checked by review.

VCA_JUSTIFICATION_END */

/**********************************************************************************************************************
 Code coverage justifications
 *********************************************************************************************************************/

/* COV_JUSTIFICATION_BEGIN

--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

  \ID COV_E2EXF_MSR_COMPATIBILITY
  \ACCEPT TX 
  \REASON [COV_MSR_COMPATIBILITY]

  \ID COV_E2EXF_STATIC_ANALYSIS
  \ACCEPT XF
  \REASON RTE_ANALYZER is set only by RteAnalyzer during static analysis. The activated statement has no side effects.
  
COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 Finite Execution Time Analysis justifications
 *********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

  \ID FETA_E2EXf_UpperHeaderBitsToShift
  \DESCRIPTION Handling of UpperHeaderBitsToShift
  \COUNTERMEASURE \D DSGN-E2EXf22772

FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: E2EXf.c
 *********************************************************************************************************************/

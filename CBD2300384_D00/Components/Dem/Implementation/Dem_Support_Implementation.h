/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Support
 *  \{
 *  \file       Dem_Support_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SUPPORT_IMPLEMENTATION_H)
#define DEM_SUPPORT_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_Support_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Client_Interface.h"

#include "Dem_ReadinessIF_Interface.h"
#include "Dem_Completion_Interface.h"
                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_SUPPORT_IMPLEMENTATION_FILENAME "Dem_Support_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Support_PrivateProperties Private Properties
 * \{
 */
/* ****************************************************************************
 * Dem_Readiness_Support_GetPid41DisabledMask
 *****************************************************************************/
/*!
 * \brief         Get PID41 disabled mask state
 *
 * \details       Get PID41 disabled mask state
 *
 * \return        Value of PID41 disabled mask
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Readiness_Support_GetPid41DisabledMask(void);

/* ****************************************************************************
 * Dem_Readiness_Support_IsPID41CalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if PID41 calculation is supported.
 *
 * \details       Tests if PID41 calculation is supported.
 *
 * \return        TRUE
 *                PID41 calculation is supported.
 * \return        FALSE
 *                PID41 calculation is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Support_IsPID41CalculationSupported(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Readiness_Support_GetPid41DisabledMask
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Readiness_Support_GetPid41DisabledMask(void)
{
#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON)
  return Dem_Cfg_AdminData.Pid41DisabledMask;
#else
  return 0;
#endif
}

/* ****************************************************************************
 % Dem_Readiness_Support_IsPID41CalculationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Support_IsPID41CalculationSupported(
  void
)
{
  return DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON;
}
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Support_PublicProperties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Support_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Support_GetSupportMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of supported state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of supported state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask of supported state for the corresponding PID01 or
 *                PID41 Data Block
 * \param[out]    BytePos
 *                Byte Position of supported state of the corresponding PID01
 *                or PID41 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForPIDs(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Support_GetSupportMaskAndPositionForDM05
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of support bits for
 *                a Readiness Group in DM 05 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of support bits for
 *                a Readiness Group in DM 05 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DM 05 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DM 05 Data Block
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForDM05(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Support_SetReadinessGroupSupported
 *****************************************************************************/
/*!
 * \brief         Set the readiness group supported or not supported in the
 *                given buffer for the respective data context.
 *
 * \details       Set the readiness group supported or not supported in the
 *                given buffer for the respective data context.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     Supported
 *                TRUE    Readiness group is set to supported
 *                FALSE   Readiness group is set to not supported
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[in,out] DestinationBuffer
 *                Destination buffer
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupSupported(
  uint8 ReadinessGroup,
  boolean Supported,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Support_TestReadinessGroupDisabled
 *****************************************************************************/
/*!
 * \brief         Checks if Readiness Group is disabled
 *
 * \details       Checks if Readiness Group is disabled
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness group is disabled
 * \return        FALSE
 *                Readiness group is enabled
 *
 * \pre           Function to be invoked only if dynamic calculation of
 *                PID41 is enabled.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Support_TestReadinessGroupDisabled(
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Support_SetGeneratedReadinessMasks
 *****************************************************************************/
/*!
 * \brief         Set generated supported and completion mask for
 *                respective data context.
 *
 * \details       Write generated supported and completion mask for
 *                respective data context to destination buffer
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[out]    DestinationBuffer
 *                Destination buffer with supported bits set and corresponding
 *                completion bits set to not completed for supported
 *                readiness groups and respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetGeneratedReadinessMasks(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Support_InitializeDestBuffer
 *****************************************************************************/
/*!
 * \brief         Initialize data buffer with 0 for respective data context.
 *
 * \details       -
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[out]    DestinationBuffer
 *                Destination buffer initialized with 0 respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_InitializeDestBuffer(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ****************************************************************************
 % Dem_Readiness_Support_GetSupportMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForPIDs(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_PID_SUPPORT_MISF_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_MISF_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_FLSYS_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_FLSYS_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_CMPRCMPT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_CAT_MASK;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_CAT_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_HCCAT_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_HCCAT_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_HTCAT_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_HTCAT_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_NOXCAT_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_NOXCAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_PID_SUPPORT_EVAP_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_EVAP_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_SECAIR_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_SECAIR_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_BOOSTPR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_BOOSTPR_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_O2SENS:
    *ByteMask = DEM_READINESS_PID_SUPPORT_O2SENS_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_O2SENS_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_EGSENS_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_SUPPORT_EGSENS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    *ByteMask = DEM_READINESS_PID_SUPPORT_O2SENSHT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_O2SENSHT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_PMFLT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_SUPPORT_PMFLT_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_PID_SUPPORT_EGR_MASK;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_EGR_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = DEM_READINESS_PID_SUPPORT_NONE_MASK;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_SUPPORT_NONE_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Support_GetSupportMaskAndPositionForDM05
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForDM05(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{

  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DM05_SUPPORT_MISF_MASK;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_MISF_BYTE;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DM05_SUPPORT_FLSYS_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_FLSYS_BYTE;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DM05_SUPPORT_CMPRCMPT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_CMPRCMPT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DM05_CAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_CAT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DM05_HTCAT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_HTCAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DM05_EVAP_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_EVAP_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DM05_SECAIR_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_SECAIR_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_AC:
    *ByteMask = DEM_READINESS_DM05_AC_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_AC_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DM05_EGSENS_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_EGSENS_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGHSENS:
    *ByteMask = DEM_READINESS_DM05_EGHSENS_MASK;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_EGHSENS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DM05_EGRVVT_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_EGRVVT_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSAS:
    *ByteMask = DEM_READINESS_DM05_CSAS_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_CSAS_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DM05_BOOSTPR_MASK;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_BOOSTPR_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DM05_PMFLT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_PMFLT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DM05_NOX_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_NOX_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DM05_HCCAT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_SUPPORT_HCCAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    /* invalid readiness group */
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Support_SetReadinessGroupSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupSupported(
  uint8 ReadinessGroup,
  boolean Supported,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lByteMask;
  uint8 lBytePos;

  lByteMask = 0u;
  lBytePos = 0;
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
  case DEM_READINESS_DATACONTEXT_PID41:
    Dem_Readiness_Support_GetSupportMaskAndPositionForPIDs(ReadinessGroup, &lByteMask, &lBytePos);                               /* SBSW_DEM_POINTER_FORWARD_STACK */
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
    Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501(ReadinessGroup, &lByteMask, &lBytePos);                            /* SBSW_DEM_POINTER_FORWARD_STACK */
    break;
  case DEM_READINESS_DATACONTEXT_DM05:
    Dem_Readiness_Support_GetSupportMaskAndPositionForDM05(ReadinessGroup, &lByteMask, &lBytePos);                               /* SBSW_DEM_POINTER_FORWARD_STACK */
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  if (lByteMask != 0u)
  {
    if (Supported == TRUE)
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lBytePos, DestinationBuffer->Buffer[lBytePos] | lByteMask);      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lBytePos, DestinationBuffer->Buffer[lBytePos] & ((uint8)~lByteMask)); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Support_TestReadinessGroupDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Support_TestReadinessGroupDisabled(
  uint8 ReadinessGroup
)
{
  boolean lReturnValue;
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_MISF_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_FLSYS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_CMPRCMPT) != 0u);
    break;
  case DEM_CFG_READINESS_CAT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_CAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_HCCAT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_HCCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_HTCAT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_HTCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_NOXCAT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EVAP:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_EVAP_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_SECAIR:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_SECAIR_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_BOOSTPR_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_O2SENS:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_O2SENS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EGSENS:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_EGSENS_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_O2SENSHT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_PMFLT:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_PMFLT_MASK) != 0u);
    break;
  case DEM_CFG_READINESS_EGR:
    lReturnValue = (boolean)((Dem_Readiness_Support_GetPid41DisabledMask() & DEM_READINESS_DISABLED_EGR_MASK) != 0u);
    break;
  default:
    /* case DEM_CFG_READINESS_NONE: */
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Support_SetGeneratedReadinessMasks
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetGeneratedReadinessMasks(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DestinationBuffer)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
#if (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON)

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID01_BYTEB,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteB());
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID01_BYTEC,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteC());
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID01_BYTED,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteD());
#endif
    break;

  case DEM_READINESS_DATACONTEXT_DIDF501:
#if (DEM_FEATURE_NEED_PIDF501_CALCULATION == STD_ON)
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DIDF501_BYTEB,                                                                                               
      (uint8)Dem_Cfg_GetGlobalDIDF501SupportedAndCompletedMaskByteB());                                                          
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DIDF501_BYTEC,                                                                                               
      (uint8)Dem_Cfg_GetGlobalDIDF501SupportedAndCompletedMaskByteC());                                                          
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DIDF501_BYTED,                                                                                               
      (uint8)Dem_Cfg_GetGlobalDIDF501SupportedAndCompletedMaskByteD());                                                          
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DIDF501_BYTEE,                                                                                               
      (uint8)Dem_Cfg_GetGlobalDIDF501SupportedAndCompletedMaskByteE());                                                          
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DIDF501_BYTEF,                                                                                               
      (uint8)Dem_Cfg_GetGlobalDIDF501SupportedAndCompletedMaskByteF());                                                          
#endif                                                                                                                           
    break;

  case DEM_READINESS_DATACONTEXT_DM05:                                                                                           
#if (DEM_FEATURE_NEED_DM05_CALCULATION == STD_ON)                                                                                
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DM05_BYTE4,                                                                                                  
      (uint8)Dem_Cfg_GetGlobalDM05SupportedAndCompletedMaskByte4());                                                             
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DM05_BYTE5,                                                                                                  
      (uint8)Dem_Cfg_GetGlobalDM05SupportedAndCompletedMaskByte5());                                                             
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DM05_BYTE6,                                                                                                  
      (uint8)Dem_Cfg_GetGlobalDM05SupportedAndCompletedMaskByte6());                                                             
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DM05_BYTE7,                                                                                                  
      (uint8)Dem_Cfg_GetGlobalDM05SupportedAndCompletedMaskByte7());                                                             
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_DM05_BYTE8,
      (uint8)Dem_Cfg_GetGlobalDM05SupportedAndCompletedMaskByte8());

#endif
    break;

  case DEM_READINESS_DATACONTEXT_PID41:
#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON)
# if (DEM_CFG_SUPPORT_MAJOR_MONITORS == STD_ON)
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID41_BYTEB,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteB() & DEM_READINESS_PID41_SUPPORT_BYTEB);
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID41_BYTEC,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteC());
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTED, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# else
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID41_BYTEB,
      (uint8)Dem_Cfg_GetGlobalPID01SupportedAndCompletedMaskByteB() & DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK);
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEC, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTED, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# endif
#else
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEB, DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK); /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEC, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTED, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#endif
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_Readiness_Support_InitializeDestBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_InitializeDestBuffer(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID01_BYTEB, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID01_BYTEC, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID01_BYTED, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  case DEM_READINESS_DATACONTEXT_DIDF501:                                                                                        
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEB, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEC, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTED, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEE, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEF, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  case DEM_READINESS_DATACONTEXT_DM05:                                                                                           
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE4, 0x00u);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE5, 0x00u);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE6, 0x00u);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE7, 0x00u);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE8, 0x00u);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  case DEM_READINESS_DATACONTEXT_PID41:
    if (Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported(DEM_READINESS_DATACONTEXT_PID41) == FALSE)
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEB, DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK); /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    }
    else
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEB, 0x00u);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEC, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTED, 0x00u);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Support_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_MISF_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_MISF_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_FLSYS_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_FLSYS_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CMPRCMPT_MASK;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CMPRCMPT_BYTE;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_ECS:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_ECS_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_ECS_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CAT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CAT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_HCCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_HCCAT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_HTCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_HTCAT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_NOXCAT_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_NOXCAT_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EVAP_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EVAP_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_SECAIR_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_SECAIR_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_BOOSTPR_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_BOOSTPR_BYTE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_PMFLT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_PMFLT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EGSENS_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EGSENS_BYTE;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PCV:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_PCV_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_PCV_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_EGR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_EGR_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_VVT_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_VVT_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_DOR:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_DOR_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_DOR_BYTE;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSER:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_CSER_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_CSER_BYTE;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_NOXADSORB_MASK;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_NOXADSORB_BYTE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_OTHER:
    *ByteMask = DEM_READINESS_DIDF501_SUPPORT_OTHER_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DIDF501_OTHER_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupSupportedAndNotCompleted(
  uint8 DataContext,
  Dem_EventIdType EventId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  if (Dem_Core_Event_IsEventAvailabilitySupported() == TRUE)
  {
    uint8 lReadinessGroup;

    lReadinessGroup = Dem_Core_Event_GetEventReadinessGroup(EventId);
    /* If event is assigned to a valid readiness group and available */
    if ( (lReadinessGroup != DEM_CFG_READINESS_NONE)
      && (Dem_Event_TestEventUnavailable(EventId) == FALSE) )
    {
      Dem_Readiness_Support_SetReadinessGroupSupported(lReadinessGroup, TRUE, DataContext, DestinationBuffer);                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      if (DataContext != DEM_READINESS_DATACONTEXT_PID41)
      {
        Dem_Readiness_Completion_SetReadinessGroupCompleted(lReadinessGroup, FALSE, DataContext, DestinationBuffer);             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates
 *****************************************************************************/
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetStaticSupportedAndNotCompletedStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  if ( (Dem_GlobalDiagnostics_Readiness_IsCalculationForDataContextSupported(DataContext) == FALSE)
    || (Dem_Core_Event_IsEventAvailabilitySupported() == TRUE))
  {
    Dem_Readiness_Support_InitializeDestBuffer(DataContext, DestinationBuffer);                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    Dem_Readiness_Support_SetGeneratedReadinessMasks(DataContext, DestinationBuffer);                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }

  if ( (Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    && ((DataContext == DEM_READINESS_DATACONTEXT_PID01) || (DataContext == DEM_READINESS_DATACONTEXT_PID41))
     )
  {
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer,                                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      DEM_READINESS_PID_SUPPORT_COMPRESSION_BYTE,
      (DestinationBuffer->Buffer[DEM_READINESS_PID_SUPPORT_COMPRESSION_BYTE] | DEM_READINESS_PID_SUPPORT_COMPRESSION_MASK));
  }
 
}

/* ****************************************************************************
 % Dem_Readiness_Support_SetReadinessGroupDisabled
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_SetReadinessGroupDisabled(
  uint8 ReadinessGroup
)
{
  if (Dem_Readiness_Support_IsPID41CalculationSupported() == TRUE)
  {
    uint16 lPid41DisabledMask;
    uint16 lSetRdyGroupDisabledMask;

    switch (ReadinessGroup)
    {
    case DEM_CFG_READINESS_MISF:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_MISF_MASK;
      break;
    case DEM_CFG_READINESS_FLSYS:
    case DEM_CFG_READINESS_FLSYS_NONCONT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_FLSYS_MASK;
      break;
    case DEM_CFG_READINESS_CMPRCMPT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_CMPRCMPT;
      break;
    case DEM_CFG_READINESS_CAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_CAT_MASK;
      break;
    case DEM_CFG_READINESS_HCCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_HCCAT_MASK;
      break;
    case DEM_CFG_READINESS_HTCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_HTCAT_MASK;
      break;
    case DEM_CFG_READINESS_NOXCAT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_NOXCAT_MASK;
      break;
    case DEM_CFG_READINESS_EVAP:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EVAP_MASK;
      break;
    case DEM_CFG_READINESS_SECAIR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_SECAIR_MASK;
      break;
    case DEM_CFG_READINESS_BOOSTPR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_BOOSTPR_MASK;
      break;
    case DEM_CFG_READINESS_O2SENS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_O2SENS_MASK;
      break;
    case DEM_CFG_READINESS_EGSENS:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EGSENS_MASK;
      break;
    case DEM_CFG_READINESS_O2SENSHT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_O2SENSHT_MASK;
      break;
    case DEM_CFG_READINESS_PMFLT:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_PMFLT_MASK;
      break;
    case DEM_CFG_READINESS_EGR:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_EGR_MASK;
      break;
    default:
      lSetRdyGroupDisabledMask = DEM_READINESS_DISABLED_NONE_MASK;
      break;
    }

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    lPid41DisabledMask = Dem_Readiness_Support_GetPid41DisabledMask();
    lPid41DisabledMask |= lSetRdyGroupDisabledMask;
    Dem_GlobalDiagnostics_Readiness_SetPid41DisabledMask(lPid41DisabledMask);

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Support_ConsiderDisabledReadinessGroups
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Support_ConsiderDisabledReadinessGroups(
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lReadinessGroup;
  /* Readiness Group with number 0 corresponds to DEM_CFG_READINESS_NONE and can be skipped */
  for (lReadinessGroup = 1u; lReadinessGroup < DEM_CFG_READINESS_GROUP_SIZE; lReadinessGroup++)
  {
    /* If readiness group is disabled, set readiness group to not supported */
    if (Dem_Readiness_Support_TestReadinessGroupDisabled(lReadinessGroup) == TRUE)
    {
      Dem_Readiness_Support_SetReadinessGroupSupported(lReadinessGroup, FALSE, DEM_READINESS_DATACONTEXT_PID41, DestinationBuffer); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_SUPPORT_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Support_Implementation.h
 *********************************************************************************************************************/

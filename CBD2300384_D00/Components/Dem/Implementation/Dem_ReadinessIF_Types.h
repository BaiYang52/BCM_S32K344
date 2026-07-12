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
/*! \addtogroup Dem_ReadinessIF
 *  \{
 *  \file       Dem_ReadinessIF_Types.h
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

#if !defined (DEM_READINESSIF_TYPES_H)
#define DEM_READINESSIF_TYPES_H

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
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/
 /*!
* \defgroup  Dem_Readiness_DataContext  Macros encoding values indicating the currently processing data context
* Macros encoding values indicating the currently processing data context
* \{
*/
#define DEM_READINESS_DATACONTEXT_PID01                    (0x01U)  /*!< Data context indicating handling of PID01 */
#define DEM_READINESS_DATACONTEXT_DIDF501                  (0x02U)  /*!< Data context indicating handling of DIDF501 */
#define DEM_READINESS_DATACONTEXT_PID41                    (0x03U)  /*!< Data context indicating handling of PID41 */
#define DEM_READINESS_DATACONTEXT_DM05                     (0x04U)  /*!< Data context indicating handling of DM05 */
/*!
* \}
*/
 
                                             /* PID01 PID41 DIDF501 DM05 Readiness */
 /* ------------------------------------------------------------------------- */

 /*!
 * \defgroup  Dem_Readiness_Initialize  Macros encoding initial values
 * Macros encoding initial values depending on engine type
 * \{
 */
 /*!< Readiness initialization for spark engines */
#define DEM_READINESS_INITIALIZE_UPDATE_SPARK                  ( ((uint32)1 << DEM_CFG_READINESS_CAT ) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_HTCAT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_EVAP)  \
                                                               | ((uint32)1 << DEM_CFG_READINESS_SECAIR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_O2SENS) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_O2SENSHT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_CMPRCMPT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_EGR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_FLSYS_NONCONT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_MISF) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_ECS) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_PMFLT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_PCV) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_VVT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_DOR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_CSER) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_OTHER) )

 /*!< Readiness initialization for compression engines */
#define DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION            ( ((uint32)1 << DEM_CFG_READINESS_HCCAT ) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_NOXCAT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_BOOSTPR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_PMFLT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_EGSENS) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_CMPRCMPT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_EGR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_FLSYS_NONCONT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_MISF) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_ECS) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_PCV) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_VVT) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_DOR) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_CSER) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_NOXADSORB) \
                                                               | ((uint32)1 << DEM_CFG_READINESS_OTHER) )
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_PID_Support_State  Macros encoding assignment of readiness group supported state for PID01 and PID41
 * Macros encoding assignment of readiness group supported state for PID01 and PID41 to 8-bit mask
 * \{
 */
#define DEM_READINESS_PID_SUPPORT_NONE_MASK               (0x00U)             /*!< Used to initialize local variables */

 /* Byte B */
#define DEM_READINESS_PID_SUPPORT_MISF_MASK               (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_MISF supported state */
#define DEM_READINESS_PID_SUPPORT_FLSYS_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT supported state */
#define DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK           (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT supported state */
#define DEM_READINESS_PID_SUPPORT_COMPRESSION_MASK        (0x08U)             /*!< Bit encoding for engine type compression ignition supported state */

 /* Byte C */
#define DEM_READINESS_PID_SUPPORT_CAT_MASK                (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CAT supported state */
#define DEM_READINESS_PID_SUPPORT_HCCAT_MASK              (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT supported state */
#define DEM_READINESS_PID_SUPPORT_HTCAT_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT supported state */
#define DEM_READINESS_PID_SUPPORT_NOXCAT_MASK             (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_NOXCAT supported state */
#define DEM_READINESS_PID_SUPPORT_EVAP_MASK               (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP supported state */
#define DEM_READINESS_PID_SUPPORT_SECAIR_MASK             (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_PID_SUPPORT_BOOSTPR_MASK            (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR supported state */
#define DEM_READINESS_PID_SUPPORT_O2SENS_MASK             (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENS supported state */
#define DEM_READINESS_PID_SUPPORT_EGSENS_MASK             (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS supported state */
#define DEM_READINESS_PID_SUPPORT_O2SENSHT_MASK           (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENSHT supported state */
#define DEM_READINESS_PID_SUPPORT_PMFLT_MASK              (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT supported state */
#define DEM_READINESS_PID_SUPPORT_EGR_MASK                (0x80U)             /*!< Bit encoding for DEM_OBD_RDY_ERG supported state */

 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Support_State Macros map readiness group supported state to PID01 and PID41 byte
 * Macros encoding mapping of readiness group supported state to PID01 and PID41 byte
 * \{
 */
#define DEM_READINESS_PID_SUPPORT_NONE_BYTE               (0x00U)             /*!< Used to initialze local variables */

 /* Byte B */
#define DEM_READINESS_PID_SUPPORT_MISF_BYTE               (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_MISF supported state */
#define DEM_READINESS_PID_SUPPORT_FLSYS_BYTE              (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT supported state */
#define DEM_READINESS_PID_SUPPORT_CMPRCMPT_BYTE           (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_CMPRCMPT supported state */
#define DEM_READINESS_PID_SUPPORT_COMPRESSION_BYTE        (0x01U)             /*!< Byte encoding for engine type compression ignition supported state */

 /* Byte C */
#define DEM_READINESS_PID_SUPPORT_CAT_BYTE                (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_CAT supported state */
#define DEM_READINESS_PID_SUPPORT_HCCAT_BYTE              (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_HCCAT supported state */
#define DEM_READINESS_PID_SUPPORT_HTCAT_BYTE              (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_HTCAT supported state */
#define DEM_READINESS_PID_SUPPORT_NOXCAT_BYTE             (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_NOXCAT supported state */
#define DEM_READINESS_PID_SUPPORT_EVAP_BYTE               (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_EVAP supported state */
#define DEM_READINESS_PID_SUPPORT_SECAIR_BYTE             (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_PID_SUPPORT_BOOSTPR_BYTE            (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_BOOSTPR supported state */
#define DEM_READINESS_PID_SUPPORT_O2SENS_BYTE             (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_O2SENS supported state */
#define DEM_READINESS_PID_SUPPORT_EGSENS_BYTE             (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_EGSENS supported state */
#define DEM_READINESS_PID_SUPPORT_O2SENSHT_BYTE           (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_O2SENSHT supported state */
#define DEM_READINESS_PID_SUPPORT_PMFLT_BYTE              (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_PMFLT supported state */
#define DEM_READINESS_PID_SUPPORT_EGR_BYTE                (0x02U)             /*!< Byte encoding for DEM_OBD_RDY_ERG supported state */

 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_PID_Support_State  Macros encoding assignment of readiness group completion state for PID01 and PID41
 * Macros encoding assignment of readiness group completion state for PID01 and PID41 to 8-bit mask
 * \{
 */
#define DEM_READINESS_PID_COMPLETION_NONE_MASK               (0x00U)             /*!< Used to initialze local variables */

 /* Byte B */
#define DEM_READINESS_PID_COMPLETION_MISF_MASK               (0x10U)             /*!< Bit encoding for DEM_OBD_RDY_MISF completion state */
#define DEM_READINESS_PID_COMPLETION_FLSYS_MASK              (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT completion state */
#define DEM_READINESS_PID_COMPLETION_CMPRCMPT_MASK           (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT completion state */

 /* Byte D */
#define DEM_READINESS_PID_COMPLETION_CAT_MASK                (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CAT completion state */
#define DEM_READINESS_PID_COMPLETION_HCCAT_MASK              (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT completion state */
#define DEM_READINESS_PID_COMPLETION_HTCAT_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT completion state */
#define DEM_READINESS_PID_COMPLETION_NOXCAT_MASK             (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_NOXCAT completion state */
#define DEM_READINESS_PID_COMPLETION_EVAP_MASK               (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP completion state */
#define DEM_READINESS_PID_COMPLETION_SECAIR_MASK             (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR completion state */
#define DEM_READINESS_PID_COMPLETION_BOOSTPR_MASK            (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR completion state */
#define DEM_READINESS_PID_COMPLETION_O2SENS_MASK             (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENS completion state */
#define DEM_READINESS_PID_COMPLETION_EGSENS_MASK             (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS completion state */
#define DEM_READINESS_PID_COMPLETION_O2SENSHT_MASK           (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENSHT completion state */
#define DEM_READINESS_PID_COMPLETION_PMFLT_MASK              (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT completion state */
#define DEM_READINESS_PID_COMPLETION_EGR_MASK                (0x80U)             /*!< Bit encoding for DEM_OBD_RDY_ERG completion state */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Support_State Macros map readiness group completion state to PID01 and PID41 byte
 * Macros encoding mapping of readiness group completion state to PID01 and PID41 byte
 * \{
 */
#define DEM_READINESS_PID_COMPLETION_NONE_BYTE               (0x00U)             /*!< Used to initialze local variables */

 /* Byte B */
#define DEM_READINESS_PID_COMPLETION_MISF_BYTE               (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_MISF completion state */
#define DEM_READINESS_PID_COMPLETION_FLSYS_BYTE              (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT completion state */
#define DEM_READINESS_PID_COMPLETION_CMPRCMPT_BYTE           (0x01U)             /*!< Byte encoding for DEM_OBD_RDY_CMPRCMPT completion state */

 /* Byte D */
#define DEM_READINESS_PID_COMPLETION_CAT_BYTE                (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_CAT completion state */
#define DEM_READINESS_PID_COMPLETION_HCCAT_BYTE              (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_HCCAT completion state */
#define DEM_READINESS_PID_COMPLETION_HTCAT_BYTE              (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_HTCAT completion state */
#define DEM_READINESS_PID_COMPLETION_NOXCAT_BYTE             (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_NOXCAT completion state */
#define DEM_READINESS_PID_COMPLETION_EVAP_BYTE               (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_EVAP completion state */
#define DEM_READINESS_PID_COMPLETION_SECAIR_BYTE             (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_SECAIR completion state */
#define DEM_READINESS_PID_COMPLETION_BOOSTPR_BYTE            (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_BOOSTPR completion state */
#define DEM_READINESS_PID_COMPLETION_O2SENS_BYTE             (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_O2SENS completion state */
#define DEM_READINESS_PID_COMPLETION_EGSENS_BYTE             (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_EGSENS completion state */
#define DEM_READINESS_PID_COMPLETION_O2SENSHT_BYTE           (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_O2SENSHT completion state */
#define DEM_READINESS_PID_COMPLETION_PMFLT_BYTE              (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_PMFLT completion state */
#define DEM_READINESS_PID_COMPLETION_EGR_BYTE                (0x03U)             /*!< Byte encoding for DEM_OBD_RDY_ERG completion state */

 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Pid01_Bytes Macros mapping bytes of PID01 for writing to PID01 buffer
 * Macros mapping bytes of PID01 for writing to PID01 buffer
 * \{
 */
#define DEM_READINESS_PID01_BYTEA                              (0x00U)             /*!< Byte 0/A of PID01 buffer */
#define DEM_READINESS_PID01_BYTEB                              (0x01U)             /*!< Byte 1/B of PID01 buffer */
#define DEM_READINESS_PID01_BYTEC                              (0x02U)             /*!< Byte 2/C of PID01 buffer */
#define DEM_READINESS_PID01_BYTED                              (0x03U)             /*!< Byte 3/D of PID01 buffer */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Pid41_Bytes Macros mapping bytes of PID41 for writing to PID41 buffer
 * Macros mapping bytes of PID41 for writing to PID41 buffer
 * \{
 */
#define DEM_READINESS_PID41_BYTEA                              (0x00U)             /*!< Byte 0/A of PID41 buffer */
#define DEM_READINESS_PID41_BYTEB                              (0x01U)             /*!< Byte 1/B of PID41 buffer */
#define DEM_READINESS_PID41_BYTEC                              (0x02U)             /*!< Byte 2/C of PID41 buffer */
#define DEM_READINESS_PID41_BYTED                              (0x03U)             /*!< Byte 3/D of PID41 buffer */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Pid41_Special_Bytes Macros mapping special bytes of PID41 for writing to PID41 buffer
 * Macros mapping special bytes of PID41 for writing to PID41 buffer
 * \{
 */
#define DEM_READINESS_PID41_SUPPORT_BYTEB      (DEM_READINESS_PID_SUPPORT_MISF_MASK | DEM_READINESS_PID_SUPPORT_FLSYS_MASK | DEM_READINESS_PID_SUPPORT_CMPRCMPT_MASK) /* Supported bit mask for PID41 Byte B */
#define DEM_READINESS_PID41_COMPLETION_SPARK_BYTED          (0xEFU)                /*!< Static completion state for spark engines in Byte D for PID41 */
#define DEM_READINESS_PID41_COMPLETION_COMPRESSION_BYTED    (0xEBU)                /*!< Static completion state for compression engines in Byte D for PID41 */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_DIDF501_Support_State  Macros encoding assignment of readiness group supported state
 * Macros encoding assignment of readiness group supported state to 8-bit mask
 * \{
 */
 /* Byte B */
#define DEM_READINESS_DIDF501_SUPPORT_MISF_MASK               (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_MISF supported state */
#define DEM_READINESS_DIDF501_SUPPORT_FLSYS_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT supported state */
#define DEM_READINESS_DIDF501_SUPPORT_CMPRCMPT_MASK           (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT supported state */
#define DEM_READINESS_DIDF501_SUPPORT_ECS_MASK                (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_ECS supported state */
 /* Byte C */
#define DEM_READINESS_DIDF501_SUPPORT_CAT_MASK                (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CAT supported state */
#define DEM_READINESS_DIDF501_SUPPORT_HCCAT_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT supported state */
#define DEM_READINESS_DIDF501_SUPPORT_HTCAT_MASK              (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT supported state */
#define DEM_READINESS_DIDF501_SUPPORT_NOXCAT_MASK             (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_NOXCAT supported state */
 /* Byte D */
#define DEM_READINESS_DIDF501_SUPPORT_EVAP_MASK               (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP supported state */
#define DEM_READINESS_DIDF501_SUPPORT_SECAIR_MASK             (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_DIDF501_SUPPORT_BOOSTPR_MASK            (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR supported state */
#define DEM_READINESS_DIDF501_SUPPORT_PMFLT_MASK              (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT supported state */
 /* Byte E */
#define DEM_READINESS_DIDF501_SUPPORT_EGSENS_MASK             (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS supported state */
#define DEM_READINESS_DIDF501_SUPPORT_PCV_MASK                (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_PCV supported state */
#define DEM_READINESS_DIDF501_SUPPORT_EGR_MASK                (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EGR supported state */
#define DEM_READINESS_DIDF501_SUPPORT_VVT_MASK                (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_VVT supported state */
 /* Byte F */
#define DEM_READINESS_DIDF501_SUPPORT_DOR_MASK                (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_DOR supported state */
#define DEM_READINESS_DIDF501_SUPPORT_CSER_MASK               (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_CSER supported state */
#define DEM_READINESS_DIDF501_SUPPORT_NOXADSORB_MASK          (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_NOXADSORB supported state */
#define DEM_READINESS_DIDF501_SUPPORT_OTHER_MASK              (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_OTHER supported state */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_Support_State Macros map readiness group to DIDF501 byte
 * Macros encoding mapping of readiness group to DIDF501 byte
 * \{
 */
 /* Byte B */
#define DEM_READINESS_DIDF501_MISF_BYTE               (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_MISF supported state */
#define DEM_READINESS_DIDF501_FLSYS_BYTE              (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT supported state */
#define DEM_READINESS_DIDF501_CMPRCMPT_BYTE           (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT supported state */
#define DEM_READINESS_DIDF501_ECS_BYTE                (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_ECS supported state */
 /* Byte C */
#define DEM_READINESS_DIDF501_CAT_BYTE                (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_CAT supported state */
#define DEM_READINESS_DIDF501_HCCAT_BYTE              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT supported state */
#define DEM_READINESS_DIDF501_HTCAT_BYTE              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT supported state */
#define DEM_READINESS_DIDF501_NOXCAT_BYTE             (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_NOXCAT supported state */
 /* Byte D */
#define DEM_READINESS_DIDF501_EVAP_BYTE               (0x03U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP supported state */
#define DEM_READINESS_DIDF501_SECAIR_BYTE             (0x03U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_DIDF501_BOOSTPR_BYTE            (0x03U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR supported state */
#define DEM_READINESS_DIDF501_PMFLT_BYTE              (0x03U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT supported state */
 /* Byte E */
#define DEM_READINESS_DIDF501_EGSENS_BYTE             (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS supported state */
#define DEM_READINESS_DIDF501_PCV_BYTE                (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_PCV supported state */
#define DEM_READINESS_DIDF501_EGR_BYTE                (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EGR supported state */
#define DEM_READINESS_DIDF501_VVT_BYTE                (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_VVT supported state */
 /* Byte F */
#define DEM_READINESS_DIDF501_DOR_BYTE                (0x05U)             /*!< Bit encoding for DEM_OBD_RDY_DOR supported state */
#define DEM_READINESS_DIDF501_CSER_BYTE               (0x05U)             /*!< Bit encoding for DEM_OBD_RDY_CSER supported state */
#define DEM_READINESS_DIDF501_NOXADSORB_BYTE          (0x05U)             /*!< Bit encoding for DEM_OBD_RDY_NOXADSORB supported state */
#define DEM_READINESS_DIDF501_OTHER_BYTE              (0x05U)             /*!< Bit encoding for DEM_OBD_RDY_OTHER supported state */
 /*!
 * \}
 */

 /*!
 * \defgroup  Dem_Readiness_DIDF501_Bytes Macros mapping bytes of DIDF501 for writing to buffer
 * Macros mapping bytes of DIDF501 for writing to buffer
 * \{
 */
#define DEM_READINESS_DIDF501_BYTEA                    (0x00U)             /*!< Byte 0/A of DIDF501 buffer */
#define DEM_READINESS_DIDF501_BYTEB                    (0x01U)             /*!< Byte 1/B of DIDF501 buffer */
#define DEM_READINESS_DIDF501_BYTEC                    (0x02U)             /*!< Byte 2/C of DIDF501 buffer */
#define DEM_READINESS_DIDF501_BYTED                    (0x03U)             /*!< Byte 3/D of DIDF501 buffer */
#define DEM_READINESS_DIDF501_BYTEE                    (0x04U)             /*!< Byte 4/E of DIDF501 buffer */
#define DEM_READINESS_DIDF501_BYTEF                    (0x05U)             /*!< Byte 5/F of DIDF501 buffer */

 /*!
 * \}
 */


/*!
* \defgroup  Dem_Readiness_DM05_Support_State  Macros encoding assignment of readiness group supported state
* Macros encoding assignment of readiness group supported state to 8-bit mask
* \{
*/
/* Byte 4 */
#define DEM_READINESS_DM05_SUPPORT_MISF_MASK               (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_MISF supported state */
#define DEM_READINESS_DM05_SUPPORT_FLSYS_MASK              (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT supported state */
#define DEM_READINESS_DM05_SUPPORT_CMPRCMPT_MASK           (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT supported state */
#define DEM_READINESS_DM05_COMPLETENESS_MISF_MASK          (0x10U)             /*!< Bit encoding for DEM_OBD_RDY_MISF completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_FLSYS_MASK         (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_CMPRCMPT_MASK      (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT completeness status */
/* Byte 5,7 */
#define DEM_READINESS_DM05_CAT_MASK                        (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CAT supported and completeness state */
#define DEM_READINESS_DM05_HTCAT_MASK                      (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT supported and completeness state */
#define DEM_READINESS_DM05_EVAP_MASK                       (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP supported and completeness state */
#define DEM_READINESS_DM05_SECAIR_MASK                     (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR supported and completeness state */
#define DEM_READINESS_DM05_AC_MASK                         (0x10U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR supported and completeness state */
#define DEM_READINESS_DM05_EGSENS_MASK                     (0x20U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS supported and completeness state */
#define DEM_READINESS_DM05_EGHSENS_MASK                    (0x40U)             /*!< Bit encoding for DEM_OBD_RDY_EGHSENS supported and completeness state */
#define DEM_READINESS_DM05_EGRVVT_MASK                     (0x80U)             /*!< Bit encoding for DEM_OBD_RDY_EGR/DEM_OBD_RDY_VVT supported and completeness state */
/* Byte 6,8 */
#define DEM_READINESS_DM05_CSAS_MASK                       (0x01U)             /*!< Bit encoding for DEM_OBD_RDY_CSAS supported and completeness state */
#define DEM_READINESS_DM05_BOOSTPR_MASK                    (0x02U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR supported and completeness state */
#define DEM_READINESS_DM05_PMFLT_MASK                      (0x04U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT supported and completeness state */
#define DEM_READINESS_DM05_NOX_MASK                        (0x08U)             /*!< Bit encoding for DEM_OBD_RDY_NOXADSORB/DEM_OBD_RDY_NOXCAT supported and completeness state */
#define DEM_READINESS_DM05_HCCAT_MASK                      (0x10U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT supported and completeness state */
/*!
* \}
*/

/*!
* \defgroup  Dem_Readiness_Support_State Macros map readiness group to DM05 byte
* Macros encoding mapping of readiness group to DM05 byte
* \{
*/
/* Byte 4 */
#define DEM_READINESS_DM05_BYTE4                           (0x00U)             /*!< Byte 4 of DM 05 data block */
#define DEM_READINESS_DM05_MISF_BYTE                       (0x00U)             /*!< Byte position for DEM_OBD_RDY_MISF Readiness Group */
#define DEM_READINESS_DM05_FLSYS_BYTE                      (0x00U)             /*!< Byte position for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT Readiness Group */
#define DEM_READINESS_DM05_CMPRCMPT_BYTE                   (0x00U)             /*!< Byte position for DEM_OBD_RDY_CMPRCMPT Readiness Group*/
#define DEM_READINESS_DM05_ECS_BYTE                        (0x00U)             /*!< Byte position for DEM_OBD_RDY_ECS Readiness Group */
/* Byte 5 */
#define DEM_READINESS_DM05_BYTE5                           (0x01U)             /*!< Byte 5 of DM 05 data block */
#define DEM_READINESS_DM05_SUPPORT_CAT_BYTE                (0x01U)             /*!< Byte position for DEM_OBD_RDY_CAT supported state */
#define DEM_READINESS_DM05_SUPPORT_HTCAT_BYTE              (0x01U)             /*!< Byte position for DEM_OBD_RDY_HTCAT supported state */
#define DEM_READINESS_DM05_SUPPORT_EVAP_BYTE               (0x01U)             /*!< Byte position for DEM_OBD_RDY_EVAP supported state */
#define DEM_READINESS_DM05_SUPPORT_SECAIR_BYTE             (0x01U)             /*!< Byte position for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_DM05_SUPPORT_AC_BYTE                 (0x01U)             /*!< Byte position for DEM_OBD_RDY_SECAIR supported state */
#define DEM_READINESS_DM05_SUPPORT_EGSENS_BYTE             (0x01U)             /*!< Byte position for DEM_OBD_RDY_EGSENS supported state */
#define DEM_READINESS_DM05_SUPPORT_EGHSENS_BYTE            (0x01U)             /*!< Byte position for DEM_OBD_RDY_EGHSENS supported state */
#define DEM_READINESS_DM05_SUPPORT_EGRVVT_BYTE             (0x01U)             /*!< Byte position for DEM_OBD_RDY_EGR/DEM_OBD_RDY_VVT supported state */
/* Byte 6 */
#define DEM_READINESS_DM05_BYTE6                           (0x02U)             /*!< Byte 6 of DM 05 data block */
#define DEM_READINESS_DM05_SUPPORT_CSAS_BYTE               (0x02U)             /*!< Byte position for DEM_OBD_RDY_CSAS supported state */
#define DEM_READINESS_DM05_SUPPORT_BOOSTPR_BYTE            (0x02U)             /*!< Byte position for DEM_OBD_RDY_BOOSTPR supported state */
#define DEM_READINESS_DM05_SUPPORT_PMFLT_BYTE              (0x02U)             /*!< Byte position for DEM_OBD_RDY_PMFLT supported state */
#define DEM_READINESS_DM05_SUPPORT_NOX_BYTE                (0x02U)             /*!< Byte position for DEM_OBD_RDY_NOXADSORB/DEM_OBD_RDY_NOXCAT supported state */
#define DEM_READINESS_DM05_SUPPORT_HCCAT_BYTE              (0x02U)             /*!< Byte position for DEM_OBD_RDY_HCCAT supported state */
/* Byte 7 */
#define DEM_READINESS_DM05_BYTE7                           (0x03U)             /*!< Byte 7 of DM 05 data block */
#define DEM_READINESS_DM05_COMPLETENESS_CAT_BYTE           (0x03U)             /*!< Byte position for DEM_OBD_RDY_CAT completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_HTCAT_BYTE         (0x03U)             /*!< Byte position for DEM_OBD_RDY_HTCAT completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_EVAP_BYTE          (0x03U)             /*!< Byte position for DEM_OBD_RDY_EVAP completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_SECAIR_BYTE        (0x03U)             /*!< Byte position for DEM_OBD_RDY_SECAIR completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_AC_BYTE            (0x03U)             /*!< Byte position for DEM_OBD_RDY_SECAIR completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_EGSENS_BYTE        (0x03U)             /*!< Byte position for DEM_OBD_RDY_EGSENS completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_EGHSENS_BYTE       (0x03U)             /*!< Byte position for DEM_OBD_RDY_EGHSENS completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_EGRVVT_BYTE        (0x03U)             /*!< Byte position for DEM_OBD_RDY_EGR/DEM_OBD_RDY_VVT completeness status */
/* Byte 8 */
#define DEM_READINESS_DM05_BYTE8                           (0x04U)             /*!< Byte 8 of DM 05 data block */
#define DEM_READINESS_DM05_COMPLETENESS_CSAS_BYTE          (0x04U)             /*!< Byte position for DEM_OBD_RDY_CSAS completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_BOOSTPR_BYTE       (0x04U)             /*!< Byte position for DEM_OBD_RDY_BOOSTPR completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_PMFLT_BYTE         (0x04U)             /*!< Byte position for DEM_OBD_RDY_PMFLT completeness status*/
#define DEM_READINESS_DM05_COMPLETENESS_NOX_BYTE           (0x04U)             /*!< Byte position for DEM_OBD_RDY_NOXADSORB/DEM_OBD_RDY_NOXCAT completeness status */
#define DEM_READINESS_DM05_COMPLETENESS_HCCAT_BYTE         (0x04U)             /*!< Byte position for DEM_OBD_RDY_HCCAT completeness status */
/*!
* \}
*/


 /*!
 * \defgroup  Dem_Readiness_Disabled_State  Macros encoding assignment of readiness group disabled state
 * Macros encoding assignment of readiness group disabled state to 16-bit mask
 * \{
 */
#define DEM_READINESS_DISABLED_NONE_MASK               (0x0000U)             /*!< Used to initialize local variables */
#define DEM_READINESS_DISABLED_MISF_MASK               (0x0001U)             /*!< Bit encoding for DEM_OBD_RDY_MISF disabled state */
#define DEM_READINESS_DISABLED_FLSYS_MASK              (0x0002U)             /*!< Bit encoding for DEM_OBD_RDY_FLSYS and DEM_OBD_RDY_FLSYS_NONCONT disabled state */
#define DEM_READINESS_DISABLED_CMPRCMPT                (0x0004U)             /*!< Bit encoding for DEM_OBD_RDY_CMPRCMPT disabled state */
#define DEM_READINESS_DISABLED_CAT_MASK                (0x0008U)             /*!< Bit encoding for DEM_OBD_RDY_CAT disabled state */
#define DEM_READINESS_DISABLED_HCCAT_MASK              (0x0010U)             /*!< Bit encoding for DEM_OBD_RDY_HCCAT disabled state */
#define DEM_READINESS_DISABLED_HTCAT_MASK              (0x0020U)             /*!< Bit encoding for DEM_OBD_RDY_HTCAT disabled state */
#define DEM_READINESS_DISABLED_NOXCAT_MASK             (0x0040U)             /*!< Bit encoding for DEM_OBD_RDY_NOXCAT disabled state */
#define DEM_READINESS_DISABLED_EVAP_MASK               (0x0080U)             /*!< Bit encoding for DEM_OBD_RDY_EVAP disabled state */
#define DEM_READINESS_DISABLED_SECAIR_MASK             (0x0100U)             /*!< Bit encoding for DEM_OBD_RDY_SECAIR disabled state */
#define DEM_READINESS_DISABLED_BOOSTPR_MASK            (0x0200U)             /*!< Bit encoding for DEM_OBD_RDY_BOOSTPR disabled state */
#define DEM_READINESS_DISABLED_O2SENS_MASK             (0x0400U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENS disabled state */
#define DEM_READINESS_DISABLED_EGSENS_MASK             (0x0800U)             /*!< Bit encoding for DEM_OBD_RDY_EGSENS disabled state */
#define DEM_READINESS_DISABLED_O2SENSHT_MASK           (0x1000U)             /*!< Bit encoding for DEM_OBD_RDY_O2SENSHT disabled state */
#define DEM_READINESS_DISABLED_PMFLT_MASK              (0x2000U)             /*!< Bit encoding for DEM_OBD_RDY_PMFLT disabled state */
#define DEM_READINESS_DISABLED_EGR_MASK                (0x4000U)             /*!< Bit encoding for DEM_OBD_RDY_ERG disabled state */
#define DEM_READINESS_DISABLED_COMPRESSION             (0x8000U)             /*!< Bit encoding for engine type compression ignition disabled state */
 /*!
 * \}
 */

 /*! Array Size of a PID 01 Data Block */
#define DEM_READINESS_PID01_SIZE                (4u)  

 /*! Array Size of a PID 41 Data Block */
#define DEM_READINESS_PID41_SIZE                (4u)  
 
 /*! Array Size of a PID F501 Data Block */
#define DEM_READINESS_DIDF501_SIZE              (6u)   

/*! Array Size of a DM 05 Data Block (Bytes 4-8 only) */
#define DEM_READINESS_DM05_SIZE                 (5u)

 /*! Word size for SetBitInBitfield operations. Calculated by ceil(DEM_CFG_READINESS_GROUP_SIZE/32u)*/
#define DEM_CFG_READINESS_GROUP_WORDSIZE        (1u)   

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* DEM_READINESSIF_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ReadinessIF_Types.h
 *********************************************************************************************************************/

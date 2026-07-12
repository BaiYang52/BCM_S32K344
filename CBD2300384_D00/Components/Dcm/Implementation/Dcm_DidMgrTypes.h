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
/**        \file  Dcm_DidMgrTypes.h
 *         \unit  DidMgr
 *        \brief  Contains native types and forward declarations of DID Manager unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_DIDMGRTYPES_H)
# define DCM_DIDMGRTYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Types.h"
# if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* DID operation types - derive from the extern filter API */
#  define DCM_DIDMGR_OP_READ                                         DCM_DID_OP_READ
#  define DCM_DIDMGR_OP_WRITE                                        DCM_DID_OP_WRITE
#  define DCM_DIDMGR_OP_IO                                           DCM_DID_OP_IO
#  define DCM_DIDMGR_OP_SCALINGINFO                                  DCM_DID_OP_SCALINGINFO
#  define DCM_DIDMGR_OP_DEFINE                                       DCM_DID_OP_DEFINE
#  define DCM_DIDMGR_OP_ROE                                          DCM_DID_OP_ROE
#  define DCM_DIDMGR_OP_CODING                                       DCM_DID_OP_CODING

/*! DID Service types */
#  define DCM_DIDMGR_SVC_NONE                                        (0x00u)
#  define DCM_DIDMGR_SVC_READ                                        (0x22u)
#  define DCM_DIDMGR_SVC_WRITE                                       (0x2Eu)
#  define DCM_DIDMGR_SVC_IOCTRL                                      (0x2Fu)
#  define DCM_DIDMGR_SVC_SCALING                                     (0x24u)
#  define DCM_DIDMGR_SVC_DEFINE                                      (0x2Cu)

/* DID operation types - bitmapped which gives the application the opportunity to "OR" multiple operations per DID */
#  define DCM_DID_OP_READ                                            ((Dcm_DidOpType)0x01u)
#  define DCM_DID_OP_WRITE                                           ((Dcm_DidOpType)0x02u)
#  define DCM_DID_OP_IO                                              ((Dcm_DidOpType)0x04u)
#  define DCM_DID_OP_SCALINGINFO                                     ((Dcm_DidOpType)0x08u)
#  define DCM_DID_OP_DEFINE                                          ((Dcm_DidOpType)0x10u)
/*! Not used directly!
#  define DCM_DID_OP_IS_AVAILABLE                                    ((Dcm_DidOpType)0x20u)
*/
#  define DCM_DID_OP_ROE                                             ((Dcm_DidOpType)0x40u)
#  define DCM_DID_OP_CODING                                          ((Dcm_DidOpType)0x80u)

/* Service port operation types */
#  define DCM_DIDMGR_OPTYPE_READ                                     ((Dcm_DidMgrOpCallMemType)0x01u)
#  define DCM_DIDMGR_OPTYPE_READLENGTH                               ((Dcm_DidMgrOpCallMemType)0x02u)
#  define DCM_DIDMGR_OPTYPE_READCHKCOND                              ((Dcm_DidMgrOpCallMemType)0x04u)
#  define DCM_DIDMGR_OPTYPE_RANGEREAD                                DCM_DIDMGR_OPTYPE_READ
#  define DCM_DIDMGR_OPTYPE_RANGEREADLENGTH                          DCM_DIDMGR_OPTYPE_READLENGTH

#  define DCM_DIDMGR_OPTYPE_WRITE                                    ((Dcm_DidMgrOpCallMemType)0x01u)
#  define DCM_DIDMGR_OPTYPE_RANGEWRITE                               DCM_DIDMGR_OPTYPE_WRITE

/* this value shall be for both OP_READ and OP_WRITE OP_TYPES the same! */
#  define DCM_DIDMGR_OPTYPE_RANGEISDIDAVAILABLE                      ((Dcm_DidMgrOpCallMemType)0x10u)

#  define DCM_DIDMGR_OPTYPE_GETSCALING                               ((Dcm_DidMgrOpCallMemType)0x01u)

#  define DCM_DIDMGR_OPTYPE_DEFINE                                   ((Dcm_DidMgrOpCallMemType)0x01u)

#  define DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU                           ((Dcm_DidMgrOpCallMemType)0x01u)
#  define DCM_DIDMGR_OPTYPE_IO_RST2DEF                               ((Dcm_DidMgrOpCallMemType)0x02u)
#  define DCM_DIDMGR_OPTYPE_IO_FRZCURSTATE                           ((Dcm_DidMgrOpCallMemType)0x04u)
#  define DCM_DIDMGR_OPTYPE_IO_SHRTTRMADJ                            ((Dcm_DidMgrOpCallMemType)0x08u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_DIDMGRTYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_DidMgrTypes.h
 *********************************************************************************************************************/

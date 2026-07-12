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
/**        \file  Dcm_PagedBuffer.h
 *         \unit  PagedBuffer
 *        \brief  Contains public inline function definitions of PagedBuffer unit.
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
#if !defined(DCM_PAGEDBUFFER_H)
# define DCM_PAGEDBUFFER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_PagedBufferTypes.h"
# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DCM_PAGEDBUFFER_STATE_INACTIVE                             (Dcm_PagedBufferStateType)0x00u /*!< No paged-buffer transmission in progress */
#  define DCM_PAGEDBUFFER_STATE_ACTIVATED                            (Dcm_PagedBufferStateType)0x01u /*!< Paged-buffer transmission activated, but not yet passed to the PduR */
#  define DCM_PAGEDBUFFER_STATE_CANCELED                             (Dcm_PagedBufferStateType)0x02u /*!< Paged-buffer transmission terminated prior passing it to the PduR */
#  define DCM_PAGEDBUFFER_STATE_ONTX                                 (Dcm_PagedBufferStateType)0x03u /*!< Paged-buffer transmission ongoing (passed to the PduR) */

#  define DCM_PAGEDBUFFER_ALL_DATA_PROVIDED                          (Dcm_PagedBufferDataProvisionStateType)0x00u /*!< All data provided */
#  define DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED                         (Dcm_PagedBufferDataProvisionStateType)0x01u /*!< Already some data provided */
#  define DCM_PAGEDBUFFER_WAIT_FOR_DATA                              (Dcm_PagedBufferDataProvisionStateType)0x02u /*!< Waiting for data provision */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_PagedBufferStateType;
typedef uint8 Dcm_PagedBufferDataProvisionStateType;

/*! Paged-buffer control data */
struct DCM_PAGEDBUFFERCONTEXTTYPE_TAG
{
  Dcm_MsgLenType                        RemainingLen;                 /*!< Keeps track on the data exchange progress */
  Dcm_CfgNetBufferSizeMemType           WriteIndex;                   /*!< Pointer to the next free buffer position ready to be written with new data portion */
  Dcm_CfgNetBufferSizeMemType           ReadIndex;                    /*!< Pointer to the next ready-to-send buffer position */
  Dcm_PagedBufferStateType              State;                        /*!< Paged-buffer state machine */
  Dcm_CfgNetBufferRefMemType            BufferHdl;                    /*!< Zero based reference to the USDT buffer object in use */
  uint8                                 UpdateFuncRef;                /*!< Zero based reference to the data provider handler Xxx_<Name>Update */
  uint8                                 CancelFuncRef;                /*!< Zero based reference to the data provider cancellation notification Xxx_<Name>Cancel */
  boolean                               UpdateFinished;               /*!< Data provider will no longer be called (job done) */
  Dcm_PagedBufferDataProvisionStateType UpdaterDataProvisionState;    /*!< Data provision state of Updater */
  Dcm_OpStatusType                      OpStatus;                     /*!< Current operation status */
};
typedef struct DCM_PAGEDBUFFERCONTEXTTYPE_TAG Dcm_PagedBufferContextType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_PAGED_BUFFER_ENABLED == STD_ON) */
#endif /* !defined(DCM_PAGEDBUFFER_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_PagedBuffer.h
 *********************************************************************************************************************/

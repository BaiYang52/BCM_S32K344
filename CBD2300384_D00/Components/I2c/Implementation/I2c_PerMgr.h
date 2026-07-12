/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*        \file  I2c_PerMgr.h
 *        \brief  I2c peripheral manager
 *
 *      \details  This sub-module controls the specific I2c peripherals of the target platform.
 *                It takes requests for simple data transfers from the higher layers and executes them on the specific
 *                hardware. The result of a data transfer which depends on successful transmission or a detected error
 *                event is reported back to the layers above. In addition, the module offers mechanisms for the
 *                handling of error states to the layers above.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author      Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-05-09  virmid      -         Initial release of the I2c driver.
 *  01.00.01  2017-11-02  virmid ESCAN00097256  Compiler error: New section opened before closing the former one.
 *  01.00.02  2019-05-21  virmid ESCAN00102048  I2c driver causes an endless interrupt loop.
 *  01.01.00  2019-12-12  virmid ESCAN00105060  I2c driver remains stuck in endless loop during a WRITE transmission.
 *  01.01.01  2019-12-19  virmid ESCAN00105247  MISRA deviation: MISRA-C:2004 Rule 11.3
 *  02.00.00  2021-07-19  fbatz  PS-514         Update I2c driver to new core version 5.
 *********************************************************************************************************************/

#if !defined (I2C_PERMGR_H)
# define I2C_PERMGR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "I2c_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define I2C_SW_MAJOR_VERSION                 (2u)
# define I2C_SW_MINOR_VERSION                 (0u)
# define I2C_SW_PATCH_VERSION                 (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * I2c_PerMgr_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function.
 *  \details     This function initializes the module I2c_PerMgr for the specified hardware unit. It initializes
 *               required variables and enables the hardware unit.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_Init(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_DeInit()
 *********************************************************************************************************************/
/*! \brief       De-Initialization function.
 *  \details     This function de-initializes the module I2c_PerMgr for the specified hardware unit.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_DeInit(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_IsHwUnitIdle()
 *********************************************************************************************************************/
/*! \brief       Checks is the hardware unit idles.
 *  \details     This function is used to check if the hardware unit idles and is available for a transfer.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \return      TRUE - The hardware unit is idle.
 *  \return      FALSE - The hardware unit is busy.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, I2C_CODE) I2c_PerMgr_IsHwUnitIdle(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_IrqHandler()
 *********************************************************************************************************************/
/*! \brief       Interrupt service routine.
 *  \details     This function processes the transmissions in the interrupt context. It is responsible for sending the
 *               header frames and also handles the read from / write to the data buffers.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        The function gets called cyclically in interrupt context.
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_IrqHandler(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main function.
 *  \details     This function performs timeout monitoring. In case the timeout has expired.
 *  \param[in]   hwUnitId               Id of the hardware unit
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_MainFunction(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_FreeBus()
 *********************************************************************************************************************/
/*! \brief       Frees the bus
 *  \details     This function frees the bus in case a the data line (SDA) is stock LOW by sending nine clock pulses.
 *               It is called by the upper-layer sub-modules in case a timeout is detected and timeout handling is set
 *               to 'I2C_RECURRENT'.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_FreeBus(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_ConfigHwUnit()
 *********************************************************************************************************************/
/*! \brief       Configures the hardware unit.
 *  \details     This function is used by upper-layer sub-modules to configure the baud-rate and the timeout value for
 *               the I2c peripheral. This function is called every time before a sequence is transferred.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \param[in]   baudrate               Baudrate to set for specified hardware unit.
 *  \param[in]   timeout                Time after which transmission should be considered as failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_PerMgr_ConfigHwUnit(I2c_HwUnitType hwUnitId, I2c_BaudrateType baudrate, I2c_TimeoutType timeout);

/**********************************************************************************************************************
 * I2c_PerMgr_TransData()
 *********************************************************************************************************************/
/*! \brief       Initiates a data transfer.
 *  \details     This function is called by the high-level to initiate a data transfer. The transfer is parameterized
 *               via a hardware-unit specific RAM which is set by the upper-layer sub-modules.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         Hardware-unit is not busy.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
 FUNC(void, I2C_CODE) I2c_PerMgr_TransData(I2c_HwUnitType hwUnitId);

/**********************************************************************************************************************
 * I2c_PerMgr_DetachBus()
 *********************************************************************************************************************/
/*! \brief       Detaches the peripheral from the bus.
 *  \details     This function restarts the I2c peripheral to detach from the bus. It is called by the upper-layer
 *               sub-modules in case:
 *                 - the transmission of the current sequence is cancelled (via API).
 *                 - an arbitration failure is detected and the handling is set to 'I2C_BURST'.
 *                 - a bus failure is detected.
 *  \param[in]   hwUnitId               Id of the hardware unit.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
 FUNC(void, I2C_CODE) I2c_PerMgr_DetachBus(I2c_HwUnitType hwUnitId);

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* I2C_PERMGR_H */

/**********************************************************************************************************************
 *  END OF FILE: Msn.h
 *********************************************************************************************************************/

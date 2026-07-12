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
/*!        \file  I2c_Regs.h
 *        \brief  Register header of the vHyperBus driver
 *
 *      \details  Defines macros and data types representing and abstracting the register layout of the
 *                I2C Interface.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (I2C_REGS_H)
# define      I2C_REGS_H

/**********************************************************************************************************************
 *  HARDWARE SOFTWARE INTERFACE (HSI)
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

/* Register address offsets */
# define I2C_REG_OFFSET_VERID                 0x0000u  /*!< Version ID Register */
# define I2C_REG_OFFSET_PARAM                 0x0004u  /*!< Parameter Register */
# define I2C_REG_OFFSET_MCR                   0x0010u  /*!< Master Control Register */
# define I2C_REG_OFFSET_MSR                   0x0014u  /*!< Master Status Register */
# define I2C_REG_OFFSET_MIER                  0x0018u  /*!< Master Interrupt Enable Register */
# define I2C_REG_OFFSET_MDER                  0x001Cu  /*!< Master DMA Enable Register */
# define I2C_REG_OFFSET_MCFGR0                0x0020u  /*!< Master Configuration Register 0 */
# define I2C_REG_OFFSET_MCFGR1                0x0024u  /*!< Master Configuration Register 1 */
# define I2C_REG_OFFSET_MCFGR2                0x0028u  /*!< Master Configuration Register 2 */
# define I2C_REG_OFFSET_MCFGR3                0x002Cu  /*!< Master Configuration Register 3 */
# define I2C_REG_OFFSET_MDMR                  0x0040u  /*!< Master Data Match Register */
# define I2C_REG_OFFSET_MCCR0                 0x0048u  /*!< Master Clock Configuration Register 0 */
# define I2C_REG_OFFSET_MCCR1                 0x0050u  /*!< Master Clock Configuration Register 1 */
# define I2C_REG_OFFSET_MFCR                  0x0058u  /*!< Master FIFO Control Register */
# define I2C_REG_OFFSET_MFSR                  0x005Cu  /*!< Master FIFO Status Register */
# define I2C_REG_OFFSET_MTDR                  0x0060u  /*!< Master Transmit Data Register */
# define I2C_REG_OFFSET_MRDR                  0x0070u  /*!< Master Receive Data Register */
# define I2C_REG_OFFSET_SCR                   0x0110u  /*!< Slave Control Register */
# define I2C_REG_OFFSET_SSR                   0x0114u  /*!< Slave Status Register */
# define I2C_REG_OFFSET_SIER                  0x0118u  /*!< Slave Interrupt Enable Register */
# define I2C_REG_OFFSET_SDER                  0x011Cu  /*!< Slave DMA Enable Register */
# define I2C_REG_OFFSET_SCFGR1                0x0124u  /*!< Slave Configuration Register 0 */
# define I2C_REG_OFFSET_SCFGR2                0x0128u  /*!< Slave Configuration Register 1 */
# define I2C_REG_OFFSET_SAMR                  0x0140u  /*!< Slave Address Match Register */
# define I2C_REG_OFFSET_SASR                  0x0150u  /*!< Slave Address Status Register */
# define I2C_REG_OFFSET_STAR                  0x0154u  /*!< Slave Transmit ACK Register */
# define I2C_REG_OFFSET_STDR                  0x0160u  /*!< Slave Transmit Data Register */
# define I2C_REG_OFFSET_SRDR                  0x0170u  /*!< Slave Receive Data Register */

/* Register masks */
/* Master Control Register */
# define I2C_REG_MASK_MCR_RRF                 0x00000200u  /*!< Reset Receive FIFO */
# define I2C_REG_MASK_MCR_RTF                 0x00000100u  /*!< Reset Transmit FIFO */
# define I2C_REG_MASK_MCR_DBGEN               0x00000008u  /*!< Debug Enable */
# define I2C_REG_MASK_MCR_RST                 0x00000002u  /*!< Software Reset */
# define I2C_REG_MASK_MCR_MEN                 0x00000001u  /*!< Master Enable */

/* Master Status Register */
# define I2C_REG_MASK_MSR_ALL_FLAGS           0x30007F03u  /*!< All status flags */
# define I2C_REG_MASK_MSR_BBF                 0x02000000u  /*!< Bus Busy Flag */
# define I2C_REG_MASK_MSR_MBF                 0x01000000u  /*!< Master Busy Flag */
# define I2C_REG_MASK_MSR_PLTF                0x00002000u  /*!< Pin Low Timeout Flag */
# define I2C_REG_MASK_MSR_FEF                 0x00001000u  /*!< FIFO Error Flag */
# define I2C_REG_MASK_MSR_ALF                 0x00000800u  /*!< Arbitration Lost Flag */
# define I2C_REG_MASK_MSR_NDF                 0x00000400u  /*!< NACK Detect Flag */
# define I2C_REG_MASK_MSR_SDF                 0x00000200u  /*!< STOP Detect Flag */
# define I2C_REG_MASK_MSR_EPIE                0x00000100u  /*!< End Packet Flag */
# define I2C_REG_MASK_MSR_RDF                 0x00000002u  /*!< Receive Data Flag */
# define I2C_REG_MASK_MSR_TDF                 0x00000001u  /*!< Transmit Data Flag */

/* Master Interrupt Enable Register */
# define I2C_REG_MASK_MIER_PLTIE              0x00002000u  /*!< Pin Low Timeout Interrupt Enable */
# define I2C_REG_MASK_MIER_FEIE               0x00001000u  /*!< FIFO Error Interrupt Enable */
# define I2C_REG_MASK_MIER_ALF                0x00000800u  /*!< Arbitration Lost Interrupt Enable */
# define I2C_REG_MASK_MIER_NDIE               0x00000400u  /*!< NACK Detect Interrupt Enable  */
# define I2C_REG_MASK_MIER_SDIE               0x00000200u  /*!< STOP Detect Interrupt Enable */
# define I2C_REG_MASK_MIER_EPIE               0x00000100u  /*!< End Packet Interrupt Enable */
# define I2C_REG_MASK_MIER_RDIE               0x00000002u  /*!< Receive Data Interrupt Enable */
# define I2C_REG_MASK_MIER_TDIE               0x00000001u  /*!< Transmit Data Interrupt Enable */
# define I2C_REG_MASK_MIER_ALL_IRQS           0x00007F03u  /*!< All interrupts */
# define I2C_REG_MASK_MIER_DISABLE_ALL        0x00000000u  /*!< Disable all interrupts */

/* Master Configuration Register 1 */
# define I2C_REG_MASK_MCFGR1_PRESCALE         0x00000007u /*!< Clock prescaler for master logic. */
# define I2C_REG_MASK_MCFGR1_PRESCALE1        0x00000000u /*!< Clock prescaler for master logic. Divide by 1. */
# define I2C_REG_MASK_MCFGR1_PRESCALE2        0x00000001u /*!< Clock prescaler for master logic. Divide by 2. */
# define I2C_REG_MASK_MCFGR1_PRESCALE4        0x00000002u /*!< Clock prescaler for master logic. Divide by 4. */
# define I2C_REG_MASK_MCFGR1_PRESCALE8        0x00000003u /*!< Clock prescaler for master logic. Divide by 8. */
# define I2C_REG_MASK_MCFGR1_PRESCALE16       0x00000004u /*!< Clock prescaler for master logic. Divide by 16. */
# define I2C_REG_MASK_MCFGR1_PRESCALE32       0x00000005u /*!< Clock prescaler for master logic. Divide by 32. */
# define I2C_REG_MASK_MCFGR1_PRESCALE64       0x00000006u /*!< Clock prescaler for master logic. Divide by 64. */
# define I2C_REG_MASK_MCFGR1_PRESCALE128      0x00000007u /*!< Clock prescaler for master logic. Divide by 128. */

/* Master FIFO Control Register */
# define I2C_REG_MASK_MCFGR1_RXWATER          0x00030000u /*!< Receive FIFO Watermark */

/* Master Receive Data Register */
# define I2C_REG_MASK_MRDR_RXEMPTY            0x00004000u /*<! RX Empty */

/* Master Transmit Data Register */
# define I2C_REG_MASK_MTDR_CMD_TRANSMIT                     (0x0uL << 8u) /*!< Transmit DATA[7:0]. */
# define I2C_REG_MASK_MTDR_CMD_RECEIVE                      (0x1uL << 8u) /*!< Receive (Data[7:0] + 1) bytes. */
# define I2C_REG_MASK_MTDR_CMD_GEN_STOP                     (0x2uL << 8u) /*!< Generate STOP condition. */
# define I2C_REG_MASK_MTDR_CMD_RECEIVE_DISCARD              (0x3uL << 8u) /*!< Receive and discard (DATA[7:0] +1) bytes. */
# define I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR         (0x4uL << 8u) /*!< Generate (repeated) START and transmit address in DATA[7:0]. */
# define I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR_NACK    (0x5uL << 8u) /*!< Generate (repeated) START and transmit address in DATA[7:0]. This transfer expects a NACK to be returned. */
# define I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR_HS      (0x6uL << 8u) /*!< Generate (repeated) START and transmit address in DATA[7:0] using high speed mode. */
# define I2C_REG_MASK_MTDR_CMD_START_AND_TRANS_ADDR_HS_NACK (0x7uL << 8u)/*!< Generate (repeated) START and transmit address in DATA[7:0] using high speed mode. This transfer expects a NACK to be returned. */

#endif /* I2C_REGS_H */

/**********************************************************************************************************************
 *  END OF FILE: I2c_Regs.h
 *********************************************************************************************************************/

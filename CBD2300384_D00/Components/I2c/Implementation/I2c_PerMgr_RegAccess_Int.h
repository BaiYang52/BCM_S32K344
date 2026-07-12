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
/*!        \file  I2c_PerMgr_RegAccess_Int.h
 *        \brief  Register access abstraction of I2c driver.
 *
 *      \details  -*
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

#if !defined (I2C_PERMGR_REGACCESS_INT_H)
# define I2C_PERMGR_REGACCESS_INT_H
typedef  I2c_HwUnitType I2c_PerMgr_HwUnitIdType;
/* PRQA S 0779 EOF */ /* MD_MSR_5.1_779 */


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# if (I2C_PROTECTED_MODE == STD_ON)
#  include "Os.h"
# endif /* I2C_PERMGR_USE_PERIPHERAL_ACCESS_API */

/**********************************************************************************************************************
 *  GLOBAL TYPES
 *********************************************************************************************************************/
/*! Width of a register */
typedef uint32 I2c_PerMgr_RegWidthType;
/*! Offset within the register space of the I2c */
typedef uint32 I2c_PerMgr_RegOffsetType;
/*! Register access type */
typedef volatile I2c_PerMgr_RegWidthType I2c_PerMgr_RegAccessType;
/*! Pointer to a register */
typedef P2VAR(I2c_PerMgr_RegAccessType, AUTOMATIC, MSR_REGSPACE) I2c_PerMgr_RegPtrType;
typedef P2CONST(I2c_PerMgr_RegAccessType, AUTOMATIC, MSR_REGSPACE) I2c_PerMgr_ConstRegPtrType;

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if !defined (I2C_PERMGR_REG_ACCESS_INLINE) /* COV_I2C_PERMGR_COMPATIBILITY */
#  define I2C_PERMGR_REG_ACCESS_INLINE static INLINE
# endif /* I2C_PERMGR_REG_ACCESS_INLINE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (I2C_PROTECTED_MODE == STD_ON)
/**********************************************************************************************************************
 *  I2c_PerMgr_GetPeripheralRegion
 *********************************************************************************************************************/
/*! \brief        Retrieves the peripheral region of the given I2c hardware unit.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \return       Peripheral region.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(Os_PeripheralIdType, I2C_CODE) I2c_PerMgr_GetPeripheralRegion
(
    I2c_PerMgr_HwUnitIdType hwUnitId
);
# endif
/**********************************************************************************************************************
 *  I2c_PerMgr_GetRegBaseAddress
 *********************************************************************************************************************/
/*! \brief        Retrieves the register base address of the given I2c hardware unit.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \return       Register base address.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(uint32, I2C_CODE) I2c_PerMgr_GetRegBaseAddress
(
    I2c_PerMgr_HwUnitIdType hwUnitId
);

# if (I2C_PROTECTED_MODE == STD_OFF)
/**********************************************************************************************************************
 *  I2c_PerMgr_CreateRegPtr
 *********************************************************************************************************************/
/*! \brief        Creates a pointer to the addressed register by the offset into the I2c register space.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \return       Pointer to the register addressed.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(I2c_PerMgr_RegPtrType, I2C_CODE) I2c_PerMgr_CreateRegPtr
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset
);
# endif

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_Write
 *********************************************************************************************************************/
/*! \brief        Writes the given value to the addressed register.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \param[in]    regValue   Value to be written to the register.
 *  \context      ANY
 *  \reentrant    TRUE for different registers
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_Write
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType regValue
);

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_Read
 *********************************************************************************************************************/
/*! \brief        Reads the value of the addressed register
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \return       Value of the register.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(I2c_PerMgr_RegWidthType, I2C_CODE) I2c_PerMgr_Reg_Read
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset
);

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_WriteBits
 *********************************************************************************************************************/
/*! \brief        Writes the given value to the specified bit group.
 *  \details      -
 *  \param[in]    hwUnitId      Id of the hardware unit.
 *  \param[in]    regOffset     Offset in bytes into the I2c register space.
 *  \param[in]    bitMask       Bit mask to apply on register value.
 *  \param[in]    bitsValue     Value to be written to the register.
 *  \return       Value of the register.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_WriteBits
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask,
    I2c_PerMgr_RegWidthType bitsValue
);

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_SetBitMask
 *********************************************************************************************************************/
/*! \brief        Sets the bits given by the bit mask in the addressed register.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \param[in]    bitMask    Bits to be set.
 *  \context      ANY
 *  \reentrant    TRUE for different registers.
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_SetBitMask
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
);

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_ClearBitMask
 *********************************************************************************************************************/
/*! \brief        Clears the bits given by the bit mask in the addressed register.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \param[in]    bitMask    Bits to be cleared.
 *  \context      ANY
 *  \reentrant    TRUE for different registers.
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_ClearBitMask
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
);

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_IsBitMaskSet
 *********************************************************************************************************************/
/*! \brief        Checks if all of the bits given by the bit mask are set in the addressed register.
 *  \details      -
 *  \param[in]    hwUnitId   Id of the hardware unit.
 *  \param[in]    regOffset  Offset in bytes into the I2c register space.
 *  \param[in]    bitMask    Bits to be checked for being set.
 *  \return       FALSE - At least one bit of the bit mask isn't set in the register.
 *  \return       TRUE - All bits of the bit mask are set in the register.
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \pre          -
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(boolean, I2C_CODE) I2c_PerMgr_Reg_IsBitMaskSet
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  I2c_PerMgr_GetRegBaseAddress
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(uint32, I2C_CODE) I2c_PerMgr_GetRegBaseAddress /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId
)
{
  return I2c_ConfigPtr->HwCfgPtr[hwUnitId].UnitBaseAddr;
} /* I2c_PerMgr_GetRegBaseAddress() */

# if (I2C_PROTECTED_MODE == STD_ON)
/**********************************************************************************************************************
 *  I2c_PerMgr_GetPeripheralRegion
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(Os_PeripheralIdType, I2C_CODE) I2c_PerMgr_GetPeripheralRegion /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId
)
{
  return I2c_ConfigPtr->HwCfgPtr[hwUnitId].OsAreaReg;
} /* I2c_PerMgr_GetPeripheralRegion() */
# endif

# if (I2C_PROTECTED_MODE == STD_OFF)
/**********************************************************************************************************************
 *  I2c_PerMgr_CreateRegPtr
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(I2c_PerMgr_RegPtrType, I2C_CODE) I2c_PerMgr_CreateRegPtr /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset
)
{
  return (I2c_PerMgr_RegPtrType)(I2c_PerMgr_GetRegBaseAddress(hwUnitId) + regOffset); /* PRQA S 0303 */ /* MD_I2c_PerMgr_0303 */
} /* I2c_PerMgr_CreateRegPtr() */
# endif

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_Write
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */

I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_Write /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType regValue
)
{
# if (I2C_PROTECTED_MODE == STD_ON)
  osWritePeripheral32(I2c_PerMgr_GetPeripheralRegion(hwUnitId), regOffset + I2c_PerMgr_GetRegBaseAddress(hwUnitId), regValue);
# else
  I2c_PerMgr_RegPtrType reg = I2c_PerMgr_CreateRegPtr(hwUnitId, regOffset);
  *reg = regValue;
# endif
} /* I2c_PerMgr_Reg_Write() */

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_Read
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(I2c_PerMgr_RegWidthType, I2C_CODE) I2c_PerMgr_Reg_Read /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset
)
{
# if (I2C_PROTECTED_MODE == STD_ON)
  return osReadPeripheral32(I2c_PerMgr_GetPeripheralRegion(hwUnitId), regOffset + I2c_PerMgr_GetRegBaseAddress(hwUnitId));
# else
  I2c_PerMgr_ConstRegPtrType reg = I2c_PerMgr_CreateRegPtr(hwUnitId, regOffset);
  return *reg;
# endif
} /* I2c_PerMgr_Reg_Read() */

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_WriteBits
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_WriteBits  /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask,
    I2c_PerMgr_RegWidthType bitsValue
)
{
  I2c_PerMgr_RegWidthType regValue = I2c_PerMgr_Reg_Read(hwUnitId, regOffset);
  I2c_PerMgr_Reg_Write(hwUnitId, regOffset, (regValue & ~bitMask) | (bitsValue & bitMask));
} /* I2c_PerMgr_Reg_WriteBits() */

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_SetBitMask
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_SetBitMask /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
)
{
  I2c_PerMgr_Reg_Write(hwUnitId, regOffset, I2c_PerMgr_Reg_Read(hwUnitId, regOffset) | bitMask);
} /* I2c_PerMgr_Reg_SetBitMask() */

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_ClearBitMask
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
I2C_PERMGR_REG_ACCESS_INLINE FUNC(void, I2C_CODE) I2c_PerMgr_Reg_ClearBitMask /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
)
{
  I2c_PerMgr_Reg_Write(hwUnitId, regOffset, I2c_PerMgr_Reg_Read(hwUnitId, regOffset) & ~bitMask);
} /* I2c_PerMgr_Reg_ClearBitMask() */

/**********************************************************************************************************************
 *  I2c_PerMgr_Reg_IsBitMaskSet
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */

I2C_PERMGR_REG_ACCESS_INLINE FUNC(boolean, I2C_CODE) I2c_PerMgr_Reg_IsBitMaskSet /* PRQA S 3219 */ /* MD_I2c_PerMgr_3219 */
(
    I2c_PerMgr_HwUnitIdType hwUnitId,
    I2c_PerMgr_RegOffsetType regOffset,
    I2c_PerMgr_RegWidthType bitMask
)
{
  boolean result = FALSE;
  if((I2c_PerMgr_Reg_Read(hwUnitId, regOffset) & bitMask) == bitMask)
  {
    result = TRUE;
  }
  return result;
} /* I2c_PerMgr_Reg_IsBitMaskSet() */

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* I2C_PERMGR_REGACCESS_INT_H */

/* Justification for module-specific MISRA deviations:
MD_I2c_PerMgr_3219:
  Reason:     This function is inlined and therefore it has to be implemented here. The function is not used
              by all implementation files which include this header file.
  Risk:       None.
  Prevention: None.

 MD_I2c_PerMgr_0303:
    Reason:     Hardware register accesses need a cast from integral type to a pointer to volatile object.
    Risk:       There is no risk as the register is mapped to the respective memory address.
    Prevention: Covered by code review.
*/

/* START_COVERAGE_JUSTIFICATION

Code coverage:

Variant coverage:

\ID COV_I2C_PERMGR_COMPATIBILITY
 \ACCEPT TX
 \REASON COV_MSR_COMPATIBILITY

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: I2c_PerMgr_RegAccess_Int.h
 *********************************************************************************************************************/


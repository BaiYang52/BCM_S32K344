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
/**        \file  _vItaHlp_MemMap.h
 *        \brief  Memory section definitions used by vItaHlp.
 *
 *      \details  This file is a template for vItaHlp_MemMap.h.
 *                This file has to be extended with the memory section which are used.
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

/**********************************************************************************************************************
 *  VITAHLP START
 *********************************************************************************************************************/

/*******  CODE sections **********************************************************************************************/

#ifdef VITAHLP_START_SEC_CODE
# undef VITAHLP_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef VITAHLP_STOP_SEC_CODE
# undef VITAHLP_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

/*******  CONST sections  ********************************************************************************************/

/* CONST sections */

#ifdef VITAHLP_START_SEC_CONST_8BIT
# undef VITAHLP_START_SEC_CONST_8BIT
# define START_SEC_CONST_8BIT
#endif
#ifdef VITAHLP_STOP_SEC_CONST_8BIT
# undef VITAHLP_STOP_SEC_CONST_8BIT
# define STOP_SEC_CONST
#endif

#ifdef VITAHLP_START_SEC_CONST_16BIT
# undef VITAHLP_START_SEC_CONST_16BIT
# define START_SEC_CONST_16BIT
#endif
#ifdef VITAHLP_STOP_SEC_CONST_16BIT
# undef VITAHLP_STOP_SEC_CONST_16BIT
# define STOP_SEC_CONST
#endif

#ifdef VITAHLP_START_SEC_CONST_32BIT
# undef VITAHLP_START_SEC_CONST_32BIT
# define START_SEC_CONST_32BIT
#endif
#ifdef VITAHLP_STOP_SEC_CONST_32BIT
# undef VITAHLP_STOP_SEC_CONST_32BIT
# define STOP_SEC_CONST
#endif

#ifdef VITAHLP_START_SEC_CONST_UNSPECIFIED
# undef VITAHLP_START_SEC_CONST_UNSPECIFIED
# define START_SEC_CONST_UNSPECIFIED
#endif
#ifdef VITAHLP_STOP_SEC_CONST_UNSPECIFIED
# undef VITAHLP_STOP_SEC_CONST_UNSPECIFIED
# define STOP_SEC_CONST
#endif

/*******  VAR sections  **********************************************************************************************/

/* VAR GLOBALSHARED sections */

#ifdef VITAHLP_START_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# undef VITAHLP_START_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# define OS_START_SEC_GLOBALSHARED_VAR_UNSPECIFIED
#endif
#ifdef VITAHLP_STOP_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# undef VITAHLP_STOP_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# define OS_STOP_SEC_GLOBALSHARED_VAR_UNSPECIFIED
#endif

/* VAR INIT sections */

#ifdef VITAHLP_START_SEC_VAR_INIT_UNSPECIFIED
# undef VITAHLP_START_SEC_VAR_INIT_UNSPECIFIED
# define START_SEC_VAR_INIT_UNSPECIFIED
#endif

#ifdef VITAHLP_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef VITAHLP_STOP_SEC_VAR_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_INIT_8BIT
# undef VITAHLP_START_SEC_VAR_INIT_8BIT
# define START_SEC_VAR_INIT_8BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_INIT_8BIT
# undef VITAHLP_STOP_SEC_VAR_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_INIT_32BIT
# undef VITAHLP_START_SEC_VAR_INIT_32BIT
# define START_SEC_VAR_INIT_32BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_INIT_32BIT
# undef VITAHLP_STOP_SEC_VAR_INIT_32BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_INIT_16BIT
# undef VITAHLP_START_SEC_VAR_INIT_16BIT
# define START_SEC_VAR_INIT_16BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_INIT_16BIT
# undef VITAHLP_STOP_SEC_VAR_INIT_16BIT
# define STOP_SEC_VAR
#endif

/* VAR NOINIT sections */

#ifdef VITAHLP_START_SEC_VAR_NOINIT_BOOLEAN
# undef VITAHLP_START_SEC_VAR_NOINIT_BOOLEAN
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_NOINIT_BOOLEAN
# undef VITAHLP_STOP_SEC_VAR_NOINIT_BOOLEAN
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_NOINIT_8BIT
# undef VITAHLP_START_SEC_VAR_NOINIT_8BIT
# define START_SEC_VAR_NOINIT_8BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_NOINIT_8BIT
# undef VITAHLP_STOP_SEC_VAR_NOINIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_NOINIT_16BIT
# undef VITAHLP_START_SEC_VAR_NOINIT_16BIT
# define START_SEC_VAR_NOINIT_16BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_NOINIT_16BIT
# undef VITAHLP_STOP_SEC_VAR_NOINIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# define START_SEC_VAR_NOINIT_UNSPECIFIED
#endif
#ifdef VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

/* VAR ZERO INIT sections */

#ifdef VITAHLP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef VITAHLP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#endif
#ifdef VITAHLP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# undef VITAHLP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_ZERO_INIT_8BIT
# undef VITAHLP_START_SEC_VAR_ZERO_INIT_8BIT
# define START_SEC_VAR_ZERO_INIT_8BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_ZERO_INIT_8BIT
# undef VITAHLP_STOP_SEC_VAR_ZERO_INIT_8BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_ZERO_INIT_16BIT
# undef VITAHLP_START_SEC_VAR_ZERO_INIT_16BIT
# define START_SEC_VAR_ZERO_INIT_16BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_ZERO_INIT_16BIT
# undef VITAHLP_STOP_SEC_VAR_ZERO_INIT_16BIT
# define STOP_SEC_VAR
#endif

#ifdef VITAHLP_START_SEC_VAR_ZERO_INIT_32BIT
# undef VITAHLP_START_SEC_VAR_ZERO_INIT_32BIT
# define START_SEC_VAR_ZERO_INIT_32BIT
#endif
#ifdef VITAHLP_STOP_SEC_VAR_ZERO_INIT_32BIT
# undef VITAHLP_STOP_SEC_VAR_ZERO_INIT_32BIT
# define STOP_SEC_VAR
#endif

/**********************************************************************************************************************
 *  VITAHLP END
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  END OF FILE: _vItaHlp_MemMap.h
 *********************************************************************************************************************/

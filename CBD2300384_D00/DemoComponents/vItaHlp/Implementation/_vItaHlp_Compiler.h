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
/**        \file  _vItaHlp_Compiler.h
 *        \brief  Memory and variable qualifiers used by vItaHlp.
 *
 *      \details  This file is a template for the _vItaHlp_Compiler.h
 *                This file has to be extended with the memory and pointer classes which where used.
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

#ifndef VITAHLP_COMPILER_H
# define VITAHLP_COMPILER_H

/**********************************************************************************************************************
 *  VITAHLP START
 *********************************************************************************************************************/

# if defined (VITAHLP_VAR_NOINIT_8BIT)
# else
#  define VITAHLP_VAR_NOINIT_8BIT
# endif

# if defined (VITAHLP_VAR)
# else
#  define VITAHLP_VAR
# endif

# if defined (VITAHLP_VAR_NOINIT_UNSPECIFIED)
# else
#  define VITAHLP_VAR_NOINIT_UNSPECIFIED
# endif

# if defined (VITAHLP_VAR_NOINIT_16BIT)
# else
#  define VITAHLP_VAR_NOINIT_16BIT
# endif

# if defined (VITAHLP_CODE)
# else
#  define VITAHLP_CODE
# endif

# if defined (VITAHLP_CONST)
# else
#  define VITAHLP_CONST
# endif

# if defined (VITAHLP_VAR_ZERO_INIT)
# else
#  define VITAHLP_VAR_ZERO_INIT
# endif

# if defined (VITAHLP_VAR_NOINIT)
# else
#  define VITAHLP_VAR_NOINIT
# endif

# if defined (VITAHLP_APPL_VAR)
# else
#  define VITAHLP_APPL_VAR
# endif

/**********************************************************************************************************************
 *  VITAHLP END
 *********************************************************************************************************************/

#endif  /* VITAHLP_COMPILER_H */

/**********************************************************************************************************************
 *  END OF FILE: _vItaHlp_Compiler.h
 *********************************************************************************************************************/

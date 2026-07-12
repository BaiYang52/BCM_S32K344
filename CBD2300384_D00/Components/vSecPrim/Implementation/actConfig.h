/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actConfig.h
 *        \brief  configuration of actCLib.
 *
 *      \details Currently the actClib version is used. 
 *               This file is part of the embedded systems library actCLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ACTCONFIG_H
# define ACTCONFIG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "vSecPrim_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/****************************************************************************
 ** MontMul version switch
 ***************************************************************************/

# ifndef actBN_MONT_MUL_VERSION_ONE /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
/* one inner loop */
#  define actBN_MONT_MUL_VERSION                                      0
# else
/* two inner loops + prime structure optimizations (equals version 1 in this state) */
#  define actBN_MONT_MUL_VERSION                                      1
# endif

/****************************************************************************
 ** Modular exponentation configuration switch (BNModExp)
 **
 **  window size    temporary variables    improvement factor
 **  -----------    -------------------    ------------------
 **     1 (sqr-and-mult)      1                     1
 **     2                     2                     1.09
 **     3                     4                     1.16
 **     4                     8                     1.22
 **     5                    16                     1.26
 **     k                  2^(k-1)
 **
 ** NOTE:
 **  - Each temporay variable is of length:   modulus_length_in_digits + 1
 **  - The improvement factors are concerning the duration for a modular
 **    exponentiation without precomputation time, assuming a random
 **    exponent and no special algorithm for modular squaring.
 ** Window size:
 **   0 - variable k-bit window algorithm
 **   1 - fixed 1-bit window algorithm (sqr-and-mult)
 **   2 - fixed 2-bit window algorithm
 **   3 - fixed 3-bit window algorithm
 ***************************************************************************/
 /* default value changed by viseag during  SEC-789 (SafeBSW for actBigNum) */
# define actBN_MOD_EXP_WINDOW_SIZE                                    1u


# if ((actBN_MOD_EXP_WINDOW_SIZE)==0u) /* COV_VSECPRIM_NOT_CONFIGURABLE XF */
#  define actEC_MOD_EXP_WINDOW_SIZE                                   (3u)
#  define actRSA_MOD_EXP_WINDOW_SIZE                                  (3u)
# else
#  define actEC_MOD_EXP_WINDOW_SIZE                                   (actBN_MOD_EXP_WINDOW_SIZE)
#  define actRSA_MOD_EXP_WINDOW_SIZE                                  (actBN_MOD_EXP_WINDOW_SIZE)
# endif
# define actEC_MOD_EXP_TMP                                            (1u << ((actEC_MOD_EXP_WINDOW_SIZE)-1u))
# define actRSA_MOD_EXP_TMP                                           (1u << ((actRSA_MOD_EXP_WINDOW_SIZE)-1u))

/****************************************************************************
 ** SHA-1 / RIPEMD-160 / SHA-256 / AES switches
 ***************************************************************************/

/*--------------------------------------------------------------------------
   Please choose a code size optimization level for the SHA-1 module:
   -------------------------------------------------------------------------
   Example: actSHAUpdate() benchmarks and SHA-1 module code sizes.
    - Machine:   Pentium 4, 3 GHz, 1 GB RAM, Windows 2000
    - Compiler:  VS VC++ 6.0
   -------------------------------------------------------------------------
                  | timings (in MB/sec, | code size (in bytes,
        level     | optimize size)      | optimize size)
      ------------|---------------------|---------------------
           0      |        40           |    1131
           1      |        60           |    1033
           2      |        85           |    4198
           3      |       120           |    4790
   -----------------------------------------------------------------------*/
# define actSHA_SPEED_UP                                              (vSecPrim_GetSha1SpeedUpOfOptimization())
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
   Please choose a code size optimization level for the RIPEMD-160 module:
   -----------------------------------------------------------------------*/
# define actRMD160_SPEED_UP                                           1
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
   Please choose a code size optimization level for the SHA-256 module:
   -------------------------------------------------------------------------
   Example: actSHA256Update() benchmarks and SHA-256 module code sizes.
    - Machine:   Pentium III, 1 GHz, 256 MB RAM, Windows 2000
    - Compiler:  VS VC++ 6.0
   -------------------------------------------------------------------------
                  | timings (in MB/sec,  | code size (in bytes,
        level     | optimize size)       | optimize size)
      ------------|----------------------|---------------------
           0      |      16.8            |     894 + 256 byte K-Box
           1      |      29.0            |    3041 + 256 byte K-Box
   -----------------------------------------------------------------------*/
# define actSHA256_SPEED_UP                                           (vSecPrim_GetSha256SpeedUpOfOptimization())
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
   Please choose a code size optimization level for the SHA-512 module:
   -------------------------------------------------------------------------
   Example: actSHA512Update() benchmarks and SHA-512 module code sizes.
    - Machine:   Pentium III, 1 GHz, 256 MB RAM, Windows 2000
    - Compiler:  VS VC++ 6.0
   -------------------------------------------------------------------------
                  | timings (in MB/sec,  | code size (in bytes,
        level     | optimize size)       | optimize size)
      ------------|----------------------|---------------------
           0      |                      |
           1      |                      |
   -----------------------------------------------------------------------*/
# define actSHA512_SPEED_UP                                           (vSecPrim_GetSha512SpeedUpOfOptimization())
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
   Please choose a code size optimization level for the MD5 module:
   -------------------------------------------------------------------------
   Example: actMD5Update() benchmarks and MD5 module code sizes.
    - Machine:   Pentium III, 1 GHz, 256 MB RAM, Windows 2000
    - Compiler:  VS VC++ 6.0
   -------------------------------------------------------------------------
                  | timings (in MB/sec,  | code size (in bytes,
        level     | optimize size)       | optimize size)
      ------------|----------------------|---------------------
           0      |      ----            |    722 + 320 byte tables
           1      |      ----            |   1248 + 256 byte tables
           2      |      ----            |   2166
   -----------------------------------------------------------------------*/
# define actMD5_SPEED_UP                                              1
/*------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
    Speed up level 0 - 3 in AES module.
   -----------------------------------------------------------------------*/
# define actAES_SPEED_UP                                              (vSecPrim_GetAesSpeedUpOfOptimization())
/*--------------------------------------------------------------------------
   level 0, 1 :       1, 0 KB memory is required for look up tables
   level 2, 3 :       8, 5 KB memory is required for look up tables
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
    Speed up level 0 / 1 / 4 for GHASH module
    "1":  256 bytes RAM (workspace) +  32 bytes ROM for look up tables
    "4": 4096 bytes RAM (workspace) + 512 bytes ROM for look up tables

    ##  Please read the "library/es Porting Guide"
    ##  for more information!
   -----------------------------------------------------------------------*/
# define actGHASH_SPEED_UP                                            1

/*--------------------------------------------------------------------------
   Speed up level 0 - 2 in ChaCha20 module.
   -----------------------------------------------------------------------*/
# define actChaCha20_SPEED_UP                                         (vSecPrim_GetChaCha20SpeedUpOfOptimization())

/****************************************************************************
 ** watchdog functionality
 ***************************************************************************/

/*--------------------------------------------------------------------------
    watchdog call frequency
  -------------------------------------------------------------------------*/
# define actWD_LEVEL                                                  (vSecPrim_GetWatchdogLevelOfGeneral())

/*--------------------------------------------------------------------------
   level 0: WD is disabled
   level 1: WD called from outer functions only
   level 2: WD called from inner functions as well
   level 3: WD also called in inner hash loops
--------------------------------------------------------------------------*/

# define VSECPRIM_ACTBNEUCLID_ENABLED                                 STD_OFF


/****************************************************************************
 ** SECURITY STRENGTH FOR CTR DRBG
 ***************************************************************************/
# if (vSecPrim_GetCTRDRBGAES128SecurityStrengthOfGeneral() == VSECPRIM_SECURITY_STRENGTH_128)
#  define actAES128CTRDRBG_SECURITY_STRENGTH                          (128u)
# else
#  define actAES128CTRDRBG_SECURITY_STRENGTH                          (112u)
# endif

# define actAES256CTRDRBG_SECURITY_STRENGTH                           (256u)

# define actHASHDRBG_SECURITY_STRENGTH                                (256u)

#endif /* ACTCONFIG_H */

/**********************************************************************************************************************
 *  END OF FILE: actConfig.h
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  wdgm_verifier.c
 *        \brief  wdgm_verifier.c source file
 *
 *      \details  This is the implementation of the WdgM verifier writer
 *
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

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "WdgM.h"

#include "wdgm_verifier.h"
#include "utilities.h"
#include "edf_utilities.h"
#include "block_a_tests.h"
#include "block_b_tests.h"
#include "block_c_tests.h"

typedef struct
{
    const char    name[11]; /* function name */

    const char    pars;     /* bitfield for parameters */
    #define PAR_1x  0x01
    #define PAR_x2  0x02
    #define PAR_12  0x03
    union                   /* union of function pointers to test functions */
    {
        bool (*test12) (const WdgM_ConfigType*, const verifier_main_struct_t*);
        bool (*testx2) (const verifier_main_struct_t*);
        bool (*test1x) (const WdgM_ConfigType*);
    } f;
} test_function_t;
#define SIZEOF_TEST_FUNCTION_TABLE  (sizeof(test_functions)/sizeof(test_function_t))


static void preamble (const WdgM_ConfigType *main_struct)
{
    printf
        ("===============================================================================\n");
    printf
        ( "                 Watchdog Manager Configuration Verifier v.%d.%d.%d\n"
        , WDGM_VERIFIER_MAJOR_VERSION
        , WDGM_VERIFIER_MINOR_VERSION
        , WDGM_VERIFIER_PATCH_VERSION
        );
    printf
        ("===============================================================================\n\n");
    printf
        (   "                          Core ID: %d\n"
        , (int) main_struct->WdgMModeCoreAssignment
        );
    printf
        ("-------------------------------------------------------------------------------\n");
    printf
        ("Data according to the main structure in the C source file\n");
    printf
        (   "   WdgMExpiredSupervisionCycleTol: %d\n"
        , (int) main_struct->WdgMExpiredSupervisionCycleTol
        );
    printf ("                     WdgMConfigId: %d\n", (int) main_struct->WdgMConfigId);
    printf
        (   "          WdgMGlobInitialEntityId: %d\n"
        , (int) main_struct->WdgMGlobInitialEntityId
        );
    printf
        (   "      WdgMGlobInitialCheckpointId: %d\n"
        , (int) main_struct->WdgMGlobInitialCheckpointId
        );
    printf
        (   "               NrOfAllCheckpoints: %d\n"
        , countAllCPInSE (main_struct, false)
        );
    printf
        (   "              NrOfWatchdogDevices: %d\n"
        , (int) main_struct->NrOfWatchdogDevices
        );
    printf
        (   "           NrOfSupervisedEntities: %d\n"
        , number_of_se_in_mode (main_struct)
        );
    newline ();
}

test_function_t  test_functions[] =
{ { "Test 1    " , PAR_12, .f.test12 = test_001 }
, { "Test 2    " , PAR_12, .f.test12 = test_002 }
, { "Test 3    " , PAR_12, .f.test12 = test_003 }
, { "Test 4    " , PAR_12, .f.test12 = test_004 }
, { "Test 5    " , PAR_12, .f.test12 = test_005 }
, { "Test 17   " , PAR_12, .f.test12 = test_017 }
, { "Test 19   " , PAR_12, .f.test12 = test_019 }
, { "Test 20   " , PAR_12, .f.test12 = test_020 }
, { "Test 21   " , PAR_12, .f.test12 = test_021 }
, { "Test 22   " , PAR_12, .f.test12 = test_022 }
, { "Test 27   " , PAR_12, .f.test12 = test_027 }
, { "Test 28   " , PAR_12, .f.test12 = test_028 }
, { "Test 32   " , PAR_12, .f.test12 = test_032 }
, { "Test 33   " , PAR_12, .f.test12 = test_033 }
, { "Test 34   " , PAR_12, .f.test12 = test_034 }
, { "Test 35   " , PAR_12, .f.test12 = test_035 }
, { "Test 37   " , PAR_12, .f.test12 = test_037 }
, { "Test 38   " , PAR_12, .f.test12 = test_038 }
, { "Test 39   " , PAR_12, .f.test12 = test_039 }
, { "Test 40   " , PAR_12, .f.test12 = test_040 }
, { "Test 41   " , PAR_12, .f.test12 = test_041 }
, { "Test 42   " , PAR_12, .f.test12 = test_042 }
, { "Test 43   " , PAR_12, .f.test12 = test_043 }
, { "Test 44   " , PAR_12, .f.test12 = test_044 }
, { "Test 45   " , PAR_12, .f.test12 = test_045 }
, { "Test 46   " , PAR_12, .f.test12 = test_046 }
, { "Test 47   " , PAR_12, .f.test12 = test_047 }
, { "Test 85   " , PAR_12, .f.test12 = test_085 }
, { "Test 86   " , PAR_12, .f.test12 = test_086 }
, { "Test 89   " , PAR_12, .f.test12 = test_089 }
, { "Test 90   " , PAR_12, .f.test12 = test_090 }
, { "Test 91   " , PAR_12, .f.test12 = test_091 }
, { "Test 92   " , PAR_12, .f.test12 = test_092 }
, { "Test 93   " , PAR_12, .f.test12 = test_093 }
, { "Test 94   " , PAR_12, .f.test12 = test_094 }
, { "Test 95   " , PAR_12, .f.test12 = test_095 }
, { "Test 96   " , PAR_12, .f.test12 = test_096 }
, { "Test 97   " , PAR_12, .f.test12 = test_097 }
, { "Test 98   " , PAR_12, .f.test12 = test_098 }
, { "Test 99   " , PAR_12, .f.test12 = test_099 }
, { "Test 100  " , PAR_12, .f.test12 = test_100 }
, { "Test 101  " , PAR_12, .f.test12 = test_101 }
, { "Test 102  " , PAR_12, .f.test12 = test_102 }
, { "Test 103  " , PAR_12, .f.test12 = test_103 }
, { "Test 104  " , PAR_12, .f.test12 = test_104 }
, { "Test 105  " , PAR_12, .f.test12 = test_105 }
, { ""           , 0     , .f.test1x = NULL     }
, { "Test 18   " , PAR_1x, .f.test1x = test_018 }
, { "Test 23   " , PAR_1x, .f.test1x = test_023 }
, { "Test 24   " , PAR_1x, .f.test1x = test_024 }
, { "Test 25   " , PAR_1x, .f.test1x = test_025 }
, { "Test 26   " , PAR_1x, .f.test1x = test_026 }
, { "Test 29   " , PAR_1x, .f.test1x = test_029 }
, { "Test 30   " , PAR_1x, .f.test1x = test_030 }
, { "Test 31   " , PAR_1x, .f.test1x = test_031 }
, { "Test 48   " , PAR_12, .f.test12 = test_048 }
, { "Test 49   " , PAR_12, .f.test12 = test_049 }
, { "Test 50   " , PAR_12, .f.test12 = test_050 }
, { "Test 51   " , PAR_12, .f.test12 = test_051 }
, { "Test 52   " , PAR_12, .f.test12 = test_052 }
, { "Test 53   " , PAR_12, .f.test12 = test_053 }
//, { "Test 54   " , PAR_12, .f.test12 = test_054 }
, { "Test 55   " , PAR_12, .f.test12 = test_055 }
, { "Test 56   " , PAR_12, .f.test12 = test_056 }
, { "Test 57   " , PAR_12, .f.test12 = test_057 }
, { "Test 58   " , PAR_12, .f.test12 = test_058 }
, { "Test 59   " , PAR_12, .f.test12 = test_059 }
, { "Test 60   " , PAR_12, .f.test12 = test_060 }
//, { "Test 61   " , PAR_x2, .f.testx2 = test_061 }
, { "Test 62   " , PAR_x2, .f.testx2 = test_062 }
, { "Test 63   " , PAR_12, .f.test12 = test_063 }
, { "Test 64   " , PAR_12, .f.test12 = test_064 }
//, { "Test 65   " , PAR_12, .f.test12 = test_065 }
, { "Test 66   " , PAR_x2, .f.testx2 = test_066 }
, { "Test 67   " , PAR_1x, .f.test1x = test_067 }
, { "Test 87   " , PAR_12, .f.test12 = test_087 }
, { "Test 88   " , PAR_12, .f.test12 = test_088 }
, { "Test 107  " , PAR_12, .f.test12 = test_107 }
, { "Test 108  " , PAR_x2, .f.testx2 = test_108 }
, { "Test 109  " , PAR_12, .f.test12 = test_109 }
, { "Test 110  " , PAR_x2, .f.testx2 = test_110 }
, { "Test 112  " , PAR_x2, .f.testx2 = test_112 }
, { ""           , 0     , .f.test1x = NULL     }
, { "Test 6    " , PAR_12, .f.test12 = test_006 }
, { "Test 7    " , PAR_1x, .f.test1x = test_007 }
, { "Test 8    " , PAR_1x, .f.test1x = test_008 }
, { "Test 9    " , PAR_1x, .f.test1x = test_009 }
, { "Test 10   " , PAR_1x, .f.test1x = test_010 }
, { "Test 11   " , PAR_1x, .f.test1x = test_011 }
, { "Test 12   " , PAR_1x, .f.test1x = test_012 }
, { "Test 13   " , PAR_12, .f.test12 = test_013 }
, { "Test 14   " , PAR_12, .f.test12 = test_014 }
, { "Test 15   " , PAR_12, .f.test12 = test_015 }
, { "Test 16   " , PAR_12, .f.test12 = test_016 }
//, { "Test 36   " , PAR_12, .f.test12 = test_036 }
, { "Test 68   " , PAR_12, .f.test12 = test_068 }
, { "Test 69   " , PAR_12, .f.test12 = test_069 }
, { "Test 70   " , PAR_12, .f.test12 = test_070 }
, { "Test 71   " , PAR_12, .f.test12 = test_071 }
//, { "Test 72   " , PAR_12, .f.test12 = test_072 }
, { "Test 73   " , PAR_12, .f.test12 = test_073 }
, { "Test 74   " , PAR_1x, .f.test1x = test_074 }
, { "Test 75   " , PAR_12, .f.test12 = test_075 }
, { "Test 76   " , PAR_12, .f.test12 = test_076 }
, { "Test 77   " , PAR_12, .f.test12 = test_077 }
, { "Test 78   " , PAR_12, .f.test12 = test_078 }
, { "Test 79   " , PAR_12, .f.test12 = test_079 }
, { "Test 80   " , PAR_12, .f.test12 = test_080 }
, { "Test 81   " , PAR_12, .f.test12 = test_081 }
, { "Test 83   " , PAR_12, .f.test12 = test_083 }
, { "Test 84   " , PAR_12, .f.test12 = test_084 }
, { "Test 111  " , PAR_x2, .f.testx2 = test_111 }
};


bool summary [SIZEOF_TEST_FUNCTION_TABLE];

bool print_summary ()
{
    int i          = 0;
    bool big       = true;
    bool result    = true;
    bool subresult = true;


    printf ("+=========================+\n");
    printf ("|      S U M M A R Y      |\n");
    printf ("+============+============+\n");

    for (i = 0; i < SIZEOF_TEST_FUNCTION_TABLE; i++)
    {
        if (test_functions[i].name[0] == 0)
        {

            printf ("+============+============+\n");
            big = true;
        }

        else
        {
            if (!big)
            {
                printf ("|------------+------------|\n");
            }
            big = false;
            subresult = summary [i];
            printf ("| %10s | %10s |\n"
                   , test_functions[i].name
                   , subresult ? "  PASSED  " : "NOT PASSED"
                   );
            result = result && subresult;
        }

    }

    printf ("+============+============+\n");
    return result;
}

/* the main attraction - 'main_struct` should be a pointer
 * to the main structure in the generated code
 */
int verify
    ( const WdgM_ConfigType        * main_struct
    , const verifier_main_struct_t * verifier_info
    )
{
    int i;

    if ( !sanity_test(main_struct, verifier_info) )
    {
        printf ( "data is too damaged for any meaningful tests \n" );
        printf ( "--------------------------------------------------\n\n");
        printf ("Exiting !\n\n");
        return 1;
    }

    preamble (main_struct);

    if ( (verifier_info->version_info->major != WDGM_VERIFIER_MAJOR_VERSION)
      || (verifier_info->version_info->minor != WDGM_VERIFIER_MINOR_VERSION)
       )
    {
        newline (); newline ();
        printf ( "--------------------------------------------------\n");
        printf ( "Verifier Info Version Mismatch: %u.%02u != %u.%02u\n"
               , verifier_info->version_info->major
               , verifier_info->version_info->minor
               , WDGM_VERIFIER_MAJOR_VERSION
               , WDGM_VERIFIER_MINOR_VERSION
               );
        printf ( "--------------------------------------------------\n\n");
        printf ("Exiting !\n\n");
        return 1;
    }

    printf ( "--------------------------------------------------\n");
    printf ( "Verifier Info Version: %u.%u.%u\n"
           , verifier_info->version_info->major
           , verifier_info->version_info->minor
           , verifier_info->version_info->patch
           );
    printf ( "--------------------------------------------------\n\n");


    for (i = 0; i < SIZEOF_TEST_FUNCTION_TABLE; i++)
    {
        switch (test_functions[i].pars)
        {
            case PAR_1x:
            {
                summary [i] =
                    test_functions[i].f.test1x (main_struct);
                break;
            }
            case PAR_x2:
            {
                summary [i] =
                    test_functions[i].f.testx2 (verifier_info);
                break;
            }
            case PAR_12:
            {
                summary [i] =
                    test_functions[i].f.test12 (main_struct, verifier_info);
                break;
            }
        }
    }

    newline ();
    newline ();
    if (print_summary ())
    {
        printf ("All tests passed.\n");
        return 0;
    }
    else
    {
       printf ("NOT ALL TESTS PASSED.\n");
       return 1;
    }
}


void  print_WdgMVerifier_version( )
{
    fprintf
        ( stderr
        , "Watchdog Manager Configuration Verifier v.%d.%d.%d\n"
        , WDGM_VERIFIER_MAJOR_VERSION
        , WDGM_VERIFIER_MINOR_VERSION
        , WDGM_VERIFIER_PATCH_VERSION
        );
}


/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : generic
*   Dependencies         : 
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file WdgIf.c
*
*   @addtogroup WdgIf
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif




/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/

#include "WdgIf.h"

#if (USE_WDG_MODULE == STD_ON)

    #ifdef WDG_INSTANCE0
    #if (WDG_INSTANCE0 == STD_ON)
        #include "Wdg_43_Instance0.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE1
    #if (WDG_INSTANCE1 == STD_ON)
        #include "Wdg_43_Instance1.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE2
    #if (WDG_INSTANCE2 == STD_ON)
        #include "Wdg_43_Instance2.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE3
    #if (WDG_INSTANCE3 == STD_ON)
        #include "Wdg_43_Instance3.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE4
    #if (WDG_INSTANCE4 == STD_ON)
        #include "Wdg_43_Instance4.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE5
    #if (WDG_INSTANCE5 == STD_ON)
        #include "Wdg_43_Instance5.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE6
    #if (WDG_INSTANCE6 == STD_ON)
        #include "Wdg_43_Instance6.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE7
    #if (WDG_INSTANCE7 == STD_ON)
        #include "Wdg_43_Instance7.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE8
    #if (WDG_INSTANCE8 == STD_ON)
        #include "Wdg_43_Instance8.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE9
    #if (WDG_INSTANCE9 == STD_ON)
        #include "Wdg_43_Instance9.h"
    #endif
    #endif

    #ifdef WDG_INSTANCE10
    #if (WDG_INSTANCE10 == STD_ON)
        #include "Wdg_43_Instance10.h"
    #endif
    #endif
#endif

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/

#define WDGIF_VENDOR_ID_C                      43
#define WDGIF_AR_RELEASE_MAJOR_VERSION_C       4
#define WDGIF_AR_RELEASE_MINOR_VERSION_C       4
/** @violates @ref WdgIf_C_REF_1 The compiler/linker shall be checked to ensure that 31
    character significance and case sensitivity are supported for external identifiers. */
#define WDGIF_AR_RELEASE_REVISION_VERSION_C    0
#define WDGIF_SW_MAJOR_VERSION_C               2
#define WDGIF_SW_MINOR_VERSION_C               0
#define WDGIF_SW_PATCH_VERSION_C               1

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/

/* Check if current file and WDGIF header file are of the same vendor.*/
#if (WDGIF_VENDOR_ID_C != WDGIF_VENDOR_ID)
    #error "WdgIf.c and WdgIf.h have different vendor ids"
#endif

/* Check if current file and WDGIF header file are of the same Autosar version.*/
#if ((WDGIF_AR_RELEASE_MAJOR_VERSION_C    != WDGIF_AR_RELEASE_MAJOR_VERSION) || \
     (WDGIF_AR_RELEASE_MINOR_VERSION_C    != WDGIF_AR_RELEASE_MINOR_VERSION) || \
     (WDGIF_AR_RELEASE_REVISION_VERSION_C != WDGIF_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of WdgIf.c and WdgIf.h are different"
#endif

/* Check if current file and WDGIF header file are of the same Software version.*/
#if ((WDGIF_SW_MAJOR_VERSION_C != WDGIF_SW_MAJOR_VERSION) || \
     (WDGIF_SW_MINOR_VERSION_C != WDGIF_SW_MINOR_VERSION) || \
     (WDGIF_SW_PATCH_VERSION_C != WDGIF_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of WdgIf.c and WdgIf.h are different"
#endif

#if (USE_WDG_MODULE == STD_ON)
#ifdef WDG_INSTANCE0
#if (WDG_INSTANCE0 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance0 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE0_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE0_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance0.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE1
#if (WDG_INSTANCE1 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance1 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE1_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE1_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance1.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE2
#if (WDG_INSTANCE2 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance2 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE2_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE2_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance2.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE3
#if (WDG_INSTANCE3 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance3 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE3_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE3_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance3.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE4
#if (WDG_INSTANCE4 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance4 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE4_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE4_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance4.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE5
#if (WDG_INSTANCE5 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance5 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE5_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE5_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance5.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE6
#if (WDG_INSTANCE6 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance6 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE6_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE6_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance6.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE7
#if (WDG_INSTANCE7 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance7 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE7_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE7_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance7.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE8
#if (WDG_INSTANCE8 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance8 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE8_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE8_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance8.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE9
#if (WDG_INSTANCE9 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance9 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE9_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE9_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance9.h are different"
     #endif
#endif
#endif
#endif

#ifdef WDG_INSTANCE10
#if (WDG_INSTANCE10 == STD_ON)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
     /* Check if source file and Wdg instance10 header file are of the same Autosar version */
     #if (( WDGIF_AR_RELEASE_MAJOR_VERSION_C != WDG_43_INSTANCE10_AR_RELEASE_MAJOR_VERSION) || \
          ( WDGIF_AR_RELEASE_MINOR_VERSION_C != WDG_43_INSTANCE10_AR_RELEASE_MINOR_VERSION))
         #error "AutoSar Version Numbers of Wdg_Channel.c and Wdg_43_Instance10.h are different"
     #endif
#endif
#endif
#endif
#endif
/*=================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=================================================================================================*/


/*=================================================================================================
*                                       LOCAL MACROS
=================================================================================================*/


/*=================================================================================================
*                                      LOCAL CONSTANTS
=================================================================================================*/


/*=================================================================================================
*                                      LOCAL VARIABLES
=================================================================================================*/


/*=================================================================================================
*                                      GLOBAL CONSTANTS
=================================================================================================*/

#if (USE_WDG_MODULE == STD_ON)
const WdgIf_SetModeFctPtrType WdgIf_SetModeFctPtr[WDGIF_NUMBER_OF_DEVICES] =
    {
    #if(WDG_INSTANCE0 == STD_ON)
            Wdg_43_Instance0_SetMode
            #else
            NULL_PTR
        #endif
    #if (WDGIF_NUMBER_OF_DEVICES>1)
        #ifdef WDG_INSTANCE1
            #if(WDG_INSTANCE1 == STD_ON)
            ,Wdg_43_Instance1_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>2)
        #ifdef WDG_INSTANCE2
            #if(WDG_INSTANCE2 == STD_ON)
            ,Wdg_43_Instance2_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>3)
        #ifdef WDG_INSTANCE3
            #if(WDG_INSTANCE3 == STD_ON)
            ,Wdg_43_Instance3_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>4)
        #ifdef WDG_INSTANCE4
            #if(WDG_INSTANCE4 == STD_ON)
            ,Wdg_43_Instance4_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>5)
        #ifdef WDG_INSTANCE5
            #if(WDG_INSTANCE5 == STD_ON)
            ,Wdg_43_Instance5_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>6)
        #ifdef WDG_INSTANCE6
            #if(WDG_INSTANCE6 == STD_ON)
            ,Wdg_43_Instance6_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>7)
        #ifdef WDG_INSTANCE7
            #if(WDG_INSTANCE7 == STD_ON)
            ,Wdg_43_Instance7_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>8)
        #ifdef WDG_INSTANCE8
            #if(WDG_INSTANCE8 == STD_ON)
            ,Wdg_43_Instance8_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>9)
        #ifdef WDG_INSTANCE9
            #if(WDG_INSTANCE9 == STD_ON)
            ,Wdg_43_Instance9_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>10)
        #ifdef WDG_INSTANCE10
            #if(WDG_INSTANCE10 == STD_ON)
            ,Wdg_43_Instance10_SetMode
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    };

#if (WDG_DIRECT_SERVICE == STD_OFF)
const WdgIf_SetTriggerFctPtrType WdgIf_SetTriggerConditionFctPtr[WDGIF_NUMBER_OF_DEVICES] =
    {
    #if(WDG_INSTANCE0 == STD_ON)
            Wdg_43_Instance0_SetTriggerCondition
            #else
            NULL_PTR
        #endif
    #if (WDGIF_NUMBER_OF_DEVICES>1)
        #ifdef WDG_INSTANCE1
            #if(WDG_INSTANCE1 == STD_ON)
            ,Wdg_43_Instance1_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>2)
        #ifdef WDG_INSTANCE2
            #if(WDG_INSTANCE2 == STD_ON)
            ,Wdg_43_Instance2_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>3)
        #ifdef WDG_INSTANCE3
            #if(WDG_INSTANCE3 == STD_ON)
            ,Wdg_43_Instance3_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>4)
        #ifdef WDG_INSTANCE4
            #if(WDG_INSTANCE4 == STD_ON)
            ,Wdg_43_Instance4_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>5)
        #ifdef WDG_INSTANCE5
            #if(WDG_INSTANCE5 == STD_ON)
            ,Wdg_43_Instance5_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>6)
        #ifdef WDG_INSTANCE6
            #if(WDG_INSTANCE6 == STD_ON)
            ,Wdg_43_Instance6_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>7)
        #ifdef WDG_INSTANCE7
            #if(WDG_INSTANCE7 == STD_ON)
            ,Wdg_43_Instance7_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>8)
        #ifdef WDG_INSTANCE8
            #if(WDG_INSTANCE8 == STD_ON)
            ,Wdg_43_Instance8_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>9)
        #ifdef WDG_INSTANCE9
            #if(WDG_INSTANCE9 == STD_ON)
            ,Wdg_43_Instance9_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    #if (WDGIF_NUMBER_OF_DEVICES>10)
        #ifdef WDG_INSTANCE10
            #if(WDG_INSTANCE10 == STD_ON)
            ,Wdg_43_Instance10_SetTriggerCondition
            #else
            ,NULL_PTR
            #endif
        #endif
    #endif
    };
#endif
#endif

/*=================================================================================================
*                                      GLOBAL VARIABLES
=================================================================================================*/


/*=================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
=================================================================================================*/


/*=================================================================================================
*                                       LOCAL FUNCTIONS
=================================================================================================*/

/*=================================================================================================
*                                       GLOBAL FUNCTIONS
=================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

[!/*
*   @file    Lin_BaudRate_Comp.m
*   @brief   AUTOSAR Lin - Baud rate computation for Lin_Cfg.c, Lin_PBcfg.c.
*   @details Baud rate computation for Lin_Cfg.c, Lin_PBcfg.c
*   
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPUART_FLEXIO
*   Dependencies         : none
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
*/!]
[!/*==================================================================================================
==================================================================================================
*/!]
[!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
[!/* LPUART baudrate */!]
    [!VAR "BaudrateConf_Value" = "($ClockFrequencyFromMcu div ($LinBaudrate * 16)) - 1"!][//
    [!IF "$BaudrateConf_Value > 8191"!]
        [!ERROR!] Lin baud rate divider = [!"$BaudrateConf_Value"!], it should be <= 8191, please select a lower MCU clock.[!ENDERROR!]
    [!ENDIF!]
    [!VAR "BaudrateConf_Value" = "num:inttohex($BaudrateConf_Value)"!][!//


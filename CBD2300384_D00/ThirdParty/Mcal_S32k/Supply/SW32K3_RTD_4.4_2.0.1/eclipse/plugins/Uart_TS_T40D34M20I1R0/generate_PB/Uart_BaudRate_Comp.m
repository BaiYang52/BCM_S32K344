/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO
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
*/!]
[!VAR "ClockFrequencyFromMcu" = "McuClockReferencePointFrequency"!]
[!/* LpuartBaudrate baudrate */!]
[!/* Baud clock divisor */!][//
[!VAR "Osr"                     = "4"!][//
[!/* Over sampling ratio */!][//
[!VAR "Sbr"                     = "num:i($ClockFrequencyFromMcu div ($LpuartBaudrate * $Osr))"!][//
[!VAR "CalculatedBaud"          = "num:i($ClockFrequencyFromMcu div ($Sbr * $Osr))"!][!//

[!IF "$CalculatedBaud > $LpuartBaudrate"!]
    [!VAR "BaudDiff" = "($CalculatedBaud - $LpuartBaudrate)"!][!//
[!ELSE!][!//
    [!VAR "BaudDiff" = "($LpuartBaudrate - $CalculatedBaud)"!][!//
[!ENDIF!][!//

[!VAR "MaxOsr"          = "num:i(($ClockFrequencyFromMcu) div ($LpuartBaudrate))"!][!//
[!IF "$MaxOsr > 32"!][!VAR "MaxOsr" = "32"!][!ENDIF!][!//

[!IF "$MaxOsr >= 5"!][!//
    [!FOR "i" = "5" TO "num:i($MaxOsr)"!][!//
        [!/* Calculate the temporary Sbr value */!][//
        [!VAR "SbrTemp"          = "num:i($ClockFrequencyFromMcu div ($LpuartBaudrate * $i))"!][!//
        [!/* Calculate the baud rate based on the temporary Osr and Sbr values */!][//
        [!VAR "CalculatedBaud"   = "num:i($ClockFrequencyFromMcu div ($SbrTemp * $i))"!][!//

        [!IF "$CalculatedBaud > $LpuartBaudrate"!]
            [!VAR "TempDiff" = "($CalculatedBaud - $LpuartBaudrate)"!][!//
        [!ELSE!][!//
            [!VAR "TempDiff" = "($LpuartBaudrate - $CalculatedBaud)"!][!//
        [!ENDIF!][!//

        [!IF "$BaudDiff >= $TempDiff"!][!//
            [!VAR "BaudDiff" = "$TempDiff"!][!//
            [!/* Update and store the best Sbr value calculated */!][//
            [!VAR "Osr" = "$i"!][!//
            [!/* Update store the best Osr value calculated */!][//
            [!VAR "Sbr" = "$SbrTemp"!][!//
        [!ENDIF!][!//
    [!ENDFOR!][!//
[!ENDIF!][!//
[!/* Update Sbr value to BaudRateDivisor value */!][//
[!VAR "BaudRateDivisor" = "num:i($Sbr)"!][!//
[!/* Update Osr value to BaudOverSamplingRatio value */!][//
[!VAR "BaudOverSamplingRatio" = "num:i($Osr)"!][!//
[!VAR "ActualBaudrate"   = "num:i($ClockFrequencyFromMcu div ($BaudRateDivisor * $BaudOverSamplingRatio))"!][!//

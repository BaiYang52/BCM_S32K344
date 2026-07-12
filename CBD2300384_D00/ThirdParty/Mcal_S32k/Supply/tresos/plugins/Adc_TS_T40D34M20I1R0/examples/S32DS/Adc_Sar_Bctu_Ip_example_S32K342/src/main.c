/*==================================================================================================
* (c) Copyright 2020 NXP
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

#include "Clock_Ip.h"
#include "IntCtrl_Ip.h"
#include "Adc_Sar_Ip.h"
#include "Bctu_Ip.h"

/* By default S32K342 boards will have 3.3V voltage reference selected while S32K312 and S32K344 will have 5V selected.
 * If you have S32K342 with default pin configuration or if you've manually selected 3.3V reference, please uncomment the following line: */
/* #define ADC_3V3_VREF_SELECTED */
#ifdef ADC_3V3_VREF_SELECTED
#define ADC_BANDGAP         5980U /* Vbandgap ~ 1.2V on 14 bits resolution, 3.3V VrefH */
#else
#define ADC_BANDGAP         3932U /* Vbandgap ~ 1.2V on 14 bits resolution, 5V VrefH */
#endif
#define ADC_SAR_USED_CH 48U /* Internal Bandgap Channel */
#define BCTU_USED_SINGLE_TRIG_IDX 0U
#define BCTU_USED_FIFO_IDX 0U
#define BCTU_FIFO_WATERMARK 1U
#define ADC_TOLERANCE(x,y) (((x > y) ? (x - y) : (y - x)) > 200U) /* Check that the data is within tolerated range */

volatile int exit_code = 0;
volatile boolean notif_triggered = FALSE;

volatile uint16 data;

extern void Adc_Sar_0_Isr(void);
extern void Bctu_0_Isr(void);

void AdcEndOfChainNotif(void)
{
    notif_triggered = TRUE;
    data = Adc_Sar_Ip_GetConvData(ADCHWUNIT_0_VS_0_INSTANCE, ADC_SAR_USED_CH);
    /* Checks the measured ADC data conversion */
    while (ADC_TOLERANCE(data, ADC_BANDGAP));
}

void BctuWatermarkNotif(void)
{
    uint8 idx;
    notif_triggered = TRUE;
    for (idx = 0u; idx < BCTU_FIFO_WATERMARK; idx++)
    {
        data = Bctu_Ip_GetFifoData(BCTUHWUNIT_0_VS_0_INSTANCE, BCTU_USED_FIFO_IDX);
        /* Checks the measured ADC data conversion */
        while (ADC_TOLERANCE(data, ADC_BANDGAP));
    }
}

int main(void)
{
    StatusType status;
    uint8 Index;
    Clock_Ip_StatusType clockStatus;

    /* Initialize and configure drivers */
    clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    while (clockStatus != CLOCK_IP_SUCCESS)
    {
        clockStatus = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    }

    Bctu_Ip_Init(BCTUHWUNIT_0_VS_0_INSTANCE, &BctuHwUnit_0_VS_0);

    status = (StatusType) Adc_Sar_Ip_Init(ADCHWUNIT_0_VS_0_INSTANCE, &AdcHwUnit_0_VS_0);
    while (status != E_OK);

    /* Install and enable interrupt handlers */
    IntCtrl_Ip_InstallHandler(ADC0_IRQn, Adc_Sar_0_Isr, NULL_PTR);
    IntCtrl_Ip_InstallHandler(BCTU_IRQn, Bctu_0_Isr, NULL_PTR);
    IntCtrl_Ip_EnableIrq(ADC0_IRQn);
    IntCtrl_Ip_EnableIrq(BCTU_IRQn);

    /* Call Calibration function multiple times, to mitigate instability of board source */
    for(Index = 0; Index <= 5; Index++)
    {
        status = (StatusType) Adc_Sar_Ip_DoCalibration(ADCHWUNIT_0_VS_0_INSTANCE);
        if(status == E_OK)
        {
            break;
        }
    }

    Adc_Sar_Ip_EnableNotifications(ADCHWUNIT_0_VS_0_INSTANCE, ADC_SAR_IP_NOTIF_FLAG_NORMAL_ENDCHAIN | ADC_SAR_IP_NOTIF_FLAG_INJECTED_ENDCHAIN);

    /* Start a SW triggered normal conversion on ADC_SAR */
    Adc_Sar_Ip_StartConversion(ADCHWUNIT_0_VS_0_INSTANCE, ADC_SAR_IP_CONV_CHAIN_NORMAL);

    /* Wait for the notification to be triggered and read the data */
    while (notif_triggered != TRUE);
    notif_triggered = FALSE;

    /* Start a SW triggered injected conversion on ADC_SAR */
    Adc_Sar_Ip_StartConversion(ADCHWUNIT_0_VS_0_INSTANCE, ADC_SAR_IP_CONV_CHAIN_INJECTED);

    /* Wait for the notification to be triggered and read the data */
    while (notif_triggered != TRUE);
    notif_triggered = FALSE;

    /* Start a SW triggered conversion on BCTU using a single trigger */
    Bctu_Ip_SetGlobalTriggerEn(BCTUHWUNIT_0_VS_0_INSTANCE, TRUE);
    Bctu_Ip_EnableNotifications(BCTUHWUNIT_0_VS_0_INSTANCE, BCTU_IP_NOTIF_FIFO1);
    Bctu_Ip_SwTriggerConversion(BCTUHWUNIT_0_VS_0_INSTANCE, BCTU_USED_SINGLE_TRIG_IDX);

    /* Wait for the watermark notification to be triggered and then read the data */
    while (notif_triggered != TRUE);

    /* Deinitialize drivers */
    Bctu_Ip_Deinit(BCTUHWUNIT_0_VS_0_INSTANCE);
    status = (StatusType) Adc_Sar_Ip_Deinit(ADCHWUNIT_0_VS_0_INSTANCE);
    while (status != E_OK);

    for(;;)
    {
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/*
 * adc_temperature.c
 *
 *  Created on: Apr 11, 2024
 *      Author: giang.nguyen-hoang
 */

#ifndef SEGGER_RTT_ADC_TEMPERATURE_C_
#define SEGGER_RTT_ADC_TEMPERATURE_C_

#include "adc_temperature.h"



void adc_temperature_init(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* Initializes the module. */
    err = R_ADC_Open (&g_adc0_ctrl, &g_adc0_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);
    /* Enable channels. */
    err = R_ADC_ScanCfg (&g_adc0_ctrl, &g_adc0_channel_cfg);
    assert(FSP_SUCCESS == err);

    /* Disable interrupts */
//    R_BSP_IrqDisable ((IRQn_Type) ADC_EVENT_SCAN_COMPLETE);

}

void adc_temperature_deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* close the ADC driver */
    err = R_ADC_Close (&g_adc0_ctrl);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* GPT Close failure message */
        APP_ERR_PRINT("** R_ADC_Close API failed **  \r\n");
    }
}

int32_t adc_temperature_read_data(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* In software trigger mode, start a scan by calling R_ADC_ScanStart(). In other modes, enable external
     * triggers by calling R_ADC_ScanStart(). */
    (void) R_ADC_ScanStart (&g_adc0_ctrl);

    /* Read converted data. */
    uint16_t temperature_conversion_result;
    err = R_ADC_Read (&g_adc0_ctrl, ADC_CHANNEL_TEMPERATURE, &temperature_conversion_result);
    assert(FSP_SUCCESS == err);

    /* If the MCU does not provide calibration data, use the value in the hardware manual or determine it
     * experimentally. */
    /* Get Calibration data from the MCU if available. */
    int32_t reference_calibration_data;
    adc_info_t adc_info;
    (void) R_ADC_InfoGet (&g_adc0_ctrl, &adc_info);
    reference_calibration_data = (int32_t) adc_info.calibration_data;

    /* NOTE: The slope of the temperature sensor varies from sensor to sensor. Renesas recommends calculating
     * the slope of the temperature sensor experimentally.
     *
     * This example uses the typical slope provided in Table 52.38  "TSN characteristics" in the RA6M1 manual
     * R01UM0011EU0050. */
    int32_t slope_uv_per_c = BSP_FEATURE_ADC_TSN_SLOPE;
    /* Formula for calculating temperature copied from section 44.3.1 "Preparation for Using the Temperature Sensor"
     * of the RA6M1 manual R01UH0884EJ0100:
     *
     * In this MCU, the TSCDR register stores the temperature value (CAL127) of the temperature sensor measured
     * under the condition Ta = Tj = 127 C and AVCC0 = 3.3 V. By using this value as the sample measurement result
     * at the first point, preparation before using the temperature sensor can be omitted.
     *
     * If V1 is calculated from CAL127,
     * V1 = 3.3 * CAL127 / 4096 [V]
     *
     * Using this, the measured temperature can be calculated according to the following formula.
     *
     * T = (Vs - V1) / Slope + 127 [C]
     * T: Measured temperature (C)
     * Vs: Voltage output by the temperature sensor when the temperature is measured (V)
     * V1: Voltage output by the temperature sensor when Ta = Tj = 127 C and AVCC0 = 3.3 V (V)
     * Slope: Temperature slope given in Table 52.38 / 1000 (V/C)
     */
    int32_t v1_uv = (ADC_VCC_MICROVOLT >> ADC_TEMPERATURE_RESOLUTION) * reference_calibration_data;
    int32_t vs_uv = (ADC_VCC_MICROVOLT >> ADC_TEMPERATURE_RESOLUTION) * temperature_conversion_result;
    int32_t temperature_c = (vs_uv - v1_uv) / slope_uv_per_c + ADC_REFERENCE_CALIBRATION_TEMPERATURE;

    return temperature_c;
}

void user_adc_callback(adc_callback_args_t *p_args)
{
    if (ADC_EVENT_WINDOW_COMPARE_A == p_args->event)
    {
//        g_window_comp_event = true;
//        IRQn_Type irq = R_FSP_CurrentIrqGet ();
//        R_BSP_IrqDisable (irq);
        /* Print temperature status warning to RTT Viewer */

//        APP_PRINT("\r\nCompare window occurred. \r\n");
//        if (temperature_c >= TEMPERATURE_HIGH)
//        {
//            APP_PRINT("\r\nTemperature is above the Upper Limit. \r\n");
//        }
//        else if (temperature_c <= TEMPERATURE_LOW)
//        {
//            APP_PRINT("\r\nTemperature is below the Lower Limit.  \r\n");
//        }
//        else
//        {
//            APP_PRINT("\r\nTemperature is normal.  \r\n");
//        }
//        IRQn_Type irq = R_FSP_CurrentIrqGet ();
//        R_BSP_IrqDisable (irq);
    }
    if (ADC_EVENT_CONVERSION_ERROR == p_args->event)
    {
        APP_PRINT("\r\nADC_EVENT_CONVERSION_ERROR\r\n");
    }
}

#endif /* SEGGER_RTT_ADC_TEMPERATURE_C_ */

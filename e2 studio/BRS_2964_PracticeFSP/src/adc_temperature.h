/*
 * adc_temperature.h
 *
 *  Created on: Apr 11, 2024
 *      Author: giang.nguyen-hoang
 */

#ifndef ADC_TEMPERATURE_H_
#define ADC_TEMPERATURE_H_
#include "common_utils.h"

#define ADC_VCC_MICROVOLT                        (3300000)
#define ADC_TEMPERATURE_RESOLUTION               (12U)
#define ADC_REFERENCE_CALIBRATION_TEMPERATURE    (127)

#define TEMPERATURE_HIGH    30
#define TEMPERATURE_LOW     20


void adc_temperature_init(void);

void adc_temperature_deinit(void);

/* Read the adc data available */
int32_t adc_temperature_read_data(void);


void user_adc_callback(adc_callback_args_t *p_args);


#endif /* ADC_TEMPERATURE_H_ */

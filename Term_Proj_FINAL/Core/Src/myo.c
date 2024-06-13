/*
 * myo.c
 *
 *  Created on: May 30, 2024
 *      Author: Julia Fay
 */

#include "myo.h"

/**
 * @brief A function to get the ADC value for the myoelectric sensor.
 *
 * @param p_myo The myoelectric sensor object to perform the function on.
 */
uint16_t read_current(myo_t *p_myo){

	HAL_ADC_Start(p_myo->hal_adc);
	HAL_ADC_PollForConversion(p_myo->hal_adc,100);
	p_myo->current_value = HAL_ADC_GetValue(p_myo->hal_adc);
	HAL_ADC_Stop(p_myo->hal_adc);


	return p_myo->current_value;

}

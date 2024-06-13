/**
 * @file myo.h
 * @brief Defines the myoelectric sensor struct and its methods. This file is used to read and interpret the
 * 		  output form the myoelectric sensor to be sent to the controller to determine the desired motor position.
 */

#ifndef INC_MYO_H_
#define INC_MYO_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
/**
 * @brief Represents a myoelectric sensor object that has an ADC object and a current sensor value.
 */
struct{

	    ADC_HandleTypeDef* hal_adc; /**< The ADC object. */
	    int16_t current_value;/**< The current ADC value. */

} typedef myo_t;

/**
 * @brief A function to get the ADC value for the myoelectric sensor.
 *
 * @param p_myo The myoelectric sensor object to perform the function on.
 *
 * @return current_value The current value of the sensor read from the ADC.
 */
uint16_t read_current(myo_t* p_myo);



#endif /* INC_MYO_H_ */
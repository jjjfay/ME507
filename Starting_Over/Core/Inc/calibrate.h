/*
 * calibrate.h
 *
 *  Created on: Jun 10, 2024
 *      Author: julia
 */

#ifndef SRC_CALIBRATE_H_
#define SRC_CALIBRATE_H_

#include <stdio.h>
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "myo.h"

/**
 * @brief Represents a encoder object that has a timer with two channels, and an encoder count.
 */
struct{


		uint32_t length; /**< The length of the array to be created . */
		uint32_t median_length; /**< The length of the array to be created . */
	    myo_t* p_myo; /**< The the myo electric sensor to read from. */


} typedef calibrate_t;

/**
 * @brief A function to find the average myoelectric sensor value from an array of myo values.
 *
 * @param p_cali The calibration object to perform the function on.
 */
uint32_t find_average(calibrate_t*p_cali);



#endif /* SRC_CALIBRATE_H_ */

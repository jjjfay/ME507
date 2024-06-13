/**
 * @class calibrate.h
 * @brief Defines a function to calibrate the myoelectric sensor values by calculating an average from
 * 		  a specified number of data points.
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
 * @brief Represents a calibration object that has a myoelectric sensor and an array length to perform the find_average
 * 		  function on.
 */
struct{


		uint32_t data_pts; /**< The number of data points to be read. */
	    myo_t* p_myo; /**< The the myo electric sensor to read from. */


} typedef calibrate_t;

/**
 * @brief A function to find the average myoelectric sensor value for a specified number of data points.
 *
 * @param p_cali The calibration object to perform the function on.
 */
uint32_t find_average(calibrate_t*p_cali);



#endif /* SRC_CALIBRATE_H_ */

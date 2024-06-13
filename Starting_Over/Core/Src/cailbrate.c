/*
 * cailbrate.c
 *
 *  Created on: Jun 10, 2024
 *      Author: julia
 */

#include "calibrate.h"

uint32_t find_average(calibrate_t *p_cali){

	//initialize the sum to be zero
	uint32_t sum = 0;
	uint32_t n = p_cali->arr_length;

	//collect the data
	for(int i = 0 ; i<n; i++){

		int curr = read_current(p_cali->p_myo);
		sum = sum + curr;

	}

	//return the average
	return (sum/n);
}


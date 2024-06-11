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
	uint32_t n = p_cali->length;

	//collect the data
	for(int i = 0 ; i<n; i++){

		int curr = read_current(p_cali->p_myo);
		sum = sum + curr;

	}

	//return the average
	return (sum/n);
}


// Function to compare two integers (for qsort)
uint32_t compare(const void *a, const void *b) {
    return (*(uint32_t *)a - *(uint32_t *)b);
}


uint32_t find_median(calibrate_t *p_cali) {
    if (p_cali == NULL || p_cali->median_length == 0) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }

    uint32_t n = p_cali->median_length;
    uint32_t *myo_arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (myo_arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Collect the data
    for (uint32_t i = 0; i < n; i++) {
        myo_arr[i] = read_current(p_cali->p_myo);
        HAL_Delay(100);
    }

    // Sort the array
    qsort(myo_arr, n, sizeof(uint32_t), compare);

    // Calculate median
    uint32_t median;
    if (n % 2 == 0) {
        median = (myo_arr[n/2 - 1] + myo_arr[n/2]) / 2;
    } else {
        median = myo_arr[n/2];
    }

    free(myo_arr);
    return median;
}

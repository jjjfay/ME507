/*
 * encoder_reader.c
 *
 *  Created on: May 18, 2024
 *      Author: julia
 */

#include "encoder_reader.h"

/**
 * @brief A function to initialize the channels for reading the encoder signals.
 *
 * @param p_enc The encoder object to perform the function on.
 */
void init_channels(encoder_t* p_enc){

	//probably some sort of HAL thing
}

/**
 * @brief A function to stop the channels from reading the encoder signals.
 *
 * @param p_enc The encoder object to perform the function on.
 */
void deinit_channels(encoder_t* p_enc){

	//probably some sort of HAL thing
}

/**
 * @brief A function to zero the encoder count.
 */

void zero(encoder_t* p_enc){

	p_enc->count = 0;

}

/**
 * @brief A function to read and return the encoder count.
 *
 * @param p_enc The encoder object to perform the function on.
 *
 * @return count The encoder count to be returned.
 */

uint16_t read_count(encoder_t* p_enc){

	//not sure exactly how to write this one yet
	return p_enc->count;
}

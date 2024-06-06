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
	HAL_TIM_Encoder_Start(p_enc->hal_tim,  TIM_CHANNEL_ALL);
	//HAL_TIM_Encoder_Start(p_enc->hal_tim,  p_enc->channel2);
}

/**
 * @brief A function to stop the channels from reading the encoder signals.
 *
 * @param p_enc The encoder object to perform the function on.
 */
void deinit_channels(encoder_t* p_enc){

	//probably some sort of HAL thing
	HAL_TIM_Encoder_Stop(p_enc->hal_tim,  TIM_CHANNEL_ALL);
	//HAL_TIM_Encoder_Stop(p_enc->hal_tim,  p_enc->channel2);
}

/**
 * @brief A function to zero the encoder count.
 */

void zero(encoder_t* p_enc){

	p_enc->mot_pos = 0;

}

/**
 * @brief A function to read and return the encoder count.
 *
 * @param p_enc The encoder object to perform the function on.
 *
 * @return count The encoder count to be returned.
 */

uint16_t get_pos(encoder_t* p_enc){

	p_enc->prev_count = p_enc->curr_count;
	p_enc->curr_count = __HAL_TIM_GET_COUNTER(p_enc->hal_tim);

	p_enc->delta = p_enc->curr_count - p_enc->prev_count;

	//address overflow issues
	if(p_enc->delta >= 32768){

		p_enc->delta -= 65536;
	}
	else if(p_enc->delta <= -32768){

		p_enc->delta += 65536;
	}

	p_enc->mot_pos += p_enc->delta;


	return p_enc->mot_pos;
}

/**
 * @file encoder_reader.h
 * @brief Defines the encoder reader struct and its methods. This file is used to read the position
          of a motor using the attached motor encoder. This is accomplished by creating an Encoder class
          with several functions defined to aid in the task of reading the encoder including init, read, zero
          and loop.
 */

#ifndef INC_ENCODER_READER_H_
#define INC_ENCODER_READER_H_
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
/**
 * @brief Represents a encoder object that has a timer with two channels, and an encoder count.
 */
struct{


	    uint32_t channel1; /**< The timer channel for the first encoder output. */
	    uint32_t channel2; /**< The timer channel for the second encoder output. */

	    TIM_HandleTypeDef* hal_tim; /**< The timer object both channels are from. */

	    int32_t mot_pos; /**< The motor position. */
	    int32_t curr_count; /**< The current encoder count. */
	    int32_t prev_count; /**< The previous encoder count. */
	    int32_t delta; /**< The difference between the previous and current encoder count. */



} typedef encoder_t;

/**
 * @brief A function to initialize the channels for reading the encoder signals.
 *
 * @param p_enc The encoder object to perform the function on.
 */
void init_channels(encoder_t *p_enc);

/**
 * @brief A function to stop the channels from reading the encoder signals.
 *
 * @param p_enc The encoder object to perform the function on.
 */
void deinit_channels(encoder_t *p_enc);

/**
 * @brief A function to zero the encoder count.
 *
 * @param p_enc The encoder object to perform the function on.
 */

void zero(encoder_t* p_enc);

/**
 * @brief A function to read and return the encoder count.
 *
 * @param p_enc The encoder object to perform the function on.
 *
 * @return count The encoder count to be returned.
 */

int32_t get_pos(encoder_t *p_enc);

#endif /* INC_ENCODER_READER_H_ */
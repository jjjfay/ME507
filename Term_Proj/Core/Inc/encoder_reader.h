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

	    uint16_t count; /**< The encoder count. */



} typedef encoder_t;

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

uint16_t read_count(encoder_t* p_enc);

#endif /* INC_ENCODER_READER_H_ */

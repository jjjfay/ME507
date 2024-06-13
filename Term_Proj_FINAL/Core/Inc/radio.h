/*
 *@file			  :	 radio.h
 *
 *@brief          : This is the radio library that interprets the pulse width signal from the radio transmitter.
 *
 *  Created on: Apr 18, 2024
 *      Author: Julia Fay
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include <stdint.h>
#include <stdio.h>


/**
 * @brief Checks the pulse width value from the radio transmitter and returns a 1 if the signal
 * 		  is larger or smaller than 1.5 ms.
 *
 * @param pulse_width The pusle width value in ms calculated in the interrupt callback function to
 * 		  be interpreted by the check_delta function.
 *
 * @return valid Returns either a 0 or 1 based on whether the signal is at the expected value of 1.5 ms,
 * 				 in which case a 0 is returned, or a 1 ,when the signal deviates from 1.5 ms in either
 * 				 direction.
 */
int check_delta(int16_t pulse_width);


#endif /* INC_RADIO_H_ */

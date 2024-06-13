/*
 * radio.c
 *
 *  Created on: May 23, 2024
 *      Author: julia
 */

#include "radio.h"

int check_delta(int16_t pulse_width)
{
	//if the delta value is invalid, or is at the 1.5 ms base width return 0. If it is not at the
	//base value return a 1.
	if (900 < pulse_width && pulse_width < 1450)
	{
	    return 1;
	}
	else if (1550 < pulse_width && pulse_width < 2500)
	{
	    return 1;
	}
	else
	{
			return 0;
	}
}

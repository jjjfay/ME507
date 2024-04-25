/*
 * motor_driver.h
 *
 *  Created on: Apr 18, 2024
 *      Author: julia
 */

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"


/*To implement your driver you will need to create a new datatype using a C struct and the typedef keyword.
 *  The structure should have fields representing the information pertinent to a specific motor driver channel
 *  - perhaps with information like:
The handle to the HAL timer object used for PWM generation.
The pins and ports associated with the pins used by the driver.
What channels on the timer are associated with the PWM pins.
Any other information needed inside the three functions mentioned above.*/

// Motor object data structure
//struct has multiple data types
struct {

	//These are all the things you have to define when you create a motor object
    int32_t  duty;

    //two channels for each motor
    uint32_t channel1;
    uint32_t channel2;

    //The handle to the HAL timer object used for PWM generation. Include * so its a pointer to the object
    TIM_HandleTypeDef* hal_tim;

    //pin data typ is int32_t
    //int32_t pwm_pin_1;
	//int32_t pwm_pin_2;

    //port data type is GPIO_TypeDef. Include * so its a pointer to the object.
	//don't really see why I need this right now
	//port stores the pin behavior information
	//GPIO_TypeDef* port

} typedef motor_t;

/*Your driver should, at the minimum, have the following functionality:
A function to enable one of the motor driver channels
A function to disable one of the motor driver channels
A function to set the duty cycle of one of the motor driver channels*/

// Prototype for motor object "method"

void start_PWM(motor_t* p_mot, uint32_t channel);

void stop_PWM(motor_t* p_mot, uint32_t channel);

void set_duty(motor_t* p_mot, int32_t duty);

#endif /* INC_MOTOR_DRIVER_H_ */

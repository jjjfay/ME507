/*
 *@file			  :	 motor_driver.h
 *
 *@brief          : This is the motor driver library used to control the PWM signals going to the motors to control direction and speed.
 *		          	Functions include starting and stopping the PWM timer channels, and updating the duty cycle for the motor.
 *
 *  Created on: Apr 18, 2024
 *      Author: Julia Fay
 */

#ifndef INC_MOTOR_DRIVER_H_
#define INC_MOTOR_DRIVER_H_
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"

/**
 * @brief Represents a motor objects with two PWM channels in a timer and a duty cycle.
 */
struct {

	//These are all the things you have to define when you create a motor object
    int32_t  pwm_val;

    //one channels for each motor
    uint32_t pwm_channel;

    //The handle to the HAL timer object used for PWM generation. Include * so its a pointer to the object
    TIM_HandleTypeDef* hal_tim;

    //pin data typ is int32_t

    uint32_t in_pin_1;
	uint32_t in_pin_2;
	uint32_t stby_pin;

    //port data type is GPIO_TypeDef. Include * so its a pointer to the object.
	//don't really see why I need this right now
	//port stores the pin behavior information

	GPIO_TypeDef* port_in_1;
	GPIO_TypeDef* port_in_2;
	GPIO_TypeDef* port_stby_pin;

} typedef motor_t;


/**
 * @brief A function to enable one of the motor driver channels.
 *
 * @param p_mot The motor object to perform the function on.
 * @param channel The timer channel to perform the function on.
 */

void start_PWM(motor_t* p_mot);

/**
 * @brief A function to disable one of the motor driver channels.
 *
 * @param p_mot The motor object to perform the function on.
 * @param channel The timer channel to perform the function on.
 */

void stop_PWM(motor_t* p_mot);

/**
 * @brief A function to set the duty cycle for the motor.
 *
 * @param p_mot The motor object to perform the function on.
 * @param duty The desired duty cycle for the motor.
 */
void set_duty(motor_t* p_mot, int32_t duty);

/**
 * @brief A function to set the standby pin for the motor.
 *
 * @param p_mot The motor object to perform the function on.
 * @param stby The desired stby pin value of 0 or 1.
 */
void set_stby(motor_t* p_mot, int32_t stby_val);

#endif /* INC_MOTOR_DRIVER_H_ */

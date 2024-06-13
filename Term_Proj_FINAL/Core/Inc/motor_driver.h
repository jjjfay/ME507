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

    int32_t  pwm_val;/**< The CCR pwm value used to set the duty cycle of the motor. */

    //two channels for each motor
    uint32_t channel1;/**< Timer channel 1 used to generate a PWM signal that is sent to the motor driver. */
    uint32_t channel2;/**< Timer channel 2 used to generate a PWM signal that is sent to the motor driver. */

    //The handle to the HAL timer object used for PWM generation. Include * so its a pointer to the object
    TIM_HandleTypeDef* hal_tim;/**< The handle to the HAL timer object used for PWM generation. */

} typedef motor_t;


/**
 * @brief A function to enable the motor driver channels.
 *
 * @param p_mot The motor object to perform the function on.
 */

void start_PWM(motor_t* p_mot);

/**
 * @brief A function to disable the motor driver channels.
 *
 * @param p_mot The motor object to perform the function on.
 */

void stop_PWM(motor_t* p_mot);

/**
 * @brief A function to set the duty cycle for the motor.
 *
 * @param p_mot The motor object to perform the function on.
 * @param duty The CCR value used to set the duty cycle of the motor.
 */
void set_duty(motor_t* p_mot, int32_t duty);


#endif /* INC_MOTOR_DRIVER_H_ */

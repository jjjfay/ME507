/*
 *@file			  :	 motor_driver.c
 *
 *@brief          : This is the motor driver library used to control the PWM signals going to the motors to control direction and speed.
 *		          	Functions include starting and stopping the PWM timer channels, and updating the duty cycle for the motor.
 *
 *  Created on: Apr 18, 2024
 *      Author: Julia Fay
 */

#include "motor_driver.h"


void start_PWM(motor_t* p_mot)
{
	/* A function to enable one of the motor driver channels.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param channel The timer channel to perform the function on.
	 *
	 *  */

		HAL_TIM_PWM_Start(p_mot->hal_tim, p_mot->pwm_channel);
}


void stop_PWM(motor_t* p_mot)
{
	/* A function to disable one of the motor driver channels.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param channel The timer channel to perform the function on.
	 *
	 */

			HAL_TIM_PWM_Stop(p_mot->hal_tim, p_mot->pwm_channel);

}

void set_duty(motor_t* p_mot, int32_t pwm_sig)
{
	/* A function to set the duty cycle for the motor.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param duty The desired duty cycle for the motor.
	 */

    // Print to the console so we can see what's happening
    printf("Setting Motor Duty Cycle to %ld", pwm_sig);

    // Assign the duty cycle to a field in the structure
    p_mot->pwm_val = pwm_sig;

    // The arrow operator is shorthand for
    // dereferencing and accessing struct fields
    // p_mot->duty = duty;
    // is shorthand for
    // (*p_mot).duty = duty;

    if (pwm_sig > 0) {

    	//forwards
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->pwm_channel,pwm_sig);

    	//set GPIO PINS
    	// GPIO_PIN_RESET: to clear the port pin
    	 //GPIO_PIN_SET: to set the port pin

    	HAL_GPIO_WritePin (p_mot->port_in_1, p_mot->in_pin_1, GPIO_PIN_SET);
    	HAL_GPIO_WritePin (p_mot->port_in_2, p_mot->in_pin_2, GPIO_PIN_RESET);


      }
    else if(pwm_sig == 0){

    	HAL_GPIO_WritePin (p_mot->port_in_1, p_mot->in_pin_1, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin (p_mot->port_in_2, p_mot->in_pin_2, GPIO_PIN_RESET);

    }
    else {

    	//backwards
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->pwm_channel,-pwm_sig);

    	//set GPIO PINS

    	HAL_GPIO_WritePin (p_mot->port_in_1, p_mot->in_pin_1, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin (p_mot->port_in_2, p_mot->in_pin_2, GPIO_PIN_SET);

    }


}

/**
 * @brief A function to set the standby pin for the motor.
 *
 * @param p_mot The motor object to perform the function on.
 * @param stby The desired stby pin value of 0 or 1.
 */
void set_stby(motor_t* p_mot, int32_t stby_val){
	if(stby_val == 0){

		HAL_GPIO_WritePin (p_mot->port_stby_pin, p_mot->stby_pin, GPIO_PIN_RESET);
	}
	else{

		HAL_GPIO_WritePin (p_mot->port_stby_pin, p_mot->stby_pin, GPIO_PIN_SET);
	}

}

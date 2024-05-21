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


void start_PWM(motor_t* p_mot, uint32_t channel)
{
	/* A function to enable one of the motor driver channels.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param channel The timer channel to perform the function on.
	 *
	 *  */

	if (channel==1){

		HAL_TIM_PWM_Start(p_mot->hal_tim, p_mot->channel1);
	}
	else{
		HAL_TIM_PWM_Start(p_mot->hal_tim, p_mot->channel2);
	}

}


void stop_PWM(motor_t* p_mot, uint32_t channel)
{
	/* A function to disable one of the motor driver channels.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param channel The timer channel to perform the function on.
	 *
	 */

	if (channel==1){

			HAL_TIM_PWM_Stop(p_mot->hal_tim, p_mot->channel1);
		}
		else{
			HAL_TIM_PWM_Stop(p_mot->hal_tim, p_mot->channel2);
		}

}

void set_duty(motor_t* p_mot, int32_t duty)
{
	/* A function to set the duty cycle for the motor.
	 *
	 * @param p_mot The motor object to perform the function on.
	 *
	 * @param duty The desired duty cycle for the motor.
	 */

    // Print to the console so we can see what's happening
    printf("Setting Motor Duty Cycle to %ld", duty);

    // Assign the duty cycle to a field in the structure
    p_mot->duty = duty;

    // The arrow operator is shorthand for
    // dereferencing and accessing struct fields
    // p_mot->duty = duty;
    // is shorthand for
    // (*p_mot).duty = duty;

    if (duty > 0) {

    	//forwards
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel1,duty);
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel2,0);

      }
    else if(duty == 0){

    	//to turn off put into braking mode by setting both pins high
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel1,640);
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel2,640);

    }
    else {

    	//backwards
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel1,0);
    	__HAL_TIM_SET_COMPARE(p_mot->hal_tim, p_mot->channel2,-duty);
      }


}


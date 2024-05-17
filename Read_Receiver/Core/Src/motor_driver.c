/*
 * motor_driver.c
 *
 *  Created on: Apr 18, 2024
 *      Author: Julia
 */

#include "motor_driver.h"

//A function to enable one of the motor driver channels
void start_PWM(motor_t* p_mot, uint32_t channel)
{
	if (channel==1){

		HAL_TIM_PWM_Start(p_mot->hal_tim, p_mot->channel1);
	}
	else{
		HAL_TIM_PWM_Start(p_mot->hal_tim, p_mot->channel2);
	}

}

//A function to disable one of the motor driver channels
void stop_PWM(motor_t* p_mot, uint32_t channel)
{
	if (channel==1){

			HAL_TIM_PWM_Stop(p_mot->hal_tim, p_mot->channel1);
		}
		else{
			HAL_TIM_PWM_Stop(p_mot->hal_tim, p_mot->channel2);
		}

}

void set_duty(motor_t* p_mot, int32_t duty)
{
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

/*
 * controller.h
 *
 *  Created on: May 18, 2024
 *      Author: julia
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

struct {

	//These are all the things you have to define when you create a motor object
	    int32_t  duty;

	    //two channels for each motor
	    uint32_t channel1;
	    uint32_t channel2;

	    //The handle to the HAL timer object used for motor PWM generation. Include * so its a pointer to the object
	    TIM_HandleTypeDef* hal_tim;

	//These are all the things you have to define when you create an encoder object




	//These are all the things specific to the controller class

	    int32_t gain;
	    int32_t setpoint;




} typedef controller_t;

void run();

void set_setpoint();

void set_Kp();

void zero();

#endif /* INC_CONTROLLER_H_ */

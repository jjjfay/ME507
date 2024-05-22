/*
 * controller.c
 *
 *  Created on: May 18, 2024
 *      Author: julia
 */

#include "controller.h"
#include "motor_driver.h"
#include "encoder_reader.h"

/**
 * @brief A function to move the controlled motor to the desired position. The run
 * 		  function in the P_Control class calculates the PWM signal that will be
 *        sent to the motor by taking into account the setpoint of the motor, and
 *        the current position of the motor. These values are subtracted to find the
 *        error which is then multiplied by Kp. If this value exceeds -100 or 100, the
 *        value is saturated to either -100 or 100. Then the set_duty_cycle function
 *        imported from the motor_driver class is run. The calculated PWM signal is
 *        returned at the end of the function.
 *
 * @param p_cont The controller object to perform the function on.
 */

/**
 * @brief A function to initialize all of the timer channels.
 *
 * @param p_cont The controller object to perform the function on.
 */
void controller_init(controller_t* p_cont){

	//initialize the motor driver channels
	start_PWM(&p_cont->p_mot,&p_cont->p_mot.channel1);
	start_PWM(&p_cont->p_mot,&p_cont->p_mot.channel2);

	//initialize the encoder reader channels
	init_channels(&p_cont->p_enc);
}

/**
 * @brief A function to de-initialize all of the timer channels.
 *
 * @param p_cont The controller object to perform the function on.
 */
void controller_deinit(controller_t* p_cont){

	//de-initialize the motor driver channels
	stop_PWM(&p_cont->p_mot,&p_cont->p_mot.channel1);
	stop_PWM(&p_cont->p_mot,&p_cont->p_mot.channel2);

	//de-initialize the encoder reader channels
	deinit_channels(&p_cont->p_enc);
}

/**
 * @brief A function to move the controlled motor to the desired position.
 *
 * @param p_cont The controller object to perform the function on.
 */
void move(controller_t* p_cont){


	int32_t calc = 100;

	p_cont->p_mot.duty = calc;
	set_duty(&p_cont->p_mot, &p_cont->p_mot.duty);

}

/**
 * @brief A function to set the new controller set point.
 *
 * @param p_cont The controller object to perform the function on.
 * @param new_setpoint The new set point for the controller object.
 */
void set_setpoint(controller_t* p_cont, int32_t new_setpoint){

	p_cont->setpoint = new_setpoint;
}


/**
 * @brief A function to update the control loop gain.
 *
 * @param p_cont The controller object to perform the function on.
 * @param new_gain The new set point for the controller object.
 */
void set_K(controller_t* p_cont){

	p_cont->gain = new_gain;
}

/**
 * @file controller.h
 * @brief Defines the controller struct and its methods.
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_
#include "motor_driver.h"
#include "encoder_reader.h"

/**
 * @brief Represents a controller object that controls a motor based on the encoder reading.
 */
struct {

	motor_t* p_mot; /**< The motor object to be controlled. */
	encoder_t* p_enc; /**< The encoder to be read from. */

	int32_t gain; /**< The desired control loop gain. */
	int32_t setpoint; /**< The desired set point for the motor. */


} typedef controller_t;

/**
 * @brief A function to initialize all of the timer channels.
 *
 * @param p_cont The controller object to perform the function on.
 */
void controller_init(controller_t* p_cont);

/**
 * @brief A function to de-initialize all of the timer channels.
 *
 * @param p_cont The controller object to perform the function on.
 */
void controller_deinit(controller_t* p_cont);

/**
 * @brief A function to move the controlled motor to the desired position. The function
 * 		  calculates the PWM signal that will be sent to the motor by taking into account
 * 		  the setpoint of the motor, and the current position of the motor. These values
 * 		  are subtracted to find the error which is then multiplied by Kp. If this value
 * 		  exceeds -3,999 or 3,999, the value is saturated to either -3,999 or 3,999. The
 * 		  function also sets a minimum threshold for when a PWM signal is generated. Anything
 * 		  below 10 times the gain value is considered to be a PWM signal of zero. Then
 * 		  the set_duty function imported from the motor_driver class is run. The calculated
 * 		  PWM signal is returned at the end of the function.
 *
 * @param p_cont The controller object to perform the function on.
 *
 * @return pwm_sig The pwm value calculated from the closed loop proportional control.
 */
int32_t move(controller_t* p_cont, int32_t gain);

/**
 * @brief A function to set the new controller set point.
 *
 * @param p_cont The controller object to perform the function on.
 * @param new_setpoint The new set point for the controller object.
 */
void set_setpoint(controller_t* p_cont, int32_t new_setpoint);

/**
 * @brief A function to update the control loop gain.
 *
 * @param p_cont The controller object to perform the function on.
 * @param new_gain The new set point for the controller object.
 */
void set_K(controller_t* p_cont, int32_t new_gain );


#endif /* INC_CONTROLLER_H_ */

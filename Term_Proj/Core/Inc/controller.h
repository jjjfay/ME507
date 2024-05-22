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

	motor_driver p_mot; /**< The motor object to be controlled. */
	encoder_reader p_enc; /**< The encoder to be read from. */

	//These are all the things specific to the controller class

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
 * @brief A function to move the controlled motor to the desired position.
 *
 * @param p_cont The controller object to perform the function on.
 */
void move(controller_t* p_cont);

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

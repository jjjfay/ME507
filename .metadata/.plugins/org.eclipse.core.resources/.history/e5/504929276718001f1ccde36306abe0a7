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

void run();

void set_setpoint();

void set_Kp();

void zero();

#endif /* INC_CONTROLLER_H_ */

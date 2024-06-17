/*
 * mainpage.h
 *
 *  Created on: May 22, 2024
 *      Author: julia
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/**
 * @mainpage Prosthetic Hand Control through Myoelectric and Pressure Sensors
 *
 * @authors Julia Fay & Jack Foxcroft
 *
 * @section description Project Description
 *
 * Our project is centered around the control of an electromechanically actuated prosthetic hand manufactured
 * by Australian orthopedic and prosthetic design company Ottobock donated to the EMPOWER Student Association.
 * The goal is to create a proof-of-concept control system to actuate the prosthetic hand via myoelectric sensor
 * input. Our design implements two myoelectric sensors, two brushed DC motors, and one rotary encoder. The hand
 * can rotate about its central axis and open/close, controlled by two myoelectric sensors. The design also includes
 * an emergency stop via radio transmitter for safety purposes. The selected MCU was the STM32L476RGT6 due to our
 * familiarity with this chip from previous quarters and its number of ADC’s and timer channels. The final product
 * of this project did not meet all the goals outlined for the project due to issues with the PCB and hardware. During
 * testing, our custom PCB’s power rail broke down, so a NUCLEO development board was used. Additionally, the myoelectric
 * sensors stopped functioning correctly during testing and no longer produced a consistently readable output. Thus,
 * instead of using muscle actuation to move the motors, the gain on the sensors were manually increased and decreased
 * by hand to force the expected output to be interpreted by our program. Lastly, the motor on the prosthetic hand initially
 * selected to use for this project was nonfunctional, so an alternative hand without an encoder was used, affecting the
 * controllability of the hand movement. An image of the final design is shown below.
 *
 * \image html images/Final_Design.png "Final Myoelectric Control System Design Used for Project Demonstration"
 * \image latex images/Final_Design.png "Final Myoelectric Control System Design Used for Project Demonstration"
 *
 *
 * The pressure sensor we selected is a simple and cost-effective part from amazon. It is a thin film
 * sensor that will easily stick onto the thumb of our gripping device. The sensor only has two wire
 * connections. One wire is the input, which will be supplied and set to 1V with the use of a resistor.
 * The second wire is the output, which will be an input to one of the ADC’s on the microcontroller.
 *
 * The DC motor we selected is also a simple and cost-effective part. It is a 12V DC motor with an encoder
 * with a top speed of 100 RPM. A convenient feature of this motor is the shaft output is perpendicular to
 * the body of the motor allowing for easy attachment to our design. The motor requires a 3.3-5V power supply.
 *
 * Overall, our design meets all the rules and requirements of the project. We have two actuators, and two
 * unique sensors including the spin motor encoder and the myo electric sensors. All our hardware was purchased,
 * or 3D printed.Our design is safe for users and bystanders as it will be a mainly stationary device that rotates and
 * moves within a confined area as it will be attached to a steady base. We used the remote controller as
 * a emergency shut-off switch for our design.
 *
 */

#endif /* SRC_MAINPAGE_H_ */

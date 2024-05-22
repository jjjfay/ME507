/*
 * mainpage.h
 *
 *  Created on: May 22, 2024
 *      Author: julia
 */

#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_

/*
 * @mainpage Prosthetic Hand Control through Myoelectric and Pressure Sensors
 *
 * @authors Julia Fay & Jack Foxcroft
 *
 * @section Project Description
 *
 * For our project, we will be controlling a robotic gripping device with a pressure sensor
 * and myoelectric sensors. The robotic hand will be able to rotate about its central axis,
 * controlled by one myoelectric sensor, and open and close, controlled by the second myoelectric
 * sensor. The pressure sensor will be attached to the thumb of the gripping device and will make
 * sure the grip force stays within a desired threshold. This project is centered around a prosthetic
 * hand device donated to the Empower club. This device was created by a biomedical company called
 * Ottobock and features a three fingered mechanical hand controlled by a DC motor with an encoder.
 *
 * The myoelectric sensors was purchased from sparkfun. The MyoWare 2.0 Muscle Sensor was selected
 * because it was created for the specific purpose of reading electric signals from muscle movements.
 * Thus, it features several output types for easy interpretation of the already filtered signal.
 * Also, this sensor requires a minimum supply of 3.3 volts, which can be easily supplied by the
 * microcontroller.
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
 * unique sensors in addition to the encoders on each motor. All our hardware was purchased, or 3D printed.
 * Our design is safe for users and bystanders as it will be a mainly stationary device that rotates and
 * moves within a confined area as it will be attached to a steady base. We used the remote controller as
 * a emergency shutoff switch for our design.
 *
 */

#endif /* SRC_MAINPAGE_H_ */

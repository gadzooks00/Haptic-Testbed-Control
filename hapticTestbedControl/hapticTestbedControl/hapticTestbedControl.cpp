/*
File: hapticTestbedControl.cpp
________________________________
Author(s): Zane Zook (gadzooks@rice.edu)

This file send messages using the MAXON motor controller 
EPOS library to the EPOS controllers and measures 
forces/torques from the NI DAQ simultaneously. Hardware 
used with this program include: 2 custom MAXON motors, 
2 EPOS4 24/1.5 CAN Motor controllers, 2 Nano 25 ATI
Force/Torque Sensors, and 1 NI DAQ for the force sensors.
*/
 

/***********************************************************
******************** LIBRARY IMPORT ************************
************************************************************/
#include "pch.h"
#include <stdio.h>
#include <iostream>
#include "Definitions.h"
#include "maxonMotor.h"

/***********************************************************
******************** SETUP FUNCTIONS ***********************
************************************************************/
void motorInitialize(maxonMotor &motor, char* portName)
{
	// define relevant parameters for the controller
	unsigned int desVel = 9500;
	unsigned int desAcc = 10000;
	unsigned int desDec = 10000;
	
	// set important parameters of the motor
	motor.setPort(portName);
	motor.setControlParam(desVel, desAcc, desDec);

	// activate motor controller
	motor.start();
}

/***********************************************************
********************* MAIN FUNCTION ************************
************************************************************/
int main()
{
	// create new motor objects
	maxonMotor motorA, motorB;

	// define USB port for each controller
	char* portNameA = (char*)"USB1";
	char* portNameB = (char*)"USB0";

	// initialize motors
	motorInitialize(motorA, portNameA);
	motorInitialize(motorB, portNameB);

	// get motor initial position
	long posA, posB;
	motorA.getPosition(posA);
	motorB.getPosition(posB);

	return EXIT_SUCCESS;
}
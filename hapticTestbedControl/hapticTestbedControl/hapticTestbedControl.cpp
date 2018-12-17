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
using namespace std;

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

	// print out current motor position
	cout << "Motor A's current position is: ";
	cout << posA << endl;
	cout << "Motor B's current position is: ";
	cout << posB << endl;

	// test motor A
	cout << endl;
	cout << "Motor A Movement Test" << endl;
	cout << "press any key to continue..." << endl;
	cin.get();
	motorA.move(40000);

	cout << endl;
	cout << "Motor A movement to 40000" << endl;
	cout << "press any key to return..." << endl;
	cin.get();
	motorA.move(0);

	// test motor B
	cout << endl;
	cout << "Motor B Movement Test" << endl;
	cout << "press any key to continue..." << endl;
	cin.get();
	motorB.move(40000);

	cout << endl;
	cout << "Motor B movement to 40000" << endl;
	cout << "press any key to return..." << endl;
	cin.get();
	motorB.move(0);

	return EXIT_SUCCESS;
}
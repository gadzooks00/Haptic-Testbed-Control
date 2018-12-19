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
#include <fstream>
#include "Definitions.h"
#include "maxonMotor.h"

using namespace std;
using namespace ATICombinedDAQFT;


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
	// create new sensors
	FTSystem sensorA, sensorB;

	// load sensor calibration files
	sensorA.LoadCalibrationFile("FT26062.cal", 1);
	sensorB.LoadCalibrationFile("FT26061.cal", 1);

	// set sensor units
	sensorA.SetForceUnits("N");
	sensorA.SetTorqueUnits("Nm");
	sensorB.SetForceUnits("N");
	sensorB.SetTorqueUnits("Nm");

	// bias sensor A data
	sensorA.StartSingleSampleAcquisition("dev1", 1000, 10, 0, FALSE);
	sensorA.BiasCurrentLoad();
	
	// bias sensor B data
	sensorB.StartSingleSampleAcquisition("dev1", 1000, 10, 16, FALSE);
	sensorB.BiasCurrentLoad();


	// print force data
	//sensorA.StartBufferedAcquisition("dev1", 1000, 10, 0, FALSE, 100);
	//while (true)
	//{
	//	cli::array<double, 1>^ readingsA = gcnew cli::array<double, 1>(600);
	//	sensorA.ReadBufferedFTRecords(100, readingsA);
	//	for (int i = 0; i < 100; i++)
	//	{
	//		cout << "Fx=" << readingsA[i] << ", ";
	//		cout << "Fy=" << readingsA[i + 1] << ", ";
	//		cout << "Fz=" << readingsA[i + 2] << ", ";
	//		cout << "Tx=" << readingsA[i + 3] << ", ";
	//		cout << "Ty=" << readingsA[i + 4] << ", ";
	//		cout << "Tz=" << readingsA[i + 5] << endl;
	//	}
	//}
	//sensorA.StopAcquisition();

	 
	// create new motors
	maxonMotor motorA, motorB;

	// define USB port for each controller
	char* portNameA = (char*)"USB1"; 
	char* portNameB = (char*)"USB0";

	// initialize motors
	motorInitialize(motorA, portNameA);
	motorInitialize(motorB, portNameB);
	   

	// preparing the test output file
	ofstream testOutput;
	testOutput.open("testOutput.csv");
	testOutput << "This file holdes the output for the motor test..." << endl;
	testOutput << "Samples, DesPosA, ActPosA, FxA, FyA, FzA, TxA, TyA, TzA, ";
	testOutput << "DesPosB, ActPosB, FxB, FyB, FzB, TxB, TyB, TzB" << endl;

	// preparing relevant variables
	cli::array<double, 1>^ readingsA = gcnew cli::array<double, 1>(6);
	cli::array<double, 1>^ readingsB = gcnew cli::array<double, 1>(6);
	long posA, posB;
	int testPos = 40000;

	// asking user to begin motor test
	cout << endl;
	cout << "Motor A Movement Test" << endl;
	cout << "press any key to continue..." << endl;
	cin.get();

	//initial sample
	int sample = 0;
	// measuring force/torque sensors
	sensorA.ReadSingleFTRecord(readingsA);
	sensorB.ReadSingleFTRecord(readingsB);
	// measuring position data
	motorA.getPosition(posA);
	motorB.getPosition(posB);
	// recording output
	testOutput << sample << "," << testPos << "," << posA << "," << readingsA[0] << "," <<
		readingsA[1] << "," << readingsA[2] << "," << readingsA[3] << "," <<
		readingsA[4] << "," << readingsA[5] << "," <<
		0 << "," << posB << "," << readingsB[0] << "," <<
		readingsB[1] << "," << readingsB[2] << "," << readingsB[3] << "," <<
		readingsB[4] << "," << readingsB[5] << endl;
	// increment sample count
	sample = sample + 1;

	// starting motor A
	motorA.move(testPos);	
	// measuring data while motor is running
	while (!motorA.targetReached())
	{
		// measuring force/torque sensors
		sensorA.ReadSingleFTRecord(readingsA);
		sensorB.ReadSingleFTRecord(readingsB);

		// measuring position data
		motorA.getPosition(posA);
		motorB.getPosition(posB);

		// recording output
		testOutput << sample << "," << testPos << "," << posA << "," << readingsA[0] << "," <<
			readingsA[1] << "," << readingsA[2] << "," << readingsA[3] << "," <<
			readingsA[4] << "," << readingsA[5] << "," <<
			0 << "," << posB << "," << readingsB[0] << "," <<
			readingsB[1] << "," << readingsB[2] << "," << readingsB[3] << "," <<
			readingsB[4] << "," << readingsB[5] << endl;

		// increment sample count
		sample = sample + 1;
	}

	// post movement sample
	// measuring force/torque sensors
	sensorA.ReadSingleFTRecord(readingsA);
	sensorB.ReadSingleFTRecord(readingsB);
	// measuring position data
	motorA.getPosition(posA);
	motorB.getPosition(posB);
	// recording output
	testOutput << sample << "," << testPos << "," << posA << "," << readingsA[0] << "," <<
		readingsA[1] << "," << readingsA[2] << "," << readingsA[3] << "," <<
		readingsA[4] << "," << readingsA[5] << "," <<
		0 << "," << posB << "," << readingsB[0] << "," <<
		readingsB[1] << "," << readingsB[2] << "," << readingsB[3] << "," <<
		readingsB[4] << "," << readingsB[5] << endl;
	// increment sample count
	sample = sample + 1;

	// asking user to return the motor
	cout << endl;
	cout << "Motor A movement to 40000" << endl;
	cout << "press any key to return..." << endl;
	cin.get();

	// returning motorA
	motorA.move(0);

	// measuring data while motor is running
	while (!motorA.targetReached())
	{
		// measuring force/torque sensors
		sensorA.ReadSingleFTRecord(readingsA);
		sensorB.ReadSingleFTRecord(readingsB);

		// measuring position data
		motorA.getPosition(posA);
		motorB.getPosition(posB);

		// recording output
		testOutput << sample << "," << 0 << "," << posA << "," << readingsA[0] << "," <<
			readingsA[1] << "," << readingsA[2] << "," << readingsA[3] << "," <<
			readingsA[4] << "," << readingsA[5] << "," <<
			0 << "," << posB << "," << readingsB[0] << "," <<
			readingsB[1] << "," << readingsB[2] << "," << readingsB[3] << "," <<
			readingsB[4] << "," << readingsB[5] << endl;


		// increment sample count
		sample = sample + 1;
	}

	// stops acquisition from sensors
	sensorA.StopAcquisition();
	sensorB.StopAcquisition();

	// closes the .csv files
	testOutput.close();
	
	return EXIT_SUCCESS;
}
/***********************************************************
maxonMotor.cpp : This file defines the Maxon Motor class
which holds all the lower level commands sent to the 
Maxon controllers in the system. This specific version is
customized to work with the EPOS4 controller but it can be 
modified to work with other controllers

Author(s): Zane Zook (gadzooks@rice.edu) 
************************************************************/


/***********************************************************
******************** LIBRARY IMPORT ************************
************************************************************/
#include "pch.h"
#include "maxonMotor.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;


/***********************************************************
********************** CONSTRUCTOR *************************
************************************************************/
maxonMotor::maxonMotor()
{
	portName = (char*)"USB0";
	nodeId = 1;
	errorCode = 0;
}

maxonMotor::~maxonMotor()
{
}

/***********************************************************
******************* PRIVATE FUNCTIONS **********************
************************************************************/
void* maxonMotor::activate(char *portName)
{
	// Configuring EPOS4 for motor control
	char deviceName[] = "EPOS4";
	char protocolName[] = "MAXON SERIAL V2";
	char interfaceName[] = "USB";
	void* keyHandle;

	keyHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);

	if (keyHandle == 0)
	{
		cout << "Open device failure, error code=" << errorCode << endl;
		exit(0);
	}
	else
	{
		cout << "Open device success!" << endl;
	}

	return keyHandle;
}

/***********************************************************
******************* PUBLIC FUNCTIONS ***********************
************************************************************/
void maxonMotor::activate()
{
	keyHandle = activate(portName);
}

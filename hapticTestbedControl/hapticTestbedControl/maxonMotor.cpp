/*
File: maxonMotor.cpp
________________________________
Author(s): Zane Zook (gadzooks@rice.edu)

This file defines the Maxon Motor class which holds all the
lower level commands sent to the Maxon controllers in the 
system. This specific version is customized to work with 
the EPOS4 controller but it can be modified to work with 
other controllers
*/


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

/*
Constructor for the vectorPatientQueue class
 */
maxonMotor::maxonMotor()
{
	portName = (char*)"USB0";
	nodeId = 1;
	errorCode = 0;
	keyHandle = 0;

	desVelocity = 9900;
	desAcceleration = 10000000;
	desDeceleration = 10000000;
}

/*
Destructor for the vectorPatientQueue class
 */
maxonMotor::~maxonMotor()
{
}



/***********************************************************
******************* PRIVATE FUNCTIONS **********************
************************************************************/

/*
Once the device has been opened, attempts to set the
controller into position control mode.
 */
void maxonMotor::enableControl()
{
	BOOL inFault = FALSE;

	// checks the controller for any faults
	if (VCS_GetFaultState(keyHandle, nodeId, &inFault, &errorCode))
	{
		// attempts to clear the fault from the controller if in a fault
		if (inFault && !VCS_ClearFault(keyHandle, nodeId, &errorCode))
		{
			cout << "Clear fault failed!, error code = " << errorCode << endl;
			return;
		}

		BOOL enabled = FALSE;

		// attempts to enable controller
		if (VCS_GetEnableState(keyHandle, nodeId, &enabled, &errorCode))
		{
			if (!enabled && !VCS_SetEnableState(keyHandle, nodeId, &errorCode))
			{
				cout << "Set enable state failed!, error code = " << errorCode << endl;
			}
			else
			{
				cout << "Set enable state succeeded!" << endl;
			}
		}
	}
	else
	{
		cout << "Get fault state failed!, error code = " << errorCode << endl;
	}

	// attempts to set controller to position control mode
	if (!VCS_ActivateProfilePositionMode(keyHandle, nodeId, &errorCode)) {
		cout << "Activate profile position mode failed!" << endl;
	}

	// sets the controller's control parameters
	setControlParam();
}

/*
Sets the position control parameters once the device
has been enabled
 */
void maxonMotor::setControlParam()
{
	// attempts to set the controllers' position control parameters
	if (!VCS_SetPositionProfile(keyHandle, nodeId, desVelocity, desAcceleration, desDeceleration, &errorCode)) {
		cout << "VCS_SetPositionProfile failed!, error code = " << errorCode << endl;
	}
	cout << "Position control parameters updated!" << endl;
}

/*
Turns off the position control on the controller
 */
void maxonMotor::disableControl()
{
	BOOL inFault = FALSE;

	// checks the controller for any faults
	if (VCS_GetFaultState(keyHandle, nodeId, &inFault, &errorCode))
	{
		// attempts to clear the fault from the controller if in a fault
		if (inFault && !VCS_ClearFault(keyHandle, nodeId, &errorCode))
		{
			cout << "Clear fault failed!, error code = " << errorCode << endl;
			return;
		}

		BOOL enabled = FALSE;

		// attempts to disable controller
		if (VCS_GetEnableState(keyHandle, nodeId, &enabled, &errorCode))
		{
			if (enabled && !VCS_SetDisableState(keyHandle, nodeId, &errorCode))
			{
				cout << "Set disable state failed!, error code = " << errorCode << endl;
			}
			else
			{
				cout << "Set disable state succeeded!" << endl;
			}
		}
	}
	else
	{
		cout << "Get fault state failed!, error code = " << errorCode << endl;
	}
}


/***********************************************************
************* DEVICE CONNECTION FUNCTIONS ******************
************************************************************/
/*
Opens communication from the computer to the specific 
controller being referenced through USB comms.
 */
void maxonMotor::start()
{
	// Configuring EPOS4 for motor control
	char deviceName[] = "EPOS4";
	char protocolName[] = "MAXON SERIAL V2";
	char interfaceName[] = "USB";

	// Opens device communication
	keyHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);
	if (keyHandle == 0)
	{
		cout << "Open device failure, error code = " << errorCode << endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		cout << "Open device success!" << endl;
	}

	// Enables device in position control mode
	enableControl();
}

/*
Closes communication from the computer to the specific
controller being referenced through USB comms.
 */
void maxonMotor::end()
{
	// turns off position control
	disableControl();

	cout << "Closing Device!" << endl;

	// closes communication with controller
	if (keyHandle != 0)
	{
		VCS_CloseDevice(keyHandle, &errorCode);
	}
	VCS_CloseAllDevices(&errorCode);
}


/***********************************************************
************** DEVICE PARAMETER FUNCTIONS ******************
************************************************************/
/*
Indicates what USB port the MAXON motor controller is 
connected to.
 */
void maxonMotor::setPort(char* port)
{
	portName = port;
}

/*
Sets each one of the control parameters for position 
control mode
 */
void maxonMotor::setControlParam(unsigned int desVel, unsigned int desAcc, unsigned int desDec)
{
	desVelocity = desVel;
	desAcceleration = desAcc;
	desDeceleration = desDec;
}


/***********************************************************
****************** MOVEMENT FUNCTIONS **********************
************************************************************/

/*
Commands motor controller to move motor to specified position
 */
void maxonMotor::move(long desPosition)
{
	BOOL Absolute = TRUE; 
	BOOL Immediately = TRUE;

	// sends signal to move Maxon motor to specified position
	if (!VCS_MoveToPosition(keyHandle, nodeId, desPosition, Absolute, Immediately, &errorCode)) {
		cout << "Move to position failed!, error code = " << errorCode << endl;
		halt();
	}
}

/*
Pings motor for its current position
 */
void maxonMotor::getPosition(long& position)
{
	// attempts to acquire current position of the motor
	if (!VCS_GetPositionIs(keyHandle, nodeId, &position, &errorCode)) {
		cout << " error while getting current position , error code = " << errorCode << endl;
	}
}

/*
Pings motor to stop
 */
void maxonMotor::halt()
{
	// attempts to stop motor in its place
	if (!VCS_HaltPositionMovement(keyHandle, nodeId, &errorCode))
	{
		cout << "Halt position movement failed!, error code = " << errorCode << endl;
	}
}
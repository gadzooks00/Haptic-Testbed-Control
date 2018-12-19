/*
File: maxonMotor.h
________________________________
Author(s): Zane Zook (gadzooks@rice.edu)

This is the header file for the Maxon Motor class. This 
class holds all the lower level commands sent to the Maxon
controllers. This specific version is customized to work 
with the EPOS4 controller but it can be modified to work 
with other controllers
*/

#ifndef MAXONMOTOR
#define MAXONMOTOR

/***********************************************************
******************** LIBRARY IMPORT ************************
************************************************************/
#include "Definitions.h" // Maxon controller header file


/***********************************************************
****************** CLASS DECLARATION ***********************
************************************************************/
class maxonMotor
{
private:
	// device variable
	char* portName;
	DWORD errorCode;
	byte nodeId;
	void* keyHandle;

	// control parameter variables
	unsigned int desVelocity;
	unsigned int desAcceleration;
	unsigned int desDeceleration;

	// device connection functions
	void enableControl();
	void disableControl();

	// device parameter functions
	void setControlParam();

	// movement functions
	void halt();

public:
	// constructor
	maxonMotor();
	~maxonMotor();

	// device connection functions
	void start();
	void end();

	// device parameter functions
	void setPort(char* port);
	void setControlParam(unsigned int desVel, unsigned int desAcc, unsigned int desDec);

	// movement functions
	void move(long desPosition);
	void getPosition(long& position);
	BOOL targetReached();
};
#endif MAXONMOTOR
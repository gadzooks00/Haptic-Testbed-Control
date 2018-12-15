/***********************************************************
maxonMotor.h : This is the header file for the Maxon Motor 
class. This class holds all the lower level commands sent
to the Maxon controllers. This specific version is 
customized to work with the EPOS4 controller but it can be
modified to work with other controllers

Author(s): Zane Zook (gadzooks@rice.edu)
************************************************************/


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
	char* portName;
	DWORD errorCode;
	unsigned short nodeId;
	void* keyHandle;

public:
	maxonMotor();
	~maxonMotor();

	void activate();
	void setPort(char* port);
};
#endif MAXONMOTOR
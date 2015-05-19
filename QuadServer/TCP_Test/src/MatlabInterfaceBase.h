/*
 * MatlabInterfaceBase.h
 *
 *  Created on: 03.04.2014
 *      Author: Werner Breitwieser
 */

#ifndef MATLABINTERFACEBASE_H_
#define MATLABINTERFACEBASE_H_

#include <cstdlib>
#include <cstdint>
#include "ThreadBase.h"

// Abstract Class for Matlab.
class MatlabInterfaceBase : public ThreadBase
{
public:
	// Connection state
	bool IsConnected() const;

	// Virtual DTor
	virtual ~MatlabInterfaceBase();

protected:
	// CTor
	MatlabInterfaceBase(int intervalMs);

	// Connect to Matlab
	virtual bool WaitForConnection() = 0;

	// Send Data to Matlab
	virtual bool SendToMatlab() = 0;

	// Thread-Function
	void Run();

	// Close Connection to Matlab
	virtual void CleanUp() = 0;


//////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////
public:

	union
	{
		struct TValues {
			std::uint16_t 	RawGyro[3];
			double 			Gyro[3];
			double 			FilteredGyro[3];
			std::uint16_t 	RawAcc[3];
			double 			Acc[3];
			double 			FilteredAcc[3];
			double 			Angle[3];
			double 			Thrust;
			double 			Tau[3];
			double 			TauSat[3];
			double 			SetPointAngular[3];
			double 			Omega[4];
			std::uint8_t 	MotorSpeed[4];
			double 			CycleTime;
			double 			Voltage;
		}  __attribute__((packed)) Values;
		std::uint8_t ByteArr[sizeof(TValues)];
	} Matlab;
	// Variables for Matlab

private:
	// Flags
	bool mIsConnected;

	// Interval for connection in ms
	int mIntervalMs;
};



#endif /* MATLABINTERFACEBASE_H_ */

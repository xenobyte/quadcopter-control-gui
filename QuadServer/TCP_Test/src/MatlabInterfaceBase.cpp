/*
 * MatlabInterfaceBase.cpp
 *
 *  Created on: 03.04.2014
 *      Author: Werner Breitwieser
 */

#include "MatlabInterfaceBase.h"
#include <unistd.h>
#include <iostream>

bool MatlabInterfaceBase::IsConnected() const
{
	return mIsConnected;
}

// DTor
MatlabInterfaceBase::~MatlabInterfaceBase() {}

// CTor
MatlabInterfaceBase::MatlabInterfaceBase(int intervalMs) : mIsConnected(false), mIntervalMs(intervalMs) {}


void MatlabInterfaceBase::Run()
{
	std::cout << "run..." << std::endl;
	// returns false if there was an error
	while (WaitForConnection())
	{
		mIsConnected = true;

		// send data cyclic
		while(SendToMatlab())
		{
			// Data successfully sent
			usleep(1000 * mIntervalMs);
		}

		mIsConnected = false;
	}
}

/*
 * MatlabInterfaceTCP.cpp
 *
 *  Created on: 03.04.2014
 *      Author: Werner
 */

#include <iostream>

#include "TCP_TestConnection.h"
#include "MatlabInterfaceTCP.h"



// CTor
MatlabInterfaceTCP::MatlabInterfaceTCP(int port, int intervalMs) : MatlabInterfaceBase(intervalMs), matlabLog(LOG.createLogger("MatlabInterfaceTCP")), mPort(port)
{
	// bind to given port number
    mServer.bind(mPort);

	// listen to max 1 connection
    mServer.listen(1);

	// blocking wait for connection
    mServer.set_blocking(true);
}

// DTor
MatlabInterfaceTCP::~MatlabInterfaceTCP()
{
}

bool MatlabInterfaceTCP::WaitForConnection()
{
    if (mClient)
        mClient->close();

	mClient.reset(mServer.accept());

	if (mClient != NULL)
	{
		mClient->set_blocking(true);
		matlabLog->info("client connected!");
	}
	else
	{
		matlabLog->info("some error in WaitForConnection()!");
	}

	// mClient
	return (mClient != NULL);
}

bool MatlabInterfaceTCP::SendToMatlab()
{
	if (mClient != NULL)
	{
		if (mClient->write(Matlab.ByteArr, sizeof(Matlab.ByteArr)) == eSuccess)
            return true;
        else {
            matlabLog->error("Write Error");
            return false;
        }
	}
	return false;
}

void MatlabInterfaceTCP::CleanUp()
{
    matlabLog->info("CleanUp");
    if (mClient)
        mClient->close();

    mServer.close();
}

MatlabInterfaceTCP& MatlabInterfaceTCP::GetInstance()
{
	static MatlabInterfaceTCP instance;
	return instance;
}

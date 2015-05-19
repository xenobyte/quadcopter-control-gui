/*
 * MatlabInterfaceTCP.h
 *
 *  Created on: 03.04.2014
 *      Author: Werner
 */

#ifndef MATLABINTERFACETCP_H_
#define MATLABINTERFACETCP_H_

#include "SocketCommunication.h"
#include "MatlabInterfaceBase.h"

class MatlabInterfaceTCP : public MatlabInterfaceBase
{
public:
	// simple singleton
	static MatlabInterfaceTCP& GetInstance();

	// virtual DTor
	~MatlabInterfaceTCP();

protected:
	// CTor
	MatlabInterfaceTCP(int port = 2345, int intervalMs = 10);

	// Create Socket and wait for connection
	virtual bool WaitForConnection();

	// Send Data via Socket to Matlab
	virtual bool SendToMatlab();

	// Close Connection and Cleanup
	virtual void CleanUp();

private:

	// Server Socket
	ServerSocket mServer;

	UnqClientSocket mClient;
    UnqLogger matlabLog;

	// Port Nr
	int mPort;
};


#endif /* MATLABINTERFACETCP_H_ */

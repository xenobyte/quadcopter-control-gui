/*
 * TCP_TestConnection.cpp
 *
 *  Created on: 17.05.2015
 *      Author: Johannes Selymes
 */

#include "ParameterUpdateThread.h"

using namespace std;

Logging LOG("TCP_TestConnection.log");

int main(int argc, char* argv[]) {
	int port = 1234;
	ParameterUpdateThread param_thread(port);
	param_thread.Start();

	while(true){}
}


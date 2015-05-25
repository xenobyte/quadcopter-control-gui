/*
 * TCP_TestConnection.cpp
 *
 *  Created on: 17.05.2015
 *      Author: Johannes
 */

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sched.h>
#include <time.h>
#include <cstdlib>
#include <string.h>
#include "ParameterUpdateThread.h"

using namespace std;

Logging LOG("TCP_TestConnection.log");
ServerSocket SERVER; // only tcp server

void PrintBuffer(unsigned char* buf, uint32_t len){
	for (uint i = 0; i < len; i++) {
			cout << buf[i];
		}
		cout << endl;
}

int main(int argc, char* argv[]) {
//	cout << "listening ..." << endl;
//	SERVER.bind(1234);
//	SERVER.listen(1);
//	ClientSocket *sock = SERVER.accept();
//
//	uint32_t bufferSize = 200;
//	unsigned char readBuffer[bufferSize];
//	TReadStatus readSuccess = eFailed;
//
//	if (sock) {
//		cout << "found client" << endl;
//		sock->set_blocking(true);
//
//		while (true) {
//			readSuccess = sock->read(readBuffer, bufferSize);
//			if (readSuccess == eSuccess) {
//				PrintBuffer(readBuffer, bufferSize);
//				// parse json
//				JSON_Test(readBuffer,bufferSize);
//			}
////			} else if (readSuccess == eFailed) {
////				cout << "does not work" << readSuccess << endl;
////			} else if (readSuccess == eWouldBlock) {
////				cout << "would block" << readSuccess << endl;
////			}
//			usleep(1000000);
//		}
//
//		SERVER.close();
//	}


	int port = 1234;
	ParameterUpdateThread param_thread(port);
	param_thread.Start();






}


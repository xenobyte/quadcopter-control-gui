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
#include "TCP_TestConnection.h"
#include "SocketCommunication.h"
#include "frozen.h"

using namespace std;

Logging LOG("TCP_TestConnection.log");
ServerSocket SERVER; // only tcp server

// maybe safer
double CStrToDouble(const char* str, int len){
	if (str != 0 && len != 0){
		string tmpStr(str,len);
		return stod(tmpStr);
	}
	return 0.0;
}

void jsonTest() {
	static const char *config_str = " { ports: [ 80.123, 443 ] } ";
	struct json_token tokens[10];
	int tokens_size = sizeof(tokens) / sizeof(tokens[0]);

	// Parse config string and make sure tokenization is correct
	parse_json(config_str, strlen(config_str), tokens, tokens_size);

/*
	tokens[0].type == JSON_TYPE_OBJECT;   // Tokens are populated
	tokens[1].type == JSON_TYPE_STRING;   // in order of their
	tokens[2].type == JSON_TYPE_ARRAY;    // appearance in the
	tokens[3].type == JSON_TYPE_NUMBER;   // JSON string
	tokens[4].type == JSON_TYPE_NUMBER;
	tokens[5].type == JSON_TYPE_EOF;      // Last token is always EOF
*/

	// Fetch port values
	json_token* token1 = find_json_token(tokens, "ports");
	json_token* token2 = find_json_token(tokens, "ports[0]");
	json_token* token3 = find_json_token(tokens, "ports[1]");

	cout << CStrToDouble(token2->ptr, token2->len) << endl;


}

int main(int argc, char* argv[]) {
	cout << "listening ..." << endl;
	SERVER.bind(1234);
	SERVER.listen(1);
	ClientSocket *sock = SERVER.accept();

	uint32_t bufferSize = 10;
	uint8_t readBuffer[bufferSize];
	TReadStatus readSuccess = eFailed;

	if (sock) {
		cout << "found client" << endl;
		sock->set_blocking(true);

		while (true) {
			readSuccess = sock->read(readBuffer, bufferSize);
			if (readSuccess == eSuccess) {
				cout << "read success: " << readSuccess << endl;
				for (uint i = 0; i < bufferSize; i++) {
					cout << readBuffer[i];
				}
				cout << endl;

				// parse json

			} else if (readSuccess == eFailed) {
				cout << "does not work" << readSuccess << endl;
			} else if (readSuccess == eWouldBlock) {
				cout << "would block" << readSuccess << endl;
			}
			usleep(1000000);
		}

		SERVER.close();
	}

}


#include <unistd.h>
#include <iostream>

#include "Parameters.h"
#include "SocketCommunication.h"
#include "ParameterUpdateThread.h"

using namespace std;

ParameterUpdateThread::ParameterUpdateThread(int port) : ThreadBase(), mPort(port),
		mLogger(LOG.createLogger("ParameterUpdate"))
{
	mServer.set_blocking(true);
	mServer.bind(port);
	mServer.listen(1);
}

void PrintToken(json_token* token) {
	if (token != 0) {
		cout << atof(token->ptr) << endl;
	} else {
		cout << "no token of this name found" << endl;
	}
}

void PrintToken(json_token* tokens, string name) {
	cout << name << ": ";
	PrintToken(find_json_token(tokens, name.c_str()));
}

double ParameterUpdateThread::GetDoubleToken(json_token* tokens, string name) {
	json_token* token = find_json_token(tokens, name.c_str());
	if (token != 0 && token->ptr != 0) {
		return atof(token->ptr);
	} else {
		mLogger->info("can't parse double json");
		return 0.0;
	}
}

void ParameterUpdateThread::Run(void) {
	Parameters & params = Parameters::getInstance();
	while (1) {
		cout << "Waiting for Parameter Client" << endl;
		UnqClientSocket client { mServer.accept() };
		if (client == NULL) {
			mLogger->error("Could not accept client");
			continue;
		}
		cout << "Client accepted" << endl;

		uint32_t bufferSize = 200;
		unsigned char readBuffer[bufferSize];
		uint32_t numOfTokens = 50;
		struct json_token tokens[numOfTokens];

		while (1) {
			TReadStatus ret = client->read(readBuffer, bufferSize);
			if (ret == eSuccess) {
				int success = parse_json((char*) readBuffer, bufferSize, tokens,
						numOfTokens);
				if (success < 0) {
					mLogger->error("Cannot parse json string");
				} else {
					PrintToken(tokens, "Angle.P");
					PrintToken(tokens, "Angle.I");
					PrintToken(tokens, "Angle.D");
					PrintToken(tokens, "Angle.N");
					PrintToken(tokens, "AngleFreq.P");
					PrintToken(tokens, "AngleFreq.I");
					PrintToken(tokens, "AngleFreq.D");
					PrintToken(tokens, "AngleFreq.N");

					// update them in parameters, axis A and B the same values // TODO: maybe not thread-safe
					// Angle
					params.PIDAngular.A.KP = GetDoubleToken(tokens, "Angle.P");
					params.PIDAngular.A.KI = GetDoubleToken(tokens, "Angle.I");
					params.PIDAngular.A.KD = GetDoubleToken(tokens, "Angle.D");
					params.PIDAngularN.A = GetDoubleToken(tokens, "Angle.N");

					params.PIDAngular.B.KP = GetDoubleToken(tokens, "Angle.P");
					params.PIDAngular.B.KI = GetDoubleToken(tokens, "Angle.I");
					params.PIDAngular.B.KD = GetDoubleToken(tokens, "Angle.D");
					params.PIDAngularN.B = GetDoubleToken(tokens, "Angle.N");

					// Angular Rate
					params.PIDAngularRate.A.KP = GetDoubleToken(tokens, "AngleFreq.P");
					params.PIDAngularRate.A.KI = GetDoubleToken(tokens, "AngleFreq.I");
					params.PIDAngularRate.A.KD = GetDoubleToken(tokens, "AngleFreq.D");
					params.PIDAngularRateN.A = GetDoubleToken(tokens, "AngleFreq.N");

					params.PIDAngularRate.B.KP = GetDoubleToken(tokens, "AngleFreq.P");
					params.PIDAngularRate.B.KI = GetDoubleToken(tokens, "AngleFreq.I");
					params.PIDAngularRate.B.KD = GetDoubleToken(tokens, "AngleFreq.D");
					params.PIDAngularRateN.B = GetDoubleToken(tokens, "AngleFreq.N");

					// Parameters have changed, MainControl has to update parameters
					mLogger->info("Parameters have been updated in Parameter.h");
					params.SetParamsHaveChanged(true);
				}
			} else if (ret == eFailed) {
				mLogger->error("can't read data, closing connection");
				client->close();
				break;
			}
			// sleep a bit to not interrupt other tasks too much
			usleep(200000);
		}
		mLogger->info("Connection closed for ParameterUpdateThread");
	}
}

void ParameterUpdateThread::CleanUp(void) {
	mServer.close();
}

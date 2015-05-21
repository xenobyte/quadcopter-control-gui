#include <iostream>
#include <stdint.h>
#include <string>  
#include "frozen.h"

using namespace std;

uint32_t const numOfValues = 8;
uint32_t const numOfTokens = 50;

void PrintToken(json_token* token){
	if (token != 0){
		cout << atof(token->ptr) << endl;
	}
	else {
		cout << "null pointer" << endl;
	}
}

void PrintToken(json_token* tokens, string name){
	cout << name << ": ";
	PrintToken(find_json_token(tokens, name.c_str()));
}

void JSON_Test(const char* json_str, uint32_t len){
	struct json_token tokens[numOfTokens];

	cout << "success: " << parse_json(json_str, len, tokens, numOfTokens) << endl;

	PrintToken(find_json_token(tokens, "Angle.P[0]"));
	PrintToken(find_json_token(tokens, "Angle..P[0]"));
	PrintToken(find_json_token(tokens, "Angle[0].P"));
	PrintToken(find_json_token(tokens, "AngleFreq[2].P"));
	PrintToken(find_json_token(tokens, "not avail"));

	PrintToken(tokens, "angle[3]");
}

int main(){
	double params[numOfValues] = { 1.2, 2.3123, 412.33, 121.3, 432.3, 123.3, 23.1, 1 };

	uint32_t const bufferSize = 100;
	char buf[bufferSize];

	string json_test2("{ \"Angle\" : [{\"P\": 23.23, \"I\" : 23.23, \"D\" : 23, 23}], \"AngleFreq\" : [{\"P\": 23.23, \"I\" : 23.23, \"D\" : 23, 23}] }");


	json_emit(buf, sizeof(buf), "{ s: [f, f, f, f] }", "angle", params[0], params[1], params[2], params[3]);
	//strcat_s(buf, bufferSize, "aj+#1´´12");
	cout << json_test2 << endl;

	cout << "size of str" << strlen(json_test2.c_str()) << endl;
	JSON_Test(json_test2.c_str(), json_test2.length());
	JSON_Test(buf, bufferSize);


	return 0;
}
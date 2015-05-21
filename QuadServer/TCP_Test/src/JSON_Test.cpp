/*
 * JSON_Test.cpp
 *
 *  Created on: 21.05.2015
 *      Author: Johannes
 */

#include "JSON_Test.h"
#include <iostream>
#include <string>
#include "frozen.h"

using namespace std;

uint32_t const numOfTokens = 50;

void PrintToken(json_token* token){
	if (token != 0){
		cout << atof(token->ptr) << endl;
	}
	else {
		cout << "no token of this name found" << endl;
	}
}

void PrintToken(json_token* tokens, string name){
	cout << name << ": ";
	PrintToken(find_json_token(tokens, name.c_str()));
}

void JSON_Test(const unsigned char* json_str, uint32_t len){
	struct json_token tokens[numOfTokens];

	int success = parse_json((char*)json_str, len, tokens, numOfTokens);
	if (success < 0) {
		cout << "Cannot parse json: " << success << endl;
		return;
	}

//	PrintToken(find_json_token(tokens, "Angle.P"));
//	PrintToken(find_json_token(tokens, "Angle[0]"));
//
//	PrintToken(find_json_token(tokens, "Angle.P[0]"));
//	PrintToken(find_json_token(tokens, "Angle..P[0]"));
//	PrintToken(find_json_token(tokens, "Angle[0].P"));
//	PrintToken(find_json_token(tokens, "AngleFreq[2].P"));
//	PrintToken(find_json_token(tokens, "not avail"));


	PrintToken(tokens, "Angle.P");
	PrintToken(tokens, "Angle.I");
	PrintToken(tokens, "Angle.D");
}

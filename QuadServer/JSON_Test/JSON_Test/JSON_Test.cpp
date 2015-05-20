#include <iostream>
#include <stdint.h>
#include <string>  
#include "frozen.h"

using namespace std;

uint32_t const numOfValues = 8;
uint32_t const numOfTokens = 20;

void PrintToken(json_token* token){
	if (token != 0){
		cout << stod(token->ptr) << endl;
	}
	else {
		cout << "null pointer" << endl;
	}
}

void PrintToken(json_token* tokens, string name){
	cout << name << ": ";
	PrintToken(find_json_token(tokens, name.c_str()));
}

void JSON_Test(char* json_str, uint32_t len){
	struct json_token tokens[numOfTokens];

	parse_json(json_str, len, tokens, numOfTokens);

	PrintToken(find_json_token(tokens, "angle[0]"));
	PrintToken(find_json_token(tokens, "angle[1]"));
	PrintToken(find_json_token(tokens, "angle[2]"));
	PrintToken(find_json_token(tokens, "not avail"));

	PrintToken(tokens, "angle[3]");
}

int main(){
	double params[numOfValues] = { 1.2, 2.3123, 412.33, 121.3, 432.3, 123.3, 23.1, 1 };

	uint32_t const bufferSize = 100;
	char buf[bufferSize];

	json_emit(buf, sizeof(buf), "{ s: [f, f, f, f] }", "angle", params[0], params[1], params[2], params[3]);
	//strcat_s(buf, bufferSize, "aj+#1´´12");
	cout << buf << endl;

	cout << "size of str" << strlen(buf) << endl;
	JSON_Test(buf, bufferSize);

	return 0;
}
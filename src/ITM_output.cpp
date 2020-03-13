/*
 * ITMoutput.cpp
 *
 *  Created on: 24.1.2020
 *      Author: Heikki Kilpeläinen 1805824
 */

#include "ITM_output.h"

#define MAX_STR_LEN 32
#define MAX_INT_LEN 16

ITM_output::ITM_output() {}

ITM_output::~ITM_output() {}

void ITM_output::print(const char* c_str) {
	ITM_write(c_str);
}
/*
void ITM_output::print(const std::string& str) {
	std::string input(str);
	char output[MAX_STR_LEN];

	strncpy(output, input.c_str(), sizeof(output));
	ITM_write(output);
}
*/
void ITM_output::print(const int number) {
	char buff[MAX_INT_LEN];

	snprintf(buff, MAX_INT_LEN, "%d", number);
	ITM_write(buff);
}


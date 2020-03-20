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

/**
*@brief 		Print to ITM console.
*@param c_str	: C-style string to print
*/
void ITM_output::print(const char* c_str) {
	ITM_write(c_str);
}

/**
*@brief 		Print to ITM console.
*@param	number	: Integer to print
*/
void ITM_output::print(const int number) {
	char buff[MAX_INT_LEN];

	snprintf(buff, MAX_INT_LEN, "%d", number);
	ITM_write(buff);
}


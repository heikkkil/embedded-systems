/*
 * DigitalIoPin.cpp
 *
 *  Created on: Jan 17, 2020
 *      Author: User
 */
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "ITMWrapper.h"

ITM_Wrapper::ITM_Wrapper() {
	ITM_init();
}

ITM_Wrapper::~ITM_Wrapper() {
	// TODO Auto-generated destructor stub
}

void ITM_Wrapper::print(string cppstring){
	ITM_write(cppstring.c_str());
}

string ITM_Wrapper::operator()(const char* cstring){
	string temp(cstring);
	return temp;
}

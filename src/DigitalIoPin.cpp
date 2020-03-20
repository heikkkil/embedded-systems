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

#include <cr_section_macros.h>


#include "DigitalIoPin.h"

/**
*@brief 		Wraps gpio configuration to a more understadable format
*@param port 	: port number of a pin
*@param pin 	: pin number of a pin
*@param input 	: sets pin to input / output mode. True for input
*@param pullup 	: sets pullup/pulldown. True for pullup
*@param invert	: inverts active low to active high when true.
*/

DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert){
	this->port = port;
	this->pin = pin;
	this->invert = invert;
	this->input = input;
	this->pullup = pullup;

	if(input){
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,(IOCON_DIGMODE_EN | (pullup ? (0x2 << 3):(0x1 << 3)) | (invert ? (0x1 << 6) : 0)));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port,pin);
	}else{
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,(IOCON_DIGMODE_EN ));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port,pin);
	}
}

bool DigitalIoPin::read()const{
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}


void DigitalIoPin::write(bool value){
	if(!input){
		Chip_GPIO_SetPinState(LPC_GPIO,port,pin, invert ? !value : value);
	}
}





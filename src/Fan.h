/*
 * Fan.h
 *
 *  Created on: Mar 6, 2020
 *      Author: User
 */

#ifndef FAN_H_
#define FAN_H_

#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include <cstring>
#include <cstdio>

extern void Sleep(int ms);

class Fan {
public:
	Fan(int);
	virtual ~Fan();
	void abbModbusTest();
	bool setFrequency(uint16_t);
private:

	ModbusMaster node;

	ModbusRegister Speed;
	ModbusRegister ControlWord;
	ModbusRegister StatusWord;
	ModbusRegister OutputFrequency;
	ModbusRegister Current;
	ModbusRegister Frequency;
};


#endif /* FAN_H_ */

/* this function is required by the modbus library */

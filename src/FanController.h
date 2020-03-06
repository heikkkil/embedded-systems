/*
 * FanController.h
 *
 *  Created on: Mar 6, 2020
 *      Author: User
 */

#ifndef FANCONTROLLER_H_
#define FANCONTROLLER_H_

#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include <cstring>
#include <cstdio>

extern void Sleep(int ms);

class FanController {
public:
	FanController(int);
	virtual ~FanController();
	void abbModbusTest();
private:
	bool setFrequency(ModbusMaster&, uint16_t);
	ModbusMaster node;
};

#endif /* FANCONTROLLER_H_ */

/* this function is required by the modbus library */

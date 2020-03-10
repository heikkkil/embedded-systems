/*
 * FanController.h
 *
 *  Created on: 10 Mar 2020
 *      Author: Eelis
 */

#ifndef FANCONTROLLER_H_
#define FANCONTROLLER_H_

#include "PressureSensor.h"
#include "Fan.h"

class FanController {
public:
	FanController(I2C_config conf, int targetPressure, int initialFanSpeed);
	virtual ~FanController();
	bool setMode();
	void run();
	void setTargetPressure();
	void setFanSpeed();
private:
	bool mode;
	Fan fan;
	PressureSensor pressureSensor;
	int targetPressure;
	int currentFanSpeed;
	int autoOutOfLimits;
	void automatic();
	void manual();
};

#endif /* FANCONTROLLER_H_ */

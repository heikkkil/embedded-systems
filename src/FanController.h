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
	void setMode(bool);
	void run();
	void setTargetPressure(int target);
	void setFanSpeed(int speed);
	int getPressure();
private:
	bool mode;
	Fan fan;
	PressureSensor pressureSensor;
	int targetPressure;
	int fanSpeed;
	int autoOutOfLimits;
	int pressure;
	void automatic();
	void manual();
};

#endif /* FANCONTROLLER_H_ */

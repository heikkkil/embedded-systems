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

#define AUTO true
#define MANUAL false
#define SENSOR_ADDRESS 0x40
#define NOT_REACHED_LIMIT 50
#define SMALL_FAN_ADJUST 100
#define MEDIUM_FAN_ADJUST 300
#define BIG_FAN_ADJUST 1000

class FanController {
public:
	FanController(I2C_config conf, int targetPressure, int initialFanSpeed);
	virtual ~FanController();
	void run();
	void setTargetPressure(int target);
	void setFanSpeed(int speed);
	void setMode(bool);
	int getPressure();
	int getFanSpeed();
	bool getPressureReachable();
	bool getMode();
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

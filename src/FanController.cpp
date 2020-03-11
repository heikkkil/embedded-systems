/*
 * FanController.cpp
 *
 *  Created on: 10 Mar 2020
 *      Author: Eelis
 */

#include "FanController.h"
#include "I2C.h"

#define AUTO true
#define MANUAL false
#define SENSOR_ADDRESS 0x40

FanController::FanController(I2C_config conf, int initialTargetPressure = 80, int initialFanSpeed = 10000) : fan(2), pressureSensor(SENSOR_ADDRESS, conf) {
	fanSpeed = initialFanSpeed;
	targetPressure = initialTargetPressure;
	autoOutOfLimits = 0;
	mode = AUTO;
}

FanController::~FanController() {
	// TODO Auto-generated destructor stub
}

void FanController::run() {
	pressure = pressureSensor.getPressure();
	//For some reason (int overlow?), when pressure is close to 0, it sometimes spikes to ~250
	//normally pressure never gets over 200 so make all of those to 0
	if (pressure > 200) {
		pressure = 0;
	}
	while(1) {
		if (mode == AUTO) {
			automatic();
		} else {
			manual();
		}
		Sleep(10);
	}
}

void FanController::automatic() {
	int comp = targetPressure - pressure;

	if (comp <= 3 && comp >= -3) {
		autoOutOfLimits = 0;
	} else {
		++autoOutOfLimits;
	}

	if (autoOutOfLimits >= 3) {
		if (comp <= 10 && comp >= -10) {
			if (comp < 0) {
				fanSpeed -= 100;
			} else {
				fanSpeed += 100;
			}
		} else {
			if (comp < 0) {
				fanSpeed -= 300;
			} else {
				fanSpeed += 300;
			}
		}
	}

	if (fanSpeed > 20000) {
		fanSpeed = 20000;
	} else if (fanSpeed < 0) {
		fanSpeed = 0;
	}

	printf("fan speed: %d pressure: %d target: %d", fanSpeed, pressure, targetPressure);
	fan.setNonRelativeFrequency(fanSpeed);
}

void FanController::manual() {
	fan.setFrequency(fanSpeed);
}

void FanController::setTargetPressure(int target) {
	if (target >= 0 && target <= 255) {
		targetPressure = target;
	}
}

void FanController::setFanSpeed(int speed) {
	if (mode == MANUAL) {
		fanSpeed = speed;
	}
}

void FanController::setMode(bool m) {
	mode = m;
}

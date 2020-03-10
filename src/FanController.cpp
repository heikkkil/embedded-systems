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
	currentFanSpeed = initialFanSpeed;
	targetPressure = initialTargetPressure;
	autoOutOfLimits = 0;
	mode = AUTO;
}

FanController::~FanController() {
	// TODO Auto-generated destructor stub
}

void FanController::run() {
	while(1) {
		if (mode == AUTO) {
			automatic();
		} /*else {
			manual();
		}*/
		Sleep(10);
	}
}

void FanController::automatic() {
	int pressure = pressureSensor.getPressure();
	/*if (pressure > 150) {
		pressure = 0;
	}*/
	int comp = targetPressure - pressure;

	if (comp <= 3 && comp >= -3) {
		autoOutOfLimits = 0;
	} else {
		++autoOutOfLimits;
	}

	if (autoOutOfLimits >= 3) {
		if (comp <= 10 && comp >= -10) {
			if (comp < 0) {
				currentFanSpeed -= 100;
			} else {
				currentFanSpeed += 100;
			}
		} else {
			if (comp < 0) {
				currentFanSpeed -= 300;
			} else {
				currentFanSpeed += 300;
			}
		}
	}

	if (currentFanSpeed > 20000) {
		currentFanSpeed = 20000;
	} else if (currentFanSpeed < 0) {
		currentFanSpeed = 0;
	}

	printf("fan speed: %d pressure: %d target: %d", currentFanSpeed, pressure, targetPressure);
	fan.setNonRelativeFrequency(currentFanSpeed);
}

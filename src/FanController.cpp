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

FanController::FanController(I2C_config conf, int initialTargetPressure = 80, int initialFanSpeed = 50) : fan(2), pressureSensor(SENSOR_ADDRESS, conf) {
	currentFanSpeed = initialFanSpeed;
	targetPressure = initialTargetPressure;
	mode = AUTO;
}

FanController::~FanController() {
	// TODO Auto-generated destructor stub
}

void FanController::run() {
	if (mode == AUTO) {
		automatic();
	} /*else {
		manual();
	}*/
}

void FanController::automatic() {
	float ratio = pressureSensor.getPressure() / targetPressure*1.0;
	if (ratio < 0.5) {
		currentFanSpeed += 3;
	} else if (ratio < 0.75) {
		currentFanSpeed += 2;
	} else if (ratio < 0.98) {
		currentFanSpeed += 1;
	} else if (ratio > 1.5) {
		currentFanSpeed -= 3;
	} else if (ratio > 1.25) {
		currentFanSpeed -= 2;
	} else if (ratio > 1.02) {
		currentFanSpeed -= 1;
	}
	if (currentFanSpeed < 0) {
		currentFanSpeed = 0;
	}
	if (currentFanSpeed > 100) {
		currentFanSpeed = 100;
	}
	printf("fan speed: %d\npressure: %d", currentFanSpeed, pressureSensor.getPressure());
	fan.setFrequency(currentFanSpeed);
}

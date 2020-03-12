/*
 * FanController.cpp
 *
 *  Created on: 10 Mar 2020
 *      Author: Eelis
 */

#include "FanController.h"
#include "I2C.h"

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
	while (1) {
		pressure = pressureSensor.getPressure();
		//For some reason (int overlow?), when pressure is close to 0, it sometimes spikes to ~250
		//normally pressure never gets over 200 so make all of those to 0
		if (pressure > 200) {
			pressure = 0;
		}

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

	if (comp <= 2 && comp >= -2) {
		autoOutOfLimits = 0;
	} else {
		++autoOutOfLimits;
	}

	if (autoOutOfLimits >= 2) {
		if (comp <= 10 && comp >= -10) {
			if (comp < 0) {
				fanSpeed -= SMALL_FAN_ADJUST;
			} else {
				fanSpeed += SMALL_FAN_ADJUST;
			}
		} else if (comp >= 25 || comp <= -25) {
			if (comp >= 25) {
				fanSpeed += BIG_FAN_ADJUST;
			} else {
				fanSpeed -= BIG_FAN_ADJUST;
			}
		} else {
			if (comp < 0) {
				fanSpeed -= MEDIUM_FAN_ADJUST;
			} else {
				fanSpeed += MEDIUM_FAN_ADJUST;
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
		fanSpeed = 200*speed;
	}
}

void FanController::setMode(bool m) {
	mode = m;
	if (mode == AUTO) {
		autoOutOfLimits = 0;
	}
}

int FanController::getPressure() {
	return pressure;
}

int FanController::getFanSpeed() {
	return fanSpeed;
}

bool FanController::getMode() {
	return mode;
}

bool FanController::getPressureReachable() {
	return (autoOutOfLimits < NOT_REACHED_LIMIT);
}

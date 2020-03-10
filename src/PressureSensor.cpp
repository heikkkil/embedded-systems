/*
 * PressureReader.cpp
 *
 *  Created on: 6 Mar 2020
 *      Author: Eelis
 */

#include "PressureSensor.h"

#define SCALE_FACTOR 240

PressureSensor::PressureSensor(uint8_t address, I2C_config conf) :
address(address), i2c(conf) {

}

PressureSensor::~PressureSensor() {
	// TODO Auto-generated destructor stub
}

int PressureSensor::getPressure() {
	int raw = getRawMeasurement();
	int pascal = raw / SCALE_FACTOR * 0.95f;
	return pascal;
}

uint16_t PressureSensor::getRawMeasurement() {
	uint8_t read = 0xF1;
	uint8_t buffer[3];
	i2c.transaction(address, &read, 1, buffer, 3);
	uint16_t measurement_bits = ((int16_t)buffer[0] << 8) | buffer[1];
	return measurement_bits;
}

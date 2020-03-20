/*
 * PressureReader.cpp
 *
 *  Created on: 6 Mar 2020
 *      Author: Eelis
 */

#include "PressureSensor.h"
#include <cstdio>


#define SCALE_FACTOR 240

/**
*@param address		: Address of sensor
*@param	conf		: Configuration object.
*/
PressureSensor::PressureSensor(uint8_t address, I2C_config conf) :
address(address), i2c(conf) {
	index = -1;
	for (int i = 0; i < 2; ++i) {
		measurementHistory[i] = 0;
	}
}

PressureSensor::~PressureSensor() {
	// TODO Auto-generated destructor stub
}

/**
*@brief 	Geter for pressure
*@return 	Returns median filtered pressure value.
*/
int PressureSensor::getPressure() {
	int raw = getRawMeasurement();
	int pascal = raw / SCALE_FACTOR * 0.95f;
	measurementHistory[++index] = pascal;
	if (index >= 2) {
		index = -1;
	}
	return medianFilter();
}

int16_t PressureSensor::getRawMeasurement() {
	uint8_t read = 0xF1;
	uint8_t buffer[3];
	i2c.transaction(address, &read, 1, buffer, 3);
	int16_t measurement_bits = ((int16_t)buffer[0] << 8) | buffer[1];
	return measurement_bits;
}

int PressureSensor::medianFilter() {
	int h_median[3];
	std::copy(measurementHistory, measurementHistory+3, h_median);
	std::sort(h_median, h_median+3);
	return h_median[1];
}

/*
 * PressureReader.h
 *
 *  Created on: 6 Mar 2020
 *      Author: Eelis
 */

#ifndef PRESSURESENSOR_H_
#define PRESSURESENSOR_H_

#include "I2C.h"
#include <algorithm>


/**
*@brief 	Wrapper class for handling the pressuresensor
*/

class PressureSensor {
public:
	PressureSensor(uint8_t address, I2C_config conf);
	virtual ~PressureSensor();
	int getPressure();
private:
	uint8_t address;
	I2C i2c;
	int16_t getRawMeasurement();
	int measurementHistory[3];
	int index;
	int medianFilter();
};

#endif /* PRESSURESENSOR_H_ */

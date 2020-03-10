/*
 * PressureReader.h
 *
 *  Created on: 6 Mar 2020
 *      Author: Eelis
 */

#ifndef PRESSURESENSOR_H_
#define PRESSURESENSOR_H_

#include "I2C.h"

class PressureSensor {
public:
	PressureSensor(uint8_t address, I2C_config conf);
	virtual ~PressureSensor();
	int getPressure();
private:
	uint8_t address;
	I2C i2c;
	uint16_t getRawMeasurement();
};

#endif /* PRESSURESENSOR_H_ */

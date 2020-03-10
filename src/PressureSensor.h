/*
 * PressureReader.h
 *
 *  Created on: 6 Mar 2020
 *      Author: Eelis
 */

#ifndef PRESSUREREADER_H_
#define PRESSUREREADER_H_

#include "I2C.h"

class PressureReader {
public:
	PressureReader(uint8_t address, I2C_config conf);
	virtual ~PressureReader();
	int getPressure();
private:
	uint8_t address;
	I2C i2c;
	uint16_t getRawMeasurement();
};

#endif /* PRESSUREREADER_H_ */

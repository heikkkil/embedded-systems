/*
 * DigitalIoPin.h
 *
 *  Created on: Jan 17, 2020
 *      Author: User
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin {
public:
	DigitalIoPin(int port, int pin, bool input = true, bool pullup = true, bool invert = false);
	virtual ~DigitalIoPin()=default;
	bool read()const;
	void write(bool value);
private:
	int port;
	int pin;
	bool input;
	bool pullup;
	bool invert;
};





#endif /* DIGITALIOPIN_H_ */

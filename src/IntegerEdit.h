/*
 * IntegerEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include <string>
#include "PropertyEdit.h"

#include "LiquidCrystal.h"
#include "FanController.h"

class IntegerEdit: public PropertyEdit {
public:
	IntegerEdit(LiquidCrystal *lcdoutput, FanController *fancon, std::string editTitle, int limit_lower, int limit_upper);
	virtual ~IntegerEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	int  getValue();
	void setValue(int value);

private:
	LiquidCrystal *lcd;
	FanController *fcon;
	std::string title;
	std::string errcode;
	int value;
	int edit;
	int lim_lower;
	int lim_upper;
	bool focus;
	void save();
	void displayEditValue();
};

#endif /* INTEGEREDIT_H_ */

/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include <cstdio>
#include "IntegerEdit.h"

IntegerEdit::IntegerEdit(LiquidCrystal *lcdoutput, FanController *fancon, std::string editTitle, int limit_lower,int limit_upper):
	lcd(lcdoutput), fcon(fancon), title(editTitle), lim_lower(limit_lower),lim_upper(limit_upper){
	errcode = "ERROR";

	if(title == "Auto  ")
		value = fcon->getTargetPressure();
	else
		value = fcon->getFanSpeed();

	edit = value;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if (edit < lim_upper) {
		++edit;
	}
}

void IntegerEdit::decrement() {
	if (edit > lim_lower) {
		--edit;
	}
}

void IntegerEdit::accept() {
	if(title == "Auto  ") {
		fcon->setMode(true);
		fcon->setTargetPressure(edit);
	} else {
		fcon->setMode(false);
		fcon->setFanSpeed(edit);
	}

	save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	return this->focus;
}

void IntegerEdit::display() {

	/*
	[0123456789012345]
	 !auto   P xxx Pa
	 >manual S[xxx]%
	*/

	int  p = fcon->getPressure();
	int  s = fcon->getFanSpeed();
	bool err  = fcon->isPressureReachable();
	char row[17];
	lcd->clear();

	/* Upper row */
	lcd->setCursor(0,0);

	if(title == "Auto  ") {
		if(focus)
			snprintf(row, 17, " %s P[%3d]Pa", title.c_str(), edit);
		else
			snprintf(row, 17, " %s P %3d Pa", title.c_str(), p);
	}
	else {
		snprintf(row, 17, " %s P %3d Pa", title.c_str(), p);
	}

	lcd->print(row);

	/* Lower row */
	lcd->setCursor(0,1);

	if(title == "Manual") {
		if(focus)
			snprintf(row, 17, " %s S[%3d]%%", !err ? "ERROR " : "      ", edit);
		else
			snprintf(row, 17, " %s S %3d %%", !err ? "ERROR " : "      ", s);
	}
	else {
		snprintf(row, 17, " %s S %3d %%", !err ? "ERROR " : "      ", s);
	}

	lcd->print(row);
}


void IntegerEdit::save() {
	// Commit edit.
	value = edit;
}


int IntegerEdit::getValue() {
	return value;
}
void IntegerEdit::setValue(int value) {

	if(value>lim_upper ){
		edit = lim_upper;
	}
	else if (value < lim_lower){
		edit= lim_lower;
	}else{
		edit = value;
	}

	save();
}

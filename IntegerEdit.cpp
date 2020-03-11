/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, FanController *fancon, std::string editTitle, int limit_lower,int limit_upper):
	lcd(lcd_), fcon(fancon), title(editTitle), lim_lower(limit_lower),lim_upper(limit_upper){
	value = 0;
	edit = 0;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if(edit < lim_upper)
		edit++;
}

void IntegerEdit::decrement() {
	if(edit > lim_lower)
		edit--;
}

void IntegerEdit::accept() {
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
 >auto   P xxx Pa
 >manual S xxx  %
*/
	int  p = fcon->getPressure();
	int  s = fcon->getFanSpeed();
	//bool err = fcon->getError();
	char row[17];
	lcd->clear();

	/* Upper row */
	lcd->setCursor(0,0);

	if(focus)
		snprintf(row, 17, ">%s  P[%3d]Pa", title, p);
	else
		snprintf(row, 17, " %s  P %3d Pa", title, p);

	lcd->print(row);

	/* Lower row */
	lcd->setCursor(0,1);

	if(focus)
		snprintf(row, 17, "        S[%3d] %", title, s);
	else
		snprintf(row, 17, "        S %3d  %", title, s);

	lcd->print(row);
}


void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
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

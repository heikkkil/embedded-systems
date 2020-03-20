/**
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *@Author	: krl
 */

#include <cstdio>
#include "IntegerEdit.h"
/**
*@brief 			Editable numerical value from which a menu item is created
*@param	lcdoutput	: Pointer to lcd screen object
*@param fancon		: Pointer to a fancontroller object
*@param editTitle	: Title of item
*@param limit_lower	: Lower end limit of the numerical value that the edit can have.
*@param limit_upper	: Higher end limit of the numerical value that the edit can have.
*/
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

/**
*@brief 	Increment edit value
*/
void IntegerEdit::increment() {
	if (edit < lim_upper) {
		++edit;
	}
}

/**
*@brief 	Decrement edit value
*/
void IntegerEdit::decrement() {
	if (edit > lim_lower) {
		--edit;
	}
}

/**
*@brief 	Commits changes to mode
*/
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

/**
*@brief 	Cancel temporary changes to edit.
*/
void IntegerEdit::cancel() {
	edit = value;
}

/**
*@brief 		Sets focus to selected item.
*@param focus	: True to set focus, false to deselect item.
*/
void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

/**
*@brief 	Increment edit value
*@return	Return current focus state.
*/
bool IntegerEdit::getFocus() {
	return this->focus;
}
/**
*@brief 	 Prints to lcd screen
*/
void IntegerEdit::display() {

	int  p = fcon->getPressure();
	int  s = fcon->getFanSpeed();
	bool err  = fcon->isPressureReachable();
	bool mode = fcon->getMode();
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

	std::string modeErrorString = "";

	if (mode == true) {
		modeErrorString += "A ";
	} else {
		modeErrorString += "M ";
	}

	if (!err) {
		modeErrorString += "ERR ";
	} else {
		modeErrorString += "    ";
	}

	if(title == "Manual") {
		if(focus)
			snprintf(row, 17, " %s S[%3d]%%", modeErrorString.c_str(), edit);
		else
			snprintf(row, 17, " %s S %3d %%", modeErrorString.c_str(), s);
	}
	else {
		snprintf(row, 17, " %s S %3d %%", modeErrorString.c_str(), s);
	}

	lcd->print(row);
}

/**
*@brief 	Commits changes to the edit value.
*/
void IntegerEdit::save() {
	value = edit;
}

/**
*@brief 	Getter for edit value.
*@return 	Value of the edit
*/
int IntegerEdit::getValue() {
	return value;
}

/**
*@brief 	Set edit to a specific value.
*/
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

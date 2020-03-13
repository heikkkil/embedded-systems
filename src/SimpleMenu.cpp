/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"

SimpleMenu::SimpleMenu() {
	position = 0;
}

SimpleMenu::~SimpleMenu() {
	// TODO Auto-generated destructor stub
}

void SimpleMenu::addItem(MenuItem *item) {
	items.push_back(item);
}

void SimpleMenu::event(MenuItem::menuEvent e) {
	if(items.size() <= 0) return;

	if(!items[position]->event(e)) {
		if(e == MenuItem::up) position++;
		else if(e == MenuItem::down) position--;

		if(position < 0) position = items.size() - 1;
		if(position >= (int) items.size()) position = 0;

		items[position]->event(MenuItem::show);
	}
}

//void SimpleMenu::addTitle(std::string &title) {
//	titles.push_back(title);
//}


//void SimpleMenu::displayMenu() {
//
//	/*
//	[0123456789012345]
//	 !auto   P xxx Pa
//	 >manual S[xxx]%
//	*/
//
//	int  p = fcon->getPressure();
//	int  s = fcon->getFanSpeed();
//	bool err  = fcon->getError() != 0 ? true : false; //TODO check this
//	bool mode = fcon->getMode();
//	char selector_symbol = err ? '!' : '>';
//	char auto_selector = mode ? selector_symbol : ' ';
//	char man_selector  = mode ? ' ' : selector_symbol;
//	char row[17];
//	lcd->clear();
//
//	/* Upper row */
//	lcd->setCursor(0,0);
//
//	if(mode && focus)
//		snprintf(row, 17, "%c%s P[%3d]Pa", auto_selector, titles[0].c_str(), p);
//	else
//		snprintf(row, 17, "%c%s P %3d Pa", auto_selector, titles[0].c_str(), p);
//
//	lcd->print(row);
//
//	/* Lower row */
//	lcd->setCursor(0,1);
//
//	if(!mode && focus)
//		snprintf(row, 17, "%c%s S[%3d]%%", man_selector, titles[0].c_str(), s);
//	else
//		snprintf(row, 17, "%c%s S %3d %%", man_selector, titles[0].c_str(), s);
//
//	lcd->print(row);
//}

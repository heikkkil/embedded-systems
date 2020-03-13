/*
 * MenuItem.cpp
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#include "MenuItem.h"

<<<<<<< HEAD
#define CANCEL_TIMER 5000
MenuItem::MenuItem(PropertyEdit *property): pe(property) {
=======

MenuItem::MenuItem(PropertyEdit *property, FanController *fancon): pe(property), fcon(fancon) {
>>>>>>> prefinal

}



MenuItem::~MenuItem() {

}
/**
 * @brief Statehandler of the program. Mostly handles menu events but added some timer events.
 */
bool MenuItem::event(menuEvent e) {
	bool handled = true;
	switch(e) {

/*******************************************
 * MENU EVENTS
 *******************************************/
	case ok:
		if(pe->getFocus()) {
			pe->accept();
			pe->setFocus(false);
		}
		else {
			pe->setFocus(true);
		}
		break;
	case back:
		if(pe->getFocus()) {
			pe->cancel();
			pe->setFocus(false);
		}
		else {
			handled = false;
		}
		break;
	case show:
		break;
	case up:
		if(pe->getFocus()) pe->increment();
		else handled = false;
		break;
	case down:
		if(pe->getFocus()) pe->decrement();
		else handled = false;
		break;

/*******************************************
 * TIMER EVENTS
 *******************************************/
	case refresh:
		break;

	}
	if(handled) pe->display();

	return handled;
}


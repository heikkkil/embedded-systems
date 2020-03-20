/*
 * MenuItem.h
 *
 *  Created on: 1.2.2016
 *      Author: krl
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include "PropertyEdit.h"
#include "FanController.h"

/**
*@brief 	Item to add to the menu. Handles some events dispatched by the parent menu.
*/
class MenuItem {
public:
	enum menuEvent {
		up,
		down,
		ok,
		back,
		show,
		refresh
	};
	MenuItem(PropertyEdit *property, FanController *fancon);
	virtual ~MenuItem();
	virtual bool event(menuEvent e);
private:
	PropertyEdit *pe;
	FanController *fcon;
};

#endif /* MENUITEM_H_ */

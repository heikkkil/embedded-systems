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


class MenuItem {
public:
	enum menuEvent {
		up,
		down,
		ok,
		back,
		show
	};
	MenuItem(PropertyEdit *property, FanController *fancon);
	virtual ~MenuItem();
	virtual bool event(menuEvent e);
private:
	PropertyEdit *pe;
	FanController *fcon;
};

#endif /* MENUITEM_H_ */

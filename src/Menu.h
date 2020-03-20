
/*
 * Menu.h
 *
 *  Created on: 6.3.2020
 *      Author: h3_g3
 */

#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include "MenuItem.h"
#include "DigitalIoPin.h"

/**
*@brief 	Main manu object. Handles events and holds list of event items.
*/
class Menu {
public:
	Menu();
	virtual ~Menu();
	Menu(const Menu &other) = delete;
	Menu(Menu &&other) = delete;

	void addItem(MenuItem *item);
	void event(MenuItem::menuEvent e);

private:
	std::vector<MenuItem *> items;
	int position;
};

#endif /* MENU_H_ */


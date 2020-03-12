/*
 * SimpleMenu.h
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#ifndef SIMPLEMENU_H_
#define SIMPLEMENU_H_

#include <vector>

#include "MenuItem.h"
//#include "LiquidCrystal.h"
//#include "FanController.h"

class SimpleMenu {
public:
	SimpleMenu();
	virtual ~SimpleMenu();
	void addItem(MenuItem *item);
	void event(MenuItem::menuEvent e);
	//void addTitle(std::string &title);
	//void displayMenu();

private:
	std::vector<MenuItem *> items;
	//std::vector<std::string> titles;
	//LiquidCrystal *lcd;
	//FanController *fcon;
	int position;
};

#endif /* SIMPLEMENU_H_ */

/*
 * Menu.cpp
 *
 *  Created on: 6.3.2020
 *      Author: h3_g3
 */

#include "Menu.h"

Menu::Menu()
{
	// default ctor
}

Menu::~Menu()
{
	// default dtor
}

/**
*@brief 		Adds items to the menu.
*@param item	: MenuItem to add to the menu
*/
void Menu::addItem(MenuItem *item)
{
	items.push_back(item);
}

/**
*@brief 	Dispatches events to the menuitems to handle. Refreshes display
*@param	e	: Event to handle
*/
void Menu::event(MenuItem::menuEvent e)
{
	if(items.size() <= 0) return;

	if(!items[position]->event(e)) {
		if(e == MenuItem::up) position++;
		else if(e == MenuItem::down) position--;

		if(position < 0) position = items.size() - 1;
		if(position >= (int) items.size()) position = 0;

		items[position]->event(MenuItem::show);
	}
}


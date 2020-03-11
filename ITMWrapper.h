/*
 * ITMWrapper.h
 *
 *  Created on: Jan 21, 2020
 *      Author: User
 */
#include <string>
#include "ITM_write.h"

using namespace std;

#ifndef ITMWRAPPER_H_
#define ITMWRAPPER_H_

class ITM_Wrapper {


public:
	ITM_Wrapper();
	virtual ~ITM_Wrapper();
	void print(string);
private:
	string operator()(const char*);
};

#endif /* ITMWRAPPER_H_ */

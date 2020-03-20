/*
 * ITMoutput.h
 *
 *  Created on: 24.1.2020
 *      Author: Heikki Kilpeläinen 1805824
 */

#ifndef ITM_OUTPUT_H_
#define ITM_OUTPUT_H_

#include "ITM_write.h"
#include <cstdio>
#include <string.h>

/**
*@brief 	Wrapper class for itm printing.
*/
class ITM_output {
public:
	ITM_output();
	~ITM_output();
	void print(const char* c_str);
	void print(const int number);

};

#endif /* ITM_OUTPUT_H_ */

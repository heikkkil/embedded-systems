/*
 * RingBuffer.h
 *
 *  Created on: Feb 18, 2020
 *      Author: User
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <stdint.h>
#include <vector>

#include "MenuItem.h"
class RingBuffer {
public:
	int8_t operator[](int);
	RingBuffer(int length);
	virtual ~RingBuffer();
	void add(enum MenuItem::menuEvent);
	enum MenuItem::menuEvent Pop();
	int8_t Head();
	int8_t Tail();
	int get_Length();
	bool IsEmpty();

private:
	std::vector<enum MenuItem::menuEvent> buffer;
	int head=0;
	int tail=0;
	int length;
	void head_Incr();
	void tail_Incr();
};


#endif /* RINGBUFFER_H_ */

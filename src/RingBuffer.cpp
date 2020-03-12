/*
 * RingBuffer.cpp
 *
 *  Created on: Feb 18, 2020
 *      Author: Jere
 */


/*
 * Created for a special purpose, not for generic use
 *
 */

#include "RingBuffer.h"
/**
*@brief Initialize with buffer length
*
*
*/
RingBuffer::RingBuffer(int length):buffer(length){
	// TODO Auto-generated constructor stub

}

RingBuffer::~RingBuffer() {
	// TODO Auto-generated destructor stub
}

void RingBuffer::head_Incr(){
	if(head == length){
		head=0;
	}
	head++;
}

void RingBuffer::tail_Incr(){
	if(tail == length){
		tail=0;
	}
	tail++;
}

void RingBuffer::add(enum MenuItem::menuEvent value){
	buffer[head] = value;
	head_Incr();
}

enum MenuItem::menuEvent RingBuffer::Pop() {
	//For a specific purpose
	enum MenuItem::menuEvent value_enum = buffer[tail];
	tail_Incr();
	return value_enum;
}

int8_t RingBuffer::Head(){
	return buffer[head];
}

int8_t RingBuffer::Tail(){
	return buffer[tail];
}

int RingBuffer::get_Length() {
	return length;
}

bool RingBuffer::IsEmpty() {

	if(tail == head){
		return true;
	}
	return false;
}



/**
 * RingBuffer.cpp
 *
 *  Created on: Feb 18, 2020
 * @author	Jere
 */

//Created for a specific purpose, not very generic

#include "RingBuffer.h"

/**
*@brief 		Initialize with buffer length
*@param lenght 	: length of the buffer
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

/**
*@brief 		Add an item to the buffer, moves head "pointer" automatically
*@param	value	:	Item to add to buffer. (An event in this project)
*/
void RingBuffer::add(enum MenuItem::menuEvent value){
	buffer[head] = value;
	head_Incr();
}

/**
*@brief 		Take out the last item (the one who has been in the buffer for the longest).
*@return 		Returns the popped event.
*/
enum MenuItem::menuEvent RingBuffer::Pop() {
	//For a specific purpose
	enum MenuItem::menuEvent value_enum = buffer[tail];
	tail_Incr();
	return value_enum;
}

/**
*@brief 		Getter for value at head.
*@return 		Returns the value at head
*/
int8_t RingBuffer::Head(){
	return buffer[head];
}

/**
*@brief 		Getter for value at tail..
*@return 		Returns the value at tail
*/
int8_t RingBuffer::Tail(){
	return buffer[tail];
}

/**
*@brief 		Getter for buffer length.
*@return 		Returns (maximum) length of buffer. Inserting lenght + 1 items will overwrite the first value item.-
*/
int RingBuffer::get_Length() {
	return length;
}

/**
*@brief 		Boolean check for empty buffer.
*@return 		Returns true if buffer is empty.
*/
bool RingBuffer::IsEmpty() {

	if(tail == head){
		return true;
	}
	return false;
}
=======
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


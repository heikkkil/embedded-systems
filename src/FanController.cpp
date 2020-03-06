/*
 * FanController.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: User
 */

#include "FanController.h"

FanController::FanController(int _node): node(_node) {


	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);
}

FanController::~FanController() {
	// TODO Auto-generated destructor stub
}



void FanController::abbModbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);


	// need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x0406; // prepare for starting

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x047F; // set drive to start mode

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	int i = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

	while (1) {

		// just print the value without checking if we got a -1
		printf("F=%4d, I=%4d\n", (int) OutputFrequency, (int) Current);

		Sleep(3000);
		i++;
		if(i >= 20) {
			i=0;
		}
		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz
		setFrequency(node, fa[i]);
	}
}


bool FanController::setFrequency(ModbusMaster& node, uint16_t freq){
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}


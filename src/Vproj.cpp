/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
#define EVENT_BUFFER_SIZE 10
#define BOUNCER_LIMIT 100
#define REFRESH_PERIOD 500
#define CANCEL_PERIOD 2000

#include <ring_buffer.h>
#include <memory>
#include <atomic>

#include "LiquidCrystal.h"
#include "SimpleMenu.h"
#include "IntegerEdit.h"
#include "FanController.h"
#include "DigitalIoPin.h"
#include "Fan.h"
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "LpcUart.h"
#include "RingBuffer.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static volatile std::atomic_int counter;
static volatile uint32_t systicks;
static volatile uint32_t prev_systicks;
static volatile uint32_t cancel_timer;
static volatile uint32_t display_refresh_timer;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
//One buffer for events that are associated more with menu, and one for Systick timer associated events
RingBuffer e_Ring(10);
RingBuffer t_Ring(10);

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/**
 * @brief 	All configuration needed for button interrupts is here
 * @return 	Nothign
 */
static void setup_Pin_Interrupts(){

	Chip_PININT_Init(LPC_GPIO_PIN_INT);

	DigitalIoPin SW1(0,0,true,true,true);
	DigitalIoPin SW2(1,3,true,true,true);
	DigitalIoPin SW3(0,10,true,true,true);
	DigitalIoPin SW4(0,9,true,true,true);

	//Enable peripheral clock
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	//Link Interrupt channels to physical pins
	Chip_INMUX_PinIntSel(0, 0, 0);
	Chip_INMUX_PinIntSel(1, 1, 3);
	Chip_INMUX_PinIntSel(2, 0, 10);
	Chip_INMUX_PinIntSel(3, 0, 9);

	//Clear interrupt status of pins
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0)|PININTCH(1)| PININTCH(2)| PININTCH(3));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT,PININTCH(0)|PININTCH(1)| PININTCH(2)| PININTCH(3));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT,PININTCH(0)|PININTCH(1)| PININTCH(2)| PININTCH(3));

	//Enable IRQ's for pins
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);
	NVIC_ClearPendingIRQ(PIN_INT3_IRQn);
	NVIC_EnableIRQ(PIN_INT3_IRQn);

}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


//Throw timer events by SysTick timer by setting timer value to systicks+[offset] and counting "down"
void SysTick_Handler(void)
{
	systicks++;
	cancel_timer++;
	display_refresh_timer++;

	//Periodically resets focus on menu items.
	if(cancel_timer <= systicks){
		e_Ring.add(MenuItem::back);
		cancel_timer = systicks + CANCEL_PERIOD;
	}

	//Periodically refreshes lcd screen.
	if(display_refresh_timer <= systicks){
		t_Ring.add(MenuItem::refresh);
		display_refresh_timer = REFRESH_PERIOD;
	}

	if(counter > 0) counter--;
}

void PIN_INT0_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(0));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		e_Ring.add(MenuItem::up);
	}
	prev_systicks = systicks;
}

void PIN_INT1_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(1));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		e_Ring.add(MenuItem::ok);
	}
	prev_systicks = systicks;
}

void PIN_INT2_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(2));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		e_Ring.add(MenuItem::down);
	}
	prev_systicks = systicks;
}

void PIN_INT3_IRQHandler(void){

Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(3));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		e_Ring.add(MenuItem::back);
	}
	prev_systicks = systicks;
}


#ifdef __cplusplus
}
#endif





/*
 * @brief 	Processes both ringbuffers and handles their events
 * @param 	menu : Object that links events and handling.
 * @return	Nothing
 */
void processEvents(SimpleMenu& menu){

	while(!e_Ring.IsEmpty()){
		enum MenuItem::menuEvent event;
		event = e_Ring.Pop();

		menu.event(event);
	}

	while(!t_Ring.IsEmpty()){
		enum MenuItem::menuEvent event;
		event = t_Ring.Pop();

		menu.event(event);
	}
}

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}


/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
#endif
#endif

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf


	//Setup FanController
	I2C_config conf;
	FanController controller(conf, 50, 80);

	//Setup pin interrupts for 4 digitalIoPins
	setup_Pin_Interrupts();

	//Initialize Pins for LCD screen
	DigitalIoPin rs(0,8,false,false,false);
	DigitalIoPin en(1,6,false,false,false);
	DigitalIoPin d4(1,8,false,false,false);
	DigitalIoPin d5(0,5,false,false,false);
	DigitalIoPin d6(0,6,false,false,false);
	DigitalIoPin d7(0,7,false,false,false);


	Chip_RIT_Init(LPC_RITIMER);
	//NVIC_EnableIRQ(RITIMER_IRQn);

	//Initialize lcd screen
	LiquidCrystal *lcd = new LiquidCrystal(&rs,&en,&d4,&d5,&d6,&d7);
	lcd->setCursor(0,0);

	SimpleMenu menu;
	IntegerEdit* automatic = new IntegerEdit(lcd, std::string("Automatic"),0,100);
	IntegerEdit* manual = new IntegerEdit(lcd, std::string("Manual"),0,150);
	menu.addItem(new MenuItem(manual));
	menu.addItem(new MenuItem(automatic));
	automatic->setValue(50);
	manual->setValue(50);

	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter

	//Uart config?For debugging or between arduino and LPC?-----------------------IS THIS NEEDED?
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);


	while(1){
		printf("Looping\n");
		Sleep(10);
		processEvents(menu);
	}

	return 1;
}


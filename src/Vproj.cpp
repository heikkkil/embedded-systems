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

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static volatile std::atomic_int counter;
static volatile uint32_t systicks;
static volatile uint32_t prev_systicks;

//Array of type
static enum MenuItem::menuEvent e_Buffer[EVENT_BUFFER_SIZE];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

//Ringbuffer frame
RINGBUFF_T e_Ring;


/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void setup_Pin_Interrupts(){

	Chip_PININT_Init(LPC_GPIO_PIN_INT);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0,
							 (IOCON_DIGMODE_EN |(0x2<<3) |IOCON_MODE_INACT) );
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3,
							 (IOCON_DIGMODE_EN |(0x2<<3) |IOCON_MODE_INACT) );
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 9,
							 (IOCON_DIGMODE_EN |(0x2<<3)| IOCON_MODE_INACT) );
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10,
							 (IOCON_DIGMODE_EN |(0x2<<3)| IOCON_MODE_INACT) );

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 3);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 9);

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 10);


	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	//Link Interrupt channels to physical pins
	Chip_INMUX_PinIntSel(0, 0, 0);
	Chip_INMUX_PinIntSel(1, 1, 3);
	Chip_INMUX_PinIntSel(2, 0, 10);
	Chip_INMUX_PinIntSel(3, 0, 9);

	//Clear interrupt status of pins
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT,PININTCH(0));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT,PININTCH(0));

	//Clear interrupt status of pins
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(1));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT,PININTCH(1));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT,PININTCH(1));

	//Clear interrupt status of pins
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(2));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT,PININTCH(2));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT,PININTCH(2));

	//Clear interrupt status of pins
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(3));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT,PININTCH(3));
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT,PININTCH(3));

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
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if(counter > 0) counter--;
}

void PIN_INT0_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(0));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		RingBuffer_Insert(&e_Ring, (void*)MenuItem::up);
	}
	prev_systicks = systicks;
}

void PIN_INT1_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(1));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		RingBuffer_Insert(&e_Ring, (void*)MenuItem::ok);
	}
	prev_systicks = systicks;
}

void PIN_INT2_IRQHandler(void){
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(2));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		RingBuffer_Insert(&e_Ring, (void*)MenuItem::down);
	}
	prev_systicks = systicks;
}

void PIN_INT3_IRQHandler(void){

Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(3));
	if(systicks - prev_systicks >= BOUNCER_LIMIT ){
		RingBuffer_Insert(&e_Ring, (void*)MenuItem::back);
	}
	prev_systicks = systicks;
}


#ifdef __cplusplus
}
#endif


/**
 * @brief 	Pop events from buffer and call handling functions until buffer is empty.
 * @note	ptr_Event is an empty pointer where data will be saved, no need to worry about warning
 */
void processEvents(SimpleMenu& menu){

	while(!RingBuffer_IsEmpty(&e_Ring)){
		enum MenuItem::menuEvent event;
		RingBuffer_Pop(&e_Ring, &event);

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

	//Setup pin interrupts for 4 digitalIoPins
	//setup_Pin_Interrupts();

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

	//Ringbuffer has to be initialized, e_Ring is something like a frame for the ringbuffer
	//and e_Buffer is a real buffer, on top of which the
	//Ringbuffer frame is put on to create the ultimate ringbuffer.
	RingBuffer_Init(&e_Ring,e_Buffer,sizeof(enum MenuItem::menuEvent),EVENT_BUFFER_SIZE);


	RingBuffer_Insert(&e_Ring, (void*)MenuItem::back);
	RingBuffer_Insert(&e_Ring, (void*)MenuItem::ok);
	RingBuffer_Insert(&e_Ring, (void*)MenuItem::up);
	RingBuffer_Insert(&e_Ring, (void*)MenuItem::down);




	while(1){
		printf("Looping\n");
		Sleep(10);
		processEvents(menu);
	}

	return 1;
}


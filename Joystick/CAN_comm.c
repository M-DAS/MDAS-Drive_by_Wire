#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"





void CAN_Setup(void)
{
	//Enable PORTE
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
	{}

	// Configure the GPIO pin muxing to select CAN0 functions for these pins.
	// This step selects which alternate function is available for these pins.
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	GPIOPinConfigure(GPIO_PE5_CAN0TX);

	//Configure the pins for CAN
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

	
	// Enable the CAN0 module.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0))
	{}

	// Reset the state of all the message objects and the state of the CAN
	// module to a known state.
	CANInit(CAN0_BASE);

	// Configure the controller for 250 Kbit operation.
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 250000);
		
	
	//Set CAN0 to looback test mode
	//CAN0_CTL_R |= CAN_CTL_TEST;
	//CAN0_TST_R |= CAN_TST_LBACK;
	
	//Start CAN0 peripheral
	CANEnable(CAN0_BASE);
}








#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"

#include "Globals_and_Defines.h"

#include "Switches_PORTF.h"
#include "Tick_Timer.h"
#include "Pedal_ADC.h"
#include "CAN_comm.h"
#include "Digital_Pot.h"



void initialization(void)
{

	/***********************
	* Bus Clock            *
	***********************/
	//Set system clock to 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	

	

	
	
	/***************************
	* IO Peripherals           *
	***************************/	
	PORTF_Setup();
	Tick_Timer_Setup();
	ADC0_Setup();
	ADC1_Setup();
  CAN_Setup();
	
	//Turn on clock to GPIOB and I2C0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
	{}
		
	//Configure GPIOB pins to be used by I2C0
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
		
	//Set the Launcpad as the I2C master with the slow clock 100 Kbps	
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
	
	
	/*************************
	* Master Interrupt Enable*
	*************************/	
	IntMasterEnable();
}






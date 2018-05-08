//This is the the braking main.
#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>

#include "Globals_and_Defines.h"
#include "initialization.h"

#include "Switches_PORTF.h"
#include "Linear_Actuator.h"
#include "Steering_Actuator.h"
#include "Throttle.h"

#include "DriveByWireIO.h"

#include "CAN_comm.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"



int main()
{
	uint32_t state =  0;
	
	uint32_t buttonPressed;
	initialization();
	

	while(1)
	{
		if (g_tick_flag == true) //check tick happened
		{
			g_tick_flag = false;   //clear tick_flag
			buttonPressed = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) & GPIO_PIN_7;
			if (buttonPressed != GPIO_PIN_7)
			{
				g_start_flag = true;
				state = 4;
			}
			else 
			{
				g_start_flag = false;
				g_stop_flag = true;
				state = 5;
			}
				
			switch(state)
			{
				
				//Idle State
				case 0:
				{
					g_stop_flag = 0;            //clear stop_flag
					PF3 = 0x08;
					
					if (g_start_flag == true)
					{
						g_start_flag = false;     //clear start_flag
						disable_start();
						state = 1;
						PF3 = 0x00;
					}
					break;
				}
				
				//Engage Clutch Linear Actuator
				case 1:
				{
					//en_clutch_lin_act();   //enable clutch on the linear actuator
					state = 2;
		
					break;
				}
				
				//Enable Motor Linear Actuator
				case 2:
				{
					//en_motor_lin_act();    //enable motor on the linear actuator
					state = 3;
					
					break;
				}
				
				//Move actuators to zero position
				case 3:
				{
					//zero_lin_act();        //move linear actuator to zero
					//zero_steering_act();   //move steering actuator to zero
					//zero_throttle();     //set throttle to zero
					state = 4;
					
					break;
				}
				
				//Drive by wire IO
				case 4:
				{
					if (g_stop_flag == true)
					{
						g_stop_flag = false;   //clear stop flag
						state = 5;             //start stop sequence
					}
					DriveByWireIO();       //read inputs and output to actuators
					PF2 ^= 0x04;           //toggle PF2
					
					break;
				}
				
				//Move actuators to zero position
				case 5:
				{
					disable_start();
					disable_stop();
					PF2 = 0x00;           //clear PF2
					//zero_lin_act();        //move linear actuator to zero
					zero_steering_act();   //move steering actuator to zero
					//zero_throttle();     //set throttle to zero
					state = 6;
					
					break;
				}
				
				//Turn linear actuator motor off
				case 6:
				{
					//disable_motor_lin_act();   //move steering actuator to zero
					state = 7;
					
					break;
				}
				
				//Disengage linear actuator clutch
				case 7:
				{
					//disable_clutch_lin_act();
					enable_start(); 
					enable_stop();
					g_start_flag = false;      //clear start flag
					state = 0;                 //return to idle state
					break;
				}
			}//end case state machine
		
		}//end tick 
		
		
		
	
	}//end while(1)
}//end main


	
	
	
	



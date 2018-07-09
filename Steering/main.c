//This is the steering folder
//At this point, the code has been untested. Written 6/11/2018.
//PRIOR TO TESTING PUT CAR ON JACKSTAND
#define PART_TM4C123GH6PM 1;
#define actuator_torque_threshold 0.00000095367431640625;//In Nm/bit
#define TORQUE_THRESHOLD	55;//Approximate value, may need to be changed

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

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
	
	tCANMsgObject sMsgObjectRx;
	uint32_t initial_canMsg;
	uint32_t new_canMsg;
	uint32_t subbed_canMsg;
	uint32_t torqueIn=0;
	

	while(1)
	{
		CANMessageGet(CAN0_BASE, 1, &sMsgObjectRx, MSG_OBJ_TYPE_RX);
		initial_canMsg = sMsgObjectRx.pui8MsgData && 0x0000FF0000000000; //Take the CAN Data at an earlier point in time.
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
					en_motor_lin_act();    //enable motor on the linear actuator
				
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
					new_canMsg = sMsgObjectRx.pui8MsgData && 0x0000FF0000000000;//CAN message taken at a later time.
					if(sMsgObjectRx.ui32MsgID == 18FF0313)
					{
						if( abs((new_canMsg - initial_canMsg /.1) > TORQUE_THRESHOLD)//100ms Update rate, assuming time is in seconds.
						{
							state = 6;//Will change to state 6, disabling the actuators but leaving the steering and driveshaft in place w/out zeroing.
							break;
						}
					}
					
					if (g_stop_flag == true)
					{
						g_stop_flag = false;   //clear stop flag
						PF2 &= 0x00;					 //Turn off LED
						state = 5;             //start stop sequence
					}
					
					else
					{
					DriveByWireIO();       //read inputs and output to actuators
					PF2 ^= 0x04;           //toggle PF2
					}
					
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


	
	
	
	



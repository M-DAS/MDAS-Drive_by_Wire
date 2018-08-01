#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>

#include "Globals_and_Defines.h"
#include "initialization.h"

#include "Drive_by_Pedal.h"
#include "Drive_by_Wire.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "Switches_PORTF.h"

#include "CAN_comm.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "dac.h"

tCANMsgObject sMsgObjectRx, sMsgObjectTx;
uint32_t state = 0x44;
uint32_t missed_CAN_data_cnt = 0;
uint32_t data_input;
uint32_t target_throttle;


void sendNew(uint32_t newThrottle){
		sMsgObjectTx.ui32MsgID = 0x012FFFFF;
		*sMsgObjectTx.pui8MsgData = state << 8;
		*sMsgObjectTx.pui8MsgData += newThrottle; //Concatinate it.
		CANMessageSet(CAN0_BASE, 1, &sMsgObjectTx, MSG_OBJ_TYPE_TX);
}

int main()
{
	
	initialization();
	while(1)
	{
		state = data_array[0];
		target_throttle = data_array[1];
		target_throttle = (target_throttle<<8)|data_array[2];
		//SetDACVoltage(50);
		if (g_tick_flag == true)   //Check if tick happened
		{
			g_tick_flag = false;     //clear tick_flag
			
			switch(state)
			{				
				//Normal drive mode
				//Throttle position is determined by pedal
				case 0x00:
				{
					//PF3 = 0x08;
					//PF2 = 0x00;
	
					g_tick_flag = false;   //clear tick_flag
					Drive_by_Pedal();
					
					SetDACVoltage(99);
					if (g_throttle_mode == true)
					{
						state = 0x11;
						//PF3 = 0x00;
					}
					break;
				}
				
				
				
				//Drive by wire throttle
				//Throttle position is received over CAN
				case 0x11:
				{
					//Check throttle mode flag
					if (g_throttle_mode == false)
						state = 0x00;
					
						
					//Update missing CAN data counter
					if (g_new_CAN_data == true)
					{
						missed_CAN_data_cnt = 0;
						g_new_CAN_data = false;   //clear new CAN data flag 
					}
					else
						missed_CAN_data_cnt++;
					
					
					//Too many missing CAN data frames reset to drive by pedal mode
					if (missed_CAN_data_cnt >= 2)
					{
						g_throttle_mode = false;
						state = 0;
					}
					
					//Send throttle postion from CAN to motor controller
					Drive_by_Wire();
					
					//PF2 ^= 0x04;                // toggle PF2
					
					break;
				}
				
				
				case 0x22:	//Debug
				{
					//if (g_throttle_mode == false)
						//state = 0;
					
						
					//Update missing CAN data counter
					/*
					if (g_new_CAN_data == true)
					{
						missed_CAN_data_cnt = 0;
						g_new_CAN_data = false;   //clear new CAN data flag 
					}
					else
						missed_CAN_data_cnt++;
					
					
					//Too many missing CAN data frames reset to drive by pedal mode
					if (missed_CAN_data_cnt >= 2)
					{
						g_throttle_mode = false;
						state = 0;
					}
					*/
					//SetDACVoltage(target_throttle);
					SetDACVoltage(50);
					break;
				}
				
				case 0x33: //Error State
				{
					SetDACVoltage(0);
					break;
				}
				
				case 0xFF: //Emergency Stop
				{
					
					break;
				}
				
				
			}//end case state machine
		}// end tick
				
	}//end while loop

}//end main


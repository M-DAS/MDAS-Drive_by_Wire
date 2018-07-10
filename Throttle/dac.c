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
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"


uint16_t ScalePercent12Bit(uint16_t TargetPercent)
{
	uint16_t TargetScaled;
	
	
	return TargetScaled;
}

uint16_t WriteDAC0(uint16_t Voltage25V)
{
	int tempOut;
	I2CMasterSlaveAddrSet(I2C0_BASE, 0x60, false);
	tempOut = Voltage25V >> 8;   //4 MSB's of the triangle wave value
	//remember to set the top 4 MSB's correctly
	I2CMasterDataPut(I2C0_BASE, tempOut); //second byte sent from the I2C module. Top 4 MSB's and control bits
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); //Outputs first byte(address) and second byte
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}
	//set temp to correct value
	tempOut = (Voltage25V & 0x00FF);   //8 LSB's triangle wave value		
	I2CMasterDataPut(I2C0_BASE, tempOut);  //third data byte sent 
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);   //Outputs bottom 8 MSB's
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}
	return 0;
}

uint16_t WriteDAC1(uint16_t Voltage5V)
{
	int tempOut;
	I2CMasterSlaveAddrSet(I2C0_BASE, 0x61, false);
	tempOut = Voltage5V >> 8;   //4 MSB's of the triangle wave value
	//remember to set the top 4 MSB's correctly
	I2CMasterDataPut(I2C0_BASE, tempOut); //second byte sent from the I2C module. Top 4 MSB's and control bits
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); //Outputs first byte(address) and second byte
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}
	//set temp to correct value
	tempOut = (Voltage5V & 0x00FF);   //8 LSB's triangle wave value		
	I2CMasterDataPut(I2C0_BASE, tempOut);  //third data byte sent 
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);   //Outputs bottom 8 MSB's
	while(I2CMasterBusy(I2C0_BASE))  //Wait for I2CO master to finish transmission
	{}
	return 0;
}

int SetDACVoltage(uint16_t setPoint)
{
	uint16_t ScaledVoltage25 = 0;
	uint16_t ScaledVoltage5 = 0;
	ScaledVoltage25 = ScalePercent12Bit(setPoint);
	ScaledVoltage5 = ScaledVoltage25 << 1;
	WriteDAC0(ScaledVoltage25);
	WriteDAC1(ScaledVoltage5);
	return 0;
}

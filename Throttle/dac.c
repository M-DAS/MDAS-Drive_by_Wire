#define PART_TM4C123GH6PM 1;

#include "dac.h"

const uint32_t LowDACPercent[] = {408,421,434,447,460,473,486,499,512,525,538,551,564,577,591,604,617,630,643,656,669,682,695,708,721,734,747,760,773,786,800,813,826,839,852,865,878,891,904,917,930,943,956,969,982,995,1008,1022,1035,1048,1061,1074,1087,1100,1113,1126,1139,1152,1165,1178,1191,1204,1217,1231,1244,1257,1270,1283,1296,1309,1322,1335,1348,1361,1374,1387,1400,1413,1426,1440,1453,1466,1479,1492,1505,1518,1531,1544,1557,1570,1583,1596,1609,1622,1635,1648,1662,1675,1688,1701};	
	
uint32_t ScalePercent12Bit(uint32_t TargetPercent)
{
	uint32_t TargetScaled;
	if (TargetPercent <101 && TargetPercent > 0)
	{
		TargetScaled = LowDACPercent[TargetPercent];
	}
	else if (TargetPercent == 0)
	{
		TargetScaled = 0;
	}
	else if (TargetPercent > 100)
	{
		TargetScaled = 0;
	}
	return TargetScaled;
}

uint32_t WriteDAC0(uint32_t Voltage25V)
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

uint32_t WriteDAC1(uint32_t Voltage5V)
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

uint32_t SetDACVoltage(uint32_t setPoint)
{
	uint32_t ScaledVoltage25 = 0;
	uint32_t ScaledVoltage5 = 0;
	ScaledVoltage25 = ScalePercent12Bit(setPoint);
	ScaledVoltage5 = ScaledVoltage25 << 1;
	WriteDAC0(ScaledVoltage25);
	WriteDAC1(ScaledVoltage5);
	return 0;
}
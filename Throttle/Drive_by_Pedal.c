#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>

#include "Globals_and_Defines.h"
#include "initialization.h"

#include "Pedal_ADC.h"
#include "Digital_pot.h"
#include "dac.h"


//Throttle scaling
uint32_t scale_pedal_throttle_pos(uint32_t input)
{
	uint32_t throttle_percent;
	if (input > 3070)
			throttle_percent = 75;
	else if (input < 48)
	{
			throttle_percent = 0;
	}
	else
	{
		//Scale value to be a percent from 0-4095
	}
	return (throttle_percent);
}



void Drive_by_Pedal(void)
{
	uint32_t adc_pedal_low;
	uint32_t adc_pedal_high;
	uint32_t throttle_percent;
	
	adc_pedal_low = get_throttle_input_low();
	adc_pedal_high = get_throttle_input_high();
	
	
	if (adc_pedal_low == (adc_pedal_high >> 1))
	{
		throttle_percent = scale_pedal_throttle_pos(adc_pedal_high);
	}
	
	
	//send out throttle position to I2C DAC
	SetDACVoltage(throttle_percent);
}




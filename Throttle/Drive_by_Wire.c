#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>

#include "Globals_and_Defines.h"
#include "initialization.h"

#include "Digital_pot.h"
#include "dac.h"


//Throttle scaling
void Drive_by_Wire(uint32_t throttle_percent)
{
	SetDACVoltage(throttle_percent);
}


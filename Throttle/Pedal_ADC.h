
#ifndef _PEDALADC_H_
#define _PEDALADC_H_

/***********************
* Includes             *
***********************/
#include <stdint.h>
#include <stdbool.h>


/***********************
* Public Types         *
***********************/

/***********************
* Public Macros        *
***********************/


/***********************
* Public Defines       *
***********************/


/***********************
* Public Constants     *
***********************/


/***********************
* Public Variables     *
***********************/


/***********************
* Public Functions     *
***********************/
uint32_t get_throttle_input_low(void);
uint32_t get_throttle_input_high(void);
void ADC0_Setup(void);
void ADC1_Setup(void);

#endif /* _PEDALADC_H_ */


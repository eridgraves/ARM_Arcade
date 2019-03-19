// Initialization, and output of DAC on PORT B
// Last modified by: Eric 4/27/15
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "dac.h"
volatile int ba;
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGC2_R |= 0x1; 
	ba = SYSCTL_RCGC2_R;
	GPIO_PORTA_AMSEL_R &= ~0xFC; // 3) disable analog function on PA2-7
  //GPIO_PORTA_PCTL_R &= ~0x00FFFFFF; // 4) enable regular GPIO
  GPIO_PORTA_DIR_R |=0xFC;    // 5) outputs PA2-7
	GPIO_PORTA_DR8R_R |= 0xFC;    // can drive up to 8mA out              
  GPIO_PORTA_AFSEL_R &= ~0xFC; // 6) regular function on PA2-7
  GPIO_PORTA_DEN_R |= 0xFC;   // 7) enable digital on PA2-7
	return;
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63
// Output: none

void DAC_Out(uint32_t data){
	data = data<<2;
	GPIO_PORTA_DATA_R =(GPIO_PORTA_DATA_R & ~0xFC) | (data & 0xFC);
	
	
	return;
	
}

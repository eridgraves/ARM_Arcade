// Gets Input from ADC, contains initialization, a get function
// Eric modified 4/27/15
#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: ADC input on PE2
// Output: none
void ADC_Init(void){ 
  volatile int a;
	volatile int b;
	volatile int c;
	volatile int d;
	volatile int e;
	
	volatile int delay;
	volatile int delay1;
	volatile int delay2;
	volatile int delay3;
	
	
	SYSCTL_RCGC2_R |= 0x10;
	a = SYSCTL_RCGC2_R;
	b = SYSCTL_RCGC2_R;
	c = SYSCTL_RCGC2_R;
	d = SYSCTL_RCGC2_R;
	e = SYSCTL_RCGC2_R;
	a = SYSCTL_RCGC2_R;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5; //Prune these delays
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	
  
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0
  delay = SYSCTL_RCGC2_R;     
	delay1 = SYSCTL_RCGC2_R;  
	delay2 = SYSCTL_RCGC2_R;  
	delay3 = SYSCTL_RCGC2_R; 
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4; //Prune these delays
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
	delay3=2;
	delay3=4;
	delay3=5;
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K
	delay = SYSCTL_RCGC2_R;     
	delay1 = SYSCTL_RCGC2_R;  
	delay2 = SYSCTL_RCGC2_R;  
	delay3 = SYSCTL_RCGC2_R; 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 1;             //    set channel Ain9 (PE4)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
	ADC0_IM_R &= ~0X0008;
	delay = SYSCTL_RCGC2_R;     
	delay1 = SYSCTL_RCGC2_R;  
	delay2 = SYSCTL_RCGC2_R;  
	delay3 = SYSCTL_RCGC2_R; 
	return ; 
}
int abs(int diff){
	if( diff >= 0){
		return diff;
	}
	diff = 0-diff;
	return diff;
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion from lab 8
// Input: none
// Output: 12-bit result of ADC conversion 
int lastinput = 61;
uint32_t ADC_In(void){  
	uint32_t input;
	volatile int vc;
	
  ADC0_PSSI_R = 0x0008;  
	
  // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  input = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
		
	
	if( input > 3650){
		input = 3650;
	}
	if( input < 2550){
		input = 2550;
	}
	input = (input/10) - 255;
	/*
	int diff = abs(input-lastinput);
	if(diff > 12){
	  input = lastinput;
	}
	
	lastinput = input; */
	return input;
}

//int ADC_Level(void){
//	int level = ADC_In();
//	if(level > 20){
//		level = 1;
//		return level;
//	}
//	return 2;
//	}
//	


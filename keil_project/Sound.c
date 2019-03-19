// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>
//#include "Sound.h"
//#include "SpaceInvaders.h.h"
#include "Timer1.h"
#include "dac.h"
int i =0;

void Sound_Init(void){
	Timer1_Init(&Sound_game,3631);


}
void Sound_game(void){
unsigned char beta[3467] = {32,32,32,32,31,31,31,32,31,32,31,31,32,32,32,32,31,31,32,32,32,31,31,31,31,31,32,32,32,31,32,32,31,31,31,31,31,32,31,32,32,32,32,31,31,31,31,31,32,32,31,31,31,32,32,32,32,31,32,32,32,32,31,31,31,32,32,32,32,31,31,32,32,32,32,32,31,32,32,31,31,32,32,31,31,32,32,32,32,31,31,31,31,32,32,31,31,31,31,32,32,31,31,31,32,32,31,32,32,32,32,32,32,32,32,32,31,31,31,31,32,32,31,31,32,32,32,32,32,31,31,32,31,31,31,31,31,31,32,32,32,32,31,32,32,33,33,36,42,48,56,57,49,38,29,22,15,11,8,4,4,8,10,12,12,13,14,13,13,18,25,32,37,41,45,51,53,55,54,51,51,50,51,51,51,51,51,50,43,36,29,23,18,11,7,7,11,14,15,16,15,15,19,24,28,32,36,39,41,43,46,48,50,52,52,51,51,51,51,51,51,49,43,38,33,27,22,18,15,13,9,7,8,11,12,13,13,13,12,12,12,16,21,25,28,32,35,37,39,42,44,47,49,51,52,50,49,49,48,48,49,49,49,44,40,36,32,29,24,21,18,16,14,11,9,10,12,12,12,11,11,10,11,10,11,17,22,25,29,32,35,38,40,43,45,46,47,49,50,53,53,51,52,52,53,53,53,54,54,52,49,45,41,36,33,30,27,24,21,18,16,14,13,11,9,10,11,11,11,10,10,9,8,8,8,11,15,18,22,26,30,33,37,39,41,44,45,47,49,49,50,51,52,53,54,54,55,55,55,55,55,55,54,51,49,48,45,42,38,34,30,26,22,20,17,15,14,12,10,9,8,7,7,8,8,8,7,7,7,7,7,7,7,10,12,15,17,20,23,26,29,32,34,37,40,42,44,46,47,48,50,51,52,53,53,54,54,53,53,53,53,53,53,53,52,51,50,48,47,45,44,42,40,38,36,33,31,28,26,23,20,18,16,15,14,13,12,11,10,9,8,8,7,7,7,8,8,8,9,9,9,9,10,11,13,14,16,19,21,24,26,29,31,33,36,38,40,43,45,47,49,50,51,52,53,54,54,55,55,55,56,56,56,56,55,55,54,54,53,52,51,51,50,49,48,46,45,44,42,40,38,37,35,33,32,31,29,27,26,24,21,19,17,15,13,11,10,9,8,8,7,7,6,6,6,6,6,6,6,7,8,9,10,11,12,13,14,15,16,17,18,20,22,25,27,29,32,34,36,38,41,43,45,46,48,49,51,52,54,55,56,57,58,59,59,59,59,59,59,59,58,58,57,56,56,54,53,52,51,50,48,47,46,44,43,41,40,38,37,36,34,33,32,30,28,27,25,24,22,21,20,19,18,17,16,15,14,14,13,12,11,10,9,8,8,7,7,7,7,7,8,8,8,9,10,10,11,12,12,13,14,15,16,17,19,20,22,23,25,26,27,29,30,31,32,33,34,36,37,38,40,41,42,44,45,46,48,48,49,50,51,51,52,52,52,53,53,53,53,53,53,53,53,53,53,53,52,52,51,50,49,48,47,46,45,44,43,42,40,39,38,36,35,34,32,31,30,29,28,27,26,25,24,23,23,22,21,20,20,19,18,18,17,16,16,15,14,14,14,14,14,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,15,15,16,17,18,19,19,20,21,22,23,24,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,38,39,40,40,41,41,42,43,43,44,45,46,46,47,47,48,48,48,48,49,49,49,49,49,49,49,49,49,49,48,48,48,47,47,46,46,45,44,44,43,42,41,40,39,38,37,36,35,34,34,33,32,31,30,29,29,28,27,26,25,25,24,23,23,22,22,21,21,20,20,20,19,19,19,19,19,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,20,20,20,20,21,21,21,22,22,23,23,23,24,25,25,26,26,27,28,29,29,30,31,32,32,33,34,34,35,35,36,37,37,38,38,39,39,40,40,41,41,41,42,42,42,42,42,42,42,42,42,42,43,43,43,43,43,43,43,43,43,43,43,43,42,42,42,42,41,41,41,41,40,40,40,39,39,39,38,38,37,37,36,36,35,35,34,33,33,32,32,32,31,30,30,29,29,28,28,28,27,27,26,26,26,25,25,25,24,24,24,24,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,25,25,25,25,26,26,26,26,26,27,27,27,28,28,28,28,29,29,29,30,30,30,31,31,32,32,32,32,33,33,33,33,34,34,34,34,35,35,35,35,35,35,35,35,36,36,36,36,36,36,36,36,36,36,36,36,36,35,35,35,35,35,35,35,35,35,34,34,34,34,34,34,34,34,33,33,33,33,33,33,33,33,32,32,32,32,32,32,32,32,32,32,32,32,32,32,31,32,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,32,32,31,31,31,32,32,32,31,31,32,32,31,31,31,30,30,32,31,30,31,32,32,32,31,31,32,32,32,32,32,32,32,32,32,31,30,31,31,31,31,31,30,31,31,31,32,31,31,32,31,31,31,32,32,32,32,32,32,32,32,32,31,30,31,31,32,31,31,32,31,31,31,31,31,31,31,31,31,31,31,32,32,32,31,32,32,31,31,31,31,31,31,31,31,32,31,31,32,32,31,31,31,31,31,32,31,31,31,31,32,32,32,32,31,31,32,31,31,32,32,32,32,32,31,31,31,31,31,31,32,31,31,31,32,32,31,32,32,31,31,31,31,32,31,32,32,31,32,31,32,32,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,32,32,32,32,32,32,32,32,31,32,31,31,31,32,31,31,31,31,31,31,32,32,31,31,31,31,31,32,31,32,32,32,31,31,32,31,32,32,32,31,31,32,32,31,32,31,31,32,32,31,31,32,31,32,32,31,31,32,32,31,32,32,32,31,31,32,32,32,32,31,32,32,31,31,32,31,31,32,31,31,31,32,31,31,31,32,32,31,32,31,31,32,32,32,31,31,32,31,32,32,31,31,32,32,31,31,32,32,31,31,31,32,31,31,32,32,32,32,31,31,31,32,31,31,32,32,31,31,32,32,32,32,31,31,32,31,31,32,32,31,32,32,32,31,31,31,31,32,31,32,32,31,32,32,32,31,32,31,31,32,31,31,32,32,31,31,31,31,32,31,32,32,31,31,31,31,31,32,32,31,32,31,31,31,31,31,32,32,31,31,31,31,31,31,32,32,31,31,31,31,31,31,32,32,32,31,31,31,31,32,32,32,32,31,31,31,32,31,31,32,32,32,31,31,31,31,32,32,32,31,31,32,31,32,31,31,32,32,32,32,32,31,32,32,31,31,32,32,31,31,31,32,32,32,31,32,31,32,32,31,32,32,32,31,31,31,32,31,31,32,31,31,31,32,32,31,31,32,32,32,31,31,31,31,31,31,31,31,32,32,31,31,31,32,32,32,31,31,31,31,31,32,32,32,32,31,31,31,32,32,31,32,32,31,31,32,32,32,32,31,32,31,31,32,32,32,31,32,32,32,31,32,32,32,32,31,32,32,32,31,31,32,31,32,32,31,32,32,31,31,32,32,31,31,32,32,31,31,32,31,32,32,31,32,31,31,31,31,32,32,31,31,32,32,31,31,32,32,31,31,31,32,31,31,31,32,31,31,32,31,31,31,31,31,31,31,31,31,31,32,32,31,31,31,32,32,31,32,32,31,32,32,31,31,32,32,31,32,32,31,31,32,32,32,32,32,32,32,31,32,32,31,32,32,32,32,31,31,32,32,31,32,32,31,31,31,32,32,32,32,31,31,31,31,31,32,32,32,31,31,31,31,32,32,31,31,31,31,31,31,32,31,31,31,32,32,31,31,31,31,31,32,32,31,31,31,32,32,31,31,31,31,31,32,31,31,31,31,31,32,32,31,31,32,32,32,32,31,32,31,31,31,32,32,32,31,31,32,31,32,32,31,32,32,32,32,31,31,32,32,32,32,32,31,31,31,32,32,32,31,31,32,30,32,32,32,32,32,32,32,31,32,32,30,32,32,32,30,30,31,32,31,31,32,31,32,31,31,31,31,31,33,33,31,30,30,32,32,32,33,32,32,31,32,32,31,32,31,31,32,32,31,31,32,31,31,32,32,31,32,32,31,32,32,31,31,31,32,32,31,31,31,32,32,31,31,32,32,31,32,32,31,31,31,32,31,31,31,32,31,31,31,31,32,31,32,32,32,31,31,31,31,32,32,32,32,32,32,31,31,32,32,31,32,32,32,32,31,31,31,32,32,32,32,32,31,31,30,31,32,31,32,31,31,32,31,31,31,32,32,31,31,32,31,31,32,31,31,32,32,31,31,32,31,31,32,31,32,32,31,31,31,32,31,32,31,31,31,31,32,31,32,32,32,32,32,31,31,31,32,32,32,31,31,31,31,32,32,31,32,31,31,32,31,32,32,32,32,31,32,32,32,32,31,31,32,32,32,31,31,32,32,31,31,32,32,31,31,31,32,32,32,32,32,32,32,31,31,31,32,32,32,32,31,32,32,32,32,31,32,31,31,32,32,32,32,32,32,32,32,32,31,32,31,31,32,32,32,32,32,31,31,32,31,32,32,32,32,32,31,32,32,32,32,32,32,31,31,32,32,32,32,32,31,31,32,31,32,32,32,32,32,31,32,31,31,32,32,32,32,31,32,32,31,32,32,32,31,32,32,31,32,32,32,32,32,31,32,31,32,32,31,32,32,31,32,31,32,32,31,32,31,32,32,31,32,31,31,32,32,32,32,32,31,31,31,32,32,32,31,32,31,32,32,31,32,31,31,32,31,32,31,31,32,31,32,32,32,32,32,31,31,31,31,31,31,31,31,32,31,31,31,31,32,31,31,31,31,32,31,31,31,31,31,31,31,32,31,32,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,32,31,31,32,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,31,31,32,31,31,32,32,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,32,32,31,31,31,31,31,31,32,32,31,31,31,32,31,31,31,32,31,31,31,31,32,32,32,32,31,31,32,32,31,32,31,31,32,31,32,31,31,32,31,31,31,32,32,31,32,32,32,32,32,32,32,31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,31,32,32,32,32,31,32,32,31,32,32,31,32,32,31,32,31,31,31,31,31,31,31,31,31,31,31,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,31,32,31,31,32,31,31,31,32,32,32,32,31,31,31,31,31,32,32,32,31,31,32,32,31,31,31,32,32,32,31,32,30,31,31,30,35,33,30,36,31,27,35,29,30,35,30,32,33,31,32,31,26,35,34,28,37,28,28,36,32,29,31,34,32,33,30,28,35,31,30,31,29,36,31,32,32,29,34,30,28,32,34,29,32,33,27,31,31,31,33,33,33,29,28,33,31,29,31,32,34,32,26,30,33,31,32,30,31,31,33,32,29,32,31,31,31,27,32,35,28,30,33,31,32,32,27,33,34,31,33,30,32,33,29,30,34,31,30,32,31,32,35,30,31,31,28,32,32,30,35,32,29,32,29,32,34,30,32,32,31,31,32,31,31,32,30,32,33,31,33,32,30,32,29,31,34,28,32,34,30,32,30,31,34,30,30,35,32,30,31,28,33,33,31,32,32,31,31,30,31,35,31,30,33,30,32,30,30,36,31,29,32,32,32,34,30,31,34,28,31,33,31,33,31,29,33,33,28,33,33,30,34,30,30,33,31,30,33,31,30,33,29,31,33,29,31,33,30,31,31,29,34,32,29,32,30,31,33,30,30,32,29,31,32,30,32,32,30,32,32,28,32,32,30,33,30,30,33,30,31,32,30,32,32,29,32,31,30,32,30,32,32,29,31,33,30,31,32,29,32,32,29,33,32,29,32,31,31,33,30,31,32,30,31,33,31,32,32,29,32,32,30,32,32,31,32,31,31,32,31,32,31,31,32,31,31,32,32,31,32,31,32,32,31,31,32,32,32,32,30,33,32,30,33,31,31,33,31,31,32,31,31,32,32,31,33,30,31,33,30,32,33,31,32,32,30,32,32,30,32,31,31,32,31,31,33,31,31,32,31,32,32,32,32,32,32,32,31,32,32,32,32,31,32,32,32,32,32,32,31,32,32,32,32,31,32,32,31,32,32,32,33,32,31,32,32,31,32,32,32,32,31,31,32,32,31,32,32,31,32,31,32,32,31,32,32,31,32,32,32,32,32,31,32,32,31,32,32,31,32,31,32,32,32,32,31,32,32,31,32,32,31,32,31,31,32,32,32,32,31,31,32,31,32,32,31,32,32,31,32,32,31,32,32,31,32,31,31,32,31,31,32,32,32,32,31,31,32,31,32,32,31,31,32,31,32,32,31,32,32,31,32,31,31,32,31,32,31,31,32,32,32,32,31,32,32,31,32,32,31,31,31,31,32,31,31,32,31,31,32,31,32,32,31,31,32,31,32,32,31,32,32,31,31,32,30,32,32,31,32,32,31,32,32,31,32,32,31,31,32,31,32,31,31,32,32,31,31,32,32,31,31,32,32,31,31,31,32,32,31,32,31,32,31,31,31,32,32,31,31,32,31,32,31,31,32,31,31,31,31,32,31,31,31,31,32,31,32,31,32,31,31,32,31,32,32,31,32,32,31,31,32,31,32,32,31,32,32,31,32,31,31,32,32,31,32,31,32,32,31,32,32,32,32,31,32,32,31,32,31,31,32,32,31,32,32,32,31,31,32,31,32,32,31,32,32,31,32,32,31,32,31,31,32,31,31,32,31,31,32,32,31,32,31,31,32,31,32,32,31,32,31,32,32,32,32,32,32,31,32,31,32,32,31,32,32,31,32,31,31,32,32,31,31,32,32,31,32};
if(i==3467)
	{i=0;
	}
	DAC_Out(beta[i]);
	i+=1;
};




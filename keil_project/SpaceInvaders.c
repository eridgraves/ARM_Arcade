// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 32*R resistor DAC bit 0 on PB0 
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2
// 4*R resistor DAC bit 3 on PB3 
// 2*R resistor DAC bit 0 on PB4 
// 1*R resistor DAC bit 0 on PB5 
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "DAC.h"
#include "Sound.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void SysTick_Init(uint32_t period);
void SysTick_Handler(void);

// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10

const unsigned short frontpage[] = {
 0x1B24, 0x1304, 0x1324, 0x1B45, 0x1B44, 0x1B44, 0x1B64, 0x1B84, 0x1B64, 0x1B65, 0x1B85, 0x1364, 0x1B64, 0x1B85, 0x1BA5, 0x1B64,
 0x1B64, 0x1B85, 0x1B85, 0x1B64, 0x1364, 0x1B64, 0x1344, 0x1344, 0x1B65, 0x1B44, 0x1344, 0x1B44, 0x1B45, 0x1324, 0x1B45, 0x1B24,
 0x1324, 0x1B65, 0x1324, 0x1B44, 0x1B65, 0x1BA5, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1B84, 0x1BC5, 0x1BA5, 0x1BA5, 0x1BC5,
 0x1BC5, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1B84, 0x1B85, 0x1B85, 0x1B65, 0x1B45, 0x1324, 0x1B45, 0x1B45, 0x1B24, 0x1B45, 0x2385,
 0x1B65, 0x1B65, 0x1B85, 0x23C5, 0x1B85, 0x1BA5, 0x1B64, 0x12A4, 0x1AE4, 0x12C4, 0x12C3, 0x12E4, 0x12E4, 0x1AE4, 0x12C4, 0x1BC5,
 0x1BA5, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1344, 0x1B44, 0x1B45, 0x1B65, 0x1324, 0x1B44, 0x1B64, 0x1B64, 0x1384, 0x1B84,
 0x1BA5, 0x1BA5, 0x1B85, 0x1BC4, 0x120C, 0xB516, 0x6AED, 0xB536, 0xA4D4, 0xACF5, 0x9432, 0x8BF0, 0x8396, 0x12E6, 0x1BC5, 0x1B85,
 0x1B85, 0x1B85, 0x1B85, 0x1B84, 0x1B64, 0x1B64, 0x1B64, 0x1B64, 0x1B44, 0x1B64, 0x1B64, 0x1B85, 0x1B85, 0x1B85, 0x1BC5, 0x1BA5,
 0x1B85, 0x1BA5, 0x1304, 0x5C0C, 0x3B27, 0x5C2B, 0x542B, 0x5C2C, 0x8CD0, 0x43A9, 0x4349, 0x1364, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BA5,
 0x1BA5, 0x1BA5, 0x1B65, 0x1B44, 0x1B85, 0x1B64, 0x1B65, 0x1B65, 0x1BA5, 0x23A5, 0x1BA5, 0x1BA5, 0x23E6, 0x1BA5, 0x1BA5, 0x23C5,
 0x2405, 0x1BE5, 0x1C05, 0x1BE5, 0x0BC4, 0x8CC9, 0xFE4B, 0x4CA9, 0x13E5, 0x1BC5, 0x1BC5, 0x23E6, 0x1BA5, 0x23C5, 0x23C6, 0x23E6,
 0x1BA5, 0x1B84, 0x23A5, 0x1B85, 0x1B64, 0x1B64, 0x1B85, 0x1B85, 0x1BA5, 0x1BA5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1BE5, 0x1BE5,
 0x1BC5, 0x1BE5, 0x1C26, 0x2C07, 0x854D, 0x13C5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1B85, 0x1BA5, 0x1BA5, 0x1BC5, 0x1BC5, 0x1BA5, 0x1B85,
 0x1B64, 0x1B64, 0x1B64, 0x1B64, 0x1B84, 0x1BA5, 0x1BC5, 0x1BA5, 0x1BA5, 0x23C5, 0x1BC5, 0x23E6, 0x1BE5, 0x1BE5, 0x1BC5, 0x23E5,
 0x2426, 0x1BE5, 0x13E5, 0x1BE5, 0x1BC5, 0x23E5, 0x1BE5, 0x1BA5, 0x1BC5, 0x1BA5, 0x1BA5, 0x1BC5, 0x1BA5, 0x1B85, 0x1B85, 0x1B64,
 0x1BA5, 0x1BC5, 0x1BA5, 0x1BE5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BC5, 0x1BC5, 0x1BE5, 0x1BE5, 0x1BE5, 0x1C05, 0x2446, 0x2446, 0x2426,
 0x2426, 0x2446, 0x1BE5, 0x2405, 0x2426, 0x1BE5, 0x1BE5, 0x1BE5, 0x1BC5, 0x1BC5, 0x1BA5, 0x1BA5, 0x23C5, 0x1B64, 0x1B85, 0x1384,
 0x1B84, 0x1BE5, 0x23C5, 0x23E6, 0x2406, 0x23E6, 0x23E5, 0x1BE5, 0x2426, 0x2406, 0x2406, 0x1C05, 0x2446, 0x2405, 0x2426, 0x2446,
 0x2406, 0x1BE5, 0x1BE5, 0x2426, 0x1C06, 0x1BE5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1BA5, 0x1B85, 0x1B85, 0x1B64, 0x1B85, 0x1BA5, 0x1BE5,
 0x1BA5, 0x1BC5, 0x1BE5, 0x2406, 0x1BE5, 0x1BE5, 0x2426, 0x1C05, 0x1C05, 0x1C05, 0x2446, 0x2426, 0x1C05, 0x2426, 0x1C05, 0x1C05,
 0x1C05, 0x2426, 0x1BC5, 0x1BC5, 0x1BE5, 0x1BC5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1B64, 0x1B85, 0x1BC5, 0x1BE5, 0x1BA5, 0x1BE5,
 0x1BE5, 0x1BE5, 0x1BE5, 0x1C05, 0x1C25, 0x1C05, 0x1C05, 0x1C26, 0x2466, 0x1C26, 0x1C26, 0x2426, 0x2426, 0x1C25, 0x1C25, 0x2446,
 0x1BC5, 0x1BE5, 0x23E5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1BC5, 0x1B85, 0x1BA4, 0x1BA4, 0x1BE5, 0x23C5, 0x2405, 0x2425, 0x2426,
 0x2446, 0x2425, 0x2446, 0x2446, 0x2446, 0x2445, 0x2C86, 0x2466, 0x2445, 0x2C86, 0x2425, 0x2426, 0x2425, 0x2446, 0x2405, 0x2405,
 0x2405, 0x2406, 0x1BC5, 0x1BA5, 0x23E6, 0x1BC5, 0x23C5, 0x1287, 0x11E8, 0x1209, 0x1208, 0x12A4, 0x1249, 0x2425, 0x12E4, 0x11E9,
 0x1209, 0x12C7, 0x1C25, 0x0A08, 0x1209, 0x12A6, 0x1288, 0x1288, 0x1B87, 0x1BC4, 0x126A, 0x0A46, 0x1209, 0x1229, 0x1325, 0x1326,
 0x1248, 0x1BA4, 0x23C5, 0x1B64, 0x1B63, 0x2CF5, 0x365F, 0x361A, 0x3EFF, 0x1B0A, 0x3E1D, 0x1BC2, 0x242E, 0x369F, 0x2D5B, 0x1BD2,
 0x1281, 0x3EDF, 0x369D, 0x2471, 0x2DBA, 0x2D78, 0x1289, 0x0984, 0x3E9E, 0x2516, 0x361E, 0x2D7C, 0x1288, 0x23CB, 0x2D9A, 0x1B83,
 0x1BA5, 0x1B85, 0x1B63, 0x2CF5, 0x23D2, 0x11E7, 0x35DD, 0x1B6A, 0x359B, 0x0A00, 0x242E, 0x2C75, 0x3E3E, 0x3E7F, 0x132B, 0x361E,
 0x1220, 0x12A5, 0x2D99, 0x24D4, 0x3EBF, 0x3EBF, 0x246B, 0x2537, 0x2C33, 0x3E5E, 0x3E5F, 0x1B0A, 0x2CF7, 0x1200, 0x1BA5, 0x1BA5,
 0x1B63, 0x24D4, 0x2DBA, 0x2516, 0x3E5F, 0x12E4, 0x3E9F, 0x2CDA, 0x1BAF, 0x361F, 0x2477, 0x2DFF, 0x242E, 0x369F, 0x2498, 0x2D1B,
 0x2D99, 0x2D16, 0x138B, 0x3E7F, 0x12E0, 0x2D99, 0x2D3B, 0x1C77, 0x3EBF, 0x1B8A, 0x365F, 0x2CDA, 0x1B04, 0x1BA5, 0x1B63, 0x2CF6,
 0x2435, 0x128D, 0x46BF, 0x1301, 0x24AD, 0x2CEF, 0x2427, 0x24CD, 0x24EE, 0x24ED, 0x1C25, 0x24CB, 0x24EE, 0x24EE, 0x1C26, 0x2D59,
 0x240B, 0x24AC, 0x1C26, 0x2448, 0x2CCE, 0x24CE, 0x246B, 0x1BC4, 0x248D, 0x24CE, 0x1B85, 0x1B84, 0x1B84, 0x244D, 0x2555, 0x2D75,
 0x24CF, 0x1BA4, 0x1BC5, 0x1BE5, 0x1BE5, 0x1BE4, 0x1C25, 0x1C05, 0x1C25, 0x2425, 0x1C04, 0x2405, 0x1C04, 0x2D32, 0x246A, 0x1C04,
 0x2425, 0x2426, 0x1BC4, 0x1BC4, 0x1BA4, 0x1BE5, 0x1B84, 0x1B84, 0x1B85, 0x1B65, 0x1B85, 0x1B64, 0x1BA3, 0x1BA3, 0x1BC4, 0x23E6,
 0x1BE6, 0x1C05, 0x1C05, 0x2426, 0x2466, 0x2446, 0x2446, 0x2446, 0x2C87, 0x2466, 0x1C46, 0x2424, 0x2445, 0x2446, 0x2446, 0x2446,
 0x1C05, 0x1BE5, 0x2406, 0x2406, 0x1BA5, 0x1BA5, 0x1B85, 0x1B85, 0x1B84, 0x1B85, 0x1BC5, 0x1BA5, 0x1BC5, 0x1BC5, 0x1BA4, 0x0A01,
 0x1221, 0x0A21, 0x1BC4, 0x12A1, 0x2405, 0x1C05, 0x12A1, 0x1BA4, 0x1261, 0x1281, 0x1303, 0x12A2, 0x1BC5, 0x2405, 0x1261, 0x1BC5,
 0x1BA5, 0x1BC5, 0x1BC5, 0x1BA5, 0x23C5, 0x1B64, 0x1B85, 0x1B84, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BC4, 0x1B88, 0x2CFB, 0x2D1A, 0x2D1B,
 0x12EA, 0x2CF9, 0x1364, 0x1B85, 0x2CD9, 0x12E9, 0x2D1B, 0x2CFB, 0x23F1, 0x2C97, 0x1325, 0x12A5, 0x2D19, 0x1B83, 0x1BC5, 0x1BC5,
 0x1B84, 0x1B85, 0x1BC5, 0x1B64, 0x1B64, 0x1B85, 0x1B85, 0x1BA5, 0x1BA5, 0x1BA4, 0x23C9, 0x35DD, 0x1266, 0x2453, 0x2452, 0x3E7F,
 0x1B84, 0x1BA4, 0x365F, 0x2410, 0x2556, 0x2CEC, 0x2427, 0x361D, 0x12CE, 0x2477, 0x2513, 0x1B83, 0x1BC5, 0x23C5, 0x1BA5, 0x1BA5,
 0x23C5, 0x1B64, 0x1B85, 0x1B85, 0x1B85, 0x1BA5, 0x1BC5, 0x1BA4, 0x2388, 0x367F, 0x2D7C, 0x35FE, 0x1BAB, 0x3E9F, 0x00E2, 0x1223,
 0x365F, 0x2410, 0x1350, 0x00C1, 0x1241, 0x2DDC, 0x2DDB, 0x35FE, 0x1B84, 0x1BA5, 0x1BC5, 0x23A5, 0x1BA5, 0x1B84, 0x1B85, 0x1B64,
 0x1B44, 0x1344, 0x1B64, 0x1B65, 0x1BA5, 0x1B84, 0x1B68, 0x2D7C, 0x0141, 0x2D9B, 0x1B26, 0x2DDA, 0x4F5F, 0x2C51, 0x35FC, 0x1C2C,
 0x46FF, 0x4F3F, 0x2C50, 0x2DDC, 0x1306, 0x3E5E, 0x1B43, 0x1BA5, 0x1BA5, 0x1B85, 0x1B84, 0x1B84, 0x1B65, 0x1324, 0x1B44, 0x1344,
 0x1B65, 0x1B85, 0x1B65, 0x1B64, 0x1BA9, 0x3EDF, 0x3DFE, 0x3EDF, 0x1BC8, 0x1342, 0x1362, 0x1B64, 0x2497, 0x1B62, 0x1342, 0x1383,
 0x1301, 0x365F, 0x1B66, 0x1B83, 0x1BC5, 0x1BA5, 0x1B64, 0x1B85, 0x1B64, 0x1B65, 0x2385, 0x1344, 0x1B44, 0x1B85, 0x2385, 0x1B65,
 0x1B65, 0x1B85, 0x1BA5, 0x1386, 0x1BC6, 0x1BA6, 0x23C5, 0x1BA5, 0x1BA5, 0x1BC5, 0x1C09, 0x1B84, 0x1BA5, 0x2406, 0x1BA5, 0x1C09,
 0x23E6, 0x23A5, 0x1BC6, 0x1BC5, 0x1B85, 0x23E6, 0x1B64, 0x1B44, 0x1B65, 0x1B64, 0x1B44, 0x1365, 0x1364, 0x1B65, 0x1B65, 0x1B85,
 0x1B85, 0x1B84, 0x1B84, 0x1BA5, 0x1BA5, 0x1BC5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BA5, 0x1BC5, 0x1B85, 0x1B84, 0x1384, 0x23A5,
 0x1364, 0x1B64, 0x1B64, 0x1B85, 0x1B44, 0x1344, 0x1B45, 0x1B45, 0x1324, 0x1344, 0x1B64, 0x1B65, 0x1B44, 0x1B64, 0x1B85, 0x1B85,
 0x1B85, 0x1B85, 0x1BA5, 0x1385, 0x1B64, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1BA5, 0x1B85, 0x1B65, 0x1B85, 0x1B85, 0x1B64, 0x1B65,
 0x1B44, 0x1B65, 0x1344, 0x1344, 0x1B44, 0x1B24, 0x1B04, 0x1B24, 0x1B44, 0x1B85, 0x1B44, 0x1364, 0x1B64, 0x1B85, 0x1B64, 0x1B85,
 0x1B85, 0x1B64, 0x1B64, 0x1B85, 0x1B85, 0x1B85, 0x1B85, 0x1BA5, 0x1B85, 0x1B65, 0x1B85, 0x1B85, 0x1B64, 0x1B64, 0x1344, 0x1B64,
 0x1B65, 0x1B64, 0x1B45, 0x1304,

};

const unsigned short asw[] = {
 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0800, 0x0800, 0x0800,
 0x0800, 0x0000, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0000, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0800,
 0x0800, 0x0800, 0x0800, 0x0000, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0800, 0x0800, 0x0800, 0x0000, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x0800,
 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800,
 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	
};

const unsigned short paddlefinal[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0027, 0x0027, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0003, 0x0029, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5088, 0x023F, 0x017B, 0x5287, 0x73AE, 0x738E, 0x738E, 0x738E, 0x738E,
 0x7BCD, 0x00EE, 0x021F, 0x20D3, 0x2000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9B0F, 0x757F, 0x5C5D, 0x9470, 0xD69A,
 0xCE79, 0xCE79, 0xCE79, 0xCE79, 0xDEBA, 0x3A90, 0x7DBF, 0x5AD9, 0x4020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x006E, 0x002C, 0x2923, 0x39C7, 0x31A6, 0x31A6, 0x31A6, 0x31A6, 0x39C6, 0x0026, 0x0070, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000


};


const unsigned short whiteball[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xDEDB, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000


};

const unsigned short blackball[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000


};


const unsigned short laser[] =  { //3x19
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10A2, 0x0861, 0x1081, 0x527F, 0x109F, 0x7BDF, 0x4A5D,
 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D,
 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E,
 0x4A5D, 0x109D, 0x739E, 0x4A5D, 0x109D, 0x739E, 0x529D, 0x18DD, 0x7BBE,

};
const unsigned short blaser[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

};
const unsigned short greenbrick[] = {
 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0F00, 0x0700, 0x0F20, 0x1DC1,
 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x0EC0,
 0x07C0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x1680,
 0x07C0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x16A0,
 0x07C0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07C0, 0x07E0, 0x0EA0,
 0x07C0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07A0, 0x07E0, 0x0EA0,
 0x07C0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x16A0,
 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07C0, 0x07E0, 0x07C0, 0x07E0, 0x1680,


};
const unsigned short bluebrick[] = {
 0xE6E0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6E0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6C0,
 0xE6C0, 0xE6C0, 0xE6C0, 0xE6E0, 0xE6C0, 0xE6C0, 0xE6C0, 0xE6E0, 0xE700, 0x9BC1, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xAC61, 0xFFA0, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFC0, 0xFFE0, 0xA441, 0xFFC0, 0xFFC0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xAC60, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xAC40, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xA460, 0xFFC0, 0xFFC0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFC0, 0xFFC0, 0xFFE0, 0xA440, 0xFFC0, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0,
 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xAC41,


};

const unsigned short yellowbrick[] = {
 0x0E99, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E79, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E79, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E79, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E79, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E79, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x0E99, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x07FF, 0x0EBA,
 0x1659, 0x07DF, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07BE, 0x07DF, 0x1699,


};

const unsigned short blackbrick[] = {
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	


};

const unsigned short pbrick[] = {
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB1F, 0xCB3F, 0xCB1F, 0xCB3F, 0xCB1F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB1F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,
 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB1F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F, 0xCB3F,


};


const unsigned short rbrick[] = {
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,
 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001A,


};



const unsigned short obrick[] = {
 0x2292, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x1A0F, 0x32D3,
 0x1A50, 0x2338, 0x2338, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2338, 0x2358, 0x1A2F,
 0x1A2F, 0x2359, 0x2359, 0x2379, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2379, 0x2359, 0x2359, 0x1A0E,
 0x19EE, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x19ED,
 0x2B98, 0x2359, 0x2359, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2B79, 0x2359, 0x2359, 0x2B78,
 0x2BB9, 0x2379, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2359, 0x2379, 0x2B99,
 0x33B9, 0x2359, 0x2338, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2358, 0x2338, 0x2359, 0x2B99,
 0x22D4, 0x2C3E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x343E, 0x2C3E, 0x22D4,


};








//**************************** Definitions **********************
int score;
char level = 1;
char flip =1;
int lives;
int vertb =0;
int horb=0;
int lowerb=0;
int brick1=0;
int brick2=0;
int brick3=0;
int brick4=0;
int brick5=0;
int missile=0;
int missilecount=2;
int control =0;
int laseron = 0;
int lazrEnable = 0;
int laserdelay = 30;
int fireEnable = 1;
int edgeTrigger =1;
int reset; //iterations of the game
	int o;
	int t ;
	int th;
	int  f;
	int fi;
	int o2;
	int t2;
	int th2;
	int  f2;
	int fi2;
	int higherreset;
struct paddle{
	int leftx;
	int boty;
}pad;

struct wball{
	int leftx;
	int boty;
	int velx;
	int vely;
}wbal;

struct bball{
	int leftx;
	int boty;
}bbal;



struct nxtball{
	int leftx;
	int boty;
	int velx;
	int vely;
}nxtbal;
	
struct brick{
	int leftx;
	int boty;
	int health;
}one,two,three,four,five;

struct laser{
		int xbotlft;
	int ybotlft;
	int vely;
}lazr;

void PrintPaddle(void){
	int adcval = 0;
	for(int i =19; i>=0; i--){
		adcval += ADC_In();
	}
	adcval /= 20;
	pad.leftx = adcval;
	ST7735_DrawBitmap(pad.leftx, pad.boty,paddlefinal,20 ,5); //draw paddle
	ST7735_DrawBitmap((pad.leftx + 20), pad.boty,asw,50 ,5); //draw clearing
	ST7735_DrawBitmap((pad.leftx - 50), pad.boty,asw,50 ,5);
}

void firstgame(void){
	PrintPaddle();
	//ST7735_DrawBitmap(pad.leftx, pad.boty,paddlefinal,20 ,5); // player ship middle bottom
  ST7735_DrawBitmap(wbal.leftx,wbal.boty,whiteball, 4,4);
	ST7735_DrawBitmap(bbal.leftx,bbal.boty,blackball, 4,4);
  if(one.health==2){
  ST7735_DrawBitmap(one.leftx,one.boty, pbrick, 16,8);}
	if(two.health==1){
  ST7735_DrawBitmap(two.leftx,two.boty, bluebrick, 16,8);}
	if(three.health==2){
  ST7735_DrawBitmap(three.leftx,three.boty, rbrick, 16,8);}
	if(four.health==1){
  ST7735_DrawBitmap(four.leftx,four.boty, greenbrick, 16,8);}
	if(five.health==1){
	ST7735_DrawBitmap(five.leftx,five.boty,yellowbrick, 16,8);}
	missilecount =2;
	ST7735_SetCursor(0,0);
  ST7735_OutString("SCORE:");
	//ST7735_SetCursor(90, 156);
	LCD_OutDec(score);
  ST7735_OutString(" LASERS: ");
	ST7735_SetCursor(120, 156);
  LCD_OutDec(missilecount);
  ST7735_OutString("  ");
	
	}   
void firstgame1(void){
	PrintPaddle();
	//ST7735_DrawBitmap(pad.leftx, pad.boty,paddlefinal,20 ,5); // player ship middle bottom
  ST7735_DrawBitmap(wbal.leftx,wbal.boty,whiteball, 4,4);
	ST7735_DrawBitmap(bbal.leftx,bbal.boty,blackball, 4,4);
  if(one.health==1){
  ST7735_DrawBitmap(one.leftx,one.boty, pbrick, 16,8);}
	if(two.health==1){
  ST7735_DrawBitmap(two.leftx,two.boty, bluebrick, 16,8);}
	if(three.health==1){
  ST7735_DrawBitmap(three.leftx,three.boty, rbrick, 16,8);}
	if(four.health==1){
  ST7735_DrawBitmap(four.leftx,four.boty, greenbrick, 16,8);}
	if(five.health==1){
	ST7735_DrawBitmap(five.leftx,five.boty,yellowbrick, 16,8);}
	missilecount =2;
	ST7735_SetCursor(70,156);
  ST7735_OutString("SCORE: ");
	ST7735_SetCursor(90, 156);
	LCD_OutDec(score);
  ST7735_OutString(" LASERS: ");
	ST7735_SetCursor(120, 156);
  LCD_OutDec(missilecount);
}   

void Game_Init(void){
	score = 0;
	lives = 3;
	missilecount =2;
	// set paddle to middle
	pad.leftx = 53;
	pad.boty = 149;
	// set ball on paddle
	wbal.leftx = 82;
	wbal.boty = 145;
	wbal.velx = 0;
	wbal.vely = -4;
	bbal.leftx =57;
	bbal.boty =145;
	// set brick health and position
	level ^= flip;
	switch(level){
		case 0: 
			one.leftx =10  ; //fill these positions in
			one.boty = 30 ;
			one.health = 2;
			two.leftx = 100;
			two.boty = 30;
			two.health = 1;
			three.leftx = 10;
			three.boty = 50;
			three.health = 2;
			four.leftx =100 ;
			four.boty =50 ;
			four.health = 1;
			five.leftx = 61;
			five.boty = 40;
			five.health = 1;
			break;
		case 1:
			one.leftx =0; //fill these positions in
			one.boty = 20 ;
			one.health = 2;
			two.leftx = 20;
			two.boty = 20;
			two.health = 1;
			three.leftx = 60;
			three.boty = 20;
			three.health = 2;
			four.leftx =80 ;
			four.boty =20 ;
			four.health = 1;
			five.leftx = 100;
			five.boty = 20;
			five.health = 1;
			break;
		/*case 3:
			one.leftx = ;
			one.boty = ;
			one.health = 1; */
		default:
			one.leftx =20  ; //fill these positions in
			one.boty = 30 ;
			one.health = 2;
			two.leftx = 90;
			two.boty = 30;
			two.health = 1;
			three.leftx = 10;
			three.boty = 70;
			three.health = 2;
			four.leftx =100 ;
			four.boty =70 ;
			four.health = 1;
			five.leftx = 61;
			five.boty = 40;
			five.health = 1;
	}
	
}



void CheckforXbounds(void){
	  if(nxtbal.leftx < -3){ // if nextbal leaves screen on the right
			vertb = 1;
			return;
		}
		if(nxtbal.leftx > 129){ // if nextball leaves the screen on right
			vertb = 1;
			return;
		}
		vertb = 0;
}
		
void CheckforYbounds(void){
		if(nxtbal.boty < 1){
				horb = 1;// sets horb depending on if it goes and touches top
				return;
			}
		horb = 0;
}

int Check_Paddle(void){
	
	if(((nxtbal.leftx+3)>=(pad.leftx))&& ((nxtbal.leftx+3)<=(pad.leftx+19)))
	{ if((nxtbal.leftx-pad.leftx)<5)
		{wbal.velx=1;
		 wbal.vely=-4;}
		else if((nxtbal.leftx-pad.leftx)<10)
		{ wbal.velx = 2;
			wbal.vely= -3;
		  
		}
		else if((nxtbal.leftx-pad.leftx)==10)
		{ wbal.velx = 0;
			wbal.vely= -4;
		  
		}
		else if((nxtbal.leftx-pad.leftx)<15)
		{ wbal.velx =3;
			wbal.vely =3;
		}
		 else
		 {  wbal.velx=4;
			  wbal.vely=-1;
		 
		 }
		
		
	  return 0;
		}
	else if((nxtbal.leftx<=(pad.leftx+19)) && (nxtbal.leftx>=pad.leftx))
	{   wbal.velx =4;
		  wbal.vely =-1;
	    return 0;
		}
		else
		{ higherreset =0;
		  return 1;
		}
	}

void Checkonehor(void){

	
	if(one.health >0)
	{   //o and 02 are zero
		
		if(((nxtbal.boty+4)>=one.boty)&&((nxtbal.boty)+4)<=((one.boty)+7))
		{  if(((nxtbal.leftx+4)>=one.leftx)&&((nxtbal.leftx+4)<=(one.leftx+15)))
			{
			  if(nxtbal.boty>=one.boty&&nxtbal.boty<=(one.boty+7) ){
			  o2=0;
				one.health-=1;
				if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
			  wbal.velx= -(wbal.velx);
				 score+=10;
				//wbal.boty = wbal.boty + wbal.vely;
				//wbal.leftx += wbal.velx;
				
			}
			}
	  }
			else if((nxtbal.boty>=one.boty)&&(nxtbal.boty<=(one.boty+7)))
			{
			  if(((nxtbal.boty+4)>=one.boty) &&((nxtbal.boty+3)<=(one.boty+7 )))
				{
					if((nxtbal.leftx>=one.leftx) && ((nxtbal.leftx <=(one.leftx+15))))
					{
					o2=0;
				 one.health-=1;
				  if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
						score+=10;
			    wbal.velx= -(wbal.velx);
				//	wbal.boty = wbal.boty + wbal.vely;
			//	wbal.leftx += wbal.velx;
					}
				
				
				}
			
			}
	
	}
}

void Checktwohor(void){
	
	if(two.health >0)
	{   //o and 02 are zero
		
		if(((nxtbal.boty+4)>=two.boty)&&(((nxtbal.boty)+4)<=((two.boty)+7)))
		{  if(((nxtbal.leftx+4)>=two.leftx)&&((nxtbal.leftx+3)<=(two.leftx+15)))
			{
			  if((nxtbal.boty>=two.boty)&&(nxtbal.boty<=(two.boty+7))){
			  t2=0;
				two.health-=1;
				ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
			  wbal.velx= -(wbal.velx);
				score+=10;
			//	wbal.boty = wbal.boty + wbal.vely;
			//	wbal.leftx += wbal.velx;
				
			}
			}
	  }
			else if(nxtbal.boty>=two.boty&&nxtbal.boty<=(two.boty+7))
			{
			  if((nxtbal.boty+4)>=two.boty &&(nxtbal.boty+4)<=(two.boty+7 ))
				{
					if((nxtbal.leftx>=two.leftx) && (nxtbal.leftx <=(two.leftx+15)))
					{
					t2=0;
				 two.health-=1;
				  ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
			    wbal.velx= -(wbal.velx);
						score+=10;
			//		wbal.boty = wbal.boty + wbal.vely;
			//	  wbal.leftx += wbal.velx;
					
					}
				
				
				}
			
			}
	
	}

	
	
}



void Checkthreehor(void){
	
	if(three.health >0)
	{   //o and 02 are zero
		
		if(((nxtbal.boty+3)>=three.boty)&&(((nxtbal.boty)+3)<=((three.boty)+7)))
		{  if(((nxtbal.leftx+4)>=three.leftx)&&((nxtbal.leftx+4)<=(three.leftx+15)))
			{
			  if((nxtbal.boty>=three.boty)&&(nxtbal.boty<=(three.boty+7))){
			  th2=0;
				three.health-=1;
				if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
					score+=10;
			  wbal.velx= -(wbal.velx);
			//	wbal.boty = wbal.boty + wbal.vely;
			//	wbal.leftx += wbal.velx;
				
			}
			}
	  }
			else if(nxtbal.boty>=three.boty&&nxtbal.boty<=(three.boty+7))
			{
			  if((nxtbal.boty+4)>=three.boty &&(nxtbal.boty+4)<=(three.boty+7))
				{
					if((nxtbal.leftx>=three.leftx) && (nxtbal.leftx <=(three.leftx+15)))
					{
					th2=0;
				 three.health-=1;
					score+=10;
				  if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
			    wbal.velx= -(wbal.velx);
			//		wbal.boty = wbal.boty + wbal.vely;
			  // 	wbal.leftx += wbal.velx;
					
					}
				
				
				}
			
			}
	
	}

	
	
}

void Checkfourhor(void){
	
	if(four.health >0)
	{   //o and 02 are zero
		
		if(((nxtbal.boty+4)>=four.boty)&&(((nxtbal.boty)+4)<=((four.boty)+7)))
		{  if(((nxtbal.leftx+4)>=four.leftx)&&((nxtbal.leftx+4)<=(four.leftx+15)))
			{
			  if((nxtbal.boty>=four.boty)&&(nxtbal.boty<=(four.boty+7))){
			  f2=1;
				four.health-=1;
				ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
			  wbal.velx= -(wbal.velx);
					score+=10;
		//		wbal.boty = wbal.boty + wbal.vely;
		//		wbal.leftx += wbal.velx;
			}
			}
	  }
			else if(nxtbal.boty>=four.boty&&nxtbal.boty<=(four.boty+7))
			{
			  if((nxtbal.boty+4)>=four.boty &&(nxtbal.boty+4)<=(four.boty+7 ))
				{
					if((nxtbal.leftx>=four.leftx) && (nxtbal.leftx <=(four.leftx+16)))
					{
					f2=0;
				 four.health-=1;
						score+=10;
				  ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
			    wbal.velx= -(wbal.velx);
			//		wbal.boty = wbal.boty + wbal.vely;
			//	  wbal.leftx += wbal.velx;
					
					}
				
				
				}
			
			}
	
	}

	
	
}





void Checkfivehor(void){
	
	if(five.health >0)
	{   //o and 02 are zero
		
		if(((nxtbal.boty+4)>=five.boty)&&(((nxtbal.boty)+4)<=((five.boty)+7)))
		{  if(((nxtbal.leftx+4)>=five.leftx)&&((nxtbal.leftx+4)<=(five.leftx+15)))
			{
			  if((nxtbal.boty>=five.boty)&&(nxtbal.boty<=(five.boty+7))){
			  fi2=0;
				five.health-=1;
				score+=10;
				ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
			  wbal.velx= -(wbal.velx);
			//	wbal.boty = wbal.boty + wbal.vely;
			//	wbal.leftx += wbal.velx;
				
			}
			}
	  }
			else if(nxtbal.boty>=five.boty&&nxtbal.boty<=(five.boty+7))
			{
			  if((nxtbal.boty+4)>=five.boty &&(nxtbal.boty+4)<=(five.boty+7 ))
				{
					if((nxtbal.leftx>=five.leftx) && (nxtbal.leftx <=(five.leftx+15)))
					{
					fi2=0;
				 five.health-=1;
					score+=10;
				  ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
			    wbal.velx= -(wbal.velx);
				//	wbal.boty = wbal.boty + wbal.vely;
				 // wbal.leftx += wbal.velx;
					
					}
				
				
				}
			
			}
	
	}

	
	
}


void Checkone(void)
{
	if (one.health>0)
	{
	    if((nxtbal.leftx>=one.leftx)&&(nxtbal.leftx<=(one.leftx+15)))
			{
			   if((nxtbal.boty>=one.boty)&&(nxtbal.boty<=one.boty+7))
				 {  
					 one.health-=1;
					 if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
					 score+=10;
					 
					 wbal.vely= -(wbal.vely);
				//	wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			}
	
	}
			
	   else if(((nxtbal.leftx+3)>=one.leftx) && ((nxtbal.leftx+3)<=one.leftx+15))
			{   if((nxtbal.boty>=one.boty )&& (nxtbal.boty<=(one.boty+7)))
				{
						one.health-=1;
					 if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
}
			}
			
			else if((nxtbal.leftx>=one.leftx) && (nxtbal.leftx<=( one.leftx+15)))
			{    if(((nxtbal.boty+3)>=one.boty) &&(( nxtbal.boty+3)<=one.boty+7)){
			
			      one.health-=1;
					 if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
				//	wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			  }
			
			}
			
			else if ((nxtbal.leftx+3>=one.leftx) && ((nxtbal.leftx+3)<=(one.leftx+15)))
			{
				if((nxtbal.boty+3)>=one.boty && (nxtbal.boty+3)<=(one.boty+7))
				{
			    one.health-=1;
					 if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			}
			
		}
	}
}
void Checktwo(void)
{
	if (two.health>0)
	{
	    if((nxtbal.leftx>=two.leftx)&&(nxtbal.leftx<=(two.leftx+15)))
			{
			   if((nxtbal.boty>=two.boty)&&(nxtbal.boty<=two.boty+7))
				 {  
					 two.health-=1;
					 ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
	
	}
			
	   else if(((nxtbal.leftx+3)>=two.leftx) && ((nxtbal.leftx+3)<=two.leftx+15))
			{   if((nxtbal.boty>=two.boty )&& (nxtbal.boty<=(two.boty+7)))
				{   score+=10;
						two.health-=1;
					 ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
					 
					 wbal.vely= -(wbal.vely);
		//			wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
}
			}
			
			else if((nxtbal.leftx>=two.leftx) && (nxtbal.leftx<=( two.leftx+15)))
			{    if(((nxtbal.boty+3)>=two.boty) &&(( nxtbal.boty+3)<=two.boty+7)){
			
			      two.health-=1;
					 ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
					 score +=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			  }
			
			}
			
			else if ((nxtbal.leftx+3>=two.leftx) && ((nxtbal.leftx+3)<=(two.leftx+15)))
			{
				if((nxtbal.boty+3)>=two.boty && (nxtbal.boty+3)<=(two.boty+7))
				{ score+=10;
			    two.health-=1;
					 ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);
					 
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
			
		}
	}
}
void Checkthree(void)
{
	if (three.health>0)
	{
	    if((nxtbal.leftx>=three.leftx)&&(nxtbal.leftx<=(three.leftx+15)))
			{
			   if((nxtbal.boty>=three.boty)&&(nxtbal.boty<=three.boty+7))
				 {  
					 score+=10;
					 three.health-=1;
					if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
					 
					 wbal.vely= -(wbal.vely);
		//			wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
	
	}
			
	   else if(((nxtbal.leftx+3)>=three.leftx) && ((nxtbal.leftx+3)<=three.leftx+15))
			{   if((nxtbal.boty>=three.boty )&& (nxtbal.boty<=(three.boty+7)))
				{
						three.health-=1;
					 if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
				//	wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
}
			}
			
			else if((nxtbal.leftx>=three.leftx) && (nxtbal.leftx<=( three.leftx+15)))
			{    if(((nxtbal.boty+3)>=three.boty) &&(( nxtbal.boty+3)<=three.boty+7)){
			
			      three.health-=1;
					if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			  }
			
			}
			
			else if ((nxtbal.leftx+3>=three.leftx) && ((nxtbal.leftx+3)<=(three.leftx+15)))
			{
				if((nxtbal.boty+3)>=three.boty && (nxtbal.boty+3)<=(three.boty+7))
				{
			    three.health-=1;
					 if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
			
		}
	}
}
void Checkfour(void)
{
	if (four.health>0)
	{
	    if((nxtbal.leftx>=four.leftx)&&(nxtbal.leftx<=(four.leftx+15)))
			{
			   if((nxtbal.boty>=four.boty)&&(nxtbal.boty<=four.boty+7))
				 {  
					 four.health-=1;
					 ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
	//				wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
	
	}
			
	   else if(((nxtbal.leftx+3)>=four.leftx) && ((nxtbal.leftx+3)<=four.leftx+15))
			{   if((nxtbal.boty>=four.boty )&& (nxtbal.boty<=(four.boty+7)))
				{
						four.health-=1;
					 ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
}
			}
			
			else if((nxtbal.leftx>=four.leftx) && (nxtbal.leftx<=( four.leftx+15)))
			{    if(((nxtbal.boty+3)>=four.boty) &&(( nxtbal.boty+3)<=four.boty+7)){
			
			      four.health-=1;
					 ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			  }
			
			}
			
			else if ((nxtbal.leftx+3>=four.leftx) && ((nxtbal.leftx+3)<=(four.leftx+15)))
			{
				if((nxtbal.boty+3)>=four.boty && (nxtbal.boty+3)<=(four.boty+7))
				{
			    four.health-=1;
					 ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			}
			
		}
	}
}

void Checkfive(void)
{
	if (five.health>0)
	{
	    if((nxtbal.leftx>=five.leftx)&&(nxtbal.leftx<=(five.leftx+15)))
			{
			   if((nxtbal.boty>=five.boty)&&(nxtbal.boty<=five.boty+7))
				 {  
					 five.health-=1;
					 ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
			//	wbal.boty +=  wbal.vely;
	//			wbal.leftx += wbal.velx;
			
			}
	
	}
			
	   else if(((nxtbal.leftx+3)>=five.leftx) && ((nxtbal.leftx+3)<=five.leftx+15))
			{   if((nxtbal.boty>=five.boty )&& (nxtbal.boty<=(five.boty+7)))
				{
						five.health-=1;
					score+=10;
					 ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
					 
					 wbal.vely= -(wbal.vely);
				//	wbal.boty +=  wbal.vely;
				//wbal.leftx += wbal.velx;
}
			}
			
			else if((nxtbal.leftx>=five.leftx) && (nxtbal.leftx<=( five.leftx+15)))
			{    if(((nxtbal.boty+3)>=five.boty) &&(( nxtbal.boty+3)<=five.boty+7)){
			        score+=10;
			      five.health-=1;
					 ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
					 
					 wbal.vely= -(wbal.vely);
			//		wbal.boty +=  wbal.vely;
			//	wbal.leftx += wbal.velx;
			
			  }
			
			}
			
			else if ((nxtbal.leftx+3>=five.leftx) && ((nxtbal.leftx+3)<=(five.leftx+15)))
			{
				if((nxtbal.boty+3)>=five.boty && (nxtbal.boty+3)<=(five.boty+7))
				{
			    five.health-=1;
					 ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);
					 score+=10;
					 wbal.vely= -(wbal.vely);
	//				wbal.boty +=  wbal.vely;
		//		wbal.leftx += wbal.velx;
			
			}
			
		}
	}
}
	
	

	
int laserCollide1(void){
	if(one.health > 0){
	if( ((lazr.xbotlft) <= (one.leftx + 15)) && ((lazr.xbotlft+3) >= one.leftx)){ //lazr inside x bounds of brick
		if( (lazr.ybotlft >= one.boty) && ((lazr.ybotlft) <= (one.boty + 4))){
			//one.health -= 1;
			if(one.health > 0){
				one.health-=1;
				lazr.vely = 0;
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft-4), blaser, 3,19);
				score+=10;
			}
					 if(one.health==0){
				ST7735_DrawBitmap(one.leftx,one.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}

				laseron =0;
			return 1;
		}
	}
}
	return 0;
}
	
int laserCollide2(void){
	if(two.health > 0){
	if( ((lazr.xbotlft) <= (two.leftx + 15)) && ((lazr.xbotlft+3) >= two.leftx)){ //lazr inside x bounds of brick
		if( (lazr.ybotlft >= two.boty) && ((lazr.ybotlft) <= (two.boty + 4))){
			//one.health -= 1;
			if(two.health > 0){
				two.health-=1;
				lazr.vely = 0;
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft-4), blaser, 3,19);
				score+=10;
			}
					 if(two.health==0){
				ST7735_DrawBitmap(two.leftx,two.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(two.leftx,two.boty, yellowbrick, 16,8);}

				laseron =0;
			return 1;
		}
	}
}
	return 0;
}
	
int laserCollide3(void){
	if(three.health > 0){
	if( ((lazr.xbotlft) <= (three.leftx + 15)) && ((lazr.xbotlft+3) >= three.leftx)){ //lazr inside x bounds of brick
		if( (lazr.ybotlft >= three.boty) && ((lazr.ybotlft) <= (three.boty + 4))){
			//three.health -= 1;
			if(three.health > 0){
				three.health-=1;
				lazr.vely = 0;
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft-4), blaser, 3,19);
				score+=10;
			}
					 if(three.health==0){
				ST7735_DrawBitmap(three.leftx,three.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(three.leftx,three.boty, yellowbrick, 16,8);}

				laseron =0;
			return 1;
		}
	}
}
	return 0;
}
	
int laserCollide4(void){
	if(four.health > 0){
	if( ((lazr.xbotlft) <= (four.leftx + 15)) && ((lazr.xbotlft+3) >= four.leftx)){ //lazr inside x bounds of brick
		if( (lazr.ybotlft >= four.boty) && ((lazr.ybotlft) <= (four.boty + 4))){
			//four.health -= 1;
			if(four.health > 0){
				four.health-=1;
				lazr.vely = 0;
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft-4), blaser, 3,19);
				score+=10;
			}
					 if(four.health==0){
				ST7735_DrawBitmap(four.leftx,four.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(four.leftx,four.boty, yellowbrick, 16,8);}

				laseron =0;
			return 1;
		}
	}
}
	return 0;
}
	
int laserCollide5(void){
	if(five.health > 0){
	if( ((lazr.xbotlft) <= (five.leftx + 15)) && ((lazr.xbotlft+3) >= five.leftx)){ //lazr inside x bounds of brick
		if( (lazr.ybotlft >= five.boty) && ((lazr.ybotlft) <= (five.boty + 4))){
			//five.health -= 1;
			if(five.health > 0){
				five.health-=1;
				lazr.vely = 0;
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft-4), blaser, 3,19);
				score+=10;
			}
					 if(five.health==0){
				ST7735_DrawBitmap(five.leftx,five.boty, blackbrick, 16,8);}
				else
				{ST7735_DrawBitmap(five.leftx,five.boty, yellowbrick, 16,8);}

				laseron =0;
			return 1;
		}
	}
}
	return 0;
}
	
void GoingBelowPaddle(void){
		if(nxtbal.boty > 148){ //sdayfgaied change this
			PrintPaddle();
			int status =Check_Paddle();
			lowerb = status;
		
			}		
	
	}




void Switch_Init(void){

SYSCTL_RCGCGPIO_R |=0x02;
int jack =0;
jack+=1;
GPIO_PORTB_DIR_R |= 0x07;
GPIO_PORTB_DEN_R |=0X07;
GPIO_PORTB_AFSEL_R &= ~0x07;
GPIO_PORTB_AMSEL_R &= ~0x07;
	


}
	
	
	
	
	// *************************** Capture image dimensions out of BMP**********


void Button_Init(void){
  DisableInterrupts();
  // activate port E
  SYSCTL_RCGCGPIO_R |= 0x10;  // enable port E
//  GPIO_PORTC_DIR_R &= ~0x10;  // make PC4 in (PC4 button) (default setting)
//  GPIO_PORTC_AFSEL_R &= ~0x10;// disable alt funct on PC4 (default setting)
//  GPIO_PORTC_DEN_R |= 0x10;   // enable digital I/O on PC4
//  GPIO_PORTC_PCTL_R &= ~0x000F0000;// configure PC4 as GPIO (default setting)
//  GPIO_PORTC_AMSEL_R &= ~0x10;// disable analog functionality on PC4 (default setting)
//  GPIO_PORTC_IS_R &= ~0x10;   // PC4 is edge-sensitive (default setting)
//  GPIO_PORTC_IBE_R &= ~0x10;  // PC4 is not both edges (default setting)
//  GPIO_PORTC_IEV_R |= 0x10;   // PC4 rising edge event
//  GPIO_PORTC_ICR_R = 0x10;    // clear flag4
//  GPIO_PORTC_IM_R |= 0x10;    // enable interrupt on PC4
//                              // GPIO PortC=priority 2
//  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00400000; // bits 21-23
  GPIO_PORTE_DIR_R &= ~0x10;  // make PE4 in (PE4 button) (default setting)
  GPIO_PORTE_AFSEL_R &= ~0x10;// disable alt funct on PE4 (default setting)
  GPIO_PORTE_DEN_R |= 0x10;   // enable digital I/O on PE4
                              // configure PE4 as GPIO (default setting)
  GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFF0FFFF)+0x00000000;
  GPIO_PORTE_AMSEL_R &= ~0x10;// disable analog functionality on PE4 (default setting)
  GPIO_PORTE_IS_R &= ~0x10;   // PE4 is edge-sensitive (default setting)
  GPIO_PORTE_IBE_R &= ~0x10;  // PE4 is not both edges (default setting)
  GPIO_PORTE_IEV_R |= 0x10;   // PE4 rising edge event
  GPIO_PORTE_ICR_R = 0x10;    // clear flag4
  GPIO_PORTE_IM_R |= 0x10;    // enable interrupt on PE4
                              // GPIO PortE=priority 2
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000040; // bits 5-7
                              // enable interrupts 2 and 4 in NVIC
  NVIC_EN0_R = (NVIC_EN0_INT4);
  EnableInterrupts();
}


void LaserFire(void){
	//laserdelay--;
	if(fireEnable){
		fireEnable = 0;
		laserdelay = 30;
	//	laserdelay = 2000;
	if(missilecount >=1){
		missilecount --;
		laseron = 1;
		lazr.xbotlft = (pad.leftx + 7);
		lazr.ybotlft = 145;
		lazr.vely = 4;
	}
}
}

void GPIOPortE_Handler(void){
	GPIO_PORTE_ICR_R = 0x10;    // acknowledge flag4
	//if(resetTime){ return;};
  edgeTrigger = 0;
	if(lazrEnable){
		LaserFire();
	}
}

void Delay100ms(uint32_t count){ 
	
	uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void TitleScreen(void){
  //ST7735_DrawBitmap(50, 70, asw, 30,30);
  ST7735_DrawBitmap(100, 120, bluebrick, 16,8);
	ST7735_DrawBitmap(70, 24, bluebrick, 16,8);
  ST7735_DrawBitmap(23, 12, bluebrick, 16,8);
	ST7735_DrawBitmap(45, 80, bluebrick, 16,8);
	ST7735_DrawBitmap(10, 135, bluebrick, 16,8);
	ST7735_DrawBitmap(1,40, greenbrick, 16,8);
	ST7735_DrawBitmap(110,35, greenbrick, 16,8);
	ST7735_DrawBitmap(5,60, greenbrick, 16,8);
	ST7735_DrawBitmap(90,70, greenbrick, 16,8);
	ST7735_DrawBitmap(73, 140, greenbrick, 16,8);
	ST7735_DrawBitmap(90, 7, yellowbrick, 16,8);
	ST7735_DrawBitmap(60, 14, yellowbrick, 16,8);
	ST7735_DrawBitmap(15, 150, yellowbrick, 16,8);
	ST7735_DrawBitmap(48, 111, yellowbrick, 16,8);
//	ST7735_SetCursor(3, 10);
//  ST7735_OutString("Brick Breaker");
	ST7735_SetCursor(4, 3);
  ST7735_OutString("=============");
	ST7735_SetCursor(4, 4);
  ST7735_OutString("Brick Breaker");
	ST7735_SetCursor(4, 5);
  ST7735_OutString("=============");
	ST7735_SetCursor(6, 15);
  ST7735_OutString("Press to start");
//	ST7735_SetCursor(6, 13);
//  ST7735_OutString("--------------");
//	ST7735_SetCursor(4, 12);
//  ST7735_OutString("Brick Breaker");
	edgeTrigger = 1;
  while(edgeTrigger){
    //wait for interrupt
  }
	//Delay100ms(50);	//wait for a bit ---- replace with interrupt for button
	//edgeTrigger = 1;
	ST7735_FillScreen(0);
}	


int main(void){
	// INITIALIZE STUFF
 // TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	//start:
	PLL_Init();
	ADC_Init();
	goto first;
		winner: // YOU WON! Clear screen and go to title; 
			// GPIO_PORTE_IM_R |= ~0x10;    // disable interrupt on PE4
			ST7735_FillScreen(0);  
			ST7735_DrawBitmap(100, 120, rbrick, 16,8);
			ST7735_DrawBitmap(70, 24, bluebrick, 16,8);
			ST7735_DrawBitmap(23, 12, pbrick, 16,8);
			ST7735_DrawBitmap(45, 80, bluebrick, 16,8);
			ST7735_DrawBitmap(10, 135, bluebrick, 16,8);
			ST7735_DrawBitmap(1,40, pbrick, 16,8);
			ST7735_DrawBitmap(110,35, greenbrick, 16,8);
			ST7735_DrawBitmap(5,60, greenbrick, 16,8);
			ST7735_DrawBitmap(90,70, greenbrick, 16,8);
			ST7735_DrawBitmap(73, 140, obrick, 16,8);
			ST7735_DrawBitmap(90, 7, yellowbrick, 16,8);
			ST7735_DrawBitmap(60, 14, yellowbrick, 16,8);
			//ST7735_DrawBitmap(15, 150, yellowbrick, 16,8);
			ST7735_DrawBitmap(48, 111, yellowbrick, 16,8);
			lazrEnable = 0;
			ST7735_SetCursor(5, 4);
			ST7735_OutString("You Win!");
			ST7735_SetCursor(5, 5);
			ST7735_OutString("Score: ");
			LCD_OutDec(score);
			//edgeTrigger = 0;
			Delay100ms(50);
			NVIC_ST_CTRL_R =0;
			//EnableInterrupts();
			goto first;
  
	loser: // YOU WON! Clear screen and go to title; 
			// GPIO_PORTE_IM_R |= ~0x10;    // disable interrupt on PE4
			//DisableInterrupts();
			ST7735_FillScreen(0);  
			ST7735_DrawBitmap(100, 120, rbrick, 16,8);
			ST7735_DrawBitmap(70, 24, bluebrick, 16,8);
			ST7735_DrawBitmap(23, 12, pbrick, 16,8);
			ST7735_DrawBitmap(45, 80, bluebrick, 16,8);
			ST7735_DrawBitmap(10, 135, bluebrick, 16,8);
			ST7735_DrawBitmap(1,40, pbrick, 16,8);
			ST7735_DrawBitmap(110,35, greenbrick, 16,8);
			ST7735_DrawBitmap(5,60, greenbrick, 16,8);
			ST7735_DrawBitmap(90,70, greenbrick, 16,8);
			ST7735_DrawBitmap(73, 140, obrick, 16,8);
			ST7735_DrawBitmap(90, 7, yellowbrick, 16,8);
			ST7735_DrawBitmap(60, 14, yellowbrick, 16,8);
			//ST7735_DrawBitmap(15, 150, yellowbrick, 16,8);
			ST7735_DrawBitmap(48, 111, yellowbrick, 16,8);
			lazrEnable = 0;
			ST7735_SetCursor(5, 4);
			ST7735_OutString("You lost");
			ST7735_SetCursor(5, 5);
			ST7735_OutString("Score: ");
			LCD_OutDec(score);
//			ST7735_SetCursor(5, 6);
//			ST7735_OutString("Wow you're bad");
			//edgeTrigger = 0;
			Delay100ms(50);
			NVIC_ST_CTRL_R =0;
			//EnableInterrupts();
	
  //Random_Init(1);

	first:
	Switch_Init();
	GPIO_PORTB_DATA_R &=0X0;
	ST7735_InitR(INITR_REDTAB);
	//int level = ADC_Level();
 	Game_Init();
  ST7735_FillScreen(0);  
	Button_Init(); //enable interrupts of laser
	TitleScreen();
	SysTick_Init(2666666);
	
	//while(1){
  //ST7735_DrawBitmap(pad.leftx, pad.boty,paddlefinal,20 ,5);           // set screen to black
	
	
	
	//}
	while(1){ 
	reset=0; //iterations of the 
	missile =0;
	missilecount=2;
	higherreset =1;
	//firstgame();// dealing with first time gmae is turned on 
	int o=0;
	int t =0;
	int th=0;
	int  f=0;
	int fi=0;
	int o2=1;
	int t2 =1;
	int th2=1;
	int  f2=1;
	int fi2=1;

	if(lives >=0){
	
	if(lives==0){
	goto loser;
	}
	//SysTick_Init (2666666);
	while(higherreset){ // this exists to monitor betwen plays while waiting for systick to interupt otherwise systiock will not interrupt and reset will fall through 
		while(reset){  
		bbal.leftx =wbal.leftx;//updaqte bb all position 
		bbal.boty =wbal.boty;
		nxtbal.leftx=wbal.leftx; //update next ball psoition
		nxtbal.boty=wbal.boty;
		nxtbal.velx=wbal.velx;
		nxtbal.vely=wbal.vely;
		nxtbal.boty = nxtbal.boty + nxtbal.vely; // set next ball location and check for everything below
		nxtbal.leftx= nxtbal.leftx+nxtbal.velx;
		CheckforXbounds();// sets vertb depending
		CheckforYbounds();// sets horb depending on if it goes and touches top
		Checkonehor();//
		Checktwohor();
		Checkthreehor();
		Checkfourhor(); 
		Checkfivehor();
			if(o2){
			  Checkone();
			}
			if(t2){
			 Checktwo();
			}
			if(th2){
			Checkthree();
			}
			if(f2){
			Checkfour();
			}
			if(fi2){
			Checkfive();
			}
			if((one.health + two.health + three.health + four.health + five.health) == 0){
				goto winner; //start;
			}
				
  GoingBelowPaddle();// sets lowerb depedning if it goes below bounds in if statements below if this is true it 
			//                        checks for paddle save if no save 		sets higher reset to zero	
	if(vertb)
			{
				wbal.velx = (-wbal.velx);
				wbal.leftx = wbal.leftx +wbal.velx;
				vertb = 0;
				
				}
			if(horb){
				wbal.vely =-wbal.vely;
				wbal.boty = 2;//wbal.leftx +wbal.velx;
				horb = 0;
			}
			if(lowerb){
					lowerb = 0;
					lives--;
				  wbal.leftx= 50;
				  wbal.boty = 50;
				  wbal.velx= 0;
				
					// set ball to middle----- this is too early
//					wbal.leftx = 61;
//					wbal.boty = 145;//was 145
//					wbal.velx = 0;
//					wbal.vely = -4;
//					bbal.leftx =57;
//					bbal.boty =145; //was 145

//					Delay100ms(10); // one second delay
			}
			//time to print crap
			wbal.leftx+=wbal.velx;
			wbal.boty+=wbal.vely;
			lazrEnable = 1;
			
			if(laseron == 1){
				//laserdelay--;
				ST7735_DrawBitmap((lazr.xbotlft),lazr.ybotlft, laser, 3,19);
				ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft+4), blaser, 3,19);
				lazr.ybotlft -= lazr.vely;
				if(laserCollide1() || laserCollide2() || laserCollide3() || laserCollide4() || laserCollide5()){
					laseron = 0;
					//ST7735_DrawBitmap((lazr.xbotlft),(lazr.ybotlft), blaser, 3,19);
					//lazr.vely = 0;
					//blaser.ybotlft = 0;
				}

			}
			//ST7735_DrawBitmap(pad.leftx, pad.boty,paddlefinal,20 ,5); // player ship middle bottom
			ST7735_DrawBitmap(bbal.leftx,bbal.boty,blackball, 4,4);
			ST7735_DrawBitmap(wbal.leftx,wbal.boty,whiteball, 4,4);
			if(one.health==2){
			ST7735_DrawBitmap(one.leftx,one.boty, pbrick, 16,8);}
			if(one.health==1){
			ST7735_DrawBitmap(one.leftx,one.boty, yellowbrick, 16,8);}
			if(two.health==1){
			ST7735_DrawBitmap(two.leftx,two.boty, bluebrick, 16,8);}
			if(three.health==2){
			ST7735_DrawBitmap(three.leftx,three.boty, rbrick, 16,8);}
			if(three.health==1){
			ST7735_DrawBitmap(three.leftx,three.boty, obrick, 16,8);}
			if(four.health==1){
			ST7735_DrawBitmap(four.leftx,four.boty, greenbrick, 16,8);}
			if(five.health==1){
			ST7735_DrawBitmap(five.leftx,five.boty,yellowbrick, 16,8);}
			//Print SCore and lasers
				ST7735_SetCursor(0,0);
			ST7735_OutString("SCORE:");
			LCD_OutDec(score);
			ST7735_OutString(" LASERS: ");
			ST7735_SetCursor(120, 156);
			LCD_OutDec(missilecount);
			ST7735_OutString("  ");
			int o2=1;
			int t2 =1;
			int th2=1;
			int  f2=1;
			int fi2=1;
			switch(lives){
				case 1: GPIO_PORTB_DATA_R &=0x01;
								break;
				case 2: GPIO_PORTB_DATA_R &=0x03;
								break;
				case 3: GPIO_PORTB_DATA_R |=0X07;
								break;
				default: GPIO_PORTB_DATA_R &=0X0;
			
			
					}
			
			reset =0; // Done at end of loop so that reinitialzwe for next iteration 
}
	}

}
}
}
	
	void SysTick_Init(uint32_t period){
			NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2          
  NVIC_ST_CTRL_R = 0x07; // enable SysTick with core clock and interrupts
  // enable interrupts after all initialization is finished
	return; 
	
			}
	void SysTick_Handler(void){
		PrintPaddle();
		if(laserdelay > 0){
			laserdelay--; //delay for laser firing speed to 1/sec
		}
		if(laserdelay == 0){
			fireEnable = 1;
		}
		reset =1;

	}



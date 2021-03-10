//  SysTick_Wait10ms() changed back to 10ms, not 0.1 ms
//  This program initiates clockwise and counterclockwise spins of the stepper motor, depending on
//  values specified

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"
// uint32_t timedelay = 210000; // global variable used to identify minimum time delay

void PortM_Init(void){
	//Use PortM pins for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};	// allow time for clock to stabilize
	GPIO_PORTM_DIR_R |= 0xFF;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTM_AFSEL_R &= ~0xFF;     								// disable alt funct on PN0
  GPIO_PORTM_DEN_R |= 0xFF;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTM_PCTL_R = (GPIO_PORTM_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTM_AMSEL_R &= ~0xFF;     								// disable analog functionality on PN0		
	return;
}

void spin_CCW(int totalCCWSpins){ // sequence of combinations for counter-clockwise rotation
	for(int i=0; i<totalCCWSpins; i++){ // totalCCWSpins has to be a multiple of 512 for complete spins
		GPIO_PORTM_DATA_R = 0b00001100;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b0000110;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b00000011;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b00001001;
		SysTick_Wait10ms(1);
	}
}

void spin_CW(int totalCWSpins){ // sequence of combinations for clockwise rotation
	for(int i=0; i<totalCWSpins; i++){ // totalCWSpins has to be a multiple of 512 for complete spins
		GPIO_PORTM_DATA_R = 0b00001100;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b0001001;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b00000011;
		SysTick_Wait10ms(1);
		GPIO_PORTM_DATA_R = 0b00000110;
		SysTick_Wait10ms(1);
	}
}
// 
void Rotation_Control(int direction, int numSpins){ // direction = 1 => CW, 2 = CCW
	int numtotal = 512 * numSpins; 
	if(direction == 1){ // Clockwise
		spin_CW(numtotal);
	}else if(direction == 2){ // counterclockwise
		spin_CCW(numtotal);
	}
}


int main(void){
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortM_Init();	
	
	
	Rotation_Control(1, 1); // 1 clockwise rotation
	SysTick_Wait10ms(100); // wait 1 second
	Rotation_Control(2, 1); // 1 counterclockwise rotation
	
	return 0;
}

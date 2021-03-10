
// this code flashes onboard LED D1 for a period of 0.5s at a duty cycle of 50%
#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

void PortN_Init(void){
	//Use PortN onboard LED1
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x05;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTN_AFSEL_R &= ~0x05;     								// disable alt funct on PN0
  GPIO_PORTN_DEN_R |= 0x05;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x05;     								// disable analog functionality on PN0		
	
	GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
	SysTick_Wait10ms(10);														
	GPIO_PORTN_DATA_R ^= 0b00000001;	
	return;
}

int main(void){
	
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortN_Init();																		// Initialize Port N 
	
	// Student number LSD = 6 => cycle period is 0.5s = 500ms
	// using halfdelay, and for 50% duty cycle, my LED will be ON for 250ms and OFF for 250 ms
	// therefore, value of halfdelay is 250 ms / 8.33 ns = 30,012
	
	uint16_t halfperiod = 25; // times 10 ms = 250 ms
	
	while(1){ // infinite loop
		GPIO_PORTN_DATA_R ^= 0b00000001; // toggle LED ON
		SysTick_Wait10ms(25); // wait half the period, or 250 ms
		GPIO_PORTN_DATA_R ^= 0b00000001; // toggle LED OFF
		SysTick_Wait10ms(25); // wait half the period, or 250 ms		
	}
}

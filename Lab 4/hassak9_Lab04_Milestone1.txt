//  this program cycles through duty cycles of an offboard RGB LED, from 10% to 100% and back to 10%
//  each duty cycle takes 10ms 

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x05;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTN_AFSEL_R &= ~0x05;     								// disable alt funct on PN0
  GPIO_PORTN_DEN_R |= 0x05;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x05;     								// disable analog functionality on PN0		
	
	GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
	SysTick_Wait10ms(10);														//.1s delay
	GPIO_PORTN_DATA_R ^= 0b00000001;	
	return;
}

// takes an integer input between 0, 255
// outputs square wave that has duty cycle approx = integer vaue / 255 * 100 duty cycle
void DutyCycle_Percent(uint8_t duty){ 
		float percent;	
		percent = ((float)duty*1000)/(255);
		int percent_int;
		percent_int = (int)percent;
		GPIO_PORTN_DATA_R ^= 0b00000100;
		SysTick_Wait10ms(percent_int);  //SysTick was changed to 0.01 ms in order for this to work
		GPIO_PORTN_DATA_R ^= 0b00000100;
		SysTick_Wait10ms(1000-percent_int);
}

void IntensitySteps(){
		uint32_t duty, 
						dutytimes;
		
	for(duty = 25; duty <= 250; duty += 25){ // increasing from 10% to 100% duty cycle
		for(dutytimes = 0; dutytimes < 10 ; dutytimes++){ // each duty cycle happens 10 times
			DutyCycle_Percent(duty);
		}
	}
	
	for(duty = 250; duty >= 25; duty -= 25){ // decreasing from 100% to 10% duty cycle
		for(dutytimes = 0; dutytimes < 10 ; dutytimes++){
			DutyCycle_Percent(duty);
		}
	}

}

int main(void){
	
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortN_Init();																		// Initialize Port N 
	
//	uint8_t duty = 51;
	while(1){
		IntensitySteps();
//		DutyCycle_Percent(duty);
	}
}

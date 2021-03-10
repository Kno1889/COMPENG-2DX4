// 2DX4 Lab 3: Analog Signals
// Code Description: Using Kiel's debugger mode to identify how analog input is stored in memory
// Using an AD2 module, voltages of 0V to 3.3V are input to the micro, and Kiel's debugger mode displays their values in memory registers.

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

volatile uint32_t ADCvalue;

//-ADC0_InSeq3-
// Busy-wait analog to digital conversion. 0 to 3.3V maps to 0 to 4095 
// Input: none 
// Output: 12-bit result of ADC conversion 
uint32_t ADC0_InSeq3(void){
	uint32_t result;
	
	ADC0_PSSI_R = 0x0008;														// 1) initiate SS3   
	while((ADC0_RIS_R&0x08)==0){}										// 2) wait for conversion done   
	result = ADC0_SSFIFO3_R&0xFFF;									// 3) read 12-bit result   
	ADC0_ISC_R = 0x0008;														// 4) acknowledge completion   
	
	return result; 
} 

void ADC_Init(void){
	//config the ADC from Valvano textbook. We're going to use Port E Pin 4
	// ADC_Init is exactly the same as from Studio 4 Session 1
	SYSCTL_RCGCGPIO_R |= 0b00010000;								// 1. activate clock for port E
	while ((SYSCTL_PRGPIO_R & 0b00010000) == 0) {}	//		wait for clock/port to be ready
	GPIO_PORTE_DIR_R &= ~0x10;											// 2) make PE4 input   
	GPIO_PORTE_AFSEL_R |= 0x10;											// 3) enable alternate function on PE4   
	GPIO_PORTE_DEN_R &= ~0x10;											// 4) disable digital I/O on PE4   
	GPIO_PORTE_AMSEL_R |= 0x10;											// 5) enable analog function on PE4   
	SYSCTL_RCGCADC_R |= 0x01;												// 6) activate ADC0   
	ADC0_PC_R = 0x01;																// 7) maximum speed is 125K samples/sec   
	ADC0_SSPRI_R = 0x0123;													// 8) Sequencer 3 is highest priority   
	ADC0_ACTSS_R &= ~0x0008;												// 9) disable sample sequencer 3   
	ADC0_EMUX_R &= ~0xF000;													// 10) seq3 is software trigger 
	ADC0_SSMUX3_R = 9;															// 11) set channel Ain9 (PE4)   
	ADC0_SSCTL3_R = 0x0006;													// 12) no TS0 D0, yes IE0 END0   
	ADC0_IM_R &= ~0x0008;														// 13) disable SS3 interrupts   
	ADC0_ACTSS_R |= 0x0008;													// 14) enable sample sequencer 3 
	//==============================================================================	
	
	return;
}

uint32_t func_debug[10]; 											// variable used to store realtime data 	
				 
int main(void){
  				
	uint32_t 	halfdelay = 5,												// this is a multiplier used for SysTick = half of delay 
						count = 0;														// my loop control variable initialized to 0	
	 
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	ADC_Init();																			// Initialize ADC as per Vavano text

	while(count < 10){															// run program for fixed number of steps/samples
		SysTick_Wait10ms(halfdelay);									// halfdelay*10ms
		func_debug[count++] = ADC0_InSeq3();					// Acquire sample
		SysTick_Wait10ms(halfdelay);									// halfdelay*10ms		
	}
}

// Lab 5: Peripheral Interfacing
// Decoding a Button Press
// This program illustrates detecting and decoding a single button press. Each button corresponds to a unique, hard-coded 8-bit code holding at most 2 0's, depending on the 
// row/column combination. This 8-bit code is stored in memory.

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"
int BinCode = 0;

void PortE0E1E2E3_Init(void){ // for rows. Row 0 is PE0, and so on
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // activate the clock for Port E
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4) == 0){}; // allow time for clock to stabilize
		GPIO_PORTE_DEN_R = 0b00001111; // Enable PE0:PE3 
	return;
}

void PortM0M1M2M3_Init(void){ // for columns. Column 0 is PM0, and so on
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11; //activate the clock for Port M
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){}; //allow time for clock to stabilize
		GPIO_PORTM_DIR_R = 0b00000000; // Make PM0:PM3 inputs, reading if the button is pressed or not
		GPIO_PORTM_DEN_R = 0b00001111; // Enable PM0:PM3
	return;
}

//Turns on D2 (PN0), D1 (PN1)
void PortN0N1_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12; //activate the clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};//allow time for clock to stabilize
		GPIO_PORTN_DIR_R=0b00000011; //Make PN0 and PN1 outputs, to turn on LED's
		GPIO_PORTN_DEN_R=0b00000011; //Enable PN0 and PN1
	return;
}

//Turns on D3 (PF4), D4 (PF0)
void PortF0F4_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; //activate the clock for Port F
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};//allow time for clock to stabilize
		GPIO_PORTF_DIR_R=0b00010001; //Make PF0 and PF4 outputs, to turn on LED's
		GPIO_PORTF_DEN_R=0b00010001;
	return;
}

//int 8bitRep(void){
//}

int main(void){
	PortE0E1E2E3_Init(); // calling port and pin initialization functions
	PortM0M1M2M3_Init();
	PortN0N1_Init();
	PortF0F4_Init();

	while(1){ 
// Row 0, PE0 --------------------------------------------------------------------------------
	GPIO_PORTE_DIR_R = 0b00000001; // To drive you use the data direction register
	GPIO_PORTE_DATA_R = 0b00000000;

	// Column 0 - Button 1 (PM0): Turn on LED 4
	while((GPIO_PORTM_DATA_R&0b00000001)==0){
		BinCode = 0b11101110;
	}
	
	// Column 1 - Button 2 (PM1): Turn on LED 3 
	while((GPIO_PORTM_DATA_R&0b00000010)==0){
		BinCode = 0b11011110;
	}
	
	// Column 2 - Button 3 {PM2): Turn on LEDs 3 and 4 
	while((GPIO_PORTM_DATA_R&0b00000100)==0){
		BinCode = 0b10111110;
	}

	// Column 3 - Button A (PM3): Turn on LEDs 1 and 3
	while((GPIO_PORTM_DATA_R&0b00001000)==0){
		BinCode = 0b01111110;
	}	

// Row 1, PE1 --------------------------------------------------------------------------------
	GPIO_PORTE_DIR_R = 0b00000010; // To drive you use the data direction register
	GPIO_PORTE_DATA_R = 0b00000000;
		
	// Column 0 - Button 4 (PM0): Turn on LED 2
	while((GPIO_PORTM_DATA_R&0b00000001)==0){
		BinCode = 0b11101101;
	}
	
	// Column 1 - Button 5 (PM1): Turn on LEDs 2 and 4
	while((GPIO_PORTM_DATA_R&0b00000010)==0){
		BinCode = 0b11011101;
	}	

	// Column 2 - Button 6 {PM2): Turn on LEDs 2 and 3
	while((GPIO_PORTM_DATA_R&0b00000100)==0){
		BinCode = 0b10111101;
	}	

	// Column 3 - Button B (PM3): Turn on LEDs 1, 3, 4
	while((GPIO_PORTM_DATA_R&0b00001000)==0){
		BinCode = 0b01111101;
	}	

	// Row 2, PE2 ----------------------------------------------------------------------------------
	GPIO_PORTE_DIR_R = 0b00000100; // To drive you use the data direction register
	GPIO_PORTE_DATA_R = 0b00000000;	
	
	// Column 0 - Button 7 (PM0): Turn on LEDs 2, 3, 4
	while((GPIO_PORTM_DATA_R&0b00000001)==0){
		BinCode = 0b11101011;
	}

	// Column 1 - Button 8 (PM1): Turn on LED 1
	while((GPIO_PORTM_DATA_R&0b00000010)==0){
		BinCode = 0b11011011;
	}

	// Column 2 - Button 9 {PM2): Turn on LEDs 1 and 4
	while((GPIO_PORTM_DATA_R&0b00000100)==0){
		BinCode = 0b10111011;
	}
		// Column 3 - Button C (PM3): Turn on LEDs 1 and 2 
	while((GPIO_PORTM_DATA_R&0b00001000)==0){
		BinCode = 0b01111011;
	}	
// Row 3, PE3 ----------------------------------------------------------------------------------
	GPIO_PORTE_DIR_R = 0b00001000; // To drive you use the data direction register
	GPIO_PORTE_DATA_R = 0b00000000;
	
		// Column 0 - Button * (PM0): Turn on LEDs 1, 2, 3
	while((GPIO_PORTM_DATA_R&0b00000001)==0){
		BinCode = 0b11100111;
	}

		// Column 1 - Button 0 (PM1): Turn on no LEDs
	while((GPIO_PORTM_DATA_R&0b00000010)==0){
		BinCode = 0b11010111;
	}
		// Column 2 - Button # {PM2): Turn on all LEDs
	while((GPIO_PORTM_DATA_R&0b00000100)==0){
		BinCode = 0b10110111;
	}
		// Column 3 - Button D (PM3): Turn on LEDs 1, 2, 4
	while((GPIO_PORTM_DATA_R&0b00001000)==0){
		BinCode = 0b01110111;
	}	
	BinCode = 0b00000000;
//-------------------------------------------------------------------------------------------------	
	
}

}


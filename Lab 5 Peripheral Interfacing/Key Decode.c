// Lab 5: Peripheral Interfacing
// Decoding a Button Press
// This program illustrates detecting nd decoding a single button press. The 8 bit code is now decoded to a 4-bit code and stored in memory, to represent using offboard LEDs // in the next part. 


#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"

void Bit4Rep(int eightCode);

int Bit8Code = 0b00000000;
int Bin4Code = 0b0000;


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

void Bit4Rep(int eightCode){
	
		if(eightCode == 0b11010111 || eightCode == 0b00000000){ // no input or 0
			Bin4Code = 0b0000;
		}
		if(eightCode == 0b11101110){ // 1 pressed
			Bin4Code = 0b0001;
		}
		if(eightCode == 0b11011110){ // 2 pressed
			Bin4Code = 0b0010;
		}
		if(eightCode == 0b10111110){ // 3 pressed
			Bin4Code = 0b0011;
		}
		if(eightCode == 0b11101101){ // 4 pressed
			Bin4Code = 0b0100;
		}
		if(eightCode == 0b11011101){ // 5 pressed
			Bin4Code = 0b0101;
		}
		if(eightCode == 0b10111101){ // 6 pressed
			Bin4Code = 0b0110;
		}
		if(eightCode == 0b11101011){ // 7 pressed
			Bin4Code = 0b0111;
		}
		if(eightCode == 0b11011011){ // 8 pressed 
			Bin4Code = 0b1000;
		}
		if(eightCode == 0b10111011){ // 9 pressed
			Bin4Code = 0b1001;
		}
		if(eightCode == 0b01111110){ // A pressed
			Bin4Code = 0b1010;
		}
		if(eightCode == 0b01111101){ // B pressed
			Bin4Code = 0b1011;
		}
		if(eightCode == 0b01111011){ // C pressed
			Bin4Code = 0b1100;
		}
		if(eightCode == 0b01110111){ // D pressed
			Bin4Code = 0b1101;
		}
		if(eightCode == 0b11100111){ // * pressed
			Bin4Code = 0b1110;
		}
		if(eightCode == 0b10110111){ // # pressed
			Bin4Code = 0b1111;
		}

}

int main(void){
	PortE0E1E2E3_Init(); // calling port and pin initialization functions
	PortM0M1M2M3_Init();
	PortN0N1_Init();
	PortF0F4_Init();
	
	while(1){
		Bit8Code = 0b00000000; // reset
	
	// Row 0, PE0 --------------------------------------------------------------------------------
		GPIO_PORTE_DIR_R = 0b00000001; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;
	
		// Column 0 - Button 1 (PM0): Turn on LED 4
		while((GPIO_PORTM_DATA_R&0b00000001)==0){
			Bit8Code = 0b11101110;
			Bit4Rep(Bit8Code);
		}
		
		// Column 1 - Button 2 (PM1): Turn on LED 3 
		while((GPIO_PORTM_DATA_R&0b00000010)==0){
			Bit8Code = 0b11011110;
			Bit4Rep(Bit8Code);
		}
	
		// Column 2 - Button 3 {PM2): Turn on LEDs 3 and 4 
		while((GPIO_PORTM_DATA_R&0b00000100)==0){
			Bit8Code = 0b10111110;
			Bit4Rep(Bit8Code);
		}

		// Column 3 - Button A (PM3): Turn on LEDs 1 and 3
		while((GPIO_PORTM_DATA_R&0b00001000)==0){
			Bit8Code = 0b01111110;
			Bit4Rep(Bit8Code);
		}	

	// Row 1, PE1 --------------------------------------------------------------------------------
		GPIO_PORTE_DIR_R = 0b00000010; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;
		
		// Column 0 - Button 4 (PM0): Turn on LED 2
		while((GPIO_PORTM_DATA_R&0b00000001)==0){
			Bit8Code = 0b11101101;
			Bit4Rep(Bit8Code);
		}
	
		// Column 1 - Button 5 (PM1): Turn on LEDs 2 and 4
		while((GPIO_PORTM_DATA_R&0b00000010)==0){
			Bit8Code = 0b11011101;
			Bit4Rep(Bit8Code);
		}	

		// Column 2 - Button 6 {PM2): Turn on LEDs 2 and 3
		while((GPIO_PORTM_DATA_R&0b00000100)==0){
			Bit8Code = 0b10111101;
			Bit4Rep(Bit8Code);
		}	
	
		// Column 3 - Button B (PM3): Turn on LEDs 1, 3, 4
		while((GPIO_PORTM_DATA_R&0b00001000)==0){
			Bit8Code = 0b01111101;
			Bit4Rep(Bit8Code);
		}	

		// Row 2, PE2 ----------------------------------------------------------------------------------
		GPIO_PORTE_DIR_R = 0b00000100; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;	
	
		// Column 0 - Button 7 (PM0): Turn on LEDs 2, 3, 4
		while((GPIO_PORTM_DATA_R&0b00000001)==0){
			Bit8Code = 0b11101011;
			Bit4Rep(Bit8Code);
		}

		// Column 1 - Button 8 (PM1): Turn on LED 1
		while((GPIO_PORTM_DATA_R&0b00000010)==0){
			Bit8Code = 0b11011011;
			Bit4Rep(Bit8Code);
		}

		// Column 2 - Button 9 {PM2): Turn on LEDs 1 and 4
		while((GPIO_PORTM_DATA_R&0b00000100)==0){
			Bit8Code = 0b10111011;
			Bit4Rep(Bit8Code);
		}
		// Column 3 - Button C (PM3): Turn on LEDs 1 and 2 
		while((GPIO_PORTM_DATA_R&0b00001000)==0){
			Bit8Code = 0b01111011;
			Bit4Rep(Bit8Code);
		}	
	// Row 3, PE3 ----------------------------------------------------------------------------------
		GPIO_PORTE_DIR_R = 0b00001000; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;
		
		// Column 0 - Button * (PM0): Turn on LEDs 1, 2, 3
		while((GPIO_PORTM_DATA_R&0b00000001)==0){
			Bit8Code = 0b11100111;
			Bit4Rep(Bit8Code);
		}

		// Column 1 - Button 0 (PM1): Turn on no LEDs
		while((GPIO_PORTM_DATA_R&0b00000010)==0){
			Bit8Code = 0b11010111;
			Bit4Rep(Bit8Code);
		}
		// Column 2 - Button # {PM2): Turn on all LEDs
		while((GPIO_PORTM_DATA_R&0b00000100)==0){
			Bit8Code = 0b10110111;
			Bit4Rep(Bit8Code);
		}
		// Column 3 - Button D (PM3): Turn on LEDs 1, 2, 4
		while((GPIO_PORTM_DATA_R&0b00001000)==0){
			Bit8Code = 0b01110111;
			Bit4Rep(Bit8Code);
		}	
		
		Bin4Code = 0b0000;
	}	

}


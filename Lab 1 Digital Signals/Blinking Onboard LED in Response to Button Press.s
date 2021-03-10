;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Description of Code: This code allows User LED D3 To Turn On when a button is pressed, and Off when the button is released
; Last Modified: 03 February 2021
 
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS
; Specifying RCGCGPIO Register Location
SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
COUNTER 					 EQU 0xFFFFF
	
; Specifying relevant Port F Register Addresses 
GPIO_PORTF_DIR_R             EQU 0x4005D400  ;GPIO Port F Direction Register address 
GPIO_PORTF_DEN_R             EQU 0x4005D51C  ;GPIO Port F Digital Enable Register address
GPIO_PORTF_DATA_R            EQU 0x4005D3FC  ;GPIO Port F Data Register address

; Specifying relevant Port M Register Addresses
GPIO_PORTM_DIR_R             EQU 0x40063400  ;GPIO Port M Direction Register address 
GPIO_PORTM_DEN_R             EQU 0x4006351C  ;GPIO Port M Digital Enable Register address
GPIO_PORTM_DATA_R            EQU 0x400633FC  ;GPIO Port M Data Register address
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Do not alter this section

        AREA    |.text|, CODE, READONLY, ALIGN=2 ;code in flash ROM
        THUMB                                    ;specifies using Thumb instructions
        EXPORT Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Define Functions in your code here 
;The function Port F_Init to configures the GPIO Port F Pin 4 for Digital Output 
; RCGCGPIO Register Binary: 0010 0000 ~ 2^5 = 32 = 0x20
PortF_Init
    ; Studio 1 STEP 1
    LDR R1, =SYSCTL_RCGCGPIO_R          
    LDR R0, [R1]                        
    ORR R0,R0, #0x20                    ;Activating the RCGCGPIO Register for Port F 
    STR R0, [R1]                         
    NOP                                 ;Waiting for GPIO Port F to be enabled
	NOP                                 
  
   ; Studio 1 STEP 5
    LDR R1, =GPIO_PORTF_DIR_R           
    LDR R0, [R1]                        
    ORR R0,R0, #0x10                  	; Setting Pin  4 to a digital output
    STR R0, [R1]                        
    
	; Studio 1 STEP 7
    LDR R1, =GPIO_PORTF_DEN_R           
    LDR R0, [R1]                        
    ORR R0, R0, #0x10                   ; Enabling the pin to become digital output
    STR R0, [R1]	                    
    BX  LR                              ;return

; RCGCGPIO Register Binary: 0000 1000 0000 0000 ~ 2^11 = 2048 = 0x0800
PortM_Init 
    ;Studio 1 STEP 1 
    LDR R1, =SYSCTL_RCGCGPIO_R          
    LDR R0, [R1]                        
    ORR R0,R0, #0x800                   ; Activating the RCGCGPIO Register for Port M 
    STR R0, [R1]                        
    NOP                                 ;Waiting for GPIO Port M to be enabled
	NOP                                 
  
   ; Studio 1 STEP 5
    LDR R1, =GPIO_PORTM_DIR_R           
    LDR R0, [R1]                        
    AND R0,R0, #0x00                  	; Setting Pin 0 to a digital input
    STR R0, [R1]                         
    
	; Studio 1 STEP 7
    LDR R1, =GPIO_PORTM_DEN_R           
    LDR R0, [R1]                        
    ORR R0, R0, #0x01                   ; Enabling the pin to become digita output
    STR R0, [R1]	                    
	
	BX  LR 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
Start                             ; Your assembly code starts executing here
	BL  PortM_Init                ;The BL instruction is like a function call to initialize Port M
	BL  PortF_Init                ;The BL instruction is like a function call to initialize Port F  
loop LDR R0, =GPIO_PORTM_DATA_R  ;Save the address of Input Port M to R0         
	LDR R1, [R0]                ;Save the contents at Input Port M to R1
	;;;;;;;;;;;;;;;;;;;;;;;;;;; Fill this;;;;;;;;;;;;;;; 	 
	EOR R1, R1, #0x1             ;Flips the input bit value at R1 bit 0 using a bitwise XOR operation  
	LSL R1, R1, #0x4             ;Left shift R1 4 bits to the left because we need Pin 4 of this Port	 
	LDR R3, =GPIO_PORTF_DATA_R   		
		STR R1, [R3] 
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;       	 
		B loop         
	ALIGN         
	END

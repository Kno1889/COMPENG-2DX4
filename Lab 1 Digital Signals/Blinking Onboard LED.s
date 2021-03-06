;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Hello World in Assembly :)
; Description of Code: This code allows User LED D2 To Blink (Port N Pin 0) 
; Last Modified: February 03rd 2021
 
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS

;The EQU directive gives a symbolic name to a numeric constant, a register-relative value or a program-relative value

SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
GPIO_PORTN_DIR_R             EQU 0x40064400  ;GPIO Port N Direction Register address 
GPIO_PORTN_DEN_R             EQU 0x4006451C  ;GPIO Port N Digital Enable Register address
GPIO_PORTN_DATA_R            EQU 0x400643FC  ;GPIO Port N Data Register address
COUNTER 					 EQU 0xFFFFF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Do not alter this section

        AREA    |.text|, CODE, READONLY, ALIGN = 2 ;code in flash ROM
        THUMB                                    ;specifies using Thumb instructions
        EXPORT Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Define Functions in your code here 
;The function PortN_Init to configures the GPIO Port N Pin 0 for Digital Output 
; Function PortN_Init configures Port N Pin 0 to make it a digital output
; RCGCGPIO Register Binary: 0001 0000 0000 0000 ~ 2^12 = 4096 = 0x1000
PortN_Init
    ; Studio 1 STEP 1
    LDR R1, = SYSCTL_RCGCGPIO_R         
    LDR R0, [R1]                        
    ORR R0,R0, #0x1000                  ; Activating the RCGCGPIO Register for Port N 
    STR R0, [R1]                        
    NOP                                 ;Waiting for GPIO Port N to be enabled
	NOP                                 
  
   ; Studio 1 STEP 5
    LDR R1, = GPIO_PORTN_DIR_R          
    LDR R0, [R1]                        
    ORR R0,R0, #0x01                  	; Setting Pin 0 to digital output
    STR R0, [R1]                         
    
	; Studio 1 STEP 7
    LDR R1, =GPIO_PORTN_DEN_R           
    LDR R0, [R1]                        
    ORR R0, R0, #0x01                   ; Enabling the Pin to become a digital output pin 
    STR R0, [R1]	                    
    BX  LR                              ;return to Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Start
	BL  PortN_Init                      ;The BL instruction is like a function call 
    ; Studio 1 STEP 8                              		
    LDR R1, =GPIO_PORTN_DATA_R          
	LDR R0,[R1]                         ;Loading the address of the DATA register to R0, then settinging relevant bit to 1 to turn on LED
    ORR R0,R0, #0x01                    
loop  STR R0, [R1]                       
	EOR R0, R0, #0x01					; EOR was used to perform bitwise XOR instead of bitwise OR as incuded in template
		
		LDR R4, = COUNTER
		
loop1 NOP
	  NOP
	  SUB R4, R4, #0x01
	  CMP R4, #0x00
		  BNE loop1
		  
		  B loop
	
	  ALIGN                               ;Do not touch this 
      END   

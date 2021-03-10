;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Description of Code: D1 represent susccessful load and D2 represents unsuccessful load of parallel combination lock

; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS

;The EQU directive gives a symbolic name to a numeric constant, a register-relative value or a program-relative value

SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
GPIO_PORTN_DIR_R             EQU 0x40064400  ;GPIO Port N Direction Register address 
GPIO_PORTN_DEN_R             EQU 0x4006451C  ;GPIO Port N Digital Enable Register address
GPIO_PORTN_DATA_R            EQU 0x400643FC  ;GPIO Port N Data Register address
	
GPIO_PORTM_DIR_R             EQU 0x40063400  ;GPIO Port M Direction Register Address (Fill in these addresses)
GPIO_PORTM_DEN_R             EQU 0x4006351C  ;GPIO Port M Direction Register Address (Fill in these addresses)
GPIO_PORTM_DATA_R            EQU 0x400633FC  ;GPIO Port M Data Register Address      (Fill in these addresses) 

COMBINATION EQU 2_1001
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Do not alter this section

        AREA    |.text|, CODE, READONLY, ALIGN=2 ;code in flash ROM
        THUMB                                    ;specifies using Thumb instructions
        EXPORT Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Function PortN_Init. LED1 is PN1 and LED2 is PN0
PortN_Init 
    ;STEP 1
    LDR R1, =SYSCTL_RCGCGPIO_R 
    LDR R0, [R1]   
    ORR R0,R0, #0x1000        ; Activating Port N		          
    STR R0, [R1]               
    NOP 
    NOP   
   
    ;STEP 5
    LDR R1, =GPIO_PORTN_DIR_R   
    LDR R0, [R1] 
    ORR R0,R0, #0x3           ; Activating pins 0 and 1 as outputs
	STR R0, [R1]   
    
    ;STEP 7
    LDR R1, =GPIO_PORTN_DEN_R   
    LDR R0, [R1] 
    ORR R0, R0, #0x3          ; Enabling pins 0 and 1 as digital outputs
    STR R0, [R1]  
    BX  LR                            
 
; Function PortM_Init. used for buttons
PortM_Init 
    ;STEP 1 
	LDR R1, =SYSCTL_RCGCGPIO_R       
	LDR R0, [R1]   
    ORR R0,R0, #0x800 		   ; Activating Port M
	STR R0, [R1]   
    NOP 
    NOP   
 
    ;STEP 5
    LDR R1, =GPIO_PORTM_DIR_R   
    LDR R0, [R1] 
    ORR R0,R0, #0x00          ; Activating pins 0, 1, 2 and 3 as inputs
	STR R0, [R1]   
    
	;STEP 7
    LDR R1, =GPIO_PORTM_DEN_R   
    LDR R0, [R1] 
    ORR R0, R0, #0xF          ; Enabling pins 0, 1, 2 and 3 as digital inputs
	                          
    STR R0, [R1]    
	BX  LR                     


State_Init LDR R5,=GPIO_PORTN_DATA_R  ;Locked is the Initial State
	       MOV R4,#2_00000001		  ; so, LED2 (PN0) is ON initially
	       STR R4,[R5]
	       BX LR 

Start                             
	BL  PortN_Init                ; Calling functions to initialize ports and pins
	BL  PortM_Init
	BL  State_Init
	LDR R0, = GPIO_PORTM_DATA_R  ; Inputs set pointer to the input from Data register of Port M
	LDR R8, =COMBINATION         ;R8 stores our combination for comparison later
	
Loop; This loop executes all the time
			LDR R1,[R0]            ; Loading register R1 with the contents of Data Register, out current input combination 
			AND R2,R1,#2_00001111  ; Bit masking the first 4 bits, as these 4 are the only ones we're interested in comparing
			CMP R2,R8			   ; Comparison statement between combination and current input
			BEQ Unlocked_State     ; If combination is equal to the current input, we go to the unlocked state tag (later)
			BNE Locked_State	   ; If unequal, we go to the locked_state tag below
 
Locked_State ; In a locked state, LED2 (PN0) is ON                   
	LDR R5,=GPIO_PORTN_DATA_R
	MOV R4,#2_00000001			   ; Switching ON LED2
	STR R4,[R5]
	B Loop							; Going back to repeat the loop
	
Unlocked_State ; In an unlocked state, LED1 (PN1) is ON
	LDR R5, =GPIO_PORTN_DATA_R
	MOV R4,#2_00000010				; Switching on LED1
	STR R4, [R5]
	B Loop							; Going back to repeat the loop
	
	ALIGN   
    END  

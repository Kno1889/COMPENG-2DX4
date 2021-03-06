;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Description of Code: D1 represent susccessful load and D2 represents unsuccessful load of parallel sequential lock

; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS

;The EQU directive gives a symbolic name to a numeric constant, a register-relative value or a program-relative value

SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
GPIO_PORTN_DIR_R             EQU 0x40064400  ;GPIO Port N Direction Register address 
GPIO_PORTN_DEN_R             EQU 0x4006451C  ;GPIO Port N Digital Enable Register address
GPIO_PORTN_DATA_R            EQU 0x400643FC  ;GPIO Port N Data Register address
	
GPIO_PORTM_DIR_R             EQU 0x40063400  ;GPIO Port M Direction Register Address
GPIO_PORTM_DEN_R             EQU 0x4006351C  ;GPIO Port M Direction Register Address 
GPIO_PORTM_DATA_R            EQU 0x400633FC  ;GPIO Port M Data Register Address       

COUNT_REMAINING EQU 3
SEQUENCE EQU 2_0001 ; redundant variable, but I need it for my reference

; LED1(PN1) => successful, LED2 (PN0) => unsuccessful
; My position 0 bit is a 1, and positions 1, 2, 3 are 0s

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
    ORR R0,R0, #0x3         ; Activating pins 0 and 1 as outputs
	STR R0, [R1]   
    
    ;STEP 7
    LDR R1, =GPIO_PORTN_DEN_R   
    LDR R0, [R1] 
    ORR R0, R0, #0x3       ; Enabling pins 0 and 1 as digital outputs                           
    STR R0, [R1]  
    BX  LR                            

; Function PortM_Init. used for buttons
PortM_Init 
    ;STEP 1 
	LDR R1, =SYSCTL_RCGCGPIO_R       
	LDR R0, [R1]   
    ORR R0,R0, #0x800      ; Activating Port M
	STR R0, [R1]   
    NOP 
    NOP   
 
    ;STEP 5
    LDR R1, =GPIO_PORTM_DIR_R   
    LDR R0, [R1] 
    ORR R0,R0, #0x00      ; Activating pins 0 and 4 as inputs
	STR R0, [R1]   
    
	;STEP 7
    LDR R1, =GPIO_PORTM_DEN_R   
    LDR R0, [R1] 
    ORR R0, R0, #2_10001     ; Enabling pins 0 and 4 as digital inputs
	                          
    STR R0, [R1]    
	BX  LR     


State_Init LDR R5,=GPIO_PORTN_DATA_R  ;Locked is the Initial State
		   MOV R4,#2_00000001		  ; So, LED2 (PN0) is ON initially
	       STR R4,[R5]
	       BX LR 


Start                             
	BL  PortN_Init                ; Calling functions to initialize ports and pins
	BL  PortM_Init
	BL  State_Init
	LDR R0, = GPIO_PORTM_DATA_R      ; set pointer to the input data register

	LDR R7, =COUNT_REMAINING		; Loading R7 with the value of the counter variable 

; We need to check if PM4 changes from 0 to 1 before accepting input
IsZero		
			LDR R1,[R0]    
			AND R2,R1,#2_00010000   
			CMP R2, #2_00000000        
			BNE IsZero
IsOne		
			LDR R1,[R0]    
			AND R2,R1,#2_00010000   
			CMP R2, #2_00010000        
			BNE IsOne

; The previous 2 blocks check if PM4, the logic testing bit, changes from 0 to 1
; If that happens, we move on to the following blocks of code
; From here, we just need to check if the sequence of bit entered through PM0 is correct

CheckInputBit	
			AND R2,R1,#2_00000001    ; Bit masking the first bit, as this is the only one we're interested in comparing
			CMP R2,#2_00000001       ; Comparing the input with a 1
			BEQ BitValueOne        ; If the input is 1, we jump to the BitValueOne tag
			BNE BitValueZero        ; If the input is zero, we jump to the BitvalueZero tag
			
BitValueOne ; This function handles the cases/positions in which a 1 bit value is expected in the sequence
			CMP R7,#0             ; i am expecting a 1 in the 0th position/last entry of my sequence, the least significant bit
			BEQ CorrectEntry   ; If a 1 is entered in the last entry, we move to the correct entry tag
			BNE Locked_State       ; If the entry is incorrect, we go to the locked state and start checking again for the sequence
	
BitValueZero ; This function handles the cases/positions in which a 0 bit value is expected
			CMP R7,#3                ; If my zeros are in the correct position, i expect them at these
			BEQ CorrectEntry   		 ; values of my count remaining variable
			CMP R7, #2               ; these values will not trigger the unlocked_state tag, but will decrement
			BEQ CorrectEntry			; the count variable. 
			CMP R7, #1					
			;or position 1
			BEQ CorrectEntry	 		
			BNE Locked_State	     ; if i get a 0 in an incorrect position (pos 0), we go to the locked_state tag and the count is reset
			

CorrectEntry
			CMP R7, #0              ; If we have all 4 correct entries, the value of the count variable will be zero and we can move to the unlocked_state tag
			BEQ Unlocked_State       
			SUB R7,#1                ; if not, we have a correct entry, just not enough, so we subtract 1 from the count remaining variable and check the next entry
			B IsZero                  
 
Locked_State ; LED2 (PN0) is ON       
	LDR R7, = 3		; and the count variable is reset
	LDR R5,=GPIO_PORTN_DATA_R
	MOV R6,#2_00000001   
	STR R6,[R5]
	B IsZero		; go back to the beginning to check the next entry
	
Unlocked_State ; LED1 (PN1) is ON
	LDR R5, =GPIO_PORTN_DATA_R
	MOV R6,#2_00000010   
	STR R6, [R5]
	LDR R7, =3 	; and the count variable is reset

	B Start 	; and go back to the start to check the next 4 entries 
	
	ALIGN   
    END  

; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
Number EQU 0
	PRESERVE8
  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R4,LR}
		SUB SP, #4     ;Make room on stack to write value
		CMP R0, #10    ;Current digit < 10? (not in ones place, need to continue on)
		BLO lastdigit
		
		MOV R1, #10
		UDIV R4, R0, R1
		;Modulo for decimal conversion
		UDIV R2, R0, R1
		MUL R1, R2
		SUB R0, R1
		;Store values for later
		STR R0, [SP,#Number]   ;Cheating push to stack
		MOV R0, R4
		;Output value
		BL LCD_OutDec  ;Get next digit
		LDR R0, [SP,#Number]   ;
		
lastdigit 
		ADD R0, #0x30   ;Convert to ASCII Number
		BL ST7735_OutChar 
		ADD SP, #4     ;Fix the stackk
		POP {R4,PC}    ;Return LR to PC to return
		BX LR         ;also works without this

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
    PUSH {R4,LR}
	SUB SP, #4 ;Allcating stack
	MOV R1, #10000 ;isolating digit
	CMP R1, R0
	MOV R1,#1000
	STR R1,[SP,#Number]
	BLS SPECIAL ; handles specific case
	MOV R1, R0
	MOV R2, R0
	MOV R3, R0
	LDR R4, [SP,#Number]
	UDIV R1, R4
	ADD R0,R1, #0X30; ASCII vlaue in r0
	MUL R1,R1,R4
	MOV R4, #100
	STR R4, [SP,#Number]
	SUB R1, R2,R1
	MOV R6, R1
	PUSH{R0-R3}
	BL ST7735_OutChar ; printing first digit
    POP{R0-R3}
 
	MOV R0, #46   ;second digit fixed
	PUSH{R0-R3}
	BL ST7735_OutChar
	POP{R0-R3}
	
    LDR R4, [SP,#Number]
	UDIV R1, R4
	ADD R0,R1, #0X30 ; turning third digit to ASCII
	MUL R1,R1,R4
	MOV R4, #10
	STR R4, [SP,#Number]
	SUB R1, R6,R1
	MOV R6, R1
	PUSH{R0-R3}
	BL ST7735_OutChar
    POP{R0-R3} ; Remaining friendly 

    LDR R4,[SP,#Number]
	UDIV R1, R4
	ADD R0,R1, #0X30
	MUL R1,R1,R4
	SUB R1, R6,R1
	MOV R6, R1
	PUSH{R0-R3}
	BL ST7735_OutChar
    POP{R0-R3}



	ADD R0,R1, #0X30
	PUSH{R0-R3}
	BL ST7735_OutChar
    POP{R0-R3}
	ADD SP, #4 ; Re-allcating stack
    BX LR

SPECIAL 
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #46
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	MOV R0, #42
	BL ST7735_OutChar
	ADD SP, #4
	POP{R4,LR}
	
	BX LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file

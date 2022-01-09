.MODEL SMALL
.STACK 64
.DATA
    NUMBER_TEN  DB 10
    WHICH_DIGIT  DB 0
	SEC_INP_DIG  DB 0
	FIRST_INP_DIG  DB 0
	FINAL_NUMBER_N DB 0

	ROW0_KEYS DB 1, 2, 3
	ROW1_KEYS DB 4, 5, 6
	ROW2_KEYS DB 7, 8, 9
	ROW3_KEYS DB 0, 0, 0

	PORTA EQU 060H
	PORTB EQU 062H
	PORTC EQU 064H
	CON_REG EQU 066H

.CODE  

MAIN          PROC FAR
	MOV AX, @DATA
	MOV DS, AX
PART_1 :
    MOV DX,CON_REG
	MOV AL, 80H            ; SET THE CONTROL WORD = 1000 0000
	OUT DX, AL

BEGIN:
	MOV DX,PORTB
	;MOV BX, PORTC
    MOV AX,0000H ; Output 00 for 0V level
	

UP: 
    CALL OUTPUT
    INC AX ; To raise wave from 0V to 5V increment AL
    CMP AX,0000H

    JNZ UP ; Jump UP till rising edge is reached i.e. 5V
    
    MOV AX,00FFH
UP1: 
    CALL OUTPUT
    DEC AX ; To fall wave from 5V to 0V decrement AL
    CMP AX,00FFH
    JNZ UP1 ; Jump UP till falling edge is reached i.e. 0V
    JMP BEGIN

OUTPUT:
    OUT DX,AX
    CALL DELAY
    INT 21H

DELAY:
    MOV CX,07H ;To vary the frequency alter the delay count
    LUP1:LOOP LUP1
    INT 21H

	

    
MAIN ENDP
    END



	EXPORT SystemInit
    EXPORT __main

	

	AREA RESET,CODE,READONLY 

SystemInit FUNCTION
	
 ENDFUNC


__main FUNCTION
	
	LDR  R0, =MYDATA
	MOV  R3, #10
	MOV  R4, #100
	MOV  R5, #10
	
	
	
	BL POPULATION
	
OUT
	B OUT


POPULATION
	PUSH {R4,R5,LR} ;to be sure we dont loose r4 & r5
	MOV R6, #0
	MOV R7, #0 ;COUNTER
	
FOR
	STR R4, [R0,R6] ;storing r4
	ADD R6, #0X20	;to go next line and show number in beautiful format ^_^
	ADD R4, R4, R5	;add another 'b' to r4 (actualy i++)
	ADD R7, #1	;counetr to control loop
	CMP R7, R3
	
	BLT FOR

	POP {R4,R5,PC}

 ENDFUNC	
 
 	AREA DATA_1, DATA, READWRITE
MYDATA	

 
 END
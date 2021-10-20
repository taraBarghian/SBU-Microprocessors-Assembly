
	EXPORT SystemInit
    EXPORT __main

	

	AREA RESET,CODE,READONLY 

SystemInit FUNCTION
	
 ENDFUNC


__main FUNCTION
	
	MOV R0, #12
	LDR R1, =MYDATA
	LDRB R2, [R1]
	
	
	BL FINDMAX	
	
OUT
	B OUT


FINDMAX
	PUSH {LR}
FOR
	LDRB R3, [R1]
	CMP R2,R3
	
	BHS CONTINUE ;update max if current number is bigger than it
	MOV R2,R3
	
CONTINUE
	ADD  R1, #0x01
	SUBS R0,#1
	BNE FOR	
	POP {PC}

 ENDFUNC	
 
 	AREA DATA_1, DATA, READONLY
MYDATA	DCB 0X05, 0X12, 0X15, 0X08, 0XA2, 0XB1, 0X35, 0X4F , 0XF6, 0XDF, 0XFF, 0XDD

 
 END
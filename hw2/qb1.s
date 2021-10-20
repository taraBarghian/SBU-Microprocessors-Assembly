
	EXPORT SystemInit
    EXPORT __main

	

	AREA RESET,CODE,READONLY 

SystemInit FUNCTION
	
 ENDFUNC


__main FUNCTION
	
	MOV R0, #10
	LDR R2, =MYDATA
	LDRB R1, [R2]
	
	
	BL FINDMIN
	
OUT
	B OUT


FINDMIN
	PUSH {LR}
FOR
	LDRB R3, [R2]
	CMP R1,R3
	
	BLS CONTINUE;update min if current number is less than it
	MOV R1,R3
	
CONTINUE 
	ADD  R2, #0x01
	
	SUBS R0,#1
	BNE FOR	
	
	POP {PC} ;end of function

 ENDFUNC	
 
 	AREA DATA_1, DATA, READONLY
MYDATA	DCB 0XD5, 0XC5, 0XA5, 0X90, 0X25, 0X20, 0X55, 0X02 , 0X25, 0X20, 0X88

 
 END
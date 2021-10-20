
	EXPORT SystemInit
    EXPORT __main


	AREA RESET,CODE,READONLY 

SystemInit FUNCTION
 ENDFUNC



__main FUNCTION


	MOV R0, #11 ;counting
	LDR R1, =0x20000000 ;start from
	
	MOV R2, #0 ;R2=0
	
FOR1
	SUB R5, R0,R2
	MOV R6 , #0
FOR2

	LDRB R3, [R1, R6] ;R3 arr[j]
	ADD  R6, #1 ;j
	LDRB R4, [R1, R6] ;R4 arr[j+1]
	CMP  R3,  R4
	
	BGE OK ; sort them if they are not
	STRB R3, [R1, R6] ; put r3 in r4's position
	SUB  R6, #1
	STRB R4, [R1 ,R6] ; put r4 in r3's position
	ADD  R6, #1
OK	
	
	CMP R6, R5
	BLE FOR2
	
	ADD R2,#1
	CMP R2,R0
	BLE FOR1
		
	
	
HERE 
	B HERE

 ENDFUNC	
 END
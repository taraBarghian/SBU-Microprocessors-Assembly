
	EXPORT SystemInit
    EXPORT __main

	
 	AREA DATA_1, DATA, READONLY
ARRAY DCB 0X10, 0X20, 0X30, 0X01, 0X36, 0X40, 0X42, 0X50

 	AREA DATA_2, DATA, READWRITE
ARRAY_W  

	AREA RESET, CODE, READONLY 

SystemInit FUNCTION
	
 ENDFUNC

MIN   RN  R1
MAX   RN  R2
N     RN  R3



__main PROC
	  
	MOV R0, #8     ;ARRAY.len
	BL FINDMIN
	BL FINDMAX
	
	MOV N, #20
	MOV R4, #5 ;a = 50
	MOV R5, #10 ;b = 100
	BL POPULATE
	
	
ABORT
    B ABORT

 ENDP	
 
 ;function findmin()
FINDMIN PROC	
	
	PUSH {R4, R6, R7, R8, LR}
	
	LDR R7, =ARRAY ;R7 = ARRAY[i].address	
	LDRB MIN, [R7] 
	MOV R6, #0     ;i = 0
LOOP1
	CMP R6, R0     ;i ? ARRAY.len 
	BHS EXIT1       ;if (i >= ARRAY.len) goto EXIT
	;else
	LDRB R8, [R7]   ;R8 = ARRAY[i]
	CMP MIN, R8     ;min ? ARRAY[i]
	BLS CONTINUE1   ;if (min <= ARRAY[i]) goto CONTINUE
	;else
	MOV MIN, R8     ;min = ARRAY[i]
CONTINUE1
    ADD R6, R6, #1 ;i++
	ADD R7, R7, #1 ;=ARRAY.next.address
	B LOOP1
EXIT1
    ;epilog
	POP {R4, R6, R7, R8, PC}
	
 ENDP
 
 
 
 
 ;function findmax()
FINDMAX PROC
	
	PUSH {R4, R6, R7, R8, LR}
	
	LDR R7, =ARRAY ;R7 = ARRAY[i].address
	LDRB MAX, [R7]   
	MOV R6, #0     ;i = 0
LOOP2
	CMP R6, R0     ;i ? ARRAY.len 
	BHS EXIT2       ;if (i >= ARRAY.len) goto EXIT
	;else
	LDRB R8, [R7]   ;R8 = ARRAY[i]
	CMP MAX, R8     ;max ? ARRAY[i]
	BHS CONTINUE2   ;if (max => ARRAY[i]) goto CONTINUE
	;else
	MOV MAX, R8     ;max = ARRAY[i]
CONTINUE2
    ADD R6, R6, #1 ;i++
	ADD R7, R7, #1 ;=ARRAY.next.address
	B LOOP2
EXIT2
    
	POP {R4, R6, R7, R8, PC}
	
 ENDP
 
 

;function populate()
POPULATE PROC
	PUSH {R4, R6, R8, R9, LR}
	
	MOV R6, #0 ; i = 0
	LDR R8, =ARRAY_W
LOOP3
    CMP R6, N
	BHS EXIT3
	MUL R9, R6, R5 ;R9 = i * b
	ADD R9, R9, R4 ;R9 = R9 + a
	STR R9, [R8]
    ADD R8, R8, #4 ;go four bytes ahead
	ADD R6, R6, #1 ;i++
	B LOOP3
EXIT3
	
	
	POP {R4, R6, R8, R9, PC}
	
 ENDP
 

 

	
 END
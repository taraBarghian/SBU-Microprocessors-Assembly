
        .model small
        .386
        .DATA   
        org 100h
        MSG1 DB "ENTER YOUR NUMBER : $"
        MSG2 DB "GCD IS : $" 
        MSG3 DB "LCM IS : $" 
        NEWLINE DB 0AH,0DH,"$" 
        DATA1 DD 0
        DATA2 DD 0
		DATA3 DD 0
        DATA4 DD 0
        DATA5 DD 0
        BUFFER db 10,?, 10 dup(' ')  ;scanf("%s", buffer);

        .CODE
MAIN    PROC FAR
        mov ax, @DATA
        mov ds, ax
		
        mov al, 03h
        mov ah, 0
        int 10h ; setup text mode

        mov bx, offset DATA1
        call GETNUM
        

        mov bx, offset DATA2
        call GETNUM

		call GCD	
        ;;;;
        mov ah,09H
        lea dx, MSG2
        int 21H
		mov bx, offset DATA3
		mov eax, [bx]
		call PRINTNUM
        
        mov ah, 09
	    mov dx, offset NEWLINE
	    int 21h

        mov eax, DATA1
        sub edx, edx
        mov ebx, DATA3
        div ebx
        mov DATA4, eax
    
        mov eax, DATA2
        mul DATA4
        mov DATA5, eax
        mov DATA5 + 4, eax
        

        ;;;;
        mov ah,09H
        lea dx, MSG3
        int 21H
        mov ebx, offset DATA5
		mov eax, [ebx]
        call PRINTNUM
        mov ah, 09
	    mov dx, offset NEWLINE
	    int 21h




        mov ah, 4ch
        int 21h
        ret    
MAIN    ENDP

GCD proc
	push eax
	push ebx
	push edx
	push esi
	push edi	

	mov bx, offset DATA1
	mov esi, [bx]
	mov bx, offset DATA2
	mov edi, [bx]
	cmp esi, edi
	jge false  ;if(esi < edi) : swap
	mov esi, edi
	mov bx, offset DATA1
	mov edi, [bx]
false:
loop1:
	cmp edi, 0
	jle gcd_end

	mov eax, esi
	sub edx, edx
	idiv edi ; edx = rem
	mov esi, edi ; a = b
	mov edi, edx ; b = rem
	
	jmp loop1

gcd_end:
	mov bx, offset DATA3
	mov [bx], esi

	pop edi
	pop esi
	pop edx
	pop ebx
	pop eax
	ret
GCD endp






GETNUM proc
	push eax
	push esi
	push ebx
	push ecx
	push edx
	push edi
    
    ;;;;
    mov ah,09H
    lea dx, MSG1
    int 21H


	
	mov dx, offset BUFFER
	mov ah, 0ah
	int 21h

	mov bx, offset BUFFER
	mov cx, [bx+1]
	add bx, 2

	mov ah, 09
	mov dx, offset NEWLINE
	int 21h

	mov esi, 0
label1:
        
	sub eax, eax
	mov al, [bx]
	sub al, 48  ; al = current digit

	imul esi, esi, 10

	add esi, eax ; esi += current digit

	inc bx
	dec cl
	jnle label1

	mov eax, esi

	pop edi
	pop edx
	pop ecx
	pop ebx
	mov [bx], esi
	pop esi
	pop eax

	ret
GETNUM endp

PRINTNUM PROC ; print eax in decimal      
        push edx
        push eax
        push ecx
        push ebx      
        push di	
        
        mov di, 00h
push_label:  
        inc di
        mov ecx, 10

		sub edx, edx
        idiv ecx 

        push edx
        cmp eax, 0
        jne push_label
print_label:      

        pop edx
        add dl, 48
        mov ah, 02h
        int 21h

        DEC di
        CMP di, 0
        JNE print_label

		pop di
		pop ebx
		pop ecx
		pop eax
		pop edx
        ret
PRINTNUM ENDP
END
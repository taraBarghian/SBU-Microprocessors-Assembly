
         .model small
        .386
        .DATA   
         org 100h
        gpu_memory_offset EQU 0B800H
        MSG1 DB "is the answer $"
        DATA1 DW 10 DUP(0AABBH)
        DATA2 DD 5  DUP(0AAAABBBBH)
        RESULT DB 0
        .CODE
MAIN    PROC FAR
        MOV ax, @DATA
        MOV ds, ax
        
	MOV ah, 0
        mov al, 03h
        int 10h	
        

    
        
        mov si, 02h
        mov dl, 0
        mov bx, offset DATA1
        mov di, offset DATA2
        mov cx, 19 ; number of bytes to compare - 1
check_loop:
        mov al, [bx]
        mov ah, [di]
        cmp al, ah
        jne not_eq
        inc dl
not_eq:
        inc bx
        inc di
        dec cx
        jge check_loop



        mov bx, offset RESULT
        mov [bx], dl
        mov al, [bx]
        call print


        mov ah, 4ch
        int 21h
        ret    
MAIN    ENDP


PRINT   PROC ; input al print al in decimal      
        push dx
        push ax
        push cx
        push bx      

        mov bx, 02h
        MOV dx, gpu_memory_offset
        MOV ds, dx         
        
        mov dx, 00h
PUSH_DIGITS:          
        inc dx
        mov al, al
        sub ah, ah
        mov cl, 10
        div cl
        push ax
        cmp al, 0
        jne PUSH_DIGITS
PRINT_DIGITS:      
        pop ax
        add ah, 48d
        mov [si], ah
        add si, 2d
        dec dx
        cmp dx, 0d
        jne PRINT_DIGITS
        add si, 2d
        pop bx
        pop cx
        pop ax
        pop dx
        ret
PRINT   ENDP







END
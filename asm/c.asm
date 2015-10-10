.model small

.data
    n_lines equ 23
    n_columns equ 4
    msg db 'enter a three-digit number <= 37$'
    ; wait time in ticks (~18.2/s)
    delay equ 18

.stack
    db 1024 dup(?)

.code

; write the address of int6eh to the interrupt vector
intv proc
    push ax
    push bx
    push es
    xor ax, ax
    mov es, ax
    ; int 6eh
    mov bx, 6eh
    ; multiplies the address on the interrupt vector table by 4
    shl bx, 1
    shl bx, 1
    ; puts the subroutine ip on the first two bytes of the vector
    mov ax, offset int6eh
    mov es:[bx], ax
    add bx, 02h
    ; puts the subroutine cs on the last two bytes of the vector
    mov ax, cs
    mov es:[bx], ax
    pop es
    pop bx
    pop ax
    ret
intv endp

; write a three-digit number to the seven-segment display
int6eh proc
    sti
    push ax
    push bx
    push cx
    push dx
    push ax
    mov bx, 0ah
    div bl
    push ax
    xor ah, ah
    mov cx, 2
    call write_num
    pop ax
    mov al, ah
    xor ah, ah
    mov bl, 10h
    mul bl
    push dx
    mov dx, 0301h
    out dx, al
    pop dx
    pop ax
    call sleep
    push ax
    mov al, ' '
    call mv
    call writech
    pop ax
    mov bx, 64h
    push ax
    div bl
    xor ah, ah
    mov cx, 1
    inc dh
    call write_num
    pop ax
    push ax
    push dx
    div bl
    mov al, ah
    xor ah, ah
    mov dx, 0301h
    push ax
    call dec_to_hex
    out dx, al
    pop ax
    pop dx
    call sleep
    call mv
    mov al, ' '
    call writech
    pop ax
    push ax
    mov bl, 0ah
    div bl
    mov dx, 0301h
    push ax
    xor ah, ah
    call dec_to_hex
    out dx, al
    pop ax
    inc dx
    mov al, ah
    xor ah, ah
    mov bx, 10h
    mul bl
    out dx, al
    call sleep
    pop ax
    push ax
    mov bx, 0ah
    div bl
    xor ah, ah
    div bl
    push ax
    xor ah, ah
    mov dx, 0301h
    out dx, al
    pop ax
    mov al, ah
    xor ah, ah
    inc dx
    mov bx, 10h
    mul bl
    out dx, al
    pop ax
    call sleep
    mov bx, 64h
    div bl
    xor ah, ah
    mov bl, 10h
    mul bl
    mov dx, 0302h
    out dx, al
    xor ax, ax
    mov dx, 0301h
    out dx, al
    call sleep
    mov dx, 0302h
    out dx, al
    pop dx
    pop cx
    pop bx
    pop ax
    iret
int6eh endp

; position the cursor on line dh, column dl
mv proc
    push ax
    push bx
    ; page number
    mov bh, 0
    ; "set cursor position" function
    mov ah, 2
    int 10h
    pop bx
    pop ax
    ret
mv endp

; write a character received on al to the screen
writech proc
    push ax
    push bx
    push cx
    ; page number
    mov bh, 0
    ; number of times to print
    mov cx, 1
    ; "write character" function
    mov ah, 0ah
    int 10h
    pop cx
    pop bx
    pop ax
    ret
writech endp

; convert a two-digit decimal number to hexadecimal
dec_to_hex proc
    ; nothing to do if < 10
    cmp al, 10
    js dec_to_hex0
    push bx
    push cx
    mov bx, 10
    div bl
    mov cl, ah
    xor ah, ah
    mov bl, 10h
    mul bl
    add al, cl
    pop cx
    pop bx
dec_to_hex0:
    ret
dec_to_hex endp

; read a three-digit number <=37
read_num_le_37 proc
    call clear
    xor dx, dx
    mov dl, 2
    call mv
    lea dx, msg
    mov ah, 9
    int 21h
    xor dx, dx
    call read_num
    cmp ax, 38
    jns read_num_le_37
    ret
read_num_le_37 endp

; read a sequence of three digits
read_num proc
    push bx
    push cx
    push dx
    xor ax, ax
    mov cl, 3
    mov ch, 10
read_num0:
    push ax
    call read_digit
    ; shift old digits left, add new digit
    mov bx, ax
    pop ax
    mul ch
    add ax, bx
    ; increments line number
    inc dh
    ; loops until three digits are read
    dec cl
    jnz read_num0
    pop dx
    pop cx
    pop bx
    ret
read_num endp

; read a digit
read_digit proc
    call mv
    ; write a space to current position to erase character
    mov al, ' '
    call writech
    ; read a character, with echo
    mov ah, 1
    int 21h
    ; convert ascii to number, test for valid digit
    xor ah, ah
    sub al, '0'
    js read_digit
    cmp al, 10
    jns read_digit
    ret
read_digit endp

; clear the screen
clear proc
    push ax
    ; clear screen function
    mov ax, 2
    int 10h
    pop ax
    ret
clear endp

; wait using the timer
sleep proc
    push ax
    push bx
    push cx
    push dx
    ; get system time in cx:dx
    xor ax, ax
    int 1ah
    mov bx, delay
    add bx, dx
    adc cx, 0
    mov ax, cx
sleep0:
    push ax
    xor ax, ax
    int 1ah
    pop ax
    cmp ax, cx
    js sleep0
    cmp dx, bx
    js sleep0
    pop dx
    pop cx
    pop bx
    pop ax
    ret
sleep endp

; writes the cl most significant digits of ax to the screen at dx
write_num proc
    push ax
    push bx
    push cx
    push si
    push di
    mov di, 10
    add dh, cl
    dec dh
write_num0:
    call mv
    mov bx, dx
    xor dx, dx
    div di
    mov si, ax
    mov ax, dx
    add ax, 30h
    call writech
    mov ax, si
    mov dx, bx
    dec dh
    dec cx
    jnz write_num0
    inc dh
    pop di
    pop si
    pop cx
    pop bx
    pop ax
    ret
write_num endp

; erase top two digits and go to next line
next_line proc
    ; erase top two digits, go to next line
    push ax
    mov al, ' '
    call mv
    call writech
    inc dh
    call mv
    call writech
    pop ax
    ret
next_line endp

main proc
    ; init data
    mov ax, @data
    mov ds, ax
    ; init interrupt vector
    call intv
    ; word to program the interface
    mov al, 80h
    ; address of the port
    mov dx, 0303h
    out dx, al
    call read_num_le_37
    call clear
    mov cx, 3
    xor dx, dx
main0:
    call write_num
    call sleep
    call next_line
    ; last line?
    cmp dh, n_lines
    jnz main0
    int 6eh
    ; go to next column
    inc dl
    ; last column? end
    cmp dl, n_columns
    jz exit
    ; multiply number
    mov bx, dx
    mul cx
    mov dx, bx
    ; go to first line
    xor dh, dh
    jmp main0
exit:
    call clear
    mov ah, 4ch
    int 21h
main endp
end main

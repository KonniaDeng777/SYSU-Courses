[bits 32]

global asm_my_function

asm_my_function:
    push eax
    xor eax, eax

    mov ah, 0x03 ;青色
    mov al, '2'
    mov [gs:2 * 0], ax

    mov al, '1'
    mov [gs:2 * 1], ax

    mov al, '3'
    mov [gs:2 * 2], ax

    mov al, '0'
    mov [gs:2 * 3], ax

    mov al, '7'
    mov [gs:2 * 4], ax

    mov al, '0'
    mov [gs:2 * 5], ax

    mov al, '3'
    mov [gs:2 * 6], ax

    mov al, '5'
    mov [gs:2 * 7], ax

    mov al, 'D'
    mov [gs:2 * 8], ax

    mov al, 'X'
    mov [gs:2 * 9], ax

    mov al, 'Y'
    mov [gs:2 * 10], ax

    pop eax
    ret
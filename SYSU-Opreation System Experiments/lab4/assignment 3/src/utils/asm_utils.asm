[bits 32]

global asm_hello_world
global asm_lidt
global asm_unhandled_interrupt
global asm_halt

ASM_UNHANDLED_INTERRUPT_INFO db 'assignment 3:Unhandled interrupt happened, halt...'
                             db 0


ASM_IDTR dw 0
         dd 0

; void asm_unhandled_interrupt()
asm_unhandled_interrupt:
    cli
    mov esi, ASM_UNHANDLED_INTERRUPT_INFO
    xor ebx, ebx
    mov ah, 0x03
.output_information:
    cmp byte[esi], 0
    je .end
    mov al, byte[esi]
    mov word[gs:bx], ax
    inc esi
    add ebx, 2
    jmp .output_information
.end:
    jmp $

; void asm_lidt(uint32 start, uint16 limit)
asm_lidt:
    push ebp
    mov ebp, esp
    push eax

    mov eax, [ebp + 4 * 3]
    mov [ASM_IDTR], ax
    mov eax, [ebp + 4 * 2]
    mov [ASM_IDTR + 2], eax
    lidt [ASM_IDTR]

    pop eax
    pop ebp
    ret

asm_hello_world:
    push eax
    xor eax, eax

    mov ah, 0x05 ;red
    mov al, 'H'
    mov [gs:2 * 80], ax

    mov al, 'e'
    mov [gs:2 * 81], ax

    mov al, 'l'
    mov [gs:2 * 82], ax

    mov al, 'l'
    mov [gs:2 * 83], ax

    mov al, 'o'
    mov [gs:2 * 84], ax

    mov al, ' '
    mov [gs:2 * 85], ax

    mov al, 'W'
    mov [gs:2 * 86], ax

    mov al, 'o'
    mov [gs:2 * 87], ax

    mov al, 'r'
    mov [gs:2 * 88], ax

    mov al, 'l'
    mov [gs:2 * 89], ax

    mov al, 'd'
    mov [gs:2 * 90], ax

    pop eax
    ret

asm_halt:
    jmp $
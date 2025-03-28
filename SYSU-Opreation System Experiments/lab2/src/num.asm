org 0x7c00
[bits 16]
xor ax, ax ; eax = 0
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; 初始化栈指针
mov sp, 0x7c00
mov ax, 0xb800
mov gs, ax


mov ah, 0x15 ; 
mov al, '2'
mov [gs:2 * (12*80 + 12)], ax

mov al, '1'
mov [gs:2 * (12*80 + 13)], ax

mov al, '3'
mov [gs:2 * (12*80 + 14)], ax

mov al, '0'
mov [gs:2 * (12*80 + 15)], ax

mov al, '7'
mov [gs:2 * (12*80 + 16)], ax

mov al, '0'
mov [gs:2 * (12*80 + 17)], ax

mov al, '3'
mov [gs:2 * (12*80 + 18)], ax

mov al, '5'
mov [gs:2 * (12*80 + 19)], ax


jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa

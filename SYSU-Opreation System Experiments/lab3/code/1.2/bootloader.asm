org 0x7e00
[bits 16]
mov ax, 0xb800
mov gs, ax
mov ah, 0x01 ;blue
mov ecx, bootloader_tag_end - bootloader_tag
xor ebx, ebx
mov esi, bootloader_tag
output_bootloader_tag:
    mov al, [esi]
    mov word[gs:bx], ax
    inc esi
    add ebx,2
    loop output_bootloader_tag
jmp $ ; 死循环

bootloader_tag db 'run bootloader assignment 1.2'
bootloader_tag_end:
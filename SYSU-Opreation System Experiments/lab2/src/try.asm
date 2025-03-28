org 0x7c00
[bits 16]

xor ax, ax
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

mov sp, 0x7c00
mov ax, 0xb800
mov gs, ax

mov ah, 0
int 0x16 ; 读取键盘输入

mov bl, 0x01
mov ah, 0x0e
mov bh, 0
mov al, [0x40:0x18] ; 将读取到的键盘扫描码转换成字符
mov [gs:2 * (80 * 12 + 40)], ax ; 将字符显示在屏幕上

jmp $ ; 死循环

times 510 - ($-$$) db 0
db 0x55,0xaa

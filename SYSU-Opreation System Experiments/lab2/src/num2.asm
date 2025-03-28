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


mov ah, 0x0e ; 设置光标显示属性
mov bh, 0x00 ; 页号为0
mov al, '2'  ; 显示字符'2'
mov cx, 0x0002 ; 光标移动次数，为2
int 0x10     ; 调用int 10h中断，将字符'2'显示在屏幕上
mov al, '1'  ; 显示字符'1'
int 0x10     ; 调用int 10h中断，将字符'1'显示在屏幕上
mov al, '3'  ; 显示字符'3'
int 0x10     ; 调用int 10h中断，将字符'3'显示在屏幕上
mov al, '0'  ; 显示字符'0'
int 0x10     ; 调用int 10h中断，将字符'0'显示在屏幕上
mov al, '7'  ; 显示字符'7'
int 0x10     ; 调用int 10h中断，将字符'7'显示在屏幕上
mov al, '0'  ; 显示字符'0'
int 0x10     ; 调用int 10h中断，将字符'0'显示在屏幕上
mov al, '3'  ; 显示字符'3'
int 0x10     ; 调用int 10h中断，将字符'3'显示在屏幕上
mov al, '5'  ; 显示字符'5'
int 0x10     ; 调用int 10h中断，将字符'5'显示在屏幕上

jmp $ ; 死循环

times 510 - ($ - $$) db 0
db 0x55, 0xaa

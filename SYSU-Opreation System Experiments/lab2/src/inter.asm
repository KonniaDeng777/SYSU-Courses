org 0x7c00

[bits 16]

mov ax, 0
mov ds, ax

; 获取光标当前位置
mov ah, 0x03 ; 功能号为0x03
int 0x10     ; 调用int 10h中断
mov ah, 0x0e ; 功能号为0x0e
mov bh, 0x00 ; 页号为0
mov dl, ' '  ; 显示字符为空格
int 0x10     ; 调用int 10h中断，将空格打印到光标所在位置
add dl, 0x30 ; 将光标所在列数转换成字符，存放在DL中
int 0x10     ; 调用int 10h中断，将光标所在列数显示在屏幕上
mov dl, ah   ; 将光标所在行数存放在DL中
add dl, 0x30 ; 将光标所在行数转换成字符，存放在DL中
mov ah, 0x0e ; 功能号为0x0e
int 0x10     ; 调用int 10h中断，将光标所在行数显示在屏幕上

; 移动光标到指定位置
mov ah, 0x02 ; 功能号为0x02
mov bh, 0x00 ; 页号为0
mov dh, 0x0a ; 移动到第10行
mov dl, 0x0a ; 移动到第10列
int 0x10     ; 调用int 10h中断，将光标移动到指定位置

jmp $

times 510 - ($ - $$) db 0
db 0x55, 0xaa

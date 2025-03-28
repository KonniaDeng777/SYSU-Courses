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
 ; mov ax, 0xb000
 ; mov gs, ax
    
 mov ah, 0x03 ; 功能码，表示调用读取光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 int 0x10 ; interrupt
    
 mov bl, 0x01 ; blue
 mov al, 'I'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt    
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'N'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'T'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'E'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'R'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'R'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'U'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'P'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, 'T'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov al, ':'
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt


 ; 键盘I/O中断调用（INT 16H）
 kbIOint:
 mov ah, 0x00 ;
 int 0x16 ;
 or al, 0x00 ;
 ; and zf, 0x01;
 jnz input
 jmp kbIOint
    
 input:
    
 mov ah, 0x02 ; 功能码，表示调用设置光标位置的中断功能
 mov bh, 0x00 ; 页码，文本状态设为0 
 add dl, 1 ; 
 int 0x10 ; interrupt
    
 mov ah, 0x09 ; 功能码，表示在当前光标位置写字符和属性的中断功能
 mov cx, 0x01 ; 输出字符的个数
 int 0x10 ; interrupt  
    
 jmp kbIOint ;  
    
 jmp $ ; 死循环
    
 times 510 - ($ - $$) db 0
 db 0x55, 0xaa
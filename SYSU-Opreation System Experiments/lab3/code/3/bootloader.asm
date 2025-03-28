%include "boot.inc"
org 0x7e00
[bits 16]


mov dword [GDT_START_ADDRESS+0x00],0x00
mov dword [GDT_START_ADDRESS+0x04],0x00  

mov dword [GDT_START_ADDRESS+0x08],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x0c],0x00cf9200    ; 粒度为4KB，存储器段描述符 

;建立保护模式下的堆栈段描述符       
mov dword [GDT_START_ADDRESS+0x10],0x00000000    ; 基地址为0x00000000，界限0x0 
mov dword [GDT_START_ADDRESS+0x14],0x00409600    ; 粒度为1个字节

;建立保护模式下的显存描述符    
mov dword [GDT_START_ADDRESS+0x18],0x80007fff    ; 基地址为0x000B8000，界限0x07FFF 
mov dword [GDT_START_ADDRESS+0x1c],0x0040920b    ; 粒度为字节

;创建保护模式下平坦模式代码段描述符
mov dword [GDT_START_ADDRESS+0x20],0x0000ffff    ; 基地址为0，段界限为0xFFFFF
mov dword [GDT_START_ADDRESS+0x24],0x00cf9800    ; 粒度为4kb，代码段描述符 

;初始化描述符表寄存器GDTR
mov word [pgdt], 39       ;描述符表的界限    
lgdt [pgdt]
       
in al,0x92                           ;南桥芯片内的端口 
or al,0000_0010B
out 0x92,al                          ;打开A20

cli                                  ;中断机制尚未工作
mov eax,cr0
or eax,1
mov cr0,eax                          ;设置PE位
       
;以下进入保护模式
jmp dword CODE_SELECTOR:protect_mode_begin

;16位的描述符选择子：32位偏移
;清流水线并串行化处理器
[bits 32]            
protect_mode_begin:                                

;定义各个符号
    _DR equ 1
    _UR equ 2
    _UL equ 3
    _DL equ 4
    delay equ 300
    double_delay equ 150
    
;代码段
;初始化各个寄存器
START:

mov eax, DATA_SELECTOR
mov ds, eax
mov gs, eax
mov eax, STACK_SELECTOR
mov ss, eax
mov eax, VIDEO_SELECTOR
mov es, eax

; mov ecx,0
; mov eax,0
; _init_:
;     cmp ecx,0x00007FFF
;     je _loop
    ; mov dword[gs:ecx], eax
;     add ecx,1
;     jmp _init_
; _loop:
; mov ebx,2
; mov ecx,0
; mov esi,1        ;offset
; mov edi,1        ;offset
; ; dead loop
; ;initilizing the start_point
;     mov ax, cs
;     mov es, ax
;     mov ds, ax
;     mov ax, 0b800h
;     mov es, ax
    mov esi, 0
    mov edi, 0

;mov ecx, protect_mode_tag_end - protect_mode_tag
;mov ebx, 80 * 2
;mov esi, protect_mode_tag
;mov ah, 0x3
;output_protect_mode_tag:
;    mov al, [esi]
;    mov word[gs:ebx], ax
;    add ebx, 2
;    inc esi
;    loop output_protect_mode_tag

;jmp $ ; 死循环

;lab2的assignment4中部分代码
;寄存器要改成32位寄存器

PRINT:    
    mov ebx, title
    mov al, [ebx+esi]
    cmp al, 0
    jz LOOP1    
    mov ebx, 52
    mov byte[es:ebx+edi], al    
    mov byte[es:ebx+edi+1], 1
    inc esi
    add edi, 2
    jmp PRINT

;循环实现延迟
LOOP1:
    dec dword[count]
    jnz LOOP1
    
    mov dword[count], delay
    dec dword[double_count]
    jnz LOOP1
    
    mov dword[count], delay
    mov dword[double_count], double_delay
    
    mov al,1
        cmp al, byte[RightDownUpLeft]
    jz DownRight 
       
    mov al, 2
           cmp al, byte[RightDownUpLeft]
    jz UpRight

           mov al, 3
           cmp al, byte[RightDownUpLeft]
    jz UpLeft
       
    mov al, 4
           cmp al, byte[RightDownUpLeft]
    jz  DownLeft

    jmp $

;往右下移动，判断是否碰壁并显示字符
DownRight:
    inc dword[x]
    inc dword[y]
    mov ebx, dword[x]
    mov eax, 25
    sub eax, ebx
          jz  DownRightToUpRight
    
    mov ebx, dword[y]
    mov eax, ebx
           jz  DownRightToDownLeft
    
    jmp show

DownRightToUpRight:
           mov dword[x], 23
           mov byte[RightDownUpLeft], _UR    
           jmp show
DownRightToDownLeft:
           mov dword[y], 78
          mov byte[RightDownUpLeft], _DL    
           jmp show

;往右上移动，判断是否碰壁并显示字符
UpRight:
    dec dword[x]
    inc dword[y]
    mov ebx, dword[y]
    mov eax, 80
    sub eax, ebx
           jz  UpRightToUpLeft
    
    mov ebx, dword[x]
    mov eax, 0
    sub eax, ebx
          jz  UpRightToDownRight
    
    jmp show

UpRightToUpLeft:
           mov dword[y], 78
           mov byte[RightDownUpLeft], _UL    
           jmp show
UpRightToDownRight:
           mov dword[x], 1
           mov byte[RightDownUpLeft], _DR    
           jmp show
    
;往左上移动，判断是否碰壁并显示字符
UpLeft:
    dec dword[x]
    dec dword[y]
    mov ebx, dword[x]
    mov eax, 0
    sub eax, ebx
           jz  UpLeftToDownLeft

    mov ebx,dword[y]
    mov eax, -1
    sub eax, ebx
          jz  UpLeftToUpRight
    
    jmp show

UpLeftToDownLeft:
           mov dword[x], 1
           mov byte[RightDownUpLeft], _DL    
           jmp show
UpLeftToUpRight:
           mov dword[y], 1
           mov byte[RightDownUpLeft], _UR    
           jmp show

;往左下移动，判断是否碰壁并显示字符
DownLeft:
    inc dword[x]
    dec dword[y]
    mov ebx, dword[y]
    mov eax, -1
    sub eax, ebx
           jz  DownLeftToDownRight
    
    mov ebx, dword[x]
    mov eax, 25
    sub eax, ebx
           jz  DownLeftToUpLeft
    
    jmp show

DownLeftToDownRight:
           mov dword[y], 1
           mov byte[RightDownUpLeft], _DR    
           jmp show
DownLeftToUpLeft:
           mov dword[x], 23
          mov byte[RightDownUpLeft], _UL    
           jmp show

;在屏幕上显示字符
show:    
    xor eax, eax                  ; 计算显存地址
    mov eax, dword[x]
    mov ebx, 80
    mul ebx
    add eax, dword[y]
    mov ebx, 2
    mul ebx
    mov ebx, eax
    mov ah, byte[color]    ;  0000：黑底、1111：亮白字（默认值为0x07）
    mov al, byte[char]    ;  AL = 显示字符值（默认值为20h=空格符）
    mov [es:ebx], eax       ;  显示字符的ASCII码值
    
    inc byte[char]
    cmp byte[char], 'z'+1
    jnz keep
    mov byte[char], '0'

keep:    
    inc byte[color]
    cmp byte[color], 0x10
    jnz LOOP1
    mov byte[color], 0x40 ;
    jmp LOOP1

end:
    jmp $


    count dd delay        
    double_count dd double_delay    
    RightDownUpLeft db _DR             
    color db 0x02        
    x dd 0                 
    y dd 0                 
    char db '1'             
    title db 'protect mode', 0    

    ; times 510-($-$$) db 0
    ; dw 0aa55h


pgdt dw 0
     dd GDT_START_ADDRESS

;bootloader_tag db 'run bootloader'
;bootloader_tag_end:

;protect_mode_tag db 'enter protect mode'
;protect_mode_tag_end:

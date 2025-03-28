org 0x7c00
[bits 16]

    _DR equ 1
    _UR equ 2
    _UL equ 3
    _DL equ 4
    delay equ 400
    double_delay equ 200

START:
    mov ax, cs
    mov es, ax
    mov ds, ax
    mov ax, 0b800h
    mov es, ax
    mov si, 0
    mov di, 0

PRINT:    
    mov bx, name
    mov al, [bx+si]
    cmp al, 0
    jz LOOP1    
    mov bx, 52
    mov byte[es:bx+di], al    
    mov byte[es:bx+di+1], 1
    inc si
    add di, 2
    jmp PRINT

LOOP1:
    dec word[count]
    jnz LOOP1
    
    mov word[count], delay
    dec word[double_count]
    jnz LOOP1
    
    mov word[count], delay
    mov word[double_count], double_delay
   
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

DownRight:
    inc word[x]
    inc word[y]
    mov bx, word[x]
    mov ax, 25
    sub ax, bx
         jz  DownRightToUpRight
    
    mov bx, word[y]
    mov ax, 80
    sub ax, bx
          jz  DownRightToDownLeft
    
    jmp show

DownRightToUpRight:
          mov word[x], 23
          mov byte[RightDownUpLeft], _UR    
          jmp show
DownRightToDownLeft:
          mov word[y], 78
         mov byte[RightDownUpLeft], _DL    
          jmp show

UpRight:
    dec word[x]
    inc word[y]
    mov bx, word[y]
    mov ax, 80
    sub ax, bx
          jz  UpRightToUpLeft
    
    mov bx, word[x]
    mov ax, 0
    sub ax, bx
         jz  UpRightToDownRight
    
    jmp show

UpRightToUpLeft:
          mov word[y], 78
          mov byte[RightDownUpLeft], _UL    
          jmp show
UpRightToDownRight:
          mov word[x], 1
          mov byte[RightDownUpLeft], _DR    
          jmp show
    
UpLeft:
    dec word[x]
    dec word[y]
    mov bx, word[x]
    mov ax, 0
    sub ax, bx
          jz  UpLeftToDownLeft

    mov bx,word[y]
    mov ax, -1
    sub ax, bx
         jz  UpLeftToUpRight
    
    jmp show

UpLeftToDownLeft:
          mov word[x], 1
          mov byte[RightDownUpLeft], _DL    
          jmp show
UpLeftToUpRight:
          mov word[y], 1
          mov byte[RightDownUpLeft], _UR    
          jmp show

DownLeft:
    inc word[x]
    dec word[y]
    mov bx, word[y]
    mov ax, -1
    sub ax, bx
          jz  DownLeftToDownRight
    
    mov bx, word[x]
    mov ax, 25
    sub ax, bx
          jz  DownLeftToUpLeft
    
    jmp show

DownLeftToDownRight:
          mov word[y], 1
          mov byte[RightDownUpLeft], _DR    
          jmp show
DownLeftToUpLeft:
          mov word[x], 23
         mov byte[RightDownUpLeft], _UL    
          jmp show

show:    
    xor ax, ax           
    mov ax, word[x]
    mov bx, 80
    mul bx
    add ax, word[y]
    mov bx, 2
    mul bx
    mov bx, ax
    mov ah, byte[color]      
    mov al, byte[char]    
    mov [es:bx], ax  
    
    inc byte[char]
    cmp byte[char], 'z'+1
    jnz keep
    mov byte[char], '0'

keep:    
    inc byte[color]
    cmp byte[color], 0x10
    jnz LOOP1
    mov byte[color], 0x40 ;    循环显示不同样式的字符
    jmp LOOP1

end:
    jmp $

    count dw delay        ;一层延迟
    double_count dw double_delay    ;二层延迟
    RightDownUpLeft db _DR        ;方向变量
    color db 0x05        ;样式（颜色）变量
    x dw 0                ;横坐标
    y dw 0                ;纵坐标
    char db '1'            ;要显示的字符
    name db 'DengXuying 21307035', 0    ;学号姓名

    times 510-($-$$) db 0
    dw 0aa55h

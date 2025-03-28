; If you meet compile error, try 'sudo apt install gcc-multilib g++-multilib' first

%include "head.include"
; you code here

your_if:
; put your implementation here

	mov eax, [a1]
	cmp eax, 0xc 
	jl less_than_12
	jmp more_than_12

less_than_12:
	mov eax, [a1]
	mov ebx, 0x2
	div ebx
	add eax, 1
	mov [if_flag], eax
	jmp your_if_end
more_than_12:
	mov eax, [a1]
	cmp eax, 24
	jl less_than_24
	jmp more_than_24
less_than_24:
	mov ecx, 0x18
	sub ecx, eax
	imul ecx, eax
	mov [if_flag], ecx
	jmp your_if_end
more_than_24:
	mov eax, [a1]
	shl eax, 4
	mov [if_flag], eax
	jmp your_if_end
your_if_end:


your_while:
; put your implementation here

	mov ebx, [a2]
	cmp ebx, 0xc
	jl your_while_end
while:
	call my_random
	mov edx, [while_flag]
	sub ebx, 0xc
	mov [edx + ebx], eax
	dec ebx
	mov [a2], ebx
	cmp ebx, 12
	jge while
your_while_end:

%include "end.include"

your_function:
; put your implementation here

	mov eax, 0
	mov edx, [your_string]
	mov ebx, 0
	cmp [edx], ebx
	je your_function_end

Loop:
	pushad 
	mov edx, [edx + eax] 
	push edx 
	call print_a_char 
	pop edx 
	popad 
	inc eax 
  
	cmp byte [edx + eax], 0 
	jne Loop 
your_function_end:
	

	
	
	
	

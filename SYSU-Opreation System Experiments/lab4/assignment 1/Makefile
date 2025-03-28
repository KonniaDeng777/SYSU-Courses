TARTGET = main

ASM_FILE = $(wildcard *.asm)


build:
	@rm -rf *.o
	@nasm -f elf32 $(ASM_FILE)
	@i686-linux-gnu-gcc -m32 -g -static -c c_func.c
	@i686-linux-gnu-g++ -m32 -g -static -c cpp_func.cpp
	@i686-linux-gnu-g++ -m32 -g -static -c main.cpp
	@i686-linux-gnu-g++ -m32 -g -static -o $(TARTGET) main.o c_func.o cpp_func.o asm_utils.o
	@echo main | cpio -o --format=newc > hwinitramfs
run:
	@qemu-system-i386 -kernel ~/lab1/linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs -append "console=ttyS0 rdinit=main" -nographic
clean:
	@rm -rf *.o

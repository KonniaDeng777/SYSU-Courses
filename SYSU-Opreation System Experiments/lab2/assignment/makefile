TARTGET = test

AS = nasm
CXX = i686-linux-gnu-g++

ASM_FILE = $(wildcard *.asm)
CXX_FILE = $(wildcard *.cpp)

ALL_OBJ += $(ASM_FILE:%.asm=%.o)
ALL_OBJ += $(CXX_FILE:%.cpp=%.o)

build:
	@rm -rf *.o
	@nasm -f elf32 $(ASM_FILE)
	@i686-linux-gnu-g++ -m32 -g -static -c $(CXX_FILE)
	@i686-linux-gnu-g++ -m32 -g -static -o $(TARTGET) $(ALL_OBJ)
	@echo test | cpio -o --format=newc > hwinitramfs
	@qemu-system-i386 -kernel ~/lab1/linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs -append "console=ttyS0 rdinit=test" -nographic
run:
	@qemu-system-i386 -kernel ~/lab1/linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs -append "console=ttyS0 rdinit=test" -nographic
clean:
	@rm -rf *.o

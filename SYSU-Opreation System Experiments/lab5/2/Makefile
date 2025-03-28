TARTGET = main

build:
	@i686-linux-gnu-g++ -m32 -g -static -c main.cpp
	@i686-linux-gnu-g++ -m32 -g -static -o $(TARTGET) main.o 
	@echo main | cpio -o --format=newc > hwinitramfs
run:
	@qemu-system-i386 -kernel ~/lab1/linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs -append "console=ttyS0 rdinit=main" -nographic
clean:
	@rm -rf *.o

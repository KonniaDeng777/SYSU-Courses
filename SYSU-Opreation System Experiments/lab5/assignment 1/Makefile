TARTGET = myprintf

build:
	@i686-linux-gnu-g++ -m32 -g -static -c myprintf.cpp
	@i686-linux-gnu-g++ -m32 -g -static -o $(TARTGET) myprintf.o 
	@echo myprintf | cpio -o --format=newc > hwinitramfs
run:
	@qemu-system-i386 -kernel ~/lab1/linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs -append "console=ttyS0 rdinit=myprintf" -nographic
clean:
	@rm -rf *.o

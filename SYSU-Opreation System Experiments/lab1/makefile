run1:
	@qemu-system-i386 -kernel linux-5.10.172/arch/x86/boot/bzImage -initrd hwinitramfs  -append "console=ttyS0 rdinit=helloworld" -nographic
run2:
	@qemu-system-i386 -kernel linux-5.10.172/arch/x86/boot/bzImage -initrd initramfs-busybox-x86.cpio.gz -nographic -append "console=ttyS0"

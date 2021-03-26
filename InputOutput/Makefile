all: kernel

kernel.o: kernel.c
	bcc -ansi -c -o kernel.o kernel.c

kernel_asm.o: kernel.o 
	as86 kernel.asm -o kernel_asm.o

kernel: kernel.o kernel_asm.o floppya.img
	ld86 -o kernel -d kernel.o kernel_asm.o
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

bootload: 
	nasm bootload.asm

floppya.img: bootload
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc

show: 
	qemu-system-i386 -fda floppya.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a &


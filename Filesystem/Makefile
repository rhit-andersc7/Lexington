DIRS=bin obj
$(shell mkdir -p $(DIRS))

all: loadFiles

loadFiles: bin/loadFile bin/kernel
	./bin/loadFile message.txt
	./bin/loadFile bigmess.txt
	./bin/loadFile tstprg

bin/kernel: obj/kernel.o obj/kernel_asm.o lexington.img
	echo $(shell pwd)
	# mkdir -p bin
	ld86 -o bin/kernel -d obj/kernel.o obj/kernel_asm.o
	dd if=bin/kernel of=lexington.img bs=512 conv=notrunc seek=3

bin/loadFile: obj/loadFile.o
	gcc obj/loadFile.o -o bin/loadFile	

obj/bootload: bootload.asm
	# mkdir -p obj
	nasm bootload.asm -o obj/bootload

obj/loadFile.o: loadFile.c
	gcc -c -o obj/loadFile.o loadFile.c

obj/kernel.o: kernel.c kernel.h builtin.h
	# mkdir -p obj
	bcc -ansi -c -o obj/kernel.o kernel.c

obj/kernel_asm.o: kernel.asm
	# mkdir -p obj
	as86 kernel.asm -o obj/kernel_asm.o

lexington.img: obj/bootload
	dd if=/dev/zero of=lexington.img bs=512 count=2880
	dd if=obj/bootload of=lexington.img bs=512 count=1 conv=notrunc
	dd if=message.txt of=lexington.img bs=512 count=1 seek=30 conv=notrunc
	dd if=map.img of=lexington.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=lexington.img bs=512 count=1 seek=2 conv=notrunc

clean:
	rm -rf bin obj lexington.img

run:
	qemu-system-i386 -fda lexington.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a

# To build in docker container:
build:
	make -C .. build

DIRS=bin obj
$(shell mkdir -p $(DIRS))

all: lexington.img bin/kernel # bin/shell

bin/kernel: obj/kernel.o obj/kernel_asm.o
	ld86 -o bin/kernel -d obj/kernel.o obj/kernel_asm.o
	dd if=bin/kernel of=lexington.img bs=512 conv=notrunc seek=3

bin/shell: obj/shell.o obj/lib.o
	ld86 -o shell -d obj/shell.o obj/lib.o
	./bin/loadFile bin/shell

bin/loadFile: obj/loadFile.o
	gcc obj/loadFile.o -o bin/loadFile	

obj/bootload: bootload.asm
	nasm bootload.asm -o obj/bootload

obj/loadFile.o: loadFile.c
	gcc -c -o obj/loadFile.o loadFile.c

obj/kernel.o: kernel.c kernel.h builtin.h
	bcc -ansi -c -o obj/kernel.o kernel.c

obj/shell.o: shell.c shell.h
	bcc -ansi -c -o obj/shell.o shell.c

obj/kernel_asm.o: kernel.asm
	as86 kernel.asm -o obj/kernel_asm.o

obj/lib.o: lib.asm
	as86 lib.asm -o obj/lib.o

lexington.img: obj/bootload
	dd if=/dev/zero of=lexington.img bs=512 count=2880
	dd if=obj/bootload of=lexington.img bs=512 count=1 conv=notrunc
	dd if=message.txt of=lexington.img bs=512 count=1 seek=30 conv=notrunc
	dd if=map.img of=lexington.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=lexington.img bs=512 count=1 seek=2 conv=notrunc
	make loadFiles

loadFiles: bin/loadFile bin/shell
	./bin/loadFile message.txt
	./bin/loadFile bigmess.txt
	./bin/loadFile tstprg
	./bin/loadFile shell

clean:
	rm -rf bin obj lexington.img

run:
	qemu-system-i386 -fda lexington.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a

# To build in docker container:
build:
	make -C .. build

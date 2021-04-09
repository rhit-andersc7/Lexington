DIRS=bin obj out
$(shell mkdir -p $(DIRS))

all: lexington.img

lexington.img: out/lexington.img bin/shell bin/loadFile
	cp out/lexington.img lexington.img
	./bin/loadFile message.txt
	./bin/loadFile bigmess.txt
	./bin/loadFile tstprg
	cp bin/shell shell
	./bin/loadFile shell
	rm shell

# Binary files {{{
bin/kernel: obj/kernel.o obj/kernel_asm.o
	ld86 -o bin/kernel -d obj/kernel.o obj/kernel_asm.o

bin/shell: obj/shell.o obj/lib.o
	ld86 -o bin/shell -d obj/shell.o obj/lib.o

bin/loadFile: obj/loadFile.o
	gcc obj/loadFile.o -o bin/loadFile	
# }}}

# Object files {{{
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
# }}}

out/lexington.img: obj/bootload bin/kernel
	dd if=/dev/zero of=out/lexington.img bs=512 count=2880
	dd if=obj/bootload of=out/lexington.img bs=512 count=1 conv=notrunc
	dd if=message.txt of=out/lexington.img bs=512 count=1 seek=30 conv=notrunc
	dd if=map.img of=out/lexington.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=out/lexington.img bs=512 count=1 seek=2 conv=notrunc
	dd if=bin/kernel of=out/lexington.img bs=512 conv=notrunc seek=3

clean:
	rm -rf bin obj out lexington.img

run:
	qemu-system-i386 -fda lexington.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a

# To build in docker container:
build:
	make -C .. build

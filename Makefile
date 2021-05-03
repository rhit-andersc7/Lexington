ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
USER_ID:=$(shell id -u)
GROUP_ID:=$(shell id -g)

DIRS=.out/bin .out/obj
$(shell mkdir -p $(DIRS))

all: lexington.img

lexington.img: .out/lexington.img .out/bin/shell .out/bin/loadFile
	cp .out/lexington.img lexington.img
	cp .out/bin/shell shell
	.out/bin/loadFile shell
	rm shell
	mv lexington.img dir/lexington.img
	cd dir && ../.out/bin/loadFile message.txt
	cd dir && ../.out/bin/loadFile bigmess.txt
	cd dir && ../.out/bin/loadFile tstprg
	cd dir && ../.out/bin/loadFile tstpr2
	mv dir/lexington.img lexington.img

# Binary files {{{
.out/bin/kernel: .out/obj/kernel.o .out/obj/kernel_asm.o
	ld86 -o .out/bin/kernel -d .out/obj/kernel.o .out/obj/kernel_asm.o

.out/bin/shell: .out/obj/shell.o .out/obj/lib.o
	ld86 -o .out/bin/shell -d .out/obj/shell.o .out/obj/lib.o

.out/bin/loadFile: .out/obj/loadFile.o
	gcc .out/obj/loadFile.o -o .out/bin/loadFile	
# }}}

# Object files {{{
.out/obj/bootload: asm/bootload.asm
	nasm asm/bootload.asm -o .out/obj/bootload

.out/obj/loadFile.o: src/loadFile.c
	gcc -c -o .out/obj/loadFile.o src/loadFile.c

.out/obj/kernel.o: src/kernel.c src/kernel.h src/builtin.h
	bcc -ansi -c -o .out/obj/kernel.o src/kernel.c

.out/obj/shell.o: src/shell.c src/shell.h
	bcc -ansi -c -o .out/obj/shell.o src/shell.c

.out/obj/kernel_asm.o: asm/kernel.asm
	as86 asm/kernel.asm -o .out/obj/kernel_asm.o

.out/obj/lib.o: asm/lib.asm
	as86 asm/lib.asm -o .out/obj/lib.o
# }}}

.out/lexington.img: .out/obj/bootload .out/bin/kernel
	dd if=/dev/zero of=.out/lexington.img bs=512 count=2880
	dd if=.out/obj/bootload of=.out/lexington.img bs=512 count=1 conv=notrunc
	dd if=dir/message.txt of=.out/lexington.img bs=512 count=1 seek=30 conv=notrunc
	dd if=asm/map.img of=.out/lexington.img bs=512 count=1 seek=1 conv=notrunc
	dd if=asm/dir.img of=.out/lexington.img bs=512 count=1 seek=2 conv=notrunc
	dd if=.out/bin/kernel of=.out/lexington.img bs=512 conv=notrunc seek=3

clean:
	rm -rf .out lexington.img

run:
	qemu-system-i386 -fda lexington.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a

# Docker setup {{{
build:
	docker run -v $(ROOT_DIR):/lexington:z --rm lexington make

dev:
	docker run -v $(ROOT_DIR):/lexington:z -it --rm lexington fish

gen:
	docker build -t lexington:latest \
		--build-arg USER_ID=$(USER_ID) \
		--build-arg GROUP_ID=$(GROUP_ID) .
# }}}

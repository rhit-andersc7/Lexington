ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
USER_ID:=$(shell id -u)
GROUP_ID:=$(shell id -g)

all: build run

gen:
	docker build -t lexington:latest \
		--build-arg USER_ID=$(USER_ID) \
		--build-arg GROUP_ID=$(GROUP_ID) .

build:
	docker run -v $(ROOT_DIR):/lexington:z --rm lexington make input_output

dev:
	docker run -v $(ROOT_DIR):/lexington:z -it --rm lexington fish

run:
	qemu-system-i386 -fda lexington.img -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot order=a

# For use inside the docker container
input_output:
	make -C ./InputOutput
	ln -sf ./InputOutput/lexington.img lexington.img

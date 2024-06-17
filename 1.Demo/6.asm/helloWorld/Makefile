#!/bin/bash
#########################################################################
# File Name: run.sh
# Author: LiHongjin
# mail: 872648180@qq.com
# Created Time: Thu Jul 20 15:10:03 2023
#########################################################################

all:
	@as -o hello.o hello.s && ld -s -o hello hello.o && ./hello

# nasm -f elf hello.asm
all2:
	@nasm -f elf64 hello.asm && ld -s -o hello hello.o && ./hello

clean:
	rm hello hello.o

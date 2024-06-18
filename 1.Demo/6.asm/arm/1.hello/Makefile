all:
	# 汇编源码
	arm-none-eabi-as -o hello.o hello.s
	# 链接生成可执行文件
	arm-none-eabi-ld -o hello hello.o

clean:
	rm hello hello.o

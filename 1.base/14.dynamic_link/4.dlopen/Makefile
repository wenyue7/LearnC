all:
	gcc -o main main.c
	gcc -shared -o libmlib.so mlib.c

.PHONY:clean
clean:
	rm  libmlib.so main

.PHONY:test
test:
	@LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH ./main
	@# 或者在命令行执行：
	@# export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && ./main

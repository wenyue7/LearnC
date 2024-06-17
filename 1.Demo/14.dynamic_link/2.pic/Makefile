all: mlib.so
	gcc -o main main.c -L. -lmlib
	gcc -o main2 main2.c -L. -lmlib
	@# 如果在编译的时候加上 -Wl,-rpath,/path/to/your/library，则不需要在运行时
	@# 指定库搜索路径，其中：
	@# -Wl: 传递选项给链接器（linker），这个选项告诉编译器接下来的参数应该直接
	@#      传递给链接器，而不是由编译器本身处理。
	@# -rpath: 是一个链接器选项，用于指定在运行时，解析共享库（如 .so 文件）时
	@#         应该搜索的路径。
	@# gcc -o main main.c -L. -lmlib -Wl,-rpath,`pwd`

mlib.so :
	gcc -shared -fPIC -o libmlib.so mlib.c

.PHONY:clean
clean:
	rm  libmlib.so main main2

.PHONY:test
test:
	@LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH ./main2 &
	@LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH ./main &
	@# 或者在命令行执行：
	@# export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH && ./main

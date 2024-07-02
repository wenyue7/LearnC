all:
	gcc -shared -fPIC -o preload_debug.so preload_debug.c

.PHONY:clean
clean:
	rm preload_debug.so

.PHONY:test
test:
	LD_PRELOAD=./preload_debug.so ls

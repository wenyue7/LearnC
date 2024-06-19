
all:
	gcc -o main main.c test.c && ./main

asm:
	gcc -o test_S.s -S test.c
	gcc -g -c test.o test.c && objdump -D -S test.o > test_D.s

clean:
	rm -f main test_S.s test_D.s test.o

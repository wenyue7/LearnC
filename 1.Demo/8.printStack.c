/*************************************************************************
    > File Name: printStack.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Thu Sep  8 17:23:39 2022
 ************************************************************************/

#include<stdio.h>
#include <execinfo.h>

static void printStack(void)
{
	#define STACK_SIZE 32
	void *trace[STACK_SIZE];
	size_t size = backtrace(trace, STACK_SIZE);
	char **symbols = (char **)backtrace_symbols(trace,size);
	size_t i = 0;
	for(; i < size; i++)
		printf("%d--->%s\n", (int)i, symbols[i]);

	return;
}

void test(idx)
{
    if (idx > 10)
        return;
    printf("======> stack idx:%d\n", idx);
    printStack();
    printf("\n");
    test(idx+1);

    return;
}

int main()
{
    test(0);

    return 0;
}

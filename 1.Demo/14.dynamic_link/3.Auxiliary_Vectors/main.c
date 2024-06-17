/*************************************************************************
    > File Name: main.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Tue 21 May 2024 09:12:00 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/auxv.h>

int main() {
    unsigned long type;
    unsigned long value;

    // 获取AT_PHDR的值，它是程序头表的地址
    value = getauxval(AT_PHDR);
    printf("AT_PHDR: %p\n", (void*) value);

    // 获取AT_BASE的值，它是程序基地址
    value = getauxval(AT_BASE);
    printf("AT_BASE: %p\n", (void*) value);

    // 获取AT_ENTRY的值，它是程序入口点的地址
    value = getauxval(AT_ENTRY);
    printf("AT_ENTRY: %p\n", (void*) value);

    // 打印其他一些辅助向量信息
    for (type = AT_NULL; type <= AT_HWCAP2; type++) {
        value = getauxval(type);
        if (value != 0) {
            printf("AT_%ld: %p\n", type, (void*) value);
        }
    }

    return 0;
}


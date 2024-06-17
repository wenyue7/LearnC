/*************************************************************************
    > File Name: 3.4.thread_local_storage.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed Mar 20 09:02:01 2024
 ************************************************************************/

/*
 * 线程局部存储（TLS），是一种变量的存储方法，这个变量在它所在的线程内是全局可
 * 访问的，但是不能被其他线程访问到，这样就保持了数据的线程独立性。而全局变量，
 * 是所有线程都可以访问的，这样就不可避免需要锁来控制，增加了控制成本和代码复杂度。
 *
 * 目前使用TLS的方法有多种，POSIX的pthread.h提供了一组API来实现此功能:
 * int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
 * int pthread_key_delete(pthread_key_t key);
 * void *pthread_getspecific(pthread_key_t key);
 * int pthread_setspecific(pthread_key_t key, const void *value);
 *
 * 除了API的方式，GCC的编译器也支持语言级别的用法，这样比用API调用，更简单
 * __thread int i;
 * extern __thread struct state s;
 * static __thread char *p;
 *
 */

#include <stdio.h>
#include<pthread.h>
#include<unistd.h>

static __thread int tlsVar = 1;
static int normalVar = 1;

void* Thread1(void *arg)
{
    (void)arg;
    tlsVar += 1;
    normalVar += 1;
    printf("Thead1 tlsVal: %d normalVar:%d\n", tlsVar, normalVar);

    return NULL;
}

void* Thread2(void *arg)
{
    (void)arg;
    tlsVar += 2;
    normalVar += 2;
    sleep(1);
    printf("Thead2 tlsVal: %d normalVar:%d\n", tlsVar, normalVar);

    return NULL;
}

int main()
{
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, Thread1, NULL);
    pthread_create(&pid2, NULL, Thread2, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    return 0;
}

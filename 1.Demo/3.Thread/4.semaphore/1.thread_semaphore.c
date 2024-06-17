/*************************************************************************
    > File Name: 2.3.thread_semaphore.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com 
    > Created Time: Wed 13 Mar 2024 08:44:38 AM CST
 ************************************************************************/

/*
 * reference: https://www.geeksforgeeks.org/use-posix-semaphores-c/
 *
 *
 * 可能需要使用 gcc ac -lpthread -lrt 进行编译
 * 说明：创建 2 个线程，其中一个在第一个线程之后 2 秒创建。但第一个线程在获取锁
 *       后会休眠4秒。因此第二个线程不会在调用后立即进入，而是在调用后 4 – 2 = 2
 *       秒后进入。
 * 
 *
 * 使用场景：
 * 信号量通常用作计数器，主要用于控制并发访问共享资源的数量。当一个进程想要访问
 * 共享资源时，它必须首先获取信号量。如果信号量的值为0，进程将被阻塞，直到其他
 * 进程释放信号量。这种机制在多进程环境中特别有用，因为它可以防止过多的进程同时
 * 访问共享资源，从而导致资源冲突或数据不一致。
 *
 *
 * 信号量和条件变量在以下方面存在区别：
 * 1. 唤醒机制：信号量一次只能唤醒一个特定的进程，而条件变量可以通过广播机制一次
 *    唤醒所有等待的线程。
 * 2. 使用场景：信号量更适合用于控制对共享资源的并发访问数量，而条件变量更适合
 *    用于线程间的同步，特别是在等待某个条件成立时。
 * 3. 同步方式：信号量自身具有wait和signal的原子操作，可以确保value的同步。而条件
 *    变量则需要配合互斥锁使用，通过互斥锁来保证对共享资源的访问控制。
 *
 *
 * 使用方法：
 *
 * 信号量通过使用sem_init（对于进程或线程）或sem_open（对于 IPC）进行初始化
 * sem_init(sem_t *sem, int pshared, unsigned int value);
 * 其中：
 * sem：指定要初始化的信号量。
 * pshared：此参数指定新初始化的信号量是否在进程之间或线程之间共享。非零值表示
 *          信号量在进程之间共享，零值表示信号量在线程之间共享。
 * value：指定分配给新初始化的信号量的值。
 *
 * 获取信号量的值，获取 sem 的当前值并将其放置到valp 指向的位置
 * int sem_getvalue (sem_t *sem, int *valp);
 *
 * 锁定信号量或等待
 * int sem_wait(sem_t *sem);
 *
 * 释放或发出信号量信号
 * int sem_post(sem_t *sem);
 *
 * 要销毁信号量
 * sem_destroy(sem_t *sem);
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t m_sem;

void* thread(void* arg)
{
    (void)arg;
    /* wait */
    sem_wait(&m_sem);
    printf("\nEntered..\n");

    /* critical section */
    sleep(4);

    /* signal */
    printf("\nJust Exiting...\n");
    sem_post(&m_sem);

    return NULL;
}


int main()
{
    sem_init(&m_sem, 0, 1);
    pthread_t t1,t2;
    pthread_create(&t1,NULL,thread,NULL);
    sleep(2);
    pthread_create(&t2,NULL,thread,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&m_sem);

    return 0;
}


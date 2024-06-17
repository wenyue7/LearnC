/*************************************************************************
    > File Name: pipe.c
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: Wed 10 Apr 2024 03:00:12 PM CST
 ************************************************************************/

/*
 * 在管道中，当读取端尚未读取所有写入端的数据时，写入端会被阻塞（即等待）。
 * 反之亦然，当写入端尚未写入数据而读取端尝试读取时，读取端会被阻塞等待写入端写入
 * 数据。
 *
 * 在示例中，父进程在向管道写入数据后，如果子进程尚未读取这些数据，父进程将会被
 * 阻塞，直到子进程读取了数据。同样地，如果子进程尝试从管道中读取数据，而父进程
 * 尚未写入数据，子进程会被阻塞，直到父进程写入了数据。
 *
 * 这种阻塞行为确保了管道的同步性，即读取端和写入端在需要时会等待对方。
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main()
{
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // create subprocess
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // subprocess
        close(pipefd[1]);  // subprocess close pipe input

        // read from pipe
        read(pipefd[0], buffer, BUFFER_SIZE);
        printf("subprocess recv info: %s\n", buffer);

        close(pipefd[0]);  // close pipe output(read)
    } else { // father process
        close(pipefd[0]);  // close pipe output(read)

        // write info to pipe
        char *message = "Hello from father process!";
        write(pipefd[1], message, strlen(message) + 1);

        close(pipefd[1]);  // close pipe input
    }

    return 0;
}


/*
 * #include <sys/timerfd.h>
 * int timerfd_create(int clockid, int flags);
 * int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
 * int timerfd_gettime(int fd, struct itimerspec *curr_value);
 *
 * 这些系统调用创建并操作一个计时器，该计时器通过文件描述符提供计时器到期通知。它们提供了使用 setitimer(2) 或 timer_create(2) 的替代方法，其优点是可以通过 select(2)、poll(2) 和 epoll(7) 监视文件描述符。
 * 这三个系统调用的使用类似于 timer_create(2)、timer_settime(2) 和 timer_gettime(2) 的使用。 （没有 timer_getoverrun(2) 的类似物，因为该功能由 read 提供，如下所述。）
 *
 *
 * ======== int timerfd_create(int clockid, int flags);
 * 创建一个新的计时器对象，并返回引用该计时器的文件描述符。 clockid 参数指定用于标记计时器进度的时钟，并且必须是 CLOCK_REALTIME 或 CLOCK_MONOTONIC。 
 *      CLOCK_REALTIME  是可设置的系统范围时钟。  // 系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户改成其他,则对应的时间相应改变。
 *      CLOCK_MONOTONIC 是一个不可设置的时钟，不受系统时钟不连续变化的影响（例如，手动更改系统时间）。  // 从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
 * 可以使用 clock_gettime 从系统中检索每个时钟的当前值。
 *
 * 从 Linux 2.6.27 开始，可以在 flags 参数中对以下值进行按位 OR 运算以更改 timerfd_create() 的行为：
 * TFD_NONBLOCK  在新打开的文件描述上设置 O_NONBLOCK 文件状态标志。 使用此标志可以节省对 fcntl 的额外调用以达到相同的结果。
 * TFD_CLOEXEC   在新文件描述符上设置 close-on-exec (FD_CLOEXEC) 标志。 请参阅 open 中 O_CLOEXEC 标志的说明，了解这可能有用的原因。
 * 在 2.6.26 及包括 2.6.26 之前的 Linux 版本中，标志必须指定为零。
 *
 *
 *
 * ======== int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
 * timerfd_settime() 启用（启动）或解除（停止）文件描述符 fd 引用的定时器。
 * new_value 参数指定计时器的初始到期时间和间隔。 用于此参数的计时器结构包含两个字段，每个字段又是一个 timespec 类型的结构：
 *      struct timespec {
 *          time_t tv_sec;                // Seconds
 *          long   tv_nsec;               // Nanoseconds
 *      };
 *      struct itimerspec {
 *          struct timespec it_interval;  // Interval for periodic timer
 *          struct timespec it_value;     // Initial expiration
 *      };
 *
 * new_value:
 *   new_value.it_value 指定计时器的初始到期时间，以秒和纳秒为单位。 将 new_value.it_value 的任一字段设置为非零值，即可启动计时器。 将 new_value.it_value 的两个字段都设置为零会解除定时器。
 *   将 new_value.it_interval 的一个或两个字段设置为非零值指定初始到期后重复计时器到期的时间段（以秒和纳秒为单位）。 如果 new_value.it_interval 的两个字段都为零，则计时器仅在 new_value.it_value 指定的时间到期一次。
 * flags:
 *   flags 参数是 0，用于启动相对计时器（ new_value.it_value 指定相对于 clockid 指定的时钟的当前值的时间）
 *   flags 参数是 TFD_TIMER_ABSTIME，用于启动绝对计时器（ new_value.it_value 指定绝对时间由 clockid 指定的时钟；也就是说，当该时钟的值达到 new_value.it_value 中指定的值时，计时器将到期）。
 * old_value:
 *   如果 old_value 参数不为 NULL，则它指向的 itimerspec 结构用于返回调用时当前计时器的设置
 *
 *
 *
 * ======== int timerfd_gettime(int fd, struct itimerspec *curr_value);
 * timerfd_gettime() 在 curr_value 中返回一个 itimerspec 结构，其中包含文件描述符 fd 引用的计时器的当前设置。
 * it_value 字段返回计时器下一次到期之前的时间量。 如果此结构的两个字段都为零，则定时器当前已解除。 无论在设置计时器时是否指定了 TFD_TIMER_ABSTIME 标志，该字段始终包含一个相对值。
 * it_interval 字段返回定时器的间隔。 如果此结构的两个字段都为零，则计时器设置为仅在 curr_value.it_value 指定的时间到期一次。
 *
 * 用例功能说明：
 * 可执行文件文件可以指定一个或者三个参数，一个参数的情况下仅 new_value.it_value 起作用，即不会做周期循环，例如： ./a.out 3  仅等待 3s 
 * 三个参数的情况下，第二个参数是周期循环的时间，第三个参数是循环的次数，例如： ./a.out 3 1 10  首先等待 3s，然后以 1s 为周期，循环10次
 *
 */


#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Definition of uint64_t */

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


static void print_elapsed_time(void)
{
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;

    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
            handle_error("clock_gettime");
    }

    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
        handle_error("clock_gettime");

    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}


int main(int argc, char *argv[])
{
    struct itimerspec new_value;
    int max_exp, fd;
    struct timespec now;
    uint64_t exp, tot_exp;
    ssize_t s;

    if ((argc != 2) && (argc != 4)) {
        fprintf(stderr, "%s init-secs [interval-secs max-exp]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        handle_error("clock_gettime");

    /* Create a CLOCK_REALTIME absolute timer with initial
       expiration and interval as specified in command line */

    new_value.it_value.tv_sec = now.tv_sec + atoi(argv[1]);
    new_value.it_value.tv_nsec = now.tv_nsec;
    if (argc == 2) {
        new_value.it_interval.tv_sec = 0;
        max_exp = 1;
    } else {
        new_value.it_interval.tv_sec = atoi(argv[2]);
        max_exp = atoi(argv[3]);
    }
    new_value.it_interval.tv_nsec = 0;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        handle_error("timerfd_create");

    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
        handle_error("timerfd_settime");

    print_elapsed_time();
    printf("timer started\n");

    for (tot_exp = 0; tot_exp < max_exp;) {
        s = read(fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            handle_error("read");

        tot_exp += exp;
        print_elapsed_time();
        printf("read: %llu; total=%llu\n",
                (unsigned long long) exp,
                (unsigned long long) tot_exp);
    }

    exit(EXIT_SUCCESS);
}

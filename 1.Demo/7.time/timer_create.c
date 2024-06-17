/*
 * int timer_create(clockid_t clockid, struct sigevent *restrict sevp, timer_t *restrict timerid);
 * 创建一个新的每进程间隔计时器。计时器的 ID 在 timerid 指向的缓冲区中返回。此 ID 在进程中是独一无二的。新计时器最先被解除。
 *
 * clockid 参数指定新计时器用于测量时间的时钟。它可以指定为以下值之一：
 *  CLOCK_REALTIME
 *      可设置的系统范围实时时钟。
 *  CLOCK_MONOTONIC
 *      一个不可设置的单调递增时钟，它从过去的某个未指定点测量时间，系统启动后不会改变。
 *  CLOCK_PROCESS_CPUTIME_ID（自 Linux 2.6.12 起）
 *      测量（用户和系统）调用进程（中的所有线程）消耗的 CPU 时间的时钟。
 *  CLOCK_THREAD_CPUTIME_ID（自 Linux 2.6.12 起）
 *      测量（用户和系统）调用线程消耗的 CPU 时间的时钟。
 *  CLOCK_BOOTTIME（自 Linux 2.6.39 起）
 *      与 CLOCK_MONOTONIC 一样，这是一个单调递增的时钟。然而，虽然 CLOCK_MONOTONIC 时钟不测量系统挂起时的时间，但 CLOCK_BOOTTIME 时钟确实包括系统挂起的时间。
 *      这对于需要挂起感知的应用程序很有用。 CLOCK_REALTIME 不适用于此类应用程序，因为该时钟会受到系统时钟不连续更改的影响。
 *  CLOCK_REALTIME_ALARM（从 Linux 3.0 开始）
 *      这个时钟类似于 CLOCK_REALTIME，但如果它被挂起就会唤醒系统。调用者必须具有 CAP_WAKE_ALARM 功能才能针对此时钟设置计时器。
 *  CLOCK_BOOTTIME_ALARM（从 Linux 3.0 开始）
 *      这个时钟就像 CLOCK_BOOTTIME，但是如果它被挂起就会唤醒系统。调用者必须具有 CAP_WAKE_ALARM 功能才能针对此时钟设置计时器。
 *  CLOCK_TAI（从 Linux 3.10 开始）
 *      源自挂钟时间但忽略闰秒的系统范围时钟。
 * 有关上述时钟的更多详细信息，请参阅clock_getres。
 * 除了上述值，clockid 还可以指定为调用clock_getcpuclockid(3) 或 pthread_getcpuclockid(3) 返回的clockid。
 *
 *
 * sevp 参数指向一个 sigevent 结构，该结构指定在计时器到期时应如何通知调用者。有关此结构的定义和一般详细信息，请参阅 sigevent(7)。
 * sevp.sigev_notify 字段可以具有以下值：
 *  SIGEV_NONE
 *      不要在计时器到期时异步通知。可以使用 timer_gettime(2) 监视计时器的进度。
 *  SIGEV_SIGNAL
 *      计时器到期后，为进程生成信号 sigev_signo。有关一般详细信息，请参阅 sigevent(7)。 siginfo_t 结构的 si_code 字段将设置为 SI_TIMER。在任何时间点，对于给定的计时器，最多有一个信号排队到进程中；有关更多详细信息，请参阅 timer_getoverrun(2)。
 *  SIGEV_THREAD
 *      在计时器到期时，调用 sigev_notify_function 就好像它是新线程的启动函数一样。有关详细信息，请参阅 sigevent(7)。
 *  SIGEV_THREAD_ID（Linux 特定的）
 *      至于SIGEV_SIGNAL，但信号是针对sigev_notify_thread_id中给出ID的线程，该线程必须是与调用者在同一个进程中的线程。 sigev_notify_thread_id 字段指定内核线程 ID，即 clone(2) 或 gettid(2) 返回的值。此标志仅供线程库使用。
 * 将 sevp 指定为 NULL 相当于指定一个指向 sigevent 结构的指针，其中 sigev_notify 是 SIGEV_SIGNAL，sigev_signo 是 SIGALRM，而 sigev_value.sival_int 是计时器 ID。
 *  
 *  
 * 返回值： 成功时，timer_create() 返回 0，新计时器的 ID 放在 *timerid 中。 失败时，返回 -1，并设置 errno 以指示错误。
 *
 *
 * 注意：编译时需要连接库 -lrt
 *
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

static void print_siginfo(siginfo_t *si)
{
    timer_t *tidp;
    int or;

    tidp = si->si_value.sival_ptr;

    printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
    printf("    *sival_ptr = %#jx\n", (uintmax_t) *tidp);

    or = timer_getoverrun(*tidp);
    if (or == -1)
        errExit("timer_getoverrun");
    else
        printf("    overrun count = %d\n", or);
}

static void handler(int sig, siginfo_t *si, void *uc)
{
    /* Note: calling printf() from a signal handler is not safe
       (and should not be done in production programs), since
       printf() is not async-signal-safe; see signal-safety(7).
       Nevertheless, we use printf() here as a simple way of
       showing that the handler was called. */

    printf("Caught signal %d\n", sig);
    print_siginfo(si);
    signal(sig, SIG_IGN);
}

int main(int argc, char *argv[])
{
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    long long freq_nanosecs;
    sigset_t mask;
    struct sigaction sa;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <sleep-secs> <freq-nanosecs>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Establish handler for timer signal. */

    printf("Establishing handler for signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Block timer signal temporarily. */

    printf("Blocking signal %d\n", SIG);
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
        errExit("sigprocmask");

    /* Create the timer. */

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCKID, &sev, &timerid) == -1)
        errExit("timer_create");

    printf("timer ID is %#jx\n", (uintmax_t) timerid);

    /* Start the timer. */

    freq_nanosecs = atoll(argv[2]);
    its.it_value.tv_sec = freq_nanosecs / 1000000000;
    its.it_value.tv_nsec = freq_nanosecs % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1)
        errExit("timer_settime");

    /* Sleep for a while; meanwhile, the timer may expire
       multiple times. */

    printf("Sleeping for %d seconds\n", atoi(argv[1]));
    sleep(atoi(argv[1]));

    /* Unlock the timer signal, so that timer notification
       can be delivered. */

    printf("Unblocking signal %d\n", SIG);
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
        errExit("sigprocmask");

    exit(EXIT_SUCCESS);
}

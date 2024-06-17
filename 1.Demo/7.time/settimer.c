#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <time.h> 


//简单打印信息，定时器触发函数
void print_info(int signo){
    printf("timer fired\n");
}

void init_sigaction(){
    struct sigaction act;
    act.sa_handler = print_info;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask); 
    sigaction(SIGPROF,&act,NULL); //设置信号 SIGPROF 的处理函数为 print_info

}


void init_time() { 
    struct itimerval value; 
    value.it_value.tv_sec=2; //定时器启动后，每隔2秒将执行相应的函数
    value.it_value.tv_usec=0; 
    value.it_interval=value.it_value; 
    setitimer(ITIMER_PROF,&value,NULL); //初始化 timer，到期发送 SIGPROF 信号
} 

int main(int argc ,char *argv[]){

    init_sigaction(); 
    init_time(); 
    while(1); 

    return 0;
}

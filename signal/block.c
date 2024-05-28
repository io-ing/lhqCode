#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void interrupt_handler(int signum)
{
    write(1, "!", 1);
    signal(SIGINT, interrupt_handler);
} 

int main(int argc, char **argv)
{
    int i;
    sigset_t set, oset, saveset;

    signal(SIGINT, interrupt_handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    /* 保存之前的状态 */
    sigprocmask(SIG_UNBLOCK, &set, &saveset);
    while (1)
    {
        sigprocmask(SIG_BLOCK, &set, &oset);
        for (i=0; i<5; i++)
        {
            write(1, "*", 1);
            sleep(1);
        }
        write(1, "\n", 1);
//        sigprocmask(SIG_UNBLOCK, &set, NULL);
        sigprocmask(SIG_SETMASK, &oset, NULL);
    }

    /* 恢复之前的状态 */
    sigprocmask(SIG_SETMASK, &saveset, NULL);

    exit(0);
}

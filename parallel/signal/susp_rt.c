#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MYRTSIG (SIGRTMIN+6)
void mysig_handler(int signum)
{
    write(1, "!", 1);
    signal(MYRTSIG, mysig_handler);
} 

int main(int argc, char **argv)
{
    int i;
    sigset_t set, oset, saveset;

    signal(MYRTSIG, mysig_handler);
    sigemptyset(&set);
    sigaddset(&set, MYRTSIG);

    /* 保存之前的状态 */
    sigprocmask(SIG_UNBLOCK, &set, &saveset);
    sigprocmask(SIG_BLOCK, &set, &oset);
    while (1)
    {
        for (i=0; i<5; i++)
        {
            write(1, "*", 1);
            sleep(1);
        }
        write(1, "\n", 1);
        /* 相当于下面几步的原子操作 */
        sigsuspend(&oset);
/*
        sigset_t tmpset;
        sigprocmask(SIG_SETMASK, &oset, &tmpset);
        pause();
        sigprocmask(SIG_SETMASK, &tmpset, NULL);
*/
    }

    /* 恢复之前的状态 */
    sigprocmask(SIG_SETMASK, &saveset, NULL);

    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

static int loop = 1;

void alrm_handler(int signum)
{
    loop = 0;
}

int main(int argc, char **argv)
{
    int64_t count = 0;
    signal(SIGALRM, alrm_handler);
    alarm(5);

    while(loop)
        count++;

    printf("%ld\n", count);

    exit(0);
}

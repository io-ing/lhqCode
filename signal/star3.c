#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void interrupt_handler(int signum)
{
    write(1, "!", 1);
} 

int main(int argc, char **argv)
{
    int i;

    signal(SIGINT, interrupt_handler);

    for (i=0; i<10; i++)
    {
        write(1, "*", 1);
        sleep(1);
    }

    exit(0);
}

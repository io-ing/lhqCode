#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>

#define SHMSIZE 1024

int main()
{
    int shmid;
    pid_t pid;
    char *ptr;

    shmid = shmget(IPC_PRIVATE, SHMSIZE, 0666);
    if (shmid < 0)
    {
        perror("shmget()");
        exit(1);
    }


    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if (pid == 0)
    {
        ptr = shmat(shmid, NULL, 0);
        if (ptr == (void *)-1)
        {
            perror("shmat()");
            exit(1);
        }

        snprintf(ptr, sizeof("Hello!"), "Hello!");
        shmdt(ptr);
        exit(0);
    }
    else
    {
        wait(NULL);
        ptr = shmat(shmid, NULL, 0);
        if (ptr == (void *)-1)
        {
            perror("shmat()");
            exit(1);
        }
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }
}

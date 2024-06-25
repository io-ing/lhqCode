#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MEMSIZE 1024

int main()
{
    /*char *shared_memory; */
    char *ptr;
    pid_t pid;

    ptr = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        munmap(ptr, MEMSIZE);
        exit(1);
    }
    if (pid == 0)       // child write
    {
        strcpy(ptr, "Hello from child process!");
        munmap(ptr, MEMSIZE);
        exit(0);
    }
    else                // parent read
    {
        wait(NULL);
        puts(ptr);
        munmap(ptr, MEMSIZE);
        exit(0);
    }
}

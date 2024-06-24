#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static void func_add()
{
    FILE *fp;
    char linebuf[LINESIZE];
    int fd;

    fp = fopen(FNAME, "r+");
    if (!fp)
    {
        perror("fopen()");
        exit(1);
    }

    fd = fileno(fp);
    /* if error */

    lockf(fd, F_LOCK, 0);
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    sleep(1);
    fprintf(fp, "%d\n", atoi(linebuf)+1);
    /* 注意：关闭文件描述符会导致文件解锁，刷新缓冲区确保写成功 */
    fflush(fp);
    lockf(fd, F_ULOCK, 0);

    fclose(fp);

    return;
}

int main()
{
    int i;
    pid_t pid;

    for(i=0; i<PROCNUM; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        if (pid == 0)
        {
            func_add();
            exit(0);
        }
    }
    for(i=0; i<PROCNUM; i++)
        wait(NULL);

    exit(0);
}

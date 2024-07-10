#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sem.h>
#include <errno.h>

#define PROCNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static int semid;

static void P()
{
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    while (semop(semid, &sops, 1) < 0)
    {
        if (errno!=EAGAIN && errno!=EINTR)
        {
            perror("semop()");
            exit(1);
        }
    }
}

static void V()
{
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;

    while (semop(semid, &sops, 1) < 0)
    {
        if (errno!=EAGAIN && errno!=EINTR)
        {
            perror("semop()");
            exit(1);
        }
    }
}

static void func_add()
{
    FILE *fp;
    char linebuf[LINESIZE];

    fp = fopen(FNAME, "r+");
    if (!fp)
    {
        perror("fopen()");
        exit(1);
    }

    P();
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    sleep(1);
    fprintf(fp, "%d\n", atoi(linebuf)+1);
    /* 注意：关闭文件描述符会导致文件解锁，刷新缓冲区确保写成功 */
    fflush(fp);
    V();

    fclose(fp);

    return;
}

int main()
{
    int i;
    pid_t pid;

    /* 匿名 IPC 不需要 ftok() */
    semid = semget(IPC_PRIVATE, 1, 0666);
    if (semid < 0)
    {
        perror("semget()");
        exit(1);
    }

    if (semctl(semid, 0, SETVAL, 1) < 0)
    {
        perror("semctl()");
        exit(1);
    }

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

    semctl(semid, 0, IPC_RMID);

    exit(0);
}

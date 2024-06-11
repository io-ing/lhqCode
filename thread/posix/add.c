#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

void *thr_add(void *)
{
    FILE *fp;
    char linebuf[LINESIZE];

    fp = fopen(FNAME, "r+");
    if (!fp)
    {
        perror("fopen()");
        exit(1);
    }

    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    sleep(1);
    fprintf(fp, "%d\n", atoi(linebuf)+1);

    fclose(fp);
    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t tid[THRNUM];
    int err;

    for(i=0; i<THRNUM; i++)
    {
        err = pthread_create(tid+i, NULL, thr_add, NULL);
        if (err < 0)
        {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }
    for(i=0; i<THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }
    exit(1);
}

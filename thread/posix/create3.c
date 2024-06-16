#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static void *thr_func(void *arg)
{
    while (1)
        pause();

    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t tid;
    int err;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_setstacksize(&attr, 1024*1024);
    /* return val */

    for (i=0; ; i++)
    {
        pthread_create(&tid, &attr, thr_func, NULL);
        if (err < 0)
        {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            break;
        }
    }
    printf("i = %d\n", i);

    pthread_attr_destroy(&attr);

    exit(1);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM 4
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

static int next(int n)
{
    return (n+1) % 4;
}

void *thread_function(void *value)
{
    int n = (int)value;
    char ch = 'a' + n;

    while (1)
    {
        pthread_mutex_lock(&mut);
        while (n != num)
            pthread_cond_wait(&cond, &mut);
        write(1, &ch, 1);
        num = next(n);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}

int main()
{
    int i;
    pthread_t tid[THRNUM];
    int err;

    for (i=0; i<THRNUM; i++)
    {
        err = pthread_create(tid+i, NULL, thread_function, (void *)i);
        if (err < 0)
        {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }

    alarm(2);

    for (i=0; i<THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    exit(0);
}

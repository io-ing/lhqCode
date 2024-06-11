#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THRNUM 4
static pthread_mutex_t mut[THRNUM];

static int next(int n)
{
    return (n+1) % 4;
}

void *thread_function(void *value)
{
    int n = (int)value;
    char ch = 'a' + n;

    while(1)
    {
        pthread_mutex_lock(mut + n);
        write(1, &ch, 1);
        pthread_mutex_unlock(mut+next(n));
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
        pthread_mutex_init(mut+i, NULL);
        pthread_mutex_lock(mut+i);
        err = pthread_create(tid+i, NULL, thread_function, (void *)i);
        if (err < 0)
        {
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }

    pthread_mutex_unlock(mut + 0);

    alarm(2);

    for (i=0; i<THRNUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    exit(0);
}

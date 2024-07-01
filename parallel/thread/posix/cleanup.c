#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanup_function(void* arg)
{
    puts(arg);
}

void* thread_function(void* arg)
{
    puts("Thread is working!");

    pthread_cleanup_push(cleanup_function, "cleanup 1");
    pthread_cleanup_push(cleanup_function, "cleanup 2");

    puts("push over!");

    pthread_cleanup_pop(1); // Execute cleanup_function for "Resource 2"
    pthread_cleanup_pop(1); // Execute cleanup_function for "Resource 1"

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int err;

    puts("Begin!");
    err = pthread_create(&tid, NULL, thread_function, NULL);
    if (err != 0)
    {
        fprintf(stderr, "pthread_create(): %s\n", strerror(err));
        exit(1);
    }

    err = pthread_join(tid, NULL);
    if (err != 0)
    {
        fprintf(stderr, "pthread_join(): %s\n", strerror(err));
        exit(1);
    }
    puts("End!");

    return 0;
}

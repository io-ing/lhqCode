#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_function(void* arg)
{
    printf("Thread is running!\n");
    ptheard_exit(NULL);
}

int main()
{
    pthread_t tid;
    int err;

    puts("Begin!");
    err = pthread_create(&tid, NULL, thread_function, NULL);
    if (err != 0)
    {
        fprintf(stderr, "pthread_create():%s\n", strerror(err));
        exit(1);
    }

    puts("End!");
    /* 使用 pthread_join 等待线程结束，确保线程运行完成 */
    pthread_join(tid, NULL);

    exit(0);
}

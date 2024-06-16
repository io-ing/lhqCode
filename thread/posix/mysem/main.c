#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "mysem.h"

#define LEFT 3000000
#define RIGHT 3000200
#define THRNUM (RIGHT-LEFT-1)
#define N 4

static mysem_t *mysem;

int is_primer(int num)
{
    int i;

    for (i=2; i<num; ++i)
    {
        if (num % i == 0)
            return 0;
    }

    return 1;
}

void *thr_prime(void *p)
{
    int i = *(int *)p;

    if (is_primer(i))
    {
        printf("%d is primer\n", i);
    }

    sleep(5);

    mysem_add(mysem, 1);
    pthread_exit(NULL);
}

int main()
{
    int i;
    int err;
    pthread_t tid[THRNUM];

    mysem = mysem_init(N);
    if (mysem == NULL)
    {
        fprintf(stderr, "mysem_init() failed!\n");
        exit(1);
    }

    for (i=LEFT; i<RIGHT; ++i)
    {
        mysem_sub(mysem, 1);
        err = pthread_create(tid+i-LEFT, NULL, thr_prime, &i);
        if (err < 0)
        {
            /* 注意：标准的处理方法需要把之前创建的线程都释放 */
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }

    /* 如果不使用数组的话，tid 只会保存一份无法全部释放 */
    for (i=LEFT; i<RIGHT; i++)
        pthread_join(tid[i-LEFT], NULL);

    mysem_destroy(mysem);

    exit(0);
}

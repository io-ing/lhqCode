#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define LEFT 3000000
#define RIGHT 3000200
#define THRNUM 4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

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
    int i;

    while (1)
    {
        pthread_mutex_lock(&mut_num);
        while (num == 0)
        {
            pthread_cond_wait(&cond_num, &mut_num);
#if 0
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
#endif
        }
        if (num == -1)
        {
            pthread_mutex_unlock(&mut_num);
            break;
        }
        i = num;
        num = 0;
        pthread_cond_broadcast(&cond_num);
        pthread_mutex_unlock(&mut_num);

        if (is_primer(i))
        {
            printf("[%d]: %d is primer\n", (int)p, i);
        }
    }

    pthread_exit(NULL);
}

int main()
{
    int i;
    int err;
    pthread_t tid[THRNUM];

    for (i=0; i<THRNUM; ++i)
    {
        err = pthread_create(tid+i, NULL, thr_prime, (void *)i);
        if (err < 0)
        {
            /* 注意：标准的处理方法需要把之前创建的线程都释放 */
            fprintf(stderr, "pthread_create(): %s\n", strerror(err));
            exit(1);
        }
    }

    for (i=LEFT; i<RIGHT; i++)
    {
        pthread_mutex_lock(&mut_num);
        while (num != 0)
        {
            pthread_cond_wait(&cond_num, &mut_num);
#if 0
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
#endif
        }
        num = i;
        pthread_cond_signal(&cond_num);
        pthread_mutex_unlock(&mut_num);
    }

    pthread_mutex_lock(&mut_num);
    while (num != 0)
    {
        pthread_mutex_unlock(&mut_num);
        sched_yield();
        pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_cond_broadcast(&cond_num);
    pthread_mutex_unlock(&mut_num);

    /* 如果不使用数组的话，tid 只会保存一份无法全部释放 */
    for (i=0; i<THRNUM; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&mut_num);
    pthread_cond_destroy(&cond_num);
    exit(0);
}

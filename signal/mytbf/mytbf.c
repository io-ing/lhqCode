#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "mytbf.h"

static struct mytbf_st *job[MYTBF_MAX]; 
static int inited = 0;
typedef void (*sighandler_t)(int);
static sighandler_t alrm_handler_save; 

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
};

static void alrm_handler(int signum)
{
    int i;

    alarm(1);

    for (i=0; i<MYTBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }

}

/* 不能放在 destory 中调用，destory 是销毁一个令牌桶的 */
static void module_unload()
{
    int i;

    signal(SIGALRM, alrm_handler_save);
    alarm(0);

    /* 不需要判断，free 的对象是 vois * 不会出错 */
    for (i=0; i<MYTBF_MAX; i++)
        free(job[i]);
}

static void module_load()
{
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1); 

    atexit(module_unload);
}

static int get_free_pos()
{
    int i;

    for (i=0; i<MYTBF_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

mytbf_t *mytbf_init(int cps, int burst)
{
    struct mytbf_st *me;
    int pos;

    if (!inited)
    {
        module_load();
        inited = 1;
    }

    pos = get_free_pos();
    if (pos == -1)
        return NULL;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;

    me->cps = cps;
    me->burst = burst;
    me->token = 0;
    me->pos = pos;

    job[pos] = me;

    return me;
}

static int min(int a, int b)
{
    if (a<b)
        return a;
    return b;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;
    int n;

    if (size < 0)
        return -EINVAL;
    if (ptr == NULL)
        return -EINVAL;

    while (me->token <= 0)
        pause();

    n = min(me->token, size);
    me->token -= n;

    return n;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;

    if (size <= 0)
        return -EINVAL;
    if (ptr == NULL)
        return -EINVAL;

    me->token += size;

    if (me->token > me->burst)
        me->token = me->burst;

    return size;
}

int mytbf_destory(mytbf_t *ptr)
{
    struct mytbf_st *me = ptr;

    job[me->pos] = NULL;
    /* 也可以 free(ptf); */
    free(me);

    return 0;
}

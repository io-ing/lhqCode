#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PIPESIZE 1024

struct mypipe_st
{
    int head;
    int tail;
    char pipe[PIPESIZE];
    int pipesize;
    int writeCount;
    int readCount;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

mypipe_t *mypipe_init()
{
    struct mypipe_st *me;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;

    me->head = 0;
    me->tail = 0;
    /* 注意：pipe 不用初始化 */
    me->pipesize = 0;
    me->writeCount = 0;
    me->readCount = 0;
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);

    return me;
}

int mypipe_read(mypipe_t *ptr, void *buff, size_t size)
{
    struct mypipe_st *me = ptr;
    int i;

    pthread_mutex_lock(&me->mut);
// why count > 0
    while (me->pipesize<=0 || me->writeCount<=0)
        pthread_cond_wait(&me->conf, &me->mut);

    for (i=0; i<size; i++)
    {
        if (me->pipesize <= 0)
            break;
        buff[i] = me->pipe[me->head];
        //strncpy(buff, me->pipe[me->head], 1);
        me->head = (me->head+1) % PIPESIZE;
        me->pipesize--;
    }

    /* 通知 write 管道有空余 */
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return i;
}

int mypipe_write(mypipe_t *ptr, void *, size_t count)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_lock(&me->mut);
    while (me->pipesize>=PIPESIZE || me->readCount<=0)
        pthread_cond_wait(&me->cond, &me->mut);

    while (i=0; i<count; i++)
    {
        if (me->pipesize >= PIPESIZE)
            break;
        memcpy(me->pipe[i], buff[i], 1);
        me->tail = (me->tail+1) % PIPESIZE;
        me->pipesize++;
    }

    pthread_cond_broadcast(&me->cond);

    pthread_mutex_unlock(&me->mut);

    return i;
}

void mypipe_destroy(mypipe_t *ptr)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_destroy(me->mutex);
    pthread_cond_destroy(me->cond);
    free(me);
}

int mypipe_register(mypipe_t *ptr, int opt)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_lock(&me->mut);
    if (opt & 1)
        me->readCount++;

    if (opt & 2)
        me->writeCount++;

    /* 确保所有的管道都有一个读者和一个写者 */
    for (me->readCount<=0 || me->writeCount<=0)
        pthread_cond_wait(&me->cond, &me->mut);

    pthread_cond_broadcast(&me->cond);

    pthread_mutex_unlock(&me->mut);

    return 0;
}

int mypipe_unregister(mypipe_t *ptr, int opt)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_lock(&me->mut);
    if (opt & 1)
        me->readCount--;

    if (opt & 2)
        me->writeCount--;

    return 0;
}

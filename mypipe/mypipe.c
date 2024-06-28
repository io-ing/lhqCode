#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mypipe.h"

#define PIPESIZE 1024

struct mypipe_st
{
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize;
    int count_rd;
    int count_wr;
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
    /* 注意：data 不用初始化，data 是一个数组，把他初始化为什么都不算特殊 */
    me->datasize = 0;
    me->count_rd = 0;
    me->count_wr = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    return me;
}

static int next(int num)
{
    return (num+1) % PIPESIZE;
}

static int mypipe_readbyte_unlocked(struct mypipe_st *me, char *datap)
{
    *datap = me->data[me->head];
    me->head = next(me->head);
    me->datasize--;
    return 0;
}

int mypipe_read(mypipe_t *ptr, char *buff, size_t count)
{
    struct mypipe_st *me = ptr;
    int i;

    pthread_mutex_lock(&me->mut);
    /* 当管道中没有数据但是有读者才会等待，也就是说管道中的数据和写着发生改变才会继续 */
    while (me->datasize<=0 && me->count_wr>0)
        pthread_cond_wait(&me->cond, &me->mut);

    if (me->datasize<=0 && me->count_wr<=0)
    {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for (i=0; i<count; i++)
    {
        if (me->datasize <= 0)
            break;
#if 0
            mypipe_readbyte_unlocked(me, buf+1);
#else
        buff[i] = me->data[me->head];
        me->head = (me->head+1) % PIPESIZE;
        me->datasize--;
#endif
    }

    /* 通知 write 管道有空余 */
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return i;
}

int mypipe_write(mypipe_t *ptr, const char *buff, size_t count)
{
    struct mypipe_st *me = ptr;
    int i;

    pthread_mutex_lock(&me->mut);
    while (me->datasize>=PIPESIZE && me->count_rd>0)
        pthread_cond_wait(&me->cond, &me->mut);

    if (me->datasize>=PIPESIZE && me->count_rd<=0)
    {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for (i=0; i<count; i++)
    {
        if (me->datasize >= PIPESIZE)
            break;
        me->data[me->tail] = buff[i];
        me->tail = (me->tail+1) % PIPESIZE;
        me->datasize++;
    }

    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return i;
}

int mypipe_destroy(mypipe_t *ptr)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(me);

    return 0;
}

int mypipe_register(mypipe_t *ptr, int opmap)
{
    struct mypipe_st *me = ptr;

    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ)
        me->count_wr++;

    if (opmap & MYPIPE_WRITE)
        me->count_rd++;

    /* 确保所有的管道都有一个读者和一个写者 */
    while (me->count_wr<=0 || me->count_rd<=0)
        pthread_cond_wait(&me->cond, &me->mut);

    /* 读者/写者发信号告诉阻塞在这里的写者/读者，写在上面/下面都可以 */
    pthread_cond_broadcast(&me->cond);

    pthread_mutex_unlock(&me->mut);

    return 0;
}

int mypipe_unregister(mypipe_t *ptr, int opmap)
{
    struct mypipe_st *me = ptr;

    /* if error */

    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ)
        me->count_wr--;

    if (opmap & MYPIPE_WRITE)
        me->count_rd--;

    /* 可能有多个阻塞在 read 中，需要唤醒 */
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "anytimer.h"

static struct anytimer_st *job[JOB_MAX];
static int inited = 0;
typedef void (*sighandler_t)(int);
static sighandler_t alrm_handler_save;

typedef enum
{
    running,
    cancelled,
    completed,
    paused,
}running_status_e;

struct anytimer_st
{
    int sec;
    at_jobfunc_t *jobp;
    void *arg;
    running_status_e status;
};

static int find_free_pos()
{
    int i;

    for (i=0; i<JOB_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }

    return -1;
}

static void alrm_handler(int signum)
{
    int pos;

    signal(SIGALRM, alrm_handler);
    alarm(1);

    for (pos=0; pos<JOB_MAX; pos++)
    {
        if (job[pos] == NULL)
            continue;

        if (job[pos]->status == running)
        {
            job[pos]->sec--;
            if (job[pos]->sec <= 0)
            {
                job[pos]->jobp(job[pos]->arg);
                job[pos]->status = completed;
            }
        }
    }
}

static void module_unload()
{
    int pos;

    signal(SIGALRM, alrm_handler_save);
    alarm(0);

    for (pos=0; pos<JOB_MAX; pos++)
        free(job[pos]);
}

static void module_load()
{
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);

    atexit(module_unload);
}

int at_addjob(int sec, at_jobfunc_t *jobp, void *arg)
{
    struct anytimer_st *me;
    int pos;

    if (sec<=0 || jobp==NULL || arg==NULL)
        return -EINVAL;

    pos = find_free_pos();
    if (pos < 0)
        return -ENOSPC;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return -ENOMEM;

    if (!inited)
    {
        module_load();
        inited = 1;
    }

    me->sec = sec;
    me->jobp = jobp;
    me->arg = arg;
    me->status = running;

    job[pos] = me;

    return pos;
}

// 为什么要有wait操作：把异步操作同步化
// 任务不能自行消亡，为什么
// 既然任务不能自行消亡，那么凭什么遍历任务的时候不再看他，增加一个 flag
//     int status; // 任务状态: 0 - running, 1 - cancelled, 2 - completed
int at_canceljob(int id)
{
    struct anytimer_st *me;

    if (id<0 || id>JOB_MAX)
        return -EINVAL;

    me = job[id];
    if (me == NULL)
        return -EINVAL;

    if(me->status == completed)
        return -EBUSY;

    if(me->status == cancelled)
        return -ECANCELED;

    me->status = cancelled;
    return 0;
}

int at_waitjob(int id)
{
    struct anytimer_st *me;

    if (id<0 || id>JOB_MAX)
        return -EINVAL;

    me = job[id];
    free(me);
    job[id] = NULL;

    return 0;
}

int at_pausejob(int id)
{
    struct anytimer_st *me;

    if (id<0 || id>JOB_MAX)
        return -EINVAL;

    me = job[id];
    if (me == NULL)
        return -EINVAL;

    if(me->status == completed)
        return -EBUSY;

    if(me->status == cancelled)
        return -ECANCELED;

    if(me->status == paused)
        return -EAGAIN;

    me->status = paused;

    return 0;
}

int at_resumejob(int id)
{
    struct anytimer_st *me;

    if (id<0 || id>JOB_MAX)
        return -EINVAL;

    me = job[id];
    if (me == NULL)
        return -EINVAL;

    if(me->status == completed)
        return -EBUSY;

    if(me->status == cancelled)
        return -ECANCELED;

    if(me->status == paused)
        return -1;

    me->status = running;
}


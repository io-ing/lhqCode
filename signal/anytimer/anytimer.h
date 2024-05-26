#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#include <stdio.h>
#include <stdlib.h>

#define JOB_MAX 1024

typedef void at_jobfunc_t(void *);

int at_addjob(int sec, at_jobfunc_t *jobp, void *arg);
/*
 *  return  >= 0        成功，返回任务 ID
 *          == -EINVAl  失败，参数非法
 *          == -ENOSPC  失败，数组满
 *          == -ENOMEM  失败，内存空间不足
 **/

// 为什么要有wait操作：把异步操作同步化
// 任务不能自行消亡，为什么
// 既然任务不能自行消亡，那么凭什么遍历任务的时候不再看他，增加一个 flag
//     int status; // 任务状态: 0 - running, 1 - cancelled, 2 - completed
int at_canceljob(int id);
/*
 *  return  == 0        成功，指定任务成功取消
 *          == -EINVAl  失败，参数非法
 *          == -EBUSY   失败，指定任务已完成
 *          == -ECANCELED   失败，制定任务重复取消
 **/

int at_waitjob(int id);
/*
 *  return  == 0        成功，指定任务成功释放
 *          == -EINVAl  失败，参数非法
 **/

int at_pausejob();

int at_resumejob();
#endif

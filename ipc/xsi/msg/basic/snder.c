#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "proto.h"

int main()
{
    key_t key;
    int msgid;
    struct msgbuf data;

    key = ftok(PATHNAME, PROJID);
    if (key < 0)
    {
        perror("ftok()");
        exit(1);
    }

    msgid = msgget(key, 0);
    if (msgid < 0)
    {
        perror("msgget()");
        exit(1);
    }

    /* 不小于 0 即可 */
    data.mtype = 1;
    strncpy(data.student.name, "Alice", sizeof(data.student.name));
    data.student.chinese = rand() % 100;
    data.student.math = rand() % 100;

    if (msgsnd(msgid, &data, sizeof(data.student), 0) < 0)
    {
        perror("msgsend()");
        exit(1);
    }

    /* send 没有创建消息队列所以不用销毁 */
    exit(0);
}

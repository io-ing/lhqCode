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

    /* receive 需要先跑起来，需要 IPC_CREAT，send 方不用 IPC_CREAT */
    msgid = msgget(key, IPC_CREAT|0666);
    if (msgid < 0)
    {
        perror("msgget()");
        exit(1);
    }

    while (1)
    {
        /* 消息队列本质不是队列，第四个参数代表是否要挑消息来接收 */
        if (msgrcv(msgid, &data, sizeof(data.student), 0, 0) < 0)
        {
            perror("msgrcv()");
            exit(1);
        }
        printf("Received message: name: %s, chinese: %d, math: %d\n", data.student.name, data.student.chinese, data.student.math);
    }

    msgctl(msgid, IPC_RMID, NULL);

    exit(0);
}

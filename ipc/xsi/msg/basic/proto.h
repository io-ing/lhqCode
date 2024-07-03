#ifndef PROTO_H__
#define PROTO_H__

#define NAMESIZE 32
#define PATHNAME "/etc/services"
/* 字符转换到程序中是 ascii 码，一定是整型数，数字转换到程序中不能确定是什么内容，因为没有单位 */
#define PROJID 'A'

struct msgbuf
{
    long mtype;
    struct
    {
        char name[NAMESIZE];
        int chinese;
        int math;
    } student;
};

#endif

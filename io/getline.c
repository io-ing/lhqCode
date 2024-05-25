#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char *linebuf;
    size_t linesize;

    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }
    
    // 这两句话非常重要
    linebuf = NULL;
    linesize = 0;
    
    while (1)
    {
        // 也可以写成 if (getline(&linebuf, &linesize, fp) != 1)，但是有的人要求在程序中不能出现不能管理的分支
        if (getline(&linebuf, &linesize, fp) < 0)
            break;
        printf("%d\n", strlen(linebuf));    // 用 strlen() 而不是 sizeof()
        printf("%d\n", linesize);
    }

    fclose(fp);
    // 不建议 free，有时候不清楚对方用的什么函数申请的内存空间
    // free(linebuf);
    
    exit(0);
}

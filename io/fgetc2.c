#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int count = 0;

    if (argc < 2)
    {
        fprintf(stdout, "Usage ...\n");
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "popen failed! errno = %d\n", errno);
        exit(1);
    }

    while(fgetc(fp) != EOF)
    {
        count++;
    }

    printf("count == %d\n", count);
    fclose(fp);

    exit(0);
}

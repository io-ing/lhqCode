#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
    FILE *fp_source, *fp_dest;
    int ch;
    int n;
    char str[BUFFSIZE] = {0};

    if (argc < 3)
    {
        fprintf(stderr, "param num error!\n");
        exit(1);
    }

    fp_source = fopen(argv[1], "r");
    if(fp_source == NULL)
    {
        fprintf(stderr, "fopen() failed, errno = %d\n", errno);
        exit(1);
    }

    fp_dest = fopen(argv[2], "w");
    if(fp_source == NULL)
    {
        fprintf(stderr, "fopen() failed, errno = %d\n", errno);
        fclose(fp_source);
        exit(1);
    }

#if 0
    while(1)
    {
        ch = fgetc(fp_source);
        if (ch == EOF)
            break;
        fputc(ch, fp_dest);
    }

    while(1)
    {
        if(NULL == fgets(str, BUFFSIZE, fp_source))
            break;
        fputs(str, fp_dest);
    }
#endif

    while(1)
    {
        if ((n = fread(str, 1, BUFFSIZE, fp_source)) == 0)
        {
            break;
        }
        fwrite(str, 1, n, fp_dest);
    }

    fclose(fp_dest);
    fclose(fp_source);

    exit(0);
}

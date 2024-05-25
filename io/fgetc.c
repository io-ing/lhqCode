#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *fp_source, *fp_dest;
    int ch;

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

    while(1)
    {
        ch = fgetc(fp_source);
        if (ch == EOF)
            break;
        fputc(ch, fp_dest);
    }

    fclose(fp_dest);
    fclose(fp_source);

    exit(0);
}

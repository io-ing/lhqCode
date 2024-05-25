#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc < 2)
    {
        perror("usage...");
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen error!\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    printf("size == %ld\n", ftell(fp));

    fclose(fp);

    exit(0);    
}

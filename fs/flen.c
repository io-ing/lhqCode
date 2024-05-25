#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static off_t flen(const char *fname)
{
    struct stat statres;

    if (stat(fname, &statres) < 0)
    {
        perror("stat()");
        exit(1);
    }

    return statres.st_size;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    /*
     * 返回值是 off_t 类型，在某些体系结构中是 64 位
     * 有两种方法：做一个难看的强转，或者在 makefile 中定义 64 位
     */
    printf("%lld\n", (long long)flen(argv[1]));

    exit(0);
}

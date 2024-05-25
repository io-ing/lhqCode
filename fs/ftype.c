#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* char 类型没那么安全，不知道有没有符号，最好用 int 代替*/
static int ftype(const char *filepath)
{
    struct stat statres;

    if (stat(filepath, &statres) < 0)
    {
        perror("stat()");
        exit(1);
    }

    if (S_ISREG(statres.st_mode))
        return '-';
    else if (S_ISDIR(statres.st_mode))
            return 'd';
         else if(S_ISSOCK(statres.st_mode))
                return 's';
              else
                return '?';
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    printf("%c\n", ftype(argv[1]));

    exit(0);
}

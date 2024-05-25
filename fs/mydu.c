#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define PATHSIZE 1024

static int no_loop(const char *path)
{
    char *pos;
    pos = strrchr(path, '/');
    if (pos == NULL)
    {
        fprintf(stderr, "pos error\n");
        exit(1);
    }

    if (strcmp(pos+1, ".")==0 || strcmp(pos+1, "..")==0)
        return 0;

    return 1;
}

static int64_t mydu(const char *path)
{
    struct stat statres;
    DIR *dp;
    char fullpath[PATHSIZE];
    struct dirent *dirres;
    int64_t sum = 0;

    if (lstat(path, &statres) < 0)
    {
        perror("lstat()");
        exit(1);
    }

    if (!S_ISDIR(statres.st_mode))
        return statres.st_blocks;

    dp = opendir(path);
    if (dp == NULL)
    {
        perror("opendir()");
        return statres.st_blocks;
    }

    sum += statres.st_blocks;

    while((dirres = readdir(dp)))
    {
        snprintf(fullpath, PATHSIZE, "%s%s%s", path, "/", dirres->d_name);

        if (no_loop(fullpath))
        {
            sum += mydu(fullpath);
        }
    }

    closedir(dp);
    return sum;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    printf("%ld\n", mydu(argv[1])/2);

    exit(0);
}

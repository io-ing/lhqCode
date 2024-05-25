#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>

#define PAT "/etc/a*.conf"

static int errfunc_(const char *errpath, int eerrno)
{
    puts(errpath);
    fprintf(stderr, "ERROR MSG:%s\n", strerror(eerrno));

    return 0;
}

int main(int argc, char **argv)
{
    glob_t globres;
    int err;
    int i;

    err = glob(PAT, 0, errfunc_, &globres);
    if (err != 0)
    {
        fprintf(stderr, "Error code = %d\n", err);
    }

    for (i=0; globres.gl_pathv[i]!=NULL; i++)
    {
        puts(globres.gl_pathv[i]);
    }

    globfree(&globres);

    exit(0);
}

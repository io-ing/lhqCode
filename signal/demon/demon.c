#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>

#define FILENAME "/tmp/out"
static FILE *fp;
int demonize()
{
    pid_t pid;
    int fd;

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if (pid > 0)
    {
        exit(0);
    }

    fd = open("/dev/null", O_RDWR);
    if (fd < 0)
    {
        perror("open()");
        return -1;
    }

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);

    if (fd > 2)
        close(fd);

    setsid();

    chdir("/");

    umask(0);

    return 0;
}

void demon_exit(int signum)
{
    fclose(fp);
    closelog();
}

int main()
{
    int i;
    struct sigaction sa;

    sa.sa_handler = demon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
/*
    signal(SIGINT, demon_exit);
    signal(SIGQUIT, demon_exit);
    signal(SIGTERM, demon_exit);
*/

    openlog("mydemon", LOG_PID, LOG_DAEMON);

    if (demonize())
    {
        syslog(LOG_ERR, "demonize() failed!");
        exit(1);
    }
    else
    {
        syslog(LOG_INFO, "demonize() successded!");
    }

    fp = fopen(FILENAME, "a+");
    if (fp == NULL)
    {
        syslog(LOG_ERR, "fopen():%s", strerror(errno));
        exit(1);
    }

    syslog(LOG_INFO, "%s was opened.", FILENAME);

    for (i=0; ; ++i)
    {
        fprintf(fp, "%d\n", i);
        fflush(fp);
        syslog(LOG_DEBUG, "%d is printed.", i);
        sleep(1);
    }

    exit(0);
}

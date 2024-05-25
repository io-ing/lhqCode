#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static int fd1;
static int fd2;
static int fd100;

void close_fd1()
{
    close(fd1);
}

void close_fd2()
{
    close(fd2);
}

/* ... 更多的关闭函数 */

void close_fd100()
{
    close(fd100);
}

int main()
{
    fd1 = open("file1.txt", O_RDONLY);
    if (fd1 == -1)
    {
        perror("Error opening file1.txt");
        exit(1);
    }
    atexit(close_fd1);  /* 注册钩子函数，以关闭文件描述符 fd1 */

    fd2 = open("file2.txt", O_RDONLY);
    if (fd2 == -1)
    {
        /* 如果打开 file2.txt 失败，atexit 确保关闭 fd1，不需要在此处 close(fd1) */
        perror("Error opening file2.txt");
        exit(1);
    }
    atexit(close_fd2); /* 注册钩子函数，以关闭文件描述符 fd2 */

    /* ... 打开更多的文件并注册其关闭函数 */

    fd100 = open("file100.txt", O_RDONLY);
    if (fd100 == -1)
    {
        /* 如果打开 file100.txt 失败，atexit 确保关闭 fd1 到 fd99，不需要在此处 close(fd1)~close(fd99) */
        perror("Error opening file100.txt");
        exit(1);
    }
    atexit(close_fd100);

    return 0; /* 正常终止时，atexit 注册的所有函数将被调用 */
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define TIMESTRSIZE 256
#define FMTSTRSIZE 256

int main(int argc, char *argv[])
{
    FILE *fp = stdout;
    time_t stamp;
    struct tm *tm;
    char timestr[TIMESTRSIZE];
    int opt;
    char formatStr[FMTSTRSIZE] = {0}; 

    while(1)
    {
        opt = getopt(argc, argv, "-H:MSy:md");
        if (opt < 0)
        {
            break;
        }

        switch (opt)
        {
            case 1:
                // 先入为主，只写第一个文件中
                if (fp == stdout)
                {
                    fp = fopen(argv[optind-1], "w");
                    if (fp == NULL)
                    {
                        perror("fopen()");
                        fp = stdout;
                    }
                }
                break;
            case 'H':
                if (strcmp(optarg, "12") == 0)
                {
                    strncat(formatStr, "%I(%P) ", FMTSTRSIZE - strlen(formatStr) - 1);
                }
                else if (strcmp(optarg, "24") == 0)
                {
                    strncat(formatStr, "%H ", FMTSTRSIZE - strlen(formatStr) - 1);
                }
                else
                {
                    fprintf(stderr, "Invalid option: %c\n", opt);
                }
                break;
            case 'M':
                strncat(formatStr, "%M ", FMTSTRSIZE - strlen(formatStr) - 1);
                break;
            case 'S':
                strncat(formatStr, "%S ", FMTSTRSIZE - strlen(formatStr) - 1);
                break;
            case 'y':
                if (strcmp(optarg, "2") == 0)
                {
                    strncat(formatStr, "%y ", FMTSTRSIZE - strlen(formatStr) - 1);
                }
                else if (strcmp(optarg, "4") == 0)
                {
                    strncat(formatStr, "%Y ", FMTSTRSIZE - strlen(formatStr) - 1);
                }
                else
                {
                    fprintf(stderr, "Invalid option: %c\n", opt);
                }
//                strncat(formatStr, "%y ", FMTSTRSIZE - strlen(formatStr) - 1);
                break;
            case 'm':
                strncat(formatStr, "%m ", FMTSTRSIZE - strlen(formatStr) - 1);
                break;
            case 'd':
                strncat(formatStr, "%d ", FMTSTRSIZE - strlen(formatStr) - 1);
                break;
            default:
//                fprintf(stderr, "Unknown option: %c\n", opt);
                _exit(EXIT_FAILURE);
        }
    }

    strncat(formatStr, "\n", FMTSTRSIZE);

    // 使用构建好的 formatStr 格式化输出当前时间
    stamp = time(NULL);
    tm = localtime(&stamp);
    strftime(timestr, sizeof(timestr), formatStr, tm);
    fputs(timestr, fp);

    if (fp != stdout)
    {
        fclose(fp);
    }

    return 0;
}

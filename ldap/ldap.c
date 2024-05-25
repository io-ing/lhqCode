#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main() {
    FILE *fp;
    char command[256];
    char line[MAX_LINE_LENGTH];

    // 构建ldapsearch命令
    sprintf(command, "ldapsearch -x -LLL -b \"dc=example,dc=com\" \"(objectClass=*)\"");

    // 打开ldapsearch命令的输出流
    fp = popen(command, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to execute command\n");
        return 1;
    }

    // 逐行读取输出并解析
    while (fgets(line, sizeof(line), fp) != NULL) {
        // 解析行
        // 这里可以根据您的需求进行相应的解析操作，例如提取属性值等
        // 这里只是简单地打印每一行的内容
        printf("%s", line);
    }

    // 关闭输出流
    pclose(fp);

    return 0;
}

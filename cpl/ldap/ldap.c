#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char path[1035];

printf("lzltest fp == %d\n", fp);
printf("lzltest fp == NULL is %d\n", fp==NULL);
    // 执行LDAP搜索命令
    fp = popen("ldapsearch -P 3 -h 172.16.1.96 -p 389 -b \"ou=方位通讯,dc=fanvil,dc=com\" -s one -l 60 -LLL -N  -D \"cn=itadmin,cn=Users,dc=fanvil,dc=com\" -w 'fanvil@2022xxx'", "r");
printf("lzltest fp == %d\n", fp);
printf("lzltest fp == NULL is %d\n", fp==NULL);
    if (fp == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    // 读取搜索结果并输出
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }

    // 关闭管道
    pclose(fp);
    return 0;
}


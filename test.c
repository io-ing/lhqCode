#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int main() {
    const char *str = "aaa";
    char *endptr;
    long int value;

    errno = 0;  // 重置 errno
    value = strtol(str, &endptr, 10);

    // 检查是否发生错误
    if (errno == ERANGE) {
        if (value == LONG_MAX) {
            printf("Overflow occurred.\n");
        } else if (value == LONG_MIN) {
            printf("Underflow occurred.\n");
        }
    } else if (endptr == str) {
        printf("No digits were found.\n");
    } else if (*endptr != '\0') {
        printf("Further characters after number: %s\n", endptr);
    } else {
        printf("Conversion successful, value = %ld\n", value);
    }

    return 0;
}


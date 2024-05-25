#include <stdio.h>
#include <stdlib.h>
#include <shadow.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct spwd *shadowline;
    char *input_pass;
    char *crypted_pass;

    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    shadowline = getspnam(argv[1]);
printf("%s\n", shadowline->sp_pwdp);

    input_pass = getpass("password:");
printf("%s\n", input_pass);

    crypted_pass = crypt(input_pass, shadowline->sp_pwdp);

    if (strcmp(crypted_pass, shadowline->sp_pwdp) == 0)
        puts("OK");
    else
        puts("no OK");

    exit(0);
}

#include "apue.h"

static void charatatime(char *);

int main(int argc, char *argv[])
{
    pid_t pid;

    if((pid = fork()) < 0)
        printf("fork error");
    else if (pid == 0)
        charatatime("output from children");
    else
        charatatime("output from parent");
    return 0;
}

static void charatatime(char *str) {
    char *p;
    int c;
    setbuf(stdout,NULL);

    for (p = str; (c = *p++) != 0;) {
        putc(c, stdout);
    }
}

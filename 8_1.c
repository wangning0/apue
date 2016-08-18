#include "apue.h"

int globvar = 6;
char buf[] = "a write to stdout\n";

int main(int argc, char *argv[])
{
    int var;
    pid_t pid;
    var = 88;
    if(write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
        printf("write error");
    }
    printf("fork bufore");

    if((pid = fork()) < 0) {
        printf("fork error");
    } else if (pid == 0) {
        globvar++;
        var++;
    } else {
        sleep(2);
    }

    printf("pid = %ld,glob = %d, var = %d\n\n",(long)getpid(), globvar,var );
    return 0;
}

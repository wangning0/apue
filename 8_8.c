#include "apue.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    if((pid = fork()) < 0 )
        printf("fork error");
    else if(pid == 0){
        printf("%dak",pid);
        if((pid = fork()) < 0)
            printf("fork error 2");
        else if(pid > 0) {
        printf("%dqq",pid);
            exit(0);
        }
        sleep(2);

        printf("%dxx",pid);
        printf("second child, parent pid = %ld", (long)getppid());
    }

    if(waitpid(pid,NULL,0) != pid) {
        printf("waitpid error");
        printf("%d",pid);
    }
    return 0;
}

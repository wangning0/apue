#include "apue.h"
#include <sys/wait.h>

void pr_exit(int status) {
    if(WIFEXITED(status))
        printf("normal termination,exit status = %d",WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("abnormal termination,exit status = %d",WTERMSIG(status));
    else if(WIFSTOPPED(status))
        printf("child stopped, signal number = %d",WSTOPSIG(status));
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;



    if((pid = fork()) < 0)
        printf("fork error");
    else if(pid == 0)
        abort();

    if(wait(&status) != pid)
        printf("wait error");

    return 0;
}

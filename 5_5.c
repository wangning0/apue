#include "apue.h"

int main(int argc, char *argv[])
{
    char buf[MAXLINE];

    while(fgets(buf,MAXLINE,stdin) != NULL) {
        if(fputs(buf, stdout) == EOF) {
            printf("fputs error");
        }
    }
    return 0;
}

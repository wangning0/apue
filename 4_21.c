#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int i, fd;
    struct stat statbuf;
    struct timespec times[2];

    for (i = 0; i < argc; ++i) {
        if (stat(argv[i],&statbuf) < 0) {
            printf("stat error");
        }
        if((fd = open(argv[i],O_RDWR | O_TRUNC)) < 0) {
            printf("open error");
        }

        times[0] = statbuf.st_atime;
        times[1] = statbuf.st_mtime;

        if(futimens(fd,times) < 0) {
            printf("error");
        }
        close(fd);
    }
    return 0;
}

#include "apue.h"
#include <fcntl.h>

char buf2[] = "abcdefghij";
char buf1[] = "ABCDEFGHIJ";

int main(int argc, char *argv[])
{
    int fd;

    if((fd = creat("flie.hole", FILE_MODE)) < 0)
        printf("create error");

    if(write(fd, buf1, 10) != 10)
        printf("write error");

    if(lseek(fd, 16384, SEEK_SET) == -1)
        printf("seek error");

    if(write(fd, buf2, 10) != 10)
        printf("buf2 error");

    return 0;
}

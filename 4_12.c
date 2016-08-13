#include "apue.h"

int main(int argc, char *argv[])
{
    struct stat statbuf;

    if(stat("foo", &statbuf) < 0) {
        printf("stat error");
    }
    if(chmod("bar",(statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
        printf("chmod error");
    }

    if(chmod("foo", (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
        printf("chmod error");
    }
    return 0;
}

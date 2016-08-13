#include "apue.h"

int main(int argc, char *argv[])
{
    printf("uid = %d, gid = %d", getuid(), getgid());
    return 0;
}

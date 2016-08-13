#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, char *argv[])
{
    umask(0);//不做屏蔽位
    if(creat("foo", RWRWRW) < 0){
        printf("error");
    }
    umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); //屏蔽了组和其他成员的读写能力
    if(creat("bar", RWRWRW) < 0){
         printf("error");
     }
    return 0;
}
//-rw-------  1 wangning  staff  0  8 12 16:32 bar
//-rw-rw-rw-  1 wangning  staff  0  8 12 16:32 foo

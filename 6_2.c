#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
struct passwd* getpwnam(const char *name) {
    struct passwd *ptr;
    setpwent();
    while((ptr = getpwent()) != NULL) {
        printf("%s",ptr->pw_name);
        if(strcmp(name,ptr->pw_name) == 0)
            break;
    }
    endpwent();
    return ptr;
}

int main(int argc, char *argv[])
{
    struct passwd *a;
    a = getpwnam("wangning");
    printf("%s",a->pw_passwd);
    return 0;
}

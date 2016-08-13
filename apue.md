## 文件和目录
* 4个stat函数

        #include <sys/stat.h>
        int stat(const char *restrict pathname, struct stat *restrict buf);
        int fstat(int fd, struct stat *buf);
        int lstat(const char *restrict pathname, struct stat *restrict buf);
        int fstatat(int fd, const char *restrict pathname,struct stat *restrict buf, int flag);
        //lstat 当命名的文件是一个符号链接时，lstat返回该符号链接的有关信息
        //fstatat flag参数控制着是否跟随一个符号链接，AT_SYMLINK_NOFOLLOW标志被设置时，不会跟随符号链接，而回返回符号链接本身的信息
        
* stat结构

        struct stat {
            mode_t            st_mode; //file type S_ISREG()普通文件...
            ino_t             st_ino            ;  //i-node number
            dev_t             st_dev;  //device number
            dev_t             st_rdev; // device number for special files
            nlink_t           st_nlink;//number of links
            uid_t             st_uid;  // user id of owner
            gid_t             st_gid;  // group id of owner
            off_t             st_size; // size in bytes for regular files
            struct timespec   st_atime;// time of last access
            struct timespec   st_mtime;// time of last moditfication
            struct timespec   st_ctime;// time of last file status change
            blksize_t         st_blksize;// best I/O block size
            blkcnt_t          st_blocks;// number of disk blocks allocated
        }
        
        
* st_mode 

        S_ISREG() 普通文件
        S_ISDIR() 目录文件
        S_ISCHR() 自字符文件
        S_ISBLK() 块文件
        S_ISFIFO()FIFO
        S_ISLNK() 符号链接
        S_ISSOCK()套接字
        S_IRUSR 用户读
        S_IWUSR 用户写
        S_IXUSR 用户执行
        S_IRGRP 
        S_IWGRP
        S_IXGRP
        S_IROTH
        S_IWOTH
        S_IXOTH
        
* 新文件的用户ID设置为进程的有效用户ID，关于新文件的组ID，可能是进城的有效组ID，也可能是它锁在目录的组ID
* 一般情况下，有效用户ID等于实际用户ID，有效用户组ID等于实际用户组ID
* access和faccessat函数是按实际用户ID和实际组ID进行访问权限测试的

        #include <unistd.h>
        int access(const char *pathname, int mode);
        int faccessat(int fd, const char *pathname, int mode, int flag);//如果flag设置为AT_EACESS 则检查用的是有效用户ID，用户组ID
        //mode    说明
        //R_OK    测试读权限
        //W_OK    测试写权限
        //E_OK    测试执行权限
        
* 当用open函数打开一个文件时，内核是以进程的有效用户ID和有效组ID为基础执行其访问权限测试的
* 函数chmod fchmod fchmodat

        #include <sys/stat.h>
        int chmod(const char *pathname, mode_t mode);
        int fchmod(int fd, mode_t mode);
        int fchmodat(int fd, const char *pathname, mode_t mode, int flag);
        //chmod函数在指定的文件上进行操作
        //fchmod函数则对已打开的文件进行操作
        //当pathname为绝对路径或者是fd为AT_FDCWD而pathname参数为相对路径，flag参数设置了AT_SYMLINK_NOFOLLOW标志时，fchmodata并不会跟随符号链接
        
* 函数chown fchown fchownat lchown用于改变文件的用户id和组id

        #include <unistd.h>
        int chown(const char* pathname, uid_t owner, gid_t group);
        int fchown(int fd, uid_t owner, gid_t group);
        int fchownat(int fd, const char *pathname, uid_t owner, gid_t group, int flag);
        int lchown(const char* pathname, uid_t owner, gid_t group); 
        
* 文件截断

        #include <unistd.h>
        int truncate(const char *pathname, off_t length);
        int ftruncate(int fd, off_t length);
        //若该文件以前的长度大于length，则length以外的数据就不能再访问
        //如果以前的长度小于length，文件长度将增加，在以前的文件尾端和新的文件尾端之间的数据将读作0
        
* i节点是固定长度的记录项，它包含有关文件的大部分信息
* 每个i节点中都有一个链接计数，其值是指向该i节点的目录项数。只有当链接计数减少至0时，才可以删除该文件
* 另外一种链接类型称为符号链接。符号链接文件的实际内容包含了该符号链接所指向的文件的名字。该i节点中的文件类型是S_IFLINK，系统才知道这是一个符号链接
* i节点包含了文件有关的所有信息，文件类型 文件访问权限位 文件长度和指向文件数据块的指针。只有两项重要的数据项存放在目录项中：**文件名和i节点编号**
* 目录项中的i节点编号指向同一文件系统中的相应i节点，一个目录项不能指向另一个文件系统的i节点
* 当在不更换文件系统的情况下为一个文件重命名时，该文件的实际内容并未移动，只需构造一个指向现有i节点的新目录项，并删除老的目录项。链接计数不会改变
* 创建一个指向现有文件的链接的方法是是用link函数或linkat函数

        #include <unistd.h>
        int link(const char *exitingpath, const char *newpath);
        int linkat(int efd, const char *exitingpath, int nfd, const char *newpath, int flag);
  
* unlink函数 删除一个现有的目录项

        #include <unistd.h>
        int unlink(const char *pathname);
        int unlinkat(int fd, const char *pathname, int flag);//如果flag为AT_REMOVEDIR标识被设置时，unlinkat函数可以类似于rmdir一样删除目录
        
        //关闭一个文件时，内核首先检查打开该文件的进程个数，如果为0再去检查其链接数，如果也为0则删除文件的内容

* unlink的这种特性经常被程序用来确保即使是在程序崩溃时，他所创建的临时文件也不会遗留下来，进程用open或create创建一个文件，然后立即调用unlink，该文件依旧是打开的，所以不会将其把内容删除，只有当进程关闭或终止时，才会被删除
* remove函数接触对一个文件／目录的链接

        #include <stdio.h>
        int remove(const char *pathname);
        
* rename renameat函数给文件和目录重命名

        #include <stdio.h>
        int rename(const char *oldname, const char *newname);
        int renameat(int oldfd, const char *oldname, int newfd, const char *newname);
        
* symlink/symlinkat函数创建符号链接

        #inlcude <unistd.h>
        int symlink(const char *oldpathname, const char *newpathname);
        int symlinkat(const char *oldpath, int fd, const char *newpath));
        //函数创建了一个指向oldpath的新目录项newpath
        
* readlink/readlinkat函数读取符号链接

        #include <unistd.h>
        int readlink(const char *path, char *buf, size_t bufsize);
        int readlinkat(int fd, const char *path, char *buf, size_t bufsize);
        
* st_mtime和st_ctime 修改时间和状态修改时间的区别：前者时内容被修改的时间，后者是文件i节点被修改的时间
* 函数mkdir mkdirat rmdir 前两个创建目录 后一个删除目录

        #include<sys/stat.h>
        int mkdir(const char *pathname, mode_t mode)
        int mkdirat(int fd, const char *pathname, mode_t mode);
        
        #include <unistd.h>
        int rmdir(const char *pathname); 
        //如果调用此函数使目录的链接计数为0，并且没有其他进程打开此目录，则释放有此目录占用的空间
* 每个目录项是16个字节，其中14个字节是文件名，2个字节是i节点编号
* 进程通过chdir和fchdir函数可以更改当前工作目录

        #include <unistd.h>
        int chdir(const char *pathnname);
        int fchdir(int fd);
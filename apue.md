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
        
## 标准I／O 库
* 对于标准I/O库，它们的操作是围绕流进行的。
* 未定向的流上使用多字节I/O函数，则该流的定向设置为宽定向的，若使用了单字节I/O函数，则该流的定向为字节定向
* fwide函数用于设置流的定向

        #include<stdio.h>
        #include <wchar.h>
        int fwide(FILE *fp, int mode) //若流是宽定向，返回正值，若流是字节定向，返回负值，未定向返回0
        //mode为正 宽定向 mode为负 字节定向 0 未定向
        
* 3种类型的缓冲  

    * 全缓冲 填满标准I/O缓冲区后才进行实际的I/O操作
    * 行缓冲 遇到换行符时，执行I/O操作
    * 不带缓冲 不对字符进行缓冲存储
 
* 当且仅当标准输入和标准输出不指向交互式设备时，全缓冲的
* 标准错误绝不会是全缓冲的
* 若是指向终端设备的流，则是行缓冲的，否则时全缓冲的
* 更改缓冲类型

        #include <stdio.h>
        void setbuf(FILE *restrict fp, char *restrict buf);
        int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
        
* 强制冲洗一个流

        #include <stdio.h>
        omt fflush(FILE *fp);
        
* 打开流

        #include <stdio.h>
        FILE *fopen(const char *pathname, const char *type);
        FILE *freopen(const char *pathname, const char *type, FILE *fp);
        FILE *fdopen(int fd, const char *type);
        
* 关闭流

        #include <stdio.h>
        int fclose(FILE *fp);
        //在该文件被关闭之前，冲洗缓冲中的输出数据，缓冲区中的任何输入数据都被丢弃
        
* 输入函数

        #include <stdio.h>
        int getc(FILE *fp); //可作为宏
        int fgetc(FILE *fp); //不可作为宏
        int getchar(void); // getc(stdin);
        
        //从流中读取数据后，可以调用ungetc将字符再压送回流中
        int ungetc(intc, FILE *fp);
        //eg:
        #include<stdio.h>
        #include<ctype.h>
        int main()
        {
            int i=0;
            char ch;
            puts("Input an integer followed by a char:");
            // 读取字符直到遇到结束符或者非数字字符
            while((ch = getchar()) != EOF && isdigit(ch))
            {
                i = 10 * i + ch - 48;  // 转为整数
            }
            // 如果不是数字，则放回缓冲区
            if (ch != EOF)
            {
                ungetc(ch,stdin);  // 把一个字符退回输入流
            }
            printf("\n\ni = %d, next char in buffer = %c\n", i, getchar());
            system("pause");
            return 0;
        }
        
* 输出函数
        
        #include <stdio.h>
        int putc(int c. FILE *fp);
        int fputs(int c ,FIle *fp);
        int putchar(int c);
        
* 每次一行I/O


        #include <stdio.h>
        char *fgets(char *buf, int n, FILE *fp);
        char *gets(char *buf); //不推荐使用 会造成缓冲区的溢出
        //若成功返回buf，若已到达文件尾端或出错，返回NULL
        #include<string.h>
        #include<stdio.h>
         
        int main ( void )
        {
            FILE*stream;
            char string[]="Thisisatest";
            char msg[20];
        /*openafileforupdate*/
            stream=fopen("DUMMY.FIL","w+");
        /*writeastringintothefile*/
            fwrite(string,strlen(string),1,stream);
        /*seektothestartofthefile*/
            fseek(stream,0,SEEK_SET);
        /*readastringfromthefile*/
            fgets(msg,strlen(string)+1,stream);
        /*displaythestring*/
            printf("%s",msg);
            fclose(stream);
            return 0;
        }
        
        // fputs 和puts 提供每次输出一行的功能
        #include <stdio.h>
        int fputs(const char *str, FILE *fp);
        int puts(const char *str);
        
* 标准I/O库与直接调用read和write函数相比并不慢很多，对于大多数比较复杂的应用程序，最主要要的用户cpu时间是由应用本身的各种处理消耗的，而不是由标准I/O例程消耗的。
* 定位流 有3种方法定位标准I/O

        #include <stdio.h>
        long ftell(FILE *fp);
        int fseek(FILE *fp, long offset, int whence);
        void rewind(FILE *fp);
        
* 格式化输出

        #include <stdio.h>
        
        int printf(const char *restrict format,...);
        int fprintf(FILE *restrict fp, const char *restrict format,...);
        int dprintf(int fd, const char *restrict format);
        //若成功 则返回输出字符数，错误则返回错值
        int sprintf(char *restrict buf, const char *restrict format,...);
        int snprintf(char *restrict buf, size_t n, const *restrict format,...);
        //若缓冲区足够大，返回将要存入数组的字符数，若编码错误，返回负值
        
* 格式化输入

        #include <stdio.h>
        
        int scanf(const char *restrict format,...);
        int fscanf(FILE *restrict fp, const char *restrict format,...);
        int sscanf(const char *restrict buf, const char *restrict format,...);
       
## 系统数据文件盒信息
* passwd结构

        struct passwd {
            char *pw_name; //用户名
            char *pw_passwd; //加密口令
            uid_t pw_uid; //数值用户ID
            gid_t pw_gid; //数值组ID
            char *pw_gecos; //注释字段
            char *pw_dir; //初始化工作目录
            char *pw_shell; //初始化shell
            char *pw_class; //用户访问类
            time_t pw_change; //下次更改口令时间
            time_t pw_expire; //账户有效期时间
           }
           
* `finger -p username` 打印username的相关信息
* getpwnam

            #include <pwd.h>
            #include <stddef.h>
         17 #include <string.h>
         16 #include <stdio.h>
         15 struct passwd* getpwnam(const char *name) {
         14     struct passwd *ptr;
         13     setpwent();
         12     while((ptr = getpwent()) != NULL) {
         11         if(strcmp(name,ptr->pw_name) == 0)
         10             break;
          9     }
          8     endpwent();
          7     return ptr;
          6 }
          5
          4 int main(int argc, char *argv[])
          3 {
          2     struct passwd *a;
          1     a = getpwnam("wangning");
        20      printf("%s",a->pw_passwd);
          1     return 0;
          2 }
          
* 阴影口令，某些系统将加密口令存放在另一个通常称为阴影口令的文件中
* 例程是某个系统对外提供的功能接口和服务的集合
## 进程环境
* 当内核执行c程序时(使用一个exec函数)，在调用main前先调用一个特殊的启动例程。可执行程序文件将此启动例程指定为程序的起始地址，这是由连接编辑器设置的，而连接编辑器则由c编译器调用。启动例程丛内核取得命令行参数和环境变量值，然后为按上述方式调用main函数做好安排
* 函数名是程序执行中，函数所占内存单元块的首地址的别称
* 登记自动调用exit来终止的函数atexit

        #include <stdlib.h>
        int atexit(void(*func)(void));
        
* 栈是由操作系统自动分配释放，存放函数的参数值，局部变量的值等。其操作方式类似于数据结构中的栈，栈使用的是一级缓存，它们通常都是被调用时处于存储空间中，调用完毕立即释放，一种先进后出的数据结构
* 堆(os)一般由程序员分配释放，若程序员不释放，程序结束时可能由os回收，存放在二级缓存中，生命周期由虚拟机的垃圾回收算法来决定，可以看成是一棵树。
* 三个用于存储空间动态分配的函数

         #include <stdlib.h>
         void *malloc(size_t size);分配指定字节数的存储区
         void *calloc(size_t nobj, size_t size);为指定数量指定长度的对象分配存储空间，该空间的每一位都初始化为0
         void *realloc(void *ptr, size_t newsize);增加或减少以前分配区的长度，当增加长度时，可能需将以前分配区的内容移动到一个足够大的区域，以便在尾端提供增加的存储区，而新增区域内的初始值则不确定
         
         void free(vpid *ptr);
         
* 获取环境变量值

        #include <stdlib.h>
        char *getenv(const char *name);//返回name关联的value的指针
* 三个函数的原型

        #include <stdlib.h>
        int putenv(char *str);
        
        int setenv(const char *name, const char *value, int rewrite);
        int unsetenv(const char *name);
        
* id为0的进程通常是调度进程，常常被称为交换进程，不执行任何磁盘上的程序，所以也被称为系统进程。
* id为1的进程为init进程，在自举过程结束时由内核调用，它是一个普通的用户进程，它不是内核进程，但是它以超级用户特权运行
* 返回标识符的函数

        #include <unistd.h>
        pid_t getpid(void); //调用进程的进程ID
        pid_t getppid(void);//调用进程的父进程ID
        uid_t getuid(void);//调用进程的实际用户ID
        uid_t geteuid(void);//调用进程的有效用户ID
        gid_t getgid(void);//调用进程的实际组用户ID
        gid_t getegid(void);//调用进程的有效组ID
        
* 函数fork 一个现有的进程可以调用fork函数创建一个新进程

        #include <unistd.h>
        pid_t fork(void); 返回值，子进程返回0.，父亲进程返回子进程id，出错返回－1
* sizeof()函数计算包括终止null字节的缓冲区长度，sizeof是在编译时加计算缓冲区长度
* read函数和write函数是不带缓冲区的，标准I/O库是带缓冲的
* 函数vfork 的调用序列和返回值与fork相同，vfork函数用于创建一个新进程，而该新进程的目的是exec一个新程序，vfork会保证子进程先运行在它调用exec或exit后父进程才能被调度运行
* `_exit()`并不执行标准I/O缓冲区的冲洗操作，如果子进程关闭标准I/O流，那么表示标准输出FILE对象的相关存储区会清0，因为子进程借用了父进程的地址空间，所以父进程恢复运行并调用printf时，将不会产生任何输出
* 在任意一种情况下，该终止进程的父进程都能用wait或者waitpid函数取得其终止状态
* 对于父进程已经终止了的所有进程，他们的父进程都改变为init进程。我们称这些进程有init进程收养
* `僵死进程` 一个已经终止，但是其父进程尚未对其进行善后处理(获取终止子进程的有关信息、释放它仍占用的资源)的进程
* 当一个进程正常或异常终止时，内核就向其父进程发送`SIGCHLD`信号
* 函数wait和waitpid

        #include <sys/wait.h>
        pid_t wait(int *statloc);
        pid_t waitpid(pid_t pid, int *statloc, int options);
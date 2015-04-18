nclude <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

typedef struct
{
    char *fp_src;
    char *fp_dest;
    int fd_src;
    int fd_dest;
    int post;
    int len;
}arg_t;

void *Thread_one(void *arg_t)
{
    
}
void *Thread_two(void *arg_t)
{
        
}

void *Thread_Copy(int fd_src,int fd_dest,int post,int len)
{
    int n;
    int count = 0;
    char buf[1024];

    lseek(fd_src,post,SEEK_SET);
    lseek(fd_dest,post,SEEK_SET);

    while (n = read(fd_src,buf,sizeof(buf)))
    {
        write(fd_dest,buf,n);
        count += n;
        if (count >= len)
            break;
    }
    return;
}
int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr,"Usage : %s fd_src fd_dest\n",argv[0]);
        return -1;
    }
    int len;
    arg_t arg1,arg2;
    pid_t fd_src,fd_dest;
    pthread_t pth1,pth2;    

    fd_src = open(argv[1],O_RDONLY);
    fd_dest = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0666);

    len = lseek(fd_src,0,SEEK_END);
    lseek(fd_dest,0,SEEK_SET);

    arg1.fp_src = argv[1];
    arg1.fp_dest = argv[2];
    arg1.fd_src = fd_src;
    arg1.fd_dest = fd_dest;
    arg1.post = 0;
    arg1.len = len;

    arg2.fp_src = argv[1];
    arg2.fp_dest = argv[2];
    arg2.fd_src = fd_src;
    arg2.fd_dest = fd_dest;
    arg2.post = len/2;
    arg2.len = len;

    pth1 = pthread_create(&pth1,NULL,Thread_one,&arg1);
    if (pth1 != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    pth2 = pthread_create(&pth2,NULL,Thread_two,&arg2);
    if (pth2 != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    return 0;
}

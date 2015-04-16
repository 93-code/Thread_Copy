#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

typedef struct
{
    const char *fp_src;
    const char *fp_dest;
    int len;
}arg_t;

void *Thread_one(void *arg)
{
    arg_t *p;
    pid_t fd_src,fd_dest;
    p = (arg_t *)arg;
    fd_src = open(p->fp_src,O_RDONLY); 
    fd_dest = open(p->fp_dest,O_WRONLY|O_CREAT|O_TRUNC,0666);
    Thread_Copy(fd_src,fd_dest,0,p->len/2);
    close(fd_src);
    close(fd_dest);
}
void *Thread_two(void *arg)
{
    arg_t *p;
    pid_t fd_src,fd_dest;
    p = (arg_t *)arg;
    fd_src = open(p->fp_src,O_RDONLY); 
    fd_dest = open(p->fp_dest,O_WRONLY|O_CREAT|O_TRUNC,0666);
    Thread_Copy(fd_src,fd_dest,p->len/2,p->len/2);
    close(fd_src);
    close(fd_dest);
}

int Thread_Copy(int fd_src,int fd_dest,int post,int len)
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
    arg_t arg;
    pid_t fd_src;
    pthread_t pth1,pth2;    

    fd_src = open(argv[1],O_RDONLY);

    len = lseek(fd_src,0,SEEK_END);
    close(fd_src);

    arg.fp_src = argv[1];
    arg.fp_dest = argv[2];
    arg.len = len;

    pth1 = pthread_create(&pth1,NULL,Thread_one,&arg);
    if (pth1 != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    pth2 = pthread_create(&pth2,NULL,Thread_two,&arg);
    if (pth2 != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    return 0;
}

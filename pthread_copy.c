/* 13:09 2015-04-18 Saturday */
#include <head.h>

typedef struct
{
    int fd_src;
    int fd_dest;
    int post;
    int len;
}arg_t;

void *do_copy(void *arg)
{
    int n;
    int count = 0;
    char buf[1024];
    arg_t *p = (arg_t *)arg;

    lseek(p->fd_src,p->post,SEEK_SET);
    lseek(p->fd_dest,p->post,SEEK_SET);

    while ((n = read(p->fd_src,buf,sizeof(buf)))!=0)
    {
        write(p->fd_dest,buf,n);
        count += n;
         
        if (count > p->len)
            break;
    }
    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    int ret;
    struct stat f_info;
    pthread_t pth1,pth2; 
    arg_t arg1,arg2;
    if (argc < 3)
    {
        fprintf(stderr,"Usage : %s fd_src fd_dest\n",argv[0]);
        return -1;
    }

    arg1.fd_src = open(argv[1],O_RDONLY);
    if (arg1.fd_src < 0)
    {
        perror("Fail to open file");
        return -1;
    }
   
    if (fstat(arg1.fd_src,&f_info) != 0)
    {
        perror("Fail to fstat");
        return -1;
    }
    
    arg1.len = f_info.st_size/2;
    arg1.fd_dest = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0666);
    if (arg1.fd_dest < 0)
    {
        perror("Fail to open file");
        return -1;
    }

    arg1.post = 0;

    ret = pthread_create(&pth1,NULL,do_copy,&arg1);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    arg2.fd_src = open(argv[1],O_RDONLY);
    if (arg2.fd_src < 0)
    {
        perror("Fail to open file");
        return -1;
    }
   
    if (fstat(arg2.fd_src,&f_info) != 0)
    {
        perror("Fail to fstat");
        return -1;
    }
    
    arg2.len = f_info.st_size/2;
    arg2.fd_dest = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0666);
    if (arg2.fd_dest < 0)
    {
        perror("Fail to open file");
        return -1;
    }

    arg2.post = f_info.st_size/2;
    getchar();
    ret = pthread_create(&pth2,NULL,do_copy,&arg2);
    if (ret != 0)
    {
        perror("Fail to pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);

    return 0;
}

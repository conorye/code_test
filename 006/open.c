#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
/*
./open 1.txt
argc =2
argv[0]="./open"
argv[1]="1.txt"
*/


int main(int argc,char **argv)
{
    int fd;
    if(argc != 2)
    {
        printf("Usage: %s <file>\n",argv[0]);
        return 0;
    }
    fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC ,0644);

    if(fd<0)
    {
        printf("can not open file %s\n",argv[1]);
        printf("errno=%d\n",errno);
        printf("err:%s",strerror(errno));
        perror("open");

    }
    else {
        printf("fd:%d\n",fd);
        }

    while (1) 
    {
        sleep(10);
    
    }
    
    close(fd);
    return 1;
}
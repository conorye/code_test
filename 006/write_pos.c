#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*
./write 1.txt str1 str2
argc =2
argv[0]="./write"
argv[1]="1.txt"
*/

int main(int argc, char **argv) {
  int fd, fd2, len, i;
  if (argc < 3) {
    printf("Usage: %s <file> <<str1>  <<str2>\n", argv[0]);
    return 0;
  }
  fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
  fd2 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    printf("can not open file %s\n", argv[1]);
    printf("errno=%d\n", errno);
    printf("err:%s", strerror(errno));
    perror("open");

  } else {
    printf("fd:%d\n", fd);
    printf("fd:%d\n", fd2);
  }

  if (fd < argc) {

    for (i = 2; i < argc; i++) {
      len = write(fd, argv[i], strlen(argv[i]));
      // 移动到文件当前位置 + 5 字节
      off_t offset = lseek(fd, 2, SEEK_END);
      if (offset == -1) {
        perror("lseek");
        return 1;
      }
      if (len != strlen(argv[i])) {
        perror("write");
        break;
      }
      write(fd, "\r\n", 2);
    }
  }

  close(fd);
  return 1;
}
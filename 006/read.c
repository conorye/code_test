#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*
./read 1.txt
argc =2
argv[0]="./read "
argv[1]="1.txt"
*/

int main(int argc, char **argv) {
  int fd, len;
  unsigned char buf[100];
  if (argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 0;
  }
  fd = open(argv[1], O_RDONLY);

  if (fd < 0) {
    printf("can not open file %s\n", argv[1]);
    printf("errno=%d\n", errno);
    printf("err:%s", strerror(errno));
    perror("open");

  } else {
    printf("fd:%d\n", fd);
  }
  /*read file and print*/
  while (1) {
    len = read(fd, buf, sizeof(buf) - 1);
    if (len < 0) {
      perror("read");
      close(fd);
      return 1;

    } else if (len == 0) {
      break;

    } else {
      /*buf[0]...buf[len-1] include data from read */
      buf[len] = '\0';
      printf("read result:%s", buf);
    }
  }

  close(fd);
  return 1;
}
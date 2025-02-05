#include <complex.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

// 参数：原始字符串 str，起始索引 x，结束索引 y，结果存储在 result 中
void substring(const char *str, int x, int y, char *result) {
  if (x < 0 || y >= strlen(str) || x > y) { // 检查索引是否合法
    printf("Invalid range: x = %d, y = %d\n", x, y);
    result[0] = '\0'; // 返回空字符串
    return;
  }
  strncpy(result, &str[x], y - x + 1); // 复制子串
  result[y - x + 1] = '\0';            // 添加字符串结束符
}
/*
./test.c 123.csv 456.csv
argc =3
argv[0]="./test"
argv[1]="123.csv "
argv[2]="456.csv "
*/

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Usage: %s <file> <<str1>  <<str2>\n", argv[0]);
    return 0;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 0;
  }
  int fd2 = open(argv[2], O_APPEND | O_CREAT | O_RDONLY, 0644);
  if (fd2 == -1) {
    perror("open fd2");
    close(fd);
    return 1;
  } else {
    printf("fd2 opened successfully: %d\n", fd2);
  }

  char buffer[BUFFER_SIZE], line_buffer[BUFFER_SIZE];
  char evaluation[BUFFER_SIZE];
  int line_pos = 0, buf_start = 0; // 当前行的写入位置
  ssize_t bytes_read;
  int comma_cont = 0, score = 0, len = 0, line_count = 0;
  // printf("***test***");
  /*read(fd, buffer, sizeof(buffer));
  printf("***bytes_read***%zd\n",
         (bytes_read = read(fd, buffer, sizeof(buffer))));

  printf("***buffer_read:%s***\n\r***buffer_size:%lu***\r\n", buffer,
         sizeof(buffer));
  if (bytes_read < 0) {
    perror("read");
    close(fd);
    return 1;
  }
  */
  while ((bytes_read = read(fd, buffer, sizeof(buffer))) >
         0) // 使用 read 从文件中读取数据到 buffer
  {

    // 逐块读取文件内容
    buf_start = 0;
    for (ssize_t i = 0; i < bytes_read; i++) { // 逐个解析字母

      char c = buffer[i];

      if (c == ',')
        comma_cont += 1;

      if (comma_cont > 4) {
        comma_cont = 0;
        score = buffer[i - 1] - '0'; // 转换为整数值
      }

      if (score / 3 > 90)
        strcpy(evaluation, "优"); // evaluation="优";
      else if (score / 3 > 80)
        strcpy(evaluation, "中");
      else if (score / 3 > 0)
        strcpy(evaluation, "差");

      if (c == '\n') {

        if (line_count > 0) {
          char result[100]; // 存储子串的缓冲区
          // test
          // printf("i:%zd\n\rbuf_start:%d\n\r", i, buf_start);
          // printf("buffer:%s\n", buffer);
          // substring(buffer, buf_start, (int)i,
          //           result); // 调用子函数

          // if (result[0] != '\0') { // 检查返回的子串是否有效
          //   printf("Substring: %s\n", result);
          // } else {
          //   printf("No valid substring extracted.\n");
          // }

          memcpy(line_buffer, &buffer[buf_start], (int)i - buf_start);
          printf("line_buffer:%s\n", line_buffer);
          // strncat(line_buffer, evaluation,
          //         sizeof(line_buffer) - strlen(evaluation) - 1);

          snprintf(line_buffer + strlen(line_buffer),
                   sizeof(line_buffer) - strlen(line_buffer), "%s", evaluation);

          printf("evaluation:%s\n", evaluation);
          printf("line_buffer + evaluation:%s\n", line_buffer);
          len = write(fd2, line_buffer, strlen(line_buffer));
          if (len != strlen(line_buffer)) {
            perror("write");
            // break;
            continue;
          }
          write(fd2, "\r\n", 2);
        }
        line_count += 1;
        buf_start = i + 1;
      }
    }
    printf("done!\n");
    close(fd);
    close(fd2);
    return 1;
  }
}
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
  if (x < 0 || y >= strlen(str) || x > y) {
    printf("Invalid range: x = %d, y = %d\n", x, y);
    result[0] = '\0';
    return;
  }

  if (str[y] == '\n') {
    y--; // 如果y是换行符，减少y，避免换行符
  }

  strncpy(result, &str[x], y - x + 1);
  result[y - x + 1] = '\0';
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <input_file> <output_file>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open input file");
    return 1;
  }

  int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 == -1) {
    perror("open output file");
    close(fd);
    return 1;
  }

  char buffer[BUFFER_SIZE];
  char line_buffer[BUFFER_SIZE];
  char evaluation[BUFFER_SIZE];
  ssize_t bytes_read;
  int buf_start = 0;
  int comma_count = 0, line_count = 0;
  float score = 0;

  while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
    buf_start = 0;
    for (ssize_t i = 0; i < bytes_read; i++) {
      char c = buffer[i];

      if (c == ',') {
        comma_count++;
      }

      if (comma_count > 4) {
        comma_count = 0;
        score = buffer[i - 1] - '0';

        if (score / 3 > 90)
          strcpy(evaluation, "优");
        else if (score / 3 > 80)
          strcpy(evaluation, "中");
        else
          strcpy(evaluation, "差");
      }

      if (c == '\n') {
        if (line_count > 0) {
          memset(line_buffer, 0, sizeof(line_buffer)); // 清空缓冲区
          substring(buffer, buf_start, (int)i, line_buffer);

          if (line_buffer[0] != '\0') {
            printf("Substring: %s\n", line_buffer);
          }

          strncat(line_buffer, evaluation,
                  sizeof(line_buffer) - strlen(line_buffer) - 1);

          printf("line_buffer + evaluation: %s\n", line_buffer);

          if (write(fd2, line_buffer, strlen(line_buffer)) !=
              strlen(line_buffer)) {
            perror("write");
            continue;
          }

          write(fd2, "\n", 1);
        }

        line_count++;
        buf_start = i + 1;
      }
    }
  }

  if (bytes_read == -1) {
    perror("read");
  }

  close(fd);
  close(fd2);
  return 0;
}

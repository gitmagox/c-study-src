#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
  char buf[10];
  int len;
  while(1) {
    // STDIN_FILENO 是标准输入的描述符，它的值是 0. STDOUT_FILENO 是标准输出的描述符，它的值是 1.
    len = read(STDIN_FILENO, buf, 10);
    write(STDOUT_FILENO, buf, len);
  }

  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int cd(char* dir) {
  chdir(dir);
  if (errno) {
    printf("Error chdir %d: %s\n", errno, strerror(errno));
  }

  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  }
  if (errno) {
    printf("Error cwd %d: %s\n", errno, strerror(errno));
  }

  return 0;
}

int main() {
  /* cd("/home"); */
  cd("~");
  return 0;
}

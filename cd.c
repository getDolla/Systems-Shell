#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void cd(char* dir) {
  chdir(dir);
  if (errno) {
    printf("Error %d: %s\n", errno, strerror(errno));
  }
}

int main() {
  cd("/home");
  return 0;
}

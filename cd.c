#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Prints the current directory
void pwd(char* cur_dir) {
  getcwd(cur_dir, 1024);
  if (errno) {
    printf("Getcwd error: %s\n", strerror(errno));
  }
  printf("\n%s$ ", cur_dir);
}

void chdir_with_tilde(char* dir) {
  char home[1024];
  strcpy(home, getenv("HOME"));
  chdir(strcat(getenv("HOME"), dir + 1));
  if (errno) {
    printf("Error chdir with ~ (tilde): %s\n", strerror(errno));
  }
  strcpy(getenv("HOME"), home);
}

int cd(char* dir) {
  if (!(*dir)) {
    return 1;
  }

  if (*dir == '~') {
    chdir_with_tilde(dir);
  } else {
    chdir(dir);
  }
  if (errno) {
    printf("Error chdir: %s\n", strerror(errno));
  }

  return 0;
}

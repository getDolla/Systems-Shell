#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void print_working_dir() {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
  }
  if (errno) {
    printf("Error cwd %d: %s\n", errno, strerror(errno));
  }
}

void chdir_with_tilde(char* dir) {
  char home[1024];
  strcpy(home, getenv("HOME"));
  chdir(strcat(getenv("HOME"), dir + 1));
  if (errno) {
    printf("Error chdir with ~ (tilde) %d: %s\n", errno, strerror(errno));
  }
  strcpy(getenv("HOME"), home);
}

int cd(char* dir) {
  if (*dir == '~') {
    chdir_with_tilde(dir);
  } else {
    chdir(dir);
  }
  if (errno) {
    printf("Error chdir %d: %s\n", errno, strerror(errno));
  }

  print_working_dir();

  return 0;
}

/* int main() { */
/*   cd(".."); */
/*   cd("~/Desktop"); */
/*   cd("/"); */
/*   cd(".."); */
/*   cd("~/Desktop"); */
/*   cd("../Downloads"); */
/*   cd("~"); */
/*   return 0; */
/* } */

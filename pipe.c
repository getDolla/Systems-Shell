#include <stdio.h>
#include <stdlib.h>
/* Several system functions */
#include <unistd.h>
#include <sys/types.h>
/* For wait function */
#include <sys/wait.h>
/* For isspace function */
#include <ctype.h>
/* Custom header files */
#include "cd.h"
#include "shell.h"
/* Error handling */
#include <string.h>
#include <errno.h>

int main(){
  int p[2];
  pipe(p); // Creates a pipe with file descriptors Eg. input = 3 and output = 4 (Since, 0,1 and 2 are not available)

  if (fork() == 0) {
    // Child process
    close(1);
    dup(p[1]); // Create duplicate of fd - 3 (pipe read end) with fd 0.
    close(p[0]);
    execlp("ls", "ls", NULL);
  } else {
    //Parent process
    close(0);
    dup(p[0]);
    close(p[1]);
    execlp("grep", "grep", "shell.c", NULL);
  }
  return 0;
}

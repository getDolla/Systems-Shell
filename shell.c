#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void pwd(char* cur_dir) {
  getcwd(cur_dir, 1024);
  if (errno) {
    printf("Getcwd error: %s\n", strerror(errno));
  }
  printf("%s$ ", cur_dir);
}

void print_exit_status(int status) {
  if (WIFEXITED(status)) {
    printf("Exit status: %d\n", WEXITSTATUS(status));
    if (WEXITSTATUS(status)) {
      printf("Error: %s\n", strerror(WEXITSTATUS(status)));
    }
  }
}

int main(int argc, char *argv[]){
  printf("Press ctrl-c to exit.");
  char cur_dir[1024]; // stores current directory

  char command[1024]; // command buffer

  char *command_ptr = command; // for parsing by strsep
  char *args[10]; // stores arguments for execvp

  int status; // stores status of the child process
  pid_t pid; // stores the pid returned by fork

  // make shell persistent
  while (1) {
    pwd(cur_dir);

    fgets(command, sizeof(command), stdin); // gets input from stream

    if ( strcmp(command, "exit") != 0 && strncmp(command, "cd ", 3) != 0 ) {
      pid = fork();

      if (pid > 0) {
	wait(&status);
	print_exit_status(status);
      } else if (pid == 0) {
	if (command[strlen(command) - 1] == '\n') {
	  command[strlen(command) - 1] = '\0';
	}

	int i = 0;
	while (command_ptr) {
	  args[i] = strsep(&command_ptr, " "); // parses series of arguments
	  i++;
	}
	args[i] = NULL; // add terminating null - necessary

	execvp(args[0], args);

	exit(errno);
      } else if (pid == -1) {
	printf("Subprocess error: %s\n", strerror(errno));
      }

    }
  }

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
  printf("Press ctrl-c to stop the program. Exit not yet implemented\n");
  while (1) {
    // stores current directory
    char cur_dir[1024];
    getcwd(cur_dir, 1024);
    if (errno) {
      printf("Getcwd error: %s\n", strerror(errno));
    }
    printf("%s$ ", cur_dir);

    // for command
    char command[1024];

    // for strsep
    char *command_ptr = command;
    char *args[10];
    int status;
    pid_t pid = 1;

    fgets(command, sizeof(command), stdin); // gets input from stream
    if ( !(strncmp(command, "exit", 4) == 0 || strncmp(command, "cd", 2) == 0) ) {
      pid = fork();
      wait(&status);
    }

    if (pid == 0) {
      if (command[strlen(command) - 1] == '\n') {
	command[strlen(command) - 1] = '\0';
      }

      int i = 0;
      // printf("%s\n", command);//debug
      while(command_ptr){
	args[i] = strsep(&command_ptr, " "); // parses series of arguments
	// printf("%s\n", args[i]);//debug
	i++;
      }

      args[i] = NULL; // add terminating null - necessary
      execvp(args[0], args);
      if (errno) {
	printf("Execvp error: %s\n", strerror(errno));
      }
    } else if (pid == -1) {
      if (errno) {
	printf("Subprocess error: %s\n", strerror(errno));
      }
    }
  }
  return 0;
}

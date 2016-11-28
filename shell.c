#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "cd.h"
#include "shell.h"

/*===================================
MAIN SHELL
=====================================*/

int main(int argc, char *argv[]){
  printf("Press ctrl-c to exit.\n");
  char cur_dir[1024]; // stores current directory

  char command[1024]; // command buffer
  char *command_ptr = command; // pointer to command buffer
  char *args[10]; // stores arguments for execvp

  int status; // stores status of the child process
  pid_t pid; // stores the pid returned by fork

  // make shell persistent
  while (1) {
    errno = 0;

    pwd(cur_dir);

    get_command(command, sizeof(command));

    if ( strncmp(command, "cd ", 3) == 0 || strcmp(command, "cd") == 0) { // Check if command is cd
      if (*(command_ptr + 2)) {
	cd(command_ptr + 3);
      }
    } else if ( strcmp(command, "exit") != 0 ) { // check for other commands
      pid = fork();

      if (pid > 0) { // if process is the parent process
  	wait(&status);
  	print_exit_status(status);
      } else if (pid == 0) { // if process is child process
  	int i = 0;
  	while (command_ptr) {
  	  args[i] = strsep(&command_ptr, " "); // parses series of arguments
  	  i++;
  	}
  	args[i] = NULL; // add terminating null - necessary

  	execvp(args[0], args);

  	exit(errno);
      } else if (pid == -1) { // if subprocess failed
  	printf("Subprocess error: %s\n", strerror(errno));
      }
    }
  }

  return 0;
}

/*===================================
SHELL HELPER FUNCTIONS
=====================================*/

// Prints the current directory
void pwd(char* cur_dir) {
  getcwd(cur_dir, 1024);
  if (errno) {
    printf("Getcwd error: %s\n", strerror(errno));
  }
  printf("\n%s$ ", cur_dir);
}

// Trims whitespace, newlines, and tabs from strings
void trim(char * str) {
  // Return if string is empty
  if (!(*str)) {
    return;
  }

  // Trim leading space
  char* start = str;
  while(isspace( (int)(*start) )) {
    start++;
  }
  // All spaces?
  if( !(*start) ) {
    *str = '\0';
    return;
  }
  // Trim trailing space
  char* end = str + strlen(str) - 1;
  while (end > start && isspace( (int) (*end) )) {
    end--;
  }

  // Remove whitespace in original string
  for (; start <= end; start++) {
    *str = *start;
    // printf("%c", *str);
    str++;
  }

  // Write new null terminator
  *str = '\0';
}

// Gets the command from stdin
void get_command(char * command, int size) {
  fgets(command, size, stdin); // gets input from stream
  trim(command);
}

// Prints the exit status of the child process
void print_exit_status(int status) {
  if (WIFEXITED(status)) {
    printf("Exit status: %d\n", WEXITSTATUS(status));
    if (WEXITSTATUS(status)) {
      printf("Error: %s\n", strerror(WEXITSTATUS(status)));
    }
  }
}


/*===================================
SHELL TESTING
=====================================*/

/* int main() { */
/*   char test[] = " Test start "; */
/*   char test2[] = "\n \n \n \t \t \t Alright testing   "; */
/*   char test3[] = "\n \t\n\t \t ok test3 \n\n\n\n\n\t \t\t\t\t\t \n \t \t \t "; */
/*   char test4[] = ""; */
/*   char test5[] = "__regex"; */
/*   char test6[] = "            \n\n\n      Word.             "; */
/*   trim(test); */
/*   trim(test2); */
/*   trim(test3); */
/*   trim(test4); */
/*   trim(test5); */
/*   trim(test6); */

/*   printf("\n%s %s %s %s %s %s Test done!\n\n", test, test2, test3, test4, test5, test6); */
/*   printf("--%s--\n", test); */
/*   printf("--%s--\n", test2); */
/*   printf("--%s--\n", test3); */
/*   printf("--%s--\n", test4); */
/*   printf("--%s--\n", test5); */
/*   printf("--%s--\n", test6); */
/* } */

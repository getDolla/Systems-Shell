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

/*===================================
  MAIN SHELL
  =====================================*/

int main(int argc, char *argv[]) {
  char cur_dir[1024]; // stores current directory

  char command[1024]; // command buffer
  char *command_ptr = command; // pointer to command buffer

  char *array_of_commands[10]; // array of commands
  char *array_of_arguments[10]; // array of commands

  // make shell persistent
  while (1) {
    errno = 0;

    pwd(cur_dir);

    get_terminal_commands(command, sizeof(command));
    run_terminal_commands(command_ptr, array_of_commands, array_of_arguments);

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
int trim(char * str) {
  // Return if string is empty
  if (!(*str)) {
    return -1;
  }

  // Trim leading space
  char* start = str;
  while(isspace( (int)(*start) )) {
    start++;
  }
  // All spaces?
  if( !(*start) ) {
    *str = '\0';
    return -1;
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

  return 0;
}

void collapse(char str[], int i){ //use to remove whitespace in CD
    while(str[i] == ' ') {
      str[i] = str[i + 1];
      i++;
    }
}

// Parse based on semicolon
int parse(char* command_ptr, char** args) {
  int i = 0;
  while (command_ptr) {
    args[i] = strsep(&command_ptr, ";");
    printf("Parse results: %s\n", args[i]);
    i++;
  }
  return i;
}

// Gets the command from stdin
void get_terminal_commands(char *command_buffer, int size) {
  fgets(command_buffer, size, stdin); // gets input from stream
  trim(command_buffer);
}

int get_num_commands(char *command_ptr, char **array_of_commands) {
  // Find the number of commands
  if (strchr(command_ptr, ';')) { // Parse by semicolon
    return parse(command_ptr, array_of_commands);
  } else {
    printf("Found single command.\n");
    array_of_commands[0] = command_ptr;
    return 1;
  }
}

int run_terminal_commands(char *command_ptr, char **array_of_commands, char **array_of_arguments) {
  // If no input is found, return -1
  if (!(*command_ptr)) {
    return -1;
  }

  int num_commands = get_num_commands(command_ptr, array_of_commands);

  // For each command, run the command
  int i;
  for (i = 0; i < num_commands; i++) {
    check_command_type(array_of_commands[i], array_of_arguments);
  }
  return 0;
}

int check_command_type(char *command_ptr, char** array_of_arguments) {
  int pid;
  pid_t status;

  trim(command_ptr);

  if ( strncmp(command_ptr, "cd ", 3) == 0 ) {
    int i = 0;
    printf("%s\n", command_ptr);
    while (command_ptr[i] != '\0') { //gets rid of spaces between cd and the directory
      if (command_ptr[i] == ' ' && command_ptr[i + 1] == ' '){
        collapse(command_ptr, i);
      }
    i++;
    }
    printf("%s\n", command_ptr);
    cd(command_ptr + 3); // CD to the specified directory
  } else if ( strcmp(command_ptr, "exit") == 0 ) {
    exit(0); // Exit shell
  } else {
    pid = fork(); // Fork process

    if (pid > 0) { // if process is the parent process
      wait(&status);
      print_exit_status(status);
    } else if (pid == 0) { // if process is child process
      if (*command_ptr) {
	execvp_commands(command_ptr, array_of_arguments); // execute command
      }
      exit(errno); // exit
    } else if (pid < 0) { // if subprocess failed
      printf("Subprocess error: %s\n", strerror(errno));
    }
  }
  return 0;
}

// Execute command
void execvp_commands(char* command_ptr, char** args) {
  // Separate along spaces - individual command
  int i = 0;
  while (command_ptr) {
    args[i] = strsep(&command_ptr, " ");
    i++;
  }
  args[i] = NULL; // add terminating null - necessary

  execvp(args[0], args);
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

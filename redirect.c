#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "cd.h"
#include "shell.h"

char isStdout = 0;
char isStdin = 0;
char isSterr = 0; 
char isPipe = 0;

void chkrdrect( char * arg ) {
	if( !strcmp( arg, "2>" ) )
		isSterr = 1;
	else if( !strcmp( arg, ">" ) )
		isStdout = 1;
	else if( !strcmp( arg, "<" ) )
		isStdin = 1;
	else if( !strcmp( arg, "|" ) )
		isPipe = 1;		
}

int main(int argc, char const *argv[]) {
  char command[100]; //for command

  //for strsep
  char *p;
  char *args[10];

  printf("What would you like to do? \t");
  fgets(command, sizeof(command), stdin);//gets input from stream
  strtok(command, "\n");//gets rid of the \n at the end. will separate command into tokens if there is \n in the string

  int i = 0;

  p = command;
  printf("%s\n", p);//debug
  while(p){
    args[i] = strsep(&p, " ");
    printf("%s\n", args[i]);
    if( strcmp(args[i], ">") == 0 ) {
      isStdout = 1;
      break;
    }
    i++;
  }

  args[i] = NULL;//add terminating null in place of redirect symbol or just in case

  int fd = isStdout?open( p, O_CREAT | O_WRONLY ):1;

  if( isStdout ) {
    dup(1);
    dup2(fd, 1);
  }

  execvp(args[0], args);

  if( isStdout ) {
    dup2(fd+1, 1);
    close(fd);
  }

  return 0;
}

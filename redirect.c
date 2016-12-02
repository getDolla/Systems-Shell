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
char isApp = 0;
char isPipe = 0;

char chkrdrect( char * arg ) {
	if( !strcmp( arg, "2>" ) )
		isSterr = 1;
	else if( !strcmp( arg, "&>") )
		isSterr = isStdout = 1;
	else if( !strcmp( arg, ">>" ) )
		isApp = isStdout = 1;
	else if( !strcmp( arg, "2>>" ) )
		isApp = isSterr = 1;
	else if( !strcmp( arg, ">" ) )
		isStdout = 1;
	else if( !strcmp( arg, "<" ) )
		isStdin = 1;
	else if( !strcmp( arg, "|" ) )
		isPipe = 1;
	else
		return 0;

	return 1;
}

int dupFD( char* p ) {

	if( !(isStdout || isStdin || isSterr || isPipe) )
		return -1;

	/* For pipe */
	//
	//
	//

	int fd = isApp?open( p, O_CREAT | O_APPEND | O_WRONLY ):open( p, O_CREAT | O_WRONLY );

	if( isStdout ) {
    dup(1);
    dup2(fd, 1);
  }

	if( isSterr ) {
		dup(2);
    dup2(fd, 2);
	}

	if( isStdin ) {
		dup(0);
    dup2(fd, 0);
	}

	return fd;
}

void revertFD( int fd ) {
	if( !(isStdout || isStdin || isSterr || isPipe) )
		return;

	if( isStdout ) {
	  dup2(fd+1, 1);
		close(fd+1);
	}

	if( isSterr ) {
		dup2(fd+1, 2);
		close(fd+1);
	}

	if( isStdin ) {
		dup2(fd+1, 0);
		close(fd+1);
	}

	close(fd);
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
    if( chkrdrect(args[i]) )
      break;

    i++;
  }

  args[i] = NULL;//add terminating null in place of redirect symbol or just in case

  int fd = dupFD( p );

  execvp(args[0], args);

  revertFD( fd );

  return 0;
}

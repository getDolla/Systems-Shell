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

int stdOut = 0;
int stdIn = 0;
int stdErr = 0;
char isApp = 0;
char isPipe = 0;

char chkrdrect( char * arg ) {
	if( !strcmp( arg, "2>" ) )
		stdErr = 1;
	else if( !strcmp( arg, "&>") )
		stdErr = stdOut = 1;
	else if( !strcmp( arg, ">>" ) )
		isApp = stdOut = 1;
	else if( !strcmp( arg, "2>>" ) )
		isApp = stdErr = 1;
	else if( !strcmp( arg, ">" ) )
		stdOut = 1;
	else if( !strcmp( arg, "<" ) )
		stdIn = 1;
	else if( !strcmp( arg, "|" ) )
		isPipe = 1;
	else
		return 0;

	return 1;
}

int dupFD( char* p ) {

	if( !(stdOut || stdIn || stdErr || isPipe) )
		return -1;

	/* For pipe */
	//
	//
	//

	int fd = isApp?open( p, O_CREAT | O_APPEND | O_WRONLY ):open( p, O_CREAT | O_WRONLY );
	int copy = dup(fd);

	if( stdOut ) {
    stdOut = dup(1);
    dup2(copy, 1);
		copy = dup(fd);
  }

	if( stdErr ) {
		stdErr = dup(2);
    dup2(copy, 2);
		copy = dup(fd);
	}

	if( stdIn ) {
		stdIn = dup(0);
    dup2(copy, 0);
		copy = dup(fd);
	}
	close(copy);

	return fd;
}

void revertFD( int fd ) {
	if( fd == -1 )
		return;

	if( stdOut )
	  dup2(stdOut, 1);

	if( stdErr )
		dup2(stdErr, 2);

	if( stdIn )
		dup2(stdIn, 0);

	stdOut = stdErr = stdIn = 0;
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

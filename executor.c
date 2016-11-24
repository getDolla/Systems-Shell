#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  char command[1024]; //for command

  //for strsep
  char *p = command;
  char *args[10];

  printf("What would you like to do? ");
  fgets(command, sizeof(command), stdin);//gets input from stream
  strtok(command, "\n");//gets rid of the \n at the end. will separate command into tokens if there is \n in the string

  int i = 0;
  //printf("%s\n", command);//debug
  while(p){
    args[i] = strsep(&p, " ");//gets series of arguments
    //printf("%s\n", args[i]);//debug
    i++;
  }

  args[i] = NULL;//add terminating null just in case
  execvp(args[0], args);
  return 0;
}

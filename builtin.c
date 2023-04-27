#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include "builtin.h"
#include <string.h>
#include <dirent.h>

//Prototypes
static void exitProgram(char** args, int argcp);
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void cp(char** args, int argcp);
static void touch(char** args, int argcp);
static void ls(char** args, int argcp);


/* builtIn
 ** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
  */
int builtIn(char** args, int argcp)
{
    //write your code
    if(strcmp(args[0], "pwd") == 0) {
      return 1;
    }
    if(strcmp(args[0], "cd") == 0) {
      return 1;      
    }
    if(strcmp(args[0], "ls") == 0) {
      return 1;
    }
    if(strcmp(args[0], "exit") == 0) {
      return 1;
    }
    if(strcmp(args[0], "cp") == 0) {
      return 1;
    }
    if(strcmp(args[0], "touch") == 0) {
      return 1;
    }    
    // if no builtin
    return 0;
}

static void exitProgram(char** args, int argcp)
{
  //write your code
  int exit_code = 0;
  if (argcp > 1) {
    exit_code = (args[1] - '0');
  }
  exit(exit_code);
}

static void pwd(char** args, int argcp)
{
  //write your code

}


static void cd(char** args, int argcp)
{
 //write your code
}
/*
* Group A built in
*/
static void cp(char** args, int arcp)
{

}

static void ls(char** args, int arcp)
{

}

static void touch(char** args, int arcp)
{

}
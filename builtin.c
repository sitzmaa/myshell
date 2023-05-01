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
#include <pwd.h>

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
      pwd(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "cd") == 0) {
      cd(args, argcp);
      return 1;      
    }
    if(strcmp(args[0], "ls") == 0) {
      ls(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "exit") == 0) {
      exitProgram(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "cp") == 0) {
      cp(args,argcp);
      return 1;
    }
    if(strcmp(args[0], "touch") == 0) {
      touch(args,argcp);
      return 1;
    }    
    // if no builtin
    return 0;
}

static void exitProgram(char** args, int argcp)
{
  if (argcp > 2) {
    perror("Improper usage\nUsage: exit [exit_code]\n");
    exit(-1);
  }
  //write your code
  int exit_code = 0;
  if (argcp > 1) {
    exit_code = (*args[1] - '0');
  }
  exit(exit_code);
}

static void pwd(char** args, int argcp)
{
  if (argcp > 1) {
    perror("Improper usage\nUsage: pwd");
  }
  //write your code
  char* current_path = getcwd(NULL, 1028);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }

  printf("pwd: %s\n", current_path);
  free(current_path);
  exit(0);

}


static void cd(char** args, int argcp)
{
  if (argcp != 2) {
    perror("Improper usage\nUsage: cd [directory]");
  }
 //write your code
}


/*
* Group A built in functions
*/
// Copy a file
static void cp(char** args, int argcp)
{
  if (argcp != 3) {
    perror("Improper usage\nUsage: cp <src_file_name target_file_name>");
  }
  FILE* reader = fopen(args[1], "r"); // open file for reading
  FILE* writer = fopen(args[2], "w+"); // create new file with name args[2]
  char buffer[1000];
  while(fread(buffer, 1, 1000, reader)) {
    fwrite(buffer, 1, strlen(buffer), writer);
  }
}

/*
* List everything in current directory
* arg -l will create a verbose list
*/
static void ls(char** args, int argcp)
{
  if (argcp > 2 || argcp < 1) {
    perror("Improper usage\nUsage: ls [-l]");
  }
    char* current_path = getcwd(NULL, 1028);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }

  printf("pwd: %s\n", current_path);
  DIR* current_dir = opendir(current_path);
  if (current_dir == NULL) {
    perror("An error was encountered in opening directory");
    exit(-1);
  }
  struct dirent* entry;
  while ((entry = readdir(current_dir)) != NULL) {
    printf("%s", entry->d_name);
    if (strcmp(args[1], "-l") == 0) {
      printf("- %llu -- %hu", entry->d_ino, entry->d_reclen);
    }
    printf("\n");
  } 

  closedir(current_dir);
  free(current_path);
  exit(0);
}

static void touch(char** args, int argcp)
{
  if (argcp < 3) {
    perror("Improper usage\nUsage: touch <file1 or directory1...fileN or directoryN>");
  }
}
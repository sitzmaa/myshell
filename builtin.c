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
#include <time.h>

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

// Implemented - needs testing
static void exitProgram(char** args, int argcp)
{
  if (argcp > 2) {
    perror("Improper usage\nUsage: exit [exit_code]\n");
    return;
  }
  //write your code
  int exit_code = 0;
  if (argcp > 1) {
    exit_code = (*args[1] - '0');
  }
  exit(exit_code);
}

//implemented
static void pwd(char** args, int argcp)
{
  if (argcp > 1) {
    perror("Improper usage\nUsage: pwd");
    return;
  }
  //write your code
  char* current_path = getcwd(NULL, 1028);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }

  printf("pwd: %s\n", current_path);
  free(current_path);
  return;

}

// implemented
static void cd(char** args, int argcp)
{
  if (argcp > 2) {
    perror("Improper usage\nUsage: cd [directory]");
    return;
  }
 //write your code
  char* current_path = getcwd(NULL, 1028);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }
  
  if(argcp == 1) { // empty path
    chdir(getenv("HOME"));
    return;
  }
  strcat(current_path, "/");
  strcat(current_path, args[1]);
  printf("move to: %s\n", current_path);
  if (chdir(current_path) == -1) {
    perror("unable to move directory");
  }
  free(current_path);
  return;
}


/*
* Group A built in functions
*/
// Copy a file -- Implemnted
static void cp(char** args, int argcp)
{
  if (argcp != 3) {
    perror("Improper usage\nUsage: cp <src_file_name target_file_name>");
    return;
  }
  FILE* reader = fopen(args[1], "r"); // open file for reading
  FILE* writer = fopen(args[2], "w+"); // create new file with name args[2]
  char buffer[1000];
  while(fread(buffer, 1, 1000, reader)) {
    fwrite(buffer, 1, strlen(buffer), writer);
  }

  return;
}

/*
* List everything in current directory -- Implemented
* arg -l will create a verbose list -- WIP
*/
static void ls(char** args, int argcp)
{
  // error checks
  if (argcp > 2 || argcp < 1) {
    perror("Improper usage\nUsage: ls [-l]");
    return;
  }
    char* current_path = getcwd(NULL, 1028);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }
  
  DIR* current_dir = opendir(current_path);
  if (current_dir == NULL) {
    perror("An error was encountered in opening directory");
    exit(-1);
  }
  struct dirent* entry;
  entry = malloc(__offsetof(struct dirent, d_name) + 100);
  readdir(current_dir);
  readdir(current_dir);
  int i = 0;

  if(argcp > 1) {
    if (strcmp(args[1], "-l") == 0) {
      while ((entry = readdir(current_dir)) != NULL) {
      
        if(i%4 == 0) {
          printf("\n");
        }
        printf("%15s", entry->d_name);
        i++;
        printf("--serial: %llu -- mode: %hu", entry->d_ino, entry->d_reclen);
      } 
    }
  } else {
    while ((entry = readdir(current_dir)) != NULL) {
      
        if(i%4 == 0) {
          printf("\n");
        }
        printf("%15s", entry->d_name);
        i++;
    } 
  }
  printf("\n\n");

  free(entry);
  closedir(current_dir);
  free(current_path);
  return;
}

// Implemented
static void touch(char** args, int argcp)
{
  if (argcp < 2) {
    perror("Improper usage\nUsage: touch <file1 or directory1...fileN or directoryN>");
    return;
  }
  for (int i = 1; i < argcp; i++) {
    FILE* file = fopen(args[i], "r");
    if(file == NULL) {
      file = fopen(args[1], "w");
    }
    fclose(file);
  }
}
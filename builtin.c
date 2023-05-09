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
#include <grp.h>
#include <time.h>
#include <utime.h>

//Prototypes
static void exitProgram(char** args, int argcp);
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void cp(char** args, int argcp);
static void touch(char** args, int argcp);
static void ls(char** args, int argcp);
static char* permissions(mode_t mode);
#define MAX_PATH_LEN 1016 // define it here since Systems have different max paths 1016 for mac 260 for windows
#define TERMINAL_WIDTH 80 // define standard terminal width


/* builtIn
 ** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
  */
int builtIn(char** args, int argcp)
{
    /*
    * String comparisons to determine if function is built in
    * if so then has a second check to ensure that the number of arguements is correct
    */
    if(strcmp(args[0], "pwd") == 0) { // %myshell% pwd
      if (argcp > 1) {
        perror("Improper usage\nUsage: pwd");
        return 1;
      }
      pwd(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "cd") == 0) { // %myshell% cd [directory]
      if (argcp > 2) {
        perror("Improper usage\nUsage: cd [directory]");
        return 1;
      }
      cd(args, argcp);
      return 1;      
    }
    if(strcmp(args[0], "ls") == 0) { // %myshell% ls [-l]
      if (argcp > 2) {
        perror("Improper usage\nUsage: ls [-l]");
        return 1;
      }
      ls(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "exit") == 0) { // %myshell% exit
      if (argcp > 2) {
        perror("Improper usage\nUsage: exit [exit_code]\n");
        return 1;
      }
      exitProgram(args, argcp);
      return 1;
    }
    if(strcmp(args[0], "cp") == 0) { // %myshell% cp origin destination
      if (argcp != 3) {
        perror("Improper usage\nUsage: cp <src_file_name target_file_name>");
        return 1;
      }
      cp(args,argcp);
      return 1;
    }
    if(strcmp(args[0], "touch") == 0) { // %myshell% touch <file1, file 2 ... filen>
      if (argcp < 2) {
        perror("Improper usage\nUsage: touch <file1 or directory1...fileN or directoryN>");
        return 1;
      }
      touch(args,argcp);
      return 1;
    }    
    // if no builtin
    return 0;
}

// Implemented
/*
* Exits program with specified exit code
* Exit with code 0 otherwise
*/
static void exitProgram(char** args, int argcp)
{
  
  //write your code
  int exit_code = 0;
  if (argcp > 1) {
    exit_code = (*args[1] - '0');
  }
  exit(exit_code);
}


/* Implemented
* Displays current directory, does not take aditional modes
* Frees string created by getcwd before return
*/
static void pwd(char** args, int argcp)
{
  char* current_path = getcwd(NULL, MAX_PATH_LEN);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }

  printf(" %s\n", current_path);
  free(current_path);
  return;
}

// implemented
static void cd(char** args, int argcp)
{
  char* current_path = getcwd(NULL, MAX_PATH_LEN);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }
  
  if(argcp == 1) { // empty path
    chdir(getenv("HOME"));
    return;
  }
  // create path to destination
  strcat(current_path, "/");
  strcat(current_path, args[1]);
  if (chdir(current_path) == -1) {
    perror("unable to move directory");
  }
  free(current_path);
  return;
}


/*
* Group A built in functions
*/

/* Copy a file -- Implemnted
* Error checks on open
*/
static void cp(char** args, int argcp)
{

  FILE* reader = fopen(args[1], "r"); // open file for reading
  if (reader == NULL) {
    perror("File failed to open");
    return;
  }
  FILE* writer = fopen(args[2], "w+"); // create new file with name args[2]
  if (writer == NULL) {
    perror("File failed to be created");
  }
  char buffer[1000];
  size_t size;
  while((size = fread(buffer, 1, 1000, reader)) != 0) {
    fwrite(buffer, 1, size, writer);
  }

  // set permissions of new file to old
  struct stat* entry_stat;
  entry_stat = malloc(sizeof(struct stat));
  if (stat(args[1], entry_stat) < 0) {
    perror("stat error");
  }
  if(chmod(args[2],entry_stat->st_mode)<0) {
    perror("chmod failed");
  }
  free(entry_stat);

  // Flush buffer and close files
  fflush(writer);
  fclose(reader);
  fclose(writer);
  return;
}

/*
* List everything in current directory -- Implemented
* arg -l will create a verbose list -- Implemented
*/
static void ls(char** args, int argcp)
{
  // error checks
    char* current_path = getcwd(NULL, MAX_PATH_LEN);
  if (current_path == NULL) {
    perror("Failed to retrieve current directory path");
    exit(-1);
  }
  // open directory
  DIR* current_dir = opendir(current_path);
  if (current_dir == NULL) {
    perror("An error was encountered in opening directory");
    exit(-1);
  }
  // get entry in directory
  struct dirent* entry;
  entry = malloc(__offsetof(struct dirent, d_name) + 100);
  readdir(current_dir);
  readdir(current_dir);
  int i = 0;
  if(argcp > 1) {
    if (strcmp(args[1], "-l") == 0) {

      // Find the file with the largest number of links and set the 
      // padding for the format string to that value
      int largest_link = 1;
      while ((entry = readdir(current_dir)) != NULL) {
        int current_largest = 1;
        struct stat* entry_stat;
        entry_stat = malloc(sizeof(struct stat));
        if (stat(entry->d_name, entry_stat) < 0) {
          perror("stat error");
        }
        int nlinks = entry_stat->st_nlink;
        while (nlinks > 10) {
          nlinks = nlinks/10;
          current_largest++;
        }
        if (current_largest > largest_link) {
          largest_link = current_largest;
        }
        free(entry_stat);
      }

      // go back to top
      rewinddir(current_dir);
      readdir(current_dir);
      readdir(current_dir);
      while ((entry = readdir(current_dir)) != NULL) {
        
        i++;
        struct stat* entry_stat;
        struct passwd *user;
        struct group *grp;
        // get stat
        entry_stat = malloc(sizeof(struct stat));
        if (stat(entry->d_name, entry_stat) < 0) {
          perror("stat error");
        }
        // get user and grp information
        user = getpwuid(entry_stat->st_uid);
        grp = getgrgid(entry_stat->st_gid);
        // get permissions
        char* mode_string = permissions(entry_stat->st_mode);
        // get access time
        char* time_string = malloc(80);
        struct tm* time = localtime(&entry_stat->st_atimespec.tv_sec);
        strftime(time_string, 80, "%c", time);
        // format string
        printf("%s@ %*hu %s %s %6lld %15s %s\n", mode_string, 
          largest_link, entry_stat->st_nlink, 
          user->pw_name, 
          grp->gr_name, 
          entry_stat->st_size,
          time_string, 
          entry->d_name);
        free(mode_string);
        free(entry_stat);
        free(time_string);
      } 
    }
  } else {
    // determine the largest filename in the directory
    int largest_name = 0;
    while ((entry = readdir(current_dir)) != NULL) {
      if (strlen(entry->d_name) > largest_name) {
        largest_name = strlen(entry->d_name);
      }
    }
    // create the name padding = largest name + extra padding
    largest_name+=5;
    // go back to top
    rewinddir(current_dir);
    readdir(current_dir);
    readdir(current_dir);
    // set the number of files per line
    int num = TERMINAL_WIDTH/largest_name;
    while ((entry = readdir(current_dir)) != NULL) {
        // every num entries newline
        if(i%num == 0 && i != 0) {
          printf("\n");
        }
        // print the file name of the entry
        printf("%*s", largest_name, entry->d_name);
        i++;
    } 
    printf("\n");
  }

  free(entry);
  closedir(current_dir);
  free(current_path);
  return;
}

/* Implemented
* Update time or create file
* takes many arguements
*/
static void touch(char** args, int argcp)
{
  // Create file if does not exist, then set access time to current time
  for (int i = 1; i < argcp; i++) {
    FILE* fp = fopen(args[i], "a");
    fclose(fp);
    struct utimbuf *utm = malloc(sizeof(utime));
    utm->actime = time(0);
    utm->modtime = time(0);
    utime(args[i], utm);
    free(utm);
  }
}

// Helper function for file permissions
static char* permissions(mode_t mode) {
  /*
  * convert mode from number format into string format
  */
  char output[10];
  char* permission_string = "rwxrwxrwx";
  for (int i = 0; i < 9; i++) {
    if ((mode & (1 << (8-i)))) {
      output[i] = permission_string[i];
    } else {
      output[i] = '-';
    }
  }
  output[9] = '\0';
  char* returner = malloc(10);
  strcpy(returner,output);
  return returner;
} 
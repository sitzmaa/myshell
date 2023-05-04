/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "argparse.h"
#include "builtin.h"


/* PROTOTYPES */

void processline (char *line);
ssize_t getinput(char** line, size_t* size);
// helper functions
void freeargs(char** args, int count);

/* main
 * This function is the main entry point to the program.  This is essentially
 * the primary read-eval-print loop of the command interpreter.
 */

int main () {


  /*
  * Start main shell
  */
  char* line;
  size_t size = 0;

  while(1) {
    getinput(&line, &size);
    processline(line);
    printf("command executed\n"); //test
    //freeargs(args, count);

  }

 //write your code
 //use getinput and processline as appropriate

  return EXIT_SUCCESS;
}


/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.
* returns  The length of the string stored in *line.
*
* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* Hint: There is a standard i/o function that can make getinput easier than it sounds.
*/
ssize_t getinput(char** line, size_t* size) {

  ssize_t len = 1;
  
  //write your code
  if(*size == 0) { // if NULL allocate
    *line = malloc(5);
    *size = 5;
  } else { // Empty
    strcpy(*line, "");
  }
  printf("%%myshell%% ");
  char input;
  char* buff = malloc(200);
  char* point = buff;
  fgets(buff,200, stdin);
  while (*buff != '\0') {
    len++;
    if (len >= *size) {
      *size += 1;
      if(realloc(*line, *size) == NULL) {
        perror("realloc error");
        exit(-1);
      }
    }

    strncat(*line, buff, 1);
    buff++;
  }
  free(point);
  return len;
}


/* processline
 * The parameter line is interpreted as a command name.  This function creates a
 * new process that executes that command.
 * Note the three cases of the switch: fork failed, fork succeeded and this is
 * the child, fork succeeded and this is the parent (see fork(2)).
 * processline only forks when the line is not empty, and the line is not trying to run a built in command
 */
void processline (char *line)
{
 /*check whether line is empty*/
  //write your code
    
  pid_t cpid;
  int   status;
  int argCount;
  char** arguments = argparse(line, &argCount);
  
  /*check whether arguments are builtin commands
   *if not builtin, fork to execute the command.
   */
    //write your code
    if (!builtIn(arguments, argCount)) {
      if ((status = fork()) == 0) {
        printf("forking\n");
        if (execv(arguments[0], arguments)) {
          perror("could not execute\n");
        }
      }
      wait(NULL);
    }
}

// helper function to free args malloc
void freeargs(char** args, int count) {
  for (int i = 0; i < count; i++) {
    free(args[i]);
  }
  free(args);
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argparse.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FALSE (0)
#define TRUE  (1)

/*
* argCount is a helper function that takes in a String and returns the number of "words" in the string assuming that whitespace is the only possible delimeter.
*/
static int argCount(char*line)
{
  /*
  * while the line is not at null terminator
  * if the current value is a white space
  * reach the end of whitespace subsequence
  * else
  * increment count then reach the end of the subsequence of chars
  * return count
  */
  int count = 0;
  while(*line != '\0') {
    if (isspace(*line)) { 
      for(; isspace(*line);line++);
    } else {
      count++;
      for(; !isspace(*line) && *line != '\0';line++);
    }
  }
  return count;
}



/*
*
* Argparse takes in a String and returns an array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer
*/
char** argparse(char* line, int* argcp)
{
  // **FREE THIS LATER**
  *argcp = argCount(line);
  if (*argcp == 0) {
    return NULL;
  }
  int word_length;
  char** args = calloc(*argcp, sizeof(char*));
  for (; isspace(*line); line++);
  for (int i = 0; i <  *argcp; i++) {
    for (word_length = 0; !isspace(*line); line++, word_length++);
    line -= word_length;
    // **FREE** 
    args[i] = malloc(word_length+1);
    strncpy(args[i],line, word_length);
    line += word_length;
    for (; isspace(*line); line++);
  }
  return args;
}

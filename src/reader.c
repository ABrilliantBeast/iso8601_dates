#include "include/reader.h"

#include <malloc.h>

// TODO:  Any specific initialization for reader
void init_reader() {}

int read_time(FILE *in_file, iso8601_time_t **time) {
  char *line = NULL;
  size_t n = 0;
  int len = getline(&line, &n, in_file);
  // Value less than one means empty line or couldn't read
  // 1 means just a carriage return
  if (len <= 1) {
    *time = NULL;
    return -1;
  }
  // This line is too long to be an actual time, throw it away
  // freeing first
  if (len > MAX_DATE_SIZE) {
    free(line);
    *time = NULL;
    // TODO: Better errors
    return -1;
  }
  // Erase the \n
  line[len - 1] = '\0';
  *time = new_time(line);
  return 0;
}
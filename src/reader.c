#include "include/reader.h"

#include <malloc.h>

// TODO:  Any specific initialization for reader
void init_reader() {}

int read_date(FILE *in_file, iso8601_date_t **date) {
  char *line = NULL;
  size_t n = 0;
  int len = getline(&line, &n, in_file);
  // Value less than one means empty line or couldn't read
  // 1 means just a carriage return
  if (len <= 1) {
    *date = NULL;
    return -1;
  }
  // This line is too long to be an actual date, throw it away
  // freeing first
  if (len > MAX_DATE_SIZE) {
    free(line);
    *date = NULL;
    // TODO: Better errors
    return -1;
  }
  // Erase the \n
  line[len - 1] = '\0';
  *date = new_date(line);
  return 0;
}
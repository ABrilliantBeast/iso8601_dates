#include <malloc.h>


#include "include/reader.h"


// TODO:  Any specifc intitalization for reader
void init_reader(){

}


int read_date(FILE *in_file, Iso8601_date_t *date) {
  char *line = NULL;
  size_t n = 0;
  int len  = getline(&line, &n, in_file);
  // Negative value means it couldn't read,
  if (len < 0) {
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
  *date = line;
  return 0;
}
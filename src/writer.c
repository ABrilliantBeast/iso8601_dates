#include "include/writer.h"

#include "include/iso8601.h"

// TODO: Anything specific to writing data
void init_writer() {}

int write_time(FILE *out_file, iso8601_time_t *time) {
  int n = fprintf(out_file, "%s\n", time->str);
  return n;
}
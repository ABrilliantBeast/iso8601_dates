#include "include/writer.h"
#include "include/iso8601.h"



// TODO: Anything specific to writing data
void init_writer() {

}

int write_date(FILE *out_file, iso8601_date_t date) {
  int n = fprintf(out_file, "%s\n", (char *)date);
  return n;
}